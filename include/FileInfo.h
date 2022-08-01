#ifndef _FILE_INFO_H_
#define _FILE_INFO_H_

#include <string>
#include <fstream>
#include <functional>

const long KB=1024,MB=KB*1024;


struct File_info
{
    size_t filesize;
    size_t filename_size;
    File_info() : filesize(0), filename_size(0) {}
};

/**
 * @brief 文件读取回调
 * @param  int =当前包序号
 * @param  size_t = 本次读取字节数 
 * @param size_t =总共读取字节数
 * @return bool 读取是否继续
 */
typedef std::function<bool(int,size_t,size_t)> fileReadCallback;

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