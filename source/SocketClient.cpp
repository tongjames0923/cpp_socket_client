#include "SocketClient.h"
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



asio::io_service io;

asio::io_service &getContext()
{
    
    return io;
}

SocketClient::~SocketClient()
{

    closeSocket();
}

SocketClient::SocketClient(const std::string &ip, const unsigned int &port) : m_port(port),
                                                                          m_socket(getContext())
{
    setIp(ip);
}

SocketClient::SocketClient() : m_socket(getContext())
{
}

bool SocketClient::connect()
{
    return connect(m_ip, m_port);
}

bool SocketClient::connect(const std::string &ip,
                         const unsigned int &port)
{
    closeSocket();
    this->connected= false;
    auto endpoint = TCP::endpoint(
            asio::ip::address_v4::from_string(
                    ip.c_str()),
            port);

    boost::system::error_code ec;
    m_socket.connect(endpoint,
                     ec);
    this->connected=!ec.operator bool();
    return     this->connected;
}

SocketClient &SocketClient::setIp(const std::string &ip)
{
    
    strcpy(m_ip,ip.c_str());
    //m_ip = ip;
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


void SocketClient::closeSocket()
{
    if (m_socket.is_open())
    {
        if(connected)
        m_socket.shutdown(TCP::socket::shutdown_both);
        m_socket.close();
    }
}