//
// Created by tbs on 2022/8/18.
//

#ifndef HANDLER_H
#define HANDLER_H
#include <functional>
#include "Pointerable.hpp"
class Message;
class Loop;
class imp_HANDLER;
struct HandlerDeleter
{
    constexpr HandlerDeleter() noexcept = default;

    void operator()(imp_HANDLER* p) const
    {
        delete p;
    }
};
class Handler:public virtual AutoAlivePointerable<imp_HANDLER,HandlerDeleter>
{
public:
    using handleAction= std::function<void(Message&)>;
    void sendMsg( Message& msg,long long timeout);
    void sendMsg( Message&& msg,long long timeout);
    void handle(Message& msg);
    explicit Handler(handleAction h);
    Handler();
    void setHandle(handleAction h);
    Loop* getLoop();
};



#endif //TESTER_HANDLER_H
