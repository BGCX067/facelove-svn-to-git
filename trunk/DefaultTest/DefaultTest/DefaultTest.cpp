// DefaultTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MixerWrapEx.h"
#include <mmsystem.h>

HWAVEIN hWaveIn;  //�����豸
WAVEFORMATEX waveform; //�ɼ���Ƶ�ĸ�ʽ���ṹ��
BYTE *pBuffer1,*pBuffer2;//�ɼ���Ƶʱ�����ݻ���
WAVEHDR wHdr1,wHdr2; //�ɼ���Ƶʱ�������ݻ���Ľṹ��
static DWORD CALLBACK MicCallback(  //��Ϣ�ص�����
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

	//waveform.wFormatTag = WAVE_FORMAT_PCM;//������ʽΪPCM
	//waveform.nSamplesPerSec = 16000;//�����ʣ�16000��/��
	//waveform.wBitsPerSample = 16;//�������أ�16bits/��
	//waveform.nChannels = 2;//������������2����
	//waveform.nAvgBytesPerSec = 16000 * 4;//ÿ��������ʣ�����ÿ���ܲɼ������ֽڵ�����
	//waveform.nBlockAlign = 4;//һ����Ĵ�С������bit���ֽ�������������
	//waveform.cbSize = 0;//һ��Ϊ0

	////ʹ��waveInOpen����������Ƶ�ɼ�
	//MMRESULT mmr = waveInOpen(&hWaveIn,WAVE_MAPPER,&waveform,
	//	(DWORD)(MicCallback), NULL, CALLBACK_FUNCTION);
	//if(mmr != MMSYSERR_NOERROR)
	//	return false;
   
	return 0;
}

