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

@module gvnurbs.cpp 
Copyright (c) 1998-1999	by blaxxun interactive - Holger Grahn & Tom Volk
All rights reserved

Purpose:

Classes:

Notes:

Changes:

$Revision: $
$Log: $




Todo :

******************************************************************************/

#include "stdafx.h"

#include "greporter.h"

#include <gvtraverse.h>

#include <gvmaterial.h>
#include <gvtexture2.h>
#include <gvtexture2transform.h>
#include "gshell.h"
#include "gshelli.h"
#include "ggeom.h"

#include "grender.h"
#include "gmodel.h"

#include <grayselect.h>

#include "gvnurbs.h"
#include "gv2geometry.h"

#include "gvisibility.h"

#include "gnurbsutil.h"


// utils 

// pack MFVec3f & optional MFFloat to vec4f
int getVec4f(GvMFVec3f &p, GvMFFloat &w, FloatArray &vec)
{
	int numCtrlPoints = p.getNum();

	vec.SetLength(numCtrlPoints*4);
	int idx=0;
	
	int wnum = w.getNum();
	float *ctrlPoints = vec.DataRW();

	for(int ii=0; ii<numCtrlPoints; ii++)
	{
		const float *t;
		t = p.get1(ii);
		ctrlPoints[idx++] = t[0];
		ctrlPoints[idx++] = t[1];
		ctrlPoints[idx++] = t[2];
		if (ii< wnum) ctrlPoints[idx++] = w.get1(ii);
		else ctrlPoints[idx++] = 1.f;
	}
	return numCtrlPoints;
}

// pack MFVec2f & optional MFFloat to vec4f
int getVec3f(GvMFVec2f &p, GvMFFloat &w, FloatArray &vec)
{
	int numCtrlPoints = p.getNum();

	vec.SetLength(numCtrlPoints*3);
	int idx=0;
	
	int wnum = w.getNum();
	float *ctrlPoints = vec.DataRW();

	for(int ii=0; ii<numCtrlPoints; ii++)
	{
		const float *t;
		t = p.get1(ii);
		ctrlPoints[idx++] = t[0];
		ctrlPoints[idx++] = t[1];
		if (ii< wnum) ctrlPoints[idx++] = w.get1(ii);
		else ctrlPoints[idx++] = 1.f;
	}
	return numCtrlPoints;
}



//////////////////////////////////////////////////////////////	
//															//	
//															//	
//				S O M E    F U N C T I O N S				//
//															//
//////////////////////////////////////////////////////////////
 
// get the error reporting object 
static GReporter* getReporter(GTraversal &state) 
{
	GView * view = state.getBrowser(); 
	ASSERT(view != NULL);
	if (!view) return GReporter::current; // return NULL;
	return view->GetReporter();
}






//////////////////////////////////////////////////////////////	
//															//	
//															//	
//				T H E   N U R B S S U R F A C E				//
//															//
//////////////////////////////////////////////////////////////
 
GV_NODE_SOURCE_IMP(GvNurbsSurface,GvNodeShell);


GvNurbsSurface::GvNurbsSurface() :
        uDimension(0),
        vDimension(0),
        uTessellation(0),
        vTessellation(0),
        uKnot(0),
        vKnot(0),

        uOrder(3),
        vOrder(3),
        controlPoint(0),
        weight(0),
        solid(TRUE),
        ccw(TRUE)
{
    GV_NODE_CONSTRUCTOR(GvNurbsSurface);
    isBuiltIn = FALSE;
	theShell = NULL;

    GV_NODE_ADD_FIELD(uDimension);
    GV_NODE_ADD_FIELD(vDimension);

 
    GV_NODE_ADD_FIELD(controlPoint);
    GV_NODE_ADD_FIELD(weight);
    GV_NODE_ADD_FIELD(texCoord);

    GV_NODE_ADD_FIELD(uOrder);
    GV_NODE_ADD_FIELD(vOrder);

    GV_NODE_ADD_FIELD(uKnot);
    GV_NODE_ADD_FIELD(vKnot);


    GV_NODE_ADD_FIELD(uTessellation);
    GV_NODE_ADD_FIELD(vTessellation);

    GV_NODE_ADD_FIELD(solid);
    GV_NODE_ADD_FIELD(ccw);


	m_bInitialized = gfalse;
	m_bBadValues = gfalse;

#ifdef _OGL_NURBS
	nurbsObj = gluNewNurbsRenderer();

    gluNurbsProperty(nurbsObj, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
//    gluNurbsProperty(nurbsObj, GLU_V_STEP, 20);
//    gluNurbsProperty(nurbsObj, GLU_U_STEP, 20);

	ASSERT(nurbsObj != NULL);

#endif

}

GvNurbsSurface::~GvNurbsSurface()
{
#ifdef _OGL_NURBS	
	if (nurbsObj)
		gluDeleteNurbsRenderer(nurbsObj );
#endif

}


int GvNurbsSurface::Do(GBBoxTraversal &state)
{

	//if (!theShell) BuildShell(state);

	if (theShell) 
		return GvNodeShell::Do(state);


	// compute BBOX from control points, use weights ???
	const Point *vp,*vpend;
	Point mi,ma;

	if (controlPoint.Length()<=0) return(0);

	vp= (const Point *) controlPoint.values;

	vpend=&vp[controlPoint.Length()];

	mi= ma= *vp++;
	for (; vp<vpend;vp++) {
		vp->MinMax(mi,ma);
	}

	BBox box(mi,ma);

	state.Union(box);

    return(1);
}


// get the internal GNurbsShell
GNurbsShell* GvNurbsSurface::getNurbsShell(GTraversal &state)
{
	if (!theShell) 
		BuildShell(state);

	return getNurbsShell();
}



int GvNurbsSurface::Do(GglRenderTraversal &state)
{ 


#ifdef _OGL
	if (state.glState->nurbsMode == GGL_NURBS_OFF)
		return 0;
#endif
	
	// tbd: estimate new Budget
	// 
	if (!theShell) 
		BuildShell(state);


	if (!theShell) return (GNode::Do(state));

	if (!CullCheck(theShell,state)) 
		return TRAVERSE_CULLED;

	
	if (!state.appearanceApplied) {
		if (state.appearance)
			state.appearance->Apply(state);
	}

	// don't draw fully transparent geometry
	if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
		return TRAVERSE_CULLED;
    }

	// collect transparent shapes  ?
	if (((GGvglRenderTraversal *) & state)->IsDelayed())
	{
		if (!theShell) { // need shell for Bounding Box
		    state.glState->SetCulling(Solid(),!ccw.value);
			BuildShell(state);
		}
		if (theShell) {
			GTransparentElement * e= new GTransparentElement(state.appearance,this,theShell->GetBBox(),state.GetMatrixRef());
			((GGvglRenderTraversal *) & state)->AddTransparentElement(e);
		}
		return TRAVERSE_DELAYED;
	}

#ifdef _OGL_NURBS
	bool doOglRender = true;
	
	doOglRender = state.glState->nurbsMode == GGL_NURBS_GLU;

	if (doOglRender) {
			GvNurbsSurface* pSurface = 	this;

			getVec4f(pSurface->controlPoint,pSurface->weight,controlPoints);
 
			// tessellation
			int uSteps,vSteps;
			pSurface->getTessellation(uSteps,vSteps,state.nurbsTessellationScale); 
			gluNurbsProperty(nurbsObj, GLU_U_STEP, uSteps);
			gluNurbsProperty(nurbsObj, GLU_V_STEP, vSteps);
	
			//gluNurbsProperty(nurbsObj, GLU_DISPLAY_MODE, GLU_OUTLINE_PATCH);
			glEnable(GL_AUTO_NORMAL);
			glEnable(GL_NORMALIZE);
			glEnable(GL_MAP2_TEXTURE_COORD_2); 

			switch (state.glState->renderMode) {
			case GRenderWireframe:
				gluNurbsProperty(nurbsObj, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);
				break;
			default :
				gluNurbsProperty(nurbsObj, GLU_DISPLAY_MODE, GLU_FILL);
				break;
			}

			
			gluBeginSurface(nurbsObj );
					
					gluNurbsSurface(nurbsObj,
									pSurface->uKnot.getNum(), 
									pSurface->uKnot,
									pSurface->vKnot.getNum(),
									pSurface->vKnot,
									4,
									pSurface->uDimension*4,
									controlPoints,
									pSurface->uOrder,
									pSurface->vOrder,
									GL_MAP2_VERTEX_4);


					glCheckError();
			gluEndSurface(nurbsObj );
			glCheckError();

			frameStats.frame = state.glState->GetCurrentFrame();
			// set number of triangles drawn 
			frameStats.tris  = (uSteps-1)*(vSteps-1)*2;
			frameStats.prims = 1;

			state.frameStats.prims++;
			state.frameStats.tris += frameStats.tris;

			return (1);
	}

#endif	



	float newBudget = state.nurbsTessellationScale;

	GNurbsShell *nurbsShell = ((GNurbsShell *) theShell);

	nurbsShell->SetRenderQuality(newBudget);

	// apply shape hints   
    state.glState->SetCulling(Solid(),!ccw.value);
	frameStats.frame = state.glState->GetCurrentFrame();
	
	DWORD saveTrisDrawn = state.glState->trisDrawn;

	// apply traversal 
	int ret = theShell->Traverse(state);

	// set number of triangles drawn 
	frameStats.tris  = state.glState->trisDrawn-saveTrisDrawn;
	frameStats.prims = 1;

	state.frameStats.prims++;
	state.frameStats.tris += frameStats.tris;

	return(ret);
}


int GvNurbsSurface::Do(GRaySelectTraversal &state)
{  	
	// apply shape hints 

    state.SetCulling(Solid(),!ccw.value);

	// we need the shell, even if not rendered eg for Collision.proxy field
	if (!theShell) 	BuildShell(state); 

	if (!theShell) return (0);

	int ret;


	// apply traversal 
	//GPUSH_PATH(state,this);
		ret = theShell->Traverse(state);
	//GPOP_PATH(state);
	return ret;
}

int GvNurbsSurface::Do(GGeometryTraversal &state)
{ 	int ret;
 	
	// apply shape hints 
    state.SetCulling(Solid(),!ccw.value);

	if (!theShell) 	BuildShell(state); 

	if (!theShell) return (0);

	ret = theShell->Traverse(state);

	return ret;
}

int	GvNurbsSurface::BuildShell(GTraversal &state)
{	
	// verify correct values
	int i;

	if (!m_bInitialized) {
		m_bInitialized = TRUE;
		m_bBadValues = FALSE;
		GReporter* reporter = getReporter(state);

		if (uOrder > GNURBS_MAX_ORDER || uOrder < 1 || vOrder > GNURBS_MAX_ORDER || vOrder < 1)
		{
			//m_bBadValues = TRUE;
			if (reporter) {
				reporter->Error("NurbsSurface: Unsupported uOrder");
				//return(0);
			}
		}

		
		if (uKnot == NULL)
		{
			//o.k., a knot vector is computed
		}
		else if (uKnot.getNum() != (uDimension + uOrder)) 
		{
			//m_bBadValues = TRUE;
			if (reporter) {
				reporter->Error("NurbsSurface: Bad length of knot vector");
				//return(0);
			}
		}

		
		if (vKnot == NULL)
		{
			//o.k., a knot vector is computed
		}		
		else if (vKnot.getNum() != (vDimension + vOrder) ) 
		{
			//m_bBadValues = TRUE;
			if (reporter) {
				reporter->Error("NurbsSurface: Bad length of knot vector");
				//return(0);
			}
		}

		
		for(i=1; i<uKnot.getNum(); i++)
		{
			if (uKnot[i]<uKnot[i-1]) 
			{
			if (reporter) {
				reporter->Error("NurbsSurface: not increasing elements in uKnot!");
				reporter->Error("uKnot[%i]<uKnot[%i], %f<%f", i, i-1, uKnot[i], uKnot[i-1]);
				return(0);	
			}
			}
		}
	
		
		for(i=1; i<vKnot.getNum(); i++)
		{
			if (vKnot[i]<vKnot[i-1]) 
			{
			if (reporter) {
				reporter->Error("NurbsSurface: not increasing elements in vKnot!");
				reporter->Error("vKnot[%i]<vKnot[%i], %f<%f", i, i-1, vKnot[i], vKnot[i-1]);
				return(0);	
			}
			}
		}
		if (controlPoint.getNum() <= 1) {
			m_bBadValues = TRUE;
		} else 
		if (controlPoint.getNum() != (uDimension * vDimension) )
		{
			if (controlPoint.getNum() < (uDimension * vDimension))
				m_bBadValues = TRUE;

			if (reporter) {
				reporter->Error("NurbsSurface: Bad number of controlPoints");
				//return(0);
			}
		}
	
		if (weight.getNum() == 0) 
		{ 
			// ok, assumed to be 1.0f
		}

		else if (weight.getNum() == 1)
		{
			//o.k., this value is going to be the default weight
		}
		else 
		{
			if (weight.getNum() != (uDimension * vDimension)) 
			{
				m_bBadValues = TRUE;
				if (reporter) {
					reporter->Error("NurbsSurface: Bad length of weight");
				}
			}
		}
	}

	// bad data, so ignore 
	if (m_bBadValues)
		return (0);
	
	GNurbsShell *theShell = new GNurbsShell();
	//PointArray controlPoints;
	//controlPoint.get(controlPoints);

	if (controlPoint.getNum() >= (uDimension * vDimension)) {

		theShell->Init(uDimension, vDimension, uOrder, vOrder, 
			controlPoint.values, 
			
			weight.getNum(), weight.values, 
			uKnot.values, vKnot.values,

			uTessellation,vTessellation); 
		// theShell->ComputeShell(); only need for ray / cd etc 
		theShell->GetBBox();
	}
	SetShell(theShell);

	return 1;
}


// ! evaluate point at parameter u,v 
GvBool GvNurbsSurface::Eval(float u, float v, Point &p)
{
	return SurfacePoint(
				  u,  v, 
				  uOrder, vOrder, uDimension, vDimension,
				  uKnot, vKnot,
				  (const Point *) controlPoint.get(), weight,
				  &p.x
				  );
}


int GvNurbsSurface::OnFieldChanged(GvField *field)
{
	setRedraw();

	if (theShell) {
//TBD: could be overhead for multiple set1value call, set flags and update at render time ?
	if (field == &controlPoint) {
		((GNurbsShell *) theShell)->setControlPoints(controlPoint.getNum(),controlPoint.values);
	} else if (field == &weight) {
		((GNurbsShell *) theShell)->setWeight(weight.getNum(),weight.values);
	} else if ( (field == &uTessellation) ||  (field == &vTessellation)) {
		((GNurbsShell *) theShell)->InitTessellationValues(uTessellation,vTessellation);
	} else if ( (field == &solid) ||  (field == &ccw)) 	{ // ok 

	} else {
		SetShell(NULL);
		m_bInitialized = gfalse;
	}
	}
	
	GvNode::OnFieldChanged(field); // node shell clears shell !

	return 1;
}




//////////////////////////////////////////////////////////////	
//															//	
//															//	
//				T H E   N U R B S C U R V E					//
//															//
//////////////////////////////////////////////////////////////
 
  
GV_NODE_SOURCE_IMP(GvNurbsCurve,GvNodeShell);


GvNurbsCurve::GvNurbsCurve() :
        dimension(0),
        tessellation(0),
        knot(0),
        order(3),
        controlPoint(0),
        weight(0),
		changed(FALSE)

{
    GV_NODE_CONSTRUCTOR(GvNurbsCurve);

    GV_NODE_ADD_FIELD(dimension);
    GV_NODE_ADD_FIELD(tessellation);
    GV_NODE_ADD_FIELD(knot);
    GV_NODE_ADD_FIELD(order);
    GV_NODE_ADD_FIELD(controlPoint);
    GV_NODE_ADD_FIELD(weight);

	m_bInitialized = gfalse;
	m_bBadValues = gfalse;
    isBuiltIn = FALSE;

}


GvNurbsCurve::~GvNurbsCurve()
{}


GvBool 
GvNurbsCurve::Verify() 
{
	int degree = order-1;
	int dim = controlPoint.Length();

		m_bInitialized = TRUE;
		m_bBadValues = FALSE;

		if (dim <= 1) {
			m_bBadValues = TRUE;
			return 0;
		}

		if (order > GNURBS_MAX_ORDER || order < 1) {
			m_bBadValues = TRUE;
			if (m_browser) {
				((GView*) m_browser)->GetReporter()->Error("NurbsCurve: Unsupported order");
				return(0);
			}
		}

		if (knot.getNum() != (dim + degree + 1)) {
			m_bBadValues = TRUE;
			if (m_browser) {
				((GView*) m_browser)->GetReporter()->Error("NurbsCurve: Bad length of knot vector");
				((GView*) m_browser)->GetReporter()->Error("Got %i, Expected %i", knot.getNum(), dim + degree + 1);
				return(0);
			}
		}
#if 0		
		if (knot[dim+degree] > 1.f) {
			//m_bBadValues = TRUE;
			if (m_browser) {
				((GView*) m_browser)->GetReporter()->Error("NurbsCurve: knot vector must be normalized!");
				//return(0);
			}
		}
#endif


		for(int i=1; i<knot.getNum(); i++)
		{
			if (knot[i]<knot[i-1]) 
			{
			if (m_browser) {
				((GView*) m_browser)->GetReporter()->Error("NurbsCurve: not increasing elements!");
				return(0);	
			}
			}
		}
	
		if (weight.getNum() == 0) { // ok, assumed to be 1.0f
		}
		else {
			if (weight.getNum() != dim) {
				m_bBadValues = TRUE;
				if (m_browser) {
					((GView*) m_browser)->GetReporter()->Error("NurbsCurve: Bad length of keyWeight");
				}
			}
		}
	return !m_bBadValues;


}

int	GvNurbsCurve::BuildShell(GTraversal &state)
{
	// delete any old internal shell version
	SetShell(NULL);


	float u = 0.f;

	Point point;
	GFaceList f;

	PointArray pts;

	int i;
	int degree = order-1;
	int dim = controlPoint.Length();

	int tess=tessellation;

	if (tess <= 1)
		tess = dim*2;
	
	
	// verify correct values
	if (!m_bInitialized) {
		Verify();
	}

	// bad data, so ignore 
	if (m_bBadValues)
		return (0);

	

	//pts.SetLength(tess);
	f.SetMaxLength(tess+2);

	u = knot.values[degree];

	float inc = (knot.values[dim] - knot.values[degree]) /(float)tess;


	GPolyline *poly=new GPolyline ();

	
	f.Append(tess+1); // counter
	for (i = 0; i<=tess; i++)
	{
		// TRACE("#%i ", i);
		if (u > knot.values[dim])//due to rounding error
			u = knot.values[dim] - .0000001f;// if u = knot.values[degree] no span is found?

		CurvePoint (dim, degree, knot.values, controlPoint.values, weight.values, u, point);

		//pts.Set(i, point);
		pts.Append(point);
		f.Append(i);
		u+=inc;

	}


	poly->SetV(pts);
	poly->SetFaceList(f);
	
	poly->UpdateActions();

	SetShell(poly);

	changed = FALSE;

	return 1;
}


int GvNurbsCurve::OnFieldChanged(GvField *field)
{
	changed = TRUE;
	m_bInitialized = gfalse;
	setRedraw();
	return GvNode::OnFieldChanged(field);
	//return 1;
}


int GvNurbsCurve::Do(GglRenderTraversal &state)
{
	if (!theShell || changed) 
		BuildShell(state);
	
	if (!theShell) return (GNode::Do(state));

	if (!CullCheck(theShell,state)) return TRAVERSE_CULLED;

	if (!state.appearanceApplied) {
		if (state.appearance)
			state.appearance->Apply(state);
	}

	// don't draw fully transparent geometry
	if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
		return 0;
    }

	// collect transparent shapes  ?
	if ( ((GGvglRenderTraversal *) & state)->IsDelayed())
	{
		if (!theShell) { // need shell for Bounding Box
			BuildShell(state);
		}
		if (theShell) {
			GTransparentElement * e= new GTransparentElement(state.appearance,this,theShell->GetBBox(),state.GetMatrixRef());
			((GGvglRenderTraversal *) & state)->AddTransparentElement(e);
		}
		return TRAVERSE_DELAYED;
	}


	// apply traversal 
	int ret = theShell->Traverse(state);

	return 1;
}


int GvNurbsCurve::Do(GRaySelectTraversal &state)
{  	
	return (0); //not supported yet
	
	if (!theShell) 	BuildShell(state); 

	if (!theShell) return (0);

	// apply shape hints 
    state.SetCulling(FALSE,FALSE);

	// apply traversal 

	int ret;
	GPUSH_PATH(state,this);
		ret = theShell->Traverse(state);
	GPOP_PATH(state);
	return ret;
}

int GvNurbsCurve::Do(GBBoxTraversal &state)
{
	// if (!theShell) 		BuildShell(state);

	if (theShell) return GvNodeShell::Do(state);
	// compute BBOX from control points, use weights 
	Point p;

	Point mi,ma;

	if (controlPoint.Length()<=0) return(0);

	int num = controlPoint.Length();

	const float *w= weight.get();
	if (weight.Length() < num) w= NULL;

	controlPoint.get1(0, p);
	if (w) p*= 1.0f / w[0];

	mi= ma= p;

	for (int i=1; i<num;i++) {
		controlPoint.get1(i, p);
		if (w) p*= 1.0f / w[i];
		p.MinMax(mi,ma);
	}

	BBox box(mi,ma);

	state.Union(box);

    return(1);
}


//////////////////////////////////////////////////////////////	
//															//	
//															//	
//				T H E   N U R B S G R O U P					//
//															//
//////////////////////////////////////////////////////////////


GV_NODE_SOURCE_IMP(GvNurbsGroup,Gv2Group);


GvNurbsGroup::GvNurbsGroup() :
        tessellationScale(1.0f)
{
    GV_NODE_CONSTRUCTOR(GvNurbsGroup);

	// from Gv2Group
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);


    GV_NODE_ADD_FIELD(tessellationScale);
    
    GV_NODE_ADD_FIELD(children);

    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);

}

GvNurbsGroup::~GvNurbsGroup()
{

}


int GvNurbsGroup::Do(GBBoxTraversal &state)
{
	return Gv2Group::Do(state);
}


int GvNurbsGroup::Do(GglRenderTraversal &state)
{ 
	// tbd: estimate new Budget
	
	GFrameStats oldStats = state.frameStats;
	float oldScale = state.nurbsTessellationScale;

	// scale the tessellation factor
	if (tessellationScale >0) {
		state.nurbsTessellationScale *= tessellationScale;
		state.nurbsTessellationGroupS = (float) tessellationScale;
	}

	// frameTime

	int ret = Gv2Group::Do(state);
	//
	
	state.nurbsTessellationScale = oldScale;

	frameStats.tris = state.frameStats.tris - oldStats.tris;
	frameStats.prims = state.frameStats.prims - oldStats.prims;

	return ret;

}

int GvNurbsGroup::Do(GRaySelectTraversal &state)
{  	
	return Gv2Group::Do(state);
}

int GvNurbsGroup::Do(GGeometryTraversal &state)
{ 
	return Gv2Group::Do(state);
}



int GvNurbsGroup::OnFieldChanged(GvField *field)
{
	return Gv2Group::OnFieldChanged(field);
}

int GvNurbsGroup::Do(GPathTraversal &state)   
{ 
	return Gv2Group::Do(state);
}

int GvNurbsGroup::Do(GOptimizeTraversal &state)
{
	return Gv2Group::Do(state);
}


int GvNurbsGroup::Do(GTraversal &state)   
{ 
	return Gv2Group::Do(state);
}


// Bitmask


//////////////////////////////////////////////////////////////	
//															//	
//															//	
//				T H E   N U R B S C U R V E	2 D 2D 2D		//
//															//
//////////////////////////////////////////////////////////////

 
  
GV_NODE_SOURCE_IMP(GvNurbsCurve2D,GvNode);

//--------------------------------------------------
//--------------------------------------------------
GvNurbsCurve2D::GvNurbsCurve2D() :
        tessellation(0),
        knot(0),
        order(3),
        controlPoint(0),
        weight(0),
		changed(FALSE)

{
    GV_NODE_CONSTRUCTOR(GvNurbsCurve2D);

    GV_NODE_ADD_FIELD(tessellation);
    GV_NODE_ADD_FIELD(knot);
    GV_NODE_ADD_FIELD(order);
    GV_NODE_ADD_FIELD(controlPoint);
    GV_NODE_ADD_FIELD(weight);

	m_bInitialized = gfalse;
	m_bBadValues = gfalse;

}


GvNurbsCurve2D::~GvNurbsCurve2D()
{}


//--------------------------------------------------
//--------------------------------------------------
// ! evaluate as polyline with n- fixed steps
GvBool GvNurbsCurve2D::EvalPolyline(int nsteps, PointArray &pts)
{
	float u = 0.f;
	
	Point point;

	int i;
	int degree = order-1;
	int dim = controlPoint.Length();

	int tess=nsteps;

	if (tess <= 1)
		tess = dim*2;
		
	// verify correct values
	if (!m_bInitialized) {
		Verify();
	}

	// bad data, so ignore 
	if (m_bBadValues)
		return (gfalse);


	u = knot.values[degree];

	float inc = (knot.values[dim] - knot.values[degree]) /(float)tess;
	
	point.z = 0;
	pts.SetMaxLength(pts.Length()+tess+1);

	for (i = 0; i<=tess; i++)
	{
		// TRACE("#%i ", i);
		if (u > knot.values[dim])//due to rounding error
			u = knot.values[dim] - .0000001f;// if u = knot.values[degree] no span is found?

		CurvePoint2D (dim, degree, knot.values, controlPoint.values, weight.values, u, point);

		//pts.Set(i, point);
		pts.Append(point);
		u+=inc;
	}
	
	return TRUE;

}
//--------------------------------------------------
//--------------------------------------------------
//! evaluate to grid tesselator
GvBool GvNurbsCurve2D::Eval(GGridTess &tess)
{
	PointArray pts;
	int degree = order-1;
	int dim = controlPoint.Length();

	int steps=tessellation;

	if (steps <= 1)
		steps = dim*2;
		
	// verify correct values
	if (!m_bInitialized) {
		Verify();
	}

	// bad data, so ignore 
	if (m_bBadValues)
		return (gfalse);


	//EvalPolyline(0,pts);
	//tess.Polyline(pts.Length(),pts.Data());
	tess.NurbsCurve(dim, degree, knot.values, controlPoint.values, weight.values,steps); 

	return gtrue;
}

//--------------------------------------------------
//--------------------------------------------------
// ! evaluate at parameter u 
GvBool GvNurbsCurve2D::Eval(float u, Point &p)
{
	int degree = order-1;
	int dim = controlPoint.Length();
	
	// verify correct values
	if (!m_bInitialized) {
		Verify();
	}

	// bad data, so ignore 
	if (m_bBadValues)
		return (gfalse);

	p.z=0;
	CurvePoint2D (dim, degree, knot.values, controlPoint.values, weight.values, u, p);
	
	return TRUE;
}



//--------------------------------------------------
//--------------------------------------------------
GvBool 
GvNurbsCurve2D::Verify() 
{
	int degree = order-1;
	int dim = controlPoint.Length();

		m_bInitialized = TRUE;
		m_bBadValues = FALSE;

		if (dim <= 1) {
			m_bBadValues = TRUE;
			return 0;
		}

		if (order > GNURBS_MAX_ORDER || order < 1) {
			m_bBadValues = TRUE;
			if (m_browser) {
				((GView*) m_browser)->GetReporter()->Error("NurbsCurve: Unsupported order");
				return(0);
			}
		}

		if (knot.getNum() != (dim + degree + 1)) {
			m_bBadValues = TRUE;
			if (m_browser) {
				((GView*) m_browser)->GetReporter()->Error("NurbsCurve: Bad length of knot vector");
				((GView*) m_browser)->GetReporter()->Error("Got %i, Expected %i", knot.getNum(), dim + degree + 1);
				return(0);
			}
		}
		/*
		if (knot[dim+degree] > 1.f) {
			m_bBadValues = TRUE;
			if (m_browser) {
				((GView*) m_browser)->GetReporter()->Error("NurbsCurve: knot vector must be normalized!");
				return(0);
			}
		}
		*/

		for(int i=1; i<knot.getNum(); i++)
		{
			if (knot[i]<knot[i-1]) 
			{
			if (m_browser) {
				((GView*) m_browser)->GetReporter()->Error("NurbsCurve: not increasing elements!");
				return(0);	
			}
			}
		}
	
		if (weight.getNum() == 0) 
		{ // ok, assumed to be 1.0f
			weight.setLength(dim);
			for(int i=0; i<dim; i++)
				weight.set1(i, 1.f);
		}
		else {
			if (weight.getNum() != dim) {
				m_bBadValues = TRUE;
				if (m_browser) {
					((GView*) m_browser)->GetReporter()->Error("NurbsCurve: Bad length of keyWeight");
				}
			}
		}
	return !m_bBadValues;

}


//--------------------------------------------------
//--------------------------------------------------
int GvNurbsCurve2D::OnFieldChanged(GvField *field)
{
	changed = TRUE;
	m_bInitialized = gfalse;

	setRedraw();
	return GvNode::OnFieldChanged(field);
}
//--------------------------------------------------
//--------------------------------------------------
int GvNurbsCurve2D::Do(GBBoxTraversal &state)
{

	// compute BBOX from control points, use weights ???
	Point p;

	Point mi,ma;

	if (controlPoint.Length()<=0) return(0);

	int num = controlPoint.Length();

	const float *w= weight.get();
	if (weight.Length() < num) w= NULL;

	controlPoint.get1(0, p);
	if (w) p*= 1.0f / w[0];

	mi= ma= p;

	for (int i=1; i<num;i++) {
		controlPoint.get1(i, p);
		if (w) p*= 1.0f / w[i];
		p.MinMax(mi,ma);
	}

	BBox box(mi,ma);

	state.Union(box);

    return(1);
}



//--------------------------------------------------
//--------------------------------------------------
int GvNurbsCurve2D::Do(GglRenderTraversal &state)
{
/*
	if (!theShell || changed) 
		BuildShell(state);
	
	if (!theShell) return (GNode::Do(state));

	if (!CullCheck(theShell,state)) return TRAVERSE_CULLED;
*/

	if (!state.appearanceApplied) {
		if (state.appearance)
			state.appearance->Apply(state);
	}

	// don't draw fully transparent geometry
	if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
		return 0;
    }
/*
	// collect transparent shapes  ?
	if ( ((GGvglRenderTraversal *) & state)->IsDelayed())
	{
		if (!theShell) { // need shell for Bounding Box
			BuildShell(state);
		}
		if (theShell) {
			GTransparentElement * e= new GTransparentElement(state.appearance,this,theShell->GetBBox(),state.GetMatrixRef());
			((GGvglRenderTraversal *) & state)->AddTransparentElement(e);
		}
		return TRAVERSE_DELAYED;
	}


	// apply traversal 
	int ret = theShell->Traverse(state);
*/

	PointArray polyline;

	if (!EvalPolyline(tessellation,polyline)) {
		return 0;
	}

	GRenderer2D *renderer = state.glState->getRenderer2D();
	if (!renderer) return NULL;
	renderer->DrawPolyline(polyline.Length(),polyline);

	return 1;
}


GV_NODE_SOURCE_IMP(GvPolyline2D,GvNode);
//--------------------------------------------------
//--------------------------------------------------
GvPolyline2D::GvPolyline2D() :
			point(0), 
			changed(TRUE)

{
    GV_NODE_CONSTRUCTOR(GvPolyline2D);
    GV_NODE_ADD_FIELD(point);
}

//--------------------------------------------------
//--------------------------------------------------
GvPolyline2D::~GvPolyline2D()
{}


//--------------------------------------------------
// ! evaluate as polyline with n- fixed steps
//--------------------------------------------------
GvBool GvPolyline2D::EvalPolyline(int nsteps, PointArray &pts)
{
	int tess=point.getNum();

	Point p;

	p.z = 0;

	pts.SetMaxLength(pts.Length()+tess);

	for (int i = 0; i<tess; i++)
	{
		point.get1(i,p);
		//pts.Set(i, point);
		pts.Append(p);
	}
	
	return TRUE;

}

//--------------------------------------------------
//! evaluate to grid tesselator
//--------------------------------------------------
GvBool GvPolyline2D::Eval(GGridTess &tess)
{
	PointArray pts;
	EvalPolyline(0,pts);
	tess.Polyline(pts.Length(),pts.Data());
	return pts.Length()>1;
}

/*
// ! evaluate at parameter u 
GvBool GvPolyline2D::Eval(float u, Point &p)
{
	p.z=0;
	int i = u* (point.getNum() -1);
	if (i <0) return FALSE;
	if (i >= point.getNum()) return FALSE;

	point.get1(i,p);
	
	
	return TRUE;
}
*/


//--------------------------------------------------
//--------------------------------------------------
int GvPolyline2D::OnFieldChanged(GvField *field)
{
	changed = TRUE;
	setRedraw();
	return GvNode::OnFieldChanged(field);
}

//--------------------------------------------------
//--------------------------------------------------
int GvPolyline2D::Do(GBBoxTraversal &state)
{
	// compute BBOX from points
	Point p;
	Point mi,ma;

	if (point.Length()<=0) return(0);
	int num = point.Length();

	point.get1(0, p);

	mi= ma= p;

	for (int i=1; i<num;i++) {
		point.get1(i, p);
		p.MinMax(mi,ma);
	}

	BBox box(mi,ma);

	state.Union(box);

    return(1);
}



//--------------------------------------------------
//--------------------------------------------------
int GvPolyline2D::Do(GglRenderTraversal &state)
{

	if (!state.appearanceApplied) {
		if (state.appearance)
			state.appearance->Apply(state);
	}

	// don't draw fully transparent geometry
	if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
		return 0;
    }

	PointArray polyline;

	if (!EvalPolyline(0,polyline)) {
		return 0;
	}

	GRenderer2D *renderer = state.glState->getRenderer2D();
	if (!renderer) return 0;
	renderer->DrawPolyline(polyline.Length(),polyline);

	return 1;
}


GV_NODE_SOURCE_IMP(GvContour2D,Gv2Group);
//--------------------------------------------------
//--------------------------------------------------
GvContour2D::GvContour2D() :
			fill(TRUE),
			changed(TRUE)

{
    GV_NODE_CONSTRUCTOR(GvContour2D);
    isBuiltIn = FALSE;

	// from Gv2group
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);

    GV_NODE_ADD_FIELD(fill);

    // from Gv2group
    GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);


   
}

//--------------------------------------------------
//--------------------------------------------------
GvContour2D::~GvContour2D()
{}


//--------------------------------------------------
//--------------------------------------------------
//! evaluate as polyline
GvBool GvContour2D::EvalPolyline(int nsteps, PointArray &pts)
{

	for (int i = 0; i<children.getNum(); i++) {
		GvNode *n=children.get1(i);
		if(!n) continue;
		n = n->Eval(); 
		if(!n) continue;

		if (RTISA(n,GvPolyline2D)) { 
			GvPolyline2D * c= (GvPolyline2D *) n;
			c->EvalPolyline(0,pts);
			continue;
		}
		if (RTISA(n,GvNurbsCurve2D)) { 
			GvNurbsCurve2D * c= (GvNurbsCurve2D *) n;
			c->EvalPolyline(c->tessellation,pts);
			continue;
		}

		TRACE("	GvContour2D::EvalPolyline bad node \n");
		ASSERT(0);


	}
	return pts.Length()>0;

}

//--------------------------------------------------
//! evaluate to grid tesselator
//--------------------------------------------------
GvBool GvContour2D::Eval(GGridTess &tess)
{

	tess.BeginContour();

	for (int i = 0; i<children.getNum(); i++) {
		GvNode *n=children.get1(i);
		if(!n) continue;
		n = n->Eval(); 
		if(!n) continue;

		if (RTISA(n,GvPolyline2D)) { 
			GvPolyline2D * c= (GvPolyline2D *) n;
			c->Eval(tess);
			continue;
		}
		if (RTISA(n,GvNurbsCurve2D)) { 
			GvNurbsCurve2D * c= (GvNurbsCurve2D *) n;
			c->Eval(tess);
			continue;
		}

		TRACE("	GvContour2D::Eval bad node \n");
		ASSERT(0);


	}
	tess.EndContour();
	return TRUE;

}



//--------------------------------------------------
//--------------------------------------------------
int GvContour2D::OnFieldChanged(GvField *field)
{
	changed = TRUE;
	setRedraw();
	return Gv2Group::OnFieldChanged(field);
}

//--------------------------------------------------
//--------------------------------------------------
int GvContour2D::OnChildChanged(GvNode *child)
{
	changed = TRUE;
	setRedraw();
	return Gv2Group::OnChildChanged(child);
}



//--------------------------------------------------
//--------------------------------------------------
int GvContour2D::Do(GBBoxTraversal &state)
{
	return children.Traverse(state);
}



//--------------------------------------------------
//--------------------------------------------------
int GvContour2D::Do(GglRenderTraversal &state)
{

	if (!state.appearanceApplied) {
		if (state.appearance)
			state.appearance->Apply(state);
	}

	// don't draw fully transparent geometry
	if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
		return 0;
    }

	PointArray pts;

	if (0)	{  // gridify test 

	Point pmin;
	Point pmax;

	pmin.Set(-10,-10);
	pmax.Set(10,10);


	GGridTess tess(pmin,pmax);
	tess.SetSteps(20,20);

	tess.BeginContour();

	for (int i = 0; i<children.getNum(); i++) {
		GvNode *n=children.get1(i);
		if(!n) continue;
		n = n->Eval(); 
		if(!n) continue;
		
		pts.SetLength(0);

		if (RTISA(n,GvPolyline2D)) { 
			GvPolyline2D * c= (GvPolyline2D *) n;
			c->EvalPolyline(0,pts);
			tess.Polyline(pts.Length(),pts.Data());
			continue;
		}
		if (RTISA(n,GvNurbsCurve2D)) { 
			GvNurbsCurve2D * c= (GvNurbsCurve2D *) n;
			/*
				c->EvalPolyline(c->tessellation,pts);
				tess.Polyline(pts.Length(),pts.Data());
			*/
			if (!c->m_bInitialized) {
				c->Verify();
			}

			if (!c->m_bBadValues) {
				int steps = c->tessellation;
				if (steps<=1) steps = 2.0*c->controlPoint.Length();
				tess.NurbsCurve(c->controlPoint.Length(), c->order-1, c->knot.values, c->controlPoint.values, c->weight.values,steps); 
			}

			continue;
		}

		TRACE("	GvContour2D::EvalPolyline bad node \n");
		ASSERT(0);


	}
	tess.EndContour();

	GRenderer2D *renderer = state.glState->getRenderer2D();
	if (!renderer) return NULL;

	renderer->DrawGrid(pmin,pmax,tess.usteps,tess.vsteps);

	for (i=0; i<tess.NumContours();i++) {
		tess.GetContour(i,pts);
		renderer->DrawPolyline(pts.Length(),pts);
		renderer->SetPointSize(5.0);
		renderer->DrawPoint(pts.Length(),pts);
		renderer->SetPointSize(1.0);
	}
	
	return 1 ;

	}


	GRenderer2D *renderer = state.glState->getRenderer2D();
	if (!renderer) return NULL;
	
	state.glState->SetCulling(FALSE,TRUE);
	state.glState->SetLighting(0);

	if (fill && 0) { // tdo problem in tesselator

		renderer->BeginPolygon();
			renderer->BeginContour();
				children.Traverse(state);
			renderer->EndContour();
		renderer->EndPolygon();
	}
	else 
		children.Traverse(state);


	return 1;
}




//////////////////////////////////////////////////////////////	
//															//	
//															//	
//			T H E    T R I M M E D S U R F A C E			//
//															//
//////////////////////////////////////////////////////////////


GV_NODE_SOURCE_IMP(GvTrimmedSurface,GvNodeShell);

//--------------------------------------------------

//--------------------------------------------------
GvTrimmedSurface::GvTrimmedSurface() :trimmingContour(0) 
{
    GV_NODE_CONSTRUCTOR(GvTrimmedSurface);


    GV_NODE_ADD_FIELD(trimmingContour);
    GV_NODE_ADD_FIELD(surface);

    GV_NODE_ADD_EVENT_IN(addTrimmingContour,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeTrimmingContour,GvMFNode);

	m_bInitialized = gfalse;
	m_bBadValues = gfalse;


#ifdef _OGL_NURBS
	nurbsObj = gluNewNurbsRenderer();

    gluNurbsProperty(nurbsObj, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
//    gluNurbsProperty(nurbsObj, GLU_V_STEP, 20);
//    gluNurbsProperty(nurbsObj, GLU_U_STEP, 20);

	ASSERT(nurbsObj != NULL);

#endif
}


//--------------------------------------------------
//--------------------------------------------------
GvTrimmedSurface::~GvTrimmedSurface()
{
#ifdef _OGL_NURBS	
	if (nurbsObj)
		gluDeleteNurbsRenderer(nurbsObj );
#endif
}


// Event IN
void GvTrimmedSurface::addTrimmingContour(GvMFNode *event)
{
    TRACE("%s::addChildren  !",ClassName());
    if (!event || event->Length() == 0) return;

    trimmingContour.add(event);
    trimmingContour.setDefault(FALSE);
    trimmingContour.OnChanged();
}

void GvTrimmedSurface::removeTrimmingContour(GvMFNode *event)
{
    TRACE("%s::removeChildren  !",ClassName());
    if (!event || event->Length() == 0) return;
    trimmingContour.remove(event);
    trimmingContour.OnChanged();
}



//--------------------------------------------------
//! evaluate trimming to grid tesselator
//--------------------------------------------------
GvBool GvTrimmedSurface::EvalTrimming(GTraversal &state,GGridTess &tess)
{
	if (trimmingContour.getNum() == 0)
		return gfalse;


	for (int i = 0; i<trimmingContour.getNum(); i++) {
		GvNode *n=trimmingContour.get1(i);

		if(!n) continue;
		n = n->Eval(state); 
		if(!n) continue;

		if (RTISA(n,GvPolyline2D)) { 
			GvPolyline2D * c= (GvPolyline2D *) n;
			tess.BeginContour();
				c->Eval(tess);
			tess.EndContour();
			continue;
		}
		if (RTISA(n,GvNurbsCurve2D)) { 
			GvNurbsCurve2D * c= (GvNurbsCurve2D *) n;
			tess.BeginContour();
				c->Eval(tess);
			tess.EndContour();
			continue;
		}

		if (RTISA(n,GvContour2D)) { 
			GvContour2D * c= (GvContour2D *) n;
		
			c->Eval(tess);
			continue;
		}

		TRACE("GvTrimmedSurface::Eval bad node \n");
		ASSERT(0);

	}
	return TRUE;

}






int GvTrimmedSurface::Do(GBBoxTraversal &state)
{
	if (!theShell || !m_bInitialized) BuildShell(state); 
	return GvNodeShell::Do(state);
}


#ifdef _OGL

int GvTrimmedSurface::DoOGL(GglRenderTraversal &state)
{

	GvNurbsSurface* pSurface = 	getNurbsSurface(state);

	if (!pSurface) return 0;

	if (!state.appearanceApplied) {
		if (state.appearance)
			state.appearance->Apply(state);
	}

	// don't draw fully transparent geometry
	if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
		return TRAVERSE_CULLED;
    }

	// control points in vec4f format
	getVec4f(pSurface->controlPoint,pSurface->weight,controlPoints);

	// tessellation
	int uSteps,vSteps;
	pSurface->getTessellation(uSteps,vSteps,state.nurbsTessellationScale); 
    gluNurbsProperty(nurbsObj, GLU_U_STEP, uSteps);
    gluNurbsProperty(nurbsObj, GLU_V_STEP, vSteps);
	
	//GLU_OUTLINE_POLYGON 

    //gluNurbsProperty(nurbsObj, GLU_DISPLAY_MODE, GLU_OUTLINE_PATCH);
	glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
	glEnable(GL_MAP2_TEXTURE_COORD_2); 

	switch (state.glState->renderMode) {
	case GRenderWireframe:
		gluNurbsProperty(nurbsObj, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);
		break;
	default :
		gluNurbsProperty(nurbsObj, GLU_DISPLAY_MODE, GLU_FILL);
		break;
	}
	
	gluBeginSurface(nurbsObj );
				
			gluNurbsSurface(nurbsObj,
							pSurface->uKnot.getNum(), 
							pSurface->uKnot,
							pSurface->vKnot.getNum(),
							pSurface->vKnot,
							4,
							pSurface->uDimension*4,
							controlPoints,
							pSurface->uOrder,
							pSurface->vOrder,
							GL_MAP2_VERTEX_4);


			glCheckError();

			// now the trim curves if any
			if (trimmingContour.getNum() >0) {
		
			for (int i = 0; i<trimmingContour.getNum(); i++) {
				GvNode *n=trimmingContour.get1(i);

				if(!n) continue;
				n = n->Eval(state); 
				if(!n) continue;

				if (RTISA(n,GvPolyline2D)) { 
					GvPolyline2D * c= (GvPolyline2D *) n;
					gluBeginTrim(nurbsObj);

						gluPwlCurve (nurbsObj,
									c->point.getNum(),
									c->point,
									2,
									GLU_MAP1_TRIM_2);

					gluEndTrim(nurbsObj);	

					continue;
				}
				if (RTISA(n,GvNurbsCurve2D)) { 
					GvNurbsCurve2D * c= (GvNurbsCurve2D *) n;

					gluBeginTrim(nurbsObj);
					if (c->weight.getNum()>0) {
						FloatArray controlPoint;
						getVec3f(c->controlPoint,c->weight,controlPoint);
						gluNurbsCurve(nurbsObj,
									c->knot.getNum(),c->knot,
									3,controlPoint,
									c->order,GLU_MAP1_TRIM_2);

					} else 
						gluNurbsCurve(nurbsObj,
									c->knot.getNum(),c->knot,
									2,c->controlPoint,
									c->order,GLU_MAP1_TRIM_2);

					gluEndTrim(nurbsObj);	

					continue;
				}	
				if (RTISA(n,GvContour2D)) { 
					GvContour2D * g= (GvContour2D *) n;
			  	    gluBeginTrim(nurbsObj);
					for (int i = 0; i<g->children.getNum(); i++) {
						GvNode *n=g->children.get1(i);

						if(!n) continue;
						n = n->Eval(state); 
						if(!n) continue;

						if (RTISA(n,GvPolyline2D)) { 
							GvPolyline2D * c= (GvPolyline2D *) n;

							gluPwlCurve (nurbsObj,
									c->point.getNum(),
									c->point,
									2,
									GLU_MAP1_TRIM_2);

						continue;
						}
						if (RTISA(n,GvNurbsCurve2D)) { 
							GvNurbsCurve2D * c= (GvNurbsCurve2D *) n;

							if (c->weight.getNum()>0) {
								FloatArray controlPoint;
								getVec3f(c->controlPoint,c->weight,controlPoint);
								gluNurbsCurve(nurbsObj,
									c->knot.getNum(),c->knot,
									3,controlPoint,
									c->order,GLU_MAP1_TRIM_2);

							} else 
								gluNurbsCurve(nurbsObj,
									c->knot.getNum(),c->knot,
									2,c->controlPoint,
									c->order,GLU_MAP1_TRIM_2);


						continue;
						}	

					TRACE("GvTrimmedSurface::Eval bad node \n");
					ASSERT(0);

					}
				
					gluEndTrim(nurbsObj);	
					glCheckError();


					continue;
				}

			TRACE("GvTrimmedSurface::Eval bad node \n");
			ASSERT(0);

		}
		
		glCheckError();

		} // trim 

	gluEndSurface(nurbsObj );
	glCheckError();
	frameStats.frame = state.glState->GetCurrentFrame();
	// set number of triangles drawn 
	frameStats.tris  = (uSteps-1)*(vSteps-1)*2;
	frameStats.prims = 1;
	state.frameStats.prims++;
	state.frameStats.tris += frameStats.tris;



	return (1);
}

#endif



int GvTrimmedSurface::Do(GglRenderTraversal &state)
{ 

	gbool doOglRender = gfalse;
	gbool drawTrimming = gfalse;
	gbool evalTrimming = gtrue;

#ifdef _OGL
	if (state.glState->nurbsMode == GGL_NURBS_OFF)
		return 0;
#endif

	
	if (!theShell || !m_bInitialized) 	
		BuildShell(state); 


#ifdef _OGL_NURBS
	doOglRender = state.glState->nurbsMode == GGL_NURBS_GLU;

	//doOglRender = gtrue;
	
	// render NURBS using GLU Nurbs tesselator

	if (doOglRender) 
		return DoOGL(state);
	
#endif	

	

	if (!theShell) return (GNode::Do(state));

	if (!CullCheck(theShell,state)) 
		return TRAVERSE_CULLED;

	
	if (!state.appearanceApplied) {
		if (state.appearance)
			state.appearance->Apply(state);
	}

	// don't draw fully transparent geometry
	if (state.materialStatus & MATERIAL_FULLY_TRANSPARENT) {
		return TRAVERSE_CULLED;
    }

	
	GvNurbsSurface *s=getNurbsSurface(state);
	if (!s) return 0;


	// collect transparent shapes  ?
	if (((GGvglRenderTraversal *) & state)->IsDelayed())
	{
		if (!theShell) { // need shell for Bounding Box
		    state.glState->SetCulling(s->Solid(),!s->ccw.value);
			BuildShell(state);
		}
		if (theShell) {
			GTransparentElement * e= new GTransparentElement(state.appearance,this,theShell->GetBBox(),state.GetMatrixRef());
			((GGvglRenderTraversal *) & state)->AddTransparentElement(e);
		}
		return TRAVERSE_DELAYED;
	}

	GNurbsShell *nurbsShell = s->getNurbsShell(state);

	float newBudget = state.nurbsTessellationGroupS;
	((GTrimmedNurbsShell* )theShell)->SetRenderQuality(newBudget);

	// apply shape hints   
    state.glState->SetCulling(s->Solid(),!s->ccw.value);
	frameStats.frame = state.glState->GetCurrentFrame();
	
	DWORD saveTrisDrawn = state.glState->trisDrawn;

	// apply traversal 
	int ret = theShell->Traverse(state);

	// set number of triangles drawn 
	frameStats.tris  = state.glState->trisDrawn-saveTrisDrawn;
	frameStats.prims = 1;

	state.frameStats.prims++;
	state.frameStats.tris += frameStats.tris;

	GvNurbsSurface* surf = getNurbsSurface(state);


	if (drawTrimming && surf) { // draw the trimming curves 


		Point pmin,pmax;
		int nU,nV;

		nurbsShell->GetURange(pmin.x,pmax.x);
		nurbsShell->GetVRange(pmin.y,pmax.y);

		GGridTess tess(pmin,pmax);

		nurbsShell->GetTessellation(nU, nV); 

		tess.SetSteps(nU,nV);

		if ( EvalTrimming(state,tess)) {
		GRenderer2D *renderer = state.glState->getRenderer2D();
		
		if (!renderer) return 0;

		//renderer->DrawGrid(pmin,pmax,tess.usteps,tess.vsteps);
		GNurbsShell* nurbsShell = surf->getNurbsShell(state);


		PointArray pts;
		if (1) { // map pts back to surface
			GGridPoint *p =tess.curveP.DataRW();
			for (int j=0;j<tess.curveP.Length();j++,p++) {

				//if (!surf->Eval(p->p.x, p->p.y,p->p)) {
				//	TRACE("Point outside NurbsSurface !\n");
				//}	
				// map points in a quad to triangulation ??
				if (!nurbsShell->EvaluateOnTessellation(p->p.x, p->p.y,p->p)) {
					TRACE("Point outside NurbsSurface !\n");
				}	
			}	

		}

		for (int i=0; i<tess.NumContours();i++) {
			tess.GetContour(i,pts);
			renderer->DrawPolyline(pts.Length(),pts);
			renderer->SetPointSize(5.0);
			renderer->DrawPoint(pts.Length(),pts);
			renderer->SetPointSize(1.0);
		}
		}

	}	


	return(ret);
}

int GvTrimmedSurface::Do(GRaySelectTraversal &state)
{  	

	GvNurbsSurface *s=getNurbsSurface(state);

	if (!s) return 0;

	// apply shape hints 

    state.SetCulling(s->Solid(),!s->ccw.value);

	// we need the shell, even if not rendered eg for Collision.proxy field
	if (!theShell || !m_bInitialized) 	BuildShell(state); 

	if (!theShell) return (0);

	int ret;

	// apply traversal 
	//GPUSH_PATH(state,this);
		ret = theShell->Traverse(state);
	//GPOP_PATH(state);
	return ret;
}


int GvTrimmedSurface::Do(GGeometryTraversal &state)
{ 
	return GvNodeShell::Do(state);
}


int GvTrimmedSurface::OnFieldChanged(GvField *field)
{
	setRedraw();
	
	GTrimmedNurbsShell *s = getNurbsShell();
	if (field == &trimmingContour && s) {
		s->SetTrimmingChanged();			
	}
	m_bInitialized = gfalse;
	return GvNodeShell::OnFieldChanged(field);
}

// some child node has been changed
int GvTrimmedSurface::OnChildChanged(GvNode *child)
{

	if (RTISA(child,GvNurbsSurface) && surface.get() == child) {
		TRACE("Sub surface had been changed \n");
		GTrimmedNurbsShell *s = getNurbsShell();
		if (s) {
			s->SetSurfaceChanged();			
		}
		else m_bInitialized = gfalse;
		
		return 1;
	}
	return GvNodeShell::OnChildChanged(child);
}

int GvTrimmedSurface::Do(GPathTraversal &state)   
{ 
	return GvNodeShell::Do(state);
}


int GvTrimmedSurface::Do(GTraversal &state)   
{ 
	return GvNodeShell::Do(state);
}

// get the base NURBS Surface
GvNurbsSurface* GvTrimmedSurface::getNurbsSurface(GTraversal &state)
{
	// get property child nodes as proper typed nodes
    GvNode *tmp = this->surface.Eval(state);
	GvNurbsSurface *surf = tmp ? (RTCAST(tmp,GvNurbsSurface)) : NULL;
	return surf;

}




//////////////////////////////////////////////////////////
//														//
//														//
//		T R I M M E D S U R F A C E						//
//														//
//////////////////////////////////////////////////////////



//--------------------------------------------------
//--------------------------------------------------

GvBool 
GvTrimmedSurface::Verify(GvNurbsCurve2D* curve2D) 
{


	int degree = curve2D->order-1;
	int dim = curve2D->controlPoint.Length();

		m_bInitialized = TRUE;
		m_bBadValues = FALSE;

		if (dim <= 1) {
			m_bBadValues = TRUE;
			if (m_browser) {
				((GView*) m_browser)->GetReporter()->Error("NurbsCurve2D: Bad length of control point vector");
			}
			return 0;
		}

		if (curve2D->order > GNURBS_MAX_ORDER || curve2D->order < 1) {
			m_bBadValues = TRUE;
			if (m_browser) {
				((GView*) m_browser)->GetReporter()->Error("NurbsCurve2D: Unsupported order");
			}
			return(0);
		}

		if (curve2D->knot.getNum() != (dim + degree + 1)) {
			m_bBadValues = TRUE;
			if (m_browser) {
				((GView*) m_browser)->GetReporter()->Error("NurbsCurve2D: Bad length of knot vector");
				((GView*) m_browser)->GetReporter()->Error("Got %i, Expected %i", curve2D->knot.getNum(), dim + degree + 1);
			}
			return(0);
		}
		
		//if the curve is not normalized,
    /*
		if (curve2D->knot[0] != 0.f || curve2D->knot[dim+degree] != 1.f) 
		{
			float length = curve2D->knot[dim+degree] - curve2D->knot[0];
			float offset = curve2D->knot[0];
			for(int i=0; i<curve2D->knot.getNum(); i++)
			{
				curve2D->knot[i] -= offset; 		
				curve2D->knot[i] /= length; 
			}
		}
    */

		for(int i=1; i<curve2D->knot.getNum(); i++)
		{
			if (curve2D->knot[i-1]>curve2D->knot[i]) 
			{
			if (m_browser) {
				((GView*) m_browser)->GetReporter()->Error("NurbsCurve2D: nonincreasing elements!");
				return(0);	
			}
			}
		}
 
		if (curve2D->weight.getNum() == 0) 
		{ // ok, assumed to be 1.0f
			curve2D->weight.makeRoom(dim);
			for(int i=0; i<dim; i++)
				curve2D->weight[i]=1.f;
		}
		else {
			if (curve2D->weight.getNum() != dim) {
				m_bBadValues = TRUE;
				if (m_browser) {
					((GView*) m_browser)->GetReporter()->Error("NurbsCurve2D: Bad length of keyWeight");
				}
			}
		}

 
	return !m_bBadValues;


}



//--------------------------------------------------
//BuildShell(GTraversal &state)
//--------------------------------------------------

int GvTrimmedSurface::BuildShell(GTraversal &state)   
{ 

#ifdef _OGL_NURBS

	GvNurbsSurface* pSurface = getNurbsSurface(state);
	
	if (!pSurface) return 0;

	getVec4f(pSurface->controlPoint,pSurface->weight,controlPoints);

#endif	




	GReporter* reporter = getReporter(state);
	
	// delete any old internal shell version
	SetShell(NULL);

	m_bInitialized = gtrue;
	m_bBadValues = gtrue;

	// get property child nodes as proper typed nodes
    GvNode *tmp = this->surface.Eval(state);
	GvNurbsSurface *surf = tmp ? (RTCAST(tmp,GvNurbsSurface)) : NULL;

	if (!surf) {
		TRACE(" GvTrimmedSurface::BuildShell no surface \n");
		return -1;
	}

    GTrimmedNurbsShell *s = new GTrimmedNurbsShell();

	if (!s->SetSurface(surf->getNurbsShell(state)))
	{
		reporter->Warning("Trimmed Surface, bad surface");
		return 0;
	}

	// set trimming curves
	for (int i=0;i<trimmingContour.getNum();i++) {
		// get the iths one 
		tmp=trimmingContour[i];
		if (!tmp) continue;
	    tmp = tmp->Eval(state);


		//tmp may be a trimming contour containing curves or a simple curve
		GvContour2D *contour2D = tmp ? (RTCAST(tmp, GvContour2D)) : NULL;
		GvNurbsCurve2D *curve2D = tmp ? (RTCAST(tmp,GvNurbsCurve2D)) : NULL;

		if (contour2D)
			//loop over all childs
		{
			s->BeginTrim();
			for(int j = 0; j<contour2D->children.getNum(); j++)
			{
				GvNode *node = contour2D->children[j];
				GvNurbsCurve2D *curve2D = node ? (RTCAST(node,GvNurbsCurve2D)) : NULL;
				GvPolyline2D *poly2D = node ? (RTCAST(node,GvPolyline2D)) : NULL;
				if (curve2D) 
				{
					if(!Verify(curve2D))
					{
						reporter->Warning("Trimmed Surface, bad trimming curve within contour2D#%d",i);	
						break;
					}
										
					if (!s->AddNurbsTrimmingCurve2D(
						curve2D->order,
						curve2D->controlPoint.getNum(), curve2D->controlPoint.values, 
						curve2D->weight.getNum(), curve2D->weight.values, 
						curve2D->knot.getNum(),curve2D->knot.values, 
						curve2D->tessellation
						)) 
					{
						reporter->Warning("Trimmed Surface, bad trimming curve in contour2D#%d",i);
					}	
				}
				else if (poly2D)
				{
					s->AddPolyline2D(poly2D->point.getNum(), poly2D->point);
				}
				else //error
					{
						reporter->Warning("Trimmed Surface, unknown trimming curve within contour2D#%d",i);	
						break;
					}
					
			
			}
			s->EndTrim();

		}

		else if (curve2D)// is it a curve?
		{
				s->BeginTrim();
			
				if(!Verify(curve2D))
					{
						reporter->Warning("Trimmed Surface, bad trimming curve within contour2D#%d",i);	
						break;
					}
						
				if (!s->AddNurbsTrimmingCurve2D(
					curve2D->order,
					curve2D->controlPoint.getNum(), curve2D->controlPoint.values, 
					curve2D->weight.getNum(), curve2D->weight.values, 
					curve2D->knot.getNum(),curve2D->knot.values, 
					curve2D->tessellation
					)) 
				{
					reporter->Warning("Trimmed Surface, bad trimming curve #%d",i);
				}
				s->EndTrim();

		}
		else
				{
					reporter->Warning("Trimmed Surface, unknown trimming curve #%d",i);	
					continue;
				}

	}
	SetShell(s);
	m_bBadValues = gfalse;
	m_bInitialized = gtrue;

	return 0;
}







//////////////////////////////////////////////////////////
//														//
//														//
//		T H E   N U R B S I N T E R P O L A T O R		//
//														//
//////////////////////////////////////////////////////////

GV_NODE_SOURCE_IMP(GvNurbsPositionInterpolator,GvInterpolator);


GvNurbsPositionInterpolator::GvNurbsPositionInterpolator() :
		dimension(0),
		knot(0),
		order(3),
		fractionAbsolute(TRUE),
		keyWeight(0),
		keyValue(0)
{
    GV_NODE_CONSTRUCTOR(GvNurbsPositionInterpolator);

    GV_NODE_ADD_FIELD(dimension);
    GV_NODE_ADD_FIELD(knot);
    GV_NODE_ADD_FIELD(order);
    GV_NODE_ADD_FIELD(fractionAbsolute);
    GV_NODE_ADD_FIELD(keyWeight);
    GV_NODE_ADD_FIELD(keyValue);
	GV_NODE_ADD_FIELD_EVENT_IN(fraction);
    GV_NODE_ADD_EVENT_OUT(value);
	m_bInitialized = gfalse;
	m_bBadValues = gfalse;
}

GvNurbsPositionInterpolator::~GvNurbsPositionInterpolator()
{

}

int GvNurbsPositionInterpolator::OnFieldChanged(GvField *field)
{
	if (field == &fraction) { // its the set_fraction eventIn
	    if (!Evaluating()) {
		    return(OnEvaluate(fraction.value));
		}
		else return 0;
	}
	
	m_bInitialized = gfalse;
	return GvInterpolator::OnFieldChanged(field);
}



// 
// get point on curve 
// page 82
// 
//extended to A4.1 on page 124

int GvNurbsPositionInterpolator::CurvePoint (int n, int p, const float *U, const float *P, const float *w, float u)
{
	//Compute point on rational B-spline curve
	//Input: n,p,U,P,w, u

	// n: dimension (number of points)
	// p: degree (3)
	// U: knot vector
	// P: control Points
	// w: weights
	// u: fraction

	//Output: Cx, Cy, Cz

	int    span; 
	float Nf[GNURBS_MAX_ORDER]; 
	float Cw, Cwx, Cwy, Cwz; 

	span = FindSpan (n, p, u, U);  
	BasisFuns (span, u, p, U, Nf);  
	Cw = Cwx = Cwy = Cwz = 0.; 
	
	if (w == NULL) Cw= 1.0f;

	for (int i=0; i<=p; i++)
	{
		Cwx = Cwx +  Nf[i] * P[(span-p+i)*3];
		Cwy = Cwy +  Nf[i] * P[(span-p+i)*3+1];
		Cwz = Cwz +  Nf[i] * P[(span-p+i)*3+2];
		if (w) Cw  = Cw  +  Nf[i]  * w[span-p+i];
		//TRACE("CV%i: %f %f %f\n", i, P[(span-p+i)*3], P[(span-p+i)*3+1], P[(span-p+i)*3+2]);
	}

	Cw = 1.0f/Cw;
	value.value[0] = Cwx*Cw;
	value.value[1] = Cwy*Cw;
	value.value[2] = Cwz*Cw;

	//TRACE("GvNurbsPositionInterpolator::CurvePoint: fraction %f  x y z: %f %f %f \n", u, value.value[0], value.value[1], value.value[2]);
	return 1;
}

GvNurbsPositionInterpolator::OnEvaluate(float f)
{
	
	int degree;
	degree = order - 1;	
	float fraction;
	
	int dim = keyValue.Length();	
	
	// verify correct values
	if (!m_bInitialized) {
		m_bInitialized = TRUE;
		m_bBadValues = FALSE;

		if (order > GNURBS_MAX_ORDER || order < 1) {
			m_bBadValues = TRUE;
			if (m_browser) {
				((GView*) m_browser)->GetReporter()->Error("NurbsPositionInterpolator: Unsupported order");
				return(0);
			}
		}

		// the knot-vector
		if (knot.getNum() != (dim + degree + 1)) {
			m_bBadValues = TRUE;
			if (m_browser) {
				((GView*) m_browser)->GetReporter()->Error("NurbsPositionInterpolator: Bad length of knot vector");
				((GView*) m_browser)->GetReporter()->Error("Got %i, Expected %i", knot.getNum(), dim + degree + 1);
				return(0);
			}
		}

		
		for(int i=1; i<knot.getNum(); i++)
		{
			if (knot[i]<knot[i-1]) 
			{
			if (m_browser) {
				((GView*) m_browser)->GetReporter()->Error("NurbsCurve: not increasing elements!");
				return(0);	
			}
			}
		}
	
	
		if (keyWeight.getNum() == 0) { // ok, assumed to be 1.0f
		}
		else {
			if (keyWeight.getNum() != dim) {
				m_bBadValues = TRUE;
				if (m_browser) {
					((GView*) m_browser)->GetReporter()->Error("NurbsPositionInterpolator: Bad length of keyWeight");
				}
			}
		}
	}

	// bad data, so ignore 
	if (m_bBadValues)
		return (0);
	
	evaluating++;

	// clamp f to valid knot range
	if (fractionAbsolute) {
		if (f<knot.values[degree])
			f= knot.values[degree];
		else if (f>knot.values[dim])
			f= knot.values[dim];
	
		fraction = f; 
	}
	else
	{
		//fraction = knot.values[degree] + f* (knot.values[dim + degree] - knot.values[degree]);
		fraction = knot.values[degree] + f* (knot.values[dim] - knot.values[degree]);//tv
	}

	// compute point on curve for given fraction 
	CurvePoint (dim, degree, knot.values, keyValue.values, keyWeight.values, fraction);
		//dim = number of points
		//degree 
		//u knot-vector
		//P controlPoint
		//upar 
    value.touch(); // trigger routes

	evaluating--;

	return(1);
}


//////////////////////////////////////////////////////////////	
//															//	
//															//	
//	T H E   C O O R D I N A T E D E F O R M E R				//
//															//
//////////////////////////////////////////////////////////////


GV_NODE_SOURCE_IMP(GvCoordinateDeformer,Gv2Group);


GvCoordinateDeformer::GvCoordinateDeformer() :

        uKnot(0),
        vKnot(0),
        wKnot(0),

        uOrder(2),
        vOrder(2),
        wOrder(2),

        controlPoint(0),
        weight(0),
		inputCoord(0),
		inputTransform(0),
		outputCoord(0)


{
    GV_NODE_CONSTRUCTOR(GvCoordinateDeformer);

    isBuiltIn = FALSE;

	// from Gv2Group
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);


    GV_NODE_ADD_FIELD(uDimension);
    GV_NODE_ADD_FIELD(vDimension);
    GV_NODE_ADD_FIELD(wDimension);
    
	GV_NODE_ADD_FIELD(uKnot);
    GV_NODE_ADD_FIELD(vKnot);
    GV_NODE_ADD_FIELD(wKnot);

    GV_NODE_ADD_FIELD(uOrder);
    GV_NODE_ADD_FIELD(vOrder);
    GV_NODE_ADD_FIELD(wOrder);

    GV_NODE_ADD_FIELD(controlPoint);
    GV_NODE_ADD_FIELD(weight);

	GV_NODE_ADD_FIELD(inputCoord);
	GV_NODE_ADD_FIELD(inputTransform);
	GV_NODE_ADD_FIELD(outputCoord);


    ///
	GV_NODE_ADD_FIELD(children);

    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);

}

GvCoordinateDeformer::~GvCoordinateDeformer()
{

}


#define getp(pui,pvi,pwi)   pui+ pvi*uDimension + pwi*uDimension*vDimension 

// evaluate deformation on float[3] (SFVec3f) input: src, output: dest
// P are the controlGrid, getp() is used to adress the 3 dimensional grid
void GvCoordinateDeformer::OnEvaluate(const float *src,float *dest,const Point *P)
{
	// basis function wil be evaluated into these arrays
	float Nu[GNURBS_MAX_ORDER]; 
	float Nv[GNURBS_MAX_ORDER]; 
	float Nw[GNURBS_MAX_ORDER]; 
	// degrees 
	int pu = uOrder-1;
	int pv = vOrder-1;
	int pw = wOrder-1;

	float u=src[0];
	// skip if src not in knot parametric range
	if (u<uKnot.values[pu]) return;
	if (u>uKnot.values[uDimension]) return;
	float v=src[1];
	if (v<vKnot.values[pv]) return;
	if (v>vKnot.values[vDimension]) return;
	float w=src[2];
	if (w<wKnot.values[pw]) return;
	if (w>wKnot.values[wDimension]) return;

	// find knot vector span and evaluate Bspline Basis functions
	int spanu=FindSpan(uDimension,pu,u,uKnot.values);
	BasisFuns (spanu, u, pu, uKnot.values, Nu);

	int spanv=FindSpan(vDimension,pv,v,vKnot.values);
	BasisFuns (spanv, v, pv, vKnot.values, Nv);

	int spanw=FindSpan(wDimension,pw,w,wKnot.values);
	BasisFuns (spanw, w, pw, wKnot.values, Nw);

	// now go and compute point 
	Point result; // result summed up here 
	result.Zero();

	int pwiDelta = uDimension*vDimension;

	int pwi=  (spanw-pw)*pwiDelta;

	int wi;
	for (wi=0; wi<=pw; wi++,pwi+=pwiDelta) // for the w's
	{
	int vi;


	int pvi=pwi+(spanv-pv)*uDimension;

	for (vi=0; vi<=pv; vi++, pvi+=uDimension) // for the v's
	{
		float w= Nw[wi]*Nv[vi];

		for (int ui=0; ui<=pu; ui++) // for the u's
		{
			int pui = spanu-pu+ui;
			result += w * Nu[ui] * P[pvi+pui];
		}

	}
	}


	// TRACE("%f %f %f \n",result.x,result.y,result.z);

	*dest++ = result.x;
	*dest++ = result.y;
	*dest++ = result.z;


}
// evaluate deformation on float[3] (SFVec3f) input: src, output: dest
// version with weight
void GvCoordinateDeformer::OnEvaluate(const float *src,float *dest,const Point *P,const float *W)
{
	
	float Nu[GNURBS_MAX_ORDER]; 
	float Nv[GNURBS_MAX_ORDER]; 
	float Nw[GNURBS_MAX_ORDER]; 

	int pu = uOrder-1;
	int pv = vOrder-1;
	int pw = wOrder-1;


	float u=src[0];
	// skip if src not in knot parametric range
	if (u<uKnot.values[pu]) return;
	if (u>uKnot.values[uDimension]) return;
	float v=src[1];
	if (v<vKnot.values[pv]) return;
	if (v>vKnot.values[vDimension]) return;
	float w=src[2];
	if (w<wKnot.values[pw]) return;
	if (w>wKnot.values[wDimension]) return;

	// find knot vector span and evaluate Bspline Basis functions
	int spanu=FindSpan(uDimension,pu,u,uKnot.values);
	BasisFuns (spanu, u, pu, uKnot.values, Nu);

	int spanv=FindSpan(vDimension,pv,v,vKnot.values);
	BasisFuns (spanv, v, pv, vKnot.values, Nv);

	int spanw=FindSpan(wDimension,pw,w,wKnot.values);
	BasisFuns (spanw, w, pw, wKnot.values, Nw);



	Point result;
	result.Zero();
	float resultw = 0.0f;


	int pwiDelta = uDimension*vDimension;

	int pwi=  (spanw-pw)*pwiDelta;

	int wi;
	for (wi=0; wi<=pw; wi++,pwi+=pwiDelta) 
	{
	int vi;


	int pvi=pwi+(spanv-pv)*uDimension;

	for (vi=0; vi<=pv; vi++, pvi+=uDimension)
	{
		float w= Nw[wi]*Nv[vi];

		for (int ui=0; ui<=pu; ui++)
		{
			int pui = spanu-pu+ui;
			result += w * Nu[ui] * P[pvi+pui];
			resultw += w * Nu[ui] * W[pvi+pui];
		}

	}
	}


	// TRACE("%f %f %f \n",result.x,result.y,result.z);
	result *= (1.0/resultw);

	*dest++ = result.x;
	*dest++ = result.y;
	*dest++ = result.z;


}

// evaluate deformation on MFVec3f input: src, output: dest
void GvCoordinateDeformer::OnEvaluate(GvMFVec3f &src,GvMFVec3f &dest)
{
	int l=src.getNum();
	
	if (dest.getNum() < l) {
		dest.set(src); // copy initial input values for the "out of knot range" case
	}

	// get pointers to src data
	const Point *P = (const Point *) controlPoint.get();
	const float *W= weight.get();

	// weights ?
	if (weight.getNum() < controlPoint.getNum())
		W=NULL;


	if (W != NULL) { // version with weights
		for(int i=0; i<l;i++) {
			OnEvaluate(src.get1(i),dest.get1(i),P,W);

		}
	}
	else 
	{
		for(int i=0; i<l;i++) {
			OnEvaluate(src.get1(i),dest.get1(i),P);
		}
	}
	
	dest.OnChanged(); // trigger routes / updates

}

// evaluate deformation on MFVec3f input: src, output: dest
// version with src transformed by Matrix m
void GvCoordinateDeformer::OnEvaluate(GvMFVec3f &src,const Matrix &m, GvMFVec3f &dest)
{
	int l=src.getNum();
	
	if (dest.getNum() < l) {
		dest.set(src); // copy initial input values for the "out of knot range" case
	}

	// get pointers to src data
	const Point *P = (const Point *) controlPoint.get();
	const float *W= weight.get();

	Point ptemp;

	// weights ?
	if (weight.getNum() < controlPoint.getNum())
		W=NULL;


	if (W != NULL) { // version with weights
		for(int i=0; i<l;i++) {
			src.get1(i,ptemp);
			ptemp = m * ptemp;
			OnEvaluate(&ptemp.x,dest.get1(i),P,W);

		}
	}
	else 
	{
		for(int i=0; i<l;i++) {
			src.get1(i,ptemp);
			ptemp = m * ptemp;
			OnEvaluate(&ptemp.x,dest.get1(i),P);
		}
	}
	
	dest.OnChanged(); // trigger routes / updates

}

// evaluate deformation 

void GvCoordinateDeformer::OnEvaluate()
{
	int l=min( inputCoord.getNum(),outputCoord.getNum());

	int lt = inputTransform.getNum();

	const Point *P = (const Point *) controlPoint.get();
	const float *W= weight.get();
	
	if (weight.getNum() < controlPoint.getNum())
		W=NULL;
	
	// for each entry in coord
	for(int i=0; i<l;i++) {
		Gv2Transform *t=NULL;

		// get src & dest as Coordinate node

		GvNode *a = inputCoord.get1(i);
		if (!a) continue;

		GvNode *b = outputCoord.get1(i);
		if (!b) continue;

		GvCoordinate *src= RTCAST(a,GvCoordinate);
		GvCoordinate *dest= RTCAST(b,GvCoordinate);

		if (src && dest) {
			if (i<lt) { // is there also an inputTransform ????
				GvNode *c = inputTransform.get1(i);
				if (c) t=RTCAST(c,Gv2Transform);
			}
			// evalute deformation, store result in dest

			if (t) { // version with Transform
				OnEvaluate(src->point,t->Transform(),dest->point);
			}
			else OnEvaluate(src->point,dest->point);
		}
	}

}



int GvCoordinateDeformer::Do(GBBoxTraversal &state)
{
	return Gv2Group::Do(state);
}


int GvCoordinateDeformer::Do(GglRenderTraversal &state)
{ 
	// cullcheck here 	
	if (!state.CullCheck(bboxCenter,bboxSize)) 
		return TRAVERSE_CULLED;

	OnEvaluate();
	
	int ret = Gv2Group::Do(state);

	return ret;

}

int GvCoordinateDeformer::Do(GRaySelectTraversal &state)
{  	
	return Gv2Group::Do(state);
}

int GvCoordinateDeformer::Do(GGeometryTraversal &state)
{ 
	return Gv2Group::Do(state);
}



int GvCoordinateDeformer::OnFieldChanged(GvField *field)
{
	return Gv2Group::OnFieldChanged(field);
}

int GvCoordinateDeformer::Do(GPathTraversal &state)   
{ 
	return Gv2Group::Do(state);
}

int GvCoordinateDeformer::Do(GOptimizeTraversal &state)
{
	return Gv2Group::Do(state);
}


int GvCoordinateDeformer::Do(GTraversal &state)   
{ 
	return Gv2Group::Do(state);
}


