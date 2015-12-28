// DefaultTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MixerWrapEx.h"
#include <mmsystem.h>

HWAVEIN hWaveIn;  //输入设备
WAVEFORMATEX waveform; //采集音频的格式，结构体
BYTE *pBuffer1,*pBuffer2;//采集音频时的数据缓存
WAVEHDR wHdr1,wHdr2; //采集音频时包含数据缓存的结构体
static DWORD CALLBACK MicCallback(  //消息回掉函数
								  HWAVEIN hWaveIn, 
								  UINT  uMsg, 
								  DWORD dwInstance, 
								  DWORD dwParam1, 
								  DWORD dwParam2);


int _tmain(int argc, _TCHAR* argv[])
{
	//SetDefaultInput();//OK
	//MixerWrapEx::SetDefaultInput(false);
	MixerWrapEx::MutSpeaker(FALSE);
	/*HWAVEIN hWaveIn;
	WAVEFORMATEX waveform;
	waveform.wFormatTag=WAVE_FORMAT_PCM;
	waveform.nChannels=1;
	waveform.nSamplesPerSec=11025;
	waveform.nAvgBytesPerSec=11025;
	waveform.nBlockAlign=1;
	waveform.wBitsPerSample=8;
	waveform.cbSize=0;
	MMRESULT re=	waveInOpen(&hWaveIn,WAVE_MAPPER,&waveform,NULL,NULL,CALLBACK_WINDOW);*/

	//waveform.wFormatTag = WAVE_FORMAT_PCM;//声音格式为PCM
	//waveform.nSamplesPerSec = 16000;//采样率，16000次/秒
	//waveform.wBitsPerSample = 16;//采样比特，16bits/次
	//waveform.nChannels = 2;//采样声道数，2声道
	//waveform.nAvgBytesPerSec = 16000 * 4;//每秒的数据率，就是每秒能采集多少字节的数据
	//waveform.nBlockAlign = 4;//一个块的大小，采样bit的字节数乘以声道数
	//waveform.cbSize = 0;//一般为0

	////使用waveInOpen函数开启音频采集
	//MMRESULT mmr = waveInOpen(&hWaveIn,WAVE_MAPPER,&waveform,
	//	(DWORD)(MicCallback), NULL, CALLBACK_FUNCTION);
	//if(mmr != MMSYSERR_NOERROR)
	//	return false;
   
	return 0;
}

