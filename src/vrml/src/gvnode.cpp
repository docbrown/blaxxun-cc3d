/*=============================================================================

This code is licensed under the Web3D-blaxxun Community Source License,
provided in distribution file LICENSE.TXT and available online at
http://www.web3D.org/TaskGroups/x3d/blaxxun/Web3D-blaxxunCommunitySourceAgreement.html
and may only be used for non-commercial use as specified in that license.

THE WEB3D CONSORTIUM AND BLAXXUN DO NOT MAKE AND HEREBY DISCLAIM ANY EXPRESS
OR IMPLIED WARRANTIES RELATING TO THIS CODE, INCLUDING BUT NOT LIMITED TO,
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR
PURPOSE, OR ANY WARRANTIES THAT MIGHT ARISE FROM A COURSE OF DEALING, USAGE
OR TRADE PRACTICE.  THE COMMUNITY SOURCE CODE IS PROVIDED UNDER THIS
AGREEMENT "AS IS," WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COMMUNITY SOURCE CODE ARE
FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR
NON-INFRINGING OR IN ANY WAY CONSTITUTE THE COMPLETE PRODUCT MARKETED UNDER
THE NAMES GIVEN SAID CODE.

==============================================================================*/
#include "stdafx.h"
/******************************************************************************
@doc

@module GvNode.cpp - Implementation of VRML Root class GvNode|

Copyright (c) 1995 - 1999 by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:


Changes:

$Revision: 1.39 $
$Log: gvnode.cpp,v $
Revision 1.39  1999/07/06 16:46:04  tom
*** empty log message ***



Todo :

******************************************************************************/

#include <ctype.h>
#include <gvdb.h>
#include <gvinput.h>
#include <gvdebugerror.h>
#include <gvreaderror.h>
#include <gvfield.h>
#include <gvfielddata.h>
#include <gvnodes.h>
#include <gvscene.h>

#ifdef _G_VRML1
	#include <gvunknownnode.h>
#endif

#include <gvproto.h>
#include <gvscript.h>
#include <gvconnection.h>
#include <gvkeyword.h>
#include <gvextrusion.h>

#include "greporter.h"

#include "gsurface.h"

#include "rtabst.h"

#include "gversion.h"

// Vendor extension nodes 
#ifdef _G_VRML1

	#include <gvwebfx.h>
	#include <gvktx.h>
#endif


#include <gvbsp.h>
#include <gvnurbs.h>
#include <gvlayer.h>

#ifdef G_METASTREAM
	#include "..\projects\metastream\gvmetastream.h"
#endif

#ifdef G_HEAD
	#include "..\projects\willamette\gvheadmodel.h"
#endif

#ifdef _G_INVENTOR
	#include "gvnurbsinventor.h"
#endif


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifdef GV_GLOBAL_DICT
// The global name dictionary
// actually no longer needed, because GvScene maintains a namespace
GvDict	*GvNode::nameDict = NULL;

#endif

#ifndef WIN32_TLS
// the global current browser pointer , need to be maintained by upper level
//xxx GvNode  *GvNode::browser=NULL;

#if 0
// global redraw flag 
GvBool  GvNode::mustRedraw=FALSE;

// the current timestamp time 
double *GvNode::theTime=NULL;
#endif

#endif



#ifdef _DEBUG_NODE_HANDLE

// non inline version 

GvNodeHandle::GvNodeHandle() { node = NULL; }
void GvNodeHandle::destruct() { if (node) node->unref(); node = NULL; }

GvNodeHandle::~GvNodeHandle() { if (node) node->unref(); /* node = NULL; */ }

GvNodeHandle::GvNodeHandle(GvNode * newNode) 
{
   node = newNode;
   if (node) node->ref();
}
    
GvNodeHandle& 
GvNodeHandle::operator=(GvNode * newNode) 
{
	if (node != newNode) {
		if (node) node->unref();			
		node = newNode;
		if (node) node->ref();			
	}

	return(*this);
}

#endif


// predicate to filter connections for delete Route 
GvBool PredToField(GvConnection *c, void *data){
	if (RTISA(c,GvSetFieldConnection)) {
		return  (((GvSetFieldConnection *) c)->toField == (GvField *) data);
	}
	return FALSE;
}

GvBool PredToNode(GvConnection *c, void *data){
	if (RTISA(c,GvNodeMethodConnection)) {
		GvNodeMethodConnection *a = (GvNodeMethodConnection *) c;
		GvNodeMethodConnection *b = (GvNodeMethodConnection *) data;

		return  (a->to == b->to && a->method == b->method);
	}
	return FALSE;
}


//
// GvNode
// 


RTIMP_ABSTRACT(GvNode,0,GNode)

GvNode::GvNode()
{
    objName = new GvName("");  // to do simply use NULL  or use non pointer ?
	refCnt = 0;
	modified = FALSE;
	isBuiltIn=0;
    copyOnInstance=0;
	//isVrml2=0;
	needLightCheck=1;
	hasOutRoute = 0;
	hasInRoute = 0;
	hasBBox_ = 0;
	m_browser = NULL;
    connections = NULL;
}

GvNode::~GvNode()
{
#ifdef _DEBUG
	// for debuggin special cases 
	//if (this == (GvNode *)0x014B0084)
	// TRACE("GvNode::~GvNode() { %p \n",this);
	//if (RTISA(this,GvScript)) {
	//		TRACE("GvNode::  GvScript Node() { %p %s  \n",this,(const char *) this->objName->getString());
	// }

#endif
	
	// for savety delete any incoming routes 
	if (hasInRoute) {
		GvConnection *c;
		GvConnection *csave= connections;

		c = connections;
		connections = NULL; // temp set to NULL for speed because connection remove may want to delete connection

		while (c!= NULL) {
			ASSERT_POINTER(c,GvConnection);
			GvConnection *next =  c->next;

			if (RTISA(c,GvRouteFromConnection)) {
				//TRACE("GvNode::~GvNode():: Removing incomming route %p \n",this);	

				GvRouteFromConnection *cfrom = (GvRouteFromConnection *) c;
				GvRouteToConnection *r = cfrom->routeToConnection;
				cfrom->routeToConnection = NULL;

				if (r) {				// this call will try to remove connection
					ASSERT_POINTER(r,GvRouteToConnection);
					ASSERT(r->to == this);

					ASSERT(r->fromEventIndex>=0);
					ASSERT(r->toEventIndex>=0);

					ASSERT_POINTER(r->from,GvNode);
					ASSERT_POINTER(r->to,GvNode);
					//TRACE("GvNode::~GvNode():: remvoing  incomming route  %p from %p to %p,\n",r,r->from,r->to);	
					
					// problem, this == to already in destruction, remove connection manually
				    //deleteRoute(r->from,r->fromEventIndex,	r->to,r->toEventIndex);
					if (RTISA(r,GvSetFieldConnection)) {

						GvSetFieldConnection *cfield = (GvSetFieldConnection *) r;
					    GvField *fromField = r->from->getField(r->fromEventIndex);

						if (fromField) 
							//int ret = fromField->removeConnection(PredToField,cfield->toField);
							int ret = fromField->removeConnection(cfield);
					}
					else if (RTISA(r,GvNodeMethodConnection)) {
						GvNodeMethodConnection *cfield = (GvNodeMethodConnection *) r;
					    GvField *fromField = r->from->getField(r->fromEventIndex);
						if (fromField) 
							//int ret = fromField->removeConnection(PredToNode,cfield);
							int ret = fromField->removeConnection(cfield);
					}

				} else {
					//TRACE("GvNode::~GvNode():: already removed incomming route \n");	
				}

			}	
			c= next;
		}
		connections = csave;
	}
	
	deleteConnections();

    if (! !(*objName))
	   removeName(this, objName->getString());
    
	delete objName;

	//TRACE("GvNode::~GvNode() } %p \n",this);	
}

#ifdef _COM

//
// COM classes needed for EventIn processing of EventIn not represented as a fields
// set value calls node member functions 
//
// e.g. EventInMFNode (addChildren, removeChildren)

class GvEventInMFNode :
	public CComObjectRoot
	,public CComDualImpl<EventInMFNode, &IID_EventInMFNode, &LIBID_GLViewVrmlLib>

{
	BEGIN_COM_MAP(GvEventInMFNode)
		COM_INTERFACE_ENTRY(EventInMFNode)
		COM_INTERFACE_ENTRY(EventIn)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvEventInMFNode)

    /* IUnknown methods */
	STDMETHOD_(ULONG, AddRef)() {return InternalAddRef(); }
	STDMETHOD_(ULONG, Release)() {
		ULONG l = InternalRelease(); if (l == 0) delete this;
		return l;
	}
	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
	{return _InternalQueryInterface(iid, ppvObject);}


    /* EventIn methods */
    STDMETHOD(setValue)(int cnt, IDispatch** newValue) {
				if (cnt >0) {
					GvMFNode event(cnt);	
					for (int i=0;i<cnt; i++) {
						Node* result=NULL;
						node->QueryInterface(IID_Node, (void **)&result);
						GvNode *n=NULL;
						if (result) result->getNative((long *) &n);
						event.set1(i,n);
						if (result) node->Release();

					}

					GvNode *n = node;
					(n->*method)(&event);
					COM_EXTERN_TOUCH();

				}

				return S_OK;
				}

	STDMETHOD(setValueFromString)( /* [in] */ BSTR value) { 
				GvNode *n = node;
				GvMFNode event(0);	
				event.setContainer(n); // allow ref to browser fo parsing
				HRESULT ret = event.setValueFromString(value);
				(n->*method)(&event); // send the event 
				COM_EXTERN_TOUCH();
				return ret;
	} 


    STDMETHOD(set1Value)(int offset, Node* newValue) {
				GvMFNode event(1);	
				GvNode *n=NULL;
				if (newValue) newValue->getNative((long *) &n);

				event.set1(0,n);
				n = node;
				(n->*method)(&event);
				COM_EXTERN_TOUCH();

				return S_OK;
				}

    STDMETHOD(getType)(int *type) { *type = MFNODE; return S_OK; }


	GvNodeHandle node; // the node 
    // EventIn method // could use offset 
    void (GvNode::*method)(GvField *event);


	GvEventInMFNode(GvNode *ofNode) : node(ofNode) { }
	~GvEventInMFNode() { 
			node = NULL;
			TRACE("~GvEventInMFNode\n");
	}

};

// EventInSFBool (set_bind)

class GvEventInSFBool :
	public CComObjectRoot
	,public CComDualImpl<EventInSFBool, &IID_EventInSFBool, &LIBID_GLViewVrmlLib>

{
	BEGIN_COM_MAP(GvEventInSFBool)
		COM_INTERFACE_ENTRY(EventInSFBool)
		COM_INTERFACE_ENTRY(EventIn)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvEventInSFBool)

    /* IUnknown methods */
	STDMETHOD_(ULONG, AddRef)() {return InternalAddRef(); }
	STDMETHOD_(ULONG, Release)() {
		ULONG l = InternalRelease(); if (l == 0) delete this;
		return l;
	}
	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
	{return _InternalQueryInterface(iid, ppvObject);}


    /* EventIn methods */
    STDMETHOD(setValue)(VARIANT_BOOL  newValue) { 
				GvSFBool event((newValue? TRUE: FALSE));	
				GvNode *n = node;
				(n->*method)(&event); // send the event 
				COM_EXTERN_TOUCH();
				return S_OK;
	}
	STDMETHOD(setValueFromString)( /* [in] */ BSTR value) { 
				GvSFBool event;	
				HRESULT ret = event.setValueFromString(value);
				GvNode *n = node;
				(n->*method)(&event); // send the event 
				COM_EXTERN_TOUCH();
				return ret;
	} 



    STDMETHOD(getType)(int *type) { *type = SFBOOL; return S_OK; }


	GvNodeHandle node; // the node 
    
	// EventIn method 
    void (GvNode::*method)(GvField *event);

	GvEventInSFBool(GvNode *ofNode) : node(ofNode) { }
	~GvEventInSFBool() { node = NULL;}

};

// EventInSFTime (set_startTime)

class GvEventInSFTime :
	public CComObjectRoot
	//public CComObjectNoLock<CComObjectRoot> // HG don't understand AddRef Release not defined ??
	,public CComDualImpl<EventInSFTime, &IID_EventInSFTime, &LIBID_GLViewVrmlLib>

{
	BEGIN_COM_MAP(GvEventInSFTime)
		COM_INTERFACE_ENTRY(EventInSFTime)
		COM_INTERFACE_ENTRY(EventIn)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvEventInSFTime)

#if 1
    /* IUnknown methods */
	STDMETHOD_(ULONG, AddRef)() {return InternalAddRef(); }
	STDMETHOD_(ULONG, Release)() {
		// return InternalRelease(); 
		ULONG l = InternalRelease(); if (l == 0) delete this;
		return l;
	}
	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
	{return _InternalQueryInterface(iid, ppvObject);}
#endif


    /* EventIn methods */
    STDMETHOD(setValue)(double newValue) { 
				GvSFTime event(newValue);	
				GvNode *n = node;
				(n->*method)(&event); // send the event 
				COM_EXTERN_TOUCH();
				return S_OK;
	}
	STDMETHOD(setValueFromString)( /* [in] */ BSTR value) { 
				GvSFTime event;	
				HRESULT ret = event.setValueFromString(value);
				GvNode *n = node;
				(n->*method)(&event); // send the event 
				COM_EXTERN_TOUCH();
				return ret;
	} 



    STDMETHOD(getType)(int *type) { *type = SFTIME; return S_OK; }


	GvNodeHandle node; // the node 
    
	// EventIn method 
    void (GvNode::*method)(GvField *event);

	GvEventInSFTime(GvNode *ofNode) : node(ofNode) { }
	~GvEventInSFTime() { 
		node = NULL;
		TRACE("~GvEventInSFTime\n");
	}

};

// GvEventInSFInt32 

class GvEventInSFInt32 :
	public CComObjectRoot
	,public CComDualImpl<EventInSFInt32, &IID_EventInSFInt32, &LIBID_GLViewVrmlLib>

{
	BEGIN_COM_MAP(GvEventInSFInt32)
		COM_INTERFACE_ENTRY(EventInSFInt32)
		COM_INTERFACE_ENTRY(EventIn)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvEventInSFInt32)

    /* IUnknown methods */
	STDMETHOD_(ULONG, AddRef)() {return InternalAddRef(); }
	STDMETHOD_(ULONG, Release)() {
		ULONG l = InternalRelease(); if (l == 0) delete this;
		return l;
	}
	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
	{return _InternalQueryInterface(iid, ppvObject);}


    /* EventIn methods */
    STDMETHOD(setValue)(int newValue) { 
				GvSFInt32 event(newValue);	
				GvNode *n = node;
				(n->*method)(&event); // send the event 
				COM_EXTERN_TOUCH();
				return S_OK;
	}
	STDMETHOD(setValueFromString)( /* [in] */ BSTR value) { 
				GvSFInt32 event;	
				HRESULT ret = event.setValueFromString(value);
				GvNode *n = node;
				(n->*method)(&event); // send the event 
				COM_EXTERN_TOUCH();
				return ret;
	} 



    STDMETHOD(getType)(int *type) { *type = SFINT32; return S_OK; }


	GvNodeHandle node; // the node 
    
	// EventIn method 
    void (GvNode::*method)(GvField *event);

	GvEventInSFInt32(GvNode *ofNode) : node(ofNode) { }
	~GvEventInSFInt32() { node = NULL;}

};

// EventInSFString (set_myAvatarURL)

class GvEventInSFString :
	public CComObjectRoot
	,public CComDualImpl<EventInSFString, &IID_EventInSFString, &LIBID_GLViewVrmlLib>

{
	BEGIN_COM_MAP(GvEventInSFString)
		COM_INTERFACE_ENTRY(EventInSFString)
		COM_INTERFACE_ENTRY(EventIn)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvEventInSFString)

    /* IUnknown methods */
	STDMETHOD_(ULONG, AddRef)() {return InternalAddRef(); }
	STDMETHOD_(ULONG, Release)() {
		ULONG l = InternalRelease(); if (l == 0) delete this;
		return l;
	}
	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
	{return _InternalQueryInterface(iid, ppvObject);}


    /* EventIn methods */
    STDMETHOD(setValue)(BSTR newValue)
	{
				const char *vs;
				USES_CONVERSION;
				vs = OLE2CT(newValue);

				GvSFString event(vs);	
				GvNode *n = node;
				(n->*method)(&event); // send the event 
				COM_EXTERN_TOUCH();
				return S_OK;
	}
	STDMETHOD(setValueFromString)( /* [in] */ BSTR value) { 
				GvSFString event;	
				HRESULT ret = event.setValueFromString(value);
				GvNode *n = node;
				(n->*method)(&event); // send the event 
				COM_EXTERN_TOUCH();
				return ret;
	} 



    STDMETHOD(getType)(int *type) { *type = SFSTRING; return S_OK; }


	GvNodeHandle node; // the node 
    
	// EventIn method 
    void (GvNode::*method)(GvField *event);

	GvEventInSFString(GvNode *ofNode) : node(ofNode) { }
	~GvEventInSFString() { node = NULL;}

};



/* Node methods */
STDMETHODIMP GvNode::getType(BSTR * typeOle)
{
	USES_CONVERSION;
	const char *type = GetType(); 
	*typeOle = SysAllocString(T2COLE(type));
	return S_OK;

}


// name extension
STDMETHODIMP GvNode::getName(BSTR *nameOle)
{
	USES_CONVERSION;
	const char *type = this->getName().getString(); 
	*nameOle = SysAllocString(T2COLE(type));
	return S_OK;
}


STDMETHODIMP GvNode::setName(BSTR nameOle)
{
	USES_CONVERSION;
	const char * name = OLE2CT(nameOle);
	this->setName(name);
	return S_OK;
}


/*

   if event / field not found return NULL and S_OK
   Java Wrapper can throw proper exception 

*/

STDMETHODIMP GvNode::getEventIn(BSTR nameOle, EventIn* *result)
{
	USES_CONVERSION;
	const char * name = OLE2CT(nameOle);
	GvField * field;

	*result = (EventIn*)NULL;


	// get eventIndex from field data
	GvFieldData *fields = getFieldData();
	int eventIndex = fields->getEventInIndex(this,name);
	if (eventIndex <0) { // no such eventIn
		// return S_FALSE; changed 28.04.92
		return S_OK; 
	}
	GvFieldEntry *entry = fields->getFieldEntry(eventIndex);

	if (entry->type == GV_NODE_EVENT_IN) { // EventIn is memberfunction of node, can' simple pass field pointer !
		if (entry->eventTypeId == GvMFNode::ClassId()) {
			GvEventInMFNode* res = new GvEventInMFNode(this);
			res->method = entry->method;
			return res->QueryInterface(IID_EventIn, (void **)result);
		}
		else if (entry->eventTypeId == GvSFBool::ClassId()) {
			GvEventInSFBool* res = new GvEventInSFBool(this);
			res->method = entry->method;
			return res->QueryInterface(IID_EventIn, (void **)result);
		}
		else if (entry->eventTypeId == GvSFTime::ClassId()) {
			GvEventInSFTime* res = new GvEventInSFTime(this);
			res->method = entry->method;
			return res->QueryInterface(IID_EventIn, (void **)result);
		}
		else if (entry->eventTypeId == GvSFInt32::ClassId()) {
			GvEventInSFInt32* res = new GvEventInSFInt32(this);
			res->method = entry->method;
			return res->QueryInterface(IID_EventIn, (void **)result);
		}
		else if (entry->eventTypeId == GvSFString::ClassId()) {
			GvEventInSFString* res = new GvEventInSFString(this);
			res->method = entry->method;
			return res->QueryInterface(IID_EventIn, (void **)result);
		}
		else {
			// to do:
			Reporter.Error("Node::getEventIn need to implement node event in for %s \n", name);
		}

	} else {
		// EventIn is a field, return pointer to field 
		field = getField(eventIndex);
		if (field == NULL) {
			// return S_FALSE; changed 28.04.92
			return S_OK; 
		}
		else { 
			return field->QueryInterface(IID_EventIn, (void **)result);
		}
	}
	// return S_FALSE; changed 28.04.92
	return S_OK; 
}

STDMETHODIMP GvNode::getEventOut(BSTR nameOle, EventOut ** result)
{
	USES_CONVERSION;
	const char * name = OLE2CT(nameOle);
	GvField * field;

	*result = (EventOut*)NULL;

	// get eventIndex from field data
	GvFieldData *fields = getFieldData();
	int eventIndex = fields->getEventOutIndex(this,name);
	if (eventIndex <0) { // no such eventOut
		// return S_FALSE; changed 28.04.92
		return S_OK; 
	}
	GvFieldEntry *entry = fields->getFieldEntry(eventIndex);

	{
		// EventOut is a field, return pointer to field 
		field = getField(eventIndex);
		if (field == NULL) {
			// return S_FALSE; changed 28.04.92
			return S_OK; 
		}
		else { 
			return field->QueryInterface(IID_EventOut, (void **)result);
		}
	}
	// return S_FALSE; changed 28.04.92
	return S_OK; 
}

STDMETHODIMP GvNode::getField(BSTR nameOle, Field** result)
{
	USES_CONVERSION;
	const char * name = OLE2CT(nameOle);
	GvField * field;
	field = getField(name);
	if (field == NULL) {
		*result = (Field*)NULL;
		// return S_FALSE; changed 28.04.92
		return S_OK; 
	}
	else { 
		return field->QueryInterface(IID_Field, (void **)result);
	}

}

STDMETHODIMP GvNode::getExposedField(BSTR nameOle, Field** result)
{
	// to do: only exposed fields 
	USES_CONVERSION;
	const char * name = OLE2CT(nameOle);
	GvField * field;

	field = getField(name);
	
	if (field == NULL) {
		*result = (Field*)NULL;
		// return S_FALSE; changed 28.04.92
		return S_OK; 
	}
	else { 
		return field->QueryInterface(IID_Field, (void **)result);
	}

}


// interface extension 
STDMETHODIMP GvNode::getNumInterfaceElements(/* [retval][out] */ int *result)
{
	GvFieldData *fields = getFieldData();
	*result= fields->getNumFields();
	return S_OK; 
}


STDMETHODIMP GvNode::getInterfaceElement(
			/* [in] */ int index,
            /* [out] */ int *elementType,
            /* [out] */ int *fieldType,
            /* [out] */ BSTR *name)
{
	USES_CONVERSION;

	GvFieldData *fields = getFieldData();
	if (index <0 || index >= fields->getNumFields()) { // no such element
		return S_FALSE; 
	}
	
	GvFieldEntry *entry = fields->getFieldEntry(index);

	if (entry == NULL) return S_FALSE; // should not happen ?
	
	*fieldType =  (int) entry->fieldClass;
	*name = SysAllocString(T2COLE(entry->name.getString()));


	// convert the type 
	switch (entry->type) { 
    case GV_EXPOSED_FIELD:  *elementType = type_exposedField; break;
    case GV_FIELD:  *elementType = type_field; break;
    case GV_FIELD_EVENT_IN:
	case GV_NODE_EVENT_IN:  *elementType = type_eventIn; break;
    case GV_FIELD_EVENT_OUT: *elementType = type_eventOut; break;
	default : *elementType = type_unknown; break;
	}

	return S_OK; 
}

STDMETHODIMP GvNode::getValueNode( 
            /* [retval][out] */ Node *  *result)
{
	GvNode * value = this->Eval();
	if (value == NULL) *result = NULL;
	else return value->QueryInterface(IID_Node, (void **)result);
	return S_OK;
}


// needed for Java SI
STDMETHODIMP GvNode::getBrowser(Browser **result)
{
	GvNode * value = this->getBrowser(); // warning currently global !!!!
	if (value == NULL) *result = NULL;
	else return value->QueryInterface(IID_Browser, (void **)result);
	return S_OK;
}



#endif



// get VRML Type name for node 
const char * GvNode::GetType()
{ const char *n;
  n = ClassName();

  if (strncmp(n,"Gv2",3) == 0) n +=3;
  else if (strncmp(n,"Gv",2) == 0) n +=2;
  else if (strncmp(n,"G",1) == 0) n +=1;

  return(n);
}

// get type class of node
int GvNode::GetNodeClassType()
{
   return(0);
}

// decrement reference count and delete if now 0
void GvNode::unref() 
{
  
	ASSERT_POINTER(this,GvNode);

  refCnt--;
  ASSERT((refCnt>=0) && (refCnt <= 32000));
  if (refCnt == 0)  // way <=
  		delete this;
}

// touch the node, call all owners of node with OnChildNodeChanged(this)
void GvNode::touch() 
{
   modified = TRUE;
   NotifiyOwners();
}


// get reference to name 
const GvName &
GvNode::getName() const
{
    return *objName;
}

// set the node name 
void
GvNode::setName(const GvName &newName,GvBool vrml2)
{	
    if (! !(*objName)) {
		removeName(this, objName->getString());
    }
    delete objName;

    const char *str = newName.getString();
    GvBool isBad = 0;

    if (newName.getLength() > 0 &&
		!GvName::isNodeNameStartChar(str[0],vrml2)) isBad = TRUE;

    int i;
    for (i = 1; i < newName.getLength() && !isBad; i++) {
		isBad = !GvName::isNodeNameChar(str[i],vrml2);
    }

    if (isBad) {
	GvString goodString;

	if (!GvName::isNodeNameStartChar(str[0],vrml2)) {
	    goodString += "_";
	}
	for (i = 0; i < newName.getLength(); i++) {
	    char temp[2];
	    temp[0] = str[i]; temp[1] = '\0';
	    if (!GvName::isNodeNameChar(str[i],vrml2))
			goodString += "_";
	    else
			goodString += temp;
	}
#ifdef DEBUG
		GvDebugError::post("GvNode::setName", "Bad characters in"
			   " name '%s'.  Replacing with name '%s'",
			   str, goodString.getString());
#endif       
		objName = new GvName(goodString);
    }
    else {
		objName = new GvName(newName);
    }
    if (! !(*objName)) {
		addName(this, objName->getString());
    }
}

void
GvNode::addName(GvNode *b, const char *name)
{

#ifdef GV_GLOBAL_DICT

    GvPList *list;
    void *t;
    

    if (!nameDict) init(); // hg need to init name dict, normaly done by GvDB:init

    if (!nameDict->find((u_long)name, t)) {
	   list = new GvPList;
	   nameDict->enter((u_long)name, list);
    }
    else {
	  list = (GvPList *)t;
    }

    list->append(b);
#endif

}

void
GvNode::removeName(GvNode *b, const char *name)
{
#ifdef GV_GLOBAL_DICT

    GvPList	*list;
    GvBool	found;
    void	*t;
    int		i;

    if (!nameDict) return;  // HG

    found = nameDict->find((u_long) name, t);

    if (found) {
	list = (GvPList *) t;
	i    = list->find(b);

	if (i < 0)
	    found = FALSE;

	else
	    list->remove(i);
    }

    if (! found)
	GvDebugError::post("GvNode::removeName",
			   "Name \"%s\" (node %x) is not in dictionary",
			   name, b);
#endif
}



int GvNode::Traverse(GTraversal &state)
{
#if 0 // not working any more !defined(_GV_MINIMAL)
	int ret;
	
	if (HasAttributes()) {  // process Attributes
	    state.Push();
			GAttributeNode::Traverse(state,0);
			ret =state.Do(this);
		state.Pop();

	}
	else {

	GvModifierElement  *e = (GvModifierElement*) state.Get(GvModifierElement::attrId); 
    if (e) {    // apply all modifiers, NodeHandle keeps track of deleting temporary copys
      //TRACE("Modifier on %s refCnt = %d \n",this->ClassName(),this->refCnt);
      GvNodeHandle node(this);   
      for (; e != NULL; e = (GvModifierElement*) e->prevThisAttr) { 
         node = e->modifier->Apply(state,node); 
      } 
      ret =state.Do(node);
   } 
   else 
		ret =state.Do(this);
   }
   return(ret);
#else
		return state.Do(this);
#endif
}

// traverse all node fields in this node
// needed to expand ProtoInstances stored in fields not part of renderGraph
int GvNode::TraverseAllNodeFields(GTraversal &state)
{
    int ret=0;

    GvFieldData *fieldData = getFieldData();

	// for all fields 
    for (int i = 0; i < fieldData->getNumFields(); i++) {
   
		GvField *field = this->getField(i);
        
        if (!field) continue;
        // traverse any Node fields
		// to think only exposedFields / fields ?

		if (RTISA(field,GvMFNode)) {
            ret = ((GvMFNode*) field)->Traverse(state);
        }
        else if (RTISA(field,GvSFNode)) {
            if (this != ((GvSFNode*) field)->get() ) // prevent direct SFNode loops
				ret = ((GvSFNode*) field)->Traverse(state);
        }
	}
	return ret;
}

int GvNode::TraverseAllNodeFields(GPathTraversal &state)
{
    int ret=0;

    GvFieldData *fieldData = getFieldData();

	// for all fields 
    for (int i = 0; i < fieldData->getNumFields(); i++) {
   
		GvField *field = this->getField(i);
        
        if (!field) continue;
        // traverse any Node fields
		// to think only exposedFields / fields ?

		if (RTISA(field,GvMFNode)) {
			state.level ++;
            ret = ((GvMFNode*) field)->Traverse(state);
			state.level --;
        }
        else if (RTISA(field,GvSFNode)) {
			state.level ++;
            if (this != ((GvSFNode*) field)->get() ) // prevent direct SFNode loops
				ret = ((GvSFNode*) field)->Traverse(state);
			state.level --;
        }
	}
	return ret;
}


void GvNode::FlushAllNodeFields()
{
    int ret=0;

    GvFieldData *fieldData = getFieldData();

	// for all fields 
    for (int i = 0; i < fieldData->getNumFields(); i++) {
   
		GvField *field = this->getField(i);
        
        if (!field) continue;

		if (RTISA(field,GvMFNode)) {
             ((GvMFNode*) field)->flush();
        }
        else if (RTISA(field,GvSFNode)) {
			((GvSFNode*) field)->set((GvNode *) NULL);
        }
	}
}

// get pointer to node field by name 
GvField* 
GvNode::getField(const char *fieldName) const
{
    return getFieldData()->getField(this,fieldName);
}

// get index of field in node, -1 if not found
int  GvNode::getFieldIndex(const GvField *field) const
{
    int n= getNumFields();
    for(int i=0; i<n; i++)
        if (getField(i) == field) return(i);
    return(-1);
}

// get number of fields
int	GvNode::getNumFields() const	
{ 
   return(getFieldData()->getNumFields()); 
}


// get name of field at index 
const GvName &
GvNode::getFieldName(int index)  const 
{
	ASSERT(index< getNumFields());
    return getFieldData()->getFieldName(index);
}

// get name of event out with index i
GvName 
GvNode::getEventOutName(int index)  const 
{
	ASSERT(index< getNumFields());
    return getFieldData()->getEventOutName(index);
}

// get name of event in with index i

GvName 
GvNode::getEventInName(int index)  const 
{
	ASSERT(index< getNumFields());
    return getFieldData()->getEventInName(index);
}



// get field descriptor for field at index 
GvFieldEntry * 
GvNode::getFieldEntry(int index) const
{
	ASSERT(index< getNumFields());
    return(getFieldData()->getFieldEntry(index)); 
}


// get pointer to field at index 
GvField *
GvNode::getField(int index) const
{
	ASSERT(index< getNumFields());

    GvFieldEntry *fieldEntry = getFieldData()->getFieldEntry(index);
    if (fieldEntry->offset <=0) return (NULL); // some events do not have a field e.g. GV_NODE_EVENT_IN

    // pointer arithmetic !!!! field is found at offset x
    // only ok for Nodes implementing fields as member variables
    return (GvField *) ((char *) (GvNode *) this +  fieldEntry->offset);

}

/*
// get scene node, this node belongs to
GvScene *GvNode::getScene() const
{
    return(GvScene::getCurrent());
}

*/


// compute flag if this node part of a PROTO definition body must be copied in each instance or not
GvBool
GvNode::computeCopyOnInstance(GvCopyContext &context) 
{
	if (context.alreadySeen(this)) 
		return copyOnInstance;
	context.setSeen(this);

    //if (copyOnInstance) return(TRUE); // already set // but need to compute on children
					 // vv no lazy eval !!!!
    copyOnInstance = getFieldData()->computeCopyOnInstance(this,context) || copyOnInstance;
    return(copyOnInstance);
}


GvBool GvNode::fieldsAreEqual(const GvNode *n) const
{
	// to do:
	return FALSE;
}

// copy all field values 
GvBool GvNode::copyFieldValues(const GvNode *srcNode) 
{

	if (!srcNode) return FALSE;
	if (!srcNode->IsA(this)) return FALSE; 	// compatible types ?
    int n= getNumFields();
    for(int i=0; i<n; i++) { // to think copy by name ??
		GvField *dest= getField(i); 
		if (dest) {

			GvField *src= srcNode->getField(i);
			if (!src) return FALSE;
			if (!dest->IsA(src)) return FALSE;// is it a compatible class, should be

			dest->set(src);
		}
	}
	return (TRUE);

}


// create a new instance of this node
GvNode * GvNode::newInstance()
{	// the class type info has a stored new function
	return  (GvNode*) this->Id().New();
}


// copy node recursivly, includes fieldValues
GvNode * GvNode::copy(GvNode * resultNode,GvBool deep)

{
 if (!resultNode) 
 	resultNode = newInstance();
 else 
 if (!resultNode->IsA(this)) 
     return NULL;

 if (!resultNode) return(NULL);
 
 resultNode->copyFieldValues(this);
 
 return(resultNode);
}

// mode = 1 allow clearance of fieldValues
int GvNode::checkForDefaultValues(int mode)
{
	return 0;
}


/*
  eval node to the actual value node of node,
  overwritten by ProtoInstance etc.
*/
GvNode*
GvNode::Eval(GTraversal &state)
{
   return(this);
}

GvNode*
GvNode::Eval()
{
   return(this);
}


// a field has been changed
int GvNode::OnFieldChanged(GvField *field) // virtual
{
	return(OnChanged());
}

// something  has been changed
int GvNode::OnChanged() // virtual 
{	//equivalent 
	// touch();
	modified = TRUE;
    NotifiyOwners();
    return(0);
}

#if 0
RTIMP(GvOwnerConnection,0,GvConnection);

int GvOwnerConnection::OnTrigger(GvNode *from) 
{ 
    return( to->OnChildChanged(from));
}

#endif



// owner connection
// add a connection from this node to owner
void GvNode::addOwner(GvNode *owner)
{
#if 0
	addConnection(new GvOwnerConnection(owner));
#endif

	ASSERT(owner != NULL);
	owners.append(owner);

	// xxxxxxxxxxxxxxxx m_browser = owner->m_browser;

}

void GvNode::removeOwner(GvNode *owner)
{
	ASSERT(owner != NULL);
	owners.remove(owner);
#if 0
    GvConnection ** prev = &connections;
    GvConnection *c = connections;
    
    while (c!= NULL) {
        GvConnection *next =  c->next;
		GvOwnerConnection *connection = RTCAST(c,GvOwnerConnection);

        if (connection  && connection->to == owner) { // found the owner 
            *prev = next;
            delete c;
            return;
        }
        prev = &c->next;
        c = next;

    }
	TRACE("GvNode::removeOwner:: Owner not found \n");
#endif
    return;
}


void GvNode::removeOwners()
{
	owners.truncate(0);

#if 0
    GvConnection ** prev = &connections;
    GvConnection *c = connections;
    
    while (c!= NULL) {
        GvConnection *next =  c->next;
		GvOwnerConnection *connection = RTCAST(c,GvOwnerConnection);

        if (connection) {
            *prev = next;
            delete c;
        } else 
          prev = &c->next;
        c = next;

    }
#endif
    return;
}

GvBool GvNode::replaceChildNode(GvNode *oldNode,GvNode *newNode)
{
	ASSERT(oldNode != NULL);

	return FALSE;
}

GvBool GvNode::replaceMe(GvNode *newNode)
{   
	GvBool ret = FALSE;
	ref();
	for(int i =owners.getLength()-1; i>=0; i--) {
		GvNode * owner = owners[i];

		ret = owner->replaceChildNode(this,newNode);
	}
	unref();
	return ret;
}



	
// some child node has been changed
int GvNode::OnChildChanged(GvNode *child)
{
   return(0);
}

// notify all registered owners, that this child has been changed
int GvNode::NotifiyOwners()
{
//	ref();
	int ret= -1;
	for(int i =owners.getLength()-1; i>=0; i--) {
		GvNode * owner = owners[i];
		ASSERT(owner != NULL);
		ret = owner->OnChildChanged(this);
	}
//	unref();
	return ret;


#if 0
    GvConnection *c = connections;
    
    while (c!= NULL) {
		GvOwnerConnection *connection = RTCAST(c,GvOwnerConnection);
		if ( connection) connection->OnTrigger(this);
        c = c->next;
    }
    return(1);
#endif
}

   
// delete all connections
int GvNode::deleteConnections()
{
    GvConnection *c = connections;
	connections = NULL;

    while (c!= NULL) {
        GvConnection *next =  c->next;
        delete c;
        c= next;
    }
    return(0);

}
    
// add a connection, and optionally trigger the value
int GvNode::addConnection(GvConnection *connection,GvBool trigger)
{
    ASSERT(connection!=NULL);

    connection->next = connections;
    connections= connection;
    // if (trigger)         connection->OnTrigger(this);
    return(1);
}
   
// remove a specific connection
int GvNode::removeConnection(GvConnection *connection)
{
    GvConnection ** prev = &connections;
    GvConnection *c = connections;
    
    while (c!= NULL) {

		ASSERT_POINTER(c,GvConnection);

        GvConnection *next =  c->next;
        if (c == connection) {
            *prev = next;
            delete c;
            return(1);
        }
        prev = &c->next;
        c = next;
    }
    return(0);
}

// remove a specific connection filtered by predicate function

GvBool GvNode::removeConnection(GvBool (*Pred)(GvConnection *c, void *data),void *data)
{
    GvConnection ** prev = &connections;
    GvConnection *c = connections;
    while (c!= NULL) {
		
		ASSERT_POINTER(c,GvConnection);

        GvConnection *next =  c->next;
        if (Pred(c,data)) {
            *prev = next;
			TRACE("GvNode::removeConnection c = %p  Node = %p\n",c,this);
            delete c;
            return(TRUE);
        }
        prev = &c->next;
        c = next;
    }
    return(FALSE);

}

// find a specific connection filtered by predicate function
GvConnection* 
GvNode::findConnection(GvBool (*Pred)(GvConnection *c, void *data),void *data)
{
    GvConnection *c = connections;
    while (c!= NULL) {
		
		ASSERT_POINTER(c,GvConnection);

        GvConnection *next =  c->next;
        if (Pred(c,data)) {
            return(c);
        }
        c = next;
    }
    return(NULL);

}




    
// find input event	field
// note: input node methods could not be handled this way 
GvField * GvNode::getEventIn(const char *event)
{
	int eventIndex = getFieldData()->getEventInIndex(this,event);
	if (eventIndex <0) return(NULL);

    GvField *f= getField(eventIndex);
    if (f) return(f);
    
    return(f);
}


// find output event field
GvField * GvNode::getEventOut(const char *event)
{
	int eventIndex = getFieldData()->getEventOutIndex(this,event);
	if (eventIndex <0) return(NULL);

    GvField *f= getField(eventIndex);
    if (f) return(f);

    return(f);
}



GvBool
GvNode::addRoute(GvNode * from, int fromIndex, GvNode * to,int toIndex)
{
	ASSERT(to!= NULL);
	ASSERT(from!= NULL);

    GvFieldData* fromData = from->getFieldData();
    GvFieldData* toData = to->getFieldData();

    GvField *fromField = from->getField(fromIndex);

    GvFieldEntry *toFieldEntry = toData->getFieldEntry(toIndex);
    GvField *toField = to->getField(toIndex);

    ASSERT(fromField);
    if (!fromField) return(FALSE);

    if ((from == to) && (fromIndex == toIndex)) {
			// fully circular route 
			// diagnostic only : ASSERT(0);
			return FALSE;
	}
	
	ASSERT(toFieldEntry->matchFieldType(fromField));


    switch(toFieldEntry->type) {
    case GV_NODE_EVENT_IN: 
			{

			// find out if route already there, bad 

//            GvNodeMethodConnection *c = new GvNodeMethodConnection(to, (GvNodeEventInMethod ) (toFieldEntry->method));
            GvNodeMethodConnection *c = new GvNodeMethodConnection
				(from,fromIndex,to,toIndex, (GvNodeEventInMethod ) (toFieldEntry->method));

		    GvConnection* alreadyThere = fromField->findConnection(PredToNode,c);

			if (alreadyThere) {
				TRACE("duplicate route to method \n ");
				delete c;
				return FALSE;
			}

            fromField->addConnection(c,0);
			ASSERT(from != to);
			if (from != to) {
				GvRouteFromConnection *cfrom = new GvRouteFromConnection(c);
				to->addConnection(cfrom,0);
			}

			}
            break;
    default:
        {
            ASSERT(toField);
            if (!toField) return(FALSE);
            if ( (from == to) && (fromIndex == toIndex)) 
				return FALSE;

			// find out if route alredy there, bad 
		    GvConnection* alreadyThere = fromField->findConnection(PredToField,toField);

			if (alreadyThere) {
				TRACE("duplicate route \n ");
				return FALSE;
			}
            
            // add a forward connection 
//          GvSetFieldConnection *c = new GvSetFieldConnection(toField);
			
			GvSetFieldConnection *c = new GvSetFieldConnection(from,fromIndex,to,toIndex,toField);

            fromField->addConnection(c,0); // do not auto trigger 
			// diagnostic only : ASSERT(from != to);

			// add backward connection to let "to" know of any writing nodes
			if (from != to) { // if this is destructed , everything is fine
				GvRouteFromConnection *cfrom = new GvRouteFromConnection(c);
				to->addConnection(cfrom,0);
			}

        }
        break;
    }
	// set flags 
	from->hasOutRoute = 1;
	to->hasInRoute = 1;

    return(TRUE);
}

/* send event fromField to Node to : eventIn toIndex */
GvBool
GvNode::sendEventIn(GvField *fromField, GvNode * to,int toIndex)
{

    GvFieldData* toData = to->getFieldData();
    GvFieldEntry *toFieldEntry = toData->getFieldEntry(toIndex);
    GvField *toField = to->getField(toIndex);

    ASSERT(fromField);

    if (!fromField) return(FALSE);

    switch(toFieldEntry->type) {
    case GV_NODE_EVENT_IN: 
			{
            GvNodeMethodConnection *c = 
				new GvNodeMethodConnection(NULL,-1,to,toIndex, (GvNodeEventInMethod ) (toFieldEntry->method));
            c->OnTrigger(fromField);
			delete c;
			}
            break;
    default:
        {
            ASSERT(toField);
            if (!toField) return(FALSE);
			// shortcut 
			toField->set(fromField);

/*            
            GvSetFieldConnection *c = new GvSetFieldConnection(this,-1,to,toIndex,toField);
            c->OnTrigger(fromField);
			delete c;
*/

        }
        break;
    }
    return(TRUE);
}


/*
   add route by event names
   and report any errors,
   store route descriptor in scene meta node
*/

GvBool 
GvNode::addRoute(GvInput *in,
        GvNode *from, const GvName& fromEvent,
        GvNode *to, const GvName& toEvent)
{
    GvFieldData* fromData = from->getFieldData();
    GvFieldData* toData = to->getFieldData();

    int fromIndex = fromData->getEventOutIndex(from,fromEvent);
    if (fromIndex<0) {
       GvReadError::post(in,GV_RE_UNKNOWN_FIELD, "%s . %s ,no such eventOut",from->getName().getString(),fromEvent.getString());
	   return FALSE;
    }

    GvFieldEntry *fromFieldEntry = fromData->getFieldEntry(fromIndex);

    int toIndex = toData->getEventInIndex(to,toEvent,fromFieldEntry->eventTypeId);
    if (toIndex<0) {
       GvReadError::post(in,GV_RE_UNKNOWN_FIELD, "%s . %s, no eventIn of type %s ",to->getName().getString(),toEvent.getString(),fromFieldEntry->getType());
	   return FALSE;
    }


    // any PROTO routes must be instanced per Instance !!
    GvProto *proto = in->currentProto;
    if (proto) {
        return(proto->addRoute(from,fromIndex,to,toIndex));
    }

	// keep track of routes
	GvScene * scene = in->getScene();
    if (scene) {
		return(scene->addRoute(from,fromIndex,to,toIndex));
	} else {
		TRACE("Adding route but no scene node \n");
		return(addRoute(from,fromIndex,to,toIndex));
	}

    return (TRUE);
}



GvBool
GvNode::deleteRoute(GvNode * from, int fromIndex, GvNode * to,int toIndex)
{
	ASSERT(to!= NULL);
	ASSERT(from!= NULL);

    GvFieldData* fromData = from->getFieldData();
    GvFieldData* toData = to->getFieldData();

    GvField *fromField = from->getField(fromIndex);

    GvFieldEntry *toFieldEntry = toData->getFieldEntry(toIndex);
    GvField *toField = to->getField(toIndex);

    ASSERT(fromField);
    if (!fromField) return(FALSE);
	GvBool ret;

    switch(toFieldEntry->type) {
    case GV_NODE_EVENT_IN: 
			{
            GvNodeMethodConnection *c = new GvNodeMethodConnection(from,fromIndex,to,toIndex, (GvNodeEventInMethod ) (toFieldEntry->method));
            ret = fromField->removeConnection(PredToNode,c);
			delete c;
			}
            break;
    default:
        {
            ASSERT(toField);
            if (!toField) return(FALSE);
            ret = fromField->removeConnection(PredToField,toField);
        }
        break;
    }

	// to do: update flags 
	//from->hasOutRoute = 0;
	//to->hasInRoute = 0;

	// remove node connections on to, pointing back to ROUTE connection 
	// automatically done in : GvRouteToConnection::~GvRouteToConnection()
	//  loop is for testing and recomputing to->hasInRoute flag, no necessary, may be speed up destructor 
#if 1
	if (to->hasInRoute) {
		GvConnection *c = to->connections;
		int hasInRoute=0;
		while (c!= NULL) {
			GvConnection *next =  c->next;
			if (RTISA(c,GvRouteFromConnection)) {
				hasInRoute=1;

				break;
			}				
			c= next;
		}
		to->hasInRoute=hasInRoute;
	}
#endif

    return(TRUE);
}


// read target connection 
// NodeName . EventOutName
GvBool GvNode::readRoute(GvInput *in)
{
    GvNodeHandle from;
    GvName fromEvent;

    GvNodeHandle to;
    GvName toEvent;
    char c;



    // from 
    from = GvNode::readReference(in);
    if (!from) return(FALSE);
	
	ASSERT(from->refCnt>0); //debug: at least one reference from scenegraph 
	if (from->refCnt<=0) return(FALSE); // for now 

    if (in->read(c) && c == '.') {
    } else {
	      in->putBack(c);
          GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Couldn't read ROUTE source event, '.' expected .\n" );
	      return FALSE;
    }
    
    if (in->read(fromEvent, TRUE)) {
    }
    else {
          GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Couldn't read ROUTE source event, event name expected.\n" );
	      return FALSE;
    }


    { GvName keyword;
        if (in->read(keyword, TRUE) && keyword == GvKeyword::to) {
        }
        else {
          GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Error reading ROUTE, \"TO\" expected.\n" );
	      return FALSE;
        }
    }

    // to 
    to = GvNode::readReference(in);
    if (!to) return(FALSE);

	ASSERT(to->refCnt>0); //debug: at least one reference from scenegraph 
   

    if (in->read(c) && c == '.') {
    } else {
	      in->putBack(c);
          GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Couldn't read ROUTE target event, '.' expected .\n" );
	      return FALSE;
    }
    
    if (in->read(toEvent, TRUE)) {
    }
    else {
          GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Couldn't read ROUTE source event, event name expected.\n" );
	      return FALSE;
    }

	// one ref from scene graph pointer, another from handle above
	ASSERT(from->refCnt >=2);
	ASSERT(to->refCnt >=2);
	
	if ((from->refCnt >= 2) && (to->refCnt >= 2)) {
		GvBool ret = addRoute(in,from,fromEvent,to,toEvent);
		// not fatal
        if (!ret) GvReadError::post(in, GV_RE_BAD_ROUTE,"Warning could not add ROUTE, duplicate route ?\n" );
	}

    return (TRUE);
}



// read functions

// is name a ROUTE, PROTO, EXTERNPROTO keyword ?

GvBool
GvNode::isRouteOrProto(const GvName &name)
{
    if (name == GvKeyword::proto) { //VRML 2.0 Proto
        return TRUE;
    }
    else if (name == GvKeyword::externProto) { //VRML 2.0 Extern Proto
        return TRUE;
    }
    else if (name == GvKeyword::route) { //VRML 2.0 Route
        return TRUE;
    }
    else {
        return FALSE;
    }
    
}

// Read a single  ROUTE, PROTO, EXTERNPROTO 

GvBool
GvNode::readRouteOrProto(GvInput *in,const GvName &name)
{
    GvBool	ret;

    if (name == GvKeyword::proto) { //VRML 2.0 Proto
		GvNodeHandle node = GvProto::read(in);
	  	ret = (node != (GvNode*) NULL);
    }
    else if (name == GvKeyword::externProto) { //VRML 2.0 Extern Proto
		GvNodeHandle node = GvExternProto::read(in);
	  	ret = (node != (GvNode*)NULL);
    }
    else if (name == GvKeyword::route) { //VRML 2.0 Route
		ret = readRoute(in);
    }
    else {
        return FALSE;
    }
    return (ret);
}

GvBool
GvNode::readRoutesOrProtos(GvInput *in)
{
    GvBool	ret;
    GvName	name;

    do {
    
    if (! in->read(name, TRUE)) {
		ret = in->headerOk;
        return(ret);
    }
    else if (name == GvKeyword::proto) { //VRML 2.0 Proto
		GvNodeHandle node = GvProto::read(in);
	  	ret = (node != (GvNode*) NULL);
    }
    else if (name == GvKeyword::externProto) { //VRML 2.0 Extern Proto
		GvNodeHandle node = GvExternProto::read(in);
	  	ret = (node != (GvNode*)NULL);
    }
    else if (name == GvKeyword::route) { //VRML 2.0 Route
		ret = readRoute(in);
    }
    else {
        in->putBack(name.getString());
		return  TRUE;
    }
    
    } while (ret == TRUE);

    return ret;
}

//
// top level read
// reads
//		NULL
//		REF readReference
//		PROTO readProto
//		EXTERNPROTO readExternProto
//		readNode
//      ROUTE
// Read any ROUTE, PROTO, EXTERNPROTO until a new node or no name

GvBool
GvNode::read(GvInput *in, GvNode *&node)
{
    GvBool	ret;
    GvName	name;

    do {
    
    if (!in->read(name, TRUE)) {
		node = NULL;
		// ret = in->headerOk;
		return FALSE;
        return(ret);
    }

    else if (! name || name == GvKeyword::null) {
		node = NULL;
		return(TRUE);
    }

    else if (name == GvKeyword::use) {
	  	node = readReference(in);
	  	return  (node != NULL);
    }
    else if (name == GvKeyword::proto) { //VRML 2.0 Proto
		GvNodeHandle nnode = GvProto::read(in);
	  	ret = (nnode != (GvNode*) NULL);
		node = nnode;
    }
    else if (name == GvKeyword::externProto) { //VRML 2.0 Extern Proto
		GvNodeHandle nnode = GvExternProto::read(in);
	  	ret = (nnode != (GvNode*)NULL);
		node = nnode;
    }
    else if (name == GvKeyword::route) { //VRML 2.0 Route
        node = NULL;
		ret = readRoute(in);
    }
    else
		return  readNode(in, name, node);
    } while (ret == TRUE);

    return ret;
}

// reads the field list of node
GvBool
GvNode::readInstance(GvInput *in)
{
    GvName	typeString;
    GvFieldData	*fieldData = getFieldData();
#ifdef _G_VRML1
    if (!in->isVrml2()) {
        if (in->read(typeString, TRUE)) {
	    if (typeString == "fields") {
	        if (! fieldData->readFieldTypes(in, this)) {
		    GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Bad field specifications for node");
		    return FALSE;
	    }
	    }
	    else
	        in->putBack(typeString.getString());
        }
    }
#endif

    if (! fieldData->read(in, this))
        return FALSE;

    return TRUE;
}

GvNode *
GvNode::readReference(GvInput *in)
{
    GvName	refName;
    GvNode	*node;
    //if (! in->read(refName, FALSE)) // was FALSE TRUE : want valid name in vrml2, string reads . as character but to restrictive

    if (!in->readNodeName(refName)) { 
	    GvReadError::post(in,GV_RE_UNEXPECTED_EOF, "Premature end of file after " REFERENCE_KEYWORD);
	    node = NULL;
    }
    else if ((node = in->findReference(refName)) == NULL)
	    GvReadError::post(in, GV_RE_UNKNOWN_NODE,"Unknown node reference \"%s\"",  refName.getString());

    return node;
}

// reads DEF name { readNodeInstance }

GvBool
GvNode::readNode(GvInput *in, GvName &className, GvNode *&node)
{
    GvBool	gotChar;
    GvName	refName;
    char	c;
    GvBool	ret = TRUE, flush = FALSE;

    node = NULL;

    if (className == GvKeyword::def) { // its a DEF name Class { } 
		// in->read(refName, TRUE ) to restrictive  false no string verification 15.10.96 
		// if (! in->read(refName, FALSE ) || ! in->read(className, TRUE)) 

		if (! in->readNodeName(refName) || ! in->read(className, TRUE)) {
			GvReadError::post(in, GV_RE_UNEXPECTED_EOF,"Premature end of file or bad character after %s", DEFINITION_KEYWORD);
			ret = FALSE;
		}

		if (!refName) {
			GvReadError::post(in,GV_RE_SYNTAX_ERROR, "No valid name given after %s", DEFINITION_KEYWORD);
			ret = FALSE;
		}

		if (!className) {
			GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Invalid definition of %s", refName.getString());
			ret = FALSE;
		}
    }

    if (ret) {

	if (! (gotChar = in->read(c)) || c != OPEN_BRACE) {
	    if (gotChar)
		GvReadError::post(in, GV_RE_SYNTAX_ERROR, "(%s) Expected '%c'; got '%c', non standard field ? ",
				className.getString(), OPEN_BRACE, c);
	    else
		GvReadError::post(in,GV_RE_UNEXPECTED_EOF, "(%s) Expected '%c'; got EOF",className.getString(), OPEN_BRACE);
	    ret = FALSE;
	}

	else {

	    ret = readNodeInstance(in, className, refName, node);

	    if (! ret)
			flush = TRUE;

	    else if (! (gotChar = in->read(c)) || c != CLOSE_BRACE) {
		ret = FALSE;
		
		if (gotChar) {
		    GvReadError::post(in,GV_RE_SYNTAX_ERROR, "(%s) Expected '%c'; got '%c', ingoring extra data",
				      className.getString(),
				      CLOSE_BRACE, c);
			
			// HG skip unknown fields until CLOSE_BRACE fount
			int level = 1;
			while (gotChar = in->read(c)) {
			  if (c == CLOSE_BRACE) {
			  	level --;
				if (level == 0) {
				   ret = TRUE; break;
				};
			  } else  
			  if (c == OPEN_BRACE) {
			  	level ++;
			  }
			}
		}
		else
		    GvReadError::post(in,GV_RE_UNEXPECTED_EOF, "(%s) Expected '%c'; got EOF",
					  className.getString(),
				      CLOSE_BRACE);
	    }
	}
    }

    if (!ret && flush)
		flushInput(in);

    return ret;
}

// creates nodeInstance, adds refname and readsInstance
GvBool
GvNode::readNodeInstance(GvInput *in, const GvName &className,
			 const GvName &refName, GvNode *&node)
{
    node = createInstance(in, className);
    if (node == NULL)
		return FALSE;

    if (! (! refName))
		in->addReference(refName, node);

	node->ref(); // keep a temp ref or better replace argument by GvNodeHandle
		GvBool ret = node->readInstance(in);
	node->unrefNoDelete();
	return ret;


}

GvNode *
GvNode::createInstance(GvInput *in, const GvName &className)
{
    GvNode		*instance;
    GvString    unknownString;

	//  check Protos first ?? TO DO: urn.inet.glview.com etc 

    // is there a proto defined for this name ? 

    GvProto* proto = in->findProto(className);
	if (proto == NULL) // no PROTO for this name, create built in node
		instance = createInstanceFromName(className,in->isVrml2());
	else instance = NULL;

    if (instance == NULL) {
        // create by creating ProtoInstance Node for name
		instance = createProtoInstance(in,className);
		if (instance) { 
			// for savety set browser member 
			if (in->getScene()) instance->setBrowser(in->getScene()->getBrowser());
			return(instance);
		}

#ifdef _G_VRML1

	// VRML 1.0 :
	if (! in->read(unknownString) || unknownString != FIELDS_KEYWORD) {
	    GvReadError::post(in, GV_RE_UNKNOWN_NODE,"Unknown class \"%s\"",
			      className.getString());
	    // return NULL;
		// HG return a GvUnknownNode
		GvUnknownNode *tmpNode = new GvUnknownNode;
	    tmpNode->setClassName(className.getString());
	    instance = tmpNode;

	}

	else if (unknownString == FIELDS_KEYWORD) {
	    GvUnknownNode *tmpNode = new GvUnknownNode;
	    tmpNode->setClassName(className.getString());
	    instance = tmpNode;
	    in->putBack(unknownString.getString());
	}
#else

	 GvReadError::post(in, GV_RE_UNKNOWN_NODE, "Unknown Node class \"%s\"", className.getString());

#endif

    }
	// for savety set browser member 
	if (instance && in->getScene()) instance->setBrowser(in->getScene()->getBrowser());
    return instance;
}


// try to create a ProtoInstance node of Proto with name classname
// see also GvScene::createProtoInstance
GvNode *
GvNode::createProtoInstance(GvInput *in, const GvName &className)
{
    GvProto* proto = in->findProto(className);
    if (!proto) {   // no proto found
        return(NULL);
    }
    GvNode		*instance;
	instance = proto->createInstance();
	// TRACE("CreateProtoInstance : %s \n",className.getString());
	return(instance);
}

// **** statics 
// the dictionary of the New functions of all VRML nodes
GvDict classDict;
GvBool classDictInit=FALSE;
GvPList classTermFuncs;
// 


void addNodeClass(const GvName &name, RTRoot* (*newFunc)()) 
{
   classDict.enter((u_long) name.getString(), (void *) newFunc);
}

#define ADD_CLASS(name) \
    addNodeClass(strq(name),&Gv##name::New); \
    classTermFuncs.append((void *) &Gv##name::termClass); \


#define ADD_CLASS2(name,classy) \
    addNodeClass(strq(name),& classy::New); \
    classTermFuncs.append((void *) &classy::termClass); \

#define DEFINE_CLASS(name) \
    classTermFuncs.append((void *) &Gv##name::termClass); \

#define DEFINE_CLASS2(name,classy) \
    classTermFuncs.append((void *) &classy::termClass); \

/*
   init dictionary of class
   by storing a class constructor function for each node
*/

void initClassTable() 
{
    classDictInit = TRUE;
    // more VRML 2.0 nodes
    ADD_CLASS2(Collision,	Gv2Collision);
    ADD_CLASS2(Selection,	Gv2Selection);
    ADD_CLASS2(Billboard,	Gv2Billboard);
    ADD_CLASS2(Anchor,		Gv2Anchor);
    
    ADD_CLASS(Shape);
    ADD_CLASS(Appearance);
    ADD_CLASS(Coordinate);
    ADD_CLASS(Color);
    ADD_CLASS(TextureCoordinate);
    ADD_CLASS2(TextureTransform,	Gv2TextureTransform);
    
	ADD_CLASS(ImageTexture);
    ADD_CLASS(PixelTexture);
    ADD_CLASS(MovieTexture);
    ADD_CLASS(Viewpoint);

    ADD_CLASS(WorldInfo);
    ADD_CLASS(NavigationInfo);
    ADD_CLASS(Background);
    ADD_CLASS(Fog);
    ADD_CLASS(AudioClip);
    ADD_CLASS(Sound);
    ADD_CLASS(ElevationGrid);

    ADD_CLASS(DirectionalLight); // 1 && 2 
    ADD_CLASS(PointLight);
    ADD_CLASS(SpotLight);

	// 1.0 nodes 
#ifdef _G_VRML1
	ADD_CLASS(Cone);
    ADD_CLASS(Coordinate3);
    ADD_CLASS(Cube);
    ADD_CLASS(Cylinder);
    ADD_CLASS(Group);
    ADD_CLASS(IndexedFaceSet);
    ADD_CLASS(IndexedLineSet);
    ADD_CLASS(Info);
    ADD_CLASS(LevelOfDetail);
    ADD_CLASS(Material);
    ADD_CLASS(MaterialBinding);
    ADD_CLASS(MatrixTransform);
    ADD_CLASS(Normal);
    ADD_CLASS(NormalBinding);
    ADD_CLASS(OrthographicCamera);
    ADD_CLASS(Pendulum);
    ADD_CLASS(PerspectiveCamera);
    ADD_CLASS(PointSet);
    ADD_CLASS(Rotation);
    ADD_CLASS(Rotor); // hg
    ADD_CLASS(Scale);
    ADD_CLASS(Separator);
    ADD_CLASS(ShapeHints);
    ADD_CLASS(Sphere);
    ADD_CLASS(Switch);
    ADD_CLASS(Texture2);
    ADD_CLASS(Texture2Transform);
    ADD_CLASS(TextureCoordinate2);
    ADD_CLASS(TextureRotor); // HG
    ADD_CLASS(Transform);
    ADD_CLASS(TransformSeparator);
    ADD_CLASS(Translation);
    ADD_CLASS(WWWAnchor);
    ADD_CLASS(WWWInline);
    ADD_CLASS(AsciiText);
    ADD_CLASS2(Text3,	GvAsciiText); // inventor Text3 node, extruded + using profile to do:check
    ADD_CLASS(BaseColor);
    ADD_CLASS(Complexity);
    ADD_CLASS(Callback);
    ADD_CLASS(Shuttle);
    ADD_CLASS(Morph);
    ADD_CLASS(LOD);	
    ADD_CLASS(Label);	
    ADD_CLASS(IndexedTriangleStripSet);	
    ADD_CLASS(FontStyle);	
    ADD_CLASS(TextureCoordinateBinding);	
    ADD_CLASS(WWWRegionAnchor);	
    ADD_CLASS(FaceSet);	
    ADD_CLASS(TriangleStripSet);	
    ADD_CLASS(QuadMesh);	
    ADD_CLASS(Blinker);	
    ADD_CLASS(Sweep);	
    ADD_CLASS(RotationXYZ);
    ADD_CLASS(TextureShuttle);
    ADD_CLASS(TimeTransform);
    ADD_CLASS(LightModel);
    ADD_CLASS(ImageElevationGrid);
    ADD_CLASS(Image);
    ADD_CLASS(ScaledImage);

    // webfx.h
    ADD_CLASS(AxisAlignment);
    ADD_CLASS(BrowserHints);
    ADD_CLASS(Animator);
    ADD_CLASS(CollideStyle);
    ADD_CLASS(CollisionDetection);
    ADD_CLASS(Sprite);
    ADD_CLASS(SpinGroup);
    ADD_CLASS(Spin);
    ADD_CLASS(DirectedSound);
    ADD_CLASS(PointSound);

    
    // hg
    ADD_CLASS(Layer);
    ADD_CLASS(ColorGradient);

#endif _G_VRML1

	// VRML 2
    ADD_CLASS(Extrusion);
    //GvSensors.h

    ADD_CLASS(TimeSensor);
    ADD_CLASS(CylinderSensor);
    ADD_CLASS(PlaneSensor);
    ADD_CLASS(ProximitySensor);
    ADD_CLASS(SphereSensor);
    ADD_CLASS(TouchSensor);
    ADD_CLASS(VisibilitySensor);

    //GvInterpolators.h
    ADD_CLASS(CoordinateInterpolator);
    ADD_CLASS(PositionInterpolator);
    ADD_CLASS(NormalInterpolator);
    ADD_CLASS(Coordinate2Interpolator);
    ADD_CLASS(ColorInterpolator);
    ADD_CLASS(OrientationInterpolator);
    ADD_CLASS(ScalarInterpolator);

#ifdef _G_VRML1

    //KTX
    ADD_CLASS2(AimTarget,GvAimTarget_ktx_com);

    ADD_CLASS(AnimationFrames_ktx_com);
    ADD_CLASS(AnimationStyle_ktx_com);
    ADD_CLASS(PositionKey_ktx_com);
    ADD_CLASS(RotationKey_ktx_com);
    ADD_CLASS(HideKey_ktx_com);
    ADD_CLASS(ScaleKey_ktx_com);
    ADD_CLASS(ColorKey_ktx_com);
    ADD_CLASS(AimTarget_ktx_com);
    ADD_CLASS(ReferenceMark_ktx_com);
#endif

	// CC3D extra
    ADD_CLASS2(CullGroup,	Gv2CullGroup);
    ADD_CLASS(Avatar);
    ADD_CLASS(MenuSensor); 
    ADD_CLASS(RayHitSensor);
	ADD_CLASS(BspTree);
	ADD_CLASS(BspGroup);
	ADD_CLASS(Fog2);
	ADD_CLASS(Occlusion);
	ADD_CLASS(Inclusion);
	ADD_CLASS(Metaverse);

	// new CC3D 4.0
	ADD_CLASS(HUD);
	ADD_CLASS(NurbsSurface);
	ADD_CLASS(NurbsGroup);
	ADD_CLASS(NurbsPositionInterpolator);
	ADD_CLASS(CoordinateDeformer);

	// 4.1
	ADD_CLASS(NurbsCurve);

    ADD_CLASS(MouseSensor);

#ifdef _G_INVENTOR

    ADD_CLASS(NurbsSurfaceInventor);
    ADD_CLASS(NurbsCurveInventor);
    ADD_CLASS(NurbsProfile);
    ADD_CLASS(LinearProfile);

    //new iv states
	ADD_CLASS(Coordinate4);
    ADD_CLASS(ProfileCoordinate2);
    ADD_CLASS(ProfileCoordinate3);

#endif

#ifdef G_METASTREAM
	ADD_CLASS(MetaGroup);
	ADD_CLASS(MetaGeometry);
#endif

#ifdef G_HEAD
	ADD_CLASS(HeadModel);
#endif

	//4.3
	ADD_CLASS(NurbsCurve2D);
	ADD_CLASS(TrimmedSurface);

	ADD_CLASS(Polyline2D);
	ADD_CLASS(Contour2D);
	ADD_CLASS(Polygon);

    ADD_CLASS(Layer3D);

    ADD_CLASS(MultiTexture);


#if !defined(_GV_MINIMAL)

	// hg GSurface
	ADD_CLASS2(Circle,GCircle);
	//ADD_CLASS2(Spiral,GSpiral);
	//ADD_CLASS(Sinus,GSinus);
	ADD_CLASS2(SphereSurface,GSphereSurface);
	ADD_CLASS2(CylinderSurface,GCylinderSurface);
	ADD_CLASS2(XYPlaneSurface,GXYPlaneSurface);


	ADD_CLASS2(CurveDisplay,GCurveDisplay);
	ADD_CLASS2(SurfaceDisplay,GSurfaceDisplay);
#endif

    DEFINE_CLASS2("Group",			Gv2Group);
    DEFINE_CLASS2("Transform",		Gv2Transform);

    DEFINE_CLASS2("Box",		    GvBox);
    DEFINE_CLASS2("Cone",		    Gv2Cone);
    DEFINE_CLASS2("Cylinder",	    Gv2Cylinder);
    DEFINE_CLASS2("IndexedFaceSet",	Gv2IndexedFaceSet);
    DEFINE_CLASS2("IndexedLineSet", Gv2IndexedLineSet);
    DEFINE_CLASS2("PointSet",       Gv2PointSet);
    DEFINE_CLASS2("Sphere",		    Gv2Sphere);
    DEFINE_CLASS2("Text",		    Gv2Text);
    DEFINE_CLASS2("FontStyle",	    Gv2FontStyle);
    DEFINE_CLASS2("Normal",		    Gv2Normal);
    DEFINE_CLASS2("Material",	    Gv2Material);
    DEFINE_CLASS2("Script",	        GvScript);
    DEFINE_CLASS2("Switch",	        Gv2Switch);
    DEFINE_CLASS2("LOD",	        Gv2LOD);
    DEFINE_CLASS2("Inline",	        Gv2Inline);
	// extra
    DEFINE_CLASS2("MatrixTransform",      Gv2MatrixTransform);
	
	// helper classes 

    DEFINE_CLASS2("Scene",       GvScene);
    DEFINE_CLASS2("CreateVrmlFromUrl",       Gv2CreateVrmlFromUrl);
    DEFINE_CLASS2("RayHitInfo",  GvRayHitInfo);
    DEFINE_CLASS2("NodeShell",   GvNodeShell);
    DEFINE_CLASS(Sensor);
    DEFINE_CLASS(GeometrySensor);
    DEFINE_CLASS(Texture);
    DEFINE_CLASS(Light);
    DEFINE_CLASS(Interpolator);
    DEFINE_CLASS2("ROUTE",   GvRoute);
    DEFINE_CLASS2("PROTO",   GvProto);
    DEFINE_CLASS2("EXTERNPROTO",   GvExternProto);
    DEFINE_CLASS2("PROTO-INSTANCE",   GvProtoInstance);
    
	DEFINE_CLASS(Event);

    //DEFINE_CLASS2("View",  GView);

};

void termClassTable() 
{

	// call class termers 
	for (int i=0; i<classTermFuncs.getLength(); i++) {
		void (*func)() = (void (*)()) classTermFuncs[i];
		(*func)();
	}

	classTermFuncs.truncate(0);

	classDict.clear();
    classDictInit = FALSE;
};



GvNode *
GvNode::createInstanceFromName(const GvName &className,int vrml2)
{
#define TRY_CLASS(name, class)						      \
    else if (className == name)						      \
	inst = new class

    GvNode *inst = NULL;

    if (vrml2)  {
    if (0) ;	
    // conflicting 2.0 nodes 
    // if not (strsuffix( _10)
    TRY_CLASS("Group",			Gv2Group);
    TRY_CLASS("Transform",		Gv2Transform);

    TRY_CLASS("Box",		    GvBox);
    TRY_CLASS("Cone",		    Gv2Cone);
    TRY_CLASS("Cylinder",	    Gv2Cylinder);
    TRY_CLASS("IndexedFaceSet",	Gv2IndexedFaceSet);
    TRY_CLASS("IndexedLineSet", Gv2IndexedLineSet);
    TRY_CLASS("PointSet",       Gv2PointSet);
    TRY_CLASS("Sphere",		    Gv2Sphere);
    TRY_CLASS("Text",		    Gv2Text);
    TRY_CLASS("FontStyle",	    Gv2FontStyle);
    TRY_CLASS("Normal",		    Gv2Normal);
    TRY_CLASS("Material",	    Gv2Material);
    TRY_CLASS("Script",	        GvScript);
    TRY_CLASS("Switch",	        Gv2Switch);
    TRY_CLASS("LOD",	        Gv2LOD);
    TRY_CLASS("Inline",	        Gv2Inline);
	// extra
    TRY_CLASS("MatrixTransform",Gv2MatrixTransform);

    if (inst!= NULL) return(inst);
	
	}
	else//if not vrml2, i.e. open inventor
	{
	#ifdef _G_INVENTOR
		if (className == "NurbsSurface")
			return new GvNurbsSurfaceInventor();
		if (className == "NurbsCurve")
			return new GvNurbsCurveInventor();
	#endif
	}

    // initialize class table
    if (!classDictInit)
        initClassTable();

    void	*val;

    if (classDict.find((u_long) className.getString(), val)) {
        RTRoot* (*newFunc)() = (RTRoot* (*)()) val;
	    return (GvNode *) newFunc();
    }
    else return NULL;


    return inst;

#undef TRY_CLASS
}

//
// GvNode
// 
void
GvNode::init()
{
#ifdef GV_GLOBAL_DICT
    if (nameDict == NULL)
	    nameDict = new GvDict;
#endif
	
	TRACE("Sizeof(GvField) = %d \n",sizeof(GvField));
	TRACE("Sizeof(GvSFFloat) = %d\n",sizeof(GvSFFloat));
	TRACE("Sizeof(GvSFRotation) = %d\n",sizeof(GvSFRotation));
	TRACE("Sizeof(GNode) = %d\n",sizeof(GNode));

#ifdef _COM
	TRACE("Sizeof(CComObjectRoot) = %d\n",sizeof(CComObjectRoot));
#endif

	TRACE("Sizeof(GvNode) = %d\n",sizeof(GvNode));
	TRACE("Sizeof(Gv2Group) = %d\n",sizeof(Gv2Group));
	TRACE("Sizeof(Gv2Transform) = %d\n",sizeof(Gv2Transform));

	// initialize class table
    if (!classDictInit)
		initClassTable();

}

void
GvNode::term()
{
#ifdef GV_GLOBAL_DICT
    if (nameDict != NULL) {
	    delete nameDict;
        nameDict = NULL;
    }
#endif
	termClassTable();

}


// read until closing brace 
void
GvNode::flushInput(GvInput *in)
{
    int		nestLevel = 1;
    char	c;

    while (nestLevel > 0 && in->get(c)) {

		if (c == CLOSE_BRACE)
			nestLevel--;
		else if (c == OPEN_BRACE)
			nestLevel++;
    }
}
/*
#undef OPEN_BRACE
#undef CLOSE_BRACE
#undef DEFINITION_KEYWORD
#undef REFERENCE_KEYWORD
#undef NULL_KEYWORD
*/


//
// GvCopyContext
//

// define  that oldnode has been  mapped to a newNode
void 
GvCopyContext::put(GvNode * oldNode, GvNode *newNode)
{
   nodeMap.enter((u_long) oldNode, (void *) newNode);
}

// check if oldnode has already been mapped to a new
// NULL if not already mapped
GvNode * 
GvCopyContext::get(GvNode *oldNode)
{

    void	*node;
    if (nodeMap.find((u_long) oldNode, node))
	    return (GvNode *) node;
    return NULL;
}

// is node already marked
GvBool GvCopyContext::alreadySeen(GvNode * oldNode)
{
    void	*node;
    if (nodeMap.find((u_long) oldNode, node))
	    return gtrue;
    return gfalse;

}
	
// mark node seen
void GvCopyContext::setSeen(GvNode * node) 
{
   nodeMap.enter((u_long) node, (void *) node);
}


