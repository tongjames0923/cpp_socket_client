//
// Created by tbs on 2022/8/10.
//

#ifndef SOCKET_CLIENT_APP_H
#define SOCKET_CLIENT_APP_H

class Launcher;

/// @brief 获取启动器
/// @param launcher 启动器指针位置
void makeLauncher(Launcher **launcher);

namespace Features
{
    /// @brief 运行帮助函数
    /// @param owner 
    void forH(Launcher *owner);

    /// @brief 运行配置昵称方法
    /// @param owner 
    void forConfigNick(Launcher *owner);

    /// @brief 运行展示昵称函数
    /// @param owner 
    void forShowNick(Launcher *owner);

    /// @brief ָ运行传输指定端口函数
    /// @param owner 
    void forRun_port(Launcher *owner);

    /// @brief 运行传输函数
    /// @param owner 
    void forRun(Launcher *owner);

    /// @brief 读取运行配置
    void readConfig();

    /// @brief 输出运行配置
    void outPutConfig();

    void readRecord();

    void outputRecord();
    void forShowHis(Launcher *owner);
    void forRetry(Launcher *owner);
    void forMem(Launcher *owner);


}

#endif //SOCKET_CLIENT_APP_H
