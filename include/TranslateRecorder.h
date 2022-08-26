//
// Created by abstergo on 2022/8/24.
//

#ifndef SOCKET_CLIENT_TRANSLATERECORDER_H
#define SOCKET_CLIENT_TRANSLATERECORDER_H

#include "Pointerable.hpp"

class LocalTranslator;
struct TranslateRecord;
class imp_TranslateRecorder;

class TranslateRecorder:public virtual AutoAlivePointerable<imp_TranslateRecorder>
{
public:
    unsigned long pushRecord(LocalTranslator* task);

    bool turnToDone(unsigned  long id);

    void getRecord(unsigned id,TranslateRecord* record);

    size_t  getNotDoneLength()const;
    size_t  getDoneLength()const;
    int getNotDone(TranslateRecord* des,int len);
    int getDone(TranslateRecord* des,int len);

};


#endif //SOCKET_CLIENT_TRANSLATERECORDER_H
