//
// Created by tbs on 2022/8/11.
//

#ifndef SOCKET_CLIENT_IMP_LAUNCHER_HPP
#define SOCKET_CLIENT_IMP_LAUNCHER_HPP

#include <memory>
#include <string>
#include <map>
struct ArgInfo
{
    /// @brief 参数长度
    int length;
    /// @brief 参数关键字
    char name[128];
    /// @brief 有效参数起始位
    int start;
    /// @brief 参数优先级
    int priority;
};
struct ArgData
{
    /// @brief 参数内容
    std::unique_ptr<char[]> args;
    /// @brief 参数内容长度
    size_t args_len;
};
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
