// WebUI.h : Declaration of the CWebUI
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include <objsafe.h> 
#include "BasePlug.h"
#include "_IWebUIEvents_CP.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// CWebUI
class ATL_NO_VTABLE CWebUI :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IWebUI, &IID_IWebUI, &LIBID_BasePlugLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IPersistStreamInitImpl<CWebUI>,
	public IOleControlImpl<CWebUI>,
	public IOleObjectImpl<CWebUI>,
	public IOleInPlaceActiveObjectImpl<CWebUI>,
	public IViewObjectExImpl<CWebUI>,
	public IOleInPlaceObjectWindowlessImpl<CWebUI>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CWebUI>,
	public CProxy_IWebUIEvents<CWebUI>,
	public IPersistStorageImpl<CWebUI>,
	public ISpecifyPropertyPagesImpl<CWebUI>,
	public IQuickActivateImpl<CWebUI>,
#ifndef _WIN32_WCE
	public IDataObjectImpl<CWebUI>,
#endif
	public IProvideClassInfo2Impl<&CLSID_WebUI, &__uuidof(_IWebUIEvents), &LIBID_BasePlugLib>,
#ifdef _WIN32_WCE // IObjectSafety is required on Windows CE for the control to be loaded correctly
	public IObjectSafetyImpl<CWebUI, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
#endif
	public IObjectSafetyImpl<CWebUI, INTERFACESAFE_FOR_UNTRUSTED_CALLER|INTERFACESAFE_FOR_UNTRUSTED_DATA>,
	public CComCoClass<CWebUI, &CLSID_WebUI>,
	public CComControl<CWebUI>,
	public IObjectSafety
{
public:


	CWebUI()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST
)

DECLARE_REGISTRY_RESOURCEID(IDR_WEBUI)


BEGIN_COM_MAP(CWebUI)
	COM_INTERFACE_ENTRY(IWebUI)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
#ifndef _WIN32_WCE
	COM_INTERFACE_ENTRY(IDataObject)
#endif
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
#ifdef _WIN32_WCE // IObjectSafety is required on Windows CE for the control to be loaded correctly
	COM_INTERFACE_ENTRY_IID(IID_IObjectSafety, IObjectSafety)
#endif
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

BEGIN_PROP_MAP(CWebUI)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CWebUI)
	CONNECTION_POINT_ENTRY(__uuidof(_IWebUIEvents))
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CWebUI)
	CHAIN_MSG_MAP(CComControl<CWebUI>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()

//��ȫ��ʾ////////////////////////////////////////////////////////////////////////
BEGIN_CATEGORY_MAP(CWebUI)
	IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
	IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
END_CATEGORY_MAP()

//////////////////////////////////////////////////////////////////////////


// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] =
		{
			&IID_IWebUI,
		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IWebUI
public:
		HRESULT OnDraw(ATL_DRAWINFO& di)
		{
		RECT& rc = *(RECT*)di.prcBounds;
		// Set Clip region to the rectangle specified by di.prcBounds
		HRGN hRgnOld = NULL;
		if (GetClipRgn(di.hdcDraw, hRgnOld) != 1)
			hRgnOld = NULL;
		bool bSelectOldRgn = false;

		HRGN hRgnNew = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);

		if (hRgnNew != NULL)
		{
			bSelectOldRgn = (SelectClipRgn(di.hdcDraw, hRgnNew) != ERROR);
		}

		Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);
		SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);
		LPCTSTR pszText = _T("ATL 8.0 : WebUI");
#ifndef _WIN32_WCE
		TextOut(di.hdcDraw,
			(rc.left + rc.right) / 2,
			(rc.top + rc.bottom) / 2,
			pszText,
			lstrlen(pszText));
#else
		ExtTextOut(di.hdcDraw,
			(rc.left + rc.right) / 2,
			(rc.top + rc.bottom) / 2,
			ETO_OPAQUE,
			NULL,
			pszText,
			ATL::lstrlen(pszText),
			NULL);
#endif

		if (bSelectOldRgn)
			SelectClipRgn(di.hdcDraw, hRgnOld);

		return S_OK;
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}
	STDMETHODIMP GetInterfaceSafetyOptions( REFIID riid, DWORD *pdwSupportedOptions, DWORD *pdwEnabledOptions );
	/*{
		*pdwSupportedOptions = INTERFACESAFE_FOR_UNTRUSTED_CALLER |	INTERFACESAFE_FOR_UNTRUSTED_DATA;
		*pdwEnabledOptions = *pdwSupportedOptions;

		return S_OK;
	}*/
	STDMETHODIMP SetInterfaceSafetyOptions(REFIID riid, DWORD dwOptionSetMask, DWORD dwEnabledOptions);
	/*{
		return S_OK;
	}*/
public:
	STDMETHOD(MicphoneBoost)(LONG isBoost);
public:
	STDMETHOD(OpenVoiceSet)(void);
public:
	STDMETHOD(GetAllPlay)(BSTR* payList);
public:
	STDMETHOD(OpenPlay)(BSTR exePath);
public:
	STDMETHOD(StereoMix)(LONG bIsMix);
public:
	STDMETHOD(IsAdminControl)(LONG* isAdmin);
	STDMETHOD(ListenMySelf)(void);
	STDMETHOD(TestUpdate)(void);
	STDMETHOD(GetVersion)(BSTR* strVersion);
public:
	static wchar_t   szVersion[20];

};

OBJECT_ENTRY_AUTO(__uuidof(WebUI), CWebUI)
