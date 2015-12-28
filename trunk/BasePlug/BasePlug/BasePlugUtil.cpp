#include "StdAfx.h"
#include "BasePlugUtil.h"

BasePlugUtil::BasePlugUtil(void)
{
}

BasePlugUtil::~BasePlugUtil(void)
{
}

int  BasePlugUtil::GetOSVer()
{
	OSVERSIONINFO   osver;     
	osver.dwOSVersionInfoSize   =   sizeof(OSVERSIONINFO);     
	GetVersionEx(&osver);     
	if(osver.dwPlatformId == 2)  
	{  
		if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 0)  
		{  
			printf("xp\n");  
			return(3);  
		} 
		if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)  
		{  
			printf("xp\n");  
			return(4);  
		}  
		if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)  
		{  
			printf("windows 2003\n");  
			return(5);  
		}  
		if(osver.dwMajorVersion ==  6 && osver.dwMinorVersion == 0)  
		{  
			printf("vista and 2008\n");  
			return(6);  
		}  
		if(osver.dwMajorVersion ==  6 && osver.dwMinorVersion == 1)  
		{  
			printf("2008 R2 and Windows 7\n");  
			return(7);  
		}  
	}  
	return 0;  

}