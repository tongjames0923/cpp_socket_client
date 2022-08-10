//
// Created by tbs on 2022/8/10.
//

#ifndef SOCKET_CLIENT_IMP_SOCKETCLIENT_H
#define SOCKET_CLIENT_IMP_SOCKETCLIENT_H

#include "FileInfo.h"
#include <boost/asio.hpp>
#include <mutex>
#include <thread>
#include <chrono>
#include <deque>
#include "Application/config.h"
#include <boost/bind/bind.hpp>
#include <chrono>
#include <iostream>

using namespace std;
using namespace boost::placeholders;

namespace asio
{
    using namespace boost::asio;
    using boost::system::error_code;
}


static asio::io_service io;

static asio::io_service &getContext()
{

    return io;
}

#if BOOST_VERSION >= 107000
#define GET_IO_SERVICE(s) \
  ((boost::asio::io_context &)(s).get_executor().context())
#else
#define GET_IO_SERVICE(s) ((s).get_io_service())
#endif

typedef boost::asio::ip::tcp TCP;


class impl_SocketClient
{
public:
    impl_SocketClient(): m_socket(getContext()),m_ip("127.0.0.1"),m_port(1997)
    {
    }

    void init(const std::string &ip, const unsigned int &port)
    {
        setIp(ip);
        setPort(port);
    }
    bool connect()
    {
        return connect(m_ip, m_port);
    }

    bool connect(const std::string &ip,
                 const unsigned int &port)
    {
        closeSocket();
        this->connected = false;
        auto endpoint = TCP::endpoint(
                asio::ip::address_v4::from_string(
                        ip.c_str()),
                port);

        boost::system::error_code ec;
        m_socket.connect(endpoint,
                         ec);
        this->connected = !ec.operator bool();
        return this->connected;
    }

    impl_SocketClient &setIp(const std::string &ip)
    {

        strcpy(m_ip, ip.c_str());
        //m_ip = ip;
        return *this;
    }

    impl_SocketClient &setPort(const unsigned int &port)
    {
        m_port = port;
        return *this;
    }

    std::string getIP() const
    {
        return this->m_ip;
    }

    unsigned int getPort() const
    {
        return m_port;
    }


    size_t send(char *buffer, size_t size)
    {
        if (m_socket.is_open())
        {
            return m_socket.send(asio::buffer(buffer, size));
        }
        return -1;
    }

    size_t receive(char *buffer, size_t size)
    {
        if (m_socket.is_open())
        {
            return m_socket.receive(asio::buffer(buffer, size));

        }
        return -1;
    }


    void closeSocket()
    {
        if (m_socket.is_open())
        {
            if (connected)
                m_socket.shutdown(TCP::socket::shutdown_both);
            m_socket.close();
        }
    }

private:
    TCP::socket m_socket;
    char m_ip[256];
    unsigned int m_port;
    bool connected = false;

};


#endif //SOCKET_CLIENT_IMP_SOCKETCLIENT_H
