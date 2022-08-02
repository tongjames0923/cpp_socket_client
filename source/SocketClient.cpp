#include "SocketClient.h"
#include <boost/bind/bind.hpp>
#include <iostream>
#include <chrono>

using namespace std;
using namespace boost::placeholders;

//#define BLOCKED

asio::io_service io;

asio::io_service &getContext()
{

    return io;
}

SocketClient::~SocketClient()
{
    if (m_socket.is_open())
        m_socket.close();
}

SocketClient::SocketClient(const std::string &ip, const unsigned int &port) : m_ip(ip), m_port(port),
                                                                          m_socket(getContext())
{
}

SocketClient::SocketClient() : m_socket(getContext())
{
}

bool SocketClient::connect()
{
    return connect(m_ip, m_port);
}

int connectFinal(const boost::system::error_code &err, bool *con)
{
    if (!err)
        *con = true;
    return 0;
}

bool SocketClient::connect(const std::string &ip,
                         const unsigned int &port)
{
    if (m_socket.is_open())
    {
        m_socket.close();
    }

    auto endpoint = TCP::endpoint(
            asio::ip::address_v4::from_string(
                    ip.c_str()),
            port);
    bool result = false;
#ifdef BLOCKED
    boost::system::error_code ec;
    m_socket.connect(endpoint,
                     ec);
                     result=!ec.operator bool();
#else
    m_socket.async_connect(endpoint, boost::bind(connectFinal, boost::asio::placeholders::error, &result));
    getContext().run_for(chrono::milliseconds(connectWait));
#endif
    return result;
}

SocketClient &SocketClient::setIp(const std::string &ip)
{
    m_ip = ip;
    return *this;
}

SocketClient &SocketClient::setPort(const unsigned int &port)
{
    m_port = port;
    return *this;
}

std::string SocketClient::getIP() const
{
    return this->m_ip;
}

unsigned int SocketClient::getPort() const
{
    return m_port;
}

size_t SocketClient::send(char *buffer, size_t size)
{
    if (m_socket.is_open())
    {
        return m_socket.send(asio::buffer(buffer, size));
    }
    return -1;
}

size_t SocketClient::receive(char *buffer, size_t size)
{
    if (m_socket.is_open())
    {
        return m_socket.receive(asio::buffer(buffer, size));
    }
    return -1;
}

long SocketClient::getConnectWait() const
{
    return connectWait;
}

SocketClient &SocketClient::setConnectWait(long connectWait)
{
    SocketClient::connectWait = connectWait;
    return *this;
}
