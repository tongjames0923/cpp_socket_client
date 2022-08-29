//
// Created by abstergo on 2022/8/24.
//

#ifndef SOCKET_CLIENT_TRANSLATERECORDER_H
#define SOCKET_CLIENT_TRANSLATERECORDER_H

#include "Pointerable.hpp"
#include <string>
#include <cstring>

class LocalTranslator;

struct TranslateRecord
{
    int _id;
    bool _isdone = false;
    int _sent = 0;
    char _filePath[128];
    char _ip[128];
    int _port;

    TranslateRecord() = default;

    TranslateRecord(int id, bool isdone, int sent, const char *filePath, const char *ip, int port) : _id(id),
                                                                                                     _isdone(isdone),
                                                                                                     _sent(sent),
                                                                                                     _port(port)
    {
        strcpy(_filePath, filePath);
        strcpy(_ip, ip);
    }

    bool operator==(const TranslateRecord &rhs) const
    {
        return _id == rhs._id;
    }

    bool operator==(const int &rhs) const
    {
        return _id == rhs;
    }


};

class imp_TranslateRecorder;

struct TranslateRecordDeleter
{
    constexpr TranslateRecordDeleter() noexcept = default;

    void operator()(imp_TranslateRecorder *p) const;
};


class TranslateRecorder : public virtual AutoAlivePointerable<imp_TranslateRecorder, TranslateRecordDeleter>
{
public:
    void import(const std::string &json);

    std::string output() const;

    int pushRecord(LocalTranslator *task);

    bool turnToDone(int id);

    TranslateRecord *getRecord(int id);

    size_t getNotDoneLength() const;

    size_t getDoneLength() const;

    int getNotDone(TranslateRecord *des, int len);

    int getDone(TranslateRecord *des, int len);

    TranslateRecorder();

};


#endif //SOCKET_CLIENT_TRANSLATERECORDER_H
