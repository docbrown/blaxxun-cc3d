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
#ifndef  _GvProto_
#define  _GvProto_

/******************************************************************************
@doc

@module GvProto.h - VRML 2.0 Prototype implementation|

Copyright (c) 1996 - 1999 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvProto>
<c GvProtoInstance>

Notes:

Changes:

$Revision: $
$Log: $


Todo :

******************************************************************************/


#include <gvnode.h>

#include <gvsubnode.h>

//#include <gvgroup.h>

#include <gvsfint32.h>

#include <gvsfnode.h>
#include <gvmfnode.h>
#include <gvmfstring.h>
#include <gvsfbool.h>

#include <gvnamespace.h>

#include "www.h"


class GvProtoRoot;

class GvProto;
class GvExternProto;
class GvProtoInstance;

class GvRoute;





/* 

  
PROTO prototypename [ eventIn      eventtypename name
                      eventOut     eventtypename name
                      exposedField fieldtypename name defaultValue
                      field        fieldtypename name defaultValue
                      ... ] {
  Scene graph
  (nodes, prototypes, and routes, containing IS statements)
}


Transform {
  set_translation IS set_position
}
processEvents
    eventTranslation
            type sourceName destNode destName



*/

class GvProto : public GvNode {

    GV_NODE_HEADER_UNKNOWN_NODE(GvProto);

  public:

    // private children field   
    GvMFNode children;

     // VRML class-name for this prototype node
    GvName className;

    // get VRML Type name for node 
	const char *GetType();


    void setClassName(const char *name);
	const char *getClassName() const;

    // interface
    GvFieldData  protoFieldData; // returned by getFielData

#ifdef _GFULL
	// local Node/PROTO dictionary
	// need to be stored only for Authoring Tool purposes
	// or to allow createVrmlFromString with references to local protos
    GvNameSpace nameSpace; 
#endif

    
    // all routes internal of this proto, must be instanced for each instance
    GvMFNode routes;

    // add route to route list
    GvBool addRoute(GvNode * From,int FromIndex,GvNode * To,int ToIndex); 

    // get pointer to field at index 
    // returns NULL, because proto has no fields
    // only a field description
	// fields are creating in a ProtoInstance node 

    GvField *getField(int index) const;
    
    // get the proto 
    virtual GvProto *getProto() { return (this); }
	
	// get pointer to implementing proto, ovverriden by EXTERNPROTO's
	virtual GvProto *getProto(GTraversal &state) { return (this); } 


	// create a new instance of proto, copy classname & fieldData
	GvNode* newInstance();

    // create a prototype instance of this prototype
    virtual GvNode *createInstance();


	// reads name fieldList { graph }
	static GvNode* read(GvInput *in);

	// read the event IS protoEvent part
	// event == fieldName

	GvBool readEventTranslation(
			GvInput *in, GvNode *object,
			const GvName &fieldName,
			int fieldIndex);

	/*
		process IS
		node fieldName IS protoEventName
	*/
	GvBool DoIs(GvInput *in,GvNode *node,GvField *field, const GvName &fieldName,GvName &protoEventName);


	//GvBool readInstance(GvInput *in);


	//int Do(GTraversal &state);   	

	int Do(GioTraversal &state);   	
	int Do(GDisplayTreeTraversal &state); // implemented in GDisplayTree.cpp
    
    friend class GvProtoInstance;
    friend class GvExternProto;
};





/*

#VRML V2.0 utf8
PROTO Gold   [] { Material { ... } }
PROTO Silver [] { Material { ... } }
...etc.

A material from this library could be used as follows:

#VRML V2.0 utf8
EXTERNPROTO Gold [] "http://.../materials.wrl#Gold"
...
    Shape {
        appearance Appearance { material Gold {} }
        geometry   ...
    
*/

class GvExternProto : public GvProto 
{
    GV_NODE_HEADER_UNKNOWN_NODE(GvExternProto);

public:

    // url to read proto from
    GvMFString url; // may contain url#name syntax 

	GvString baseUrl; // home url of scene
    GvNodeHandle proto; // pointer to prototype if loaded from file

	GvString    externProtoName;  // the name of the extern proto

	// create a prototype instance of this prototype, check for native node URN's
	GvNode*		createInstance();

	GFileRef    loader;			// url loader

	// get absolute home url
	const char *GetHomeUrl() const;


	// try to load url or get proto definition
	int Load(GTraversal &state);


    // get pointer to implementing proto
    GvProto *getProto();

	// get pointer to implementing proto, check for read of externProto
	GvProto *getProto(GTraversal &state);  
 

   	// reads name fieldList [ url ]
	static GvNode* read(GvInput *in);

	int Do(GioTraversal &state);   	
	int Do(GDisplayTreeTraversal &state); // implemented in GDisplayTree.cpp
};



// this node instances a given proto


class GvProtoInstance : public GvNode 
{
    GV_NODE_HEADER_UNKNOWN_NODE(GvProtoInstance);

public:

    // private children field   
    GvMFNode children;
    
    // Eval the "value" node of proto = first children node 
    GvNode* Eval();

    // Eval the "value" node of proto = first children node 
    GvNode* Eval(GTraversal &state);

    // create an instance of this proto
    GvProtoInstance(GvProto *proto);
    
    GvNodeHandle proto; // pointer to prototype
    
    // list of pointers to fields
    GvFieldList fields;
	int *fieldMap;

	// map field index in proto interface
	// to field index in proto instance
	// normally identity, but can occure in EXTERNPROTO
	int MapProtoField(int protoFieldIndex);

    
    // instancing relating helper functions
	GvBool instanced; // is Instance instanced ?

	// delete children
	void deleteAll(); 

	// create a new instance of instance, copy proto field and instance
	GvNode * newInstance();

    // Instance, create copies/refs of proto scene graph as needed 
    GvBool Instance(GTraversal &state);

private:
    // create fields according to proto´s interface
    void instanceFields();

	GvBool computeCopyOnInstance(GvCopyContext &context); 

    // Instance an MFNode field
    void Instance(GvCopyContext &context, GvMFNode *src,GvMFNode *dest);
    
    // Instance an SFNode field
    void Instance(GvCopyContext &context, GvSFNode *src,GvSFNode *dest);

    // instance proto node tree
    GvNode* InstanceNode(GvCopyContext &context, GvNode *node);
    
    // instance a single route 
    GvBool InstanceRoute(GvCopyContext &context, GvRoute *route);

    // Instance an MFNode field containing routes
    void InstanceRoutes(GvCopyContext &context,GvMFNode *src);


public:

	// get VRML class name
    const char *GetType() { return(getClassName()); }
	const char *getClassName() const;


    // get proto this instance is a proto from
    GvProto *getProto(GTraversal &state) const;

    // get proto this instance is a proto from
    GvProto *getDefiningProto(GTraversal &state) const;
    GvProto *getDefiningProto() const;

    // get field index of this instance
    GvField *getField(int index) const;

    //GvRoutes localRoutes; // local routes instanced

    // notification on field change
    virtual int OnChanged();


    int Do(GTraversal &state);   	
	int Do(GPathTraversal &state);   

	int Do(GioTraversal &state);   	
	//int Do(GDisplayTreeTraversal &state); // implemented in GDisplayTree.cpp

	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp


};




// helper class for storing route defintion
// for editing i/o etc.
// low level connections can't increment the reference counter of the target nodes,
// circular dependencies would result
// so route keeps also a ref to both nodes of the route 

class GvRoute : public GvNode {
    GV_NODE_HEADER(GvRoute);
public:
    // source node for route
	GvSFNode from;
    GvSFInt32 fromEventIndex;
    
	// destination node for route
    GvSFNode to;
    GvSFInt32 toEventIndex;

	// the initial value of field toEventIndex in to

	// could be used to restore all fiels with routes to initial values
	GvField *toInitialValue;

	void SaveInitialValue();
	void RestoreInitialValue();

	// is Node To the "to" NODE of this route ?
	GvBool isTo(GvNode *To) const { return(to.get() == To); }

	// is Node From the "from" NODE of this route ?
	GvBool isFrom(GvNode *From) const { return( from.get() == From); }

    // assign a parse route
    GvBool set(GvNode * From,int FromIndex,GvNode * To,int ToIndex); 

	// set by event names, return FALSE on mismatch
	GvBool set(GvNode * from, const char* fromEventOut, GvNode * to,const char * toEventIn,GvBool verbose = FALSE);

    // matches this route 
    GvBool match(GvNode * From,int FromIndex,GvNode * To,int ToIndex) const; 
	GvBool operator ==(const GvRoute &b) const;


    GvBool computeCopyOnInstance(GvCopyContext &context);

	// is this route in use  ?
    GvBool isUnused();

	// get the names of the events from the stored index 
    GvName getFromEventName() const;
    GvName getToEventName() const;

    int Do(GioTraversal &state);   	// in GioTraversal.cpp
	int Do(GDisplayTreeTraversal &state); // implemented in GDisplayTree.cpp

};



#endif /* _GVProto_ */

