//
// Created by tbs on 2022/8/12.
//

#ifndef SOCKET_CLIENT_PACKQUEUE_LIMITED_LEN_HPP
#define SOCKET_CLIENT_PACKQUEUE_LIMITED_LEN_HPP
#include "NetPack.hpp"
template <typename D>
class PackQueue_Limited_Len : public virtual LimitedQueue<D>
{
public:
    using pack = PackData<D>;

    PackQueue_Limited_Len() : LimitedQueue<D>(), len(0)
    {
    }
    virtual ~PackQueue_Limited_Len()=default;

    size_t getLen() const
    {
        return len;
    }

    void setLen(size_t len)
    {
        if (LimitedQueue<D>::is_ready())
        {
            throw runtime_error("should set on not ready");
        }
        PackQueue_Limited_Len::len = len;
    }

protected:
    bool Ok_To_Push(pack *obj) override
    {

        return LimitedQueue<D>::getQueueSize() < len;
    }

    bool LimitOnPush(pack *pk) override
    {
        return Ok_To_Push(pk);
    }

private:
    atomic_ulong len;
};

#endif //SOCKET_CLIENT_PACKQUEUE_LIMITED_LEN_HPP
