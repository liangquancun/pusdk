/********************************************************************
created:	2013/12/24
modify :    2013/12/24  
author:		
purpose:    VAG-SDK接口
*********************************************************************/
#ifndef VIDEO_ACCESS_GATEWAY_H
#define VIDEO_ACCESS_GATEWAY_H

#ifdef WIN32
#include <stddef.h>
#include <windef.h>
#else
#include <unistd.h>
#endif
#include <time.h>

#include "ms_pu_lib.h"

extern unsigned long g_param;

/////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
	#ifdef _WINDLL
		#define DLLEXPORT __declspec(dllexport)
	#else
		#define DLLEXPORT
	#endif
#else
	#define DLLEXPORT
	#define WINAPI
#endif //WIN32
/////////////////////////////////////////////////////////////////////////////


namespace vag
{  

////基础数据类型
typedef unsigned char            uint8_t;
typedef unsigned short           uint16_t;
typedef unsigned int             uint32_t;
typedef long long                int64_t;
typedef unsigned long long       uint64_t;



enum Constants
{
	IPV4_LEN    = 16,
	MAX_ID      = 24,
	MAX_NAME    = 256,
	MAX_DESC    = 256,
	MAX_SPROP   = 512
};


///告警、故障类型
enum AlarmType
{
    ALARM_INTRUSION = 1,/// 入侵
    ALARM_LOITER,       /// 徘徊
    ALARM_MOVING,       /// 移动侦测
    ALARM_STAY,         /// 逗留
    ALARM_LEFT_TAKE,    /// 遗留物检测
    ALARM_LINE,         /// 穿越绊线
    ALARM_DISK_FULL,    /// 硬盘满
    ALARM_DISK_FAULT,   /// 硬盘故障
    ALARM_SERVER_OFF,   /// 服务器断开
    ALARM_VIDEO_LOST,   /// 视频丢失
    ALARM_ENCODER_OFF,  /// 编码器断开
    ALARM_INPUT,        /// 告警输入告警
    ALARM_USER_PLAY_ERROR     /// 用户实时预览异常
};



///告警、状态通知的状态
enum StatusType
{
	STATUS_BEGIN = 0,   ///状态开始
	STATUS_END = 1      ///状态结束
};


/*
///告警通知
struct AlarmNotification
{
	char    id[MAX_ID];     ///资源编码,可以是VAGID或者PUID
	uint32_t videoId;       ///通道ID
	char    name[MAX_NAME]; ///资源名字
	int     type;           ///告警类型
	int     priority;       ///告警级别
	time_t  time;           ///告警时间 
	int     status;         ///告警状态
	char    description[MAX_DESC]; ///告警描述
};*/

typedef union
{
    char *stringValue;
    int   integerValue;
} NET_CMU_ALARM_DATA, *LPNET_CMU_ALARM_DATA;

struct AlarmNotification
{
    char pu_id[32];   ///视频服务器的id
    char pu_ip[16];   ///cmu的ip
    int  alarm_type;		//告警类型

    char server_type[32];		///VS：视频服务器 （包括DVS/DVR的开关量告警） 	CAM：摄像机  （包括移动侦测，遮挡等图像告警,智能分析）
    int  disk_number;			///发生告警的硬盘号
    char time_stamp[32];			///包括日期和时间，格式为:	YYYY-MM-DD hh:mm:ss
    NET_CMU_ALARM_DATA data;
    char eliminated[8];			///true:警报消除 false：警报发生,缺省值
    unsigned int select_platform;///平台选择
};

///状态通知
struct StatusNotification
{
	char     id[MAX_ID];  ///资源编码，可以是VAGID或者PUID
	uint32_t videoId;     ///通道ID
	time_t   time;        ///状态发生时间
	int      status;      ///状态
};



//错误类型定义
enum VagErrorCode
{
	VAG_ERR_FAIL              = -1,  //失败
	VAG_ERR_OK                = 0,   //成功
	VAG_ERR_NO_INIT           = 1,   //未初始化
	VAG_ERR_AUTH_FAIL         = 2,   //鉴权失败
	VAG_ERR_OUT_OF_SERVER     = 3,   //不能提供服务
	VAG_ERR_CONNT_AGU_FAIL    = 4,   //连接接入网关服务器失败
	VAG_ERR_SOCKET_ERR        = 5,   //SOCKET错误
	VAG_ERR_RECV_TIMEOUT      = 6,   //网络接收超时
	VAG_ERR_MEM_ALLOC_FAIL    = 7,   //分配内存错误
	VAG_ERR_REPORT_RES_FAIL   = 8,   //上报资源失败
	VAG_ERR_INVALID_RES       = 9,   //无效的资源
	VAG_ERR_SEND_RTP_FAIL     = 10,  //发送流媒体失败
	VAG_ERR_INVAID_CHANNEL    = 11,  //错误的通道号
	VAG_ERR_RES_NO_EXIST      = 12,  //请求的资源不存在
	VAG_ERR_FILE_NO_EXIST     = 13,  //文件不存在
	VAG_ERR_RES_NO_ONLINE     = 14,  //请求的资源不在线
	VAG_ERR_R_OVER_MAXLINK    = 15,  //资源超过最大连接数
	VAG_ERR_ISFINDING         = 16,  //录像文件正在查询中

	VAG_ERR_VERSION_NO_MATCH  = 98,  //版本不匹配
	VAG_ERR_OTHER_FAIL        = 99   //其它失败原因
};





///上报资源信息
struct ResNode
{
	char     puid[MAX_ID];        /// 统一资源编码
	uint32_t videoId;             /// 通道ID
	char     name[MAX_NAME];      /// 资源名称
	int      codec;               /// 解码标签
	int      status;              /// 状态，0可用，1不可用
	int      hasPTZ;              /// 是否支持PTZ
    char     desc[MAX_NAME];      /// 资源描述
};





///录像类型
enum RecordType
{
	RECORD_SYSTEM   = 0x01, /// 平台录像
	RECORD_FRONTEND = 0x02, /// 前端录像
	RECORD_MANUAL   = 0x04, /// 手动录像
	RECORD_ALARM    = 0x08  /// 告警录像
};


///录像信息
struct RecordItem
{
	char     id[MAX_ID];         ///资源编码
	uint32_t videoId;            ///通道ID
	time_t   beginTime;          ///实际开始时间
	time_t   endTime;            ///实际结束时间
	int      type;               ///录像类型
	char     recordID[MAX_NAME]; ///该片段录像信息唯一标签编码（区别于其它片段）
};


///查询录像信息请求
struct QueryRecordRequest
{
	char     id[MAX_ID];    ///资源编码
	uint32_t videoId;       ///通道ID
	time_t   beginTime;     ///开始时间
	time_t   endTime;       ///结束时间
	int      type;          ///录像类型
};


///查询录像信息响应
struct QueryRecordResponse
{  
    int  sum;               ///输入限制返回的总数目，即pItems的内存长度；输出实际返回的录像信息数量
    RecordItem* pItems;     ///录像信息结构
};


//VAG注册信息
struct VAGRegInfo
{
	char  VagId[MAX_ID];              //VAG的编码ID号
	char  VagUserName[MAX_NAME];      //VAG的用户名
	char  VagPassword[MAX_NAME];      //VAG的密码
	char  VagIp[IPV4_LEN];            //VAG的IP地址
	char  CmuId[MAX_ID];              //VAG要注册的CMU中心服务器的编码
	char  CmugIp[IPV4_LEN];           //VAG要注册的CMU中心服务器IP地址
	//uint16_t CmuPort;                 //VAG要注册的CMU中心服务器的端口	
    int CmuPort;
};


///媒体标签，对应解码库
enum CompressionCodec
{	
	G711U_CODEC            = 0,   
	G711A_CODEC            = 0x08,
	AAC_ADTS_CODEC         = 0x20,

	STANDARD_MPEG4_CODEC   = 97,
	STANDARD_H264_CODEC    = 98,
	HIK_MEDIA_CODEC        = 101,
	DH_MEDIA_CODEC         = 102,
	POVAG_MEDIA_CODEC      = 103
};


///媒体信息描述
struct StreamFormat
{
	double duration;			 ///媒体总时长（秒），历史视频有效
	int codec;                   ///媒体标签，对应解码库
	int width;                   ///视频宽度
	int height;                  ///视频高度
	int fps;                     ///帧率
	int bitrate;                 ///码率
	int iFrame;                  ///关键帧间隔
	int propSize;                ///媒体描述信息长度，即prop实际长度
	uint8_t prop[MAX_SPROP];     ///媒体描述信息，比如海康40字节解码头数据
	char mediaName[MAX_ID];      ///媒体名字

	int audioCodec;              ///音频编码
	int audioChannel;            ///音频通道
	int audioVAGmpleRate;         ///采样率VAGmple rate
	int audioPropSize;           ///音频描述信息长度，即audioProp实际长度
	uint8_t audioProp[MAX_DESC]; ///音频描述信息
};


///媒体数据帧类型
enum FrameType
{
	FRAME_G711U    = 0,   
	FRAME_G711A    = 0x08,
	FRAME_AAC_ADTS = 0x20,

	FRAME_VIDEO_I  = 0x10,
	FRAME_VIDEO_P  = 0x11,
	FRAME_VIDEO_B  = 0x12	
};



///媒体数据发送结构
struct StreamPacket
{
	uint8_t* data;      ///媒体数据
	int      size;      ///媒体数据长度
	int      flag;      ///媒体帧类型
	uint64_t timestamp; ///数据帧产生的时间，milliseconds    
};

///预置位类型
enum PresetFlag
{
	PRESET_FLAG_NORM  = 0,   ///普通预置位
	PRESET_FLAG_HOME  = 1    ///守望位,看守位
};


///预置位信息
struct Preset
{
	int  num;            ///预置位编号
	char name[MAX_NAME]; ///预置位名字
	int  flag;           ///预置位类型
};



///云镜控制动作
enum PTZCommand
{
	PTZ_CMD_START   = 0,   ///动作开始  
	PTZ_CMD_STOP    = 1,   ///动作停止

	PTZ_CMD_RIGHT   = 2,   ///右	
	PTZ_CMD_LEFT    = 4,   ///左	
	PTZ_CMD_DOWN    = 6,   ///下	
	PTZ_CMD_UP      = 8,   ///上	

	PTZ_CMD_SMALL   = 10,    ///光圈变小
	PTZ_CMD_LARGE   = 12,    ///光圈变大

	PTZ_CMD_ZOOM_IN  = 14,   ///焦距变大
	PTZ_CMD_ZOOM_OUT = 16,   ///焦距变小

	PTZ_CMD_FOCUS_IN  = 18,  ///焦点前调	
	PTZ_CMD_FOCUS_OUT = 20,  ///焦点后调

	PTZ_CMD_ASSIST_FUNC_1 = 30,	 ///雨刷
	PTZ_CMD_ASSIST_FUNC_2 = 32,	 ///辅佐开关2
	PTZ_CMD_ASSIST_FUNC_3 = 34,	 ///辅佐开关3
	PTZ_CMD_ASSIST_FUNC_4 = 36,	 ///辅佐开关4

	PTZ_RIGHT_APPLY   = 40,    ///权限申请
	PTZ_RIGHT_RELEASE = 42,    ///权限释放

	PTZ_CMD_SCAN = 120,        ///自动扫描开     
	PTZ_CMD_LENGTH
};


///媒体数据处理
class  StreamSink
{
public:
	virtual ~StreamSink() {}

	 //在第一次写数据前需要告诉SDK视频数据的格式（仅需要设置一次），以便解码库解码    
	virtual void setFormat(const StreamFormat& fmt) =0;

	 //在请求媒体流时，调用该函数将流媒体数据写入SDK，由SDK负责发送   
	virtual int writePacket(const StreamPacket& packet) =0;

	///通知发送媒体数据结束
	virtual void endOfStream() =0;
};


///媒体控制，实时视频只需实现部分，历史视频都需实现
class  MediaPlayer
{
public:
	///媒体状态
	enum State
	{
		STATE_OPEN = 0,
		STATE_STOP,
		STATE_PAUSE,
		STATE_PLAY
	};

	virtual ~MediaPlayer() {}

	///或者媒体格式
	virtual int getFormat(StreamFormat& fmt) =0;

	///播放
	virtual int play() =0;

	///暂停
	virtual int pause() =0;

	///停止
	virtual int stop() =0;

	///拖拽,相对于实际开始时间的秒数
	virtual int setSeek(int offset) =0;

	///倍速，1/4.0 ---- 4.0
	virtual int setScale(double scale) =0;

    virtual int step() =0;

	///获取播放位置
	virtual int64_t getPos() =0;

	///获取媒体状态
	virtual int getState() =0;

	///设置流获取回调
	virtual int setSink(StreamSink* pSink) =0;
};


///回调类
class  NotificationSink
{
public:
	virtual ~NotificationSink() {}

	/**
     * VAG注册成功回调
     */
	virtual int OnVagRegisterOk() =0;

	/**
     * 请求资源上报
     */
	virtual int OnResReport() =0;

	/**
     * 打开实时视频
     */
    virtual int OnOpenLiveStream(const char* resId, 
		uint32_t videoId, int streamIdx,
		MediaPlayer** ppPlayer) =0;
   
    /**
     * 关闭视频流
     */
    virtual int OnCloseStream(MediaPlayer* pPlayer) =0;



    /**
     * 控制云镜,
	 * @param cmd [in] 命令
	 * @param data [in] 动作（开始、停止）
     */
    virtual int OnControlPtz(const char* id, 
		uint32_t videoId, int cmd, int speed, int data) =0;

   	/**
	* 设置预置位
	*/
	virtual int OnSetPreset(const char* id, uint32_t videoId, 
		const Preset& preset) =0;

	/**
	* 删除预置位
	*/
	virtual int OnDelPreset(const char* id, uint32_t videoId, int num) =0;

	/**
	* 调用预置位
	*/
	virtual int OnGotoPreset(const char* id, uint32_t videoId, int num) =0;


    /**
     * 查询历史视频
     */
    virtual int OnQueryRecord(const QueryRecordRequest& req,
		QueryRecordResponse& resp) =0;

    /**
     * 打开历史视频流
     */
    virtual int OnOpenRecordStream(const RecordItem& rec, 
		MediaPlayer** ppStream) =0;


   /**
	* 请求关键帧
	*/
    virtual int OnMakeKeyFrame (const char *resId,
		uint32_t videoId, int streamIdx) =0;
};


///资源变更通知命令
enum ResUpdateCmd
{
	RESNODE_ADD_CMD  = 0, 	
	RESNODE_DEL_CMD  = 0x01,
	RESNODE_MOD_CMD  = 0x02
};

typedef int ( *msg_proc )(
						  char *cmd, 
						  char *buf, 
						  unsigned long *size, 
						  unsigned long param );

typedef int ( *stream_proc )( 
							 unsigned long id, 
							 char *cmd, 
							 char *buf, 
							 unsigned long size, 
							 unsigned long param );

//视频代理接口 
class VAGProxy
{
public:
	
	 /**
     * 注册
	 * @param regInfo [in] 注册信息
     */
    virtual int Login(const VAGRegInfo &regInfo);  
    virtual int Logout();

   

	 /**
     * 状态通知,告警通知
	 * @param status [in] 信息
     */
	virtual int NotifyStatus(const StatusNotification* status,size_t length);
	virtual int NotifyAlarm(const AlarmNotification* alarm,size_t length);
	 /**
     * 设置回调
	 * @param pSink [in] 回调类指针
     */
	virtual	int SetNotificationSink(msg_proc msg_cb, stream_proc stream_cb,unsigned long param);
	virtual int SetVoiceIntercomCallBack(net_voice_intercom_proc voice_callback);

protected:
	virtual ~VAGProxy() {}
};


}





/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif

/////////////////////////////////////////////////////////////////////////////


/**
 * 获取VAGProxy接口，需要调用VAG_destroy释放
 * @return VAGProxy接口
 */
DLLEXPORT vag::VAGProxy* WINAPI VAG_create();


/**
 * 释放VAGProxy接口
 * @param pVAG 该指针由VAG_create返回
 *
 */
DLLEXPORT void WINAPI VAG_destroy(vag::VAGProxy* pVAG);


/**
 * 获取版本
 */
DLLEXPORT const char* WINAPI VAG_getVersion();


/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

/////////////////////////////////////////////////////////////////////////////


#endif


