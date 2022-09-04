//
// Created by tbs on 2022/8/19.
//

#ifndef MESSAGE_H
#define MESSAGE_H

#include "Pointerable.hpp"
#include "Handler/Handler_Config.h"

class Handler;

PIMPL_BEGIN_WITH_DELETER(Message)
public:
    int getCode() const;

    void setCode(int mCode);

    int getArg1() const;

    void setArg1(int mArg1);

    int getArg2() const;

    void setArg2(int mArg2);


#if  defined(MAX_MESSAGE_SIZE)&&MAX_MESSAGE_SIZE>0

    void getData(void *des, size_t len) const;

    void setData(const void *data, size_t len);

    template<class T>
    void setData(const T &data)
    {
        setData(&data, sizeof(T));
    }
    template<class T>
    void getData(T *data)const

    {
        getData(data,sizeof(T));
    }
    size_t getDataSize() const noexcept;
#endif
    Message& operator=(Message&& o)noexcept;
    Message(Message&& o)noexcept;
    Message(const Message& ot);
    Message& operator=(const Message& o);

    Handler *getHandler();

    explicit Message(Handler *handler);

    Message();

    explicit Message(int code);

    void setHandler(Handler *mHandler);
PIMPL_END



#endif //TESTER_MESSAGE_H
