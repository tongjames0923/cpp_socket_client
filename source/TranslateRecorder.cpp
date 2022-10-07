//
// Created by abstergo on 2022/8/24.
//

#include "TranslateRecorder.h"
#include "Backend/imp_TranslateRecorder.hpp"
#include "Application/Components/LocalTranslator.h"
#include <chrono>


void TranslateRecorder::import(const std::string &json)
{
    myImpl().toObject(json);

}

int TranslateRecorder::pushRecord(LocalTranslator *task)
{
    if (task != nullptr)
    {
        int id =
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                        std::chrono::system_clock::now().time_since_epoch()).count() %
                (int) INT32_MAX;
        TranslateRecord rc(id, false, task->getSent(), task->getFileName().c_str(), task->getIp().c_str(),
                           task->getPort());
        myImpl().push(rc);
        return id;
    }

    return 0;
}

bool TranslateRecorder::turnToDone(int id)
{

    return myImpl().turnDone(id);
}

TranslateRecord *TranslateRecorder::getRecord(int id)
{
    return myImpl().getRecord(id);
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
    TranslateRecord **rcs = new TranslateRecord *[len];
    int all = myImpl().getNotDone(rcs, len);
    for (int i = 0; i < all; i++)
    {
        des[i] = *rcs[i];
    }
    delete[]rcs;
    return all;
}

int TranslateRecorder::getDone(TranslateRecord *des, int len)
{
    TranslateRecord **rcs = new TranslateRecord *[len];
    int all = myImpl().getHasDone(rcs, len);
    for (int i = 0; i < all; i++)
    {
        des[i] = *rcs[i];
    }
    delete[]rcs;
    return all;
}

std::string TranslateRecorder::output() const
{
    return cMyImpl().to_string();
}

TranslateRecorder::TranslateRecorder()
{

}

void TranslateRecordDeleter::operator()(imp_TranslateRecorder *p) const
{
    delete p;
}
