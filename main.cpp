#include <cstdio>
#include <cstring>
#include <iostream>
#include "LocalTranslator.h"
#include <stdio.h>
#include "cJSON.h"
#include <map>
#include "Launcher.h"


using namespace std;

static const size_t Q_LEN = 32;

string filePath = "C:\\Users\\Abstergo\\Desktop\\test.zip";
size_t fileTotal;
unsigned port = 1997;
string ip = "127.0.0.1";
string config_file = "config.txt";
map<string, string> nickNames;
cJSON *root;
string config_NICKNAME = "nickname";
string config_NICKNAME_NICK = "nick";
string config_NICKNAME_IP = "ip";

void outPutConfig()
{
    cJSON *root = cJSON_CreateObject();
    cJSON *nick = cJSON_AddArrayToObject(root, config_NICKNAME.c_str());
    for (auto i :nickNames)
    {
        cJSON *obj = cJSON_CreateObject();
        cJSON_AddStringToObject(obj, config_NICKNAME_NICK.c_str(), i.first.c_str());
        cJSON_AddStringToObject(obj, config_NICKNAME_IP.c_str(), i.second.c_str());
        cJSON_AddItemToArray(nick, obj);
    }
    ofstream out(config_file.c_str());
    out << cJSON_Print(root);
    out.close();
    cJSON_free(root);
}

void readConfig()
{
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


}

void forH(Launcher *owner);

void forConfigNick(Launcher *owner);

void forShowNick(Launcher *owner);

void forRun(Launcher *owner);

class TranslateLauncher : public virtual Launcher
{
public:
    static constexpr char *COMMAND[] = {"-h", "--help", "-config_nick", "-nick", "run"};
    static constexpr int cmd_h = 0, cmd_help = 1, cmd_run = 4, cmd_config_nick = 2, cmd_nick = 3;
private:

protected:
    void ready(int argc, char **argv) override
    {
        setArgFunction(COMMAND[0], forH);
        setArgFunction(COMMAND[1], forH);
        setArgFunction(COMMAND[2], forConfigNick);
        setArgFunction(COMMAND[3], forShowNick);
        setArgFunction(COMMAND[4], forRun);
    }
};


int main(int args, char *argc[])
{
    readConfig();
    try
    {
        printf("version:%s\n", VERSION);

#ifdef RELEASE_MODE
        TranslateLauncher launcher;
        launcher.Start(args,argc);
#endif // !Debug
        if (root != NULL)
        {
            cJSON_free(root);
            root = NULL;
        }
        return 1;

    }
    catch (std::exception &err)
    {
        std::cerr << err.what() << "\n";
        return 0;
    }

    return 1;
}

void forConfigNick(Launcher *owner)
{
    string key = TranslateLauncher::COMMAND[TranslateLauncher::cmd_config_nick];
    ArgInfo info;
    bool found = owner->getArgInfo(key, &info);
    if (found)
    {
        if (info.length == 2)
        {
            char nick[256], _ip[256];
            owner->getData(owner->getDefaultArgName(key, 0), nick, 256);
            owner->getData(owner->getDefaultArgName(key, 1),_ip, 256);
            nickNames[nick] = ip;
            outPutConfig();
            return;
        }
    }
    cout << "ERROR: Unknown command for config to nick" << endl;
}

void forShowNick(Launcher *owner)
{
    string key = TranslateLauncher::COMMAND[TranslateLauncher::cmd_nick];
    ArgInfo info;
    bool found = owner->getArgInfo(key, &info);
    if (found)
    {
        if (info.length == 0)
        {
            for (auto i : nickNames)
            {
                printf("nickName:%s\tip:%s\n", i.first.c_str(), i.second.c_str());
            }
            return;
        }
    }
    cout << "ERROR: Unknown command for show nick" << endl;
    return;

}
void runit()
{
    cout << "init..." << endl;
    clock_t cost_time = clock();
    LocalTranslator translator(filePath.c_str(), ip.c_str(), port, Q_LEN);
    fileTotal = translator.getTotalFileSize();
    size_t hasSent = 0;
    translator.setOnSentFail([](PackData<char> *pk, size_t should, size_t sent) -> bool
                             {
                                 cout << "send function error" << endl;


                                 return false;
                             });
    translator.setOnSentSuccess([&hasSent](PackData<char> *pk, size_t should, size_t sent) -> bool
                                {
                                    hasSent += sent;
                                    float pec = (float) hasSent / fileTotal * 100;
                                    printf("\r%.2f", pec);
                                    cout << "%";
                                    return true;
                                });
    cout << "connect..." << endl;
    bool connected = translator.readyForConnect();
    if (connected)
    {
        printf("file=%s\tfile_size=%.2f kb \t ip=%s:%d\n", filePath.c_str(), fileTotal / 1024.0, ip.c_str(), port);
        translator.runIt();

    } else
    {

        cout << "fail to connect" << endl;
        return;
    }
    cost_time = clock() - cost_time;
    std::cout << "\ncost time: " << cost_time / (double) CLOCKS_PER_SEC << " s ";
    double speed = fileTotal * (CLOCKS_PER_SEC / 1024.0 / 1024.0) /
                   cost_time;
    cout << "Speed:" << speed << " MB/s" << endl;

    return;
}

void forRun(Launcher *owner)
{
    string key = TranslateLauncher::COMMAND[TranslateLauncher::cmd_run];
    ArgInfo info;
    bool found = owner->getArgInfo(key, &info);
    if (found)
    {
        if (info.length == 2)
        {
            char target[256], _ip[256];
            owner->getData(owner->getDefaultArgName(key, 0), target, 256);
            owner->getData(owner->getDefaultArgName(key, 1), _ip, 256);

            filePath = string(target);
            ip = string(ip);
            if (nickNames.count(ip) > 0)
                ip = nickNames[ip];
            runit();
            return;
        }
        else if(info.length==3)
        {
            char target[256], _ip[256],_port[256];
            owner->getData(owner->getDefaultArgName(key, 0), target, 256);
            owner->getData(owner->getDefaultArgName(key, 1), _ip, 256);
            owner->getData(owner->getDefaultArgName(key, 2), _port, 256);
            filePath = string(target);
            ip = string(ip);
            if (nickNames.count(ip) > 0)
                ip = nickNames[ip];
            port= atoi(_port);
            runit();
            return;
        }
    }
    cout << "ERROR: Unknown command run" << endl;
    return;
}

void forH(Launcher *owner)
{

}