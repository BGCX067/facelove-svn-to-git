#pragma once

class MixerWrap
{
public:
	MixerWrap(void){};
public:
	virtual ~MixerWrap(void){};

	

	static void MicphoneBoost(bool bIsBoost);
	static bool MicphoneBoost_W7(bool bIsBoost);
	static void MicphoneMut(bool bIsMut);
	static bool MicphoneMut_W7(bool bIsMut,float fVolume);
	//¡¢ÃÂ…˘ªÏ“Ùæ≤“Ù
	static void StereoMix(bool bIsMix);	
	static BOOL MutSpeaker_W7(BOOL bMute,float fVolume);

	static void StereoMixVolume(int nVolume);
	static void OutPutMicphoneVolume(int nVolume);
	static void InPutMicphoneVolume(int nVolume);

	static int  SelectRecordIn(DWORD  dwSrcType);
	//static int  SelectRecordInByName(WCHAR*  szSrcName);
	static int  SelectRecordInByMicphone();
	static int  SelectRecordInByStereo();
	

	static bool  IsInputSingleSelect();
	
	
};
