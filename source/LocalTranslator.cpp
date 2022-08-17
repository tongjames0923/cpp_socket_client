//
// Created by tbs on 2022/8/10.
//

#include "Application/Components/LocalTranslator.h"
#include "Application/config.h"
#include "Backend/imp_LocalTranslator.hpp"
#include "FileInfo.h"
#include <queue>

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


static char b_tmp[pack_Len];

size_t LocalTranslator::runIt(callback startread, callback startsent, callback finish)
{
    impl_LocalTranslator &ins = myImpl();

#ifdef IMPL_ASIO
    auto sendAction = [&ins](asio::mutable_buffer& buf)->bool {
        size_t sent = ins.client.send(buf);
        if (sent != buf.size())
        {
            return false;

        }
        else
        {
            ins.hassent += sent;
            return true;
        }
    };
    using data_ =asio::mutable_buffer;
#else
    auto sendAction = [&ins](char *buf, size_t len) -> bool
    {

        size_t sent = ins.client.send(b_tmp, len);
        if (sent != len)
        {
            return false;
        } else
        {
            ins.hassent += sent;
            return true;
        }
    };
    struct data_
    {
        std::unique_ptr<char[]> data;
        size_t len;

        data_(char *d, size_t l)
        {
            data.reset(d);
            len = l;
        }
    };
#endif

    vector<data_> _data_q;
    bool isok;
    impl_LocalTranslator &imp = myImpl();
    if (startread != nullptr)
    {
        startread(this);
    }
    isok = imp.fi.readFile(b_tmp, pack_Len, [&_data_q](const int &pks, const size_t &per, const size_t &total) -> bool
    {
#ifdef IMPL_ASIO
        _data_q.emplace_back(data_(b_tmp, per));
#else
        char *d = new char[per];
        std::memcpy(d, b_tmp, per);
        _data_q.emplace_back(std::move(data_(d, per)));
#endif
        return true;
    });
    if (isok)
    {
        if (startsent != nullptr)
        {
            startsent(this);
        }
        size_t ls = makeHead(cMyImpl().fi, b_tmp);
#ifdef IMPL_ASIO
        auto bf=asio::buffer(b_tmp,ls);
        sendAction(bf);
#else
        sendAction(b_tmp, ls);
#endif
        for (data_ &d : _data_q)
        {
#ifdef IMPL_ASIO
            sendAction(d);
#else
            sendAction(d.data.get(), d.len);
#endif

        }
    } else
    {
        return -1;
    }
    if (finish != nullptr)
    {
        finish(this);
    }
    size_t r = cMyImpl().hassent;

    myImpl().hassent = 0;
    return r;
}


size_t LocalTranslator::getTotalFileSize() const
{

    return cMyImpl().fi.getFileSize();
}

mutex locker;

size_t LocalTranslator::getSent() const
{
    lock_guard<mutex> lg(locker);
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


void imp_TranslatorDeleter::operator()(impl_LocalTranslator *p) const
{
    delete p;
}
