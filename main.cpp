//www.cnblogs.com/flyinghearts

#include <iostream>
#include <cstdio>
#include <cstring>
#include "FileInfo.h"
#include "progress.h"



using namespace std;

// void sender(asio::io_service& io, const char* ip_address, unsigned port, const char* filename)
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
//   socket.connect(TCP::endpoint(asio::ip::address_v4::from_string(ip_address), port));

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
//   double speed = total_bytes_read * (CLOCKS_PER_SEC / 1024.0 / 1024.0) / cost_time;
//   std::cout << "cost time: " << cost_time / (double) CLOCKS_PER_SEC  << " s " 
//     << "  transferred_bytes: " << total_bytes_read << " bytes\n"
//     << "speed: " <<  speed << " MB/s\n\n"; 
// }

// void sendAction()
// {
//   asio::io_service io;
//   sender(io, "192.168.50.55", 1997, "/Users/abstergo/Desktop/test.data");
// }

ofstream out("outfile.data");
char b[64 * 1024];
int main(int args, char* argc[])
{
//   if (args < 3) {
//     std::cerr << "Usage: " << argc[0] << " ip_address  filename1 filename2 \n"; 
//     return 1;
//   }
    // try {
      
    //   consoleProgress::setLen(25);
    //   consoleProgress::progress(sendAction); }
    // catch (std::exception& err) {
    //   std::cerr << err.what() << "\n";
    // }
    FileInfo file("/Users/abstergo/Desktop/test.data");

   cout<< file.readFile(b,64*1024,([](int pack,size_t per,size_t total){
      printf("read at %d packs,read %ld,total read %ld\n",pack,per,total);
      out.write(b,per);
      printf("%d pack has writed\n",pack);
    }))<<endl;
}