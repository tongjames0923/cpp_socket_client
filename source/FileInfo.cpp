#include "FileInfo.h"
#include <cstdio>
FileInfo::FileInfo()
{

}
FileInfo::~FileInfo()
{
    closeFile();
}
FileInfo::FileInfo(const std::string path)
{
    readFile(path);

}
void FileInfo::readFile(const std::string filename)
{
    closeFile();

    this->filePath = filename;
    fp = fopen(filename.c_str(), "rb");
    if (fp == NULL)
    {
        throw std::string("file open fail");
    }
    this->info.filename_size=filename.length()+1;
    fseek(fp, 0, SEEK_END);
    this->info.filesize = ftell(fp);
    rewind(fp);
}
File_info FileInfo::getinfo()
{
    return this->info;
}
std::string FileInfo::getFile()
{
    return filePath;
}
FILE * FileInfo::getFileInstance()
{
    return fp;
}
void FileInfo::closeFile()
{
    if(fp!=nullptr)
    {
        fclose(fp);
        fp=nullptr;
    }

}