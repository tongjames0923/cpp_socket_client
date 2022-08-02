
namespace consoleProgress
{
    using callBack = void (*)();

    void progress(callBack);

    void pushProgress(float value);

    void setUnDoneChar(char c);

    void setDoneChar(char c);


    void setLen(float len);
}
