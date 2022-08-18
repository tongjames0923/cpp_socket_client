//
// Created by tbs on 2022/8/9.
//
#include "Launcher.h"
#include "Backend/imp_Launcher.hpp"
#include <queue>
#include <list>

using namespace std;

/**
 * 判断是否为arg传入参数关键字
 */
using key_function = std::function<bool(const char *argtext, size_t len)>;

bool cmp_Arginfo(const ArgInfo &a, const ArgInfo &b)
{
    return a.priority < b.priority;
}

/// @brief 捕获关键字参数
/// @param argc 参数长度
/// @param argv 所有参数的值
/// @param isKey 判断是否是参数关键字
/// @param des  储存位置
static void catchArg(int argc, char **argv, key_function isKey, vector<ArgInfo> &des);


void Launcher::Start(int argc, char **argv)
{
    ready(argc, argv);
    vector<ArgInfo> infos;
    catchArg(argc, argv, [this](const char *name, size_t len) -> bool
    {
        return existFunction(std::string(name));
    }, infos);
    int defaultGroup = -1;
    for (ArgInfo &item : infos)
    {
        size_t ls = strlen(item.name);
        item.priority = prioritySet(item.name, ls);
        int gp = groupSet(item.name, ls);
        if (gp < 0)
            throw std::runtime_error("Invalid group number,must need >=0");
        item.group = gp;
        if (defaultGroup != -1 && defaultGroup != item.group)
        {
            throw std::runtime_error("Incompatible groups commands");
        }
        defaultGroup = gp;
        myImpl().m_infos[item.name] = item;
        if (autoPutdataFromArgs())
        {
            for (int i = 0; i < item.length; ++i)
            {
                putData(getDefaultArgName(item.name, i), argv[i + item.start], strlen(argv[i + item.start]));
            }
        }
    }
    sort(infos.begin(), infos.end(), cmp_Arginfo);
    imp_Launcher &imp = myImpl();
    for (const ArgInfo &info:infos)
    {
        imp.m_functionMaper[info.name](&imp);
    }

}

void Launcher::putData(const std::string &dataName, void *data, size_t length) noexcept
{
    char *d = new char[length];
    memcpy(d, data, length);
    if (!existData(dataName))
    {
        myImpl().m_dataMapper[dataName] = ArgData();
    }
    myImpl().m_dataMapper[dataName].args.reset(d);
    myImpl().m_dataMapper[dataName].args_len = length;
}

bool Launcher::existData(const std::string &data) const noexcept
{
    return cMyImpl().m_dataMapper.count(data) > 0;
}

bool Launcher::getData(const std::string &dataName, void *data, size_t length) const noexcept
{
    if (existData(dataName))
    {
        const ArgData &d = cMyImpl().m_dataMapper.at(dataName);
        if (length >= d.args_len)
        {
            memcpy(data, d.args.get(), d.args_len);
            return true;
        }
    }
    return false;
}

void Launcher::removeData(const std::string &dataName) noexcept
{
    myImpl().m_dataMapper.erase(dataName);
}

void Launcher::setArgFunction(const std::string &argName, Launcher::ArgFunction fun) noexcept
{
    imp_Launcher::imp_ArgFunction fuc = [fun, this](imp_Launcher *self)
    {
        fun(this);
    };
    myImpl().m_functionMaper[argName] = fuc;
}

bool Launcher::existFunction(const std::string &argName) const noexcept
{
    return cMyImpl().m_functionMaper.count(argName) > 0;
}

bool Launcher::getArgInfo(const std::string &argName, ArgInfo *des) const noexcept
{
    bool hasit = cMyImpl().m_infos.count(argName) > 0;
    if (hasit)
    {
        memcpy(des, &cMyImpl().m_infos.at(argName), sizeof(ArgInfo));
        return true;
    }
    return false;
}

std::string Launcher::getDefaultArgName(const std::string &arg, int argc) const noexcept
{
    string k(arg);
    k += "_" + to_string(argc);
    return k;
}

int Launcher::prioritySet(const char *argKey, size_t len) const noexcept
{
    return 0;
}

void Launcher::ready(int argc, char **argv)
{
}

void Launcher::cleanup()
{
}

bool Launcher::autoPutdataFromArgs()
{
    return true;
}

Launcher::Launcher()
{
    selfType::makeAlive(this);
}

int Launcher::groupSet(const char *argKey, size_t len) const noexcept
{
    return 0;
}

Launcher::~Launcher() = default;

void imp_Launcher_Deleter::operator()(imp_Launcher *p) const
{
    delete p;
}


void catchArg(int argc, char **argv, key_function isKey, vector<ArgInfo> &des)
{
    int i = 0;
    std::string lstr;
    ArgInfo info;
    info.start = -1;
    for (; i < argc; ++i)
    {
        std::string now(argv[i]);
        if (isKey(now.c_str(), now.length()))
        {
            if (info.start == -1)
            {
                info.start = i + 1;
                strcpy(info.name, now.c_str());
            } else
            {
                info.length = i - info.start;
                des.push_back(info);
                //memcpy(des+bg,&info,sizeof(ArgInfo));
                strcpy(info.name, now.c_str());
                info.start = i + 1;
                //++bg;
            }
        }
    }
    if (info.start >= 0)
    {
        info.length = i - info.start;
        des.push_back(info);
    }
}
