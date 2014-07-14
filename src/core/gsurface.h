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
#ifndef _GSurface_h_
#define _GSurface_h_

// HG 
#include <gvsfenum.h>
#include <gvsfvec2f.h>
#include <gvsfvec3f.h>
#include <gvsffloat.h>
#include <gvsfbool.h>
#include <gvsflong.h>
#include <gvgroup.h>
#include <gvsubnode.h>
#include <gvsfnode.h>
#include <gvmfnode.h>
#include <gvnodeshell.h>


class GCurve : public GvNode
{
   GV_NODE_HEADER(GCurve);
public:

   GvSFVec2f	urange;
   
   int dim; // 1 1d 2: 2d 3 : 3d
   int Dim()  { return(dim); }

   // evaluate point at paratmer (u) 
   virtual int Eval(const float *parameter,Point &p);
	
   // Evaluate normal at parameter (u)
   virtual int EvalNormal(const float *parameter,Point &n);


};

class GModulatedCurve : public GCurve
{
   GV_NODE_HEADER(GModulatedCurve);
public:
   GvSFNode curve;
   GvSFNode modulator;
};

class GTransformedCurve : public GCurve
{
   GV_NODE_HEADER(GTransformedCurve);
public:
   GvSFNode curve;
   GvSFNode transformation;
};

class GConcatenatedCurve : public GCurve
{
   GV_NODE_HEADER(GConcatenatedCurve);

public:
   GvMFNode curves;
};

class GInterpolatedCurve : public GCurve
{
   GV_NODE_HEADER(GInterpolatedCurve);

public:
   GvSFFloat t;
   GvMFNode curves;

};


class GCircle : public GCurve
{
   GV_NODE_HEADER(GCircle);
public:
   GvSFFloat r;	 // 1.0
	int Eval(const float *parameter,Point &p);
	int EvalNormal(const float *parameter,Point &n);

};


class GSpiral : public GCurve
{
   GV_NODE_HEADER(GSpiral);
public:
   GvSFFloat r0;
   GvSFFloat r1;
   GvSFFloat angle0;
   GvSFFloat angle1;
   GvSFFloat z0;
   GvSFFloat z1;

};

class GSinus : public GCurve
{
   GV_NODE_HEADER(GSinus);
public:
   GvSFFloat r0;
   GvSFFloat r1;

};

class GPointOnCurve : public GvNode
{
   GV_NODE_HEADER(GPointOnCurve);
public:
	// fields
	GvSFNode	curve; // the curve

	// EventIn
	GvSFFloat	fraction;	// parameter to evaluate cuve

	// EventOut
	GvSFVec3f	value;		// evaluated point

};

// Point on Curve2


// ********** GSurface
class GSurface : public GvNode {
   GV_NODE_HEADER(GSurface);
public:

	// Evaluate a point at parameter(u,v)
	virtual int Eval(const Point &parameter,Point &p);
	
	// Evaluate normal at parameter(u,v)
	virtual int EvalNormal(const Point &parameter,Point &n);

	// Evaluate a point & normal at parameter(u,v)
	virtual int Eval(const Point &parameter,Point &p,Point &n);
	
	// Map point to parameter
	virtual int Map(const Point &p,Point &parameter);

	// Ray intersection
	virtual int Intersect(const Ray &ray,float &t); 

	virtual int Intersect(const Ray &r,Point &p);

	// is Eval(0,x) == Eval(1,x) ?
	virtual int ClosedInU() { return(0); };
	// is Eval(x,0) == Eval(x,1) ?
	virtual int ClosedInV() { return(0); };

	// get bbox
	// Inside
	// Distance(const Point *p)

};

class GSkinSurface : public GSurface {
   GV_NODE_HEADER(GSkinSurface);
public:

   GvMFNode curves;  // a set of profiles to connect

};

class GSweepSurface : public GSurface {
   GV_NODE_HEADER(GSweepSurface);
public:

   GvSFNode profile;  //profile
   GvSFNode path;     // sweep profile along path
   GvSFBool tangential; // flag
   
   GvSFNode modulator;  // optional 1d modulator curve 


};


// coons bpathc
class GBilinearSurface : public GSurface 
{
   GV_NODE_HEADER(GBilinearSurface);
public:

   GvSFNode left,right;
   GvSFNode top,bottom;

};


class GModulatedSurface : public GSurface

{
   GV_NODE_HEADER(GModulatedSurface);
public:

   GvSFNode surface;
   GvSFNode modulator;  // 1 D curve
};

class GTransformedSurface : public GSurface
{
   GV_NODE_HEADER(GTransformedSurface);
public:

   GvSFNode curve;
   GvSFNode transformation;
};

class GConcatenatedSurface : public GSurface
{
   GV_NODE_HEADER(GConcatenatedSurface);
public:

   GvSFLong rows;
   GvSFLong cols;

   GvMFNode surfaces;
};


class GInterpolatedSurface : public GSurface
{
   GV_NODE_HEADER(GInterpolatedSurface);
public:

   GvSFFloat t;
   GvMFNode surfaces;

};




class GSphereSurface : public GSurface {


   GV_NODE_HEADER(GSphereSurface);


public:
	
	GvSFFloat r;	// Default : 1.0

	int ClosedInU() { return(1); };
	int ClosedInV() { return(1); };


	int Eval(const Point &parameter,Point &p) 
	{
	   float u = (parameter.x+0.25) * TWOPI;
	   float v = (parameter.y-0.0 ) * PI;
	   p.Set(r*cos(u)*sin(v),r*sin(u)*sin(v),r*cos(v));
	   return(1);
	};
	
	int EvalNormal(const Point &parameter,Point &n) {
	   Eval(parameter,n);
	   n.Normalize();
	   return(1);
	};

	// Evaluate a point & normal at parameter(u,v)
	int Eval(const Point &parameter,Point &p,Point &n)
	{
	   float u = (parameter.x+0.25) * TWOPI;
	   float v = (parameter.y-0.0 ) * PI;
	   n.Set(cos(u)*sin(v),sin(u)*sin(v),cos(v));
	   p = n * r;
	   return(1);
	}

	
	int Map(const Point &p,Point &parameter); 
	int Intersect(const Ray &ray,float &t); 


}; // Sphere Surface


// cylinder surface
class GCylinderSurface : public GSurface {
   GV_NODE_HEADER(GCylinderSurface);
public:

public:
	GvSFFloat r;	// default : 1.0
	GvSFFloat h;    // default : 2.0
	

	int ClosedInU() { return(1); };

	int Eval(const Point &parameter,Point &p) 
	{
	   float u = (parameter.x+0.25) * TWOPI;
	   float v = (parameter.y-0.5) * h;
	   p.Set(r*cos(u),r*sin(u),v);
	   return(1);
	};
	
	int EvalNormal(const Point &parameter,Point &n) {
	   float u = (parameter.x+0.25) * TWOPI;
	   n.Set(cos(u),sin(u),0);
	   return(1);
	};


	int Eval(const Point &parameter,Point &p,Point &n) {
	   float u = (parameter.x+0.25) * TWOPI;
	   float v = (parameter.y-0.5) * h;
	   n.Set(cos(u),sin(u),0);
	   p.Set(r*n.x,r*n.y,v);
	   return(1);
	}

    int Map(const Point &p,Point &parameter);
	int Intersect(const Ray &ray,float &t);


};


// Simple xy-plane surface, 
class GXYPlaneSurface : public GSurface {
   GV_NODE_HEADER(GXYPlaneSurface);
public:

	GvSFVec2f scale;

public:

	int Eval(const Point &parameter,Point &p) 
	{
	   p.Set(scale[0]*parameter.x,scale[1]*parameter.y,0);
	   return(1);
	};
	
	int EvalNormal(const Point &parameter,Point &n) {
	   n.Set(0.0,0.0,1.0);
	   return(1);
	};

	int Eval(const Point &parameter,Point &p,Point &n)
	{
	   p.Set(scale[0]*parameter.x,scale[1]*parameter.y,0);
	   n.Set(0.0,0.0,1.0);
	   return(1);
	}


    int Map(const Point &p,Point &parameter) { parameter.Set(p.x/scale[0],p.y/scale[1],0.0); return(1); }

	int Intersect(const Ray &ray,float &t);

};


class GTorusSurface : public GSurface {
   GV_NODE_HEADER(GTorusSurface);
public:

	GvSFFloat rMajor;
	GvSFFloat rMinor;
	
//	GTorusSurface(float RMajor=1.0,float RMinor=0.25) : rMajor(RMajor),rMinor(RMinor) {};
	int ClosedInU() { return(1); };
	int ClosedInV() { return(1); };

};

//
//  a point evaluated on a surface 
// 


class GPointOnSurface : public GvNode
{
   GV_NODE_HEADER(GPointOnSurface);
public:
	// fields
	GvSFNode	surface; // the surface

	// EventIn
	GvSFVec2f	parameter;	// parameter to evaluate surface

	// EventOut
	GvSFVec3f	value;		// evaluated point on surface

};

//
//  a curve evaluated on a surface 
// 

class GCurveOnSurface : public GCurve
{
   GV_NODE_HEADER(GCurveOnSurface);
public:
	// fields
	GvSFNode	surface; // the surface

	GvSFNode	parameterCurve;	// curve in parameter space 


};


class GCurveDisplay : public GvNodeShell
{
   GV_NODE_HEADER(GCurveDisplay);
public:
	// fields
	GvSFVec2f	range;	// the range to display (default 0,1)
	GvSFFloat	parameterStep;	//  parameter stepping value  (default 0.1)
	GvSFNode	curve; // the curve

	int	BuildShell(GTraversal &state);

};

class GSurfaceDisplay : public GvNodeShell
{
   GV_NODE_HEADER(GSurfaceDisplay);
public:

	// fields
	GvSFVec2f	urange;	// the u range to display (default 0,1)
	GvSFVec2f	vrange;	// the v range to display (default 0,1)
	GvSFVec2f	parameterStep;	//  parameter stepping value 
	GvSFNode	surface; // the surface

	int	BuildShell(GTraversal &state);

};


#endif
