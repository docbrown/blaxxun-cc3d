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

/******************************************************************************
@doc

@module GvField.cpp - VRML 1.0/2.0 abstract field base class|

Copyright (c) 1996-1997	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvField>
<c GvConnection> - low level event forwarding 

Notes:
	based on QvLib

Changes:
$Revision: 1.24 $
$Log: gvfield.cpp,v $
Revision 1.24  1999/07/06 16:45:42  tom
*** empty log message ***

Revision 1.23  1999/05/10 13:54:51  tom
*** empty log message ***


Todo :

******************************************************************************/

#include "stdafx.h"

#include "rtabst.h"

#include <gvinput.h>
#include <gvreaderror.h>
#include <gvfields.h>
#include <gvconnection.h>

#include <gvproto.h>

#include <gvfielddata.h>

// for CLOSE_BRACE
#include <gvkeyword.h>

// for the time 
#include <gvsensors.h>



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Special characters in files
#define OPEN_BRACE_CHAR		'['
#define CLOSE_BRACE_CHAR	']'
#define OPEN_CLOSE_BRACE "[]"
#define VALUE_SEPARATOR_CHAR	','
#define IGNORE_CHAR		'~'
#define IS_KEYWORD "IS"


// counter on triggered connections 
// int  GvConnection::connectionsTriggeredCnt;


#define EQUAL_CHAR '='
//
// Connections 
//

RTIMP(GvConnection,0,RTRoot);

int  GvConnection::OnTrigger(GvField *from)
{
    TRACE("GvConnection::OnTrigger field called, not implemented \n");
    return(0);
}

int  GvConnection::OnTrigger(GvNode *from)
{
    //TRACE("GvConnection::OnTrigger Node called, not implemented \n");
    return(0);
}

// predicate to filter connections for delete Route 
static 
GvBool PredRouteFrom(GvConnection *c, void *data){
	if (RTISA(c,GvRouteFromConnection)) {
		return  ((GvRouteFromConnection *) c)->routeToConnection == (GvRouteToConnection *) data;
	}
	return FALSE;
}

RTIMP(GvRouteToConnection,0,GvConnection);
GvRouteToConnection::~GvRouteToConnection()
{
	if (to) {
		//TRACE("GvRouteToConnection::~ Removing outgoing route %p to node %p\n",this,to);
		ASSERT_POINTER(to,GvNode);
		GvNode *tmpTo= to;
		to = NULL; 
		if (!tmpTo->removeConnection(PredRouteFrom,this)) {
			//TRACE("~GvRouteToConnection::~ corresponding routeFrom not found %p\n",this);
		}	
	}	
	//TRACE("GvRouteToConnection::~ %p\n",this);		
}


RTIMP(GvRouteFromConnection,0,GvConnection);

RTIMP(GvSetFieldConnection,0,GvRouteToConnection);

int GvSetFieldConnection::OnTrigger(GvField *fromField) 
{ 
	//connectionsTriggeredCnt++;
	ASSERT_POINTER(fromField,GvField);	
	ASSERT_POINTER(toField,GvField);	
	ASSERT(fromField->getFieldType() == toField->getFieldType());

	
//	TRACE("GvSetFieldConnection::OnTrigger %s %d \n",from->ClassName(),connectionsTriggeredCnt);

    return( toField->set(fromField));
}

/*
GvSetFieldConnection::~GvSetFieldConnection()
{
	to = NULL;
}

*/


RTIMP(GvNodeMethodConnection,0,GvRouteToConnection);

int GvNodeMethodConnection::OnTrigger(GvField *from) 
{ 
   // GvNodeEventInMethod f = (GvNodeEventInMethod) method;

   //see vc Help:"Expressions with Pointer-to-Member Operators"

	// connectionsTriggeredCnt++;

	//TRACE("GvNodeMethodConnection::OnTrigger %s %d \n",from->ClassName(),connectionsTriggeredCnt);
    
    (to->*method)(from);

    return(1);
}

#if 0

int GvSendNodeEventConnection::OnTrigger(GvField *value) 
{ 
	connectionsTriggeredCnt++;

    return( to->OnEventIn(event,value));
}
#endif


RTIMP(GvIsFieldConnection,0,GvConnection);

int GvIsFieldConnection::OnTrigger(GvField *value)
{
    TRACE("GvIsFieldConnection::OnTrigger called %s\n",value->ClassName());
    return(0);
}

// add connection from to->field to toField 
int GvIsFieldConnection::addFromConnection(GvField *toField)
{
    int ret= -1;
    // lookup eventOut


    GvField *outEvent = to->getEventOut(event);
    
    // + check type 
    //
    if (outEvent) {
        // add a SetField connection to out 
        GvSetFieldConnection *c = new GvSetFieldConnection(toField);
        ret=outEvent->addConnection(c,1);

    } else {
        TRACE("GvIsFieldConnection::addFromConnection outEvent not found \n");
    }
    return(ret);

}


// read target connection 
// NodeName . EventOutName
GvBool GvIsFieldConnection::read(GvInput *in, const GvName &name)
{
    to = GvNode::readReference(in);
    
    if (!to) return(FALSE);
    
    char c;

    // or IS keyword 
    if (in->read(c) && c == '.') {
    } else {
	     in->putBack(c);
          GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Couldn't read = field target connection for field \"%s\"",name.getString());
	      return FALSE;
    }

    
    if (in->read(this->event, TRUE)) {

    
    }
    else {
	      GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Couldn't read = field target connection for field \"%s\"",name.getString());
	      return FALSE;
    }
    return (TRUE);

}

RTIMP(GvProtoIsConnection,0,GvConnection);

int GvProtoIsConnection::OnTrigger(GvField *value)
{
    TRACE("GvProtoIsConnection::OnTrigger called %s\n",value->ClassName());
    return(0);
}

// get the actual value 
GvField *
GvProtoIsConnection::eval(GvProtoInstance *instance)
{  
    int newIndex=instance->MapProtoField(index);    
	if (newIndex <0) {
		// the requested field is not present in proto instance (because created by EXTERNPROTO) 
		// use the default value, if present in Proto field definition 
 		GvFieldData *protoFieldData = proto->getFieldData(); 
		GvFieldEntry *protoEntry =protoFieldData->getFieldEntry(index); 
		if (protoEntry->getDefault()) {
			return (protoEntry->getDefault());
		}
		return(NULL);  
	}

	return(instance->getField(newIndex));
}

//GvProtoIsConnection::~GvProtoIsConnection()
//{
//}


RTIMP(GvProtoEventIsConnection,0,GvConnection);

int GvProtoEventIsConnection::OnTrigger(GvField *value)
{
    TRACE("GvProtoIsConnection::OnTrigger called %s\n",value->ClassName());
    return(0);
}



GvBool GvProtoEventIsConnection::instanceRoute(GvProtoInstance *protoInstance, GvNode *me)
{
	int newIndex = protoInstance->MapProtoField(protoIndex);

	if (newIndex <0)  // field/event not present in EXTERNPROTO instance, ingore ROUTE
		return(FALSE);
   // should we keep routes ? 
   GvBool ret=FALSE;
   if (eventIn)
	   ret= GvNode::addRoute(protoInstance,newIndex,me,nodeIndex);

   if (eventOut) {
	   ret= GvNode::addRoute(me,nodeIndex, protoInstance,newIndex);
	   //copy eventOutout 
	   if (copyEventOutValue) {
			GvField *f = protoInstance->getField(newIndex);
			if (f) {
				int old = f->enableConnections(0); // disable ROUTES 
				f->set(me->getField(nodeIndex));
				f->enableConnections(old);
			}
	   }
   }	
   return ret;


}

//GvProtoEventIsConnection::~GvProtoEventIsConnection()
//{
//}

RTIMP(GvProtoFieldIsConnection,0,GvConnection);

int GvProtoFieldIsConnection::OnTrigger(GvField *value)
{
    TRACE("GvProtoFieldIsConnection::OnTrigger called %s\n",value->ClassName());
    return(0);
}


// copy the value 
GvBool GvProtoFieldIsConnection::copyValue(GvProtoInstance *protoInstance,GvNode *me)
{
	GvField *f = me->getField(nodeIndex);

	int old = f->enableConnections(0); // disable ROUTES 

	GvBool ret=TRUE;

	int newIndex = protoInstance->MapProtoField(protoIndex);

	if (newIndex <0) {
		// the requested field is not present in proto instance (because created by EXTERNPROTO) 
		// use the default value, if present in Proto field definition 
 		GvFieldData *protoFieldData = proto->getFieldData(); 
		GvFieldEntry *protoEntry =protoFieldData->getFieldEntry(protoIndex); 
		if (protoEntry->getDefault()) {
			f->set(protoEntry->getDefault());
		}
		else ret = FALSE;
	} else {
		f->set(protoInstance->getField(newIndex));
	}

	f->enableConnections(old);
	
	return(ret);
}


//GvProtoFieldIsConnection::~GvProtoFieldIsConnection()
//{
//}

//
// GvField
//

RTIMP_ABSTRACT(GvField,0,RTRoot);


#ifdef _COM
#include "gtrace.h"


// propagate to node containing field
ULONG GvField::InternalAddRef()
{
	ASSERT(container != (GvNode*) NULL);
	if (container != (GvNode*) NULL)
		return container->AddRef(); 
	return 0;
}

ULONG  GvField::InternalRelease() {
	ASSERT(container != (GvNode*) NULL);
	if (container != (GvNode*) NULL)
		return container->Release();
	return 0;
}


STDMETHODIMP_(ULONG) GvField::AddRef() 
{
	return InternalAddRef();
}	


STDMETHODIMP_(ULONG) GvField::Release()
{ 
	 return InternalRelease();
}

HRESULT WINAPI GvField::InternalQueryInterface(void* pThis,
	const _ATL_INTMAP_ENTRY* entries, REFIID iid, void** ppvObject)
{
/* 
	// get vtbl from tear off
	if (InlineIsEqualGUID(*(pEntries->piid), iid)) {
        CONST_VTBL struct EventInVtbl __RPC_FAR *lpVtbl;

		IUnknown* pUnk = (IUnknown*)((int)pThis+pEntries->dw);
		pUnk->AddRef();
		*ppvObject = pUnk;
		return S_OK;
	}
*/

	return CComObjectRoot::InternalQueryInterface(pThis,entries,iid,ppvObject);
}

HRESULT WINAPI GvField::_Chain(void* pv, REFIID iid, void** ppvObject, DWORD dw)
{
	return CComObjectRoot::_Chain(pv,iid,ppvObject,dw);
}



/* EventIn methods */
STDMETHODIMP GvField::getType(int *type)
{
	*type = getFieldType();
	return S_OK;
}


STDMETHODIMP GvField::setValueFromString( /* [in] */ BSTR value)
{
	const char *vs;
	USES_CONVERSION;
	vs = OLE2CT(value);
	int ret=set(vs);
	COM_EXTERN_TOUCH();
	return ((ret>0) ?  S_OK : S_FALSE);
}




/*
    trigger does a callback on observer 
*/
#define DOREF

class GvComEventOutObserverConnection : public GvConnection {

    RT(GvComEventOutObserverConnection);


protected:

	EventOutObserver *observer;	   // the observer object 
	IDispatch* userData;		   // data passed to observer 			
    
	double *timeStampPtr;			// pointer to browsers time stamp ptr

public:

    GvComEventOutObserverConnection(EventOutObserver *Observer=NULL,IDispatch* UserData=NULL,double *TheTimestampPtr=NULL)
	{	
		observer = Observer;
		userData = UserData;
		timeStampPtr = TheTimestampPtr;

#ifdef DOREF
		if (observer) observer->AddRef();
		if (userData) userData->AddRef();
#endif
	};

    ~GvComEventOutObserverConnection()
	{	
#ifdef DOREF
		if (observer) observer->Release();
		if (userData) userData->Release();
#endif
	};

    
    int OnTrigger(GvField *from);

	friend GvBool PredToObserver(GvConnection *c, void *data);


};

RTIMP(GvComEventOutObserverConnection,0, GvConnection);


int GvComEventOutObserverConnection::OnTrigger(GvField *from)
{
	int ret=0;
	// tested :  refCnt is only 1 even if object is present in java
	//


	EAI_TRACE("calling EventOut %s (",from->ClassName());

	// call the callback 
	EventOut *result=NULL;

	from->QueryInterface(IID_EventOut, (void **)&result);

	if (result)  {
		TRACE("calling Eventout %s (",from->ClassName());

		// observer->callback(result,GvNode::getTime(),userData);
		observer->callback(result,*timeStampPtr,userData);

		ret = 1;
		result->Release();
		TRACE(")\n");
	}

	EAI_TRACE(")\n");

	return(ret);
}


/* EventOut methods */ 
STDMETHODIMP GvField::advise(EventOutObserver *observer, IDispatch* userData) 
{
	EAI_TRACE("GvField::advise %s\n",this->ClassName());

	if (observer == NULL) return S_FALSE;

	ASSERT(observer != NULL);
	
	static double invalidTime=-1.0;

	double * timeStampPtr = &invalidTime;

	if (getContainer()) {
		timeStampPtr = getContainer()->getTimePtr();
	}


    // create connection from field to eventOutObserver 
	GvComEventOutObserverConnection *connection = new GvComEventOutObserverConnection(observer,userData,timeStampPtr);
	
	if (!connection) return S_FALSE;

	this->addConnection(connection);

	return S_OK; 
}

// TRUE if Connection is EventOutObserver
GvBool PredToObserver(GvConnection *c, void *data){
	if (RTISA(c,GvComEventOutObserverConnection)) {
		GvComEventOutObserverConnection *a = (GvComEventOutObserverConnection *) c;
		return  (a->observer == (EventOutObserver*) data);
	}
	return FALSE;
}

STDMETHODIMP GvField::unAdvise(EventOutObserver *observer) 
{
	EAI_TRACE("GvField::unAdvise %s\n",this->ClassName());

	if (observer == NULL) return S_FALSE;

	ASSERT(observer != NULL);
    int ret = this->removeConnection(PredToObserver,observer);
	if (ret == 0)
		return S_FALSE; 
	else return S_OK; 
}

//long __stdcall GvSFFloat::callback(struct EventOut *,double,struct IDispatch *)' 

STDMETHODIMP GvField::toString(/* [retval][out] */ BSTR  *value)
{

	USES_CONVERSION;

	GvString v;
	
	int ret= get(v); // convert value to string 

	if (ret > 0) {
		const char *vs = v.getString();
		*value = SysAllocString(T2COLE(vs));
		return S_OK;
	}  else {
		*value = NULL;
		return S_FALSE;
	}
}


#endif


GvField::~GvField()
{
    deleteConnections();
}


const GvField& GvField::operator=(const GvField &f)
{
    TRACE("%s::operator = not implemented (%s) \n",ClassName(),f.ClassName());
	ASSERT(0);

    return(f);
}


int GvField::set(const GvField *value)
{
	if (this == value) { // hg 20.04.99
		// already equal pointers !
		// had problems with this = this because array destroyed before copy
        OnChanged();
	    return TRUE;
	}
	// TBD: to do check type, no part of proper scene graph construction

   
	*this = *value;  // virtual

   /* already called by = operator
        setDefault(FALSE);
        return OnChanged();
    */
    // copy default flag ?? 
    // if (value->isDefault())   setDefault(TRUE);

   return TRUE;
}

// create a acopy of this field, copy will have same class as this 
// copy with have the same default flag 
GvField* GvField::copy()
{
    // create new instance
    GvField *theCopy = (GvField*) this->Id().New();
    // copy value 
    theCopy->set(this);
    // copy default bit 
    if (isDefault())
          theCopy->setDefault(TRUE);
    return(theCopy);
}


// get VRML Type name for field as string 
const char * GvField::GetType()
{ const char *n;
  n = ClassName();

  if (strncmp(n,"Gv",2) == 0) n +=2;
  return(n);
}


void
GvField::setContainer(GvNode *cont)
{
    container = cont;
    setDefault(TRUE);
}

int GvField::OnChanged()
{  
   
   if (connectionsEnabled()) // new 26.11.99 HG problem on shutdonw GvScene::worldInfo = NULL
		if (container) // tell container, that this member field has beein changed
			(container->OnFieldChanged(this)); 
   return (touch());  // and now trigger outgoing routes 
} 



   
// delete all connections
int GvField::deleteConnections()
{
    GvConnection *c = connections;
	GvConnection *next;

	connections = NULL;


    while (c!= NULL) {

	
		ASSERT_POINTER(c,GvConnection);
        
		next =  c->next;

        delete c;

        c= next;
    }
	

    return(0);

}
    
// add a connection, and optionally trigger the value
GvBool GvField::addConnection(GvConnection *connection,GvBool trigger)
{
    ASSERT(connection!=NULL);

    connection->next = connections;
    connections= connection;
    if (trigger)
        connection->OnTrigger(this);
    return(TRUE);
}
   
// remove a specific connection
// if found connection is deleted, only first is searched 
GvBool GvField::removeConnection(GvConnection *connection)
{
    GvConnection ** prev = &connections;
    GvConnection *c = connections;
    
    while (c!= NULL) {


		ASSERT_POINTER(c,GvConnection);

        GvConnection *next =  c->next;
        if (c == connection) {
            *prev = next;
            delete c;
            return(TRUE);
        }
        prev = &c->next;
        c = next;
    }
    return(FALSE);

}

// remove a specific connection filtered by predicate function

GvBool GvField::removeConnection(GvBool (*Pred)(GvConnection *c, void *data),void *data)
{
    GvConnection ** prev = &connections;
    GvConnection *c = connections;
    while (c!= NULL) {
		
		ASSERT_POINTER(c,GvConnection);

        GvConnection *next =  c->next;
        if (Pred(c,data)) {
            *prev = next;
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
GvField::findConnection(GvBool (*Pred)(GvConnection *c, void *data),void *data)
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


// trigger all outgoing connections 
int GvField::OnTrigger() 
{
    GvConnection *c = connections;
	if (!c) return 0;
	
	if (container != (GvNode*) NULL)
		container->ref();

    int ret=0;
    while (c!=NULL) {
		  
		  ASSERT_POINTER(c,GvConnection);
		  
		  GvConnection *next = c->next;
		  
          ret=c->OnTrigger(this);
          c = next;
    }
	if (container != (GvNode*) NULL)
		 container->unrefNoDelete();
	

    return(ret);
}



GvBool
GvField::read(GvInput *in,  const GvName &name)
{
    char c;

    setDefault(FALSE);
	GvBool putBack = gtrue;

    if (in->read(c) && c == IGNORE_CHAR && !in->vrml2) {
	    setDefault(TRUE);
	    setIgnored(TRUE);
        return(TRUE); // for VRML 1.0 files is 20.01.97

    }
    if ( c ==  EQUAL_CHAR) { // HG direct Route extension:  field  node.eventOut
	    setIgnored(FALSE);
        GvIsFieldConnection *is = new GvIsFieldConnection();
        if (is->read(in,name)) {
            addConnection(is);
            is->addFromConnection(this);
            return(TRUE);

        } else {
            delete is;
	        GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Couldn't read = field connection for field \"%s\"",name.getString());
	        return FALSE;
	    }


    }
    else if (c == 'I')  {
		GvBool gotIs=gfalse;

		GvName keyword;
		
		in->putBack(c); // first char 

		if (in->read(keyword, TRUE) && keyword == GvKeyword::is) gotIs= gtrue;

        if (!gotIs) {
			in->putBack(keyword);
			putBack = gfalse;
		} else { 
			//in->read(c) && c == 'S')       // IS keyword
            GvName event;
            if (in->read(event, TRUE)) {
                // to do: event translation with IS
                GvProto *proto = in->currentProto;

                if (!proto) {
	                GvReadError::post(in,GV_RE_SYNTAX_ERROR, "%s IS %s only allowed in PROTO definition",(const char *)name.getString(),(const char *)event.getString());
	                //return FALSE;
	                return TRUE;
                }
            
                GvFieldData *protoFields = proto->getFieldData();
				// int srcFieldIndex= protoFields->getEventOutIndex(proto,event);
                int srcFieldIndex= protoFields->getFieldIndex(event);

                if (srcFieldIndex<0) {
	                GvReadError::post(in,GV_RE_SYNTAX_ERROR, "%s IS %s ,no such Event in PROTO interface",name.getString(),event.getString());
	                //return FALSE;
	                return TRUE;
                }
            
                GvFieldEntry *srcFieldEntry = protoFields->getFieldEntry(srcFieldIndex);
                //if (!srcFieldEntry->IsEventOut())) {
                //}
                if (!srcFieldEntry->matchFieldType(this)) {
	                GvReadError::post(in,GV_RE_TYPE_ERROR, "%s IS %s ,type mismatch ",name.getString(),event.getString());
	                //return FALSE;
	                return TRUE;
                }

				GvBool doIs = FALSE;
				GvBool doRouteIs = FALSE;
				switch (srcFieldEntry->type) {
				case GV_FIELD:
						doIs = TRUE;
						break;
				case GV_EXPOSED_FIELD:
						doIs = TRUE;
						doRouteIs=TRUE;
						break;
				case GV_FIELD_EVENT_IN:
				case GV_FIELD_EVENT_OUT:
				case GV_NODE_EVENT_IN:
						doRouteIs=TRUE;
						break;
				default :
					 break;
				}	


				if (doIs) {
					GvProtoIsConnection *is = new GvProtoIsConnection(proto,srcFieldIndex);
					// this node need to be copied on instancing, set copyOnInstance flag 
					this->addConnection(is);
					GvNode *container=getContainer();
					if (container) container->copyOnInstance = TRUE;
				}
				if (doRouteIs) {
					proto->DoIs(in,this->container,this,name,event);
				}

                //is->addFromConnection(this);
            }
            else {
	            GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Couldn't read IS source field/event field \"%s\"",name.getString());
	            return FALSE;
            }
            return(TRUE);
        
		} // IS
		
		//in->putBack(c); // second char 

		// c = 'I'; // need to putback	   // be carefull 2 * ungetc workaround in putback

	} // I

    
    {
	    setIgnored(FALSE);

	    if (putBack)
			in->putBack(c);

	    if (! readValue(in)) {
	        GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Couldn't read value for field \"%s\"",
			      name.getString());
	        return FALSE;
	    }

	    if (in->read(c)) {
	        if (c == IGNORE_CHAR)
		        setIgnored(TRUE);
	        else if (c == VALUE_SEPARATOR_CHAR)
            {
                // VRML 2: comma is white space
            }
            else
		        in->putBack(c);
        }
	}

    return TRUE;
}

int
GvField::io(AStream &a, const GvName &name)
{
    char c;

	if (a.IsInput()) {
    	setDefault(FALSE);
		c = a.Skip();
    	if (c == IGNORE_CHAR) {
			c = a.getc();
			setDefault(TRUE);
			setIgnored(TRUE);
    	} else {
	
		setIgnored(FALSE);


		if (!ioValue(a)) {
	    	a.Error("Couldn't read value for field \"%s\"", name.getString());
	    	return FALSE;
		}

		c = a.Skip();
	    if (c == IGNORE_CHAR) {
			c = a.getc();
			setIgnored(TRUE);
		}
      }
	}
	else { // write
	  if (isDefault()) {
		 a.putc(IGNORE_CHAR);
	  }	else {
	  		ioValue(a);
			if (isIgnored()) 
		 	   a.putc(IGNORE_CHAR);

	  }
	}
    return TRUE;
}

int
GvField::ioValue(AStream &a)
{
  TRACE("Error:%s::ioValue(AStream &a) not implemented.\n",ClassName());
  return(0);	
}

int GvField::get(GvString &value)
{
  TRACE("Error:%s::get(GvString &value) not implemented.\n",ClassName());
  return(0);	

}

int GvField::set(const char* value)
{
  TRACE("Error:%s::set(const char * %s) not implemented.\n",ClassName(),value);
  return(0);	
}



// could be optimized



GvField *
GvField::createInstanceFromName(const GvName &className)
{
#define TRY_CLASS(name, class)						      \
    else if (className == name)						      \
	return new class

    GvField *inst = NULL;
	const char *n=className.getString();

	if (*n=='S') {
	if (n[2] <= 'L') {	
    if (0) ;			// So "else" works in first TRY_CLASS
    TRY_CLASS("SFBitMask",	GvSFBitMask); // not VMRL2
    TRY_CLASS("SFBool",		GvSFBool);
    TRY_CLASS("SFColor",	GvSFColor);
    TRY_CLASS("SFEnum",		GvSFEnum);	  // not VMRL2
    TRY_CLASS("SFFloat",	GvSFFloat);
    TRY_CLASS("SFImage",	GvSFImage);
    TRY_CLASS("SFInt32",	GvSFInt32);
    TRY_CLASS("SFLong",		GvSFLong);	  // not VMRL2	
	} else {
    if (0) ;			// So "else" works in first TRY_CLASS
    TRY_CLASS("SFMatrix",	GvSFMatrix);	// not VMRL2
    TRY_CLASS("SFRotation",	GvSFRotation);
    TRY_CLASS("SFString",	GvSFString);
    TRY_CLASS("SFVec2f",	GvSFVec2f);
    TRY_CLASS("SFVec3f",	GvSFVec3f);
    TRY_CLASS("SFNode",		GvSFNode);
    TRY_CLASS("SFTime",	    GvSFTime);
	}
	}
	else {	
    if (0) ;			// So "else" works in first TRY_CLASS
    TRY_CLASS("MFColor",	GvMFColor);
    TRY_CLASS("MFFloat",	GvMFFloat);
    TRY_CLASS("MFInt32",	GvMFInt32);
    TRY_CLASS("MFLong",		GvMFLong);
    TRY_CLASS("MFNode",		GvMFNode);
    TRY_CLASS("MFRotation",	GvMFRotation);
    TRY_CLASS("MFString",	GvMFString);
    TRY_CLASS("MFTime",		GvMFTime);
    TRY_CLASS("MFVec2f",	GvMFVec2f);
    TRY_CLASS("MFVec3f",	GvMFVec3f);
    }
    return inst;

#undef TRY_CLASS
}


//
// GvSField
//
RTIMP_ABSTRACT(GvSField,0,GvField);


GvSField::GvSField()
{
}


GvSField::~GvSField()
{
}


//
// GvMField
//
RTIMP_ABSTRACT(GvMField,0,GvField);

GvMField::GvMField()
{
    maxNum = num = 0;
}

GvMField::~GvMField()
{
}

void
GvMField::makeRoom(int newNum)
{
    if (newNum != num)
	    allocValues(newNum);
}

GvBool
GvMField::readValue(GvInput *in)
{
    char c;
    int	curIndex = 0;

    if (in->read(c) && c == OPEN_BRACE_CHAR) { // we got the [ 

	if (in->read(c) && c == CLOSE_BRACE_CHAR)
	    ;				
	else {
	    in->putBack(c);

	    while (TRUE) {

		if (curIndex >= num)
		    makeRoom(curIndex + 1);

		if (! read1Value(in, curIndex++) || ! in->read(c)) {
			GvNode *container = getContainer();
			if (container) {
				GvName fieldName = container->getFieldName(container->getFieldIndex(this));
				GvReadError::post(in, GV_RE_SYNTAX_ERROR, "%s::Couldn't read value %d of %s field %s", container->GetType(),
                                    curIndex,this->GetType(),fieldName.getString());
			 }
			 else {
				GvReadError::post(in, GV_RE_SYNTAX_ERROR, "%s::Couldn't read value %d of field %s", "",
                                    curIndex,"");
			 }
             return FALSE;
		}

		if (c == VALUE_SEPARATOR_CHAR) {
		    if (in->read(c)) {
			if (c == CLOSE_BRACE_CHAR)
			    break;
			else
			    in->putBack(c);
		    }
		}
		else if (c == CLOSE_BRACE_CHAR)
		    break;

		else {
            if (in->isVrml2() && (c != CLOSE_BRACE)) { // VRML2: no comma required (whitespace)
			    in->putBack(c);
            }
            else {
       			GvReadError::post(in,GV_RE_SYNTAX_ERROR,
				      "Expected '%c' or '%c' but got "
				      "'%c' while reading value %d",
				      VALUE_SEPARATOR_CHAR,
				      CLOSE_BRACE_CHAR, c,
				      curIndex);

			in->putBack(c);
		    return FALSE;
            }
		}
		} // while 
	} //  [ xxxx ]


	// single value 

	if (curIndex < num)
	    makeRoom(curIndex);
    }

    else {
	    in->putBack(c);
	    makeRoom(1);
	    if (! read1Value(in, 0))
	        return FALSE;
        }

    return TRUE;
}

// read a list of value until endChar found
// endChar is not read !
GvBool
GvMField::readValueList(GvInput *in,int endChar)
{
    char c;
    int	curIndex = num; // means append

    GvBool needComma = FALSE;

    GvBool isMfNode = RTISA(this,GvMFNode);


    // check for list end 
	if (in->read(c)) {
        if ( c == endChar) {
	        in->putBack(c);
            return TRUE;
        }
    } else {
        if (endChar == EOF && in->eof()) 
            return TRUE;
		else return FALSE;
    }

    in->putBack(c);
    

    // read all values
    {
	    while (TRUE) {

		if (curIndex >= num)
		    makeRoom(curIndex + 1);

		if (! read1Value(in, curIndex++)) {
             num--; // ignore last value 

			// get next char
		    if (!in->read(c)) {
				if (endChar == EOF) break;
			}
			else if (c == endChar) {
  				in->putBack(c);
				break;
			}
			else if ((c == CLOSE_BRACE) || (c == CLOSE_BRACE_CHAR)) { // 22.09.96. Syntax error ? 
  				in->putBack(c);
				break;
			}


            GvReadError::post(in, GV_RE_SYNTAX_ERROR, "%s::Couldn't read value %d of field", getContainer() ? getContainer()->GetType():"",
                                    curIndex);
            return FALSE;
		}

        // get next char
        if (!in->read(c)) {
            if (endChar == EOF) break;
            else {
               GvReadError::post(in,GV_RE_SYNTAX_ERROR, "%s::Couldn't read value %d of field", getContainer() ? getContainer()->GetType():"",curIndex);
            }    
             return FALSE;
		}

		if (c == VALUE_SEPARATOR_CHAR) {
		    if (in->read(c)) {
			    if (c == endChar) {
			        in->putBack(c);
            	    break;
                }
			    else
			        in->putBack(c);
		    }
		}

		else if (c == endChar) {
  	        in->putBack(c);
		    break;
        }
		else if ((c == CLOSE_BRACE) || (c == CLOSE_BRACE_CHAR)) { // 22.09.96. Syntax error ? 
  	        in->putBack(c);
		    break;
		}
		else {
            if (!needComma || in->isVrml2()) { // VRML2: no comma required (whitespace)
			    in->putBack(c);
            }
            else {
       	    GvReadError::post(in,GV_RE_SYNTAX_ERROR,
				      "Expected '%c' or '%c' but got "
				      "'%c' while reading value %d",
				      VALUE_SEPARATOR_CHAR,
				      endChar, c,
				      curIndex);
		    return FALSE;
            }

            if (isMfNode)
				if (!GvNode::readRoutesOrProtos(in)) 
					return(FALSE);
		}
	  } // while
   }


    return TRUE;
}


int 
GvMField::ioValue(AStream &a)
{
    char c;
    int	curIndex = 0;

	if (a.IsInput()) {
    	c = a.Skip();
    	if (c == OPEN_BRACE_CHAR) {
			  c = a.getc();
			  c = a.Skip();

			  if (CLOSE_BRACE_CHAR) {
			  	 c = a.getc();
	    	   }						
			   else {

	    		while (TRUE) {

				if (curIndex >= num)
		    		makeRoom(curIndex + 1);

				if (!io1Value(a, curIndex++) ) {
		    		a.Error( "Couldn't read value %d of field",  curIndex);
		    	    return FALSE;
				}
				c = a.Skip();

				if (c == VALUE_SEPARATOR_CHAR) {
 				    c = a.getc();
		    		c = a.Skip();
					if (c == CLOSE_BRACE_CHAR) {
 				       c = a.getc();
			    	   break;
					}
				}

				else if (c == CLOSE_BRACE_CHAR) {
 				    c = a.getc();
		    		break;
				}
				else {
		    		a.Error("Expected '%c' or '%c' but got "
				      "'%c' while reading value %d",
				      VALUE_SEPARATOR_CHAR,
				      CLOSE_BRACE_CHAR, c,
				      curIndex);
		    		return FALSE;
				}
	    		} // While 
			}

	if (curIndex < num)
	    makeRoom(curIndex);
    
    }  // no open brace
    else {
		makeRoom(1);
		if (! io1Value(a, 0))
	    return FALSE;
    }
	}	// input 
	else { // output 

		if (Length() == 0 ) {
		  a.Sep(OPEN_BRACE_CHAR);
		  a.Sep(CLOSE_BRACE_CHAR);

		} else 
		if (Length() == 1 ) {
		   io1Value(a,0);
		} else {
		a.Sep(OPEN_BRACE_CHAR); a.TabIn();
		while (curIndex < Length()) {
		 	if (curIndex>0) a.Sep(VALUE_SEPARATOR_CHAR);
		 	io1Value(a,curIndex);
			if (curIndex % 4 == 0) a.nl();
			curIndex ++;
		}
		a.TabOut();
		a.Sep(CLOSE_BRACE_CHAR);
		}

	}

    return TRUE;
}

int GvMField::get(GvString &s)
{

	if (Length() == 0 ) {
	  s = OPEN_CLOSE_BRACE; // was uncommented, why ?
	} else 
	if (Length() == 1 ) {
	   get1(s,0);
	} else {
			int curIndex=0;
			s = OPEN_BRACE_CHAR;
			GvString v;

			while (curIndex < Length()) {
				get1(v,curIndex);
		 		if (curIndex>0) v = GvString(", ") + v;
				s+=v;
				curIndex ++;

			}
		    s+= CLOSE_BRACE_CHAR;
		}

	return(1);	

}

int GvMField::set(const char* s)
{
	//delete contents 
	makeRoom(0);

	// TRACE("Parsing an MField from :%s\n",s);

	if (strcmp(s,"[]") == 0) {
		setDefault(TRUE);
		OnChanged();
  		return(1);
	} 


	// create a parser 
	GvInput in;

    in.setString(s);
	in.version = 2.0;
	in.vrml2 = 1;
	
	// read 
	int ret = this->readValue(&in);

	setDefault(FALSE);
	OnChanged();

	return ret;
}


#if 0
// experiment 

{

    // create pseudo file from string 
    FILE str;
    FILE *infile = &str;

    memset(&str,sizeof(str),0);

    infile->_flag = _IOREAD|_IOSTRG|_IOMYBUF;
    infile->_ptr = infile->_base = (char *) s;
    infile->_cnt = strlen(s);

    GvInput in;
    in.setFilePointer(infile);
    in.readHeader = TRUE;
    
    makeRoom(0);
    if (readValue(&in)) return(1);


//     retval = (_input(infile,format,arglist));


    
    TRACE("Error:%s::set(const char * %s) not implemented.\n",ClassName(),s);


#if 0
int G__cdecl sscanf (
        REG2 const char *string,
        const char *format,
        ...
        )
/*
 * 'S'tring 'SCAN', 'F'ormatted
 */
{
        va_list arglist;
        FILE str;
        REG1 FILE *infile = &str;
        REG2 int retval;

        va_start(arglist, format);

        _ASSERTE(string != NULL);
        _ASSERTE(format != NULL);

        infile->_flag = _IOREAD|_IOSTRG|_IOMYBUF;
        infile->_ptr = infile->_base = (char *) string;
        infile->_cnt = strlen(string);

        retval = (_input(infile,format,arglist));

        return(retval);
}
#endif



  return(0);	
}
#endif


int GvMField::get1(GvString &s, int index)
{
  TRACE("Error:%s::get1(GvString &) not implemented.\n",ClassName());
  return(0);	
}

int GvMField::set1(const char* s, int index)
{
  TRACE("Error:%s::set1(const char * %s) not implemented.\n",ClassName(),s);
  return(0);	
}




int
GvMField::io1Value(AStream &a, int index)
{
  TRACE("Error:%s::io1Value(AStream &a,int) not implemented.\n",ClassName());
  return(0);	
}



// set value to interpolation from two same fields 
int GvField::interpolateFrom(GvField *a,GvField *b, float t)
{
  TRACE("Unimplemented interpolateFrom message ");
  return(-1);
}





