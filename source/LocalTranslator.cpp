//
// Created by tbs on 2022/8/10.
//

#include "Application/Components/LocalTranslator.h"
#include "Application/config.h"
#include "Backend/imp_LocalTranslator.hpp"
#include "FileInfo.h"

using namespace std;




LocalTranslator::LocalTranslator(const char *filePath, const char *ip, unsigned int port)
{
    impl_LocalTranslator& instance=myImpl();
    instance.client.init(ip,port);
    instance.fi.init(filePath);
}


size_t makeHead(const impl_fileinfo &info, char *buffer, size_t bufferLen = pack_Len)
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



static char b_tmp[pack_Len];
size_t LocalTranslator::runIt()
{
    impl_LocalTranslator& ins = myImpl();
    auto sendAction = [&ins](char* buf, size_t len)->bool {

        size_t sent = ins.client.send(b_tmp, len);
        if (sent != len)
        {
            if (ins.onSentFail != nullptr)
            {
                if (!ins.onSentFail(buf, len, sent))
                {
                    return false;
                }
                return true;
            }
            else
            {
                return false;
            }

        }
        else
        {
            if (ins.onSentSuccess != nullptr)
                ins.onSentSuccess(b_tmp, len, sent);
            ins.hassent += sent;
            return true;
        }

    };
    size_t ls = makeHead(cMyImpl().fi, b_tmp);
    sendAction(b_tmp, ls);
    myImpl().fi.readFile(b_tmp, pack_Len, [this,&sendAction](int pks, size_t per, size_t total) -> bool
                {
            if (myImpl().onFileRead != nullptr)
            {
                myImpl().onFileRead(pks, per);
            }
            if (sendAction(b_tmp, per))
            {
                return true;
           }
            else
            {
                return false;
            } });
    size_t r = cMyImpl().hassent;
    myImpl().hassent = 0;
    return r;
}


size_t LocalTranslator::getTotalFileSize() const
{

    return cMyImpl().fi.getFileSize();
}

size_t LocalTranslator::getSent() const
{
    return cMyImpl().hassent;
}

std::string LocalTranslator::getFileName() const
{
    return cMyImpl().fi.getFileName();
}

void LocalTranslator::setOnSentFail(callback_data_sent cb)
{
    myImpl().onSentFail = cb;
}

void LocalTranslator::setOnSentSuccess(callback_data_sent cb)
{
    myImpl().onSentSuccess = cb;
}

void LocalTranslator::setOnFileRead(callback_file_read cb)
{
    myImpl().onFileRead = cb;
}

LocalTranslator::~LocalTranslator()
{

}

bool LocalTranslator::readyForConnect()
{
    bool con = myImpl().client.connect();
    return con;
}


void imp_TranslatorDeleter::operator()(impl_LocalTranslator* p) const
{
    delete p;
}
