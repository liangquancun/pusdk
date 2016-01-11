#ifndef NET_MEDIAPLAYER_H
#define NET_MEDIAPLAYER_H

#include "TSocket.h"
#include "Vagsdk.h"
#include "XmlUtil.h"
#include "BufferFrame.h"
#include "SharedPtr.h"
#include "TThread.h"
#include "TCriticalSection.h"

class CMvComm;


void * streamProc(void * arg);

	
class NetMedia : public vag::StreamSink,public comn::Thread
{
public:
	explicit NetMedia(const StartMediaReq &req,
		CMvComm *pMvcomm);
    virtual ~NetMedia();	

	///播放
	virtual int play();

	///暂停
	virtual int pause() ;

	///停止
	virtual int stopMedia();

	///拖拽,相对于实际开始时间的秒数
	virtual int setSeek(int offset);

	///倍速，1/4.0 ---- 4.0
	virtual int setScale(double scale) ;

    //单步
    virtual int step();

	StartMediaReq &getStartMediaReqMsg();

	int getSessionId();

	//在请求媒体流时，调用该函数将流媒体数据写入SDK，由SDK负责发送   
	virtual int writePacket(const vag::StreamPacket& packet) ;
	int readPacket(vag::StreamPacket& packet);

    void SetVoiceIntercomCallBack(net_voice_intercom_proc voice_callback);
    char m_sessionId[128];
    bool m_exitThread;
	BufferPtrQueue m_queue;
	bool sendCurrFrame();
    comn::CriticalSection m_lock;
    
	///通知发送媒体数据结束
	virtual void endOfStream() ;
protected:
	virtual int run() ;
	virtual void doStop();
	//在第一次写数据前需要告诉SDK视频数据的格式（仅需要设置一次），以便解码库解码    
	virtual void setFormat(const vag::StreamFormat& fmt);

private:
	bool sendInfoPacket();

	int rawWrite(const char* buffer, int len);
	int send(const char* buffer, int length);

	int  sendFrame(BufferFramePtr pFrame);
	int cleanQueuedFrames();
protected:  

	net_voice_intercom_proc m_voice_callback;
	
	bool m_isAudioTask;
	TSocket m_steamHandle;
	StartMediaReq m_req;
	vag::StreamFormat m_fmt;
	CMvComm *m_pMvcomm;
	unsigned m_tryCount;

	int m_mediaType;
	int m_streamType;
	
	time_t m_lastPacketTime;

	XFrameAllocator m_allocator;
	BufferFramePtr m_sendingFrame;
	int m_length;
	bool m_hasError;
};


typedef shared_ptr<NetMedia>  NetMediaPtr;  ///智能指针

#endif


