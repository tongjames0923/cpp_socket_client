//
// Created by tbs on 2022/8/12.
//

#ifndef SOCKET_CLIENT_IMP_LOCALTRANSLATOR_HPP
#define SOCKET_CLIENT_IMP_LOCALTRANSLATOR_HPP

#include <functional>
#include "imp_FileInfo.hpp"
#include "imp_SocketClient.hpp"

template<typename T>
class PackData;

using pack=PackData<char>;


class impl_LocalTranslator
{
public:
    using imp_callback_data_sent =
    function<
    bool(pack
    * pk,
    size_t should, size_t
    sent)>;
    using imp_callback_file_read = function<void(
    int pks, size_t
    read)>;
    impl_fileinfo fi;
    impl_SocketClient client;
    imp_callback_data_sent onSentFail = nullptr;
    imp_callback_data_sent onSentSuccess = nullptr;
    imp_callback_file_read onFileRead = nullptr;
};

#endif //SOCKET_CLIENT_IMP_LOCALTRANSLATOR_HPP
