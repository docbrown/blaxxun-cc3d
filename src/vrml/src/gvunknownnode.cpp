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

#ifdef _G_VRML1

#include <gvunknownnode.h>
#include <gvinput.h>
#include <gvreaderror.h>
#include <gvfield.h>


GV_NODE_SOURCE_IMP(GvUnknownNode,GvGroup);
// GvNode


GvUnknownNode::GvUnknownNode()
{
    GV_NODE_CONSTRUCTOR(GvUnknownNode);

    className = NULL;

    // Set global field data to this instance's
    instanceFieldData = new GvFieldData;
    fieldData = instanceFieldData; // ???????????? ok
	isAOk = -1;
	isA = NULL;
}

void
GvUnknownNode::setClassName(const char *name)
{
    className = strdup(name);
}


// HG
const char *GvUnknownNode::getClassName() const
{
	return(className);
}

// get VRML Type name for node 
const char * GvUnknownNode::GetType()
{ const char *n;
  n= getClassName();
  if (n) return(n);
  n = ClassName();

  if (strncmp(n,"Gv",2) == 0) n +=2;
  return(n);
}


//HG
GvFieldData *GvUnknownNode::getFieldData() const 
{
	return(instanceFieldData);
}

GvUnknownNode::~GvUnknownNode()
{
    if (className != NULL)
		free((void *) className);
	className = NULL;
// HG 
	if (isA) {
		isA->unref();
		isA = NULL;
	}
	if (instanceFieldData) {
		delete instanceFieldData;
		fieldData = NULL;
		// to do: check !!!!!!!!!!11
	}
}

GvBool
GvUnknownNode::readInstance(GvInput *in)
{
    GvName	typeString;
    GvFieldData	*fieldData = getFieldData();

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
	instanceFields();

    return (fieldData->read(in, this, FALSE) && readChildren(in));
}

// create fields according to protoFieldData
// see also GvProtoInstance
void GvUnknownNode::instanceFields()
{
    fields.DeleteFields();
    GvFieldData *fieldData = getFieldData();

    // instance all fields / events and assign default valus
    int numFields;
    fields.setSize(numFields = fieldData->getNumFields());
    for (int i=0; i<numFields; i++) 
    {
        GvField *field;
        fields[i]= field = fieldData->getFieldEntry(i)->createInstance();
        if (field) field->setContainer(this); // will set setDefault(TRUE);
    }

}

GvField *
GvUnknownNode::getField(int index) const
{
    ASSERT(index>=0 && index < fields.getLength());
    return(fields[index]);
}

#endif _G_VRML1



