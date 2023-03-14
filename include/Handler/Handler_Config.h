//
// Created by abstergo on 2022/8/31.
//

#ifndef TESTER_HANDLER_CONFIG_H
#define TESTER_HANDLER_CONFIG_H

#include <chrono>
#include "Pointerable.hpp"
#include <string>

///单条消息最大数据尺寸
#define MAX_MESSAGE_SIZE 128


/**
 * 消息队列的速度平衡因子，平衡发送和处理消息的速度推荐1-30内
 */
#define Speed_Balance_Factor 4

/**
 * 消息队列的速度平衡因子，队列等待因子，推荐1000-5000
 */
#define Speed_Balance_Factor_MAX_QUEUE 128
/// @brief 标准延迟时间单位
using delay_time = long long int;
/**
 * 内部运行所用的时间单位
 */
using acc_time = std::chrono::microseconds;

/**
 * 加入队列时所用的时间单位
 */
using _acc = std::chrono::milliseconds;

#define DEBUG
#define MESSAGEQUEUE_TYPE_PRIORITYQUEUE

#ifndef MESSAGEQUEUE_TYPE_PRIORITYQUEUE

#define TIMEWHEEL_CLEAN_LIMIT 0
#endif // !MESSAGEQUEUE_TYPE_PRIORITYQUEUE

const std::string MessageQueueImpl=
#ifdef MESSAGEQUEUE_TYPE_PRIORITYQUEUE
"优先队列"
#else
        "时间轮"
#endif
        ;
/// @brief 时间转换工具
namespace Tool
{
    template <class T>
    /// @brief 将时间T单位转化成内部时间单位
    /// @tparam T 
    /// @param dur 需要转化的时间数据
    /// @return 
    delay_time exchange(T dur)
    {
        return std::chrono::duration_cast<acc_time>(dur).count();
    }
    /// @brief
    /// @return 符合当前的内部单位的时间戳
    delay_time getTimeForNow();

}

#endif // TESTER_HANDLER_CONFIG_H
