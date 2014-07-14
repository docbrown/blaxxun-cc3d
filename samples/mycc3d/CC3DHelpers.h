
#ifndef __CC3DHelpers_h
#define __CC3DHelpers_h
//
// blaxxun CC3D EAI helper and sample callbacks 
// holger@blaxxun.com
// 03/15/98


#include "CC3DApi.h"


//
// CMyEventOutObserver
//

// a sample event out observer implemented with the ATL helper templates and macros 
// you can derive specialised observers from this one 


class CMyEventOutObserver : public CComObjectRoot,
public CComDualImpl<EventOutObserver, &IID_EventOutObserver, &LIBID_blaxxunVRMLLib>
{

// standard ATL stuf 
    BEGIN_COM_MAP(CMyEventOutObserver)
		//COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(EventOutObserver)
	END_COM_MAP()

    DECLARE_NOT_AGGREGATABLE(CMyEventOutObserver)


public:
    /* IUnknown methods */
    STDMETHOD_(ULONG, AddRef)() { return InternalAddRef(); }
	STDMETHOD_(ULONG, Release)() { return InternalRelease(); }
	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject) {return _InternalQueryInterface(iid, ppvObject);}

    /* EventOutObserver methods */
    STDMETHOD(callback) (EventOut *value, double timeStamp,IDispatch *userData);

public:

// atl stuf ends here 

	// override this callback in derived classes 
	virtual HRESULT OnCallback(EventOut *value, double timeStamp,IDispatch *userData);

};



// a dummy user data for the observer 
// extend this, or create derived version 
class CMyObserverUserData : public CComObjectRoot, public IDispatch 
{
    BEGIN_COM_MAP(CMyObserverUserData)
	END_COM_MAP()

    DECLARE_NOT_AGGREGATABLE(CMyObserverUserData)

    /* IUnknown methods */

    STDMETHOD_(ULONG, AddRef)() { return InternalAddRef(); }
	STDMETHOD_(ULONG, Release)() { return InternalRelease(); }

	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject) {return _InternalQueryInterface(iid, ppvObject);}

    /* dummy IDispatch methods */

	STDMETHOD(GetTypeInfoCount)(unsigned int *){ return 0; }
	STDMETHOD(GetTypeInfo)(unsigned int,unsigned long,struct ITypeInfo ** ){ return 0; }
	STDMETHOD(GetIDsOfNames)(const struct _GUID &,unsigned short ** ,unsigned int,unsigned long,long *){ return 0; }
	STDMETHOD(Invoke)(long,const struct _GUID &,unsigned long,unsigned short,struct tagDISPPARAMS *,struct tagVARIANT *,struct tagEXCEPINFO *,unsigned int *){ return 0; }

public:
	// your data goes here 
	// int myData;


};



class CMyBrowserObserver : public CComObjectRoot,
public CComDualImpl<BrowserObserver, &IID_BrowserObserver, &LIBID_blaxxunVRMLLib>
{

    BEGIN_COM_MAP(CMyBrowserObserver)
		//COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(BrowserObserver)
	END_COM_MAP()

    DECLARE_NOT_AGGREGATABLE(CMyBrowserObserver)


public:
    /* IUnknown methods */
    STDMETHOD_(ULONG, AddRef)() { return InternalAddRef(); }
	STDMETHOD_(ULONG, Release)() { return InternalRelease(); }
	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject) {return _InternalQueryInterface(iid, ppvObject);}


    STDMETHOD(OnStatusMessage)( /* [in] */ BSTR message, /* [in] */ int messageType);
        
    STDMETHOD(OnLoadUrl)( /* [in] */ BSTR url,/* [in] */ BSTR targetFrame);
    
    STDMETHOD(OnLoadAnchor)(/* [in] */ BSTR description, /* [in] */ int numUrl, 
                            /* [size_is][in] */ BSTR __RPC_FAR *url,/* [in] */ int numParameter,
                            /* [size_is][in] */ BSTR __RPC_FAR *parameter);
    
    STDMETHOD(OnViewpointChanged)(  /* [size_is][in] */ float __RPC_FAR *position, /* [size_is][in] */ float __RPC_FAR *orientation);
    
    STDMETHOD(OnSceneChanged)( /* [in] */ BSTR url);
    
    STDMETHOD(LoadUrlToFile)(  /* [in] */ BSTR url);

    STDMETHOD(OnUrlError)( /* [in] */ BSTR url,/* [in] */ BSTR message, /* [in] */ int errorCode);


protected:

};

#endif // __CC3DHelpers_h
