//
// Created by tbs on 2022/8/5.
//

#include "LocalTranslator.h"
#include <iostream>

const size_t pack_Len = 32 * KB;
size_t hassent=0;

LocalTranslator::LocalTranslator(const char *filePath, const char *ip, unsigned int port,
                                 size_t packLimit) :
        fi(filePath), client(ip, port)
{
    setLen(packLimit);
    setHandler([this](pack *pk, unsigned md) -> bool
               {
                   size_t sent = client.send(pk->pullData(), pk->getObjectLen());
                   if (sent != pk->getObjectLen())
                   {
                       if(onSentFail!= nullptr)
                       {
                           if(!onSentFail(pk,pk->getObjectLen(),sent))
                           {
                               done();
                               return false;
                           }
                           return true;
                       }
                       else
                       {
                           done();
                           return false;
                       }

                   } else
                   {
                       if(onSentSuccess!= nullptr)
                           onSentSuccess(pk,pk->getObjectLen(),sent);
                       hassent+=sent;
                       return true;
                   }
               });
}


bool LocalTranslator::OnReady()
{
    bool connect = client.connect();
    return connect;
}

bool LocalTranslator::OnDone()
{
    return true;
}

size_t makeHead(const FileInfo &info, char *buffer, size_t bufferLen = pack_Len)
{
    size_t filename_size = info.getFileName().length() + 1;
    size_t file_info_size = sizeof(File_info);
    size_t total_size = file_info_size + filename_size;
    if (total_size > bufferLen)
    {
        throw runtime_error("File name is too long");
    }
    File_info ff;
    ff.filesize = info.getFileSize();
    ff.filename_size = filename_size;

    memcpy(buffer, &ff, file_info_size);
    memcpy(buffer + file_info_size, info.getFileName().c_str(),
           filename_size);
    return total_size;
}

using pack = PackData<char>;

void LocalTranslator::runIt()
{
    if (!is_ready())
    {
        ready();
    }

    char b_tmp[pack_Len];
    pack head(pack_Len);
    size_t ls= makeHead(this->fi, b_tmp);
    head.setData(b_tmp,ls);
    this->pushIn(std::move(head), true);
    fi.readFile(b_tmp, pack_Len, [&b_tmp, this](int pks, size_t per, size_t total) -> bool
    {
        pack pk(pack_Len);
        pk.setData(b_tmp, per);
        this->pushIn(std::move(pk), true);
        if(onFileRead!= nullptr)
        {
            onFileRead(pks,per);
        }
        return true;
    });
    handle(nullptr);
}

bool LocalTranslator::readyForConnect()
{
    ready();
    return is_ready();
}

size_t LocalTranslator::getTotalFileSize() const
{

    return fi.getFileSize();
}

std::string LocalTranslator::getFileName()const
{
    return fi.getFileName();
}

void LocalTranslator::setOnSentFail(callback_data_sent cb)
{
    this->onSentFail=cb;
}

void LocalTranslator::setOnSentSuccess(callback_data_sent cb)
{
    onSentSuccess=cb;
}

void LocalTranslator::setOnFileRead(callback_file_read cb)
{
    onFileRead=cb;
}

