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
//g_sleep_time是快放，慢放的时候，控制发送速率的，demo中简单的实现
int g_sleep_time = 1000000 / 25;

//码流操作Demo
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

int on_msg( char *cmd, char *buf, unsigned long *size, unsigned long param )//消息处理,参数size,param暂时未用
{
    //printf("on_msg cmd ++++++++++++%s\n",cmd);
    //设置设备时间参数
    if(strcmp(cmd, SET_PU_TIME) == 0)
    {
        //LPNET_CMU_DEV_TIME basic = (LPNET_CMU_DEV_TIME)buf;
        /*
           typedef struct
           {
           int hour;		// 时 (0~23)
           int minute;		// 分 (0~59)
           int second;		// 秒 (0~59)
           int date;		// 日 (1~31)
           int month;		// 月 (1~12)
           int year;		// 年 nnnn
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

    //获取设备版本
    else if(strcmp(cmd, GET_PU_SOFTWAREVERSION) == 0)
    {
        LPNET_CMU_SOFT_VERSION basic = (LPNET_CMU_SOFT_VERSION)buf;
        strncpy(basic->pu_id, "10002", 32);
        strncpy(basic->version, "10.1.1", 32);
        strncpy(basic->build_time, "2001-03-23 23:34:34", 32);
    }

    //设置设备网络参数
    else if(strcmp(cmd, SET_PU_IPINFO) == 0)
    {
        LPNET_CMU_IP_INFO basic = (LPNET_CMU_IP_INFO)buf;
        /*
           typedef struct
           {
           int   net_id;				// 网口编号 (1,2,3...)
           char  ip_addr[15];			// IP地址
           int   ctrl_port;			    // 业务控制端口号
           char  net_mask[15];			// 子网掩码
           char  dns_addr[2][15];		// DNS服务器地址,可以有多个
           char  gate_way[15];			// 网关地址
           }NET_CMU_IP_INFO, *LPNET_CMU_IP_INFO;
           */
        printf("netId is %d\n", basic->net_id);
        printf("ip_addr is %s\n", basic->ip_addr);
        printf("ctrl_port is %d\n", basic->ctrl_port);
        printf("net_mask is %s\n", basic->net_mask);
        printf("dns_addr is %s\n", basic->ip_addr);
        printf("gate_way is %s\n", basic->gate_way);
    }

    //获取设备网络参数
    else if(strcmp(cmd, GET_PU_IPINFO) == 0)
    {
        LPNET_CMU_IP_INFO basic = (LPNET_CMU_IP_INFO)buf;
        /*
           typedef struct
           {
           int   net_id;	            // 网口编号 (1,2,3...)
           char  ip_addr[15];           // IP地址
           int   ctrl_port;		        // 业务控制端口号
           char  net_mask[15];	        // 子网掩码
           char  dns_addr[2][15];		// DNS服务器地址,可以有多个
           char  gate_way[15];			// 网关地址
           }NET_CMU_IP_INFO, *LPNET_CMU_IP_INFO;
           */

        basic->net_id = 2;
        strncpy(basic->ip_addr, "192.168.2.72", 15);
        basic->ctrl_port = 1001;
        strncpy(basic->net_mask, "255.255.255.0", 15);
        strncpy(basic->dns_addr[0], "192.168.1.1", 15);
        strncpy(basic->gate_way, "192.168.1.1", 15);

    }

    //设置云台
    else if(strcmp(cmd, OPER_PU_CONTROLPTZ) == 0)
    {
        LPNET_CMU_OPER_CTRL_PTZ basic = (LPNET_CMU_OPER_CTRL_PTZ)buf;
        /*
           typedef struct
           {
           int  ptz_id;	云台标识

           char cmd[32];
           焦距拉近/拉远：ZIN/ZOUT；
           焦点调近/调远：FN/FR；
           光圈自动调整/增大/缩小：IA_ON/IO/IC；
           擦拭启动：WP_ON；
           灯光开启：LP_ON
           巡航启动：AUTO_START/AUTO_STOP；
           方向调整： 上TU,下TD,左PL,右PR, 右上TUPR , 左上TUPL, 右下TDPR, 左下TDPL,
           转到预设点：GOTO_PRESET
           轨迹巡航：
           开始巡航：START_TRACK_CRUISE
           停止：STOP

           char param[200];
           当cmd是GOTO_PRESET时，该参数表示预设点序号。
           当cmd是START_TRACK_CRUISE该参数是巡航轨迹索引。
           当cmd是STOP时，该参数指出要停止的控制命令（即导致云台运动的命令）

           int  speed;// 云台速度	1-8，共8个等级，数值越高速度越快。

           }NET_CMU_OPER_CTRL_PTZ, *LPNET_CMU_OPER_CTRL_PTZ;
           */

        printf("ptz_id is%d \n", basic->ptz_id); //云台标识
        printf("cmd is%s\n", basic->cmd);
        printf("param is%s\n", basic->param);
        printf("seep is%d\n", basic->speed); //云台速度
    }


    //设置云台预置点
    else if(strcmp(cmd, SET_PU_PRESETPTZ) == 0)
    {

        LPNET_CMU_PRESET_LIST basic = (LPNET_CMU_PRESET_LIST)buf;
        /*
           typedef struct
           {
           int  preset_index;					// 预置点编号
           char preset_name[16];				// 预置点名称
           }NET_CMU_PRESET, *LPNET_CMU_PRESET;

           typedef struct						// 设备云台预置点
           {
           int ptz_id;							// 云台标识
           int preset_list_cnt;					//
           NET_CMU_PRESET *preset_lists;
           }NET_CMU_PRESET_LIST, *LPNET_CMU_PRESET_LIST;
           */
        printf("basic->ptz_id %d\n", basic->ptz_id); //云台标识
        printf("basic->preset_lists->preset_index%d\n", basic->preset_lists->preset_index); //预置点索引
        printf("basic->preset_lists->preset_name%s\n", basic->preset_lists->preset_name); //预置点名称

    }
    //删除云台预置点
    else if(strcmp(cmd, OPER_PU_DELPRESETPTZ ) == 0)
    {
        // LPNET_CMU_OPER_DEL_PRESET basic = (LPNET_CMU_OPER_DEL_PRESET)buf;

    }
    //查询录像文件
    else if(strcmp(cmd, OPER_PU_QUERYPULOCALSTORAGEFILES) == 0)
    {
        LPNET_CMU_QUERY_STORE_FILE basic = (LPNET_CMU_QUERY_STORE_FILE)buf;
        int cnt = 10;
        int num = 0;
        int i = 1;
        char fileName[64] = {0};
        //char startTime[20]={0};
        //char endTime[20]={0};

        printf("video_id is %d \n", basic->video_id); //查询文件的视频通道
        printf("file_type is %d\n", basic->file_type); //铭软查询文件的类型
        printf("file_type is %s \n",basic->type);
        printf("from_date is %04d-%02d-%02d %02d:%02d:%02d\n",
               basic->from_date.year,
               basic->from_date.month,
               basic->from_date.date,
               basic->from_date.hour,
               basic->from_date.minute,
               basic->from_date.second);//开始的日期及时间

        printf("from_date is %04d-%02d-%02d %02d:%02d:%02d\n",
               basic->to_date.year,
               basic->to_date.month,
               basic->to_date.date,
               basic->to_date.hour,
               basic->to_date.minute,
               basic->to_date.second);//开始的日期及时间

        //basic->record_file_lists = (LPNET_CMU_RECORD_FILE)malloc(cnt * sizeof(NET_CMU_RECORD_FILE));
        printf("+_++++++++++++++++++++++++++++++++++++++++++++%d  %d\n", cnt * sizeof(NET_CMU_RECORD_FILE), sizeof(NET_CMU_QUERY_STORE_FILE));
        //basic->record_file_size = 4096;
        basic->file_list_cnt = cnt;

        basic->total_size = 1000;//DISK 容量大小
        basic->free_size = 1000;//DISK剩余容量
        basic->total_num = 100;//总共文件条数
        basic->leave_num = 20;//剩余文件条数
        basic->result_code = 0;//注册结果 （code）: 0：OK  1：不支持的文件类型 48：客户端错误	49：服务器错误 50：其他失败原因

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

    //状态查询
    else if(strcmp(cmd, OPER_PU_STATUS) == 0)
    {
        //前段告警状态查询。
        DEVICE_ARLARMSTATUS_INFO **alarm_status = (DEVICE_ARLARMSTATUS_INFO **)buf;
        int i = 0;
        //此处根据告警设备的ID，填充告警设备的状态。
        while( i < *size)
        {
            printf("DEVICE_ARLARMSTATUS_INFO:%s lu%\n", alarm_status[i]->deviceid, *size);
            strcpy(alarm_status[i]->dutystatus, "ONDUTY");//如实上报告警的状态
            i++;
        }
    }

    // 设备远程重启
    else if(strcmp(cmd, OPER_PU_CONTROLPU) == 0)
    {
        LPNET_CMU_OPER_CTRL_DEV basic = (LPNET_CMU_OPER_CTRL_DEV)buf;
        printf("LPNET_CMU_OPER_CTRL_DEV %d\n", basic->control_action); //basic->control_action==0重启==1关机==2通知上线了==3通知掉线了
    }
    
    //时间文本设置
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
    //设备图像编码参数
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
    //串口参数
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
    //图像显示参数
    else if(strcmp(cmd, SET_PU_IMAGEDISPLAYPARA) == 0)
    {

        LPNET_CMU_IMG_DISPLAY basic = (LPNET_CMU_IMG_DISPLAY)buf;
        printf("video_id is %d\n", basic->video_id);
        printf("contrast is %d\n", basic->contrast);
        printf("bright is %d\n", basic->bright);
        printf("hue is %d\n", basic->hue);
        printf("saturation is %d\n", basic->saturation);

    }
    //设备图像制式参数
    else if(strcmp(cmd, SET_PU_VIDEOFORMAT) == 0)
    {

        LPNET_CMU_VIDEO_FORMAT basic = (LPNET_CMU_VIDEO_FORMAT)buf;
        printf("video_id is %d\n", basic->video_id);
        printf("video_format is %d\n", basic->video_format);

    }
    // 设备视频遮蔽告警参数
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

    /*告警布撤防*/
    else if(strcmp(cmd, SET_PU_CONTROL_ALARM) == 0)
    {
        LP_NET_CMU_OPER_CTRL_ALARM control_alarm = (LP_NET_CMU_OPER_CTRL_ALARM)buf;
        printf("control alarm puid:%s\n",control_alarm->pu_id);
        printf("control alarm video_id:%d\n",control_alarm->video_id);
        printf("control alarm alarm_type:%d\n",control_alarm->alarm_type);
        printf("control alarm enable_arming:%s\n",control_alarm->enable_arming);
        
    }
    /*设置移动侦测设置*/
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
		if(start_media->request_type == 0)//实时流请求
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

        else if(start_media->request_type == 1)//历史流请求
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
        
        else if(start_media->request_type == 2)//下载请求
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
    
    else if(strcmp(cmd, "Control_MEDIA") == 0)//回放控制
    {
        LPNET_MDU_START_MEDIA start_media = (LPNET_MDU_START_MEDIA)buf;

        int video_id = start_media->media.video_id;//通道号
        float scale = start_media->scale;//操作参数
        printf("scale:%f\n",scale);
        if(scale == 0)
        {
            //暂停
        }
        else if(1.0 > scale && scale > 0)
        {
            //慢放scale 的值大于0,就说明开始播放
        }
        else if(scale > 1) //快放
        {

        }
        //改变g_sleep_time 的值，控制发送速度，这里的全局变量，只为简单的实现
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
            //读取数据失败,关闭流任务
            fseek(ptr_param->ptr_file, 0, SEEK_SET);
            continue;
        }

        //数据头中存着下一帧视频的大小
        size = nalheader.size;
        //继续读取一帧数据
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
		
        //结束视频保存
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

