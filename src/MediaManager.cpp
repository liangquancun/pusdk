#include "MediaManager.h"
#include "log.h"

NetMediaManager::NetMediaManager()
{
}

NetMediaManager::~NetMediaManager()
{	
}


bool NetMediaManager::startNetMedia(const NetMediaPtr &ptr)
{	
	if (!ptr)
	{
		printf("Error:startNetMedia ptr is Null\n");
        MSLogEx(MS_LOG_LEVEL_INFO,"[%s:%d]ptr is NULL\n",__FUNCTION__,__LINE__);
		return false;
	}

	if (ptr->play() != 0)
	{
        MSLogEx(MS_LOG_LEVEL_INFO,"[%s:%d]play error\n",__FUNCTION__,__LINE__);
		printf("Error:startNetMedia Play Error\n");
		return false;
	}
	ptr->SetVoiceIntercomCallBack(m_voice_callback);
	comn::AutoCritSec lock(m_cs);
	m_mediaLst.push_back(ptr);

	return true;
}

bool NetMediaManager::stopNetMedia(int sessionId)
{
	MSLogEx(MS_LOG_LEVEL_INFO,
		"[%s:%d] NetMediaManager::stopNetMedia, sessionId:%d",
		__FUNCTION__, __LINE__,	sessionId);


	std::vector<NetMediaPtr>::iterator it = m_mediaLst.begin();
	for (; it != m_mediaLst.end(); it++)
	{
		if ((*it)->getSessionId() == sessionId)
		{
			break;
		}
	}

	if (it != m_mediaLst.end())
	{

		MSLogEx(MS_LOG_LEVEL_INFO,
			"[%s:%d] NetMediaManager::stopNetMedia, puid:%s, videoId:%d, streamType:%d",
			__FUNCTION__, __LINE__,	
			(*it)->getStartMediaReqMsg().m_puId.c_str(),
			(*it)->getStartMediaReqMsg().m_videoId,
			(*it)->getStartMediaReqMsg().m_iStreamType);
        printf("stop sessionId:%s\n",(*it)->m_sessionId);
        (*it)->stopMedia();
	    comn::AutoCritSec lock(m_cs);
		m_mediaLst.erase(it);
            
	}
    return true;
}
void NetMediaManager::SetVoiceIntercomCallBack(net_voice_intercom_proc voice_callback)
{
	m_voice_callback = voice_callback;
}
bool NetMediaManager::stopNetMedia(const StopMediaReq &req)
{
	int sessid = 0;
	{
		comn::AutoCritSec lock(m_cs);
		std::vector<NetMediaPtr>::iterator it = m_mediaLst.begin();
		for (; it != m_mediaLst.end(); it++)
		{
            if(req.m_fileName.empty())
            {
                printf("11111req.m_fileName.empty()\n");
                if ((*it)->getStartMediaReqMsg().m_videoId == req.m_videoId && 
    				(*it)->getStartMediaReqMsg().m_iStreamType == req.m_iStreamType &&
    				(*it)->getStartMediaReqMsg().m_iMediaType == req.m_iMediaType)
    			{
    				sessid = (*it)->getSessionId();
    				break;
    			}
            }
            else
            {
                printf("22222req.m_fileName.empty()\n");

                if ((*it)->getStartMediaReqMsg().m_videoId == req.m_videoId &&
                    ((*it)->getStartMediaReqMsg().m_fileName).compare(req.m_fileName) == 0)
    			{
    				sessid = (*it)->getSessionId();
    				break;
    			}

            }
		}
	}
	
	if (sessid > 0)
	{
		stopNetMedia(sessid);
	}

	return true;
}

NetMediaPtr  NetMediaManager::findNetMedia(unsigned long id)
{
	std::vector<NetMediaPtr>::iterator it = m_mediaLst.begin();
    
	for (; it != m_mediaLst.end(); it++)
	{
        //if ( (*it).get() == (NetMedia* )id )
  
        if(strcmp((*it)->m_sessionId,(char *)id) == 0)
		{
			return (*it);
		}
	}
	return NetMediaPtr();
}

