//
// Created by tbs on 2022/8/18.
//

#ifndef HANDLER_H
#define HANDLER_H
#include <functional>
#include "Pointerable.hpp"
#include "Handler/Handler_Config.h"
class Message;
class Loop;
class imp_HANDLER;
struct HandlerDeleter
{
    constexpr HandlerDeleter() noexcept = default;

    void operator()(imp_HANDLER *p) const;
};
class Handler:public virtual AutoAlivePointerable<imp_HANDLER,HandlerDeleter>
{
public:
    static long long int getTimeForNow();
    using handleAction= std::function<void(Message&)>;
    void sendMsg( Message& msg,long long timeout) const;
    void sendMsg( Message&& msg,long long timeout) const;
    void sendMsgAt(Message&& msg,long long time)const;
    void sendMsgAt( Message& msg,long long time) const;
    void handle(Message& msg) const;
    explicit Handler(handleAction h);
    Handler();
    explicit Handler(Loop* loop);
    void setHandle(handleAction h);
    Loop* getLoop() const;
};



#endif //TESTER_HANDLER_H
