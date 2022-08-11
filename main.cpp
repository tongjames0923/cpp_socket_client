
#include <iostream>
#include <Launcher.h>
#include "Application/App.h"
#include "Application/config.h"
using namespace std;



//NOTE:

int main(int args, char *argc[])
{
     Features::readConfig();
    try
    {
        printf("version:%s\n", VERSION);

#ifdef RELEASE_MODE
        Launcher* launcher;
        makeLauncher(&launcher);
        launcher->Start(args, argc);
#endif // !Debug
        
        return 1;
    }
    catch (std::exception &err)
    {
        std::cerr << err.what() << "\n";
        return 0;
    }

    return 1;
}


