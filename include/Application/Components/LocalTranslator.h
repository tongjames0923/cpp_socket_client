//
// Created by tbs on 2022/8/10.
//

#ifndef SOCKET_CLIENT_LOCALTRANSLATOR_H
#define SOCKET_CLIENT_LOCALTRANSLATOR_H
#include "Pointerable.hpp"
#include "PackQueue_Limited_Len.hpp"
class impl_LocalTranslator;
class LocalTranslator final : private virtual PackQueue_Limited_Len<char>, public virtual None_Copyable,public virtual AutoAlivePointerable<impl_LocalTranslator>
{
public:

    using callback_data_sent =
    function< bool(PackQueue_Limited_Len<char>::pack
    * pk,
    size_t should, size_t
    sent)>;
    using callback_file_read = function<void(int pks, size_t read)>;

    LocalTranslator(const LocalTranslator &cpy) = delete;

    LocalTranslator(const char *filePath, const char *ip, unsigned int port,
                    size_t packLimit);
    ~LocalTranslator();
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
};



#endif //SOCKET_CLIENT_LOCALTRANSLATOR_H
