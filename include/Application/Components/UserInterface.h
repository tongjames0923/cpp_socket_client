//
// Created by tbs on 2022/8/20.
//

#ifndef SOCKET_CLIENT_USERINTERFACE_H
#define SOCKET_CLIENT_USERINTERFACE_H

#include <cstdio>
#include <string>

void readyForThread();
void loop();
void shutdown();
void shutdown_WhenEmpty();


void printText(const std::string& str);
void printProgress(size_t progressLen, double progress,const std::string& append);





#endif //SOCKET_CLIENT_USERINTERFACE_H
