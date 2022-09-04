//
// Created by abstergo on 2022/9/7.
//

#ifndef TESTER_WRITEREADLOCK_H
#define TESTER_WRITEREADLOCK_H

#include "Pointerable.hpp"

PIMPL_BEGIN_WITH_DELETER(WriteReadLock)
public:
    static constexpr const int READ_FIRST=0,WRITE_FIRST=1;
WriteReadLock();
    void lockWrite();

    void lockRead();

    void unlockWrite();

    void unlockRead();

PIMPL_END



#endif //TESTER_WRITEREADLOCK_H
