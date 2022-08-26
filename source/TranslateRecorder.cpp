//
// Created by abstergo on 2022/8/24.
//

#include "TranslateRecorder.h"
#include "Backend/imp_TranslateRecorder.hpp"
#include "Application/Components/LocalTranslator.h"
#include <chrono>

unsigned long TranslateRecorder::pushRecord(LocalTranslator *task)
{
    if(task!= nullptr)
    {
        unsigned long id=duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        TranslateRecord rc(id,false,task->getSent(),task->getFileName().c_str(),task->getIp().c_str(),task->getPort());
        myImpl().push(rc);
        return id;
    }

    return 0;
}

bool TranslateRecorder::turnToDone(unsigned long id)
{

    return myImpl().turnDone(id);
}

void TranslateRecorder::getRecord(unsigned int id, TranslateRecord *record)
{
    TranslateRecord* rc= myImpl().getRecord(id);
    *record=*rc;
}

size_t TranslateRecorder::getNotDoneLength() const
{
    return cMyImpl().notDone_size();
}

size_t TranslateRecorder::getDoneLength() const
{
    return cMyImpl().hasDone_size();
}

int TranslateRecorder::getNotDone(TranslateRecord *des, int len)
{
    TranslateRecord** rcs=new TranslateRecord*[len];
    int all=myImpl().getNotDone(rcs,len);
    for(int i=0;i<all;i++)
    {
        des[i]= *rcs[i];
    }
    delete []rcs;
    return all;
}

int TranslateRecorder::getDone(TranslateRecord *des, int len)
{
    TranslateRecord** rcs=new TranslateRecord*[len];
    int all=myImpl().getHasDone(rcs,len);
    for(int i=0;i<all;i++)
    {
        des[i]= *rcs[i];
    }
    delete rcs;
    return all;
}
