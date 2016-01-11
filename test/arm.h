#ifndef __ARM_H__
#define __ARM_H__
#include "stdafx.h"
#include <stdio.h>


#ifdef WIN32
#include <Winsock2.h>
#include <io.h>
#endif
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ms_pu_lib.h"

#define DEFAULT_DATA_BUFFER_SIZE	(1024*4+24)

#ifdef WIN32
#define TEST_MEDIA_FILE ".\\test-%d.dat"
#define TEST_AUDIO_FILE ".\\test-audio%d.dat"
#else
#define TEST_MEDIA_FILE "../test-%d.dat"
#define TEST_AUDIO_FILE "../test-audio%d.dat"
#endif
typedef enum
{
    R_NOCREATE,
    R_Accept,
    R_Worked,
    R_Error
} runningStatus;

typedef struct
{
    unsigned  int flag;
    unsigned  int size;
} NAL_HEADER;

typedef struct
{
    FILE *ptr_file;
#ifdef WIN32
	HANDLE pid;
#else
    pthread_t pid;
#endif
    void *thrd;
    int   flag;
    int   video_id;
    int   media_type;
    int   stream_type;
    int   audio_skt;
    char filename[64];
    unsigned long id;
    int count;
} NET_STREAM_OPEN_PARAM, *LPNET_STREAM_OPEN_PARAM;

typedef struct
{
    void *child_pid;
    runningStatus child_status;
    int  alarm_type;
} CHILD_PROCESS_INFO_S, *LCHILD_PROCESS_INFO_S;

int fill_channel_info(void *info_t);
int on_login ( int type, char *ip, unsigned int port, unsigned long error, unsigned long times, unsigned long param  );
int on_msg   ( char *cmd, char *buf, unsigned long *size, unsigned long param );
int on_stream( unsigned long id, char *cmd, char *buf, unsigned long size, unsigned long param );
int ms_read_conf(void *info);
int on_audio_stream (long unsigned id, DataType type, char *buf, unsigned long size, unsigned long param );
//char pu_id[30];
#endif
