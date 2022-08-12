//
// Created by tbs on 2022/8/10.
//

#ifndef SOCKET_CLIENT_IMP_FILEINFO_HPP
#define SOCKET_CLIENT_IMP_FILEINFO_HPP
#include <string>
#include <fstream>
using namespace std;

class impl_fileinfo
{
public:
    std::ifstream file;
    size_t filesize;
    std::string filePath;
    using imp_read_file_callback=std::function<bool(int packindex, size_t perRead, size_t totalread)>;
    size_t calFileSize()
    {
        std::streampos cur = file.tellg();
        file.seekg(0, std::ios_base::end);
        std::streampos e = file.tellg();
        size_t r = e;
        file.seekg(cur, std::ios_base::beg);
        return e;
    }

    void init(const std::string& path)
    {
        file.open(path, ios_base::binary);
        if (!file.is_open())
        {
            throw runtime_error("wrong file Path") ;
        }
        this->filePath = path;
        this->filesize = calFileSize();
    }
    size_t getFileSize() const
    {

        return filesize;
    }

    std::string getFileName() const
    {
        return filePath;
    }
    bool readFile(char *buffer, size_t bufferSize, imp_read_file_callback callback)
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

};

#endif //SOCKET_CLIENT_IMP_FILEINFO_HPP
