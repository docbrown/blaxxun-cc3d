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
#ifndef  _GV_SUB_NODE_
#define  _GV_SUB_NODE_

/******************************************************************************
@doc

@module GvSubNode.h - VRML Node declaration MACROS|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:
  Macros for implementing sub-Nodes 
  and registering field information 

  originally based on QvLib

Classes:
  
Notes:
 be carefull, no space after \ in Macros allowed

  Changes:

$Revision: 1.5 $
$Log: gvsubnode.h,v $
Revision 1.5  1999/07/06 16:35:33  tom
*** empty log message ***

Revision 1.4  1999/05/10 13:54:22  tom
*** empty log message ***


Todo :

******************************************************************************/


#include <gvfielddata.h>
#include <gvnode.h>

/*! \def GV_NODE_HEADER(className)
 * add Node interface and Runtime Type information members to class defintion
 */

#define GV_NODE_HEADER(className)			  \
  public:								      \
    RT(className);							  \
    className();						      \
    virtual ~className();				      \
    virtual GvFieldData *getFieldData() const { return fieldData; } \
    static void termClass() { delete fieldData; fieldData = NULL; firstInstance= TRUE; } \
  private:								      \
    static GvBool	firstInstance;			  \
    static GvFieldData	*fieldData			  \

/*! \def GV_NODE_HEADER_CUSTOM(className)
 * custom constructor, add Node interface and Runtime Type information members to class defintion
 */

#define GV_NODE_HEADER_CUSTOM(className)					      \
  public:								      \
    RT(className);								\
    virtual ~className();						      \
    virtual GvFieldData *getFieldData() const { return fieldData; } \
    static void termClass() { delete fieldData; fieldData = NULL; firstInstance= TRUE; } \
  private:								      \
    static GvBool	firstInstance;					      \
    static GvFieldData	*fieldData					      \


#define GV_NODE_HEADER_UNKNOWN_NODE(className)   \
  public:								        \
    RT(className);							    \
    className();						\
    virtual ~className();						\
    virtual GvFieldData *getFieldData() const; \
    static void termClass() { delete fieldData; fieldData = NULL; firstInstance= TRUE; } \
  private:								      \
    static GvBool	firstInstance;			  \
    static GvFieldData	*fieldData			   \


/*! \def GV_NODE_SOURCE(className)
 * goes into source, to init class static variables
 */

#define GV_NODE_SOURCE(className)					      \
    GvFieldData	       *className::fieldData;				      \
    GvBool		className::firstInstance = TRUE;

/*! \def GV_NODE_SOURCE_IMP(className,superClassName)
 * goes into source, to init class static variables
 * and setup run-time type information
 */

#define GV_NODE_SOURCE_IMP(className,superClassName)					      \
	 RTIMP(className,0,superClassName); 						\
    GvFieldData	       *className::fieldData = NULL;	     \
    GvBool		className::firstInstance = TRUE;


/*! \def GV_NODE_CONSTRUCTOR(className)
 * goes into Node constructor, to aid in construction of fieldata
 */
#define GV_NODE_CONSTRUCTOR(className)					      \
    if (fieldData == NULL)						      \
		fieldData = new GvFieldData;					      \
    else								      \
		firstInstance = FALSE;						      \
    isBuiltIn = FALSE;							      \

#define GV_NODE_IS_FIRST_INSTANCE() (firstInstance == TRUE)

/*! \def GV_NODE_ADD_FIELD(fieldName)
 * add an field/exposedField to Node given name of member field
 */
#define GV_NODE_ADD_FIELD(fieldName)					      \
    if (firstInstance)							      \
		 fieldData->addField(this, GV__QUOTE(fieldName), &this->fieldName);    \
    this->fieldName.setContainer(this);

/*! \def GV_NODE_ADD_FIELD_EVENT_IN(fieldName)
 * add an eventIn field to Node given name of field member
 */
#define GV_NODE_ADD_FIELD_EVENT_IN(fieldName)					      \
    if (firstInstance)							      \
		 fieldData->addField(this, GV__QUOTE(fieldName), &this->fieldName,GV_FIELD_EVENT_IN);    \
    this->fieldName.setContainer(this);	  \
    this->fieldName.flags.isInput = TRUE;  \
    this->fieldName.flags.isOutput = FALSE;  


// add a EventIn method to field information
#define GV_NODE_ADD_EVENT_IN_OLD(eventName,typeClass)	\
    if (firstInstance) {							      \
        void (GvNode::*func)(typeClass *val); \
        func =(void (GvNode::*)(typeClass *val)) eventName; \
	    fieldData->addEventIn(this, GV__QUOTE(eventName),*((void **) &func),typeClass::ClassId(),GV_NODE_EVENT_IN);    \
    }

/*! \def GV_NODE_ADD_EVENT_IN(eventName,typeClass)
 * add an eventIn given a member function
 */
#define GV_NODE_ADD_EVENT_IN(eventName,typeClass)	\
    if (firstInstance) {							      \
        void (GvNode::*func)(typeClass *val); \
        func = (void (GvNode::*)(typeClass *val))  eventName; \
	    fieldData->addEventIn(this, GV__QUOTE(eventName), (GvNodeEventInMethod) func,typeClass::ClassId(),typeClass::getClassFieldType(),GV_NODE_EVENT_IN);    \
    }

// hg: VC does not like  &function
/*! \def GV_NODE_ADD_EVENT_IN_NAME(eventName,function,typeClass)
 * add an eventIn given the name and a member function
 */

#define GV_NODE_ADD_EVENT_IN_NAME(eventName,function,typeClass)	\
    if (firstInstance) {							      \
        void (GvNode::*func)(typeClass *val); \
        func = (void (GvNode::*)(typeClass *val)) function; \
	    fieldData->addEventIn(this, GV__QUOTE(eventName), (GvNodeEventInMethod) func,typeClass::ClassId(),typeClass::getClassFieldType(),GV_NODE_EVENT_IN);    \
    }

/*! \def GV_NODE_ADD_EVENT_OUT(fieldName)
 * add an eventOut field to Node given name of field member
 */
#define GV_NODE_ADD_EVENT_OUT(fieldName)					      \
    if (firstInstance)							      \
		 fieldData->addField(this, GV__QUOTE(fieldName), &this->fieldName,GV_FIELD_EVENT_OUT);    \
    this->fieldName.setContainer(this); \
    this->fieldName.flags.isInput = FALSE;  \
    this->fieldName.flags.isOutput = TRUE;  

/*! \def GV_NODE_DEFINE_ENUM_VALUE(enumType,enumValue)
 * add an Inventor, VRML 1 ENUM Value definition
 */
#define GV_NODE_DEFINE_ENUM_VALUE(enumType,enumValue)			      \
    if (firstInstance)							      \
		fieldData->addEnumValue(GV__QUOTE(enumType),			      \
				GV__QUOTE(enumValue), enumValue)

#endif /* _GV_SUB_NODE_ */

