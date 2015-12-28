#include "stdafx.h"
#include "MixerWrapEx.h"
#include "BasePlugUtil.h"
#include "comutil.h"

HRESULT MixerWrapEx::SetDefaultAudioPlaybackDevice( LPCWSTR devID)
{
	if (BasePlugUtil::GetOSVer()>6) //win7
	{
		IPolicyConfig *pPolicyConfig;
		ERole reserved = eConsole;

		HRESULT hr = CoCreateInstance( __uuidof( CPolicyConfigClient ),
			NULL, CLSCTX_ALL, __uuidof( IPolicyConfig ), ( LPVOID* )&pPolicyConfig);
		if( SUCCEEDED( hr ) )
		{
			hr =pPolicyConfig->SetEndpointVisibility(devID,reserved+1);//分win7和vista
			hr = pPolicyConfig->SetDefaultEndpoint( devID, reserved );
			pPolicyConfig->Release();
		}
		return hr;
	} 
	else //vista
	{
		IPolicyConfigVista *pPolicyConfig;
		ERole reserved = eConsole;

		HRESULT hr = CoCreateInstance( __uuidof( CPolicyConfigVistaClient ),
			NULL, CLSCTX_ALL, __uuidof( IPolicyConfigVista ), ( LPVOID* )&pPolicyConfig);
		if( SUCCEEDED( hr ) )
		{
			hr =pPolicyConfig->SetEndpointVisibility(devID,reserved+1);//分win7和vista
			hr = pPolicyConfig->SetDefaultEndpoint( devID, reserved );
			pPolicyConfig->Release();
		}
		return hr;
	}
	return 0;	

}

BOOL MixerWrapEx::SetDefaultInput(bool bDefault)
{
	IMMDeviceEnumerator* pEnumerator;
	IMMDeviceCollection *pCollection = NULL;
	IMMDevice *pDevice=NULL;
	IPropertyStore *pProperties=NULL;
	IAudioEndpointVolume *pVolumeAPI=NULL;

	IAudioAutoGainControl* pAutoGain=NULL;
	UINT deviceCount = 0;
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	HRESULT hr=CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),(void**)&pEnumerator);
	if (hr != S_OK)
	{
		printf("CoCreateInstance Failed!/n");
		return 0;
	}
	hr = pEnumerator->EnumAudioEndpoints(eCapture/*eRender*/, DEVICE_STATEMASK_ALL, &pCollection); 
	if (hr != S_OK)
	{
		printf("EnumAudioEndpoints Failed!/n");
		goto releasepEnumerator;
	}

	hr = pCollection->GetCount(&deviceCount);
	if (hr != S_OK)
	{
		printf("GetCount Failed!/n");
		goto releasepCollection;
	}	
	for (UINT dev=0;dev<deviceCount;dev++)
	{
		pDevice = NULL;
		hr = pCollection->Item(dev,&pDevice);
		if (hr == S_OK)
		{
			hr = pDevice->OpenPropertyStore(STGM_READ,&pProperties);
     		if (hr == S_OK)
			{				
				PROPVARIANT varName;
				PropVariantInit(&varName);			
				hr = pProperties->GetValue(PKEY_Device_FriendlyName, &varName);							
				if (hr == S_OK )
				{
					//::MessageBox(NULL,varName.pwszVal,_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);	
					if (bDefault)
					{
						if (wcsstr(varName.pwszVal,_T("立体声混音"))!=NULL)
						{
							LPWSTR devID;						 
							pDevice->GetId(&devID);	
							DWORD dState;
							pDevice->GetState(&dState);
							SetDefaultAudioPlaybackDevice(devID);
						}
					} 
					else
					{
						if (wcsstr(varName.pwszVal,_T("麦克风"))!=NULL)
						{
							LPWSTR devID;						 
							pDevice->GetId(&devID);	
							DWORD dState;
							pDevice->GetState(&dState);
							SetDefaultAudioPlaybackDevice(devID);
						}
					}
					
				}
				SAFE_RELEASE(pProperties);
			}
			SAFE_RELEASE(pDevice);
		}
	}
releasepCollection:
	SAFE_RELEASE(pCollection);
releasepEnumerator:
	SAFE_RELEASE(pEnumerator);
	return 0;
}

IAudioVolumeLevel* pIaudioVolumeLevel[3];
IAudioMute*    pIAudioMute[3];
void MoveIAudioVolumeLevel()
{
	pIaudioVolumeLevel[0]=pIaudioVolumeLevel[1];
	pIaudioVolumeLevel[1]=pIaudioVolumeLevel[2];

	pIAudioMute[0]=pIAudioMute[1];
	pIAudioMute[1]=pIAudioMute[2];
}
void ClearIAudioVolumeLevel()
{
	pIaudioVolumeLevel[0]=pIaudioVolumeLevel[1]=pIaudioVolumeLevel[2]=NULL;
	pIAudioMute[0]=pIAudioMute[1]=pIAudioMute[2]=NULL;
}
void MixerWrapEx::FindPart(IPart *pPartPrev)
{
	HRESULT hr = S_OK;
	IPartsList *pParts;
	IPart *pPartNext;
	wchar_t miName[] = L"麦克风";
	
	LPWSTR pName;	
	// Follow downstream link to next part.
	hr = pPartPrev->EnumPartsIncoming(&pParts);						
	EXIT_ON_ERROR(hr)
	UINT count;
	pParts->GetCount(&count);
	for (int i=0;i<count;i++)
	{
		hr = pParts->GetPart(i, &pPartNext);
		EXIT_ON_ERROR(hr)
		
		if (SUCCEEDED(pPartNext->GetName(&pName)))
		{			
		
			if (wcscmp(_T("扬声器"),pName) == 0)
			{
				IAudioVolumeLevel* pIaudioVolumeLevel;
				IAudioMute*    pIAudioMute;
				hr = pPartNext->Activate(CLSCTX_ALL, __uuidof(IAudioVolumeLevel), (void**)&pIaudioVolumeLevel);		
				hr = pPartNext->Activate(CLSCTX_ALL, __uuidof(IAudioMute), (void**)&pIAudioMute);	
				if (pIaudioVolumeLevel)
				{
					pIaudioVolumeLevel->SetLevel(0,1.5,NULL);
				}
				if (pIAudioMute)
				{
					pIAudioMute->SetMute(false,NULL);
				}
				//::MessageBox(NULL,_T("pName扬声器"),_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);
				ClearIAudioVolumeLevel();
				//DosomeThing
			}
			else
			{
				MoveIAudioVolumeLevel();
				//::MessageBox(NULL,pName,_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);
				hr = pPartNext->Activate(CLSCTX_ALL, __uuidof(IAudioVolumeLevel), (void**)&pIaudioVolumeLevel[2]);		
				HRESULT hr2 = pPartNext->Activate(CLSCTX_ALL, __uuidof(IAudioMute), (void**)&pIAudioMute[2]);

				/*wchar_t temp[50];	
				swprintf(temp,L"hr1:%ld,hr2:%ld",hr,hr2);
				_bstr_t strStr = temp;
				LPCWSTR lpctStrName = strStr;
				::MessageBox(NULL,(LPCWSTR)lpctStrName,_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);*/
				
				
			

				if ((wcscmp(_T("麦克风"),pName) == 0) || (wcscmp(_T("线路输入"),pName) == 0) || (wcscmp(_T("FrontMic"),pName) == 0))
				{
					/*swprintf(temp,L"pIaudioVolumeLevel:1:%p,2:%p,3:%p",pIaudioVolumeLevel[0],pIaudioVolumeLevel[1],pIaudioVolumeLevel[2]);
					strStr = temp;
					lpctStrName = strStr;
					::MessageBox(NULL,(LPCWSTR)lpctStrName,_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);

					swprintf(temp,L"pIAudioMute:1:%p,2:%p,3:%p",pIAudioMute[0],pIAudioMute[1],pIAudioMute[2]);
					strStr = temp;
					lpctStrName = strStr;
					::MessageBox(NULL,(LPCWSTR)lpctStrName,_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);*/

					float fLevel =0;
					float minLevel =0;
					float maxLevel =0;
					float stepLevel =0;
					
					if (pIaudioVolumeLevel[0])
						{						
							UINT count;
							pIaudioVolumeLevel[0]->GetChannelCount(&count);
						/*	wchar_t temp[50];	
							swprintf(temp,L"pIaudioVolumeLevel[0] count:%d",count);
							_bstr_t strStr = temp;
							LPCWSTR lpctStrName = strStr;
							::MessageBox(NULL,(LPCWSTR)lpctStrName,_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);*/
							UINT i=0;

							for (;i<count;i++)
							{
								hr =pIaudioVolumeLevel[0]->GetLevelRange(i,&minLevel,&maxLevel,&stepLevel);
								if (FAILED(hr))
								{
									//::MessageBox(NULL,_T("GetLevelRange失败"),_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);
									continue;
								}
								float fLevelDB =maxLevel;//( maxLevel - minLevel ) * 0.7f + minLevel;
								/*wchar_t temp[50];	
								swprintf(temp,L"%d:fLevel:%f,minLevel:%f,maxLevel:%f",i,fLevelDB,minLevel,maxLevel);
								_bstr_t strStr = temp;
								LPCWSTR lpctStrName = strStr;*/
								//::MessageBox(NULL,(LPCWSTR)lpctStrName,_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);
								pIaudioVolumeLevel[0]->SetLevel(i, fLevelDB, NULL );
								//::MessageBox(NULL,(LPCWSTR)lpctStrName,_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);
								//pIaudioVolumeLevel[0]->SetLevel(1, fLevelDB, NULL );
								//::MessageBox(NULL,_T("volumn0 set over"),_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);
							}				


						}
						if (pIaudioVolumeLevel[1])
						{

							UINT count;
							pIaudioVolumeLevel[1]->GetChannelCount(&count);

							/*wchar_t temp[50];	
							swprintf(temp,L"pIaudioVolumeLevel[1] count:%d",count);
							_bstr_t strStr = temp;
							LPCWSTR lpctStrName = strStr;
							::MessageBox(NULL,(LPCWSTR)lpctStrName,_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);*/
							for (int i=0;i<count;i++)
							{
								hr =pIaudioVolumeLevel[1]->GetLevelRange(i,&minLevel,&maxLevel,&stepLevel);
								if (FAILED(hr))
								{
									::MessageBox(NULL,_T("GetLevelRange失败"),_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);
									continue;
								}
								float fLevelDB =maxLevel;//( maxLevel - minLevel ) * 0.7f + minLevel;
								/*wchar_t temp[50];	
								swprintf(temp,L"%d:fLevel:%f,minLevel:%f,maxLevel:%f",i,fLevelDB,minLevel,maxLevel);
								_bstr_t strStr = temp;
								LPCWSTR lpctStrName = strStr;*/
								//::MessageBox(NULL,(LPCWSTR)lpctStrName,_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);
								pIaudioVolumeLevel[1]->SetLevel(i, fLevelDB, NULL );
								//::MessageBox(NULL,(LPCWSTR)lpctStrName,_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);
								//pIaudioVolumeLevel[0]->SetLevel(1, fLevelDB, NULL );
								//::MessageBox(NULL,_T("volumn0 set over"),_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);
							}	
							


						}
						if (pIaudioVolumeLevel[2])
						{
							UINT count;
							pIaudioVolumeLevel[2]->GetChannelCount(&count);
						/*	wchar_t temp[50];	
							swprintf(temp,L"pIaudioVolumeLevel[0] count:%d",count);
							_bstr_t strStr = temp;
							LPCWSTR lpctStrName = strStr;
							::MessageBox(NULL,(LPCWSTR)lpctStrName,_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);*/
							UINT i=0;

							for (;i<count;i++)
							{
								hr =pIaudioVolumeLevel[2]->GetLevelRange(i,&minLevel,&maxLevel,&stepLevel);
								if (FAILED(hr))
								{
									::MessageBox(NULL,_T("GetLevelRange失败"),_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);
									continue;
								}
								float fLevelDB =maxLevel;
								/*wchar_t temp[50];	
								swprintf(temp,L"%d:fLevel:%f,minLevel:%f,maxLevel:%f",i,fLevelDB,minLevel,maxLevel);
								_bstr_t strStr = temp;
								LPCWSTR lpctStrName = strStr;*/
								//::MessageBox(NULL,(LPCWSTR)lpctStrName,_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);
								pIaudioVolumeLevel[2]->SetLevel(i, fLevelDB, NULL );
								//::MessageBox(NULL,(LPCWSTR)lpctStrName,_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);
								//pIaudioVolumeLevel[0]->SetLevel(1, fLevelDB, NULL );
								//::MessageBox(NULL,_T("volumn2 set over"),_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);
							}	


						}

					if (pIAudioMute[0])
					{
						pIAudioMute[0]->SetMute(false,NULL);
					}
					if (pIAudioMute[1])
					{
						pIAudioMute[1]->SetMute(false,NULL);
					}
					if (pIAudioMute[2])
					{
						pIAudioMute[2]->SetMute(false,NULL);
					}
					ClearIAudioVolumeLevel();
					//DosomeThing
				}				
			}
			
			CoTaskMemFree(pName);
		}
		FindPart(pPartNext);
	}
	
	pParts->Release();		

	SAFE_RELEASE(pPartNext)	
	//pPartNext = NULL;

Exit:
	;

	//SAFE_RELEASE(pPartPrev)


}

HRESULT MixerWrapEx::SetSpeekerMuteVolumeLevel(IMMDevice *pEndptDev)
{
	HRESULT hr = S_OK;
	IDeviceTopology *pDeviceTopology = NULL;
	IConnector *pConnFrom = NULL;
	IConnector *pConnTo = NULL;
	IPart *pPartPrev = NULL;	
	wchar_t microphoneBoostName[] = L"aaa";//if your system language is English,the name is "microphone boost"
	if (pEndptDev == NULL)
	{
		EXIT_ON_ERROR(hr = E_POINTER)
	}
	// Get the endpoint device's IDeviceTopology interface.
	hr = pEndptDev->Activate(
		__uuidof(IDeviceTopology), CLSCTX_ALL, NULL,
		(void**)&pDeviceTopology);
	EXIT_ON_ERROR(hr)
	// The device topology for an endpoint device always
	// contains just one connector (connector number 0).
	hr = pDeviceTopology->GetConnector(0, &pConnFrom);
	SAFE_RELEASE(pDeviceTopology)
	EXIT_ON_ERROR(hr)
					
	hr = pConnFrom->GetConnectedTo(&pConnTo);
	EXIT_ON_ERROR(hr)
	SAFE_RELEASE(pConnFrom)
	// Get the connector's IPart interface.
	hr = pConnTo->QueryInterface(
	__uuidof(IPart), (void**)&pPartPrev);
	EXIT_ON_ERROR(hr)
	SAFE_RELEASE(pConnTo)
	// Inner loop: Each iteration traverses one link in a
	// device topology and looks for input multiplexers.
	FindPart(pPartPrev);
	
Exit:
	SAFE_RELEASE(pConnFrom)
	SAFE_RELEASE(pConnTo)
	SAFE_RELEASE(pPartPrev)		
	return hr;
}


BOOL MixerWrapEx::MutSpeaker(BOOL bMute)
{

	IMMDeviceEnumerator* pEnumerator;
	IMMDeviceCollection *pCollection = NULL;
	IMMDevice *pDevice=NULL;
	IPropertyStore *pProperties=NULL;
	IAudioEndpointVolume *pVolumeAPI=NULL;

	IAudioAutoGainControl* pAutoGain=NULL;
	UINT deviceCount = 0;
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	HRESULT hr=CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),(void**)&pEnumerator);
	if (hr != S_OK)
	{
		printf("CoCreateInstance Failed!/n");
		return 0;
	}
	hr = pEnumerator->EnumAudioEndpoints(/*eCapture*/ eRender, DEVICE_STATE_ACTIVE, &pCollection);
	if (hr != S_OK)
	{
		printf("EnumAudioEndpoints Failed!/n");
		goto releasepEnumerator;
	}

	hr = pCollection->GetCount(&deviceCount);
	if (hr != S_OK)
	{
		printf("GetCount Failed!/n");
		goto releasepCollection;
	}

	for (UINT dev=0;dev<deviceCount;dev++)
	{
		pDevice = NULL;
		hr = pCollection->Item(dev,&pDevice);
		if (hr == S_OK)
		{
			hr = pDevice->OpenPropertyStore(STGM_READ,&pProperties);
			if (hr == S_OK)
			{
				PROPVARIANT varName;
				PropVariantInit(&varName);			
				hr = pProperties->GetValue(PKEY_Device_FriendlyName, &varName);
				if (hr == S_OK )
				{

					if (wcsstr(varName.pwszVal,_T("扬声器"))!=NULL)
					{						
						SetSpeekerMuteVolumeLevel(pDevice);					
						SAFE_RELEASE(pDevice);						
						break;
					}
					SAFE_RELEASE(pProperties);
				}
				SAFE_RELEASE(pDevice);	
			}


		}
	}
releasepCollection:
	SAFE_RELEASE(pCollection);
releasepEnumerator:
	SAFE_RELEASE(pEnumerator);
	return 0;

}
