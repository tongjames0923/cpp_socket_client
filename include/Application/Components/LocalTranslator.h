//
// Created by tbs on 2022/8/10.
//

#ifndef SOCKET_CLIENT_LOCALTRANSLATOR_H
#define SOCKET_CLIENT_LOCALTRANSLATOR_H

#include "Pointerable.hpp"
#include <functional>

enum prepare_status
{
    not_prepared=0,preparing=1,prepared=2
};


/// @brief 传输文件封装类
PIMPL_BEGIN_WITH_DELETER(LocalTranslator)
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
    prepare_status getPrepareStatus() const noexcept;
    bool prepareData();
    void clear_data() noexcept;

    size_t sendPreparedData();
PIMPL_END



#endif //SOCKET_CLIENT_LOCALTRANSLATOR_H
