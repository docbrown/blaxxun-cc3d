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
#ifndef _GSelection_h
#define _GSelection_h
/******************************************************************************
@doc

@module GSelection.h - GLView selection classes|

Copyright (c) 1996	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GSelection>

Notes:

Changes:

Todo :

******************************************************************************/

#include "quaternation.h"


class GView;

class GvGroup;

//
// GSelectionHandler 
// base class for EventHandlers
//

class GSelection : public RTRoot {
public:
	RT(GSelectionHandler);

	// the current state of eventhandler 
	int refCnt;
	void ref() { refCnt ++; }
	void unref() { refCnt--; if (refCnt<=0) delete this; }


	// the view this selection  is attached to
	GView *view;

	GSelection(GView *View =NULL) : view(View) { refCnt = 0; state = 0; }
	virtual ~GSelection();

	// get reference to camera in effect for view 
	GCamera &GetCamera() { return view->GetGlobalCamera(); }


};


//
// GPathSelection 
// handler maintain a path to a scene node 
//


class GPathSelection : public GSelection {
public:
	RT(GPathSelection);

	GPathSelection(GView *View =NULL) : GSelectionHandler(View) {  }
	~GPathSelection();

	// transform from world to local 
	MatrixAndInverse m;

	// path to node affected by handler 
	GvPath path;

	// hit information from ComputeRayHit
	GHitInfo lastHit;

	void World2Local(const Point &world, Point &local) {  local = m.Inverse() * world; }
	void Local2World(const Point &local, Point &world) {  world = m.M() * local; }

	// Compute hit ray with scene and initialize path
	BOOL Set(RayRange  &ray);

	// set by selected node
	BOOL SetGvNode *node);
	
	BOOL Set(GvPath &newPath);

	// points to VRML 2.0 transform node
	GvNodeHandle transform;

	// get the transform Node 
	Gv2Transform * GetTransform() const { return (Gv2Transform*) (GvNode*) transform; }

	// set pointer to field in node to be transformed 
	// in VRML 1.0 search a transformable childNode up to index
	BOOL SetTransformField(TransformMode mode,GvGroup *group,int upToChild);

	BOOL SetTransformField(TransformMode mode);
	GvField * GetTransformField() { return(transformField); }

	// translate translation field of  node by delta 
	BOOL Translate(const Point &delta);
	// scale scale field of Transform node by delta 
	BOOL Scale(const Point &delta);
	// rotate rotate field of node by delta 
	BOOL Rotate(const Quaternation &delta);


	// initialize path to innermost Transform node of path 
	BOOL InitTransformPath();

	// Search next higher Transform in path 
	BOOL InitTransfromPathUp();

	// Search next lower Transform in path 
	BOOL InitTransformPathDown();
	// Shape
	// Texture
	// Material

	BOOL Cut();
	BOOL Copy();
	BOOL Paste();

};



#endif




