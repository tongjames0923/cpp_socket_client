#include "Backend/imp_FileInfo.hpp"
#include "FileInfo.h"


size_t FileInfo::getFileSize() const noexcept
{

    return m_impl->getFileSize();
}

std::string FileInfo::getFileName() const noexcept
{
    return m_impl->getFileName();
}

bool FileInfo::readFile(char *buffer, size_t bufferSize, fileReadCallback callback)
{
    return m_impl->readFile(buffer, bufferSize, callback);
}

FileInfo::FileInfo(const string &path) noexcept
{
    PIMPL_INIT(FileInfo);
    m_impl->init(path);
}

FileInfo::~FileInfo() = default;

void FileInfoDeleter::operator()(imp_FileInfo *p) const
{
    delete p;
}