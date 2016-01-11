#include  "ms_pu_lib.h"
#include "ms_pu_param.h"

#include "VagManager.h"
#include "TTime.h"
#include "log.h"
#include "GlobalFunc.h"

vag::VAGProxy* g_pVAG = NULL; 


#ifdef __cplusplus
extern "C"
{
#endif

int net_initlib(
				net_info_t info, 
				net_register_proc reg_proc, 
				net_msg_proc msg_proc, 
				net_stream_proc stream_proc, 
				unsigned long param )
{
    printf("************************************************************************\n");
    printf("this is the start\nbuild time is :%s \nversion:%s, %s\n",SOFT_VERSION,__DATE__,__TIME__);
    printf("************************************************************************\n");
    MSConfParams configParams;
	memset(&configParams,0,sizeof(MSConfParams));
	configParams.maxLogFileSize = DEFAULT_LOG_FILE_SIZE;
	configParams.nLogLevel = MS_LOG_LEVEL_INFO;
	configParams.nLogType = MS_LOG_FILE;
	configParams.port = 61000;
	strcpy(configParams.ip, "127.0.0.1");

	int ret = MSLogInit(MSLogDumpCB, DEFAULT_LOG_MEM_SIZE,&configParams,(char*)("/tmp/pusdk.log"));
    if(ret == -1)
    {
        printf("MSLogInit fail\n");
        return -1;
    }
#ifndef WIN32
	void (*handler)(int);
	handler = signal(SIGPIPE, SIG_IGN);
	if (handler != SIG_DFL)
	{
		signal(SIGPIPE, handler);
	}
#endif

	if(NULL == msg_proc || NULL == stream_proc)
	{
        MSLogEx(MS_LOG_LEVEL_WARNING,
		"[%s:%d] net_initlib, msg_proc || stream_proc is NULL!!!",
		__FUNCTION__, __LINE__);

		return ERR_INVALID_PARAM;
	}
    MSLogEx(MS_LOG_LEVEL_INFO,
    "build time is :%s %s\nversion:%s\n",__DATE__,__TIME__,SOFT_VERSION);
	ret = -1;
	g_pVAG = VAG_create();
	if (g_pVAG)
	{
		g_pVAG->SetNotificationSink(msg_proc, stream_proc,param);

		VAGRegInfo vInfo;
		memset(&vInfo, 0, sizeof(VAGRegInfo));
	
		strncpy(vInfo.VagId, info.ms_info.pu_id,sizeof(info.ms_info.pu_id));
		strncpy(vInfo.VagUserName, info.ms_info.name,sizeof(info.ms_info.name));
		strncpy(vInfo.VagPassword, (const char*)info.ms_info.passwd, sizeof(info.ms_info.passwd));
		strncpy(vInfo.VagIp, info.ms_info.local_ip, sizeof(info.ms_info.local_ip));
		strncpy(vInfo.CmugIp, info.ms_info.ip, sizeof(info.ms_info.ip));
		vInfo.CmuPort = info.ms_info.port;

		//strncpy(vInfo.VagPassword, (const char*)info.ms_info.passwd, strlen((const char *)info.ms_info.passwd));

        /*
        printf("vInfo.VagIp:%s\n",vInfo.VagIp);
        printf("vInfo.VagId:%s\n",vInfo.VagId);
        printf("vInfo.VagUserName:%s\n",vInfo.VagUserName);
        printf("vInfo.VagPassword:%s\n",vInfo.VagPassword);
        printf("vInfo.CmugIp:%s\n",vInfo.CmugIp);
        printf("vInfo.CmuPort:%d\n",vInfo.CmuPort);
        */
		ret = g_pVAG->Login(vInfo);
        if(ret != 0)
        {
             MSLogEx(MS_LOG_LEVEL_WARNING,
    		"[%s:%d] g_pVAG->Login fail!!!",
    		__FUNCTION__, __LINE__);
             return -1;
        }
	}
    else
    {
        MSLogEx(MS_LOG_LEVEL_WARNING,
		"[%s:%d] g_pVAG = VAG_create(); fail!!!",
		__FUNCTION__, __LINE__);
        return -1;
    }
    printf("net_initlib success\n");
	return ret;
}

int net_fililib( int select_platform )
{
	int ret = -1;

	if (g_pVAG)
	{
		ret = g_pVAG->Logout();
		VAG_destroy(g_pVAG);
	}

	MSLogUnInit();

#ifndef WIN32
	void (*handler)(int);
	handler = signal(SIGPIPE, SIG_DFL);
	if (handler != SIG_IGN)
	{
		signal(SIGPIPE, handler);
	}
#endif
	return ret;
}

int net_send_alarm(
				   char *cmd, 
				   char *buf, 
				   unsigned long size )
{
    NET_CMU_UPLOAD_ALARM* xx = (NET_CMU_UPLOAD_ALARM* )buf;
    printf("raise alarm buf:<<<%s>>>>\n",buf);
    printf("raise alarm buf:<<<%d>>>>\n",xx->alarm_type);

    //printf("raise alarm buf:<<<%s>>>>\n",cmd);

	int ret = -1;

	if (g_pVAG)
	{
		VAGSdkManager *pSdk = dynamic_cast<VAGSdkManager *>(g_pVAG);

		if(strcmp(cmd, UPLOAD_PU_RAISEALARM) == 0)			///告警上报
		{
			ret = pSdk->NotifyAlarm((AlarmNotification*)buf, size);
		}
	}
	return ret;
}

int net_send_stream(
					unsigned long id, 
					char *buf, 
					unsigned long size, 
					DataType frame_type )
{
	int ret = -1;

	if (g_pVAG == NULL || buf == NULL)
	{
        printf("g_pVAG == NULL || buf == NULL\n");
		MSLogEx(MS_LOG_LEVEL_WARNING,
			"[%s:%d]g_pVAG == NULL || buf == NULL\n",
			__FUNCTION__, __LINE__);
		return ret;
	}
	MSLogEx(MS_LOG_LEVEL_WARNING,
		"[%s:%d] net_send_stream success\n",
		__FUNCTION__, __LINE__);
	comn::AutoCritSec lock(getMediaManager().m_cs);
	NetMediaPtr ptr = getMediaManager().findNetMedia(id);
	if (ptr)
	{
		vag::StreamPacket pkg;
       
		pkg.data = (vag::uint8_t*)buf;
		pkg.size = size;

		pkg.flag = frame_type;		
	
		comn::Time time_now;
		pkg.timestamp = time_now.getSecond();
		
		ret = ptr->writePacket(pkg);
	}
	else
	{
		MSLogEx(MS_LOG_LEVEL_WARNING,
			"[%s:%d] net_send_stream, NetMediaPtr ptr invalid!!!\n",
			__FUNCTION__, __LINE__);
	}
	return ret;
}

int net_send_audio_stream(
						  unsigned long id, 
						  char *buf, 
						  unsigned long size, 
						  DataType frame_type )
{
	return net_send_stream(id,buf,size,frame_type);
}

int net_stop_stream( unsigned long id )
{
	printf("net_stop_stream\n");
	int ret = -1;

	if (g_pVAG)
	{
		VAGSdkManager *pSdk = dynamic_cast<VAGSdkManager *>(g_pVAG);

		printf("find   Net Media :%s\n",(char *)id);
		NetMediaPtr ptr = getMediaManager().findNetMedia(id);

		if (ptr)
		{
#if 0
			StopMediaReq req;
			memset(&req, 0, sizeof(StopMediaReq));

			req.m_videoId = ptr->getStartMediaReqMsg().m_videoId;
			pSdk->getCMvComm().stopNetMedia(req);
#else
            printf("ms_pu_lib:%s\n",ptr->m_sessionId);
			int sessionId = ptr->getSessionId();
			if (sessionId > 0)
			{
				pSdk->getCMvComm().stopMedia(sessionId);
				ret = 0;
			}
			else
			{
				MSLogEx(MS_LOG_LEVEL_WARNING,
					"[%s:%d] net_stop_stream, sessionId == -1!!!",
					__FUNCTION__, __LINE__);
			}
#endif
			
		}
		else
		{
			MSLogEx(MS_LOG_LEVEL_WARNING,
				"[%s:%d] net_stop_stream, findNetMedia error!!!",
				__FUNCTION__, __LINE__);
		}
	}
	return ret;
}

int net_send_voice_intercom(
		unsigned long id, 
		char *buf, 
		int size, 
		DataType frame_type )//前端发来的对讲数据
{
		int ret = -1;

	if (g_pVAG == NULL || buf == NULL)
	{
		return ret;
	}
	comn::AutoCritSec lock(getMediaManager().m_cs);
	NetMediaPtr ptr = getMediaManager().findNetMedia(id);
	if (ptr)
	{
		vag::StreamPacket pkg;

		pkg.data = (vag::uint8_t*)buf;
		pkg.size = size;

		pkg.flag = frame_type;		
	
		comn::Time time_now;
		pkg.timestamp = time_now.getSecond();
		
		ret = ptr->writePacket(pkg);
	}
	else
	{
		//MSLogEx(MS_LOG_LEVEL_WARNING,
		//	"[%s:%d] net_send_stream, NetMediaPtr ptr invalid!!!",
		//	__FUNCTION__, __LINE__);

	}
	return ret;
}

int net_set_voice_intercom_callback(
		net_voice_intercom_proc voice_proc)
{
	if (g_pVAG)
	{
		g_pVAG->SetVoiceIntercomCallBack(voice_proc);
	}
}
int net_recv_voice_intercom(
		unsigned long id, 
		char **buf, 
		int *size, 
		DataType *frame_type )
{
		int ret = -1;

	if (g_pVAG == NULL)
	{
		return ret;
	}

	NetMediaPtr ptr = getMediaManager().findNetMedia(id);
	if (ptr)
	{
		vag::StreamPacket pkg;
		
		ret = ptr->readPacket(pkg);
		
		if(ret <= 0)
			return ret;

		*size = ret;
		
		*frame_type = (DataType)pkg.flag;
		*buf=(char *)pkg.data;
	}
	else
	{
		MSLogEx(MS_LOG_LEVEL_WARNING,
			"[%s:%d] net_send_stream, NetMediaPtr ptr invalid!!!",
			__FUNCTION__, __LINE__);

	}
	return ret;
}


#ifdef __cplusplus
}
#endif
