//
// Created by tbs on 2022/8/10.
//

#ifndef SOCKET_CLIENT_COMPONENT_H
#define SOCKET_CLIENT_COMPONENT_H
#include "NetPack.h"
#include "Launcher.h"


template <typename D>
class PackQueue_Limited_Len : public virtual LimitedQueue<D>
{
public:
    using pack = PackData<D>;

    PackQueue_Limited_Len() : LimitedQueue<D>(), len(0)
    {
    }
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

class TranslateLauncher : public virtual Launcher
{
public:
    static constexpr char *COMMAND[] = {"-h", "--help", "-config_nick", "-nick", "run"};
    static constexpr int cmd_h = 0, cmd_help = 1, cmd_run = 4, cmd_config_nick = 2, cmd_nick = 3;
protected:
    void ready(int argc, char **argv) override;
};




#endif //SOCKET_CLIENT_COMPONENT_H
