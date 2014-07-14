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
#ifndef __gnurbsshell__
#define __gnurbsshell__
/******************************************************************************
@doc

@module GNurbsShell.h 

Copyright (c) 1998 - 1999 by blaxxun interactive
All rights reserved

Purpose:

Classes:
  
Notes:

Changes:

$Revision: 1.8 $
$Log: gnurbs.h,v $
Revision 1.8  1999/07/06 16:54:41  tom
*** empty log message ***


Todo :

******************************************************************************/

#include "gshell.h"

#include "astream.h"


/*------------------
		config
-------------------*/



#ifdef _DEBUG	

	//#define GN_LOG // enable logging 

	//#define _LOG_TRIMMED_//enable gul logging

	#ifdef _LOG_TRIMMED_

	// logging
	#include <fstream.h>

	#endif

#endif

/*------------------
		config
-------------------*/


typedef struct tagHPoint
{
	float x;
	float y;
	float z;
	float w;
} HPoint;

#define VECTOR_SIZE 4//(sizeof( __m128 )/sizeof( float ) )
#define LOWER_TESSELLATION_BOUND 0.4f

#ifdef _KATMAI_XXX
	#define ALIGNED_NEW( s, t ) (t *)_mm_malloc( (s) * sizeof( t ), 16 )
	#define ALIGNED_DELETE( x ) do { if( (x) ) { _mm_free( (char *)x ); x = NULL; } } while (0)
#else
	#define ALIGNED_NEW( s, t ) (t *) malloc( (s) * sizeof( t ))
	#define ALIGNED_DELETE( x ) do { if( (x) ) { free( (char *)x ); x = NULL; } } while (0)
#endif

#define SAFE_DELETE( x ) if( x ) { delete x; x = NULL; }


typedef enum GNurbsMode {

	GNURBS_STATIC_TESSELATION=0, 

	GNURBS_DYNAMIC_TESSELATION=1, 
	GNURBS_FRAMERATE_TESSELATION=2,
	GNURBS_DYNAMIC_FRAMERATE_TESSELATION=3, 

} GNurbsMode;


class GTrimmedNurbsShell;




//----------------------------------------------------------------------
//
//							GNurbsShell
//
//----------------------------------------------------------------------


class GNurbsShell : public GShell
{
public:
  friend class GTrimmedNurbsShell;

	RT(GNurbsShell);

	GNurbsShell();
	~GNurbsShell();

	gbool Init(int uDimension, int vDimension, int uOrder, int vOrder,
					   const float *controlPoint, 
					   int numWeight,const float* weight, 
					   const float* uKnot, const float* vKnot, 
					   int nUTessellations, int nVTessellations);
	
	// set the initial tesselation // multiplied later by budget
	void InitTessellationValues(int nUTessellations, int nVTessellations);
	
	int ComputeShell();

	void setControlPoints(int numCtrlPts, const float *ctrlPts, 
						int numWeights, const float *weights);

	void setControlPoints(int num, const float *ctrlPts);

	void setWeight(int num, const float * weights);

	int  GetCurrentTriangleBudget() const {
		return (2 * m_iUTessellations * m_iVTessellations);
	}

	int SetRenderQuality(float budget);

	// is the current cached tessellation up to date ?
	gbool IsShellTessellationOk() const {
		if ( tessellationChanged || !shellVerticesOk || !shellFacelistOk)
			return gfalse;
		return gtrue;
	}



private:

#ifdef GN_LOG
//	AStream out("Framestats.log", "w");
	ofstream tris;
	ofstream bbsurf;
	ofstream scaleStream;
#endif

	BOOL texCompute;
	gbool triStrip; // use triStips or triangles for draw 

	// are the vertices of the shell Computed ??
	gbool shellVerticesOk;					
	// is the facelist of the shell Computed ??
	gbool shellFacelistOk;					

	// has the tesselation be changed ?
	gbool tessellationChanged;

	//the intel stuff
	BOOL m_bInitialized;
	int g_CurrentUKnot;
	
	int m_iUCoefficients;
	int m_iVCoefficients;
	int m_iUControlPoints;
	int m_iVControlPoints;
	
	unsigned int m_iUKnots;
	unsigned int m_iVKnots;

	unsigned int m_iUDegree, m_iUOrder;
	unsigned int m_iVDegree, m_iVOrder;

	unsigned int m_iUTessellations, m_iMinUTessellations, m_iMaxUTessellations;
	unsigned int m_iVTessellations, m_iMinVTessellations, m_iMaxVTessellations;
	unsigned int m_iLastUTess, m_iLastVTess;

	unsigned int m_iInitialUTess, m_iInitialVTess; // initial tesselation numbers

	float m_fBudget;
	Point bboxMin, bboxMax;
	float m_fBBSurface;
	float m_fInitialRenderQuality;
	float m_fRenderQuality;//=triangles per BBSurface = const
			


	FloatArrayAligned m_ControlPoints;
	FloatArrayAligned m_TxControlPoints;

	FloatArrayAligned m_UKnots;
	FloatArrayAligned m_VKnots;
	FloatArrayAligned m_UBasisCoefficients;
	FloatArrayAligned m_VBasisCoefficients;
	FloatArrayAligned m_UBasis;
	FloatArrayAligned m_VBasis;
	FloatArrayAligned m_dUBasis;
	FloatArrayAligned m_dVBasis;

	FloatArrayAligned m_dUTemp;
    FloatArrayAligned m_UTemp;

	int *m_TessUKnot;
	int *m_TessVKnot;

	PointArray m_pNormals;
	
	WORD *m_iVertexIndices[2]; // tri strip indices for DrawPrimitive

	FloatArrayAligned m_pVertexPos; // tesselated vertices in view coordinates
	float *m_pQView;
	float *m_pQWV;


	int m_iVertices;
	float m_fHalfX, m_fHalfY;

	virtual void ComputeBasisCoefficients();
	virtual void EvaluateBasisFunctions();

	void SetTessellationLimits( int iMinUTessellations, int iMinVTessellations, int iMaxUTessellations, int iMaxVTessellations);

	float ComputeCoefficient( float *fKnots, int iInterval, int i, int p, int k );
	float ComputeBBSurface(Matrix &model2view, Matrix &projection, float w, float h);
	void SetTessellation(int nUTessellations, int nVTessellations);

	//! computes new tessellation values depending on mode, quality bbox
	int ComputeTessellationValues(int mode);

	//! transforms the control points from model2view
	void TransformControlPoints(const Matrix &m);
	

public:

	//! Evaluate point on surface using the current computed tesselation
	gbool EvaluateOnTessellation(float u, float v, Point &p, Point *vn=NULL, Point *vp=NULL);

	void GetTessellation(int &nUTessellations, int &nVTessellations) 
	{
		nUTessellations = m_iUTessellations,nVTessellations=m_iVTessellations;
	}
	//! get maximum parameter range in u
	void GetURange(float &umin,float &umax) {
			 umin = m_UKnots[m_iUDegree];
			 umax = m_UKnots[m_iUControlPoints];
	}

	//! get maximum parameter range in v
	void GetVRange(float &vmin,float &vmax) {
			 vmin = m_VKnots[m_iVDegree];
			 vmax = m_VKnots[m_iVControlPoints];
	}


	// virtual ovverrides for GShell
	//! compute bounding box, based on projected control points
	void ComputeBBox(BBox &box);

	//! do boundingbox computation on control vertices
	int Do(GBBoxTraversal &state);       

	// check if vertices or shell need to be recomputed
	int Do(GglRenderTraversal &state);
	int Do(GRaySelectTraversal &state);



#ifdef _D3D

	//! get pointer to TLVERTEX Buffer
	inline D3DTLVERTEX *GetScreenVertices() { return (D3DTLVERTEX*) vtl.Data(); }

	// get pointer to TLVERTEX Buffer with size check
	inline D3DTLVERTEX *GetScreenVertices(int nv) 
	{ 
		// DTLVERTEX = 8 floats x y z w color spec tu tv 
		vtl.SetLength( (sizeof(D3DTLVERTEX) / sizeof(float)) * ((nv+3) & ~3)); // katmai uses at least multiple of 4 
		return (D3DTLVERTEX*) vtl.Data(); 
	}

	//Render using ownLighting/Transform 
	int D3dRenderTL(GglRenderTraversal &traversalState);
	
	void TransformControlPointsKatmai(const Matrix &m);

	HRESULT D3DRenderKatmaixx(LPDIRECT3DDEVICE2 pd3dDevice, LPDIRECT3DMATERIAL2 pd3dSurfMaterial,
		const Matrix &m_MxProjection, // projection 
	    float sx,float tx, float sy,float ty, // 2D viewpoint transform
		DWORD clipFlags	// for DrawPrimitive
		);

	HRESULT D3DRenderKatmai(RenderState &state, 
		const Matrix &m_MxProjection, // projection 
	    float sx,float tx, float sy,float ty, // 2D viewpoint transform
		DWORD clipFlags	// for DrawPrimitive
		);

	HRESULT D3DRenderX86(RenderState &state, 
		const Matrix &m_MxProjection, // 
	    float sx,float tx, float sy,float ty, // 2D viewpoint transform
		DWORD clipFlags	// for DrawPrimitive
		);

	int LightVertices(RenderState &state,
					  int nv, // number of vertices
					  const HPoint *v, const Point *vn,
					  D3DTLVERTEX * vtl,
					  int vtlstep // VTL BUFFER OFFSET
					  );

#endif

};





//----------------------------------------------------------------------
//
//							GTrimmedNurbsShell
//
//----------------------------------------------------------------------

#if defined(G_T_NURBS)

#include "gul/common/gul_types.h"

using gul::Ptr;
using gul::hpoint;
using gul::point;
using gul::hpoint2;
using gul::point2;
using gul::List;
using gul::List2;
using gul::ListNode;


// data structures
struct trim_element   // can be a polyline (if poly.nElems() > 0) or a curve
{
  Ptr< point2<float> >               poly;  // polyline
  gul::curve<float,hpoint2<float> >  curv;  // curve
};
typedef List2< ListNode<trim_element> > trim_region; // list with two ends
typedef List< ListNode<trim_region> > trim_partition;

#endif



class GTrimmedNurbsShell : public GShell
{
public:

	RT(GTrimmedNurbsShell);

	GTrimmedNurbsShell();
	virtual ~GTrimmedNurbsShell();

	// ! the base surface
	GNurbsShell *surface;

	// array trimmingCurves;

	// ! set the base surface
	gbool SetSurface(GNurbsShell *newSurface);

	gbool BeginTrim(void);
	
	//! add a trimming curve
	gbool AddNurbsTrimmingCurve2D(
					   int order, 
					   int numControlPoint,const float *controlPoint, 
					   int numWeight,const float* weight, 
					   int numKnot, const float* knot, 
					   int tessellations );

	gbool AddPolyline2D(int numOfPoints, const float* points);

	gbool EndTrim(void);

	// ! delete all trimming curves
	gbool DeleteTrimmingCurves();

	int ComputeShell();
	
	gbool changed;

	// notify
	void SetChanged() { changed = gtrue; }
	void SetSurfaceChanged() { SetChanged(); }
	void SetTrimmingChanged() { SetChanged(); }

	//! is the current cached tessellation up to date ?
	gbool IsShellTessellationOk() const {
		if ( tessellationChanged || changed)
			return gfalse;
		return gtrue;
	}

	//! set the render quality
	int SetRenderQuality(float budget);


private:
	//! scales the tessellation according to the scale in the state
	float m_fRenderQuality;

	//! has the tesselation be changed ?
	gbool tessellationChanged;

	float ComputeBBSurface(Matrix &model2view, Matrix &projection, float w, float h);

	void SetTessellation(int nUTessellations, int nVTessellations);

	//computes new tessellation values depending on mode , quality bbox
	int ComputeTessellationValues(int mode);


	// virtual ovvrrides for GShell

	void ComputeBBox(BBox &box);

	//! do boundingbox computation on control vertices
	int Do(GBBoxTraversal &state);       

	// check if vertices or shell need to be recomputed
	int Do(GglRenderTraversal &state);
	int Do(GRaySelectTraversal &state);
	
	float m_ScaleFactor;   //!< Scale Factor
 
	float m_TessTol;       //!< relative to (diagonal of BoundingBox) / ScaleFactor
						   //!< default = 0.0025

	float m_DomainTessTol; //!< relative to (diagonal of parametric domain) / ScaleFactor 
						   //!< (length diagonal = sqrt(2), since the knot vectors will get
						   //!< normalized), default = 0.0001

	int m_MaxSubDivisions; //!< maximum of how often the surface gets quartered,
						   //!< default = 7

#if defined(G_T_NURBS)

	//! for storing the trim regions
	trim_partition mContours2D;   
	
	//! for storing a contour
	ListNode<trim_region> *contour;
  	
	//! the curve segment
	ListNode<trim_element> *elnode;

#ifdef _LOG_TRIMMED_
    void dump_to_file();
#endif


#endif


};


#endif __gnurbsshell__
