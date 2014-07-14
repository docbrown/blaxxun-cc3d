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
#ifndef _GDEFORM_H
#define _GDEFORM_H



class Point;
class BBox;

class Matrix;
class MatrixAndInverse;

class GSurface;


#include "gutils.h"

enum GIdDeformID 
	{
	GIdSphereDeform=1000,
	GIdTapperDeform,
	GIdBezierDeform	
	};


class GDeform {

public:
	int id;
   	void SetId(int Id) { id = Id; }

	GDeform(int Id =0) { hasbox=0; hasm=0; SetId(Id); }

	// evaluate Deformation at Point p and add to delta
	// Result 1, deform has been applied
	virtual int Eval(const Point &p,Point  &delta);

	// evaluate Deformation of a set of Points, newp will be initialized
	virtual int Eval(int np, const Point *p,Point *newp);

	virtual int Eval(int np, const Point *pparam,const Point *p,Point *newp);


	int hasbox;
	BBox box;

	int hasm;
	MatrixAndInverse m;  //opt invtrans pts to deformer space and transform deform pts back

	void SetMatrix(const Matrix &M) { m.Set(M); hasm=1; }
	void UnSetMatrix() { m.SetIdentity(); hasm=0; }
	void AppendMatrix(const Matrix &M) { m.Append(M); hasm=1; }

	virtual void Translate(const Point &p) { AppendMatrix(TranslationMatrix(p)); }

	void SetBBox(BBox Box) { box = Box; hasbox = !box.IsEmpty(); }

	// Check if Points within test box are potentially affected

	virtual int CheckBBox(const BBox &testBox) {
		if (hasbox) return(!box.Outside(testBox));
		return(1); }

	// Map Points from objectspace to boxspace [0..1,0..1,0..1]
	void Map2Box(int np,const Point *p,Point *op);


};

class GSphereDeform : public GDeform 
{
public:
	Point center;
	float r;
	float scale;		// scale <0 magnet !!!?
	int display;
	// diffrent rules ?

	GSphereDeform(Point Center,float R=1.0, float Scale = 0.5) : 
				GDeform(GIdSphereDeform) {display=0; SetCenter(Center); r=R; scale = Scale; }


	void SetCenter(Point Center) { center = Center; }
	void SetRadius(float Radius) { r = Radius; }
	void SetScale(float Scale) { scale = Scale; }
	void GetCenter(Point &Center) { Center = center; }
	void GetRadius(float &Radius) {  Radius = r; }
	void GetScale(float &Scale) { Scale = scale; }

	void Translate(const Point &p) { center +=p; }

	virtual int Eval(const Point &p, Point &delta);
};



#if 0
// ********** GSurface
class GSurfaceX {
public :

	// Evalue a point for parameter(u,v)
	virtual int Eval(const Point &parameter,Point &p) = 0;
	
	// Evaluate normal at parameter(u,v)
	virtual int EvalNormal(const Point &parameter,Point &n);
	
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
class GSphereSurfaceX : public GSurfaceX {
public:
	float r;

	float r2;		// Radius sqared for Interersect
	
	GSphereSurfaceX(float R=1.0) : r(R) { r2=sqr(r);};
	
	int ClosedInU() { return(1); };
	int ClosedInV() { return(1); };

	void SetR(float R) {r=R; r2=sqr(r); };

	int Eval(const Point &parameter,Point &p) 
	{
	   float u = (parameter.x-0.0) * TWOPI;
	   float v = (parameter.y-0.0 ) * PI;
	   p.Set(r*cos(u)*sin(v),r*sin(u)*sin(v),r*cos(v));
	   return(1);
	};
	
	int EvalNormal(const Point &parameter,Point &n) {
	   return(Eval(parameter,n));
	};
	
	int Map(const Point &p,Point &parameter); 
	int Intersect(const Ray &ray,float &t); 


}; // Sphere Surface


// cylinder surface
class GCylinderSurfaceX : public GSurfaceX {
public:
	float r;
	float h;
	
	GCylinderSurfaceX(float R=1.0,float H=2.0) : r(R),h(H) {};

	int ClosedInU() { return(1); };

	int Eval(const Point &parameter,Point &p) 
	{
	   float u = (parameter.x-0.0) * TWOPI;
	   float v = (parameter.y-0.0) * h;
	   p.Set(r*cos(u),r*sin(u),v);
	   return(1);
	};
	
	int EvalNormal(const Point &parameter,Point &n) {
	   float u = (parameter.x-0.0) * TWOPI;
	   n.Set(cos(u),sin(u));
	   return(1);
	};

    int Map(const Point &p,Point &parameter);
	int Intersect(const Ray &ray,float &t);


};


// Simple xy-plane surface, 
class GXYPlaneSurfaceX : public GSurfaceX {
public:
	
	GXYPlaneSurfaceX() {};


	int Eval(const Point &parameter,Point &p) 
	{
	   p.Set(parameter.x,parameter.y);
	   return(1);
	};
	
	int EvalNormal(const Point &parameter,Point &n) {
	   n.Set(0.0,0.0,1.0);
	   return(1);
	};

    int Map(const Point &p,Point &parameter) { parameter.Set(p.x,p.y); return(1); }

	int Intersect(const Ray &ray,float &t);

};


class GTorusSurfaceX : public GSurfaceX {
public:
	float rMajor;
	float rMinor;
	
	GTorusSurfaceX(float RMajor=1.0,float RMinor=0.25) : rMajor(RMajor),rMinor(RMinor) {};
	int ClosedInU() { return(1); };
	int ClosedInV() { return(1); };

};

#endif




// GParameter
// compute a parametrization for texture mapping	
enum GParameterSource
	{
	GPNative,		// native parametrization eg. QuadMesh, cylinder ..
	GPObject,		// object coordintes
	GPWorld,		// Object world coordinates
	GPNormal,		// object vertex normals
	GPParameter,	// object vertex parameters
	GPColor,		// object vertex color
	};

enum GParameterFunction {
	GPFxyz,
	GPFxzy,
	GPFyzx,
	GPFsurface,
	};

enum GMappingFucntion {
	GPMNone,
	GPMReflected, // Intersect Ray(op,reflected eyeray) surface
	GPMEye,			// Intersect Ray(op,eye)
	GPMONormal,	// Intersect Ray(op,on) with intermediate surface
	GPMOCenter,  // Intersect Ray(ocenter, op-ocenter with intermediate surface
	GPMINormal,	 // Intersect Ray(op,surfaceN, with intermediate surface ???
	};


// Forward
class GShell;


class GParameter {

public:
	int id;
   	
   	int enabled;
	int changed;

   	void SetId(int Id) { id = Id; }
	
	int source;
	int function;
	int mapping;

	
	int showSurface;		// show mapping surface
	int showMappedObject;	// show object with coordinates = parameters

	char Function[80];
	char Source[80];
	char Mapping[80];

	int SetSource(const char * f);
	int SetFunction(const char * f);
	int SetMapping(const char * f);
	
	int normalize;	// normalize to object extent
	int NeedsOCenter() { return(mapping == GPMOCenter); };
	int NeedsOBBox()   { return(normalize); };
	int NeedsViewpoint() { return(mapping == GPMReflected)||(mapping == GPMEye); };
	int NeedsWorldMatrix() { return(source == GPWorld); };


	GSurface *surface;		// the mapping surface

	Point center;
	Point viewpoint;
	Matrix object2world;	// for source == GPWorld

	GParameter(int Id =0) { 
			SetId(Id);
			enabled = 0; changed = 0;
			source = GPNative; function = GPFxyz;
			mapping = GPMNone;
			showSurface = 0;
			showMappedObject = 0;

			strcpy(Source,"native");
			strcpy(Function,"xyz");
			strcpy(Mapping,"none");

			hasbox=0; hasm=0;normalize=0; SetId(id); 
			surface = 0;
			object2world.Identity();
	};
	
	~GParameter();


	// map point p to parameter value
	int EvalType(const Point &p,Point  &parameter); 

	// evaluate Paramter for Point p
	// Result 1, if parameter was computed
	virtual int Eval(const Point &p,Point  &parameter);

	// evaluate Parameter of a set of Points, newp will be initialized
	virtual int Eval(int np, const Point *p,Point *newp);

	int ApplyShell(GShell &shell);

	  
	int hasbox;
	BBox box;

	int hasm;
	MatrixAndInverse m;  //opt invtrans pts to deformer space and transform deform pts back

	void SetMatrix(const Matrix &M) { m.Set(M); hasm=1; }
	void UnSetMatrix() { m.SetIdentity(); hasm=0; }
	void AppendMatrix(const Matrix &M) { m.Append(M); hasm=1; }

	virtual void Translate(const Point &p) { AppendMatrix(TranslationMatrix(p)); }

	void SetBBox(BBox Box) { box = Box; hasbox = !box.IsEmpty(); }

	// Check if Points within test box are potentially affected

//	virtual int CheckBBox(const BBox &testBox) {
//		if (hasbox) return(!box.Outside(testBox));
//		return(1); }

	// Map Points from objectspace to boxspace [0..1,0..1,0..1]
   // void Map2Box(int np,const Point *p,Point *op);
};



#endif
