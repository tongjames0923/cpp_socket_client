#include "SocketClient.h"
#include <boost/bind/bind.hpp>
#include <chrono>

using namespace std;
using namespace boost::placeholders;


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
    auto endpoint = TCP::endpoint(
            asio::ip::address_v4::from_string(
                    ip.c_str()),
            port);
    bool result = false;
    boost::system::error_code ec;
    m_socket.connect(endpoint,
                     ec);
    result = !ec.operator bool();
    return result;
}

SocketClient &SocketClient::setIp(const std::string &ip)
{
    strcpy(this->m_ip, ip.c_str());
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
        m_socket.close();
    }
}
