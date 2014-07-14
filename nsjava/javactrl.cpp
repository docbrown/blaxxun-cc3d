/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.10 $
   Date:        $Date: 1999/03/25 16:09:13 $
   Modification History:
   $Log: javactrl.cpp,v $
   Revision 1.10  1999/03/25 16:09:13  kristof
   *** empty log message ***

   Revision 1.9  1999/03/18 09:07:16  kristof
   setvalue for mffloat corrected

   Revision 1.8  1998/11/12 17:23:53  kristof
   mffields corrected

   Revision 1.7  1998/10/07 17:17:05  kristof
   use qiptr

   Revision 1.6  1998/09/16 15:24:29  kristof
   no addref for nodes and events

   Revision 1.5  1998/05/05 16:23:30  kristof
   gettype added

   Revision 1.4  1998/03/03 16:16:12  bernd
   correct getValue for Strings

   Revision 1.3  1997/12/16 17:57:31  kristof
   *** empty log message ***

   Revision 1.2  1997/12/15 12:44:50  kristof
   *** empty log message ***

   ----   file information  ---------------------------------------------- */

#include "stdafx.h"
#include "javactrl.h"

#define IMPLEMENT_vrml_external_field_EventOut

#ifndef _vrml_external_Browser_H_
#include "vrml_external_Browser.h"
#endif
#ifndef _vrml_external_Node_H_
#include "vrml_external_Node.h"
#endif
#ifndef _java_lang_IllegalArgumentException_H_
#include "java_lang_IllegalArgumentException.h"
#endif
#ifndef _vrml_external_field_EventOut_H_
#include "vrml_external_field_EventOut.h"
#endif
#ifndef _vrml_external_field_EventIn_H_
#include "vrml_external_field_EventIn.h"
#endif
#ifndef _vrml_external_exception_InvalidVrmlException_H_
#include "vrml_external_exception_InvalidVrmlException.h"
#endif
#ifndef _vrml_external_exception_InvalidRouteException_H_
#include "vrml_external_exception_InvalidRouteException.h"
#endif
#ifndef _vrml_external_exception_InvalidNodeException_H_
#include "vrml_external_exception_InvalidNodeException.h"
#endif
#ifndef _vrml_external_exception_InvalidEventInException_H_
#include "vrml_external_exception_InvalidEventInException.h"
#endif
#ifndef _vrml_external_exception_InvalidEventOutException_H_
#include "vrml_external_exception_InvalidEventOutException.h"
#endif

//#define IID_DEFINED
#include "blaxxunVrml_i.c"

#ifndef _DEBUG
#define TRACE_RELEASE(expr) { (expr)->Release();expr = NULL; }

#else

#define TRACE_RELEASE(expr) { HRESULT tmp= (expr)->Release(); \
		if (tmp !=0) TRACE("Release %p, RefCnt still %ld Line %d\n",expr,tmp,__LINE__); \
		expr = NULL; }

#endif

// ATL Global Module only instance
CAtlGlobalModule _Module;

// ATL GLobal Object Map
BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

// com entry
ComEntry::ComEntry(void * com, ComType_t t)
{
    pCom      = com;
    type      = t;
    /* already added by cc3d
    switch (type)
    {
    case TypeNode:
        //((Node *)pCom)->AddRef();
        break;
    case TypeEventIn:
        ((EventIn *)pCom)->AddRef();
        break;
    case TypeEventOut:
        ((EventOut *)pCom)->AddRef();
        break;
    }*/
}

ComEntry::~ComEntry()
{
    switch (type)
    {
    case TypeNode:
    {
        Node * p = (Node *)pCom;
        TRACE_RELEASE( p );
        break;
    }
    case TypeEventIn:
    {
        EventIn * p = (EventIn *)pCom;
        TRACE_RELEASE( p );
        break;
    }
    case TypeEventOut:
    {
        EventOut * p = (EventOut *)pCom;
        TRACE_RELEASE( p );
        break;
    }
    }
}

// observer entry
ObsEntry::ObsEntry(EventOut* out, CPPBrowserObserver* obs, jref ev)
{
    pObserver = obs;
    pOut      = out;
	event	  = ev;
    pObserver->AddRef();
    //pOut->AddRef(); eventouts already referenced in com list
}

ObsEntry::~ObsEntry()
{
    pOut->unAdvise(pObserver);
    pObserver->Release();
	delete pObserver;
	//JRI_DisposeGlobalRef( NPN_GetJavaEnv() , (JRIGlobalRef)event );
	event = NULL;
    //pOut->Release();
    pOut        = NULL;
    pObserver   = NULL;
}

// control entry
CtrlEntry::CtrlEntry(CJavaCtrl *ctrl, jref b)
{
    controller = ctrl;
    browser    = (jref)ctrl; 
}

CtrlEntry::CtrlEntry()
{
    controller = NULL;
    browser    = NULL; 
}

CtrlEntry::~CtrlEntry()
{
}

// callback implementation for eventOut observer
STDMETHODIMP CPPBrowserObserver::callback(EventOut *valueEvent, double timeStamp,IDispatch *userData) 
{
	JRIEnv* env = NPN_GetJavaEnv();
	jdouble jTime = timeStamp;
	//vrml_external_field_EventOut * ev = (vrml_external_field_EventOut *)JRI_GetGlobalRef( env , event );
	//struct vrml_external_field_EventOutObserver * obs = get_vrml_external_field_EventOut_observer(env, ev);
    vrml_external_field_EventOut_callback(env, (vrml_external_field_EventOut *)event,  jTime);
    return S_OK; 
}

// static members
//CtrlArray CJavaCtrl::theControllers;
extern HINSTANCE g_hDllInstance;
static bool g_isInitialized = false;
static const int CONTROL_SIZE	= 20;
static CtrlEntry JavaControllers[CONTROL_SIZE] ;

// check whether the controller is currently active
bool CJavaCtrl::checkController(CJavaCtrl* ctrl)
{
    int i;
    //for ( i = 0; i < CJavaCtrl::theControllers.GetSize(); i++) 
    for ( i = 0; i < CONTROL_SIZE; i++) 
    {
        //if (CJavaCtrl::theControllers.GetAt(i)->controller == ctrl)
        if (JavaControllers[i].controller == ctrl)
        {
            if (ctrl->getCC3D()) // already initialized
                return true;
            else
                return false;
        }
    }

    // controller no longer active => old Java reference
    return false;
}

// get the controller 
CJavaCtrl* CJavaCtrl::getController(jref browser)
{
    int i;
    //for ( i = CJavaCtrl::theControllers.GetSize() - 1; i >= 0 ; i--) 
	// check existing browser first
    for ( i = 0 ; i < CONTROL_SIZE ; i++) 
    {
        if (!JavaControllers[i].controller) 
			continue;
		//ctrl = CJavaCtrl::theControllers.GetAt(i);
        if (JavaControllers[i].browser == browser)
        {
            if (JavaControllers[i].controller->getCC3D()) // already initialized
                return JavaControllers[i].controller;
            else
                return NULL;
        }
    }
	// not found, so try to make first assignment
    for ( i = CONTROL_SIZE - 1; i >= 0 ; i--) 
    {
        if (!JavaControllers[i].controller) 
			continue;
		//ctrl = CJavaCtrl::theControllers.GetAt(i);
        if (JavaControllers[i].browser == NULL)
        {
			JavaControllers[i].browser = browser;
			JavaControllers[i].controller->setBrowser(browser);
            if (JavaControllers[i].controller->getCC3D()) // already initialized
                return JavaControllers[i].controller;
            else
                return NULL;
        }
    }

    // controller no longer active => old Java reference
    return NULL;
}

jref CJavaCtrl::init(JRIEnv* env)
{
	// Initialize the ATL Module
	if (!g_isInitialized)
	{
		g_isInitialized =true;
		_Module.Init(ObjectMap,g_hDllInstance);
	}

	use_netscape_plugin_Plugin(env);
	//use_java_lang_IllegalArgumentException(env);
	use_vrml_external_Node(env);
	use_vrml_external_field_EventIn(env);
	use_vrml_external_field_EventOut(env);
	use_vrml_external_exception_InvalidVrmlException(env);
	use_vrml_external_exception_InvalidRouteException(env);
	use_vrml_external_exception_InvalidNodeException(env);
	use_vrml_external_exception_InvalidEventInException(env);
	use_vrml_external_exception_InvalidEventOutException(env);
	return (jref)use_vrml_external_Browser(env);
}

CJavaCtrl::CJavaCtrl( NPP instance )
{
    m_ctrl = NULL;
    m_browser = NULL;
    m_instance = instance;
	//jref javapeer = NPN_GetJavaPeer(instance);
    //CtrlEntry* ctrl = new CtrlEntry(this, NULL);
    //CJavaCtrl::theControllers.Add(ctrl);
    for ( int i = 0; i < CONTROL_SIZE; i++) 
    {
        if (JavaControllers[i].controller == NULL)
		{
			JavaControllers[i].controller = this;
			break;
		}
	}
}

CJavaCtrl::~CJavaCtrl()
{
    m_ctrl = NULL;
    m_browser = NULL;
    m_instance = NULL;
    int i;
    //for ( i = 0; i < JavaCtrl::theControllers.GetSize(); i++) 
    for ( i = 0; i < CONTROL_SIZE; i++) 
    {
        //ctrl = CJavaCtrl::theControllers.GetAt(i);
        if (JavaControllers[i].controller == this)
        {
            //CJavaCtrl::theControllers.RemoveAt(i);
            JavaControllers[i].controller = NULL;
            JavaControllers[i].browser = NULL;
            //delete ctrl;
            break;
        }
    }

    for ( i = 0; i < m_observer.GetSize(); i++) 
    {
        delete m_observer.GetAt(i);
    }
    m_observer.RemoveAll();

    for ( i = 0; i < m_objects.GetSize(); i++) 
    {
        delete m_objects.GetAt(i);
    }
    m_objects.RemoveAll();
}

// just to check whether it is initialized
CJavaCtrl* CJavaCtrl::getControl() 
{
    if (getCC3D()) // already initialized
        return this;
    else
        return NULL;
}

void CJavaCtrl::addRef(void * pUnknown, ComType_t type)
{
    ComEntry * com = new ComEntry( pUnknown, type);
    m_objects.Add(com);
}

void CJavaCtrl::release(void * pUnknown)
{
    for ( int i = 0; i < m_objects.GetSize(); i++) 
    {
		if (m_objects.GetAt(i)->pCom == pUnknown)
		{
			delete m_objects.GetAt(i);
			m_objects.RemoveAt(i);
			break;
		}
    }
}

void CJavaCtrl::advise(void * pUnknown, jref observer)
{
    CPPBrowserObserver * pObserver = new CPPBrowserObserver( );
	//JRIGlobalRef globalRef = JRI_NewGlobalRef(NPN_GetJavaEnv(), observer );
    //pObserver->setEvent( (jref) globalRef );
    pObserver->setEvent( (jref) observer );
    ((EventOut *)pUnknown)->advise(pObserver, NULL);
    ObsEntry * com = new ObsEntry( (EventOut *)pUnknown, pObserver, (jref)observer);
    m_observer.Add(com);
    return ;
}

bool CJavaCtrl::checkRef(void * hdl, ComType_t type)
{
    int i;
    for ( i = 0; i < m_objects.GetSize(); i++) 
    {
        if (m_objects.GetAt(i)->pCom == hdl)
        {
            return true;
        }
    }
    return false;
}

CString CJavaCtrl::getNodeType(void * node)
{    
    CString ret = "";

    BSTR v = NULL;
    ((Node *)node)->getType(&v);
    if (v != NULL)
    {
        ret = v;
        // BSTR allocated by GLView 
        ::SysFreeString(v);
    }
    
    return ret;
}

void * CJavaCtrl::getNodeQIPtr(LPUNKNOWN pNode)
{
	if (pNode == NULL)
	{
        return NULL;
	}
	CComQIPtr<Node, &IID_Node > eNode(pNode);
    eNode->AddRef();
    pNode->Release();
	addRef( (void *)eNode, TypeNode );
    /*CComBSTR type;
    eNode->getType(&type);
    CString s = type;
    eNode->getName(&type);
    s = type;*/
    return (void *)eNode;
}
 
void * CJavaCtrl::getEventIn(void * node, const char * event, int * type )
{
    EventIn * pEvent = NULL;
    void    * qiptr  = NULL;

    if (node)
    {
        CComBSTR tmpBstr = event;
        ((Node *)node)->getEventIn(tmpBstr,&pEvent);
    }
    else
    {
        pEvent = (EventIn *)m_ctrl->getEventIn(event);
    }
	if (pEvent == NULL)
	{
        return NULL;
	}
	pEvent->getType(type);
    switch (*type)
    {
    case SFFLOAT:
        {
            CComQIPtr<EventInSFFloat, &IID_EventInSFFloat> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case MFFLOAT:
        {
            CComQIPtr<EventInMFFloat, &IID_EventInMFFloat> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFCOLOR:
        {
            CComQIPtr<EventInSFColor, &IID_EventInSFColor> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case MFCOLOR:
        {
            CComQIPtr<EventInMFColor, &IID_EventInMFColor> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFNODE:
        {
            CComQIPtr<EventInSFNode, &IID_EventInSFNode> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case MFNODE:
        {
            CComQIPtr<EventInMFNode, &IID_EventInMFNode> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFINT32:
        {
            CComQIPtr<EventInSFInt32, &IID_EventInSFInt32> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case MFINT32:
        {
            CComQIPtr<EventInMFInt32, &IID_EventInMFInt32> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFVEC2F:
        {
            CComQIPtr<EventInSFVec2f, &IID_EventInSFVec2f> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case MFVEC2F:
        {
            CComQIPtr<EventInMFVec2f, &IID_EventInMFVec2f> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFVEC3F:
        {
            CComQIPtr<EventInSFVec3f, &IID_EventInSFVec3f> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case MFVEC3F:
        {
            CComQIPtr<EventInMFVec3f, &IID_EventInMFVec3f> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFROTATION:
        {
            CComQIPtr<EventInSFRotation, &IID_EventInSFRotation> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case MFROTATION:
        {
            CComQIPtr<EventInMFRotation, &IID_EventInMFRotation> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFSTRING:
        {
            CComQIPtr<EventInSFString, &IID_EventInSFString> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case MFSTRING:
        {
            CComQIPtr<EventInMFString, &IID_EventInMFString> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFBOOL:
        {
            CComQIPtr<EventInSFBool, &IID_EventInSFBool> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFTIME:
        {
            CComQIPtr<EventInSFTime, &IID_EventInSFTime> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFIMAGE:
        {
            CComQIPtr<EventInSFImage, &IID_EventInSFImage> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    }
    pEvent->Release();
	addRef( qiptr, TypeEventIn );
    return qiptr;
}
 
void * CJavaCtrl::getEventOut(void * node, const char * event, int * type)
{
    EventOut * pEvent = NULL;
    void    * qiptr  = NULL;

    if (node)
    {
        CComBSTR tmpBstr = event;
        ((Node *)node)->getEventOut(tmpBstr,&pEvent);
    }
    else
    {
        pEvent = (EventOut *)m_ctrl->getEventOut(event);
    }
	if (pEvent == NULL)
	{
        return NULL;
	}
	pEvent->getType(type);
    switch (*type)
    {
    case SFFLOAT:
        {
            CComQIPtr<EventOutSFFloat, &IID_EventOutSFFloat> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case MFFLOAT:
        {
            CComQIPtr<EventOutMFFloat, &IID_EventOutMFFloat> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFCOLOR:
        {
            CComQIPtr<EventOutSFColor, &IID_EventOutSFColor> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case MFCOLOR:
        {
            CComQIPtr<EventOutMFColor, &IID_EventOutMFColor> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFNODE:
        {
            CComQIPtr<EventOutSFNode, &IID_EventOutSFNode> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case MFNODE:
        {
            CComQIPtr<EventOutMFNode, &IID_EventOutMFNode> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFINT32:
        {
            CComQIPtr<EventOutSFInt32, &IID_EventOutSFInt32> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case MFINT32:
        {
            CComQIPtr<EventOutMFInt32, &IID_EventOutMFInt32> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFVEC2F:
        {
            CComQIPtr<EventOutSFVec2f, &IID_EventOutSFVec2f> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case MFVEC2F:
        {
            CComQIPtr<EventOutMFVec2f, &IID_EventOutMFVec2f> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFVEC3F:
        {
            CComQIPtr<EventOutSFVec3f, &IID_EventOutSFVec3f> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case MFVEC3F:
        {
            CComQIPtr<EventOutMFVec3f, &IID_EventOutMFVec3f> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFROTATION:
        {
            CComQIPtr<EventOutSFRotation, &IID_EventOutSFRotation> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case MFROTATION:
        {
            CComQIPtr<EventOutMFRotation, &IID_EventOutMFRotation> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFSTRING:
        {
            CComQIPtr<EventOutSFString, &IID_EventOutSFString> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case MFSTRING:
        {
            CComQIPtr<EventOutMFString, &IID_EventOutMFString> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFBOOL:
        {
            CComQIPtr<EventOutSFBool, &IID_EventOutSFBool> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFTIME:
        {
            CComQIPtr<EventOutSFTime, &IID_EventOutSFTime> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    case SFIMAGE:
        {
            CComQIPtr<EventOutSFImage, &IID_EventOutSFImage> eEvent(pEvent);
            eEvent->AddRef();
            qiptr = (void *)eEvent;
            break;
        }
    }
    pEvent->Release();
	addRef( qiptr, TypeEventOut );
    return qiptr;
}

void CJavaCtrl::setString(void * pUnknown, const char * string, int index, int type)
{
    switch (type)
    {
    case SFSTRING:
        {
            CComBSTR tmpBstr = string;
            ((EventInSFString *)pUnknown)->setValue(tmpBstr);
            break;
        }
    case MFSTRING:
        {
            CComBSTR tmpBstr = string;
            ((EventInMFString *)pUnknown)->set1Value(index, tmpBstr);
            break;
        }
    }
}

void CJavaCtrl::setNode(void * pEvent, void * pNode, int index, int type)
{
    switch (type)
    {
    case SFNODE:
        {
            ((EventInSFNode *)pEvent)->setValue((Node *)pNode);
            break;
        }
    case MFNODE:
        {
            ((EventInMFNode *)pEvent)->set1Value(index, (Node *)pNode);
            break;
        }
    }
}

void CJavaCtrl::setNodeArray(void * pEvent, void ** pNode, int len)
{
    ((EventInMFNode *)pEvent)->setValue(len, (LPDISPATCH*)pNode);
}

void CJavaCtrl::setInt(void * pEvent, int * val, int index, int cnt, int type)
{
    switch (type)
    {
    case SFINT32:
        {
            ((EventInSFInt32 *)pEvent)->setValue(*val);
            break;
        }
    case MFINT32:
        {
            if (cnt)
                ((EventInMFInt32 *)pEvent)->setValue(cnt, val);
            else
                ((EventInMFInt32 *)pEvent)->set1Value(index, *val);
            break;
        }
    }
}

void CJavaCtrl::setFloat(void * pEvent, float val, int index, int type)
{
    switch (type)
    {
    case SFFLOAT:
        {
            ((EventInSFFloat *)pEvent)->setValue(val);
            break;
        }
    case MFFLOAT:
        {
            ((EventInMFFloat *)pEvent)->set1Value(index, val);
            break;
        }
    }
}

void CJavaCtrl::setFloat(void * pEvent, float * val, int size, int index, int type)
{
    switch (type)
    {
    case SFCOLOR:
        {
            if (size < 3) return;
            ((EventInSFColor *)pEvent)->setValue(val);
            break;
        }
    case SFROTATION:
        {
            if (size < 4) return;
            ((EventInSFRotation *)pEvent)->setValue(val);
            break;
        }
    case SFVEC2F:
        {
            if (size < 2) return;
            ((EventInSFVec2f *)pEvent)->setValue(val);
            break;
        }
    case SFVEC3F:
        {
            if (size < 3) return;
            ((EventInSFVec3f *)pEvent)->setValue(val);
            break;
        }
    case MFFLOAT:
        {
            CComQIPtr<EventInMFFloat, &IID_EventInMFFloat> eEventIn((EventIn *)pEvent);
            eEventIn->setValue(size, val);
            break;
        }
    case MFCOLOR:
        {
            if (index >= 0)
            {
                if (size < 3) return;
                ((EventInMFColor *)pEvent)->set1Value(index, val);
            }
            else
            {
                if (size % 3 != 0) return;
                ((EventInMFColor *)pEvent)->setValue(size, val);
            }
            break;
        }
    case MFROTATION:
        {
            if (index >= 0)
            {
                if (size < 4) return;
                ((EventInMFRotation *)pEvent)->set1Value(index, val);
            }
            else
            {
                if (size % 4  != 0) return;
                ((EventInMFRotation *)pEvent)->setValue(size, val);
            }
            break;
        }
    case MFVEC2F:
        {
            if (index >= 0)
            {
                if (size < 2) return;
                ((EventInMFVec2f *)pEvent)->set1Value(index, val);
            }
            else
            {
                if (size % 2 != 0) return;
                ((EventInMFVec2f *)pEvent)->setValue(size, val);
            }
            break;
        }
    case MFVEC3F:
        {
            if (index >= 0)
            {
                if (size < 3) return;
                ((EventInMFVec3f *)pEvent)->set1Value(index, val);
            }
            else
            {
                if (size % 3 != 0) return;
                ((EventInMFVec3f *)pEvent)->setValue(size, val);
            }
            break;
        }
    }
}

void CJavaCtrl::setDouble(void * pEvent, double val, int type)
{
    switch (type)
    {
    case SFTIME:
        {
            ((EventInSFTime *)pEvent)->setValue(val);
            break;
        }
    }
}

void CJavaCtrl::setBool(void * pEvent, bool val, int type)
{
    switch (type)
    {
    case SFBOOL:
        {
            ((EventInSFBool *)pEvent)->setValue(val);
            break;
        }
    }
}

void CJavaCtrl::setImage(void * pEvent, int width, int height, int num, int len, unsigned char * pixels, int type)
{
    switch (type)
    {
    case SFIMAGE:
        {
            ((EventInSFImage *)pEvent)->setValue(width, height, num, len, pixels);
            break;
        }
    }
}

CString CJavaCtrl::getString(void * pEvent, int index, int type)
{
    CString ret = "";
    switch (type)
    {
    case SFSTRING:
        {
            BSTR v = NULL;
            ((EventOutSFString *)pEvent)->getValue(&v);
            if (v)
            {
                ret = v;
                // BSTR allocated by GLView 
                ::SysFreeString(v);
            }
            break;
        }
    case MFSTRING:
        {
            BSTR v = NULL;
            ((EventOutMFString *)pEvent)->get1Value(index, &v);
            if (v != NULL)
            {
                ret = v;
                // BSTR allocated by GLView 
                ::SysFreeString(v);
            }
            break;
        }
    }
    return ret;
}

void * CJavaCtrl::getNode(void * pEvent, int index, int type)
{
    Node * pNode = NULL;
    switch (type)
    {
    case SFNODE:
        {
            ((EventOutSFNode *)pEvent)->getValue(&pNode);
            break;
        }
    case MFNODE:
        {
            ((EventOutMFNode *)pEvent)->get1Value(index, &pNode);
            break;
        }
    }
	if (pNode == NULL)
	{
        return NULL;
	}
	addRef( (void *)pNode, TypeNode );
    return (void *)pNode;
}

void CJavaCtrl::getInt(void * pEvent, int * result, int len, int index, int type)
{
    switch (type)
    {
    case SFINT32:
        {
            ((EventOutSFInt32 *)pEvent)->getValue(result);
            break;
        }
    case MFINT32:
        {
            if (len == 1)
                ((EventOutMFInt32 *)pEvent)->get1Value(index, result);
            else
                ((EventOutMFInt32 *)pEvent)->getValue(len, result);
            break;
        }
    case SFIMAGE:
        {
            if (index == 0)
                ((EventOutSFImage *)pEvent)->getWidth(result);
            else if (index == 1)
                ((EventOutSFImage *)pEvent)->getHeight(result);
            else
                ((EventOutSFImage *)pEvent)->getNumComponents(result);
            break;
        }
    }
}

void CJavaCtrl::getFloat(void * pEvent, float * result, int len, int index, int type)
{
    switch (type)
    {
    case SFFLOAT:
        {
            ((EventOutSFFloat *)pEvent)->getValue(result);
            break;
        }
    case MFFLOAT:
        {
            if (len == 1)
                ((EventOutMFFloat *)pEvent)->get1Value(index, result);
            else
                ((EventOutMFFloat *)pEvent)->getValue(len, result);
            break;
        }
    case SFCOLOR:
        {
            if (len < 3) return;
            ((EventOutSFColor *)pEvent)->getValue(result);
            break;
        }
    case MFCOLOR:
        {
            if (len < 3) return;
            ((EventOutMFColor *)pEvent)->get1Value(index, result);
            break;
        }
    case SFROTATION:
        {
            if (len < 4) return;
            ((EventOutSFRotation *)pEvent)->getValue(result);
            break;
        }
    case MFROTATION:
        {
            if (len < 4) return;
            ((EventOutMFRotation *)pEvent)->get1Value(index, result);
            break;
        }
    case SFVEC2F:
        {
            if (len < 2) return;
            ((EventOutSFVec2f *)pEvent)->getValue(result);
            break;
        }
    case MFVEC2F:
        {
            if (len < 2) return;
            ((EventOutMFVec2f *)pEvent)->get1Value(index, result);
            break;
        }
    case SFVEC3F:
        {
            if (len < 3) return;
            ((EventOutSFVec3f *)pEvent)->getValue(result);
            break;
        }
    case MFVEC3F:
        {
            if (len < 3) return;
            ((EventOutMFVec3f *)pEvent)->get1Value(index, result);
            break;
        }
    }
}

void CJavaCtrl::getByte(void * pEvent, char * result, int len, int type)
{
    switch (type)
    {
    case SFIMAGE:
        {
            ((EventOutSFImage *)pEvent)->getPixels(len, (unsigned char *)result);
            break;
        }
    }
}

double CJavaCtrl::getDouble(void * pEvent, int type)
{
    double ret = 0.0f;
    switch (type)
    {
    case SFTIME:
        {
            ((EventOutSFTime *)pEvent)->getValue(&ret);
            break;
        }
    }
    return ret;
}

bool CJavaCtrl::getBool(void * pEvent, int type)
{
    switch (type)
    {
    case SFBOOL:
        {
            VARIANT_BOOL v;
            ((EventOutSFBool *)pEvent)->getValue(&v);
            if ( (int)v ) 
                return true;
            break;
        }
    }
    return false;
}

int CJavaCtrl::getSize (void * pEvent, int type)
{
    int ret = 0;
    switch (type)
    {
    case MFSTRING:
        {
            ((EventOutMFString *)pEvent)->getSize(&ret);
            break;
        }
    case MFNODE:
        {
            ((EventOutMFNode *)pEvent)->getSize(&ret);
            break;
        }
    case MFINT32:
        {
            ((EventOutMFInt32 *)pEvent)->getSize(&ret);
            break;
        }
    case MFFLOAT:
        {
            ((EventOutMFFloat *)pEvent)->getSize(&ret);
            break;
        }
    case MFCOLOR:
        {
            ((EventOutMFColor *)pEvent)->getSize(&ret);
            break;
        }
    case MFROTATION:
        {
            ((EventOutMFRotation *)pEvent)->getSize(&ret);
            break;
        }
    case MFVEC2F:
        {
            ((EventOutMFVec2f *)pEvent)->getSize(&ret);
            break;
        }
    case MFVEC3F:
        {
            ((EventOutMFVec3f *)pEvent)->getSize(&ret);
            break;
        }
	}
    return ret;
}
