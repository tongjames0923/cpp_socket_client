//
// Created by tbs on 2022/8/19.
//

#ifndef MESSAGE_H
#define MESSAGE_H

#include "Handler/Handler_Config.h"

class Handler;


PIMPL_BEGIN_WITH_DELETER(Message)
public:

/**
 * @brief 获取消息代号
 * 
 * @return int 
 */
int getCode() const;

/// @brief 设置消息代号
/// @param mCode 
void setCode(int mCode);

/// @brief 获取参数1
/// @return 
int getArg1() const;

/// @brief 设置参数1
/// @param mArg1 
void setArg1(int mArg1);

/// @brief 获取参数2
/// @return 
int getArg2() const;

/// @brief 设置参数2
/// @param mArg2 
void setArg2(int mArg2);

#if defined(MAX_MESSAGE_SIZE) && MAX_MESSAGE_SIZE > 0

/// @brief 获取万能数据
/// @param des 所需存放的位置
/// @param len 数据尺寸
void getData(void *des, size_t len) const;

/// @brief 设置万能数据
/// @param data 所需存放的数据
/// @param len 数据尺寸
void setData(const void *data, size_t len);

template <class T>
/// @brief 设置万能数据
/// @tparam T 万能数据的类型
/// @param data 
void setData(const T &data)
{
    setData(&data, sizeof(T));
}
template <class T>
/// @brief 获取万能数据
/// @tparam T 
/// @param data 所需存放的位置
void getData(T *data) const
{
    getData(data, sizeof(T));
}
/// @brief 获取当前万能数据的有效长度
/// @return 
size_t getDataSize() const noexcept;

static size_t  getDataMaxSize()noexcept;
#endif
int getPriority() const noexcept;
void setPriority(int priority) noexcept;
Message &operator=(Message &&o) noexcept;
Message(Message &&o) noexcept;
Message(const Message &ot);
Message &operator=(const Message &o);

/// @brief 获取当前消息的Handler
/// @return 
Handler *getHandler();
/// @brief 
/// @param handler 消息所拥有的Handler
explicit Message(Handler *handler);

Message();

/// @brief 
/// @param code 设置消息代码
explicit Message(int code);

/// @brief 设置当前消息的Handler
/// @param mHandler 
void setHandler(Handler *mHandler);
PIMPL_END
#ifdef MESSAGEQUEUE_TYPE_PRIORITYQUEUE
class DelayMessage
{
public:
	Message msg;
	delay_time time;
	DelayMessage(Message&& msg, delay_time time);
	bool  operator <(const DelayMessage& mg) const;
};

#endif
#endif // TESTER_MESSAGE_H
