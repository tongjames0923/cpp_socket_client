#include <cstdio>
#include <cstring>
#include <iostream>
#include "LocalTranslator.h"
#include <stdio.h>
#include "cJSON.h"
#include <map>


using namespace std;

static const size_t Q_LEN = 32;

string filePath = "C:\\Users\\Abstergo\\Desktop\\test.zip";
size_t fileTotal;
unsigned port = 1997;
string ip = "127.0.0.1";
string config_file = "config.txt";
map<string, string> nickNames;
cJSON* root;
string config_NICKNAME = "nickname";
string config_NICKNAME_NICK = "nick";
string config_NICKNAME_IP = "ip";
void readConfig()
{
    try
    {
        char bf[512];
        ifstream s(config_file, ios_base::in);

        stringstream str;
        while (!s.eof())
        {
            s >> bf;
            str << bf;
        }
        string json =
            str.str();
        root = cJSON_Parse(json.c_str());
        cJSON* nick = cJSON_GetObjectItem(root, config_NICKNAME.c_str());
        size_t arr = cJSON_GetArraySize(nick);
        for (int i = 0; i < arr; i++)
        {
            cJSON* item = cJSON_GetArrayItem(nick, i);
            if (item != NULL)
            {
                string k(cJSON_GetObjectItem(item, config_NICKNAME_NICK.c_str())->valuestring);
                string v(cJSON_GetObjectItem(item, config_NICKNAME_IP.c_str())->valuestring);
                nickNames[k] = v;
            }
        }
    }
    catch (exception& ex)
    {
        cout << ex.what() << endl;

    }



}

int main(int args, char *argc[])
{
    readConfig();
    try
    {
        printf("version:%s\n", VERSION);
        cout << "init..." << endl;
        clock_t cost_time = clock();
#ifdef RELEASE_MODE
        if (args == 3)
        {
            filePath = string(argc[2]);
            ip = string(argc[1]);
            if (nickNames.count(ip) > 0)
                ip = nickNames[ip];


        } else if (args == 4)
        {
            ip = string(argc[1]);
            if (nickNames.count(ip) > 0)
                ip = nickNames[ip];
            filePath = string(argc[3]);
            port = atoi(argc[2]);
        } else
        {
            cout << "usage socket_client <target_ip/nickName> <file_Location> or \n" <<
                 "usage socket_client <target_ip/nickName> <port> <file_Location> or \n"

                 << endl;
            return 0;
        }
#endif // !Debug
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
            return 0;
        }
        cost_time = clock() - cost_time;
        std::cout << "\ncost time: " << cost_time / (double) CLOCKS_PER_SEC << " s ";
        double speed = fileTotal * (CLOCKS_PER_SEC / 1024.0 / 1024.0) /
                       cost_time;
        cout << "Speed:" << speed << " MB/s" << endl;
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