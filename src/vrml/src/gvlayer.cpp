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

@module GvLayer.cpp - GLView Layer nodes implementation|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvLayer>

Notes:

Changes:

$Revision: 1.3 $
$Log: gvlayer.cpp,v $
Revision 1.3  1999/07/06 16:45:55  tom
*** empty log message ***




Todo :

******************************************************************************/

#include "stdafx.h"

#include <gv2group.h>

#include "gcamera.h"

#include "gvisibility.h"
#include "gvorthographiccamera.h"
#include "gvperspectivecamera.h"

// for light check
#include "gvdirectionallight.h"

// only needed for cullstats 
#include "gvbsp.h"


// for ApplyTouchSensor
#include <gvsensors.h>
#include <grayselect.h>

#include "grender.h"
#include "gmodel.h"

#include <gvlayer.h>
#include <gvinfo.h>

////////////////////////////////////////////////////////////////////////////////////////


GV_NODE_SOURCE_IMP(GvLayer,Gv2Group);

GvLayer::GvLayer() : viewpointStretch(TRUE), size(-1,-1)
{
    GV_NODE_CONSTRUCTOR(GvLayer);
    isBuiltIn = FALSE;

	// from Gv2group
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);

    // from Gv2group
    GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);


    GV_NODE_ADD_FIELD(background);
    GV_NODE_ADD_FIELD(fog_);
    GV_NODE_ADD_FIELD(navigationInfo);

    GV_NODE_ADD_FIELD(viewpoint);
    GV_NODE_ADD_FIELD(viewpointStretch);
    GV_NODE_ADD_FIELD(pick);
    GV_NODE_ADD_FIELD(clear);

    GV_NODE_ADD_FIELD(depthWrite);
    GV_NODE_ADD_FIELD(depthFunc);

    GV_NODE_ADD_FIELD(lighted);
    GV_NODE_ADD_FIELD(fog);

    GV_NODE_ADD_FIELD(alphaFunc);
    GV_NODE_ADD_FIELD(alphaRef);
    GV_NODE_ADD_FIELD(alphaComp);

    GV_NODE_ADD_FIELD(blend);
    GV_NODE_ADD_FIELD(blendSrcFunc);
    GV_NODE_ADD_FIELD(blendDestFunc);

    GV_NODE_ADD_FIELD(stencilFunc);
    GV_NODE_ADD_FIELD(stencilRef);
    GV_NODE_ADD_FIELD(stencilMask);

    GV_NODE_ADD_FIELD(stencilFail);
    GV_NODE_ADD_FIELD(stencilZFail);
    GV_NODE_ADD_FIELD(stencilZPass);


    GV_NODE_DEFINE_ENUM_VALUE(Clear, NONE);
    GV_NODE_DEFINE_ENUM_VALUE(Clear, COLOR);
    GV_NODE_DEFINE_ENUM_VALUE(Clear, DEPTH);
    GV_NODE_DEFINE_ENUM_VALUE(Clear, ACCUM);
    GV_NODE_DEFINE_ENUM_VALUE(Clear, STENCIL);
    GV_NODE_SET_SF_ENUM_TYPE(clear, Clear);



    GV_NODE_DEFINE_ENUM_VALUE(DepthFunc, NEVER);
    GV_NODE_DEFINE_ENUM_VALUE(DepthFunc, LESS);
    GV_NODE_DEFINE_ENUM_VALUE(DepthFunc, EQUAL);
    GV_NODE_DEFINE_ENUM_VALUE(DepthFunc, LEQUAL);
    GV_NODE_DEFINE_ENUM_VALUE(DepthFunc, GREATER);
    GV_NODE_DEFINE_ENUM_VALUE(DepthFunc, NOTEQUAL);
    GV_NODE_DEFINE_ENUM_VALUE(DepthFunc, GEQUAL);
    GV_NODE_DEFINE_ENUM_VALUE(DepthFunc, ALWAYS);
    GV_NODE_SET_SF_ENUM_TYPE(depthFunc, DepthFunc);

    GV_NODE_DEFINE_ENUM_VALUE(BlendSrcFunc, ZERO);
    GV_NODE_DEFINE_ENUM_VALUE(BlendSrcFunc, ONE);
    GV_NODE_DEFINE_ENUM_VALUE(BlendSrcFunc, DST_COLOR);
    GV_NODE_DEFINE_ENUM_VALUE(BlendSrcFunc, ONE_MINUS_DST_COLOR);
    GV_NODE_DEFINE_ENUM_VALUE(BlendSrcFunc, SRC_ALPHA);
    GV_NODE_DEFINE_ENUM_VALUE(BlendSrcFunc, ONE_MINUS_SRC_ALPHA);
    GV_NODE_DEFINE_ENUM_VALUE(BlendSrcFunc, DST_ALPHA);
    GV_NODE_DEFINE_ENUM_VALUE(BlendSrcFunc, ONE_MINUS_DST_ALPHA);
    GV_NODE_DEFINE_ENUM_VALUE(BlendSrcFunc, SRC_ALPHA_SATURATE);
    GV_NODE_SET_SF_ENUM_TYPE(blendSrcFunc, BlendSrcFunc);

    GV_NODE_DEFINE_ENUM_VALUE(BlendDestFunc, ZERO);
    GV_NODE_DEFINE_ENUM_VALUE(BlendDestFunc, ONE);
    GV_NODE_DEFINE_ENUM_VALUE(BlendDestFunc, DST_COLOR);
    GV_NODE_DEFINE_ENUM_VALUE(BlendDestFunc, ONE_MINUS_DST_COLOR);
    GV_NODE_DEFINE_ENUM_VALUE(BlendDestFunc, SRC_ALPHA);
    GV_NODE_DEFINE_ENUM_VALUE(BlendDestFunc, ONE_MINUS_SRC_ALPHA);
    GV_NODE_DEFINE_ENUM_VALUE(BlendDestFunc, DST_ALPHA);
    GV_NODE_DEFINE_ENUM_VALUE(BlendDestFunc, ONE_MINUS_DST_ALPHA);
    GV_NODE_DEFINE_ENUM_VALUE(BlendDestFunc, SRC_ALPHA_SATURATE);
    GV_NODE_SET_SF_ENUM_TYPE(blendDestFunc, BlendDestFunc);



    GV_NODE_DEFINE_ENUM_VALUE(AlphaFunc, NEVER);
    GV_NODE_DEFINE_ENUM_VALUE(AlphaFunc, LESS);
    GV_NODE_DEFINE_ENUM_VALUE(AlphaFunc, EQUAL);
    GV_NODE_DEFINE_ENUM_VALUE(AlphaFunc, LEQUAL);
    GV_NODE_DEFINE_ENUM_VALUE(AlphaFunc, GREATER);
    GV_NODE_DEFINE_ENUM_VALUE(AlphaFunc, NOTEQUAL);
    GV_NODE_DEFINE_ENUM_VALUE(AlphaFunc, GEQUAL);
    GV_NODE_DEFINE_ENUM_VALUE(AlphaFunc, ALWAYS);
    GV_NODE_SET_SF_ENUM_TYPE(alphaFunc, AlphaFunc);


    GV_NODE_ADD_FIELD(translation);
    GV_NODE_ADD_FIELD(depth);
    GV_NODE_ADD_FIELD(size);
    

	clear.value = NONE;
	pick.value = TRUE;
	depthWrite.value = FALSE;
	depthFunc.value = LESS;

    lighted.value = TRUE;
	fog.value = FALSE; // TRUE;
	
    alphaFunc.value = ALWAYS;
    alphaRef.value = 0.0;

    blend.value = FALSE;
    blendSrcFunc.value = SRC_ALPHA;
    blendDestFunc.value = ONE_MINUS_SRC_ALPHA;

}

GvLayer::~GvLayer()
{
}

// a field has been changed
int GvLayer::OnFieldChanged(GvField *field)
{
  if (field == & children)
	  layerBBox.SetEmpty();

  return Gv2Group::OnFieldChanged(field);

}


int GvLayer::Do(GTraversal &state)   
{ 

    state.Push();

	int ret = TraverseChildren(state);

	state.Pop();

	return(ret);

}


// compute new viewport rect 

GvBool GvLayer::getViewportRect(const GRectInt &srcViewport, GRectInt &newViewport) 
{

	if (translation.isDefault() && size.isDefault()) return FALSE;

	// offset
	newViewport.x=srcViewport.x+srcViewport.dx*translation.x();
	newViewport.y=srcViewport.y+srcViewport.dy*translation.y();

	// size
	newViewport.dx=srcViewport.dx*size.x();
	newViewport.dy=srcViewport.dy*size.y();

	if (newViewport.dx  < 0) {		// eg 1 -0.5, right aligned ??
		newViewport.x += newViewport.dx;
		newViewport.dx = - newViewport.dx;
	}
	if (newViewport.dy  < 0) {		// eg 1 -0.5, right aligned ??
		newViewport.y += newViewport.dy;
		newViewport.dy = - newViewport.dy;
	} else if (newViewport.dy  == 0) { // match 
		newViewport.dy = newViewport.dx;
	}

	return TRUE;

}


int GvLayer::Do(GglRenderTraversal &state)   
{ 
    int ret=0;


	// for savety process pending transparent stuff,
	// layer nodes should be ordered properly by higher levels

	//// render transparent geometry in layer here
	
	state.RenderTransparentElements();



    state.Push();

	// push a dummy matrix, because view dependent values may cached in stack
	Matrix m;
	m.Identity();
	state.AppendMatrix(m);
	gbool viewCulling = state.doViewCulling; 
	
	// state.doViewCulling = FALSE;

	RenderState &glState = *state.glState;

	RenderFlags _vertices=glState.vertices;
	RenderFlags _normals=glState.normals;
	RenderFlags _edges=glState.edges;
	RenderFlags _faces=glState.faces;
	RenderFlags _bboxes=glState.bboxes;


	// need to to state changes in render Flags

	// viewport

	gbool doViewport = gfalse;

	GRectInt _vp;
	GRectInt  vp;


	glState.GetViewport(_vp.x,_vp.y,_vp.dx,_vp.dy);
	
	if (getViewportRect(_vp,vp)) {
		doViewport = gtrue;
		glState.SetViewport(vp.x,vp.y,vp.dx,vp.dy);

	}


	// clear 
	if (clear.value != NONE) 

		glState.Clear((clear.value & COLOR) !=0,
				  (clear.value & DEPTH) !=0,
				  (clear.value & STENCIL) !=0,
				  (clear.value & ACCUM) !=0);
  

    gbool _depthMask = glState.zwrite;

	// ?? if (!depthWrite.isDefault()) 
	glState.SetZWrite(depthWrite.value);

    gbool _depth = glState.zbuffer;
    GLint _depthFunc = glState.depthFunc;


	// D3D : turning off ZBuffer seems to disable proper z-clip 
	// atleast for TL 
    if (depthFunc.value == ALWAYS) {  // to do: remap defines API independent
#ifdef _D3D
			
		if (glState.doOwnLighting) 
			glState.SetDepthFunc(D3DCMP_ALWAYS);
		else //glState.canDoRgbLighting == TRUE )
			glState.SetZBuffer(FALSE); // seems to turn off z-clipping in D3d !!!!

		

#else 
		glState.SetZBuffer(FALSE); // seems to turn off z-clipping in D3d !!!!
#endif
	}
    else {
	        // if (!depthFunc.isDefault()) 
            glState.SetDepthFunc(depthFunc.value);
            glState.SetZBuffer(TRUE); // or use current default
    }


    gbool _lighted = glState.lighting;
    if (!lighted.isDefault()) { // specific overriden 
		glState.SetLighting(lighted.value);
		glState.SetAnyLighting(lighted.value);
	}

	gbool _fog = glState.fogEnabled;
	glState.SetFogEnable(fog.value); // new 29.05.99


    gbool _alpha = glState.alphaTest;
    GLenum  _alphaFunc=glState.alphaFunc; 
    GLfloat _alphaRef=glState.alphaRef;

    if (alphaFunc.value == ALWAYS) { glState.SetAlphaTest(FALSE); }
    else  {
        glState.SetAlphaFunc(alphaFunc.value,alphaRef.value);
        glState.SetAlphaTest(TRUE);
    }


    GLenum _blendSrcFunc,_blendDestFunc; 	
    gbool _blend = glState.blend;
    
    if (blend.value) {
        if (!_blend) glState.SetBlend(TRUE);
        _blendSrcFunc = glState.blendSrcFunc;
        _blendDestFunc = glState.blendSrcFunc;
        glState.SetBlendFunc(blendSrcFunc.value,blendDestFunc.value); 	
    } else {
        if (_blend) glState.SetBlend(FALSE);
    }


	// camera to be used 
	// change physical camera
	GCamera *camera = state.camera;
	GCamera *physicalCamera = state.physicalCamera;
	
	layerCamera.znear = 0.1;
	layerCamera.zfar = 5;


	if (viewpoint.value) {
		GvNode * c= viewpoint.Eval(state);

		if (layerBBox.IsEmpty()) {
			children.getBBox(layerBBox);
		}
		if (c) {
/*
		if (RTISA(c,GvPerspectiveCamera)) 
			layerCamera.Set( (GvPerspectiveCamera *)  c);
		else if (RTISA(c,OrthographicCamera)) 
			layerCamera.Set( (GvOrthographicCamera *)  c);
		else 
*/
		if (RTISA(c,GvViewpoint)) {
			 ((GvViewpoint *)  c)->getCamera(layerCamera,layerBBox);
		}
		}
	}

	state.doClipping = gtrue;

	// set the aspect

	if (doViewport) {
		layerCamera.SetFieldPixel(vp.dx,vp.dy);
		if (!viewpointStretch.get())
			layerCamera.SetAspect(vp.dx/(float) vp.dy);
	}
	else {
		layerCamera.SetFieldPixel(_vp.dx,_vp.dy);
		if (!viewpointStretch.get())
			layerCamera.SetAspect(_vp.dx/(float) _vp.dy);
	}


//	if (!stretch) 
//		SetAspect(w/h);

	state.glState->SetCamera(&layerCamera);
	state.SetCamera(&layerCamera);
	state.SetPhysicalCamera(&layerCamera);


	/////////////////////////////
	ret = TraverseChildren(state);
	//////////////////////////////


	//// render transparent geometry in layer here
	
	state.RenderTransparentElements();


	////

    
	// undo GL State changes 
	// RenderState will check, if same value NOOP 

    glState.SetZWrite(_depthMask);
    
    glState.SetZBuffer(_depth);
    
    
	glState.SetDepthFunc(_depthFunc);

    glState.SetLighting(_lighted);

	glState.SetFogEnable(_fog);

    
    glState.SetAlphaFunc(_alphaFunc,_alphaRef);
    glState.SetAlphaTest(_alpha);
    
    if (blend.value) 
       glState.SetBlendFunc(_blendSrcFunc,_blendDestFunc); 	

    glState.SetBlend(_blend);

	glState.vertices=_vertices;
	glState.normals=_normals;
	glState.edges =_edges;
	glState.faces=_faces;
	glState.bboxes=_bboxes;

	state.doViewCulling = viewCulling; 

	if (doViewport)
		glState.SetViewport(_vp.x,_vp.y,_vp.dx,_vp.dy);

	// undo camera change

	if (physicalCamera) {
		state.SetCamera(camera);
		state.glState->SetCamera(physicalCamera);
		state.SetPhysicalCamera(physicalCamera);
	}

	state.Pop();
	return(ret);

}




int GvLayer::Do(GRaySelectTraversal &state) 
{
	int ret;

	if (!pick) return 0;

	if (state.mode != GRaySelectTraversal::MODE_SELECTION) // only selection supported
		return 0;


	
    state.Push();
	GPUSH_PATH(state,this);


	{
		// push a dummy matrix, because view dependent values may cached in stack
		Matrix m;
		m.Identity();
		state.AppendMatrix(m);

		gbool doTraverse = gtrue;

		//getViewportRect
		gbool doViewport = gfalse;
		RayRange savedRay;

		GRectInt _vp; // saved viewport 
		GRectInt  vp;

		_vp = state.GetViewport();
		savedRay = state.ray;
	
		if (getViewportRect(_vp,vp)) {

				if (!_vp.Outside(vp)) {

					if (vp.Outside(state.pscreen.x,	state.pscreen.y)) {
						doTraverse = gfalse; // mouse outside viewport
					} else {
						doViewport = gtrue;
						state.SetViewport(vp);
					}	

				}	
				else  doTraverse = gfalse;


		} else vp=_vp;


	if (doTraverse && vp.dx >1 && vp.dy>1) {

	// camera to be used 
	// change physical camera
	GCamera *camera = state.camera;
	GCamera *physicalCamera = state.physicalCamera;
	
	layerCamera.znear = 0.1;
	layerCamera.zfar = 5;
	//state.doClipping = gtrue;


	if (viewpoint.value) {
		GvNode * c= viewpoint.Eval(state);

		if (layerBBox.IsEmpty()) {
			children.getBBox(layerBBox);
		}
/*
		if (RTISA(c,GvPerspectiveCamera)) 
			layerCamera.Set( (GvPerspectiveCamera *)  c);
		else if (RTISA(c,OrthographicCamera)) 
			layerCamera.Set( (GvOrthographicCamera *)  c);
		else 
*/
		if (RTISA(c,GvViewpoint)) {
			 ((GvViewpoint *)  c)->getCamera(layerCamera,layerBBox);
		}

	}

	// set  viewport & aspect

	if (doViewport) {
		layerCamera.SetFieldPixel(vp.dx,vp.dy);
		if (!viewpointStretch.get())
			layerCamera.SetAspect(vp.dx/(float) vp.dy);
	}
	else {
		layerCamera.SetFieldPixel(_vp.dx,_vp.dy);
		if (!viewpointStretch.get())
			layerCamera.SetAspect(_vp.dx/(float) _vp.dy);
	}

	layerCamera.UpdateViewProjectionMatrix(); 
	
	// set the new camera
	state.SetCamera(&layerCamera);
	state.SetPhysicalCamera(&layerCamera);

	// compute new ray 
	Point mouseNormalized;
   
	Point objP;
	Point objP1;

    mouseNormalized.Set( (float) (state.pscreen.x -vp.x) / (float) (vp.dx-1),
						 (float) (state.pscreen.y -vp.y) / (float) (vp.dy-1),
							0.0f);

	layerCamera.ComputeWorldRay(mouseNormalized.x,mouseNormalized.y, objP,objP1, 0.0f,1.0f);
	TRACE(" MN %f %f p1 (%f %f %f) p2 (%f %f %f) \n",mouseNormalized.x,mouseNormalized.y,objP.x,objP.y,objP.z,objP1.x,objP1.y,objP1.z);
    state.ray.SetByPoints(objP,objP1);
	
	ArrayNew<GHitInfo> oldHits;		

	// ASSERT(state.ray.d.Len()>0.0f);

	if ((clear.value & COLOR) !=0) { // mouse in viewport clear hits behind
		state.FlushHits();
	}
	else {
		oldHits.SetLength(state.hits.Length());
		for (int i=0; i<state.hits.Length();i++) {
			oldHits[i]=state.hits[i];
		}		
		state.FlushHits(); // flush hits
	}		
		

		// put geometry sensors in state
		ret = ApplyGeometrySensors(state);
    	ret = TraverseChildren(state);

	// restore camera
	if (physicalCamera) {
		state.SetCamera(camera);
		state.SetPhysicalCamera(physicalCamera);
	}
	
	if (state.hits.Length()>0) { // there was some hit, fine
		// could store layer info in hit 
	}
	else { // restore back old hits from behind 
		state.hits.SetLength(state.hits.Length());
		for (int i=0; i<state.hits.Length();i++) {
			state.hits[i]=oldHits[i];
		}		
		oldHits.Flush();
	}

	} // mouse in viewport

	if (doViewport) {
		// restore viewport
		state.SetViewport(_vp);
	}
	// restore ray
	state.ray = savedRay;

	}

	GPOP_PATH(state);
	state.Pop();
	return(ret);

}


//
// GvLayer3D
//

GV_NODE_SOURCE_IMP(GvLayer3D,Gv2Group);

GvLayer3D::GvLayer3D() : childrenLayer(0), childrenLayerSorted(0), size(-1,-1)
{
    GV_NODE_CONSTRUCTOR(GvLayer3D);
    isBuiltIn = FALSE;

	// from Gv2group
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);

    GV_NODE_ADD_EVENT_IN(addChildrenLayer,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildrenLayer,GvMFNode);

	//
    GV_NODE_ADD_FIELD(childrenLayer);
    GV_NODE_ADD_FIELD(translation);
    GV_NODE_ADD_FIELD(depth);
    GV_NODE_ADD_FIELD(size);

	// eventIn"s ???
    GV_NODE_ADD_FIELD(background);
    GV_NODE_ADD_FIELD(fog);
    GV_NODE_ADD_FIELD(navigationInfo);
    GV_NODE_ADD_FIELD(viewpoint);

    // from Gv2group
    GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);

	childrenLayerChanged=gtrue;
	
	layerBBox.SetEmpty();
  
}

GvLayer3D::~GvLayer3D()
{
}

int GvLayer3D::OnChildChanged(GvNode *child)
{
	setRedraw();
	return Gv2Group::OnChildChanged(child);
}

int GvLayer3D::OnFieldChanged(GvField *field)
{
	if (field == &childrenLayer)
	{
		childrenLayerChanged=gtrue;
		childrenLayerSorted.deleteAll();
	}
	if (field == &children)
	{
		layerBBox.SetEmpty();
	}
	return Gv2Group::OnFieldChanged(field);
}


int GvLayer3D::OnChanged()
{
   return Gv2Group::OnChanged();
}




// Event IN
void GvLayer3D::addChildrenLayer(GvMFNode *event)
{
    TRACE("%s::addChildren  !",ClassName());
    if (!event || event->Length() == 0) return;

    childrenLayer.add(event);
    childrenLayer.setDefault(FALSE);
    childrenLayer.OnChanged();
}

void GvLayer3D::removeChildrenLayer(GvMFNode *event)
{
    TRACE("%s::removeChildren  !",ClassName());
    if (!event || event->Length() == 0) return;
    childrenLayer.remove(event);
    childrenLayer.OnChanged();
}

// compute new viewport rect 

GvBool GvLayer3D::getViewportRect(const GRectInt &srcViewport, GRectInt &newViewport) 
{

	if (translation.isDefault() && size.isDefault()) return FALSE;

	// offset
	newViewport.x=srcViewport.x+srcViewport.dx*translation.x();
	newViewport.y=srcViewport.y+srcViewport.dy*translation.y();

	// size
	newViewport.dx=srcViewport.dx*size.x();
	newViewport.dy=srcViewport.dy*size.y();

	if (newViewport.dx  < 0) {		// eg 1 -0.5, right aligned ??
		newViewport.x += newViewport.dx;
		newViewport.dx = - newViewport.dx;
	}
	if (newViewport.dy  < 0) {		// eg 1 -0.5, right aligned ??
		newViewport.y += newViewport.dy;
		newViewport.dy = - newViewport.dy;
	} else if (newViewport.dy  == 0) { // match 
		newViewport.dy = newViewport.dx;
	}

	return TRUE;

}



int GvLayer3D::Do(GTraversal &state)   
{ 
    int ret = 0;
    state.Push();

        ret = children.Traverse(state);
		childrenLayer.Traverse(state);

	state.Pop();
    return(ret);
}

int GvLayer3D::Do(GPathTraversal &state)   
{ 
    int ret = 0;
    state.Push();
	GPUSH_PATH(state,this);

        ret = children.Traverse(state);
		childrenLayer.Traverse(state);

	GPOP_PATH(state);
	state.Pop();
    return(ret);
}


int GvLayer3D::Do(GRaySelectTraversal &state)
{	int ret = 0;

	if (state.mode != GRaySelectTraversal::MODE_SELECTION) // only selection supported
		return 0;

    state.Push();
	GPUSH_PATH(state,this);


	{
		// push a dummy matrix, because view dependent values may cached in stack
		Matrix m;
		m.Identity();
		state.AppendMatrix(m);

		gbool doTraverse = gtrue;

		//getViewportRect
		gbool doViewport = gfalse;
		RayRange savedRay;

		GRectInt _vp; // saved viewport 
		GRectInt  vp;

		_vp = state.GetViewport();
		savedRay = state.ray;
	
		if (getViewportRect(_vp,vp)) {

				if (!_vp.Outside(vp)) {

					if (vp.Outside(state.pscreen.x,	state.pscreen.y)) {
						doTraverse = gfalse; // mouse outside viewport
					} else {
						doViewport = gtrue;
						state.SetViewport(vp);
					}	

				}	
				else  doTraverse = gfalse;


		} else vp=_vp;


	if (doTraverse && vp.dx >1 && vp.dy>1) {

	// camera to be used 
	// change physical camera
	GCamera *camera = state.camera;
	GCamera *physicalCamera = state.physicalCamera;
	
	layerCamera.znear = 0.1;
	layerCamera.zfar = 5;
	//state.doClipping = gtrue;


	if (viewpoint.value) {
		GvNode * c= viewpoint.Eval(state);

		if (layerBBox.IsEmpty()) {
			children.getBBox(layerBBox);
		}
/*
		if (RTISA(c,GvPerspectiveCamera)) 
			layerCamera.Set( (GvPerspectiveCamera *)  c);
		else if (RTISA(c,OrthographicCamera)) 
			layerCamera.Set( (GvOrthographicCamera *)  c);
		else 
*/
		if (RTISA(c,GvViewpoint)) {
			 ((GvViewpoint *)  c)->getCamera(layerCamera,layerBBox);
		}

	}

	// set  viewport & aspect

	if (doViewport) {
		layerCamera.SetFieldPixel(vp.dx,vp.dy);
		//if (!viewpointStretch.get())
		//	layerCamera.SetAspect(vp.dx/(float) vp.dy);
	}
	else {
		layerCamera.SetFieldPixel(_vp.dx,_vp.dy);
		//if (!viewpointStretch.get())
		//	layerCamera.SetAspect(_vp.dx/(float) _vp.dy);
	}

	layerCamera.UpdateViewProjectionMatrix(); 
	
	// set the new camera
	state.SetCamera(&layerCamera);
	state.SetPhysicalCamera(&layerCamera);

	// compute new ray 
	Point mouseNormalized;
   
	Point objP;
	Point objP1;

    mouseNormalized.Set( (float) (state.pscreen.x -vp.x) / (float) (vp.dx-1),
						 (float) (state.pscreen.y -vp.y) / (float) (vp.dy-1),
							0.0f);

	layerCamera.ComputeWorldRay(mouseNormalized.x,mouseNormalized.y, objP,objP1, 0.0f,1.0f);
	TRACE(" MN %f %f p1 (%f %f %f) p2 (%f %f %f) \n",mouseNormalized.x,mouseNormalized.y,objP.x,objP.y,objP.z,objP1.x,objP1.y,objP1.z);
    state.ray.SetByPoints(objP,objP1);
	
	ArrayNew<GHitInfo> oldHits;		

	// ASSERT(state.ray.d.Len()>0.0f);
	gbool transparent = background.get() == (GvNode *) NULL;

	if (!transparent) { // mouse in viewport, clear hits behind
		state.FlushHits();
	}
	else {
		oldHits.SetLength(state.hits.Length());
		for (int i=0; i<state.hits.Length();i++) {
			oldHits[i]=state.hits[i];
		}		
		state.FlushHits(); // flush hits
	}		
		

		// put geometry sensors in state
		ret = ApplyGeometrySensors(state);
    	ret = TraverseChildren(state);

		if (state.hits.Length()>0) { // there was some hit, fine
			// could store layer info in hit 
		}
		else { // restore back old hits from behind 
			state.hits.SetLength(state.hits.Length());
			for (int i=0; i<state.hits.Length();i++) {
				state.hits[i]=oldHits[i];
			}		
			oldHits.Flush();
		}


	// tbd: sort by depth
		childrenLayer.Traverse(state);

	// restore camera
	if (physicalCamera) {
		state.SetCamera(camera);
		state.SetPhysicalCamera(physicalCamera);
	}
	

	} // mouse in viewport

	if (doViewport) {
		// restore viewport
		state.SetViewport(_vp);
	}
	// restore ray
	state.ray = savedRay;

	}
	GPOP_PATH(state);
	state.Pop();
	return(ret);
}



// BBOX is bbox of children, not including transform 
int GvLayer3D::Do(GBBoxTraversal &state)
{
    int ret = 0;
    state.Push();


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



int GvLayer3D::Do(GglRenderTraversal &state)   
{ 
    int ret = 0;
	
	RenderState &glState = *state.glState;

	if (!(statusFlags & GVG_FLAGS_VALID)) 
		computeStatusFlags(state);


	gbool doViewport = gfalse;
	gbool viewCulling = state.doViewCulling; 

	GRectInt _vp;
	GRectInt  vp;

	glState.GetViewport(_vp.x,_vp.y,_vp.dx,_vp.dy);
	
	if (getViewportRect(_vp,vp)) {
		doViewport = gtrue;
		glState.SetViewport(vp.x,vp.y,vp.dx,vp.dy);
	}


    state.Push();

	// camera to be used 
	// change physical camera
	GCamera *camera = state.camera;
	GCamera *physicalCamera = state.physicalCamera;
	
	layerCamera.znear = 0.1;
	layerCamera.zfar = 5;


	if (viewpoint.value) {
		GvNode * c= viewpoint.Eval(state);

		if (layerBBox.IsEmpty()) {
			children.getBBox(layerBBox);
		}

		if (c) {
			if (RTISA(c,GvViewpoint)) {
				 ((GvViewpoint *)  c)->getCamera(layerCamera,layerBBox);
			}
		}

	}

	// background 
	GvBackground *bg=NULL;
	if (background.value) {
		GvNode * c= background.Eval(state);
		if (c) {
			if (RTISA(c,GvBackground)) {
				 bg = (GvBackground*) c;
			}
		}
	}

	gbool transparent = bg == (GvNode *) NULL;

	RGBAColor savedBgc;
	if (bg) {
		savedBgc= glState.background;
		if (bg->skyColor.getNum()>0) {
			Point color;
			bg->skyColor.get1(0,color);
			glState.SetBackgroundColor(color);
		}
	}
	gbool clearZ=children.getNum()>0; // optimization if layer is color rectangle only 
	glState.Clear(!transparent,clearZ);

	// fog
	gbool _fog = glState.fogEnabled;
	glState.SetFogEnable(0);

	// set the aspect

	if (doViewport) {
		layerCamera.SetFieldPixel(vp.dx,vp.dy);
		//if (!viewpointStretch.get())
			layerCamera.SetAspect(vp.dx/(float) vp.dy);
	}
	else {
		layerCamera.SetFieldPixel(_vp.dx,_vp.dy);
		//if (!viewpointStretch.get())
			layerCamera.SetAspect(_vp.dx/(float) _vp.dy);
	}
	state.doClipping = gtrue; // turn off any clipping hints


//	if (!stretch) 
//		SetAspect(w/h);

	state.glState->SetCamera(&layerCamera);
	state.SetCamera(&layerCamera);
	state.SetPhysicalCamera(&layerCamera);


		/*
		if ((statusFlags & GVG_HAS_BBOX) && !CULLCHECK(this,state))  {
			ret = TRAVERSE_CULLED;

		} else 
		*/
		{
			////

			// ApplyDirectionalLights
			if ((statusFlags & GVG_HAS_DIRECTIONAL_LIGHT) && getNeedLightCheck()) {	// move lights in Front 
				children.CheckLights();
				setNeedLightCheck(FALSE);
			}


			////

			ret = children.Traverse(state);
			//// render transparent geometry in layer here
	
			state.RenderTransparentElements();

			// tbd: sort by depth
			childrenLayer.Traverse(state);
		}

	// undo state changes
	
	glState.SetFogEnable(_fog);

	if (bg) {
		glState.SetBackgroundColor(savedBgc);
	}

	if (doViewport)
		glState.SetViewport(_vp.x,_vp.y,_vp.dx,_vp.dy);

	// undo camera change

	if (physicalCamera) {
		state.SetCamera(camera);
		state.glState->SetCamera(physicalCamera);
		state.SetPhysicalCamera(physicalCamera);
	}
	state.doClipping = gtrue;

	state.Pop();
    return(ret);
}


