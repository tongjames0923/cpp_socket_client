// www.cnblogs.com/flyinghearts

#include "FileInfo.h"
#include "FileSender.h"
#include "progress.h"
#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

const size_t bufferLen = 64 * KB;
const size_t progressLen=25;
char buffer[bufferLen];
FileSender sender ("127.0.0.1", 1997);
size_t
makeHead (const FileInfo &info)
{
  size_t filename_size = info.getFileName ().length () + 1;
  size_t file_info_size = sizeof (File_info);
  size_t total_size = file_info_size + filename_size;
  if (total_size > bufferLen)
    {
      throw std::string ("File name is too long");
    }
  memcpy (buffer, &info, file_info_size);
  memcpy (buffer + file_info_size, info.getFileName ().c_str (),
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
string filePath = "C:\\Users\\Abstergo\\Desktop\\报告.docx";
size_t fileTotal;
bool isok=false;
void
sendAction ()
{
  FileInfo file(filePath);
  fileTotal=file.getFileSize();
  sender.connect ();
  isok = false;
  size_t callback = 0;
  size_t firstpack = makeHead (file);
  callback = sender.send (buffer, firstpack);
  if (callback == firstpack)
    {
     isok= file.readFile (buffer, bufferLen,
                     ([] (int pack, size_t per, size_t total) {
                      sender.send(buffer,per);
                      float pec=(float)total/fileTotal*progressLen;
                      consoleProgress::pushProgress(pec);
                     }));
    }
  else
    {
      cout << "头包发送失败..." << endl;
    }
}

int
main (int args, char *argc[])
{
  //   if (args < 3) {
  //     std::cerr << "Usage: " << argc[0] << " ip_address  filename1 filename2
  //     \n"; return 1;
  //   }
  try {

    consoleProgress::setLen(progressLen);
    clock_t cost_time = clock ();
    consoleProgress::progress(sendAction);
    cost_time = clock () - cost_time;
    std::cout << "cost time: " << cost_time / (double)
     CLOCKS_PER_SEC  << " s ";
     if(isok)
     {
       double speed =fileTotal * (CLOCKS_PER_SEC / 1024.0 / 1024.0) /
         cost_time;
       cout <<"Speed"<< speed << " MB/s"<<endl;
     }
    }

  catch (std::exception& err) {
    std::cerr << err.what() << "\n";
  }
}