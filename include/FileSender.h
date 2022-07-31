#pragma once
#include "FileInfo.h"
#include <boost/asio.hpp>
namespace asio
{
using namespace boost::asio;
using boost::system::error_code;
}
#if BOOST_VERSION >= 107000
#define GET_IO_SERVICE(s)                                                     \
  ((boost::asio::io_context &)(s).get_executor ().context ())
#else
#define GET_IO_SERVICE(s) ((s).get_io_service ())
#endif

typedef asio::ip::tcp TCP;

asio::io_service &getContext ();

class FileSender
{
private:
  TCP::socket m_socket;
  std::string m_ip;
  unsigned int m_port;

public:
  ~FileSender ();
  FileSender ();
  FileSender (const std::string &ip, const unsigned int &port);
  FileSender &connect ();
  FileSender &connect (const std::string &ip, const unsigned int &port);
  FileSender &setIp (const std::string &ip);
  FileSender &setPort (const unsigned int &port);
  std::string getIP () const;
  unsigned int getPort () const;
  size_t send (char *buffer, size_t size);
};