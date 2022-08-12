//
// Created by tbs on 2022/8/9.
//

#ifndef SOCKET_CLIENT_LAUNCHER_H
#define SOCKET_CLIENT_LAUNCHER_H

#include <functional>
#include <memory>
#include <queue>
#include <string>
#include "Pointerable.h"
struct ArgData
{
    std::shared_ptr<char> args;
    size_t args_len;
};
struct ArgInfo
{
    int length;
    char name[128];
    int start;
};
class imp_Launcher;
class Launcher:protected virtual Pointerable<imp_Launcher>,public virtual None_Copyable
{
public:
    Launcher();
    virtual ~Launcher();
    using ArgFunction = std::function<void(Launcher *self)>;

    void Start(int argc, char **argv);

    void putData(std::string dataName, void *data, size_t length);

    bool getData(std::string dataName, void *data, size_t length);

    bool existData(std::string data);

    void removeData(std::string dataName);

    void setArgFunction(std::string argName, ArgFunction function);

    bool existFunction(std::string argName);
    bool getArgInfo(std::string argName,ArgInfo*des);
    std::string getDefaultArgName(const std::string& arg,int argc);
protected:
    virtual void ready(int argc, char **argv)
    {

    }
    virtual void cleanup()
    {

    }
    virtual bool autoPutdataFromArgs()
    {
        return true;
    }
};


/**
 * 判断是否为arg传入参数关键字
 */
using key_function = std::function<bool(const char* argtext,size_t len)>;
static std::queue<const std::string> makeArgQueue(int argc, char **argv)
{
    std::queue<const std::string> q;
    for(int i=0;i<argc;i++)
    {
        q.push(argv[i]);
    }
    return q;
}



static void catchArg(std::queue<const std::string> q,key_function isKey,std::vector<ArgInfo>& des);

#endif //SOCKET_CLIENT_LAUNCHER_H
