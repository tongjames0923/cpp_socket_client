//
// Created by tbs on 2022/8/12.
//

#ifndef SOCKET_CLIENT_TRANSLATELAUNCHER_H
#define SOCKET_CLIENT_TRANSLATELAUNCHER_H
#include "Launcher.h"

class TranslateLauncher : public virtual Launcher
{
public:
    static constexpr const char * COMMAND[] = {"-h", "--help", "-config_nick", "-nick", "run","run_port",
                                               "-history","-retry","-mem"};
    static constexpr int cmd_h = 0, cmd_help = 1, cmd_run = 4, cmd_config_nick = 2, cmd_nick = 3,cmd_run_port = 5,cmd_history=6,
    cmd_retry=7,cmd_mem=8;
    ~TranslateLauncher() override;
    int prioritySet(const char* argKey, size_t len)const noexcept override;

    int groupSet(const char *argKey, size_t len) const noexcept override;

protected:
    void ready(int argc, char **argv) override;
    void cleanup() override;
};

#endif //SOCKET_CLIENT_TRANSLATELAUNCHER_H
