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
#include "Application/config.h"

using data_ =
#ifdef IMPL_ASIO
asio::mutable_buffer
#else
PackedData
#endif
;

PIMPL_IMPL(LocalTranslator)
public:
    imp_LocalTranslator()
    {
        prepared_status = not_prepared;
    }

    static constexpr const int not_prepared = 0, preparing = 1, prepared = 2;
    std::vector<data_> _data_q;
    atomic_int prepared_status{not_prepared};
    char b_tmp[pack_Len]{0};
    imp_FileInfo fi;
    atomic_ulong hassent{0};
    imp_SocketClient client;
PIMPL_IMPL_END

#endif //SOCKET_CLIENT_IMP_LOCALTRANSLATOR_HPP
