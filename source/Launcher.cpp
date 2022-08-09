//
// Created by tbs on 2022/8/9.
//

#include "Launcher.h"

using namespace std;



void Launcher::Start(int argc, char **argv)
{
    ready(argc, argv);
    auto q= makeArgQueue(argc,argv);
    vector<ArgInfo> infos;
    catchArg(q,[this](const char* name,size_t len)->bool{
        return existFunction(std::string(name));
    },infos);
    for(ArgInfo& item:infos)
    {
        m_infos[item.name]=item;
        if(autoPutdataFromArgs())
        {
            for(int i=0;i<item.length;++i)
            {
                putData(getDefaultArgName(item.name,i),argv[i], strlen(argv[i]));
            }
        }
    }

    for (int i = 0; i < argc; i++)
    {
        if (existFunction(argv[i]))
        {
            m_functionMaper[argv[i]](this);
        }
    }
}

void Launcher::putData(std::string dataName, void *data, size_t length)
{
    char *d = new char[length];
    memcpy(d, data, length);
    if (!existData(dataName))
    {
        m_dataMapper[dataName] = ArgData();
    }
    m_dataMapper[dataName].args.reset(d, std::default_delete<char[]>());
    m_dataMapper[dataName].args_len = length;
}

bool Launcher::existData(std::string data)
{
    return m_dataMapper.count(data) > 0;
}

bool Launcher::getData(std::string dataName, void *data, size_t length)
{
    if (existData(dataName))
    {
        ArgData &d = m_dataMapper[dataName];
        if (length >= d.args_len)
        {
            memcpy(data, d.args.get(), length);
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

void Launcher::removeData(std::string dataName)
{
    m_dataMapper.erase(dataName);
}

void Launcher::setArgFunction(std::string argName, Launcher::ArgFunction function)
{
    m_functionMaper[argName] = function;
}

bool Launcher::existFunction(std::string argName)
{
    return m_functionMaper.count(argName) > 0;
}

bool Launcher::getArgInfo(std::string argName, ArgInfo *des)
{
    bool hasit=m_infos.count(argName)>0;
    if(hasit)
    {
        memcpy(des,&m_infos[argName],sizeof(ArgInfo));
        return true;
    }
    return false;
}

std::string Launcher::getDefaultArgName(const std::string &arg, int argc)
{
    string k(arg);
    k+="_"+ to_string(argc);
    return k;
}
