// WebUI.cpp : Implementation of CWebUI
#include "stdafx.h"
#include "WebUI.h"
#include "MixerWrap.h"
#include "KeyWrap.h"
#include "MixerWrapEx.h"
#include "comutil.h"
#include "BasePlugUtil.h"
#include <mmsystem.h>


#pragma comment(lib,"comsuppw.lib");
#pragma comment(lib,"winmm.lib");

wchar_t CWebUI::szVersion[] = _T("0.5.5");
// CWebUI

BEGIN_CATEGORY_MAP(CWebUI)      
	IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)      
	IMPLEMENTED_CATEGORY(CATID_SafeForScripting)      
END_CATEGORY_MAP() 

STDMETHODIMP CWebUI::GetInterfaceSafetyOptions(REFIID riid,
														   DWORD *pdwSupportedOptions,
														   DWORD *pdwEnabledOptions)
{
	ATLTRACE(_T("CObjectSafetyImpl::GetInterfaceSafetyOptions\n"));
	if (!pdwSupportedOptions || !pdwEnabledOptions)
		return E_FAIL;
	LPUNKNOWN pUnk;
	if (_InternalQueryInterface (riid, (void**)&pUnk) == E_NOINTERFACE) {
		// Our object doesn't even support this interface.
		return E_NOINTERFACE;
	}else{
		// Cleanup after ourselves.
		pUnk->Release();
		pUnk = NULL;
	}
	if (riid == IID_IDispatch) {
		// IDispatch is an interface used for scripting. If your
		// control supports other IDispatch or Dual interfaces, you
		// may decide to add them here as well. Client wants to know
		// if object is safe for scripting. Only indicate safe for
		// scripting when the interface is safe.
		*pdwSupportedOptions = INTERFACESAFE_FOR_UNTRUSTED_CALLER;
		*pdwEnabledOptions = m_dwCurrentSafety &
			INTERFACESAFE_FOR_UNTRUSTED_CALLER;
		return S_OK;
	}else if ((riid == IID_IPersistStreamInit) ||
		(riid == IID_IPersistStorage)) {
			// IID_IPersistStreamInit and IID_IPersistStorage are
			// interfaces used for Initialization. If your control
			// supports other Persistence interfaces, you may decide to
			// add them here as well. Client wants to know if object is
			// safe for initializing. Only indicate safe for initializing
			// when the interface is safe.
			*pdwSupportedOptions = INTERFACESAFE_FOR_UNTRUSTED_DATA;
			*pdwEnabledOptions = m_dwCurrentSafety &
				INTERFACESAFE_FOR_UNTRUSTED_DATA;
			return S_OK;
	}else{
		// We are saying that no other interfaces in this control are
		// safe for initializing or scripting.
		*pdwSupportedOptions = 0;
		*pdwEnabledOptions = 0;
		return E_FAIL;
	}
}
STDMETHODIMP CWebUI::SetInterfaceSafetyOptions(REFIID riid,
														   DWORD dwOptionSetMask,
														   DWORD dwEnabledOptions)
{
	ATLTRACE(_T("CObjectSafetyImpl::SetInterfaceSafetyOptions\n"));
	if (!dwOptionSetMask && !dwEnabledOptions) return E_FAIL;
	LPUNKNOWN pUnk;
	if (_InternalQueryInterface (riid, (void**)&pUnk) == E_NOINTERFACE) {
		// Our object doesn't even support this interface.
		return E_NOINTERFACE;
	}else{
		// Cleanup after ourselves.
		pUnk->Release();
		pUnk = NULL;
	}
	// Store our current safety level to return in
	// GetInterfaceSafetyOptions
	m_dwCurrentSafety |= dwEnabledOptions & dwOptionSetMask;
	if ((riid == IID_IDispatch) &&
		(m_dwCurrentSafety & INTERFACESAFE_FOR_UNTRUSTED_CALLER)) {
			// Client wants us to disable any functionality that would
			// make the control unsafe for scripting. The same applies to
			// any other IDispatch or Dual interfaces your control may
			// support. Because our control is safe for scripting by
			// default we just return S_OK.
			return S_OK;
	}else if (((riid == IID_IPersistStreamInit) ||
		(riid == IID_IPersistStorage)) &&
		(m_dwCurrentSafety & INTERFACESAFE_FOR_UNTRUSTED_DATA)) {
			// Client wants us to make the control safe for initializing
			// from persistent data. For these interfaces, this control
			// is safe so we return S_OK. For Any interfaces that are not
			// safe, we would return E_FAIL.
			return S_OK;
	}else{
		// This control doesn't allow Initialization or Scripting
		// from any other interfaces so return E_FAIL.
		return E_FAIL;
	}
}

STDMETHODIMP CWebUI::MicphoneBoost(LONG isBoost)
{
	// TODO: Add your implementation code here
	
	/*char temp[20];	
	sprintf_s(temp,"%d\0",BasePlugUtil::GetOSVer());
	_bstr_t strStr = temp;
	LPCSTR lpctStrName = strStr;
	::MessageBox(NULL,(LPCWSTR)lpctStrName,_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);*/
	if (isBoost ==1)
	{
		if (BasePlugUtil::GetOSVer() < 6/*==2 || BasePlugUtil::GetOSVer()==3*/)
		{
			MixerWrap::MicphoneBoost(true);
		}
		else 
		{
			MixerWrap::MicphoneBoost_W7(true);
		}
	}
	else 
	{
		if (BasePlugUtil::GetOSVer() < 6/*==2 || BasePlugUtil::GetOSVer()==3*/)
		{
			MixerWrap::MicphoneBoost(false);
		}
		else 
		{
			MixerWrap::MicphoneBoost_W7(false);
		}
		
	}

	return S_OK;
}

STDMETHODIMP CWebUI::OpenVoiceSet(void)
{
	// TODO: Add your implementation code here
	if (BasePlugUtil::GetOSVer() < 6/*==2 || BasePlugUtil::GetOSVer()==3*/)
	{
		ShellExecute(NULL, _T("open"), _T("Sndvol32.exe"), _T("-Record"),NULL,SW_SHOW);
	}
	else 
	{
		ShellExecute(NULL, _T("open"), _T("control.exe"), _T("mmsys.cpl,,1"),NULL,SW_SHOW);
	}
	
	return S_OK;
}

STDMETHODIMP CWebUI::GetAllPlay(BSTR* payList)
{
	// TODO: Add your implementation code here
	HKEY hKey;
	wchar_t vecbuf[20000];
	wcsset(vecbuf,0);
	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,
		TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths"),

		0,
		KEY_READ,
		&hKey) == ERROR_SUCCESS
		)
	{
		KeyWrap::QueryKey(hKey,vecbuf);
	}
	//For QQ
	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,
		TEXT("SOFTWARE\\Tencent"),

		0,
		KEY_READ,
		&hKey) == ERROR_SUCCESS
		)
	{
		KeyWrap::QueryKeyForQQ(hKey,vecbuf);
	}
	//
	int len =wcslen(vecbuf);
	vecbuf[len-1]='\0';
	RegCloseKey(hKey);
	//::MessageBox(NULL,vecbuf,_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);
	*payList = SysAllocString(vecbuf);
	return S_OK;
}

STDMETHODIMP CWebUI::OpenPlay(BSTR exePath)
{
	// TODO: Add your implementation code here
    //////////////////////////////////////////////////////////////////////////
	/*HWAVEIN hWaveIn;


	WAVEFORMATEX waveform;
	waveform.wFormatTag=WAVE_FORMAT_PCM;
	waveform.nChannels=1;
	waveform.nSamplesPerSec=11025;
	waveform.nAvgBytesPerSec=11025;
	waveform.nBlockAlign=1;
	waveform.wBitsPerSample=8;
	waveform.cbSize=0;

	MMRESULT re=	waveInOpen(&hWaveIn,WAVE_MAPPER,&waveform,NULL,NULL,CALLBACK_WINDOW);
	if (re==0)
	{
          ::MessageBox(NULL,_T("Test"),_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);
	}*/
	//////////////////////////////////////////////////////////////////////////




	_bstr_t strStr = exePath;
	LPCSTR lpctStrName = strStr;
	//::MessageBox(NULL,lpctStrName,"提示信息",MB_OK   |  MB_ICONINFORMATION);
	WinExec(lpctStrName,SW_NORMAL);
	return S_OK;
}

STDMETHODIMP CWebUI::StereoMix(LONG bIsMix)
{
	// TODO: Add your implementation code here
	if (bIsMix ==1)
	{
		if (BasePlugUtil::GetOSVer() < 6/*==2 || BasePlugUtil::GetOSVer()==3*/)
		{
			if (MixerWrap::IsInputSingleSelect())
	        {
				MixerWrap::MicphoneMut(false);
				MixerWrap::StereoMix(false);
				MixerWrap::MicphoneBoost(true);

				MixerWrap::SelectRecordInByStereo();
				MixerWrap::StereoMixVolume(19660);
				MixerWrap::OutPutMicphoneVolume(65535);
				MixerWrap::InPutMicphoneVolume(52428);
	        } 
	        else	        			
			{
				
				MixerWrap::MicphoneMut(false);
				MixerWrap::StereoMix(false);
				MixerWrap::MicphoneBoost(true);

				//MixerWrap::OutPutMicphoneVolume(26214);
				//录音控制40%
			}
		}
		else 
		{

			//麦克风加强，并设置麦克风音量
			MixerWrapEx::SetDefaultInput(false);//先将micphone设为可用状态
			MixerWrap::MicphoneMut_W7(false,1.0);//取消麦克风静音，并将音量设为最大
			MixerWrap::MicphoneBoost_W7(true);//麦克风加强


			//KeyWrap::StereoMix_W7(true);			
			//::MessageBox(NULL,(LPCWSTR)_T("StereoMix "),_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);
			
			MixerWrapEx::SetDefaultInput(true);            		
			MixerWrapEx::MutSpeaker(FALSE);//     扬声器下面的麦克风和线路输入设置
			MixerWrap::MutSpeaker_W7(false,0.25);//扬声器静音设为否，并调整音量到25%
		}			
	}
	else if(bIsMix ==0)
	{
		
		if (BasePlugUtil::GetOSVer() < 6/*==2 || BasePlugUtil::GetOSVer()==3*/)
		{
			MixerWrap::StereoMix(true);
		}
		else 
		{			
			//KeyWrap::StereoMix_W7(false);
			MixerWrapEx::SetDefaultInput(false);

		}

	}

	return S_OK;
}

STDMETHODIMP CWebUI::IsAdminControl(LONG* isAdmin)
{
	// TODO: Add your implementation code here

	*isAdmin =KeyWrap::IsHaveAdminControl();
	return S_OK;
}

STDMETHODIMP CWebUI::ListenMySelf(void)
{
	// TODO: Add your implementation code here
	if (BasePlugUtil::GetOSVer() < 6/*==2 || BasePlugUtil::GetOSVer()==3*/)
	{
		if (MixerWrap::IsInputSingleSelect())
		{
			//单选
			MixerWrap::MicphoneMut(false);
			//MixerWrap::StereoMix(false);
			MixerWrap::MicphoneBoost(true);
			
			MixerWrap::SelectRecordInByMicphone();
			MixerWrap::OutPutMicphoneVolume(65535);
			MixerWrap::InPutMicphoneVolume(52428);
		}
		else
		{
			MixerWrap::MicphoneMut(false);
			//MixerWrap::StereoMix(false);
			MixerWrap::MicphoneBoost(true);
	
			MixerWrap::OutPutMicphoneVolume(26214);
			//录音控制80%

		}
	}
	else 
	{

		//麦克风加强，并设置麦克风音量
		MixerWrapEx::SetDefaultInput(false);//先将micphone设为可用状态
		MixerWrap::MicphoneMut_W7(false,1.0);//取消麦克风静音，并将音量设为最大
		MixerWrap::MicphoneBoost_W7(true);//麦克风加强


		//KeyWrap::StereoMix_W7(true);			
		//::MessageBox(NULL,(LPCWSTR)_T("StereoMix "),_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);
		
		//MixerWrapEx::SetDefaultInput(true);            		
		MixerWrapEx::MutSpeaker(FALSE);//     扬声器下面的麦克风和线路输入设置
		MixerWrap::MutSpeaker_W7(false,0.25);//扬声器静音设为否，并调整音量到25%
	}			
	
	return S_OK;
}

STDMETHODIMP CWebUI::TestUpdate(void)
{
	// TODO: Add your implementation code here
	::MessageBox(NULL,_T("Version 2"),_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);

	return S_OK;
}

STDMETHODIMP CWebUI::GetVersion(BSTR* strVersion)
{
	// TODO: Add your implementation code here
	
	*strVersion = SysAllocString(szVersion);

	return S_OK;
}
