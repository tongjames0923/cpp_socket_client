//
// Created by tbs on 2022/8/19.
//

#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <functional>
#include "Handler/Handler_Config.h"
class Message;

PIMPL_BEGIN_WITH_DELETER(MessageQueue)
public:

/**
 * @brief 加入消息队列
 * 
 * @param msg 加入的消息
 * @param delay 触发的延迟，以_acc为准
 */
void enqueue(Message &&msg, delay_time delay);
/// @brief 加入消息队列
/// @param msg 加入的消息
/// @param at 触发消息的时间戳，以acc_time为准
void enqueueAt(Message &&msg, delay_time at);
/// @brief 获取下一个需要处理的消息，阻塞
/// @return 
Message *next();
/// @brief 取消等待
void cancel() noexcept;
/// @brief 设置队列为空时的方法
/// @param f 
void setOnEmptyFunction(std::function<void()> f);
/// @brief 当前队列是否为空
/// @return 
bool isEmpty() const;

size_t size() const;

void clear();

MessageQueue();
PIMPL_END

#endif // MESSAGEQUEUE_H
