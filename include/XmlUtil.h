#ifndef XML_UTIL_H_
#define XML_UTIL_H_

#include "TSocket.h"
#include "tinyxml.h"
#include <string>
#include "Vagsdk.h"
#include <vector>


struct MvRegInfoEx    
{
	std::string m_mduIp;
	int m_mduPort;

	int m_keepAlivePeriod;
	int m_puKeepAlivePort;
	int m_code;
};

struct CmuKeepAlive   
{
	std::string m_puId;
	std::string m_puName;
	int m_cmuKeepAlivePeriod;
};

struct MvMsgHeader     // Э��ͷ
{
	unsigned short ver;
	unsigned short length;
	unsigned int  cseq;
};

struct ControlPtzMsg    
{
	std::string m_puId;
	int  m_videoId;
	std::string  m_cmd;
	int  m_speed;
	std::string  m_data;
};

struct DeletePtzPreset
{
	int  ptz_id;							/// ��̨��ʶ
	int  preset_index;						/// Ԥ�õ�����
	char preset_name[16];					/// Ԥ�õ�����
};

typedef struct
{
	int  preset_index;						// Ԥ�õ�����
	char preset_name[16];					// Ԥ�õ�����
} CMU_PRESET, *LP_CMU_PRESET;

struct PtzPresets							// �豸��̨Ԥ�õ�
{
	int ptz_id;								// ��̨��ʶ
	int preset_list_cnt;					//
	CMU_PRESET *preset_lists;			//
};

typedef struct										// �豸ʱ�����
{
	int hour;										// ʱ (0~23)
	int minute;										// �� (0~59)
	int second;										// �� (0~59)
	int date;										// �� (1~31)
	int month;										// �� (1~12)
	int year;										// �� nnnn
} CMU_DEV_TIME, *LP_CMU_DEV_TIME;

typedef struct										// �豸�������
{
	int   net_id;									// ���ڱ�� (1,2,3...)
	char  ip_addr[16];								// IP��ַ
	int   ctrl_port;								// ҵ����ƶ˿ں�
	char  net_mask[16];								// ��������
	char  dns_addr[2][16];							// DNS��������ַ,�����ж��
	char  gate_way[16];								// ���ص�ַ
} CMU_IP_INFO, *LP_CMU_IP_INFO;

/*
struct StartMediaReq 
{
	std::string m_puId;
	int  m_videoId;
	int  m_iMediaType;
	int  m_iStreamType;
	int  m_iLinkMode;
	std::string  m_szTargetIp;
	int m_targetPort;
};
*/
struct StartMediaReq 
{
	std::string m_puId;
	int  m_videoId;
	int  m_iMediaType;
	int  m_iStreamType;
	int  m_iLinkMode;
	std::string  m_szTargetIp;
	int m_targetPort;
    std::string m_fileName;
    std::string m_startTime;
    std::string m_endTime;
    int m_playType;
    std::string m_callId;
};

struct StopMediaReq 
{
	std::string m_puId;
	int  m_videoId;
	int  m_iMediaType;
	int  m_iStreamType;
	std::string  m_fileName;
};

struct PlayBackReq 
{
	std::string		m_cuId;
	std::string		m_puId;
	std::string		m_fileName;
	std::string		m_startTime;
	std::string		m_endTime;
	std::string		m_cuSessId;
    std::string     m_cmd;
	int				m_videoId;
	int				m_param;
    int				m_sessId;
	int				m_playRate;
	int				m_dragValue;
	bool			m_bDownload;
};
typedef struct									// �������澯���
{
	//	char enable[5];								// �澯ʹ�� �澯��true; ���澯:false
	int alarm_channel;							// �澯���ͨ��
	int alarm_time;								// �����澯���������ʱ�䣨�룩
} CMU_ALARM_OUTPUT, *LP_CMU_ALARM_OUTPUT;

typedef struct
{
	int	alarm_record_channel;					// ����������¼��ͨ����
	int	alarm_shot_channel;						// ����������ץ��ͨ����
} CMU_ALARM_VIDEO, *LP_CMU_ALARM_VIDEO;

typedef struct
{
	int x;										// X���ꣻ���Ͻ�Ϊԭ��
	int y;										// Y����
	int width;									// ���
	int height;									// �߶�
} CMU_COORDINATE, *LP_CMU_COORDINATE;

typedef struct									// �豸��Ƶ�ڱθ澯����
{
	int	 video_id;								// ��Ƶͨ�� (1,2...)
	char alarm_enabled[8];						// �Ƿ�ʹ����Ƶͨ�����ڵ���⡣ true:��ʾ��� false����ʾ�����
	char alarm_time[16];						// ����ʱ��(hhmm-hhmm, ǰ��2λ��ʾСʱ������2Ϊ��ʾ���� )
	CMU_COORDINATE hide_coord;				//
	int  sensitivity;							// �����ȡ� 0���ر� 1���� 2���� 3����

	CMU_ALARM_OUTPUT   hide_alarm_output;	// �������澯���
	CMU_ALARM_VIDEO    hide_alarm_video;    // �������澯¼��
} CMU_HIDE_DETECT, *LP_CMU_HIDE_DETECT;

typedef struct
{
	int video_id;									// ��Ƶͨ����ʶ��1,2...��
	int video_format;								// ͼ����ʽ 0��PAL 1��NTSC
} CMU_VIDEO_FORMAT, *LP_CMU_VIDEO_FORMAT;

typedef struct										// �豸ͼ����ʾ����
{
	int video_id;									// ��Ƶͨ�� (1,2...)
	int contrast;									// �Աȶȣ�0-255��
	int bright;										// ����   (0-255��
	int hue;										// ɫ��  ��0-255��
	int saturation;									// ���Ͷȣ�0-255��
} CMU_IMG_DISPLAY, *LP_CMU_IMG_DISPLAY;

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
} CMU_SERIAL_PORT, *LP_CMU_SERIAL_PORT;

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
} CMU_IMG_ENCODE, *LP_CMU_IMG_ENCODE;

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
} CMU_IMG_TEXT, *LP_CMU_IMG_TEXT;

typedef struct								// �豸Զ�̿���
{
	int control_action;						// 0 - reboot  1 - shutdown
} CMU_OPER_CTRL_DEV, *LP_CMU_OPER_CTRL_DEV;

typedef struct
{
    char pu_id[32];
    int video_id;
    int alarm_type;
    char enable_arming[16];
}CMU_OPER_CTRL_ALARM,*LP_CMU_OPER_CTRL_ALARM;

typedef struct
{
    char file_name[260];
    CMU_DEV_TIME start_time;
    CMU_DEV_TIME stop_time;
    int  size;
    int  type;//TIMER��ʱ¼�� GPIN�澯¼�� MOTION_DETECTION�ƶ����
} CMU_RECORD_FILE, *LPCMU_RECORD_FILE;

typedef struct								// ��ѯ�豸¼���ļ�
{
    int  video_id;							// ��Ƶͨ�� (1,2...)
    int  file_type;							// 255=ȫ����ȱʡ����0=��ʱ¼��1=�ƶ���⣬2=����������3=��Ƶ��ʧ
    CMU_DEV_TIME from_date;			// ��ʼ�����ڼ�ʱ�䡣 ��ʽ��YYYY-MM-DD hh:mm:ss
    CMU_DEV_TIME to_date;			// ���������ڼ�ʱ�䡣 ��ʽ��YYYY-MM-DD hh:mm:ss
    long total_size;						// DISK ������С ��λ��Bytes
    long free_size;							// DISKʣ������ ��λ��Bytes

    int file_list_cnt;
    int  total_num;							// �ܹ��ļ�����
    int  leave_num;							// ʣ���ļ�����
    int  result_code;						// ע���� ��code��: 0��OK  1����֧�ֵ��ļ����� 48���ͻ��˴���	49������������ 50������ʧ��ԭ��
    LPCMU_RECORD_FILE record_file_lists;

    char name[32];
    char file_path[32];
    char address[32];
    int secrecy;
    char type[16];//����¼���ѯ
    char record_id[32];
    int record_file_size;//��ѯ�����ܵ��ļ��ĳ���
} CMU_QUERY_STORE_FILE, *LPCMU_QUERY_STORE_FILE;


typedef std::vector<vag::ResNode> VagResNodeArray;

std::string structToXml(const vag::AlarmNotification* alarm);
std::string structToXml(const vag::VAGRegInfo &regInfo);
std::string structToXml(const CmuKeepAlive &alive);
std::string structToXml(unsigned maxPage,
						unsigned curPage,
						const std::string &vagid,
						const VagResNodeArray &resLst);

bool structFromXmlElement(MvRegInfoEx& obj, TiXmlElement* pElement);
bool structFromXmlElement(ControlPtzMsg& obj, TiXmlElement* pElement);
bool structFromXmlElement(StartMediaReq& obj, TiXmlElement* pElement);
bool structFromXmlElement(StopMediaReq& obj, TiXmlElement* pElement);
bool structFromXmlElement(DeletePtzPreset& obj, TiXmlElement* pElement);
bool structFromXmlElement(PtzPresets& obj, TiXmlElement* pElement);
bool structFromXmlElement(CMU_DEV_TIME& obj, TiXmlElement* pElement);
bool structFromXmlElement(CMU_IP_INFO& obj, TiXmlElement* pElement);

bool structFromXmlElement(CMU_HIDE_DETECT& obj, TiXmlElement* pElement);
bool structFromXmlElement(CMU_VIDEO_FORMAT& obj, TiXmlElement* pElement);
bool structFromXmlElement(CMU_IMG_DISPLAY& obj, TiXmlElement* pElement);
bool structFromXmlElement(CMU_SERIAL_PORT& obj, TiXmlElement* pElement);
bool structFromXmlElement(CMU_IMG_ENCODE& obj, TiXmlElement* pElement);
bool structFromXmlElement(CMU_IMG_TEXT& obj, TiXmlElement* pElement);

bool structFromXmlElement(CMU_OPER_CTRL_DEV& obj, TiXmlElement* pElement);
bool structFromXmlElement(CMU_OPER_CTRL_ALARM& obj, TiXmlElement* node);
bool structFromXmlElement(CMU_QUERY_STORE_FILE& obj, TiXmlElement* node);
bool structFromXmlElement(PlayBackReq& obj, TiXmlElement* node);
template < class T >
bool structFromXml(T& t, const std::string& xml)
{
	try
	{
		TiXmlDocument doc;
		doc.Parse(xml.c_str());
		if (doc.Error())
		{		
			return false;
		}
		TiXmlElement* node = doc.RootElement();
		if (node == NULL)
		{
			return false;
		}	
		return structFromXmlElement(t, node);
	}
	catch (...)
	{		
		return false;
	}
}


#endif




