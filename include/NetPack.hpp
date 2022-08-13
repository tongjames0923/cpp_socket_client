//
// Created by tbs on 2022/8/4.
//

#ifndef TESTER_NETPACK_H
#define TESTER_NETPACK_H

#include <deque>
#include <memory>
#include <string>
#include <fstream>
#include <functional>
#include <atomic>

using namespace std;


template<typename D>
     size_t ObjectToBuffer(const D &source, size_t objlen, char *des, size_t bsize)
    {
        size_t ds = sizeof(D) * objlen;
        if (ds <= bsize)
        {
            memcpy(des, &source, ds);
            return ds;
        }
        else
        {
            return bsize - ds;
        }
    }

     template<typename D>
     size_t BufferToObject(D *des, size_t objlen, char *bf, size_t size)
    {
        size_t ds = sizeof(D) * objlen;
        if (ds > size)
        {
            memcpy(des, bf, size);
            return ds;
        }
        else
            return ds - size;
    }



#endif // TESTER_NETPACK_H
