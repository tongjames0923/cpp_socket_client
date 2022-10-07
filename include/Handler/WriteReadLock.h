//
// Created by abstergo on 2022/9/7.
//

#ifndef TESTER_WRITEREADLOCK_H
#define TESTER_WRITEREADLOCK_H

#include "Pointerable.hpp"

PIMPL_BEGIN_WITH_DELETER(WriteReadLock)
public:
WriteReadLock();
void lockWrite();

void lockRead();

void unlockWrite();

void unlockRead();

PIMPL_END

class w_Lock_Guard
{
    public:
    w_Lock_Guard(WriteReadLock& lock)
    {
        rk=&lock;
        rk->lockWrite();
    }
    ~w_Lock_Guard()
    {
        rk->unlockWrite();
    }
    private:
    WriteReadLock* rk;
};
class r_Lock_Guard
{
    public:
    r_Lock_Guard(WriteReadLock& lock)
    {
        rk=&lock;
        rk->lockRead();
    }
    ~r_Lock_Guard()
    {
        rk->unlockRead();
    }
    private:
        WriteReadLock* rk;
};


#endif // TESTER_WRITEREADLOCK_H
