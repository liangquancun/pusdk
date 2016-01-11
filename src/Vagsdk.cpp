
#include  "VagManager.h"
#include "Vagsdk.h"



/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * 获取VAGProxy接口，需要调用VAG_destroy释放
 * @return VAGProxy接口
 */
DLLEXPORT vag::VAGProxy* WINAPI VAG_create()
{
	return new VAGSdkManager();
}


/**
 * 释放VAGProxy接口
 * @param pVAG 该指针由VAG_create返回
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
 * 获取版本
 */
DLLEXPORT const char* WINAPI VAG_getVersion()
{
	return "Vag1.0.0.1";
}


#ifdef __cplusplus
}
#endif

/////////////////////////////////////////////////////////////////////////////






