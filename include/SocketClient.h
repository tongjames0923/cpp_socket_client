//
// Created by tbs on 2022/8/4.
//

#ifndef SOCKET_CLIENT_SOCKETCLIENT_H
#define SOCKET_CLIENT_SOCKETCLIENT_H

#include <string>
#include "Pointerable.hpp"
class impl_SocketClient;
/**
 * 端口客户端
 */
class SocketClient:public virtual AutoAlivePointerable<impl_SocketClient>,None_Copyable
{
public:

    SocketClient();

    ~SocketClient();
    /**
     *
     * @param ip 连接的ip
     * @param port 连接的端口
     */
    SocketClient(const std::string &ip, const unsigned int &port);

    /**
     * 连接自成员变量ip和port
     * @return 是否连接成功
     */
    bool connect();

    /**
     * 连接
     * @param ip 连接到的ip
     * @param port 连接到的端口
     * @return 是否连接成功
     */
    bool connect(const std::string &ip, const unsigned int &port);


    /// 设置ip
    /// \param ip
    /// \return
    SocketClient &setIp(const std::string &ip);

    /// 设置端口
    /// \param port
    /// \return
    SocketClient &setPort(const unsigned int &port);

    /// 获取ip
    /// \return
    std::string getIP() const;

    /// 获取端口
    /// \return
    unsigned int getPort() const;

    /// 发送数据
    /// \param buffer 数据缓存区
    /// \param size 数据尺寸
    /// \return 成功发送的长度
    size_t send(char *buffer, size_t size);

    /// 接收数据
    /// \param buffer 数据缓存区
    /// \param size 缓存区长度
    /// \return 接收到的数据长度
    size_t receive(char *buffer, size_t size);

    ///关闭连接
    void closeSocket();

};



#endif //SOCKET_CLIENT_SOCKETCLIENT_H
