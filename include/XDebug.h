#ifndef XDEBUG_H_
#define XDEBUG_H_
#ifdef   ADRIOD_DEVICE
#include <android/log.h>
#endif

class XDebug
{

private:
    const char *path;
    int level;

public:
    XDebug();
    ~XDebug();

    void SetTraceFilePath(char *path);
    void SetTraceLevel(int level);
	void GetTime(char *pBuf);
    void Trace(int tag, const char * format, ...);

};


#endif