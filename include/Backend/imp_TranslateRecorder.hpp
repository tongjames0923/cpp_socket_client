//
// Created by abstergo on 2022/8/24.
//

#ifndef SOCKET_CLIENT_IMP_TRANSLATERECORDER_HPP
#define SOCKET_CLIENT_IMP_TRANSLATERECORDER_HPP
#include <string>
#include <list>
#include "cJSON.h"
using namespace  std;


const char* key_[6]={"id","isdone","sent","path","ip","port"};
const int _key_id=0,_key_isdone=1,_key_sent=2,_key_path=3,_key_ip=4,_key_port=5;

static cJSON * toCjson(const TranslateRecord& rec)
{
    cJSON* root= cJSON_CreateObject();
    cJSON_AddBoolToObject(root,key_[_key_isdone],rec._isdone);
    cJSON_AddNumberToObject(root,key_[_key_port],rec._port);
    cJSON_AddNumberToObject(root,key_[_key_sent],rec._sent);
    cJSON_AddNumberToObject(root,key_[_key_id],rec._id);
    cJSON_AddStringToObject(root,key_[_key_path],rec._filePath);
    cJSON_AddStringToObject(root,key_[_key_ip],rec._ip);
    return root;
}
static TranslateRecord jsonToObject(cJSON* obj)
{
    TranslateRecord rc;
    rc._isdone= cJSON_IsTrue(cJSON_GetObjectItem(obj,key_[_key_isdone]));
    rc._id= cJSON_GetObjectItem(obj,key_[_key_id])->valueint;
    rc._port= cJSON_GetObjectItem(obj,key_[_key_port])->valueint;
    rc._sent=cJSON_GetObjectItem(obj,key_[_key_sent])->valueint;
    strcpy(rc._filePath,cJSON_GetObjectItem(obj,key_[_key_path])->valuestring);
    strcpy(rc._ip,cJSON_GetObjectItem(obj,key_[_key_ip])->valuestring);
    return rc;
}
TranslateRecord jsonToObject(const string& json)
{
    TranslateRecord rc;
    cJSON * obj= cJSON_Parse(json.c_str());
    rc._isdone= cJSON_IsBool(cJSON_GetObjectItem(obj,key_[_key_isdone]));
    rc._id= cJSON_GetObjectItem(obj,key_[_key_id])->valueint;
    rc._port= cJSON_GetObjectItem(obj,key_[_key_port])->valueint;
    rc._sent=cJSON_GetObjectItem(obj,key_[_key_sent])->valueint;
    strcpy(rc._filePath,cJSON_GetObjectItem(obj,key_[_key_path])->valuestring);
    strcpy(rc._ip,cJSON_GetObjectItem(obj,key_[_key_ip])->valuestring);
    cJSON_free(obj);
    return rc;
}

class imp_TranslateRecorder
{
public:
    void push(const TranslateRecord& rc)
    {
        if(rc._isdone)
        {
            m_has_done.emplace_back(rc);
        }
        else
        {
            m_not_done.emplace_back(rc);
        }
    }
    bool turnDone(int id)
    {
        bool  hasit= false;
        list<TranslateRecord>::iterator it=m_not_done.begin();
        while (it!=m_not_done.end())
        {
            if(it->_id==id)
            {
                hasit= true;
                break;
            }
            ++it;
        }
        if(!hasit)
            return false;
        it->_isdone= true;
        m_has_done.emplace_back(*it);
        m_not_done.erase(it);
        return true;
    }

    void toObject(const string& cjson)
    {
        cJSON * root= cJSON_Parse(cjson.c_str());
        cJSON * un=cJSON_GetObjectItem(root,"undone");
        for(int i=0;i< cJSON_GetArraySize(un);i++)
        {
            cJSON * obj= cJSON_GetArrayItem(un,i);
            m_not_done.emplace_back(jsonToObject(obj));
        }
        cJSON * dn=cJSON_GetObjectItem(root,"done");
        for(int i=0;i< cJSON_GetArraySize(dn);i++)
        {
            cJSON * obj= cJSON_GetArrayItem(dn,i);
            m_has_done.emplace_back(jsonToObject(obj));
        }

    }
    TranslateRecord* getRecord(int id)
    {
        for(TranslateRecord& r:m_not_done)
        {
            if(r==id)
            {

                return &r;
            }
        }
        for(TranslateRecord& r:m_has_done)
        {
            if(r==id)
            {
                return &r;
            }
        }
        return nullptr;
    }

    string to_string() const
    {
        string str;
        cJSON* root=cJSON_CreateObject();
        cJSON* undone= cJSON_AddArrayToObject(root,"undone");
        for(const TranslateRecord& rc: m_not_done)
        {
            cJSON_AddItemToArray(undone, toCjson(rc));
        }
        cJSON *done= cJSON_AddArrayToObject(root,"done");
        for(const TranslateRecord& rc: m_has_done)
        {
            cJSON_AddItemToArray(done, toCjson(rc));
        }
        str=cJSON_Print(root);
        cJSON_free(root);
        return str;
    }

    size_t notDone_size()const
    {
       return m_not_done.size();
    }
    size_t hasDone_size() const
    {
        return m_has_done.size();
    }
    int getNotDone(TranslateRecord** rcs,int l)
    {
        int cnt=0;
        for(TranslateRecord& r:m_not_done)
        {
            *rcs=&r;
            ++rcs;
            --l;
            ++cnt;
            if(l==0)
                break;
        }
        return cnt;
    }
    int getHasDone(TranslateRecord** rcs,int l)
    {
        int cnt=0;
        for(TranslateRecord& r:m_has_done)
        {
            *rcs=&r;
            ++rcs;
            --l;
            ++cnt;
            if(l==0)
                break;
        }
        return cnt;
    }

private:
    list<TranslateRecord> m_not_done;
    list<TranslateRecord> m_has_done;
};

#endif //SOCKET_CLIENT_IMP_TRANSLATERECORDER_HPP
