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
#ifndef  _GVPath_
#define  _GVPath_

class GvField;


#include <gvnode.h>


/*

  a path stores a comple path to an selected node


*/


#include <arraynew.h>



class GvPathElement 
{
public:
	GvNodeHandle node;		// pointer to node
	GvField *field;			// pointer to field in node if applicable
	int		index;			// -1 of MField Index of GvGroup childIndex if field == NULL
	void	*data;

	void Flush() { field = NULL; index = -1; node = (GvNode*) NULL; }

public:
	GvPathElement(GvNode *Node=NULL,GvField *Field_=NULL,int Index=-1,void *Data=NULL): 
		node(Node), field(Field_),index(Index),data(Data) { }
	
	GvPathElement(GvPathElement &p) { Set(p.node,p.field,p.index,p.data); }
	GvPathElement(GPathElement &p) { Set((GvNode*)p.node,p.field,p.index,p.data); }
	~GvPathElement() { }

	void Set(GPathElement &p) { node = (GvNode*) p.node, field = p.field, index = p.index,data = p.data;}
	void Set(GvPathElement &p) { Set(p.node,p.field,p.index,p.data); }
	void Set(GvNode *Node,GvField *Field=NULL,int Index=-1,void *Data=NULL)  
	{ node = Node, field = Field, index = Index,data = Data;}



};


class GvPath {
public:
	ArrayNew<GvPathElement> path;

	// set path by current traversal path on stack
	void SetByList(GPathElement* p);

protected:
	void SetByList_(GPathElement* p);

public:
	// Append new elements to path 
	void Append(GPathElement *p);

	void Append(GvPathElement &p);

	BOOL isEmpty() const  { return path.Length() == 0; }
	int  Length() const { return path.Length(); }

	// flush all elements and truncate to zero
	void Flush();

	// truncate to certain length 
	void Truncate(int newLength);


	// get effective transformation matrix for last element of path
	BOOL getMatrix(Matrix &m);

	// get effective transformation matrix up to and including path element lastIndex
	BOOL getMatrix(Matrix &m,int lastIndex);

	// Modification


	// insert a transform node at index, move childs to children 
	BOOL InsertTransform(int index);



	// dump debug trace of path
	void Trace();

};






#endif /* _GVPath_ */
