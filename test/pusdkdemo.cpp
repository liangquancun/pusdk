#include "stdafx.h"
#include <signal.h>

#include "ms_pu_lib.h"
#include "ms_pu_param.h"
#include "arm.h"

#ifdef WIN32
#else
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "time.h"
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#endif



typedef struct media_list{
    NET_STREAM_OPEN_PARAM data;
    struct media_list * next;
}media_list_t;
media_list_t *head = NULL; 
//g_sleep_time�ǿ�ţ����ŵ�ʱ�򣬿��Ʒ������ʵģ�demo�м򵥵�ʵ��
int g_sleep_time = 1000000 / 25;

//��������Demo
void *stream_proc(void *param);
void *audio_stream_proc(void *param);
int on_audio_stream (long id, int type, char *buf, unsigned long size, unsigned long param );


media_list_t *mk_node(NET_STREAM_OPEN_PARAM data)
{   
    media_list_t *p = (media_list_t *)malloc(sizeof(media_list_t));  
    if(p != NULL)
    {      
        p->data = data;     
        p->next = NULL; 
    }   
    return p;
}

media_list_t *link_insert_head(media_list_t *head, NET_STREAM_OPEN_PARAM data)
{  
    media_list_t *newp;   
    newp = mk_node(data);   
    if(head == NULL || newp == NULL)
    {       
        if(head == NULL)           
            head = newp;        
        return head;   

    }   
    newp->next = head; 
    head = newp;    
    return head;

}

int link_search(media_list_t*head, unsigned long data)
{   
    media_list_t*cur;    
    for(cur = head; cur; cur = cur->next)
    {      
        if(strcmp((char *)(cur->data.id),(char *)data)  == NULL)
        {
            return 0;
        }
        printf("find:%d\n", cur->data); 

    }
    return -1;

}

media_list_t* link_remove(media_list_t **headp, unsigned long data)
{   
    media_list_t  *rmvp, *pre, *cur;   
    if(*headp == NULL)
    {
        return NULL;   

    }   
    if(strcmp((char *)((*headp)->data.id),(char *)data)  == NULL)
    {     
        rmvp = *headp;     
        *headp = (*headp)->next;        
        rmvp->next = NULL;     
        return rmvp;    

    }   
    for(pre = *headp, cur = (*headp)->next; cur; pre = pre->next, cur = cur->next)  
    {       
        if(strcmp((char *)(cur->data.id),(char *)data)  == NULL)
        {          
            rmvp = cur;        
            pre->next = cur->next;         
            rmvp->next = NULL;         
            return rmvp;        
        }   

    }   
    return NULL;

}

int on_login( int type, char *ip, unsigned int port, unsigned long error, unsigned long times, unsigned long param  )
{
    return 0;
}

int on_msg( char *cmd, char *buf, unsigned long *size, unsigned long param )//��Ϣ����,����size,param��ʱδ��
{
    //printf("on_msg cmd ++++++++++++%s\n",cmd);
    //�����豸ʱ�����
    if(strcmp(cmd, SET_PU_TIME) == 0)
    {
        //LPNET_CMU_DEV_TIME basic = (LPNET_CMU_DEV_TIME)buf;
        /*
           typedef struct
           {
           int hour;		// ʱ (0~23)
           int minute;		// �� (0~59)
           int second;		// �� (0~59)
           int date;		// �� (1~31)
           int month;		// �� (1~12)
           int year;		// �� nnnn
           }NET_CMU_DEV_TIME, *LPNET_CMU_DEV_TIME;
           */

        /*	printf("hour is %d\n",basic->hour);
        	printf("minute is %d\n",basic->minute);
        	printf("second is %d\n",basic->second);
        	printf("date is %d\n",basic->date);
        	printf("month is %d\n",basic->month);
        	printf("year is %d\n", basic->year);
        	*/
        printf("SET_PU_TIME %s ++++++\n", SET_PU_TIME);
    }

    //��ȡ�豸�汾
    else if(strcmp(cmd, GET_PU_SOFTWAREVERSION) == 0)
    {
        LPNET_CMU_SOFT_VERSION basic = (LPNET_CMU_SOFT_VERSION)buf;
        strncpy(basic->pu_id, "10002", 32);
        strncpy(basic->version, "10.1.1", 32);
        strncpy(basic->build_time, "2001-03-23 23:34:34", 32);
    }

    //�����豸�������
    else if(strcmp(cmd, SET_PU_IPINFO) == 0)
    {
        LPNET_CMU_IP_INFO basic = (LPNET_CMU_IP_INFO)buf;
        /*
           typedef struct
           {
           int   net_id;				// ���ڱ�� (1,2,3...)
           char  ip_addr[15];			// IP��ַ
           int   ctrl_port;			    // ҵ����ƶ˿ں�
           char  net_mask[15];			// ��������
           char  dns_addr[2][15];		// DNS��������ַ,�����ж��
           char  gate_way[15];			// ���ص�ַ
           }NET_CMU_IP_INFO, *LPNET_CMU_IP_INFO;
           */
        printf("netId is %d\n", basic->net_id);
        printf("ip_addr is %s\n", basic->ip_addr);
        printf("ctrl_port is %d\n", basic->ctrl_port);
        printf("net_mask is %s\n", basic->net_mask);
        printf("dns_addr is %s\n", basic->ip_addr);
        printf("gate_way is %s\n", basic->gate_way);
    }

    //��ȡ�豸�������
    else if(strcmp(cmd, GET_PU_IPINFO) == 0)
    {
        LPNET_CMU_IP_INFO basic = (LPNET_CMU_IP_INFO)buf;
        /*
           typedef struct
           {
           int   net_id;	            // ���ڱ�� (1,2,3...)
           char  ip_addr[15];           // IP��ַ
           int   ctrl_port;		        // ҵ����ƶ˿ں�
           char  net_mask[15];	        // ��������
           char  dns_addr[2][15];		// DNS��������ַ,�����ж��
           char  gate_way[15];			// ���ص�ַ
           }NET_CMU_IP_INFO, *LPNET_CMU_IP_INFO;
           */

        basic->net_id = 2;
        strncpy(basic->ip_addr, "192.168.2.72", 15);
        basic->ctrl_port = 1001;
        strncpy(basic->net_mask, "255.255.255.0", 15);
        strncpy(basic->dns_addr[0], "192.168.1.1", 15);
        strncpy(basic->gate_way, "192.168.1.1", 15);

    }

    //������̨
    else if(strcmp(cmd, OPER_PU_CONTROLPTZ) == 0)
    {
        LPNET_CMU_OPER_CTRL_PTZ basic = (LPNET_CMU_OPER_CTRL_PTZ)buf;
        /*
           typedef struct
           {
           int  ptz_id;	��̨��ʶ

           char cmd[32];
           ��������/��Զ��ZIN/ZOUT��
           �������/��Զ��FN/FR��
           ��Ȧ�Զ�����/����/��С��IA_ON/IO/IC��
           ����������WP_ON��
           �ƹ⿪����LP_ON
           Ѳ��������AUTO_START/AUTO_STOP��
           ��������� ��TU,��TD,��PL,��PR, ����TUPR , ����TUPL, ����TDPR, ����TDPL,
           ת��Ԥ��㣺GOTO_PRESET
           �켣Ѳ����
           ��ʼѲ����START_TRACK_CRUISE
           ֹͣ��STOP

           char param[200];
           ��cmd��GOTO_PRESETʱ���ò�����ʾԤ�����š�
           ��cmd��START_TRACK_CRUISE�ò�����Ѳ���켣������
           ��cmd��STOPʱ���ò���ָ��Ҫֹͣ�Ŀ��������������̨�˶������

           int  speed;// ��̨�ٶ�	1-8����8���ȼ�����ֵԽ���ٶ�Խ�졣

           }NET_CMU_OPER_CTRL_PTZ, *LPNET_CMU_OPER_CTRL_PTZ;
           */

        printf("ptz_id is%d \n", basic->ptz_id); //��̨��ʶ
        printf("cmd is%s\n", basic->cmd);
        printf("param is%s\n", basic->param);
        printf("seep is%d\n", basic->speed); //��̨�ٶ�
    }


    //������̨Ԥ�õ�
    else if(strcmp(cmd, SET_PU_PRESETPTZ) == 0)
    {

        LPNET_CMU_PRESET_LIST basic = (LPNET_CMU_PRESET_LIST)buf;
        /*
           typedef struct
           {
           int  preset_index;					// Ԥ�õ���
           char preset_name[16];				// Ԥ�õ�����
           }NET_CMU_PRESET, *LPNET_CMU_PRESET;

           typedef struct						// �豸��̨Ԥ�õ�
           {
           int ptz_id;							// ��̨��ʶ
           int preset_list_cnt;					//
           NET_CMU_PRESET *preset_lists;
           }NET_CMU_PRESET_LIST, *LPNET_CMU_PRESET_LIST;
           */
        printf("basic->ptz_id %d\n", basic->ptz_id); //��̨��ʶ
        printf("basic->preset_lists->preset_index%d\n", basic->preset_lists->preset_index); //Ԥ�õ�����
        printf("basic->preset_lists->preset_name%s\n", basic->preset_lists->preset_name); //Ԥ�õ�����

    }
    //ɾ����̨Ԥ�õ�
    else if(strcmp(cmd, OPER_PU_DELPRESETPTZ ) == 0)
    {
        // LPNET_CMU_OPER_DEL_PRESET basic = (LPNET_CMU_OPER_DEL_PRESET)buf;

    }
    //��ѯ¼���ļ�
    else if(strcmp(cmd, OPER_PU_QUERYPULOCALSTORAGEFILES) == 0)
    {
        LPNET_CMU_QUERY_STORE_FILE basic = (LPNET_CMU_QUERY_STORE_FILE)buf;
        int cnt = 10;
        int num = 0;
        int i = 1;
        char fileName[64] = {0};
        //char startTime[20]={0};
        //char endTime[20]={0};

        printf("video_id is %d \n", basic->video_id); //��ѯ�ļ�����Ƶͨ��
        printf("file_type is %d\n", basic->file_type); //�����ѯ�ļ�������
        printf("file_type is %s \n",basic->type);
        printf("from_date is %04d-%02d-%02d %02d:%02d:%02d\n",
               basic->from_date.year,
               basic->from_date.month,
               basic->from_date.date,
               basic->from_date.hour,
               basic->from_date.minute,
               basic->from_date.second);//��ʼ�����ڼ�ʱ��

        printf("from_date is %04d-%02d-%02d %02d:%02d:%02d\n",
               basic->to_date.year,
               basic->to_date.month,
               basic->to_date.date,
               basic->to_date.hour,
               basic->to_date.minute,
               basic->to_date.second);//��ʼ�����ڼ�ʱ��

        //basic->record_file_lists = (LPNET_CMU_RECORD_FILE)malloc(cnt * sizeof(NET_CMU_RECORD_FILE));
        printf("+_++++++++++++++++++++++++++++++++++++++++++++%d  %d\n", cnt * sizeof(NET_CMU_RECORD_FILE), sizeof(NET_CMU_QUERY_STORE_FILE));
        //basic->record_file_size = 4096;
        basic->file_list_cnt = cnt;

        basic->total_size = 1000;//DISK ������С
        basic->free_size = 1000;//DISKʣ������
        basic->total_num = 100;//�ܹ��ļ�����
        basic->leave_num = 20;//ʣ���ļ�����
        basic->result_code = 0;//ע���� ��code��: 0��OK  1����֧�ֵ��ļ����� 48���ͻ��˴���	49������������ 50������ʧ��ԭ��

        for(; num < cnt; num++, i++)
        {
            sprintf(fileName, "./test.h264");
            memset( basic->record_file_lists[num].file_name, 0, sizeof(basic->record_file_lists[num].file_name));
            sprintf(basic->record_file_lists[num].file_name, "../store-%d.txt", i);
            basic->record_file_lists[num].start_time.year = 2013;
            basic->record_file_lists[num].start_time.month = 3;
            basic->record_file_lists[num].start_time.date = 1;
            basic->record_file_lists[num].start_time.hour = 15;
            basic->record_file_lists[num].start_time.minute = 45;
            basic->record_file_lists[num].start_time.second = 12;


            basic->record_file_lists[num].stop_time.year = 2013;
            basic->record_file_lists[num].stop_time.month = 3;
            basic->record_file_lists[num].stop_time.date = 1;
            basic->record_file_lists[num].stop_time.hour = 18;
            basic->record_file_lists[num].stop_time.minute = 45;
            basic->record_file_lists[num].stop_time.second = 12;
            basic->record_file_lists[num].type=100;

        }

    }

    //״̬��ѯ
    else if(strcmp(cmd, OPER_PU_STATUS) == 0)
    {
        //ǰ�θ澯״̬��ѯ��
        DEVICE_ARLARMSTATUS_INFO **alarm_status = (DEVICE_ARLARMSTATUS_INFO **)buf;
        int i = 0;
        //�˴����ݸ澯�豸��ID�����澯�豸��״̬��
        while( i < *size)
        {
            printf("DEVICE_ARLARMSTATUS_INFO:%s lu%\n", alarm_status[i]->deviceid, *size);
            strcpy(alarm_status[i]->dutystatus, "ONDUTY");//��ʵ�ϱ��澯��״̬
            i++;
        }
    }

    // �豸Զ������
    else if(strcmp(cmd, OPER_PU_CONTROLPU) == 0)
    {
        LPNET_CMU_OPER_CTRL_DEV basic = (LPNET_CMU_OPER_CTRL_DEV)buf;
        printf("LPNET_CMU_OPER_CTRL_DEV %d\n", basic->control_action); //basic->control_action==0����==1�ػ�==2֪ͨ������==3֪ͨ������
    }
    
    //ʱ���ı�����
    else if(strcmp(cmd, SET_PU_IMAGETEXTPARA) == 0)
    {
        LPNET_CMU_IMG_TEXT basic = (LPNET_CMU_IMG_TEXT)buf;
        printf("video_id is %d\n", basic->video_id);
        printf("time_enable is %s\n", basic->time_enable);
        printf("time_X is %d\n", basic->time_X);
        printf("time_Y is %d\n", basic->time_Y);
        printf("text_enable is %s\n", basic->text_enable);
        printf("text is %s\n", basic->text);
        printf("text_X is %d\n", basic->text_X);
        printf("text_Y is %d\n", basic->text_Y);

    }
    //�豸ͼ��������
    else if(strcmp(cmd, SET_PU_IMAGEENCODEPARA) == 0)
    {
        LPNET_CMU_IMG_ENCODE basic = (LPNET_CMU_IMG_ENCODE)buf;
        printf("video_id is %d\n", basic->video_id);
        printf("encode_mode is %s\n", basic->encode_mode);
        printf("pic_quality is %d\n", basic->pic_quality);
        printf("bit_rate is %d\n", basic->bit_rate);
        printf("bit_rate_type is %d\n", basic->bit_rate_type);
        printf("frame_rate is %d\n", basic->frame_rate);
        printf("image_size is %s\n", basic->image_size);
        printf("stream_type is %d\n", basic->stream_type);
        printf("iframe_interval is %d", basic->iframe_interval);

    }
    //���ڲ���
    else if(strcmp(cmd, SET_PU_SERIALPORT) == 0)
    {

        LPNET_CMU_SERIAL_PORT basic = (LPNET_CMU_SERIAL_PORT)buf;
        printf("for_decoder is %s\n", basic->for_decoder);
        printf("serial_port is %d\n", basic->serial_port);
        printf("video_id is %d\n", basic->video_id);
        printf("baud_rate is %d\n", basic->baud_rate);
        printf("data_bit is %d\n", basic->data_bit);
        printf("parity is %d\n", basic->parity);
        printf("stop_bit is %d\n", basic->stop_bit);
        printf("mode is %s\n", basic->mode);
        printf("time_out is %d\n", basic->time_out);
        printf("flow_control is %d\n", basic->flow_control);
        printf("decoder_type is %d\n", basic->decoder_type);
        printf("decoder_address is %d\n", basic->decoder_address);
        printf("work_mode is %d\n", basic->work_mode);
    }
    //ͼ����ʾ����
    else if(strcmp(cmd, SET_PU_IMAGEDISPLAYPARA) == 0)
    {

        LPNET_CMU_IMG_DISPLAY basic = (LPNET_CMU_IMG_DISPLAY)buf;
        printf("video_id is %d\n", basic->video_id);
        printf("contrast is %d\n", basic->contrast);
        printf("bright is %d\n", basic->bright);
        printf("hue is %d\n", basic->hue);
        printf("saturation is %d\n", basic->saturation);

    }
    //�豸ͼ����ʽ����
    else if(strcmp(cmd, SET_PU_VIDEOFORMAT) == 0)
    {

        LPNET_CMU_VIDEO_FORMAT basic = (LPNET_CMU_VIDEO_FORMAT)buf;
        printf("video_id is %d\n", basic->video_id);
        printf("video_format is %d\n", basic->video_format);

    }
    // �豸��Ƶ�ڱθ澯����
    else if(strcmp(cmd, SET_PU_HIDEDETECTION) == 0)
    {

        LPNET_CMU_HIDE_DETECT basic = (LPNET_CMU_HIDE_DETECT)buf;


        printf("video_id is %d\n", basic->video_id);
        printf("alarm_dnabled is %s\n", basic->alarm_enabled);
        printf("alarm_time is %s\n", basic->alarm_time);
        printf("sensitivity  is %d\n", basic->sensitivity);

        printf("hide_coord.x %d\n", basic->hide_coord.x);
        printf("hide_coord.y %d\n", basic->hide_coord.y);
        printf("hide_coord.width %d\n", basic->hide_coord.width);
        printf("hide_coord.height %d\n", basic->hide_coord.height);

        printf("alarm_output alarm_channel %d\n", basic->hide_alarm_output.alarm_channel);
        printf("alarm_output alarm_time %d\n", basic->hide_alarm_output.alarm_time);

        printf("hide_alarm_viod.alarm_record_channel  is %d\n", basic->hide_alarm_video.alarm_record_channel);
        printf("hide_alarm_viod.alam_short_channel is  %d\n", basic->hide_alarm_video.alarm_shot_channel);

    }

    /*�澯������*/
    else if(strcmp(cmd, SET_PU_CONTROL_ALARM) == 0)
    {
        LP_NET_CMU_OPER_CTRL_ALARM control_alarm = (LP_NET_CMU_OPER_CTRL_ALARM)buf;
        printf("control alarm puid:%s\n",control_alarm->pu_id);
        printf("control alarm video_id:%d\n",control_alarm->video_id);
        printf("control alarm alarm_type:%d\n",control_alarm->alarm_type);
        printf("control alarm enable_arming:%s\n",control_alarm->enable_arming);
        
    }
    /*�����ƶ��������*/
    else if(strcmp(cmd, SET_PU_MOTIONDETECTION) == 0)
    {

    }
    return 0;
}

int on_stream( unsigned long id, char *cmd, char *buf, unsigned long size, unsigned long param )
{
    if(strcmp(cmd, "Start_MEDIA") == 0)
    {

        LPNET_MDU_START_MEDIA start_media = (LPNET_MDU_START_MEDIA)buf;
		if(start_media->request_type == 0)//ʵʱ������
        {
            NET_STREAM_OPEN_PARAM net_media;
            memset(&net_media,0,sizeof(NET_STREAM_OPEN_PARAM));

            net_media.ptr_file = fopen("./test.h264", "r+b");

            if(net_media.ptr_file == NULL)
            {
                printf("[on_stream] ptr_file\n");
                return -1;
            }
            net_media.id   = id;
            net_media.count = 0;
            net_media.flag = 1;
            net_media.video_id = start_media->media.video_id;
            net_media.media_type = start_media->media.media_type;
            net_media.stream_type = start_media->media.stream_type;
            while(!link_search(head,id))
            {
                printf("have have have have have havehave have havehave have have\n");
                return 0;
            }
            head = link_insert_head(head,net_media);

#ifdef WIN32

			net_media.pid = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&stream_proc, (void *)&net_media, NULL, NULL);
#else
	
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	    	pthread_create(&net_media.pid, NULL, stream_proc, (void *)&(head->data));
            pthread_attr_destroy (&attr);
#endif

        }

        else if(start_media->request_type == 1)//��ʷ������
        {
            NET_STREAM_OPEN_PARAM net_media;
            memset(&net_media,0,sizeof(NET_STREAM_OPEN_PARAM));

            net_media.ptr_file = fopen("./test.h264", "r+b");

            if(net_media.ptr_file == NULL)
            {
                printf("[on_stream] ptr_file\n");
                return -1;
            }
            net_media.id   = id;
            net_media.count = 0;
            net_media.flag = 1;
            net_media.video_id = start_media->media.video_id;
            while(!link_search(head,id))
            {
                printf("have have have have have havehave have havehave have have\n");
            }
            head = link_insert_head(head,net_media);

#ifdef WIN32

			net_media.pid = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&stream_proc, (void *)&net_media, NULL, NULL);
#else
	
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	    	pthread_create(&net_media.pid, NULL, stream_proc, (void *)&(head->data));
            pthread_attr_destroy (&attr);
#endif

        }
        
        else if(start_media->request_type == 2)//��������
        {

        }
        
    }

    else if(strcmp(cmd, "Stop_MEDIA") == 0)
    {
       
        LPNET_MDU_START_MEDIA start_media = (LPNET_MDU_START_MEDIA)buf;
        if(start_media->request_type == 0)
        {
            media_list_t * net_media = link_remove(&head,id);
            if(net_media != NULL)
            {
                printf("net_media->data.flag:%d\n",net_media->data.flag);
                while(net_media->data.flag != 0)
                {
                    net_media->data.flag = 2;
                    usleep(10);
                    //printf("net_media->data.flag:%d\n",net_media->data.flag);
                }
                printf("1111net_media->data.flag:%d\n",net_media->data.flag);
                if (NULL != net_media->data.ptr_file )
                {
                    printf("close ptr_param->ptr_file\n");
                    fclose(net_media->data.ptr_file);
                    net_media->data.ptr_file = NULL;
                }
                free(net_media);

            }
#ifdef WIN32
			WaitForSingleObject(net_media->data.pid, INFINITE);
			CloseHandle(net_media->data.pid);
			Sleep(1*1000);
#else

            printf("pthread_join\n");
#endif

        }
        else if(start_media->request_type == 1)
        {
            media_list_t * net_media = link_remove(&head,id);
            if(net_media != NULL)
            {
                while(net_media->data.flag != 0)
                {
                    net_media->data.flag = 2;
                    usleep(10);
                }
                if (NULL != net_media->data.ptr_file )
                {
                    printf("close ptr_param->ptr_file\n");
                    fclose(net_media->data.ptr_file);
                    net_media->data.ptr_file = NULL;
                }
                free(net_media);

            }
#ifdef WIN32
			WaitForSingleObject(net_media->data.pid, INFINITE);
			CloseHandle(net_media->data.pid);
			Sleep(1*1000);
#else
            printf("pthread_join\n");
#endif

        }
        else if(start_media->request_type == 2)
        {

        }
        
    }

    else  if(strcmp(cmd, "Start_AUDIO") == 0)
    {
        printf("Start AUdio is start start\n");
        	
        LPNET_MDU_START_MEDIA start_media = (LPNET_MDU_START_MEDIA)buf;
        int video_id = start_media->media.video_id;
    }
    
    else if(strcmp(cmd, "Stop_AUDIO") == 0)
    {

    }
    
    else if(strcmp(cmd, "Control_MEDIA") == 0)//�طſ���
    {
        LPNET_MDU_START_MEDIA start_media = (LPNET_MDU_START_MEDIA)buf;

        int video_id = start_media->media.video_id;//ͨ����
        float scale = start_media->scale;//��������
        printf("scale:%f\n",scale);
        if(scale == 0)
        {
            //��ͣ
        }
        else if(1.0 > scale && scale > 0)
        {
            //����scale ��ֵ����0,��˵����ʼ����
        }
        else if(scale > 1) //���
        {

        }
        //�ı�g_sleep_time ��ֵ�����Ʒ����ٶȣ������ȫ�ֱ�����ֻΪ�򵥵�ʵ��
        if(scale == 1.0)
            g_sleep_time = 40000;
        else if(scale == 2.0)
            g_sleep_time = 40000 / 2;
        else if(scale == 4.0)
            g_sleep_time = 40000 / 4;
        else if(scale == 8.0)
            g_sleep_time = 40000 / 8;
        else if(scale == 0.5)
            g_sleep_time = 40000 * 2;
        else if(scale == 0.25)
            g_sleep_time = 40000 * 4;
        else if(scale == 0.125)
            g_sleep_time = 40000 * 8;
        else
            g_sleep_time = 40000;
    }
    return 0;
}

int on_audio_stream (long unsigned id, DataType type, char *buf, unsigned long size, unsigned long param )
{
	printf("Get buf is :%d\n",size);
	net_send_voice_intercom(id,buf,size,type);
}

#ifdef WIN32
void*  stream_proc(void *param)
#else
void *stream_proc(void *param)
#endif
{

    LPNET_STREAM_OPEN_PARAM ptr_param = (LPNET_STREAM_OPEN_PARAM)param;
    if(ptr_param->ptr_file == NULL)
    {
        return NULL;
    }
    char *buffer = NULL;
    buffer = (char *)malloc(102400);
    memset(buffer,0,sizeof(102400));
    if(buffer == NULL)
    {
        printf("malloc failed %d %s ", errno, strerror(errno));
        exit(1);
    }
    int size = 0;
    int iRet = 0;
    NAL_HEADER nalheader;
    while(ptr_param->flag == 1)
    {
#ifdef WIN32
		Sleep(40);
#else
        usleep(200000);
#endif
        memset(&nalheader, 0, sizeof(NAL_HEADER));
        int ret = fread(&nalheader , 1, sizeof(NAL_HEADER), ptr_param->ptr_file);
        if(ret != sizeof(NAL_HEADER))
        {
            //��ȡ����ʧ��,�ر�������
            fseek(ptr_param->ptr_file, 0, SEEK_SET);
            continue;
        }

        //����ͷ�д�����һ֡��Ƶ�Ĵ�С
        size = nalheader.size;
        //������ȡһ֡����
        ret = fread(buffer, 1, size, ptr_param->ptr_file);
        if(ret != size)
        {
            printf("read data error:%d,%d\n", ret, size);
            fseek(ptr_param->ptr_file, 0, SEEK_SET);
            break;
        }
       	
        ptr_param->count++;
        if(ptr_param->count % 30 == 0)
        {
            iRet = net_send_stream(ptr_param->id, buffer, size, I_FRAME_TYPE);
            ptr_param->count = 0;
        }
        else
        {   
            iRet = net_send_stream(ptr_param->id, buffer, size, NO_I_FRAME_TYPE);
        }
		
        //������Ƶ����
        if(iRet == -1)
        {
            fseek(ptr_param->ptr_file, 0, SEEK_SET);
            printf("jie shu shi pin bao cun\n");
            net_stop_stream(ptr_param->id);
            break;
        }

    }
    printf("free buffer_____________________________________________________\n");
    if(buffer != NULL)
    {
        free(buffer);
        buffer = NULL;
    }
    ptr_param->flag = 0;
    printf("stop media\n");
    return NULL;
}

