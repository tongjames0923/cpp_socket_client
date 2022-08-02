#include "FileInfo.h"
#include "SocketClient.h"
#include "progress.h"
#include <cstdio>
#include <cstring>
#include <iostream>

//#define Debug

using namespace std;

const size_t bufferLen = 64 * KB;
const size_t progressLen = 25;
char buffer[bufferLen];
SocketClient *sender;

size_t makeHead(const FileInfo &info)
{
    size_t filename_size = info.getFileName().length() + 1;
    size_t file_info_size = sizeof(File_info);
    size_t total_size = file_info_size + filename_size;
    if (total_size > bufferLen)
    {
        throw std::string("File name is too long");
    }
    File_info ff;
    ff.filesize = info.getFileSize();
    ff.filename_size = filename_size;

    memcpy(buffer, &ff, file_info_size);
    memcpy(buffer + file_info_size, info.getFileName().c_str(),
           filename_size);
    return total_size;
}

string filePath = "/Users/abstergo/Desktop/imguisample.zip";
size_t fileTotal;
bool isok = false;

void sendAction()
{
    try
    {
        cout << "reading..." << endl;
        FileInfo file(filePath);
        fileTotal = file.getFileSize();
        cout << "connect..." << endl;
        bool isConnected = sender->connect();
        if (!isConnected)
        {
            cout << "fail to connect..." << endl;
            consoleProgress::pushProgress(progressLen);
            return;
        }
        cout << "connected..." << endl;
        isok = false;
        size_t callback = 0;
        size_t firstpack = makeHead(file);
        cout << "head sending..." << endl;
        callback = sender->send(buffer, firstpack);
        cout << "head sent..." << endl;
        char repbuffer[128];
        size_t rep = sender->receive(repbuffer, 128);
        int repCode = *(int *) repbuffer;
        if (callback == firstpack && repCode == 99)
        {
            cout << "file send..." << endl;
            isok = file.readFile(buffer, bufferLen,
                                 ([](int pack, size_t per, size_t total) -> bool
                                 {
                                     size_t sentSize = sender->send(buffer, per);
                                     if (sentSize != per)
                                     {
                                         return false;
                                     }
                                     float pec = (float) total / fileTotal * progressLen;
                                     consoleProgress::pushProgress(pec);
                                     return true;
                                 }));
            if (isok)
            {
                consoleProgress::finish();
            } else
            {
                cout << "pack sent fail..." << endl;
                consoleProgress::abort();
            }
        } else
        {
            cout << "head sent fail..." << endl;
            consoleProgress::abort();
        }

    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << '\n';
        consoleProgress::abort();
    }
}

int main(int args, char *argc[])
{

    try
    {
#ifndef Debug
        if (args == 3)
        {
            filePath = string(argc[2]);
            SocketClient sr(argc[1], 1997);
            sender = &sr;
        } else if (args == 4)
        {
            filePath = string(argc[3]);
            SocketClient sr(argc[1], atoi(argc[2]));
            sender = &sr;
        } else
        {
            cout << "usage socket_client <target_ip> <file_Location> or \n" <<
                 "usage socket_client <target_ip> <port> <file_Location> or \n"

                 << endl;
            return 0;
        }

#else
        SocketClient sr("127.0.0.1", 1997);
        sender = &sr;
#endif // !Debug

        cout << "init..." << endl;
        consoleProgress::setLen(progressLen);
        clock_t cost_time = clock();
        consoleProgress::progress(sendAction);
        cost_time = clock() - cost_time;
        std::cout << "cost time: " << cost_time / (double) CLOCKS_PER_SEC << " s ";
        if (isok)
        {
            double speed = fileTotal * (CLOCKS_PER_SEC / 1024.0 / 1024.0) /
                           cost_time;
            cout << "Speed:" << speed << " MB/s" << endl;
        }
    }

    catch (std::exception &err)
    {
        std::cerr << err.what() << "\n";
    }
}