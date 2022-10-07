//
// Created by tbs on 2022/8/10.
//

#ifndef SOCKET_CLIENT_IMP_FILEINFO_HPP
#define SOCKET_CLIENT_IMP_FILEINFO_HPP

#include <string>
#include <functional>
#include <fstream>
#include <Error.hpp>
#include "Pointerable.hpp"

using namespace std;

PIMPL_IMPL(FileInfo)
public:
    std::ifstream file;
    size_t filesize;
    std::string filePath;
    using imp_read_file_callback = std::function<bool(const int &packindex, const size_t &perRead,
                                                      const size_t &totalread)>;

    size_t calFileSize()
    {
        std::streampos cur = file.tellg();
        file.seekg(0, std::ios_base::end);
        std::streampos e = file.tellg();
        size_t r = e;
        file.seekg(cur, std::ios_base::beg);
        return e;
    }

    void init(const std::string &path)
    {
        file.open(path, ios_base::binary);
        _if_error(!file.is_open(),"wrong file Path at "+path)
        this->filePath = path;
        this->filesize = calFileSize();
    }

    size_t getFileSize() const noexcept
    {

        return filesize;
    }

    std::string getFileName() const noexcept
    {
        return filePath;
    }

    bool readFile(char *buffer, size_t bufferSize, imp_read_file_callback callback)
    {
        streampos cur = file.tellg();
        file.seekg(0, ios_base::beg);
        size_t hasread = 0, singleRead = 0;
        int pack = 0;
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
        return suc && iscon;
    }
PIMPL_IMPL_END

#endif //SOCKET_CLIENT_IMP_FILEINFO_HPP
