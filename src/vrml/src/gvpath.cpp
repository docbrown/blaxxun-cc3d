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
#include <gvpath.h>

#include <gv2group.h>
#include <gvgroup.h>



void GvPath::Flush()
{
	int l= path.Length();
	for(int i=0; i<l; i++) {
		path[i].Flush();
	}
	path.SetLength(0);
}


// truncate to certain length 
void GvPath::Truncate(int newLength)
{
	int l= path.Length();
	if ( newLength<l) l = newLength;
	if (l<0) l = 0;
	for(int i=0; i<l; i++) {
		path[i].Flush();
	}
	path.SetLength(l);
}

void GvPath::Append(GPathElement *p)
{
  int l= path.Length();
  path.SetLength(l+1);
  path[l].Set(*p); 

}

void GvPath::Append(GvPathElement &p)
{
  int l= path.Length();
  path.SetLength(l+1);
  path[l].Set(p); 
}


// insert a transform node at index, move childs to children 
BOOL GvPath::InsertTransform(int index)
{
	if (index<=0) return(FALSE);
	BOOL ret = FALSE;

	Gv2Transform *n= new Gv2Transform();
	n->addChild(path[index].node);

	if (RTISA(path[index-1].field,GvSFNode)) {
		GvSFNode *f = (GvSFNode*) path[index-1].field;
		f->set(n);
		ret= TRUE;
	}
	else if (RTISA(path[index-1].field,GvMFNode)) {
		GvMFNode *f = (GvMFNode*) path[index-1].field;
		f->set1(path[index-1].index,n);
		// onCHanged ?? 
		ret= TRUE;

	}
	if (ret) {
		
		ArrayNew<GvPathElement> pe;
		GvPathElement e(n,&n->children,0);
		pe.Append(e);
		path.Insert(index,pe);
	}
	else delete n;

	return(ret);



}




void GvPath::SetByList(GPathElement* p)
{
	Flush();
	SetByList_(p);
}



// set to reversed path p 
void GvPath::SetByList_(GPathElement* p)
{
	
	if (!p) return;
	if (p->next) SetByList_(p->next); // recursive 
	ASSERT(p->node != NULL);
	if (RTISA(p->node,GvNode)) {
		Append(p);
	} else {
	// not interested in any toplevel GNode left
	}
}

// get effective transformation matrix up to and including path element lastIndex
// to do: context/camera dep. nodes e.g. billboard ????????????????
BOOL GvPath::getMatrix(Matrix &m,int lastIndex)
{
	BOOL ret = FALSE;

	m.Identity();

	for(int i=0; i<=lastIndex; i++) {
		Matrix m1;
		GvNode *node = path[i].node;
		if (node->getMatrix(m1)) {
			//m = m1 * m; 
			m.MultiplyLeft(m1);
			ret = TRUE;
		} else {
			//TRACE(" %s= %s ",node->getName().getString(),node->ClassName());
			if (path[i].index >=0) {
#ifdef _G_VRML1
				if (RTISA(node,GvGroup)) { // VRML 1.0 
					// get matrix of all direct childnodes before
					// assuming GvScale, GvTranslate nodes etc.
					GvGroup *g=(GvGroup *) node;
				
					if (g->getChildrenMatrix(m1,path[i].index)) {
						// m = m1 * m;
						m.MultiplyLeft(m1);
						ret = TRUE;
					}
				}
#endif _G_VRML1


			}
		}

	}
	return(ret);
}

// get effective transformation matrix for last element of path
BOOL GvPath::getMatrix(Matrix &m)
{
	return getMatrix(m,path.Length()-1);
}


void GvPath::Trace()
{
	int l= path.Length();
	TRACE("(");
	for(int i=0; i<l; i++) {

		TRACE(" %s= %s ",path[i].node->getName().getString(),path[i].node->ClassName());
		if (path[i].index >=0) {
			TRACE(" %d ",path[i].index);

		}

	}
	TRACE(")\n");
}


