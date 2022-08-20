//
// Created by tbs on 2022/8/20.
//

#include "Application/Components/UserInterface.h"
#include "Handler/Loop.h"
#include "Handler/Message.h"
#include "Handler/Handler.h"
#include <memory>
#include <iostream>

using namespace std;
unique_ptr<Handler> _ui_handler;
constexpr const int code_print_text = 0,code_print_progress = 1;
constexpr const char _done='*',_undone='-';
struct ProgressData
{
    char text[1024];
    double progress;
};

void readyForThread()
{
    if (!isReady())
    {
        ready();
        _ui_handler.reset(new Handler([](Message& message){
            size_t  len=message.getArg1();

            switch (message.getCode())
            {
                case code_print_text:
                    if (len>=1024)
                        throw std::runtime_error("Invalid message length,max length is 1024");
                    char text[1024];
                    message.getData(text,len);
                    text[len]=0;
                    cout<<text;
                    cout.flush();
                    break;
                case code_print_progress:
                   ProgressData data;
                    message.getData(&data);
                    printf("\r[");
                    for(int i=0;i<data.progress*len;++i)
                    {
                        printf("%c",_done);
                    }
                    for(int i=data.progress*len;i<len;++i)
                    {
                        printf("%c",_undone);
                    }
                    printf("] ");
                    printf("%s",data.text);
                    fflush(stdout);
                    break;
            }
        }));
    }
}

void printText(const string& str)
{
    if (_ui_handler)
    {
        Message msg=Message(code_print_text);
        msg.setArg1(str.length());
        msg.setData(str.data(),str.length());
        _ui_handler->sendMsg(msg,0);
    }
}

void printProgress(size_t progressLen, double progress,const string& append)
{
    if (_ui_handler)
    {
        Message msg=Message(code_print_progress);
        msg.setArg1(progressLen);
        ProgressData data{.progress=progress};
        memcpy(data.text,append.c_str(),append.length());
        data.text[append.length()+1]=0;
        msg.setData(data);
        _ui_handler->sendMsg(msg,0);
    }
}

void loop()
{
    readyForThread();

    getMyLoop().loop();
}

void shutdown()
{
    if (_ui_handler&&_ui_handler->getLoop()->isRunning())
    {
        _ui_handler->getLoop()->stopLoop_util_empty();
    }
}
