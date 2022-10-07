//
// Created by tbs on 2022/8/18.
//

#ifndef HANDLER_H
#define HANDLER_H
#include <functional>
#include "Handler/Handler_Config.h"
class Message;
class Loop;


PIMPL_BEGIN_WITH_DELETER(Handler)

public:

///处理消息的方法
using handleAction = std::function<void(Message &)>;

/// @brief 发送消息
/// @param msg 所需发送的消息
/// @param timeout 消息延迟触发，以_acc为准
void sendMsg(Message &&msg, long long timeout) const;
/// @brief 发送消息
/// @param msg  所需发送的消息
/// @param time 需要发送的时间戳，以acc_time为准
void sendMsgAt(Message &&msg, long long time) const;
/// @brief 处理消息
/// @param msg 需要处理的消息
void handle(Message &msg) const;
/// @brief 
/// @param h 处理消息的方法 
explicit Handler(handleAction h);
Handler();
/// @brief 
/// @param loop 配置目标loop 
explicit Handler(Loop *loop);
/// @brief 
/// @param h 设置处理消息的方法 
void setHandle(handleAction h);
/// @brief 获取当前handler所在的loop
/// @return 
Loop *getLoop() const;

PIMPL_IMPL_END

#endif // TESTER_HANDLER_H
