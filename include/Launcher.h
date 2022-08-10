//
// Created by tbs on 2022/8/9.
//

#ifndef SOCKET_CLIENT_LAUNCHER_H
#define SOCKET_CLIENT_LAUNCHER_H

#include <map>
#include <functional>
#include <memory>
#include <queue>
#include <string>
struct ArgInfo
{
    int length;
    char name[128];
    int start;
};

class Launcher
{
public:
    struct ArgData
    {
        std::shared_ptr<char> args;
        size_t args_len;
    };


    Launcher(const Launcher &otherwise) = delete;

    Launcher()=default;

    using ArgFunction = std::function<void(Launcher *self)>;
    using functionMaper = std::map<std::string, ArgFunction>;
    using dataMapper = std::map<std::string, ArgData>;

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
private:
    dataMapper m_dataMapper;
    functionMaper m_functionMaper;
    std::map<std::string,ArgInfo> m_infos;
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



static void catchArg(std::queue<const std::string> q,key_function isKey,std::vector<ArgInfo>& des)
{
    int i=0;
    std::string lstr;
    ArgInfo info{.start=-1};
    while (!q.empty())
    {
        std::string now=q.front();
        q.pop();
        if(isKey(now.c_str(),now.length()))
        {
            if(info.start==-1)
            {
                info.start=i+1;
                strcpy(info.name,now.c_str());
            }
            else
            {
                info.length=i-info.start;
                des.push_back(info);
                //memcpy(des+bg,&info,sizeof(ArgInfo));
                strcpy(info.name,now.c_str());
                info.start=i+1;
                //++bg;
            }
        }
        ++i;
    }
    if(info.start>=0)
    {
        info.length= i-info.start;
        des.push_back(info);
    }
}

#endif //SOCKET_CLIENT_LAUNCHER_H
