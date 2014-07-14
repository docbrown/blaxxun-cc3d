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
// GDisplayTree.h
#ifndef _GDisplayTree_H
#define _GDisplayTree_H

class GvNameSpace;

class DTreeEditor;

#define TREE_PUSH(state,newItem) \
{ \
	HTREEITEM savedTopItem = state.top; \
	state.top = newItem;

#define TREE_POP(state) \
	state.top = savedTopItem; \
} 

//
//	GDisplayTreeTraversal
//
// traverse graph to do selection for all geometry nodes
class GDisplayTreeTraversal : public GGeometryTraversal 
{
public :
	RT(GDisplayTreeTraversal);

	GDisplayTreeTraversal(GvDict *dict = NULL);
	~GDisplayTreeTraversal();
	
	DTreeEditor *treeEditor;

	enum Mode {
		MODE_TREE,		// display tree
		MODE_DEF,       // display DEF 
		MODE_EXTERN_REF	// display nodes with extern refs 
        //MODE_PROTO    // display PROTO and EXTERN PROTOS
	};

	Mode	mode;

	
	HTREEITEM top;			// (current) top item top insert childs, default TVI_ROOT
	HTREEITEM topInsertAfter;  // where to insert new nodes in top, default TVI_LAST


	HTREEITEM returnItem;  // top level item return 

	int refDictByRef;	// dict passed by reference;
	GvDict		*refDict;	// NAME HtreeItem 
    
    //GvNameSpace *nameSpace;

	
	int displayFields;
	int displayChilds;
	
	int expandInlines;
	
	int Do(GNode *n) {	return (n->Do(*this)); }
	
   	void InitializeAttributes(); 


	HTREEITEM InsertNodeItem(GvNode *node, 	int &traverseChildren, int type);

	HTREEITEM InsertFieldItem(GvField *field);

};



#endif

