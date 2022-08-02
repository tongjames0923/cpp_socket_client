
#pragma  once

#include <functional>
namespace consoleProgress
{
    using callBack =std::function<void(void)>;

    bool progress(callBack);
    void pushProgress(float value);
    void setUnDoneChar(char c);
    void setDoneChar(char c);
    void setLen(float len);
    void abort();
    void finish();
}
