//
// Created by tbs on 2022/8/10.
//

#ifndef SOCKET_CLIENT_APP_H
#define SOCKET_CLIENT_APP_H

class Launcher;
void makeLauncher(Launcher** launcher);
namespace Features
{

    void forH(Launcher *owner);

    void forConfigNick(Launcher *owner);

    void forShowNick(Launcher *owner);
    void forRun_port(Launcher *owner);
    void forRun(Launcher *owner);
    void readConfig();
    void outPutConfig();
}





#endif //SOCKET_CLIENT_APP_H
