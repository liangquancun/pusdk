#include "NetMedia.h"
#include "MvComm.h"
#include "tinyxml.h"
#include <string>

#include "ms_pu_param.h"

#include "ms_pu_lib.h"
#include "log.h"
#include <arpa/inet.h>

#define  ERROR_PACKET_NUM (1000)
#define MAX_BUFFER_POOL_SIZE       (1024*1024)

static long long count_size;
enum ConnectionState
{
	STATE_DEFAULT = 0, 
	STATE_FINISH = 1, 
	STATE_CLOSE = 2
};

NetMedia::NetMedia(const StartMediaReq &req,
				   CMvComm *pMvcomm)
	: m_req(req),
	 m_pMvcomm(pMvcomm),
	 m_tryCount(0),
	 m_sendingFrame(NULL),
	 m_length(0),
	 m_hasError(false),
	 m_isAudioTask(true),
	 m_voice_callback(NULL)
{
    memset(m_sessionId,0,sizeof(m_sessionId));
    if(m_req.m_fileName.empty())
    {
        sprintf(m_sessionId,"%d:%d:%d:%d",m_req.m_videoId,
                m_req.m_iMediaType,m_req.m_iStreamType,0);
    }
    else
    {
        sprintf(m_sessionId,"%d:%s",m_req.m_videoId,
                m_req.m_fileName.c_str());
    }
    printf("m_sessionId:%s\n",m_sessionId);
}

NetMedia::~NetMedia()
{

	//stopMedia();
	m_allocator.destroy();
    printf("~NetMedia()\n");
}



int NetMedia::play()
{
	if (!m_steamHandle.isOpen())
	{
		if (!m_steamHandle.open())
		{
            MSLogEx(MS_LOG_LEVEL_INFO,"[%s:%d]open socket fail\n",__FUNCTION__,__LINE__);
			return -1;
		}

		if (m_steamHandle.connect(m_req.m_szTargetIp.c_str(),m_req.m_targetPort) != 0)
		{
            MSLogEx(MS_LOG_LEVEL_INFO,"[%s:%d]connect mdu fail:[%s:%d]\n",__FUNCTION__,
                __LINE__,m_req.m_szTargetIp.c_str(),m_req.m_targetPort);
			m_steamHandle.close();
			return -1;
		}
        int ret = -1;
		int ret = m_steamHandle.setNonblock(true);
		//printf("setNonblock, ret=%d\n",ret);

		ret = m_steamHandle.setSendBufferSize(1024 * 1024 * 1);
		//printf("setSendBufferSize, ret=%d\n",ret);

		ret = sendInfoPacket();
        if(!ret)
        {
            MSLogEx(MS_LOG_LEVEL_INFO,"[%s:%d]send info fail\n",__FUNCTION__,__LINE__);
        }
        bool ok = m_allocator.create(MAX_BUFFER_POOL_SIZE);
    	if(!ok)
    	{
    		return -1;
    	}
	}

	NET_MDU_START_MEDIA start_media;
	memset(&start_media, 0, sizeof(NET_MDU_START_MEDIA));

	memcpy(start_media.media.pu_id, m_req.m_puId.c_str(), sizeof(start_media.media.pu_id)-1);
	start_media.media.video_id = m_req.m_videoId;
	start_media.media.media_type = m_req.m_iMediaType;
	start_media.media.stream_type = m_req.m_iStreamType;
	start_media.link_mode = m_req.m_iLinkMode;
	memcpy(start_media.target_ip, m_req.m_szTargetIp.c_str(), sizeof(start_media.target_ip)-1);
	start_media.target_port = m_req.m_targetPort;
    if(m_req.m_playType == 1)
    {
        memcpy(start_media.fileName,m_req.m_fileName.c_str(),m_req.m_fileName.size());
        sscanf(m_req.m_startTime.c_str(), "%04d-%02d-%02d %02d:%02d:%02d",
                       &(start_media.start_time.year),
                       &(start_media.start_time.month),
                       &(start_media.start_time.date),
                       &(start_media.start_time.hour),
                       &(start_media.start_time.minute),
                       &(start_media.start_time.second)
                      );
        sscanf(m_req.m_endTime.c_str(), "%04d-%02d-%02d %02d:%02d:%02d",
                       &(start_media.stop_time.year),
                       &(start_media.stop_time.month),
                       &(start_media.stop_time.date),
                       &(start_media.stop_time.hour),
                       &(start_media.stop_time.minute),
                       &(start_media.stop_time.second)
                      );   
        
	    start_media.request_type = 1;
    }//end if
    else
    {
	    start_media.request_type = 0;
    }
	
	/*MSLogEx(MS_LOG_LEVEL_INFO,
		"[%s:%d] NetMedia::play,puId:%s,videoId:%d,media_type:%d,link_mode:%d,\
		target_ip:%s,target_port:%d,streamType:%d,play_type:%d",
		__FUNCTION__, __LINE__, 
		start_media.media.pu_id,
		start_media.media.video_id,
		start_media.media.media_type,
		start_media.link_mode,
		start_media.target_ip,
		start_media.target_port,
		start_media.media.stream_type,
		start_media.request_type);*/
	
	    m_mediaType=m_req.m_iMediaType;

	int ret = 0;
//Media Type 0 为音频	
	if(start_media.media.media_type == 0)
		{
		
		 ret = m_pMvcomm->m_stream_proc(
							(unsigned long)m_sessionId, 
							(char *)"Start_AUDIO", 
							(char*)&start_media, 
							sizeof(NET_MDU_START_MEDIA), g_param);//对讲
		if(ret != 0)
		{
            MSLogEx(MS_LOG_LEVEL_INFO,"[%s:%d]open socket fail,ret=%d\n",__FUNCTION__,__LINE__,ret);
            return ret;
        }
		start();
		printf("start Audio Task\n");
		}
	else
		 ret = m_pMvcomm->m_stream_proc(
							(unsigned long)m_sessionId, 
							(char *)"Start_MEDIA", 
							(char*)&start_media, 
							sizeof(NET_MDU_START_MEDIA), g_param);
	if (ret != 0)
	{
		printf("m_stream_proc,ret===%d\n",ret);
        MSLogEx(MS_LOG_LEVEL_INFO,"[%s:%d]open socket fail,ret=%d\n",__FUNCTION__,__LINE__,ret);
	}

	return ret;
}

int NetMedia::pause()
{
    NET_MDU_START_MEDIA start_media;
	memset(&start_media, 0, sizeof(NET_MDU_START_MEDIA));
    start_media.media.video_id = m_req.m_videoId;
    start_media.scale = 0;
    int ret = m_pMvcomm->m_stream_proc(
    						(unsigned long)m_sessionId, 
    						(char *)"Control_MEDIA", 
    						(char*)&start_media, 
    						sizeof(NET_MDU_START_MEDIA), g_param);
	if (ret != 0)
	{
		printf("m_stream_proc,ret===%d\n",ret);
	}
	return 0;
}


int NetMedia::setSeek(int offset)
{
    NET_MDU_START_MEDIA start_media;
	memset(&start_media, 0, sizeof(NET_MDU_START_MEDIA));
    start_media.media.video_id = m_req.m_videoId;
    start_media.scale = 0;
    int ret = m_pMvcomm->m_stream_proc(
    						(unsigned long)m_sessionId, 
    						(char *)"Control_MEDIA", 
    						(char*)&start_media, 
    						sizeof(NET_MDU_START_MEDIA), g_param);
	if (ret != 0)
	{
		printf("m_stream_proc,ret===%d\n",ret);
	}
}

int NetMedia::setScale(double param)
{
    NET_MDU_START_MEDIA start_media;
	memset(&start_media, 0, sizeof(NET_MDU_START_MEDIA));
    start_media.media.video_id = m_req.m_videoId;
    start_media.scale = param;
    int ret = m_pMvcomm->m_stream_proc(
    						(unsigned long)m_sessionId, 
    						(char *)"Control_MEDIA", 
    						(char*)&start_media, 
    						sizeof(NET_MDU_START_MEDIA), g_param);
	if (ret != 0)
	{
		printf("m_stream_proc,ret===%d\n",ret);
	}
}

int NetMedia::step()
{
    NET_MDU_START_MEDIA start_media;
	memset(&start_media, 0, sizeof(NET_MDU_START_MEDIA));
    start_media.media.video_id = m_req.m_videoId;
    start_media.scale = 0;
    int ret = m_pMvcomm->m_stream_proc(
    						(unsigned long)m_sessionId, 
    						(char *)"Control_MEDIA", 
    						(char*)&start_media, 
    						sizeof(NET_MDU_START_MEDIA), g_param);
	if (ret != 0)
	{
		printf("m_stream_proc,ret===%d\n",ret);
	}
}

StartMediaReq &NetMedia::getStartMediaReqMsg()
{
	return m_req;
}

int NetMedia::getSessionId()
{
	if (m_steamHandle.isOpen())
	{
		return m_steamHandle.getSocket();
	}
	return -1;
}

int NetMedia::stopMedia()
{
    printf("stopMedia:%s\n",m_sessionId);
	NET_MDU_START_MEDIA stop_media;
	memset(&stop_media, 0, sizeof(NET_MDU_START_MEDIA));

	memcpy(stop_media.media.pu_id, m_req.m_puId.c_str(), sizeof(stop_media.media.pu_id));
	stop_media.media.video_id = m_req.m_videoId;
	stop_media.media.media_type = m_req.m_iMediaType;
	stop_media.media.stream_type = m_req.m_iStreamType;
    if(!m_req.m_fileName.empty())
    {
        memcpy(stop_media.fileName,m_req.m_fileName.c_str(),sizeof(stop_media.fileName));
        stop_media.request_type = 1;
    }
	MSLogEx(MS_LOG_LEVEL_INFO,
		"[%s:%d] NetMedia::stop, <<<--- puId:%s,videoId:%d,streamType:%d,mediaType:%d",
		__FUNCTION__, __LINE__, 
		stop_media.media.pu_id,
		stop_media.media.video_id,
		stop_media.media.stream_type,
		stop_media.media.media_type);
		//0 为对讲
	
		if(stop_media.media.media_type == 0)
		{
            stop();
			m_pMvcomm->m_stream_proc(
						(unsigned long)m_sessionId, 
						(char *)"Stop_AUDIO", 
						(char*)&stop_media, 
						sizeof(NET_MDU_START_MEDIA), g_param);
		}
		else
		{
			m_pMvcomm->m_stream_proc(
						(unsigned long)m_sessionId, 
						(char *)"Stop_MEDIA", 
						(char*)&stop_media, 
						sizeof(NET_MDU_START_MEDIA), g_param);
        }

	if (m_steamHandle.isOpen())
	{
		m_steamHandle.close();
	}
    return 0;
}


void NetMedia::setFormat(const vag::StreamFormat& fmt)
{
	m_fmt = fmt;
}

// int NetMedia::rawWrite(const char* buffer, int len)
// {
// 	int length = len;
// 	int bytes = 0;
// 	int m_tryCount = 0;
// 
// 	do
// 	{
// 		bytes = m_steamHandle.send(buffer, length, 0);
// 		if (bytes > 0)
// 		{
// 			length -= bytes;
// 			buffer += bytes;
// 		}
// 		else if (SOCKET_ERROR == bytes)
// 		{
// 			int err = TSocketLib::getLastError();
// 			if (ERR_INTR == err)
// 			{
// 				continue;
// 			}
// 			else if (err != ERR_WOULDBLOCK)
// 			{
// 				return (len == length) ? -1 : (len - length);
// 			}
// 			else	//ERR_WOULDBLOCK
// 			{
// 				//printf("ERR_WOULDBLOCK(%p)\n",this);
// 
// 				//
// 				if (++m_tryCount > 1000)
// 				{
// 					return bytes;
// 				}
// 
// 				bytes = 1;
// 
// #ifdef WIN32
// 				Sleep(1);
// #else
// 				usleep(1000);
// #endif
// 				//
// 			}
// 
// // 			else
// // 			{
// // 				return (len == length) ? -1 : (len - length);
// // 			}
// 
// 		}
// 	} while (bytes > 0 && length > 0 );
// 	return (len - length);
// }


int NetMedia::rawWrite(const char* buffer, int len)
{
	int length = len;
	int bytes = 0;
	do
	{
		bytes = m_steamHandle.send(buffer, length, 0);
		if (bytes > 0)
		{
			length -= bytes;
			buffer += bytes;
        MSLogEx(MS_LOG_LEVEL_INFO,"[%s:%d]rawWrite ,length:%d,lenWrite:%d\n",__FUNCTION__,__LINE__,length,bytes);
		}
		else if (SOCKET_ERROR == bytes)
		{
			int err = TSocketLib::getLastError();
			if (ERR_INTR == err)
			{
				continue;
			}
			else if (ERR_WOULDBLOCK == err)
			{
				//printf("ERR_WOULDBLOCK (%d)\n", errno);
				break;
			}
			else
			{
				return (len == length) ? -1 : (len - length);
			}
		}
	} while (bytes > 0 && length > 0);
	return (len - length);
}

int NetMedia::send(const char* buffer, int length)
{
	if (!m_steamHandle.isOpen())
	{
		return vag::VAG_ERR_OTHER_FAIL;
	}

	int result = 0;
	int lenWrite = rawWrite(buffer,length);
	if (lenWrite == length)
	{
		result = 0;
		m_tryCount = 0;
	}
	else if (lenWrite == -1)
	{
		MSLogEx(MS_LOG_LEVEL_INFO,
			"[%s:%d] NetMedia::send, endOfStream, lenWrite == -1",
			__FUNCTION__, __LINE__);

		endOfStream();
	}
	else if (0 == lenWrite)
	{
		m_tryCount += 1;
		if (m_tryCount > (25 * 10))
		{
			MSLogEx(MS_LOG_LEVEL_INFO,
				"[%s:%d] NetMedia::send, endOfStream, 0 == lenWrite",
				__FUNCTION__, __LINE__);

			endOfStream();			
		}
	}
	else
	{
		result = 0;
	}
	return result;
}

//读取音频数据
int NetMedia::readPacket(vag::StreamPacket& packet)
{
	RTPHEADER_S rtpHeader;
	int ret = 0,len = 0;
	//接收RTP头
	ret=m_steamHandle.readnInTime(&rtpHeader,sizeof(RTPHEADER_S),5000);
	
	if(ret  == SOCKET_ERROR)
		{
			return -1;
		}
	else if(ret == 0)
		{
			return 0;
		}
	len = ntohs(rtpHeader.usPtLen);

	if(packet.data == NULL)
		{
			MSLogEx(MS_LOG_LEVEL_ERROR,
				"[%s:%d] NetMedia::readPacket, malloc filed",
				__FUNCTION__, __LINE__);
			return -1;
		}
	//接收数据
	ret=m_steamHandle.readnInTime(packet.data,len,2000);
	
	if(ret  == SOCKET_ERROR)
		{
			return -1;
		}
	else if(ret == 0)
		{
			return -1;
		}
	packet.size = len;

	if(rtpHeader.pt == 0)
		packet.flag=G711_U_TYPE;
	else
		packet.flag=G711_A_TYPE;

	return packet.size;
	
}
	
int NetMedia::writePacket(const vag::StreamPacket& packet)
{
    if (packet.data == NULL)
    {
        printf("packet.data == NULL:%d\n",packet.size);
        MSLogEx(MS_LOG_LEVEL_INFO,"[%s:%d]packet.data == NULL\n",__FUNCTION__,__LINE__);
        return vag::VAG_ERR_OTHER_FAIL;
    }
    if(m_hasError)
    {
        printf("m_hasError\n");
        MSLogEx(MS_LOG_LEVEL_INFO,"[%s:%d]m_hasError\n",__FUNCTION__,__LINE__);
        return vag::VAG_ERR_FAIL;
    }

	int iLen = packet.size;
	int iCount = 0;
	int iSend = 4096 - sizeof(RTPHEADER_S);

	//rtp_packet_header rtpHeader;er
	RTPHEADER_S rtpHeader;
	memset(&rtpHeader,0,sizeof(RTPHEADER_S));

	rtpHeader.x = 1;
	rtpHeader.cc = 0;
	
	rtpHeader.seqno = 0;	
	rtpHeader.version = 2;
	rtpHeader.usExtLen = htons(2);
	rtpHeader.usProfile = htons(1);
	rtpHeader.uiDecoderID = htonl(0x00100000);

	if (packet.flag == G711_A_TYPE|| packet.flag == G711_U_TYPE)
	{
		if(G711_U_TYPE ==packet.flag)
			rtpHeader.pt = 0;
		else
			rtpHeader.pt = 8;
		rtpHeader.marker = 0;
	}
	else
	{
		rtpHeader.pt = 96;		
		rtpHeader.marker = packet.flag;		
	}


	rtpHeader.ucFrameNum = iLen / iSend + 1;
	rtpHeader.ucFrameSeq = 0;
	rtpHeader.lTimeSec = htonl(packet.timestamp / 1000);
	rtpHeader.ssrc = htonl(
		(packet.timestamp - (vag::uint64_t)rtpHeader.lTimeSec*1000)%1000);

	int ret = -1; 
	while(iLen > iSend)
	{
		rtpHeader.usPtLen = htons(iSend);
		rtpHeader.ucFrameSeq = iCount;			
#if 0
		ret = send((char*)&rtpHeader,sizeof(rtpHeader));
		if(ret == 0)
		{
			send((char*)(packet.data+iSend*iCount),iSend);
		}
#else
		BufferFramePtr pFrame = m_allocator.allocFrame(iSend + sizeof(rtpHeader));
		if (pFrame != NULL)
		{	
			pFrame->create(rtpHeader, (unsigned char*)(packet.data+iSend*iCount));
			pFrame->addRef();	
			m_queue.push_back(pFrame);

			if (!sendCurrFrame())
			{
				endOfStream();
                printf("%s %d:send error\n",__FILE__,__LINE__);
                MSLogEx(MS_LOG_LEVEL_INFO,"[%s:%d]send error\n",__FUNCTION__,__LINE__);
                return vag::VAG_ERR_FAIL;  	
			}			
		}
		else
		{
			cleanQueuedFrames();
		}
#endif
		iLen -= iSend;
        MSLogEx(MS_LOG_LEVEL_INFO,"[%s:%d]send iCount:%d,iSend:%d\n",__FUNCTION__,__LINE__,iCount,iSend);
		iCount++;
	}
	if(0 != iLen)
	{
		rtpHeader.usPtLen = htons(iLen);
		rtpHeader.ucFrameSeq = iCount;

#if 0	
		ret = send((char*)&rtpHeader,sizeof(rtpHeader));
		if(ret == 0)
		{
			send((char*)(packet.data+iSend*iCount),iLen);
		}
#else
		BufferFramePtr pFrame = m_allocator.allocFrame(iLen + sizeof(rtpHeader));
		if (pFrame != NULL)
		{	
			pFrame->create(rtpHeader, (unsigned char*)(packet.data+iSend*iCount));
			pFrame->addRef();	
			m_queue.push_back(pFrame);

			if (!sendCurrFrame())
			{
				endOfStream();
                printf("%s %d:send error\n",__FILE__,__LINE__);
                MSLogEx(MS_LOG_LEVEL_INFO,"[%s:%d]send fail\n",__FUNCTION__,__LINE__);
        		return vag::VAG_ERR_FAIL;  	
			}			
		}
		else
		{
			cleanQueuedFrames();
		}
#endif
        MSLogEx(MS_LOG_LEVEL_INFO,"[%s:%d]send iCount:%d,iLen:%d\n",__FUNCTION__,__LINE__,iCount,iLen);

	}
	if(0 == iLen)
	{
		return vag::VAG_ERR_OK;  	
	}
	return vag::VAG_ERR_OK;  
}
int NetMedia::sendFrame(BufferFramePtr pFrame)
{
	int result = STATE_DEFAULT;
	int offset = m_length;
	int length = pFrame->getLength() - offset;
	int lenWrite = rawWrite(pFrame->getPtr() + offset, length);
	if (lenWrite == length)
	{
		m_length += lenWrite;
		result = STATE_FINISH;
		m_tryCount = 0;
        MSLogEx(MS_LOG_LEVEL_INFO,"[%s:%d]send ,length:%d,lenWrite:%d\n",__FUNCTION__,__LINE__,length,lenWrite);
	}
	else if (lenWrite < 0)
	{
		result = STATE_CLOSE;		
	}	
	else if (0 == lenWrite)
	{	
		if (++m_tryCount > ERROR_PACKET_NUM)
		{
            printf("m_tryCountm_tryCount:%d\n",m_tryCount);
            MSLogEx(MS_LOG_LEVEL_INFO,"[%s:%d]send error,m_tryCount:%d\n",__FUNCTION__,__LINE__,m_tryCount);
			result = STATE_CLOSE;
		}
        MSLogEx(MS_LOG_LEVEL_INFO,"[%s:%d]send error,m_tryCount:%d\n",__FUNCTION__,__LINE__,m_tryCount);
        
	}
	else
	{
		m_length += lenWrite;
        MSLogEx(MS_LOG_LEVEL_INFO,"[%s:%d]send error,m_length:%d,lenWrite:%d\n",__FUNCTION__,__LINE__,m_length,lenWrite);
		m_tryCount = 0;
	}
	return result;
}
bool NetMedia::sendCurrFrame()
{
	if (m_hasError)
	{
        printf("%s %d:m_hasError\n",__FILE__,__LINE__);
		return false;
	}

	bool success = true;
	while (true)
	{
		if (m_sendingFrame)
		{
			int result = sendFrame(m_sendingFrame);
			if (STATE_FINISH == result)
			{
				m_sendingFrame->release();
				m_sendingFrame = NULL;
				m_length = 0;
			}
			else if (STATE_DEFAULT == result) // not complete
			{
				break;
			}
			else
			{
				m_sendingFrame->release();
				m_sendingFrame = NULL;
				m_length = 0;
				success = false;
				break;
			}
		}
		else if (!m_queue.empty())
		{
			m_sendingFrame = m_queue.front();
			m_queue.pop_front();
		}
		else // all frames sent
		{
			break;
		}
	}
	return success;
}
int NetMedia::cleanQueuedFrames()
{
	int count = m_queue.size();
	/// clean queue
	while (!m_queue.empty())
	{
		BufferFramePtr pFrame = m_queue.front();
		m_queue.pop_front();
		pFrame->release();		
	}

	if (count > 0)
	{
	    MSLogEx(MS_LOG_LEVEL_INFO,
			"[%s:%d] NetMedia::cleanQueuedFrames, count:%d",
			__FUNCTION__, __LINE__, count);
	}
	return count;
}

void NetMedia::endOfStream()
{
	if (m_hasError)
	{
        printf("%s %d:m_hasError\n",__FILE__,__LINE__);
		return;
	}
	m_hasError = true;

	if (m_steamHandle.isOpen())
	{
		if (m_pMvcomm != NULL)
		{
			m_pMvcomm->stopMedia(m_steamHandle.getSocket());
			//m_steamHandle.close();
		}
	} 
    return;
}

bool NetMedia::sendInfoPacket()
{
	TiXmlDocument *myDocument = new TiXmlDocument();          
	TiXmlDeclaration Declaration( "1.0","UTF-8", "yes" );     
	myDocument->InsertEndChild( Declaration );
	TiXmlElement *RootElement = new TiXmlElement("request"); 
	myDocument->LinkEndChild(RootElement);	
	RootElement->SetAttribute("command","Media_Info");

	TiXmlElement* parameters = 
		RootElement->InsertEndChild( TiXmlElement( "parameters") )->ToElement();		
	TiXmlElement* xpuId = parameters->InsertEndChild( TiXmlElement( "puId") )->ToElement();	
	xpuId->InsertEndChild( TiXmlText(m_req.m_puId.c_str()) ); 

	char invite_resp_msg[2048] = {0};
	memset(invite_resp_msg,0,sizeof(invite_resp_msg));


	sprintf(invite_resp_msg,"%d",m_req.m_videoId);
	TiXmlElement* videoHeader = 
		parameters->InsertEndChild( TiXmlElement( "videoId") )->ToElement();	
	videoHeader->InsertEndChild( TiXmlText(invite_resp_msg) );	

	memset(invite_resp_msg,0,sizeof(invite_resp_msg));
	sprintf(invite_resp_msg,"%d",m_req.m_iMediaType);
	TiXmlElement* mediaType = 
		parameters->InsertEndChild( TiXmlElement( "mediaType") )->ToElement();	
	mediaType->InsertEndChild( TiXmlText(invite_resp_msg) ); 

	memset(invite_resp_msg,0,sizeof(invite_resp_msg));
	sprintf(invite_resp_msg,"%d",m_req.m_iStreamType);
	TiXmlElement* streamType = 
		parameters->InsertEndChild( TiXmlElement( "streamType") )->ToElement();	
	streamType->InsertEndChild( TiXmlText(invite_resp_msg) ); 
    if(NULL != m_req.m_fileName.c_str())
    {
    	TiXmlElement* fileName = 
    		parameters->InsertEndChild( TiXmlElement( "fileName") )->ToElement();	
    	fileName->InsertEndChild( TiXmlText(m_req.m_fileName.c_str()) ); 
    }	    
    TiXmlPrinter printer; 
    myDocument->Accept(&printer); 
     std::string xml(printer.CStr());
	
    delete myDocument;	

	memset(invite_resp_msg,0,sizeof(invite_resp_msg));
	sprintf(invite_resp_msg,"INFO sip:%s SIP/2.0/TCP\r\n"\
		"To: %s\r\n"\
		"From: %s\r\n"\
		"CSeq: %d INFO\r\n"\
		"Call-ID: %s\r\n"\
		"Content-Type: text/xml\r\n"\
		"Max-Forwards: 70\r\n"\
		"Content-Length: %d\r\n\r\n%s",
		m_req.m_szTargetIp.c_str(),
		m_req.m_puId.c_str(),
		m_req.m_puId.c_str(),
		1/*m_iSeq++*/,
		m_req.m_puId.c_str(),
		xml.length(), 
		xml.c_str());

	return m_steamHandle.send(invite_resp_msg,strlen(invite_resp_msg)) >0 ;
}
//接收数据语音对讲
int NetMedia::run() 
{
	vag::uint8_t buf[4096]={0};
	
	while(m_isAudioTask)
		{
			vag::StreamPacket packet;
            memset(&packet,0,sizeof(vag::StreamPacket));
            memset(buf,0,sizeof(buf));

			int ret;
			packet.data=buf;
			ret = readPacket(packet);
			if(ret ==0)
				usleep(10*1000);
			else if(ret == -1)
			{
				endOfStream();
                printf("%s %d:send error\n",__FILE__,__LINE__);
				break;
			}
           //printf("*********************************************************\n");
           if(packet.data != NULL)
           {
           }
          
#if 1			
			if(m_voice_callback)
			{
				m_voice_callback((unsigned long)m_sessionId,(DataType)packet.flag,(char *)packet.data,packet.size,g_param);
			}
			else
			{
				endOfStream();
                printf("%s %d:send error\n",__FILE__,__LINE__);
				break;
			}
#endif
			memset(buf,0,sizeof(buf));	
		}
	return 0;
}
void NetMedia::SetVoiceIntercomCallBack(net_voice_intercom_proc voice_callback)
{
	m_voice_callback=voice_callback;
}
void NetMedia::doStop()
{
	m_isAudioTask = false;
}
