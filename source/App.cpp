//
// Created by tbs on 2022/8/10.
//

#include "Application/Component.h"
#include "Application/App.h"
#include "Launcher.h"
TranslateLauncher launcher;
void makeLauncher(Launcher **launch)
{
    *launch = static_cast<Launcher *>(&launcher);
}