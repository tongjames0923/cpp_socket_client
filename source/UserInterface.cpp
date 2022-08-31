//
// Created by tbs on 2022/8/20.
//

#include "Application/Components/UserInterface.h"
#include "Handler/Loop.h"
#include "Handler/Message.h"
#include "Handler/Handler.h"
#include <memory>
#include <iostream>
#include <atomic>
#include "Handler/Handler_Config.h"
#include <cstring>

using namespace std;
namespace UI
{
    unique_ptr<Handler> _ui_handler;
    constexpr const int code_print_text = 0,code_print_progress = 1,code_shutdown=2,code_delay_shutdown=3,code_in_progress=4,code_out_progress=5;
    constexpr const char _done='*',_undone='-';
    atomic_bool _progressing;
    struct ProgressData
    {
        char text[512];
        double progress;
    };

    void readyForThread()
    {
        if (!isReady())
        {
            ready();
            _progressing= false;
            _ui_handler.reset(new Handler());
            _ui_handler->setHandle([](Message& message){
                size_t  len=message.getArg1();

                switch (message.getCode())
                {
                    case code_print_text:
                        if(!_progressing)
                        {
                            if (len>=1024)
                                throw std::runtime_error("Invalid message length,max length is 1024");
                            char text[1024];
                            message.getData(text,len);
                            text[len]=0;
                            printf("%s",text);
                            fflush(stdout);
                        }
                        else
                        {
                            _ui_handler->sendMsg(message,1);
                        }

                        //cout.flush();
                        break;
                    case code_print_progress:
                        if(_progressing)
                        {
                            ProgressData data;
                            message.getData(&data);
                            printf("[");
                            for(int i=0;i<data.progress*len;++i)
                            {
                                printf("%c",_done);
                            }
                            for(int i=data.progress*len;i<len;++i)
                            {
                                printf("%c",_undone);
                            }
                            printf("] ");
                            printf("%s\r",data.text);
                            fflush(stdout);
                        }
                        else
                            _ui_handler->sendMsg(message,1);
                        break;
                    case code_shutdown:
                        _ui_handler->getLoop()->stopLoop();
                        break;
                    case code_delay_shutdown:
                        _ui_handler->getLoop()->stopLoop_util_empty();
                        break;
                }
            });
        }
    }

    void printText(const string& str)
    {
        if (_ui_handler)
        {
            auto now= Tool::getTimeForNow();
            Message msg=Message(code_print_text);
            msg.setArg1(str.length());
            msg.setData(str.data(),str.length());
            _ui_handler->sendMsgAt(std::move(msg),now);
            this_thread::sleep_for(chrono::milliseconds(1));//使得输出较为有序
        }
    }

    void printProgress(size_t progressLen, double progress,const string& append)
    {
        if (_ui_handler)
        {
            auto now= Tool::getTimeForNow();
            Message msg=Message(code_print_progress);
            msg.setArg1(progressLen);
            ProgressData data;
            data.progress = progress;
            memcpy(data.text,append.c_str(),append.length());
            data.text[append.length()+1]=0;
            msg.setData(data);
            _ui_handler->sendMsgAt(std::move(msg),now);
            this_thread::sleep_for(chrono::milliseconds(1));//使得输出较为有序
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
            _ui_handler->sendMsg(Message(code_shutdown),0);
        }
    }

    void shutdown_WhenEmpty()
    {
        if (_ui_handler&&_ui_handler->getLoop()->isRunning())
            _ui_handler->sendMsg(Message(code_delay_shutdown), 100);
    }

//note 不确定这样的操作是否有益于输出顺序
    void progressing()
    {
        _progressing= true;
    }

    void outProgressing()
    {
        _progressing=false;
    }

}
