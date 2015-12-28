#pragma once
#include <Windows.h>
class MixerWrap
{
public:
	MixerWrap(void){};
public:
	virtual ~MixerWrap(void){};

	static void MicphoneBoost(bool bIsBoost);
	static void MicphoneBoost_W7(bool bIsBoost);
	static void MicphoneMut(bool bIsMut);
	static void StereoMix(bool bIsMix);
	static bool MicphoneMut_W7(bool bIsMut,float fVolume);

		
	static bool MutSpeaker_W7(bool bMute,float fVolume);

	//立体声混音音量调节
	static void StereoMixVolume(int nVolume);	
	static void OutPutMicphoneVolume(int nVolume);
	static void InPutMicphoneVolume(int nVolume);


	static int  SelectRecordIn(DWORD  dwSrcType);
	static int  SelectRecordInByName(WCHAR*  szSrcName);
	

};
