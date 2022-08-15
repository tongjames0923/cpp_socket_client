//
// Created by tbs on 2022/8/9.
//

#ifndef SOCKET_CLIENT_LAUNCHER_H
#define SOCKET_CLIENT_LAUNCHER_H

#include <functional>
#include <memory>
#include <queue>
#include <string>
#include "Pointerable.hpp"
class imp_Launcher;
/// @brief 参数数据
struct ArgData
{
    /// @brief 参数内容
    std::unique_ptr<char[]> args;
    /// @brief 参数内容长度
    size_t args_len;
};
/// @brief 参数信息结构体
struct ArgInfo
{    
    /// @brief 参数长度
    int length;
    /// @brief 参数关键字
    char name[128];
    /// @brief 有效参数起始位
    int start;
};
struct imp_Launcher_Deleter final
{
    constexpr imp_Launcher_Deleter() noexcept = default;
    void operator()(imp_Launcher *p) const;
};
/// @brief 程序参数启动器
class Launcher: protected virtual Pointerable<imp_Launcher,imp_Launcher_Deleter>, public virtual None_Copyable
{
public:
    /// @brief 默认构造函数
    Launcher();
    ~Launcher() override;
    /// @brief 对应的参数运行方法
    using ArgFunction = std::function<void(Launcher *self)>;
    /// @brief 启动运行
    /// @param argc 参数个数
    /// @param argv 参数值
    void Start(int argc, char **argv);
    /// @brief 存入持久数据
    /// @param dataName  数据名
    /// @param data 数据
    /// @param length 数据长度
    void putData(std::string dataName, void *data, size_t length);
    /// @brief 获取持久数据
    /// @param dataName  数据名
    /// @param data  数据目标地址
    /// @param length 数据长度
    /// @return  是否获取成功
    bool getData(std::string dataName, void *data, size_t length) noexcept;
    /// @brief 是否存在数据
    /// @param data 数据名
    /// @return 
    bool existData(std::string data);
    /// @brief 移除数据
    /// @param dataName 数据名
    void removeData(std::string dataName);
    /// @brief 设置参数功能方法
    /// @param argName 参数指定关键字
    /// @param function 运行功能方法
    void setArgFunction(std::string argName, ArgFunction function);
    /// @brief 
    /// @param argName 
    /// @return 
    bool existFunction(std::string argName);
    /// @brief 获取数据信息
    /// @param argName   参数关键字名
    /// @param des  数据储存地
    /// @return  是否获取成功
    bool getArgInfo(std::string argName,ArgInfo*des);
    /// @brief 获取默认存入的数据名
    /// @param arg 参数关键字
    /// @param argc 参数序号
    /// @return arg参数的第argc个参数内容的默认数据名
    std::string getDefaultArgName(const std::string& arg,int argc);
protected:
    /// @brief 当功能运行前的准备函数
    /// @param argc 
    /// @param argv 
    virtual void ready(int argc, char **argv)
    {

    }
    /// @brief 当主功能完成后的清理函数
    virtual void cleanup()
    {

    }
    /// @brief 读取时是否将参数直接放入data区域，以getDefaultArgName方法进行命名
    /// @return 
    virtual bool autoPutdataFromArgs()
    {
        return true;
    }


};


/**
 * 判断是否为arg传入参数关键字
 */
using key_function = std::function<bool(const char* argtext,size_t len)>;



/// @brief 捕获关键字参数
/// @param argc 参数长度
/// @param argv 所有参数的值
/// @param isKey 判断是否是参数关键字
/// @param des  储存位置
static void catchArg(int argc,char** argv,key_function isKey,std::vector<ArgInfo>& des);

#endif //SOCKET_CLIENT_LAUNCHER_H
