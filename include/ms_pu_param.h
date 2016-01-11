#ifndef __MS_PU_PARAM_H__
#define __MS_PU_PARAM_H__

//#include "stdafx.h"

/*******************************************************************************/
/* ----Register---- */
#define REG_PU_REGISTER							"PuRegister"				// �豸ע��
#define REG_PU_KEEPALIVE						"PuKeepAlive"				// �豸����
/*******************************************************************************/

/*******************************************************************************/
/* ----Set---- */
#define SET_PU_BASICINFO						"SetPuBasicInfo"			// �豸��������
#define SET_PU_IPINFO							"SetPuIpInfo"				// �豸�������
#define SET_PU_USER								"SetPuUser"					// �豸�û�����
#define SET_PU_SERIALPORT						"SetPuSerialPort"			// �豸���ڲ���
#define SET_PU_OUTDEV							"SetPuOutDev"				// �豸����ӿڲ���
#define SET_PU_TIME								"SetPuTime"					// �豸ʱ�����

#define SET_PU_VIDEOFORMAT						"SetPuVideoFormat"			// �豸ͼ����ʽ����
#define SET_PU_IMAGEENCODEPARA					"SetPuImageEncodePara"		// �豸ͼ��������
#define SET_PU_IMAGEDISPLAYPARA					"SetPuImageDisplayPara"		// �豸ͼ����ʾ����
#define SET_PU_IMAGETEXTPARA					"SetPuImageTextPara"		// �豸ͼ����ص����ֲ���

#define SET_PU_ALARMPARAMS						"SetPuAlarmParams"			// �豸�����澯����
#define SET_PU_GIPINALARM						"SetPuGpinAlarm"			// �豸Gpin�澯����

#define SET_PU_VIDEOLOSE						"SetPuVideoLose"			// �豸��Ƶ��ʧ�澯����
#define SET_PU_HIDEDETECTION					"SetPuHideDetection"		// �豸��Ƶ�ڱθ澯����
#define SET_PU_MOTIONDETECTION					"SetPuMotionDetection"		// �豸�ƶ����澯��ʼ������
#define SET_PU_IMAGEHIDEAREA					"SetPuImageHideArea"		// �豸ͼ���ڵ�����

#define SET_PU_LOCALSTORAGETASK					"SetPuLocalStorageTask"		// �豸���ش洢����

#define SET_PU_PRESETPTZ						"SetPresetPTZ"				// �豸��̨Ԥ�õ�
#define SET_PU_DEFAULTPRESETPTZ					"SetDefaulePresetPTZ"		// �豸Ĭ����̨Ԥ�õ�
#define SET_PU_CRUISETRACK						"SetCruiseTrack"			// �豸Ѳ���켣
#define SET_PU_CONTROL_ALARM                    "ControlArming"             //�澯������
/*******************************************************************************/

/*******************************************************************************/
//Get
#define GET_PU_BASICINFO						"GetPuBasicInfo"			// �豸��������
#define GET_PU_IPINFO							"GetPuIpInfo"				// �豸�������
#define GET_PU_USER								"GetPuUser"					// �豸�û�����
#define GET_PU_SERIALPORT						"GetPuSerialPort"			// �豸���ڲ���
#define GET_PU_INOUTDEV							"GetPuInOutDev"				// �豸����ӿڲ���
#define GET_PU_TIME								"GetPuTime"					// �豸ʱ�����

#define GET_PU_VIDEOFORMAT						"GetPuVideoFormat"			// �豸ͼ����ʽ����
#define GET_PU_IMAGEENCODEPARA					"GetPuImageEncodePara"		// �豸ͼ��������
#define GET_PU_IMAGEDISPLAYPARA					"GetPuImageDisplayPara"		// �豸ͼ����ʾ����
#define GET_PU_IMAGETEXTPARA					"GetPuImageTextPara"		// �豸ͼ����ص����ֲ���

#define GET_PU_ALARMPARAMS						"GetPuAlarmParams"			// �豸�����澯����
#define GET_PU_GIPINALARM						"GetPuGpinAlarm"			// �豸Gpin�澯����

#define GET_PU_VIDEOLOSE						"GetPuVideoLose"			// �豸��Ƶ��ʧ�澯����
#define GET_PU_HIDEDETECTION					"GetPuHideDetection"		// �豸��Ƶ�ڱθ澯����
#define GET_PU_MOTIONDETECTION					"GetPuMotionDetection"		// �豸�ƶ����澯��ʼ������
#define GET_PU_IMAGEHIDEAREA					"GetPuImageHideArea"		// �豸ͼ���ڵ�����

#define GET_PU_LOCALSTORAGETASK					"GetPuLocalStorageTask"		// �豸���ش洢����

#define GET_PU_PRESETPTZ						"GetPresetPTZ"				// �豸��̨Ԥ�õ�
#define GET_PU_DEFAULTPRESETPTZ					"GetDefaulePresetPTZ"		// �豸Ĭ����̨Ԥ�õ�
#define GET_PU_CRUISETRACK						"GetCruiseTrack"			// �豸Ѳ���켣

#define GET_PU_STATE							"GetPuState"				// �豸״̬��Ϣ
#define GET_PU_LOG								"GetPuLog"					// �豸��־��Ϣ

#define GET_PU_SOFTWAREVERSION					"GetPuSoftwareVersion"		// �豸����汾��Ϣ
/*******************************************************************************/
#define OPER_PU_STATUS							"DeviceStatus"  //�豸״̬��ѯ

/*******************************************************************************/
//operation
#define OPER_PU_DEL_USER						"DeletePuUser"				// ɾ���豸�û���Ϣ
#define OPER_PU_CLEANPULOCALSTORAGEFILE			"CleanPuLocalStorageFile"	// ɾ���豸¼���ļ�
#define OPER_PU_QUERYPULOCALSTORAGEFILES		"QueryPuLocalStorageFiles"	// ��ѯ�豸¼���ļ�
#define OPER_PU_CONTROLPTZ						"ControlPTZ"				// �豸�ƾ���������
#define GET_LOCAL_IP                            "GetLocalIP"                //��ȡ����IP
#define OPER_PU_STOPRECORD                      "StopReocrdCmd"             //����/����
#define OPER_PU_DELPRESETPTZ					"DelPresetPTZ"				// �豸ɾ����̨Ԥ�õ�
#define OPER_PU_CONTROLPU						"ControlPU"					// �豸Զ������
#define OPER_PU_TRANSPARENTCHANNEL				"TransparentChannel"		// �豸ͨ��ͨ��
#define OPER_PU_TELEBOOT				        "TeleBoot"		            // �豸ͨ��ͨ��
#define OPER_PU_ARLRMCMD				        "AlarmCmd"		            // �豸ͨ��ͨ��
/*******************************************************************************/
#define OPER_PU_CONTROLFILEBACK                 "ControlFileBack"
/*******************************************************************************/
//upload
#define UPLOAD_PU_RAISEALARM					"RaiseAlarm"				// �豸�澯�ϱ�
#define UPLOAD_PU_RAISEDIGITAL					"RaiseDigital"				// �豸�������ϱ�
#define OPER_PU_AlARMQUERY                      "AlarmQuery"                //������ѯ
/*******************************************************************************/

//���֧���ƶ����������
#define 	MAX_MOTIONDETECTIONNUM	8

//���֧���ڵ�������
#define	MAX_IMAGEHIDEAREANUM	8

//ǰ������
#define PU_TYPE_DVS		0
#define PU_TYPE_DVR		1
#define PU_TYPE_IP_CAM	2
#define PU_TYPE_OTHER	3

//�洢���Բ�������
#define STORE_POLICY_CLEAN		1
#define STORE_POLICY_SET		2

//�澯����
#define VS_DISKERROR		0
#define VS_DISKFULL			1
#define VS_DISCONN			2
#define VS_UPGRADE			3
#define VS_GPIN				4//2
#define CAM_VIDEOLOSE		5
#define CAM_HIDEDETECT		6
#define CAM_MOTIONDETECT	7
#define CAM_PIRMOTION       8  /*2014/10/13��� �������澯*/
#define VCA_INTRUSION       9
#define DEFAULT_ALARM_TYPE     20
//��������
#define REQUEST_REAL_STREAM		0
#define REQUEST_FILE_STREAM		1
#define DOWNLOAD_FILE_STREAM     2

//�澯����ʱ����
#define ALARM_INTERVAL_TIME 30;
//Ŀǰֻ֧��TCP
enum
{
    STREAM_TYPE_MAIN = 0, //������
    STREAM_TYPE_SUB,  //������
    STREAM_TYPE_ALARM //�澯����
};


//ý������
enum
{
    MEDIA_TYPE_AUDIO = 0, 	//��Ƶ
    MEDIA_TYPE_VIDEO,		//��Ƶ
    MEDIA_TYPE_ALL			//��Ƶ����Ƶ
};



typedef struct										// �豸ʱ�����
{
    int hour;										// ʱ (0~23)
    int minute;										// �� (0~59)
    int second;										// �� (0~59)
    int date;										// �� (1~31)
    int month;										// �� (1~12)
    int year;										// �� nnnn
} NET_CMU_DEV_TIME, *LPNET_CMU_DEV_TIME;


typedef struct								// �豸Ŀ¼��ѯ
{
    char pu_id[32];
    int control_action;						// 0 - ֹͣ¼��  1 - ��ʼ¼��
} NET_CMU_OPER_RECORD_DEV, *LPNET_CMU_OPER_RECORD_DEV; //�ֶ�¼��,�ṹ��

/*******************************************************************************/
// MDU stream
/*******************************************************************************/
//MDU ��Ƶ������
typedef struct
{
    char pu_id[64];			// PU�ı�ʶ
    int  video_id;			// ��Ƶͨ�� (1,2...)
    int  media_type;		// ý�����ͣ� 0����Ƶ������Ƶ�Խ��ĻỰ 1����Ƶ�� 2����Ƶ+��Ƶ
    int  stream_type;		// �������ͣ� 0�������� 1�������� 	2���澯����

    char server_id[64];
} NET_MDU_MEDIA, *LPNET_MDU_MEDIA;


//MDU ������Ƶ����Ϣ
typedef struct
{
    NET_MDU_MEDIA media;	// ������Ƶ��������Ϣ
    int  link_mode;			// ý�������ͷ�ʽ�� 0��TCP 	1��UDP 2��RTP/RTCP 3���鲥
    char target_ip[32];     // ý��������Ŀ�ĵ�ַ
    int  target_port;		// ý��������Ŀ�Ķ˿�
    NET_CMU_DEV_TIME start_time;
    NET_CMU_DEV_TIME stop_time;
    char fileName[128];/*��ǰ��64����Щ�ļ����ᳬ�����������ȫ���޸���2014-12-31*/
    int  size;

    //int  image_width;		// ͼ����
    //int  image_hight;		// ͼ��߶�
    //int  video_format;		// ��Ƶ��ʽ  0��PAL   1��NTSC
    //int  frame_rate;		// 1fps, 2fps, 4fps, 6fps, 8fps, 10fps, 12fps, 16fps 20fps, 25fps
    //char video_header[1024];// ��Ƶͷ����(Base64����)
    int  result_code;   // ��������	0���ɹ� 1����֧�ֵ�ý������ 2����֧�ֵĴ��䷽ʽ 3��ͨ����ռ��(�����Խ�) 50����������
    int request_type;   //�������ͣ�0:ʵʱ��Ƶ 1:��ʷ�ط� 2:����

    float scale;
    /*
       �����ٶ�
       8:��Ű˱��� 4:����ı���2:��Ŷ����� 1:�������ţ�0:Ϊ��ͣ�� 0.5:���Ŷ��� 0.25:����4�� 0.125:����8��
       1.��ͣ������ֹͣ������
       2.��ţ����ٷ�����
       3.���ţ����ͷ����ٶ�
    */
    char cmd[20];
    char param[20];

    long iSeq;
    unsigned long long  timestamp;
    int time_intival;

    int did;
} NET_MDU_START_MEDIA, *LPNET_MDU_START_MEDIA;

/*******************************************************************************/
//Get--Set
/*******************************************************************************/
typedef struct										// �豸��������
{
    char pu_id[32];									// PU�ı�ʶ
    char pu_name[32];								// PU������
    char pu_passwd[32];								// PU���豸����
} NET_CMU_BASIC_INFO, *LPNET_CMU_BASIC_INFO;

typedef struct										// �豸�������
{
    int   net_id;									// ���ڱ�� (1,2,3...)
    char  ip_addr[16];								// IP��ַ
    int   ctrl_port;								// ҵ����ƶ˿ں�
    char  net_mask[16];								// ��������
    char  dns_addr[2][16];							// DNS��������ַ,�����ж��
    char  gate_way[16];								// ���ص�ַ
} NET_CMU_IP_INFO, *LPNET_CMU_IP_INFO;

typedef struct
{
    char user_name[16];								// ����PU���û���
    char pass_word[32];								// ����PU������
} NET_CMU_USER, *LPNET_CMU_USER;

typedef struct										// �豸�û�����
{
    int				user_cnt;
    LPNET_CMU_USER  user_list;
} NET_CMU_USER_LIST, *LPNET_CMU_USER_LIST;

typedef struct										// �豸���ڲ���
{
    char for_decoder[8];							// true����ȡ��̨��������Ϣ false����ȡ���ô�����Ϣ
    int  serial_port;								// ���ڱ�� (1,2...), ��forDecoderΪfalseʱ����ҪЯ�����ֶ�
    int  video_id;									// ͨ����(1,2...)����forDecoderΪtrueʱ����ҪЯ�����ֶ�
    int  baud_rate;									// ������(bps) (50, 75, 110, 150, 300, 600, 1200, 2400, 4800, 9600, 19200 38400, 57600, 115200)
    int  data_bit;									// ����λ  ��5��6��7��8��
    int  parity;									// У�� ��0����У�飬1����У�飬2��żУ�飩
    int  stop_bit;									// ֹͣλ ��1��2��
    char mode[16];									// (RS485��RS232)
    int  time_out;									// �������ݳ�ʱ ��0...65535�� unit(100ms)
    int  flow_control;								// 0�������أ�1��������,2-Ӳ����
    int  decoder_type;								// ��̨����������, ��forDecoderΪtrueʱ����ҪЯ�����ֶ�
    int  decoder_address;							// ��̨��������ַ, ��forDecoderΪtrueʱ����ҪЯ�����ֶ�
    int  work_mode;									// ��forDecoderΪfalseʱ����ҪЯ�����ֶ� 1����̨ 2͸��ͨ��
} NET_CMU_SERIAL_PORT, *LPNET_CMU_SERIAL_PORT;

typedef struct										// ͨ�����������������Ϣ
{
    int channel_num;								// ͨ�����
    int state;										// (����-on , 0-off)�ж����豸��״̬
} NET_CMU_INOUT_INFO, *LPNET_CMU_INOUT_INFO;



typedef struct										// ����������/����ӿڲ���
{
    int					in_list_cnt;				// ��������������
    int					out_list_cnt;				// �������������

    LPNET_CMU_INOUT_INFO	in_list;					// ����������
    LPNET_CMU_INOUT_INFO	out_list;					// ���������
} NET_CMU_INOUT_DEV, *LPNET_CMU_INOUT_DEV;



typedef struct
{
    int video_id;									// ��Ƶͨ����ʶ��1,2...��
    int video_format;								// ͼ����ʽ 0��PAL 1��NTSC
} NET_CMU_VIDEO_FORMAT, *LPNET_CMU_VIDEO_FORMAT;

typedef struct										// �豸ͼ����ʽ����
{
    int video_id;									// ͨ����ʶ��1,2...��������ʱ������ȡ������Ƶͨ����ͼ����ʽ��
    int vformat_cnt;								// ͨ��ͼ����ʽ ����
    LPNET_CMU_VIDEO_FORMAT channel_video_format;	// ͨ��ͼ����ʽ
} NET_CMU_VIDEO_FORMAT_LIST, *LPNET_CMU_VIDEO_FORMAT_LIST;

typedef struct										// �豸ͼ��������
{
    int  video_id;									// ��Ƶͨ�� (1,2...)
    char encode_mode[32];							// ����ģʽMJPEG MPEG-1 MPEG-2 MPEG-4 H261 H263 H264 AVS
    int  pic_quality;								// ͼ������ ��0-��� 1-�κ� 2-�Ϻ� 3-һ�� 4-�ϲ� 5-�
    int  bit_rate;									// ���� unit(K/s)
    int  bit_rate_type;								// ��������0:�����ʣ�1:������
    int  frame_rate;								// ֡��1fps, 2fps, 4fps, 6fps, 8fps, 10fps, 12fps, 16fps 20fps, 25fps
    char image_size[32];							// DCIF,  CIF,  QCIF, 2CIF, 4CIF, 720I, 720P, 1080I, 1080P
    int  stream_type;								// ���ǰ���豸���Ա��������������� 0-������ 1-������
    int  iframe_interval;							// I֡���
} NET_CMU_IMG_ENCODE, *LPNET_CMU_IMG_ENCODE;

typedef struct										// �豸ͼ����ʾ����
{
    int video_id;									// ��Ƶͨ�� (1,2...)
    int contrast;									// �Աȶȣ�0-255��
    int bright;										// ����   (0-255��
    int hue;										// ɫ��  ��0-255��
    int saturation;									// ���Ͷȣ�0-255��
} NET_CMU_IMG_DISPLAY, *LPNET_CMU_IMG_DISPLAY;

typedef struct										// �豸ͼ����ص����ֲ���
{
    int  video_id;									// ��Ƶͨ�� (1,2...)
    char time_enable[8];//need chage to char		// �Ƿ���ʾʱ�� false-hiden  true-show
    int  time_X;									// ʱ�����X����
    int  time_Y;									// ʱ�����Y����
    char text_enable[8];//need chage to char		// �Ƿ���ʾ���� false-hiden  true-show
    char text[128];									// ��������
    int  text_X;									// ���ֵ�X����
    int  text_Y;									// ���ֵ�Y����
} NET_CMU_IMG_TEXT, *LPNET_CMU_IMG_TEXT;

typedef struct									// �������澯¼��
{
    //	char enable[5];								// ץ��ʹ�� ץ�ģ�true; ��ץ��:false
    int	 record_channel;						// ¼��ͨ��1--16�ɶ�ѡ
    int  record_time;							// ¼��ʱ�� ��λ��
    int  precord_time;							// �澯¼��Ԥ¼ʱ��
} NET_CMU_ALARM_RECORD, *LPNET_CMU_ALARM_RECORD;

typedef struct									// �������澯���
{
    //	char enable[5];								// �澯ʹ�� �澯��true; ���澯:false
    int alarm_channel;							// �澯���ͨ��
    int alarm_time;								// �����澯���������ʱ�䣨�룩
} NET_CMU_ALARM_OUTPUT, *LPNET_CMU_ALARM_OUTPUT;

typedef struct									// �澯����
{
    char active_start[8];						// ������ʼʱ��(hhmm-hhmm, ǰ��2λ��ʾСʱ������2Ϊ��ʾ���� )
    char active_end[8];							// ��������ʱ��(hhmm-hhmm, ǰ��2λ��ʾСʱ������2Ϊ��ʾ���� )
} NET_CMU_ACTIVE_TIME, *LPNET_CMU_ACTIVE_TIME;

typedef struct									// ������ץ��
{
    //	char enable[5];								// ץ��ʹ�� ץ�ģ�true; ��ץ��:false
    int  shot_channel;							// ץ��ͨ��
    int  shot_times;							// ץ�Ĵ���
} NET_CMU_SHOT_PHOTO, *LPNET_CMU_SHOT_PHOTO;

typedef struct									// ��������̨Ԥ��λ
{
    //	char enable[5];								// ��������̨Ԥ��λʹ�� ������true; ������:false
    int ptz_channel;							// ��̨ͨ��
    int ptz_preset;								// ��̨Ԥ��λ
} NET_CMU_PTZ_PRESET, *LPNET_CMU_PTZ_PRESET;

typedef struct									// �豸�����澯����
{
    int  alarm_in_channel;						// �澯����ͨ��
    char alarm_enabled[8];						// �澯ʹ�� true : false
    char is_copy_toall[8];						// ���Ƶ�����ͨ�� true : false

    NET_CMU_ALARM_RECORD  alarm_record ;		// �������澯¼��
    NET_CMU_ALARM_OUTPUT  alarm_output ;		// �������澯���
    NET_CMU_SHOT_PHOTO    shot_photo;			// ������ץ��
    NET_CMU_PTZ_PRESET    ptz_preset;			// ��������̨����
    NET_CMU_ACTIVE_TIME   active_times;			// ����ʱ��
} NET_CMU_BASIC_ALARM, *LPNET_CMU_BASIC_ALARM;

typedef struct
{
    int	alarm_inout_channel;					// �澯�������ͨ����
    int	alarm_inout_state;						// �澯��������˿ڵ�״̬,0-û�б���,1-�б���
} NET_CMU_ALARM_INOUT, *LPNET_CMU_ALARM_INOUT;

typedef struct
{
    int	alarm_record_channel;					// ����������¼��ͨ����
    int	alarm_shot_channel;						// ����������ץ��ͨ����
} NET_CMU_ALARM_VIDEO, *LPNET_CMU_ALARM_VIDEO;


typedef struct
{
    int x;										// X���ꣻ���Ͻ�Ϊԭ��
    int y;										// Y����
    int width;									// ���
    int height;									// �߶�
} NET_CMU_COORDINATE, *LPNET_CMU_COORDINATE;

/************************************************************************/
typedef struct									// �豸Gpin�澯����
{
    int  gpin_id;
    char alarm_enabled[8];
    int  alarm_status;
    NET_CMU_ALARM_OUTPUT gpin_alarm_output;
    NET_CMU_ALARM_VIDEO  gpin_alarm_video;
} NET_CMU_GPIN_ALARM_INFO, *LPNET_CMU_GPIN_ALARM_INFO;
/************************************************************************/

typedef struct											// �豸��Ƶ��ʧ�澯����
{
    int video_id;
    int alarm_enabled;
    int alarm_output;
    int alarm_state;
} NET_CMU_VIDEO_LOSE, *LPNET_CMU_VIDEO_LOSE;

typedef struct									// �豸��Ƶ�ڱθ澯����
{
    int	 video_id;								// ��Ƶͨ�� (1,2...)
    char alarm_enabled[8];						// �Ƿ�ʹ����Ƶͨ�����ڵ���⡣ true:��ʾ��� false����ʾ�����
    char alarm_time[16];						// ����ʱ��(hhmm-hhmm, ǰ��2λ��ʾСʱ������2Ϊ��ʾ���� )
    NET_CMU_COORDINATE hide_coord;				//
    int  sensitivity;							// �����ȡ� 0���ر� 1���� 2���� 3����

    NET_CMU_ALARM_OUTPUT   hide_alarm_output;	// �������澯���
    NET_CMU_ALARM_VIDEO    hide_alarm_video;    // �������澯¼��
} NET_CMU_HIDE_DETECT, *LPNET_CMU_HIDE_DETECT;


typedef struct									// �豸�ƶ����澯��ʼ������
{
    int  video_id;								// ��Ƶͨ�� (1,2...)
    char alarm_enabled[8];						// �Ƿ�ʹ����Ƶͨ�����ڵ���⡣true:��ʾ��� false����ʾ�����
    char alarm_time[16];						// ����ʱ��(hhmm-hhmm, ǰ��2λ��ʾСʱ������2Ϊ��ʾ���� )

    int block_list_cnt;
    LPNET_CMU_COORDINATE block_lists;

    int frequency;								// Ƶ��
    int sensitivity;							// �����ȡ� �����ȡ� 0���ر� 1���� 	2���� 	3����

    NET_CMU_ALARM_OUTPUT  motion_alarm_output;  // �������澯���
    NET_CMU_ALARM_VIDEO   motion_alarm_video;   // �������澯¼��
} NET_CMU_MOTION_DETECT, *LPNET_CMU_MOTION_DETECT;

typedef struct									// �豸ͼ���ڵ�����
{
    int  video_id;								// ��Ƶͨ�� (1,2...)
    char enabled[8];							// �Ƿ�ʹ����Ƶͨ�����ڵ��� true:��ʾ���� false����ʾ������

    int area_list_cnt;
    LPNET_CMU_COORDINATE  area_lists;
} NET_CMU_IMG_HIDE_AREA, *LPNET_CMU_IMG_HIDE_AREA;

typedef struct
{
    int week_day;							// weekday: 1-7
    int record_type;						// recordType:: 0=��ʱ¼��1=�ƶ����, 2=��������
    int start_hour;							// startHour: 0-23
    int start_min;							// startMin: 0-59
    int stop_hour;							// stopHour: 0-23
    int stop_min;							// stopMin: 0-59 ȫ���ʾΪ��0��0��23��0
} NET_CMU_STORAGE_SCHEDULE, *LPNET_CMU_STORAGE_SCHEDULE;

typedef struct								 // �豸���ش洢����
{
    int video_id;							 // ��Ƶͨ�� (1,2...)
    int record_enable;						 // 0:ֹͣ¼�� 1:��ʼ¼��
    int recycle_enable;						 // 0=��¼���󲻸��� 1=�ǣ�¼���󸲸ǣ�ѭ��¼��
    int record_policy;						 // 0=ȫ���洢��1=ѡ��洢
    int frame_interval;						 // ѡ��洢ʱ��ÿ��洢֡��

    int schedule_list_cnt;
    LPNET_CMU_STORAGE_SCHEDULE schedule_lists;
} NET_CMU_STORAGE_TASK, *LPNET_CMU_STORAGE_TASK;

typedef struct
{
    int  preset_index;						// Ԥ�õ�����
    char preset_name[16];					// Ԥ�õ�����
} NET_CMU_PRESET, *LPNET_CMU_PRESET;

typedef struct								// �豸��̨Ԥ�õ�
{
    int ptz_id;								// ��̨��ʶ
    int preset_list_cnt;					//
    NET_CMU_PRESET *preset_lists;			//
} NET_CMU_PRESET_LIST, *LPNET_CMU_PRESET_LIST;

typedef struct								// �豸Ĭ����̨Ԥ�õ�
{
    int  ptz_id;							// ��̨��ʶ
    int  preset_index;                      // Ԥ�õ�����
    char preset_name[16];                   // Ԥ�õ�����
    int  restore_time;                      // ��̨�Զ���λʱ�䣬��λ"��"
} NET_CMU_DEFAULT_PRESET, *LPNET_CMU_DEFAULT_PRESET;

typedef struct								// �豸Ѳ���켣
{
    int  ptz_id;							// ��̨��ʶ
    int  cruise_num;						// Ѳ������
    char cruise_point[200];					// Ѳ����
} NET_CMU_CRUISE_TRACK, *LPNET_CMU_CRUISE_TRACK;

/************************************************************************/
typedef struct								// �豸Ѳ���켣
{
    int  ptz_id;							// ��̨��ʶ
    int  cruise_track_cnt;
    LPNET_CMU_CRUISE_TRACK cruise_lists;
} NET_CMU_CRUISE_TRACK_LIST, *LPNET_CMU_CRUISE_TRACK_LIST;
/************************************************************************/

typedef struct
{
    int disk_id;							// 1��2��3...
    int total_size;						    // Ӳ�̵�����
    int free_size;							// Ӳ�̵�ʣ��ռ�
    int status;							    // Ӳ�̵�״̬,0-������ 1-���ߣ�2-������
} NET_CMU_DISK_STATUS, *LPNET_CMU_DISK_STATUS;

typedef struct
{
    int video_id;							// 1,2,3...
    int record_status;						// ��Ƶͨ���Ƿ���¼��,0-��¼��,1-¼��
    int signal_status;						// ���ӵ���Ƶ�ź�״̬,0-����,1-�źŶ�ʧ
    int hardware_status;					// ͨ��Ӳ��״̬,0-����,1-�쳣,����DSP����
    int bit_rate;						    // ʵ������
} NET_CMU_VIDEO_CHANNEL, *LPNET_CMU_VIDEO_CHANNEL;


typedef struct								// �豸״̬��Ϣ
{
    int device_status;						// �豸��״̬,0-����,1-CPUռ����̫��,����85%,2-Ӳ������

    int disk_list_cnt;
    int video_channel_list_cnt;
    int alarm_in_list_cnt;
    int alarm_out_list_cnt;

    LPNET_CMU_DISK_STATUS   disk_status_lists;
    LPNET_CMU_VIDEO_CHANNEL video_channel_lists;
    LPNET_CMU_ALARM_INOUT   alarm_in_lists;
    LPNET_CMU_ALARM_INOUT   alarm_out_lists;
} NET_CMU_DEV_STATE, *LPNET_CMU_DEV_STATE;

typedef struct								// �豸��־��Ϣ
{
    int  log_type;							// ��־���ͣ�1-����; 2-�쳣; 3-����; 0xff-ȫ��
    char log_time[32];						// ��־ʱ��
    char net_user[32];						// �����û���
    int  video_id;							// ��Ƶͨ����
    int  disk_num;							// Ӳ�̺�
    int  alarm_in_port;						// �澯����˿�
    int  alarm_out_port;					// �澯����˿�
} NET_CMU_LOG_INFO, *LPNET_CMU_LOG_INFO;

typedef struct								// �豸��־��Ϣ
{
    int  log_type;							// ��־���ͣ�1-����; 2-�쳣; 3-����; 0xff-ȫ��
    char start_log_date[32];				// ��ʼʱ�䣺YYYY-MM-DD hh:mm:ss
    char end_log_date[32];					// ��ֹʱ�䣺YYYY-MM-DD hh:mm:ss
    int  start_index;						// ��ʼ��¼����0��ʼ
    int  max_results;						// ��󷵻ؼ�¼����
    int  log_list_cnt;						// ��־����
    LPNET_CMU_LOG_INFO log_lists;			// ��־��ϸ��Ϣ
} NET_CMU_DEV_LOG, *LPNET_CMU_DEV_LOG;

typedef struct								// �豸����汾��Ϣ
{
    char pu_id[32];							// PU�ı�ʶ
    char version[32];						// ����汾��
    char build_time[32];					// �������ں�ʱ�䣬��ʽΪ: YYYY-MM-DD hh:mm:ss
} NET_CMU_SOFT_VERSION, *LPNET_CMU_SOFT_VERSION;

/*******************************************************************************/
// operation
/*******************************************************************************/
typedef struct								// ɾ���豸�û���Ϣ
{
    char user_name[16];                    // �û���
} NET_CMU_DEL_USER, *LPNET_CMU_DEL_USER;

typedef struct
{
    char file_name[260];
    NET_CMU_DEV_TIME start_time;
    NET_CMU_DEV_TIME stop_time;
    int  size;
    int  type;//TIMER��ʱ¼�� GPIN�澯¼�� MOTION_DETECTION�ƶ����
} NET_CMU_RECORD_FILE, *LPNET_CMU_RECORD_FILE;

typedef struct								// ��ѯ�豸¼���ļ�
{
    int  video_id;							// ��Ƶͨ�� (1,2...)
    int  file_type;							// 255=ȫ����ȱʡ����0=��ʱ¼��1=�ƶ���⣬2=����������3=��Ƶ��ʧ
    NET_CMU_DEV_TIME from_date;			// ��ʼ�����ڼ�ʱ�䡣 ��ʽ��YYYY-MM-DD hh:mm:ss
    NET_CMU_DEV_TIME to_date;			// ���������ڼ�ʱ�䡣 ��ʽ��YYYY-MM-DD hh:mm:ss
    long total_size;						// DISK ������С ��λ��Bytes
    long free_size;							// DISKʣ������ ��λ��Bytes

    int file_list_cnt;
    int  total_num;							// �ܹ��ļ�����
    int  leave_num;							// ʣ���ļ�����
    int  result_code;						// ע���� ��code��: 0��OK  1����֧�ֵ��ļ����� 48���ͻ��˴���	49������������ 50������ʧ��ԭ��
    LPNET_CMU_RECORD_FILE record_file_lists;

    char name[32];
    char file_path[32];
    char address[32];
    int secrecy;
    char type[16];//����¼���ѯ
    char record_id[32];
    int record_file_size;//��ѯ�����ܵ��ļ��ĳ���
} NET_CMU_QUERY_STORE_FILE, *LPNET_CMU_QUERY_STORE_FILE;

typedef struct								// ɾ���豸¼���ļ�
{
    int  video_id;							// ��Ƶͨ���ţ�1��2����
    char file_name[260];					// Ҫɾ�����ļ���
    char from_date[20];						// ��ʼ�����ڼ�ʱ�䡣��ʽ��YYYY-MM-DD hh:mm:ss
    char to_date [20]; 						// ���������ڼ�ʱ�䡣��ʽ��YYYY-MM-DD hh:mm:ss
} NET_CMU_CLEAN_STORE_FILE, *LPNET_CMU_CLEAN_STORE_FILE;

typedef struct								// �豸�ƾ���������
{
    int  ptz_id;				//��̨��ʶ��ʵ����video_id
    char cmd[32];					/*
											��������/��Զ��ZIN/ZOUT
											�������/��Զ��FN/FR
											��Ȧ�Զ�����/����/��С��IA_ON/IO/IC��
											����������WP_ON
											�ƹ⿪����LP_ON

											��������� ��TU, ��TD, ��PL, ��PR, ����TUPR, ����TUPL, ����TDPR, ����TDPL

											Ԥ�õ������
											����Ԥ�õ㣺SET_PRESET
											ɾ��Ԥ�õ㣺DEL_PRESET
											ת��Ԥ��㣺GOTO_PRESET

											�켣Ѳ����
											����Ѳ���㣺SET_CRUISE_POINT
											ɾ��Ѳ���㣺DEL_CRUISE_POINT
											����Ѳ���ٶȣ�SET_CRUISE_SPEED
											����Ѳ��ͣ��ʱ�䣺SET_CRUISE_TIME
											��ʼѲ����START_TRACK_CRUISE

											�Զ�ɨ�裺
											�����Զ�ɨ��߽磺SET_SCAN_EDGE
											�����Զ�ɨ���ٶȣ�SET_SCAN_SPEED
											��ʼ�Զ�ɨ�裺START_SCAN

											ֹͣ��STOP
											*/
    char param[200];			//��cmd��STOPʱ���ò���ָ��Ҫֹͣ�Ŀ��������������̨�˶������
    int param1;						/*
											��cmdΪԤ�õ���ز���ʱ���ò�����ʾԤ������
											��cmdΪ�켣Ѳ����ز���ʱ���ò�����ʾѲ���켣����
											��cmdΪ�Զ�ɨ����ز���ʱ���ò�����ʾ�Զ�ɨ�����
											*/
    int param2;						/*
											��cmdΪ���롢ɾ��Ѳ����ʱ���ò�����ʾԤ�õ�ֵ
											��cmdΪ����Ѳ���ٶ�ʱ���ò�����ʾѲ���ٶ�ֵ
											��cmdΪ����Ѳ��ͣ��ʱ��ʱ���ò�����ʾѲ��ͣ��ʱ��
											��cmdΪ�����Զ�ɨ��߽�ʱ���ò�����ʾ�߽����ͣ�1Ϊ��߽磬2Ϊ�ұ߽�
											��cmdΪ�����Զ�ɨ���ٶ�ʱ���ò�����ʾ�ٶ�ֵ
											*/
    int  speed;						// ��̨�ٶ�	1-8����8���ȼ�����ֵԽ���ٶ�Խ�졣
} NET_CMU_OPER_CTRL_PTZ, *LPNET_CMU_OPER_CTRL_PTZ;

typedef struct								// �豸ɾ����̨Ԥ�õ�
{
    int  ptz_id;							// ��̨��ʶ
    int  preset_index;						// Ԥ�õ�����
    char preset_name[16];					// Ԥ�õ�����
} NET_CMU_OPER_DEL_PRESET, *LPNET_CMU_OPER_DEL_PRESET;

typedef struct								// �豸Զ�̿���
{
    int control_action;						// 0 - reboot  1 - shutdown
} NET_CMU_OPER_CTRL_DEV, *LPNET_CMU_OPER_CTRL_DEV;

typedef struct								// �豸ͨ��ͨ��
{
    char user_name[16];                    // �û���
} NET_CMU_OPER_TRANS_CHANNEL, *LPNET_CMU_OPER_TRANS_CHANNEL;

/*******************************************************************************/
// upload
/*******************************************************************************/
typedef union
{
    char *stringValue;
    int   integerValue;
    /*
    ��ΪDISK_FULL�澯ʱ,ֵΪ"����·��:����ʹ����"
    ��GPIN�澯ʱ,����ֵΪͨ����
    ��CAM�澯ʱ,����ֵΪvideoId
    ��VS_UPGRADE�澯ʱ������ֵΪ"userId, ����ǰ������汾�� �����������汾�ţ����������" ������������루0. �ɹ���1��ʱ��2�����ļ����԰汾��ƥ�䣬3��ȡ�����ļ�ʧ�ܣ�4δ֪��
    ���澯Ϊ���ܷ����澯ʱ��ֵΪͨ����
    */
} NET_CMU_ALARM_DATA, *LPNET_CMU_ALARM_DATA;

typedef struct								// �豸�澯�ϱ�
{
    char pu_id[32];   //��Ƶ��������id
    char pu_ip[16];   //cmu��ip
    int  alarm_type;		//�澯����
    /*
    VS_DISKERROR��Ӳ�̴���
    VS_DISKFULL��Ӳ����
    VS_DISCONN��ǰ���豸�Ͽ�
    VS_UPGRADE��ǰ���豸����
    VS_GPIN���������澯
    CAM_VIDEOLOSE����Ƶ��ʧ
    CAM_HIDEDETECT���ڵ��澯
    CAM_MOTIONDETECT���ƶ����

    ����Ϊ���ܷ���
    VCA_TRAVERSE_PLANE : ��Խ������
    VCA_ENTER_AREA: ��������
    VCA_EXIT_AREA: �뿪����
    VCA_INTRUSION: ����
    VCA_LOITER: �ǻ�
    VCA_LEFT_TAKE: �������
    VCA_PARKING: ͣ��
    VCA_RUN: ����
    VCA_HIGH_DENSITY: ��Ա�ܶ�
    */
    char server_type[32];		//VS����Ƶ������ ������DVS/DVR�Ŀ������澯�� 	CAM�������  �������ƶ���⣬�ڵ���ͼ��澯,���ܷ�����
    int  disk_number;			//�����澯��Ӳ�̺�
    char time_stamp[32];			//�������ں�ʱ�䣬��ʽΪ:	YYYY-MM-DD hh:mm:ss
    NET_CMU_ALARM_DATA data;
    char eliminated[8];			//true:�������� false����������,ȱʡֵ
    unsigned int select_platform;//ƽ̨ѡ��
} NET_CMU_UPLOAD_ALARM, *LPNET_CMU_UPLOAD_ALARM;

typedef struct								// �豸�������ϱ�
{
    char pu_id[32];
    char pu_ip[16];
    char time_stamp[32];	//�������ں�ʱ�䣬��ʽΪ:YYYY-MM-DD hh:mm:ss
    int  digital_in_id;		//��ʪ���豸ID
    char value[32];			//�¶ȡ�ʪ�ȵ�������
} NET_CMU_UPLOAD_DIGITAL, *LPNET_CMU_UPLOAD_DIGITAL;

typedef struct
{
    char pu_id[32];
    int startAlarmPriority;
    int endAlarmPriority;
    int alarmMethod;
    NET_CMU_DEV_TIME startTime;
    NET_CMU_DEV_TIME endTime;
} NET_CMU_QUERY_AlARM;

typedef struct								// �豸Զ�̿���
{
    char pu_id[32];
    int control_action;						//1//1:������λ
} NET_CMU_OPER_ALARMREST_DEV, *LPNET_CMU_OPER_ALARMREST_DEV;


typedef struct
{
    char pu_id[32];
    int video_id;
    int alarm_type;///*�������ͣ�0��������� */ 
                   /*1���ƶ���⣨Ĭ�ϻ��������Χ,��������ߣ�����ʱ��Ϊ����ʱ�䣩��
                    2��ȫ������**/
    char enable_arming[16];/*true ������false ����*/
}NET_CMU_OPER_CTRL_ALARM,*LP_NET_CMU_OPER_CTRL_ALARM;


typedef struct								// �豸Զ�̿���
{
    char pu_id[32];
    int control_action;						// 0 - ����  1 - ����
} NET_CMU_OPER_GUARD_DEV, *LPNET_CMU_OPER_GUARD_DEV;

enum
{
    PU_DUPLICATE_REGISTER,//�ظ�ע��
    PU_REGISTER_FAILD,//��SERVERע��ʧ��
    PU_KEEPALIVE_FAILD,//��SERVER��������ʧ��
    PU_INIT_FAILED,//SERVERͨѶģ���ʼ��ʧ��
    ERR_INVALID_PARAM,   //��������
    ERR_MEMEORY_MALLOC,//�ڴ�����ʧ��
    ERR_QUEUE_INITIALEZE,
    ERR_PTHREAD_CREATE
};
#endif


