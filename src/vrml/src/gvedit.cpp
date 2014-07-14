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

@module GvEdit.cpp - Implementation of Edit and Undo Operators|

Copyright (c) 1996	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:


Changes:

$Revision: 1.8 $
$Log: gvedit.cpp,v $
Revision 1.8  1999/07/06 16:45:39  tom
*** empty log message ***



Todo :

******************************************************************************/


#include <gvnode.h>

#include <gvsfnode.h>
#include <gvmfnode.h>

// support for older VRML 1.0 Group nodes
#include <gvgroup.h>

#include <gvedit.h>


class GvUndoNode : public GvUndoActionField
{
public:
	GvUndoNode(GvNode *theNode,GvField *theField,int theIndex) :
		GvUndoActionField(theNode,theField), index(theIndex) {}

	int index;	// MFNode index value 

};


class GvUndoNodeDeleteNode: public GvUndoNode
{
public:
	GvUndoNodeDeleteNode(GvNode *theNode,GvField *theField,int theIndex)
	   : GvUndoNode(theNode,theField,theIndex) {}

//	GvBool Do() {  return( ((GvMFNode*) field)->deleteElement(index));}

    GvBool Do() {  return DeleteNode(node,field,index);;}

    GvUndoAction* NewRedoAction(); 

};

class GvUndoNodeInsertNode : public GvUndoNode
{
public:
	
    GvNodeHandle newNode;

    GvUndoNodeInsertNode(GvNode *theNode,GvField *theField,int theIndex,GvNode *theNewNode)
	   : GvUndoNode(theNode,theField,theIndex), newNode(theNewNode) {}

    GvBool Do() {  return InsertNode(node,field,index,newNode);}

    GvUndoAction* NewRedoAction() 
    {
        return new GvUndoNodeDeleteNode(node,field,index);
    }

};

class GvUndoMoveNode: public GvUndoNode
{
public:
    GvNodeHandle destNode;
    GvField *destField;
    int destIndex;

	GvUndoMoveNode(GvNode *theNode,GvField *theField,int theIndex,
        GvNode *theDestNode,GvField *theDestField,int theDestIndex)
	   : GvUndoNode(theNode,theField,theIndex), 
        destNode(theDestNode),destField(theDestField),destIndex(theDestIndex){}

	GvBool Do();

    GvUndoAction* NewRedoAction(); 

};


// return node of MFNode or SFNode field at index srcIndex
// Result : NULL on problem
GvNode* GetNode(GvNode *srcNode,GvField *srcField,int srcIndex)
{

    if (RTISA(srcField,GvMFNode)) {
        return ((GvMFNode*) srcField)->get1(srcIndex);
    }
    else if (RTISA(srcField,GvSFNode)) {
        return ((GvSFNode*) srcField)->value;
    }
#ifdef _G_VRML1
    else if (RTISA(srcNode,GvGroup)) {
        GvGroup *group = (GvGroup *) srcNode;
		return group->getChild(srcIndex);
    }
#endif _G_VRML1

    else return NULL;
}

// delete node at MFNode or SFNode at index srcIndex
// true: if successfull

GvBool DeleteNode(GvNode *srcNode,GvField *srcField,int srcIndex)
{

    if (RTISA(srcField,GvMFNode)) {
        return ((GvMFNode*) srcField)->deleteElement(srcIndex);
    }
    else if (RTISA(srcField,GvSFNode)) {
        if (srcIndex >0) return FALSE;
        ((GvSFNode*) srcField)->value = NULL;
        return(TRUE);
    }

#ifdef _G_VRML1
    else if (RTISA(srcNode,GvGroup)) {
        GvGroup *group = (GvGroup *) srcNode;
		return(group->deleteChild(srcIndex)>=0);
    }
#endif _G_VRML1
    else return NULL;

}

// delete node, but return reference to node in element

GvBool DeleteNode(GvNode *srcNode,GvField *srcField,int srcIndex,
        GvNodeHandle &element)
{

    if (RTISA(srcField,GvMFNode)) {
        element = ((GvMFNode*) srcField)->get1(srcIndex);
        ((GvMFNode*) srcField)->deleteElement(srcIndex);
    }
    else if (RTISA(srcField,GvSFNode)) {
        element = ((GvSFNode*) srcField)->value;
        ((GvSFNode*) srcField)->value = NULL;
    }
#ifdef _G_VRML1
    else if (RTISA(srcNode,GvGroup)) {
        GvGroup *group = (GvGroup *) srcNode;
		element = group->getChild(srcIndex);
		return(group->deleteChild(srcIndex)>=0);
    }
#endif _G_VRML1

    else return FALSE;
    return(TRUE);

}


// insert node at MFNode or SFNode at index destIndex
// true: if successfull
GvBool InsertNode(GvNode *destNode,GvField *destField,int destIndex,GvNode *node)
{
    
    if (RTISA(destField,GvMFNode)) {
        return ((GvMFNode*) destField)->insert(destIndex,node);
    }
    else if (RTISA(destField,GvSFNode)) {
        if (destIndex >0) return FALSE;
        ((GvSFNode*) destField)->value = node;
        return(TRUE);

    }

#ifdef _G_VRML1
    else if (RTISA(destNode,GvGroup)) {
        GvGroup *group = (GvGroup *) destNode;
		return group->insertChild(destIndex,node)>=0;
    }
#endif _G_VRML1

    else return FALSE;

}



GvBool MoveNode(GvNode *srcNode,GvField *srcField,int srcIndex,
        GvNode *destNode,GvField *destField,int destIndex)
{
    GvNodeHandle tmp;

    if (!DeleteNode(srcNode,srcField,srcIndex,tmp)) return FALSE;
    return InsertNode(destNode,destField,destIndex,tmp);
    
}



GvBool GvUndoMoveNode::Do() 
{
    return(MoveNode(node,field,index,destNode,destField,destIndex));

}

GvUndoAction* GvUndoMoveNode::NewRedoAction()
{
    return new GvUndoMoveNode(destNode,destField,destIndex,node,field,index);
}





GvUndoAction* GvUndoNodeDeleteNode::NewRedoAction() 
{
   return new GvUndoNodeInsertNode(node,field,index,((GvMFNode*) field)->get1(index));
}


GvBool GvEditor::EditField(GvNode* node, GvField*field, const GvString &newValueS)
{
    GvField *oldValue = field->copy(); // create copy of old value
    field->set(newValueS); // assign new value
    Push(new GvUndoActionSetField(node,field,oldValue));
    return (TRUE);
}


// delete node at MFNode or SFNode at index srcIndex
// true: if successfull
GvBool GvEditor::DeleteNode(GvNode *destNode,GvField *destField,int destIndex)
{
    GvNodeHandle element;
    if (!::DeleteNode(destNode,destField,destIndex,element)) 
    {
        // could not delete, bad field types
        return FALSE;
    }
    Push(new GvUndoNodeInsertNode(destNode,destField,destIndex,element));
    return (TRUE);

}

// delete node at MFNode or SFNode at index srcIndex
// true: if successfull
GvBool GvEditor::Cut(GvNode *destNode,GvField *destField,int destIndex)
{
    if (!::DeleteNode(destNode,destField,destIndex,copyBuffer)) 
    {
        // could not delete, bad field types
        return FALSE;
    }
    Push(new GvUndoNodeInsertNode(destNode,destField,destIndex,copyBuffer));
    return (TRUE);
}



// insert node at MFNode or SFNode at index destIndex
// true: if successfull
GvBool GvEditor::InsertNode(GvNode *destNode,GvField *destField,int destIndex,GvNode *node)
{
    
    if (!InsertNode(destNode,destField,destIndex,node)) 
    {
        // could not move, bad field types
        return FALSE;
    }
    Push(new GvUndoNodeDeleteNode(destNode,destField,destIndex));
    return (TRUE);

}

GvBool GvEditor::Paste(GvNode *destNode,GvField *destField,int destIndex)
{
    
    if (!InsertNode(destNode,destField,destIndex,copyBuffer)) 
    {
        return FALSE;
    }
    Push(new GvUndoNodeDeleteNode(destNode,destField,destIndex));
    return (TRUE);
}



GvBool GvEditor::MoveNode(GvNode *srcNode,GvField *srcField,int srcIndex,
        GvNode *destNode,GvField *destField,int destIndex)
{
    
    if (!MoveNode(srcNode,srcField,srcIndex,destNode,destField,destIndex)) 
    {
        // could not move, bad field types
        return FALSE;
    }

    Push(new GvUndoMoveNode(destNode,destField,destIndex,srcNode,srcField,srcIndex));
    return (TRUE);

}


// pop action from undo stack and do action
// Return FALSE, stack empty or error on operation

GvBool GvEditor::Undo()
{
    // get top undo action
    GvUndoAction *action = Pop();
    if (!action) return(FALSE);

    // push corrensponding undo of undo on redo stack
    PushRedo(action->NewRedoAction());
    
    // do the undo
    GvBool ret = action->Do();
    
    delete action;

    return(ret);
}

GvBool GvEditor::Redo()
{
    GvUndoAction *action = PopRedo();
    if (!action) return(FALSE);
    Push(action->NewRedoAction());
    GvBool ret = action->Do();
    delete action;
    return(ret);
}

void GvEditor::setFieldBuffer(GvField *field)
{
    if(copyField) delete copyField; 
    if (field) copyField = field->copy(); 
    else copyField = NULL;
}

// copy the copyField value to field
GvBool GvEditor::Paste(GvField *field)
{
   if (!copyField) return FALSE;
   if (!field) return FALSE;

//   if (!RTISA(field,copyField->Id()) return FALSE
    GvField *oldValue = field->copy(); // create copy of old value
    Push(new GvUndoActionSetField(field->getContainer(),field,oldValue));
    return field->set(copyField);
   // copy default bit  ???
}

