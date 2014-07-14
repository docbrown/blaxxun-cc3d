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
/*
  transl sweep like deform

  controlpoints 2d-grid sweeped tangential along curve,
  used as bezier deformation

  curve point normal,scale and twist ?
  defom ctrlpt :

	p, scale, twist

   make conic = tapper : scale 1=r1,   scalen=rn
   bend : replace curve with required curve eg line ==> arc
   twist : rotate along tangent = twist

*/
#include "stdafx.h"

#include "gutils.h"

//#include "point.h"
//#include "matrix.h"
//#include <math.h>
// For GParameter
#include "gclass.h"
#include "gshell.h"
#include "gshellop.h"
#include "gshelli.h"

#include "gsurface.h"

#include "gdeform.h"

 

// ********** GDeform

int GDeform::Eval(const Point &p,Point	&newp)
{
  newp=p;
  return(0);
}


int GDeform::Eval(int np, const Point *p,Point *newp)
{ int cnt=0;
  const Point *pp=p;
  Point *newpp=newp;

  for (; np>0; np--,p++,newpp++) {

	Point target(*p);		;
	if (hasm) target *= m.inverse; // transform to deformer space

	*newpp=target;

	cnt+=Eval(*p,*newpp);	/* apply deformation */

	if (hasm) *newpp *= m.m; // transform back from deformer space
  }
  return(cnt);
}

int GDeform::Eval(int np, const Point *pparam,const Point *p,Point *newp)
{ return(Eval(np,p,newp)); }



// at radius delta = 0
// at center delta = 1.0*radiusvector*scale

int GSphereDeform::Eval(const Point &p,Point &delta)
{
 Point d=p-center;
 float l = d.Len();
 if (l>r) return(0);
 float amount = (r-l)/r * scale;
 delta += d*amount;
 return(1);
}
class GTapperDeform : public GDeform {
public:
	float scale1;
	float scale2;
	// diffrent rules ?

	void SetScale1(float Scale) { scale1 = Scale; }
	void SetScale2(float Scale) { scale2 = Scale; }
	virtual int Eval(const Point &p, Point &delta);
};
// at radius delta = 0
// at center delta = 1.0*radiusvector*scale

int GTapperDeform::Eval(const Point &p,Point &newP)
{ float scale;
  float t = p.z;
  if ((t<0.0) || (t>1.0)) return(0);
  scale = (1.0-t) *scale1 + (t*scale2);		// or a spline = curve->eval(t);
  newP.x = p.x * scale;
  newP.y = p.y * scale;
  return(1);
}
#if 0
class GBendDeform : public GDeform {
public:
	Point center;
	float radius1;
	float radius2;
	float angle1;
	float angle2;
	// diffrent rules ?

	void SetScale1(float Scale) { scale1 = Scale; }
	void SetScale2(float Scale) { scale2 = Scale; }
	virtual int Eval(const Point &p, Point &newP);
};
// at radius delta = 0
// at center delta = 1.0*radiusvector*scale

int GBendDeform::Eval(const Point &p,Point &newP)
{ float scale;
  t = p.z;
  if (t<0.0) || (t>1.0) return(0);
  angle = (1.0-t) *angle1 + (t*angle2);
  radius = (1.0-t) *radius1 + (t*radius2);		// or a spline = curve->eval(t);
  newP.x = p.x * scale;
  newP.y = p.y * scale
  return(1);
}
#endif



/* stretched_box class */

/* normalize Points p to box box */
void norm_points(int np,const Point *p,BBox &box,Point *op)
{ Point d;

  d=box.ma-box.mi;
  if (!iszero(d.x)) d.x = 1.0 / d.x;
  if (!iszero(d.y)) d.y = 1.0 / d.y;
  if (!iszero(d.z)) d.z = 1.0 / d.z;

  for ( ; np>0; np--,p++,op++) {
	op->x = (p->x - box.mi.x) * d.x;
	op->y = (p->y - box.mi.y) * d.y;
	op->z = (p->z - box.mi.z) * d.z;
  }
}

void GDeform::Map2Box(int np,const Point *p,Point *op)
{
  norm_points(np,p,box,op);
}
/*****************************************************************************/
/* bilinear interpolation inside a quadrilateral */
static
void interp22_point(const Point *a,const Point *b, const Point *c, const Point *d,
		   float u,float v,Point *i)
{ float fa,fb,fc,fd;
  fa=(1-u)*(1-v);
  fb=(1-u)*(v);
  fc=(u)*(v);
  fd=(u)*(1-v);
  i->x = fa * a->x + fb * b->x + fc * c->x + fd * d->x;
  i->y = fa * a->y + fb * b->y + fc * c->y + fd * d->y;
  i->z = fa * a->z + fb * b->z + fc * c->z + fd * d->z;
}

/* trilinear interpolation inside a 8 point box  */
static
void interp3_point(const Point *box, float u,float v,float w, Point *i)
{ Point i1,i2;
  float fa,fb;
  fa=(1.0-w);
  fb=w;
  interp22_point(&box[0],&box[1],&box[2],&box[3],u,v,&i1);
  interp22_point(&box[4],&box[5],&box[6],&box[7],u,v,&i2);
  i->x = fa * i1.x + fb * i2.x;
  i->y = fa * i1.y + fb * i2.y;
  i->z = fa * i1.z + fb * i2.z;
}
/* compute n! */
static int fak(int n)
{int r;
  if (n<=1 ) return (1);
  r=1;
  do  r *= n;
  while (n-- > 1);
  return r;
}

/* compute n ! / (k!*(n-k)! ) */
static double binominal(int n,int k)
{
 if (k==0)  return(1.0);
 if (k>n)  return(0.0);
/*  if ((k==1) || (k==n))	return(n); */
 return ((double) fak(n) / ( (double) fak(k) * (double) fak(n-k)));
}


static
double bernstein(int k,int n,double t)
{
  if (n==0) return(1.0);
/*
  f1=1.0;
  if (k>4) f1 = pow(t,(double) k);
  else
  switch (k) {
  case 4: f1*=t;
  case 3: f1*=t;
  case 2: f1*=t;
  case 1: f1*=t;
  case 0:
  }
*/

  return (binominal(n,k)*pow(t,(double) k)*pow(1.0-t,(double)(n-k)));

/*  return((1.0-t) * bernstein(k,n-1,t) + t* bernstein(k,n-1,t);	 */
}

/* compute transformed Points */
void stretch_Points(int np,const Point *p,const Point *stretched_box,Point *op)
{
  for ( ; np>0; np--,p++,op++) {
     interp3_point(stretched_box,p->x,p->y,p->z,op);
  }
}


void compute_bezier_volume_point(int bx,int by,int bz, const Point *b,
			float u,float v,float w,
			Point *o)
{ int i,j,k;
  double fi,fj,fk;
  double x,y,z;
  const Point *pp;

  o->Zero();

  x=y=z=0.0;

  pp=b;
  for (j=0; j<bz; j++) {
      fj=bernstein(j,bz-1,w);
      for (i=0; i<by; i++) {
	fi=fj*bernstein(i,by-1,v);
	for (k=0; k<bx; k++) {
	  fk=fi*bernstein(k,bx-1,u);
	  *o += *pp * fk;
	  pp++;
	}
    }
  }
}



#define EPS0 -0.00001
#define EPS1  1.00001

void bezier_stretch_points(int np,const Point *p,const Point *org_p,
			   int bx,int by, int bz,const Point *ctrl_box,Point *op)
{
  for ( ; np>0; np--,p++,op++,org_p++) {
     if ((p->x >= EPS0) && (p->x <= EPS1) && (p->y >= EPS0)
	  && (p->y <= EPS1) && (p->z >= EPS0) && (p->z <= EPS1))
	compute_bezier_volume_point(bx,by,bz,ctrl_box,p->x,p->y,p->z,op);
     else *op = *org_p;
  }

}

class GBezierDeform : public GDeform
{
public :
  int bx,by,bz;		// number of control-points in each dimension
  int trilinear;	// if bx==by==bz==2
  Point *stretchbox;	// the Array[bx*bx*bz] of controlpoints

  int Eval(int np,
			const Point *p, 	/*  in parameter space */
			const Point *org_p,	/*  in object space */
			Point *op);		/*  in object space */
  int EvalBox(int np,
			const Point *p, 	/*  in parameter space */
			const Point *org_p,	/*  in object space */
			Point *op);		/*  in object space */

};

int GBezierDeform::Eval(int np,
			const Point *p, 	/*  in parameter space */
			const Point *org_p,	/*  in object space */
			Point *op)			/*  in object space */
{ int cnt=0;

  if (trilinear) return(EvalBox(np,p,org_p,op));

  for ( ; np>0; np--,p++,op++,org_p++) {
     if ((p->x >= EPS0) && (p->x <= EPS1) && (p->y >= EPS0)
	  && (p->y <= EPS1) && (p->z >= EPS0) && (p->z <= EPS1)) {
	 compute_bezier_volume_point(bx,by,bz,stretchbox,p->x,p->y,p->z,op);
	 cnt ++;
     }
     else *op = *org_p;
  }
  return(cnt);
}
int GBezierDeform::EvalBox(int np,
			const Point *p, 	/*  in parameter space */
			const Point *org_p,	/*  in object space */
			Point *op)		/*  in object space */
{ int cnt=0;
  for ( ; np>0; np--,p++,op++,org_p++) {
     if ((p->x >= EPS0) && (p->x <= EPS1) && (p->y >= EPS0)
	  && (p->y <= EPS1) && (p->z >= EPS0) && (p->z <= EPS1)) {

		 interp3_point(stretchbox,p->x,p->y,p->z,op);
		 cnt++;
     }
     else *op = *org_p;
  }
  return(cnt);
}


#if 0
void apply_sphere_stretch(stretcher *s,
			int np,const Point *p,const Point *org_p,
			Point *op)
{
  for ( ; np>0; np--,p++,op++,org_p++) {
     if ((p->x >= EPS0) && (p->x <= EPS1) && (p->y >= EPS0)
	  && (p->y <= EPS1) && (p->z >= EPS0) && (p->z <= EPS1)) {
	Point pc,d;
	/* VECSUB(pc,*p,s->center); */
	VECSUB(pc,*org_p,s->center);
	vecnorm(&d,&pc);
	VECOP4C(pc,=,s->center,+,d,*,s->radius*s->radius_scale); /* Point on sphere */

	VECIPOL(*op,*org_p,pc,s->t);
     }
     else *op = *org_p;
  }
}
#endif
#if 0
class GSinDeform : public GDeform
{
public:
   float wave_offset,waves;
   float begin_scale;
   float end_scale;

}

#define LERP(A,B,T) ((A)+ ((B)-(A)) * (T))

void apply_sin_stretch(stretcher *s,
			   int np,const Point *p,const Point *org_p,
			   Point *op)
{
  for ( ; np>0; np--,p++,op++,org_p++) {
     if ((p->x >= EPS0) && (p->x <= EPS1) && (p->y >= EPS0)
	  && (p->y <= EPS1) && (p->z >= EPS0) && (p->z <= EPS1))
     {
	double x = (s->wave_offset + p->x + s->waves) * TWOPI;
	double y = sin(x);
	double z = y * (LERP(s->begin_scale,s->end_scale,p->x));
	op->x = org_p->x;
	/* op->y = org_p->y; */
	op->y = LERP(s->bbox[0].y,s->bbox[1].y,z);
	op->z = LERP(s->bbox[0].z,s->bbox[1].z,z);
     }
     else *op = *org_p;
  }
}
#endif




// ********** GParameter
GParameter::~GParameter()
{
  if (surface) delete surface;
  surface = 0;
}

int GParameter::SetSource(const char * f)
{
			if (streq(f,"vertex")) source = GPObject;
			else if (streq(f,"vertex normal")) source = GPNormal;
			else if (streq(f,"vertex color")) source = GPColor;
			else if (streq(f,"vertex parameter")) source = GPParameter;
			else if (streq(f,"native")) source = GPNative;
			else return(-1);
			strcpy(Source,f);
			return(source);
};

int GParameter::SetFunction(const char * f) {
			if (streq(f,"xyz")) function = GPFxyz;
			else if (streq(f,"xzy")) function = GPFxzy;
			else if (streq(f,"yzx")) function = GPFyzx;
			else if (streq(f,"sphere")) {
			    function = GPFsurface;
				if (surface) delete surface;
				surface = new GSphereSurface;
			}
			else if (streq(f,"cylinder")) {
			    function = GPFsurface;
				if (surface) delete surface;
				surface = new GCylinderSurface;
			}
			else if (streq(f,"plane")) {
			    function = GPFsurface;
				if (surface) delete surface;
				surface = new GXYPlaneSurface;
			}
			else {
			   TRACE("GParameter:: Bad function : %s\n",f);	
			   return(-1);
			}
			strcpy(Function,f);
			return(function);
};

int GParameter::SetMapping(const char * f)
{
			if (streq(f,"none")) mapping = GPMNone;
			else if (streq(f,"project")) mapping = GPMNone;
			else if (streq(f,"intersect point + normal")) mapping = GPMONormal;
			else if (streq(f,"intersect center + normal")) mapping = GPMOCenter;
			else if (streq(f,"intersect reflected eye ray")) mapping = GPMReflected;
			else if (streq(f,"intersect eye ray")) mapping = GPMEye;
			else {
			      TRACE("GParameter::Unknown mapping : %s\n",f);
				return(-1);
			}
			strcpy(Mapping,f);
			return(mapping);
};




int GParameter::ApplyShell(class GShell &shell)
{   Point P,N;
    int  transform=0;
    Point scale,translate;

	PointArray nativeP;

	// Allocate parameters array
	if (shell.vp.Length() <=0) {
	   shell.SetVP(shell.v.Length(),shell.v);
	}
	
	if (shell.vp.Length() < shell.v.Length()) shell.vp.SetLength(shell.v.Length());


	if (source == GPNative) {
	   nativeP.SetLength(shell.v.Length());
	   shell.GetVertexParametersNative(nativeP.Length(),nativeP.DataRW());
	}

    if (NeedsOBBox()) {
	  BBox box;
	  shell.ComputeBBox(box);
	  //SetBBox(box);
	  /* compute mapping to unit cube (-1,-1,-1) (1, 1, 1) */
	  Point d = box.ma - box.mi;
	  float r= max(max(d.x,d.y),d.z);
	  //scale.Set(2.0/d.x,2.0/d.y,2.0/d.z);
	  scale.Set(2.0/r,2.0/r,2.0/r);
	  translate =  -(box.mi + d *0.5);
	  transform = 1;
	};

	if (NeedsOCenter()) {
	  Point center;
	  shell.ComputeCenter(center);
	  if (transform) {  center += translate; center *= scale;}
	}

	for (int i=0; i<shell.v.Length(); i++) {

	// get source input into p
	switch (source) {
	case GPWorld:		// Object world coordinates
		P = shell.v[i];
		P *= object2world;
		break;
	case GPNormal:		// object vertex normals
		if (shell.vn) { P = shell.vn[i]; break; }
	case GPParameter:	// object vertex parameters
		if (shell.vp) { P = shell.vp[i]; break; }
	case GPColor:		// object vertex color
		if (shell.vc) { P = shell.vc[i]; break; }
	case GPNative : 		// native parametrization eg. QuadMesh, cylinder ..
		 if (nativeP) {  P = nativeP[i]; break;}
	case GPObject:		// object coordintes
	default :
		P = shell.v[i];
		if (transform) { P += translate; P *= scale; }
		break;
	} // source

	switch (mapping) {
	default :
	case GPMNone:
			if (hasm) P *= m.inverse; // transform to mapping surface space
			EvalType(P,P);
			break;

	case GPMONormal : {
			Ray ray(P,shell.vn[i]);
			if (hasm) ray *= m.inverse; // transform to mapping surface space
			Point ip;
			if (surface->Intersect(ray,ip)) {
			   surface->Map(ip,P);
			}
			break;
			}
	case GPMEye : {
			Ray ray(viewpoint,P-viewpoint);
			if (hasm) ray *= m.inverse; // transform to mapping surface space
			ray.Normalize();
			Point ip;
			if (surface->Intersect(ray,ip)) {
			   surface->Map(ip,P);
			}
			break;
			}
	case GPMReflected : {
			Ray ray(viewpoint,P-viewpoint);
			ray.Normalize();
			Vector R;
			R = ray.Reflect(shell.vn[i]);
			ray.p = P;
			ray.d = R;
			if (hasm) ray *= m.inverse; // transform to mapping surface space
			ray.Normalize();
			Point ip;
			if (surface->Intersect(ray,ip)) {
			   surface->Map(ip,P);
			}
			break;
			}
	case GPMOCenter : {
			Ray ray(center,shell.vn[i]);
			if (hasm) P *= m.inverse; // transform to mapping surface space
			Point ip;
			if (surface->Intersect(ray,ip)) {
			   surface->Map(ip,P);
			}
			break;
			}
	}  // Mapping

	shell.vp[i] = P;
	} // vertices
	shell.edit_flags |= SHELL_VERTEX_ATTRIBUTES_CHANGED;
	return(0);
}


// map point p to parameter value
int GParameter::EvalType(const Point &p,Point  &parameter) 
{
	switch (function) {
	case GPFxyz: parameter = p; break;
	case GPFxzy: parameter.Set(p.x,p.z,p.y); break;
	case GPFyzx: parameter.Set(p.y,p.z,p.x); break;
	case GPFsurface : if (surface) surface->Map(p,parameter); break;
	}
	return(1);
};


int GParameter::Eval(const Point &p,Point  &parameter)
{
  return(EvalType(p,parameter));
}

int GParameter::Eval(int np, const Point *p,Point *newp)
{ int cnt=0;
  const Point *pp=p;

  Point *newpp=newp;

  for (; np>0; np--,p++,newpp++) {

	Point target(*p);		;
	if (hasm) target *= m.inverse; // transform to deformer space

	*newpp=target;

	cnt+=Eval(*p,*newpp);	/* apply deformation */

	if (hasm) *newpp *= m.m; // transform back from deformer space
  }
  return(cnt);
}


#if 0
int Map2Sphere(const Point &p,Point  &parameter)
{
  return (0);
}

int Map2Cylinder(const Point &p,Point  &parameter)
{
  return (0);
}


#endif

