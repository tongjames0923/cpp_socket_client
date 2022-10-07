//
// Created by tbs on 2022/8/10.
//

#include "Application/Components/LocalTranslator.h"
#include "Application/config.h"
#include "Backend/imp_LocalTranslator.hpp"

using namespace std;


LocalTranslator::LocalTranslator(const char *filePath, const char *ip, unsigned int port)
{
    m_impl.reset(new imp_LocalTranslator());
    m_impl->client.init(ip,port);
    m_impl->fi.init(filePath);
}


size_t makeHead(const imp_FileInfo &info, char *buffer, size_t bufferLen = pack_Len)
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



size_t LocalTranslator::getTotalFileSize() const
{

    return m_impl->fi.getFileSize();
}



size_t LocalTranslator::getSent() const
{
    size_t r = m_impl->hassent;
    return r;
}

std::string LocalTranslator::getFileName() const
{
    return m_impl->fi.getFileName();
}


LocalTranslator::~LocalTranslator()
{

}

bool LocalTranslator::Connect()
{
    bool con = m_impl->client.connect();
    return con;
}

bool LocalTranslator::prepareData()
{
    imp_LocalTranslator &imp =*m_impl;
    if (imp.prepared_status==imp_LocalTranslator::preparing)
    {
        throw std::runtime_error("object is preparing...");
    }
    clear_data();
    imp.prepared_status=imp_LocalTranslator::preparing;
    bool isok = false;
    isok = imp.fi.readFile(imp.b_tmp, pack_Len, [&imp](const int &pks, const size_t &per, const size_t &total) -> bool
    {
#ifdef IMPL_ASIO
        imp._data_q.emplace_back(asio::buffer(imp.b_tmp, per));
#else
        data_ d( per);
        std::memcpy((char*)d,imp.b_tmp, per);
        imp._data_q.emplace_back(std::move(d));
#endif
        return true;
    });
    imp.prepared_status=imp_LocalTranslator::prepared;
    return isok;
}

size_t LocalTranslator::sendPreparedData()
{
    if (getPrepareStatus()!=prepared)
    {
        throw std::runtime_error("file was not prepared for send");
    }

    imp_LocalTranslator &ins =*m_impl;
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

        size_t sent = ins.client.send(buf, len);
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
    asio::mutable_buffer bf=asio::buffer(ins.b_tmp,headlen);
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
        sucess=sendAction(data.getData(),data.getSize());
#endif
        if (!sucess)
            break;
    }
    return ins.hassent;
}

LocalTranslator::LocalTranslator(LocalTranslator &&other) noexcept
{
    m_impl=std::move(other.m_impl);
}

std::string LocalTranslator::getIp() const noexcept
{
    return m_impl->client.getIP();
}

int LocalTranslator::getPort() const noexcept
{
    return m_impl->client.getPort();
}

prepare_status LocalTranslator::getPrepareStatus() const noexcept {
    int v=m_impl->prepared_status;
    return (prepare_status)(v);
}

void LocalTranslator::clear_data() noexcept
{
    if (getPrepareStatus()==prepared)
    {
        m_impl->prepared_status=imp_LocalTranslator::not_prepared;
        m_impl->hassent=0;
        m_impl->_data_q.clear();
    }
}

void LocalTranslatorDeleter::operator()(imp_LocalTranslator *p) const
{
    delete p;
}
