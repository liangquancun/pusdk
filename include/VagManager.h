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
     * ע��
	 * @param regInfo [in] ע����Ϣ
     */
    virtual int Login (const VAGRegInfo &regInfo);  
    virtual int Logout ();
   

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
	 //���öԽ��ص�
	virtual int SetVoiceIntercomCallBack(net_voice_intercom_proc voice_callback);
	 /**
     * ��Դ�ϱ�
     */    

	virtual void Destroy() ;

	CMvComm& getCMvComm(){ return m_mvComm; }
private:
	CMvComm m_mvComm;
};

#endif







