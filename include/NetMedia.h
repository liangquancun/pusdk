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

	///����
	virtual int play();

	///��ͣ
	virtual int pause() ;

	///ֹͣ
	virtual int stopMedia();

	///��ק,�����ʵ�ʿ�ʼʱ�������
	virtual int setSeek(int offset);

	///���٣�1/4.0 ---- 4.0
	virtual int setScale(double scale) ;

    //����
    virtual int step();

	StartMediaReq &getStartMediaReqMsg();

	int getSessionId();

	//������ý����ʱ�����øú�������ý������д��SDK����SDK������   
	virtual int writePacket(const vag::StreamPacket& packet) ;
	int readPacket(vag::StreamPacket& packet);

    void SetVoiceIntercomCallBack(net_voice_intercom_proc voice_callback);
    char m_sessionId[128];
    bool m_exitThread;
	BufferPtrQueue m_queue;
	bool sendCurrFrame();
    comn::CriticalSection m_lock;
    
	///֪ͨ����ý�����ݽ���
	virtual void endOfStream() ;
protected:
	virtual int run() ;
	virtual void doStop();
	//�ڵ�һ��д����ǰ��Ҫ����SDK��Ƶ���ݵĸ�ʽ������Ҫ����һ�Σ����Ա��������    
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


typedef shared_ptr<NetMedia>  NetMediaPtr;  ///����ָ��

#endif


