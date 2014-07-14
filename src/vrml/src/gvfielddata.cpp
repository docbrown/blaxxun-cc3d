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

@module GvFieldData.cpp - VRML 1.0/2.0 node field/event information|

Copyright (c) 1996-1997	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvFieldEntry>
<c GvFieldData>


Notes:
	based on QvLib

Changes:

$Revision: 1.7 $
$Log:

Todo :

******************************************************************************/

#include "stdafx.h"
#include <gvinput.h>
#include <gvreaderror.h>
#include <gvfield.h>
#include <gvfielddata.h>
#include <gvconnection.h>

#include <gvkeyword.h>

// for ComputeCopyOnInstance
#include <gvmfnode.h>
#include <gvsfnode.h>

#include <gvproto.h>



#include <gvunknownnode.h>

#define OPEN_BRACE_CHAR		'['
#define CLOSE_BRACE_CHAR	']'
#define VALUE_SEPARATOR_CHAR	','


// the type pf field or pseudo field
const char * GvFieldTypeNames[] = 
{
		"field",
		"exposedField",
		"eventIn",
		"eventOut",
		"eventIn",
        NULL
};





const char* GvFieldEntry::getFieldType() const
{
      return(GvFieldTypeNames[type]);
}


const char *GvFieldEntry::getType() const 
{
    const char *t= eventTypeId.info->Name();
	if (strncmp(t,"Gv",2) == 0) return (t+2);
	return(t);
}


GvFieldEntry::GvFieldEntry(const GvFieldEntry &src) : 
		name(src.name),offset(src.offset),type(src.type),
		fieldClass(src.fieldClass),
		eventTypeId(src.eventTypeId),
		method(src.method)
{
	defaultValue = NULL;
	// defaultValue = clone(src.defaultValue)
}


// create a new field with the type of this fieldEntry
// and optionally assign the default value
GvField* GvFieldEntry::createInstance() const
{
    GvField *newField;
    // to do:could be optimize using TypeId
    // newField = GvField::createInstanceFromName(getType());

    // create a new instance of this type , or NULL if abstract
	newField = ( GvField*) eventTypeId.New();

    
    if (!newField) { 
        TRACE("Couldn´t allocate fieldInstance : %s\n", getType());
        return NULL;
    }
    
    // assign defaultvalue
    if (defaultValue != NULL) 
        *newField = *defaultValue;
    return(newField);
}

GvFieldEntry::~GvFieldEntry() 
{
    if(defaultValue) delete defaultValue;
}

void GvFieldEntry::setDefault(GvField *newDefault) 
{
	if(defaultValue) delete defaultValue;
	defaultValue = newDefault;
}

GvBool GvFieldEntry::matchFieldType(GvField*  field) 
{ 
    return(field->IsA(eventTypeId)); 
}


int GvEnumEntry::growSize = 6;

GvEnumEntry::GvEnumEntry(const GvName &name)
{
    typeName	= name;
    num		= 0;
    arraySize	= growSize;
    vals	= new int[arraySize];
    names	= new GvName[arraySize];
}

GvEnumEntry::~GvEnumEntry()
{
    delete [] vals;
    delete [] names;
}

GvFieldData::~GvFieldData()
{
    GvFieldEntry *tmpField;
    struct GvEnumEntry  *tmpEnum;
	int i;

    for (i=0; i<fields.getLength(); i++) {
        tmpField = (GvFieldEntry *)fields[i];
        delete tmpField;
    }
/*
    for (i=0; i<eventsIn.getLength(); i++) {
	    GvEventInEntry *tmp;
        tmp = (GvEventInEntry *) eventsIn[i];
        delete tmp;
    }
*/

    for (int j=0; j<enums.getLength(); j++) {
        tmpEnum = (struct GvEnumEntry *)enums[j];
        delete tmpEnum;
    }
}


// set field data from src 
void GvFieldData::set(GvFieldData &src)
{
    GvFieldEntry *tmpField;
    GvFieldEntry *newField;
	int i;

	ASSERT(fields.getLength() == 0);

    for (i=0; i<src.fields.getLength(); i++) {
        tmpField = (GvFieldEntry *)src.fields[i];
        newField = new GvFieldEntry(*tmpField); 

	    fields.append((void *) newField);
    }
	ASSERT(src.enums.getLength()==0); // to do:

}



GvFieldEntry*
GvFieldData::addField(  GvNode *defobj,// node containing field
                        const char *fieldName, // field name
		                const GvField *field,   // pointer to field
                        GvFieldType fieldType,  // field / event type
                        GvField* fieldDefault)  // default field value (of same class as GvField )
{
	ASSERT(field);
    GvFieldEntry *newField = new GvFieldEntry(fieldName,
							(const char *) field - (const char *) defobj,
                            fieldType,
							field->Id(),field->getFieldType(),
							fieldDefault);
    fields.append((void *) newField);
	return(newField);
}

GvFieldEntry* 
GvFieldData::addField(GvFieldEntry *newEntry)
{
   fields.append((void *) newEntry);
   return(newEntry);
}


// add EventIn
GvFieldEntry*
GvFieldData::addEventIn(GvNode *defObject, 
                             const char *eventName,
                             //void *method, 
							 GvNodeEventInMethod  method,
                             TypeId eventTypeId, 
							 GvFieldClass eventFieldClass,
                             GvFieldType type)
{

    GvFieldEntry *newField = new GvFieldEntry(eventName,
							0,
                            type,
							eventTypeId,
							eventFieldClass,
							NULL);

    newField->method = method;
    fields.append((void *) newField);
  
	return(newField);
}

#if 0
// now inline HG
const GvName &
GvFieldData::getFieldName(int index) const
{
    return ((GvFieldEntry *) fields[index])->name;
}
#endif

GvField *
GvFieldData::getField(const GvNode *object, int index) const
{
    return(object->getField(index));
    // VRML 2.0 getField(int) is now a virtual function of node for proto implementation
    //   return (GvField *) ((char *) object +  ((GvFieldEntry *) fields[index])->offset);
}

GvName GvFieldData::getEventInName(int index) const
{
	const GvName &n = getFieldName(index);
    GvFieldEntry *fieldEntry = getFieldEntry(index);

	if (fieldEntry->type /* getType()*/ <= GV_EXPOSED_FIELD) {
		GvString s("set_");
		s+=n.getString();
		return(GvName(s));
	} else return n;
}

GvName GvFieldData::getEventOutName(int index) const
{
	const GvName &n = getFieldName(index);
    GvFieldEntry *fieldEntry = getFieldEntry(index);

	if (fieldEntry->type /*getType()*/ <= GV_EXPOSED_FIELD) {
		GvString s(n.getString());
		s+="_changed";
		return(GvName(s));
	}
	else return(n);

}


const char * GvFieldData::getFieldType(const GvNode *object, int index) const 
{
	
    const char *t = NULL;
    GvField *f = object->getField(index);

    if (f == NULL) {
        GvFieldEntry *fieldEntry = getFieldEntry(index);
        return(fieldEntry->getType());
    }
    else 
	  t = f->ClassName();
	if (strncmp(t,"Gv",2) == 0) return (t+2);
	return(t);
}


// Find field with name fieldName in object
// NULL if field not found
GvField*
GvFieldData::getField(const GvNode *object,const char * fieldName) const
{
    for (int i = 0; i < fields.getLength(); i++) {
	    if (getFieldName(i) == fieldName) {
	        return(getField(object,i));
	    }
    }
    return(NULL);
}

// Find field with name fieldName in object, Result -1 if not found
int
GvFieldData::getFieldIndex(const char * fieldName) const
{
    for (int i = 0; i < fields.getLength(); i++) {
	    if (getFieldName(i) == fieldName) {
	        return(i);
	    }
    }
    return(-1);
}

// Find eventIn with name fieldName in object
// NULL if field not found
GvConnection*
GvFieldData::getConnectionIn(GvNode *object,const char * eventName, TypeId 	eventTypeId) const
{
    for (int i = 0; i < fields.getLength(); i++) {
        GvFieldEntry *entry= (GvFieldEntry *) fields[i];
        
        if (entry->eventTypeId == eventTypeId) // are types matching  ??
        switch(entry->type)  { 
        case GV_EXPOSED_FIELD: 
        case GV_FIELD_EVENT_IN:
            {
            if (entry->name == eventName)
                return new GvSetFieldConnection(getField(object,i));
            }
            break;
        case GV_NODE_EVENT_IN: 
            if (entry->name == eventName) {
                // strange cast, need method as proper type 
                GvNodeEventInMethod m = *((GvNodeEventInMethod*) &entry->method);
                return new GvNodeMethodConnection(object, m);
            }
            break;
        default : 
            break;
        }
    }
    {
        if (strncmp(eventName,"set_",4) == 0) {
            return getConnectionIn(object,&eventName[4],eventTypeId);
        }
    }
    return(NULL);
}



// Find eventIn with name eventName in object
// -1 if event not found 
int 
GvFieldData::getEventInIndex(GvNode *object,const char * eventName, TypeId 	eventTypeId) const
{  
    for (int i = 0; i < fields.getLength(); i++) {
        GvFieldEntry *entry= (GvFieldEntry *) fields[i];
        
        if (entry->eventTypeId == eventTypeId) // are types matching  ??
        switch(entry->type)  { 
        case GV_EXPOSED_FIELD: 
        case GV_FIELD_EVENT_IN:
            {
            if (entry->name == eventName)
                return i;
            }

            break;
        case GV_NODE_EVENT_IN: 
            if (entry->name == eventName) 
			{
                return(i);
            }
			// match "set_startTime" with "startTime"
			if (strncmp(entry->name.getString(),"set_",4) == 0) {
				if (strcmp(entry->name.getString()+4,eventName) == 0)
					return(i);
			}

            break;
        default : 
            break;
        }
    }

	// if prefix is set_eventName look for eventName 
    {
        if (strncmp(eventName,"set_",4) == 0) {
            return getEventInIndex(object,&eventName[4],eventTypeId);
        }
    }
    return(-1);
}

// Find eventIn with name eventName in object
// -1 if event not found 
int 
GvFieldData::getEventInIndex(GvNode *object,const char * eventName) const
{  
    for (int i = 0; i < fields.getLength(); i++) {
        GvFieldEntry *entry= (GvFieldEntry *) fields[i];
        
        switch(entry->type)  { 
        case GV_EXPOSED_FIELD: 
        case GV_FIELD_EVENT_IN:
            {
            if (entry->name == eventName)
                return i;
            }
            break;
        case GV_NODE_EVENT_IN: 
            if (entry->name == eventName) {
                return(i);
            }
			// match "set_startTime" with "startTime"
			if (strncmp(entry->name.getString(),"set_",4) == 0) {
				if (strcmp(entry->name.getString()+4,eventName) == 0)
					return(i);
			}
            break;
        default : 
            break;
        }
    }

	// if prefix is set_eventName look for eventName 
    {
        if (strncmp(eventName,"set_",4) == 0) {
            return getEventInIndex(object,&eventName[4]);
        }
    }
    return(-1);
}


// Find eventOut with name eventName in object
// return index in event info, -1 if event not found 

int 
GvFieldData::getEventOutIndex(const GvNode *object,const char * eventName) const
{   int index = -1;
    for (int i = 0; i < fields.getLength(); i++) {
        GvFieldEntry *entry= (GvFieldEntry *) fields[i];
        
        switch(entry->type)  { 
        case GV_EXPOSED_FIELD: 
        case GV_FIELD_EVENT_OUT:
            if (entry->name == eventName) {
               return(i); 
            }
            break;
        default : 
            break;
        }
    }			
    // do: _changed ...
    {
      //const char *p= strstr(eventName,"_changed");
       const char *p= strsuffix(eventName,"_changed");
       if (p) {
          GvString newEventName;
          char *x= newEventName.getBuffer(p-eventName+1);
          strncpy(x,eventName,p-eventName);
          x[p-eventName]=0;
          newEventName.releaseBuffer();

          return getEventOutIndex(object,newEventName);
       }
    }

    return(index);
}

#if 0


// Find eventIn with name fieldName in object
// NULL if field not found
GvField*
GvFieldData::getConnectionOut(const GvNode *object,const char * eventName) const
{
    for (int i = 0; i < fields.getLength(); i++) {
        GvFieldEntry *entry= (GvFieldEntry *) fields[i];
        
        switch(entry->type)  { 
        case GV_EXPOSED_FIELD: 
        case GV_FIELD_EVENT_OUT:
            {
            if (entry->name == eventName)
               return getField(object,i);
            }
            break;
        default : 
            break;
        }
    }
    {
       //const char *p= strstr(eventName,"_changed");
       const char *p= strsuffix(eventName,"_changed");

       if (p) {
          GvString newEventName;
          char *x= newEventName.getBuffer(p-eventName+1);
          strncpy(x,eventName,p-eventName);
          x[p-eventName]=0;
          newEventName.releaseBuffer();

          return getConnectionOut(object,newEventName);
       }
    }
    return(NULL);
}
#endif


// computes copyOnInstance flag from any child nodes flag set
GvBool 
GvFieldData::computeCopyOnInstance(GvNode *object,GvCopyContext &context)
{
    GvBool flag = FALSE;

    // for all fields 
    for (int i = 0; i < fields.getLength(); i++) {
        
        GvFieldEntry *entry= (GvFieldEntry *) fields[i];
        GvField *field = getField(object, i);

        
        if (!field) continue;

        if (RTISA(field,GvMFNode)) {
            if (((GvMFNode*) field)->computeCopyOnInstance(context)) flag = TRUE;
        }
        else if (RTISA(field,GvSFNode)) {
            //if (((GvSFNode*) field)->computeCopyOnInstance(context)) flag = TRUE;
			// PROBELEM are self references !!
			GvNode *n =((GvSFNode*) field)->value;
            if (n) { 
				if (n != object) {
					if (n->computeCopyOnInstance(context)) flag = TRUE;
                } 
				else { ASSERT(0); } // diag only
			}

        }
    }

   return(flag);
}



#include <ctype.h>
static GvName
stripWhite(const char *name)
{
    int firstchar = -1;
    int lastchar = -1;
    int lastwhite = -1;
    int i = 0;

    for ( i=0; name[i]; i++) {
	  if (isspace(name[i]))
	      lastwhite = i;
	  else {
	      if (firstchar == -1) firstchar = i;
	      lastchar = i;
	  }
    }

    if (lastchar > lastwhite)
	return GvName(&name[firstchar]);

    char buf[500];
    int b;
    for (b=0, i=firstchar; i<=lastchar; i++, b++)
	buf[b] = name[i];
    buf[b] = 0;
    return GvName(buf);
}

void
GvFieldData::addEnumValue(const char *typeNameArg,
			  const char *valNameArg, int val)
{
    struct GvEnumEntry	*e = NULL;
    GvName typeName = stripWhite(typeNameArg);
    GvName valName = stripWhite(valNameArg);

    for (int i=0; i<enums.getLength(); i++) {
	e = (struct GvEnumEntry *) enums[i];
	if (e->typeName == typeName)
	    break;
	else
	    e = NULL;
    }
    if (e == NULL) {
	e = new GvEnumEntry(typeName);
	enums.append((void*) e);
    }
    if (e->num == e->arraySize) {
	e->arraySize += GvEnumEntry::growSize;
	int *ovals = e->vals;
	GvName *onames = e->names;
	e->vals = new int[e->arraySize];
	e->names = new GvName[e->arraySize];
	for (int i=0; i<e->num; i++) {
	    e->vals[i] = ovals[i];
	    e->names[i] = onames[i];
	}
	delete [] ovals;
	delete [] onames;
    }
    e->vals[e->num] = val;
    e->names[e->num] = valName;
    e->num++;
}

void
GvFieldData::getEnumData(const char *typeNameArg, int &num,
			 const int *&vals, const GvName *&names)
{
    GvName typeName = stripWhite(typeNameArg);

    for (int i=0; i<enums.getLength(); i++) {
	struct GvEnumEntry *e = (struct GvEnumEntry *) enums[i];
	if (e->typeName == typeName) {
	    num		= e->num;
	    vals	= e->vals;
	    names	= e->names;
	    return;
	}
    }
    num = 0;
    vals = NULL;
    names = NULL;
}

GvBool
GvFieldData::readFieldTypes(GvInput *in, GvNode *object)
{
    GvBool gotChar;
    GvName fieldType, fieldName;
    char   c;

    if (! ((gotChar = in->read(c)) || c != OPEN_BRACE_CHAR))
	return FALSE;

    if (in->read(c) && c == CLOSE_BRACE_CHAR)
	return TRUE;

    in->putBack(c);

    GvBool alreadyHasFields = (object->isBuiltIn || getNumFields() != 0);

    while (TRUE) {

        if (! in->read(fieldType, TRUE) || ! in->read(fieldName, TRUE))
            return FALSE;

		if (! alreadyHasFields) {
			GvField *fld = GvField::createInstanceFromName(fieldType);
			if (fld == NULL) {
				GvReadError::post(in,GV_RE_UNKNOWN_FIELD, "%s No such field type \"%s\" ",fieldName.getString(), fieldType.getString());
				return FALSE; // hg 16.11.99
			} else {
				fld->setContainer(object);
				addField(object, fieldName.getString(), fld);
				delete fld; 
				fld = NULL;
			}
        }

	if (! in->read(c))
	    return FALSE;

	if (c == VALUE_SEPARATOR_CHAR) {

	    if (in->read(c)) {
		if (c == CLOSE_BRACE_CHAR)
		    return TRUE;
		else
		    in->putBack(c);
	    }
	}
	else if (c == CLOSE_BRACE_CHAR)
	    return TRUE;
	else
	    return FALSE;
    }
}

GvBool
GvFieldData::read(GvInput *in, GvNode *object,
		  GvBool errorOnUnknownField) const 
{
    GvName	fieldName;

    if (fields.getLength() == 0) return TRUE;    

    while (TRUE) {

	if (! in->read(fieldName, TRUE) || ! fieldName)
	    return TRUE;

    // there might be a ROUTE in between
    if (GvNode::isRouteOrProto(fieldName)) {
        if (GvNode::readRouteOrProto(in,fieldName))  continue;
        else return FALSE;
    }


	GvBool foundName;

	if (! read(in, object, fieldName, foundName)) {
		if (!foundName) { /* HG */
	    	if (errorOnUnknownField) 
				GvReadError::post(in,GV_RE_UNKNOWN_FIELD, "Bad field \"%s\" for a %s node",fieldName.getString(),object->GetType());
		}

	    return FALSE;
	}

// HG problem, can´t give error errorOnUnknown Field because
// it may be a child classname
	if (!foundName) {
	    if (errorOnUnknownField) {
		GvReadError::post(in,GV_RE_UNKNOWN_FIELD, "Unknown field \"%s\" for a %s node",
				  fieldName.getString(),object->GetType());
		return FALSE;
	    }
	    else {
		    in->putBack(fieldName.getString());
		return TRUE;
	    }
	}
    }
}




// Lookup field fieldName in object, and read value
// foundName = TRUE, field was found


GvBool
GvFieldData::read(GvInput *in, GvNode *object,
		  const GvName &fieldName, GvBool &foundName) const
{
    int i;

    // to do: event translation in PROTOS 
    for (i = 0; i < fields.getLength(); i++) {
		GvName fni = getFieldName(i);

	if (fieldName == fni) {
        foundName = TRUE;
        GvField *field = getField(object, i);
		GvFieldEntry *fieldEntry = getFieldEntry(i);

		GvBool isEvent = FALSE;


			switch (fieldEntry->type) {
			case GV_FIELD_EVENT_OUT:
			case GV_FIELD_EVENT_IN:
			case GV_NODE_EVENT_IN:
					isEvent=TRUE;
					break;
			}
			if (isEvent || field == NULL) {
				if (in->currentProto != NULL) {
					return in->currentProto->readEventTranslation(in,object,fieldName,i);
				}
				else { 
					if (field == NULL) { // eventIn Out can not be readed but ...
						GvReadError::post(in,GV_RE_UNKNOWN_FIELD, "%s::Can not read event field %s",object->GetType(), fieldName.getString());
						return FALSE; // hg 16.11.99
					}
					if (fieldEntry->type == GV_NODE_EVENT_IN)
						return FALSE;
					// can continue, simply read in field 
				}
			}


        if (!field) return(FALSE);
	    if (!field->read(in, fieldName))
		return FALSE;
	    break;
	}
    }
    
    if (i == fields.getLength())
	    foundName = FALSE;
    else foundName = TRUE;

	// VRML 2.0 draft  #2 fix, ignore s at end
	if (!foundName) {
		// Interpolater values ==> keyValues
		if (fieldName == "values") { //
			return read(in,object,"keyValue",foundName);
		}
		if (fieldName == "whichChild") { // switch 
			return read(in,object,"whichChoice",foundName);
		}
		if (strsuffix(fieldName.getString(),'s')) {
			const char *s = fieldName.getString();
			int slen = strlen(s);
			GvString ss;
			char *p=ss.getBuffer(slen+1);
			strcpy(p,s);
			p[slen-1]= 0;
			ss.releaseBuffer();
			GvName newFieldName(ss);
			return read(in,object,newFieldName,foundName);
		}
		if (fieldName == "rgb") { // Color 
			return read(in,object,"color",foundName);
		}
		// return (FALSE); gives problem in VRML 1.0
	}	
	
	if (!foundName) {
		GvBool isEvent = FALSE;
		int i = getEventInIndex(object,fieldName);
		if (i<0) i = getEventOutIndex(object,fieldName);
		if (i>=0) {
			foundName = TRUE;

			if (in->currentProto != NULL) {
				return in->currentProto->readEventTranslation(in,object,fieldName,i);
			}
			else { 
				GvReadError::post(in,GV_RE_UNKNOWN_FIELD, "%s::Can not read event field %s outside PROTO ",object->GetType(), fieldName.getString());
			}
		}
	}

    return TRUE;
}


// VRML 2.0

//// read interface element e.g. eventIn SFBool value 
// to do:exposedField 
// = equiv to field eventOut eventIn

GvBool
GvFieldData::readInterfaceElement(GvInput *in,GvBool alreadyHasFields,GvNode *object,GvBool readDefaults)
{
	GvName type;
    GvName fieldType, fieldName;
	GvBool alreadyDefined=FALSE;

          if (!in->read(type, TRUE))
            return FALSE;

        if (!in->read(fieldType, TRUE) || ! in->read(fieldName, TRUE))
            return FALSE;

		{
			GvBool isField = FALSE;
			GvFieldType eventType;

			if (type == "eventIn") {
				isField = TRUE;
				eventType = GV_FIELD_EVENT_IN;
			}
			else if (type == "eventOut") {
				isField = TRUE;
				eventType = GV_FIELD_EVENT_OUT;
			}
			else if (type == "field") {
				isField = TRUE;
				eventType = GV_FIELD;
			}
			else if (type == "exposedField") {
				isField = TRUE;
				eventType = GV_EXPOSED_FIELD;
			} else {
			   GvReadError::post(in,GV_RE_UNKNOWN_FIELD, "%s : Unknown event/field type \"%s\" for \"%s\"",object->getName().getString(),type.getString(),fieldName.getString());
               return(FALSE);
            }
			if (isField) {
				// if hasField(fieldName)
				// .... warning ? update info ??
				
				GvField *fld = GvField::createInstanceFromName(fieldType);
                if (!fld) {
				    GvReadError::post(in,GV_RE_UNKNOWN_FIELD, "%s : Unknown field type \"%s\" for \"%s\"",object->getName().getString(),fieldType.getString(),fieldName.getString());
                    return(FALSE);
                }

				ASSERT(fld!=NULL);

				GvField *defaultValue = NULL;
				fld->setContainer(object);

				if (in->currentProto) {
					// check for IS keyword !!!!!!!!!!!
					GvName is;
					if (in->read(is, TRUE) && is == "IS" ) {
						addField(object, fieldName.getString(), fld, eventType,defaultValue);
						GvName event;
						in->read(event, TRUE);
						in->currentProto->DoIs(in,object,fld,fieldName,event);
						delete fld; // 29.03.99 hg 
						return(TRUE);
					}
	                
					in->putBack(is.getString());
				}

				// read default value 
				if (readDefaults && (eventType == GV_FIELD || eventType == GV_EXPOSED_FIELD)) {
					defaultValue = GvField::createInstanceFromName(fieldType);	
					if (defaultValue->read(in,fieldName)) {
					}							
				}

                if (!alreadyHasFields) {
				    // TRACE("storing field %s \n",(const char *) fieldName.getString());
					
					// check if field defined 
					if (getField(object,fieldName) != NULL) {
						alreadyDefined = TRUE;
					} 
					else 
					if (getEventInIndex(object,fieldName) >=0) {
						alreadyDefined = TRUE;
					}
					else 
					if (getEventOutIndex(object,fieldName) >=0) {
						alreadyDefined = TRUE;
					}
		

					if (alreadyDefined){
					    GvReadError::post(in,GV_RE_UNKNOWN_FIELD, "%s : %s %s %s , event/field with this name already defined",object->getName().getString(),type.getString(),fieldType.getString(),fieldName.getString());
						delete fld;
						delete defaultValue;

						return FALSE; // hg 16.11.99
					}
				    // create field info for field data

					addField(object, fieldName.getString(), fld, eventType,defaultValue);
                    delete fld;
					// addField stores defaultValue

                } else {
                    delete fld;
                    delete defaultValue;
                }                    
			} // is field
			else {

				return FALSE;
			}
	}
    return(TRUE);
}

// reads PROTO interfaceElements up to ]
GvBool
GvFieldData::readProtoInterface(GvInput *in, GvNode *object,GvBool externProto)
{
    GvBool gotChar;
    char   c;
  
    if (! ((gotChar = in->read(c)) || c != OPEN_BRACE_CHAR))
	return FALSE;

    if (in->read(c) && c == CLOSE_BRACE_CHAR) {
	    return TRUE;
    }

    in->putBack(c);

    GvBool alreadyHasFields = (object->isBuiltIn || getNumFields() != 0);

    // read all field / eventIn / eventOut 
    while (TRUE) {
        if (!readInterfaceElement(in,alreadyHasFields,object,!externProto)) return(FALSE);

	    if (!in->read(c))
	        return FALSE;

	    if (c == CLOSE_BRACE_CHAR)
	        break;
    
        in->putBack(c);

    } // while
	return TRUE;
}


// read interface of script 

GvBool
GvFieldData::readScriptInterface(GvInput *in, GvNode *object)
{
    char   c;

    if (in->read(c) && c == CLOSE_BRACE) {
	    return TRUE;
    }
    in->putBack(c);

    GvBool alreadyHasFields = (object->isBuiltIn || getNumFields() != 0);

    // read all field / eventIn / eventOut 
    while (TRUE) {
        GvName fieldName;
        GvBool foundName=FALSE;

        // read a name
        if (!in->read(fieldName, TRUE) || ! fieldName)
                return(FALSE);

         // check if built in field
	     if (! read(in, object, fieldName, foundName)) {
                if (foundName) return FALSE; // couldn´t read value
         }
         
         if (!foundName)    // a field with name not found,
         {
                in->putBack(fieldName.getString());
                if (!readInterfaceElement(in,FALSE,object,TRUE)) return(FALSE);
         }

        // check for terminator 
	    if (!in->read(c))
	        return FALSE;

	    if (c == CLOSE_BRACE) {
            in->putBack(c);
	        return TRUE;
        }
	    else if (c == VALUE_SEPARATOR_CHAR) 
        ;
        else 
        in->putBack(c);

    } // while
	return TRUE;
}




// set field pointer at index, list is expandend
// old value is not freed
void GvFieldList::set(int index, GvField *newField)
{
    if (index>=getLength()) setSize(index+1);
    (*this)[index] = newField;
}


void GvFieldList::append(GvField *newField) 
{
    GvPList::append((void *) newField);
}

// delete all fields and truncate
void GvFieldList::DeleteFields() 
{
  int l = getLength();
  if (l <=0) return;
  for (int i=l-1; i>=0; i--) {
      GvField *n= (*this)[i];;
	  if (n) {
		  delete n;
		  (*this)[i]=NULL;
	  }	
  }
  truncate(0);
}

GvFieldList::~GvFieldList() 
{ 
	DeleteFields();
	//Plist destructur must be called !
}

