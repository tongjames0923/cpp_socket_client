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
#define GET_IO_SERVICE(s)                                                     \
  ((boost::asio::io_context &)(s).get_executor ().context ())
#else
#define GET_IO_SERVICE(s) ((s).get_io_service ())
#endif
asio::io_service& getContext ();
typedef asio::ip::tcp TCP;
class FileSender
{
private:
  TCP::socket m_socket;
  std::string m_ip;
  unsigned int m_port;
  long connectWait=5000;
public:
  ~FileSender ();
  FileSender ();
  FileSender (const std::string &ip, const unsigned int &port);
  bool connect ();
  bool connect (const std::string &ip, const unsigned int &port);
  FileSender &setIp (const std::string &ip);
  FileSender &setPort (const unsigned int &port);
  std::string getIP () const;
  unsigned int getPort () const;
  size_t send (char *buffer, size_t size);
  size_t recive(char* buffer,size_t size);
};
template<class R1,class ...ARGS>
class Timer
{
  private:
  using fcter=std::function<R1(ARGS...)>;
  fcter functioner;
  long delay;
  bool ready=false;
  std::mutex lock;
  
  static R1 threadFunction(Timer* obj,ARGS ...args)
  {
    std::lock_guard<std::mutex> g(obj->lock);
    std::this_thread::sleep_for(std::chrono::milliseconds(obj->delay));
    R1 result=obj->functioner(args...);
    obj->ready=true;
    return result;
  }
  public:
  Timer(const long& d,fcter f):functioner(f),delay(d),ready(true)
  {
    
  }
  R1 delayInvoke(ARGS ...args,bool isAsyc=true)
  {
    if(ready)
    {
      ready=false;
      std::thread back(threadFunction,this,args...);
      if(isAsyc)
      back.detach();
      else
      {
        back.join();
      }
    }
    else
    throw "not ready";
  }

};