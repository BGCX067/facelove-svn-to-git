// ----------------------------------------------------------------------------
// PolicyConfig.h
// Undocumented COM-interface IPolicyConfig.
// Use for set default audio render endpoint
// @author EreTIk
// ----------------------------------------------------------------------------
#include "windows.h"
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <Functiondiscoverykeys_devpkey.h>

#pragma once

#define SAFE_RELEASE(punk)  \
	if ((punk) != NULL)  \
				{ (punk)->Release(); (punk) = NULL; }

#define EXIT_ON_ERROR(hres)  	if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  	if ((punk) != NULL)  { (punk)->Release(); (punk) = NULL; }

#define DECLSPEC_UUID(x) __declspec(uuid(x))

interface DECLSPEC_UUID("f8679f50-850a-41cf-9c72-430f290290c8")
IPolicyConfig;
class DECLSPEC_UUID("870af99c-171d-4f9e-af0d-e63df40c2bc9")
	CPolicyConfigClient;

interface IPolicyConfig : public IUnknown
{
public:

	virtual HRESULT GetMixFormat(
		PCWSTR,
		WAVEFORMATEX **
		);

	virtual HRESULT STDMETHODCALLTYPE GetDeviceFormat(
		PCWSTR,
		INT,
		WAVEFORMATEX **
		);

	virtual HRESULT STDMETHODCALLTYPE ResetDeviceFormat(
		PCWSTR
		);

	virtual HRESULT STDMETHODCALLTYPE SetDeviceFormat(
		PCWSTR,
		WAVEFORMATEX *,
		WAVEFORMATEX *
		);

	virtual HRESULT STDMETHODCALLTYPE GetProcessingPeriod(
		PCWSTR,
		INT,
		PINT64,
		PINT64
		);

	virtual HRESULT STDMETHODCALLTYPE SetProcessingPeriod(
		PCWSTR,
		PINT64
		);

	virtual HRESULT STDMETHODCALLTYPE GetShareMode(
		PCWSTR,
	struct DeviceShareMode *
		);

	virtual HRESULT STDMETHODCALLTYPE SetShareMode(
		PCWSTR,
	struct DeviceShareMode *
		);

	virtual HRESULT STDMETHODCALLTYPE GetPropertyValue(
		PCWSTR,
		const PROPERTYKEY,
		PROPVARIANT *
		);

	virtual HRESULT STDMETHODCALLTYPE SetPropertyValue(
		PCWSTR,
		const PROPERTYKEY,
		PROPVARIANT *
		);

	virtual HRESULT STDMETHODCALLTYPE SetDefaultEndpoint(
		__in PCWSTR wszDeviceId,
		__in ERole eRole
		);

	virtual HRESULT STDMETHODCALLTYPE SetEndpointVisibility(
		PCWSTR,
		INT
		);
};

interface DECLSPEC_UUID("568b9108-44bf-40b4-9006-86afe5b5a620")
IPolicyConfigVista;
class DECLSPEC_UUID("294935CE-F637-4E7C-A41B-AB255460B862")
	CPolicyConfigVistaClient;

interface IPolicyConfigVista : public IUnknown
{
public:

	virtual HRESULT GetMixFormat(
		PCWSTR,
		WAVEFORMATEX **
		);  // not available on Windows 7, use method from IPolicyConfig

	virtual HRESULT STDMETHODCALLTYPE GetDeviceFormat(
		PCWSTR,
		INT,
		WAVEFORMATEX **
		);

	virtual HRESULT STDMETHODCALLTYPE SetDeviceFormat(
		PCWSTR,
		WAVEFORMATEX *,
		WAVEFORMATEX *
		);

	virtual HRESULT STDMETHODCALLTYPE GetProcessingPeriod(
		PCWSTR,
		INT,
		PINT64,
		PINT64
		);  // not available on Windows 7, use method from IPolicyConfig

	virtual HRESULT STDMETHODCALLTYPE SetProcessingPeriod(
		PCWSTR,
		PINT64
		);  // not available on Windows 7, use method from IPolicyConfig

	virtual HRESULT STDMETHODCALLTYPE GetShareMode(
		PCWSTR,
	struct DeviceShareMode *
		);  // not available on Windows 7, use method from IPolicyConfig

	virtual HRESULT STDMETHODCALLTYPE SetShareMode(
		PCWSTR,
	struct DeviceShareMode *
		);  // not available on Windows 7, use method from IPolicyConfig

	virtual HRESULT STDMETHODCALLTYPE GetPropertyValue(
		PCWSTR,
		const PROPERTYKEY,
		PROPVARIANT *
		);

	virtual HRESULT STDMETHODCALLTYPE SetPropertyValue(
		PCWSTR,
		const PROPERTYKEY,
		PROPVARIANT *
		);

	virtual HRESULT STDMETHODCALLTYPE SetDefaultEndpoint(
		__in PCWSTR wszDeviceId,
		__in ERole eRole
		);

	virtual HRESULT STDMETHODCALLTYPE SetEndpointVisibility(
		PCWSTR,
		INT
		);  // not available on Windows 7, use method from IPolicyConfig
};

class MixerWrapEx
{
public:
	MixerWrapEx(void){};
public:
	virtual ~MixerWrapEx(void){};
public:
	static HRESULT SetDefaultAudioPlaybackDevice( LPCWSTR devID );
	static BOOL SetDefaultInput(bool bDefault);
	static void FindPart(IPart *pPartPrev);
	static HRESULT SetSpeekerMuteVolumeLevel(IMMDevice *pEndptDev);
	static BOOL MutSpeaker(BOOL bMute);
};
