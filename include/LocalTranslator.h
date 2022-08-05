//
// Created by tbs on 2022/8/5.
//

#ifndef SOCKET_CLIENT_LOCALTRANSLATOR_H
#define SOCKET_CLIENT_LOCALTRANSLATOR_H

#include "NetPack.h"
#include "FileInfo.h"
#include "SocketClient.h"
using namespace  std;


class LocalTranslator: private virtual PackQueue_Limited_Len<char>
{
public:
    using callback_data_sent=function<bool(pack* pk,size_t should,size_t sent)>;
    LocalTranslator(const LocalTranslator& cpy)=delete;
    LocalTranslator(const char *filePath, const char *ip, unsigned int port,
                    size_t packLimit);
    void runIt();

protected:
    bool OnReady() override;

    bool OnDone() override;

private:
    FileInfo fi;
    SocketClient client;
    callback_data_sent onSentFail= nullptr;
    callback_data_sent onSentSuccess= nullptr;
    
};


#endif //SOCKET_CLIENT_LOCALTRANSLATOR_H
