//
// Created by tbs on 2022/8/10.
//

#ifndef SOCKET_CLIENT_IMP_SOCKETCLIENT_H
#define SOCKET_CLIENT_IMP_SOCKETCLIENT_H

#include "FileInfo.h"

#include <mutex>
#include <thread>

#include <deque>
#include "Application/config.h"
#include <iostream>
#include "Application/config.h"

using namespace std;
#define IMPL_ASIO

#ifdef IMPL_ASIO
#include <boost/asio.hpp>
#include <chrono>
#include <boost/bind/bind.hpp>
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
#else
#include <event.h>
#include <arpa/inet.h>
#endif

class impl_SocketClient
{
public:
#ifdef IMPL_ASIO
    impl_SocketClient(): m_socket(getContext()),m_ip("127.0.0.1"),m_port(1997)
    {
    }
#else
impl_SocketClient():base(event_base_new())
{
}

#endif
    ~impl_SocketClient()
    {
        closeSocket();
#ifndef IMPL_ASIO

        if (base != NULL)
        {
            event_base_free(base);
            base = nullptr;
        }
#else
#endif
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
                 const unsigned int &port,bool remember = false)
    {
        closeSocket();
        this->connected = false;
#ifdef IMPL_ASIO
        auto endpoint = TCP::endpoint(
                asio::ip::address_v4::from_string(
                        ip.c_str()),
                port);

        boost::system::error_code ec;
        m_socket.connect(endpoint,
                         ec);
        this->connected = !ec.operator bool();
#else
        sockaddr_in sin;
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = inet_addr(ip.c_str());
        sin.sin_port = htons(port);
        bev= bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
        if (bev==NULL)
        {
            return false;
        }

        //bufferevent_setcb(bev,conn_readcb,conn_writecb,conn_eventcb,this);
        int flag = bufferevent_socket_connect(bev, (struct sockaddr*)&sin, sizeof(sin));
        bufferevent_enable(bev, EV_READ | EV_WRITE);
        bufferevent_set_max_single_read(bev,pack_Len);
        bufferevent_set_max_single_write(bev,pack_Len);
        //FIXME 无法确认是否连接成功
        connected =flag==0;
#endif
        if (connected&&remember)
        {
            std::strcpy(m_ip,ip.c_str());
            m_port=port;
        }
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
#ifdef IMPL_ASIO
        if (m_socket.is_open())
        {
            return m_socket.send(asio::buffer(buffer, size));
        }
#else
        if (connected)
        {
            int fg= bufferevent_write(bev,buffer,size);
            auto ev= bufferevent_get_output(bev);
            size_t left= evbuffer_get_length(ev);
            while (left)
            {
                //cout<<"left:"<<left<<endl;
                event_base_loop(base,EVLOOP_ONCE);
                ev= bufferevent_get_output(bev);
                left=evbuffer_get_length(ev);
            }
            return fg==0?size:0;
        }
#endif

        return -1;
    }

    size_t receive(char *buffer, size_t size)
    {

#ifdef IMPL_ASIO
        if (m_socket.is_open())
        {
            return m_socket.receive(asio::buffer(buffer, size));

        }
#else
        if (connected)
        {
            int fg=0;
            event_base_loop(base,EVLOOP_ONCE);
            fg= bufferevent_read(bev,buffer,size);
            //event_base_loop(base,EVLOOP_ONCE);
            return fg;
        }
#endif

        return -1;
    }


    void closeSocket()
    {
        if (connected)
        {
#ifdef IMPL_ASIO
            if (m_socket.is_open())
        {
            if (connected)
                m_socket.shutdown(TCP::socket::shutdown_both);
            m_socket.close();

        }
#else
            if (bev!= nullptr)
            {
                bufferevent_free(bev);
                bev= nullptr;
            }
#endif
        }

        connected=false;
    }

private:
#ifdef IMPL_ASIO
    TCP::socket m_socket;
#else
    event_base* base= nullptr;
    bufferevent* bev= nullptr;
#endif
    char m_ip[256];
    unsigned int m_port;
    bool connected = false;

};


#endif //SOCKET_CLIENT_IMP_SOCKETCLIENT_H
