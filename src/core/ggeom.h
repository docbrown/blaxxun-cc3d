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
#ifndef _GGEOM_H
#define _GGEOM_H
/******************************************************************************
@doc

@module GGeom.cpp - GLView geometry creation routines | 

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:
Different routines for Extruding, Sweeping contours,
Circle Routine ...

Classes:

Notes:

Changes:

Todo :

******************************************************************************/


//@class Sphere drawing by directly using a recursive algorithm
class GSphereSubdiv: public GShell {
	
	RT(GShell);

	// center point
	Point center;
	// radius
	float r;
	
	int computed;

	// recursion level
	int level;
	int viewDependent;

	GSphereSubdiv(const Point &Center=Point(0,0,0), float R=1.0,int Level=1,int compute=0);
    
    int ComputeSphere(const Point &center, float r,int level, int VP);
	
	int Compute() { return(ComputeSphere(center,r,level,1)); }
	
    void TriangleSub(int level,int p1,int p2,int p3);



    //@cmember  Compute bounding box 
	virtual void ComputeBBox(BBox &box);

	
	/*@cmember  Compute the center  */
	virtual void ComputeCenter(Point &center);
	
	//@cmember  Compute a facelist containing triangles only in tri
	// tri2face maps triangle # i to shells face number #fi
	// Return: number of triangles

	virtual int ComputeTriangles(GFaceList &tri,IntArray &tri2face,int useTriangulator=0);

#ifdef _OGL
	// OPENGL Rendering
	//@cmember rendering faces of shell directly to OpenGL, either parse f list or prims
	virtual void RenderGlFaces(RenderState &state,int FN=1, int VN=0, int VC=0, int VP=0);
	

	//@cmember rendering edges / edgetable of shell directly to OpenGL
	virtual void RenderGlEdges(RenderState &state,int VN=0,int VC=0,int VP=0);
	
	//@cmember rendering vertices of shell directly to OpenGL
	virtual void RenderGlVertices(RenderState &state,int VN=0,int		VC=0);

	//@cmember rendering normals of shell directly to OpenGL
	virtual void RenderGlNormals(RenderState &state,float scale=0.2,int VN=1,int FN=0);
#endif


	// compute native vertex parameters for object
	virtual void GetVertexParametersNative(int n,Point *vp);

	/* @cmember Evaluate object as object of type "type"
	  if return = this, don´t destroy result, otherwise destroy */
    GEvalAs_TYPE*  EvalAs(TypeId type,int copyAttributes=1);


	//@cmember GClass do ray selection 
	int Do(GRaySelectTraversal &state); // declared in GGeom.h implmented in GRaySelect


	// many other functions missing !!!!
};




//@class computes a translational sweep / extrusion
class GSweep
{
public:
   int vsteps;	// How many steps for Trans / Rot Sweep
   int closeTop;   // close top of sweep object
   int hardTop;	   // separate vertices for top face to have flat shading
   int closeBottom;   // close bottom of sweep object
   int hardBottom;    // separate vertices for bottom face to have flat shading

   int computeParams; // compute vertex parameters
   float paramVScale; // Extra parameter scale factor
   float paramVOffset; // Extra parameter offset factor

   GSweep() {	vsteps=16;
   		closeTop=closeBottom=0; hardTop=hardBottom=1;
		computeParams=0;
		paramVScale = 1.0; paramVOffset = 0.0;
	    }
} ;

//@class computes a rotational sweep / object of revolution
class GRotSweep : public GSweep
{
public:
   	float startAngle,endAngle;
   	int rotateFull;	// full 360 deg rotation
   	int apexTop; 	// there is one single point at top
   	int apexBottom;	// there is one single point at top
   	Vector axis; 	// rotation axis,

   	GShell *profile;	// profile to be sweeped
   	GShell * result;	// current result

   	
   	// constructor
	GRotSweep(GShell *Profile = 0,Vector Axis = Vector(0,0,1)) :
			GSweep()
		{
		 profile = Profile;
		 axis = Axis;
		 closeTop=closeBottom= 1;
		 hardTop=hardBottom=1;
		 apexTop=apexBottom=0;
		 rotateFull = 1;
		 startAngle = 0; endAngle = TWOPI;

		};

	~GRotSweep() { delete profile; delete result; }


   	virtual int Compute();
   	virtual GShell * Eval()	{
		if (!result) Compute();
		return result;
	}

};



class GTransSweep : public GSweep
{
public:
	int sweepNormal;	// Sweep in direction of vertex normals
	float sweepOffset;	// the sweep distance
	Vector axis;

	// constructor
	GTransSweep(GShell *Profile = 0,const Vector Axis = Vector(0,0,1),float SweepOffset=1.0) :
			GSweep()
		{
		 profile = Profile;
		 axis = Axis;
		 sweepOffset = SweepOffset;
		 vsteps = 1;
		 closeTop=closeBottom= 1;
		 hardTop=hardBottom=1;
		};

	~GTransSweep() { delete profile; delete result; }

	GShell * profile;	// profile to be sweeped
	GShell * result;	// current result

	void FlushResult() { if (result) { delete result; result = 0;} }

   	virtual int Compute();
   	
   	virtual GShell * Eval()
	{
		if (!result) Compute();
		return result;
	}


};

// GTransTangSweep
// GSkin

//@func Create a GPolygon primitive with circle data

GPolygon *NewCircle(float r=1.0,float cx=0.0,float cy=0.0,int n=12);

//@func create a shell with sphere data
GShell * NewSphere(float r=1.0,int usteps=16,int vsteps=8);


/* for sweep solids */
#define BIT(v, mask)	(((v) & (mask)) != 0)

#define ITOP  256
#define IBOTTOM  512
#define ILEFT  1024
#define IRIGHT	2048
#define ICLOSE	(2048*2)
#define ICLOSE_COLS (2048*2)	/* close colums of a mesh */
#define ICLOSE_ROWS (2048*16)	/* close rows of a mesh */
#define IHARDTOPBOTTOM	(2048*4) /* make hard edge */

#define IQUAD	(2048*8)	/* use quadrilaterals (instead of triangles) */
#define IPARAMETERS (2048*16)	/* set default vertex parameters */
#define ILINES (2048*32)	/* use POLYLINE  */

typedef int istyle;

// Invert facelist by adding faces as holes to fin, and than append holes in fin
// as normal faces to fout
// fout should contain the one master face at input
int ComputeInvertedFaceList(const GFaceList &fin,GFaceList &fout);

//@func compute extruded version of shell
GShell * ComputeExtrudedShell(
				istyle style,
				GShell &s,
				PointArray &offsets);


#endif
