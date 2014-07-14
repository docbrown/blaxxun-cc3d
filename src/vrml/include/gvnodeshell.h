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
#ifndef  _GV_NODE_SHELL_
#define  _GV_NODE_SHELL_

class GShell;


#include <gvmflong.h>
#include <gvsubnode.h>
#include <gvmfnode.h>

class GvNodeShell : public GvNode {

    GV_NODE_HEADER(GvNodeShell);

  public:
    // Fields:

	// get the bounding box 
	GvBool getBBox(BBox &bbox);

	// is p outside the object
	// may fall back to very simply outside bbox computation
	virtual GvBool isOutside(const Point &p) const;

	// is the a planar object ?
	virtual GvBool isPlanar() const;

	// is this a solid  i.e. closed ?
	virtual GvBool isSolid() const;

	// is this a convex object ?
	virtual GvBool isConvex() const;


  	// do by generic traversal on constructed theShell
  	int Do(GTraversal &state);
  	int Do(GBBoxTraversal &state);

  	int Do(GglRenderTraversal &state);
  	int Do(GglTransparencyRenderTraversal &state);
  	int Do(GBuildShellTraversal &state);
  	int Do(GRaySelectTraversal &state);
    int Do(GOptimizeTraversal &state);		// implemented in GOptimizeTraversal.cpp

	//@cmember GClass io Traversal
	int Do(GioTraversal &state); 

  
  	GShell *theShell;  // build shell for rendering
	GvMFNode theShapes; // expanded list of shapes with separated Appearances
	
  	// build shell for current state 
  	virtual int BuildShell(GTraversal &state);  
	
	void SetShell(GShell *newShell);
	GShell * GetShell() { return(theShell);}

	int OnFieldChanged(GvField *field);

	// Evaluate object as object of type "type"
	// if return = this, don´t destroy result, otherwise destroy
	GvNode *EvalAs(TypeId type,int copyAttributes); 



};

#endif /* _GV_NODE_SHELL_ */
