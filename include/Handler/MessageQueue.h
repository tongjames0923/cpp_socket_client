//
// Created by tbs on 2022/8/19.
//

#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include "Pointerable.hpp"
#include <functional>
#include "Handler/Handler_Config.h"
class Message;
class imp_MESSAGEQUEUE;
struct MessageQueueDeleter
{
    constexpr MessageQueueDeleter() noexcept = default;

    void operator()(imp_MESSAGEQUEUE *p) const;
};
class MessageQueue:public virtual Pointerable<imp_MESSAGEQUEUE,MessageQueueDeleter>
{
public:
    void enqueue(Message&& msg,delay_time delay);
    void enqueueAt(Message&& msg,delay_time at);
    Message* next();
    void cancel()noexcept;
    void setOnEmptyFunction(std::function<void()> f);
    bool isEmpty() const;

    MessageQueue();

};


#endif //MESSAGEQUEUE_H
