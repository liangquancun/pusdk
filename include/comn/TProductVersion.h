/**-------------------------------------
* 文件名称：TProductVersion.h
* 功能描述：获取应用程序或动态链接库的版本号
* 创建标志：chenhq 2012.11.26


* 修改标示：
* 修改描述：
---------------------------------------**/


#ifndef TPRODUCTVERSION_H_
#define TPRODUCTVERSION_H_

#include <xstring>

#ifdef WIN32

#pragma comment(lib, "Version")

// 格式化文本
std::string versionformat(const char* lpszFormat, ...)
{
	const int MAX_FORMAT_CHAR = 512 * 4;
	char buffer[MAX_FORMAT_CHAR] = {0};
	memset(buffer, '\0', MAX_FORMAT_CHAR);

	va_list pars;
	va_start(pars, lpszFormat);

#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
	vsprintf_s(buffer, MAX_FORMAT_CHAR, lpszFormat, pars);
#else
	vsprintf(buffer, lpszFormat, pars);
#endif
	va_end(pars);

	return buffer;
}

/**
* @brief 获取当前程序版本号
* @param  hInst 程序句柄
* @return 
* @see
*/
std::string GetProductVersion(IN const HMODULE hInst = NULL)
{
	std::string version;
	VS_FIXEDFILEINFO *pVerInfo = NULL;
	DWORD	dwTemp, dwSize, dwHandle = 0;
	BYTE	*pData = NULL;
	UINT	uLen;
	char    file[256];

	try 
	{
		GetModuleFileName(hInst, file, 256);
		dwSize = GetFileVersionInfoSize((LPTSTR) file, &dwTemp);
		if (dwSize == 0) throw 1;

		pData = new BYTE[dwSize];
		if (pData == NULL) throw 1;

		if (!GetFileVersionInfo((LPTSTR) file, dwHandle, dwSize, pData))
			throw 1;

		if (!VerQueryValue(pData, "\\", (void **) &pVerInfo, &uLen)) 
			throw 1;

		DWORD verMS = pVerInfo->dwFileVersionMS;
		DWORD verLS = pVerInfo->dwFileVersionLS;

		int ver[4];
		ver[0] = HIWORD(verMS);
		ver[1] = LOWORD(verMS);
		ver[2] = HIWORD(verLS);
		ver[3] = LOWORD(verLS);

		// Are lo-words used?
		if (ver[2] != 0 || ver[3] != 0)
		{
			version = versionformat("%d.%d.%d.%d", ver[0], ver[1], ver[2], ver[3]);
		}
		else if (ver[0] != 0 || ver[1] != 0)
		{
			version = versionformat("%d.%d", ver[0], ver[1]);
		}

		delete pData;
		return version;
	}
	catch(...) 
	{
		return "";
	}	
}

#else

#ifndef PRODUCT_VERSION "1.0.0.1"
#define PRODUCT_VERSION
#endif

typedef DWORD HMODULE;

std::string GetProductVersion(IN const HMODULE hInst = NULL)
{
	return PRODUCT_VERSION;
}

#endif

#endif

