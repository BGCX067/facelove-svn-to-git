

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IWebUI,0xD7A9A5F8,0x5B1C,0x4D61,0x89,0x87,0x0A,0xAE,0x05,0xDC,0xD0,0x96);


MIDL_DEFINE_GUID(IID, LIBID_BasePlugLib,0x975572E4,0xB68A,0x417F,0x8A,0x8A,0x42,0x61,0x96,0x85,0xF7,0x33);


MIDL_DEFINE_GUID(IID, DIID__IWebUIEvents,0x2DD4B76B,0xE668,0x4A6D,0xB4,0xF3,0xB5,0x3B,0x4F,0xAB,0x2F,0x4D);


MIDL_DEFINE_GUID(CLSID, CLSID_WebUI,0x31CDE2B7,0xEC7B,0x409A,0xB8,0x8B,0xC2,0xCF,0x63,0x7B,0x86,0xBD);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



