#ifndef __MS_PU_LIB_H__H
#define __MS_PU_LIB_H__H

#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define     MINGSOFT_PLATFORM  0 //铭软平台
#define     GB28181_PLATFORM   1 //GB28181平台

//帧类型
typedef  enum
{
	NO_I_FRAME_TYPE=0,//非I帧
	I_FRAME_TYPE,//I帧
	G711_A_TYPE,//G711A
	G711_U_TYPE//G711u
}DataType;

    typedef int (* net_register_proc )( 
		int type, 
		char *ip, 
		unsigned short port,
		unsigned long error, 
		unsigned long times, 
		unsigned long param );

    typedef int (* net_msg_proc )(
		char *cmd, 
		char *buf, 
		unsigned long *size, 
		unsigned long param );

    typedef int (* net_alarm_proc )( 
		char *cmd );

    typedef int (* net_stream_proc )( 
		unsigned long id, 
		char *cmd, 
		char *buf, 
		unsigned long size, 
		unsigned long param );

    typedef int (* net_voice_intercom_proc )( 
		unsigned long id, 
		DataType type, 
		char *buf, 
		unsigned long size, 
		unsigned long param );
	
    typedef struct
    {
        char deviceid[128];//系统ID
        char name[128];
        char manufacturer[128];
        char model[128];
        char owner[128];
        char civilcode[128];
        char address[128];
        int parental;
        char parentld[128];
        int registerway;//default  1
        char certnum[128];
        int certifiable;
        int errcode;
        char endtime[128];
        int secrecy;//default 0
        char  status[128];
    } DEVICE_CATALOG_INFO;


    typedef struct
    {
        char deviceid[32];
        char dutystatus[64];
    } DEVICE_ARLARMSTATUS_INFO;

    typedef struct
    {
        char deviceid[32];
    } DEVICE_ALARMOUT_INFO;

    typedef struct
    {
        //上级平台 信息
        int		hcPort;
        char	hcIp[16];
        char	hcName[128];
        char	hcPwd[32];
        char	hcId[32];

        //本代理网关的信息
        int	lcPort;
        char	lcIp[16];
        char	lcName[128];
        char	lcPwd[32];
        char	lcId[32];

        int alarm_num;
        int channel_num;//通道数
        int alarmout_num;

        DEVICE_CATALOG_INFO  **channel_info;//通道名称
        DEVICE_ARLARMSTATUS_INFO  **alarmstatus_info;
        DEVICE_ALARMOUT_INFO  **alarmout_info;

        int     keeplive;
        int     keeplivenum;
        int     expires;
    } sip_net_info;

    typedef struct
    {
        char     ip[16];	    // cmu ip
        unsigned int port;    // cmu port
        char pu_id[32];
        char name[32];
        unsigned char passwd[32];
        char local_ip[20];
        unsigned long pu_type;  //0-DVS 1-DVR 2-IPC 3-OTHER
        unsigned long company;  //
    } ms_net_info;

    typedef struct
    {
        int select_platform;
        ms_net_info  ms_info;
        sip_net_info sip_net;
    } net_info_t;

    int net_initlib(
		net_info_t info, 
		net_register_proc register_proc, 
		net_msg_proc msg_proc, 
		net_stream_proc stream_proc, 
		unsigned long param );

    int net_fililib(
		int select_platform );

    int net_send_alarm(
		char *cmd, 
		char *buf, 
		unsigned long size );

    int net_send_stream(
		unsigned long id, 
		char *buf, 
		unsigned long size, 
		DataType frame_type );

    int net_send_audio_stream(
		unsigned long id, 
		char *buf, 
		unsigned long size, 
		DataType frame_type );

    int net_stop_stream(
		unsigned long id );
/*注册对讲回调*/
	int net_set_voice_intercom_callback(
		net_voice_intercom_proc voice_proc);
	
//语音对讲
	int net_send_voice_intercom(
		unsigned long id, 
		char *buf, 
		int size, 
		DataType frame_type );

#ifdef __cplusplus
}
#endif

#endif//__MS_PU_LIB_H__

