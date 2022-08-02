#pragma once

#include "FileInfo.h"
#include <boost/asio.hpp>
#include <mutex>
#include <thread>
#include <chrono>
#include <deque>

namespace asio
{
    using namespace boost::asio;
    using boost::system::error_code;
}
#if BOOST_VERSION >= 107000
#define GET_IO_SERVICE(s) \
  ((boost::asio::io_context &)(s).get_executor().context())
#else
#define GET_IO_SERVICE(s) ((s).get_io_service())
#endif

/**
 * 获取asio的运行时
 * @return
 */
asio::io_service &getContext();

typedef asio::ip::tcp TCP;


/**
 * 端口客户端
 */
class SocketClient
{
private:
    TCP::socket m_socket;
    std::string m_ip;
    unsigned int m_port;
    long connectWait = 5000;

public:
    ~SocketClient();

    SocketClient();

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

    ///
    /// \return 获取连接超时
    long getConnectWait() const;

    ///
    /// \param connectWait 连接超时
    /// \return 返回自身
    SocketClient &setConnectWait(long connectWait);

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
};