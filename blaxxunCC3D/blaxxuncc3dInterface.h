/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.01.75 */
/* at Thu Jan 27 19:47:29 2000
 */
/* Compiler settings for C:\client\cc3d\blaxxunCC3D\blaxxuncc3d.odl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )
#include "rpc.h"
#include "rpcndr.h"

#ifndef __blaxxuncc3dInterface_h__
#define __blaxxuncc3dInterface_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IblaxxunCC3D_FWD_DEFINED__
#define __IblaxxunCC3D_FWD_DEFINED__
typedef interface IblaxxunCC3D IblaxxunCC3D;
#endif 	/* __IblaxxunCC3D_FWD_DEFINED__ */


#ifndef __IblaxxunCC3DEvents_FWD_DEFINED__
#define __IblaxxunCC3DEvents_FWD_DEFINED__
typedef interface IblaxxunCC3DEvents IblaxxunCC3DEvents;
#endif 	/* __IblaxxunCC3DEvents_FWD_DEFINED__ */


#ifndef __blaxxunCC3D_FWD_DEFINED__
#define __blaxxunCC3D_FWD_DEFINED__

#ifdef __cplusplus
typedef class blaxxunCC3D blaxxunCC3D;
#else
typedef struct blaxxunCC3D blaxxunCC3D;
#endif /* __cplusplus */

#endif 	/* __blaxxunCC3D_FWD_DEFINED__ */


void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __blaxxunCC3DLib_LIBRARY_DEFINED__
#define __blaxxunCC3DLib_LIBRARY_DEFINED__

/****************************************
 * Generated header for library: blaxxunCC3DLib
 * at Thu Jan 27 19:47:29 2000
 * using MIDL 3.01.75
 ****************************************/
/* [control][helpstring][version][uuid] */ 


typedef 
enum BrowserObserverFlags
    {	OBSERVE_MESSAGE	= 0x1,
	OBSERVE_ANCHOR	= 0x2,
	OBSERVE_VIEWPOINT	= 0x4,
	OBSERVE_URLLOADING	= 0x8,
	OBSERVE_URLERRORS	= 0x10
    }	BrowserObserverFlags;

typedef 
enum BrowserUiMode
    {	BROWSER_UI_NONE	= 0,
	BROWSER_UI_SIMPLE	= BROWSER_UI_NONE + 1,
	BROWSER_UI_STANDARD	= BROWSER_UI_SIMPLE + 1,
	BROWSER_UI_EXPERT	= BROWSER_UI_STANDARD + 1
    }	BrowserUiMode;

typedef 
enum BrowserObjects
    {	BROWSER_OBJECT_DIRECTDRAW	= 0,
	BROWSER_OBJECT_DIRECT3D	= BROWSER_OBJECT_DIRECTDRAW + 1,
	BROWSER_OBJECT_DIRECT3DDEVICE	= BROWSER_OBJECT_DIRECT3D + 1,
	BROWSER_OBJECT_DIRECT3DVIEWPORT	= BROWSER_OBJECT_DIRECT3DDEVICE + 1,
	BROWSER_OBJECT_DD_FRONTBUFFER	= BROWSER_OBJECT_DIRECT3DVIEWPORT + 1,
	BROWSER_OBJECT_DD_BACKBUFFER	= BROWSER_OBJECT_DD_FRONTBUFFER + 1,
	BROWSER_OBJECT_DD_ZBUFFER	= BROWSER_OBJECT_DD_BACKBUFFER + 1,
	BROWSER_OBJECT_DIRECTSOUND	= BROWSER_OBJECT_DD_ZBUFFER + 1,
	BROWSER_OBJECT_DIRECTSOUND3DLISTENER	= BROWSER_OBJECT_DIRECTSOUND + 1,
	BROWSER_OBJECT_RSX	= BROWSER_OBJECT_DIRECTSOUND3DLISTENER + 1,
	BROWSER_OBJECT_RSX_LISTENER	= BROWSER_OBJECT_RSX + 1
    }	BrowserObjects;

typedef 
enum blaxxunCC3DInterfaceVersion
    {	CURRENT_VERSION	= 0x104
    }	blaxxunCC3DInterfaceVersion;


DEFINE_GUID(LIBID_blaxxunCC3DLib,0x4B6E3010,0x6E45,0x11D0,0x93,0x09,0x00,0x20,0xAF,0xE0,0x5C,0xC8);

#ifndef __IblaxxunCC3D_DISPINTERFACE_DEFINED__
#define __IblaxxunCC3D_DISPINTERFACE_DEFINED__

/****************************************
 * Generated header for dispinterface: IblaxxunCC3D
 * at Thu Jan 27 19:47:29 2000
 * using MIDL 3.01.75
 ****************************************/
/* [hidden][helpstring][uuid] */ 



DEFINE_GUID(DIID_IblaxxunCC3D,0x4B6E3011,0x6E45,0x11D0,0x93,0x09,0x00,0x20,0xAF,0xE0,0x5C,0xC8);

#if defined(__cplusplus) && !defined(CINTERFACE)

    interface DECLSPEC_UUID("4B6E3011-6E45-11D0-9309-0020AFE05CC8")
    IblaxxunCC3D : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IblaxxunCC3DVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IblaxxunCC3D __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IblaxxunCC3D __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IblaxxunCC3D __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IblaxxunCC3D __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IblaxxunCC3D __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IblaxxunCC3D __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IblaxxunCC3D __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } IblaxxunCC3DVtbl;

    interface IblaxxunCC3D
    {
        CONST_VTBL struct IblaxxunCC3DVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IblaxxunCC3D_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IblaxxunCC3D_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IblaxxunCC3D_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IblaxxunCC3D_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IblaxxunCC3D_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IblaxxunCC3D_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IblaxxunCC3D_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IblaxxunCC3D_DISPINTERFACE_DEFINED__ */


#ifndef __IblaxxunCC3DEvents_DISPINTERFACE_DEFINED__
#define __IblaxxunCC3DEvents_DISPINTERFACE_DEFINED__

/****************************************
 * Generated header for dispinterface: IblaxxunCC3DEvents
 * at Thu Jan 27 19:47:29 2000
 * using MIDL 3.01.75
 ****************************************/
/* [helpstring][uuid] */ 



DEFINE_GUID(DIID_IblaxxunCC3DEvents,0x4B6E3012,0x6E45,0x11D0,0x93,0x09,0x00,0x20,0xAF,0xE0,0x5C,0xC8);

#if defined(__cplusplus) && !defined(CINTERFACE)

    interface DECLSPEC_UUID("4B6E3012-6E45-11D0-9309-0020AFE05CC8")
    IblaxxunCC3DEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IblaxxunCC3DEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IblaxxunCC3DEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IblaxxunCC3DEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IblaxxunCC3DEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IblaxxunCC3DEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IblaxxunCC3DEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IblaxxunCC3DEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IblaxxunCC3DEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } IblaxxunCC3DEventsVtbl;

    interface IblaxxunCC3DEvents
    {
        CONST_VTBL struct IblaxxunCC3DEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IblaxxunCC3DEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IblaxxunCC3DEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IblaxxunCC3DEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IblaxxunCC3DEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IblaxxunCC3DEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IblaxxunCC3DEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IblaxxunCC3DEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IblaxxunCC3DEvents_DISPINTERFACE_DEFINED__ */


#ifdef __cplusplus
DEFINE_GUID(CLSID_blaxxunCC3D,0x4B6E3013,0x6E45,0x11D0,0x93,0x09,0x00,0x20,0xAF,0xE0,0x5C,0xC8);

class DECLSPEC_UUID("4B6E3013-6E45-11D0-9309-0020AFE05CC8")
blaxxunCC3D;
#endif
#endif /* __blaxxunCC3DLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
