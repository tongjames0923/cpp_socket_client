
#include <iostream>
#include <Launcher.h>

#include "Application/App.h"
#include "Application/config.h"
#include "Application/Components/UserInterface.h"
#include <thread>
#include <map>

using namespace std;


int main(int args, char *argc[])
{
#ifdef WIN32
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);
#endif
    Features::readConfig();
    Features::readRecord();
    UI::readyForThread();
    try
    {
        printf("version:%s\t detail:%s\n", VERSION, SOCKET_IMPL);

#ifdef RELEASE_MODE
        Launcher *launcher;
        makeLauncher(&launcher);
        thread run([launcher, args, argc]()
                   {
        	this_thread::sleep_for(std::chrono::milliseconds(100));
                       try
                       {
                           launcher->Start(args, argc);
                       }
                       catch (const std::exception &err)
                       {
                           UI::printText(err.what());
                           UI::shutdown_WhenEmpty();
                       }

                   });
        run.detach();
        UI::loop();
        cout << "\nbye~" << endl;

#endif

        return 1;
    }
    catch (std::exception &err)
    {
        std::cerr << err.what() << "\n";
        return 0;
    }
}


