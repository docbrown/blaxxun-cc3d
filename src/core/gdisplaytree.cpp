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

@module GDisplayTree.cpp - GLView tree display traversal routines	|

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:
create a tree view display of the qv scene tree

Classes:
GDisplayTreeTraversal

Notes:

Changes:

Todo :

******************************************************************************/

#include "stdafx.h"



#include "gclass.h"
#include "gshell.h"
#include "ggeom.h"

#include "greporter.h"



#include <gvnodes.h>
#include <gvfields.h>
#include <gvnamespace.h>
#include <gvkeyword.h>
#include <gvscene.h>

#include <gvelement.h>
#include <gvunknownnode.h>

#include <gvproto.h>



#if defined(_GLVIEW_DLL)
#define _NO_DISPLAY_TREE

#else

#include "glviewhdr.h"
#include "DTreeEditor.h"

#endif

#include "gdisplaytree.h"


/*

   insert tree item for node,

   keep track of DEF / USE, expand only first instance == DEF

   type = type number for AddItem inorder to choose icon


*/

static GvBool nameGenerationWarning = FALSE;


HTREEITEM GDisplayTreeTraversal::InsertNodeItem(GvNode *node,
	int &traverseChildren, int type)
{
  CString buf;

  traverseChildren = 1;

  if (node->objName) { 

  
  const GvName &name0 = node->getName();


  if ((node->refCnt >1) && (name0.getLength() == 0)) {
		if (nameGenerationWarning) Reporter.Warning("Node Reference counter >1, but no name, generating a default name");
		// to do: generate name
		//char buf[60];
		//sprintf(buf,"OBJ_%lx",(long) node);
		//GvName newName(buf);
        GvName newName;
        // assume _1 _2 names could not occur
        // or use node dict
        generateNodeName(*refDict,"",newName);
		node->setName(newName);
  }	

  
  if (node->getName().getLength()>0) {

    void	*n;

	// check if node with this name has been written
    if (refDict->find((u_long) node->getName().getString(), n)) {

		if (((GvNode *) n) == node) { // write out a reference
  			buf +="USE ";
			traverseChildren = 0;
			//return(GV_NODE_ALREADY_SEEN);

		} else {
			//Reporter.Warning("Warning duplicate definition of node name '%s'",node->getName().getString());
			int suffix = 1;
			char buf[260];
			// create a unique name by adding a suffix _###
			while (1) {
				sprintf(buf,"%s_%d",node->getName().getString(),suffix);
				GvName newName(buf);
				if (!refDict->find((u_long) newName.getString(), n)) {
					TRACE("Created unique name %s \n",newName.getString());
					node->setName(newName);
					break; // so it will be defe´d 
				}
				suffix++;
			}

		}

	}

    const GvName &name = node->getName();

  	if (traverseChildren) {

        if (RTISA(node,GvProto)) {
            buf += "PROTO ";
            // name = classname, so don´t append class
            // put proto in extras name space
        }
        else if (RTISA(node,GvExternProto)) {
            buf += "EXTERNPROTO ";
            // name = classname, so don´t append class
            // put proto in extras name space
        }
  		else  { 
            // to do:? if (format >= FORMAT_EXTENDED) buf += "DEF " ;
  		    buf += name.getString();  	
        }
		buf += " ";
    	refDict->enter((u_long) name.getString(), (void *) node);
	} 
	else buf += name.getString();  	

	} // length > 0
   	
   	} // node->objName

	if (traverseChildren)
   	   buf += node->GetType(); 

#ifdef  _NO_DISPLAY_TREE
    return NULL;
#else 
	return(returnItem = treeEditor->AddItem(top,buf,node,topInsertAfter,0,type));
#endif
}


int GNode::Do(GDisplayTreeTraversal &state) // declared in GClass.h
{
	CString buf;
    buf += ClassName(); 
#ifndef  _NO_DISPLAY_TREE
	state.returnItem = state.treeEditor->AddItem(state.top,buf,this,state.topInsertAfter);
#endif

	return(1);

}

int GvNode::Do(GDisplayTreeTraversal &state) 
{  

   switch (state.mode) {
   case GDisplayTreeTraversal::MODE_EXTERN_REF:
		return(0);
   case GDisplayTreeTraversal::MODE_DEF:
   		if (getName().getLength() == 0)  return(0);

   case GDisplayTreeTraversal::MODE_TREE:
   default: {
#ifndef  _NO_DISPLAY_TREE
		int traverse;
		state.InsertNodeItem(this,traverse,DGvTreeItem::GV_NODE);
#endif
		}
		break;
   }
   return(1);
}

#ifdef _G_VRML1

int GvGroup::Do(GDisplayTreeTraversal &state) 
{  int traverse=1;
   int ret = 1;
 

   switch (state.mode) {
   case GDisplayTreeTraversal::MODE_EXTERN_REF:
   		ret=TraverseChildren(state);
		break;
/*
   case GDisplayTreeTraversal::MODE_DEF:
        // if (
     	ret=TraverseChildren(state);
		break;
*/

   case GDisplayTreeTraversal::MODE_TREE:
   default:
		{
#ifndef  _NO_DISPLAY_TREE

   		HTREEITEM thisItem = state.InsertNodeItem(this,traverse, DGvTreeItem::GV_GROUP_NODE);

   		if (traverse) {
   			TREE_PUSH(state,thisItem);
   				state.topInsertAfter = TVI_LAST;
   				ret=TraverseChildren(state);
   			TREE_POP(state);
   		}
   
   		state.returnItem = thisItem;
#endif
		}
   break;
   }
   return(ret);
}

#endif _G_VRML1

int Gv2Group::Do(GDisplayTreeTraversal &state) 
{  int traverse;
   int ret = 1;
 

   switch (state.mode) {
   case GDisplayTreeTraversal::MODE_EXTERN_REF:
   		ret=TraverseChildren(state);
		break;
   case GDisplayTreeTraversal::MODE_DEF:
     	ret=TraverseChildren(state);
		break;

   case GDisplayTreeTraversal::MODE_TREE:
   default:
		{
#ifndef  _NO_DISPLAY_TREE

   		HTREEITEM thisItem = state.InsertNodeItem(this,traverse, DGvTreeItem::GV_GROUP_NODE);
	    // must be expanded via NodeFields 
#if 0
   		if (traverse) {
   			TREE_PUSH(state,thisItem);
   				state.topInsertAfter = TVI_LAST;
   				ret=TraverseChildren(state);
   			TREE_POP(state);
   		}
#endif
   
   		state.returnItem = thisItem;
#endif
		}
   break;
   }
   return(ret);
}


#ifdef _G_VRML1

int GvTexture2::Do(GDisplayTreeTraversal &state) 
{  
   int ret = 1;
 

   switch (state.mode) {
   case GDisplayTreeTraversal::MODE_EXTERN_REF:
   			if (filename.value.getLength() == 0)  return(0);
			// fall through
   case GDisplayTreeTraversal::MODE_TREE:
   default:
		{
   		ret =GvNode::Do(state);
#ifndef  _NO_DISPLAY_TREE
		int traverse;
		state.InsertNodeItem(this,traverse,DGvTreeItem::GV_NODE);
		ret = 1;
#endif
		}
   break;
   }
   return(ret);
}

int GvWWWInline::Do(GDisplayTreeTraversal &state) 
{
   //return GvGroup::Do(state);		

   int traverse=0;
   int ret=0;
   switch (state.mode) {
   case GDisplayTreeTraversal::MODE_EXTERN_REF:

   case GDisplayTreeTraversal::MODE_TREE:
   default: {

 #ifndef  _NO_DISPLAY_TREE
   HTREEITEM thisItem = state.InsertNodeItem(this,traverse, DGvTreeItem::GV_GROUP_NODE);

   if (traverse) {
   	TREE_PUSH(state,thisItem);
		// inline has separate name space
		GvDict		*saveRefDict = state.refDict;	// NAME HtreeItem 
    	state.refDict = new GvDict();
   		state.topInsertAfter = TVI_LAST;
   		ret = TraverseChildren(state);
		
		delete state.refDict;
		state.refDict = saveRefDict;

   	TREE_POP(state);
   }
   
   state.returnItem = thisItem;
#endif
   break;
   }
   }
   return(ret);
}
#endif _G_VRML1

int Gv2Inline::Do(GDisplayTreeTraversal &state) 
{

   int traverse=0;
   int ret=0;
   switch (state.mode) {
   case GDisplayTreeTraversal::MODE_EXTERN_REF:
   case GDisplayTreeTraversal::MODE_TREE:
   default: {

 #ifndef  _NO_DISPLAY_TREE
   HTREEITEM thisItem = state.InsertNodeItem(this,traverse, DGvTreeItem::GV_GROUP_NODE);

   if (traverse) {
   	TREE_PUSH(state,thisItem);
		// inline has separate name space
		GvDict		*saveRefDict = state.refDict;	// NAME HtreeItem 
    	state.refDict = new GvDict();
   		state.topInsertAfter = TVI_LAST;
   		ret = TraverseChildren(state);
		
		delete state.refDict;
		state.refDict = saveRefDict;

   	TREE_POP(state);
   }
   
   state.returnItem = thisItem;
#endif
   break;
   }
   }
   return(ret);
}

#ifdef _G_VRML1

int GvWWWAnchor::Do(GDisplayTreeTraversal &state) 
{
   //return GvGroup::Do(state);		

   int traverse=1;
   int ret=0;
   switch (state.mode) {
   case GDisplayTreeTraversal::MODE_EXTERN_REF:
   		if (name.value.getLength() == 0)  return(TraverseChildren(state));

   case GDisplayTreeTraversal::MODE_TREE:
   default: 
		{
#ifndef  _NO_DISPLAY_TREE

   		HTREEITEM thisItem = state.InsertNodeItem(this,traverse, DGvTreeItem::GV_GROUP_NODE);

   		if (traverse) {
   			TREE_PUSH(state,thisItem);
   				state.topInsertAfter = TVI_LAST;
   				ret=TraverseChildren(state);
   			TREE_POP(state);
   		}
   
   		state.returnItem = thisItem;
#endif
		}

   		break;
   }
   return(ret);
}
#endif _G_VRML1

int Gv2Anchor::Do(GDisplayTreeTraversal &state) 
{
   //return Gv2Group::Do(state);		

   int traverse=1;
   int ret=0;
   switch (state.mode) {
   case GDisplayTreeTraversal::MODE_EXTERN_REF:
   		if (url.getNum() == 0)  return(TraverseChildren(state));

   case GDisplayTreeTraversal::MODE_TREE:
   default: 
		{
#ifndef  _NO_DISPLAY_TREE

   		HTREEITEM thisItem = state.InsertNodeItem(this,traverse, DGvTreeItem::GV_GROUP_NODE);

		if (traverse) {
   			TREE_PUSH(state,thisItem);
   				state.topInsertAfter = TVI_LAST;
   				ret=TraverseChildren(state);
   			TREE_POP(state);
   		}
   
   		state.returnItem = thisItem;
#endif
		}

   		break;
   }
   return(ret);
}

#ifdef _G_VRML1
int GvUnknownNode::Do(GDisplayTreeTraversal &state) 
{
   return GvNode::Do(state);		
}
#endif _G_VRML1


int GvSFNode::Do(GDisplayTreeTraversal &state) 
{
   	if (value != NULL) {
	    int ret;
   		ret = value->Traverse(state);
   		return(ret);
   } 
   return(0) ; 
}

int GvMFNode::Do(GDisplayTreeTraversal &state) 
{
  int ret=0;

/*
	to do: store indices, instead of node pointer to get child position

*/
		int curIndex=0;
		while (curIndex < Length()) {
            GvNode *n = get1(curIndex);
			if (n) ret = n->Traverse(state);
			curIndex ++;
		}
	return ret;
}

int GvMField::Do(GDisplayTreeTraversal &state) 
{
  int ret=0;
  // insert all elements
/*
		int curIndex=0;
		while (curIndex < Length()) {
			ret = Get(curIndex)->Traverse(state);
			curIndex ++;
		}
*/
	return ret;
}

int GvProto::Do(GDisplayTreeTraversal &state) 
{

   int traverse;
   int ret=0;
   switch (state.mode) {

   case GDisplayTreeTraversal::MODE_TREE:
   default: 
		{
#ifndef  _NO_DISPLAY_TREE
   		HTREEITEM thisItem = state.InsertNodeItem(this,traverse, DGvTreeItem::GV_GROUP_NODE);

   		if (traverse) {
   			TREE_PUSH(state,thisItem);
   				state.topInsertAfter = TVI_LAST;
   				ret=children.Traverse(state);
   				ret=routes.Traverse(state);
   			TREE_POP(state);
   		}  
   		state.returnItem = thisItem;
#endif
		}
   		break;
   }
   return(ret);
}

int GvExternProto::Do(GDisplayTreeTraversal &state) 
{

   int traverse;
   int ret=0;
   switch (state.mode) {

   case GDisplayTreeTraversal::MODE_TREE:
   default: 
		{
#ifndef  _NO_DISPLAY_TREE
   		HTREEITEM thisItem = state.InsertNodeItem(this,traverse, DGvTreeItem::GV_GROUP_NODE);

   		if (traverse) {
   			TREE_PUSH(state,thisItem);
   				state.topInsertAfter = TVI_LAST;
   				// ?? url.Do
   				//ret=proto.Traverse(state);
   			TREE_POP(state);
   		}  
   		state.returnItem = thisItem;
#endif
		}
   		break;
   }
   return(ret);
}

int GvRoute:: Do(GDisplayTreeTraversal &state)
{	int ret = 0;

    {
        CString buf;
        buf = GvKeyword::route; 
		buf += ' ';
      	buf += from.value->getName(); // to do name nodes 
        buf += '.';
      	buf += getFromEventName();
        buf += ' ';
        buf += GvKeyword::to; 
     	
        buf += ' ';
        buf += to.value->getName();
        buf += '.';
        buf +=getToEventName();
#ifndef  _NO_DISPLAY_TREE
    	HTREEITEM thisItem = state.treeEditor->AddItem(state.top,buf,this,state.topInsertAfter,0,DGvTreeItem::GV_NODE);
   		state.returnItem = thisItem;
#endif
        ret = 1;
        
    }
	return(ret);
}




//
// class GDisplayTreeTraversal
//
RTIMP(GDisplayTreeTraversal,0,GGeometryTraversal);


GDisplayTreeTraversal::GDisplayTreeTraversal(GvDict *dict) : GGeometryTraversal() 
{  
   displayFields = 0;
   displayChilds = 0;
   expandInlines = 1;
   treeEditor = NULL;
   
   mode = MODE_TREE;
   
#if defined(_WIN32)

   top = TVI_ROOT;
   topInsertAfter = TVI_LAST;

#endif

   returnItem = NULL;

   traverseAll= 1;
   if (dict) {
      refDict = dict;
	  refDictByRef=1;
   } else {
     refDict = new GvDict();
	 refDictByRef=0;
   }

}


void GDisplayTreeTraversal::InitializeAttributes()
{	
 	GGeometryTraversal::InitializeAttributes();
}

GDisplayTreeTraversal::~GDisplayTreeTraversal()
{
 	if (refDict  && ! refDictByRef) delete refDict;
	refDict = 0;

}

