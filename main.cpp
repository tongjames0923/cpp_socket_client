#include <cstdio>
#include <cstring>
#include <iostream>
#include "LocalTranslator.h"


using namespace std;

static const size_t progressLen = 25;
static const size_t Q_LEN = 32;

string filePath = "/Users/abstergo/Desktop/imguisample.zip";
size_t fileTotal;
unsigned port = 1997;
string ip = "127.0.0.1";


int main(int args, char *argc[])
{

    try
    {
        printf("version:%s\n", VERSION);
        cout << "init..." << endl;
        clock_t cost_time = clock();
#ifdef RELEASE_MODE
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
//        SocketClient sr("127.0.0.1", 1997);
//        sender = &sr;
#endif // !Debug
        LocalTranslator translator(filePath.c_str(), ip.c_str(), port, Q_LEN);
        fileTotal = translator.getTotalFileSize();
        size_t hasSent = 0;
        translator.setOnSentFail([](PackData<char> *pk, size_t should, size_t sent) -> bool
                                 {
                                     cout << "send function error" << endl;

                                     //consoleProgress::abort();
                                     return false;
                                 });
        translator.setOnSentSuccess([&hasSent](PackData<char> *pk, size_t should, size_t sent) -> bool
                                    {
                                        hasSent += sent;
                                        float pec = (float) hasSent / fileTotal * 100;
                                        printf("\r%.2f", pec);
                                        cout << "%";
                                        //consoleProgress::pushProgress(pec);
                                        return true;
                                    });
        cout << "connect..." << endl;
        bool connected = translator.readyForConnect();
        if (connected)
        {
            cout << "file:" << filePath << "file_size:" << fileTotal / 1024.0 << "kb,target IP:" << ip << ":"
                 << port << endl;
            //printf("file:%s,file_size:%zu,target_ip:%s,target_port:%d",filePath.c_str(),translator.getTotalFileSize(),ip.c_str(),port);
            translator.runIt();
            //consoleProgress::finish();
        } else
        {
            //consoleProgress::abort();
            cout << "fail to connect" << endl;
            return 0;
        }
        cost_time = clock() - cost_time;
        std::cout << "\ncost time: " << cost_time / (double) CLOCKS_PER_SEC << " s ";
        double speed = fileTotal * (CLOCKS_PER_SEC / 1024.0 / 1024.0) /
                       cost_time;
        cout << "Speed:" << speed << " MB/s" << endl;
        return 1;

    }
    catch (std::exception &err)
    {
        std::cerr << err.what() << "\n";
        return 0;
    }
}