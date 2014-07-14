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

#include "gcamera.h"
#include "grayselect.h"


#include <gvlod.h>
#include <gvwwwinline.h>

#ifdef _G_VRML1

GV_NODE_SOURCE_IMP(GvLOD,GvGroup);

GvLOD::GvLOD()
{
    GV_NODE_CONSTRUCTOR(GvLOD);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(range);
    GV_NODE_ADD_FIELD(center);

    range.values[0] = 0;
    center.value[0] = center.value[1] =center.value[2] = 0;
    
    lastRenderedChild = 0;

}

GvLOD::~GvLOD()
{
}

int GvLOD::Do(GBBoxTraversal &state)
{
	// union of all levels ! 
	return(TraverseChildren(state)); 
}

#endif _G_VRML1

//
// Gv2LOD
//

GV_NODE_SOURCE_IMP(Gv2LOD,GvNode);

Gv2LOD::Gv2LOD() : level(0),range(0)
{
    GV_NODE_CONSTRUCTOR(Gv2Inline);
    isBuiltIn = TRUE;

	GV_NODE_ADD_FIELD(level);
	GV_NODE_ADD_FIELD(center);
	GV_NODE_ADD_FIELD(range);

    center.value[0] = center.value[1] =center.value[2] = 0;


}

Gv2LOD::~Gv2LOD()
{
}

// copy on proto instancing
GvBool Gv2LOD::computeCopyOnInstance(GvCopyContext &context)
{
	setCopyOnInstance(TRUE); // mmh really ??
	return GvNode::computeCopyOnInstance(context);
}

//eval node to the actual value node of node,
GvNode* Gv2LOD::Eval(GTraversal &state)
{
    int which = lastRenderedChild;
  
    if (which <0) {
        return(NULL); 
    }
	else if (which < level.Length())
	    return level.values[which];
    return(NULL);
}




int Gv2LOD:: Do(GTraversal &state)
{

	if (state.traverseAll) { return(level.Traverse(state)); }

    int nChilds = level.Length();
	if (nChilds == 0) return(0);
	
	// traverse first 
	GvNode *child = level[0];

	if (!child) return 0; 

	return(child->Traverse(state));
}


int Gv2LOD::Do(GglRenderTraversal &state)
{
    int nChilds = level.Length();

	if (nChilds == 0) return(0);

	int i=0;

	// highest to lowest 

	if (range.Length() == 0) { // TBD: Performance LOD 
		i= 0; 
		const Matrix &m= *state.GetMatrix();


		const Point &eye = state.camera->position;
		Point pos = center.value;
	
		pos *= m; // transform center to global
	
		Point d= pos-eye;
		float dist = d.Len();

		if (state.doLodScale)
			dist *= (1.0f/state.lodScale); 

		float f = dist/state.camera->zfar; // proportional to view distance
		f=exp(f); // not just linear 

		f*=nChilds-1; // scale to num childs
		
		i = (int) f;

		TRACE("dyna lod, choosing level %d \n",i);

	
	} else	{
		const Matrix &m=state.GetMatrixInverse();


		Point eye = state.camera->position;
		const Point pos = center.value;
	
		eye *= m;
	
		Point d= pos-eye;
		float dist = d.Len();
		if (state.doLodScale)
			dist *= (1.0f/state.lodScale)*m.ScaleFactor(); // must be transformed to local 

		for(i=0; i<range.Length(); i++) {
			if (dist <= range.values[i]) {
				break;
			}
		}

	}
    
    if (i>= nChilds)  i = nChilds-1;

    GvNode * child;
	int ret;

	// if child is busy loading, try previous LOD level 
	do {

		child = level[i];
		
		if (!child) break; // ! NULL

        lastRenderedChild = i; // the child that was rendered the last time 
		
		child = child->Eval(state); // proto instance ?
		if (!child) break; // ! NULL

		if (RTISA(child,Gv2Inline)) {
			ret = child->Traverse(state);
			if (ret<0) {   // Inline is busy
			  i++;      // try next one
			}
			else break;
		} else {
			ret = child->Traverse(state);
			break;
		}	

	} while (i<nChilds);

	return (ret);
}


int Gv2LOD::Do(GRaySelectTraversal &state)
{

	if (state.traverseAll) { return(Do( *((GPathTraversal *) &state)));}

    
	int nChilds = level.Length();

    if (nChilds == 0) return(0);

	// 
	// need to reevaluate LOD, because of DEF / USE 
	//
	int i=0;

	// highest to lowest 

	if (range.Length() == 0) { // TBD: Performance LOD 
		i= 0; 
	} else 
	if (state.camera)
	{
		const Matrix &m=state.GetMatrixInverse();


		Point eye = state.camera->position;
		Point pos = center.value;
	
		eye *= m;
	
		Point d= pos-eye;
		float dist = d.Len();
		if (state.doLodScale)
			dist *= (1.0f/state.lodScale)*m.ScaleFactor(); // must be transformed to local 

		for(i=0; i<range.Length(); i++) {
			if (dist <= range.values[i]) {
				break;
			}
		}

	}
	else 
		i= lastRenderedChild;
    
    if (i>= nChilds)  i = nChilds-1;


	if (i<0) return(0);

    GvNode *child;
	int ret;


	GPUSH_PATH(state,this);

		GSET_PATH_FIELD(state,level);

		// if child is busy loading, try previous LOD level 
		do {

			child = level[i];
			if (!child) break; // ! NULL

			GSET_PATH_INDEX(state,i);
			
			// lastRenderedChild = i; // the child that was rendered the last time 
			
			child = child->Eval(state); // proto instance ?
			if (!child) break; // ! NULL

			if (RTISA(child,Gv2Inline)) {
				ret = child->Traverse(state);
				if (ret<0) {  // Inline is busy
					i++;      // try next one
				}
				else break;
			} else {
				ret = child->Traverse(state);
				break;
			}	

		} while (i<nChilds);

	GPOP_PATH(state);

	return(ret);

}


int Gv2LOD::Do(GBBoxTraversal &state)
{
	// union of all levels ! 
	return(level.Traverse(state)); 

	// Idea: we could store bboxes of all level elements
	// in order to make an area projected auto LOD 
}

int Gv2LOD::OnChanged()
{
	setRedraw();
	return GvNode::OnChanged();
}

