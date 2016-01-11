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
    sprintf(info->deviceid, "%s%d",channel_id,number);//�豸/����/ϵͳ����
    number++;
    strcpy(info->name, "Camera 1");//�豸/����/ϵͳ����
    strcpy(info->manufacturer, "Abell");//�豸����
    strcpy(info->model, "Abell");//�豸�ͺ�
    strcpy(info->owner, "Abell");//�豸����

    strcpy(info->civilcode, "Abell");//

    strcpy(info->address, "Abell");

    info->parental = 0;

    info->registerway = 1;//ע�᷽ʽ

    info->secrecy = 0;
    strcpy(info->status, "Status 1");//�豸״̬
    return 0;
}

//�������ļ�
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

//�澯�ϱ�
int alarm_proc(net_info_t info, int t, int select)
{
    char *cmd = UPLOAD_PU_RAISEALARM;//�豸�澯�ϱ�

    NET_CMU_UPLOAD_ALARM  ptr_param;//�澯��Ϣ

    int type = 0;
    char pszDate[32] = "\0"; //��ű���ʱ��Ļ�����
    char data[64];//��������ϸ��Ϣ

#ifdef WIN32
	SYSTEMTIME st;
#else
    struct tm *ptm;
    time_t TimeFile;
#endif

    //Ŀǰֻʵ�� VS_GPIN���������澯��CAM_VIDEOLOSE����Ƶ��ʧ ��CAM_HIDEDETECT���ڵ��澯


    //NET_CMU_UPLOAD_ALARM �ϴ��澯�Ľṹ��
    memset(&ptr_param, 0, sizeof(NET_CMU_UPLOAD_ALARM));

    // �������� �澯ʱ��
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
		st.wSecond); //������ʱ��
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
		ptm->tm_sec); //������ʱ��
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
    //Ȼ������澯����
    if(t == 1)
        type = VS_GPIN;//CAM_HIDEDETECT;//Ŀǰֻʵ�� VS_GPIN���������澯��CAM_VIDEOLOSE����Ƶ��ʧ ��CAM_HIDEDETECT���ڵ��澯CAM_MOTIONDETECT�ƶ����
    else if(t == 2)
        type  = CAM_VIDEOLOSE;
    else if(t == 3)
        type = CAM_HIDEDETECT;
    else if( t == 4)
        type = VCA_INTRUSION;
    else
        type = CAM_MOTIONDETECT;

    ptr_param.alarm_type = type;//��������

    //�����Ӳ�̴�������Ӳ�̺�
    ptr_param.disk_number = 0; //������Ӳ�̺�

    strcpy(data, "1");
    /*�澯����ϸ��Ϣ����һ���ַ���
      strcpy(data,"d:70%");
      if((strlen(data) + 1) < 64)//������ϸ��Ϣ���Ȳ��ܳ���64���ֽ�
      strcpy(ptr_param.data,data);

    //������net_send_alarm,�ϴ��澯��Ϣ*/
        //�澯���豸����
    if(type == VS_GPIN)
    {
        strncpy(ptr_param.server_type,"VS",32);//�澯�豸����
    }
    else
    {
        strncpy(ptr_param.server_type,"CAM",32);//�澯�豸����
    }

    strcpy(ptr_param.eliminated, "false"); //�澯����
    ptr_param.data.integerValue = 1;//ͨ���Ŷ��Ǵ�1��ʼ
    ptr_param.select_platform = MINGSOFT_PLATFORM;
    net_send_alarm(cmd, (char *)&ptr_param, sizeof(NET_CMU_UPLOAD_ALARM)); //�澯�ϱ�
    return 0;
}

int main(int argc, char *argv[])
{

#ifdef WIN32
#else
    struct rlimit r;
    r.rlim_cur = -1;
    r.rlim_max = -1;
    //�ó������ʱ�����һ��core�ļ������ڷ���
    if (setrlimit(RLIMIT_CORE, &r) < 0) 
    {
        fprintf(stderr, "setrlimit error\n");
        exit(1);
    }
    
    /*���ιܵ��Ͽ��ź�*/
	signal(SIGHUP, SIG_IGN);	
#endif

    net_info_t info;
    memset(&info, 0, sizeof(net_info_t));

    info.select_platform = MINGSOFT_PLATFORM;
    if(MINGSOFT_PLATFORM == info.select_platform)
    {
        /*�������ļ���ȡ��Ϣ�����info*/
        ms_read_conf(&info);
        //��ʼ��SDK
        int result = net_initlib(info, NULL, on_msg, on_stream, 0); 
        if(0 != result  )
        {
            printf("error num:%d\n",result); 
        }        //ע�������Խ��Ļص�����
        result = net_set_voice_intercom_callback(on_audio_stream);
        if(0 != result  )
        {
            printf("error num:%d\n",result); 
        }       

    }

    /*�澯����*/
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
    
    //��Դ�ͷ�
    net_fililib(MINGSOFT_PLATFORM);
    return 0;
}


