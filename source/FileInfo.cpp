#include "FileInfo.h"
#include "Backend/imp_FileInfo.h"

size_t FileInfo::getFileSize() const
{
    
    return m_impl->getFileSize();
}

std::string FileInfo::getFileName() const
{
    return m_impl->getFileName();
}

bool FileInfo::readFile(char *buffer, size_t bufferSize, fileReadCallback callback)
{
    return m_impl->readFile(buffer, bufferSize, callback);
}

FileInfo::FileInfo(const string &path):m_impl(new impl_fileinfo())
{
    m_impl->init(path);
}

