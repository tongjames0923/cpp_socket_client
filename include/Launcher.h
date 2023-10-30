//
// Created by tbs on 2022/8/9.
//

#ifndef SOCKET_CLIENT_LAUNCHER_H
#define SOCKET_CLIENT_LAUNCHER_H

#include <functional>
#include <memory>
#include <string>
#include "Pointerable.hpp"

class imp_Launcher;

static constexpr const char * COMMAND[] = {"-h", "--help", "-config_nick", "-nick", "run","run_port",
                                           "-history","-retry","-mem"};
static constexpr int cmd_h = 0, cmd_help = 1, cmd_run = 4, cmd_config_nick = 2, cmd_nick = 3,cmd_run_port = 5,cmd_history=6,
        cmd_retry=7,cmd_mem=8;

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
    /// @brief 兼容的组别号
    int group;
};
struct ArgData
{
    /// @brief 参数内容
    std::unique_ptr<char[]> args;
    /// @brief 参数内容长度
    size_t args_len;
};

struct imp_Launcher_Deleter final
{
    constexpr imp_Launcher_Deleter() noexcept = default;

    void operator()(imp_Launcher *p) const;
};

/// @brief 程序参数启动器
class Launcher : protected virtual Pointerable<imp_Launcher, imp_Launcher_Deleter>, public virtual None_Copyable
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
    void putData(const std::string &dataName, void *data, size_t length) noexcept;

    /// @brief 获取持久数据
    /// @param dataName  数据名
    /// @param data  数据目标地址
    /// @param length 数据长度
    /// @return  是否获取成功
    bool getData(const std::string &dataName, void *data, size_t length) const noexcept;

    /// @brief 是否存在数据
    /// @param data 数据名
    /// @return 
    bool existData(const std::string &data) const noexcept;

    /// @brief 移除数据
    /// @param dataName 数据名
    void removeData(const std::string &dataName) noexcept;

    /// @brief 设置参数功能方法
    /// @param argName 参数指定关键字
    /// @param function 运行功能方法
    void setArgFunction(const std::string &argName, ArgFunction function) noexcept;

    /// @brief 是否存在功能方法
    /// @param argName 功能方法对应的关键字
    /// @return 
    bool existFunction(const std::string &argName) const noexcept;

    /// @brief 获取数据信息
    /// @param argName   参数关键字名
    /// @param des  数据储存地
    /// @return  是否获取成功
    bool getArgInfo(const std::string &argName, ArgInfo *des) const noexcept;

    /// @brief 获取默认存入的数据名
    /// @param arg 参数关键字
    /// @param argc 参数序号
    /// @return arg参数的第argc个参数内容的默认数据名
    virtual std::string getDefaultArgName(const std::string &arg, int argc) const noexcept;

    /**
     *关键字优先度设置
     * @param argKey 关键字字符串
     * @param len 字符串长度
     * @return
     */
    virtual int prioritySet(const char *argKey, size_t len) const noexcept;

    /**
     * 关键字组别设置
     * @param argKey 关键字字符串
     * @param len 关键字长度
     * @return
     */
    virtual int groupSet(const char *argKey, size_t len) const noexcept;

protected:
    /// @brief 当功能运行前的准备函数
    /// @param argc 
    /// @param argv 
    virtual void ready(int argc, char **argv);

    /// @brief 当主功能完成后的清理函数
    virtual void cleanup();

    /// @brief 读取时是否将参数直接放入data区域，以getDefaultArgName方法进行命名
    /// @return 
    virtual bool autoPutdataFromArgs();


};

#endif //SOCKET_CLIENT_LAUNCHER_H
