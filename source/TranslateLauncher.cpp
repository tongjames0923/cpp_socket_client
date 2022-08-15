//
// Created by tbs on 2022/8/12.
//

#include <Application/Components/TranslateLauncher.h>
#include "Application/App.h"
void TranslateLauncher::ready(int argc, char** argv)
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