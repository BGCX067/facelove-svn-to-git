// MixerWrapD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MixerWrap.h"
#include <locale.h>
#include <windows.h>
#include <ShellAPI.h>
#include "VolController.h"
#include <string>
#pragma comment(lib,"winmm.lib");

int GetOSVer()  
{  
	OSVERSIONINFO   osver;     
	osver.dwOSVersionInfoSize   =   sizeof(OSVERSIONINFO);     
	GetVersionEx(&osver);     
	if(osver.dwPlatformId == 2)  
	{  
		if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)  
		{  
			printf("xp\n");  
			return(2);  
		}  
		if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)  
		{  
			printf("windows 2003\n");  
			return(3);  
		}  
		if(osver.dwMajorVersion ==  6 && osver.dwMinorVersion == 0)  
		{  
			printf("vista and 2008\n");  
			return(4);  
		}  
		if(osver.dwMajorVersion ==  6 && osver.dwMinorVersion == 1)  
		{  
			printf("2008 R2 and Windows 7\n");  
			return(5);  
		}  
	}  
	return 0;  
} 


int _tmain(int argc, _TCHAR* argv[])
{
	//ShellExecute(NULL, _T("open"), _T("control.exe"), _T("mmsys.cpl,,1"),NULL,SW_SHOW);
	 _wsetlocale(LC_ALL,_T("Chinese-simplified"));
	// int temp =GetOSVer();
	//MixerWrap::MicphoneBoost_W7(true);
	//MixerWrap::MicphoneMut(true);
	//MixerWrap::StereoMix(true);
	//Sleep(5000);


	//MixerWrap::MicphoneBoost(false);
	//MixerWrap::MicphoneMut(false);
	//MixerWrap::StereoMix(false);


	// MixerWrap::StereoMixVolume(50);
	 /*CVolController::InitializeMixer();
	 	
	 LONG min =CVolController::GetVolumeRangMin();
	 LONG max =CVolController::GetVolumeRangMax();
     CVolController::SetVolumeValue((max-min)*30/100);
	 LONG lValue=CVolController::GetVolumeValue(MainChannel);

	  CVolController::SetMicrPhoneVolume(100);*/
     // MixerWrap::SelectRecordIn(MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC);
	 //MixerWrap::SelectRecordInByName(_T("¡¢ÃÂ…˘ªÏ“Ù"));

	// MixerWrap::StereoMix(true);
	// MixerWrap::StereoMixVolume(19660);
	 //MixerWrap::StereoMix(false);
	 MixerWrap::InPutMicphoneVolume(52428);

    

	//putchar('e');

	getchar();
	return 0;
}

