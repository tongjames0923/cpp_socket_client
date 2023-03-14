//
// Created by tbs on 2022/8/19.
//

#ifndef LOOP_H
#define LOOP_H
#include <thread>
#include <functional>
#include "Handler/Handler_Config.h"

class Message;

/// @brief 使当前线程的Loop初始化
void ready();
/// @brief 判断当前线程的Loop是否初始化成功
/// @return 
bool isReady();

PIMPL_BEGIN_WITH_DELETER(Loop)
public:
using LoopStopAction = std::function<void(Loop *lp)>;
/// @brief 开始消息循环
void loop();
/// @brief 在mill毫秒内运行消息循环
/// @param mill 需要运行的时间
/// @param wait_empty true:直到消息队列空了才停止，false：到时立刻停止
void loop_intime(size_t mill, bool wait_empty = false);
/// @brief 加入队列
/// @param msg 需要加入的消息
/// @param delay 消息触发的延迟时间，与_acc为准
void enqueue(Message &&msg, long long int delay);
/// @brief 加入队列
/// @param msg  需要加入的消息
/// @param at  消息触发的时间点，与acc_time为准
void enqueueAt(Message &&msg, long long int at);
/// @brief 停止消息循环
void stopLoop() noexcept;
/// @brief 当消息循环中没有消息停止
void stopLoop_util_empty() noexcept;
/// @brief 设置处理消息循环停止时的方法
/// @param ac 
void setOnLoopStop(LoopStopAction ac);
/// @brief 消息循环是否在运行
/// @return 
bool isRunning() const noexcept;
/// @brief 当前的线程id
/// @return 
std::thread::id getThreadId() const noexcept;

void clearQueue();



Loop();
PIMPL_END

Loop &getMyLoop();

#endif // TESTER_LOOP_H
