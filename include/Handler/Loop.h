//
// Created by tbs on 2022/8/19.
//

#ifndef LOOP_H
#define LOOP_H
#include <thread>
#include "Pointerable.hpp"
#include "Handler/Handler_Config.h"
class Message;
void ready();
bool isReady();

PIMPL_BEGIN_WITH_DELETER(Loop)
public:
    using LoopStopAction=std::function< void(Loop* lp)>;
    void loop();
    void loop_intime(size_t mill);
    void enqueue(Message&& msg,long long int delay);
    void enqueueAt(Message&& msg,long long int at);
    void stopLoop() noexcept;
    void stopLoop_util_empty()noexcept;
    void setOnLoopStop(LoopStopAction ac);
    bool isRunning() const noexcept;
    std::thread::id getThreadId() const noexcept;
    Loop();
PIMPL_END


Loop &getMyLoop();

#endif //TESTER_LOOP_H
