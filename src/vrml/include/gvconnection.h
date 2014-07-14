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
#ifndef  _GV_GvConnection_
#define  _GV_GvConnection_
/******************************************************************************
@doc

@module GvConnection.h - VRML Node low-level ROUTE handling|

Copyright (c) 1996 - 1999 by Holger Grahn
All rights reserved

Purpose:
Connections are low-level linked list items stored
at field or node level in order to implement :

	ROUTE event Forwarding 
	script eventIn function calling
	Parent - child relations ship
	PROTO IS information storage


Classes:
<c GvConnection>


Notes:


Changes:

$Revision: 1.7 $
$Log: gvconnection.h,v $
Revision 1.7  1999/07/06 16:34:35  tom
*** empty log message ***

Revision 1.6  1999/05/10 13:53:43  tom
*** empty log message ***



Todo :

******************************************************************************/



#include <gvnode.h>

class GvProto;
class GvProtoInstance;


class GvConnection  : public RTRoot {
    
    RT(GvConnection);
public:
    
    GvConnection *next; // linked list of connections 
    
	GvConnection() : next(NULL) {} 
    virtual ~GvConnection() {} 


	// for connections using fields / event forwarding
    int virtual OnTrigger(GvField *from);

	// connections using nodes
    int virtual OnTrigger(GvNode *from);

/*  for performance / debug tuning
	// counter on triggered connections 
	static int connectionsTriggeredCnt;

	// get counter off triggered connections 
	static int getTriggeredConnectionsCnt() { return connectionsTriggeredCnt; }

	// set counter off triggered connections to zero
	static void clearTriggeredConnectionsCnt() { connectionsTriggeredCnt = 0 ;}
*/

};


#if 1

/*
    GvRouteToConnection
	helper class to store all ROUTE related information
*/

class GvRouteToConnection : public GvConnection {

    RT(GvRouteToConnection);

public:
  // source node for route
	GvNode		*from;
    int			fromEventIndex;
    
	// destination node for route
    GvNode		*to;
    int			toEventIndex;

    
    GvRouteToConnection(GvNode *from_=NULL, int fromIndex_=-1, 
						GvNode *to_=NULL,int toIndex_=-1)
		: from(from_),fromEventIndex(fromIndex_),  
		  to(to_),toEventIndex(toIndex_) 
	{};

    
    // int OnTrigger(GvField *from); // do nothing here

	// special case here :
	// if destroyed, inform "to" that incomming route has been deleted
	// inorder to prevent ROUTE triggering to deleted target nodes
	~GvRouteToConnection();


};

/*

  ROUTE target node stores a GvRouteFromConnection
  at node level for each incomming ROUTE

  so destructor can safely call deleteRoute all in routes

*/
class GvRouteFromConnection : public GvConnection {

    RT(GvRouteFromConnection);

	GvRouteToConnection *routeToConnection;
	// points to corresponding ROUTE Connection 

	GvRouteFromConnection (GvRouteToConnection *from=NULL)
		: routeToConnection(from)
	{ }



};

/*
    GvSetFieldConnection
	trigger does a set value on to field 
*/

class GvSetFieldConnection : public GvRouteToConnection {

    RT(GvSetFieldConnection);

public:

    GvField *toField; // ROUTE target field
    
    GvSetFieldConnection(GvNode *from=NULL, int fromIndex=-1, 
						 GvNode *to=NULL, int toIndex=-1,
						 GvField *toField_=NULL):
						GvRouteToConnection(from,fromIndex,to,toIndex),
						toField(toField_) {};


	// simple constructor, for PROTO IS tzpe connections 
    GvSetFieldConnection(GvField *to_):toField(to_) {};
						
    int OnTrigger(GvField *from);

};

// node callback signature, for eventIn methods
//
typedef void (GvNode::*GvNodeEventInMethod)(GvField *val);


/*
    OnTrigger calls a node method with field as event argument
*/


class GvNodeMethodConnection : public GvRouteToConnection {

    RT(GvNodeMethodConnection);

public:

    void (GvNode::*method)(GvField *event);		// the eventIn method
    
    GvNodeMethodConnection(GvNode *from=NULL, int fromIndex=-1, 
						 GvNode *to=NULL, int toIndex=-1,
						 GvNodeEventInMethod method_ = NULL):
						
						GvRouteToConnection(from,fromIndex,to,toIndex),
						method(method_) { };

	// allow simple constructor for OnTrigger calls 
    GvNodeMethodConnection(GvNode *to_,GvNodeEventInMethod method_): method(method_) { to = to_;};
  
    int OnTrigger(GvField *from);

};


#else
/*
    GvSetFieldConnection
	trigger does a set value on to field 
*/

class GvSetFieldConnection : public GvConnection {

    RT(GvSetFieldConnection);

public:

    GvField *to; // ROUTE target field
    
    GvSetFieldConnection(GvField *to_=NULL):to(to_) {};

    
    int OnTrigger(GvField *from);



};

// node callback signature, for eventIn methods
//
typedef void (GvNode::*GvNodeEventInMethod)(GvField *val);

/*
    trigger call a node method with field
*/


class GvNodeMethodConnection : public GvConnection {

    RT(GvNodeMethodConnection);

public:

    GvNode *to;		// ROUTE target node 

    void (GvNode::*method)(GvField *event);		// the eventIn method
    
    GvNodeMethodConnection(GvNode *to_=NULL,GvNodeEventInMethod method_ = NULL):to(to_),method(method_) { };

  
    int OnTrigger(GvField *from);

};

#endif


#if 0
/*
    trigger does a set value on to field 

  not needed
*/

class GvSendNodeEventConnection : public GvConnection {
public:

    GvNode *to;
    const char *event;


    GvSendNodeEventConnection(GvNode *to_=NULL,const char *event_=NULL): to(to_), event(event_) {};

    int OnTrigger(GvField *value);

};
#endif

/*
  Pseudo connection storing IS node.field relations for Proto storage 

*/
class GvIsFieldConnection : public GvConnection 
{
    RT(GvIsFieldConnection);


public:
    GvNode *to;
    GvName event;
    int OnTrigger(GvField *value);

    // add connection from to->field to toField 
    int addFromConnection(GvField *toField);

    // read target connection 
    GvBool read(GvInput *in, const GvName &name);

};


/*
  Pseudo connection storing IS node.field relations for Proto storage 

*/
class GvProtoIsConnection : public GvConnection 
{
    RT(GvProtoIsConnection);

public:
    GvNode* proto;		//Pointer to node // GvNodeHandle would keep REF !!!

    //GvName event;   // not really need
    int index;      // index into field/table of proto
    
    GvProtoIsConnection(GvNode *Proto=NULL,int toIndex=-1) : proto(Proto),index(toIndex) {}

    int OnTrigger(GvField *value);

    // get the actual value 
    GvField *eval(GvProtoInstance *instance);

    // add connection from to->field to toField 
    //int addFromConnection(GvField *toField);

	//~GvProtoIsConnection();

};


/*
  Pseudo connection storing IS node.event relations for Proto storage 
  stored in connection at NODE level
  will be instanced during ProtoInstance::Instance

*/
class GvProtoEventIsConnection : public GvConnection 
{
    RT(GvProtoEventIsConnection);

public:
    GvNode *proto; // GvNodeHandle would keep REF !!!
  
	int protoIndex;     // index into field/table of proto
    int nodeIndex;      // index into field of node 
	
	GvBool eventIn;		// this will be an eventIn to node (event nodeIndex) , otherwise eventOut
	GvBool eventOut;	// this will be an eventOut from node (event nodeIndex) , otherwise eventOut
	GvBool copyEventOutValue;
    
    GvProtoEventIsConnection(GvNode *Proto=NULL,int toIndex=-1,int fromIndex = -1,
		GvBool isEventIn=TRUE,GvBool isEventOut=FALSE,GvBool copyEventOut = FALSE) : 
	proto(Proto),protoIndex(toIndex),nodeIndex(fromIndex),eventIn(isEventIn),eventOut(isEventOut),copyEventOutValue(copyEventOut) {}

    int OnTrigger(GvField *value);

	// add the route 
	GvBool instanceRoute(GvProtoInstance *protoInstance,GvNode *me); 

	//~GvProtoEventIsConnection();
};

/*
  Pseudo connection storing IS  node.field relations for Proto storage 
  stored in connection at NODE level
  will be instanced during ProtoInstance::Instance
  Normal field IS connections are stored in field level,
  need for Script because fields are not yet instanced when IS in Script
  interface encountered.


*/
class GvProtoFieldIsConnection : public GvConnection 
{
    RT(GvProtoFieldIsConnection);

public:
    GvNode *proto; // GvNodeHandle would keep REF !!!
  
	int protoIndex;     // index into field/table of proto
    int nodeIndex;      // index into field of node 
	
	GvBool eventIn;		// this will be an eventIn to node (event nodeIndex) , otherwise eventOut
    
    GvProtoFieldIsConnection(GvNode *Proto=NULL,int toIndex=-1,int fromIndex = -1,GvBool isEventIn = TRUE) : proto(Proto),protoIndex(toIndex),nodeIndex(fromIndex),eventIn(isEventIn) {}

    int OnTrigger(GvField *value);

	// copy the value 
	GvBool copyValue(GvProtoInstance *protoInstance,GvNode *me); 

	//~GvProtoFieldIsConnection();
};


#if 0 
// Connections to a node 

// replaced by owners list in Node 

class GvOwnerConnection : public GvConnection {

    RT(GvOwnerConnection);

public:

    GvNode *to;
    
    GvOwnerConnection(GvNode *to_=NULL):to(to_) {};
    
    int OnTrigger(GvNode *from);


};

#endif


#endif /* _GV_GvConnection_ */
