//
// Created by tbs on 2022/8/12.
//

#include <Application/Components/TranslateLauncher.h>
#include "Application/App.h"

void TranslateLauncher::ready(int argc, char **argv)
{
    using namespace Features;
    setArgFunction(COMMAND[0], forH);
    setArgFunction(COMMAND[1], forH);
    setArgFunction(COMMAND[2], forConfigNick);
    setArgFunction(COMMAND[3], forShowNick);
    setArgFunction(COMMAND[4], forRun);
    setArgFunction(COMMAND[5], forRun_port);
}

TranslateLauncher::~TranslateLauncher() = default;

int TranslateLauncher::prioritySet(const char *argKey, size_t len) const noexcept
{
    int pri[] = {0, 0, 1, 2, 3, 3};
    for (int i = 0; i < 6; ++i)
    {
        if (strcmp(COMMAND[i], argKey) == 0)
            return pri[i];
    }
    return 0;
}

int TranslateLauncher::groupSet(const char *argKey, size_t len) const noexcept
{
    int grp[] = {0, 0, 1, 1, 2, 3};
    for (int i = 0; i < 6; ++i)
    {
        if (strcmp(COMMAND[i], argKey) == 0)
            return grp[i];
    }
    return 0;
}
