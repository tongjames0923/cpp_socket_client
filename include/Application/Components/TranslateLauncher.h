//
// Created by tbs on 2022/8/12.
//

#ifndef SOCKET_CLIENT_TRANSLATELAUNCHER_H
#define SOCKET_CLIENT_TRANSLATELAUNCHER_H
#include "Launcher.h"

class TranslateLauncher : public virtual Launcher
{
public:

    ~TranslateLauncher() override;
    int prioritySet(const char* argKey, size_t len)const noexcept override;

    int groupSet(const char *argKey, size_t len) const noexcept override;

protected:
    void ready(int argc, char **argv) override;
    void cleanup() override;
};

#endif //SOCKET_CLIENT_TRANSLATELAUNCHER_H
