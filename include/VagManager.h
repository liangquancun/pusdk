#ifndef VAG_SDK_H_
#define VAG_SDK_H_

#include "MvComm.h"
#include <string>

using namespace vag;

class VAGSdkManager : public VAGProxy
{
public:
	VAGSdkManager();
	virtual ~VAGSdkManager() {}

	 /**
     * 注册
	 * @param regInfo [in] 注册信息
     */
    virtual int Login (const VAGRegInfo &regInfo);  
    virtual int Logout ();
   

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
	 //设置对讲回调
	virtual int SetVoiceIntercomCallBack(net_voice_intercom_proc voice_callback);
	 /**
     * 资源上报
     */    

	virtual void Destroy() ;

	CMvComm& getCMvComm(){ return m_mvComm; }
private:
	CMvComm m_mvComm;
};

#endif







