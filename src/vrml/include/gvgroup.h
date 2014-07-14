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
#ifndef  _GV_GROUP_
#define  _GV_GROUP_


#ifdef _G_VRML1

#include <gvsubnode.h>

#include <gvlists.h>
class GvChildList;
#include <gvchildlist.h>


//
// GvGroup - VRML 1.0 Group node
// 

/*! \class GvGroup
 *  \brief VRML 1.0 Group node
 *   
 *  Group node, using a \a GvChildList member to store children
 */

class GvGroup : public GvNode {

    GV_NODE_HEADER(GvGroup);

  public:

#if 0
    GvNode *	getChild(int index) const;
    int			getNumChildren() const;
#else
    GvNode *	getChild(int index) const { return(*children)[index];}
    int			getNumChildren() const { return children->getLength();};
#endif

	// list of children nodes
    GvChildList	*children;  

	// get children list
    virtual GvChildList *getChildren() const;
    
    virtual GvBool	readInstance(GvInput *in);
    virtual GvBool	readChildren(GvInput *in);


	// add (append) a new child
	void addChild(GvNode *node);

	
	// delete child at index
	int deleteChild(int index);

	// delete all children
	int deleteChildren();
	
	// insert & ref child at index
	int insertChild(int index,GvNode *node);
	
	// insertChild(index, srcGroup[index])
	int moveChild(int index,GvNode *scrGroup, int srcIndex);
	
	// find a child by index
	GvNode *findChild(int index);

	// find a child by ptr, return index, -1  if not found
	int  findChild(GvNode * node);

	// find a child by name
	GvNode *findChild(const GvName &name);

	// find a child by type
	GvNode *findChild(TypeId type);

	//
	// copy()
	// copyChildrenTo(GvGroup	*dest)
	
	// GLView generic traversal
	// default : pass to Do void Traverse(GTraversal &state);
	virtual int TraverseChildren(GTraversal &state);
	virtual int TraverseChildren(GPathTraversal &state);
	virtual int TraverseChildrenIo(GioTraversal &state);

	// get the effective matrix up to child node #i (not included)
	virtual GvBool getChildrenMatrix(Matrix &m, int upToChild);



	int Do(GTraversal &state);   	
	int Do(GPathTraversal &state);   	

	int Do(GioTraversal &state);
	int Do(GDisplayTreeTraversal &state); // implemented in GDisplayTree.cpp
	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp


	// Evaluate object as object of type "type"
	// if return = this, don´t destroy result, otherwise destroy
	GvNode *EvalAs(TypeId type,int copyAttributes); 

};

#endif


#endif /* _GV_GROUP_ */

