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
#ifndef _gvisibility_h
#define _gvisibility_h

/******************************************************************************

gvisibility.h - tools for visibilty computation

Copyright (c) 1997 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

$Revision: 1.14 $
$Log: gvisibility.h,v $
Revision 1.14  1999/07/06 16:55:15  tom
*** empty log message ***

Revision 1.13  1999/03/10 10:22:19  tom
*** empty log message ***



Todo :

******************************************************************************/

#include <float.h>

#define BEPS 0.00001


enum GVisibility {
	INSIDE=1,		// object is inside
	ON=0,			// exactly on  plane
	OUTSIDE=2,		// outside
	OVERLAP=3		// don"t know or on both sides
};



#define TRACE_BBOX(b) TRACE("BBox (%g %g) (%g %g) (%g %g) \n",b.mi.x,b.ma.x, b.mi.y,b.ma.y, b.mi.z,b.ma.z)
#define TRACE_PLANE(b) TRACE("PLANE (%g %g %g) %g) \n",b.n.x,b.n.y,b.n.z,b.d)

// is bbox b transformed by m outside a ?
gbool BoxBoxOutside(const BBox &a, const Matrix &m,const BBox &b);

// INSIDE, OUTSIDE ON
int BoxPlaneStatus(const BBox &box, const Plane &plane); 

// return TRUE if box fully outside of box 
gbool BoxPlaneOutside(const BBox &box, const Plane &plane); 

// test a box against a set of plane 
// returns only OUTSIDE & OVERLAP 
int BoxPlaneStatus(const BBox &box,const Array<Plane> &planes);


// test a box against 5 planes of view frustrum 
// returns true if box outside off all planes 
// planes are defined by the view point position and 4 points on far plane
gbool BoxViewVolumeOutside(const BBox &box,const Point &viewpoint,PointArray &viewVolume);

// test a box against 5 planes of view frustrum 
// returns true if box outside off all planes 
// planes are defined by the view point position and 4 points on far plane
// return also if resulting object need clipping to view frustrum
gbool BoxViewVolumeOutside(const BBox &box,const Point &viewpoint,PointArray &viewVolume,BOOL &needClip);



// test a list of points against plane 
// can return all 4 cases 
int PlaneStatus(const PointArray &v, const Plane &plane);

int PlaneStatus(int np, const Point *p, const Plane &plane);

// Compute a separation Planes 
gbool ComputeSeparationPlane(const BBox &a,const BBox &b, Plane &plane, int &frontIndex,int &backIndex);
/*

  compute a separation plane splitting a box
  along the dominant axis

*/

gbool ComputeSeparationPlane(const BBox &a, Plane &plane);

// each bit(i) says outside of plane (i)
typedef unsigned long GPlaneMask;
#define GMAX_PLANES (sizeof(GPlaneMask)*8)

class GCollisionResponse 
{
public :
	GCollisionResponse() : 
	    currentMinDistance(FLT_MAX), 
		currentMinPlaneIndex(-1),
		currentMinVertexIndex(-1),
		collision(gfalse)
	{}

	float currentMinDistance;			// maximum penetrations distance (negative)
	int currentMinPlaneIndex;	// index of collission plane
	int currentMinVertexIndex;	// vertex index which is most inside

	int faceIndex;				// index of face, penetrating
	Plane currentMinPlane ;
	Point currentMinVertex;
	gbool collision;

};

//
// GConvexVolume
// 
/*
 Convex volume is a convex polyhedra
 defined by a set of points
 and a set of planes definining the volume

*/

class GConvexVolume {
public :
		
		BBox bbox;		// bounding box over vertices	
		PointArray v;	// vertices (convex hull)

		Array<Plane> planes; // the planes of the hull
		
		GConvexVolume() {}

		GConvexVolume(const PointArray &v_, const Array<Plane> &planes_) :
				v(v_), planes(planes_)
			{
				UpdateBBox();
			}
		~GConvexVolume() { }

		GConvexVolume(const Matrix &m,const Matrix &inverse,const PointArray &v_, const Array<Plane> &planes_);

		// set to new 
		void Set(const Matrix &m,const Matrix &inverse,const PointArray &v_, const Array<Plane> &planes_);

		// set to new 
		void Set(const BBox &box) { bbox = box; }

		// set to new 
		void Set(const BBox &box, const Matrix &m) { bbox = box;  bbox *= m; }

		// recompute bounding box from vertices
		void UpdateBBox()
		{
			v.ComputeBBox(bbox);
		}


		// outside bounding box test
		inline gbool Outside(const BBox &object) {
			// outside if outside of my bounding box
			if (bbox.Outside(object)) return gtrue;
			// outside if object bbox outtside of all planes
			return (BoxPlaneStatus(object,planes) == OUTSIDE);
		}

		// test if bbox of object transformed by m is outside
		gbool Outside(const Matrix &m,const BBox &object);


		// compute the status of all vertices with respect to planes
		// and update masks

		// if the ormask = 0, all points are inside
		// if the andmask !=0  all points are outside  

		int ComputePlaneMasks(const PointArray &v,Array<GPlaneMask> &masks); 
		
		// a triangle is outside if the corresponding vertices
		// are outside of a common plane 

		inline gbool Outside(int v1,int v2, int v3,const Array<GPlaneMask> &masks) const {
			GPlaneMask m =masks[v1] & masks[v2] & masks[v3];
			return m !=0;
		}

		// same but for convex polygon 
		inline gbool Outside(int n, const int *fp, const Array<GPlaneMask> &masks) const {
			ASSERT(n>0);

			GPlaneMask m = masks[*fp++];
			n--;
			while (n>0) {
				m &= masks[*fp++];
				n--;
			}
			return m !=0;
		}

		// a triangle is inside if the corresponding vertices
		// are not outside of any plane 

		inline gbool Inside(int v1, int v2, int v3,const Array<GPlaneMask> &masks) const {
			return masks[v1] == 0 && masks[v2] ==0 && masks[v3]==0;
		}

		// a point is inside if the corresponding vertices
		// are not outside of any plane 

		inline gbool Inside(int v1,const Array<GPlaneMask> &masks) const 
		{
			return masks[v1] == 0;
		}

		
		// once a polygon is intersecting
		// compute collision respone & update response info

		gbool ComputePenetration(
			int n, const int *fp,	// the polygon vertex indices
			const PointArray &v,	// the vertices
			const Array<GPlaneMask> &masks,	// precomputed masks
			GCollisionResponse &response	// the response info
			) const;


		// for Bsp Tree, test transformed plane against volume
		int PlaneStatus(const Matrix &m,const Matrix &minverse, const Plane &plane);

};

#endif







