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


#include "stdafx.h"

#include "gshell.h"

#include "gsurface.h"



//
// GCurve
//

GV_NODE_SOURCE_IMP(GCurve,GvNode);


GCurve::GCurve():urange(0.0,1.0)
{
    GV_NODE_CONSTRUCTOR(GCurve);
    isBuiltIn = FALSE;
    GV_NODE_ADD_FIELD(urange);

}

GCurve::~GCurve()
{
}

int GCurve::Eval(const float *parameter,Point &p)
{
	p.Set(0,0.0);
	return(9);
}

int GCurve::EvalNormal(const float *parameter,Point &n)
{
	n.Set(0,1,0);
	return(0);
}


//
// GCircle
//
GV_NODE_SOURCE_IMP(GCircle,GCurve);

GCircle::GCircle(): r(1.0)
{
	GV_NODE_CONSTRUCTOR(GCircle);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(r);
    GV_NODE_ADD_FIELD(urange);
}

GCircle::~GCircle()
{
}

int GCircle::Eval(const float *parameter,Point &p)
{
	float w = (0.25 + parameter[0]) * TWOPI;	
	p.Set(r*cos(w),r*sin(w),0.0);
	return(2);
}

int GCircle::EvalNormal(const float *parameter,Point &n)
{
	float w = (0.25 + parameter[0]) * TWOPI;	
	n.Set(cos(w),sin(w),0.0);
	return(2);
}


//
// GSurface
//
GV_NODE_SOURCE_IMP(GSurface,GvNode);

GSurface::GSurface()
{
	GV_NODE_CONSTRUCTOR(GSurface);
}

GSurface::~GSurface()
{
}

// Evaluate a point for parameter(u,v)
int GSurface::Eval(const Point &parameter,Point &p)
{
	return(0);
}

// Evaluate normal at parameter(u,v)
int GSurface::EvalNormal(const Point &parameter,Point &n)
{
	TRACE("GSurface::EvalNormal not implemented! \n");

	return(0);
}

// Evaluate a point & normal for parameter(u,v)
int GSurface::Eval(const Point &parameter,Point &p,Point &n)
{
	Eval(parameter,p);
	return(EvalNormal(parameter,n));
}


// Map point to parameter
int GSurface::Map(const Point &p,Point &parameter)
{
	return(0);
}

// Ray intersection
int GSurface::Intersect(const Ray &ray,float &t)
{
	TRACE("GSurface::Intersect not implemented! \n");

	return(0);
}

int GSurface::Intersect(const Ray &ray,Point &p)
{
    float t;
    if (Intersect(ray,t)) {
	   p = ray.Eval(t);
	   return(1);
	} else return(0); 
}



//
// GSphereSurface
//
GV_NODE_SOURCE_IMP(GSphereSurface,GSurface);

GSphereSurface::GSphereSurface(): r(1.0)
{
	GV_NODE_CONSTRUCTOR(GSphereSurface);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(r);
}

GSphereSurface::~GSphereSurface()
{
}


//
// GCylinderSurface
//
GV_NODE_SOURCE_IMP(GCylinderSurface,GSurface);

GCylinderSurface::GCylinderSurface(): r(1.0),h(2.0)
{
	GV_NODE_CONSTRUCTOR(GCylinderSurface);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(r);
    GV_NODE_ADD_FIELD(h);
}

GCylinderSurface::~GCylinderSurface()
{
}


//
// GXYPlaneSurface
//
GV_NODE_SOURCE_IMP(GXYPlaneSurface,GSurface);

GXYPlaneSurface::GXYPlaneSurface() : scale(1,1)
{
	GV_NODE_CONSTRUCTOR(GXYPlaneSurface);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(scale);
}

GXYPlaneSurface::~GXYPlaneSurface()
{
}



// to do: check mappings !!!



// ********** Gsphere Surface
int GSphereSurface::Map(const Point &px,Point &parameter)
{
	Point p(px);

	p.Normalize();

	float u,v = p.z / r;

	if (v <= -1.0) { u = 0.0; v = 0.0; }
	else if (v >= 1.0)  { u=0.0; v = 1.0; }
	else {
	  v=	acos(v) / PI;
	  u = (atan2(p.y,p.x)+PI) / TWOPI;
	}
	parameter.Set(u,v,0.0);
	//opt set v to distance
#if 0
	float phi = acos(p.y);
    float theta = p.x / sqrt(1.0 - p.y * p.y);
    if (fabs(theta) > 1.0) {
	parameter.Set(0, phi / PI, 0);
	return(1);
    }
    theta = acos(theta) / (2.0 * PI);
    if (p.z > 0) {
	parameter.Set(theta, phi / PI, 0);
    }
    else parameter.Set(1.0 - theta, phi / PI, 0);
#endif
	return(1);
};


int GSphereSurface::Intersect(const Ray &ray,float &t)
/* from MTV Ray-Tracer */
/* see also Graphic Gems I, Pg. 388 */
{
	float b, disc;
	Point V ;

	//v = *sph_center -  ray.p;;
	V = -ray.p;

	b = DotProd(V, ray.d);

	disc = b * b - DotProd(V, V) + (r*r) ;

	if (disc < 0.0)
		return(0);

	disc = sqrt(disc);

	t = (b - disc < EPS) ? b + disc : b - disc ;

	if (t < EPS) {
//?		return(0);
	}

//	*isect_t = t ;
/*	hit -> isect_enter = VECDOT(V, V) > r2 + EPS ? 1 : 0 ; */
	return (1);
};




// GCylinder Surface
int GCylinderSurface::Map(const Point &p,Point &parameter)
{

	float u,v = p.z / h;

/*	if (v <= -1.0) { u = 0.0; v = 0.0; }
	else if (v >= 1.0)  { u=0.0; v = 1.0; }
	else
*/
	{
	  u =  (atan2(p.y,p.x)+PI) / TWOPI;
	}
	parameter.Set(u,v,0.0);
	return(1);
}

/* From povray */
int GCylinderSurface::Intersect(const Ray &ray,float &t)
{
  const double Cone_Tolerance = 1.0e-6;

  double a, b, c, z, t1, t2, len;
  double d;
  Point P, D;
  int i=0;

   len = ray.d.Len();
   D = ray.d * 1.0/ len;
   P = ray.p;


   /* Solve intersections with a cylinder */
   a = D.x * D.x + D.y * D.y;
   if (a > EPS)
      {
      b = P.x * D.x + P.y * D.y;
      c = P.x * P.x + P.y * P.y - 1.0;
      d = b * b - a * c;
      if (d >= 0.0)
	{
	d = sqrt(d);
	t1 = (-b + d) / a;
	t2 = (-b - d) / a;
	z = P.z + t1 * D.z;
	// HG return nearest intersection
	if (t1 > Cone_Tolerance) {
		if ( t2 > Cone_Tolerance) {
			t = min(t1,t2);
		}
		else t = t1;
		return(1);
      }

#if 0
	if (t1 > Cone_Tolerance && t1 < Max_Distance && z >= 0.0 && z <= 1.0)
	  Depths[i++] = t1/len;
	z = P.z + t2 * D.z;
	if (t2 > Cone_Tolerance && t1 < Max_Distance && z >= 0.0 && z <= 1.0)
	  Depths[i++] = t2/len;
#endif
	}
      }
   return(0);
}

int GXYPlaneSurface::Intersect(const Ray &ray,float &t)
{
      double NormalDotOrigin, NormalDotDirection;

      NormalDotOrigin = -ray.p.z;

      NormalDotDirection= ray.d.z;
      if (iszero(NormalDotDirection )) return (0);
      t  = NormalDotOrigin / NormalDotDirection;
      return (1);
}




//
// GCurveDisplay
//

GV_NODE_SOURCE_IMP(GCurveDisplay,GvNodeShell);


GCurveDisplay::GCurveDisplay():
		range(0.0,1.0),
		parameterStep(0.1)
{
    GV_NODE_CONSTRUCTOR(GCurveDisplay);
    isBuiltIn = FALSE;
    GV_NODE_ADD_FIELD(range);
    GV_NODE_ADD_FIELD(parameterStep);
    GV_NODE_ADD_FIELD(curve);
}

GCurveDisplay::~GCurveDisplay()
{
}

int			
GCurveDisplay::BuildShell(GTraversal &state)
{
	SetShell(NULL);
	GCurve *c = (GCurve*) curve.Eval(state);
	if (!c) return(NULL);
	PointArray values;
	Point value;
	float u;
	float du= parameterStep;
	if (du<0)  du = (range[1]-range[0]) / -du;

	if (du== 0.0) du = 0.1;

	// evaluate points 
	for(u=range[0]; u<range[1];u+=du) {
		c->Eval(&u,value);
		values.Append(value);
	}
	c->Eval(&range[1],value);
	values.Append(value);
	
	SetShell(new GPolyline(values.Length(),values.Data()));
	return(1);
}

//
// GSurfaceDisplay
//

GV_NODE_SOURCE_IMP(GSurfaceDisplay,GvNodeShell);


GSurfaceDisplay::GSurfaceDisplay():
		urange(0.0,1.0),
		vrange(0.0,1.0),
		parameterStep(0.1,0.1)
{
    GV_NODE_CONSTRUCTOR(GSurfaceDisplay);
    isBuiltIn = FALSE;
    GV_NODE_ADD_FIELD(urange);
    GV_NODE_ADD_FIELD(vrange);
    GV_NODE_ADD_FIELD(parameterStep);
    GV_NODE_ADD_FIELD(surface);
}

GSurfaceDisplay::~GSurfaceDisplay()
{
}

int			
GSurfaceDisplay::BuildShell(GTraversal &state)
{
	SetShell(NULL);
	GSurface *c = (GSurface*) surface.Eval(state);
	if (!c) return(NULL);
	PointArray values;
	Point value;
	float u;
	float du= parameterStep[0];
	if (du<0)  du = (urange[1]-urange[0]) / -du;
	if (du== 0.0) du = 0.1;
	float v;
	float dv= parameterStep[1];
	if (dv<0)  dv = (vrange[1]-vrange[0]) / -dv;
	if (dv== 0.0) dv = 0.1;

	// evaluate points 
	int rows=0;
	int cols = 0;
	Point param(0,0,0);
	
	for(v=vrange[0]; v<=vrange[1];v+=dv) {
		cols = 0;
		param.y = v;
		for(u=urange[0]; u<urange[1];u+=du) {
			param.x = u;
			c->Eval(param,value);
			values.Append(value);
			cols++;
		}
		param.x = urange[1];
		c->Eval(param,value);
		values.Append(value);
		cols++;
		rows++;
	}
	// create geometry 	
	SetShell(new GMesh(rows,cols,values.Data()));
	return(1);
}

