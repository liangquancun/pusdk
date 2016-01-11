#ifndef CMU_PRO_H_
#define CMU_PRO_H_

#include "TSocket.h"
#include "TEvent.h"
#include "TThread.h"
#include "TCriticalSection.h"
#include <string>
#include "Vagsdk.h"
#include "XmlUtil.h"
#include <vector>
#include <functional>
#include "MediaManager.h"
#include "TResQueue.h"

#include "ms_pu_param.h"

class CMvComm;

class MduClient : public comn::Thread
{
public:
	MduClient();
	virtual ~MduClient();

	bool startMduClient(const vag::VAGRegInfo &regInfo,const MvRegInfoEx &regEx);
	bool stopMduClient();

	virtual int run();
	virtual void doStop();

	//mdu register
	bool loginMdu();
	bool logoutMdu();
	bool hasLoginMdu();	

	void setNotificationSink(CMvComm* pSink);

private:	
	bool sendHeartToMdu();	
	bool sendRspDataToMdu(const std::string &xml,unsigned seq = 0);
	
private:	
	TSocket m_mduHandle;
	vag::VAGRegInfo m_regInfo;
	unsigned m_cseq;
	MvRegInfoEx m_regEx;

	comn::Event m_eventWaiter;

	CMvComm *m_pMvComm;
	unsigned m_keepAliveCount;
};


class CmuClient : public comn::Thread
{
public:
	CmuClient();	
	virtual ~CmuClient();

	bool startCmuClient(const vag::VAGRegInfo &regEx);
	bool stopCmuClient();

	virtual int run();
	virtual void doStop();

	//cmu register
	bool loginCmu();
	bool logoutCmu();
	bool hasLoginCmu();

	bool sendDataToCmu(const std::string &xml,unsigned seq = 0);

	void setNotificationSink(CMvComm* pSink);

	bool sendAlarmToCmu(const vag::AlarmNotification* alarm,size_t length);
private:	
	bool sendHeartToCmu();

	template < class T >
	bool sendStructToCmu(const T& t)
	{
		try
		{
			std::string xml = structToXml(t);
			if (!xml.empty())
			{
				return sendDataToCmu(xml);
			}
		}
		catch (...)
		{				
		}
		return false;
	}

private:
	TSocket m_cmuHandle;
	vag::VAGRegInfo m_regInfo;
	unsigned m_cseq;
	MvRegInfoEx m_regEx;

	comn::Event m_eventWaiter;
	CMvComm *m_pMvComm;
};



///STL算法find_if查找资源,根据puid和channelid
class EqVagNode : public std::unary_function<vag::ResNode,bool>
{
public:
	explicit EqVagNode(const std::string &puId,int channelId) 
		:m_puId(puId),
		m_channelId(channelId)
	{
	} 

	bool operator()(const vag::ResNode &lst)
	{            
		return    (m_puId.compare(lst.puid) == 0)
			   && (lst.videoId == m_channelId);
	}
private:
	std::string m_puId;
	int m_channelId;
};

class CMvComm : public comn::Thread 
{
public:
	typedef int (*msg_proc)(
		char *cmd, 
		char *buf, 
		unsigned long *size, 
		unsigned long param );

	typedef int (*stream_proc)( 
		unsigned long id, 
		char *cmd, 
		char *buf, 
		unsigned long size, 
		unsigned long param );

	CMvComm();	
	virtual ~CMvComm();

	bool startMvClient(const vag::VAGRegInfo &regInfo);
	bool stopMvClient();

	int SetNotificationSink(msg_proc msg_cb, stream_proc stream_cb);
	int SetVoiceIntercomCallBack(net_voice_intercom_proc voice_callback);

	int NotifyStatus(const vag::StatusNotification* status,size_t length);
	int NotifyAlarm(const vag::AlarmNotification* alarm,size_t length);

	std::string handleCmuMsg(const std::string &msg,unsigned seq = 0);
	std::string handleMduMsg(const std::string &msg,unsigned seq = 0);

	virtual int run();
	virtual void doStop();

	bool stopMedia(int sessionId);

	bool findNetMedia(unsigned long id);
	bool stopNetMedia(const StopMediaReq &req);

private:
	std::string getMsgCmd(const std::string &xml);

	std::string GetPuSoftwareVersion(const std::string &xml);
	std::string GetPuIpInfo(const std::string &xml);
	std::string SetPuIpInfo(const std::string &xml);

	std::string SetPuHideDetection(const std::string &xml);
	std::string SetPuVideoFormat(const std::string &xml);
	std::string SetPuImageDisplayPara(const std::string &xml);
	std::string SetPuSerialPort(const std::string &xml);
	std::string SetPuImageEncodePara(const std::string &xml);
	std::string SetPuImageTextPara(const std::string &xml);

	std::string GetPuSerialPort(const std::string &xml);

	std::string ControlPU(const std::string &xml);
	//std::string GuardCmd(const std::string &xml);
	std::string DeviceStatus(const std::string &xml);
    std::string ControlArming(const std::string &xml);
    std::string QueryPuStorageFiles(const std::string &xml);
	std::string SetPuTime(const std::string &xml);
	std::string GetPresetPTZ(const std::string &xml);
	std::string SetPresetPTZ(const std::string &xml);
	std::string DelPresetPTZ(const std::string &xml);

	std::string handleControlPTZ(const std::string &xml);
	std::string handlePuRegister(const std::string &xml);

	std::string buildCommonRsp(const std::string &cmd,int code);
	std::string buildGetPresetPtzRsp(const std::string &cmd, 
						PtzPresets& presets, int code, int rett);

	std::string buildGetPuSoftwareVersionRsp(const std::string &cmd,
		NET_CMU_SOFT_VERSION& version, int code);
	std::string buildGetPuIpInfoRsp(const std::string & cmd,
				CMU_IP_INFO& ipInfo, int code);
    std::string buildGetPuStorageFilesRsp(
	const std::string &cmd, CMU_QUERY_STORE_FILE& storagefiles, int code);

	std::string buildGetPuSerialPortRsp(const std::string &cmd,
		CMU_SERIAL_PORT& serialPort, int code);

	std::string handleStartMedia(const std::string &xml);
	std::string handleStopMedia(const std::string &xml);
    float transformPlaybackRate(int rate);
    std::string handleControlFileBack(const std::string &xml);
	void flushVagResLst();  
	
public:
	msg_proc	m_msg_proc;
	stream_proc m_stream_proc;
private:

	MduClient m_mduClient;
	CmuClient m_cmuClient;

	vag::VAGRegInfo m_regInfo;
	MvRegInfoEx m_regEx;

	comn::CriticalSection m_csRes;
	VagResNodeArray m_vagResLst;

	comn::CQueue<int>  m_taskQueue;

	//NetMediaManager m_mediaManager;

	unsigned m_mduSeq;
	unsigned m_cmuSeq;

	comn::Event m_eventWaiter;
};
 

#endif




