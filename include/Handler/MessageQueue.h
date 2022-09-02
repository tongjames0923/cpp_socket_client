//
// Created by tbs on 2022/8/19.
//

#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include "Pointerable.hpp"
#include <functional>
#include "Handler/Handler_Config.h"
class Message;

PIMPL_BEGIN_WITH_DELETER(MessageQueue)
public:
    void enqueue(Message&& msg,delay_time delay);
    void enqueueAt(Message&& msg,delay_time at);
    Message* next();
    void cancel()noexcept;
    void setOnEmptyFunction(std::function<void()> f);
    bool isEmpty() const;

    MessageQueue();
PIMPL_END


#endif //MESSAGEQUEUE_H
