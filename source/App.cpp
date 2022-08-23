//
// Created by tbs on 2022/8/10.
//
#include "Application/App.h"
#include <cJSON.h>
#include <sstream>
#include "Application/Components/TranslateLauncher.h"
#include "Application/Components/LocalTranslator.h"
#include <memory>
#include "Backend/imp_Launcher.hpp"

#include <vector>
#include <map>
#include <queue>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <chrono>
#include "Application/Components/UserInterface.h"
#include <boost/format.hpp>
#include <atomic>
#include <cmath>

static TranslateLauncher launcher;
using boost::format;

void makeLauncher(Launcher **launch)
{
    *launch = static_cast<Launcher *>(&launcher);
}

constexpr const unsigned SUCCESS = 1, NOT_FOUND = 2, INVALID_NEED = 3, FAILED_TO_PICK = 4;

int pickArg(int cmd, int need, std::vector<std::string> &args, Launcher *launcher)
{
    std::string key(TranslateLauncher::COMMAND[cmd]);
    ArgInfo info;
    bool found = launcher->getArgInfo(key, &info);
    if (found)
    {
        if (info.length == need)
        {
            char tmp[1024];

            for (int i = 0; i < need; i++)
            {
                memset(tmp, 0, 1024);
                bool su = launcher->getData(launcher->getDefaultArgName(key, i), tmp, 1024);
                if (!su)
                    return FAILED_TO_PICK;
                else
                {
                    args.emplace_back(tmp);
                }
            }
            return SUCCESS;
        } else if (need == -1)
        {
            char tmp[1024];

            for (int i = 0; i < info.length; i++)
            {
                memset(tmp, 0, 1024);
                bool su = launcher->getData(launcher->getDefaultArgName(key, i), tmp, 1024);
                if (!su)
                    return FAILED_TO_PICK;
                else
                {
                    args.emplace_back(tmp);
                }
            }
            return SUCCESS;
        } else
            return INVALID_NEED;
    } else
    {
        return NOT_FOUND;
    }

}

namespace Features
{

    using namespace std;

    bool runit(LocalTranslator &translator);


    string filePath = "C:\\Users\\Abstergo\\Desktop\\test.zip";
    size_t fileTotal;
    unsigned port = 1997;
    string ip = "127.0.0.1";
    string config_file = "config.txt";
    map<string, string> nickNames;

    string config_NICKNAME = "nickname";
    string config_NICKNAME_NICK = "nick";
    string config_NICKNAME_IP = "ip";


    void outPutConfig()
    {
        cJSON *root = cJSON_CreateObject();
        cJSON *nick = cJSON_AddArrayToObject(root, config_NICKNAME.c_str());
        for (auto i : nickNames)
        {
            cJSON *obj = cJSON_CreateObject();
            cJSON_AddStringToObject(obj, config_NICKNAME_NICK.c_str(), i.first.c_str());
            cJSON_AddStringToObject(obj, config_NICKNAME_IP.c_str(), i.second.c_str());
            cJSON_AddItemToArray(nick, obj);
        }
        std::ofstream out(config_file.c_str());
        out << cJSON_Print(root);
        out.close();
        cJSON_free(root);
    }

    void readConfig()
    {
        cJSON *root = NULL;
        try
        {
            char bf[512];
            ifstream s(config_file, ios_base::in);
            if (!s.is_open())
                return;
            stringstream str;
            while (!s.eof())
            {
                s >> bf;
                str << bf;
            }
            string json =
                    str.str();

            root = cJSON_Parse(json.c_str());
            cJSON *nick = cJSON_GetObjectItem(root, config_NICKNAME.c_str());
            size_t arr = cJSON_GetArraySize(nick);
            for (int i = 0; i < arr; i++)
            {
                cJSON *item = cJSON_GetArrayItem(nick, i);
                if (item != NULL)
                {
                    string k(cJSON_GetObjectItem(item, config_NICKNAME_NICK.c_str())->valuestring);
                    string v(cJSON_GetObjectItem(item, config_NICKNAME_IP.c_str())->valuestring);
                    nickNames[k] = v;
                }
            }

        }
        catch (exception &ex)
        {
            cout << ex.what() << endl;
        }
        if (root != NULL)
        {
            cJSON_free(root);
            root = NULL;
        }
    }

    void forTool(function<bool(vector<string> &)> fun, int cmd, int len, ::Launcher *owner)
    {
        vector<string> args;
        unsigned r = pickArg(cmd, len, args, owner);
        bool d = false;
        switch (r)
        {
            case SUCCESS:
                d = fun(args);
                if (d)
                    UI::printText("everything went fine!\n");
                    //cout << "everything went fine!" << endl;
                else
                {
                    UI::printText("command runing failed!\n");
                    //cout << "command runing failed!" << endl;
                }
                break;
            case NOT_FOUND:
                UI::printText("not found the command!\n");
                // cout << "not found the command!" << endl;
                break;
            case INVALID_NEED:
                UI::printText("wrong number of arguments!\n");
                //cout << "wrong number of arguments!" << endl;
                break;
            case FAILED_TO_PICK:
                UI::printText("failed to pick the command args!\n");
                //cout << "failed to pick the command args!" << endl;
                break;
            default:
                UI::printText("Unknown Error\n");
                //cout << "Unknown Error" << endl;
                break;
        }
    }

    void forConfigNick(::Launcher *owner)
    {
        forTool([](vector<string> &args) -> bool
                {
                    nickNames[args[0]] = args[1];
                    outPutConfig();
                    return true;
                }, TranslateLauncher::cmd_config_nick, 2, owner);
    }

    void forShowNick(Launcher *owner)
    {

        forTool([](vector<string> &args) -> bool
                {
                    for (auto i : nickNames)
                    {
                        string text = "nickName:" + i.first + "\tip:" + i.second + "\n";
                        UI::printText(text);
                        //printf("nickName:%s\tip:%s\n", i.first.c_str(), i.second.c_str());
                    }
                    return true;
                }, TranslateLauncher::cmd_nick, 0, owner);

    }


    void forRun(Launcher *owner)
    {

        forTool([](vector<string> &args) -> bool
                {
                    if (args.size() < 2)
                    {
                        UI::printText("wrong number of arguments!\n");
                        return false;
                    }

                    ip = string(args[0]);
                    if (nickNames.count(ip) > 0)
                        ip = nickNames[ip];
                    deque<LocalTranslator> translators;
                    for (int i = 1; i < args.size(); i++)
                    {
                        translators.emplace_back(std::move(LocalTranslator(args[i].c_str(), ip.c_str(), port)));
                    }
                    mutex ready_mut;
                    unique_lock<mutex> lk(ready_mut, defer_lock);
                    condition_variable ready_control;
                    int ready_count = -1;
                    //int now_count=0;
                    thread ready_data([&translators, &ready_count, &ready_control]()
                                      {
                                          mutex tp_lock;
                                          for (LocalTranslator &lt:translators)
                                          {
                                              {
                                                  lock_guard<mutex> lg(tp_lock);
                                                  lt.prepareData();
                                                  ++ready_count;
                                                  ready_control.notify_one();
                                              }
                                          }
                                      });
                    ready_data.detach();
                    for (int i = 0; i < translators.size(); ++i)
                    {
                        lk.lock();
                        ready_control.wait(lk, [&ready_count, &i]()
                        {
                            return ready_count >= i;
                        });
                        lk.unlock();
                        if(runit(translators[i]))
                        {
                            UI::printText("send success\n\n");
                        }
                        else
                        {
                            UI::printText("send fail\n\n");
                        }
                        this_thread::sleep_for(chrono::milliseconds(200));
                    }
                    return true;
                }, TranslateLauncher::cmd_run, -1, owner);

    }

    void forH(Launcher *owner)
    {
        string text =
                string("usage:socket_client run <ip|nickname> <filePath>  [send file to target with port 1997]\n") +
                "usage:socket_client run_port <ip|nickname> <port> <filePath> [send file to target" +
                "with specified port]\n" +
                "usage:socket_client -nick [to show all your nick names]\n" +
                "usage:socket_client -config_nick <nickname> <ip>  [to set nickname for ip]\n";
        UI::printText(text);
    }

    void forRun_port(Launcher *owner)
    {
        forTool([](vector<string> &args) -> bool
                {
                    try
                    {
                        filePath = args[2];
                        ip = args[0];
                        if (nickNames.count(ip) > 0)
                            ip = nickNames[ip];
                        port = atoi(args[1].c_str());
                        //NOTE done for it
                        return false;
                    }
                    catch (...)
                    {
                        return false;
                    }
                }, TranslateLauncher::cmd_run_port, 3, owner);
    }

    std::mutex finish_mut;
    condition_variable finish_control;

    chrono::steady_clock::time_point cost_time;

    void readingAction(LocalTranslator *owner)
    {
        UI::printText("reading file...\n");
    }


    void sendingAction(LocalTranslator *owner)
    {
        UI::printText("sending...\n");
        cost_time = std::chrono::steady_clock::now();
        thread td([owner]()
                  {
                      size_t sent = 0;
                      size_t old = 0;
                      double speed = 0;
                      bool done = false;
                      std::unique_lock<mutex> lk(finish_mut, defer_lock);
                      while (!done)
                      {
                          lk.lock();
                          cv_status status = finish_control.wait_for(lk,
                                                                     chrono::milliseconds(
                                                                             500));
                          if (status == cv_status::no_timeout)
                          {
                              done = true;
                          }
                          sent = owner->getSent();
                          if (sent == 0)
                              continue;
                          speed = (sent - old) / 1024.0 / 1024.0 * 2;
                          old = sent;

                          double persent = ((double) sent / fileTotal);
                          UI::printProgress(25, persent,
                                            (format("%.2f%% . has sent %zu bytes\tspeed:%.2f mb/s") %
                                             (persent * 100) %
                                             sent % speed).str());
                          lk.unlock();
                      }
                      //cout << "\r100%" << endl;
                  });
        td.detach();
    }

    void finishAction(LocalTranslator *owner)
    {
        finish_control.notify_all();
        auto edtime = std::chrono::steady_clock::now();
        double cost = chrono::duration<double>(edtime - cost_time).count();
        size_t sent = owner->getSent();
        double times = round(cost * 100.0) / 100.0;
        double mbs = round(1024.0 * 1024.0 * 100.0) / 100.0;
        double speed = round((double) fileTotal / mbs
                             / times * 100.0) / 100.0;
        lock_guard<mutex> gard(finish_mut);
        {
            UI::printText(
                    (format("\nhas sent %.2f kb\ncost time :%.2f s\nspeed :%.2f mb/s\n") %
                     (sent / 1024.0f) % cost % speed).str());
        }
    }

    bool runit(LocalTranslator &translator)
    {
        UI::printText("init...\n");

        fileTotal = translator.getTotalFileSize();
        UI::printText("connect...\n");
        bool connected = translator.Connect();

        if (connected)
        {
            UI::printText(
                    (format("file=%s\tfile_size=%.3lf kb \t ip=%s:%d\n") % filePath.c_str() % (fileTotal / 1024.0) %
                     ip.c_str() % port).str());
            if (!translator.hasPrepared())
            {
                translator.runIt(readingAction, sendingAction, finishAction);
            } else
            {
//                readingAction(&translator);
//                translator.prepareData();
                sendingAction(&translator);
                translator.sendPreparedData();
                finishAction(&translator);
            }

        } else
        {

            UI::printText("fail to connect\n");
            return false;
        }

        //cout << "Speed:" << speed << " MB/s" << endl;

        return true;
    }


}
