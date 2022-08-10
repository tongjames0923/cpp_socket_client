#include "FileInfo.h"
#include "Backend/imp_FileInfo.h"

size_t FileInfo::getFileSize() const
{
    
    return cMyImpl().getFileSize();
}

std::string FileInfo::getFileName() const
{
    return cMyImpl().getFileName();
}

bool FileInfo::readFile(char *buffer, size_t bufferSize, fileReadCallback callback)
{
    return myImpl().readFile(buffer, bufferSize, callback);
}

FileInfo::FileInfo(const string &path)
{
    myImpl().init(path);
}

