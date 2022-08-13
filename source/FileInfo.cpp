#include "Backend/imp_FileInfo.hpp"
#include "FileInfo.h"


size_t FileInfo::getFileSize() const noexcept
{
    
    return cMyImpl().getFileSize();
}

std::string FileInfo::getFileName() const noexcept
{
    return cMyImpl().getFileName();
}

bool FileInfo::readFile(char *buffer, size_t bufferSize, fileReadCallback callback)
{
    return myImpl().readFile(buffer, bufferSize, callback);
}

FileInfo::FileInfo(const string &path) noexcept
{
    Pointerable<impl_fileinfo>::makeAlive(this);
    myImpl().init(path);
}

FileInfo::~FileInfo()=default;

