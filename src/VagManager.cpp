#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 )
#pragma warning( disable : 4267 )
#pragma warning( disable : 4312 )
#pragma warning( disable : 4018 )
#endif


#include "VagManager.h"
#include "log.h"


int VAGProxy::Login (const VAGRegInfo &regInfo)
{
	return vag::VAG_ERR_FAIL;
}

int VAGProxy::Logout()
{
	return vag::VAG_ERR_FAIL;
}
   

	 /**
     * 状态通知,告警通知
	 * @param status [in] 信息
     */
int VAGProxy::NotifyStatus(const StatusNotification* status,size_t length)
{
	return vag::VAG_ERR_FAIL;
}

int VAGProxy::NotifyAlarm(const AlarmNotification* alarm,size_t length)
{
	return vag::VAG_ERR_FAIL;
}

 /**
 * 设置回调
 * @param pSink [in] 回调类指针
 */
int VAGProxy::SetNotificationSink(msg_proc msg_cb, stream_proc stream_cb,unsigned long param)
{
	return vag::VAG_ERR_FAIL;
}

int VAGProxy::SetVoiceIntercomCallBack(net_voice_intercom_proc voice_callback)
{
	return  vag::VAG_ERR_FAIL;
}

////////////////////////////////////////////////////////////////////////////////

VAGSdkManager::VAGSdkManager()
{
}

int VAGSdkManager::Login (const VAGRegInfo &regInfo)
{	
	return m_mvComm.startMvClient(regInfo)?vag::VAG_ERR_OK:vag::VAG_ERR_OTHER_FAIL;
}

int VAGSdkManager::Logout()
{
	return m_mvComm.stopMvClient()?vag::VAG_ERR_OK:vag::VAG_ERR_OTHER_FAIL;
}
   

	 /**
     * 状态通知,告警通知
	 * @param status [in] 信息
     */
int VAGSdkManager::NotifyStatus(const StatusNotification* status,size_t length)
{
	return m_mvComm.NotifyStatus(status,length);
}

int VAGSdkManager::NotifyAlarm(const AlarmNotification* alarm,size_t length)
{
	return m_mvComm.NotifyAlarm(alarm,length);
}

unsigned long g_param = 0;

 /**
 * 设置回调
 * @param pSink [in] 回调类指针
 */
int VAGSdkManager::SetNotificationSink(msg_proc msg_cb, stream_proc stream_cb,unsigned long param)
{	
	g_param = param;
	return m_mvComm.SetNotificationSink(msg_cb, stream_cb);
}


int VAGSdkManager::SetVoiceIntercomCallBack(net_voice_intercom_proc voice_callback)
{
	return m_mvComm.SetVoiceIntercomCallBack(voice_callback);
}
void VAGSdkManager::Destroy() 
{
	delete this;
}

