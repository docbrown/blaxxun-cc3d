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

@module GvGroup.cpp - GLView VRML 1.0 group nodes|

Copyright (c) 1996 - 1998	by Holger Grahn
All rights reserved

Purpose:

Classes:


Notes:


  Changes:

$Revision: 1.3 $
$Log:


Todo :

******************************************************************************/

#include "stdafx.h"

#ifdef _G_VRML1

#include <gvinput.h>
#include <gvreaderror.h>
#include <gvfielddata.h>
#include <gvchildlist.h>
#include <gvgroup.h>

GV_NODE_SOURCE_IMP(GvGroup,GvNode);

GvGroup::GvGroup()
{

    children = new GvChildList();
    GV_NODE_CONSTRUCTOR(GvGroup);
    isBuiltIn = TRUE;

}

GvGroup::~GvGroup()
{	
	// if childs left, make sure to unref them 
    if (children->getLength() >0) {
	 	children->UnrefNodes();
	}
    delete children;
}

void GvGroup::addChild(GvNode *node)
{
 	node->ref();
   	children->append(node);
}

#if 0
// HG
GvNode *
GvGroup::getChild(int index) const
{
    return(*children)[index];
}

int
GvGroup::getNumChildren() const
{
    return children->getLength();
}
#endif


GvChildList *
GvGroup::getChildren() const
{
    return children;
}

GvBool
GvGroup::readInstance(GvInput *in)
{
    GvName	typeString;
    GvFieldData	*fieldData = getFieldData();

//    if (! isBuiltIn) { // fix from Eric Haines
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
//    }

    return (fieldData->read(in, this, FALSE) && readChildren(in));
}

GvBool
GvGroup::readChildren(GvInput *in)
{
    GvNode	*child;
    GvBool	ret = TRUE;
    char c;


    while (TRUE) {

			// get next char
      if (!in->read(c)) {  // EOF 
		  break;
	  }

      if ((c == '}')) { // CLOSE_BRACE) || (c == CLOSE_BRACE_CHAR)) { 
		  in->putBack(c);
		  break; 
	  }
	  in->putBack(c);

	// to do: bad : check for } manually to ignore bad nodes
	if (read(in, child)) {
	    if (child != NULL) {
		    child->ref(); // HG
		    children->append(child);
		}
	    else
			break; 
	}
	else {
		ret = FALSE;
	    break;
	}
    }
    return ret;
}


// HG edit functions 

// delete child at index
int GvGroup::deleteChild(int index)
{
   int numChilds=getNumChildren();
   
   if (index<0) index = numChilds+1-index;
   if (index >= numChilds)  {
   		return(-1);
   }
   else  if (index<0) return(-1);
   else {
   		
   		GvNode *child=(*children)[index];
   		children->remove(index);
		
		if (child) child->unref();

   }
   return(index);
}


// delete all children
int GvGroup::deleteChildren()
{
   int numChilds=getNumChildren();
   
   for(int i=0; i<numChilds;i++) {
   		GvNode *child=(*children)[i];
		if (child) child->unref();
		(*children)[i]= NULL;
   }
//   children->setSize(0);
   children->truncate(0);
   return(0);
}

// insert & ref child at index, return actual index of node in this
int GvGroup::insertChild(int index,GvNode *node)
{
   if (node == NULL) return(-1);

   int numChilds=getNumChildren();
   
   if (index<0) index = numChilds+1-index;
   if (index >= numChilds)  {
        index=numChilds;
   		addChild(node);
   }
   else {
   	 if (index<0) index = 0;
	 children->insert(index,node);
 	 node->ref();
   }
   return(index);
}


// insertChild(index, srcGroup[index])
int GvGroup::moveChild(int index,GvNode *srcGroup, int srcIndex)
{  
   if (!RTISA(srcGroup,GvGroup)) return(-1);
   
   GvGroup *s = (GvGroup *) srcGroup;

   if (srcGroup == this) {
   		GvNode * n= s->findChild(srcIndex); 
  		children->remove(index);
  		children->insert(index,n);


   } else {
   		GvNode * n= s->findChild(srcIndex); 
		if (!n) return(-1);
		this->insertChild(index,n);	// refcnt ++
		s->deleteChild(srcIndex);	// refcnt --
   }	
   return(0);
}

// find a child by index
GvNode * GvGroup::findChild(int index)
{
   int numChilds=getNumChildren();
   
   if (index<0) index = numChilds+1-index;
   if (index<0 || index >= numChilds) return(NULL);
   return(getChild(index));
}

// find a child by ptr, return index, -1  if not found
int  GvGroup::findChild(GvNode * node)
{
   return(children->find(node));
}


// find a child by name
GvNode * GvGroup::findChild(const GvName &name)
{
   int numChilds=getNumChildren();
   
   for(int i=0; i<numChilds;i++) {
   		GvNode *child=(*children)[i];
		if ((* child->objName) == name) return(child); 
   }
   return(NULL);
}

// find a child by type
GvNode * GvGroup::findChild(TypeId type)
{
   int numChilds=getNumChildren();
   
   for(int i=0; i<numChilds;i++) {
   		GvNode *child=(*children)[i];
		if (child->Id() == type) return(child); 
		if (child->IsA(type)) return(child); 
   }
   return(NULL);
}

//
// copy()
// copyChildrenTo(GvGroup	*dest)


#endif _G_VRML1

