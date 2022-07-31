#include "FileSender.h"
using namespace std;
asio::io_service io;
asio::io_service& getContext ()
{
  return io;
}
FileSender::~FileSender ()
{
    if(m_socket.is_open())
    m_socket.close();
}
FileSender::FileSender (const std::string &ip, const unsigned int &port):m_ip(ip),m_port(port),m_socket(io)
{

}
FileSender::FileSender () : m_socket (getContext ()) 
{

}
FileSender &FileSender::connect () 
{
   return  connect(m_ip,m_port);
}
FileSender &FileSender::connect (const std::string &ip,
                                 const unsigned int &port)
                                 {
                                   if (m_socket.is_open ())
                                     {
                                       m_socket.close ();
                                     }
                                   m_socket.connect (TCP::endpoint (
                                       asio::ip::address_v4::from_string (
                                           ip.c_str ()),
                                       port));
                                       return *this;
                                 }
FileSender &FileSender::setIp (const std::string &ip)
{
    m_ip=ip;
    return *this;
}
FileSender &FileSender::setPort (const unsigned int &port)
{
    m_port=port;
    return *this;
}
std::string FileSender::getIP () const
{
    return this->m_ip;
}
unsigned int FileSender::getPort () const
{
    return m_port;
}
size_t FileSender::send (char *buffer, size_t size)
{
    if(m_socket.is_open())
    {
      return   m_socket.send(asio::buffer(buffer,size));
    }
    return -1;
}