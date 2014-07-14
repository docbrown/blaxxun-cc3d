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
#ifndef  _GV_FIELD_DATA_
#define  _GV_FIELD_DATA_
/******************************************************************************
@doc

@module GvFieldData.h - Node field description|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:


Notes:

Changes:
$Revision: 1.15 $
$Log: gvfielddata.h,v $
Revision 1.15  1999/07/06 16:34:42  tom
*** empty log message ***


Todo :

******************************************************************************/




#include <gvbasic.h>
#include <gvplist.h>
#include <gvstring.h>


#ifndef _COM
// Support typing for Java and for type checking when parsing routes.
// COM : already in GlviewVrml.h

enum GvFieldClass {
        UnknownType = 0,
        SFBOOL,     SFIMAGE,     SFTIME,
        SFCOLOR,    MFCOLOR,     SFFLOAT,     MFFLOAT,
        SFINT32,    MFINT32,     SFNODE,      MFNODE,
        SFROTATION, MFROTATION,  SFSTRING,    MFSTRING,
        SFVEC2F,    MFVEC2F,     SFVEC3F,     MFVEC3F,
		
		// VRML 2.0
		SFENUM, SFBITMASK, SFMATRIX, SFLONG, MFLONG, 

		// VRML 97
		MFTIME,

		// Inventor
		SFVEC4F,	MFVEC4F
};

#endif

//
// field are storing a value
// 
#ifdef _COM


#include "glviewvrml.h"

typedef FieldType GvFieldClass;

#endif








// to be able to declare pointer to member function 
#include <gvnode.h>

typedef   void (GvNode::*GvNodeEventInMethod)(GvField *event);


class GvConnection;

class GvField;
class GvInput;
class GvNode;
class GioTraversal;

// the type pf field or pseudo field
enum GvFieldType {
		GV_FIELD,
		GV_EXPOSED_FIELD,	// field supporting eventIn,Event Out
		GV_FIELD_EVENT_IN,
		GV_FIELD_EVENT_OUT,
		GV_NODE_EVENT_IN, // node is receiving event in
	};


enum GvEventInType {
		GV_EVENT_NODE_EVENT_IN, // node is receiving event in
		GV_EVENT_FIELD_EVENT_IN, // field is receiving event in
	};

// Field // Eventdescriptor
//
class GvFieldEntry {
public:

    GvName		name;	// name of field /event
    long		offset;	// binary pointer offset to field in a container node

	// VRML 2.0 type event information

	TypeId		eventTypeId;	// the event/field type GvSFString ...
	GvFieldType type;	// type of event/field
	GvFieldClass fieldClass; // we need this for VRML script 
	
	GvField		*defaultValue; // the default value 

    //void *method;	// EventIn method // could use offset 
    void (GvNode::*method)(GvField *event);


	GvFieldEntry( const char *Name, long Offset,
                  GvFieldType Type,TypeId EventTypeId,GvFieldClass FieldClass,
                  GvField *Default=NULL) :
				  name(Name),offset(Offset),
					  type(Type),fieldClass(FieldClass),
					  eventTypeId(EventTypeId),defaultValue(Default)
					{
                      method = NULL;
					}
	GvFieldEntry(const GvFieldEntry &src);

	~GvFieldEntry();
	
	void setDefault(GvField *newDefault=NULL);

    const GvName &	getName() const
        {return name; }

    const char *getType() const;

    const char* getFieldType() const;
	//GvFieldClass getFieldType()

    GvBool isOutput() { return (type == GV_EXPOSED_FIELD) || (type == GV_FIELD_EVENT_OUT); }
    GvBool isInput() { return (type == GV_EXPOSED_FIELD) || (type == GV_FIELD_EVENT_IN) || (type == GV_NODE_EVENT_IN); }

    //GvBool matchFieldType(TypeId otherTypeId) { return(eventTypeId == otherTypeId); }
    
    GvBool matchFieldType(TypeId otherTypeId) { return(otherTypeId.info->IsA(eventTypeId.info)); }
    GvBool matchFieldType(GvFieldClass otherType) { return(otherType == fieldClass); }
    GvBool matchFieldType(GvFieldEntry &other) { return(matchFieldType(other.eventTypeId)); }
    GvBool matchFieldType(GvField*  field);
    
    GvField *getDefault() const { return(defaultValue); }

    // create a new field with the type of this fieldEntry
    // and optionally assign the default value
    GvField* createInstance() const;


};

#if 0

// EventIn desrciptor
//
class GvEventInEntry {
public:

    GvName		name;	// name of event
	GvEventInType type;	// type of eventIn
	TypeId	eventTypeId;	// the event/field type GvSFString ...
	void *method;	// EventIn method // could use offset

//    GvNodeEventInMethod nodeMethod;

	GvEventInEntry(const char *Name, void *Method,TypeId EventTypeId, GvEventInType Type) :
					name(Name),method(Method),eventTypeId(EventTypeId), type(Type)
					{
					}
};
#endif


struct GvEnumEntry {
    GvName		typeName;
    int			num;
    int			arraySize;
    int			*vals;
    GvName		*names;			
    GvEnumEntry(const GvName &name);
    ~GvEnumEntry();

    static int		growSize;
};


class GvFieldData {
  public:
    GvFieldData() {}
    ~GvFieldData();

	// Fields
	
	// add a field descriptor, 
    // fieldDefault NULL, or ptr is owned
    // offset is computed relative to defObject !! to do: how to deal with Proto, Script fields
    //
    GvFieldEntry* addField(GvNode *defObject, const char *fieldName, const GvField *field,GvFieldType type=GV_EXPOSED_FIELD,GvField* fieldDefault=NULL);

    GvFieldEntry* addField(GvFieldEntry *entry);

    // get number of fields
    int	getNumFields() const	{ return fields.getLength(); }

    // get name for field #i
    const GvName &	getFieldName(int index) const
        {return ((GvFieldEntry *) fields[index])->name; }

	// get event in name for field #i
	GvName getEventInName(int index) const;

	// get event out name for field #i
	GvName getEventOutName(int index) const;

    // get field type name for field #i
    const char * getFieldType(const GvNode *object,int index) const;	// HG

    // get field ptr in node object  for field #i
    GvField *	getField(const GvNode *object, int index) const;

	GvFieldEntry *getFieldEntry(int index) const { return ((GvFieldEntry *) fields[index]); }

    // Find field with name fieldName in object
    // NULL if field not found
    GvField*    getField(const GvNode *object,const char * fieldName) const;

    // Find field with name fieldName in object, Result -1 if not found
    int getFieldIndex(const char *fieldName) const;

	// add EventIn
    GvFieldEntry*
    addEventIn(GvNode *defObject, const char *eventName,GvNodeEventInMethod  method, 
	TypeId eventTypeId, GvFieldClass eventFieldClass, GvFieldType type=GV_NODE_EVENT_IN);
    
    // get number of eventsIn
    // int	getNumEventsIn() const	{ return eventsIn.getLength(); }
	//GvEventInEntry *getEventInEntry(int index) const {   return ((GvEventInEntry *) eventsIn[index]); }

	// lookup event/field info, and return a toConection object 
	GvConnection* getConnectionIn(GvNode *object,const char *eventName, TypeId 	eventTypeId) const;

	// lookup event/field info, and return a fromConection object 
	GvField* getConnectionOut(const GvNode *object,const char *eventName) const;

    // Find eventIn with name eventName in object
    // -1 if event not found 
    int  getEventInIndex(GvNode *object,const char * eventName, TypeId 	eventTypeId) const;
    int  getEventInIndex(GvNode *object,const char * eventName) const;

    // Find eventOut with name eventName in object
    // return index in event info, -1 if event not found 
    int  getEventOutIndex(const GvNode *object,const char * eventName) const;

    // computes copyOnInstance flag from any child nodes flag set
    GvBool computeCopyOnInstance(GvNode *node,GvCopyContext &context);
    GvBool copyOnInstance(GvNode *node);



	// Enum

    void		addEnumValue(const char *typeName,
				     const char *valName, int val);
    void		getEnumData(const char *typeName, int &num,
				    const int *&vals, const GvName *&names);

    GvBool		read(GvInput *in, GvNode *object,
			     GvBool errorOnUnknownField = TRUE) const;

    GvBool		read(GvInput *in, GvNode *object,
			     const GvName &fieldName,
			     GvBool &foundName) const;

    GvBool		readFieldTypes(GvInput *in, GvNode *object);

    GvBool      readInterfaceElement(GvInput *in,GvBool alreadyHasFields,GvNode *object,GvBool readDefaults=TRUE);

    // read proto fields / interface 
	GvBool	    readProtoInterface(GvInput *in, GvNode *object,GvBool externProto=FALSE);
    GvBool      readScriptInterface(GvInput *in, GvNode *object);

	// set field data from src 
	void set(GvFieldData &src);


	// write all fields of node 
	int Do(GioTraversal &state, GvNode *object);

	// write a single field including PROTO IS connections 
	int Do(GioTraversal &state, GvNode *object,int index);

	// write all IsConnections of node 
	int DoIsConnections(GioTraversal &state, GvNode *object);

	int DoFieldTypes(GioTraversal &state,const GvNode *object);

    // write interface for Proto, externproto etc.
    int DoInterface(GioTraversal &state,const GvNode *object,GvBool writeDefaults= TRUE,int offset=0);

	// write interface for Script 
	int DoScriptInterface(GioTraversal &state, GvNode *object,int offset);





  private:
    GvPList		fields;
    GvPList		enums;

};    


// store a list of field pointers
// fields are deleted on deletion
//

class GvFieldList : public GvPList {
  public:
    
    GvFieldList(int size=0) : GvPList(size) {}

    // delete all fields and truncate
    void DeleteFields(); 

    
    ~GvFieldList();

    void append(GvField *newField); 
    
    // set field pointer at index, list is expandend
    // old value is not freed
    void set(int index, GvField *newField);

    GvField*	& operator [](int i) 
	{ return ( (GvField * &) ( (*(GvPList *) this) [i] ) ); }
    
    GvField*	 operator [](int i) const 
	{ return ( (GvField *) ( (*(GvPList *) this) [i] ) ); }

};



#endif /* _GV_FIELD_DATA_ */
