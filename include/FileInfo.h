#ifndef _FILE_INFO_H_
#define _FILE_INFO_H_
#include <boost/asio.hpp>
#include <string>
#include <fstream>
namespace asio
{
    using namespace boost::asio;
    using boost::system::error_code;
}
const long KB=1024,MB=KB*1024;
#if BOOST_VERSION >= 107000
#define GET_IO_SERVICE(s) ((boost::asio::io_context &)(s).get_executor().context())
#else
#define GET_IO_SERVICE(s) ((s).get_io_service())
#endif

struct File_info
{
    size_t filesize;
    size_t filename_size;
    File_info() : filesize(0), filename_size(0) {}
};

using fileReadCallback=void(*)(int packNum,size_t bufferd,size_t totalRead);

class FileInfo
{
    private:
    std::ifstream file;
    File_info info;
    std::string filePath;
    size_t fileSize();
    public:
    FileInfo(const std::string);
    size_t getFileSize() const;
    std::string getFileName() const;
    bool readFile(char* buffer,size_t bufferSize,fileReadCallback callback);
};

#endif