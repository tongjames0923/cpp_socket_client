//
// Created by abstergo on 2022/8/31.
//

#ifndef TESTER_HANDLER_CONFIG_H
#define TESTER_HANDLER_CONFIG_H

#include <chrono>

#define MAX_MESSAGE_SIZE 1024
using delay_time = long long int;
/**
 * 内部运行所用的时间单位
 */
using acc_time=std::chrono::microseconds;

/**
 * 加入队列时所用的时间单位
 */
using _acc=std::chrono::milliseconds;

namespace Tool
{
    template<class T>
    delay_time exchange(T dur)
    {
        return std::chrono::duration_cast<acc_time>(dur).count();
    }
    delay_time getTimeForNow();

}





#endif //TESTER_HANDLER_CONFIG_H
