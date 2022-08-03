#include <iostream>

#include <string>
#include "progress.h"
#include <future>
#include <mutex>
#include <deque>

using namespace std;

namespace consoleProgress
{
    mutex m;
    float prog;
    const int status_runable = 1, status_finish = 2, status_abort = 0;
    const int c_undone = 0, c_done = 1;
    char cs[2] = {'-', '*'};
    float lens = 10;
    deque<float> q_prog;
    int status = status_runable;
    bool started = false;
#define LOCKIT lock_guard<mutex> g1(m);
#define LOCK_INTIME(code)         m.lock(); \
code \
    m.unlock();


    void output(float len,float prg,char done,char undone)
    {
        printf("\r[");
        for (int i = 0; i < prg; i++)
        {
            printf("%c", done);
        }
        for (int i = prg; i < len; i++)
            printf("%c", undone);
        cout << "]";
        printf("%.2f", prg / len * 100);
        cout << "%";
        cout.flush();
    }

    void progressImp(char undone, char done, float len)
    {
        while (status == status_runable)
        {
            if (!q_prog.empty())
            {
                prog = q_prog.front();
                q_prog.pop_front();
            } else
                continue;
            output(len,prog,done,undone);
        }
        LOCKIT
        if(status==status_finish)
        {
            output(len,len,done,undone);
            cout << "\nfinished" << endl;
        }
        else if(status==status_abort)
        {
            cout<<"\nprogress aborted"<<endl;
        }
        else
        {
            cout<<"\nunKnown Stop"<<endl;
        }
        started = false;
    }

    void pushProgress(float value)
    {
        if (value >= lens)
        {
            finish();
            return;
        }
        LOCKIT

        if (q_prog.empty())
            q_prog.push_back(value);
        else
        {
            if (value > q_prog.back())
                q_prog.push_back(value);
        }
    }

    void setUnDoneChar(char c)
    {
        LOCKIT
        if (!started)
            cs[c_undone] = c;
    }

    void setDoneChar(char c)
    {
        LOCKIT
        if (!started)
            cs[c_done] = c;
    }

    void setLen(float l)
    {
        LOCKIT
        if (!started)
            lens = l;
    }

    bool progress(callBack callBack)
    {
        bool isStarted;
        LOCK_INTIME(
                isStarted = started;
                if (!started)
                {
                    started = true;
                    status = status_runable;
                    q_prog.clear();
                    prog = 0;
                }

        )
        if (isStarted)
        {
            return false;
        } else
        {
            shared_future<void> fu = async(callBack);
            progressImp(cs[c_undone], cs[c_done], lens);
            return true;
        }

    }

    void abort()
    {
        LOCKIT
        status = status_abort;
    }

    void finish()
    {
        LOCKIT
        status=status_finish;
    }
}
