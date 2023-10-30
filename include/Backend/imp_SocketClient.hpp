//
// Created by tbs on 2022/8/10.
//

#ifndef SOCKET_CLIENT_IMP_SOCKETCLIENT_HPP
#define SOCKET_CLIENT_IMP_SOCKETCLIENT_HPP

#include <string>
#include <iostream>
#include "Application/config.h"


using namespace std;


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
#include <string>
#include <thread>
#include <sstream>
#include <atomic>
#include <winsock2.h>
#include <cstring>
#include "Application/config.h"

#endif

PIMPL_IMPL(SocketClient)
public:
#ifdef IMPL_ASIO

    imp_SocketClient() : m_socket(getContext()), m_ip("127.0.0.1"), m_port(1997)
    {
    }

#else

    imp_SocketClient() : base(event_base_new())
    {
    }

#endif

    ~imp_SocketClient()
    {
        closeSocket();
#ifndef IMPL_ASIO
        if (base != nullptr)
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
                 const unsigned int &port, bool remember = false)
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
        closeSocket();
        sockaddr_in sin{};
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = inet_addr(ip.c_str());
        sin.sin_port = htons(port);

        bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
        if (bev == NULL)
        {
            return false;
        }
        bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, this);
        bufferevent_set_max_single_write(bev, pack_Len);
        bufferevent_set_max_single_read(bev, pack_Len);
        //bufferevent_setcb(bev,conn_readcb,conn_writecb,conn_eventcb,this);
        bufferevent_socket_connect(bev, (struct sockaddr *) &sin, sizeof(sin));
        bufferevent_enable(bev, EV_READ | EV_WRITE | EV_PERSIST);
        event_base_loop(base, EVLOOP_ONCE);
#endif
        if (connected && remember)
        {
            std::strcpy(m_ip, ip.c_str());
            m_port = port;
        }
        return this->connected;
    }

    imp_SocketClient &setIp(const std::string &ip)
    {

        strcpy(m_ip, ip.c_str());
        //m_ip = ip;
        return *this;
    }

    imp_SocketClient &setPort(const unsigned int &port)
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

#ifdef IMPL_ASIO

    size_t send(asio::mutable_buffer &bf)
    {
        if (connected)
        {
            return m_socket.send(bf);
        }
        return -1;
    }

#endif // IM

    size_t send(char *buffer, size_t size)
    {
#ifdef IMPL_ASIO
        if (connected)
        {
            return m_socket.send(asio::buffer(buffer, size));
        }

#else
        if (connected)
        {
            int fg = bufferevent_write(bev, buffer, size);
            ++sends;
            event_base_dispatch(base);
//            std::unique_lock<std::mutex> lock(waiter_mutex);
//            waiter.wait(lock,[this]{return sends==0;});
            return sends == 0 && fg == 0 ? size : 0;
        }
#endif

        return -1;
    }


    size_t receive(char *buf, size_t buflen)
    {

#ifdef IMPL_ASIO
        if (m_socket.is_open())
        {
            return m_socket.receive(asio::buffer(buf, buflen));
        }
#else
        if (connected)
        {
            size_t receives = 0;
            char buffer[singleReadLen];
            receives = bufferevent_read(bev, buffer, singleReadLen);
            reader.write(buffer, receives);
            event_base_dispatch(base);
            reader.read(buf, buflen);
            receives = reader.gcount();
            reader.clear();
            return receives;
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
            event_loopbreak();
            event_base_loopexit(base, nullptr);
            if (bev != nullptr)
            {
                bufferevent_free(bev);
                bev = nullptr;
            }
#endif
        }

        connected = false;
    }

private:
#ifdef IMPL_ASIO
    TCP::socket m_socket;
#else

    static void conn_eventcb(struct bufferevent *bev, short events, void *user_data)
    {

        imp_SocketClient *client = (imp_SocketClient *)
                user_data;
        if (events & BEV_EVENT_CONNECTED)
        {
            client->connected = true;
        }
        else if (events & BEV_EVENT_EOF || events & BEV_EVENT_ERROR)
        {
            client->connected = false;
        }
        /* None of the other events can happen here, since we haven't enabled
         * timeouts */
        //bufferevent_free(bev);
    }

    static void
    conn_writecb(struct bufferevent *bev, void *user_data)
    {
        imp_SocketClient *client = (imp_SocketClient *)
                user_data;
        --(client->sends);
        if (client->sends == 0)
        {
            event_loopbreak();
            event_base_loopexit(client->base, nullptr);
        }

//        size_t ln= evbuffer_get_length(bufferevent_get_output(bev));
//        while (ln > 0)
//        {
//
//            //event_base_loop(client->base, EVLOOP_ONCE);
//            ln= evbuffer_get_length(bufferevent_get_output(bev));
//        }
//        event_loopbreak();
//        event_base_loopexit(client->base, nullptr);
    }

    static void conn_readcb(struct bufferevent *bev, void *user_data)
    {
        imp_SocketClient *client = (imp_SocketClient *)
                user_data;
        size_t ln = evbuffer_get_length(bufferevent_get_input(bev));
        char buffer[singleReadLen];
        while (ln > 0)
        {

            size_t rd = bufferevent_read(client->bev, buffer, ln);
            client->reader.write(buffer, rd);
            ln = evbuffer_get_length(bufferevent_get_input(bev));
        }

    }

    size_t sends = 0;
    std::stringstream reader;
    event_base *base = nullptr;
    bufferevent *bev = nullptr;
    static constexpr const size_t singleReadLen = 4 * 1024;
#endif
    char m_ip[256];
    unsigned int m_port;
    bool connected = false;

PIMPL_IMPL_END


#endif //SOCKET_CLIENT_IMP_SOCKETCLIENT_HPP
