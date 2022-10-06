//
// Created by tbs on 2022/8/12.
//

#ifndef SOCKET_CLIENT_IMP_LOCALTRANSLATOR_HPP
#define SOCKET_CLIENT_IMP_LOCALTRANSLATOR_HPP

#include <functional>
#include <vector>
#include <FileInfo.h>
#include "imp_FileInfo.hpp"
#include "imp_SocketClient.hpp"

#ifdef IMPL_ASIO

using data_ = asio::mutable_buffer;
#else

class data_ {
private:
    std::unique_ptr<char[]> data = nullptr;
    size_t len = 0;
public:
    explicit data_(size_t l) {
        setSize(l);
    }
    explicit operator char*() noexcept
    {
        return getData();
    }
    void setSize(size_t t)
    {
        len=t;
        if(t<=0)
            data.reset();
        else
        data.reset(new char[len]);
    }
    size_t getSize()const noexcept
    {
        return len;
    }
    char* getData()noexcept
    {
        return data.get();
    }

    size_t getData(char* des,size_t l)const noexcept
    {
        size_t t=getSize();
        t=l>=t?t:l;
        memcpy(des,data.get(),t);
        return t;
    }

};

#endif


PIMPL_IMPL(LocalTranslator)
public:
    std::vector<data_> _data_q;
    atomic_bool prepared;
    char b_tmp[pack_Len];
    imp_FileInfo fi;
    atomic_ulong hassent;
    imp_SocketClient client;
PIMPL_IMPL_END

#endif //SOCKET_CLIENT_IMP_LOCALTRANSLATOR_HPP
