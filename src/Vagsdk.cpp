
#include  "VagManager.h"
#include "Vagsdk.h"



/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * ��ȡVAGProxy�ӿڣ���Ҫ����VAG_destroy�ͷ�
 * @return VAGProxy�ӿ�
 */
DLLEXPORT vag::VAGProxy* WINAPI VAG_create()
{
	return new VAGSdkManager();
}


/**
 * �ͷ�VAGProxy�ӿ�
 * @param pVAG ��ָ����VAG_create����
 *
 */
DLLEXPORT void WINAPI VAG_destroy(vag::VAGProxy* pVAG)
{
	if (pVAG != NULL)
	{
		VAGSdkManager *pSdk = dynamic_cast<VAGSdkManager *>(pVAG);
		if (pSdk != NULL)
		{
			pSdk->Destroy();
		}		
	}
}


/**
 * ��ȡ�汾
 */
DLLEXPORT const char* WINAPI VAG_getVersion()
{
	return "Vag1.0.0.1";
}


#ifdef __cplusplus
}
#endif

/////////////////////////////////////////////////////////////////////////////






