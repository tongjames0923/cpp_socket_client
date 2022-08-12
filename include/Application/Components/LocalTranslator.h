//
// Created by tbs on 2022/8/10.
//

#ifndef SOCKET_CLIENT_LOCALTRANSLATOR_H
#define SOCKET_CLIENT_LOCALTRANSLATOR_H

#include <FileInfo.h>
#include <SocketClient.h>
#include "PackQueue_Limited_Len.hpp"

class LocalTranslator : private virtual PackQueue_Limited_Len<char>
{
public:
    using callback_data_sent =
    function< bool(pack
    * pk,
    size_t should, size_t
    sent)>;
    using callback_file_read = function<void(int pks, size_t read)>;

    LocalTranslator(const LocalTranslator &cpy) = delete;

    LocalTranslator(const char *filePath, const char *ip, unsigned int port,
                    size_t packLimit);

    bool readyForConnect();

    size_t getTotalFileSize() const;

    std::string getFileName() const;

    void setOnSentFail(callback_data_sent cb);

    void setOnSentSuccess(callback_data_sent cb);

    void setOnFileRead(callback_file_read cb);


    size_t runIt();

protected:
    bool OnReady() override;

    bool OnDone() override;

private:
    FileInfo fi;
    SocketClient client;
    callback_data_sent onSentFail = nullptr;
    callback_data_sent onSentSuccess = nullptr;
    callback_file_read onFileRead = nullptr;

};



#endif //SOCKET_CLIENT_LOCALTRANSLATOR_H
