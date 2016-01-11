/********************************************************************
created:	2013/12/24
modify :    2013/12/24  
author:		
purpose:    VAG-SDK�ӿ�
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

////������������
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


///�澯����������
enum AlarmType
{
    ALARM_INTRUSION = 1,/// ����
    ALARM_LOITER,       /// �ǻ�
    ALARM_MOVING,       /// �ƶ����
    ALARM_STAY,         /// ����
    ALARM_LEFT_TAKE,    /// ��������
    ALARM_LINE,         /// ��Խ����
    ALARM_DISK_FULL,    /// Ӳ����
    ALARM_DISK_FAULT,   /// Ӳ�̹���
    ALARM_SERVER_OFF,   /// �������Ͽ�
    ALARM_VIDEO_LOST,   /// ��Ƶ��ʧ
    ALARM_ENCODER_OFF,  /// �������Ͽ�
    ALARM_INPUT,        /// �澯����澯
    ALARM_USER_PLAY_ERROR     /// �û�ʵʱԤ���쳣
};



///�澯��״̬֪ͨ��״̬
enum StatusType
{
	STATUS_BEGIN = 0,   ///״̬��ʼ
	STATUS_END = 1      ///״̬����
};


/*
///�澯֪ͨ
struct AlarmNotification
{
	char    id[MAX_ID];     ///��Դ����,������VAGID����PUID
	uint32_t videoId;       ///ͨ��ID
	char    name[MAX_NAME]; ///��Դ����
	int     type;           ///�澯����
	int     priority;       ///�澯����
	time_t  time;           ///�澯ʱ�� 
	int     status;         ///�澯״̬
	char    description[MAX_DESC]; ///�澯����
};*/

typedef union
{
    char *stringValue;
    int   integerValue;
} NET_CMU_ALARM_DATA, *LPNET_CMU_ALARM_DATA;

struct AlarmNotification
{
    char pu_id[32];   ///��Ƶ��������id
    char pu_ip[16];   ///cmu��ip
    int  alarm_type;		//�澯����

    char server_type[32];		///VS����Ƶ������ ������DVS/DVR�Ŀ������澯�� 	CAM�������  �������ƶ���⣬�ڵ���ͼ��澯,���ܷ�����
    int  disk_number;			///�����澯��Ӳ�̺�
    char time_stamp[32];			///�������ں�ʱ�䣬��ʽΪ:	YYYY-MM-DD hh:mm:ss
    NET_CMU_ALARM_DATA data;
    char eliminated[8];			///true:�������� false����������,ȱʡֵ
    unsigned int select_platform;///ƽ̨ѡ��
};

///״̬֪ͨ
struct StatusNotification
{
	char     id[MAX_ID];  ///��Դ���룬������VAGID����PUID
	uint32_t videoId;     ///ͨ��ID
	time_t   time;        ///״̬����ʱ��
	int      status;      ///״̬
};



//�������Ͷ���
enum VagErrorCode
{
	VAG_ERR_FAIL              = -1,  //ʧ��
	VAG_ERR_OK                = 0,   //�ɹ�
	VAG_ERR_NO_INIT           = 1,   //δ��ʼ��
	VAG_ERR_AUTH_FAIL         = 2,   //��Ȩʧ��
	VAG_ERR_OUT_OF_SERVER     = 3,   //�����ṩ����
	VAG_ERR_CONNT_AGU_FAIL    = 4,   //���ӽ������ط�����ʧ��
	VAG_ERR_SOCKET_ERR        = 5,   //SOCKET����
	VAG_ERR_RECV_TIMEOUT      = 6,   //������ճ�ʱ
	VAG_ERR_MEM_ALLOC_FAIL    = 7,   //�����ڴ����
	VAG_ERR_REPORT_RES_FAIL   = 8,   //�ϱ���Դʧ��
	VAG_ERR_INVALID_RES       = 9,   //��Ч����Դ
	VAG_ERR_SEND_RTP_FAIL     = 10,  //������ý��ʧ��
	VAG_ERR_INVAID_CHANNEL    = 11,  //�����ͨ����
	VAG_ERR_RES_NO_EXIST      = 12,  //�������Դ������
	VAG_ERR_FILE_NO_EXIST     = 13,  //�ļ�������
	VAG_ERR_RES_NO_ONLINE     = 14,  //�������Դ������
	VAG_ERR_R_OVER_MAXLINK    = 15,  //��Դ�������������
	VAG_ERR_ISFINDING         = 16,  //¼���ļ����ڲ�ѯ��

	VAG_ERR_VERSION_NO_MATCH  = 98,  //�汾��ƥ��
	VAG_ERR_OTHER_FAIL        = 99   //����ʧ��ԭ��
};





///�ϱ���Դ��Ϣ
struct ResNode
{
	char     puid[MAX_ID];        /// ͳһ��Դ����
	uint32_t videoId;             /// ͨ��ID
	char     name[MAX_NAME];      /// ��Դ����
	int      codec;               /// �����ǩ
	int      status;              /// ״̬��0���ã�1������
	int      hasPTZ;              /// �Ƿ�֧��PTZ
    char     desc[MAX_NAME];      /// ��Դ����
};





///¼������
enum RecordType
{
	RECORD_SYSTEM   = 0x01, /// ƽ̨¼��
	RECORD_FRONTEND = 0x02, /// ǰ��¼��
	RECORD_MANUAL   = 0x04, /// �ֶ�¼��
	RECORD_ALARM    = 0x08  /// �澯¼��
};


///¼����Ϣ
struct RecordItem
{
	char     id[MAX_ID];         ///��Դ����
	uint32_t videoId;            ///ͨ��ID
	time_t   beginTime;          ///ʵ�ʿ�ʼʱ��
	time_t   endTime;            ///ʵ�ʽ���ʱ��
	int      type;               ///¼������
	char     recordID[MAX_NAME]; ///��Ƭ��¼����ϢΨһ��ǩ���루����������Ƭ�Σ�
};


///��ѯ¼����Ϣ����
struct QueryRecordRequest
{
	char     id[MAX_ID];    ///��Դ����
	uint32_t videoId;       ///ͨ��ID
	time_t   beginTime;     ///��ʼʱ��
	time_t   endTime;       ///����ʱ��
	int      type;          ///¼������
};


///��ѯ¼����Ϣ��Ӧ
struct QueryRecordResponse
{  
    int  sum;               ///�������Ʒ��ص�����Ŀ����pItems���ڴ泤�ȣ����ʵ�ʷ��ص�¼����Ϣ����
    RecordItem* pItems;     ///¼����Ϣ�ṹ
};


//VAGע����Ϣ
struct VAGRegInfo
{
	char  VagId[MAX_ID];              //VAG�ı���ID��
	char  VagUserName[MAX_NAME];      //VAG���û���
	char  VagPassword[MAX_NAME];      //VAG������
	char  VagIp[IPV4_LEN];            //VAG��IP��ַ
	char  CmuId[MAX_ID];              //VAGҪע���CMU���ķ������ı���
	char  CmugIp[IPV4_LEN];           //VAGҪע���CMU���ķ�����IP��ַ
	//uint16_t CmuPort;                 //VAGҪע���CMU���ķ������Ķ˿�	
    int CmuPort;
};


///ý���ǩ����Ӧ�����
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


///ý����Ϣ����
struct StreamFormat
{
	double duration;			 ///ý����ʱ�����룩����ʷ��Ƶ��Ч
	int codec;                   ///ý���ǩ����Ӧ�����
	int width;                   ///��Ƶ���
	int height;                  ///��Ƶ�߶�
	int fps;                     ///֡��
	int bitrate;                 ///����
	int iFrame;                  ///�ؼ�֡���
	int propSize;                ///ý��������Ϣ���ȣ���propʵ�ʳ���
	uint8_t prop[MAX_SPROP];     ///ý��������Ϣ�����纣��40�ֽڽ���ͷ����
	char mediaName[MAX_ID];      ///ý������

	int audioCodec;              ///��Ƶ����
	int audioChannel;            ///��Ƶͨ��
	int audioVAGmpleRate;         ///������VAGmple rate
	int audioPropSize;           ///��Ƶ������Ϣ���ȣ���audioPropʵ�ʳ���
	uint8_t audioProp[MAX_DESC]; ///��Ƶ������Ϣ
};


///ý������֡����
enum FrameType
{
	FRAME_G711U    = 0,   
	FRAME_G711A    = 0x08,
	FRAME_AAC_ADTS = 0x20,

	FRAME_VIDEO_I  = 0x10,
	FRAME_VIDEO_P  = 0x11,
	FRAME_VIDEO_B  = 0x12	
};



///ý�����ݷ��ͽṹ
struct StreamPacket
{
	uint8_t* data;      ///ý������
	int      size;      ///ý�����ݳ���
	int      flag;      ///ý��֡����
	uint64_t timestamp; ///����֡������ʱ�䣬milliseconds    
};

///Ԥ��λ����
enum PresetFlag
{
	PRESET_FLAG_NORM  = 0,   ///��ͨԤ��λ
	PRESET_FLAG_HOME  = 1    ///����λ,����λ
};


///Ԥ��λ��Ϣ
struct Preset
{
	int  num;            ///Ԥ��λ���
	char name[MAX_NAME]; ///Ԥ��λ����
	int  flag;           ///Ԥ��λ����
};



///�ƾ����ƶ���
enum PTZCommand
{
	PTZ_CMD_START   = 0,   ///������ʼ  
	PTZ_CMD_STOP    = 1,   ///����ֹͣ

	PTZ_CMD_RIGHT   = 2,   ///��	
	PTZ_CMD_LEFT    = 4,   ///��	
	PTZ_CMD_DOWN    = 6,   ///��	
	PTZ_CMD_UP      = 8,   ///��	

	PTZ_CMD_SMALL   = 10,    ///��Ȧ��С
	PTZ_CMD_LARGE   = 12,    ///��Ȧ���

	PTZ_CMD_ZOOM_IN  = 14,   ///������
	PTZ_CMD_ZOOM_OUT = 16,   ///�����С

	PTZ_CMD_FOCUS_IN  = 18,  ///����ǰ��	
	PTZ_CMD_FOCUS_OUT = 20,  ///������

	PTZ_CMD_ASSIST_FUNC_1 = 30,	 ///��ˢ
	PTZ_CMD_ASSIST_FUNC_2 = 32,	 ///��������2
	PTZ_CMD_ASSIST_FUNC_3 = 34,	 ///��������3
	PTZ_CMD_ASSIST_FUNC_4 = 36,	 ///��������4

	PTZ_RIGHT_APPLY   = 40,    ///Ȩ������
	PTZ_RIGHT_RELEASE = 42,    ///Ȩ���ͷ�

	PTZ_CMD_SCAN = 120,        ///�Զ�ɨ�迪     
	PTZ_CMD_LENGTH
};


///ý�����ݴ���
class  StreamSink
{
public:
	virtual ~StreamSink() {}

	 //�ڵ�һ��д����ǰ��Ҫ����SDK��Ƶ���ݵĸ�ʽ������Ҫ����һ�Σ����Ա��������    
	virtual void setFormat(const StreamFormat& fmt) =0;

	 //������ý����ʱ�����øú�������ý������д��SDK����SDK������   
	virtual int writePacket(const StreamPacket& packet) =0;

	///֪ͨ����ý�����ݽ���
	virtual void endOfStream() =0;
};


///ý����ƣ�ʵʱ��Ƶֻ��ʵ�ֲ��֣���ʷ��Ƶ����ʵ��
class  MediaPlayer
{
public:
	///ý��״̬
	enum State
	{
		STATE_OPEN = 0,
		STATE_STOP,
		STATE_PAUSE,
		STATE_PLAY
	};

	virtual ~MediaPlayer() {}

	///����ý���ʽ
	virtual int getFormat(StreamFormat& fmt) =0;

	///����
	virtual int play() =0;

	///��ͣ
	virtual int pause() =0;

	///ֹͣ
	virtual int stop() =0;

	///��ק,�����ʵ�ʿ�ʼʱ�������
	virtual int setSeek(int offset) =0;

	///���٣�1/4.0 ---- 4.0
	virtual int setScale(double scale) =0;

    virtual int step() =0;

	///��ȡ����λ��
	virtual int64_t getPos() =0;

	///��ȡý��״̬
	virtual int getState() =0;

	///��������ȡ�ص�
	virtual int setSink(StreamSink* pSink) =0;
};


///�ص���
class  NotificationSink
{
public:
	virtual ~NotificationSink() {}

	/**
     * VAGע��ɹ��ص�
     */
	virtual int OnVagRegisterOk() =0;

	/**
     * ������Դ�ϱ�
     */
	virtual int OnResReport() =0;

	/**
     * ��ʵʱ��Ƶ
     */
    virtual int OnOpenLiveStream(const char* resId, 
		uint32_t videoId, int streamIdx,
		MediaPlayer** ppPlayer) =0;
   
    /**
     * �ر���Ƶ��
     */
    virtual int OnCloseStream(MediaPlayer* pPlayer) =0;



    /**
     * �����ƾ�,
	 * @param cmd [in] ����
	 * @param data [in] ��������ʼ��ֹͣ��
     */
    virtual int OnControlPtz(const char* id, 
		uint32_t videoId, int cmd, int speed, int data) =0;

   	/**
	* ����Ԥ��λ
	*/
	virtual int OnSetPreset(const char* id, uint32_t videoId, 
		const Preset& preset) =0;

	/**
	* ɾ��Ԥ��λ
	*/
	virtual int OnDelPreset(const char* id, uint32_t videoId, int num) =0;

	/**
	* ����Ԥ��λ
	*/
	virtual int OnGotoPreset(const char* id, uint32_t videoId, int num) =0;


    /**
     * ��ѯ��ʷ��Ƶ
     */
    virtual int OnQueryRecord(const QueryRecordRequest& req,
		QueryRecordResponse& resp) =0;

    /**
     * ����ʷ��Ƶ��
     */
    virtual int OnOpenRecordStream(const RecordItem& rec, 
		MediaPlayer** ppStream) =0;


   /**
	* ����ؼ�֡
	*/
    virtual int OnMakeKeyFrame (const char *resId,
		uint32_t videoId, int streamIdx) =0;
};


///��Դ���֪ͨ����
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

//��Ƶ����ӿ� 
class VAGProxy
{
public:
	
	 /**
     * ע��
	 * @param regInfo [in] ע����Ϣ
     */
    virtual int Login(const VAGRegInfo &regInfo);  
    virtual int Logout();

   

	 /**
     * ״̬֪ͨ,�澯֪ͨ
	 * @param status [in] ��Ϣ
     */
	virtual int NotifyStatus(const StatusNotification* status,size_t length);
	virtual int NotifyAlarm(const AlarmNotification* alarm,size_t length);
	 /**
     * ���ûص�
	 * @param pSink [in] �ص���ָ��
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
 * ��ȡVAGProxy�ӿڣ���Ҫ����VAG_destroy�ͷ�
 * @return VAGProxy�ӿ�
 */
DLLEXPORT vag::VAGProxy* WINAPI VAG_create();


/**
 * �ͷ�VAGProxy�ӿ�
 * @param pVAG ��ָ����VAG_create����
 *
 */
DLLEXPORT void WINAPI VAG_destroy(vag::VAGProxy* pVAG);


/**
 * ��ȡ�汾
 */
DLLEXPORT const char* WINAPI VAG_getVersion();


/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

/////////////////////////////////////////////////////////////////////////////


#endif


