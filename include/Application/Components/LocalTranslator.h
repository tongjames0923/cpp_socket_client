//
// Created by tbs on 2022/8/10.
//

#ifndef SOCKET_CLIENT_LOCALTRANSLATOR_H
#define SOCKET_CLIENT_LOCALTRANSLATOR_H
#include "Pointerable.hpp"
#include <functional>
class impl_LocalTranslator;

struct imp_TranslatorDeleter
{
    constexpr imp_TranslatorDeleter() noexcept = default;
    void operator()(impl_LocalTranslator* p) const;
};

class LocalTranslator final :  public virtual None_Copyable,public virtual AutoAlivePointerable<impl_LocalTranslator,imp_TranslatorDeleter>
{
public:

    using callback_data_sent =
    std::function< bool(char* bf,
    size_t should, size_t
    sent)>;
    using callback_file_read =std:: function<void(int pks, size_t read)>;

    LocalTranslator(const LocalTranslator &cpy) = delete;

    LocalTranslator(const char *filePath, const char *ip, unsigned int port);
    ~LocalTranslator();
    bool readyForConnect();
    size_t getTotalFileSize() const;
    size_t getSent()const;

    std::string getFileName() const;

    void setOnSentFail(callback_data_sent cb);

    void setOnSentSuccess(callback_data_sent cb);

    void setOnFileRead(callback_file_read cb);


    size_t runIt();
};



#endif //SOCKET_CLIENT_LOCALTRANSLATOR_H
