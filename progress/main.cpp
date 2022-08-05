//
// Created by tbs on 2022/8/2.
//
#include "progress.h"
#include <iostream>
#include <thread>
using namespace std;
int main()
{
    int z=5,t=0;
    consoleProgress::setLen(20);
    consoleProgress::progress([&z,&t]{
        while (t<20)
        {
            this_thread::sleep_for(chrono::milliseconds(500));
            t++;
            consoleProgress::pushProgress(t/20.0*20.0);
            if(--z<0)
            {
                consoleProgress::finish();
                break;
            }

        }


    });
    return  0;
}