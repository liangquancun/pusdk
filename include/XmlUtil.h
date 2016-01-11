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

struct MvMsgHeader     // 协议头
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
	int  ptz_id;							/// 云台标识
	int  preset_index;						/// 预置点索引
	char preset_name[16];					/// 预置点名称
};

typedef struct
{
	int  preset_index;						// 预置点索引
	char preset_name[16];					// 预置点名称
} CMU_PRESET, *LP_CMU_PRESET;

struct PtzPresets							// 设备云台预置点
{
	int ptz_id;								// 云台标识
	int preset_list_cnt;					//
	CMU_PRESET *preset_lists;			//
};

typedef struct										// 设备时间参数
{
	int hour;										// 时 (0~23)
	int minute;										// 分 (0~59)
	int second;										// 秒 (0~59)
	int date;										// 日 (1~31)
	int month;										// 月 (1~12)
	int year;										// 年 nnnn
} CMU_DEV_TIME, *LP_CMU_DEV_TIME;

typedef struct										// 设备网络参数
{
	int   net_id;									// 网口编号 (1,2,3...)
	char  ip_addr[16];								// IP地址
	int   ctrl_port;								// 业务控制端口号
	char  net_mask[16];								// 子网掩码
	char  dns_addr[2][16];							// DNS服务器地址,可以有多个
	char  gate_way[16];								// 网关地址
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
typedef struct									// 联动到告警输出
{
	//	char enable[5];								// 告警使能 告警：true; 不告警:false
	int alarm_channel;							// 告警输出通道
	int alarm_time;								// 发生告警后，输出保持时间（秒）
} CMU_ALARM_OUTPUT, *LP_CMU_ALARM_OUTPUT;

typedef struct
{
	int	alarm_record_channel;					// 报警触发的录象通道号
	int	alarm_shot_channel;						// 报警触发的抓拍通道号
} CMU_ALARM_VIDEO, *LP_CMU_ALARM_VIDEO;

typedef struct
{
	int x;										// X坐标；左上角为原点
	int y;										// Y坐标
	int width;									// 宽度
	int height;									// 高度
} CMU_COORDINATE, *LP_CMU_COORDINATE;

typedef struct									// 设备视频遮蔽告警参数
{
	int	 video_id;								// 视频通道 (1,2...)
	char alarm_enabled[8];						// 是否使能视频通道的遮挡检测。 true:表示检测 false：表示不检测
	char alarm_time[16];						// 布防时间(hhmm-hhmm, 前面2位表示小时，后面2为表示分钟 )
	CMU_COORDINATE hide_coord;				//
	int  sensitivity;							// 灵敏度。 0：关闭 1：低 2：中 3：高

	CMU_ALARM_OUTPUT   hide_alarm_output;	// 联动到告警输出
	CMU_ALARM_VIDEO    hide_alarm_video;    // 联动到告警录像
} CMU_HIDE_DETECT, *LP_CMU_HIDE_DETECT;

typedef struct
{
	int video_id;									// 视频通道标识（1,2...）
	int video_format;								// 图像制式 0：PAL 1：NTSC
} CMU_VIDEO_FORMAT, *LP_CMU_VIDEO_FORMAT;

typedef struct										// 设备图像显示参数
{
	int video_id;									// 视频通道 (1,2...)
	int contrast;									// 对比度（0-255）
	int bright;										// 亮度   (0-255）
	int hue;										// 色调  （0-255）
	int saturation;									// 饱和度（0-255）
} CMU_IMG_DISPLAY, *LP_CMU_IMG_DISPLAY;

typedef struct										// 设备串口参数
{
	char for_decoder[8];							// true：获取云台解码器信息 false：获取设置串口信息
	int  serial_port;								// 串口编号 (1,2...), 当forDecoder为false时，需要携带该字段
	int  video_id;									// 通道号(1,2...)，当forDecoder为true时，需要携带该字段
	int  baud_rate;									// 波特率(bps) (50, 75, 110, 150, 300, 600, 1200, 2400, 4800, 9600, 19200 38400, 57600, 115200)
	int  data_bit;									// 数据位  （5、6、7、8）
	int  parity;									// 校验 （0－无校验，1－奇校验，2－偶校验）
	int  stop_bit;									// 停止位 （1、2）
	char mode[16];									// (RS485，RS232)
	int  time_out;									// 接收数据超时 （0...65535） unit(100ms)
	int  flow_control;								// 0－无流控，1－软流控,2-硬流控
	int  decoder_type;								// 云台解码器类型, 当forDecoder为true时，需要携带该字段
	int  decoder_address;							// 云台解码器地址, 当forDecoder为true时，需要携带该字段
	int  work_mode;									// 当forDecoder为false时，需要携带该字段 1控制台 2透明通道
} CMU_SERIAL_PORT, *LP_CMU_SERIAL_PORT;

typedef struct										// 设备图像编码参数
{
	int  video_id;									// 视频通道 (1,2...)
	char encode_mode[32];							// 编码模式MJPEG MPEG-1 MPEG-2 MPEG-4 H261 H263 H264 AVS
	int  pic_quality;								// 图像质量 （0-最好 1-次好 2-较好 3-一般 4-较差 5-差）
	int  bit_rate;									// 码率 unit(K/s)
	int  bit_rate_type;								// 码率类型0:定码率，1:变码率
	int  frame_rate;								// 帧率1fps, 2fps, 4fps, 6fps, 8fps, 10fps, 12fps, 16fps 20fps, 25fps
	char image_size[32];							// DCIF,  CIF,  QCIF, 2CIF, 4CIF, 720I, 720P, 1080I, 1080P
	int  stream_type;								// 如果前端设备可以编码两种码流，则 0-主码流 1-子码流
	int  iframe_interval;							// I帧间隔
} CMU_IMG_ENCODE, *LP_CMU_IMG_ENCODE;

typedef struct										// 设备图像相关的文字参数
{
	int  video_id;									// 视频通道 (1,2...)
	char time_enable[8];//need chage to char		// 是否显示时间 false-hiden  true-show
	int  time_X;									// 时间戳的X坐标
	int  time_Y;									// 时间戳的Y坐标
	char text_enable[8];//need chage to char		// 是否显示文字 false-hiden  true-show
	char text[128];									// 文字内容
	int  text_X;									// 文字的X坐标
	int  text_Y;									// 文字的Y坐标
} CMU_IMG_TEXT, *LP_CMU_IMG_TEXT;

typedef struct								// 设备远程控制
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
    int  type;//TIMER定时录像 GPIN告警录像 MOTION_DETECTION移动侦测
} CMU_RECORD_FILE, *LPCMU_RECORD_FILE;

typedef struct								// 查询设备录像文件
{
    int  video_id;							// 视频通道 (1,2...)
    int  file_type;							// 255=全部（缺省），0=定时录像，1=移动侦测，2=报警触发，3=视频丢失
    CMU_DEV_TIME from_date;			// 开始的日期及时间。 格式：YYYY-MM-DD hh:mm:ss
    CMU_DEV_TIME to_date;			// 结束的日期及时间。 格式：YYYY-MM-DD hh:mm:ss
    long total_size;						// DISK 容量大小 单位：Bytes
    long free_size;							// DISK剩余容量 单位：Bytes

    int file_list_cnt;
    int  total_num;							// 总共文件条数
    int  leave_num;							// 剩余文件条数
    int  result_code;						// 注册结果 （code）: 0：OK  1：不支持的文件类型 48：客户端错误	49：服务器错误 50：其他失败原因
    LPCMU_RECORD_FILE record_file_lists;

    char name[32];
    char file_path[32];
    char address[32];
    int secrecy;
    char type[16];//国标录像查询
    char record_id[32];
    int record_file_size;//查询到的总得文件的长度
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




