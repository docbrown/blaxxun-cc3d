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
#ifndef  _GV_LOD_
#define  _GV_LOD_

#include <gvmffloat.h>
#include <gvsfvec3f.h>

#include <gvmfnode.h>

#include <gvsubnode.h>

#ifdef _G_VRML1
#include <gvgroup.h>

/*! VRML 1.0 LOD Node */

class GvLOD : public GvGroup {

    GV_NODE_HEADER(GvLOD);

  public:
    // Fields
    GvMFFloat		range;	// distances 
    GvSFVec3f		center;	// center 

    int lastRenderedChild; // the child that was rendered the last time 

	int Do(GTraversal &state);
	int Do(GBBoxTraversal &state);

	int Do(GglRenderTraversal &state);

    // Problem, for what traversals evaluate LOD ??????????????
  	int Do(GRaySelectTraversal &state);


};

#endif _G_VRML1


class Gv2LOD : public GvNode {

    GV_NODE_HEADER(Gv2LOD);

public:
    // Fields :

    GvMFNode		level;          // []
    GvSFVec3f		center;         // 0 0 0
    GvMFFloat		range;         // []

    int lastRenderedChild; // the child that was rendered the last time 

    int Do(GTraversal &state);

	int Do(GBBoxTraversal &state);

    int Do(GglRenderTraversal &state);
	int Do(GPathTraversal &state);

    // Problem, for what traversals evaluate LOD ??
  	int Do(GRaySelectTraversal &state);

	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp


	int OnChanged();

	// copy on proto instancing
    GvBool computeCopyOnInstance(GvCopyContext &context);

	//eval node to the actual value node of node,
	// return last rendered child 
	GvNode* Eval(GTraversal &state);

   


};


#endif /* _GVLOD_ */
