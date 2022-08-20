//
// Created by tbs on 2022/8/19.
//

#ifndef LOOP_H
#define LOOP_H
#include <thread>
#include "Pointerable.hpp"
class Message;
class imp_LOOP;
void ready();
bool isReady();
class Loop:public virtual AutoAlivePointerable<imp_LOOP>
{
public:
    void loop();
    void enqueue(Message& msg,long long int delay);
    void enqueue(Message&& msg,long long int delay);
    void stopLoop() noexcept;
    void stopLoop_util_empty()noexcept;
    bool isRunning() const noexcept;

    std::thread::id getThreadId() const noexcept;
    Loop();
};


Loop &getMyLoop();

#endif //TESTER_LOOP_H
