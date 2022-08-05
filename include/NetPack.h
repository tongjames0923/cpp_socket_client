//
// Created by tbs on 2022/8/4.
//

#ifndef TESTER_NETPACK_H
#define TESTER_NETPACK_H

#include <deque>
#include <memory>
#include <string>
#include <fstream>
#include <functional>
#include <atomic>

using namespace std;

template<typename D>
class PackData
{
private:
    unique_ptr<D[]> data = nullptr;
    unique_ptr<char[]> buffer = nullptr;
    size_t bufferSize = -1;
    size_t objSize = -1;
protected:
    virtual size_t ObjectToBuffer(const D &source, size_t objlen, char *des, size_t bsize)
    {
        size_t ds = sizeof(D) * objlen;
        if (ds <= bsize)
        {
            memcpy(des, &source, ds);
            return ds;
        } else
        {
            return bsize - ds;
        }
    }

    virtual size_t BufferToObject(D *des, size_t objlen, char *bf, size_t size)
    {
        size_t ds = sizeof(D) * objlen;
        if (ds > size)
        {
            memcpy(des, bf, size);
            return ds;
        } else
            return ds-size;

    }

public:

    PackData() = default;

    explicit PackData(size_t bsize)
    {
        setBufferSize(bsize);
    }

    explicit PackData(const D *d, size_t bs = sizeof(D))
    {
        setData(d, bs / (sizeof(D)));
        setBufferSize(bs);
    }

    size_t getBufferSize() const
    {
        return bufferSize;
    }

    size_t setBufferSize(size_t bs)
    {
        size_t old = bufferSize;
        if (bs == 0)
        {
            if (buffer != nullptr)
                buffer.reset(nullptr);
            return -1;
        } else if (bs == bufferSize)
        {
            return 0;
        } else if (buffer.get() == nullptr)
        {
            bufferSize = bs;
            char *tmp = new char[bs];
            buffer.reset(tmp);
            return bs;
        } else
        {
            bufferSize = bs;
            char *tmp = new char[bs];
            size_t cplen = min(bs, old);
            memcpy(tmp, buffer.get(), cplen);
            return bufferSize - old;
        }
    }

    D *getData() const
    {
        return data.get();
    }

    size_t getObjectLen()
    {
        return objSize;
    }
    void setObjectLen(size_t len)
    {
        data.reset(new D[len]);
        objSize = len;
    }

    void setData(const D *d, size_t len = 1)
    {
        if(len!=getObjectLen())
        {
            setObjectLen(len);
        }
        std::memcpy(data.get(), d, sizeof(D) * len);
    }

    D *pushBuffer(char* bf,size_t bfs,bool force= false)
    {
        size_t stz = getObjectLen() * sizeof(D);
        size_t  ojl=getObjectLen();
        if(stz<bfs)
        {
            if(force)
            {
                setObjectLen(bfs/ sizeof(D));

            }
            else
                throw runtime_error("too small buffer for objects");

        }
         BufferToObject(getData(), ojl, bf, bfs);
        return getData();

    }

    char *pullData(const D &d, size_t len = 1)
    {
        size_t obj_succe = ObjectToBuffer(d, len, buffer.get(), bufferSize);
        if (obj_succe < 0)
            throw std::runtime_error("out of size for it");
        return buffer.get();
    }

    char *pullData()
    {
        return pullData(*getData(), objSize);
    }
};


template<typename D>
class LimitedQueue
{
public:
    using pack = PackData<D>;
    using packHandler = function<bool(pack *)>;

    LimitedQueue() = default;

    explicit LimitedQueue(packHandler fun) : handler(fun)
    {

    }

    const packHandler &getHandler() const
    {
        return handler;
    }

    void setHandler(const packHandler &fun)
    {
        LimitedQueue::handler = fun;
    }

    void handle_once(packHandler h = nullptr)
    {
        if (!is_ready())
        {
            throw runtime_error("nothing to handler");
        }
        if (getQueueSize() > 0)
        {
            if (h != nullptr)
                setHandler(h);
            packHandler ph = getHandler();
            pack &pk = list.front();
            ph(&pk);
            OnPopWhenHandle(&pk);
            list.pop_front();
        }

    }

    void handle(packHandler h = nullptr)
    {
        if (!is_ready())
        {
            throw runtime_error("nothing to handler");
        }
        if (h != nullptr)
            setHandler(h);
        packHandler ph = getHandler();
        while (ph != nullptr && !list.empty())
        {
            pack &pk = list.front();
            ph(&pk);
            OnPopWhenHandle(&pk);
            list.pop_front();
        }
        done();
    }

    size_t getQueueSize()
    {
        return list.size();
    }

    bool pushIn(const D &d, bool force = false)
    {
        return pushIn(pack(&d), force);
    }

    size_t PushAll(pack *pkgs, size_t len, bool force)
    {
        if (!is_ready())
        {
            throw runtime_error("you need ready for push");
        }
        size_t ts = 0;
        for (int i = 0; i < len; i++)
        {
            pack *beg = pkgs + i;
            if (force || Ok_To_Push(beg))
            {
                list.push_back(std::move(*beg));
                ts++;
            }
            beg++;
        }
        auto iter = list.begin();
        while (handler != nullptr && iter < list.end())
        {
            pack &pk = *(iter++);
            if (LimitOnPush(&pk))
                break;

            bool oktoD = handler(&pk);
            if (oktoD)
            {
                OnPopWhenPush(&pk);
                list.pop_front();
            }
        }
        return ts;

    }

    bool pushIn(pack &&pkg, bool force = false)
    {
        bool rs = PushAll(&pkg, 1, force);
        return rs;
    }

    bool is_ready()
    {
        return m_ready;
    }

    void ready()
    {
        m_ready = true;
    }

    void done()
    {
        m_ready = false;
        list.clear();
    }

private:
    deque<pack> list;
    bool m_ready = false;
    packHandler handler = nullptr;
protected:
    virtual bool Ok_To_Push(pack *obj)
    {
        return true;
    }

    virtual bool LimitOnPush(pack *pk)
    {
        return true;
    }

    virtual void OnPopWhenPush(pack *will)
    {

    }

    virtual void OnPopWhenHandle(pack *left)
    {

    }
};


template<typename D>
class PackQueue_Limited_Len : public virtual LimitedQueue<D>
{
public:
    using pack = PackData<D>;

    PackQueue_Limited_Len() : LimitedQueue<D>(), len(0)
    {
    }

    explicit PackQueue_Limited_Len(size_t z) : LimitedQueue<D>(), len(z)
    {
    }

    size_t getLen() const
    {
        return len;
    }

    void setLen(size_t len)
    {
        if (!LimitedQueue<D>::is_ready())
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
        return !Ok_To_Push(pk);
    }

private:
    atomic_ulong len;


};


#endif //TESTER_NETPACK_H
