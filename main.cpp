// www.cnblogs.com/flyinghearts

#include "FileInfo.h"
#include "FileSender.h"
#include "progress.h"
#include <cstdio>
#include <cstring>
#include <iostream>

#define Debug

using namespace std;

const size_t bufferLen = 64 * KB;
const size_t progressLen = 25;
char buffer[bufferLen];
FileSender *sender;
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

// void sender(asio::io_service& io, const char* ip_address, unsigned port,
// const char* filename)
// {
//   typedef asio::ip::tcp TCP;
//   //open file and read
//   FileInfo info(filename);
//   FILE* fp =info.getFileInstance();

// //make buffer for pack
//   const size_t k_buffer_size = 64 * KB;
//   char buffer[k_buffer_size];

// //make first pack
//   File_info file_info=info.getinfo();

//   int filename_size  = strlen(filename) + 1;
//   size_t file_info_size = sizeof(file_info);
//   size_t total_size = file_info_size + filename_size;
//   if (total_size > k_buffer_size) {
//     throw std::string("File name is too long");
//     return;
//   }
//   memcpy(buffer, &file_info, file_info_size);
//   memcpy(buffer + file_info_size, filename, filename_size);
// //start to send

//   clock_t cost_time = clock();
//   TCP::socket socket(io);
//   socket.connect(TCP::endpoint(asio::ip::address_v4::from_string(ip_address),
//   port));

//   std::cout << "Sending file : " << filename << "\n";
//   size_t len = total_size;
//   unsigned long long total_bytes_read = 0;
//   while (true) {
//     socket.send(asio::buffer(buffer, len), 0);
//     if (feof(fp)) break;
//     len = fread(buffer, 1, k_buffer_size, fp);
//     total_bytes_read += len;
//     float p=(float)total_bytes_read/(float)file_info.filesize*25;
//     consoleProgress::pushProgress(p);
//   }

//   cost_time = clock() - cost_time;
//   if (cost_time == 0) cost_time = 1;
//   double speed = total_bytes_read * (CLOCKS_PER_SEC / 1024.0 / 1024.0) /
//   cost_time; std::cout << "cost time: " << cost_time / (double)
//   CLOCKS_PER_SEC  << " s "
//     << "  transferred_bytes: " << total_bytes_read << " bytes\n"
//     << "speed: " <<  speed << " MB/s\n\n";
// }
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
  bool isConnected= sender->connect();
  if(!isConnected)
  {
    cout<<"fail to connect..."<<endl;
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
  size_t rep = sender->recive(repbuffer, 128);
  int repCode = *(int *)repbuffer;
  if (callback == firstpack && repCode == 99)
  {
    isok = file.readFile(buffer, bufferLen,
                         ([](int pack, size_t per, size_t total) -> bool
                          {
                      sender->send(buffer,per);
                      float pec=(float)total/fileTotal*progressLen;
                      consoleProgress::pushProgress(pec); 
                      return true;
                      }));
  }
  else
  {
    cout << "sent fail..." << endl;
  }
  consoleProgress::pushProgress(progressLen);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    consoleProgress::pushProgress(progressLen);
  }
}


int mx(int z)
{
  cout<<"hello world"<<z<<endl;
  return z+1;
}

int main(int args, char *argc[])
{
  try
  {
#ifndef Debug
    if (args == 3)
    {
    filePath = string(argc[2]);
    FileSender sr(argc[1], 1997);
        sender = &sr;
    }
    else if(args==4)
    {
      filePath = string(argc[3]);
    FileSender sr(argc[1],atoi(argc[2]));
        sender = &sr;
    }
    else{
            cout << "usage socket_client <target_ip> <file_Location> or \n"<<
            "usage socket_client <target_ip> <port> <file_Location> or \n"
            
             << endl;
      return 0;
    }

#else

    FileSender sr("127.0.0.1", 1997);
     sender = &sr;
#endif // !Debug

    cout << "init..." << endl;
    consoleProgress::setLen(progressLen);
    clock_t cost_time = clock();
    consoleProgress::progress(sendAction);
    cost_time = clock() - cost_time;
    std::cout << "cost time: " << cost_time / (double)CLOCKS_PER_SEC << " s ";
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