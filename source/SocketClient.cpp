#include "Backend/imp_SocketClient.hpp"
#include "SocketClient.h"



SocketClient::SocketClient(const std::string &ip, const unsigned int &port)
{
    myImpl().init(ip, port);
}


bool SocketClient::connect()
{
    return myImpl().connect();
}

bool SocketClient::connect(const string &ip, const unsigned int &port)
{
    return myImpl().connect(ip, port);
}

SocketClient &SocketClient::setIp(const string &ip)
{
    myImpl().setIp(ip);
    return *this;
}

SocketClient &SocketClient::setPort(const unsigned int &port)
{
    myImpl().setPort(port);
    return *this;
}

std::string SocketClient::getIP() const
{
    return cMyImpl().getIP();
}

unsigned int SocketClient::getPort() const
{
    return cMyImpl().getPort();
}

size_t SocketClient::send(char *buffer, size_t size)
{
    return myImpl().send(buffer, size);
}

size_t SocketClient::receive(char *buffer, size_t size)
{
    return myImpl().receive(buffer, size);
}

void SocketClient::closeSocket()
{
    myImpl().closeSocket();
}

SocketClient::SocketClient()
{
}

SocketClient::~SocketClient()=default;

