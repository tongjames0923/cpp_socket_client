//
// Created by tbs on 2022/8/11.
//

#ifndef SOCKET_CLIENT_IMP_LAUNCHER_HPP
#define SOCKET_CLIENT_IMP_LAUNCHER_HPP

#include <memory>
#include <string>
#include <map>



class imp_Launcher
{
public:
    using imp_ArgFunction = std::function<void(imp_Launcher *self)>;
    using functionMaper = std::map<std::string, imp_ArgFunction>;
    using dataMapper = std::map<std::string, ArgData>;

    dataMapper m_dataMapper;
    functionMaper m_functionMaper;
    std::map<std::string, ArgInfo> m_infos;
};


#endif //SOCKET_CLIENT_IMP_LAUNCHER_HPP
