#include "FileSender.h"
#include <boost/bind.hpp>
#include <iostream>
using namespace std;
asio::io_service io;
asio::io_service &getContext()
{

  return io;
}
FileSender::~FileSender()
{
  if (m_socket.is_open())
    m_socket.close();
}
FileSender::FileSender(const std::string &ip, const unsigned int &port) : m_ip(ip), m_port(port), m_socket(getContext())
{
}
FileSender::FileSender() : m_socket(getContext())
{
}
bool FileSender::connect()
{
  return connect(m_ip, m_port);
}

int connectFinal(const boost::system::error_code &err, bool &con)
{
  if (!err)
    con = true;
  return 0;
}

bool FileSender::connect(const std::string &ip,
                         const unsigned int &port)
{
  if (m_socket.is_open())
  {
    m_socket.close();
  }
  boost::system::error_code ec;
  m_socket.connect(TCP::endpoint(
                       asio::ip::address_v4::from_string(
                           ip.c_str()),
                       port),
                   ec);
  return !ec.operator bool();
}
FileSender &FileSender::setIp(const std::string &ip)
{
  m_ip = ip;
  return *this;
}
FileSender &FileSender::setPort(const unsigned int &port)
{
  m_port = port;
  return *this;
}
std::string FileSender::getIP() const
{
  return this->m_ip;
}
unsigned int FileSender::getPort() const
{
  return m_port;
}
size_t FileSender::send(char *buffer, size_t size)
{
  if (m_socket.is_open())
  {
    return m_socket.send(asio::buffer(buffer, size));
  }
  return -1;
}
size_t FileSender::recive(char *buffer, size_t size)
{
  if (m_socket.is_open())
  {
    return m_socket.receive(asio::buffer(buffer, size));
  }
  return -1;
}