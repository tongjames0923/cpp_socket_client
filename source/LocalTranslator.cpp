//
// Created by tbs on 2022/8/10.
//

#include "Application/Components/LocalTranslator.h"
#include "Application/config.h"
#include "Backend/imp_LocalTranslator.hpp"
#include <queue>
#include <mutex>

using namespace std;


LocalTranslator::LocalTranslator(const char *filePath, const char *ip, unsigned int port)
{
    impl_LocalTranslator &instance = myImpl();
    instance.client.init(ip, port);
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

size_t LocalTranslator::runIt(callback startread, callback startsent, callback finish)
{
    if (startread != nullptr)
        startread(this);
    prepareData();
    if(startsent!= nullptr)
        startsent(this);
    size_t r= sendPreparedData();
    if(finish!= nullptr)
        finish(this);
    return r;
}


size_t LocalTranslator::getTotalFileSize() const
{

    return cMyImpl().fi.getFileSize();
}



size_t LocalTranslator::getSent() const
{
    size_t r = cMyImpl().hassent;
    return r;
}

std::string LocalTranslator::getFileName() const
{
    return cMyImpl().fi.getFileName();
}


LocalTranslator::~LocalTranslator()
{

}

bool LocalTranslator::Connect()
{
    bool con = myImpl().client.connect();
    return con;
}

bool LocalTranslator::prepareData()
{
    impl_LocalTranslator &imp = myImpl();
    if (imp.prepared)
    {
        imp._data_q.clear();
        imp.prepared = false;
    }
    bool isok = false;
    isok = imp.fi.readFile(imp.b_tmp, pack_Len, [&imp](const int &pks, const size_t &per, const size_t &total) -> bool
    {
#ifdef IMPL_ASIO
        imp._data_q.emplace_back(asio::buffer(b_tmp, per));
#else
        char *d = new char[per];
        std::memcpy(d,imp.b_tmp, per);
        imp._data_q.emplace_back(std::move(data_(d, per)));
#endif
        return true;
    });
    imp.prepared = isok;
    return isok;
}

size_t LocalTranslator::sendPreparedData()
{
    if (!hasPrepared())
    {
        throw std::runtime_error("file was not prepared for send");
    }

    impl_LocalTranslator &ins = myImpl();
    size_t headlen = makeHead(ins.fi,ins. b_tmp);
    ins.hassent = 0;
#ifdef IMPL_ASIO
    auto sendAction = [&ins](asio::mutable_buffer &buf) -> bool
    {
        size_t sent = ins.client.send(buf);
        if (sent != buf.size())
        {
            return false;

        } else
        {
            ins.hassent += sent;
            return true;
        }
    };
#else
    auto sendAction = [&ins](char *buf, size_t len) -> bool
    {

        size_t sent = ins.client.send(ins.b_tmp, len);
        if (sent != len)
        {
            return false;
        } else
        {
            ins.hassent += sent;
            return true;
        }
    };
#endif
#ifdef IMPL_ASIO
    asio::mutable_buffer bf=asio::buffer(b_tmp,headlen);
    sendAction(bf);
#else
    sendAction(ins.b_tmp,headlen);
#endif
    bool sucess = false;
    for (data_ &data :ins._data_q)
    {
#ifdef IMPL_ASIO
        sucess = sendAction(data);
#else
        sucess=sendAction(data.data.get(),data.len);
#endif
        if (!sucess)
            break;
    }
    return ins.hassent;
}

bool LocalTranslator::hasPrepared() const noexcept
{
    return cMyImpl().prepared;
}

LocalTranslator::LocalTranslator(LocalTranslator &&other) noexcept
{
    LocalTranslator::move(this,std::move(other));
}

std::string LocalTranslator::getIp() const noexcept
{
    return cMyImpl().client.getIP();
}

int LocalTranslator::getPort() const noexcept
{
    return cMyImpl().client.getPort();
}


void imp_TranslatorDeleter::operator()(impl_LocalTranslator *p) const
{
    delete p;
}
