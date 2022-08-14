//
// Created by tbs on 2022/8/10.
//
#include "Application/App.h"
#include <cJSON.h>
#include <sstream>
#include "Application/Components/TranslateLauncher.h"
#include "Application/Components/LocalTranslator.h"
#include <memory>


#include <map>
#include <iostream>
#include <fstream>
#include <string>

TranslateLauncher launcher;

void makeLauncher(Launcher **launch)
{
    *launch = static_cast<Launcher *>(&launcher);
}

constexpr const unsigned SUCCESS=1,NOT_FOUND=2,INVALID_NEED=3,FAILED_TO_PICK=4;
int pickArg(int cmd,size_t need,std::vector<std::string>& args,Launcher *launcher)
{
    std::string key = TranslateLauncher::COMMAND[cmd];
    ArgInfo info;
    bool found = launcher->getArgInfo(key, &info);
    if (found)
    {
        if(info.length==need)
        {
            char tmp[1024];

            for(int i = 0; i<need; i++)
            {
                memset(tmp, 0, 1024);
                bool su= launcher->getData( launcher->getDefaultArgName(key, i), tmp, 1024);
                if (!su)
                    return FAILED_TO_PICK;
                else
                {
                    args.emplace_back(tmp);
                }
            }
            return SUCCESS;
        }
        else
            return INVALID_NEED;
    }
    else
    {
        return NOT_FOUND;
    }

    }

    namespace Features
    {

        using namespace std;

        bool runit();


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
            using namespace std;
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
        void forTool(function<bool(vector<string>&)>fun,int cmd,unsigned len,::Launcher *owner)
        {
            vector<string> args;
            unsigned r= pickArg(cmd,len,args,owner);
            bool d= false;
            switch (r)
            {
                case SUCCESS:
                    d= fun(args);
                    if (d)
                    cout<<"everything went fine!"<<endl;
                    else
                    {
                        cout<<"command runing failed!"<<endl;
                    }
                    break;
                case NOT_FOUND:
                    cout<<"not found the command!"<<endl;
                    break;
                case INVALID_NEED:
                    cout<<"wrong number of arguments!"<<endl;
                    break;
                case FAILED_TO_PICK:
                    cout<<"failed to pick the command args!"<<endl;
                    break;
                default:
                    cout<<"Unknown Error"<<endl;
                    break;
            }
        }

        void forConfigNick(::Launcher *owner)
        {
            forTool([](vector<string>& args)->bool{
                nickNames[args[0]]=args[1];
                outPutConfig();
                return true;
                },TranslateLauncher::cmd_config_nick,2,owner);
        }

        void forShowNick(Launcher *owner)
        {

            forTool([](vector<string>& args)->bool{
                for (auto i : nickNames)
                {
                    printf("nickName:%s\tip:%s\n", i.first.c_str(), i.second.c_str());
                }
                return true;
                },TranslateLauncher::cmd_nick,0,owner);

        }


        void forRun(Launcher *owner)
        {

            forTool([](vector<string>& args)->bool{
                filePath = args[1];
                ip = string(args[0]);
                if (nickNames.count(ip) > 0)
                    ip = nickNames[ip];
                return runit();
            },TranslateLauncher::cmd_run,2,owner);
            
        }

        void forH(Launcher *owner)
        {
            cout << "usage:socket_client run <ip|nickname> <filePath>  [send file to target with port 1997]\n";
            cout << "usage:socket_client run_port <ip|nickname> <port> <filePath> [send file to target"
                    "with specified port]\n";
            cout << "usage:socket_client -nick [to show all your nick names]" << endl;
            cout << "usage:socket_client -config_nick <nickname> <ip>  [to set nickname for ip]\n";
        }

        void forRun_port(Launcher *owner)
        {
            forTool([](vector<string>& args)->bool{
                try
                {
                    filePath = args[2];
                    ip = args[0];
                    if (nickNames.count(ip) > 0)
                        ip = nickNames[ip];
                    port = atoi(args[1].c_str());

                    return runit();
                }catch (...)
                {
                    return false;
                }
                },TranslateLauncher::cmd_run_port,3,owner);
        }


        bool runit()
        {
            cout << "init..." << endl;
            clock_t cost_time = clock();
            LocalTranslator translator(filePath.c_str(), ip.c_str(), port);
            fileTotal = translator.getTotalFileSize();
            translator.setOnSentFail([](char *pk, size_t should, size_t sent) -> bool
                                     {
                                         cout << "send function error" << endl;


                                         return false;
                                     });
            translator.setOnSentSuccess([&translator](char*pk, size_t should, size_t sent) -> bool
                                        {
                                            size_t hasSent = translator.getSent();
                                            float pec = (float) hasSent / fileTotal * 100.0;
                                            printf("\r%.2f", pec);
                                            cout << "%";
                                            return true;
                                        });
            cout << "connect..." << endl;
            bool connected = translator.readyForConnect();
            if (connected)
            {
                printf("file=%s\tfile_size=%.3lf kb \t ip=%s:%d\n", filePath.c_str(), fileTotal / 1024.0, ip.c_str(),
                       port);
                double totalsent = translator.runIt() / 1024.0;
                printf("\nhas sent ：%.3lf kb\n", totalsent);
                //cout << "\nhas sent:" << totalsent << "kb" << endl;
            } else
            {

                cout << "fail to connect" << endl;
                return false;
            }
            cost_time = clock() - cost_time;
            std::cout << "\ncost time: " << cost_time / (double) CLOCKS_PER_SEC << " s ";
            double speed = (double)fileTotal/
                    (double )cost_time/CLOCKS_PER_SEC/1024.0/1024.0;
            cout << "Speed:" << speed << " MB/s" << endl;

            return true;
        }


    }
