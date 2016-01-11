#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <Windows.h>
#include <direct.h>

#else
#include <pthread.h>        
#include <sys/types.h> 
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif



#ifdef __cplusplus
extern "C" {
#endif

#define MAX_STR_SIZE			256
#define LOG_FOLDER				"/var/log"
#define MAX_LOG_FILE_INDEX		5
#define DEFAULT_LOG_FILE_SIZE	2 * 1024 * 1024
#define DEFAULT_LOG_MEM_SIZE	1 * 1024

void initLock(void* pLock);
void lock(void* pLock);
void unLock(void* pLock);

typedef void (*MSLogEv)(
						 const int		type,
						 const char		*logMessage,
						 const int		size);

typedef enum
{
	MS_LOG_NONE = 0,
	MS_LOG_SCREEN,
	MS_LOG_FILE,
	MS_LOG_MEMORY,
	MS_LOG_NET
}MS_LOG_TYPE;

typedef enum
{
	MS_LOG_LEVEL_ERROR = 0,
	MS_LOG_LEVEL_WARNING,
	MS_LOG_LEVEL_INFO,
	MS_LOG_LEVEL_UNKNOWN
}MS_LOG_LEVEL;

typedef struct _tag_MSConfParams
{
	MS_LOG_TYPE nLogType;
	MS_LOG_LEVEL nLogLevel;
	unsigned int maxLogFileSize;
	int			port;
	char		ip[16];
}MSConfParams,*MSConfParamsPtr;
void getLocalTime(char* strTime);

int MSLogUnInit();
int MSLogInit(MSLogEv cb,int memsize,MSConfParams* params,char* logFile);
void MSLogEx(MS_LOG_LEVEL,const char *logStr, ...);
void MSLogDumpCB(int type,const char* logMessage,const int size);



typedef struct _tag_MSLOGMGR
{
	MSLogEv logCB;
	char* pMemBuffer;
	unsigned int offset;
	int type;
	int level;
	unsigned int index;
	unsigned int maxFileSize;
	FILE* logfd;
	char fileName[MAX_STR_SIZE];
#ifdef WIN32
	CRITICAL_SECTION lock;
#else
	pthread_mutex_t lock;
#endif
	int port;
	char ip[16];

}MSLOGMGR,*MSLOGMGRPtr;

#ifdef __cplusplus
}
#endif
