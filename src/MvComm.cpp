#include "MvComm.h"
#include "tinyxml.h"
#include "TStringUtil.h"
#include "NetMedia.h"
#include "log.h"

#include "ms_pu_param.h"
#include "GlobalFunc.h"

MduClient::MduClient()
	:m_pMvComm(NULL),
	 m_cseq(1),
	 m_keepAliveCount(0)
{
}

MduClient::~MduClient()
{
}

int MduClient::run()
{
	time_t startTimer = time(NULL);

	while (!m_canExit)
	{
		if (!hasLoginMdu())
		{
			if (!loginMdu())
			{				
				m_eventWaiter.timedwait(1000 * 10);
				continue;
			}
		}

		time_t curTimer = time(NULL);
		if (curTimer <= startTimer)
		{
			startTimer = curTimer;
		}
		else if (curTimer - startTimer >= m_regEx.m_keepAlivePeriod-1)
		{
			if (m_keepAliveCount++ > 20)
			{
				MSLogEx(MS_LOG_LEVEL_INFO,
					"[%s:%d] mdu keepAlive timeout",
					__FUNCTION__, __LINE__);

				logoutMdu();
				m_keepAliveCount = 0;
				continue;
			}
			if (hasLoginMdu())
			{
				sendHeartToMdu();
			}

			startTimer = curTimer;
		}

		if (!m_mduHandle.checkReadable(1000))
		{				
			continue;
		}

		m_keepAliveCount = 0;

		char buffer[1024 * 30] = {0};
		TSockAddr addr;
		int ret = m_mduHandle.receiveFrom(buffer,sizeof(buffer)-1,0,addr);
		if (ret > 0)
		{
			std::string mduMsg;
			mduMsg.assign(buffer,ret);
            //printf("mduMsg:%s\n",mduMsg.c_str());
			if (m_pMvComm != NULL)
			{
				std::string rsp(m_pMvComm->handleMduMsg(mduMsg));
				///需要回复的消息
				if (!rsp.empty())
				{
					sendRspDataToMdu(rsp);
				}
			}				
		}		
	}

	logoutMdu();

	return 0;
}


void MduClient::doStop()
{
	m_eventWaiter.post();

	//m_mduHandle.sendTo()
}

//mdu register
bool MduClient::loginMdu()
{
	if (hasLoginMdu())
	{
		return true;
	}	

	if (!m_mduHandle.open(SOCK_DGRAM))
	{
		return false;
	}


	std::string strData = comn::StringUtil::format("REGISTER sip:%s SIP/2.0/UDP\r\n"\
		"To: %s\r\n"\
		"From: %s\r\n"\
		"Max-Forwards: 70\r\n"\
		"Call-ID: %s\r\n"\
		"CSeq: %d REGISTER\r\n"\
		"Content-Type: text/xml\r\n"\
		"Content-Length: 0\r\n\r\n",
		m_regInfo.VagIp,//register	
		m_regInfo.VagId,//to
		m_regInfo.VagId,//from
		m_regInfo.VagId,	
		m_cseq++);

	TSockAddr mduAddr(m_regEx.m_mduIp,m_regEx.m_mduPort);
	m_mduHandle.sendTo(strData.c_str(),strData.length(),0,mduAddr);

	MSLogEx(MS_LOG_LEVEL_INFO,
		"[%s:%d] login mdu:%s",
		__FUNCTION__, __LINE__,m_regInfo.VagId);

	return true;
}

bool MduClient::logoutMdu()
{
	if (m_mduHandle.isOpen())
	{
		m_mduHandle.close();
		MSLogEx(MS_LOG_LEVEL_INFO,
			"[%s:%d] logout mdu:%s",
			__FUNCTION__, __LINE__,m_regInfo.VagId);
	}
	

	return true;
}

bool MduClient::hasLoginMdu()
{
	return m_mduHandle.isOpen();
}

bool MduClient::sendRspDataToMdu(const std::string &xml,unsigned seq)
{
	if (!hasLoginMdu())
	{
		return false;
	}	

	if (seq == 0)
	{
		seq = ++m_cseq;
	}	

	std::string strData = comn::StringUtil::format
		( "SIP/2.0 200OK\r\n"\
		"To: %s\r\n"\
		"From: %s\r\n"\
		"Max-Forwards: 70\r\n"\
		"Call-ID: %s\r\n"\
		"CSeq: %d OPTIONS\r\n"\
		"Content-Type: text/xml\r\n"\
		"Content-Length: %d\r\n\r\n%s",
		m_regInfo.VagId,//to
		m_regInfo.VagId,//from
		m_regInfo.VagId,	
		seq,
		xml.length(),
		xml.c_str());

	TSockAddr mduAddr(m_regEx.m_mduIp,m_regEx.m_mduPort);
	m_mduHandle.sendTo(strData.c_str(),strData.length(),0,mduAddr);

	return true;
}

bool MduClient::startMduClient(const vag::VAGRegInfo &regInfo,
							   const MvRegInfoEx &regEx)
{	

	m_regEx = regEx;
	m_regInfo = regInfo;

	if (!isRunning())
	{
		start();

		MSLogEx(MS_LOG_LEVEL_INFO,
			"[%s:%d] startMduClient ok.",
			__FUNCTION__, __LINE__);
	}
	
	return true;
}

bool MduClient::stopMduClient()
{	
	stop();

	MSLogEx(MS_LOG_LEVEL_INFO,
		"[%s:%d] stopMduClient",
		__FUNCTION__, __LINE__);

	return true;
}

bool MduClient::sendHeartToMdu()
{
	if (!hasLoginMdu())
	{
		return false;
	}	

	std::string strData = comn::StringUtil::format
	  ( "OPTIONS sip:%s SIP/2.0/UDP\r\n"\
		"To: %s\r\n"\
		"From: %s\r\n"\
		"Max-Forwards: 70\r\n"\
		"Call-ID: %s\r\n"\
		"CSeq: %d OPTIONS\r\n"\
		"Content-Type: text/xml\r\n"\
		"Content-Length: 0\r\n\r\n",
		m_regInfo.VagIp,//register	
		m_regInfo.VagId,//to
		m_regInfo.VagId,//from
		m_regInfo.VagId,	
		m_cseq++);
	

	TSockAddr mduAddr(m_regEx.m_mduIp,m_regEx.m_mduPort);
	m_mduHandle.sendTo(strData.c_str(),strData.length(),0,mduAddr);

	//MSLogEx(MS_LOG_LEVEL_INFO,
		//"[%s:%d] sendHeartToMdu",
		//__FUNCTION__, __LINE__);

	return true;
}

CmuClient::CmuClient()
	:m_pMvComm(NULL),
	 m_cseq(1)
{
	m_regEx.m_keepAlivePeriod = 20;
}

CmuClient::~CmuClient()
{
}

bool CmuClient::sendDataToCmu(const std::string &xml,unsigned seq)
{
	if (!hasLoginCmu())
	{
		return false;
	}
	MvMsgHeader rcPacket;
	rcPacket.ver = htons(0x0100);
	if (seq == 0)
	{
		rcPacket.cseq = htonl(m_cseq++);
	}
	else
	{
		rcPacket.cseq = htonl(seq);
	}
	
	rcPacket.length = htons(xml.length());

	m_cmuHandle.send((char *)&rcPacket,sizeof(rcPacket));
	int ret = m_cmuHandle.send(xml.c_str(),xml.length());
	return ret > 0;
}

int CmuClient::run()
{
	time_t startTimer = time(NULL);

	while (!m_canExit)
	{
		///还没有注册
		if (!hasLoginCmu())
		{
			if (!loginCmu())
			{
				m_eventWaiter.timedwait(1000 * 10);

				MSLogEx(MS_LOG_LEVEL_INFO,
					"[%s:%d] loginCmu error,try doing",
					__FUNCTION__, __LINE__);

				continue;
			}
		}


		time_t curTimer = time(NULL);
		if (curTimer <= startTimer)
		{
			startTimer = curTimer;
		}
		else if (curTimer - startTimer >= m_regEx.m_keepAlivePeriod-1)
		{			
			///超时就心跳
			sendHeartToCmu();	
			startTimer = curTimer;
		}

		if (!m_cmuHandle.checkReadable(1000))
		{
			continue;
		}	
		
	
		///有数据先接收数据头
		MvMsgHeader hdr;
		memset(&hdr,0,sizeof(hdr));
		int retlen = m_cmuHandle.readnInTime((char *)&hdr,sizeof(hdr),10 * 1000);
		if (retlen != sizeof(hdr))
		{	
			MSLogEx(MS_LOG_LEVEL_INFO,
				"[%s:%d] receiveBlock MvMsgHeader error,retlen:%d,sizeof(hdr):%d",
				__FUNCTION__, __LINE__,retlen,sizeof(hdr));

			logoutCmu();
			m_eventWaiter.timedwait(1000 * 10);
			continue;
		}	

		///接收消息体
		char buffer[1024 * 30] = {0};
		
		///数据长度非法
		retlen = ntohs(hdr.length); //从头中得到指令体长度
		if( retlen > sizeof(buffer)-1 )
		{
			MSLogEx(MS_LOG_LEVEL_INFO,
				"[%s:%d] receiveBlock data len error retlen:%d,sizeof(buffer):%d,errno:%d,%s",
				__FUNCTION__, __LINE__,retlen,sizeof(buffer)-1,errno,strerror(errno));

			logoutCmu();
			m_eventWaiter.timedwait(1000 * 10);
			continue;
		}

		///数据长度为空
		if (retlen == 0)
		{
			continue;
		}
		
		int ret = m_cmuHandle.readnInTime(buffer,retlen,10 * 1000);
		if (ret != retlen)
		{
			MSLogEx(MS_LOG_LEVEL_INFO,
				"[%s:%d] receiveBlock data msg error",
				__FUNCTION__, __LINE__);

			logoutCmu();
			m_eventWaiter.timedwait(1000 * 10);
			continue;
		}
		
		///处理消息
		if (m_pMvComm != NULL)
		{
            //printf("buffer:%s ret:%d\n",buffer,ret);
			std::string rsp(m_pMvComm->handleCmuMsg(buffer,ntohl(hdr.cseq)));
			///需要回复的消息
			if (!rsp.empty())
			{
				sendDataToCmu(rsp,ntohl(hdr.cseq));
			}
		}	
	}

	logoutCmu();

	return 0;
}

void CmuClient::doStop()
{
	
}

//cmu register
bool CmuClient::loginCmu()
{
	if (hasLoginCmu())
	{
        MSLogEx(MS_LOG_LEVEL_WARNING,"[%s:%d] hasLoginCmu",__FUNCTION__,__LINE__);

		return true;
	}
    
	if (!m_cmuHandle.open())
	{
       MSLogEx(MS_LOG_LEVEL_WARNING,"[%s:%d] open fail",__FUNCTION__,__LINE__);
		return false;
	}

	m_cmuHandle.setKeepAlive(true,10,10,3);

	if (m_cmuHandle.connect(m_regInfo.CmugIp,m_regInfo.CmuPort) != 0)
	{
		m_cmuHandle.close();
        MSLogEx(MS_LOG_LEVEL_WARNING,"[%s:%d] connect fail",__FUNCTION__,__LINE__);

		return false;
	}


	std::string regMsg = structToXml(m_regInfo);
	if (regMsg.empty())
	{
		m_cmuHandle.close();
        MSLogEx(MS_LOG_LEVEL_WARNING,"[%s:%d] regMsg.empty fail",__FUNCTION__,__LINE__);
		return false;
	}

	bool ok = sendDataToCmu(regMsg);
	if (!ok)
	{
        MSLogEx(MS_LOG_LEVEL_WARNING,"[%s:%d] sendDataToCmu fail",__FUNCTION__,__LINE__);
		m_cmuHandle.close();
	}
	
	return ok;
}

bool CmuClient::logoutCmu()
{
	if (m_cmuHandle.isOpen())
	{
		m_cmuHandle.close();

		MSLogEx(MS_LOG_LEVEL_INFO,
			"[%s:%d] logoutCmu",
			__FUNCTION__, __LINE__);

	}	
	return true;
}

bool CmuClient::hasLoginCmu()
{
	return m_cmuHandle.isOpen();
}

bool CmuClient::sendAlarmToCmu(const vag::AlarmNotification* alarm,size_t length)
{
	if (!hasLoginCmu())
	{
		return false;
	}

	std::string alarmMsg = structToXml(alarm);
	if (alarmMsg.empty())
	{
		m_cmuHandle.close();
		return false;
	}

	MSLogEx(MS_LOG_LEVEL_INFO,
		"[%s:%d] sendAlarmToCmu",
		__FUNCTION__, __LINE__);


	return sendDataToCmu(alarmMsg);
}

bool CmuClient::sendHeartToCmu()
{
	if (!hasLoginCmu())
	{
		return false;
	}


	CmuKeepAlive cKeepAlive;
	cKeepAlive.m_puId.assign(m_regInfo.VagId);
	cKeepAlive.m_cmuKeepAlivePeriod = m_regEx.m_keepAlivePeriod;

	std::string regMsg = structToXml(cKeepAlive);
	if (regMsg.empty())
	{
		m_cmuHandle.close();
		return false;
	}

	//MSLogEx(MS_LOG_LEVEL_INFO,
		//"[%s:%d] sendHeartToCmu",
		//__FUNCTION__, __LINE__);


	return sendDataToCmu(regMsg);
}


void CmuClient::setNotificationSink(CMvComm* pSink)
{
	m_pMvComm = pSink;
}

void MduClient::setNotificationSink(CMvComm* pSink)
{
	m_pMvComm = pSink;
}


bool CmuClient::startCmuClient(const vag::VAGRegInfo &regEx)
{
	m_regInfo = regEx;

	bool ok = loginCmu();
	if (!ok)
	{
       	MSLogEx(MS_LOG_LEVEL_INFO,
		"[%s:%d] login cmu fail",
		__FUNCTION__, __LINE__);

		return false;
	}

	if (!isRunning())
	{
		start();
	}

	MSLogEx(MS_LOG_LEVEL_INFO,
		"[%s:%d] startCmuClient ok.",
		__FUNCTION__, __LINE__);

	return true;
}

bool CmuClient::stopCmuClient()
{
	stop();

	MSLogEx(MS_LOG_LEVEL_INFO,
		"[%s:%d] stopCmuClient",
		__FUNCTION__, __LINE__);

	return true;
}

CMvComm::CMvComm()
	: m_msg_proc(NULL),
	m_stream_proc(NULL),
	 m_mduSeq(0),
	 m_cmuSeq(0)
{
	m_regEx.m_mduPort = 0;
}

CMvComm::~CMvComm()
{
	stopMvClient();
}


bool CMvComm::startMvClient(const vag::VAGRegInfo &regInfo)
{	
	TSocketLib::init();
#if 0
	if (!isRunning())
	{
		start();
	}

#endif
	m_regInfo = regInfo;
	return m_cmuClient.startCmuClient(regInfo);
}

bool CMvComm::stopMvClient()
{	
	m_cmuClient.stopCmuClient();
	m_mduClient.stopMduClient();

	stop();

	TSocketLib::unInit();

	MSLogEx(MS_LOG_LEVEL_INFO,
		"[%s:%d] stopMvClient",
		__FILE__, __LINE__);

	return true;
}

int CMvComm::SetNotificationSink(msg_proc msg_cb, stream_proc stream_cb)
{	
	m_msg_proc = msg_cb;
	m_stream_proc = stream_cb;

	m_mduClient.setNotificationSink(this);
	m_cmuClient.setNotificationSink(this);
	return vag::VAG_ERR_OK;
}
//设置对讲回调函数
int CMvComm::SetVoiceIntercomCallBack(net_voice_intercom_proc voice_callback)
{
	getMediaManager().SetVoiceIntercomCallBack(voice_callback);
}

std::string CMvComm::getMsgCmd(const std::string &xml)
{
	TiXmlDocument doc ;		
	doc.Parse(xml.c_str());	
	if (doc.Error())
	{		
		return "";
	}
	TiXmlElement* node = doc.RootElement();
	if (node == NULL)
	{
		return "";
	}	
	const char *ptr = node->Attribute("command");//这会不会有隐患
	if (ptr == NULL)
	{
		return "";
	}
	return std::string(ptr);
}

std::string CMvComm::handleCmuMsg(const std::string &msg,unsigned seq )
{	
	m_cmuSeq = seq;

	std::string cmd(getMsgCmd(msg));

	//MSLogEx(MS_LOG_LEVEL_INFO,
		//"[%s:%d] recv cmu msg cmd:%s",
		//__FUNCTION__, __LINE__, cmd.c_str());

	
	if(cmd.compare("PuRegister") == 0)	
	{
		return handlePuRegister(msg);
	}		
	else if(cmd.compare("ControlPTZ") == 0)
	{
		return handleControlPTZ(msg);		
	}
	else if(cmd.compare("DelPresetPTZ") == 0)
	{
		return DelPresetPTZ(msg);
	}
	else if(cmd.compare("SetPresetPTZ") == 0)
	{
		return SetPresetPTZ(msg);
	}
	else if(cmd.compare("GetPresetPTZ") == 0)
	{
		return GetPresetPTZ(msg);
	}
	else if(cmd.compare("SetPuTime") == 0)
	{
		return SetPuTime(msg);			
	}
	else if(cmd.compare("GetPuSoftwareVersion") == 0)
	{
		return GetPuSoftwareVersion(msg);
	}
	else if(cmd.compare("GetPuIpInfo") == 0)// 设备网络参数
	{
		return GetPuIpInfo(msg);
	}
	else if(cmd.compare("SetPuIpInfo") == 0)// 设备网络参数
	{
		return SetPuIpInfo(msg);
	}
	else if(cmd.compare("SetPuHideDetection") == 0)// 设备视频遮蔽告警参数
	{
		return SetPuHideDetection(msg);
	}
	else if(cmd.compare("SetPuVideoFormat") == 0)   //设备图像制式参数
	{
		return SetPuVideoFormat(msg);
	}
	else if(cmd.compare("SetPuImageDisplayPara") == 0)// 设备图像显示参数
	{
		return SetPuImageDisplayPara(msg);
	}
	else if(cmd.compare("SetPuSerialPort") == 0)//串口参数
	{
		return SetPuSerialPort(msg);
	}
	else if(cmd.compare("GetPuSerialPort") == 0)//串口参数
	{
		return GetPuSerialPort(msg);
	}
	else if(cmd.compare("SetPuImageEncodePara") == 0)   //设备图像编码参数
	{
		return SetPuImageEncodePara(msg);
	}
	else if(cmd.compare("SetPuImageTextPara") == 0)    //时间文本设置
	{
		return SetPuImageTextPara(msg);
	}
	else if(cmd.compare("ControlPU") == 0)//控制，重启，关机
	{
		return ControlPU(msg);
	}
    /*
	else if(cmd.compare("GuardCmd") == 0)
	{
		return GuardCmd(msg);
	}
	*/
	else if(cmd.compare("DeviceStatus") == 0)//设备状态获取
	{
		return DeviceStatus(msg);
	}
    else if(cmd.compare("ControlArming") == 0)/*布撤防*/
    {
        return ControlArming(msg);
    }
    else if(cmd.compare("QueryPuLocalStorageFiles") == 0)/*查询前端历史录像*/
    {
        return QueryPuStorageFiles(msg);
    }
    else if (cmd.compare("ControlFileBack") == 0)/*前端历史录像控制命令*/
    {
        return handleControlFileBack(msg);
    }
	return "";
}

std::string CMvComm::handleMduMsg(const std::string &msg,unsigned seq)
{
	m_mduSeq = seq;

	char *pMsg = strstr((char *)msg.c_str(), "<?xml ");
	if (pMsg == NULL)
	{
		pMsg = strstr((char *)msg.c_str(), "OPTIONS");
		if (pMsg != NULL)
		{
			//MSLogEx(MS_LOG_LEVEL_INFO,
				//"[%s:%d] recv mdu msg cmd:OPTIONS",
				//__FUNCTION__, __LINE__);
		}
		return "";
	}
	

	std::string cmd(getMsgCmd(pMsg));

	MSLogEx(MS_LOG_LEVEL_INFO,
		"[%s:%d] recv mdu msg cmd:%s",
		__FUNCTION__, __LINE__, cmd.c_str());

	if (cmd.compare("Start_MEDIA") == 0)
	{
		return handleStartMedia(pMsg);		
	}
	else if(cmd.compare("Stop_MEDIA") == 0)
	{
		return handleStopMedia(pMsg);			
	}
    else if(cmd.compare("Start_FilePlayBack") == 0)
	{		
        return handleStartMedia(pMsg);	
	}
	else if(cmd.compare("ControlFileBack") == 0)
	{		
	}	
	return "";
}

std::string CMvComm::handleStartMedia(const std::string &xml)
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
    StartMediaReq startMedia;
    startMedia.m_iMediaType = 2;
    startMedia.m_videoId = startMedia.m_iStreamType \
    = startMedia.m_iLinkMode \
    =startMedia.m_targetPort \
    = startMedia.m_playType = 0;
	bool ok = structFromXml(startMedia,xml);
	if (!ok || !m_stream_proc)
	{
		return "";
	}

	NetMediaPtr pNetMedia(new NetMedia(startMedia,this));
	if (!pNetMedia)
	{
		return "";
	}

	MSLogEx(MS_LOG_LEVEL_INFO,
		"[%s:%d] CMvComm::handleStartMedia, --->>> puId:%s,videoId:%d,streamType:%d",
		__FUNCTION__, __LINE__, 
		startMedia.m_puId.c_str(),
		startMedia.m_videoId,
		startMedia.m_iStreamType);
	getMediaManager().startNetMedia(pNetMedia);			
	
	return "";
}

std::string CMvComm::handleStopMedia(const std::string &xml)
{
	StopMediaReq stopMedia;
    stopMedia.m_iMediaType = 2;
    stopMedia.m_iStreamType = 0;
	bool ok = structFromXml(stopMedia,xml);
	if (!ok || !m_stream_proc)
	{
		return "";
	}

	MSLogEx(MS_LOG_LEVEL_INFO,
		"[%s:%d]CMvComm::handleStopMedia, <<<--- puId:%s,videoId:%d,streamType:%d",
		__FUNCTION__, __LINE__, 
		stopMedia.m_puId.c_str(),
		stopMedia.m_videoId,
		stopMedia.m_iStreamType);

	getMediaManager().stopNetMedia(stopMedia);

	return "";
}

float CMvComm::transformPlaybackRate(int rate)
{
	float playbackRate = 0.0;

	switch (rate)
	{
	case 0: 
		playbackRate = 0.0;
		break;
	case 1:
		playbackRate = 1.0;
		break;
	case -1:
		playbackRate = -1.0;
		break;
	case 2:
		playbackRate = 2.0;
		break;
	case -2:
		playbackRate = -2.0;
		break;
	case 3:
		playbackRate = 3.0;
		break;
	case -3:
		playbackRate = -3.0;
		break;
	default:
		playbackRate = 0.0;
	}
	return playbackRate;
}

std::string CMvComm::handleControlFileBack(const std::string &xml)
{
	PlayBackReq playback;

	bool ok = structFromXml(playback, xml);
	if (!ok )
	{
		return "";
	}
    char id[128];
    memset(id,0,sizeof(id));
    sprintf(id,"%d:%s",playback.m_videoId,playback.m_fileName.c_str());
	NetMediaPtr pMedia = getMediaManager().findNetMedia((unsigned long)id);
	if (!pMedia)
	{
		return "";
	}
    int ret = 0;
    if (playback.m_cmd.compare("PAUSE") == 0)//暂停
    {
        ret = pMedia->pause();
    }
    else if (playback.m_cmd.compare("RATE") == 0)//设置播放速度
    {
    	float rate = transformPlaybackRate(playback.m_param);

    	ret = pMedia->setScale(rate);
    }
    else if (playback.m_cmd.compare("SEEK") == 0)//拖拽
    {
        ret = pMedia->setSeek(playback.m_param);
    }
    else if (playback.m_cmd.compare("RESTART") == 0)//暂停恢复
    {
        ret = pMedia->play();
    }
    else if (playback.m_cmd.compare("STEP") == 0)//单帧
    {
        ret = pMedia->step();
    }
    return buildCommonRsp("ControlFileBack",ret);		

}

std::string CMvComm::handlePuRegister(const std::string &xml)
{
	bool ok = structFromXml(m_regEx,xml);
	if (ok && (m_regEx.m_mduPort > 0))
	{
		m_mduClient.startMduClient(m_regInfo,m_regEx);

		//MSLogEx(MS_LOG_LEVEL_INFO,
			//"[%s:%d] handlePuRegister ok, mdu ip:%s, mdu port:%d, keepalive:%d",
			//__FUNCTION__, __LINE__, 
			//m_regEx.m_mduIp.c_str(), 
			//m_regEx.m_mduPort, 
			///m_regEx.m_keepAlivePeriod);
	}
	else
	{
		m_eventWaiter.timedwait(1000 * 10);
	}
	return "";
}

std::string CMvComm::GetPuIpInfo(const std::string &xml)
{
	CMU_IP_INFO ipInfo;
	memset(&ipInfo, 0, sizeof(CMU_IP_INFO));

	if (m_msg_proc != NULL)
	{
		unsigned long size = sizeof(CMU_IP_INFO);
		m_msg_proc((char *)"GetPuIpInfo", (char*)&ipInfo, &size, g_param);
	}

	return buildGetPuIpInfoRsp("GetPuIpInfo", ipInfo, 0);
}

std::string CMvComm::GetPuSerialPort(const std::string &xml)
{
	CMU_SERIAL_PORT serialPort;
	memset(&serialPort, 0, sizeof(CMU_SERIAL_PORT));

	bool ok = structFromXml(serialPort, xml);
	if (ok && m_msg_proc != NULL)
	{
		unsigned long size = sizeof(CMU_SERIAL_PORT);
		m_msg_proc((char *)"GetPuSerialPort", (char*)&serialPort, &size, g_param);
	}

	return buildGetPuSerialPortRsp("GetPuSerialPort", serialPort, 0);
}

std::string CMvComm::GetPuSoftwareVersion(const std::string &xml)
{
	NET_CMU_SOFT_VERSION version;
	memset(&version, 0, sizeof(NET_CMU_SOFT_VERSION));

	if (m_msg_proc != NULL)
	{
		unsigned long size = sizeof(NET_CMU_SOFT_VERSION);
		m_msg_proc((char *)"GetPuSoftwareVersion", (char*)&version, &size, g_param);
	}

	return buildGetPuSoftwareVersionRsp("GetPuSoftwareVersion", version, 0);	
}


std::string CMvComm::SetPuHideDetection(const std::string &xml)
{
	CMU_HIDE_DETECT hideDetc;
	memset(&hideDetc, 0, sizeof(CMU_HIDE_DETECT));

	bool ok = structFromXml(hideDetc, xml);
	if (ok && m_msg_proc != NULL)
	{
		unsigned long size = sizeof(CMU_HIDE_DETECT);
		m_msg_proc((char *)"SetPuHideDetection", (char*)&hideDetc, &size, g_param);

		return buildCommonRsp("SetPuHideDetection",0);
	}

	return "";
}

std::string CMvComm::SetPuVideoFormat(const std::string &xml)
{
	CMU_VIDEO_FORMAT format;
	memset(&format, 0, sizeof(CMU_VIDEO_FORMAT));

	bool ok = structFromXml(format, xml);
	if (ok && m_msg_proc != NULL)
	{
		unsigned long size = sizeof(CMU_VIDEO_FORMAT);
		m_msg_proc((char *)"SetPuVideoFormat", (char*)&format, &size, g_param);

		return buildCommonRsp("SetPuVideoFormat",0);
	}

	return "";
}

std::string CMvComm::SetPuImageDisplayPara(const std::string &xml)
{
	CMU_IMG_DISPLAY  display;
	memset(&display, 0, sizeof(CMU_IMG_DISPLAY));

	bool ok = structFromXml(display, xml);
	if (ok && m_msg_proc != NULL)
	{
		unsigned long size = sizeof(CMU_IMG_DISPLAY);
		m_msg_proc((char *)"SetPuImageDisplayPara", (char*)&display, &size, g_param);

		return buildCommonRsp("SetPuImageDisplayPara",0);
	}

	return "";	
}

std::string CMvComm::SetPuSerialPort(const std::string &xml)
{
	CMU_SERIAL_PORT  serialPort;
	memset(&serialPort, 0, sizeof(CMU_SERIAL_PORT));

	bool ok = structFromXml(serialPort, xml);
	if (ok && m_msg_proc != NULL)
	{
		unsigned long size = sizeof(CMU_SERIAL_PORT);
		m_msg_proc((char *)"SetPuSerialPort", (char*)&serialPort, &size, g_param);

		return buildCommonRsp("SetPuSerialPort",0);
	}

	return "";
}

std::string CMvComm::SetPuImageEncodePara(const std::string &xml)
{	
	CMU_IMG_ENCODE  encParm;
	memset(&encParm, 0, sizeof(CMU_IMG_ENCODE));

	bool ok = structFromXml(encParm, xml);
	if (ok && m_msg_proc != NULL)
	{
		unsigned long size = sizeof(CMU_IMG_ENCODE);
		m_msg_proc((char *)"SetPuImageEncodePara", (char*)&encParm, &size, g_param);

		return buildCommonRsp("SetPuImageEncodePara",0);
	}

	return "";
}

std::string CMvComm::SetPuImageTextPara(const std::string &xml)
{
	CMU_IMG_TEXT textParm;
	memset(&textParm, 0, sizeof(CMU_IMG_TEXT));

	bool ok = structFromXml(textParm, xml);
	if (ok && m_msg_proc != NULL)
	{
		unsigned long size = sizeof(CMU_IMG_TEXT);
		m_msg_proc((char *)"SetPuImageTextPara", (char*)&textParm, &size, g_param);

		return buildCommonRsp("SetPuImageTextPara",0);
	}

	return "";
}

std::string CMvComm::SetPuIpInfo(const std::string &xml)
{
	CMU_IP_INFO ipInfo;
	memset(&ipInfo, 0, sizeof(CMU_IP_INFO));

	bool ok = structFromXml(ipInfo, xml);
	if (ok && m_msg_proc != NULL)
	{
		unsigned long size = sizeof(NET_CMU_IP_INFO);
		m_msg_proc((char *)"SetPuIpInfo", (char*)&ipInfo, &size, g_param);

		return buildCommonRsp("SetPuIpInfo",0);
	}

	return "";
}

std::string CMvComm::SetPuTime(const std::string &xml)
{
	CMU_DEV_TIME devTime;
	memset(&devTime, 0, sizeof(CMU_DEV_TIME));

	bool ok = structFromXml(devTime, xml);
	if (ok && m_msg_proc != NULL)
	{
		unsigned long size = sizeof(CMU_DEV_TIME);
		m_msg_proc((char *)"SetPuTime", (char*)&devTime, &size, g_param);

		return buildCommonRsp("SetPuTime",0);
	}

	return "";
}

std::string CMvComm::ControlPU(const std::string &xml)
{
	CMU_OPER_CTRL_DEV control;
	memset(&control, 0, sizeof(CMU_OPER_CTRL_DEV));

	bool ok = structFromXml(control, xml);
	if (ok && m_msg_proc != NULL)
	{
		unsigned long size = sizeof(CMU_OPER_CTRL_DEV);
		m_msg_proc((char *)"ControlPU", (char*)&control, &size, g_param);

		return buildCommonRsp("ControlPU",0);
	}
	return "";
}
/*
std::string CMvComm::GuardCmd(const std::string &xml)
{
	return "";
}
*/
std::string CMvComm::DeviceStatus(const std::string &xml)
{
	return "";
}

std::string CMvComm::ControlArming(const std::string &xml)
{
	CMU_OPER_CTRL_ALARM controlAlarm;
	memset(&controlAlarm, 0, sizeof(CMU_OPER_CTRL_ALARM));

	bool ok = structFromXml(controlAlarm, xml);
	if (ok && m_msg_proc != NULL)
	{
		unsigned long size = sizeof(CMU_OPER_CTRL_ALARM);
		m_msg_proc((char *)"ControlArming", (char*)&controlAlarm, &size, g_param);

		return buildCommonRsp("ControlArming",0);
	}
	return "";
}

std::string CMvComm::QueryPuStorageFiles(const std::string &xml)
{
	CMU_QUERY_STORE_FILE storageFiles;
	memset(&storageFiles, 0, sizeof(CMU_QUERY_STORE_FILE));

    bool ok = structFromXml(storageFiles,xml);
	if (ok && m_msg_proc != NULL)
	{
		unsigned long size = sizeof(CMU_QUERY_STORE_FILE);
        storageFiles.record_file_lists = new CMU_RECORD_FILE[100];
		int result = m_msg_proc((char *)"QueryPuLocalStorageFiles", (char*)&storageFiles, &size, g_param);
	    std::string xml = buildGetPuStorageFilesRsp("QueryPuLocalStorageFiles", storageFiles, result);
        delete [] storageFiles.record_file_lists;
        return xml;
    }
    return "";
}

std::string CMvComm::SetPresetPTZ(const std::string &xml)
{
	PtzPresets ptzPresets;
	memset(&ptzPresets, 0, sizeof(PtzPresets));

	ptzPresets.preset_lists = (CMU_PRESET *)malloc(sizeof(CMU_PRESET));

	bool ok = structFromXml(ptzPresets, xml);
	if (ok && m_msg_proc != NULL)
	{
		unsigned long size = sizeof(PtzPresets);
		m_msg_proc((char *)"SetPresetPTZ", (char*)&ptzPresets, &size, g_param);
		
		if(ptzPresets.preset_lists)
			free(ptzPresets.preset_lists);

		return buildCommonRsp("SetPresetPTZ",0);
	}
	if(ptzPresets.preset_lists)
		free(ptzPresets.preset_lists);

	return "";
}

std::string CMvComm::GetPresetPTZ(const std::string &xml)
{
	int rett = -1;
	PtzPresets ptzPresets;
	memset(&ptzPresets, 0, sizeof(PtzPresets));

	bool ok = structFromXml(ptzPresets, xml);
	if (ok && m_msg_proc != NULL)
	{
		unsigned long size = sizeof(PtzPresets);
		rett = m_msg_proc((char *)"GetPresetPTZ", (char*)&ptzPresets, &size, g_param);
	}

	std::string rsp = buildGetPresetPtzRsp("GetPresetPTZ",ptzPresets,0, rett);

	if(ptzPresets.preset_lists)
		free(ptzPresets.preset_lists);

	return rsp;
}

std::string CMvComm::DelPresetPTZ(const std::string &xml)
{
	DeletePtzPreset delPreset;
	memset(&delPreset, 0, sizeof(DeletePtzPreset));

	bool ok = structFromXml(delPreset, xml);
	if (ok && m_msg_proc != NULL)
	{
		unsigned long size = sizeof(DeletePtzPreset);
		m_msg_proc((char *)"DelPresetPTZ", (char*)&delPreset, &size, g_param);

		int ret = 0;
		return buildCommonRsp("DelPresetPTZ",ret);
	}
	return "";
}

std::string CMvComm::handleControlPTZ(const std::string &xml)
{
	ControlPtzMsg ptzMsg;
	int cmd = vag::PTZ_CMD_START;
	int data = vag::PTZ_CMD_START;
	bool ok = structFromXml(ptzMsg,xml);
	if (ok && m_msg_proc != NULL)
	{
		if(0 == ptzMsg.m_cmd.compare("ZIN"))//焦距拉近
		{
			cmd = vag::PTZ_CMD_ZOOM_IN;
		}
		else if(0 == ptzMsg.m_cmd.compare("ZOUT"))//焦距拉远
		{
			cmd = vag::PTZ_CMD_ZOOM_OUT;
		}
		else if(0 == ptzMsg.m_cmd.compare("FN"))//焦点调近
		{
			cmd = vag::PTZ_CMD_FOCUS_IN;
		}
		else if(0 == ptzMsg.m_cmd.compare("FR"))//焦点调远
		{
			cmd = vag::PTZ_CMD_FOCUS_OUT;
		}
		else if(0 == ptzMsg.m_cmd.compare("IO"))//光圈自动增大
		{
			cmd = vag::PTZ_CMD_LARGE;
		}
		else if(0 == ptzMsg.m_cmd.compare("IC"))//光圈自动减小
		{
			cmd = vag::PTZ_CMD_SMALL;
		}	
		else if(0 == ptzMsg.m_cmd.compare("TU"))//上
		{
			cmd = vag::PTZ_CMD_UP;
		}
		else if(0 == ptzMsg.m_cmd.compare("TD"))//下
		{
			cmd = vag::PTZ_CMD_DOWN;
		}
		else if(0 == ptzMsg.m_cmd.compare("PL"))//左
		{
			cmd = vag::PTZ_CMD_LEFT;
		}
		else if(0 == ptzMsg.m_cmd.compare("PR"))//右
		{
			cmd = vag::PTZ_CMD_RIGHT;
		}
		else if(0 == ptzMsg.m_cmd.compare("GOTO_PRESET"))//转到预置点
		{
			cmd = vag::PTZ_CMD_RIGHT;
		}
		else if(0 == ptzMsg.m_cmd.compare("STOP"))//停止
		{
			if(0 == ptzMsg.m_data.compare("ZIN"))//焦距拉近
			{
				cmd = vag::PTZ_CMD_ZOOM_IN;
			}
			else if(0 == ptzMsg.m_data.compare("ZOUT"))//焦距拉远
			{
				cmd = vag::PTZ_CMD_ZOOM_OUT;
			}
			else if(0 == ptzMsg.m_data.compare("FN"))//焦点调近
			{
				cmd = vag::PTZ_CMD_FOCUS_IN;
			}
			else if(0 == ptzMsg.m_data.compare("FR"))//焦点调远
			{
				cmd = vag::PTZ_CMD_FOCUS_OUT;
			}
			else if(0 == ptzMsg.m_data.compare("IO"))//光圈自动增大
			{
				cmd = vag::PTZ_CMD_LARGE;
			}
			else if(0 == ptzMsg.m_data.compare("IC"))//光圈自动减小
			{
				cmd = vag::PTZ_CMD_SMALL;
			}	
			else if(0 == ptzMsg.m_data.compare("TU"))//上
			{
				cmd = vag::PTZ_CMD_UP;
			}
			else if(0 == ptzMsg.m_data.compare("TD"))//下
			{
				cmd = vag::PTZ_CMD_DOWN;
			}
			else if(0 == ptzMsg.m_data.compare("PL"))//左
			{
				cmd = vag::PTZ_CMD_LEFT;
			}
			else if(0 == ptzMsg.m_data.compare("PR"))//右
			{
				cmd = vag::PTZ_CMD_RIGHT;
			}

			data = vag::PTZ_CMD_STOP;
		}

		int ret = 0;
		if (cmd != 0)
		{
			NET_CMU_OPER_CTRL_PTZ ctrl_ptz;
			memset(&ctrl_ptz, 0, sizeof(NET_CMU_OPER_CTRL_PTZ));
			
			ctrl_ptz.ptz_id = ptzMsg.m_videoId;
			strncpy(ctrl_ptz.cmd, ptzMsg.m_cmd.c_str(), sizeof(ctrl_ptz.cmd));
			strncpy(ctrl_ptz.param, ptzMsg.m_data.c_str(), sizeof(ctrl_ptz.param));
			ctrl_ptz.speed = ptzMsg.m_speed;
            ctrl_ptz.param1 = atoi(ptzMsg.m_data.c_str());
			unsigned long size = sizeof(NET_CMU_OPER_CTRL_PTZ);
			m_msg_proc((char *)"ControlPTZ", (char*)&ctrl_ptz, &size, g_param);
		}		 
		
		return buildCommonRsp("ControlPTZ",ret);		
	}
	return "";
}

std::string CMvComm::buildGetPuSerialPortRsp(const std::string &cmd,
									CMU_SERIAL_PORT& serialPort, int code)
{
	TiXmlDocument *resmyDocument = new TiXmlDocument();
	TiXmlDeclaration resDeclaration( "1.0","gb2312", "" );//UTF-8"
	resmyDocument->InsertEndChild( resDeclaration );
	TiXmlElement *resRootElement = new TiXmlElement("response");
	resmyDocument->LinkEndChild(resRootElement);
	resRootElement->SetAttribute("command", cmd.c_str());

	TiXmlElement* resultcode = 
		resRootElement->InsertEndChild( TiXmlElement("result") )->ToElement();

	resultcode->SetAttribute("code", code);
	resultcode->InsertEndChild(TiXmlText("success"));


	TiXmlElement* resparameters = 
		resRootElement->InsertEndChild( TiXmlElement("parameters") )->ToElement();

	TiXmlElement *forDecoder = 
		resparameters->InsertEndChild(TiXmlElement("forDecoder"))->ToElement();
	forDecoder->InsertEndChild(TiXmlText(serialPort.for_decoder));

	char port[32] = {0};
	sprintf(port, "%d", serialPort.serial_port);
	TiXmlElement *serial_port = 
		resparameters->InsertEndChild(TiXmlElement("serialPort"))->ToElement();
	serial_port->InsertEndChild(TiXmlText(port));

	char video_id[32] = {0};
	sprintf(video_id, "%d", serialPort.video_id);
	TiXmlElement *videoId = 
		resparameters->InsertEndChild(TiXmlElement("videoId"))->ToElement();
	videoId->InsertEndChild(TiXmlText(video_id));

	char baud_rate[32] = {0};
	sprintf(baud_rate, "%d", serialPort.baud_rate);
	TiXmlElement *baudRate = 
		resparameters->InsertEndChild(TiXmlElement("baudRate"))->ToElement();
	baudRate->InsertEndChild(TiXmlText(baud_rate));

	char data_bit[32] = {0};
	sprintf(data_bit, "%d", serialPort.data_bit);
	TiXmlElement *dataBit = 
		resparameters->InsertEndChild(TiXmlElement("dataBit"))->ToElement();
	dataBit->InsertEndChild(TiXmlText(data_bit));

	char cparity[32] = {0};
	sprintf(cparity, "%d", serialPort.parity);
	TiXmlElement *parity = 
		resparameters->InsertEndChild(TiXmlElement("parity"))->ToElement();
	parity->InsertEndChild(TiXmlText(cparity));

	char stop_bit[32] = {0};
	sprintf(stop_bit, "%d", serialPort.stop_bit);
	TiXmlElement *stopBit = 
		resparameters->InsertEndChild(TiXmlElement("stopBit"))->ToElement();
	stopBit->InsertEndChild(TiXmlText(stop_bit));

	TiXmlElement *mode = 
		resparameters->InsertEndChild(TiXmlElement("mode"))->ToElement();
	mode->InsertEndChild(TiXmlText(serialPort.mode));

	char time_out[32] = {0};
	sprintf(time_out, "%d", serialPort.time_out);
	TiXmlElement *timeOut = 
		resparameters->InsertEndChild(TiXmlElement("timeOut"))->ToElement();
	timeOut->InsertEndChild(TiXmlText(time_out));

	char flow_control[32] = {0};
	sprintf(flow_control, "%d", serialPort.flow_control);
	TiXmlElement *flowControl = 
		resparameters->InsertEndChild(TiXmlElement("flowControl"))->ToElement();
	flowControl->InsertEndChild(TiXmlText(flow_control));

	char decoder_type[32] = {0};
	sprintf(decoder_type, "%d", serialPort.decoder_type);
	TiXmlElement *decoderType = 
		resparameters->InsertEndChild(TiXmlElement("decoderType"))->ToElement();
	decoderType->InsertEndChild(TiXmlText(decoder_type));

	char decoder_address[32] = {0};
	sprintf(decoder_address, "%d", serialPort.decoder_address);
	TiXmlElement *decoderAddress = 
		resparameters->InsertEndChild(TiXmlElement("decoderAddress"))->ToElement();
	decoderAddress->InsertEndChild(TiXmlText(decoder_address));

	char work_mode[32] = {0};
	sprintf(work_mode, "%d", serialPort.work_mode);
	TiXmlElement *workMode = 
		resparameters->InsertEndChild(TiXmlElement("workMode"))->ToElement();
	workMode->InsertEndChild(TiXmlText(work_mode));

	
	//resss << *resmyDocument;

    TiXmlPrinter printer; 
    resmyDocument->Accept(&printer); 
     std::string ress(printer.CStr());

	delete resmyDocument;
	return ress;
}

std::string CMvComm::buildGetPuSoftwareVersionRsp(
	const std::string &cmd, NET_CMU_SOFT_VERSION& version, int code)
{
	TiXmlDocument *resmyDocument = new TiXmlDocument();
	TiXmlDeclaration resDeclaration( "1.0","gb2312", "" );//UTF-8"
	resmyDocument->InsertEndChild( resDeclaration );
	TiXmlElement *resRootElement = new TiXmlElement("response");
	resmyDocument->LinkEndChild(resRootElement);
	resRootElement->SetAttribute("command", cmd.c_str());

	TiXmlElement* resultcode = 
		resRootElement->InsertEndChild( TiXmlElement("result") )->ToElement();

	resultcode->SetAttribute("code", code);
	resultcode->InsertEndChild(TiXmlText("success"));


	TiXmlElement* resparameters = 
	resRootElement->InsertEndChild( TiXmlElement("parameters") )->ToElement();

	TiXmlElement *puId = 
		resparameters->InsertEndChild(TiXmlElement("puId"))->ToElement();
	puId->InsertEndChild(TiXmlText(version.pu_id));

	TiXmlElement *ver = 
		resparameters->InsertEndChild(TiXmlElement("version"))->ToElement();
	ver->InsertEndChild(TiXmlText(version.version));

	TiXmlElement *buildTime = 
		resparameters->InsertEndChild(TiXmlElement("buildTime"))->ToElement();
	buildTime->InsertEndChild(TiXmlText(version.build_time));


	
    TiXmlPrinter printer; 
    resmyDocument->Accept(&printer); 
     std::string ress(printer.CStr());

	delete resmyDocument;
	return ress;
}

std::string CMvComm::buildGetPuIpInfoRsp(const std::string & cmd,
								CMU_IP_INFO& ipInfo, int code)
{
	TiXmlDocument *resmyDocument = new TiXmlDocument();
	TiXmlDeclaration resDeclaration( "1.0","gb2312", "" );//UTF-8"
	resmyDocument->InsertEndChild( resDeclaration );
	TiXmlElement *resRootElement = new TiXmlElement("response");
	resmyDocument->LinkEndChild(resRootElement);
	resRootElement->SetAttribute("command", cmd.c_str());

	TiXmlElement* resultcode = 
		resRootElement->InsertEndChild( TiXmlElement("result") )->ToElement();

	resultcode->SetAttribute("code", code);
	resultcode->InsertEndChild(TiXmlText("success"));


	TiXmlElement* resparameters = 
		resRootElement->InsertEndChild( TiXmlElement("parameters") )->ToElement();

	char netId[32] = {0};
	sprintf(netId, "%d", ipInfo.net_id);
	TiXmlElement *netIdElem = 
		resparameters->InsertEndChild(TiXmlElement("netId"))->ToElement();
	netIdElem->InsertEndChild(TiXmlText(netId));

	TiXmlElement *ipAddress = 
		resparameters->InsertEndChild(TiXmlElement("ipAddress"))->ToElement();
	ipAddress->InsertEndChild(TiXmlText(ipInfo.ip_addr));

	char ctrl_port[32] = {0};
	sprintf(ctrl_port, "%d", ipInfo.ctrl_port);
	TiXmlElement *controlPort = 
		resparameters->InsertEndChild(TiXmlElement("controlPort"))->ToElement();
	controlPort->InsertEndChild(TiXmlText(ctrl_port));

	TiXmlElement *subnetMask = 
		resparameters->InsertEndChild(TiXmlElement("subnetMask"))->ToElement();
	subnetMask->InsertEndChild(TiXmlText(ipInfo.net_mask));

	TiXmlElement *dns1 = 
		resparameters->InsertEndChild(TiXmlElement("dns"))->ToElement();
	dns1->InsertEndChild(TiXmlText(ipInfo.dns_addr[0]));

	TiXmlElement *dns2 = 
		resparameters->InsertEndChild(TiXmlElement("dns"))->ToElement();
	dns2->InsertEndChild(TiXmlText(ipInfo.dns_addr[1]));
	
	TiXmlElement *gateway = 
		resparameters->InsertEndChild(TiXmlElement("gateway"))->ToElement();
	gateway->InsertEndChild(TiXmlText(ipInfo.gate_way));


	
    TiXmlPrinter printer; 
    resmyDocument->Accept(&printer); 
     std::string ress(printer.CStr());

	delete resmyDocument;
	return ress;
}


std::string CMvComm::buildGetPresetPtzRsp(
	const std::string &cmd, PtzPresets& presets, int code, int rett)
{
	TiXmlDocument *resmyDocument = new TiXmlDocument();
	TiXmlDeclaration resDeclaration( "1.0","gb2312", "" );//UTF-8"
	resmyDocument->InsertEndChild( resDeclaration );
	TiXmlElement *resRootElement = new TiXmlElement("response");
	resmyDocument->LinkEndChild(resRootElement);
	resRootElement->SetAttribute("command", cmd.c_str());

	TiXmlElement* resultcode = 
		resRootElement->InsertEndChild( TiXmlElement("result") )->ToElement();

	resultcode->SetAttribute("code", code);
	resultcode->InsertEndChild(TiXmlText("success"));


	
	if (rett == 0)
	{
		TiXmlElement* resparameters = 
		resRootElement->InsertEndChild( TiXmlElement("parameters") )->ToElement();
		/*
		TiXmlElement *vagDeviceId = 
			resparameters->InsertEndChild(TiXmlElement("vagDeviceId"))->ToElement();
		vagDeviceId->InsertEndChild(TiXmlText(vagid));
		*/
		for(int i = 0; i < presets.preset_list_cnt; i++)
		{
			TiXmlElement *preset = 
				resparameters->InsertEndChild(TiXmlElement("preset"))->ToElement();

			char preset_index[32] = {0};
			sprintf(preset_index, "%d", presets.preset_lists[i].preset_index);
			TiXmlElement *presetIndex = 
				preset->InsertEndChild(TiXmlElement("presetIndex"))->ToElement();
			presetIndex->InsertEndChild(TiXmlText(preset_index));
			TiXmlElement *presetName = 
				preset->InsertEndChild(TiXmlElement("presetName"))->ToElement();
			presetName->InsertEndChild(TiXmlText(presets.preset_lists[i].preset_name));
		}
	}

	
    TiXmlPrinter printer; 
    resmyDocument->Accept(&printer); 
     std::string ress(printer.CStr());

	delete resmyDocument;
	return ress;
}

std::string CMvComm::buildGetPuStorageFilesRsp(
	const std::string &cmd, CMU_QUERY_STORE_FILE& storagefiles, int code)
{
	int		ulNumFlag = 1, ulSize;
	size_t	ulFileCnt;
	TiXmlDocument *resmyDocument = new TiXmlDocument();
	TiXmlDeclaration resDeclaration( "1.0","gb2312", "" );//UTF-8"
	resmyDocument->InsertEndChild( resDeclaration );
	TiXmlElement *resRootElement = new TiXmlElement("response");
	resmyDocument->LinkEndChild(resRootElement);
	resRootElement->SetAttribute("command", "QueryPuLocalStorageFiles");

	TiXmlElement* resultcode = 
		resRootElement->InsertEndChild( TiXmlElement("result") )->ToElement();

	resultcode->SetAttribute("code", code);
	resultcode->InsertEndChild(TiXmlText("PU_SUCCESS"));

	//
	TiXmlElement *parameters = resRootElement->InsertEndChild(TiXmlElement("parameters"))->ToElement();

	//TiXmlElement *puId = parameters->InsertEndChild(TiXmlElement("puId"))->ToElement();
	//puId->SetAttribute("puId", req.m_puid.c_str());

    char buf[24];
    memset(buf,0,sizeof(buf));
    sprintf(buf,"%d",storagefiles.video_id);
	TiXmlElement *videoId = parameters->InsertEndChild(TiXmlElement("videoId"))->ToElement();
	videoId->InsertEndChild(TiXmlText(buf));

	//TiXmlElement *cuId = parameters->InsertEndChild(TiXmlElement("cuId"))->ToElement();
	//cuId->SetAttribute("cuId", req.m_cuId.c_str());
    memset(buf,0,sizeof(buf));
    sprintf(buf,"%d",storagefiles.file_list_cnt);
	TiXmlElement *numberFlag = parameters->InsertEndChild(TiXmlElement("numberFlag"))->ToElement();
	numberFlag->InsertEndChild(TiXmlText(buf));

    for(int i = 0; i < storagefiles.file_list_cnt; i++)	
    {
		TiXmlElement *recordFile = parameters->InsertEndChild(TiXmlElement("recordFile"))->ToElement();
        std::string sName(storagefiles.record_file_lists[i].file_name) ;
		TiXmlElement *filename = recordFile->InsertEndChild(TiXmlElement("filename"))->ToElement();
		filename->InsertEndChild(TiXmlText(sName.c_str())); 
        char time[40];
        memset(time,0,sizeof(time));
        sprintf(time, "%04d-%02d-%02d %02d:%02d:%02d",
        storagefiles.record_file_lists[i].start_time.year, \
        storagefiles.record_file_lists[i].start_time.month, \
        storagefiles.record_file_lists[i].start_time.date, \
        storagefiles.record_file_lists[i].start_time.hour, \
        storagefiles.record_file_lists[i].start_time.minute, \
        storagefiles.record_file_lists[i].start_time.second);

		TiXmlElement *startTime = recordFile->InsertEndChild(TiXmlElement("startTime"))->ToElement();
		startTime->InsertEndChild(TiXmlText(time)); 

        memset(time,0,sizeof(time));
        sprintf(time, "%04d-%02d-%02d %02d:%02d:%02d",
        storagefiles.record_file_lists[i].stop_time.year, \
        storagefiles.record_file_lists[i].stop_time.month, \
        storagefiles.record_file_lists[i].stop_time.date, \
        storagefiles.record_file_lists[i].stop_time.hour, \
        storagefiles.record_file_lists[i].stop_time.minute, \
        storagefiles.record_file_lists[i].stop_time.second);
		TiXmlElement *stopTime = recordFile->InsertEndChild(TiXmlElement("stopTime"))->ToElement();
		stopTime->InsertEndChild(TiXmlText(time)); 
        char fileSize[24];
        sprintf(fileSize,"%d",storagefiles.record_file_lists[i].size);
		TiXmlElement *size = recordFile->InsertEndChild(TiXmlElement("size"))->ToElement();
		size->InsertEndChild(TiXmlText(fileSize)); 
	}

    TiXmlPrinter printer; 
    resmyDocument->Accept(&printer); 
    std::string resss(printer.CStr());

   
    delete resmyDocument;
	return resss;
}

std::string CMvComm::buildCommonRsp(const std::string &cmd,int code)
{
	TiXmlDocument *resmyDocument = new TiXmlDocument();
	TiXmlDeclaration resDeclaration( "1.0","gb2312", "" );//UTF-8"
	resmyDocument->InsertEndChild( resDeclaration );
	TiXmlElement *resRootElement = new TiXmlElement("response");
	resmyDocument->LinkEndChild(resRootElement);
	resRootElement->SetAttribute("command", cmd.c_str());

	TiXmlElement* resultcode = 
		resRootElement->InsertEndChild( TiXmlElement("result") )->ToElement();

	resultcode->SetAttribute("code", code);
	resultcode->InsertEndChild(TiXmlText("success"));
	TiXmlElement* resparameters = 
		resRootElement->InsertEndChild( TiXmlElement("parameters") )->ToElement();

	
    TiXmlPrinter printer; 
    resmyDocument->Accept(&printer); 
     std::string ress(printer.CStr());

	delete resmyDocument;
	return ress;
}

void CMvComm::flushVagResLst()
{
	comn::AutoCritSec lock(m_csRes);
	m_vagResLst.clear();
}

int CMvComm::NotifyStatus(const vag::StatusNotification* status,size_t length)
{
	comn::AutoCritSec lock(m_csRes);

	//VagResNodeArray::iterator itVec = 
	//	std::find_if(m_vagResLst.begin(),
	//	m_vagResLst.end(),
	//	EqVagNode(status.id,status.videoId));
	//if (itVec != m_vagResLst.end())
	//{
	//	itVec->status = status.status;
	//}

	return vag::VAG_ERR_OK;
}

int CMvComm::NotifyAlarm(const vag::AlarmNotification* alarm,size_t length)
{
	if(m_cmuClient.sendAlarmToCmu(alarm, length))
		return vag::VAG_ERR_OK;

	return vag::VAG_ERR_FAIL;
}

bool CMvComm::findNetMedia(unsigned long id)
{
	return getMediaManager().findNetMedia(id);
}

bool CMvComm::stopNetMedia(const StopMediaReq &req)
{
	return getMediaManager().stopNetMedia(req);	
}

void CMvComm::doStop()
{
	stopMedia(0);	
}

bool CMvComm::stopMedia(int sessionId)
{
	m_taskQueue.push(sessionId);
	return true;
}

int CMvComm::run()
{
	while(!m_canExit)
	{
		int taskValue = 0;

		if (!m_taskQueue.popTimeWait(taskValue,5000))
		{
			continue;
		}	

		MSLogEx(MS_LOG_LEVEL_INFO,
			"[%s:%d] CMvComm::run, pop task id:%d",
			__FUNCTION__, __LINE__, taskValue);


		getMediaManager().stopNetMedia(taskValue);

	}
	return 0;
}
