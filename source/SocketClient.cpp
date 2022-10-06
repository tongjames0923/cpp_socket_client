
#include "SocketClient.h"
#include "Backend/imp_SocketClient.hpp"

SocketClient::SocketClient(const std::string &ip, const unsigned int &port)
{
    PIMPL_INIT(SocketClient);
    m_impl->init(ip,port);
}


bool SocketClient::connect()
{
    return m_impl->connect();
}

bool SocketClient::connect(const string &ip, const unsigned int &port)
{
    return m_impl->connect(ip, port);
}

SocketClient &SocketClient::setIp(const string &ip)
{
    m_impl->setIp(ip);
    return *this;
}

SocketClient &SocketClient::setPort(const unsigned int &port)
{
    m_impl->setPort(port);
    return *this;
}

std::string SocketClient::getIP() const
{
    return m_impl->getIP();
}

unsigned int SocketClient::getPort() const
{
    return m_impl->getPort();
}

size_t SocketClient::send(char *buffer, size_t size)
{
    return m_impl->send(buffer, size);
}

size_t SocketClient::receive(char *buffer, size_t size)
{
    return m_impl->receive(buffer, size);
}

void SocketClient::closeSocket()
{
    m_impl->closeSocket();
}

SocketClient::SocketClient()
{
    PIMPL_INIT(SocketClient);
}

#ifdef IMPL_ASIO

size_t SocketClient::send(boost::asio::mutable_buffer &buffer)
{
    return m_impl->send(buffer);
}

#endif

SocketClient::~SocketClient() = default;

void SocketClientDeleter::operator()(imp_SocketClient *p) const
{
    delete p;
}

