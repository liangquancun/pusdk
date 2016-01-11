#include "stdafx.h"
#include "arm.h"
#include "ms_pu_lib.h"
#include "ms_pu_param.h"

#include <signal.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/socket.h>

#ifdef WIN32
#else
#include <time.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>
#endif

CHILD_PROCESS_INFO_S alarm_ChildInfo[8];

char local_ip[20];
char channel_id[19];
char alarm_id[20];

int fill_channel_info(void *info_t)
{
    static int number = 0;
    DEVICE_CATALOG_INFO *info = (DEVICE_CATALOG_INFO *)info_t;
    sprintf(info->deviceid, "%s%d",channel_id,number);//设备/区域/系统编码
    number++;
    strcpy(info->name, "Camera 1");//设备/区域/系统名称
    strcpy(info->manufacturer, "Abell");//设备厂商
    strcpy(info->model, "Abell");//设备型号
    strcpy(info->owner, "Abell");//设备归属

    strcpy(info->civilcode, "Abell");//

    strcpy(info->address, "Abell");

    info->parental = 0;

    info->registerway = 1;//注册方式

    info->secrecy = 0;
    strcpy(info->status, "Status 1");//设备状态
    return 0;
}

//读配置文件
int ms_read_conf(void *info)
{
    char line[100];
    FILE *file;
    net_info_t *s_info = (net_info_t *)info;
    file = fopen("./ms_pusdk.conf", "r");
	if(file == NULL)
		exit(0);
    while(fgets(line, 100, file))
    {
        line[strlen(line)-1] = '\0'; //add a flag of string end
        if(!memcmp("pu_id=", line, strlen("pu_id="))) //if find username
        {
            memcpy((*s_info).ms_info.pu_id, (line + sizeof("pu_id")), sizeof((*s_info).ms_info.pu_id));
            //printf("++++++++++++++++++++%s\n",(*s_info).ms_info.pu_id);
            continue;
        }
        if(!memcmp("name=", line, strlen("name=")))
        {
            memcpy((*s_info).ms_info.name, (line + sizeof("name")), sizeof((*s_info).ms_info.name));
            //printf("********\n");
            continue;
        }
        if(!memcmp("passwd=", line, strlen("passwd=")))
        {
            memcpy((*s_info).ms_info.passwd, (unsigned char *)(line + sizeof("passwd")), sizeof((*s_info).ms_info.passwd));
            continue;
        }
        if(!memcmp("ip=", line, strlen("ip=")))
        {
            memcpy((*s_info).ms_info.ip, (unsigned char *)(line + sizeof("ip")), sizeof((*s_info).ms_info.ip));
            continue;
        }
        if(!memcmp("port=", line, strlen("port=")))
        {
            (*s_info).ms_info.port = (unsigned int)atoi((const char *)(line + sizeof("port")));
            continue;
        }
        if(!memcmp("local_ip=", line, strlen("local_ip=")))
        {
            memcpy((*s_info).ms_info.local_ip, (unsigned char *)(line + sizeof("local_ip")), sizeof((*s_info).ms_info.local_ip));
            continue;
        }

        if(!memcmp("pu_type=", line, strlen("pu_type=")))
        {
            (*s_info).ms_info.pu_type = (unsigned int)atoi((const char *)(line + sizeof("pu_type")));
            continue;
        }
        if(!memcmp("company=", line, strlen("company=")))
        {
            (*s_info).ms_info.company = (unsigned int)atoi((const char *)(line + sizeof("company")));
            break;
        }

    }
    fclose(file);
    file = NULL;
    return 0;
}

//告警上报
int alarm_proc(net_info_t info, int t, int select)
{
    char *cmd = UPLOAD_PU_RAISEALARM;//设备告警上报

    NET_CMU_UPLOAD_ALARM  ptr_param;//告警信息

    int type = 0;
    char pszDate[32] = "\0"; //存放报警时间的缓冲区
    char data[64];//报警的详细信息

#ifdef WIN32
	SYSTEMTIME st;
#else
    struct tm *ptm;
    time_t TimeFile;
#endif

    //目前只实现 VS_GPIN：开关量告警，CAM_VIDEOLOSE：视频丢失 ，CAM_HIDEDETECT：遮挡告警


    //NET_CMU_UPLOAD_ALARM 上传告警的结构体
    memset(&ptr_param, 0, sizeof(NET_CMU_UPLOAD_ALARM));

    // 首先填入 告警时间
    memset(&pszDate, 0, sizeof(pszDate));

#ifdef WIN32
	GetLocalTime(&st);
	sprintf(pszDate, 
		"%04d-%02d-%02d %02d:%02d:%02d", 
		st.wYear, 
		st.wMonth, 
		st.wDay, 
		st.wHour, 
		st.wMinute, 
		st.wSecond); //报警的时间
#else
	TimeFile = time(NULL);
    ptm = localtime(&TimeFile);
    sprintf(pszDate, 
		"%04d-%02d-%02d %02d:%02d:%02d", 
		ptm->tm_year + 1900, 
		ptm->tm_mon + 1, 
		ptm->tm_mday, 
		ptm->tm_hour, 
		ptm->tm_min, 
		ptm->tm_sec); //报警的时间
#endif
	
	strncpy(ptr_param.time_stamp, pszDate, sizeof(pszDate));

    if(MINGSOFT_PLATFORM == select)
    {
        strncpy(ptr_param.pu_id, info.ms_info.pu_id, sizeof(info.ms_info.pu_id));
        strncpy(ptr_param.pu_ip, info.ms_info.ip, sizeof(info.ms_info.ip));
    }
    else
    {
        printf("select = %d\n", select);
        return 0;
    }
    //然后填入告警类型
    if(t == 1)
        type = VS_GPIN;//CAM_HIDEDETECT;//目前只实现 VS_GPIN：开关量告警，CAM_VIDEOLOSE：视频丢失 ，CAM_HIDEDETECT：遮挡告警CAM_MOTIONDETECT移动侦测
    else if(t == 2)
        type  = CAM_VIDEOLOSE;
    else if(t == 3)
        type = CAM_HIDEDETECT;
    else if( t == 4)
        type = VCA_INTRUSION;
    else
        type = CAM_MOTIONDETECT;

    ptr_param.alarm_type = type;//报警类型

    //如果是硬盘错误，填入硬盘号
    ptr_param.disk_number = 0; //报警的硬盘号

    strcpy(data, "1");
    /*告警的详细信息，是一个字符串
      strcpy(data,"d:70%");
      if((strlen(data) + 1) < 64)//报警详细信息长度不能超过64个字节
      strcpy(ptr_param.data,data);

    //最后调用net_send_alarm,上传告警信息*/
        //告警的设备类型
    if(type == VS_GPIN)
    {
        strncpy(ptr_param.server_type,"VS",32);//告警设备类型
    }
    else
    {
        strncpy(ptr_param.server_type,"CAM",32);//告警设备类型
    }

    strcpy(ptr_param.eliminated, "false"); //告警发生
    ptr_param.data.integerValue = 1;//通道号都是从1开始
    ptr_param.select_platform = MINGSOFT_PLATFORM;
    net_send_alarm(cmd, (char *)&ptr_param, sizeof(NET_CMU_UPLOAD_ALARM)); //告警上报
    return 0;
}

int main(int argc, char *argv[])
{

#ifdef WIN32
#else
    struct rlimit r;
    r.rlim_cur = -1;
    r.rlim_max = -1;
    //让程序崩溃时候产生一个core文件，便于分析
    if (setrlimit(RLIMIT_CORE, &r) < 0) 
    {
        fprintf(stderr, "setrlimit error\n");
        exit(1);
    }
    
    /*屏蔽管道断开信号*/
	signal(SIGHUP, SIG_IGN);	
#endif

    net_info_t info;
    memset(&info, 0, sizeof(net_info_t));

    info.select_platform = MINGSOFT_PLATFORM;
    if(MINGSOFT_PLATFORM == info.select_platform)
    {
        /*从配置文件读取信息，填充info*/
        ms_read_conf(&info);
        //初始化SDK
        int result = net_initlib(info, NULL, on_msg, on_stream, 0); 
        if(0 != result  )
        {
            printf("error num:%d\n",result); 
        }        //注册语音对讲的回调函数
        result = net_set_voice_intercom_callback(on_audio_stream);
        if(0 != result  )
        {
            printf("error num:%d\n",result); 
        }       

    }

    /*告警测试*/
    int input = 4;
    while( 1 )
    {
       alarm_proc(info,input,MINGSOFT_PLATFORM);
#ifdef WIN32
		Sleep(10 * 1000);
#else
        sleep(5);
#endif
    }
    
    //资源释放
    net_fililib(MINGSOFT_PLATFORM);
    return 0;
}


