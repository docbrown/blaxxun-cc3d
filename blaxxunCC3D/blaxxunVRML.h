/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.01.75 */
/* at Tue Sep 14 17:17:12 1999
 */
/* Compiler settings for blaxxunVRML.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )
#include "rpc.h"
#include "rpcndr.h"

#ifndef __blaxxunVRML_h__
#define __blaxxunVRML_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __Node_FWD_DEFINED__
#define __Node_FWD_DEFINED__
typedef interface Node Node;
#endif 	/* __Node_FWD_DEFINED__ */


#ifndef __Browser_FWD_DEFINED__
#define __Browser_FWD_DEFINED__
typedef interface Browser Browser;
#endif 	/* __Browser_FWD_DEFINED__ */


#ifndef __Field_FWD_DEFINED__
#define __Field_FWD_DEFINED__
typedef interface Field Field;
#endif 	/* __Field_FWD_DEFINED__ */


#ifndef __EventIn_FWD_DEFINED__
#define __EventIn_FWD_DEFINED__
typedef interface EventIn EventIn;
#endif 	/* __EventIn_FWD_DEFINED__ */


#ifndef __EventOut_FWD_DEFINED__
#define __EventOut_FWD_DEFINED__
typedef interface EventOut EventOut;
#endif 	/* __EventOut_FWD_DEFINED__ */


#ifndef __EventInSFFloat_FWD_DEFINED__
#define __EventInSFFloat_FWD_DEFINED__
typedef interface EventInSFFloat EventInSFFloat;
#endif 	/* __EventInSFFloat_FWD_DEFINED__ */


#ifndef __EventInSFBool_FWD_DEFINED__
#define __EventInSFBool_FWD_DEFINED__
typedef interface EventInSFBool EventInSFBool;
#endif 	/* __EventInSFBool_FWD_DEFINED__ */


#ifndef __EventOutMField_FWD_DEFINED__
#define __EventOutMField_FWD_DEFINED__
typedef interface EventOutMField EventOutMField;
#endif 	/* __EventOutMField_FWD_DEFINED__ */


#ifndef __EventInMFColor_FWD_DEFINED__
#define __EventInMFColor_FWD_DEFINED__
typedef interface EventInMFColor EventInMFColor;
#endif 	/* __EventInMFColor_FWD_DEFINED__ */


#ifndef __EventInMFFloat_FWD_DEFINED__
#define __EventInMFFloat_FWD_DEFINED__
typedef interface EventInMFFloat EventInMFFloat;
#endif 	/* __EventInMFFloat_FWD_DEFINED__ */


#ifndef __EventInMFTime_FWD_DEFINED__
#define __EventInMFTime_FWD_DEFINED__
typedef interface EventInMFTime EventInMFTime;
#endif 	/* __EventInMFTime_FWD_DEFINED__ */


#ifndef __EventInMFInt32_FWD_DEFINED__
#define __EventInMFInt32_FWD_DEFINED__
typedef interface EventInMFInt32 EventInMFInt32;
#endif 	/* __EventInMFInt32_FWD_DEFINED__ */


#ifndef __EventInMFNode_FWD_DEFINED__
#define __EventInMFNode_FWD_DEFINED__
typedef interface EventInMFNode EventInMFNode;
#endif 	/* __EventInMFNode_FWD_DEFINED__ */


#ifndef __EventInMFRotation_FWD_DEFINED__
#define __EventInMFRotation_FWD_DEFINED__
typedef interface EventInMFRotation EventInMFRotation;
#endif 	/* __EventInMFRotation_FWD_DEFINED__ */


#ifndef __EventInMFString_FWD_DEFINED__
#define __EventInMFString_FWD_DEFINED__
typedef interface EventInMFString EventInMFString;
#endif 	/* __EventInMFString_FWD_DEFINED__ */


#ifndef __EventInMFVec2f_FWD_DEFINED__
#define __EventInMFVec2f_FWD_DEFINED__
typedef interface EventInMFVec2f EventInMFVec2f;
#endif 	/* __EventInMFVec2f_FWD_DEFINED__ */


#ifndef __EventInMFVec3f_FWD_DEFINED__
#define __EventInMFVec3f_FWD_DEFINED__
typedef interface EventInMFVec3f EventInMFVec3f;
#endif 	/* __EventInMFVec3f_FWD_DEFINED__ */


#ifndef __EventInSFColor_FWD_DEFINED__
#define __EventInSFColor_FWD_DEFINED__
typedef interface EventInSFColor EventInSFColor;
#endif 	/* __EventInSFColor_FWD_DEFINED__ */


#ifndef __EventInSFImage_FWD_DEFINED__
#define __EventInSFImage_FWD_DEFINED__
typedef interface EventInSFImage EventInSFImage;
#endif 	/* __EventInSFImage_FWD_DEFINED__ */


#ifndef __EventInSFInt32_FWD_DEFINED__
#define __EventInSFInt32_FWD_DEFINED__
typedef interface EventInSFInt32 EventInSFInt32;
#endif 	/* __EventInSFInt32_FWD_DEFINED__ */


#ifndef __EventInSFNode_FWD_DEFINED__
#define __EventInSFNode_FWD_DEFINED__
typedef interface EventInSFNode EventInSFNode;
#endif 	/* __EventInSFNode_FWD_DEFINED__ */


#ifndef __EventInSFRotation_FWD_DEFINED__
#define __EventInSFRotation_FWD_DEFINED__
typedef interface EventInSFRotation EventInSFRotation;
#endif 	/* __EventInSFRotation_FWD_DEFINED__ */


#ifndef __EventInSFString_FWD_DEFINED__
#define __EventInSFString_FWD_DEFINED__
typedef interface EventInSFString EventInSFString;
#endif 	/* __EventInSFString_FWD_DEFINED__ */


#ifndef __EventInSFTime_FWD_DEFINED__
#define __EventInSFTime_FWD_DEFINED__
typedef interface EventInSFTime EventInSFTime;
#endif 	/* __EventInSFTime_FWD_DEFINED__ */


#ifndef __EventInSFVec2f_FWD_DEFINED__
#define __EventInSFVec2f_FWD_DEFINED__
typedef interface EventInSFVec2f EventInSFVec2f;
#endif 	/* __EventInSFVec2f_FWD_DEFINED__ */


#ifndef __EventInSFVec3f_FWD_DEFINED__
#define __EventInSFVec3f_FWD_DEFINED__
typedef interface EventInSFVec3f EventInSFVec3f;
#endif 	/* __EventInSFVec3f_FWD_DEFINED__ */


#ifndef __EventOutObserver_FWD_DEFINED__
#define __EventOutObserver_FWD_DEFINED__
typedef interface EventOutObserver EventOutObserver;
#endif 	/* __EventOutObserver_FWD_DEFINED__ */


#ifndef __EventOutMFColor_FWD_DEFINED__
#define __EventOutMFColor_FWD_DEFINED__
typedef interface EventOutMFColor EventOutMFColor;
#endif 	/* __EventOutMFColor_FWD_DEFINED__ */


#ifndef __EventOutMFFloat_FWD_DEFINED__
#define __EventOutMFFloat_FWD_DEFINED__
typedef interface EventOutMFFloat EventOutMFFloat;
#endif 	/* __EventOutMFFloat_FWD_DEFINED__ */


#ifndef __EventOutMFTime_FWD_DEFINED__
#define __EventOutMFTime_FWD_DEFINED__
typedef interface EventOutMFTime EventOutMFTime;
#endif 	/* __EventOutMFTime_FWD_DEFINED__ */


#ifndef __EventOutMFInt32_FWD_DEFINED__
#define __EventOutMFInt32_FWD_DEFINED__
typedef interface EventOutMFInt32 EventOutMFInt32;
#endif 	/* __EventOutMFInt32_FWD_DEFINED__ */


#ifndef __EventOutMFNode_FWD_DEFINED__
#define __EventOutMFNode_FWD_DEFINED__
typedef interface EventOutMFNode EventOutMFNode;
#endif 	/* __EventOutMFNode_FWD_DEFINED__ */


#ifndef __EventOutMFRotation_FWD_DEFINED__
#define __EventOutMFRotation_FWD_DEFINED__
typedef interface EventOutMFRotation EventOutMFRotation;
#endif 	/* __EventOutMFRotation_FWD_DEFINED__ */


#ifndef __EventOutMFString_FWD_DEFINED__
#define __EventOutMFString_FWD_DEFINED__
typedef interface EventOutMFString EventOutMFString;
#endif 	/* __EventOutMFString_FWD_DEFINED__ */


#ifndef __EventOutMFVec2f_FWD_DEFINED__
#define __EventOutMFVec2f_FWD_DEFINED__
typedef interface EventOutMFVec2f EventOutMFVec2f;
#endif 	/* __EventOutMFVec2f_FWD_DEFINED__ */


#ifndef __EventOutMFVec3f_FWD_DEFINED__
#define __EventOutMFVec3f_FWD_DEFINED__
typedef interface EventOutMFVec3f EventOutMFVec3f;
#endif 	/* __EventOutMFVec3f_FWD_DEFINED__ */


#ifndef __EventOutSFBool_FWD_DEFINED__
#define __EventOutSFBool_FWD_DEFINED__
typedef interface EventOutSFBool EventOutSFBool;
#endif 	/* __EventOutSFBool_FWD_DEFINED__ */


#ifndef __EventOutSFColor_FWD_DEFINED__
#define __EventOutSFColor_FWD_DEFINED__
typedef interface EventOutSFColor EventOutSFColor;
#endif 	/* __EventOutSFColor_FWD_DEFINED__ */


#ifndef __EventOutSFFloat_FWD_DEFINED__
#define __EventOutSFFloat_FWD_DEFINED__
typedef interface EventOutSFFloat EventOutSFFloat;
#endif 	/* __EventOutSFFloat_FWD_DEFINED__ */


#ifndef __EventOutSFImage_FWD_DEFINED__
#define __EventOutSFImage_FWD_DEFINED__
typedef interface EventOutSFImage EventOutSFImage;
#endif 	/* __EventOutSFImage_FWD_DEFINED__ */


#ifndef __EventOutSFInt32_FWD_DEFINED__
#define __EventOutSFInt32_FWD_DEFINED__
typedef interface EventOutSFInt32 EventOutSFInt32;
#endif 	/* __EventOutSFInt32_FWD_DEFINED__ */


#ifndef __EventOutSFNode_FWD_DEFINED__
#define __EventOutSFNode_FWD_DEFINED__
typedef interface EventOutSFNode EventOutSFNode;
#endif 	/* __EventOutSFNode_FWD_DEFINED__ */


#ifndef __EventOutSFRotation_FWD_DEFINED__
#define __EventOutSFRotation_FWD_DEFINED__
typedef interface EventOutSFRotation EventOutSFRotation;
#endif 	/* __EventOutSFRotation_FWD_DEFINED__ */


#ifndef __EventOutSFString_FWD_DEFINED__
#define __EventOutSFString_FWD_DEFINED__
typedef interface EventOutSFString EventOutSFString;
#endif 	/* __EventOutSFString_FWD_DEFINED__ */


#ifndef __EventOutSFTime_FWD_DEFINED__
#define __EventOutSFTime_FWD_DEFINED__
typedef interface EventOutSFTime EventOutSFTime;
#endif 	/* __EventOutSFTime_FWD_DEFINED__ */


#ifndef __EventOutSFVec2f_FWD_DEFINED__
#define __EventOutSFVec2f_FWD_DEFINED__
typedef interface EventOutSFVec2f EventOutSFVec2f;
#endif 	/* __EventOutSFVec2f_FWD_DEFINED__ */


#ifndef __EventOutSFVec3f_FWD_DEFINED__
#define __EventOutSFVec3f_FWD_DEFINED__
typedef interface EventOutSFVec3f EventOutSFVec3f;
#endif 	/* __EventOutSFVec3f_FWD_DEFINED__ */


#ifndef __ScriptContainer_FWD_DEFINED__
#define __ScriptContainer_FWD_DEFINED__
typedef interface ScriptContainer ScriptContainer;
#endif 	/* __ScriptContainer_FWD_DEFINED__ */


#ifndef __Script_FWD_DEFINED__
#define __Script_FWD_DEFINED__
typedef interface Script Script;
#endif 	/* __Script_FWD_DEFINED__ */


#ifndef __BrowserObserver_FWD_DEFINED__
#define __BrowserObserver_FWD_DEFINED__
typedef interface BrowserObserver BrowserObserver;
#endif 	/* __BrowserObserver_FWD_DEFINED__ */


void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __blaxxunVRMLLib_LIBRARY_DEFINED__
#define __blaxxunVRMLLib_LIBRARY_DEFINED__

/****************************************
 * Generated header for library: blaxxunVRMLLib
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [helpstring][version][uuid] */ 










typedef 
enum InterfaceElementTypes
    {	type_unknown	= 0,
	type_field	= type_unknown + 1,
	type_exposedField	= type_field + 1,
	type_eventIn	= type_exposedField + 1,
	type_eventOut	= type_eventIn + 1
    }	InterfaceElementTypes;

typedef 
enum FieldTypes
    {	UnknownType	= 0,
	SFBOOL	= UnknownType + 1,
	SFIMAGE	= SFBOOL + 1,
	SFTIME	= SFIMAGE + 1,
	SFCOLOR	= SFTIME + 1,
	MFCOLOR	= SFCOLOR + 1,
	SFFLOAT	= MFCOLOR + 1,
	MFFLOAT	= SFFLOAT + 1,
	SFINT32	= MFFLOAT + 1,
	MFINT32	= SFINT32 + 1,
	SFNODE	= MFINT32 + 1,
	MFNODE	= SFNODE + 1,
	SFROTATION	= MFNODE + 1,
	MFROTATION	= SFROTATION + 1,
	SFSTRING	= MFROTATION + 1,
	MFSTRING	= SFSTRING + 1,
	SFVEC2F	= MFSTRING + 1,
	MFVEC2F	= SFVEC2F + 1,
	SFVEC3F	= MFVEC2F + 1,
	MFVEC3F	= SFVEC3F + 1,
	SFENUM	= MFVEC3F + 1,
	SFBITMASK	= SFENUM + 1,
	SFMATRIX	= SFBITMASK + 1,
	SFLONG	= SFMATRIX + 1,
	MFLONG	= SFLONG + 1,
	MFTIME	= MFLONG + 1
    }	FieldType;



EXTERN_C const IID LIBID_blaxxunVRMLLib;

#ifndef __Node_INTERFACE_DEFINED__
#define __Node_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: Node
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_Node;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC50393-75D9-11D0-9315-0020AFE05CC8")
    Node : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getType( 
            /* [retval][out] */ BSTR __RPC_FAR *type) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getEventIn( 
            /* [in] */ BSTR name,
            /* [retval][out] */ EventIn __RPC_FAR *__RPC_FAR *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getEventOut( 
            /* [in] */ BSTR name,
            /* [retval][out] */ EventOut __RPC_FAR *__RPC_FAR *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getField( 
            /* [in] */ BSTR name,
            /* [retval][out] */ Field __RPC_FAR *__RPC_FAR *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getExposedField( 
            /* [in] */ BSTR name,
            /* [retval][out] */ Field __RPC_FAR *__RPC_FAR *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getBrowser( 
            /* [retval][out] */ Browser __RPC_FAR *__RPC_FAR *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getName( 
            /* [retval][out] */ BSTR __RPC_FAR *name) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setName( 
            /* [in] */ BSTR name) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getNumInterfaceElements( 
            /* [retval][out] */ int __RPC_FAR *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getInterfaceElement( 
            /* [in] */ int index,
            /* [out] */ int __RPC_FAR *elementType,
            /* [out] */ int __RPC_FAR *fieldType,
            /* [out] */ BSTR __RPC_FAR *name) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getValueNode( 
            /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getNative( 
            /* [retval][out] */ long __RPC_FAR *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getRefCnt( 
            /* [retval][out] */ long __RPC_FAR *result) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct NodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            Node __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            Node __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            Node __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            Node __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            Node __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            Node __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            Node __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            Node __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getEventIn )( 
            Node __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [retval][out] */ EventIn __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getEventOut )( 
            Node __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [retval][out] */ EventOut __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getField )( 
            Node __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [retval][out] */ Field __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getExposedField )( 
            Node __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [retval][out] */ Field __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getBrowser )( 
            Node __RPC_FAR * This,
            /* [retval][out] */ Browser __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            Node __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setName )( 
            Node __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getNumInterfaceElements )( 
            Node __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getInterfaceElement )( 
            Node __RPC_FAR * This,
            /* [in] */ int index,
            /* [out] */ int __RPC_FAR *elementType,
            /* [out] */ int __RPC_FAR *fieldType,
            /* [out] */ BSTR __RPC_FAR *name);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValueNode )( 
            Node __RPC_FAR * This,
            /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getNative )( 
            Node __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getRefCnt )( 
            Node __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *result);
        
        END_INTERFACE
    } NodeVtbl;

    interface Node
    {
        CONST_VTBL struct NodeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Node_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define Node_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define Node_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define Node_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define Node_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define Node_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define Node_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define Node_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define Node_getEventIn(This,name,result)	\
    (This)->lpVtbl -> getEventIn(This,name,result)

#define Node_getEventOut(This,name,result)	\
    (This)->lpVtbl -> getEventOut(This,name,result)

#define Node_getField(This,name,result)	\
    (This)->lpVtbl -> getField(This,name,result)

#define Node_getExposedField(This,name,result)	\
    (This)->lpVtbl -> getExposedField(This,name,result)

#define Node_getBrowser(This,result)	\
    (This)->lpVtbl -> getBrowser(This,result)

#define Node_getName(This,name)	\
    (This)->lpVtbl -> getName(This,name)

#define Node_setName(This,name)	\
    (This)->lpVtbl -> setName(This,name)

#define Node_getNumInterfaceElements(This,result)	\
    (This)->lpVtbl -> getNumInterfaceElements(This,result)

#define Node_getInterfaceElement(This,index,elementType,fieldType,name)	\
    (This)->lpVtbl -> getInterfaceElement(This,index,elementType,fieldType,name)

#define Node_getValueNode(This,result)	\
    (This)->lpVtbl -> getValueNode(This,result)

#define Node_getNative(This,result)	\
    (This)->lpVtbl -> getNative(This,result)

#define Node_getRefCnt(This,result)	\
    (This)->lpVtbl -> getRefCnt(This,result)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE Node_getType_Proxy( 
    Node __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *type);


void __RPC_STUB Node_getType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Node_getEventIn_Proxy( 
    Node __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [retval][out] */ EventIn __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB Node_getEventIn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Node_getEventOut_Proxy( 
    Node __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [retval][out] */ EventOut __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB Node_getEventOut_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Node_getField_Proxy( 
    Node __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [retval][out] */ Field __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB Node_getField_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Node_getExposedField_Proxy( 
    Node __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [retval][out] */ Field __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB Node_getExposedField_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Node_getBrowser_Proxy( 
    Node __RPC_FAR * This,
    /* [retval][out] */ Browser __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB Node_getBrowser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Node_getName_Proxy( 
    Node __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *name);


void __RPC_STUB Node_getName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Node_setName_Proxy( 
    Node __RPC_FAR * This,
    /* [in] */ BSTR name);


void __RPC_STUB Node_setName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Node_getNumInterfaceElements_Proxy( 
    Node __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *result);


void __RPC_STUB Node_getNumInterfaceElements_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Node_getInterfaceElement_Proxy( 
    Node __RPC_FAR * This,
    /* [in] */ int index,
    /* [out] */ int __RPC_FAR *elementType,
    /* [out] */ int __RPC_FAR *fieldType,
    /* [out] */ BSTR __RPC_FAR *name);


void __RPC_STUB Node_getInterfaceElement_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Node_getValueNode_Proxy( 
    Node __RPC_FAR * This,
    /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB Node_getValueNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Node_getNative_Proxy( 
    Node __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *result);


void __RPC_STUB Node_getNative_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Node_getRefCnt_Proxy( 
    Node __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *result);


void __RPC_STUB Node_getRefCnt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __Node_INTERFACE_DEFINED__ */


#ifndef __Browser_INTERFACE_DEFINED__
#define __Browser_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: Browser
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_Browser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC5046F-75D9-11D0-9315-0020AFE05CC8")
    Browser : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getName( 
            /* [retval][out] */ BSTR __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getVersion( 
            /* [retval][out] */ BSTR __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getCurrentSpeed( 
            /* [retval][out] */ float __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getCurrentFrameRate( 
            /* [retval][out] */ float __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getWorldURL( 
            /* [retval][out] */ BSTR __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE replaceWorld( 
            Node __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE loadURL( 
            /* [in] */ int cntUrl,
            /* [size_is][in] */ BSTR __RPC_FAR *url,
            /* [in] */ int cntParameter,
            /* [size_is][in] */ BSTR __RPC_FAR *parameter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setDescription( 
            /* [in] */ BSTR description) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE createVrmlFromString( 
            /* [in] */ BSTR vrmlSyntax,
            /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE createVrmlFromURL( 
            /* [in] */ BSTR url,
            /* [in] */ Node __RPC_FAR *node,
            /* [in] */ BSTR event) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getNode( 
            /* [in] */ BSTR name,
            /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE addRoute( 
            /* [in] */ Node __RPC_FAR *fromNode,
            /* [in] */ BSTR fromEventOut,
            /* [in] */ Node __RPC_FAR *toNode,
            /* [in] */ BSTR toEventIn) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE deleteRoute( 
            /* [in] */ Node __RPC_FAR *fromNode,
            /* [in] */ BSTR fromEventOut,
            /* [in] */ Node __RPC_FAR *toNode,
            /* [in] */ BSTR toEventIn) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE createNode( 
            /* [in] */ BSTR nodeclass,
            /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getWorld( 
            /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE beginUpdate( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE endUpdate( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE addNode( 
            /* [in] */ Node __RPC_FAR *node) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE removeNode( 
            /* [in] */ Node __RPC_FAR *node,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setNodeName( 
            /* [in] */ Node __RPC_FAR *node,
            /* [in] */ BSTR name) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getNodeName( 
            /* [in] */ Node __RPC_FAR *node,
            /* [retval][out] */ BSTR __RPC_FAR *name) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getEventOut( 
            BSTR eventOutName,
            /* [retval][out] */ EventOut __RPC_FAR *__RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getEventIn( 
            BSTR eventInName,
            /* [retval][out] */ EventIn __RPC_FAR *__RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getControlPathname( 
            /* [retval][out] */ BSTR __RPC_FAR *directoryName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE saveWorld( 
            /* [in] */ BSTR fileName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE saveViewpoint( 
            /* [in] */ BSTR viewpointName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setObserver( 
            /* [in] */ IUnknown __RPC_FAR *observer,
            /* [in] */ int flags,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setViewpointByValue( 
            /* [size_is][in] */ float __RPC_FAR *position,
            /* [size_is][in] */ float __RPC_FAR *orientation,
            /* [in] */ float fov,
            /* [in] */ VARIANT_BOOL animate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setFullscreen( 
            /* [in] */ VARIANT_BOOL mode,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setCollisionDetection( 
            /* [in] */ VARIANT_BOOL mode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setRenderMode( 
            /* [in] */ BSTR mode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setNavigationMode( 
            /* [in] */ BSTR mode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setViewpoint( 
            /* [in] */ BSTR viewpointName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setHeadlight( 
            /* [in] */ VARIANT_BOOL mode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setAvatarHeight( 
            /* [in] */ float value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setCollisionDistance( 
            /* [in] */ float value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setVisibilityLimit( 
            /* [in] */ float value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setWalkSpeed( 
            /* [in] */ float value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setNavigationPanel( 
            /* [in] */ VARIANT_BOOL mode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getNavigationPanel( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct BrowserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            Browser __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            Browser __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            Browser __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            Browser __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            Browser __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            Browser __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            Browser __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getName )( 
            Browser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getVersion )( 
            Browser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getCurrentSpeed )( 
            Browser __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getCurrentFrameRate )( 
            Browser __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getWorldURL )( 
            Browser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *replaceWorld )( 
            Browser __RPC_FAR * This,
            Node __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *loadURL )( 
            Browser __RPC_FAR * This,
            /* [in] */ int cntUrl,
            /* [size_is][in] */ BSTR __RPC_FAR *url,
            /* [in] */ int cntParameter,
            /* [size_is][in] */ BSTR __RPC_FAR *parameter);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setDescription )( 
            Browser __RPC_FAR * This,
            /* [in] */ BSTR description);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *createVrmlFromString )( 
            Browser __RPC_FAR * This,
            /* [in] */ BSTR vrmlSyntax,
            /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *createVrmlFromURL )( 
            Browser __RPC_FAR * This,
            /* [in] */ BSTR url,
            /* [in] */ Node __RPC_FAR *node,
            /* [in] */ BSTR event);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getNode )( 
            Browser __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addRoute )( 
            Browser __RPC_FAR * This,
            /* [in] */ Node __RPC_FAR *fromNode,
            /* [in] */ BSTR fromEventOut,
            /* [in] */ Node __RPC_FAR *toNode,
            /* [in] */ BSTR toEventIn);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *deleteRoute )( 
            Browser __RPC_FAR * This,
            /* [in] */ Node __RPC_FAR *fromNode,
            /* [in] */ BSTR fromEventOut,
            /* [in] */ Node __RPC_FAR *toNode,
            /* [in] */ BSTR toEventIn);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *createNode )( 
            Browser __RPC_FAR * This,
            /* [in] */ BSTR nodeclass,
            /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getWorld )( 
            Browser __RPC_FAR * This,
            /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *beginUpdate )( 
            Browser __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *endUpdate )( 
            Browser __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addNode )( 
            Browser __RPC_FAR * This,
            /* [in] */ Node __RPC_FAR *node);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeNode )( 
            Browser __RPC_FAR * This,
            /* [in] */ Node __RPC_FAR *node,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setNodeName )( 
            Browser __RPC_FAR * This,
            /* [in] */ Node __RPC_FAR *node,
            /* [in] */ BSTR name);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getNodeName )( 
            Browser __RPC_FAR * This,
            /* [in] */ Node __RPC_FAR *node,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getEventOut )( 
            Browser __RPC_FAR * This,
            BSTR eventOutName,
            /* [retval][out] */ EventOut __RPC_FAR *__RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getEventIn )( 
            Browser __RPC_FAR * This,
            BSTR eventInName,
            /* [retval][out] */ EventIn __RPC_FAR *__RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getControlPathname )( 
            Browser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *directoryName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *saveWorld )( 
            Browser __RPC_FAR * This,
            /* [in] */ BSTR fileName,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *saveViewpoint )( 
            Browser __RPC_FAR * This,
            /* [in] */ BSTR viewpointName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setObserver )( 
            Browser __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *observer,
            /* [in] */ int flags,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setViewpointByValue )( 
            Browser __RPC_FAR * This,
            /* [size_is][in] */ float __RPC_FAR *position,
            /* [size_is][in] */ float __RPC_FAR *orientation,
            /* [in] */ float fov,
            /* [in] */ VARIANT_BOOL animate);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setFullscreen )( 
            Browser __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL mode,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setCollisionDetection )( 
            Browser __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL mode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setRenderMode )( 
            Browser __RPC_FAR * This,
            /* [in] */ BSTR mode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setNavigationMode )( 
            Browser __RPC_FAR * This,
            /* [in] */ BSTR mode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setViewpoint )( 
            Browser __RPC_FAR * This,
            /* [in] */ BSTR viewpointName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setHeadlight )( 
            Browser __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL mode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setAvatarHeight )( 
            Browser __RPC_FAR * This,
            /* [in] */ float value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setCollisionDistance )( 
            Browser __RPC_FAR * This,
            /* [in] */ float value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setVisibilityLimit )( 
            Browser __RPC_FAR * This,
            /* [in] */ float value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setWalkSpeed )( 
            Browser __RPC_FAR * This,
            /* [in] */ float value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setNavigationPanel )( 
            Browser __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL mode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getNavigationPanel )( 
            Browser __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value);
        
        END_INTERFACE
    } BrowserVtbl;

    interface Browser
    {
        CONST_VTBL struct BrowserVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Browser_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define Browser_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define Browser_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define Browser_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define Browser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define Browser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define Browser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define Browser_getName(This,value)	\
    (This)->lpVtbl -> getName(This,value)

#define Browser_getVersion(This,value)	\
    (This)->lpVtbl -> getVersion(This,value)

#define Browser_getCurrentSpeed(This,value)	\
    (This)->lpVtbl -> getCurrentSpeed(This,value)

#define Browser_getCurrentFrameRate(This,value)	\
    (This)->lpVtbl -> getCurrentFrameRate(This,value)

#define Browser_getWorldURL(This,value)	\
    (This)->lpVtbl -> getWorldURL(This,value)

#define Browser_replaceWorld(This,value)	\
    (This)->lpVtbl -> replaceWorld(This,value)

#define Browser_loadURL(This,cntUrl,url,cntParameter,parameter)	\
    (This)->lpVtbl -> loadURL(This,cntUrl,url,cntParameter,parameter)

#define Browser_setDescription(This,description)	\
    (This)->lpVtbl -> setDescription(This,description)

#define Browser_createVrmlFromString(This,vrmlSyntax,value)	\
    (This)->lpVtbl -> createVrmlFromString(This,vrmlSyntax,value)

#define Browser_createVrmlFromURL(This,url,node,event)	\
    (This)->lpVtbl -> createVrmlFromURL(This,url,node,event)

#define Browser_getNode(This,name,value)	\
    (This)->lpVtbl -> getNode(This,name,value)

#define Browser_addRoute(This,fromNode,fromEventOut,toNode,toEventIn)	\
    (This)->lpVtbl -> addRoute(This,fromNode,fromEventOut,toNode,toEventIn)

#define Browser_deleteRoute(This,fromNode,fromEventOut,toNode,toEventIn)	\
    (This)->lpVtbl -> deleteRoute(This,fromNode,fromEventOut,toNode,toEventIn)

#define Browser_createNode(This,nodeclass,value)	\
    (This)->lpVtbl -> createNode(This,nodeclass,value)

#define Browser_getWorld(This,value)	\
    (This)->lpVtbl -> getWorld(This,value)

#define Browser_beginUpdate(This)	\
    (This)->lpVtbl -> beginUpdate(This)

#define Browser_endUpdate(This)	\
    (This)->lpVtbl -> endUpdate(This)

#define Browser_addNode(This,node)	\
    (This)->lpVtbl -> addNode(This,node)

#define Browser_removeNode(This,node,value)	\
    (This)->lpVtbl -> removeNode(This,node,value)

#define Browser_setNodeName(This,node,name)	\
    (This)->lpVtbl -> setNodeName(This,node,name)

#define Browser_getNodeName(This,node,name)	\
    (This)->lpVtbl -> getNodeName(This,node,name)

#define Browser_getEventOut(This,eventOutName,value)	\
    (This)->lpVtbl -> getEventOut(This,eventOutName,value)

#define Browser_getEventIn(This,eventInName,value)	\
    (This)->lpVtbl -> getEventIn(This,eventInName,value)

#define Browser_getControlPathname(This,directoryName)	\
    (This)->lpVtbl -> getControlPathname(This,directoryName)

#define Browser_saveWorld(This,fileName,value)	\
    (This)->lpVtbl -> saveWorld(This,fileName,value)

#define Browser_saveViewpoint(This,viewpointName)	\
    (This)->lpVtbl -> saveViewpoint(This,viewpointName)

#define Browser_setObserver(This,observer,flags,value)	\
    (This)->lpVtbl -> setObserver(This,observer,flags,value)

#define Browser_setViewpointByValue(This,position,orientation,fov,animate)	\
    (This)->lpVtbl -> setViewpointByValue(This,position,orientation,fov,animate)

#define Browser_setFullscreen(This,mode,value)	\
    (This)->lpVtbl -> setFullscreen(This,mode,value)

#define Browser_setCollisionDetection(This,mode)	\
    (This)->lpVtbl -> setCollisionDetection(This,mode)

#define Browser_setRenderMode(This,mode)	\
    (This)->lpVtbl -> setRenderMode(This,mode)

#define Browser_setNavigationMode(This,mode)	\
    (This)->lpVtbl -> setNavigationMode(This,mode)

#define Browser_setViewpoint(This,viewpointName)	\
    (This)->lpVtbl -> setViewpoint(This,viewpointName)

#define Browser_setHeadlight(This,mode)	\
    (This)->lpVtbl -> setHeadlight(This,mode)

#define Browser_setAvatarHeight(This,value)	\
    (This)->lpVtbl -> setAvatarHeight(This,value)

#define Browser_setCollisionDistance(This,value)	\
    (This)->lpVtbl -> setCollisionDistance(This,value)

#define Browser_setVisibilityLimit(This,value)	\
    (This)->lpVtbl -> setVisibilityLimit(This,value)

#define Browser_setWalkSpeed(This,value)	\
    (This)->lpVtbl -> setWalkSpeed(This,value)

#define Browser_setNavigationPanel(This,mode)	\
    (This)->lpVtbl -> setNavigationPanel(This,mode)

#define Browser_getNavigationPanel(This,value)	\
    (This)->lpVtbl -> getNavigationPanel(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE Browser_getName_Proxy( 
    Browser __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *value);


void __RPC_STUB Browser_getName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_getVersion_Proxy( 
    Browser __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *value);


void __RPC_STUB Browser_getVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_getCurrentSpeed_Proxy( 
    Browser __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *value);


void __RPC_STUB Browser_getCurrentSpeed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_getCurrentFrameRate_Proxy( 
    Browser __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *value);


void __RPC_STUB Browser_getCurrentFrameRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_getWorldURL_Proxy( 
    Browser __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *value);


void __RPC_STUB Browser_getWorldURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_replaceWorld_Proxy( 
    Browser __RPC_FAR * This,
    Node __RPC_FAR *value);


void __RPC_STUB Browser_replaceWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_loadURL_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ int cntUrl,
    /* [size_is][in] */ BSTR __RPC_FAR *url,
    /* [in] */ int cntParameter,
    /* [size_is][in] */ BSTR __RPC_FAR *parameter);


void __RPC_STUB Browser_loadURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_setDescription_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ BSTR description);


void __RPC_STUB Browser_setDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_createVrmlFromString_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ BSTR vrmlSyntax,
    /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB Browser_createVrmlFromString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_createVrmlFromURL_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ BSTR url,
    /* [in] */ Node __RPC_FAR *node,
    /* [in] */ BSTR event);


void __RPC_STUB Browser_createVrmlFromURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_getNode_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB Browser_getNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_addRoute_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ Node __RPC_FAR *fromNode,
    /* [in] */ BSTR fromEventOut,
    /* [in] */ Node __RPC_FAR *toNode,
    /* [in] */ BSTR toEventIn);


void __RPC_STUB Browser_addRoute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_deleteRoute_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ Node __RPC_FAR *fromNode,
    /* [in] */ BSTR fromEventOut,
    /* [in] */ Node __RPC_FAR *toNode,
    /* [in] */ BSTR toEventIn);


void __RPC_STUB Browser_deleteRoute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_createNode_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ BSTR nodeclass,
    /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB Browser_createNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_getWorld_Proxy( 
    Browser __RPC_FAR * This,
    /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB Browser_getWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_beginUpdate_Proxy( 
    Browser __RPC_FAR * This);


void __RPC_STUB Browser_beginUpdate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_endUpdate_Proxy( 
    Browser __RPC_FAR * This);


void __RPC_STUB Browser_endUpdate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_addNode_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ Node __RPC_FAR *node);


void __RPC_STUB Browser_addNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_removeNode_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ Node __RPC_FAR *node,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value);


void __RPC_STUB Browser_removeNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_setNodeName_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ Node __RPC_FAR *node,
    /* [in] */ BSTR name);


void __RPC_STUB Browser_setNodeName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_getNodeName_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ Node __RPC_FAR *node,
    /* [retval][out] */ BSTR __RPC_FAR *name);


void __RPC_STUB Browser_getNodeName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_getEventOut_Proxy( 
    Browser __RPC_FAR * This,
    BSTR eventOutName,
    /* [retval][out] */ EventOut __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB Browser_getEventOut_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_getEventIn_Proxy( 
    Browser __RPC_FAR * This,
    BSTR eventInName,
    /* [retval][out] */ EventIn __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB Browser_getEventIn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_getControlPathname_Proxy( 
    Browser __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *directoryName);


void __RPC_STUB Browser_getControlPathname_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_saveWorld_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ BSTR fileName,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value);


void __RPC_STUB Browser_saveWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_saveViewpoint_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ BSTR viewpointName);


void __RPC_STUB Browser_saveViewpoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_setObserver_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *observer,
    /* [in] */ int flags,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value);


void __RPC_STUB Browser_setObserver_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_setViewpointByValue_Proxy( 
    Browser __RPC_FAR * This,
    /* [size_is][in] */ float __RPC_FAR *position,
    /* [size_is][in] */ float __RPC_FAR *orientation,
    /* [in] */ float fov,
    /* [in] */ VARIANT_BOOL animate);


void __RPC_STUB Browser_setViewpointByValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_setFullscreen_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL mode,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value);


void __RPC_STUB Browser_setFullscreen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_setCollisionDetection_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL mode);


void __RPC_STUB Browser_setCollisionDetection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_setRenderMode_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ BSTR mode);


void __RPC_STUB Browser_setRenderMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_setNavigationMode_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ BSTR mode);


void __RPC_STUB Browser_setNavigationMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_setViewpoint_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ BSTR viewpointName);


void __RPC_STUB Browser_setViewpoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_setHeadlight_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL mode);


void __RPC_STUB Browser_setHeadlight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_setAvatarHeight_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ float value);


void __RPC_STUB Browser_setAvatarHeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_setCollisionDistance_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ float value);


void __RPC_STUB Browser_setCollisionDistance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_setVisibilityLimit_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ float value);


void __RPC_STUB Browser_setVisibilityLimit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_setWalkSpeed_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ float value);


void __RPC_STUB Browser_setWalkSpeed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_setNavigationPanel_Proxy( 
    Browser __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL mode);


void __RPC_STUB Browser_setNavigationPanel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Browser_getNavigationPanel_Proxy( 
    Browser __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value);


void __RPC_STUB Browser_getNavigationPanel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __Browser_INTERFACE_DEFINED__ */


#ifndef __Field_INTERFACE_DEFINED__
#define __Field_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: Field
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_Field;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC50398-75D9-11D0-9315-0020AFE05CC8")
    Field : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getType( 
            /* [retval][out] */ int __RPC_FAR *type) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct FieldVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            Field __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            Field __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            Field __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            Field __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            Field __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            Field __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            Field __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            Field __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        END_INTERFACE
    } FieldVtbl;

    interface Field
    {
        CONST_VTBL struct FieldVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Field_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define Field_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define Field_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define Field_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define Field_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define Field_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define Field_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define Field_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE Field_getType_Proxy( 
    Field __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *type);


void __RPC_STUB Field_getType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __Field_INTERFACE_DEFINED__ */


#ifndef __EventIn_INTERFACE_DEFINED__
#define __EventIn_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventIn
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventIn;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC5039D-75D9-11D0-9315-0020AFE05CC8")
    EventIn : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getType( 
            /* [retval][out] */ int __RPC_FAR *type) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setValueFromString( 
            /* [in] */ BSTR value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventIn __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventIn __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventIn __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventIn __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventIn __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventIn __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventIn __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventIn __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventIn __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        END_INTERFACE
    } EventInVtbl;

    interface EventIn
    {
        CONST_VTBL struct EventInVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventIn_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventIn_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventIn_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventIn_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventIn_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventIn_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventIn_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventIn_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventIn_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventIn_getType_Proxy( 
    EventIn __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *type);


void __RPC_STUB EventIn_getType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventIn_setValueFromString_Proxy( 
    EventIn __RPC_FAR * This,
    /* [in] */ BSTR value);


void __RPC_STUB EventIn_setValueFromString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventIn_INTERFACE_DEFINED__ */


#ifndef __EventOut_INTERFACE_DEFINED__
#define __EventOut_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOut
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOut;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503F7-75D9-11D0-9315-0020AFE05CC8")
    EventOut : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getType( 
            /* [retval][out] */ int __RPC_FAR *type) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE advise( 
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE unAdvise( 
            /* [in] */ EventOutObserver __RPC_FAR *observer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE toString( 
            /* [retval][out] */ BSTR __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOut __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOut __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOut __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOut __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOut __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOut __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOut __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOut __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOut __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOut __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOut __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutVtbl;

    interface EventOut
    {
        CONST_VTBL struct EventOutVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOut_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOut_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOut_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOut_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOut_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOut_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOut_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOut_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOut_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOut_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOut_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOut_getType_Proxy( 
    EventOut __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *type);


void __RPC_STUB EventOut_getType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventOut_advise_Proxy( 
    EventOut __RPC_FAR * This,
    /* [in] */ EventOutObserver __RPC_FAR *observer,
    /* [in] */ IDispatch __RPC_FAR *userData);


void __RPC_STUB EventOut_advise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventOut_unAdvise_Proxy( 
    EventOut __RPC_FAR * This,
    /* [in] */ EventOutObserver __RPC_FAR *observer);


void __RPC_STUB EventOut_unAdvise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventOut_toString_Proxy( 
    EventOut __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *value);


void __RPC_STUB EventOut_toString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOut_INTERFACE_DEFINED__ */


#ifndef __EventInSFFloat_INTERFACE_DEFINED__
#define __EventInSFFloat_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInSFFloat
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInSFFloat;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503D9-75D9-11D0-9315-0020AFE05CC8")
    EventInSFFloat : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ float value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInSFFloatVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInSFFloat __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInSFFloat __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInSFFloat __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInSFFloat __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInSFFloat __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInSFFloat __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInSFFloat __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInSFFloat __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInSFFloat __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInSFFloat __RPC_FAR * This,
            /* [in] */ float value);
        
        END_INTERFACE
    } EventInSFFloatVtbl;

    interface EventInSFFloat
    {
        CONST_VTBL struct EventInSFFloatVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInSFFloat_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInSFFloat_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInSFFloat_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInSFFloat_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInSFFloat_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInSFFloat_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInSFFloat_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInSFFloat_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInSFFloat_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInSFFloat_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInSFFloat_setValue_Proxy( 
    EventInSFFloat __RPC_FAR * This,
    /* [in] */ float value);


void __RPC_STUB EventInSFFloat_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInSFFloat_INTERFACE_DEFINED__ */


#ifndef __EventInSFBool_INTERFACE_DEFINED__
#define __EventInSFBool_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInSFBool
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInSFBool;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503CA-75D9-11D0-9315-0020AFE05CC8")
    EventInSFBool : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ VARIANT_BOOL value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInSFBoolVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInSFBool __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInSFBool __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInSFBool __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInSFBool __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInSFBool __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInSFBool __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInSFBool __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInSFBool __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInSFBool __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInSFBool __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL value);
        
        END_INTERFACE
    } EventInSFBoolVtbl;

    interface EventInSFBool
    {
        CONST_VTBL struct EventInSFBoolVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInSFBool_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInSFBool_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInSFBool_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInSFBool_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInSFBool_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInSFBool_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInSFBool_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInSFBool_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInSFBool_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInSFBool_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInSFBool_setValue_Proxy( 
    EventInSFBool __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL value);


void __RPC_STUB EventInSFBool_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInSFBool_INTERFACE_DEFINED__ */


#ifndef __EventOutMField_INTERFACE_DEFINED__
#define __EventOutMField_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutMField
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutMField;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC5040B-75D9-11D0-9315-0020AFE05CC8")
    EventOutMField : public EventOut
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getSize( 
            /* [retval][out] */ int __RPC_FAR *cnt) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutMFieldVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutMField __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutMField __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutMField __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutMField __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutMField __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutMField __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutMField __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutMField __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutMField __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutMField __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutMField __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getSize )( 
            EventOutMField __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *cnt);
        
        END_INTERFACE
    } EventOutMFieldVtbl;

    interface EventOutMField
    {
        CONST_VTBL struct EventOutMFieldVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutMField_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutMField_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutMField_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutMField_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutMField_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutMField_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutMField_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutMField_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutMField_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutMField_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutMField_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutMField_getSize(This,cnt)	\
    (This)->lpVtbl -> getSize(This,cnt)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutMField_getSize_Proxy( 
    EventOutMField __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *cnt);


void __RPC_STUB EventOutMField_getSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutMField_INTERFACE_DEFINED__ */


#ifndef __EventInMFColor_INTERFACE_DEFINED__
#define __EventInMFColor_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInMFColor
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInMFColor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503A2-75D9-11D0-9315-0020AFE05CC8")
    EventInMFColor : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ int cnt,
            /* [size_is][in] */ float __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE set1Value( 
            /* [in] */ int index,
            /* [size_is][in] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInMFColorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInMFColor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInMFColor __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInMFColor __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInMFColor __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInMFColor __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInMFColor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInMFColor __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInMFColor __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInMFColor __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInMFColor __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][in] */ float __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            EventInMFColor __RPC_FAR * This,
            /* [in] */ int index,
            /* [size_is][in] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } EventInMFColorVtbl;

    interface EventInMFColor
    {
        CONST_VTBL struct EventInMFColorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInMFColor_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInMFColor_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInMFColor_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInMFColor_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInMFColor_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInMFColor_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInMFColor_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInMFColor_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInMFColor_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInMFColor_setValue(This,cnt,value)	\
    (This)->lpVtbl -> setValue(This,cnt,value)

#define EventInMFColor_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInMFColor_setValue_Proxy( 
    EventInMFColor __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][in] */ float __RPC_FAR *value);


void __RPC_STUB EventInMFColor_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventInMFColor_set1Value_Proxy( 
    EventInMFColor __RPC_FAR * This,
    /* [in] */ int index,
    /* [size_is][in] */ float __RPC_FAR *value);


void __RPC_STUB EventInMFColor_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInMFColor_INTERFACE_DEFINED__ */


#ifndef __EventInMFFloat_INTERFACE_DEFINED__
#define __EventInMFFloat_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInMFFloat
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInMFFloat;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503A7-75D9-11D0-9315-0020AFE05CC8")
    EventInMFFloat : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ int cnt,
            /* [size_is][in] */ float __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE set1Value( 
            /* [in] */ int index,
            /* [in] */ float value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInMFFloatVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInMFFloat __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInMFFloat __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInMFFloat __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInMFFloat __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInMFFloat __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInMFFloat __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInMFFloat __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInMFFloat __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInMFFloat __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInMFFloat __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][in] */ float __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            EventInMFFloat __RPC_FAR * This,
            /* [in] */ int index,
            /* [in] */ float value);
        
        END_INTERFACE
    } EventInMFFloatVtbl;

    interface EventInMFFloat
    {
        CONST_VTBL struct EventInMFFloatVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInMFFloat_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInMFFloat_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInMFFloat_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInMFFloat_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInMFFloat_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInMFFloat_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInMFFloat_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInMFFloat_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInMFFloat_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInMFFloat_setValue(This,cnt,value)	\
    (This)->lpVtbl -> setValue(This,cnt,value)

#define EventInMFFloat_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInMFFloat_setValue_Proxy( 
    EventInMFFloat __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][in] */ float __RPC_FAR *value);


void __RPC_STUB EventInMFFloat_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventInMFFloat_set1Value_Proxy( 
    EventInMFFloat __RPC_FAR * This,
    /* [in] */ int index,
    /* [in] */ float value);


void __RPC_STUB EventInMFFloat_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInMFFloat_INTERFACE_DEFINED__ */


#ifndef __EventInMFTime_INTERFACE_DEFINED__
#define __EventInMFTime_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInMFTime
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInMFTime;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("C5EEE9A1-6771-11d2-ABB2-00A0C9C7B4A1")
    EventInMFTime : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ int cnt,
            /* [size_is][in] */ double __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE set1Value( 
            /* [in] */ int index,
            /* [in] */ double value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInMFTimeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInMFTime __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInMFTime __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInMFTime __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInMFTime __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInMFTime __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInMFTime __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInMFTime __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInMFTime __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInMFTime __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInMFTime __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][in] */ double __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            EventInMFTime __RPC_FAR * This,
            /* [in] */ int index,
            /* [in] */ double value);
        
        END_INTERFACE
    } EventInMFTimeVtbl;

    interface EventInMFTime
    {
        CONST_VTBL struct EventInMFTimeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInMFTime_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInMFTime_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInMFTime_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInMFTime_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInMFTime_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInMFTime_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInMFTime_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInMFTime_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInMFTime_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInMFTime_setValue(This,cnt,value)	\
    (This)->lpVtbl -> setValue(This,cnt,value)

#define EventInMFTime_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInMFTime_setValue_Proxy( 
    EventInMFTime __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][in] */ double __RPC_FAR *value);


void __RPC_STUB EventInMFTime_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventInMFTime_set1Value_Proxy( 
    EventInMFTime __RPC_FAR * This,
    /* [in] */ int index,
    /* [in] */ double value);


void __RPC_STUB EventInMFTime_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInMFTime_INTERFACE_DEFINED__ */


#ifndef __EventInMFInt32_INTERFACE_DEFINED__
#define __EventInMFInt32_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInMFInt32
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInMFInt32;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503AC-75D9-11D0-9315-0020AFE05CC8")
    EventInMFInt32 : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ int cnt,
            /* [size_is][in] */ int __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE set1Value( 
            /* [in] */ int index,
            /* [in] */ int value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInMFInt32Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInMFInt32 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInMFInt32 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInMFInt32 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInMFInt32 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInMFInt32 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInMFInt32 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInMFInt32 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInMFInt32 __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInMFInt32 __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInMFInt32 __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][in] */ int __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            EventInMFInt32 __RPC_FAR * This,
            /* [in] */ int index,
            /* [in] */ int value);
        
        END_INTERFACE
    } EventInMFInt32Vtbl;

    interface EventInMFInt32
    {
        CONST_VTBL struct EventInMFInt32Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInMFInt32_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInMFInt32_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInMFInt32_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInMFInt32_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInMFInt32_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInMFInt32_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInMFInt32_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInMFInt32_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInMFInt32_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInMFInt32_setValue(This,cnt,value)	\
    (This)->lpVtbl -> setValue(This,cnt,value)

#define EventInMFInt32_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInMFInt32_setValue_Proxy( 
    EventInMFInt32 __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][in] */ int __RPC_FAR *value);


void __RPC_STUB EventInMFInt32_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventInMFInt32_set1Value_Proxy( 
    EventInMFInt32 __RPC_FAR * This,
    /* [in] */ int index,
    /* [in] */ int value);


void __RPC_STUB EventInMFInt32_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInMFInt32_INTERFACE_DEFINED__ */


#ifndef __EventInMFNode_INTERFACE_DEFINED__
#define __EventInMFNode_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInMFNode
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInMFNode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503B1-75D9-11D0-9315-0020AFE05CC8")
    EventInMFNode : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ IDispatch __RPC_FAR *__RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE set1Value( 
            /* [in] */ int index,
            /* [in] */ Node __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInMFNodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInMFNode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInMFNode __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInMFNode __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInMFNode __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInMFNode __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInMFNode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInMFNode __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInMFNode __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInMFNode __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInMFNode __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ IDispatch __RPC_FAR *__RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            EventInMFNode __RPC_FAR * This,
            /* [in] */ int index,
            /* [in] */ Node __RPC_FAR *value);
        
        END_INTERFACE
    } EventInMFNodeVtbl;

    interface EventInMFNode
    {
        CONST_VTBL struct EventInMFNodeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInMFNode_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInMFNode_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInMFNode_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInMFNode_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInMFNode_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInMFNode_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInMFNode_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInMFNode_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInMFNode_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInMFNode_setValue(This,cnt,value)	\
    (This)->lpVtbl -> setValue(This,cnt,value)

#define EventInMFNode_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInMFNode_setValue_Proxy( 
    EventInMFNode __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ IDispatch __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB EventInMFNode_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventInMFNode_set1Value_Proxy( 
    EventInMFNode __RPC_FAR * This,
    /* [in] */ int index,
    /* [in] */ Node __RPC_FAR *value);


void __RPC_STUB EventInMFNode_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInMFNode_INTERFACE_DEFINED__ */


#ifndef __EventInMFRotation_INTERFACE_DEFINED__
#define __EventInMFRotation_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInMFRotation
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInMFRotation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503B6-75D9-11D0-9315-0020AFE05CC8")
    EventInMFRotation : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ int cnt,
            /* [size_is][in] */ float __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE set1Value( 
            /* [in] */ int index,
            /* [size_is][in] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInMFRotationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInMFRotation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInMFRotation __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInMFRotation __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInMFRotation __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInMFRotation __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInMFRotation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInMFRotation __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInMFRotation __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInMFRotation __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInMFRotation __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][in] */ float __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            EventInMFRotation __RPC_FAR * This,
            /* [in] */ int index,
            /* [size_is][in] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } EventInMFRotationVtbl;

    interface EventInMFRotation
    {
        CONST_VTBL struct EventInMFRotationVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInMFRotation_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInMFRotation_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInMFRotation_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInMFRotation_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInMFRotation_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInMFRotation_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInMFRotation_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInMFRotation_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInMFRotation_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInMFRotation_setValue(This,cnt,value)	\
    (This)->lpVtbl -> setValue(This,cnt,value)

#define EventInMFRotation_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInMFRotation_setValue_Proxy( 
    EventInMFRotation __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][in] */ float __RPC_FAR *value);


void __RPC_STUB EventInMFRotation_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventInMFRotation_set1Value_Proxy( 
    EventInMFRotation __RPC_FAR * This,
    /* [in] */ int index,
    /* [size_is][in] */ float __RPC_FAR *value);


void __RPC_STUB EventInMFRotation_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInMFRotation_INTERFACE_DEFINED__ */


#ifndef __EventInMFString_INTERFACE_DEFINED__
#define __EventInMFString_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInMFString
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInMFString;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503BB-75D9-11D0-9315-0020AFE05CC8")
    EventInMFString : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ int cnt,
            /* [size_is][in] */ BSTR __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE set1Value( 
            /* [in] */ int index,
            /* [in] */ BSTR value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInMFStringVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInMFString __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInMFString __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInMFString __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInMFString __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInMFString __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInMFString __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInMFString __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInMFString __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInMFString __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInMFString __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][in] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            EventInMFString __RPC_FAR * This,
            /* [in] */ int index,
            /* [in] */ BSTR value);
        
        END_INTERFACE
    } EventInMFStringVtbl;

    interface EventInMFString
    {
        CONST_VTBL struct EventInMFStringVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInMFString_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInMFString_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInMFString_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInMFString_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInMFString_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInMFString_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInMFString_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInMFString_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInMFString_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInMFString_setValue(This,cnt,value)	\
    (This)->lpVtbl -> setValue(This,cnt,value)

#define EventInMFString_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInMFString_setValue_Proxy( 
    EventInMFString __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][in] */ BSTR __RPC_FAR *value);


void __RPC_STUB EventInMFString_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventInMFString_set1Value_Proxy( 
    EventInMFString __RPC_FAR * This,
    /* [in] */ int index,
    /* [in] */ BSTR value);


void __RPC_STUB EventInMFString_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInMFString_INTERFACE_DEFINED__ */


#ifndef __EventInMFVec2f_INTERFACE_DEFINED__
#define __EventInMFVec2f_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInMFVec2f
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInMFVec2f;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503C0-75D9-11D0-9315-0020AFE05CC8")
    EventInMFVec2f : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ int cnt,
            /* [size_is][in] */ float __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE set1Value( 
            /* [in] */ int index,
            /* [size_is][in] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInMFVec2fVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInMFVec2f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInMFVec2f __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInMFVec2f __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInMFVec2f __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInMFVec2f __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInMFVec2f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInMFVec2f __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInMFVec2f __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInMFVec2f __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInMFVec2f __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][in] */ float __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            EventInMFVec2f __RPC_FAR * This,
            /* [in] */ int index,
            /* [size_is][in] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } EventInMFVec2fVtbl;

    interface EventInMFVec2f
    {
        CONST_VTBL struct EventInMFVec2fVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInMFVec2f_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInMFVec2f_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInMFVec2f_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInMFVec2f_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInMFVec2f_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInMFVec2f_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInMFVec2f_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInMFVec2f_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInMFVec2f_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInMFVec2f_setValue(This,cnt,value)	\
    (This)->lpVtbl -> setValue(This,cnt,value)

#define EventInMFVec2f_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInMFVec2f_setValue_Proxy( 
    EventInMFVec2f __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][in] */ float __RPC_FAR *value);


void __RPC_STUB EventInMFVec2f_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventInMFVec2f_set1Value_Proxy( 
    EventInMFVec2f __RPC_FAR * This,
    /* [in] */ int index,
    /* [size_is][in] */ float __RPC_FAR *value);


void __RPC_STUB EventInMFVec2f_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInMFVec2f_INTERFACE_DEFINED__ */


#ifndef __EventInMFVec3f_INTERFACE_DEFINED__
#define __EventInMFVec3f_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInMFVec3f
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInMFVec3f;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503C5-75D9-11D0-9315-0020AFE05CC8")
    EventInMFVec3f : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ int cnt,
            /* [size_is][in] */ float __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE set1Value( 
            /* [in] */ int index,
            /* [size_is][in] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInMFVec3fVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInMFVec3f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInMFVec3f __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInMFVec3f __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInMFVec3f __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInMFVec3f __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInMFVec3f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInMFVec3f __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInMFVec3f __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInMFVec3f __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInMFVec3f __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][in] */ float __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *set1Value )( 
            EventInMFVec3f __RPC_FAR * This,
            /* [in] */ int index,
            /* [size_is][in] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } EventInMFVec3fVtbl;

    interface EventInMFVec3f
    {
        CONST_VTBL struct EventInMFVec3fVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInMFVec3f_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInMFVec3f_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInMFVec3f_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInMFVec3f_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInMFVec3f_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInMFVec3f_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInMFVec3f_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInMFVec3f_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInMFVec3f_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInMFVec3f_setValue(This,cnt,value)	\
    (This)->lpVtbl -> setValue(This,cnt,value)

#define EventInMFVec3f_set1Value(This,index,value)	\
    (This)->lpVtbl -> set1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInMFVec3f_setValue_Proxy( 
    EventInMFVec3f __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][in] */ float __RPC_FAR *value);


void __RPC_STUB EventInMFVec3f_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventInMFVec3f_set1Value_Proxy( 
    EventInMFVec3f __RPC_FAR * This,
    /* [in] */ int index,
    /* [size_is][in] */ float __RPC_FAR *value);


void __RPC_STUB EventInMFVec3f_set1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInMFVec3f_INTERFACE_DEFINED__ */


#ifndef __EventInSFColor_INTERFACE_DEFINED__
#define __EventInSFColor_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInSFColor
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInSFColor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503CF-75D9-11D0-9315-0020AFE05CC8")
    EventInSFColor : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [size_is][in] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInSFColorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInSFColor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInSFColor __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInSFColor __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInSFColor __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInSFColor __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInSFColor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInSFColor __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInSFColor __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInSFColor __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInSFColor __RPC_FAR * This,
            /* [size_is][in] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } EventInSFColorVtbl;

    interface EventInSFColor
    {
        CONST_VTBL struct EventInSFColorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInSFColor_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInSFColor_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInSFColor_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInSFColor_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInSFColor_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInSFColor_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInSFColor_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInSFColor_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInSFColor_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInSFColor_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInSFColor_setValue_Proxy( 
    EventInSFColor __RPC_FAR * This,
    /* [size_is][in] */ float __RPC_FAR *value);


void __RPC_STUB EventInSFColor_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInSFColor_INTERFACE_DEFINED__ */


#ifndef __EventInSFImage_INTERFACE_DEFINED__
#define __EventInSFImage_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInSFImage
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInSFImage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503DE-75D9-11D0-9315-0020AFE05CC8")
    EventInSFImage : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ int width,
            /* [in] */ int height,
            /* [in] */ int numComponents,
            /* [in] */ int cnt,
            /* [size_is][in] */ unsigned small __RPC_FAR *pixels) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInSFImageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInSFImage __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInSFImage __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInSFImage __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInSFImage __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInSFImage __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInSFImage __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInSFImage __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInSFImage __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInSFImage __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInSFImage __RPC_FAR * This,
            /* [in] */ int width,
            /* [in] */ int height,
            /* [in] */ int numComponents,
            /* [in] */ int cnt,
            /* [size_is][in] */ unsigned small __RPC_FAR *pixels);
        
        END_INTERFACE
    } EventInSFImageVtbl;

    interface EventInSFImage
    {
        CONST_VTBL struct EventInSFImageVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInSFImage_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInSFImage_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInSFImage_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInSFImage_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInSFImage_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInSFImage_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInSFImage_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInSFImage_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInSFImage_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInSFImage_setValue(This,width,height,numComponents,cnt,pixels)	\
    (This)->lpVtbl -> setValue(This,width,height,numComponents,cnt,pixels)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInSFImage_setValue_Proxy( 
    EventInSFImage __RPC_FAR * This,
    /* [in] */ int width,
    /* [in] */ int height,
    /* [in] */ int numComponents,
    /* [in] */ int cnt,
    /* [size_is][in] */ unsigned small __RPC_FAR *pixels);


void __RPC_STUB EventInSFImage_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInSFImage_INTERFACE_DEFINED__ */


#ifndef __EventInSFInt32_INTERFACE_DEFINED__
#define __EventInSFInt32_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInSFInt32
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInSFInt32;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503E3-75D9-11D0-9315-0020AFE05CC8")
    EventInSFInt32 : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ int value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInSFInt32Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInSFInt32 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInSFInt32 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInSFInt32 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInSFInt32 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInSFInt32 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInSFInt32 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInSFInt32 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInSFInt32 __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInSFInt32 __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInSFInt32 __RPC_FAR * This,
            /* [in] */ int value);
        
        END_INTERFACE
    } EventInSFInt32Vtbl;

    interface EventInSFInt32
    {
        CONST_VTBL struct EventInSFInt32Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInSFInt32_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInSFInt32_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInSFInt32_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInSFInt32_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInSFInt32_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInSFInt32_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInSFInt32_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInSFInt32_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInSFInt32_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInSFInt32_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInSFInt32_setValue_Proxy( 
    EventInSFInt32 __RPC_FAR * This,
    /* [in] */ int value);


void __RPC_STUB EventInSFInt32_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInSFInt32_INTERFACE_DEFINED__ */


#ifndef __EventInSFNode_INTERFACE_DEFINED__
#define __EventInSFNode_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInSFNode
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInSFNode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503E8-75D9-11D0-9315-0020AFE05CC8")
    EventInSFNode : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ Node __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInSFNodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInSFNode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInSFNode __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInSFNode __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInSFNode __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInSFNode __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInSFNode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInSFNode __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInSFNode __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInSFNode __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInSFNode __RPC_FAR * This,
            /* [in] */ Node __RPC_FAR *value);
        
        END_INTERFACE
    } EventInSFNodeVtbl;

    interface EventInSFNode
    {
        CONST_VTBL struct EventInSFNodeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInSFNode_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInSFNode_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInSFNode_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInSFNode_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInSFNode_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInSFNode_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInSFNode_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInSFNode_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInSFNode_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInSFNode_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInSFNode_setValue_Proxy( 
    EventInSFNode __RPC_FAR * This,
    /* [in] */ Node __RPC_FAR *value);


void __RPC_STUB EventInSFNode_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInSFNode_INTERFACE_DEFINED__ */


#ifndef __EventInSFRotation_INTERFACE_DEFINED__
#define __EventInSFRotation_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInSFRotation
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInSFRotation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503ED-75D9-11D0-9315-0020AFE05CC8")
    EventInSFRotation : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [size_is][in] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInSFRotationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInSFRotation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInSFRotation __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInSFRotation __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInSFRotation __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInSFRotation __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInSFRotation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInSFRotation __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInSFRotation __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInSFRotation __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInSFRotation __RPC_FAR * This,
            /* [size_is][in] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } EventInSFRotationVtbl;

    interface EventInSFRotation
    {
        CONST_VTBL struct EventInSFRotationVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInSFRotation_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInSFRotation_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInSFRotation_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInSFRotation_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInSFRotation_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInSFRotation_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInSFRotation_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInSFRotation_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInSFRotation_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInSFRotation_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInSFRotation_setValue_Proxy( 
    EventInSFRotation __RPC_FAR * This,
    /* [size_is][in] */ float __RPC_FAR *value);


void __RPC_STUB EventInSFRotation_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInSFRotation_INTERFACE_DEFINED__ */


#ifndef __EventInSFString_INTERFACE_DEFINED__
#define __EventInSFString_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInSFString
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInSFString;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503F2-75D9-11D0-9315-0020AFE05CC8")
    EventInSFString : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ BSTR value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInSFStringVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInSFString __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInSFString __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInSFString __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInSFString __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInSFString __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInSFString __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInSFString __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInSFString __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInSFString __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInSFString __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        END_INTERFACE
    } EventInSFStringVtbl;

    interface EventInSFString
    {
        CONST_VTBL struct EventInSFStringVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInSFString_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInSFString_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInSFString_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInSFString_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInSFString_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInSFString_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInSFString_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInSFString_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInSFString_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInSFString_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInSFString_setValue_Proxy( 
    EventInSFString __RPC_FAR * This,
    /* [in] */ BSTR value);


void __RPC_STUB EventInSFString_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInSFString_INTERFACE_DEFINED__ */


#ifndef __EventInSFTime_INTERFACE_DEFINED__
#define __EventInSFTime_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInSFTime
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInSFTime;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503D4-75D9-11D0-9315-0020AFE05CC8")
    EventInSFTime : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [in] */ double value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInSFTimeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInSFTime __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInSFTime __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInSFTime __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInSFTime __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInSFTime __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInSFTime __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInSFTime __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInSFTime __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInSFTime __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInSFTime __RPC_FAR * This,
            /* [in] */ double value);
        
        END_INTERFACE
    } EventInSFTimeVtbl;

    interface EventInSFTime
    {
        CONST_VTBL struct EventInSFTimeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInSFTime_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInSFTime_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInSFTime_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInSFTime_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInSFTime_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInSFTime_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInSFTime_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInSFTime_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInSFTime_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInSFTime_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInSFTime_setValue_Proxy( 
    EventInSFTime __RPC_FAR * This,
    /* [in] */ double value);


void __RPC_STUB EventInSFTime_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInSFTime_INTERFACE_DEFINED__ */


#ifndef __EventInSFVec2f_INTERFACE_DEFINED__
#define __EventInSFVec2f_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInSFVec2f
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInSFVec2f;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC503FC-75D9-11D0-9315-0020AFE05CC8")
    EventInSFVec2f : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [size_is][in] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInSFVec2fVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInSFVec2f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInSFVec2f __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInSFVec2f __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInSFVec2f __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInSFVec2f __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInSFVec2f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInSFVec2f __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInSFVec2f __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInSFVec2f __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInSFVec2f __RPC_FAR * This,
            /* [size_is][in] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } EventInSFVec2fVtbl;

    interface EventInSFVec2f
    {
        CONST_VTBL struct EventInSFVec2fVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInSFVec2f_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInSFVec2f_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInSFVec2f_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInSFVec2f_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInSFVec2f_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInSFVec2f_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInSFVec2f_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInSFVec2f_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInSFVec2f_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInSFVec2f_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInSFVec2f_setValue_Proxy( 
    EventInSFVec2f __RPC_FAR * This,
    /* [size_is][in] */ float __RPC_FAR *value);


void __RPC_STUB EventInSFVec2f_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInSFVec2f_INTERFACE_DEFINED__ */


#ifndef __EventInSFVec3f_INTERFACE_DEFINED__
#define __EventInSFVec3f_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventInSFVec3f
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventInSFVec3f;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC50401-75D9-11D0-9315-0020AFE05CC8")
    EventInSFVec3f : public EventIn
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setValue( 
            /* [size_is][in] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventInSFVec3fVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventInSFVec3f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventInSFVec3f __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventInSFVec3f __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventInSFVec3f __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventInSFVec3f __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventInSFVec3f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventInSFVec3f __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventInSFVec3f __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValueFromString )( 
            EventInSFVec3f __RPC_FAR * This,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setValue )( 
            EventInSFVec3f __RPC_FAR * This,
            /* [size_is][in] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } EventInSFVec3fVtbl;

    interface EventInSFVec3f
    {
        CONST_VTBL struct EventInSFVec3fVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventInSFVec3f_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventInSFVec3f_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventInSFVec3f_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventInSFVec3f_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventInSFVec3f_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventInSFVec3f_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventInSFVec3f_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventInSFVec3f_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventInSFVec3f_setValueFromString(This,value)	\
    (This)->lpVtbl -> setValueFromString(This,value)


#define EventInSFVec3f_setValue(This,value)	\
    (This)->lpVtbl -> setValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventInSFVec3f_setValue_Proxy( 
    EventInSFVec3f __RPC_FAR * This,
    /* [size_is][in] */ float __RPC_FAR *value);


void __RPC_STUB EventInSFVec3f_setValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventInSFVec3f_INTERFACE_DEFINED__ */


#ifndef __EventOutObserver_INTERFACE_DEFINED__
#define __EventOutObserver_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutObserver
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutObserver;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC50406-75D9-11D0-9315-0020AFE05CC8")
    EventOutObserver : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE callback( 
            /* [in] */ EventOut __RPC_FAR *value,
            /* [in] */ double timeStamp,
            /* [in] */ IDispatch __RPC_FAR *userData) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutObserverVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutObserver __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutObserver __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutObserver __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutObserver __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutObserver __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutObserver __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutObserver __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *callback )( 
            EventOutObserver __RPC_FAR * This,
            /* [in] */ EventOut __RPC_FAR *value,
            /* [in] */ double timeStamp,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        END_INTERFACE
    } EventOutObserverVtbl;

    interface EventOutObserver
    {
        CONST_VTBL struct EventOutObserverVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutObserver_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutObserver_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutObserver_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutObserver_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutObserver_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutObserver_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutObserver_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutObserver_callback(This,value,timeStamp,userData)	\
    (This)->lpVtbl -> callback(This,value,timeStamp,userData)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutObserver_callback_Proxy( 
    EventOutObserver __RPC_FAR * This,
    /* [in] */ EventOut __RPC_FAR *value,
    /* [in] */ double timeStamp,
    /* [in] */ IDispatch __RPC_FAR *userData);


void __RPC_STUB EventOutObserver_callback_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutObserver_INTERFACE_DEFINED__ */


#ifndef __EventOutMFColor_INTERFACE_DEFINED__
#define __EventOutMFColor_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutMFColor
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutMFColor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC50410-75D9-11D0-9315-0020AFE05CC8")
    EventOutMFColor : public EventOutMField
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutMFColorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutMFColor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutMFColor __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutMFColor __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutMFColor __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutMFColor __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutMFColor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutMFColor __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutMFColor __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutMFColor __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutMFColor __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutMFColor __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getSize )( 
            EventOutMFColor __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *cnt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutMFColor __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            EventOutMFColor __RPC_FAR * This,
            /* [in] */ int index,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutMFColorVtbl;

    interface EventOutMFColor
    {
        CONST_VTBL struct EventOutMFColorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutMFColor_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutMFColor_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutMFColor_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutMFColor_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutMFColor_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutMFColor_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutMFColor_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutMFColor_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutMFColor_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutMFColor_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutMFColor_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutMFColor_getSize(This,cnt)	\
    (This)->lpVtbl -> getSize(This,cnt)


#define EventOutMFColor_getValue(This,cnt,value)	\
    (This)->lpVtbl -> getValue(This,cnt,value)

#define EventOutMFColor_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutMFColor_getValue_Proxy( 
    EventOutMFColor __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB EventOutMFColor_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventOutMFColor_get1Value_Proxy( 
    EventOutMFColor __RPC_FAR * This,
    /* [in] */ int index,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB EventOutMFColor_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutMFColor_INTERFACE_DEFINED__ */


#ifndef __EventOutMFFloat_INTERFACE_DEFINED__
#define __EventOutMFFloat_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutMFFloat
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutMFFloat;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC50415-75D9-11D0-9315-0020AFE05CC8")
    EventOutMFFloat : public EventOutMField
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [retval][out] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutMFFloatVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutMFFloat __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutMFFloat __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutMFFloat __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutMFFloat __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutMFFloat __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutMFFloat __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutMFFloat __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutMFFloat __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutMFFloat __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutMFFloat __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutMFFloat __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getSize )( 
            EventOutMFFloat __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *cnt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutMFFloat __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            EventOutMFFloat __RPC_FAR * This,
            /* [in] */ int index,
            /* [retval][out] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutMFFloatVtbl;

    interface EventOutMFFloat
    {
        CONST_VTBL struct EventOutMFFloatVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutMFFloat_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutMFFloat_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutMFFloat_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutMFFloat_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutMFFloat_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutMFFloat_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutMFFloat_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutMFFloat_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutMFFloat_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutMFFloat_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutMFFloat_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutMFFloat_getSize(This,cnt)	\
    (This)->lpVtbl -> getSize(This,cnt)


#define EventOutMFFloat_getValue(This,cnt,value)	\
    (This)->lpVtbl -> getValue(This,cnt,value)

#define EventOutMFFloat_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutMFFloat_getValue_Proxy( 
    EventOutMFFloat __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB EventOutMFFloat_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventOutMFFloat_get1Value_Proxy( 
    EventOutMFFloat __RPC_FAR * This,
    /* [in] */ int index,
    /* [retval][out] */ float __RPC_FAR *value);


void __RPC_STUB EventOutMFFloat_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutMFFloat_INTERFACE_DEFINED__ */


#ifndef __EventOutMFTime_INTERFACE_DEFINED__
#define __EventOutMFTime_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutMFTime
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutMFTime;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("C5EEE9A0-6771-11d2-ABB2-00A0C9C7B4A1")
    EventOutMFTime : public EventOutMField
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ double __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [retval][out] */ double __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutMFTimeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutMFTime __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutMFTime __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutMFTime __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutMFTime __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutMFTime __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutMFTime __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutMFTime __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutMFTime __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutMFTime __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutMFTime __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutMFTime __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getSize )( 
            EventOutMFTime __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *cnt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutMFTime __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ double __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            EventOutMFTime __RPC_FAR * This,
            /* [in] */ int index,
            /* [retval][out] */ double __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutMFTimeVtbl;

    interface EventOutMFTime
    {
        CONST_VTBL struct EventOutMFTimeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutMFTime_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutMFTime_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutMFTime_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutMFTime_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutMFTime_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutMFTime_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutMFTime_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutMFTime_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutMFTime_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutMFTime_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutMFTime_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutMFTime_getSize(This,cnt)	\
    (This)->lpVtbl -> getSize(This,cnt)


#define EventOutMFTime_getValue(This,cnt,value)	\
    (This)->lpVtbl -> getValue(This,cnt,value)

#define EventOutMFTime_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutMFTime_getValue_Proxy( 
    EventOutMFTime __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ double __RPC_FAR *value);


void __RPC_STUB EventOutMFTime_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventOutMFTime_get1Value_Proxy( 
    EventOutMFTime __RPC_FAR * This,
    /* [in] */ int index,
    /* [retval][out] */ double __RPC_FAR *value);


void __RPC_STUB EventOutMFTime_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutMFTime_INTERFACE_DEFINED__ */


#ifndef __EventOutMFInt32_INTERFACE_DEFINED__
#define __EventOutMFInt32_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutMFInt32
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutMFInt32;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC5041A-75D9-11D0-9315-0020AFE05CC8")
    EventOutMFInt32 : public EventOutMField
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ int __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [retval][out] */ int __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutMFInt32Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutMFInt32 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutMFInt32 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutMFInt32 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutMFInt32 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutMFInt32 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutMFInt32 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutMFInt32 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutMFInt32 __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutMFInt32 __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutMFInt32 __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutMFInt32 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getSize )( 
            EventOutMFInt32 __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *cnt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutMFInt32 __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ int __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            EventOutMFInt32 __RPC_FAR * This,
            /* [in] */ int index,
            /* [retval][out] */ int __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutMFInt32Vtbl;

    interface EventOutMFInt32
    {
        CONST_VTBL struct EventOutMFInt32Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutMFInt32_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutMFInt32_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutMFInt32_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutMFInt32_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutMFInt32_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutMFInt32_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutMFInt32_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutMFInt32_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutMFInt32_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutMFInt32_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutMFInt32_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutMFInt32_getSize(This,cnt)	\
    (This)->lpVtbl -> getSize(This,cnt)


#define EventOutMFInt32_getValue(This,cnt,value)	\
    (This)->lpVtbl -> getValue(This,cnt,value)

#define EventOutMFInt32_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutMFInt32_getValue_Proxy( 
    EventOutMFInt32 __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ int __RPC_FAR *value);


void __RPC_STUB EventOutMFInt32_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventOutMFInt32_get1Value_Proxy( 
    EventOutMFInt32 __RPC_FAR * This,
    /* [in] */ int index,
    /* [retval][out] */ int __RPC_FAR *value);


void __RPC_STUB EventOutMFInt32_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutMFInt32_INTERFACE_DEFINED__ */


#ifndef __EventOutMFNode_INTERFACE_DEFINED__
#define __EventOutMFNode_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutMFNode
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutMFNode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC5041F-75D9-11D0-9315-0020AFE05CC8")
    EventOutMFNode : public EventOutMField
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ IDispatch __RPC_FAR *__RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutMFNodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutMFNode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutMFNode __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutMFNode __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutMFNode __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutMFNode __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutMFNode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutMFNode __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutMFNode __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutMFNode __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutMFNode __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutMFNode __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getSize )( 
            EventOutMFNode __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *cnt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutMFNode __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ IDispatch __RPC_FAR *__RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            EventOutMFNode __RPC_FAR * This,
            /* [in] */ int index,
            /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *value);
        
        END_INTERFACE
    } EventOutMFNodeVtbl;

    interface EventOutMFNode
    {
        CONST_VTBL struct EventOutMFNodeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutMFNode_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutMFNode_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutMFNode_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutMFNode_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutMFNode_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutMFNode_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutMFNode_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutMFNode_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutMFNode_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutMFNode_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutMFNode_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutMFNode_getSize(This,cnt)	\
    (This)->lpVtbl -> getSize(This,cnt)


#define EventOutMFNode_getValue(This,cnt,value)	\
    (This)->lpVtbl -> getValue(This,cnt,value)

#define EventOutMFNode_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutMFNode_getValue_Proxy( 
    EventOutMFNode __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ IDispatch __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB EventOutMFNode_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventOutMFNode_get1Value_Proxy( 
    EventOutMFNode __RPC_FAR * This,
    /* [in] */ int index,
    /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB EventOutMFNode_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutMFNode_INTERFACE_DEFINED__ */


#ifndef __EventOutMFRotation_INTERFACE_DEFINED__
#define __EventOutMFRotation_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutMFRotation
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutMFRotation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC50424-75D9-11D0-9315-0020AFE05CC8")
    EventOutMFRotation : public EventOutMField
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutMFRotationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutMFRotation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutMFRotation __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutMFRotation __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutMFRotation __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutMFRotation __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutMFRotation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutMFRotation __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutMFRotation __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutMFRotation __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutMFRotation __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutMFRotation __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getSize )( 
            EventOutMFRotation __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *cnt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutMFRotation __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            EventOutMFRotation __RPC_FAR * This,
            /* [in] */ int index,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutMFRotationVtbl;

    interface EventOutMFRotation
    {
        CONST_VTBL struct EventOutMFRotationVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutMFRotation_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutMFRotation_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutMFRotation_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutMFRotation_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutMFRotation_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutMFRotation_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutMFRotation_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutMFRotation_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutMFRotation_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutMFRotation_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutMFRotation_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutMFRotation_getSize(This,cnt)	\
    (This)->lpVtbl -> getSize(This,cnt)


#define EventOutMFRotation_getValue(This,cnt,value)	\
    (This)->lpVtbl -> getValue(This,cnt,value)

#define EventOutMFRotation_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutMFRotation_getValue_Proxy( 
    EventOutMFRotation __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB EventOutMFRotation_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventOutMFRotation_get1Value_Proxy( 
    EventOutMFRotation __RPC_FAR * This,
    /* [in] */ int index,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB EventOutMFRotation_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutMFRotation_INTERFACE_DEFINED__ */


#ifndef __EventOutMFString_INTERFACE_DEFINED__
#define __EventOutMFString_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutMFString
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutMFString;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC50429-75D9-11D0-9315-0020AFE05CC8")
    EventOutMFString : public EventOutMField
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ BSTR __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [retval][out] */ BSTR __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutMFStringVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutMFString __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutMFString __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutMFString __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutMFString __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutMFString __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutMFString __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutMFString __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutMFString __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutMFString __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutMFString __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutMFString __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getSize )( 
            EventOutMFString __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *cnt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutMFString __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            EventOutMFString __RPC_FAR * This,
            /* [in] */ int index,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutMFStringVtbl;

    interface EventOutMFString
    {
        CONST_VTBL struct EventOutMFStringVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutMFString_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutMFString_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutMFString_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutMFString_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutMFString_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutMFString_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutMFString_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutMFString_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutMFString_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutMFString_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutMFString_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutMFString_getSize(This,cnt)	\
    (This)->lpVtbl -> getSize(This,cnt)


#define EventOutMFString_getValue(This,cnt,value)	\
    (This)->lpVtbl -> getValue(This,cnt,value)

#define EventOutMFString_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutMFString_getValue_Proxy( 
    EventOutMFString __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ BSTR __RPC_FAR *value);


void __RPC_STUB EventOutMFString_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventOutMFString_get1Value_Proxy( 
    EventOutMFString __RPC_FAR * This,
    /* [in] */ int index,
    /* [retval][out] */ BSTR __RPC_FAR *value);


void __RPC_STUB EventOutMFString_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutMFString_INTERFACE_DEFINED__ */


#ifndef __EventOutMFVec2f_INTERFACE_DEFINED__
#define __EventOutMFVec2f_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutMFVec2f
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutMFVec2f;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC5042E-75D9-11D0-9315-0020AFE05CC8")
    EventOutMFVec2f : public EventOutMField
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutMFVec2fVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutMFVec2f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutMFVec2f __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutMFVec2f __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutMFVec2f __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutMFVec2f __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutMFVec2f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutMFVec2f __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutMFVec2f __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutMFVec2f __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutMFVec2f __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutMFVec2f __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getSize )( 
            EventOutMFVec2f __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *cnt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutMFVec2f __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            EventOutMFVec2f __RPC_FAR * This,
            /* [in] */ int index,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutMFVec2fVtbl;

    interface EventOutMFVec2f
    {
        CONST_VTBL struct EventOutMFVec2fVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutMFVec2f_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutMFVec2f_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutMFVec2f_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutMFVec2f_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutMFVec2f_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutMFVec2f_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutMFVec2f_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutMFVec2f_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutMFVec2f_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutMFVec2f_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutMFVec2f_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutMFVec2f_getSize(This,cnt)	\
    (This)->lpVtbl -> getSize(This,cnt)


#define EventOutMFVec2f_getValue(This,cnt,value)	\
    (This)->lpVtbl -> getValue(This,cnt,value)

#define EventOutMFVec2f_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutMFVec2f_getValue_Proxy( 
    EventOutMFVec2f __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB EventOutMFVec2f_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventOutMFVec2f_get1Value_Proxy( 
    EventOutMFVec2f __RPC_FAR * This,
    /* [in] */ int index,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB EventOutMFVec2f_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutMFVec2f_INTERFACE_DEFINED__ */


#ifndef __EventOutMFVec3f_INTERFACE_DEFINED__
#define __EventOutMFVec3f_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutMFVec3f
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutMFVec3f;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC50433-75D9-11D0-9315-0020AFE05CC8")
    EventOutMFVec3f : public EventOutMField
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get1Value( 
            /* [in] */ int index,
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutMFVec3fVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutMFVec3f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutMFVec3f __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutMFVec3f __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutMFVec3f __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutMFVec3f __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutMFVec3f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutMFVec3f __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutMFVec3f __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutMFVec3f __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutMFVec3f __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutMFVec3f __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getSize )( 
            EventOutMFVec3f __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *cnt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutMFVec3f __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get1Value )( 
            EventOutMFVec3f __RPC_FAR * This,
            /* [in] */ int index,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutMFVec3fVtbl;

    interface EventOutMFVec3f
    {
        CONST_VTBL struct EventOutMFVec3fVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutMFVec3f_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutMFVec3f_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutMFVec3f_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutMFVec3f_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutMFVec3f_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutMFVec3f_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutMFVec3f_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutMFVec3f_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutMFVec3f_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutMFVec3f_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutMFVec3f_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutMFVec3f_getSize(This,cnt)	\
    (This)->lpVtbl -> getSize(This,cnt)


#define EventOutMFVec3f_getValue(This,cnt,value)	\
    (This)->lpVtbl -> getValue(This,cnt,value)

#define EventOutMFVec3f_get1Value(This,index,value)	\
    (This)->lpVtbl -> get1Value(This,index,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutMFVec3f_getValue_Proxy( 
    EventOutMFVec3f __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB EventOutMFVec3f_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventOutMFVec3f_get1Value_Proxy( 
    EventOutMFVec3f __RPC_FAR * This,
    /* [in] */ int index,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB EventOutMFVec3f_get1Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutMFVec3f_INTERFACE_DEFINED__ */


#ifndef __EventOutSFBool_INTERFACE_DEFINED__
#define __EventOutSFBool_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutSFBool
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutSFBool;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC50438-75D9-11D0-9315-0020AFE05CC8")
    EventOutSFBool : public EventOut
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutSFBoolVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutSFBool __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutSFBool __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutSFBool __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutSFBool __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutSFBool __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutSFBool __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutSFBool __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutSFBool __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutSFBool __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutSFBool __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutSFBool __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutSFBool __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutSFBoolVtbl;

    interface EventOutSFBool
    {
        CONST_VTBL struct EventOutSFBoolVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutSFBool_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutSFBool_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutSFBool_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutSFBool_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutSFBool_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutSFBool_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutSFBool_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutSFBool_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutSFBool_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutSFBool_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutSFBool_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutSFBool_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutSFBool_getValue_Proxy( 
    EventOutSFBool __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value);


void __RPC_STUB EventOutSFBool_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutSFBool_INTERFACE_DEFINED__ */


#ifndef __EventOutSFColor_INTERFACE_DEFINED__
#define __EventOutSFColor_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutSFColor
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutSFColor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC5043D-75D9-11D0-9315-0020AFE05CC8")
    EventOutSFColor : public EventOut
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutSFColorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutSFColor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutSFColor __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutSFColor __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutSFColor __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutSFColor __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutSFColor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutSFColor __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutSFColor __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutSFColor __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutSFColor __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutSFColor __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutSFColor __RPC_FAR * This,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutSFColorVtbl;

    interface EventOutSFColor
    {
        CONST_VTBL struct EventOutSFColorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutSFColor_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutSFColor_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutSFColor_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutSFColor_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutSFColor_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutSFColor_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutSFColor_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutSFColor_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutSFColor_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutSFColor_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutSFColor_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutSFColor_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutSFColor_getValue_Proxy( 
    EventOutSFColor __RPC_FAR * This,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB EventOutSFColor_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutSFColor_INTERFACE_DEFINED__ */


#ifndef __EventOutSFFloat_INTERFACE_DEFINED__
#define __EventOutSFFloat_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutSFFloat
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutSFFloat;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC50442-75D9-11D0-9315-0020AFE05CC8")
    EventOutSFFloat : public EventOut
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [retval][out] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutSFFloatVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutSFFloat __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutSFFloat __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutSFFloat __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutSFFloat __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutSFFloat __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutSFFloat __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutSFFloat __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutSFFloat __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutSFFloat __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutSFFloat __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutSFFloat __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutSFFloat __RPC_FAR * This,
            /* [retval][out] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutSFFloatVtbl;

    interface EventOutSFFloat
    {
        CONST_VTBL struct EventOutSFFloatVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutSFFloat_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutSFFloat_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutSFFloat_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutSFFloat_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutSFFloat_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutSFFloat_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutSFFloat_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutSFFloat_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutSFFloat_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutSFFloat_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutSFFloat_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutSFFloat_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutSFFloat_getValue_Proxy( 
    EventOutSFFloat __RPC_FAR * This,
    /* [retval][out] */ float __RPC_FAR *value);


void __RPC_STUB EventOutSFFloat_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutSFFloat_INTERFACE_DEFINED__ */


#ifndef __EventOutSFImage_INTERFACE_DEFINED__
#define __EventOutSFImage_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutSFImage
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutSFImage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC50447-75D9-11D0-9315-0020AFE05CC8")
    EventOutSFImage : public EventOut
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getWidth( 
            /* [retval][out] */ int __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getHeight( 
            /* [retval][out] */ int __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getNumComponents( 
            /* [retval][out] */ int __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getPixels( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ unsigned small __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutSFImageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutSFImage __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutSFImage __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutSFImage __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutSFImage __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutSFImage __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutSFImage __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutSFImage __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutSFImage __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutSFImage __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutSFImage __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutSFImage __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getWidth )( 
            EventOutSFImage __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getHeight )( 
            EventOutSFImage __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getNumComponents )( 
            EventOutSFImage __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getPixels )( 
            EventOutSFImage __RPC_FAR * This,
            /* [in] */ int cnt,
            /* [size_is][out][in] */ unsigned small __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutSFImageVtbl;

    interface EventOutSFImage
    {
        CONST_VTBL struct EventOutSFImageVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutSFImage_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutSFImage_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutSFImage_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutSFImage_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutSFImage_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutSFImage_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutSFImage_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutSFImage_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutSFImage_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutSFImage_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutSFImage_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutSFImage_getWidth(This,value)	\
    (This)->lpVtbl -> getWidth(This,value)

#define EventOutSFImage_getHeight(This,value)	\
    (This)->lpVtbl -> getHeight(This,value)

#define EventOutSFImage_getNumComponents(This,value)	\
    (This)->lpVtbl -> getNumComponents(This,value)

#define EventOutSFImage_getPixels(This,cnt,value)	\
    (This)->lpVtbl -> getPixels(This,cnt,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutSFImage_getWidth_Proxy( 
    EventOutSFImage __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *value);


void __RPC_STUB EventOutSFImage_getWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventOutSFImage_getHeight_Proxy( 
    EventOutSFImage __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *value);


void __RPC_STUB EventOutSFImage_getHeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventOutSFImage_getNumComponents_Proxy( 
    EventOutSFImage __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *value);


void __RPC_STUB EventOutSFImage_getNumComponents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE EventOutSFImage_getPixels_Proxy( 
    EventOutSFImage __RPC_FAR * This,
    /* [in] */ int cnt,
    /* [size_is][out][in] */ unsigned small __RPC_FAR *value);


void __RPC_STUB EventOutSFImage_getPixels_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutSFImage_INTERFACE_DEFINED__ */


#ifndef __EventOutSFInt32_INTERFACE_DEFINED__
#define __EventOutSFInt32_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutSFInt32
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutSFInt32;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC5044C-75D9-11D0-9315-0020AFE05CC8")
    EventOutSFInt32 : public EventOut
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [retval][out] */ int __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutSFInt32Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutSFInt32 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutSFInt32 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutSFInt32 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutSFInt32 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutSFInt32 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutSFInt32 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutSFInt32 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutSFInt32 __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutSFInt32 __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutSFInt32 __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutSFInt32 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutSFInt32 __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutSFInt32Vtbl;

    interface EventOutSFInt32
    {
        CONST_VTBL struct EventOutSFInt32Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutSFInt32_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutSFInt32_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutSFInt32_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutSFInt32_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutSFInt32_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutSFInt32_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutSFInt32_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutSFInt32_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutSFInt32_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutSFInt32_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutSFInt32_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutSFInt32_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutSFInt32_getValue_Proxy( 
    EventOutSFInt32 __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *value);


void __RPC_STUB EventOutSFInt32_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutSFInt32_INTERFACE_DEFINED__ */


#ifndef __EventOutSFNode_INTERFACE_DEFINED__
#define __EventOutSFNode_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutSFNode
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutSFNode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC50451-75D9-11D0-9315-0020AFE05CC8")
    EventOutSFNode : public EventOut
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutSFNodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutSFNode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutSFNode __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutSFNode __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutSFNode __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutSFNode __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutSFNode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutSFNode __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutSFNode __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutSFNode __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutSFNode __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutSFNode __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutSFNode __RPC_FAR * This,
            /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *value);
        
        END_INTERFACE
    } EventOutSFNodeVtbl;

    interface EventOutSFNode
    {
        CONST_VTBL struct EventOutSFNodeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutSFNode_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutSFNode_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutSFNode_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutSFNode_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutSFNode_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutSFNode_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutSFNode_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutSFNode_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutSFNode_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutSFNode_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutSFNode_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutSFNode_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutSFNode_getValue_Proxy( 
    EventOutSFNode __RPC_FAR * This,
    /* [retval][out] */ Node __RPC_FAR *__RPC_FAR *value);


void __RPC_STUB EventOutSFNode_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutSFNode_INTERFACE_DEFINED__ */


#ifndef __EventOutSFRotation_INTERFACE_DEFINED__
#define __EventOutSFRotation_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutSFRotation
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutSFRotation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC50456-75D9-11D0-9315-0020AFE05CC8")
    EventOutSFRotation : public EventOut
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutSFRotationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutSFRotation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutSFRotation __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutSFRotation __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutSFRotation __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutSFRotation __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutSFRotation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutSFRotation __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutSFRotation __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutSFRotation __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutSFRotation __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutSFRotation __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutSFRotation __RPC_FAR * This,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutSFRotationVtbl;

    interface EventOutSFRotation
    {
        CONST_VTBL struct EventOutSFRotationVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutSFRotation_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutSFRotation_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutSFRotation_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutSFRotation_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutSFRotation_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutSFRotation_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutSFRotation_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutSFRotation_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutSFRotation_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutSFRotation_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutSFRotation_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutSFRotation_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutSFRotation_getValue_Proxy( 
    EventOutSFRotation __RPC_FAR * This,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB EventOutSFRotation_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutSFRotation_INTERFACE_DEFINED__ */


#ifndef __EventOutSFString_INTERFACE_DEFINED__
#define __EventOutSFString_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutSFString
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutSFString;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC5045B-75D9-11D0-9315-0020AFE05CC8")
    EventOutSFString : public EventOut
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [retval][out] */ BSTR __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutSFStringVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutSFString __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutSFString __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutSFString __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutSFString __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutSFString __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutSFString __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutSFString __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutSFString __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutSFString __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutSFString __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutSFString __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutSFString __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutSFStringVtbl;

    interface EventOutSFString
    {
        CONST_VTBL struct EventOutSFStringVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutSFString_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutSFString_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutSFString_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutSFString_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutSFString_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutSFString_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutSFString_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutSFString_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutSFString_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutSFString_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutSFString_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutSFString_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutSFString_getValue_Proxy( 
    EventOutSFString __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *value);


void __RPC_STUB EventOutSFString_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutSFString_INTERFACE_DEFINED__ */


#ifndef __EventOutSFTime_INTERFACE_DEFINED__
#define __EventOutSFTime_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutSFTime
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutSFTime;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC50460-75D9-11D0-9315-0020AFE05CC8")
    EventOutSFTime : public EventOut
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [retval][out] */ double __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutSFTimeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutSFTime __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutSFTime __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutSFTime __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutSFTime __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutSFTime __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutSFTime __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutSFTime __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutSFTime __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutSFTime __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutSFTime __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutSFTime __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutSFTime __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutSFTimeVtbl;

    interface EventOutSFTime
    {
        CONST_VTBL struct EventOutSFTimeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutSFTime_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutSFTime_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutSFTime_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutSFTime_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutSFTime_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutSFTime_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutSFTime_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutSFTime_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutSFTime_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutSFTime_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutSFTime_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutSFTime_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutSFTime_getValue_Proxy( 
    EventOutSFTime __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *value);


void __RPC_STUB EventOutSFTime_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutSFTime_INTERFACE_DEFINED__ */


#ifndef __EventOutSFVec2f_INTERFACE_DEFINED__
#define __EventOutSFVec2f_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutSFVec2f
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutSFVec2f;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC50465-75D9-11D0-9315-0020AFE05CC8")
    EventOutSFVec2f : public EventOut
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutSFVec2fVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutSFVec2f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutSFVec2f __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutSFVec2f __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutSFVec2f __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutSFVec2f __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutSFVec2f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutSFVec2f __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutSFVec2f __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutSFVec2f __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutSFVec2f __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutSFVec2f __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutSFVec2f __RPC_FAR * This,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutSFVec2fVtbl;

    interface EventOutSFVec2f
    {
        CONST_VTBL struct EventOutSFVec2fVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutSFVec2f_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutSFVec2f_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutSFVec2f_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutSFVec2f_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutSFVec2f_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutSFVec2f_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutSFVec2f_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutSFVec2f_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutSFVec2f_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutSFVec2f_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutSFVec2f_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutSFVec2f_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutSFVec2f_getValue_Proxy( 
    EventOutSFVec2f __RPC_FAR * This,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB EventOutSFVec2f_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutSFVec2f_INTERFACE_DEFINED__ */


#ifndef __EventOutSFVec3f_INTERFACE_DEFINED__
#define __EventOutSFVec3f_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: EventOutSFVec3f
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_EventOutSFVec3f;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC5046A-75D9-11D0-9315-0020AFE05CC8")
    EventOutSFVec3f : public EventOut
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE getValue( 
            /* [size_is][out][in] */ float __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct EventOutSFVec3fVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            EventOutSFVec3f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            EventOutSFVec3f __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            EventOutSFVec3f __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            EventOutSFVec3f __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            EventOutSFVec3f __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            EventOutSFVec3f __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            EventOutSFVec3f __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getType )( 
            EventOutSFVec3f __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *type);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *advise )( 
            EventOutSFVec3f __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer,
            /* [in] */ IDispatch __RPC_FAR *userData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unAdvise )( 
            EventOutSFVec3f __RPC_FAR * This,
            /* [in] */ EventOutObserver __RPC_FAR *observer);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *toString )( 
            EventOutSFVec3f __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getValue )( 
            EventOutSFVec3f __RPC_FAR * This,
            /* [size_is][out][in] */ float __RPC_FAR *value);
        
        END_INTERFACE
    } EventOutSFVec3fVtbl;

    interface EventOutSFVec3f
    {
        CONST_VTBL struct EventOutSFVec3fVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define EventOutSFVec3f_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define EventOutSFVec3f_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define EventOutSFVec3f_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define EventOutSFVec3f_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define EventOutSFVec3f_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define EventOutSFVec3f_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define EventOutSFVec3f_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define EventOutSFVec3f_getType(This,type)	\
    (This)->lpVtbl -> getType(This,type)

#define EventOutSFVec3f_advise(This,observer,userData)	\
    (This)->lpVtbl -> advise(This,observer,userData)

#define EventOutSFVec3f_unAdvise(This,observer)	\
    (This)->lpVtbl -> unAdvise(This,observer)

#define EventOutSFVec3f_toString(This,value)	\
    (This)->lpVtbl -> toString(This,value)


#define EventOutSFVec3f_getValue(This,value)	\
    (This)->lpVtbl -> getValue(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE EventOutSFVec3f_getValue_Proxy( 
    EventOutSFVec3f __RPC_FAR * This,
    /* [size_is][out][in] */ float __RPC_FAR *value);


void __RPC_STUB EventOutSFVec3f_getValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __EventOutSFVec3f_INTERFACE_DEFINED__ */


#ifndef __ScriptContainer_INTERFACE_DEFINED__
#define __ScriptContainer_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: ScriptContainer
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_ScriptContainer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC50474-75D9-11D0-9315-0020AFE05CC8")
    ScriptContainer : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ScriptContainerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ScriptContainer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ScriptContainer __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ScriptContainer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ScriptContainer __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ScriptContainer __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ScriptContainer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ScriptContainer __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } ScriptContainerVtbl;

    interface ScriptContainer
    {
        CONST_VTBL struct ScriptContainerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ScriptContainer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ScriptContainer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ScriptContainer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ScriptContainer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ScriptContainer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ScriptContainer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ScriptContainer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ScriptContainer_INTERFACE_DEFINED__ */


#ifndef __Script_INTERFACE_DEFINED__
#define __Script_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: Script
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_Script;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("BAC50479-75D9-11D0-9315-0020AFE05CC8")
    Script : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setContainer( 
            /* [in] */ Node __RPC_FAR *container) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE loadScriptObject( 
            /* [in] */ BSTR urlData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE initialize( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE shutdown( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE processEvent( 
            /* [in] */ BSTR name,
            /* [in] */ int eventId,
            /* [in] */ EventOut __RPC_FAR *value,
            /* [in] */ double timeStamp) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE eventsProcessed( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ScriptVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            Script __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            Script __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            Script __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            Script __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            Script __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            Script __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            Script __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setContainer )( 
            Script __RPC_FAR * This,
            /* [in] */ Node __RPC_FAR *container);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *loadScriptObject )( 
            Script __RPC_FAR * This,
            /* [in] */ BSTR urlData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *initialize )( 
            Script __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *shutdown )( 
            Script __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *processEvent )( 
            Script __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [in] */ int eventId,
            /* [in] */ EventOut __RPC_FAR *value,
            /* [in] */ double timeStamp);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *eventsProcessed )( 
            Script __RPC_FAR * This);
        
        END_INTERFACE
    } ScriptVtbl;

    interface Script
    {
        CONST_VTBL struct ScriptVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Script_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define Script_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define Script_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define Script_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define Script_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define Script_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define Script_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define Script_setContainer(This,container)	\
    (This)->lpVtbl -> setContainer(This,container)

#define Script_loadScriptObject(This,urlData)	\
    (This)->lpVtbl -> loadScriptObject(This,urlData)

#define Script_initialize(This)	\
    (This)->lpVtbl -> initialize(This)

#define Script_shutdown(This)	\
    (This)->lpVtbl -> shutdown(This)

#define Script_processEvent(This,name,eventId,value,timeStamp)	\
    (This)->lpVtbl -> processEvent(This,name,eventId,value,timeStamp)

#define Script_eventsProcessed(This)	\
    (This)->lpVtbl -> eventsProcessed(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE Script_setContainer_Proxy( 
    Script __RPC_FAR * This,
    /* [in] */ Node __RPC_FAR *container);


void __RPC_STUB Script_setContainer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Script_loadScriptObject_Proxy( 
    Script __RPC_FAR * This,
    /* [in] */ BSTR urlData);


void __RPC_STUB Script_loadScriptObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Script_initialize_Proxy( 
    Script __RPC_FAR * This);


void __RPC_STUB Script_initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Script_shutdown_Proxy( 
    Script __RPC_FAR * This);


void __RPC_STUB Script_shutdown_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Script_processEvent_Proxy( 
    Script __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [in] */ int eventId,
    /* [in] */ EventOut __RPC_FAR *value,
    /* [in] */ double timeStamp);


void __RPC_STUB Script_processEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE Script_eventsProcessed_Proxy( 
    Script __RPC_FAR * This);


void __RPC_STUB Script_eventsProcessed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __Script_INTERFACE_DEFINED__ */


#ifndef __BrowserObserver_INTERFACE_DEFINED__
#define __BrowserObserver_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: BrowserObserver
 * at Tue Sep 14 17:17:12 1999
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */ 



EXTERN_C const IID IID_BrowserObserver;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("B8B46AC1-185B-11D1-9447-0020AFE05CC8")
    BrowserObserver : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnStatusMessage( 
            /* [in] */ BSTR message,
            /* [in] */ int messageType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnUrlError( 
            /* [in] */ BSTR url,
            /* [in] */ BSTR message,
            /* [in] */ int errorCode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnLoadUrl( 
            /* [in] */ BSTR url,
            /* [in] */ BSTR targetFrame) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnLoadAnchor( 
            /* [in] */ BSTR description,
            /* [in] */ int numUrl,
            /* [size_is][in] */ BSTR __RPC_FAR *url,
            /* [in] */ int numParameter,
            /* [size_is][in] */ BSTR __RPC_FAR *parameter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnViewpointChanged( 
            /* [size_is][in] */ float __RPC_FAR *position,
            /* [size_is][in] */ float __RPC_FAR *orientation) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnSceneChanged( 
            /* [in] */ BSTR url) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LoadUrlToFile( 
            /* [in] */ BSTR url) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct BrowserObserverVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            BrowserObserver __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            BrowserObserver __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            BrowserObserver __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            BrowserObserver __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            BrowserObserver __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            BrowserObserver __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            BrowserObserver __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnStatusMessage )( 
            BrowserObserver __RPC_FAR * This,
            /* [in] */ BSTR message,
            /* [in] */ int messageType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUrlError )( 
            BrowserObserver __RPC_FAR * This,
            /* [in] */ BSTR url,
            /* [in] */ BSTR message,
            /* [in] */ int errorCode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnLoadUrl )( 
            BrowserObserver __RPC_FAR * This,
            /* [in] */ BSTR url,
            /* [in] */ BSTR targetFrame);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnLoadAnchor )( 
            BrowserObserver __RPC_FAR * This,
            /* [in] */ BSTR description,
            /* [in] */ int numUrl,
            /* [size_is][in] */ BSTR __RPC_FAR *url,
            /* [in] */ int numParameter,
            /* [size_is][in] */ BSTR __RPC_FAR *parameter);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnViewpointChanged )( 
            BrowserObserver __RPC_FAR * This,
            /* [size_is][in] */ float __RPC_FAR *position,
            /* [size_is][in] */ float __RPC_FAR *orientation);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnSceneChanged )( 
            BrowserObserver __RPC_FAR * This,
            /* [in] */ BSTR url);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LoadUrlToFile )( 
            BrowserObserver __RPC_FAR * This,
            /* [in] */ BSTR url);
        
        END_INTERFACE
    } BrowserObserverVtbl;

    interface BrowserObserver
    {
        CONST_VTBL struct BrowserObserverVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define BrowserObserver_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define BrowserObserver_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define BrowserObserver_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define BrowserObserver_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define BrowserObserver_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define BrowserObserver_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define BrowserObserver_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define BrowserObserver_OnStatusMessage(This,message,messageType)	\
    (This)->lpVtbl -> OnStatusMessage(This,message,messageType)

#define BrowserObserver_OnUrlError(This,url,message,errorCode)	\
    (This)->lpVtbl -> OnUrlError(This,url,message,errorCode)

#define BrowserObserver_OnLoadUrl(This,url,targetFrame)	\
    (This)->lpVtbl -> OnLoadUrl(This,url,targetFrame)

#define BrowserObserver_OnLoadAnchor(This,description,numUrl,url,numParameter,parameter)	\
    (This)->lpVtbl -> OnLoadAnchor(This,description,numUrl,url,numParameter,parameter)

#define BrowserObserver_OnViewpointChanged(This,position,orientation)	\
    (This)->lpVtbl -> OnViewpointChanged(This,position,orientation)

#define BrowserObserver_OnSceneChanged(This,url)	\
    (This)->lpVtbl -> OnSceneChanged(This,url)

#define BrowserObserver_LoadUrlToFile(This,url)	\
    (This)->lpVtbl -> LoadUrlToFile(This,url)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE BrowserObserver_OnStatusMessage_Proxy( 
    BrowserObserver __RPC_FAR * This,
    /* [in] */ BSTR message,
    /* [in] */ int messageType);


void __RPC_STUB BrowserObserver_OnStatusMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE BrowserObserver_OnUrlError_Proxy( 
    BrowserObserver __RPC_FAR * This,
    /* [in] */ BSTR url,
    /* [in] */ BSTR message,
    /* [in] */ int errorCode);


void __RPC_STUB BrowserObserver_OnUrlError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE BrowserObserver_OnLoadUrl_Proxy( 
    BrowserObserver __RPC_FAR * This,
    /* [in] */ BSTR url,
    /* [in] */ BSTR targetFrame);


void __RPC_STUB BrowserObserver_OnLoadUrl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE BrowserObserver_OnLoadAnchor_Proxy( 
    BrowserObserver __RPC_FAR * This,
    /* [in] */ BSTR description,
    /* [in] */ int numUrl,
    /* [size_is][in] */ BSTR __RPC_FAR *url,
    /* [in] */ int numParameter,
    /* [size_is][in] */ BSTR __RPC_FAR *parameter);


void __RPC_STUB BrowserObserver_OnLoadAnchor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE BrowserObserver_OnViewpointChanged_Proxy( 
    BrowserObserver __RPC_FAR * This,
    /* [size_is][in] */ float __RPC_FAR *position,
    /* [size_is][in] */ float __RPC_FAR *orientation);


void __RPC_STUB BrowserObserver_OnViewpointChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE BrowserObserver_OnSceneChanged_Proxy( 
    BrowserObserver __RPC_FAR * This,
    /* [in] */ BSTR url);


void __RPC_STUB BrowserObserver_OnSceneChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE BrowserObserver_LoadUrlToFile_Proxy( 
    BrowserObserver __RPC_FAR * This,
    /* [in] */ BSTR url);


void __RPC_STUB BrowserObserver_LoadUrlToFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __BrowserObserver_INTERFACE_DEFINED__ */

#endif /* __blaxxunVRMLLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
