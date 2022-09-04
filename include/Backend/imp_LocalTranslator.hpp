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

struct data_
{
public:
    std::unique_ptr<char[]> data = nullptr;
    size_t len = 0;

    data_(char *d, size_t l)
    {
        data.reset(d);
        len = l;
    }

    data_()
    {

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
