/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.7 $
   Date:        $Date: 1999/03/25 16:09:15 $
   Modification History:
   $Log: javactrl.h,v $
   Revision 1.7  1999/03/25 16:09:15  kristof
   *** empty log message ***

   Revision 1.6  1998/11/12 17:24:05  kristof
   *** empty log message ***

   Revision 1.5  1998/10/07 17:17:20  kristof
   *** empty log message ***

   Revision 1.4  1998/05/05 16:23:43  kristof
   getttype added

   Revision 1.3  1997/12/15 16:35:49  bernd
   *** empty log message ***

   Revision 1.2  1997/12/15 12:44:51  kristof
   *** empty log message ***

   ----   file information  ---------------------------------------------- */


#ifndef _JAVACTRL_H_
#define _JAVACTRL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "jri.h"
#include "blaxxunVrml.h"
#include "blaxxuncc3d.h"		// passport control

#ifndef _NPAPI_H_
#include "npapi.h"
#endif
#ifndef _NPUPP_H_
#include "npupp.h"
#endif

class CJavaCtrl;

// EventOut Observer 

class CPPBrowserObserver : public CComObjectRoot,
public CComDualImpl<EventOutObserver, &IID_EventOutObserver, &LIBID_blaxxunVRMLLib>
{

    BEGIN_COM_MAP(CPPBrowserObserver)
		//COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(EventOutObserver)
	END_COM_MAP()

    DECLARE_NOT_AGGREGATABLE(CPPBrowserObserver)


public:
    /* IUnknown methods */
    STDMETHOD_(ULONG, AddRef)() { return InternalAddRef(); }
	STDMETHOD_(ULONG, Release)() { return InternalRelease(); }
	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject) {return _InternalQueryInterface(iid, ppvObject);}

    /* EventOutObserver methods */
    STDMETHOD(callback) (EventOut *value, double timeStamp,IDispatch *userData);

    void setEvent( jref ev ) { event = ev; };

protected:
    jref        event;
};

typedef enum ComType { 	
    TypeNode          = 0,
    TypeEventIn       = 1,
    TypeEventOut      = 2
} ComType_t;

struct ComEntry
{ 
    void *	pCom;
    ComType_t   type;
    ComEntry(void * com, ComType_t t);
    ~ComEntry();
};

typedef CArray <ComEntry *, ComEntry *>  ComArray;

struct ObsEntry
{
    EventOut*               pOut;
    CPPBrowserObserver*     pObserver;
    jref					event;
    ObsEntry(EventOut* pOut, CPPBrowserObserver* pObserver, jref ev);
    ~ObsEntry();
};

typedef CArray <ObsEntry *, ObsEntry *>  ObsArray;

class CtrlEntry
{ 
public:
    CJavaCtrl*  controller;
    jref		browser;
    CtrlEntry();
    CtrlEntry(CJavaCtrl*  ctrl, jref b);
    ~CtrlEntry();
};

//typedef CArray <CtrlEntry *, CtrlEntry *>  CtrlArray;
//extern CtrlArray JavaControllers;


class CJavaCtrl : public CObject
{
public:
    // class constants

public:
    // construction
	CJavaCtrl( NPP instance );
	~CJavaCtrl();

public:
	// static method to check whether the browser is active and get the controller
	static bool	        checkController(CJavaCtrl* ctrl);
	static CJavaCtrl*   getController(jref browser);
	static jref	        init(JRIEnv* env);

	//static CtrlArray	theControllers;

    // operations
	CJavaCtrl*	        getControl();
	void				setBrowser(jref b) { m_browser = b; };
	jref				getBrowser() { return m_browser; };
    void        		setCC3D( CblaxxunCC3D * ctrl ) { m_ctrl = ctrl; };
    CblaxxunCC3D*		getCC3D() { return m_ctrl; };
    void				addRef(void * pUnknown, ComType_t type);
    void				release(void * pUnknown);
    void				advise(void * pUnknown, jref observer);
    CString             getNodeType(void * node);
    void *              getNodeQIPtr(LPUNKNOWN pNode);
    void *              getEventIn(void * node, const char * event, int * type ); 
    void *              getEventOut(void * node, const char * event, int * type); 
    bool                checkRef(void * hdl, ComType_t type);
    void                setString(void * pEvent, const char * string, int index, int type);
    void                setNode(void * pEvent, void * pNode, int index, int type);
    void                setNodeArray(void * pEvent, void ** pNode, int len);
    void                setInt(void * pEvent, int * val, int index, int len, int type);
    void                setFloat(void * pEvent, float val, int index, int type);
    void                setFloat(void * pEvent, float *val, int size, int index, int type);
    void                setDouble(void * pEvent, double val, int type);
    void                setBool(void * pEvent, bool val, int type);
    void                setImage(void * pEvent, int width, int height, int num, int len, unsigned char * pixels, int type);
    CString             getString(void * pEvent, int index, int type);
    void *              getNode(void * pEvent, int index, int type);
    void                getInt(void * pEvent, int * result, int len, int index, int type);
    void                getFloat(void * pEvent, float * result, int len, int index, int type);
    void                getByte(void * pEvent, char * result, int len, int type);
    double              getDouble(void * pEvent, int type);
    bool                getBool(void * pEvent, int type);
    int                 getSize (void * pEvent, int type);

private:
    CblaxxunCC3D*		m_ctrl;
	jref				m_browser;
    ComArray			m_objects;
    ObsArray			m_observer;
	NPP					m_instance;

};


//-----------------------------------------------------------------------------
//---   Inline functions for class Cn                              ---

//-----------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////

#endif // _JAVACTRL_H_
