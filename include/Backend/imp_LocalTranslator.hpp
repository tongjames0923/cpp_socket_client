//
// Created by tbs on 2022/8/12.
//

#ifndef SOCKET_CLIENT_IMP_LOCALTRANSLATOR_HPP
#define SOCKET_CLIENT_IMP_LOCALTRANSLATOR_HPP

#include <functional>
#include "imp_FileInfo.hpp"
#include "imp_SocketClient.hpp"



class impl_LocalTranslator
{
public:
    using imp_callback_data_sent =
    function<
    bool(char
    * pk,
    size_t should, size_t
    sent)>;
    impl_fileinfo fi;
    size_t hassent = 0;
    impl_SocketClient client;
};

#endif //SOCKET_CLIENT_IMP_LOCALTRANSLATOR_HPP
