// MMTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <Functiondiscoverykeys_devpkey.h>
#include "Audioclient.h"

#pragma comment(lib, "uuid.lib") 


#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

//得到设备硬件ID (设备管理器可以看到的硬件ID)
bool GetDeviceDsc(IMMDevice *pDevice,wchar_t* DeviceDsc)  
{
    HRESULT hr;
    IPropertyStore *pStore;
    hr = pDevice->OpenPropertyStore(STGM_READ, &pStore);
    if (SUCCEEDED(hr))
    {
        PROPERTYKEY Drvidkey ={0xb3f8fa53, 0x0004, 0x438e, 0x90, 0x03, 0x51, 0xa4, 0x6e, 0x13, 0x9b, 0xfc, 2};
        PROPVARIANT pDrvidkey;
        PropVariantInit(&pDrvidkey);
        hr = pStore->GetValue(Drvidkey , &pDrvidkey);  
        if (SUCCEEDED(hr))
        {
            wcscpy(DeviceDsc,pDrvidkey.pwszVal);
            PropVariantClear(&pDrvidkey);
            pStore->Release();
            return true;
        }
        pStore->Release();
    }
    return false;
}



bool VerifyDev(IMMDevice *pDevice,EDataFlow dataFlow)
{
    wchar_t DeviceDsc[255]; 
    if (GetDeviceDsc(pDevice,DeviceDsc)) 
    {
        // 这里省略判断具体设备的 匹配硬件　如 HDAUDIO\FUNC_01&VEN_10EC&DEV_0888&SUBSYS_14627514&REV_1000
        return true;

    }
    return false;
}

int GetDevicePlayVol(void)
{   
    IMMDeviceEnumerator* pEnumerator;  
    IMMDeviceCollection* pCollection = NULL;  
    IMMDevice *pDevice = NULL;  
    IAudioEndpointVolume *pVolumeAPI=NULL;    
    UINT deviceCount = 0;  
    HRESULT hr;
    float fVolume = -1;  
    
    CoInitializeEx( NULL , COINIT_MULTITHREADED );  
    //实例化 MMDeviceEnumerator 枚举器
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),(void**)&pEnumerator);  
    if (hr != S_OK)    
    {    
        goto FreeEnumerator;
    }    
    // 枚举 设备到设备容器 eRander：放音设备，DEVICE_STATE_ACTIVE 为当前已激活的设备，禁用和无连接的用其他状态参数
    hr = pEnumerator->EnumAudioEndpoints( eRender , DEVICE_STATEMASK_ALL , &pCollection );  
    if (hr != S_OK)    
    {       
        goto FreeCollection; 
    }    
    // 设备容器里的总数
    hr = pCollection->GetCount(&deviceCount);    
    if (hr != S_OK)    
    {    
        goto FreeCollection;
    }    

    for (UINT dev=0; dev<deviceCount; dev++)    
    {    
        pDevice = NULL;    
        hr = pCollection->Item(dev,&pDevice);    
        if (hr == S_OK)    
        {
            if (VerifyDev(pDevice,eRender)) 
            {    // 用 pDevice 的 Activate 方法初始一个 IAudioEndpointVolume 接口
                hr = pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void **)(&pVolumeAPI));    
                // 使用 IAudioEndpintVolume 的方法获取音量，设置音量，设置静音等 
                hr = pVolumeAPI->GetMasterVolumeLevelScalar(&fVolume);
                break;
            }
        }    
    }  

FreeCollection:
    SAFE_RELEASE(pCollection);
FreeEnumerator:
    SAFE_RELEASE(pEnumerator);
    CoUninitialize();
    if (fVolume > 0) 
      return fVolume*100; 
    else
      return fVolume;
}
bool ChangeVolume(double nVolume,bool bScalar)
{
 
    HRESULT hr=NULL;
    bool decibels = false;
    bool scalar = false;
    double newVolume=nVolume;
 
    CoInitialize(NULL);
    IMMDeviceEnumerator *deviceEnumerator = NULL;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, 
                          __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
    IMMDevice *defaultDevice = NULL;
 
    hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
    deviceEnumerator->Release();
    deviceEnumerator = NULL;
 
    IAudioEndpointVolume *endpointVolume = NULL;
    hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), 
         CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
    defaultDevice->Release();
    defaultDevice = NULL;
 
    // -------------------------
    float currentVolume = 0;
    endpointVolume->GetMasterVolumeLevel(&currentVolume);
    //printf("Current volume in dB is: %f\n", currentVolume);

    hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
    //CString strCur=L"";
    //strCur.Format(L"%f",currentVolume);
    //AfxMessageBox(strCur);

    // printf("Current volume as a scalar is: %f\n", currentVolume);
    if (bScalar==false)
    {
        hr = endpointVolume->SetMasterVolumeLevel((float)newVolume, NULL);
    }
    else if (bScalar==true)
    {
        hr = endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);
    }
    endpointVolume->Release();
 
    CoUninitialize();
 
    return FALSE;
}
bool GetVolumnVista(UINT& uVol)        
{
     IMMDeviceEnumerator* m_pEnumerator = NULL;
     HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&m_pEnumerator);
     if (FAILED(hr)) return FALSE;
     IMMDevice* m_pDeviceOut = NULL;
     hr = m_pEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &m_pDeviceOut);
     if (FAILED(hr)) return FALSE;
     IAudioEndpointVolume* m_AudioEndpointVolume = NULL;
     hr = m_pDeviceOut->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&m_AudioEndpointVolume);
     if (FAILED(hr)) return FALSE;
     IAudioClient* pAudioClient = NULL;
     hr = m_pDeviceOut->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&pAudioClient);
     if (FAILED(hr)) return FALSE;
     UINT uStep = 0;
     UINT uStepCount = 0;
     hr = m_AudioEndpointVolume->GetVolumeStepInfo(&uStep, &uStepCount);
     if (FAILED(hr)) return FALSE;
     uVol = (uStep * 100) / uStepCount;
     return TRUE;
}

bool GetVolumn(UINT& uVol) 
{ 
     CoInitialize(NULL);
     GetVolumnVista(uVol);
     CoUninitialize();
	 return true;
}
void SetVolumnVista(UINT nInputStep)
{
     IMMDeviceEnumerator* m_pEnumerator = NULL;
     HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&m_pEnumerator);
     if (FAILED(hr)) return;
     IMMDevice* m_pDeviceOut = NULL;
     hr = m_pEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &m_pDeviceOut);
     if (FAILED(hr)) return;
     IAudioEndpointVolume* m_AudioEndpointVolume = NULL;
     hr = m_pDeviceOut->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&m_AudioEndpointVolume);
     if (FAILED(hr)) return;
     IAudioClient* pAudioClient = NULL;
     hr = m_pDeviceOut->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&pAudioClient);
     if (FAILED(hr)) return;
     UINT uStep = 0;
     UINT uStepCount = 0;
     hr = m_AudioEndpointVolume->GetVolumeStepInfo(&uStep, &uStepCount);
     if (FAILED(hr)) return;
     UINT uVol = (nInputStep * uStepCount) / 100;
     BOOL bUp = uVol < uStep;
     UINT uLess = bUp ? (uStep - uVol) : (uVol - uStep);
     for (UINT i = 0; i < uLess; i++)
     {
         if (!bUp)
         {
             hr = m_AudioEndpointVolume->VolumeStepUp(&GUID_NULL);
         }
         else
         {
             hr = m_AudioEndpointVolume->VolumeStepDown(&GUID_NULL);
         }
         if (FAILED(hr)) return;
     }
} 
void SetVolumn(UINT nInputSetp)
{
     CoInitialize(NULL);
     SetVolumnVista(nInputSetp);
     CoUninitialize();
}

//-----------------------------------------------------------
// The input argument to this function is a pointer to the
// IMMDevice interface for a capture endpoint device. The
// function traverses the data path that extends from the
// endpoint device to the system bus (for example, PCI)
// or external bus (USB). If the function discovers microphone
// boost,it active the IAudioVolumeLevel and return
//-----------------------------------------------------------
#define EXIT_ON_ERROR(hres)  	if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  	if ((punk) != NULL)  { (punk)->Release(); (punk) = NULL; }
//input argument : pEndptDev
//output argument : ppVolumeLevel
HRESULT getMicrophoneBoostVolumeLevel(IMMDevice *pEndptDev, IAudioVolumeLevel** ppVolumeLevel)
{
	HRESULT hr = S_OK;
	DataFlow flow;
	IDeviceTopology *pDeviceTopology = NULL;
	IConnector *pConnFrom = NULL;
	IConnector *pConnTo = NULL;
	IPart *pPartPrev = NULL;
	IPart *pPartNext = NULL;
	*ppVolumeLevel = NULL;
	wchar_t microphoneBoostName[] = L"aaa麦克风加强";//if your system language is English,the name is "microphone boost"
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
		// Make sure that this is a capture device.
		hr = pConnFrom->GetDataFlow(&flow);
	EXIT_ON_ERROR(hr)
		if (flow != Out)
		{
			// Error -- this is a rendering device.
			//EXIT_ON_ERROR(hr = AUDCLNT_E_WRONG_ENDPOINT_TYPE)
		}
		// Outer loop: Each iteration traverses the data path
		// through a device topology starting at the input
		// connector and ending at the output connector.
		while (TRUE)
		{
			BOOL bConnected;
			hr = pConnFrom->IsConnected(&bConnected);
			EXIT_ON_ERROR(hr)
				// Does this connector connect to another device?
				if (bConnected == FALSE)
				{
					// This is the end of the data path that
					// stretches from the endpoint device to the
					// system bus or external bus. Verify that
					// the connection type is Software_IO.
					ConnectorType  connType;
					hr = pConnFrom->GetType(&connType);
					EXIT_ON_ERROR(hr)
						if (connType == Software_IO)
						{
							break;  // finished
						}
						EXIT_ON_ERROR(hr = E_FAIL)
				}
				// Get the connector in the next device topology,
				// which lies on the other side of the connection.
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
					while (TRUE)
					{
						PartType parttype;
						IPartsList *pParts;
						// Follow downstream link to next part.
						hr = pPartPrev->EnumPartsOutgoing(&pParts);
						EXIT_ON_ERROR(hr)
							hr = pParts->GetPart(0, &pPartNext);
						pParts->Release();
						EXIT_ON_ERROR(hr)
							hr = pPartNext->GetPartType(&parttype);
						EXIT_ON_ERROR(hr)
							
							LPWSTR pName;
							if (SUCCEEDED(pPartNext->GetName(&pName)))
							{
								// Failure of the following call means only that
								// the part is not a boost (micrphone boost).
								if (wcscmp(microphoneBoostName,pName) == 0)
								{
									//get IAudioVolumeLevel to control volume 
									hr = pPartNext->Activate(CLSCTX_ALL, __uuidof(IAudioVolumeLevel), (void**)ppVolumeLevel);
									goto Exit;
								}
								CoTaskMemFree(pName);
							}
							GUID subType;
							pPartNext->GetSubType(&subType);
							if (parttype == Connector)
							{
								// We've reached the output connector that
								// lies at the end of this device topology.
								hr = pPartNext->QueryInterface(
									__uuidof(IConnector),
									(void**)&pConnFrom);
								EXIT_ON_ERROR(hr)
								SAFE_RELEASE(pPartPrev)
								SAFE_RELEASE(pPartNext)
									break;
							}
							SAFE_RELEASE(pPartPrev)
							pPartPrev = pPartNext;
							pPartNext = NULL;
					}
		}
Exit:
		SAFE_RELEASE(pConnFrom)
		SAFE_RELEASE(pConnTo)
		SAFE_RELEASE(pPartPrev)
		SAFE_RELEASE(pPartNext)
		return hr;
}





HRESULT getMicrophoneBoostVolumeLevelTest(IMMDevice *pEndptDev, IAudioVolumeLevel** ppVolumeLevel)
{
	HRESULT hr = S_OK;
	DataFlow flow;
	IDeviceTopology *pDeviceTopology = NULL;
	IConnector *pConnFrom = NULL;
	IConnector *pConnTo = NULL;
	IPart *pPartPrev = NULL;
	IPart *pPartNext = NULL;
	*ppVolumeLevel = NULL;
	wchar_t microphoneBoostName[] = L"麦克风";//if your system language is English,the name is "microphone boost"
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
		// Make sure that this is a capture device.
		hr = pConnFrom->GetDataFlow(&flow);
	EXIT_ON_ERROR(hr)
		if (flow != Out)
		{
			// Error -- this is a rendering device.
			//EXIT_ON_ERROR(hr = AUDCLNT_E_WRONG_ENDPOINT_TYPE)
		}
		// Outer loop: Each iteration traverses the data path
		// through a device topology starting at the input
		// connector and ending at the output connector.
		while (TRUE)
		{
			BOOL bConnected;
			hr = pConnFrom->IsConnected(&bConnected);
			EXIT_ON_ERROR(hr)
				// Does this connector connect to another device?
				if (bConnected == FALSE)
				{
					// This is the end of the data path that
					// stretches from the endpoint device to the
					// system bus or external bus. Verify that
					// the connection type is Software_IO.
					ConnectorType  connType;
					hr = pConnFrom->GetType(&connType);
					EXIT_ON_ERROR(hr)
						if (connType == Software_IO)
						{
							break;  // finished
						}
						EXIT_ON_ERROR(hr = E_FAIL)
				}
				// Get the connector in the next device topology,
				// which lies on the other side of the connection.
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
					while (TRUE)
					{
						PartType parttype;
						IPartsList *pParts;
						// Follow downstream link to next part.
						hr = pPartPrev->EnumPartsIncoming(&pParts);
						if (pParts==NULL)
						{
							hr = pPartPrev->EnumPartsOutgoing(&pParts);							
						}
						EXIT_ON_ERROR(hr)
							hr = pParts->GetPart(0, &pPartNext);
						pParts->Release();
						EXIT_ON_ERROR(hr)
							hr = pPartNext->GetPartType(&parttype);
						EXIT_ON_ERROR(hr)

							LPWSTR pName;
						if (SUCCEEDED(pPartNext->GetName(&pName)))
						{
							// Failure of the following call means only that
							// the part is not a boost (micrphone boost).
							if (wcscmp(microphoneBoostName,pName) == 0)
							{
								//get IAudioVolumeLevel to control volume 
								hr = pPartNext->Activate(CLSCTX_ALL, __uuidof(IAudioVolumeLevel), (void**)ppVolumeLevel);
								static int i=0;
								
								if (i==1)
								{
									goto Exit;
								}			
								i++;
							}
							CoTaskMemFree(pName);
						}
						GUID subType;
						pPartNext->GetSubType(&subType);
						if (parttype == Connector)
						{
							// We've reached the output connector that
							// lies at the end of this device topology.
							hr = pPartNext->QueryInterface(
								__uuidof(IConnector),
								(void**)&pConnFrom);
							EXIT_ON_ERROR(hr)
								SAFE_RELEASE(pPartPrev)
								SAFE_RELEASE(pPartNext)
								break;
						}
						SAFE_RELEASE(pPartPrev)
							pPartPrev = pPartNext;
						pPartNext = NULL;
					}
		}
Exit:
		SAFE_RELEASE(pConnFrom)
			SAFE_RELEASE(pConnTo)
			SAFE_RELEASE(pPartPrev)
			SAFE_RELEASE(pPartNext)
			return hr;
}





BOOL MuteDevice(BOOL bMute)

{
	IMMDeviceEnumerator* pEnumerator;
	IMMDeviceCollection *pCollection = NULL;
	IMMDevice *pDevice=NULL;
	IPropertyStore *pProperties=NULL;
	IAudioEndpointVolume *pVolumeAPI=NULL;
	UINT deviceCount = 0;

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	HRESULT hr=CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),(void**)&pEnumerator);
	if (hr != S_OK)
	{
		printf("CoCreateInstance Failed!/n");
		return 0;
	}
	hr = pEnumerator->EnumAudioEndpoints(eCapture /*eRender*/, DEVICE_STATE_ACTIVE, &pCollection);
	//hr = pEnumerator->EnumAudioEndpoints( eRender , DEVICE_STATEMASK_ALL , &pCollection );  
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
			hr = pDevice->OpenPropertyStore(STGM_WRITE,&pProperties);
			if (hr == S_OK)
			{				
				PROPVARIANT varName;
				PropVariantInit(&varName);

				//hr = pProperties->GetValue(PKEY_Device_BusTypeGuid, &varName);
				hr = pProperties->GetValue(PKEY_Device_FriendlyName, &varName);
				if (hr == S_OK )
				{
					//if (wcsstr(varName.pwszVal,_T("Line I"))!=NULL)
					//if (wcsstr(varName.pwszVal,_T("线路输入"))!=NULL)
					if (wcsstr(varName.pwszVal,_T("麦克风"))!=NULL)
					{
						//麦克风静音////////////////////////////////////////////////////////////////////////
						/*hr=pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void **)(&pVolumeAPI));
						if (hr==S_OK)
						{								

							hr = pVolumeAPI->SetMute(bMute,NULL);
							if (hr == S_OK)
							{

								if (bMute)
									printf("Line In Muted./n");
								else
									printf("Line In Activated./n");
							}
							SAFE_RELEASE(pVolumeAPI);

						}*/
						//////////////////////////////////////////////////////////////////////////

						//立体声混音////////////////////////////////////////////////////////////////////////
						hr=pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void **)(&pVolumeAPI));
						if (hr==S_OK)
						{					
							float fVolume = 0.0;
							UINT iChannelSum=0;

							hr = pVolumeAPI->GetChannelCount(&iChannelSum);
							if (SUCCEEDED(hr) &&(iChannelSum>=2))
							{
								for (int i=0;i<iChannelSum;i++)
								{
									pVolumeAPI->SetMute(FALSE,NULL);
									pVolumeAPI->GetChannelVolumeLevelScalar(i,&fVolume);
									BOOL bUp =TRUE;
									if(bUp)
									{
										fVolume += 0.1;
										if(fVolume>1.0)
										{
											fVolume = 1.0;
										}
									}
									else
									{
										fVolume -= 0.1;
										if(fVolume<0)
										{
											fVolume = 0;
										}
									}
									pVolumeAPI->SetChannelVolumeLevelScalar(i,fVolume,NULL);
								}
							}
							SAFE_RELEASE(pVolumeAPI);

						}
						//////////////////////////////////////////////////////////////////////////     

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



// set fVolume with a value from 0 to 1.

BOOL SetVolume(float fVolume = 0)
{
	IMMDeviceEnumerator* pEnumerator;
	IMMDeviceCollection *pCollection = NULL;
	IMMDevice *pDevice=NULL;	IPropertyStore *pProperties=NULL;
	IAudioEndpointVolume *pVolumeAPI=NULL;
	UINT deviceCount = 0;

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	HRESULT hr=CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),(void**)&pEnumerator);
	if (hr != S_OK)
	{
		printf("CoCreateInstance Failed!/n");
		return 0;
	}
	hr = pEnumerator->EnumAudioEndpoints(eCapture /*eRender*/, DEVICE_STATE_ACTIVE, &pCollection);
	
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
				hr=pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void **)(&pVolumeAPI));
				if (hr==S_OK)
				{
					pVolumeAPI->SetMasterVolumeLevelScalar(fVolume,NULL);
				}
				PROPVARIANT varName;
				PropVariantInit(&varName);
				//hr = pProperties->GetValue(PKEY_Device_BusTypeGuid, &varName);
				hr = pProperties->GetValue(PKEY_Device_FriendlyName, &varName);
				if (hr == S_OK )
				{
					//if (wcsstr(varName.pwszVal,_T("Line In"))!=NULL)

					//if (wcsstr(varName.pwszVal,_T("线路输入"))!=NULL)

					//if (wcsstr(varName.pwszVal,_T("扬声器"))!=NULL)

					{
						hr=pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void **)(&pVolumeAPI));
						if (hr==S_OK)
						{
							UINT iChannelSum=0;
							hr = pVolumeAPI->GetChannelCount(&iChannelSum);
							if (SUCCEEDED(hr) &&(iChannelSum>=2))
							{
								for (int i=0;i<iChannelSum;i++)
								{
									pVolumeAPI->SetChannelVolumeLevelScalar(i,fVolume,NULL);
								}
							}
							SAFE_RELEASE(pVolumeAPI);
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




BOOL AdjustVolume(BOOL bUp)

{

	IMMDeviceEnumerator* pEnumerator;
	IMMDeviceCollection *pCollection = NULL;
	IMMDevice *pDevice=NULL;
	IPropertyStore *pProperties=NULL;
	IAudioEndpointVolume *pVolumeAPI=NULL;
	UINT deviceCount = 0;
	float fVolume = 0.0;
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	HRESULT hr=CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),(void**)&pEnumerator);
	if (hr != S_OK)
	{
		printf("CoCreateInstance Failed!/n");
		return 0;
	}
	hr = pEnumerator->EnumAudioEndpoints(eCapture /*eRender*/, DEVICE_STATE_ACTIVE, &pCollection);
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
				hr=pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void **)(&pVolumeAPI));
				if (hr==S_OK)
				{
					//pVolumeAPI->SetMute(FALSE,NULL);
					pVolumeAPI->GetMasterVolumeLevelScalar(&fVolume);
					if(bUp)
					{

						fVolume += 0.1;
						if(fVolume>1.0)

						{
							fVolume = 1.0;
						}
					}
					else
					{
						fVolume -= 0.1;
						if(fVolume<0)
						{
							fVolume = 0;
						}
					}
					pVolumeAPI->SetMasterVolumeLevelScalar(fVolume,NULL);
				}
				PROPVARIANT varName;
				PropVariantInit(&varName);
				//hr = pProperties->GetValue(PKEY_Device_BusTypeGuid, &varName);
				hr = pProperties->GetValue(PKEY_Device_FriendlyName, &varName);
				if (hr == S_OK )
				{
					//if (wcsstr(varName.pwszVal,_T("Line In"))!=NULL)
					//if (wcsstr(varName.pwszVal,_T("线路输入"))!=NULL)
					//if (wcsstr(varName.pwszVal,_T("扬声器"))!=NULL)
					{
						hr=pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void **)(&pVolumeAPI));
						if (hr==S_OK)
						{
							UINT iChannelSum=0;
							hr = pVolumeAPI->GetChannelCount(&iChannelSum);
							if (SUCCEEDED(hr) &&(iChannelSum>=2))
							{
								for (int i=0;i<iChannelSum;i++)
								{
									pVolumeAPI->SetMute(FALSE,NULL);
									pVolumeAPI->GetChannelVolumeLevelScalar(i,&fVolume);
									if(bUp)
									{
										fVolume += 0.1;
										if(fVolume>1.0)
										{
											fVolume = 1.0;
										}
									}
									else
									{
										fVolume -= 0.1;
										if(fVolume<0)
										{
											fVolume = 0;
										}
									}
									pVolumeAPI->SetChannelVolumeLevelScalar(i,fVolume,NULL);
								}
							}
							SAFE_RELEASE(pVolumeAPI);
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

void SelectInput()
{
    const IID IID_IDeviceTopology = __uuidof(IDeviceTopology);
	const IID IID_IPart = __uuidof(IPart);
	const IID IID_IAudioAutoGainControl = __uuidof(IAudioAutoGainControl);

	HRESULT hr = S_OK;
	CoInitialize(NULL);

	IMMDeviceEnumerator *deviceEnumerator = NULL;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);

	IMMDevice *pEndptDev = NULL;

	hr = deviceEnumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &pEndptDev);
	deviceEnumerator->Release();
	deviceEnumerator = NULL;


	IDeviceTopology *pDevTopoEndpt = NULL;
	IConnector *pConnEndpt = NULL;
	IConnector *pConnHWDev = NULL;
	IPart *pPartConn = NULL;
	IAudioAutoGainControl *pAGC = NULL;
	IControlInterface *pControl = NULL;
	UINT pCount = 0;
	LPCGUID pIID =0 ;

	GUID m_guidCaptureDevice;

	// Get the endpoint device's IDeviceTopology interface.
	hr = pEndptDev->Activate(IID_IDeviceTopology, CLSCTX_ALL, NULL, (void**)&pDevTopoEndpt);

	// The device topology for an endpoint device always
	// contains just one connector (connector number 0).
	hr = pDevTopoEndpt->GetConnector(0, &pConnEndpt);

	// Use the connector in the endpoint device to get the
	// connector in the adapter device.
	hr = pConnEndpt->GetConnectedTo(&pConnHWDev);

	// Query the connector in the adapter device for
	// its IPart interface.
	hr = pConnHWDev->QueryInterface(IID_IPart, (void**)&pPartConn);

	// Use the connector's IPart interface to get the
	// IDeviceTopology interface for the adapter device.
	hr = pPartConn->Activate(CLSCTX_ALL, IID_IAudioAutoGainControl, (void**)&pAGC);
	hr = pPartConn->GetControlInterfaceCount(&pCount);
	hr = pPartConn->GetControlInterface(pCount - 1, &pControl);
	hr = pControl->GetIID((LPGUID)&m_guidCaptureDevice);

	//BOOL bEnabled = false;
	hr = pAGC->SetEnabled(true,(LPGUID) &m_guidCaptureDevice);
	

}



//OK 
BOOL MutSpeaker(BOOL bMute)
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
	//hr = pEnumerator->EnumAudioEndpoints( eRender , DEVICE_STATEMASK_ALL , &pCollection );  
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
	printf("deviceCount:%d!/n",deviceCount);

	for (UINT dev=0;dev<deviceCount;dev++)
	{
		pDevice = NULL;
		hr = pCollection->Item(dev,&pDevice);
		if (hr == S_OK)
		{
			hr = pDevice->OpenPropertyStore(STGM_WRITE,&pProperties);
			if (hr == S_OK)
			{				
				PROPVARIANT varName;
				PropVariantInit(&varName);			
				hr = pProperties->GetValue(PKEY_Device_FriendlyName, &varName);
				printf("varName.pwszVal:%s!/n",varName.pwszVal);
				if (hr == S_OK )
				{
			
					if (wcsstr(varName.pwszVal,_T("扬声器"))!=NULL)
					{

						//hr=pDevice->Activate(__uuidof(IAudioAutoGainControl),CLSCTX_ALL,NULL,(void **)(&pAutoGain));
						hr=pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void **)(&pVolumeAPI));
						if (hr==S_OK)
						{			
							UINT chanelCount = 0;
							hr =pVolumeAPI->GetChannelCount(&chanelCount);
							for (int i=0;i<chanelCount;i++)
							{
								pVolumeAPI->SetChannelVolumeLevelScalar(i,1.0,NULL);
							}
							hr = pVolumeAPI->SetMute(bMute,NULL);						
							SAFE_RELEASE(pVolumeAPI);
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
HRESULT getSpeekerMuteVolumeLevel(IMMDevice *pEndptDev, IAudioVolumeLevel** ppVolumeLevel)
{
	HRESULT hr = S_OK;
	DataFlow flow;
	IDeviceTopology *pDeviceTopology = NULL;
	IConnector *pConnFrom = NULL;
	IConnector *pConnTo = NULL;
	IPart *pPartPrev = NULL;
	IPart *pPartNext = NULL;
	*ppVolumeLevel = NULL;
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
		// Make sure that this is a capture device.
		hr = pConnFrom->GetDataFlow(&flow);
	EXIT_ON_ERROR(hr)
		if (flow != Out)
		{
			// Error -- this is a rendering device.
			//EXIT_ON_ERROR(hr = AUDCLNT_E_WRONG_ENDPOINT_TYPE)
		}
		// Outer loop: Each iteration traverses the data path
		// through a device topology starting at the input
		// connector and ending at the output connector.
		while (TRUE)
		{
			BOOL bConnected;
			hr = pConnFrom->IsConnected(&bConnected);
			EXIT_ON_ERROR(hr)
				// Does this connector connect to another device?
				if (bConnected == FALSE)
				{
					// This is the end of the data path that
					// stretches from the endpoint device to the
					// system bus or external bus. Verify that
					// the connection type is Software_IO.
					ConnectorType  connType;
					hr = pConnFrom->GetType(&connType);
					EXIT_ON_ERROR(hr)
						if (connType == Software_IO)
						{
							break;  // finished
						}
						EXIT_ON_ERROR(hr = E_FAIL)
				}
				// Get the connector in the next device topology,
				// which lies on the other side of the connection.
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
					while (TRUE)
					{
						PartType parttype;
						IPartsList *pParts;
						// Follow downstream link to next part.
						hr = pPartPrev->EnumPartsOutgoing(&pParts);
						EXIT_ON_ERROR(hr)
							hr = pParts->GetPart(0, &pPartNext);
						pParts->Release();
						EXIT_ON_ERROR(hr)
							hr = pPartNext->GetPartType(&parttype);
						EXIT_ON_ERROR(hr)

							LPWSTR pName;
						if (SUCCEEDED(pPartNext->GetName(&pName)))
						{
							// Failure of the following call means only that
							// the part is not a boost (micrphone boost).
							if (wcscmp(microphoneBoostName,pName) == 0)
							{
								//get IAudioVolumeLevel to control volume 
								hr = pPartNext->Activate(CLSCTX_ALL, __uuidof(IAudioVolumeLevel), (void**)ppVolumeLevel);
								goto Exit;
							}
							CoTaskMemFree(pName);
						}
						GUID subType;
						pPartNext->GetSubType(&subType);
						if (parttype == Connector)
						{
							// We've reached the output connector that
							// lies at the end of this device topology.
							hr = pPartNext->QueryInterface(
								__uuidof(IConnector),
								(void**)&pConnFrom);
							EXIT_ON_ERROR(hr)
								SAFE_RELEASE(pPartPrev)
								SAFE_RELEASE(pPartNext)
								break;
						}
						SAFE_RELEASE(pPartPrev)
							pPartPrev = pPartNext;
						pPartNext = NULL;
					}
		}
Exit:
		SAFE_RELEASE(pConnFrom)
			SAFE_RELEASE(pConnTo)
			SAFE_RELEASE(pPartPrev)
			SAFE_RELEASE(pPartNext)
			return hr;
}



BOOL MutSpeaker1(BOOL bMute)
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
	//hr = pEnumerator->EnumAudioEndpoints( eRender , DEVICE_STATEMASK_ALL , &pCollection );  
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
			hr = pDevice->OpenPropertyStore(STGM_WRITE,&pProperties);
			if (hr == S_OK)
			{
				PROPVARIANT varName;
				PropVariantInit(&varName);			
				hr = pProperties->GetValue(PKEY_Device_FriendlyName, &varName);
				if (hr == S_OK )
				{

					if (wcsstr(varName.pwszVal,_T("扬声器"))!=NULL)
					{
						IAudioVolumeLevel* pIaudioVolumeLevel;
						getSpeekerMuteVolumeLevel(pDevice,&pIaudioVolumeLevel);					
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


HRESULT getSpeekerMute(IMMDevice * m_pDevice,IAudioMute** m_pAudioMute)
{
	try
	{
		HRESULT hr = S_OK;
		DataFlow flow;
		IDeviceTopology *pDeviceTopology = NULL;
		IConnector *pConnFrom = NULL;
		IConnector *pConnTo = NULL;
		IPart *pPartPrev = NULL;
		IPart *pPartNext = NULL;

		WCHAR muteCHName[] = L"静音";
		WCHAR muteENName[] = L"mute";

		if (m_pDevice == NULL)
		{
			return FALSE;
		}

		hr = m_pDevice->Activate(__uuidof(IDeviceTopology), CLSCTX_ALL, NULL, (void**)&pDeviceTopology);
		EXIT_ON_ERROR(hr);

		LPWSTR lpDeviceID;
		pDeviceTopology->GetDeviceId(&lpDeviceID);
		UINT pCount;
		pDeviceTopology->GetConnectorCount(&pCount);

		hr = pDeviceTopology->GetConnector(0, &pConnFrom);
		SAFE_RELEASE(pDeviceTopology);
		EXIT_ON_ERROR(hr);

		hr = pConnFrom->GetDataFlow(&flow);
		EXIT_ON_ERROR(hr);

		//if (flow != Out)
		//{
		//	// Error -- this is a Capture device.
		//	EXIT_ON_ERROR(hr = S_FALSE);
		//}

		while (TRUE)
		{
			BOOL bConnected;
			hr = pConnFrom->IsConnected(&bConnected);
			EXIT_ON_ERROR(hr);

			if (bConnected == FALSE)
			{
				DataFlow dateFlow;
				hr = pConnFrom->GetDataFlow(&dateFlow);

				ConnectorType connType;
				hr = pConnFrom->GetType(&connType);
				EXIT_ON_ERROR(hr);

				if (connType == Software_IO)
				{
					break; // finished
				}

				EXIT_ON_ERROR(hr = E_FAIL);
			}

			hr = pConnFrom->GetConnectedTo(&pConnTo);

			if (pConnTo == NULL)
			{
				::OutputDebugString(_T("##_播放麦克风静音检索_pConnto is null.\n"));
			}

			EXIT_ON_ERROR(hr);
			SAFE_RELEASE(pConnFrom);

			// Get the connector's IPart interface.
			hr = pConnTo->QueryInterface(__uuidof(IPart), (void**)&pPartPrev);
			EXIT_ON_ERROR(hr);
			SAFE_RELEASE(pConnTo);

			while (TRUE)
			{
				PartType parttype;
				IPartsList *pParts = NULL;

				// Follow downstream link to next part.
				hr = pPartPrev->EnumPartsIncoming(&pParts);

				if (pParts == NULL)
				{
					::OutputDebugString(_T("##_## EnumPartsOutgoing partslist is null.\n"));
				}

				EXIT_ON_ERROR(hr);

				hr = pParts->GetPart(0, &pPartNext);
				pParts->Release();
				EXIT_ON_ERROR(hr);

				hr = pPartNext->GetPartType(&parttype);
				EXIT_ON_ERROR(hr);

				LPWSTR pName;

				hr = pPartNext->GetName(&pName);

				if (SUCCEEDED(hr))
				{
					if ((wcsncmp(muteCHName, pName, MAX_PATH) == 0)
						|| wcsncmp(muteENName, pName, MAX_PATH) == 0)
					{
						// 这里想获取麦克风静音对象。

						hr = pPartNext->Activate(CLSCTX_ALL, __uuidof(IAudioMute), (void**)&m_pAudioMute);
                        bool bMute=true;
						if (m_pAudioMute)
						{
							(*m_pAudioMute)->SetMute(bMute, NULL);
							SAFE_RELEASE(*m_pAudioMute);
						}
						CoTaskMemFree(pName);
						goto Exit;
					}

					CoTaskMemFree(pName);
				}

				if (parttype == Connector)
				{
					hr = pPartNext->QueryInterface(__uuidof(IConnector), (void**)&pConnFrom);
					EXIT_ON_ERROR(hr);
					SAFE_RELEASE(pPartPrev);
					SAFE_RELEASE(pPartNext);

					break;
				}

				SAFE_RELEASE(pPartPrev);
				pPartPrev = pPartNext;
				pPartNext = NULL;
			}
		}

Exit:
		SAFE_RELEASE(pConnFrom);
		SAFE_RELEASE(pConnTo);
		SAFE_RELEASE(pPartPrev);
		SAFE_RELEASE(pPartNext);

		return SUCCEEDED(hr);
	}
	catch (...)
	{
		// error
	}

	return FALSE;
}

BOOL MutSpeaker2(BOOL bMute)
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
	hr = pEnumerator->EnumAudioEndpoints(/*eCapture*/ eCapture, DEVICE_STATE_ACTIVE, &pCollection);
	//hr = pEnumerator->EnumAudioEndpoints( eRender , DEVICE_STATEMASK_ALL , &pCollection );  
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
			hr = pDevice->OpenPropertyStore(STGM_WRITE,&pProperties);
			if (hr == S_OK)
			{
				PROPVARIANT varName;
				PropVariantInit(&varName);			
				hr = pProperties->GetValue(PKEY_Device_FriendlyName, &varName);
				if (hr == S_OK )
				{

					/*if (wcsstr(varName.pwszVal,_T("扬声器"))!=NULL)
					{*/
						IAudioVolumeLevel* pIaudioVolumeLevel;
						IAudioMute* pIAudioMute;
						//getSpeekerMute(pDevice,&pIAudioMute);	
						getMicrophoneBoostVolumeLevelTest(pDevice,&pIaudioVolumeLevel);
						if (pIaudioVolumeLevel!=NULL)
						{
							pIaudioVolumeLevel->SetLevel(0,50,NULL);
						}
						
						/*break;
					}*/
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

int _tmain(int argc, _TCHAR* argv[])
{
	MutSpeaker1(false);
	//MutSpeaker(false);
	return 0;
	//OK now////////////////////////////////////////////////////////////////////////
	/*MuteDevice(false);
	HRESULT hr=NULL; 
    CoInitialize(NULL);
    IMMDeviceEnumerator *deviceEnumerator = NULL;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, 
                          __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
    IMMDevice *defaultDevice = NULL; 
    hr = deviceEnumerator->GetDefaultAudioEndpoint(eCapture, eMultimedia, &defaultDevice);
    deviceEnumerator->Release();
    deviceEnumerator = NULL;
	 IAudioVolumeLevel* pIaudioVolumeLevel;
	getMicrophoneBoostVolumeLevel(defaultDevice,&pIaudioVolumeLevel);
	defaultDevice->Release();
    defaultDevice = NULL; 
	pIaudioVolumeLevel->SetLevel(0,30,NULL);
	return 0;*/
	//////////////////////////////////////////////////////////////////////////	
	IMMDeviceEnumerator* pEnumerator;  
    IMMDeviceCollection* pCollection = NULL;  
    IMMDevice *pDevice = NULL;  
   
    UINT deviceCount = 0;  
    //HRESULT hr;
   
    
    CoInitializeEx( NULL , COINIT_MULTITHREADED );  
    //实例化 MMDeviceEnumerator 枚举器
     HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),(void**)&pEnumerator);  
    if (hr != S_OK)    
    {    
        goto FreeEnumerator;
    }    
    // 枚举 设备到设备容器 eRander：放音设备，DEVICE_STATE_ACTIVE 为当前已激活的设备，禁用和无连接的用其他状态参数
    hr = pEnumerator->EnumAudioEndpoints( eRender , DEVICE_STATEMASK_ALL , &pCollection );  
	

    if (hr != S_OK)    
    {       
        goto FreeCollection; 
    }    
    // 设备容器里的总数
    hr = pCollection->GetCount(&deviceCount);    
    if (hr != S_OK)    
    {    
        goto FreeCollection;
    }    

    for (UINT dev=0; dev<deviceCount; dev++)    
    {    
        pDevice = NULL;    
        hr = pCollection->Item(dev,&pDevice);    
        if (hr == S_OK)    
        {

			   LPWSTR wstrID = NULL;
			   hr= pDevice->GetId(&wstrID);
			   if(SUCCEEDED(hr))
			   {
				  // TRACE(wstrID);
				   IPropertyStore* pStore=NULL;
				   hr= pDevice->OpenPropertyStore(STGM_READ,&pStore);
				   if(SUCCEEDED(hr))
				   {
					   PROPERTYKEY temp;
					    hr= pStore->GetAt(0,&temp);

					   PROPVARIANT deviceName;
					   PropVariantInit(&deviceName);
					   hr= pStore->GetValue(temp,&deviceName);
					   if(SUCCEEDED(hr))
					   {
						   LPWSTR  tempVar= deviceName.pwszVal;
						  /* if(tempVar.Find(L"ArcSoft")!=-1)
						   {*/
							   //找到虚拟声卡
							   //set default
							   //IPolicyConfigVista* pPolicyConfig;
							   //ERole reserved = eConsole;
							   //hr= CoCreateInstance(__uuidof(CPolicyConfigVistaClient),NULL,CLSCTX_ALL, __uuidof(IPolicyConfigVista),(LPVOID *)&pPolicyConfig);
							   //if(SUCCEEDED(hr))
							   //{
								  // hr= pPolicyConfig->SetDefaultEndpoint(wstrID,reserved);
								  // if(SUCCEEDED(hr))
								  // {

									 //  pPolicyConfig->Release();
									 //  pStore->Release();

									 //  pDevice->Release();
									 //  pDevices->Release();

									 //  pEnum->Release();

									 //  return 0;

 
								  // }
								  // else

								  // {

									 ////  AfxMessageBox(L"Error");

								  // }

 
							   //}

 
						  // }

						   //deviceName LPWSTR = 0x004a2a28 "Realtek Digital Output (Realtek High DefinitionAudio)"
						   //deviceName LPWSTR = 0x0048f390 "扬声器 (ArcSoft Virtual AudioDevice (Multiple Streams) (WDM))"
						   //deviceName LPWSTR = 0x0048f518 "扬声器 (Realtek High DefinitionAudio)"
					   }
				   }
			   }



            //IAudioVolumeLevel* pIaudioVolumeLevel;			
			//getMicrophoneBoostVolumeLevel(pDevice,&pIaudioVolumeLevel);
        }    
    }




	GetDevicePlayVol();
	UINT t;
	GetVolumn(t);
	t =50;
	SetVolumn(t);
FreeCollection:
    SAFE_RELEASE(pCollection);
FreeEnumerator:
    SAFE_RELEASE(pEnumerator);
    CoUninitialize();
	return 0;
}

