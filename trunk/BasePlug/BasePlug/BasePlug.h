

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Jun 08 15:46:20 2012
 */
/* Compiler settings for .\BasePlug.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __BasePlug_h__
#define __BasePlug_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWebUI_FWD_DEFINED__
#define __IWebUI_FWD_DEFINED__
typedef interface IWebUI IWebUI;
#endif 	/* __IWebUI_FWD_DEFINED__ */


#ifndef ___IWebUIEvents_FWD_DEFINED__
#define ___IWebUIEvents_FWD_DEFINED__
typedef interface _IWebUIEvents _IWebUIEvents;
#endif 	/* ___IWebUIEvents_FWD_DEFINED__ */


#ifndef __WebUI_FWD_DEFINED__
#define __WebUI_FWD_DEFINED__

#ifdef __cplusplus
typedef class WebUI WebUI;
#else
typedef struct WebUI WebUI;
#endif /* __cplusplus */

#endif 	/* __WebUI_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IWebUI_INTERFACE_DEFINED__
#define __IWebUI_INTERFACE_DEFINED__

/* interface IWebUI */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IWebUI;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D7A9A5F8-5B1C-4D61-8987-0AAE05DCD096")
    IWebUI : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MicphoneBoost( 
            /* [in] */ LONG isBoost) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenVoiceSet( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAllPlay( 
            /* [retval][out] */ BSTR *payList) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenPlay( 
            /* [in] */ BSTR exePath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StereoMix( 
            /* [in] */ LONG bIsMix) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsAdminControl( 
            /* [retval][out] */ LONG *isAdmin) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ListenMySelf( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TestUpdate( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetVersion( 
            /* [retval][out] */ BSTR *strVersion) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWebUIVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWebUI * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWebUI * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWebUI * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWebUI * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWebUI * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWebUI * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWebUI * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MicphoneBoost )( 
            IWebUI * This,
            /* [in] */ LONG isBoost);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenVoiceSet )( 
            IWebUI * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAllPlay )( 
            IWebUI * This,
            /* [retval][out] */ BSTR *payList);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenPlay )( 
            IWebUI * This,
            /* [in] */ BSTR exePath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StereoMix )( 
            IWebUI * This,
            /* [in] */ LONG bIsMix);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IsAdminControl )( 
            IWebUI * This,
            /* [retval][out] */ LONG *isAdmin);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ListenMySelf )( 
            IWebUI * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TestUpdate )( 
            IWebUI * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IWebUI * This,
            /* [retval][out] */ BSTR *strVersion);
        
        END_INTERFACE
    } IWebUIVtbl;

    interface IWebUI
    {
        CONST_VTBL struct IWebUIVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWebUI_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWebUI_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWebUI_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWebUI_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IWebUI_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IWebUI_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IWebUI_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IWebUI_MicphoneBoost(This,isBoost)	\
    ( (This)->lpVtbl -> MicphoneBoost(This,isBoost) ) 

#define IWebUI_OpenVoiceSet(This)	\
    ( (This)->lpVtbl -> OpenVoiceSet(This) ) 

#define IWebUI_GetAllPlay(This,payList)	\
    ( (This)->lpVtbl -> GetAllPlay(This,payList) ) 

#define IWebUI_OpenPlay(This,exePath)	\
    ( (This)->lpVtbl -> OpenPlay(This,exePath) ) 

#define IWebUI_StereoMix(This,bIsMix)	\
    ( (This)->lpVtbl -> StereoMix(This,bIsMix) ) 

#define IWebUI_IsAdminControl(This,isAdmin)	\
    ( (This)->lpVtbl -> IsAdminControl(This,isAdmin) ) 

#define IWebUI_ListenMySelf(This)	\
    ( (This)->lpVtbl -> ListenMySelf(This) ) 

#define IWebUI_TestUpdate(This)	\
    ( (This)->lpVtbl -> TestUpdate(This) ) 

#define IWebUI_GetVersion(This,strVersion)	\
    ( (This)->lpVtbl -> GetVersion(This,strVersion) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWebUI_INTERFACE_DEFINED__ */



#ifndef __BasePlugLib_LIBRARY_DEFINED__
#define __BasePlugLib_LIBRARY_DEFINED__

/* library BasePlugLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_BasePlugLib;

#ifndef ___IWebUIEvents_DISPINTERFACE_DEFINED__
#define ___IWebUIEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IWebUIEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IWebUIEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("2DD4B76B-E668-4A6D-B4F3-B53B4FAB2F4D")
    _IWebUIEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IWebUIEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IWebUIEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IWebUIEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IWebUIEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IWebUIEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IWebUIEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IWebUIEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IWebUIEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IWebUIEventsVtbl;

    interface _IWebUIEvents
    {
        CONST_VTBL struct _IWebUIEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IWebUIEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IWebUIEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IWebUIEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IWebUIEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IWebUIEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IWebUIEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IWebUIEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IWebUIEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_WebUI;

#ifdef __cplusplus

class DECLSPEC_UUID("31CDE2B7-EC7B-409A-B88B-C2CF637B86BD")
WebUI;
#endif
#endif /* __BasePlugLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


