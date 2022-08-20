
#include <iostream>
#include <Launcher.h>
#include "Application/App.h"
#include "Application/config.h"
#include "Application/Components/UserInterface.h"
#include <thread>

using namespace std;

int main(int args, char *argc[])
{
    Features::readConfig();
    readyForThread();
    try
    {
        printf("version:%s\t detail:%s\n", VERSION, SOCKET_IMPL);

#ifdef RELEASE_MODE
        Launcher *launcher;
        makeLauncher(&launcher);
        thread run([launcher,args,argc](){
            launcher->Start(args, argc);
        });
        run.detach();
        loop();

#endif // !Debug

        return 1;
    }
    catch (std::exception &err)
    {
        std::cerr << err.what() << "\n";
        return 0;
    }
}


