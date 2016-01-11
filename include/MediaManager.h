#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H

#include "NetMedia.h"
#include "XmlUtil.h"
#include <vector>
#include "TCriticalSection.h"


class NetMediaManager
{
public:
	NetMediaManager();
    virtual ~NetMediaManager();

	bool startNetMedia(const NetMediaPtr &ptr);
	bool stopNetMedia(int sessionId);
	bool stopNetMedia(const StopMediaReq &req);
	
	NetMediaPtr  findNetMedia(unsigned long id);

	void SetVoiceIntercomCallBack(net_voice_intercom_proc voice_callback);
	comn::CriticalSection m_cs;
private:
	std::vector<NetMediaPtr> m_mediaLst;
	net_voice_intercom_proc m_voice_callback;
};

#endif


