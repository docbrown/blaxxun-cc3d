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
#ifndef _GvNurbs_
#define  _GvNurbs_

/******************************************************************************
@doc

@module GvNurbs.h - blaxxun/ VRML 2.0 Nurbs related nodes|

Copyright (c) 1998 - 1999 by blaxxun interactive
All rights reserved

Purpose:

Classes:
<c GvNurbsSurface>
...

  
Notes:

Changes:

$Revision: 1.13 $
$Log: gvnurbs.h,v $
Revision 1.13  1999/07/06 16:35:13  tom
*** empty log message ***

Revision 1.12  1999/05/31 13:15:05  tom
empty message


Todo :

******************************************************************************/


#include <gvsubnode.h>
#include <gvsfvec3f.h>
#include <gvmfvec3f.h>
#include <gvsfbool.h>

#include <gvsffloat.h>
#include <gvmffloat.h>

#include <gvsfnode.h>
#include <gvmfnode.h>

#include <gvsfint32.h>
#include <gvmfint32.h>
#include <gvmfstring.h>
#include <gvmfvec2f.h>
#include <gvmfcolor.h>

#include <gvnodeshell.h>

#include "grender.h"
#include "gnurbs.h"

#include "gv2group.h"
#include "gvinterpolators.h"



// forwards
class GGridTess;



#ifdef _OGL
	#define _OGL_NURBS
#endif _OGL



//////////////////////////////////////////////////////////////	
//															//	
//															//	
//				T H E   N U R B S S U R F A C E				//
//															//
//////////////////////////////////////////////////////////////
 

class GvNurbsSurface : public GvNodeShell {

    GV_NODE_HEADER(GvNurbsSurface);

  public:
    // Fields
	
    GvSFInt32		uDimension;	// 
    GvSFInt32		vDimension;	// 
 

	GvSFInt32		uOrder;
	GvSFInt32		vOrder;

	GvMFVec3f		controlPoint;
	GvMFFloat		weight;

	GvMFFloat		uKnot;
	GvMFFloat		vKnot;

	
	GvSFNode		texCoord;


    GvSFInt32		uTessellation;	// 
    GvSFInt32		vTessellation;	// 


	GvSFBool        solid;
    GvSFBool		ccw;              // Default :TRUE


	// is this a solid shape ?
	GvBool Solid() const { return (solid.value); }
	GvBool isSolid() const { return Solid(); }
	//GvBool ccw() const { return ccw.get(); }

	// ! evaluate point at parameter u,v 
	GvBool Eval(float u, float v, Point &p);


  	// build shell for current state 
  	int BuildShell(GTraversal &state);  

    int Do(GBBoxTraversal &state);
    int Do(GglRenderTraversal &state);
	int Do(GGeometryTraversal &state);
	int Do(GRaySelectTraversal &state);  
	
	int Do(GOptimizeTraversal &state); 


 
	// internal
	// try to update internal shell
	// int OnControlPointChanged();
	
	int OnFieldChanged(GvField *field);


	// statistic update
	// the last frame number this object was rendered
	// unchanged if culled by bbox e.g.

	GFrameStats frameStats;

	gbool m_bInitialized, m_bBadValues;


	//! get initial estimate of number of triangles
	int getInitialTriangleEstimate() 
	{ 
		int x =uTessellation;
		if (x==0) x = uDimension * 2;
		else if (x<0) x = uDimension* -x;

		int y=vTessellation;
		if (y==0) y = vDimension * 2;
		else if (y<0) y = vDimension* -y;

		return x*y*2;
	}

	//! get tesselation 
	void getTessellation(int &uSteps,int &vSteps,float scaleFactor) 
	{ 
		int x =uTessellation;
		if (x==0) x = uDimension * 2;
		else if (x<0) x = uDimension* -x;

		int y=vTessellation;
		if (y==0) y = vDimension * 2;
		else if (y<0) y = vDimension* -y;

		x*=scaleFactor;
		y*=scaleFactor;
		
		uSteps = max(2,min(x,1024));
		vSteps = max(2,min(y,1024));

	}


	//! get the internal GNurbsShell object
	GNurbsShell* getNurbsShell(GTraversal &state);

private:

	//! get the current internal GNurbsShell object (without build)
	inline 
		GNurbsShell * getNurbsShell() { return (GNurbsShell* )theShell; }

#ifdef _OGL_NURBS
	//! cached vec4f
	FloatArray  controlPoints;

	//! GLU nurbs tesselator 
	GLUnurbsObj* nurbsObj;

	//! GL call list 
	glCache		callList;
#endif


};




//////////////////////////////////////////////////////////////	
//															//	
//															//	
//				T H E   N U R B S C U R V E	2D 2D 2D		//
//															//
//////////////////////////////////////////////////////////////
/*
only data container for trimmed nurbs surface
geometry processing can be found in trimmedsurface
*/
class GvNurbsCurve2D : public GvNode {

    GV_NODE_HEADER(GvNurbsCurve2D);

  public:
    // Fields
	GvMFFloat  knot;//             []     
	GvSFInt32  order;//            3 
	GvMFVec2f  controlPoint;//     []
	GvMFFloat  weight;//           []
	GvSFInt32  tessellation;//     0

	GvBool changed;					// changed flag
	

	GvBool m_bInitialized, m_bBadValues;

	int OnFieldChanged(GvField *field);

	//! verify the values
	GvBool Verify(); 

	// ! evaluate as polyline with n- fixed steps
	GvBool EvalPolyline(int nsteps, PointArray &p);
	//! evaluate to grid tesselator
	GvBool Eval(GGridTess &tess);


	// ! evaluate at parameter u 
	GvBool Eval(float u, Point &p);

	// ! get parameter range 
	GvBool GetRange(float &umin,float &umax);


    int Do(GglRenderTraversal &state);
    int Do(GBBoxTraversal &state);


};
//////////////////////////////////////////////////////////////	
//															//	
//															//	
//				T H E   P O L Y L I N E 2 D					//
//															//
//////////////////////////////////////////////////////////////
/*
only data container for trimmed nurbs surface
geometry processing can be found in trimmedsurface
*/
class GvPolyline2D : public GvNode {

    GV_NODE_HEADER(GvPolyline2D);

  public:
    // Fields
	GvMFVec2f  point;//     []

	GvBool changed;					// changed flag

	int OnFieldChanged(GvField *field);

	// ! evaluate as polyline with n- fixed steps
	GvBool EvalPolyline(int nsteps, PointArray &p);
	//! evaluate to grid tesselator
	GvBool Eval(GGridTess &tess);

    int Do(GglRenderTraversal &state);
    int Do(GBBoxTraversal &state);

};
//////////////////////////////////////////////////////////////	
//															//	
//															//	
//				T H E   C O N T O U R 2 D					//
//															//
//////////////////////////////////////////////////////////////
/*
only data container for trimmed nurbs surface
geometry processing can be found in trimmedsurface
*/
class GvContour2D : public Gv2Group {

    GV_NODE_HEADER(GvContour2D);

	GvSFBool	fill;


  public:

	GvBool changed;					// changed flag

	int OnFieldChanged(GvField *field);
	int OnChildChanged(GvNode *child);

	//! evaluate as polyline with n- fixed steps
	GvBool EvalPolyline(int nsteps, PointArray &p);

	//! evaluate to grid tesselator
	GvBool Eval(GGridTess &tess);

    int Do(GglRenderTraversal &state);

    int Do(GBBoxTraversal &state);

};



//////////////////////////////////////////////////////////////	
//															//	
//															//	
//				T H E   N U R B S C U R V E					//
//															//
//////////////////////////////////////////////////////////////
 
class GvNurbsCurve : public GvNodeShell {

    GV_NODE_HEADER(GvNurbsCurve);

  public:
    // Fields
	GvSFInt32  dimension;//        0   obsolete HG 
	GvMFFloat  knot;//             []     
	GvSFInt32  order;//            3 
	GvMFVec3f  controlPoint;//     []
	GvMFFloat  weight;//           []
	GvSFInt32  tessellation;//     0


	GvBool changed;					// changed flag
	GvBool m_bInitialized, m_bBadValues;
	
	int OnFieldChanged(GvField *field);

	//! verify the values
	GvBool Verify(); 

	////! evaluate as polyline with n- fixed steps
	//GvBool EvalPolyline(int nsteps, PointArray &p);



    // build shell for current state 
  	int BuildShell(GTraversal &state);  
    int Do(GglRenderTraversal &state);
	int Do(GRaySelectTraversal &state);  
    int Do(GBBoxTraversal &state);


};




//////////////////////////////////////////////////////////////	
//															//	
//															//	
//			T H E   T R I M M E D S U R F A C E				//
//															//
//////////////////////////////////////////////////////////////
/* the trimmed surface is derived of nodeshell although it
is very similiar to nurbssurface
the trimmedsurface owns a nurbssurface and trimming curves
the trimming curves should be DEF/USEd for adjacent trimmed
surfaces!
the children of the trimmedsurface must not be traversed!
the nurbssurface is misused as data container
*/

class GvTrimmedSurface : public GvNodeShell {

    GV_NODE_HEADER(GvTrimmedSurface);

  public:
    // Fields
	GvMFNode  trimmingContour;  //of type Contour2D
	GvSFNode  surface;			//of type NurbsSurface

	void addTrimmingContour(GvMFNode *event);

	void removeTrimmingContour(GvMFNode *event);


	int Do(GTraversal &state);

	int Do(GPathTraversal &state);   

	// apply touchsensors first 
	int Do(GRaySelectTraversal &state);

	int Do(GBBoxTraversal &state);
    int Do(GglRenderTraversal &state);

	int Do(GOptimizeTraversal &state);	
	int Do(GGeometryTraversal &state);
	
    // notification on field change
    int OnFieldChanged(GvField *field);

	// some child node has been changed
	int OnChildChanged(GvNode *child);


  	int BuildShell(GTraversal &state);  
	
	//! get the base NURBS Surface
	GvNurbsSurface* getNurbsSurface(GTraversal &state);

	//! evaluate trimming to grid tesselator
	GvBool EvalTrimming(GTraversal &state,GGridTess &tess);



  private:

	GFrameStats frameStats;

	//! verify the values
	GvBool Verify(GvNurbsCurve2D* curve2D); 

	gbool m_bInitialized, m_bBadValues;

		//! get the current internal GNurbsShell object (without build)
	inline 
		GTrimmedNurbsShell * getNurbsShell() { return (GTrimmedNurbsShell* )theShell; }



#ifdef _OGL_NURBS

	int DoOGL(GglRenderTraversal &state);

	//! cached vec4f
	FloatArray  controlPoints;

	//! GLU nurbs tesselator 
	GLUnurbsObj* nurbsObj;

	//! GL call list 
	glCache		callList;	
#endif
};




//////////////////////////////////////////////////////////////	
//															//	
//															//	
//				T H E   N U R B S G R O U P					//
//															//
//////////////////////////////////////////////////////////////

class GvNurbsGroup : public Gv2Group {

    GV_NODE_HEADER(GvNurbsGroup);

  public:
    // Fields

    GvSFFloat		tessellationScale;	

	GFrameStats frameStats;

	int Do(GTraversal &state);

	int Do(GPathTraversal &state);   

	// apply touchsensors first 
	int Do(GRaySelectTraversal &state);

	int Do(GBBoxTraversal &state);
    int Do(GglRenderTraversal &state);

	int Do(GOptimizeTraversal &state);	
	int Do(GGeometryTraversal &state);
	
    // notification on field change
    virtual int OnFieldChanged(GvField *field);

};






///////////////////////////////////////////////////////////////////////////
//
//
//					T H E   N U R B S I N T E R P O L A T O R 
//
///////////////////////////////////////////////////////////////////////////

class GvNurbsPositionInterpolator : public GvInterpolator{

    GV_NODE_HEADER(GvNurbsPositionInterpolator);

  public:
    
	 // Fields
	GvSFInt32		dimension;//obsolete, will be removed sooner or later
	GvMFFloat		knot;
	GvSFInt32		order;
	GvSFBool		fractionAbsolute;
	GvMFFloat		keyWeight;
    GvMFVec3f		keyValue;		// [] keyframe values


    // Event OUT   
    GvSFVec3f  value;

    // Event IN
    //set_fraction SFFloat;

	BOOL m_bInitialized, m_bBadValues;// normalized;

	// evaluate point on curve 
    int OnEvaluate(float f);

    // notification on field change
    virtual int OnFieldChanged(GvField *field);

	int CurvePoint (int n, int p, const float *U, const float *P, const float *w, float u);
};




//////////////////////////////////////////////////////////////	
//															//	
//															//	
//	T H E   C O O R D I N A T E D E F O R M E R				//
//															//
//////////////////////////////////////////////////////////////


class GvCoordinateDeformer : public Gv2Group {

    GV_NODE_HEADER(GvCoordinateDeformer);

  public:
    // Fields

    GvSFInt32		uDimension;	// 
    GvSFInt32		vDimension;	// 
    GvSFInt32		wDimension;	// 
 

	GvSFInt32		uOrder;
	GvSFInt32		vOrder;
	GvSFInt32		wOrder;


	GvMFFloat		uKnot;
	GvMFFloat		vKnot;
	GvMFFloat		wKnot;

	GvMFVec3f		controlPoint;
	GvMFFloat		weight;

	GvMFNode		inputCoord;
	GvMFNode		inputTransform;
	GvMFNode		outputCoord;

	// internal
	GvBool mustEvaluate;  // must re-evaluate output
	GvBool mustCheck;	// must verify input fields

	// evaluate deformer
	void OnEvaluate();
	
	void OnEvaluate(const float *src,float *dest,const Point *P);
	void OnEvaluate(const float *src,float *dest,const Point *P,const float *W);

	void OnEvaluate(GvMFVec3f &src,GvMFVec3f &dest);
	// version with src transformed by m
	void OnEvaluate(GvMFVec3f &src,const Matrix &m, GvMFVec3f &dest);


	int Do(GTraversal &state);

	int Do(GPathTraversal &state);   

	// apply touchsensors first 
	int Do(GRaySelectTraversal &state);

	int Do(GBBoxTraversal &state);
    int Do(GglRenderTraversal &state);

	int Do(GOptimizeTraversal &state);	
	int Do(GGeometryTraversal &state);

	
    // notification on field change
    virtual int OnFieldChanged(GvField *field);

};




#endif _GvNurbsSurface_


