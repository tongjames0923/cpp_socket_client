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
#include <cmath>
#include <TranslateRecorder.h>
#include "Error.hpp"
#include <condition_variable>

static TranslateLauncher launcher;
using boost::format;

void makeLauncher(Launcher **launch) {
    *launch = static_cast<Launcher *>(&launcher);
}

constexpr const unsigned SUCCESS = 1, NOT_FOUND = 2, INVALID_NEED = 3, FAILED_TO_PICK = 4;


int pickArg(int cmd, int need, std::vector<std::string> &args, Launcher *launcher) {
    std::string key(COMMAND[cmd]);
    ArgInfo info;
    bool found = launcher->getArgInfo(key, &info);
    if (found) {
        if (info.length == need) {
            char tmp[1024];

            for (int i = 0; i < need; i++) {
                memset(tmp, 0, 1024);
                bool su = launcher->getData(launcher->getDefaultArgName(key, i), tmp, 1024);
                if (!su)
                    return FAILED_TO_PICK;
                else {
                    args.emplace_back(tmp);
                }
            }
            return SUCCESS;
        } else if (need == -1) {
            char tmp[1024];

            for (int i = 0; i < info.length; i++) {
                memset(tmp, 0, 1024);
                bool su = launcher->getData(launcher->getDefaultArgName(key, i), tmp, 1024);
                if (!su)
                    return FAILED_TO_PICK;
                else {
                    args.emplace_back(tmp);
                }
            }
            return SUCCESS;
        } else
            return INVALID_NEED;
    } else {
        return NOT_FOUND;
    }

}

namespace Features {

    using namespace std;


    std::mutex finish_mut;
    condition_variable finish_control;
    bool need_rem = false;

    chrono::steady_clock::time_point cost_time;

    bool runit(int ids, LocalTranslator &translator);

    string filePath = "C:\\Users\\Abstergo\\Desktop\\test.zip";
    size_t fileTotal;
    static unsigned port = 1997;
    static string ip = "127.0.0.1";
    static const string config_file = "config.txt";
    static map<string, string> nickNames;
    static const string config_NICKNAME = "nickname";
    static const string config_NICKNAME_NICK = "nick";
    static const string config_NICKNAME_IP = "ip";
    static const string record_file = "records.txt";
    TranslateRecorder recorder;


    static string readJsonFromFile(string file) {
        char bf[512];
        ifstream s(file, ios_base::in);
        if (!s.is_open())
            return "";
        stringstream str;
        while (!s.eof()) {
            s >> bf;
            str << bf;
        }
        string content = str.str();
        s.close();
        return content;
    }

    static bool writeToFile(string content, string file) {
        try {
            std::ofstream out(file.c_str());
            if (!out.is_open())
                return false;
            out << content;
            out.close();
            return true;
        } catch (...) {
            return false;
        }
    }

    void outPutConfig() {
        cJSON *root = cJSON_CreateObject();
        cJSON *nick = cJSON_AddArrayToObject(root, config_NICKNAME.c_str());
        for (auto i : nickNames) {
            cJSON *obj = cJSON_CreateObject();
            cJSON_AddStringToObject(obj, config_NICKNAME_NICK.c_str(), i.first.c_str());
            cJSON_AddStringToObject(obj, config_NICKNAME_IP.c_str(), i.second.c_str());
            cJSON_AddItemToArray(nick, obj);
        }
        writeToFile(cJSON_Print(root), config_file);
        cJSON_free(root);
    }

    void readConfig() {
        cJSON *root = NULL;
        try {
            string json = readJsonFromFile(config_file);

            root = cJSON_Parse(json.c_str());
            cJSON *nick = cJSON_GetObjectItem(root, config_NICKNAME.c_str());
            size_t arr = cJSON_GetArraySize(nick);
            for (int i = 0; i < arr; i++) {
                cJSON *item = cJSON_GetArrayItem(nick, i);
                if (item != NULL) {
                    string k(cJSON_GetObjectItem(item, config_NICKNAME_NICK.c_str())->valuestring);
                    string v(cJSON_GetObjectItem(item, config_NICKNAME_IP.c_str())->valuestring);
                    nickNames[k] = v;
                }
            }

        }
        catch (exception &ex) {
            cout << ex.what() << endl;
        }
        if (root != NULL) {
            cJSON_free(root);
            root = NULL;
        }
    }

    void forTool(function<bool(vector<string> &)> fun, int cmd, int len, ::Launcher *owner) {
        vector<string> args;
        unsigned r = pickArg(cmd, len, args, owner);
        bool d = false;
        switch (r) {
            case SUCCESS:
                d = fun(args);
                if (d)
                    UI::printText("everything went fine!\n");
                    //cout << "everything went fine!" << endl;
                else {
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

    void forConfigNick(::Launcher *owner) {
        forTool([](vector<string> &args) -> bool {
            nickNames[args[0]] = args[1];
            outPutConfig();
            return true;
        },cmd_config_nick, 2, owner);
    }

    void forShowNick(Launcher *owner) {

        forTool([](vector<string> &args) -> bool {
            for (auto i : nickNames) {
                string text = "nickName:" + i.first + "\tip:" + i.second + "\n";
                UI::printText(text);
                //printf("nickName:%s\tip:%s\n", i.first.c_str(), i.second.c_str());
            }
            return true;
        }, cmd_nick, 0, owner);

    }


    bool multiRun(deque<LocalTranslator> &translators) {
        mutex ready_mut;
        unique_lock<mutex> lk(ready_mut, defer_lock);
        condition_variable ready_control;
        int ready_count = -1;
        //int now_count=0;
        vector<int> ids;
        for (LocalTranslator &lc:translators) {
            ids.push_back(recorder.pushRecord(&lc));
        }
        outputRecord();
        thread ready_data([&translators, &ready_count, &ready_control]() {
            mutex tp_lock;
            for (LocalTranslator &lt:translators) {

                {
                    lock_guard<mutex> lg(tp_lock);
                    lt.prepareData();
                    ++ready_count;
                }
                ready_control.notify_one();
            }
        });
        ready_data.detach();
        bool isok = true;
        for (int i = 0; i < translators.size(); ++i) {
            lk.lock();
            ready_control.wait(lk, [&ready_count, &i]() {
                return ready_count >= i;
            });
            lk.unlock();
            if (runit(ids[i], translators[i])) {
                UI::printText("send success\n\n");
                recorder.turnToDone(ids[i]);
                outputRecord();
            } else {
                UI::printText("send fail\n\n");
                isok = false;
            }
            this_thread::sleep_for(chrono::milliseconds(100));
        }
        return isok;
    }


    void forRun(Launcher *owner) {

        forTool([](vector<string> &args) -> bool {
            if (args.size() < 2) {
                UI::printText("wrong number of arguments!\n");
                return false;
            }

            ip = string(args[0]);
            if (nickNames.count(ip) > 0)
                ip = nickNames[ip];
            deque<LocalTranslator> translators;
            for (int i = 1; i < args.size(); i++) {
                translators.emplace_back(std::move(LocalTranslator(args[i].c_str(), ip.c_str(), port)));
            }

            return multiRun(translators);
        }, cmd_run, -1, owner);

    }

    void forH(Launcher *owner) {
        string text =
                string("usage:socket_client run <ip|nickname> <filePath>  [send file to target with port 1997]\n") +
                "usage:socket_client run_port <ip|nickname> <port> <filePath> [send file to target" +
                "with specified port]\n" +
                "usage:socket_client -nick [to show all your nick names]\n" +
                "usage:socket_client -config_nick <nickname> <ip>  [to set nickname for ip]\n";
        UI::printText(text);
    }

    void forRun_port(Launcher *owner) {
        forTool([](vector<string> &args) -> bool {
            try {
                filePath = args[2];
                ip = args[0];
                if (nickNames.count(ip) > 0)
                    ip = nickNames[ip];
                port = atoi(args[1].c_str());

                LocalTranslator translator(filePath.c_str(), ip.c_str(), port);
                deque<LocalTranslator> dq;
                dq.emplace_back(std::move(translator));

                return multiRun(dq);
            }
            catch (...) {
                return false;
            }
        }, cmd_run_port, 3, owner);
    }

    void readRecord() {
        string text = readJsonFromFile(record_file);
        recorder.import(text);
    }

    void outputRecord() {
        if (need_rem) {
            writeToFile(recorder.output(), record_file);
        }
    }

    void forShowHis(Launcher *owner) {
        forTool([](vector<string> &args) {
            int n = recorder.getNotDoneLength();
            unique_ptr<TranslateRecord[]> tr;
            tr.reset(new TranslateRecord[n]);
            //TranslateRecord *tr = ;
            n = recorder.getNotDone(tr.get(), n);
            UI::printText("not done History number:" + to_string(n) + "\n");
            for (int i = 0; i < n; i++) {
                UI::printText((format("%d. to{%s:%s} sent:%d bytes  file:%s\n") % i % tr[i]._ip % tr[i]._port %
                               tr[i]._sent % tr[i]._filePath).str());
            }

            n = recorder.getDoneLength();
            tr.reset(new TranslateRecord[n]);
            n = recorder.getDone(tr.get(), n);
            UI::printText("done History number:" + to_string(n) + "\n");
            for (int i = 0; i < n; i++) {
                UI::printText((format("%d. to{%s:%s} sent:%d bytes  file:%s\n") % i % tr[i]._ip % tr[i]._port %
                               tr[i]._sent % tr[i]._filePath).str());
            }
            //delete [] tr;
            return true;
        }, cmd_history, 0, owner);
    }

    void forRetry(Launcher *owner) {
        forTool([](vector<string> &args) {
            int ls = atoi(args[0].c_str());
            _if_error(ls > recorder.getNotDoneLength(),"out of range for not done task number")
            int n = recorder.getNotDoneLength();
            if (ls > 0) {
                n = ls;
            }
            TranslateRecord *tr = new TranslateRecord[n];
            n = recorder.getNotDone(tr, n);
            deque<LocalTranslator> translators;
            for (int i = 0; i < n; i++) {
                translators.emplace_back(std::move(LocalTranslator(tr[i]._filePath, tr[i]._ip, tr[i]._port)));
            }
            return multiRun(translators);
        }, cmd_retry, 1, owner);

    }

    void forMem(Launcher *owner) {
        need_rem = true;
    }


    void readingAction(LocalTranslator *owner) {
        UI::printText("reading file...\n");
    }


    void sendingAction(int ids, LocalTranslator *owner) {
        UI::printText("sending...\n");
        cost_time = std::chrono::steady_clock::now();
        thread td([owner, ids]() {
            size_t sent = 0;
            size_t old = 0;
            double speed = 0;
            bool done = false;
            std::unique_lock<mutex> lk(finish_mut, defer_lock);
            //UI::progressing();
            while (!done) {
                lk.lock();
                cv_status status = finish_control.wait_for(lk,
                                                           chrono::milliseconds(
                                                                   500));
                if (status == cv_status::no_timeout) {
                    done = true;
                }
                sent = owner->getSent();
                TranslateRecord *rc = recorder.getRecord(ids);
                rc->_sent = sent;
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
            UI::printProgress(25, 1.0,
                              (format("%.2f%% . has sent %zu bytes\tspeed:%.2f mb/s") %
                               (1.0 * 100) %
                               sent % speed).str());
            //UI::outProgressing();
            //UI::printText("\n");
            //cout << "\r100%" << endl;
        });
        td.detach();
    }

    void finishAction(LocalTranslator *owner) {
        finish_control.notify_all();
        auto edtime = std::chrono::steady_clock::now();
        double cost = chrono::duration<double>(edtime - cost_time).count();
        size_t sent = owner->getSent();
        double times = round(cost * 100.0) / 100.0;
        double mbs = round(1024.0 * 1024.0 * 100.0) / 100.0;
        double speed = round((double) fileTotal / mbs
                             / times * 100.0) / 100.0;


        {
            lock_guard<mutex> gard(finish_mut);
            UI::printText(
                    (format("\nhas sent %.2f kb\ncost time :%.2f s\nspeed :%.2f mb/s\n") %
                     (sent / 1024.0f) % cost % speed).str());
        }
    }

    bool runit(int ids, LocalTranslator &translator) {
        UI::printText("init...\n");
        fileTotal = translator.getTotalFileSize();
        UI::printText("connect...\n");
        bool connected = translator.Connect();

        if (connected) {
            UI::printText(
                    (format("file=%s\tfile_size=%.3lf kb \t ip=%s:%d\n") % filePath.c_str() % (fileTotal / 1024.0) %
                     ip.c_str() % port).str());
            sendingAction(ids, &translator);
            translator.sendPreparedData();
            finishAction(&translator);
        } else {

            UI::printText("fail to connect\n");
            return false;
        }

        //cout << "Speed:" << speed << " MB/s" << endl;

        return true;
    }


}
