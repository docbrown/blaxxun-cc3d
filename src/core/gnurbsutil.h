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
#ifndef __GNURBSUTIL
#define __GNURBSUTIL

/******************************************************************************
@doc

@module GNurbsUtil.h - NURBS utilities|

Copyright (c) 1999 by blaxxun interactive
All rights reserved

Purpose:

  
Notes:

Changes:

Todo :


******************************************************************************/



#include "stdafx.h"

#include "greporter.h"
#include "pointa.h"


#define GNURBS_MAX_ORDER 16

// 
// find span - see the NURBS Book
// page 68 
//
extern int FindSpan (int n, int p, float u, const float *U);

extern void BasisFuns ( int i, float u, int p, const float *U, float *Nf ); 

//
// Compute a 3D Nurbscurve fitting a set of sample points
//
// used in tree-editor
//


void ComputeNurbsCurve ( // input 
					   int numValues, 
					   const float *key, 
					   const float * keyValue,
					   float SCALE,
					   
					   // output
					   int &dimension,
					   int &order,
					   FloatArray &knot,
					   PointArray &ctrlPts
					   );



extern void CurvePoint (int n, int p, const float *U, const float *P, const float *w, const float u, Point &outPoint);

extern void CurvePoint2D (int n, int p, const float *U, const float *P, const float *w, const float u, Point &outPoint);

// evaluate point on Nurbs Surface 
// version with weight
gbool SurfacePoint(
				  float u, float v, int uOrder, int vOrder, int uDimension, int vDimension,
				  const float *uKnot, const float * vKnot,
				  const Point *P,const float *W,
				  float *dest // result
				  );





//static void lubksb (float **a, int n, int np, int *indx, float *b);
//static void ludcmp (float **a, int n, int np, int *indx, float d);
//static int RemoveKnot(int n, int p, float *U, float *Px, float *Py, float *Pz,
//			   float u, int r, int s, int num, float SCALE);
//void ComputeKnotVector(int N, int pdim, int m, float *uk, float *u);
//void ComputeUkVector(int N, float *t, float* uk);




typedef enum {
	G_ON_ULINE=1,
	G_ON_VLINE=2,
	G_ON_UVLINE=3,

} GGridStatus;

struct GGridPoint 
{

	float t;	// curve parameter
	Point p;	// 2D parameter point
	int iu,iv;	// grid point 
	
	unsigned int gridStatus : 2;
	
	Point v;	// surface point
	Point vn;	// surface normal

	friend 
	inline bool	InSameGrid(GGridPoint &p0, GGridPoint &p1) 
	{
		return (p0.iu == p1.iu && p0.iv == p1.iv);
	}

};


class GGridTess {

public:

	Point pmin,pmax; // parameter range of surface

	Point toGridScale; // scale factor mapping u/v to the grid

	// the array of grid curves
	Array<GGridPoint> curveP;

	// start ending point indices
	IntArray		curveS;


	// the domain fixed size step tesselation
	int usteps,vsteps;

	GGridTess(const Point &mi, const Point &ma) : pmin(mi), pmax(ma) { } 
	~GGridTess() { } 


	// set the grid stepping and compute toGridScale
	void SetSteps(int u, int v) {
		usteps =u;
		vsteps=v;
		toGridScale.Set( (usteps) / (pmax.x-pmin.x), (vsteps) / (pmax.y-pmin.y));
	}

	// map the u v paramter (p.p) to nearest grid point 
	void MapToGridPoint (GGridPoint &p) 
	{
		float gu,gv;

		float fgu,fgv;


		gu = (p.p.x-pmin.x) * toGridScale.x;
		gv = (p.p.y-pmin.y) * toGridScale.y;

		p.iu = fgu=floor(gu);
		p.iv = fgv=floor(gv);

		if (iszero(gu-fgu)) 
			p.gridStatus = G_ON_ULINE;
		else p.gridStatus =0;
		if (iszero(gv-fgv)) 
			p.gridStatus |= G_ON_VLINE;

	}

	// store computed curve point 
	void Output(GGridPoint &p);

	void BeginContour();
	void EndContour();

	int NumContours() { return curveS.Length() / 2; } ;

	void GetContour(int ci,PointArray & pts); 

	void SplitLine(const GGridPoint &p0, const GGridPoint &p1, float t, 
		GGridPoint &p)
	{
		p.t = t;
		Interpolate(p0.p,1.0-t, p1.p,t, p.p);
		MapToGridPoint(p);

	}

	
	void SubdivideLine(GGridPoint &p0, GGridPoint &p1); 

	// griddify a polyline 
	void Polyline(int np, const Point *p); 

	// nurbs curve stuff

	// current curve data
	int n; 
	int p; 
	const float *U; 
	const float *P; 
	const float *w;

	float teps; // parameter epsilon

	void SetNurbsCurve(int n, int p, const float *U, const float *P, const float *w)
	{
		this->n=n;
		this->p=p;
		this->U=U;
		this->P=P;
		this->w=w;
	}

	void NurbsCurve(int n, int p, const float *U, const float *P, const float *w,int steps); 

	
	// get curve point 
	void EvalNurbsPoint(float u, Point &outPoint)
	
	{ CurvePoint2D(n,p,U,P,w, u,outPoint); }
	
	void SubdivideCurve(GGridPoint &p0, GGridPoint &p1) ;



};





#endif

