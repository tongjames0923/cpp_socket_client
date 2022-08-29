//
// Created by tbs on 2022/8/19.
//

#ifndef MESSAGE_H
#define MESSAGE_H

#include "Pointerable.hpp"


class imp_MESSAGE;

class Handler;
#define MAX_MESSAGE_SIZE 1024

struct MessageDeleter
{
    constexpr MessageDeleter() noexcept = default;

    void operator()(imp_MESSAGE *p) const;
};

class Message : public Pointerable<imp_MESSAGE, MessageDeleter>
{
public:
    int getCode() const;

    void setCode(int mCode);

    int getArg1() const;

    void setArg1(int mArg1);

    int getArg2() const;

    void setArg2(int mArg2);


#if  defined(MAX_MESSAGE_SIZE)&&MAX_MESSAGE_SIZE>0

    void getData(void *des, size_t len);

    void setData(const void *data, size_t len);

    template<class T>
    void setData(const T &data)
    {
        setData(&data, sizeof(T));
    }
    template<class T>
    void getData(T *data)
    {
        getData(data,sizeof(T));
    }
    size_t getDataSize() const noexcept;
#endif
    Message& operator=(Message&& o)noexcept;
    Message(Message&& o)noexcept;


    Handler *getHandler();

    explicit Message(Handler *handler);

    Message();

    explicit Message(int code);

    void setHandler(Handler *mHandler);

};

#endif //TESTER_MESSAGE_H
