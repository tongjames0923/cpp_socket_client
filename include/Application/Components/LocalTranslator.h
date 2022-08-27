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

    void operator()(impl_LocalTranslator *p) const;
};

/// @brief 传输文件封装类
class LocalTranslator final
        : public virtual None_Copyable, public virtual AutoAlivePointerable<impl_LocalTranslator, imp_TranslatorDeleter>
{
public:

    /// @brief 
    /// @param filePath 文件路径
    /// @param ip  发送ip
    /// @param port 发送端口
    LocalTranslator(const char *filePath, const char *ip, unsigned int port);
    LocalTranslator(LocalTranslator&& other)noexcept;

    ~LocalTranslator();

    /// @brief 连接socket
    /// @return 
    bool Connect();

    /// @brief 文件的总大小
    /// @return 
    size_t getTotalFileSize() const;

    /// @brief 已发送的数据长度
    /// @return 
    size_t getSent() const;

    /// @brief 当前发送的文件名
    /// @return 
    std::string getFileName() const;

    std::string getIp()const noexcept;
    int getPort()const noexcept;


    bool prepareData();
    bool hasPrepared()const noexcept;

    size_t sendPreparedData();
};


#endif //SOCKET_CLIENT_LOCALTRANSLATOR_H
