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
/******************************************************************************
@doc

@module Gv2Group.cpp - GLView VRML 2.0 Group nodes implementation|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c Gv2Group>
<c Gv2Transform>
<c Gv2MatrixTransform>

Notes:

Changes:

$Revision: 1.31 $
$Log: gv2group.cpp,v $
Revision 1.31  1999/07/06 16:45:31  tom
*** empty log message ***

Revision 1.11  1998/07/06 17:53:52  holger
empty message



Todo :

******************************************************************************/

#include "stdafx.h"

#include <gv2group.h>

#include "gcamera.h"

#include "gvisibility.h"
// for light check
#include "gvdirectionallight.h"

// only needed for cullstats 
#include "gvbsp.h"


// for ApplyTouchSensor
#include <gvsensors.h>
#include <grayselect.h>

#include "grender.h"
#include "gmodel.h"


// perform check of bounding box againt transformed view pyramid
// Result : true : continue processing, false : box not visible 
// bbox treated not empty if bboxSize.x >0 


gbool GglRenderTraversal::CullCheck(GvSFVec3f &bboxCenter,GvSFVec3f &bboxSize)
{
if (bboxSize[0]>=0.0f) { 
	Point s = bboxSize * 0.5f; 
	BBox bbox(bboxCenter-s,bboxCenter+s);	

	// get the viepoint in local coordinates	
	GMatrixStack *mstack = GetCurrentMatrix();
	if (mstack->m.contents & ( Matrix::HAS_NEGATIVE_SCALE) )  {
	    doClipping = gtrue;

		return gfalse; // not working with neg scale 
	}

	

	if (!mstack->viewpointLocalOk) { // need to transform it 
		mstack->SetViewpointLocal(physicalCamera->position);	
	}  
	if (!mstack->viewVolumeLocalOk) {
		mstack->SetViewVolumeLocal(viewVolume); 
	}
#ifdef _DEBUG	
	GvBspTree::cullChecks++;  // debug 
#endif

	if (BoxViewVolumeOutside(bbox,mstack->viewpointLocal,mstack->viewVolumeLocal)) { 

#ifdef _DEBUG	
		GvBspTree::cullChecksSucceeded++;  // debug 
#endif
		return gfalse;
	} 
}	
  return gtrue;
}
gbool GglRenderTraversal::CullCheck(BBox &bbox)
{
	if (doViewCulling) { 

	// get the viepoint in local coordinates	
	GMatrixStack *mstack = GetCurrentMatrix();
	if (mstack->m.contents & ( Matrix::HAS_NEGATIVE_SCALE) )  {
	    doClipping = gtrue;
		return gfalse; // not working with neg scale 
	}

	

	if (!mstack->viewpointLocalOk) { // need to transform it 
		mstack->SetViewpointLocal(physicalCamera->position);	
	}  
	if (!mstack->viewVolumeLocalOk) {
		mstack->SetViewVolumeLocal(viewVolume); 
	}
#ifdef _DEBUG	
	GvBspTree::cullChecks++;  // debug 
#endif

	if (BoxViewVolumeOutside(bbox,mstack->viewpointLocal,mstack->viewVolumeLocal)) { 

#ifdef _DEBUG	
		GvBspTree::cullChecksSucceeded++;  // debug 
#endif
		return gfalse;
	} 
	}	
	return gtrue;
}


static inline gbool CULLCHECK(Gv2Group *me,GglRenderTraversal &state)
{
if (state.doViewCulling && !me->bboxIsEmpty())	{ 
	Point s = me->bboxSize * 0.5f; 
	BBox bbox(me->bboxCenter-s,me->bboxCenter+s);	

	// get the viepoint in local coordinates	
	GMatrixStack *mstack = state.GetCurrentMatrix();	
	if (mstack->m.contents & ( Matrix::HAS_NEGATIVE_SCALE) ) 
		return gtrue; // not working with neg scale 

	if (!mstack->viewpointLocalOk) { // need to transform it 
		mstack->SetViewpointLocal(state.physicalCamera->position);	
	}  
	if (!mstack->viewVolumeLocalOk) {
		mstack->SetViewVolumeLocal(state.viewVolume); 
	}
#ifdef _DEBUG	
	GvBspTree::cullChecks++;  // debug 
#endif
	if (BoxViewVolumeOutside(bbox,mstack->viewpointLocal,mstack->viewVolumeLocal)) { 
#ifdef _DEBUG	
		GvBspTree::cullChecksSucceeded++;  // debug 
#endif
		return gfalse;
	} 
  }	
return gtrue;
}

//
// Gv2Group
//

GV_NODE_SOURCE_IMP(Gv2Group,GvNode);

Gv2Group::Gv2Group() : children(0), 
	bboxCenter(0,0,0),bboxSize(-1,-1,-1),
	statusFlags(0)
{
    GV_NODE_CONSTRUCTOR(Gv2Group);
    isBuiltIn = TRUE;
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);
    GV_NODE_ADD_FIELD(children);

    GV_NODE_ADD_EVENT_IN(/*&Gv2Group::*/addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(/*&Gv2Group::*/removeChildren,GvMFNode);
}

Gv2Group::~Gv2Group()
{
}

// set bounding box 
void Gv2Group::setBBox(const BBox& box)
{
  if (box.IsEmpty()) {
    bboxSize.value[0] = bboxSize.value[1] = bboxSize.value[2] = -1.0;
    bboxSize.setDefault(TRUE);
    bboxSize.OnChanged();
   
  } else { 
    Point s = box.Size(); 
    Point center(box.mi+s * 0.5);
    bboxSize.set(s);
    bboxCenter.set(center);
  }
}

// set bounding box , false if empty box 
GvBool Gv2Group::getBBox( BBox& box)
{
  if (bboxIsEmpty()) {
	  return GvNode::getBBox(box);
      //box.SetEmpty();  
      //return(FALSE);
  }  
  Point s = bboxSize * 0.5;
  box.mi = bboxCenter-s;
  box.ma = bboxCenter+s;
  return(TRUE);

}


// Event IN
void Gv2Group::addChildren(GvMFNode *event)
{
    TRACE("%s::addChildren  !",ClassName());
    if (!event || event->Length() == 0) return;

    children.add(event);
    children.setDefault(FALSE);
    children.OnChanged();
}

void Gv2Group::removeChildren(GvMFNode *event)
{
    TRACE("%s::removeChildren  !",ClassName());
    if (!event || event->Length() == 0) return;
    children.remove(event);
    children.OnChanged();
}



void Gv2Group::addChild(GvNode *node)
{
   	children.add(node);
    children.setDefault(FALSE); 
    children.OnChanged();
}

void Gv2Group::computeStatusFlags(GTraversal &state)
{
	unsigned char newFlags=0;
	if (hasBBoxEx()) newFlags |= GVG_HAS_BBOX;
	newFlags |= GVG_FLAGS_VALID;

	for (int curIndex=0; curIndex < children.Length(); curIndex++) {
		GvNode *n = children.get1(curIndex); // for savety ignore NULL nodes 
		if (n) {
			n = n->Eval(state); // could be in Proto Instance
			if (n) {
				 if (RTISA(n,GvGeometrySensor)) 	newFlags |= GVG_HAS_GEOMETRY_SENSOR;
				 else if (RTISA(n,GvDirectionalLight)) 	newFlags |= GVG_HAS_DIRECTIONAL_LIGHT;
			}
		}
	}
	statusFlags= newFlags;
}


// set redraw bit if a field changed 
int Gv2Group::OnFieldChanged(GvField *field)
{
	if (field == &children) {
		statusFlags &= ~GVG_FLAGS_VALID;
	}
	else if (field == &bboxSize) {
		if (hasBBoxEx()) statusFlags |= GVG_HAS_BBOX;
		else statusFlags &= ~GVG_HAS_BBOX;

	}
	setRedraw();
	return GvNode::OnFieldChanged(field);
}



int Gv2Group:: TraverseChildren(GTraversal &state)
{
    return(children.Traverse(state));
}

int Gv2Group:: TraverseChildren(GPathTraversal &state)
{
    return(children.Traverse(state));
}

// put all GeometrySensors in children into state 
// add call apply to register in state
int Gv2Group::ApplyGeometrySensors(GTraversal &state)
{	
	if (!(statusFlags & GVG_FLAGS_VALID)) 
		computeStatusFlags(state);

	if (!(statusFlags & GVG_HAS_GEOMETRY_SENSOR)) 
		return 0;
	
	GvGeometrySensor *s = NULL;
	int ret=0;
		for (int curIndex=0; curIndex < children.Length(); curIndex++) {
			GvNode *n = children.get1(curIndex); // for savety ignore NULL nodes 
			if (n) {

				n = n->Eval(state); // could be in Proto Instance

				if (n && RTISA(n,GvGeometrySensor)) {
					s = (GvGeometrySensor *) n;
				    ret = s->Apply(state,this);
				}
			}
		}
	return(ret);
}



int Gv2Group:: Do(GTraversal &state)
{	int ret = 0;
    // ??? why state.Push();
    	ret = TraverseChildren(state);
	// ??? state.Pop();
	return(ret);
}


int Gv2Group:: Do(GPathTraversal &state)
{	int ret = 0;
    // ??? state.Push();
	GPUSH_PATH(state,this);
    	ret = TraverseChildren(state);
	GPOP_PATH(state);
	// ???? state.Pop();
	return(ret);
}

int Gv2Group:: Do(GRaySelectTraversal &state)
{	int ret = 0;

	if (!state.CullCheck(bboxCenter,bboxSize)) return GV_NODE_CULLED;

    state.Push();
	GPUSH_PATH(state,this);

	if (state.mode == GRaySelectTraversal::MODE_SELECTION)
		ret = ApplyGeometrySensors(state);
    ret = TraverseChildren(state);

	GPOP_PATH(state);

	state.Pop();
	return(ret);
}

int Gv2Group::Do(GglRenderTraversal &state)
{  	int ret = 0;

	// better ??
	if (!(statusFlags & GVG_FLAGS_VALID)) 
		computeStatusFlags(state);

	if ((statusFlags & GVG_HAS_BBOX) && !CULLCHECK(this,state)) 
		return TRAVERSE_CULLED;

	if (!(statusFlags & GVG_HAS_DIRECTIONAL_LIGHT)) 
    	return children.Traverse(state);


    state.Push(); // because of Directional lights 
		// children.ApplyDirectionalLights(state);
		if (getNeedLightCheck()) {	// move lights in Front 
			children.CheckLights();
			setNeedLightCheck(FALSE);
		}
    	ret = children.Traverse(state);
	state.Pop();
	return(ret);
}



int Gv2Group::Do(GBBoxTraversal &state)
{

  if (bboxIsEmpty() || !state.useBBoxes) { 
	int ret = 0;
    state.Push();
    	ret = TraverseChildren(state);
	state.Pop();
	return(ret);
  }
  
  BBox box;

  Point s = bboxSize * 0.5;
  box.mi = bboxCenter-s;
  box.ma = bboxCenter+s;
 
  state.Union(box);
  return(1);
}



#if 0
GvNode *Gv2Group::EvalAs(TypeId type,int copyAttributes)
{							    
	return(NULL);
}
#endif


//
// Gv2Group
//

GV_NODE_SOURCE_IMP(Gv2CullGroup,Gv2Group);

Gv2CullGroup::Gv2CullGroup() : bboxOk(FALSE), lod (FALSE) ,level(0),range(0)
{
    GV_NODE_CONSTRUCTOR(Gv2CullGroup);
    isBuiltIn = FALSE;
    
	GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);
    GV_NODE_ADD_FIELD(children);

	GV_NODE_ADD_FIELD(lod);
    GV_NODE_ADD_FIELD(level);
    GV_NODE_ADD_FIELD(range);



    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);
	
}

Gv2CullGroup::~Gv2CullGroup()
{
}

int Gv2CullGroup::OnChanged()
{
   bboxOk = FALSE;
   return(1);
}

int Gv2CullGroup::Do(GglRenderTraversal &state)
{
 	int ret = 0;

	// do we need bbox update
	if (!bboxOk) {
		if (!getBBox(bbox))
			bbox.SetEmpty();
		bboxOk = TRUE;
	}

	
	if (state.doViewCulling) 
	{


	if (!bbox.IsEmpty()) {	
	// get the viepoint in local coordinates
	GMatrixStack *mstack = state.GetCurrentMatrix();
	 
	if (!mstack->viewpointLocalOk) { // need to transform it
		mstack->SetViewpointLocal(state.physicalCamera->position);
	}  
	if (!mstack->viewVolumeLocalOk) {
		mstack->SetViewVolumeLocal(state.viewVolume);
	}  

#ifdef _DEBUG
	GvBspTree::cullChecks++;  // debug
#endif	

	if (BoxViewVolumeOutside(bbox,mstack->viewpointLocal,mstack->viewVolumeLocal)) {
#ifdef _DEBUG
		//TRACE("Gv2CullGroup:: Total cull");
		GvBspTree::cullChecksSucceeded++;  // debug
#endif	
		return TRAVERSE_CULLED;
	}

	/* alternative algorithm
	   transform bbox
	   BBox b(bbox);
	   b *= mstack->m;
	   if (BoxPlaneStatus(b,state.viewPlanes) == OUTSIDE) {
			return TRAVERSE_CULLED;
	   }
	*/
   
	}
	}

	int i = 0; 
	if (lod && !bbox.IsEmpty()) 
	{
	if (range.Length() == 0) { // TBD: Performance LOD 
		i= 0; 

	} else {
		const Matrix &m=state.GetMatrixInverse();

		Point eye = state.camera->position;
		Point pos = bbox.Center();
	
		eye *= m;
	
		Point d= pos-eye;
		float dist = d.Len();

		if (state.doLodScale)
			dist *= state.lodScale*m.ScaleFactor(); // must be transformed to local 

		for(i=0; i<range.Length(); i++) {
			if (dist <= range.values[i]) {
				break;
			}
		}

	}
    

	}
	
	if (i==0 ) {
	int xi = state.view->lastFrameInlineLoadCnt;
	int xp = state.view->lastFrameProtoInstanceCnt;

    state.Push(); // because of Directional lights 
		if (getNeedLightCheck()) {	// move lights in Front 
			children.CheckLights();
			setNeedLightCheck(FALSE);
		}
    	ret = children.Traverse(state);
	state.Pop();
	
	// invalidate bbox if inline / proto loaded
	if ( (xi != state.view->lastFrameInlineLoadCnt)
		 || (xp != state.view->lastFrameProtoInstanceCnt))
	{
		bboxOk = gfalse;
	}
	}
	else { // LOD case 
	    int nChilds = level.Length();
		i--; // first level is children

		if ( i <  nChilds) { 
			 GvNode * child;
			 child = level[i];
			 if (child) { 
				 Matrix m;
				 state.Push();
					// compute normalization matrix 

				Point translation  = bbox.Center(); 
				Point size = bbox.Size();
				//float factor = max(size.x,max(size.y,size.z)); uniform
				//factor = 2.0f/factor;
				//scale.Set(factor,factor,factor);
				
				Point scale;
				scale.Set(size.x/2.0,size.y/2.0,size.z/2.0 );
				//translation *= scale;

	  		    m.TranslationMatrix(translation);
				m.ScaleByLeft(scale);
				state.AppendMatrix(m);

					ret = child->Traverse(state);
				 state.Pop();
			 }
		}
	
	}
	return(ret);
}

int Gv2CullGroup::Do(GRaySelectTraversal &state)
{	int ret = 0;

	if (!bboxOk) {
		if (!getBBox(bbox))
			bbox.SetEmpty();
		bboxOk = TRUE;
	}
	

	if (!state.CullCheck(bbox)) return GV_NODE_CULLED;

    state.Push();
	GPUSH_PATH(state,this);

	if (state.mode == GRaySelectTraversal::MODE_SELECTION)
		ret = ApplyGeometrySensors(state);
    ret = TraverseChildren(state);

	GPOP_PATH(state);

	state.Pop();
	return(ret);
}


//
// Gv2Transform
//

GV_NODE_SOURCE_IMP(Gv2Transform,Gv2Group);

Gv2Transform::Gv2Transform() : scale(1,1,1)
{
    GV_NODE_CONSTRUCTOR(Gv2Transform);
    isBuiltIn = TRUE;

	// from Gv2group
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);

    GV_NODE_ADD_FIELD(translation);
    GV_NODE_ADD_FIELD(rotation);
    GV_NODE_ADD_FIELD(scale);
    GV_NODE_ADD_FIELD(scaleOrientation);
    GV_NODE_ADD_FIELD(center);

    // from Gv2group
    GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);

  
	transformCached = 0;
}

int Gv2Transform::checkForDefaultValues(int mode)
{
	const Point zeroPoint(0,0,0);
	const Point onePoint(1,1,1);
	GvSFRotation	zeroRot; zeroRot.value.set(0,0,1,0);




	if (translation.getPoint() == zeroPoint) translation.setDefault(TRUE);
	if (scale.getPoint() == onePoint) scale.setDefault(TRUE);
	if (center.getPoint() == zeroPoint) center.setDefault(TRUE);
	
	if (rotation == zeroRot) rotation.setDefault(TRUE); 
	if (scaleOrientation == zeroRot) scaleOrientation.setDefault(TRUE); 


	return 0;
}


int Gv2Transform::OnChanged()
{
   transformCached = 0;
   return(1);
}

Gv2Transform::~Gv2Transform()
{
}

void Gv2Transform::get(Matrix &m)
{
		 // would be one copy less : m.SetTranslation(center+translation);
#if 0  
  		 m = TranslationMatrix(center+translation);
         if (!rotation.isDefault()) 
		    m = RotationAxisMatrix(rotation.Axis(),rotation.Angle()) * m;
		 if (!scale.isDefault()) {
		    m = RotationAxisMatrix(scaleOrientation.Axis(),scaleOrientation.Angle()) * m;
		    m = ScaleMatrix(scale) * m;
		    m = RotationAxisMatrix(scaleOrientation.Axis(),-scaleOrientation.Angle()) * m;
		 }
		 m = TranslationMatrix(-center.getPoint()) * m;
#else
  		 m.TranslationMatrix(center+translation);

		 if ( m.x[3][0]== 0.0f && m.x[3][1]== 0.0f && m.x[3][2] == 0.0f) {
			 m.contents = 0;
		 }


         if (!rotation.isDefault() || rotation.Angle() != 0.0f) 
		    m.MultiplyLeft(RotationAxisMatrix(rotation.Axis(),rotation.Angle()));

		 if (!scale.isDefault()) {
			if (scaleOrientation.isDefault() || scaleOrientation.Angle()==0.0f) {
				//MultiplyLeft(ScaleMatrix(scale));
				m.ScaleByLeft(scale);
			} else {
				m.MultiplyLeft(RotationAxisMatrix(scaleOrientation.Axis(),scaleOrientation.Angle()));
				// m.MultiplyLeft(ScaleMatrix(scale));
				m.ScaleByLeft(scale);
				m.MultiplyLeft(RotationAxisMatrix(scaleOrientation.Axis(),-scaleOrientation.Angle()));
			}
		 }
		 if (!center.isDefault()) 
			 m.MultiplyLeft(TranslationMatrix(-center.getPoint()));

#endif

}   

// get matrix of node , FALSE if node has no matrix 
GvBool Gv2Transform::getMatrix(Matrix &m)
{
	m = Transform();
	return(TRUE);
}

// set transform by matrix 
void Gv2Transform::setMatrix(const Matrix &m)
{
	Point translate,s,shear;
	Rotation r;
	DecomposeMatrix(m,s,shear,r,translate);
	translation.set(translate);
	scale.set(s);
	rotation.set(r);
	if (!center.isDefault()) center.set(0,0,0);
	if (!scaleOrientation.isDefault()) scaleOrientation.set(0,1,0,0);
}


int Gv2Transform::Do(GTraversal &state)   
{ 
    int ret = 0;
    state.Push();

		//state.AppendMatrix(Transform());

        ret = children.Traverse(state);

	state.Pop();
    return(ret);
}

int Gv2Transform::Do(GPathTraversal &state)   
{ 
    int ret = 0;
    state.Push();
	GPUSH_PATH(state,this);

		state.AppendMatrix(Transform());

        ret = children.Traverse(state);

	GPOP_PATH(state);
	state.Pop();
    return(ret);
}


int Gv2Transform::Do(GRaySelectTraversal &state)
{	int ret = 0;

	Matrix& m=Transform();

	if (m.contents == 0) {
		return Gv2Group::Do(state); // identity matrix
	}


    state.Push();
	GPUSH_PATH(state,this);

		state.AppendMatrix(m);

		if ((statusFlags & GVG_HAS_BBOX) && !state.CullCheck(bboxCenter,bboxSize)) {
			ret = GV_NODE_CULLED;
		} else {
			// put geometry sensors in state
			ret = ApplyGeometrySensors(state);
    		ret = TraverseChildren(state);
		}

	GPOP_PATH(state);
	state.Pop();
	return(ret);
}



// BBOX is bbox of children, not including transform 
int Gv2Transform::Do(GBBoxTraversal &state)
{
    int ret = 0;
    state.Push();

		state.AppendMatrix(Transform());

		if (bboxIsEmpty() || !state.useBBoxes) { 
			ret = children.Traverse(state);
		}
		else {
			BBox box;

			Point s =bboxSize * 0.5;
			box.mi = bboxCenter-s;
			box.ma = bboxCenter+s;
 
			state.Union(box);
		}

  state.Pop();
		
  return(ret);
}



int Gv2Transform::Do(GglRenderTraversal &state)   
{ 
    int ret = 0;

	Matrix& m=Transform();

	if (m.contents == 0) {
		return Gv2Group::Do(state); // identity matrix
	}

	if (!(statusFlags & GVG_FLAGS_VALID)) 
		computeStatusFlags(state);

    state.Push();

		state.AppendMatrix(m);

		if ((statusFlags & GVG_HAS_BBOX) && !CULLCHECK(this,state))  {
			ret = TRAVERSE_CULLED;

		} else {
			// ApplyDirectionalLights
			if ((statusFlags & GVG_HAS_DIRECTIONAL_LIGHT) && getNeedLightCheck()) {	// move lights in Front 
				children.CheckLights();
				setNeedLightCheck(FALSE);
			}

			ret = children.Traverse(state);
		}

	state.Pop();
    return(ret);
}

//
// Gv2MatrixTransform
//

GV_NODE_SOURCE_IMP(Gv2MatrixTransform,Gv2Group);

Gv2MatrixTransform::Gv2MatrixTransform() 
{
    GV_NODE_CONSTRUCTOR(Gv2MatrixTransform);
    isBuiltIn = FALSE;

	// from Gv2group
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);

    GV_NODE_ADD_FIELD(matrix);

    // from Gv2group
    GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);

  
}


int Gv2MatrixTransform::OnChanged()
{
   return(1);
}

Gv2MatrixTransform::~Gv2MatrixTransform()
{
}

// get matrix of node , FALSE if node has no matrix 
GvBool Gv2MatrixTransform::getMatrix(Matrix &m)
{
	m = Transform();
	return(TRUE);
}


int Gv2MatrixTransform::Do(GTraversal &state)   
{ 
    int ret = 0;
    state.Push();

		// state.AppendMatrix(Transform());

        ret = children.Traverse(state);

	state.Pop();
    return(ret);
}

int Gv2MatrixTransform::Do(GPathTraversal &state)   
{ 
    int ret = 0;
    state.Push();
	GPUSH_PATH(state,this);

		state.AppendMatrix(Transform());

        ret = children.Traverse(state);

	GPOP_PATH(state);
	state.Pop();
    return(ret);
}


int Gv2MatrixTransform::Do(GRaySelectTraversal &state)
{	int ret = 0;
    state.Push();
	GPUSH_PATH(state,this);

		state.AppendMatrix(Transform());
		if (!state.CullCheck(bboxCenter,bboxSize)) {
			ret = GV_NODE_CULLED;
		} else {

			ret = ApplyGeometrySensors(state);
    		ret = TraverseChildren(state);
		}

	GPOP_PATH(state);
	state.Pop();
	return(ret);
}



// BBOX is bbox of children, not including transform 
int Gv2MatrixTransform::Do(GBBoxTraversal &state)
{
    int ret = 0;
    state.Push();

		state.AppendMatrix(Transform());

		if (bboxIsEmpty() || !state.useBBoxes) { 
			ret = children.Traverse(state);
		}
		else {
			BBox box;

			Point s = bboxSize * 0.5;
			box.mi = bboxCenter-s;
			box.ma = bboxCenter+s;
 
			state.Union(box);
		}

  state.Pop();
		
  return(ret);
}



int Gv2MatrixTransform::Do(GglRenderTraversal &state)   
{ 
    int ret = 0;
    state.Push();

		state.AppendMatrix(Transform());

        // to do: do render culling here
		if (CULLCHECK(this,state)) {
		if (getNeedLightCheck()) {	// move lights in Front 
			children.CheckLights();
			setNeedLightCheck(FALSE);
		}


        ret = children.Traverse(state);
		}

	state.Pop();
    return(ret);
}


//
// Gv2Billboard
//

GV_NODE_SOURCE_IMP(Gv2Billboard,Gv2Group);

Gv2Billboard::Gv2Billboard() : axisOfRotation(0,1,0)
{
    GV_NODE_CONSTRUCTOR(Gv2Billboard);
    isBuiltIn = TRUE;

	// from Gv2group
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);

    GV_NODE_ADD_FIELD(axisOfRotation);

    // from Gv2group
    GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);


	lastMatrix.Identity();
}


Gv2Billboard::~Gv2Billboard()
{
}

// copy on proto instancing
GvBool Gv2Billboard::computeCopyOnInstance(GvCopyContext &context)
{
	setCopyOnInstance(TRUE);
	return GvNode::computeCopyOnInstance(context);
}


void Gv2Billboard::getMatrix(GGeometryTraversal &state, Matrix &m)
{
	if (!state.physicalCamera) {
		m.Identity();
		return;
	}

	getMatrix(state.physicalCamera,state.GetMatrixInverse(),m);
}

void Gv2Billboard::getMatrix(GCamera *camera,const Matrix &world2local, Matrix &m)
{
		//get eye vector in local coordinate space 
		Point eye = camera->position;
		eye *= world2local;
		eye.Normalize();

		//Point eye = camera->position-camera->target;
		//eye = RotateOnly(world2local,eye);
		//eye.Normalize();

		// rotate z into plane 
		Point z(0,0,1);
		Point axis;



		axisOfRotation.get(axis);
		
		float alen = axis.Length();

		if (alen <= 0.001f) { // compute reference frame : new x,y,z axes
			// screen space 
			Point x,y,z;
			x = RotateOnly(world2local,camera->NormRight());
			x.Normalize();
			y = RotateOnly(world2local,camera->Up());
			y.Normalize();
			z =	NormCrossProd(x,y);
			m = GenRotation(x,y,z);
			m.Invert();
		} else {

			axis *= 1.0f/alen; // Normalize 

			// map eye & z into plane with normal axis through 0.0

			float d=-DotProd(axis,eye);
			eye += axis*d;
			eye.Normalize();

			d=-DotProd(axis,z);
			z += axis*d;
			z.Normalize();

			// now take the angle between eye and z 
			double cosw=DotProd(eye,z);

			Point v = CrossProd(eye,z);
			float sinw = v.Len(); // not right sign 
			double angle = acos(cosw); 
			
			v.Normalize();

			//TRACE("eye (%g %g %g) cosw %g sin %g angle %g len2cross %g\n",eye.x,eye.y,eye.z, cosw,sinw,angle,v.Len2());
			if ((sinw>0.0) && (DotProd(axis,v) >0.0f))  // axis,v are opposite 
				m = RotationAxisMatrix(-axis, angle);
			else m = RotationAxisMatrix(axis, angle);

		}


}

GvBool Gv2Billboard::getMatrix(Matrix &m)
{
	m = lastMatrix;
	return(TRUE);
}

// apply the billboard matrix to state 
int Gv2Billboard::ApplyMatrix(GTraversal &state)
{
	Matrix &m=lastMatrix;
	if (RTISA(&state,GGeometryTraversal)) {
		GGeometryTraversal *s = (GGeometryTraversal *) &state;
		getMatrix(*s,m);
		s->AppendMatrix(m);

	} // else state.AppendMatrix(m);	

	return(1);
}


int Gv2Billboard::Do(GTraversal &state)
{
    int ret = 0;
    state.Push();
		ApplyMatrix(state);
        ret = children.Traverse(state);
	state.Pop();
    return(ret);											  
}

int Gv2Billboard::Do(GPathTraversal &state)
{
    int ret = 0;
	GPUSH_PATH(state,this);

    state.Push();
		ApplyMatrix(state);
        ret = children.Traverse(state);
	state.Pop();

	GPOP_PATH(state);

    return(ret);											  
}

int Gv2Billboard::Do(GRaySelectTraversal &state)
{	int ret = 0;
    state.Push();

	GPUSH_PATH(state,this);
	
	ApplyMatrix(state);

	if (!state.CullCheck(bboxCenter,bboxSize)) {
			ret = GV_NODE_CULLED;
	} else {


		if (state.mode == GRaySelectTraversal::MODE_SELECTION)
			ret = ApplyGeometrySensors(state);
    
		ret = TraverseChildren(state);
	}

	GPOP_PATH(state);

	state.Pop();
	return(ret);
}


int Gv2Billboard::Do(GBBoxTraversal &state)
{
  int ret = 1;

	if (!(statusFlags & GVG_FLAGS_VALID)) 
		computeStatusFlags(state);

  state.Push();
	ApplyMatrix(state);

	if (bboxIsEmpty() || !state.useBBoxes) { 
    		ret = TraverseChildren(state);
	}
  	else  {
		BBox box;

		Point s = bboxSize * 0.5;
		box.mi = bboxCenter-s;
		box.ma = bboxCenter+s;
 
	    state.Union(box);
	}
  state.Pop();

  return(1);
}


int Gv2Billboard::Do(GglRenderTraversal &state)
{  	int ret = 0;

	if (!(statusFlags & GVG_FLAGS_VALID)) 
		computeStatusFlags(state);

    // to do: do render culling here
    state.Push();
		ApplyMatrix(state);

		if ((statusFlags & GVG_HAS_BBOX) && !CULLCHECK(this,state))  {
			ret = TRAVERSE_CULLED;
		} else {
			// ApplyDirectionalLights
			if ((statusFlags & GVG_HAS_DIRECTIONAL_LIGHT) && getNeedLightCheck()) {	// move lights in Front 
				children.CheckLights();
				setNeedLightCheck(FALSE);
			}

			ret = children.Traverse(state);
		}

	state.Pop();
	return(ret);
}

//
// GvHUD
//

GV_NODE_SOURCE_IMP(GvHUD,Gv2Group);

GvHUD::GvHUD() : align(0) 
{
    GV_NODE_CONSTRUCTOR(GvHUD);
    isBuiltIn = FALSE; // extension

	// from Gv2group
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);

    GV_NODE_ADD_FIELD(align);
 
    // from Gv2group
    GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);


	lastMatrix.Identity();
}


GvHUD::~GvHUD()
{
}

// copy on proto instancing
GvBool GvHUD::computeCopyOnInstance(GvCopyContext &context)
{
	setCopyOnInstance(TRUE);
	return GvNode::computeCopyOnInstance(context);
}


void GvHUD::getMatrix(GGeometryTraversal &state, Matrix &m)
{
	if (!state.physicalCamera) {
		m.Identity();
		return;
	}
	getMatrix(state.physicalCamera,state.GetMatrixInverse(),m);
}


void GvHUD::getMatrix(GCamera *camera,const Matrix &world2local, Matrix &m)
{
		//get eye vector in local coordinate space 
		Point eye = camera->position;
		eye *= world2local;
		// ???? eye.Normalize();

		//Point eye = camera->position-camera->target;
		//eye = RotateOnly(world2local,eye);
		//eye.Normalize();

		// rotate z into plane 
		Point z(0,0,1);
		Point axis;
		
		Matrix rot;

  		m.TranslationMatrix(eye);
		//m.Identity();

		{	// compute reference frame : new x,y,z axes
			// screen space 
			Point x,y,z;
			x = RotateOnly(world2local,camera->NormRight());
			x.Normalize();
			y = RotateOnly(world2local,camera->Up());
			y.Normalize();
			z =	NormCrossProd(x,y);
			rot = GenRotation(x,y,z);
			rot.Invert();
		} 
		m.MultiplyLeft(rot);
}


GvBool GvHUD::getMatrix(Matrix &m)
{
	m = lastMatrix;
	return(TRUE);
}

// apply the billboard matrix to state 
int GvHUD::ApplyMatrix(GTraversal &state)
{
	Matrix &m=lastMatrix;
	if (RTISA(&state,GGeometryTraversal)) {
		GGeometryTraversal *s = (GGeometryTraversal *) &state;
		getMatrix(*s,m);
		s->AppendMatrix(m);

	} // else state.AppendMatrix(m);	

	return(1);
}



int GvHUD::Do(GTraversal &state)
{
    int ret = 0;
    state.Push();
		ApplyMatrix(state);
        ret = children.Traverse(state);
	state.Pop();
    return(ret);											  
}

int GvHUD::Do(GPathTraversal &state)
{
    int ret = 0;
	GPUSH_PATH(state,this);

    state.Push();
		ApplyMatrix(state);
        ret = children.Traverse(state);
	state.Pop();

	GPOP_PATH(state);

    return(ret);											  
}

int GvHUD::Do(GRaySelectTraversal &state)
{	int ret = 0;

	// ignore for collisions
	if (state.mode >= GRaySelectTraversal::MODE_COLLISION) {
		return 0;
	}
    

	state.Push();

	GPUSH_PATH(state,this);
	
	ApplyMatrix(state);

	if (!state.CullCheck(bboxCenter,bboxSize)) {
			ret = GV_NODE_CULLED;
	} else {


		if (state.mode == GRaySelectTraversal::MODE_SELECTION)
			ret = ApplyGeometrySensors(state);
    
		ret = TraverseChildren(state);
	}

	GPOP_PATH(state);

	state.Pop();
	return(ret);
}


int GvHUD::Do(GBBoxTraversal &state)
{
  int ret = 1;

  state.Push();
	ApplyMatrix(state);

	if (bboxIsEmpty() || !state.useBBoxes) { 
    		ret = TraverseChildren(state);
	}
  	else  {
		BBox box;

		Point s = bboxSize * 0.5;
		box.mi = bboxCenter-s;
		box.ma = bboxCenter+s;
 
	    state.Union(box);
	}
  state.Pop();

  return(1);
}


int GvHUD::Do(GglRenderTraversal &state)
{  	int ret = 0;

	if (!(statusFlags & GVG_FLAGS_VALID)) 
		computeStatusFlags(state);

    state.Push();

		ApplyMatrix(state);

		if ((statusFlags & GVG_HAS_BBOX) && !CULLCHECK(this,state))  {
			ret = TRAVERSE_CULLED;

		} else {
			// ApplyDirectionalLights
			if ((statusFlags & GVG_HAS_DIRECTIONAL_LIGHT) && getNeedLightCheck()) {	// move lights in Front 
				children.CheckLights();
				setNeedLightCheck(FALSE);
			}

			ret = children.Traverse(state);
		}
	state.Pop();
	return(ret);
}



//
// Gv2Collision
//

GV_NODE_SOURCE_IMP(Gv2Collision,Gv2Group);

Gv2Collision::Gv2Collision() : collide(TRUE)
{
    GV_NODE_CONSTRUCTOR(Gv2Collision);
    isBuiltIn = TRUE;

	// from Gv2group
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);

    GV_NODE_ADD_FIELD(collide);
    GV_NODE_ADD_FIELD(proxy);

    // from Gv2group
    GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);

    GV_NODE_ADD_EVENT_OUT(collideTime);

}


Gv2Collision::~Gv2Collision()
{
}


//
// Gv2Selection
//

GV_NODE_SOURCE_IMP(Gv2Selection,Gv2Group);

Gv2Selection::Gv2Selection() : collide(TRUE),select(TRUE)
{
    GV_NODE_CONSTRUCTOR(Gv2Selection);
    isBuiltIn = FALSE;

	// from Gv2group
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);

    GV_NODE_ADD_FIELD(collide);
    GV_NODE_ADD_FIELD(select);
    GV_NODE_ADD_FIELD(proxy);

    // from Gv2group
    GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);


}


Gv2Selection::~Gv2Selection()
{
}


