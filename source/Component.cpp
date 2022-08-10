//
// Created by tbs on 2022/8/10.
//

#include "Application/Component.h"
#include <Application/App.h>

void TranslateLauncher::ready(int argc, char **argv)
{
    using namespace Features;
    setArgFunction(COMMAND[0], forH);
    setArgFunction(COMMAND[1], forH);
    setArgFunction(COMMAND[2], forConfigNick);
    setArgFunction(COMMAND[3], forShowNick);
    setArgFunction(COMMAND[4], forRun);
}


