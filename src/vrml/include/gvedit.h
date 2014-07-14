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
#ifndef __GvEdit_h_
#define __GvEdit_h_

class GvNode;
class GvSFNode;
class GvMFNode;
class GvField;



// low level edit operations
// operation on SFNode/MFNode fields


// return node of MFNode or SFNode field at index srcIndex
// Result : NULL on problem
GvNode* GetNode(GvNode *srcNode,GvField *srcField,int srcIndex);

// delete node at MFNode or SFNode at index srcIndex
// true: if successfull
GvBool DeleteNode(GvNode *srcNode,GvField *srcField,int srcIndex);

// delete node, but return reference to node in element
GvBool DeleteNode(GvNode *srcNode,GvField *srcField,int srcIndex,GvNodeHandle &element);


// insert node at MFNode or SFNode at index destIndex
// true: if successfull
GvBool InsertNode(GvNode *destNode,GvField *destField,int destIndex,GvNode *node);

GvBool MoveNode(GvNode *srcNode,GvField *srcField,int srcIndex,
        GvNode *destNode,GvField *destField,int destIndex);




class GvUndoAction
{
public:
    GvUndoAction *next; // pointer to next to from stack

    GvNodeHandle node;
	GvUndoAction(GvNode *theNode) : node(theNode),next(NULL) { };

    // apply the undo operation
	virtual GvBool Do() = 0;

    // create the corresonding Redo action 
    virtual GvUndoAction* NewRedoAction() = 0; 

	virtual ~GvUndoAction() {};
};

class GvUndoActionField : public GvUndoAction
{
public:
	GvUndoActionField(GvNode *theNode,GvField *theField) :
		GvUndoAction(theNode), field(theField) {}


	GvField *field; // pointer to field into node
};

class GvUndoActionSetField : public GvUndoActionField
{
public:

    GvField *newValue; // allocated field value, same type as field

    GvUndoActionSetField(GvNode *theNode,GvField *theField,GvField *theNewValue) :
		GvUndoActionField(theNode,theField), newValue(theNewValue) {}
	~GvUndoActionSetField() { delete newValue; }


	GvBool Do() { field->set(newValue); return(TRUE);}

    GvUndoAction* NewRedoAction() 
    {
        return new GvUndoActionSetField(node,field,field->copy());
    }
};

//
// GvEdtitor
// support certain edit operations and also
// supports an undo / redo stack
// 

class GvEditor 
{
public:


    GvUndoAction *undo;
    GvUndoAction *redo;

    GvEditor(): undo(NULL),redo(NULL),copyField(NULL) {}
    ~GvEditor() { FlushBuffers(); setFieldBuffer(NULL); }


    //
    // editor operations supporting undo and redo
    //


    GvBool EditField(GvNode* node, GvField*field, const GvString &newValue);

    // delete node at MFNode or SFNode at index srcIndex
    // true: if successfull
    GvBool DeleteNode(GvNode *srcNode,GvField *srcField,int srcIndex);

    // insert node at MFNode,SFNode or Group at index destIndex
    // true: if successfull
    GvBool InsertNode(GvNode *destNode,GvField *destField,int destIndex,GvNode *node);


    GvBool MoveNode(GvNode *srcNode,GvField *srcField,int srcIndex,
        GvNode *theDestNode,GvField *theDestField,int theDestIndex);



	// Node Buffer for cut, copy, paste
	GvNodeHandle  copyBuffer;
    GvField *copyField; // set by value 
    
    void Copy(GvNode *node) { copyBuffer = node; }
    //void CopyDeep(GvNode *node) { copyBuffer = node->copy(); }

    // delete node at MFNode or SFNode at index srcIndex
    // true: if successfull
    GvBool Cut(GvNode *srcNode,GvField *srcField,int srcIndex);

    // insert copyBuffer at MFNode,SFNode or Group at index destIndex
    // true: if successfull
    GvBool Paste(GvNode *destNode,GvField *destField,int destIndex);



    // Field value  Buffer for cut, copy, paste
    void setFieldBuffer(GvField *field); 

    // copy the field value to copyField
    void Copy(GvField *field) { setFieldBuffer(field); }
    


    // copy the copyField value to field
    GvBool Paste(GvField *field);

    // cut the copyField value to field
    GvBool Cut(GvField *field) {return(FALSE); }


    // flush undo / redo buffer 
    void FlushBuffers();

    GvBool hasUndo() {return(undo != NULL); }
    GvBool hasRedo() {return(redo != NULL); }

    void Push(GvUndoAction *newTop) 
    {
        ASSERT(newTop);
        newTop->next = undo;
        undo = newTop;
    }
    
    void PushRedo(GvUndoAction *newTop) 
    {
        ASSERT(newTop);
        newTop->next = redo;
        redo = newTop;
    }

    GvUndoAction *Pop() {
        if (!hasUndo()) return(NULL);
        GvUndoAction* ret = undo;
        undo = undo->next;
        ret->next = NULL;
        return ret;
    }

    GvUndoAction *PopRedo() {
        if (!hasRedo()) return(NULL);
        GvUndoAction* ret = redo;
        redo = redo->next;
        ret->next = NULL;
        return ret;
    }

    GvBool Undo();
    GvBool Redo();

};


#endif //  __GvEdit_h_
