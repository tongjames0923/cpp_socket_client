#include "FileInfo.h"
#include <cstdio>

using namespace std;

size_t FileInfo::fileSize()
{
    streampos cur = file.tellg();
    file.seekg(0, ios_base::end);
    streampos e = file.tellg();
    size_t r = e;
    file.seekg(cur, ios_base::beg);
    return e;
}


size_t FileInfo::getFileSize() const
{
    return info.filesize;
}

std::string FileInfo::getFileName() const
{
    return filePath;
}

bool FileInfo::readFile(char *buffer, size_t bufferSize, fileReadCallback callback)
{
    streampos cur = file.tellg();
    file.seekg(0, ios_base::beg);
    size_t hasread = 0, singleRead = 0, pack = 0;
    bool iscon = true;
    while (!file.eof() && iscon)
    {
        file.read(buffer, bufferSize);
        singleRead = file.gcount();
        hasread += singleRead;
        iscon = callback(++pack, singleRead, hasread);
    }
    bool suc = hasread == getFileSize();
    file.seekg(cur, ios_base::beg);
    return suc&&iscon;
}

FileInfo::FileInfo(const string &path)
{
    file.open(path, ios_base::binary);
    if (!file.is_open())
    {
        throw runtime_error("wrong file Path") ;
    }
    this->filePath = path;
    this->info.filename_size = path.length() + 1;
    this->info.filesize = fileSize();
}
