// KeyWrapD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <map>
using namespace std;
#include "KeyWrap.h"


int _tmain(int argc, _TCHAR* argv[])
{
	 _wsetlocale(LC_ALL,_T("Chinese-simplified"));
	 //is have manager power
	// KeyWrap::IsHaveAdminControl();

	//QueryAllPlay
	//HKEY hKey;
	//wchar_t vecbuf[20000];
	//wcsset(vecbuf,0);
	//if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,
	//	TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths"),

	//	0,
	//	KEY_READ,
	//	&hKey) == ERROR_SUCCESS
	//	)
	//{
	//	KeyWrap::QueryKey(hKey,vecbuf);
	//}
	////For QQ
	//if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,
	//	TEXT("SOFTWARE\\Tencent"),

	//	0,
	//	KEY_READ,
	//	&hKey) == ERROR_SUCCESS
	//	)
	//{
	//	KeyWrap::QueryKeyForQQ(hKey,vecbuf);
	//}
	////
	//int len =wcslen(vecbuf);
	//vecbuf[len-1]='\0';
	//RegCloseKey(hKey);

	//QueryAllInput	
	HKEY hKey;
	map<wstring,MMAudioDev> m_devs;
	
	
	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,
		TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\MMDevices\\Audio\\Capture"),

		0,
		KEY_READ,
		&hKey) == ERROR_SUCCESS
		)
	{
		KeyWrap::QueryInputKey(hKey,m_devs);
		
	}
	RegCloseKey(hKey);
	printf("m_devs:%d\r\n",m_devs.size());
	map<wstring,MMAudioDev>::iterator it =m_devs.begin();
	for (;it!=m_devs.end();it++)
	{
		MMAudioDev temp =it->second;
		//printf("dev's name:%s\r\n",temp.szDeviceName);
		_tprintf(TEXT("%s\n"),  temp.szEndPointName); 
	}


	
	KeyWrap::SetInputDevice(m_devs,1,0);
	char c = getchar();
	KeyWrap::SetInputDevice(m_devs,1,1);

	return 0;
}

