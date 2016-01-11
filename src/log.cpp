#include <stdarg.h>
#include <time.h>
#include "log.h"


#ifdef WIN32
typedef SOCKET socket_t;
#else
typedef int socket_t;
#endif // WIN32


#ifdef __cplusplus
extern "C" {
#endif

static MSLOGMGR g_MSLog;
static socket_t g_sock = -1;
struct sockaddr_in lServerAddr;


void initLock(void* pLock)
{
#ifdef WIN32
	InitializeCriticalSection((CRITICAL_SECTION *)pLock);
#else
	pthread_mutex_init((pthread_mutex_t *)pLock,NULL);
#endif
}

void unInitLock(void* pLock)
{
#ifdef WIN32
	DeleteCriticalSection((CRITICAL_SECTION *)pLock);
#else
	pthread_mutex_destroy((pthread_mutex_t *)pLock);
#endif
}

void lock(void* pLock)
{
#ifdef WIN32
	EnterCriticalSection((CRITICAL_SECTION *)pLock);
#else
	pthread_mutex_lock((pthread_mutex_t *)pLock);
#endif
}

void unLock(void* pLock)
{
#ifdef WIN32
	LeaveCriticalSection((CRITICAL_SECTION *)pLock);
#else
	pthread_mutex_unlock((pthread_mutex_t *)pLock);
#endif
}

void getLocalTime(char* strTime)
{
	time_t now;
	struct tm  *timenow;
	if (strTime == NULL)
		return;
	time(&now);

	timenow = localtime(&now);

	char* p = asctime(timenow);
	memcpy(strTime,p,strlen(p) - 1);
}

void LogDump(FILE* logfd,char* pBuffer,const char* logMessage,int size)
{
}

int MSVsnprintf(
	char* s,
	size_t len,
	const char* format,
	va_list arg)
{
	return vsnprintf(s, len, format, arg);
}

int checkLogDirectory(char* folder)
{
	struct stat statbuf;

#ifdef WIN32
	if (0 != stat(folder,&statbuf))
	{
		if (_mkdir(folder) < 0)
		{
			printf("can not create folder{%s} in current path\n",folder);
			return -1;
		}
	}
#else
	if (0 != stat(folder,&statbuf))
	{
		if (ENOENT == errno)
		{
			if (-1 == mkdir(folder,S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH|S_IXOTH))
			{
				printf("failed to create folder : %s\n",folder);
				return -1;
			}
			chmod("/tmp/",S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH|S_IXOTH);
		}
		else
		{
			printf("can not create folder{%s} in current path\n",folder);
			return -1;
		}
	}
#endif
	
	return 0;
}

int checkLogFileSize()
{
	unsigned int nFileSize = 0;
	char fileName[MAX_STR_SIZE] = "\0";

	struct stat buf;

	stat(g_MSLog.fileName,&buf);
	nFileSize = buf.st_size;

	if (nFileSize >= g_MSLog.maxFileSize)
	{
		if (checkLogDirectory((char *)LOG_FOLDER) != 0)
			return -1;
		g_MSLog.index ++;
		if (g_MSLog.index > MAX_LOG_FILE_INDEX)
			g_MSLog.index = 0;
		sprintf(fileName,"%s%d%s","/tmp/pusdk",g_MSLog.index,".log");
		
		g_MSLog.logfd = fopen(fileName,"ab+");
	
		if (NULL == g_MSLog.logfd)
			return -1;
		memset(g_MSLog.fileName,0,sizeof(g_MSLog.fileName));
		strcpy(g_MSLog.fileName,fileName);
	}
	

	return 0;
}

void MSLogDumpCB(int type,const char* logMessage,const int size)
{
	char strTime[128] = "\0";
	char strLog[4096] = "\0";
	memset(strTime,0,sizeof(strTime));
	memset(strLog,0,sizeof(strLog));
	getLocalTime(strTime);

	sprintf(strLog,"%s: %s\n",strTime,logMessage);
	
	switch((MS_LOG_TYPE)type)
	{
	case MS_LOG_NONE:
		break;
	case MS_LOG_SCREEN:
		{
			printf("%s",strLog);
		}
		break;
	case MS_LOG_FILE:
		{
			//here check the current log file's size,if big than 5M,change to next one
			if (checkLogFileSize() != 0)
				return;
			fwrite(strLog,1,strlen(strLog),g_MSLog.logfd);
			fflush(g_MSLog.logfd);
		}
		break;
	case MS_LOG_NET:
		{
			int ret = sendto(g_sock, strLog, strlen(strLog), 0,
				(struct sockaddr *)&lServerAddr, sizeof(lServerAddr));
		}
#if 0
	case MS_LOG_MEMORY:
		{
			LogDump(g_MSLog.logfd,g_MSLog.pMemBuffer,strLog,strlen(strLog));
		}
		break;
#endif

	default:
		{
			//here check the current log file's size,if big than 5M,change to next one
			if (checkLogFileSize() != 0)
				return;
			fwrite(strLog,1,strlen(strLog),g_MSLog.logfd);
			fflush(g_MSLog.logfd);
		}
		break;
	}
}

void MSLogEx(MS_LOG_LEVEL level,const char *logStr, ...)
{
	lock(&(g_MSLog.lock));
	char msg[2048] = "\0";
	va_list varArg;

	//firstly check the current line's level with g_MSLog.level
	if (level > g_MSLog.level)
	{
		unLock(&(g_MSLog.lock));
		return;
	}

	memset(msg,0,sizeof(msg));
	va_start(varArg, logStr);
	MSVsnprintf(msg, sizeof(msg), logStr, varArg);
	va_end(varArg);
	g_MSLog.logCB(g_MSLog.type,msg,strlen(msg));
	unLock(&(g_MSLog.lock));
}

int MSLogUnInit()
{
	unInitLock(&g_MSLog.lock);
	if(g_MSLog.logfd)
		fclose(g_MSLog.logfd);
	if(g_MSLog.pMemBuffer)
		free(g_MSLog.pMemBuffer);

#if defined(WIN32)
	::closesocket(g_sock);
#else
	::close(g_sock);
#endif
	g_sock = -1;

	memset(&g_MSLog,0,sizeof(MSLOGMGR));

#ifdef WIN32                                   
	if(WSACleanup() == SOCKET_ERROR)
	{
		if(WSAGetLastError() == WSAEINPROGRESS)
		{
			WSACancelBlockingCall();
			WSACleanup();
		}
	}
#else
#endif 
	return 0;
}

int MSLogInit(MSLogEv cb,int memsize,MSConfParams* params,char* logFile)
{
#ifdef WIN32
	WORD versionWanted = MAKEWORD(2,2);
	WSADATA wsaData;
	WSAStartup(versionWanted, &wsaData);
#else
#endif

	memset(&g_MSLog,0,sizeof(MSLOGMGR));

	g_MSLog.port = params->port;
	strcpy(g_MSLog.ip, params->ip);

	g_MSLog.logCB = cb;
	g_MSLog.type = params->nLogType;
	g_MSLog.level = params->nLogLevel;
	g_MSLog.index = 0;
	g_MSLog.maxFileSize = params->maxLogFileSize;
	strcpy(g_MSLog.fileName,logFile);

	if (NULL == logFile)
	{
        printf("[%s:%d] NULL == logFile!!!",
		__FUNCTION__, __LINE__);
		return -1;

    }

	if (strlen(logFile) == 0)
	{
        printf("[%s:%d] strlen(logFile) == 0!!!",
		__FUNCTION__, __LINE__);
		return -1;

    }

	if (checkLogDirectory((char *)LOG_FOLDER) != 0)
	{
        printf("[%s:%d] checkLogDirectory(LOG_FOLDER) != 0!!!",
		__FUNCTION__, __LINE__);
		return -1;

    }

	g_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(g_sock <= 0)
	{
        printf("[%s:%d] g_sock <= 0!!!",
		__FUNCTION__, __LINE__);
		return -1;

    }
	lServerAddr.sin_family = AF_INET;
	lServerAddr.sin_port = htons(g_MSLog.port);
	lServerAddr.sin_addr.s_addr = inet_addr(g_MSLog.ip);

	g_MSLog.pMemBuffer = (char *)malloc(memsize);
	if (NULL == g_MSLog.pMemBuffer)
	{
        printf("[%s:%d] NULL == g_MSLog.pMemBuffer!!!",
		__FUNCTION__, __LINE__);
		return -1;

    }

	g_MSLog.logfd = fopen(logFile,"ab+");
	if (NULL == g_MSLog.logfd)
	{
		if(g_MSLog.pMemBuffer)
          	free(g_MSLog.pMemBuffer);
        else
	    {
            printf("[%s:%d] NULL == g_MSLog.pMemBuffer!!!",
    		__FUNCTION__, __LINE__);

        }
        printf("open logFile error\n");
        return -1;
	}

	initLock(&g_MSLog.lock);
	return 0;
}

#ifdef __cplusplus
}
#endif

