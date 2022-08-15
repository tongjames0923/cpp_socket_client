//
// Created by tbs on 2022/8/9.
//
#include "Launcher.h"
#include "Backend/imp_Launcher.hpp"

using namespace std;

void Launcher::Start(int argc, char** argv)
{
	ready(argc, argv);
	vector<ArgInfo> infos;
	catchArg(argc,argv, [this](const char* name, size_t len) -> bool
		{
			return existFunction(std::string(name));
		}, infos);
	for (ArgInfo& item : infos)
	{
		myImpl().m_infos[item.name] = item;
		if (autoPutdataFromArgs())
		{
			for (int i = 0; i < item.length; ++i)
			{
				putData(getDefaultArgName(item.name, i), argv[i + item.start], strlen(argv[i + item.start]));
			}
		}
	}

	for (int i = 0; i < argc; i++)
	{
		if (existFunction(argv[i]))
		{
			myImpl().m_functionMaper[argv[i]](&myImpl());
		}
	}
}

void Launcher::putData(std::string dataName, void* data, size_t length)
{
	char* d = new char[length];
	memcpy(d, data, length);
	if (!existData(dataName))
	{
		myImpl().m_dataMapper[dataName] = ArgData();
	}
	myImpl().m_dataMapper[dataName].args.reset(d);
	myImpl().m_dataMapper[dataName].args_len = length;
}

bool Launcher::existData(std::string data)
{
	return  myImpl().m_dataMapper.count(data) > 0;
}

bool Launcher::getData(std::string dataName, void* data, size_t length) noexcept
{
	if (existData(dataName))
	{
		ArgData& d = myImpl().m_dataMapper[dataName];
		if (length >= d.args_len)
		{
			memcpy(data, d.args.get(), d.args_len);
			return true;
		}
	}
	return false;
}

void Launcher::removeData(std::string dataName)
{
	myImpl().m_dataMapper.erase(dataName);
}

void Launcher::setArgFunction(std::string argName, Launcher::ArgFunction fun)
{
	imp_Launcher::imp_ArgFunction fuc = [fun, this](imp_Launcher* self)
	{
		fun(this);
	};
	myImpl().m_functionMaper[argName] = fuc;
}

bool Launcher::existFunction(std::string argName)
{
	return myImpl().m_functionMaper.count(argName) > 0;
}

bool Launcher::getArgInfo(std::string argName, ArgInfo* des)
{
	bool hasit = myImpl().m_infos.count(argName) > 0;
	if (hasit)
	{
		memcpy(des, &myImpl().m_infos[argName], sizeof(ArgInfo));
		return true;
	}
	return false;
}

std::string Launcher::getDefaultArgName(const std::string& arg, int argc)
{
	string k(arg);
	k += "_" + to_string(argc);
	return k;
}

Launcher::Launcher()
{
	selfType::makeAlive(this);
}

Launcher::~Launcher() = default;

void imp_Launcher_Deleter::operator()(imp_Launcher * p) const
{
	delete p;
}

void catchArg(int argc, char** argv, key_function isKey, std::vector<ArgInfo>& des)
{
	int i = 0;
	std::string lstr;
	ArgInfo info;
	info.start = -1;
	for(;i<argc;++i)
	{
		std::string now(argv[i]);
		if (isKey(now.c_str(), now.length()))
		{
			if (info.start == -1)
			{
				info.start = i + 1;
				strcpy(info.name, now.c_str());
			}
			else
			{
				info.length = i - info.start;
				des.emplace_back(info);
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
		des.emplace_back(info);
	}
}
