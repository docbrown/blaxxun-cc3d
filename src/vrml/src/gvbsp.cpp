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

gvbsp.cpp - VRML 2 Bsp Tree extension nodes

Copyright (c) 1997-1998	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

$Revision: 1.31 $
$Log: gvbsp.cpp,v $
Revision 1.31  1999/07/06 16:45:33  tom
*** empty log message ***

Revision 1.30  1999/05/10 13:54:45  tom
*** empty log message ***



Todo :

******************************************************************************/

#include "stdafx.h"

#include "gvbsp.h"
#include "gv2geometry.h"

#include "gvwwwanchor.h"

#include "gvtraverse.h"

#include "grayselect.h"

#include "gcamera.h"
#include "gshell.h"

#include "gvsensors.h"
#include "gvwwwinline.h"

#include "gvisibility.h"
#include "ghull.h"

#include "gversion.h"

#include "greporter.h"

#include "gmodel.h"

	
BSP_HEURISTIC GvBspTree::bspHeuristic=COMPUTE_SEPARATION_PLANE; // USE_FIRST_PLANAR_OBJECT;

BOOL	GvBspTree::bspSplit = FALSE;
int		GvBspTree::maxBspSplit = 33;
int		GvBspTree::bspSearchLimit = 66;



// statistic
int GvBspTree::bspLevel;
int GvBspTree::bspMaxLevel;
int GvBspTree::bspOverlapMax;
int GvBspTree::bspOverlapMean;
int GvBspTree::bspOverlapMeanCnt;
int GvBspTree::bspNumNodes;
int GvBspTree::bspNumGeomNodes;

void GvBspTree::ClearStats()
{

	bspLevel = 0;
	bspMaxLevel=0;
	bspOverlapMax = 0;
	bspOverlapMean = 0;
	bspOverlapMeanCnt = 0;
	bspNumNodes=0;
	bspNumGeomNodes=0;
}


int ShapePlaneStatus(GvNode *s,const Plane &plane); 


#if 1

//#ifndef _GV_MINIMAL




// try to get a plane for node
// normally only possible for IFS geoemtry with one face

GvBool GetPlane(GvNode *a, Plane &plane)
{
Redo:
	if (a == NULL) return FALSE;

	if (RTISA(a,GvShape)) {
		a = ((GvShape *) a)->getGeometry();
		goto Redo; //return GetPlane(a,plane);
	}

	if (RTISA(a,GvPolygon)) {

		GvPolygon *n = (GvPolygon *) a;
		if (!n->planeOk)
			n->Update(1,1);
		plane = n->plane;
		return 1;

	}


	if (RTISA(a,GvNodeShell)) {
		GShell *theShell = ((GvNodeShell *) a)->theShell;
		if (theShell) return (theShell->isPlanar(plane) == 1);
	}

	if (RTISA(a,Gv2Anchor)) {
		Gv2Anchor *g = (Gv2Anchor *) a;
		if (g->children.getNum() == 1) {
			a = g->children[0];
			goto Redo; //return GetPlane(a,plane);
		}
	}
	

	return FALSE;
}

GvBool ComputeSeparationPlane(GvNode *a,GvNode *b, Plane &plane, int &frontIndex,int &backIndex)
{
	BBox bbox1;
	BBox bbox2;
	a->getBBox(bbox1);
	b->getBBox(bbox2);

	return ComputeSeparationPlane(bbox1,bbox2,plane,frontIndex,backIndex);
}




// helper class 
// stores info for BSP Tree Processing 

class GvBspObject {

public :
	float sortKey;

	BBox bbox; // bbox of node
	
	BOOL isPlanar;	// simple planar polygon ?
	Plane plane;	// the plane in this case	
	BOOL hasHull;	// do we have convex hull
	BOOL hullComputed;
	
	PointArray pts;	// the pts for computing hull

	int nTris;
	Triangle* tris;

	Array<Plane> triPlanes;
	
	GvNodeHandle node;

	// setup bsp object 
	void set(GvNode *shape) {
		node = shape;
		isPlanar=GetPlane(shape, plane);
		if (shape) { 
			if (!shape->getBBoxEx(bbox)) {

			}		
		}
	}
	
	GvBspObject (GvNode *shape) : tris(NULL) { hullComputed = FALSE; hasHull = FALSE; set(shape); };
	
	~GvBspObject () { if (tris) delete []tris; };
	
	GvBool ComputeConvexHull(const PointArray &pts);
	
	GvBool ComputeHull(GvNode *node);

	// compute status of object with respect to plane 
	int PlaneStatus(const Plane &thePlane) 
	{
		int status;
		status = (BoxPlaneStatus(bbox, thePlane)); // quick bbox check first 
		if (status != OVERLAP) return status;
		if (pts.Length() >0) {
			return ::PlaneStatus(pts,thePlane);
		}
		return  ShapePlaneStatus(node,thePlane); 

	}

};

GvBool ComputeSeparationPlane(GvBspObject &a,GvBspObject &b, Plane &plane, int &frontIndex,int &backIndex)
{
	if (ComputeSeparationPlane(a.bbox,b.bbox,plane,frontIndex,backIndex))
		return TRUE;

	if (a.isPlanar) {
		plane = a.plane;
		frontIndex = -1;
		backIndex = -1;
		return TRUE;
	}
	else if (b.isPlanar) {
		plane = b.plane;
		frontIndex = -1;
		backIndex = -1;
		return TRUE;
	}
	return FALSE;
}


GvBool ComputeSeparationPlaneHull(GvBspObject &a,GvBspObject &b, Plane &plane, int &frontIndex,int &backIndex)
{
	frontIndex = -1;
	backIndex = -1;

	if (!a.hullComputed) {
		a.ComputeHull(a.node);
		a.hullComputed = TRUE;
	}

	if (!b.hullComputed) {
		b.ComputeHull(b.node);
		b.hullComputed = TRUE;
	}

	if (a.hasHull && b.hasHull && SeparatePointSets3D(a.pts.Length(),a.pts.Data(),a.nTris,a.tris,
		b.pts.Length(),b.pts.Data(),b.nTris,b.tris,plane)) 
	{
		// found separation plane 
		return TRUE;
	}
	else return ComputeSeparationPlane(a,b,plane,frontIndex,backIndex);

}

float GvBspQuality(GvNode *a);

float GvBspQuality(GvMFNode &g)
{   float sum=0;
	for (int i=0; i< g.getNum();i++)
			sum+=GvBspQuality(g[i]);
	return sum;
}


float GvBspQuality(GvNode *a)
{
Redo:	
	if (a == NULL) return 0;

	if (RTISA(a,GShell)) {
		 return 1;
		 // or number of faces, or volume 
	}

	if (RTISA(a,GvShape)) {
		a = ((GvShape *) a)->getGeometry();
		goto Redo; 
	}

	if (RTISA(a,GvNodeShell)) {
		a  = ((GvNodeShell *) a)->theShell;
		goto Redo; 
	}

	if (RTISA(a,Gv2Anchor)) {
		Gv2Anchor *g = (Gv2Anchor *) a;
		return GvBspQuality(g->children);
	}

	if (RTISA(a,Gv2Group)) {
		Gv2Group *g = (Gv2Group *) a;
		return GvBspQuality(g->children);
	}
	if (RTISA(a,GvBspTree)) {
		GvBspTree *g = (GvBspTree *) a;
		float l = GvBspQuality(g->front);
		float r = GvBspQuality(g->back);
		float c = GvBspQuality(g->overlap);

		return (fabs(l-r) + 1.5 *c);
	}
	return 0;
}



GvBool GvBspObject::ComputeHull(GvNode *a)
{
Redo:	
	if (a == NULL) return FALSE;

	if (RTISA(a,GShell)) {
		return ComputeConvexHull(((GShell *) a)->v);
	}

	if (RTISA(a,GvShape)) {
		a = ((GvShape *) a)->getGeometry();
		goto Redo; 
	}

	if (RTISA(a,GvNodeShell)) {
		GShell *theShell = ((GvNodeShell *) a)->theShell;
		return ComputeHull(theShell);
	}

	if (RTISA(a,Gv2Anchor)) {
		Gv2Anchor *g = (Gv2Anchor *) a;
		if (g->children.getNum() == 1) {
			a = g->children[0];
			goto Redo; 
		}
	}
	return FALSE;
}


GvBool GvBspObject::ComputeConvexHull(const PointArray &pts) 
{
	    
    this->pts = pts;

	hullComputed = TRUE;
	if (ConvexHull3D (pts.Length(), pts.Data(), 
			nTris, tris) >0) {
			hasHull = TRUE;
			return TRUE;
	}
	return FALSE;
}



GvBool ComputeSeparationPlaneBox(GvBspObject &a,GvBspObject &b, Plane &plane, int &frontIndex,int &backIndex)
{
	return ComputeSeparationPlane(a.bbox,b.bbox,plane,frontIndex,backIndex);
}



/*
ComputeSeparationPlane(Plane a,Plane b)
if (parallel)
    mid plane,
compute intersection
ComputeSeparationPlane(Plane a,Poly ap,Plane b)
if !Intersection Segment crooses ap etc

*/




//BoxPlaneStatus()
//PlaneRayStatus(); ==> RaySelectionTraversal


// choose a good candidate plane, separating the set of shape nodes
//
GvBool ChooseSeparationPlane(GvMFNode &shapes,Plane &plane,GvNode* &separator) 
{
	// simple strategy : use largest dimension 

	BBox bbox;
	GvNode *candidate = NULL;

	float candidateVolume =	 -1.0;

	int limit = min(shapes.getNum(),GvBspTree::bspSearchLimit);

	if (GvBspTree::bspHeuristic <= USE_LARGEST_PLANAR_OBJECT) {
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
	// search a planar object 
	for (int i=0; i<limit; i++) {
		GvNode *s = shapes[i];
		if (GetPlane(s,plane)) {

			// use planar object with "largest volume" 
			BBox bbox;
			s->getBBox(bbox);
			Point size= bbox.Size();

			float v = size.x * size.y * size.z;

			if (v >candidateVolume) {
				candidate = s;
				candidateVolume = v;
				
				if (GvBspTree::bspHeuristic==USE_FIRST_PLANAR_OBJECT) break;

			}

			//separator = s;
			//return TRUE;
		}
	}

	if (candidate) {
		separator = candidate;
		return TRUE;
	}

	TRACE("No separation Plane !\n");
	
	}


	if (shapes.getNum() == 2) {
		int frontIndex,backIndex;

		if (ComputeSeparationPlane(shapes[0],shapes[1],plane,frontIndex,backIndex)) 
		{
			return TRUE;
		}

	}

	// default approach
	// use bounding box of whole group, and split it
	
	if (shapes.getBBox(bbox)) {
	  	//TRACE_BBOX(bbox);
		return ComputeSeparationPlane(bbox,plane);
	} else return FALSE;

}

//check out the quality if shapes are split by plane
// 0 good,  larger bad
float ExamineQuality(Array<GvBspObject*> &shapes,		 float currentMaxQuality, const Plane &plane) 
{
   int cnt[OVERLAP+1];
   cnt[INSIDE]=cnt[ON]=cnt[OVERLAP]=cnt[OUTSIDE]=0;

   int total = shapes.Length();


   for (int i=0; i<total; i++) {
			GvBspObject *a = shapes[i];
			cnt[a->PlaneStatus(plane)]++;
		    float q= cnt[OVERLAP]*1.5;
			if (q >= currentMaxQuality) break; // can't get better 

   }	
   if ( cnt[INSIDE] == total) return 1E30;
   if ( cnt[OUTSIDE] == total) return 1E30;
   if ( cnt[OVERLAP] == total) return 1E30;

   // pref equal split  minimize overlap
   return (fabs(cnt[INSIDE]-cnt[OUTSIDE])*1.0 + cnt[OVERLAP]*1.5);

}


gbool isSampePlane(const Plane &p1,
					const Plane &p2)
{

	if (p1.d == p2.d && p1.n == p2.n) {
		return 1;
	}
	return 0;
}

GvBool ChooseSeparationPlane(Array<GvBspObject *> &shapes,const Plane &parentPlane,
							 Plane &plane,GvBspObject * &separator) 
{

	BBox bbox;
	int candidate = -1;

	BOOL haveCandidate = FALSE;
	float candidateVolume =	 -1.0;
	Plane candidatePlane;
	float candidateQuality = 1E30;

	int frontIndex,backIndex;

	int limit = min(shapes.Length(),GvBspTree::bspSearchLimit);

	separator = NULL;

	if (GvBspTree::bspHeuristic == COMPUTE_SEPARATION_PLANE) {
		Plane newPlane;
		int limit=GvBspTree::bspSearchLimit;
		
		for (int i=0; i<shapes.Length(); i++) {
			
			GvBspObject *a = shapes[i];

			if (a->isPlanar == 1 ) { // its a planar object itselft use it as candiate
				newPlane = a->plane;
		
				if (isSampePlane(parentPlane,newPlane)) {
					TRACE("Same plane \n");
					TRACE_PLANE(newPlane);
				}


				float q=ExamineQuality(shapes,candidateQuality,newPlane);
				
				if (q < candidateQuality) {	// found a better one 
					TRACE("BspTree :: Improved Quality = %f \n",q);
					candidatePlane = newPlane;
					candidateQuality = q;
					haveCandidate = TRUE;
					candidate = i;
					continue ;;;
				}
				else { // do an edge test ????????????????
						// new fall through 
				}	

			}
			// else 
			for (int j=i+1; j<shapes.Length(); j++) {
				
				gbool hullTest = gtrue;
				float q;

				GvBspObject *b = shapes[j];

				if (b->isPlanar) {
					newPlane = b->plane;

					q=ExamineQuality(shapes,candidateQuality,newPlane);
				
					if (q < candidateQuality) {	// found a better one 
						hullTest = gfalse;
					} else 
					{
					}

				}

				if (hullTest) {
					if (ComputeSeparationPlaneHull(*a,*b,newPlane,frontIndex,backIndex)) {
						q=ExamineQuality(shapes,candidateQuality,newPlane);
					}
					else 
						continue; // hull test failed 
				}

				if (isSampePlane(parentPlane,newPlane)) {
					TRACE("Same plane \n");
					TRACE_PLANE(newPlane);
				}
				else 
				if (q < candidateQuality) {	// found a better one 
					TRACE("Improved Quality = %f \n",q);
					candidatePlane = newPlane;
					candidateQuality = q;
					haveCandidate = TRUE;
					candidate = -1;
					if (q <=0.5) break; // found a good one
				}

				if (candidateQuality <=0.5) 
					break; 
				limit --;

			} // for j

			// limit seach 
			if (candidateQuality <=0.5) break; 
			if (haveCandidate && limit < 0) break;
		}

		if (candidateQuality >= (shapes.Length() *0.4)) { // all planar ?
			BBox bbox;
			bbox.SetEmpty();

			for (int i=0; i<shapes.Length(); i++) {

				GvBspObject *s = shapes[i];
				bbox.Union(s->bbox);
			}

			ComputeSeparationPlane(bbox,newPlane);
			float q=ExamineQuality(shapes,candidateQuality,newPlane);
			if (isSampePlane(parentPlane,newPlane)) {
					TRACE("Same plane \n");
					TRACE_PLANE(newPlane);
					// tbd try other axis
			}
			else 	
			if (q < candidateQuality) {	// found a better one 
					TRACE("Improved Quality via bbox = %f \n",q);
					candidatePlane = newPlane;
					candidateQuality = q;
					haveCandidate = TRUE;
					candidate = -1;
			}

		}

		if (candidate>=0) { // the object on the plane 
			separator = shapes[candidate];
		}

		if (haveCandidate) {
			plane=candidatePlane;
			return TRUE;
		}
		TRACE("Hull separation failed \n");

	} 
	else  if (GvBspTree::bspHeuristic <= USE_LARGEST_PLANAR_OBJECT) {

		
	// search a planar object 
	for (int i=0; i<limit; i++) {
		GvBspObject *s = shapes[i];
		if (s->isPlanar) {

			// use planar object with "largest volume" 
			Point size= s->bbox.Size();

			float v = size.x * size.y * size.z;

			if (v >candidateVolume) {
				candidate = i;
				candidateVolume = v;
				
				if (GvBspTree::bspHeuristic==USE_FIRST_PLANAR_OBJECT) break;

			}

			//separator = s;
			//return TRUE;
		}
	}

	if (candidate>=0) {
		separator = shapes[candidate];
		return TRUE;
	}

	TRACE("No separation Plane !\n");
	
	}


	if (shapes.Length() == 2) {

		if (ComputeSeparationPlane(*shapes[0],*shapes[1],plane,frontIndex,backIndex)) 
		{
			return TRUE;
		}

	}


	// get bbox

	// sort elements along axis
	// use center element and use bbox 
	// or search non overlap bbox 

	// default approach
	// use bounding box of whole group, and split it

/*	
	if (shapes.getBBox(bbox)) {
	  	//TRACE_BBOX(bbox);
		return ComputeSeparationPlane(bbox,plane);
	} 
	else 
*/		
		return FALSE;

}


//
// check a list of points against a bbox
//

int
BoxStatusStatus(int np,const Point *p, const BBox &box)
{
   int x0out = 0, x1out = 0;
   int y0out = 0, y1out = 0;
   int z0out = 0, z1out = 0;
   int i;

   // count vertices "outside" with respect to each
   // of the six planes
   const Point *pp = p;

   for (i=np; i>0; i--, p++) {


			if (p->x < box.mi.x) x0out++;	// out on left
      else	if (p->x > box.ma.x) x1out++;	// out on right
			if (p->y < box.mi.y) y0out++;	// out on top
      else	if (p->y > box.ma.y) y1out++;	// out on bottom
			if (p->z < box.mi.z) z0out++;	// out on near
      else	if (p->z > box.ma.z) z1out++;	// out on far
      }

   // Check if all vertices inside
   if (x0out+x1out+y0out+y1out+z0out+z1out == 0)
      return INSIDE;

   // Check if all vertices are "outside" any of the six planes
   if (x0out== np || x1out==np || y0out==np ||
       y1out==np || z0out==np || z1out==np) {
      return OUTSIDE;
      }
   return(OVERLAP);
}


/*

  test shape against plane 
*/

int ShapePlaneStatus(GvNode *s,const Plane &plane) 
{
	if (s == NULL) return ON;	
	if (RTISA(s,GvShape)) {
		s = ((GvShape *) s)->getGeometry();
		return ShapePlaneStatus(s,plane);
	}

	if (RTISA(s,GvNodeShell)) {
		GShell *theShell = ((GvNodeShell *) s)->theShell;
		if (theShell) return (PlaneStatus(theShell->v,plane));
	}

	if (RTISA(s,Gv2Anchor)) {
		Gv2Anchor *g = (Gv2Anchor *) s;
		if (g->children.getNum() == 1) {
			s = g->children[0];
			return ShapePlaneStatus(s,plane);
		}
	}

	// default approach :
	// test the bbox against plane 
		
		BBox box;
		// on which side of plane is s 

		s->getBBox(box);
		if (box.IsEmpty()) {
			TRACE("Empty node %s \n",s->ClassName());
			return OVERLAP;
		}
		else 
		return (BoxPlaneStatus(box, plane));

}

/*

  separate nodes into disjoint lists 

*/



void SeparateShapes(GvMFNode &shapes,
					Plane &plane,  GvNode* separator,
					GvMFNode &front,GvMFNode &overlap,GvMFNode &back)
{

	for (int i=0; i<shapes.getNum(); i++) {
	
		GvNode *s = shapes[i];

		if (s == NULL) continue;

		if (s == separator) { // separator object is on Plane, add to overlap
			overlap.add(s); 
			continue;

		}

		// on which side of plane is s 
		switch (ShapePlaneStatus(s, plane)) {
			case INSIDE : front.add(s); break;
			case OUTSIDE : back.add(s); break;
			case OVERLAP : // break down ? 
			case ON :
			default : overlap.add(s); break;
		}

	}
	TRACE("Separated front %d overlap %d back %d \n",front.getNum(),overlap.getNum(),back.getNum());
}

void SeparateShapes(Array<GvBspObject *> &shapes,
					Plane &plane,  GvBspObject* separator,
					Array<GvBspObject *> &front,Array<GvBspObject *> &overlap,Array<GvBspObject *> &back)
{

	for (int i=0; i<shapes.Length(); i++) {
	
		GvBspObject *s = shapes[i];

		if (s == NULL) continue;

		if (s == separator) { // separator object is on Plane, add to overlap
			shapes[i] = NULL;
			overlap.Append(s); 
			continue;
		}

		// on which side of plane is s 
		switch (s->PlaneStatus(plane)) {
			case INSIDE : front.Append(s); break;
			case OUTSIDE : back.Append(s); break;
			case OVERLAP : // break down ? 
			case ON :
			default : overlap.Append(s); break;
		}
		shapes[i]=NULL;

	}
	TRACE("Separated front %d overlap %d back %d \n",front.Length(),overlap.Length(),back.Length());
}

/*
   find some plane
   and decompose shapes into the three sets 

*/
BOOL ChoosePlaneSeparateShapes(GvMFNode &shapes,Plane &plane, 
			   GvMFNode &front,GvMFNode &overlap,GvMFNode &back)
{

	int nShapes = shapes.getNum();
	GvNode * separator = NULL;


	if (ChooseSeparationPlane(shapes,plane,separator)) {
	
		SeparateShapes(shapes,plane,separator, front,overlap,back);
		if (front.getNum() == nShapes || back.getNum() == nShapes || overlap.getNum() == nShapes) {
			// bad plane , continue 
		}
		else return TRUE;
	}

	front.deleteAll();
	back.deleteAll();
	overlap.deleteAll();

	BBox bbox1;
	BBox bbox2;

	for (int i= 0; i<(nShapes-1); i++ ) {
		shapes[i]->getBBox(bbox1);

		for (int j=nShapes-1; j>i; j--) {
			shapes[j]->getBBox(bbox2);
			int i1,i2;
			if (ComputeSeparationPlane(bbox1,bbox2,plane, i1,i2)) {

				SeparateShapes(shapes,plane,NULL,front,overlap,back);
				if (front.getNum() == nShapes || back.getNum() == nShapes || overlap.getNum() == nShapes) {
					// bad plane , continue 
					front.deleteAll();
					back.deleteAll();
					overlap.deleteAll();
				}
				else return TRUE;

			}
		}
	}
	return FALSE;

}

BOOL ChoosePlaneSeparateShapes(Array<GvBspObject *> &shapes,const Plane &parentPlane,
							   Plane &plane, 
			   Array<GvBspObject *> &front,Array<GvBspObject *> &overlap,Array<GvBspObject *> &back)
{

	int nShapes = shapes.Length();
	GvBspObject * separator = NULL;

	if (ChooseSeparationPlane(shapes,parentPlane,plane,separator)) {
		SeparateShapes(shapes,plane,separator, front,overlap,back);
		return TRUE;
	}

#if 0
	front.deleteAll();
	back.deleteAll();
	overlap.deleteAll();

	BBox bbox1;
	BBox bbox2;

	for (int i= 0; i<(nShapes-1); i++ ) {
		shapes[i]->getBBox(bbox1);

		for (int j=nShapes-1; j>i; j--) {
			shapes[j]->getBBox(bbox2);
			int i1,i2;
			if (ComputeSeparationPlane(bbox1,bbox2,plane, i1,i2)) {

				SeparateShapes(shapes,plane,NULL,front,overlap,back);
				if (front.getNum() == nShapes || back.getNum() == nShapes || overlap.getNum() == nShapes) {
					// bad plane , continue 
					front.deleteAll();
					back.deleteAll();
					overlap.deleteAll();
				}
				else return TRUE;

			}
		}
	}
#endif
	return FALSE;

}



/* 
   Build a BSP Tree 
   out of a set of nodes

*/


GvNode *BuildBspTreeOld(GvMFNode &shapes) 
{
	Plane aPlane;
	GvBspTree *tree;

//	int frontIndex,backIndex;

	int nShapes = shapes.getNum();

	// TRACE("(BSP-Tree Nodes = %d \n",nShapes);


	if (nShapes == 0) return NULL;
	if (nShapes == 1) return shapes[0];
/*
	if (nShapes == 2) {
		if (ComputeSeparationPlane(shapes[0],shapes[1],aPlane,frontIndex,backIndex)) 
		{
			tree = new GvBspTree();
			tree->front.set(shapes[frontIndex]);
			tree->back.set(shapes[backIndex]);
			tree->setPlane(aPlane);
			return tree;
		}
		else {
			Gv2Group *newGroup = new Gv2Group(); 
			newGroup->children.set(shapes);
			return newGroup;
		}
			
	}
*/


	GvMFNode front(0);
	GvMFNode back(0);
	GvMFNode overlap(0);


	if (GvBspTree::bspLevel<GvBspTree::maxBspSplit 
		&& ChoosePlaneSeparateShapes(shapes,aPlane,front,overlap,back))
	{
		GvBspTree::bspLevel++;

		if (GvBspTree::bspLevel > GvBspTree::bspMaxLevel) GvBspTree::bspMaxLevel=GvBspTree::bspLevel;

		tree = new GvBspTree();
		GvBspTree::bspNumNodes++;

		tree->setPlane(aPlane);

#ifdef _DEBUG
		shapes.getBBox(tree->bbox);
#endif
		if (overlap.getNum() >1) {
			TRACE("BSP-TREE Big Overlap %d from %d \n",overlap.getNum(),nShapes);
		}

		GvBspTree::bspOverlapMean+=	overlap.getNum();
		GvBspTree::bspOverlapMeanCnt++;


		tree->overlap.set(BuildBspTree(overlap));

		

		//tree->overlap.set(overlap);
/*
		if (overlap.getNum() <=2) {
			tree->overlap.set(overlap);
		} else 
			tree->overlap.set(BuildBspTree(overlap));
*/

		// could check overlap for # of elements 
		tree->front.set(BuildBspTree(front));
		//front.flush();
		tree->back.set(BuildBspTree(back));
		
		GvBspTree::bspLevel--;

		return tree;

	} else {
		Gv2Group *newGroup = new Gv2Group(); 
		newGroup->children.set(shapes);
		return newGroup;

	}

#if 0
	if (!ChooseSeparationPlane(shapes,aPlane)) {
		Gv2Group *newGroup = new Gv2Group(); 
		newGroup->children.set(shapes);
		return newGroup;
	}

	tree = new GvBspTree();
	GvBspTree::bspNumNodes++;

	tree->setPlane(aPlane);

	GvMFNode front(0);
	GvMFNode back(0);

	SeparateShapes(shapes,aPlane,front,tree->overlap,back);

	if (front.getNum() == nShapes || back.getNum() == nShapes) {
		TRACE("Degenerated Tree ");
		Gv2Group *newGroup =NULL;
		if (front.getNum()> 0) {
			newGroup= new Gv2Group(); 
			newGroup->children.set(front);
			tree->front.set(newGroup);
		} else {
			newGroup = new Gv2Group(); 
			newGroup->children.set(back);
			tree->back.set(newGroup);
		}
		return(tree);

	} else {
		// could check overlap for # of elements 
		tree->front.set(BuildBspTree(front));
		//front.flush();
		tree->back.set(BuildBspTree(back));
	}
	TRACE(")\n");

	return(tree);
#endif 

}

static int stepsDone;
static int stepsToDo;

/* 
   Build a BSP Tree 
   out of a set of nodes

*/
GvNode *BuildBspTree(Array<GvBspObject *> shapes, const Plane &parentPlane)
 
{
	Plane aPlane;
	GvBspTree *tree; // constructed Tree Node 

	int nShapes = shapes.Length();



	if (nShapes == 0) return NULL;
	if (nShapes == 1) { 
		GvNode *node = shapes[0]->node;
		delete shapes[0]; shapes[0] = NULL;
		return node;
	}

#ifndef _GV_MINIMAL

	if (nShapes >3)
		Reporter.Status("Building Bsp Tree %f ...",100.0f*stepsDone/ (float)stepsToDo);
#endif

	Array<GvBspObject *> front(0);
	Array<GvBspObject *> back(0);
	Array<GvBspObject *> overlap(0);


	if (GvBspTree::bspLevel<GvBspTree::maxBspSplit 
		&& ChoosePlaneSeparateShapes(shapes,parentPlane,aPlane,front,overlap,back))
	{
		GvBspTree::bspLevel++; // the tree level
		stepsToDo+=nShapes;

		if (GvBspTree::bspLevel > GvBspTree::bspMaxLevel)  // store maxlevels
			GvBspTree::bspMaxLevel=GvBspTree::bspLevel;

		tree = new GvBspTree();
		GvBspTree::bspNumNodes++;

		tree->setPlane(aPlane);

#ifdef _DEBUG
	//	shapes.getBBox(tree->bbox);
#endif
		if (overlap.Length() >1) {
			TRACE("BSP-TREE Big Overlap %d from %d \n",overlap.Length(),nShapes);
		}

		GvBspTree::bspOverlapMean+=	overlap.Length();
		GvBspTree::bspOverlapMeanCnt++;

		tree->overlap.set(BuildBspTree(overlap,aPlane));
		stepsDone+=overlap.Length();


		// could check overlap for # of elements 
		tree->front.set(BuildBspTree(front,aPlane));
		//front.flush();
		stepsDone+=front.Length();

		tree->back.set(BuildBspTree(back,aPlane));

		// set default bits 
		if (tree->front.get() == (GvNode *) NULL) tree->front.setDefault(TRUE);
		if (tree->overlap.get() == (GvNode *) NULL) tree->overlap.setDefault(TRUE);
		if (tree->back.get() == (GvNode *) NULL) tree->back.setDefault(TRUE);

		stepsDone+=back.Length();;
		
		GvBspTree::bspLevel--;

		return tree;

	} else {
		// simply build a group node 
		Gv2Group *newGroup = new Gv2Group(); 
		for (int i=0; i<shapes.Length(); i++) {
			newGroup->children.append(shapes[i]->node);
			delete shapes[i];
			shapes[i] = NULL;
		}
		return newGroup;
	}
}



GvNode *BuildBspTree(GvMFNode &shapes) 
{

	GvMFNode invalidShapes(0);
	stepsDone=0;
	stepsToDo=1;

	int nShapes = shapes.getNum();

	TRACE(" Building BSP-Tree Nodes = %d \n",nShapes);

	if (nShapes == 0) return NULL;
	if (nShapes == 1) return shapes[0];

	
	Array<GvBspObject *> bspShapes;

	for (int i=0; i<nShapes; i++) {

		GvNode *n= shapes[i];

		GvBspObject *o=new GvBspObject(n);

		if (o->bbox.IsEmpty()) {
			invalidShapes.append(n);
			delete o;
		}
		else bspShapes.Append(o);
	}

	GvBspTree::bspNumGeomNodes += bspShapes.Length();

	Plane dummyPlane(0,0,0,0);

	GvNode * tree = BuildBspTree(bspShapes,dummyPlane);


	if (invalidShapes.getNum()>0) { // need a group
		Gv2Group *newGroup = new Gv2Group(); 
		newGroup->children.set(invalidShapes);
		if (tree) newGroup->children.append(tree);
		return newGroup;
	}
	else return tree;
}


#if 0
//
// GvBspShape
//


GV_NODE_SOURCE_IMP(GvBspShape,GvNode);

GvBspShape::GvBspShape()
{
    GV_NODE_CONSTRUCTOR(GvBspShape);
    isBuiltIn = FALSE;
    GV_NODE_ADD_FIELD(transform);
    GV_NODE_ADD_FIELD(appearance);
    GV_NODE_ADD_FIELD(geometry);

}

GvBspShape::~GvBspShape()
{
}

// apply transform to state 
int GvBspShape::setMatrix(GTraversal &state)
{
	GMatrixStack *current,*prev;
	prev = (GMatrixStack*) state.Get(GMatrixStack::attrId);

	Matrix m;
	get(m);

	if (prev->level == state.level) { // already pushed
       prev->AppendLeft(m);
	   prev->Set(state);
	} else { 																	
		current = (GMatrixStack*) prev->NewInstance();
		current->m = m * prev->m;
		state.Push(current);
	}

	return(1);

}


int GvBspShape:: Do(GTraversal &state)
{	int ret = 0;


	state.Push();
	setMatrix(state);

	ret = appearance.Traverse(state);
   	ret = geometry.Traverse(state);

	state.Pop();
	return(ret);
}


int GvBspShape:: Do(GBBoxTraversal &state)
{	int ret = 0; 
	state.Push();
	setMatrix(state);
	ret = geometry.Traverse(state);
	state.Pop();
	return(ret);
}


int GvBspShape::Do(GglRenderTraversal &state)
{  	int ret = 0;

	state.Push();
	setMatrix(state);

	if (appearance.value == (GvNode*) NULL) { // use default 
		ret = state.defaultAppearance->Do(state);
	}
	else {
		ret = appearance.Traverse(state);
	}
    ret = geometry.Traverse(state);

	state.Pop();
	return(ret);

}

#endif

#endif



//
// GvBspTree
//


GV_NODE_SOURCE_IMP(GvBspTree,GvNode);

GvBspTree::GvBspTree()
{
    GV_NODE_CONSTRUCTOR(GvBspTree);
    isBuiltIn = FALSE;
    GV_NODE_ADD_FIELD(plane);
    GV_NODE_ADD_FIELD(front);
    GV_NODE_ADD_FIELD(overlap);
    GV_NODE_ADD_FIELD(back);
#ifdef _DEBUG
	bboxOk = FALSE;
#endif

}

GvBspTree::~GvBspTree()
{
}


int GvBspTree::Do(GTraversal &state)
{	int ret = 0;

	// simply traverse all
	ret = front.Traverse(state);
	ret = overlap.Traverse(state);
   	ret = back.Traverse(state);

	return(ret);
}

int GvBspTree::Do(GPathTraversal &state)
{	int ret = 0;

	GPUSH_PATH(state,this);

	// simply traverse all

	ret = front.Traverse(state);
	ret = overlap.Traverse(state);
   	ret = back.Traverse(state);

	GPOP_PATH(state);

	return(ret);
}

int GvBspTree::Do(GRaySelectTraversal &state)
{	int ret = 0;

	Plane plane;
	 
	getPlane(plane); 

	if (state.mode == GRaySelectTraversal::MODE_RAPID) {

		GPUSH_PATH(state,this);

		int status =  state.speedBoxVolume->PlaneStatus(state.GetMatrixRef(), state.GetMatrixInverse(),plane);
		
		// front to back 
		switch (status) {
		case INSIDE:
			ret = front.Traverse(state);
			ret = overlap.Traverse(state);
			break;
		case OUTSIDE:
			ret = overlap.Traverse(state);
   			ret = back.Traverse(state);
			break;

		case ON :	
		case OVERLAP :	

			ret = front.Traverse(state);
			ret = overlap.Traverse(state);
   			ret = back.Traverse(state);
			break;
		}


		GPOP_PATH(state);

		return(ret);

	}

	GPUSH_PATH(state,this);

/*
	ret = front.Traverse(state);
	ret = overlap.Traverse(state);
   	ret = back.Traverse(state);
*/
	 // get the viepoint in local coordinates
	GMatrixStack *mstack = state.GetCurrentMatrix();
	const Matrix &mI = mstack->GetMatrixInverse();
	 

	


	// transform world ray to object coordinates
    RayRange tRay;

    tRay = state.ray;
    tRay *= mI;


	Point p[2];
	//p[0] = state.ray.Eval(state.ray.tmin);
	//p[1] = state.ray.Eval(state.ray.tmax);

	p[0] = tRay.Eval(tRay.tmin);
	p[1] = tRay.Eval(tRay.tmax);

	int status =  PlaneStatus(2,p, plane);


		// front to back 
		switch (status) {
		case INSIDE:
			ret = front.Traverse(state);
			ret = overlap.Traverse(state);
			break;
		case OUTSIDE:
			ret = overlap.Traverse(state);
   			ret = back.Traverse(state);
			break;

		case ON :	
		case OVERLAP :	

			ret = front.Traverse(state);
			ret = overlap.Traverse(state);
   			ret = back.Traverse(state);
			break;
		}


	GPOP_PATH(state);

	return(ret);
}


int GvBspTree::Do(GBBoxTraversal &state)
{	int ret = 0; 

	ret = front.Traverse(state);
	ret = overlap.Traverse(state);
   	ret = back.Traverse(state);

	return(ret);
}


// render culling statistics 

int GvBspTree::boxCullsSucceeded;
int GvBspTree::viewerPlaneCullsSucceeded;
int GvBspTree::checkedNodes;

int GvBspTree::cullChecks;
int GvBspTree::cullChecksSucceeded;

void GvBspTree::ClearBspStats()
{
	checkedNodes=0;
	boxCullsSucceeded=0;
	viewerPlaneCullsSucceeded=0;
	cullChecks=0;
	cullChecksSucceeded=0;
}


void GvBspTree::TraceBspStats()
{
	if (cullChecks>0) {
		Reporter.Trace("Group cull checks %d, Culled : %d",cullChecks,cullChecksSucceeded);
	}
	if (checkedNodes <=0) return;
	Reporter.Trace("BSP Nodes visited : %d, Culled :  bbox-viewer %d ,  viewer / plane %d ",checkedNodes,boxCullsSucceeded,viewerPlaneCullsSucceeded);
}



int GvBspTree::Do(GglRenderTraversal &state)
{  	int ret = 0;

#ifdef _DEBUG
	checkedNodes++;

#ifdef _OGL
	state.glState->RenderCubeEdges(bbox.mi,bbox.ma);
#endif
#if 0
	// with inlines Bbox could be empty yet 
	if (!bboxOk) {
		if (!getBBox(bbox))
			bbox.SetEmpty();
		bboxOk = TRUE;
	}
	if (BoxPlaneStatus(bbox,state.viewPlanes) == OUTSIDE) {
		TRACE("Total cull");
		boxCullsSucceeded++;
		return 0;
	}
#endif
#endif

	if (state.traversalOrder == TRAVERSE_NORMAL) { // traverse everything 
			ret = front.Traverse(state);
			ret = overlap.Traverse(state);
   			ret = back.Traverse(state);
			return(ret);
	 }

	 // get the viepoint in local coordinates
	 GMatrixStack *mstack = state.GetCurrentMatrix();
	 
	 if (!mstack->viewpointLocalOk) { // need to transform it
		mstack->SetViewpointLocal(state.physicalCamera->position);
	 }  

	 // the viewer position with plane 
	 Plane plane;
	 
	 getPlane(plane); 
	 
	 float temp= DotProd(mstack->viewpointLocal, plane.n)+plane.d;
	 int status;
	 
	 if (state.doViewCulling) {
		if (!mstack->viewVolumeLocalOk) {
			mstack->SetViewVolumeLocal(state.viewVolume);
		}  

		status =  PlaneStatus(mstack->viewVolumeLocal, plane);

		// combine with viewpoint status
		if (status == OUTSIDE && temp>=0) {}
		else if (status == INSIDE && temp<=0) {}
		else status = OVERLAP;

	 } 
	 else 
		status = OVERLAP;

	 //  GRenderTraversal
	 
	 if (state.traversalOrder == TRAVERSE_FRONT_TO_BACK) temp = - temp;

	 if (temp <= 0.0 ) {
	  //  back  to front 
		switch (status) {
		case INSIDE:
			ret = overlap.Traverse(state);
			if (ret == TRAVERSE_STOP) break; 
			ret = front.Traverse(state);
			viewerPlaneCullsSucceeded++;
			break;

		case OUTSIDE:
   			ret = back.Traverse(state);
			if (ret == TRAVERSE_STOP) break; 
			ret = overlap.Traverse(state);
			viewerPlaneCullsSucceeded++;
			break;
				 
		case ON :	
		case OVERLAP :	
   			ret = back.Traverse(state);
			if (ret == TRAVERSE_STOP) break; 
			ret = overlap.Traverse(state);
			if (ret == TRAVERSE_STOP) break; 
			ret = front.Traverse(state);
			break;
		}

	 } else {
		// front to back 
		switch (status) {
		case INSIDE:
			ret = front.Traverse(state);
			if (ret == TRAVERSE_STOP) break; 
			ret = overlap.Traverse(state);
			viewerPlaneCullsSucceeded++;
			break;
		case OUTSIDE:
			ret = overlap.Traverse(state);
			if (ret == TRAVERSE_STOP) break; 
   			ret = back.Traverse(state);
			viewerPlaneCullsSucceeded++;
			break;

		case ON :	
		case OVERLAP :	

			ret = front.Traverse(state);
			if (ret == TRAVERSE_STOP) break; 
			ret = overlap.Traverse(state);
			if (ret == TRAVERSE_STOP) break; 
   			ret = back.Traverse(state);
			break;
		}
	 }

	return(ret);
}

// very similar to above

int GvBspTree::Do(GLoadInlinesTraversal &state)
{  	int ret = 0;

	 // already to much work in progress
	 if (state.pendingLoads > 5) 
		 return TRAVERSE_STOP;

	 if (state.traversalOrder == TRAVERSE_NORMAL) {
			ret = front.Traverse(state);
			ret = overlap.Traverse(state);
   			ret = back.Traverse(state);
			return(ret);
	 }


	 // get the viepoint in local coordinates
	 GMatrixStack *mstack = state.GetCurrentMatrix();
	 
	 if (!mstack->viewpointLocalOk) { // need to transform it
		mstack->SetViewpointLocal(state.physicalCamera->position);
	 }  

	 // the viewer position with plane 

	 Plane plane;
	 
	 getPlane(plane); 
	 
	 float temp= DotProd(mstack->viewpointLocal, plane.n)+plane.d;
	 int status;
	 
	 if (TRUE /*state.doViewCulling */ ) {

		if (!mstack->viewVolumeLocalOk) {
			mstack->SetViewVolumeLocal(state.viewVolume);
		}  

		status =  PlaneStatus(mstack->viewVolumeLocal, plane);

		// combine with viewpoint status
		if (status == OUTSIDE && temp>=0) {}
		else if (status == INSIDE && temp<=0) {}
		else status = OVERLAP;

	 } 
	 else 
		status = OVERLAP;

	 
	 if (state.traversalOrder == TRAVERSE_FRONT_TO_BACK) temp = - temp;

	 if (temp <= 0.0 ) {
	  //  back  to front 
		switch (status) {
		case INSIDE:
			ret = overlap.Traverse(state);
			if (ret == TRAVERSE_STOP) break; 
			ret = front.Traverse(state);
			break;

		case OUTSIDE:
   			ret = back.Traverse(state);
			if (ret == TRAVERSE_STOP) break; 
			ret = overlap.Traverse(state);
			break;
				 
		case ON :	
		case OVERLAP :	
   			ret = back.Traverse(state);
			if (ret == TRAVERSE_STOP) break; 
			ret = overlap.Traverse(state);
			if (ret == TRAVERSE_STOP) break; 
			ret = front.Traverse(state);
			break;
		}

	 } else {
		// front to back 
		switch (status) {
		case INSIDE:
			ret = front.Traverse(state);
			if (ret == TRAVERSE_STOP) break; 
			ret = overlap.Traverse(state);
			break;
		case OUTSIDE:
			ret = overlap.Traverse(state);
			if (ret == TRAVERSE_STOP) break; 
   			ret = back.Traverse(state);
			break;

		case ON :	
		case OVERLAP :	

			ret = front.Traverse(state);
			if (ret == TRAVERSE_STOP) break; 
			ret = overlap.Traverse(state);
			if (ret == TRAVERSE_STOP) break; 
   			ret = back.Traverse(state);
			break;
		}
	 }

	return(ret);
}


//
// GvBspGroup
//

GV_NODE_SOURCE_IMP(GvBspGroup,Gv2Group);

GvBspGroup::GvBspGroup() : buildBspTree(TRUE)
{
    GV_NODE_CONSTRUCTOR(GvBspGroup);
    isBuiltIn = FALSE;

	// from Gv2group
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);


    // from Gv2group
    GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);

	// for testing 
    GV_NODE_ADD_EVENT_OUT(bspTree);

}


GvBspGroup::~GvBspGroup()
{
}

// build the bsp tree
GvBool GvBspGroup::BuildBspTree()
{
	buildBspTree = FALSE;
	
	GvBspTree::ClearStats();

	bspTree.set(::BuildBspTree(children));

	return TRUE;
}

int GvBspGroup::OnFieldChanged(GvField *field)
{
	if (field == &children)
		buildBspTree = TRUE;

	return Gv2Group::OnFieldChanged(field);
}



int GvBspGroup::Do(GTraversal &state)
{

	if (buildBspTree) {
		return TraverseChildren(state);
	}
	else return bspTree.Traverse(state);
}

int GvBspGroup::Do(GPathTraversal &state)
{

	if (buildBspTree) {
		return TraverseChildren(state);
	}
	else return bspTree.Traverse(state);
}


int GvBspGroup::Do(GglRenderTraversal &state)
{
	int ret = 0; // return code 

	if (buildBspTree) {
		if (BuildBspTree()) {
		}
	}
	return bspTree.Traverse(state);
}




//
// GvOcclusion
//

GV_NODE_SOURCE_IMP(GvOcclusion,Gv2Group);

GvOcclusion::GvOcclusion() : enabled(TRUE), isActive(FALSE),lastPosition(-1e35f,0,0)
{
    GV_NODE_CONSTRUCTOR(GvOcclusion);
    isBuiltIn = FALSE;

	// from Gv2group
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);

    GV_NODE_ADD_FIELD(enabled);

    GV_NODE_ADD_FIELD(proxy);

    // from Gv2group
    GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);

    GV_NODE_ADD_EVENT_OUT(isActive);
    GV_NODE_ADD_EVENT_OUT(enterTime);
    GV_NODE_ADD_EVENT_OUT(exitTime);

	wasRendered = FALSE;


}


GvOcclusion::~GvOcclusion()
{
}


int GvOcclusion::Do(GglRenderTraversal &state)
{
	int ret = 0; // return code 


	if (!enabled) { // standard processing if not enabled 
		wasRendered = TRUE; // that flag that is was traversed
		return Gv2Group::Do(state);
	}

	GvBool wasActive = isActive;
	Point eye = state.physicalCamera->position;
	
	eye *= state.GetMatrixInverse();

	GvBool visible = TRUE;

	// tbd:bbox cull check ??

	if (eye == lastPosition) {
		// same position, don't check again
		visible = wasActive;
	} else {
		// check if eye is in proxy

		GvNode *prox = proxy.Eval(state);
		visible = !isOutside(state,prox,eye);
	}

	if (visible)
	{	// its in, trigger events 

		if (!wasActive)  {
			setActive(TRUE);
			enterTime.set(state.view->theTime);
			state.view->activeZones.add(this);
		}
		
		ret = children.Traverse(state);
		wasRendered = TRUE; // that flag that is was traversed

	}  // not visible 
	else {
	   if (wasActive) {
			setActive(FALSE);
			exitTime.set(state.view->theTime);
	   }	

	}

	lastPosition = eye;
	return(ret);
}


int GvOcclusion::Do(GRaySelectTraversal &state)
{	int ret = 0;

	// ignore if not active 
	if (!isActive) return ret;

	// what if multiple instances ????????

	return Gv2Group::Do(state);

}



// helpers 
GvBool GvOcclusion::isOutside(GTraversal &state,const Point &p)
{
		GvNode *prox = proxy.Eval(state);
		return isOutside(state,prox,p);
}

GvBool GvOcclusion::isOutside(GTraversal &state, GvNode* proxy,const Point &p) const
{
	if (proxy == NULL) return FALSE;
	if (RTISA(proxy,GvNodeShell)) {
		GvNodeShell *g = (GvNodeShell *) proxy;
		if (RTISA(proxy,Gv2IndexedFaceSet)) {
			if (g->theShell == NULL) { 
				g->BuildShell(state);
				// g->theShell->setFlag(CONVEX);
			}
		}
		return 	((GvNodeShell*) proxy) ->isOutside(p);
	}
	else if (RTISA(proxy,Gv2Transform)) {
		Gv2Transform *g = (Gv2Transform *) proxy;
		Point newP(p);

		Matrix m;
		if (g->getMatrix(m)) {
			m.Invert();
			newP = m * p;
		}


		return isOutside(state,g->children,newP);

	}
	else if (RTISA(proxy,Gv2Group)) {
		Gv2Group *g = (Gv2Group *) proxy;
		return isOutside(state,g->children,p);

	}
	else {
		// switch etc
		GvNode *n= proxy->Eval(state);
		if (n != proxy) return isOutside(state,n,p);
	}
	TRACE("GvOcclusion::isOutside bad node type %s \n",proxy->ClassName());
	return FALSE;
}

// must be outside of all group elements
GvBool GvOcclusion::isOutside(GTraversal &state, GvMFNode &proxy,const Point &p) const
{
	for(int i=0; i <proxy.getNum(); i++) {
		if (!isOutside(state,proxy.get1(i),p)) return FALSE;
	}
	return TRUE;
}



//
// GvInclusion
//

GV_NODE_SOURCE_IMP(GvInclusion,Gv2Group);

GvInclusion::GvInclusion() : 
enabled(TRUE), isActive(FALSE),isInside(FALSE),
portals(0),
lastPosition(-1e35f,0,0)
{
    GV_NODE_CONSTRUCTOR(GvInclusion);
    isBuiltIn = FALSE;

	// from Gv2group
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);

    GV_NODE_ADD_FIELD(enabled);

    GV_NODE_ADD_FIELD(proxy);

    GV_NODE_ADD_FIELD(portals);

    // from Gv2group
    GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);

    GV_NODE_ADD_EVENT_OUT(isActive);
    GV_NODE_ADD_EVENT_OUT(isInside);

    GV_NODE_ADD_EVENT_OUT(enterTime);
    GV_NODE_ADD_EVENT_OUT(exitTime);

	wasRendered = FALSE;


}


GvInclusion::~GvInclusion()
{
}



int GvInclusion::Do(GglRenderTraversal &state)
{
	int ret = 0; // return code 


	if (!enabled) { // standard processing if not enabled 
		wasRendered = TRUE; // that flag that is was traversed
		return Gv2Group::Do(state);
	}

	GvBool wasActive = isActive;
	GvBool wasInside = isInside;

	Point eye = state.physicalCamera->position;
	
	eye *= state.GetMatrixInverse();

	GvBool visible = TRUE;
	
	GvBool doorOpen = anyPortalOpen(state);


	if (eye == lastPosition) {
		// same position, don't check again
		visible = wasActive;

	} else {
		// check if eye is in proxy

		GvNode *prox = proxy.Eval(state);
		visible = isPointInside(state,prox,eye);

		setInside(visible);

	}

	if (visible || doorOpen)
	{	// its in, trigger events 

		if (!wasActive)  {
			setActive(TRUE);
			if (visible) {
				enterTime.set(state.view->theTime);
				state.view->activeZones.add(this);
			}

		}
		
		ret = children.Traverse(state);

		wasRendered = TRUE; // that flag that is was traversed

		if (!doorOpen && (ret>=0)) ret = TRAVERSE_STOP;

	}  // not visible 
	else {
	   if (wasActive) {
			setActive(FALSE);
			exitTime.set(state.view->theTime);
	   }	

	}

	lastPosition = eye;
	return(ret);
}


int GvInclusion::Do(GRaySelectTraversal &state)
{	int ret = 0;

	// ignore if not active 
	if (!isActive) return ret;

	// what if multiple instances ????????

	return Gv2Group::Do(state);
}


GvBool GvInclusion::anyPortalOpen(GGeometryTraversal &state) 
{
	if (portals.getNum() == 0) return FALSE;
	return isNodeVisible(state,portals);
}

GvBool GvInclusion::isBBoxVisible(GGeometryTraversal &state, BBox &proxy)
{
		GMatrixStack *mstack = state.GetCurrentMatrix();	
		if (mstack->m.contents & ( Matrix::HAS_NEGATIVE_SCALE) ) 
			return gtrue; // not working with neg scale 

		if (!mstack->viewpointLocalOk) { // need to transform it 
			mstack->SetViewpointLocal(state.physicalCamera->position);	
		}  

		if (!mstack->viewVolumeLocalOk) {
			mstack->SetViewVolumeLocal(state.viewVolume); 
		}

		BOOL doClipping;
		if (BoxViewVolumeOutside(proxy,mstack->viewpointLocal,mstack->viewVolumeLocal,doClipping)) { 
			return gfalse;
		}
		else return TRUE;

}

GvBool GvInclusion::isNodeVisible(GGeometryTraversal &state, GvNode* proxy)
{
	if (proxy == NULL) return FALSE;

	if (RTISA(proxy,GvNodeShell)) {
		GvNodeShell *g = (GvNodeShell *) proxy;

		if (g->theShell == NULL) { 
			g->BuildShell(state);
		}
		GShell *theShell = g->theShell;

		if (!theShell) return FALSE;

		return isBBoxVisible(state,theShell->GetBBox());


	}
	else if (RTISA(proxy,Gv2Transform)) {
		Gv2Transform *g = (Gv2Transform *) proxy;
		state.Push();

			state.AppendMatrix(g->Transform());



		GvBool ret = isNodeVisible(state,g->children);
		state.Pop();
		return ret;

	}
	else if (RTISA(proxy,Gv2Group)) {
		Gv2Group *g = (Gv2Group *) proxy;
		return isNodeVisible(state,g->children);
	}
	else {
		// switch 
		GvNode *n= proxy->Eval(state);
		if (n != proxy) return isNodeVisible(state,n);
	}
	TRACE("GvInclusion::isNodeVisible bad node type %s \n",proxy->ClassName());
	return FALSE;
}

// must be outside of all group elements
GvBool GvInclusion::isNodeVisible(GGeometryTraversal &state, GvMFNode &proxy) 
{
	for(int i=0; i <proxy.getNum(); i++) {
		if (isNodeVisible(state,proxy.get1(i))) return TRUE;
	}
	return FALSE;
}



// helpers 
GvBool GvInclusion::isPointInside(GTraversal &state,const Point &p)
{
		GvNode *prox = proxy.Eval(state);
		return isPointInside(state,prox,p);
}

GvBool GvInclusion::isPointInside(GTraversal &state, GvNode* proxy,const Point &p) const
{
	if (proxy == NULL) return TRUE;
	if (RTISA(proxy,GvNodeShell)) {
		GvNodeShell *g = (GvNodeShell *) proxy;
		if (RTISA(proxy,Gv2IndexedFaceSet)) {
			if (g->theShell == NULL) { 
				g->BuildShell(state);
				// g->theShell->setFlag(CONVEX);
			}
		}
		// special code for cylinder sphere etc 
		return 	 !((GvNodeShell*) proxy) ->isOutside(p);
	}
	else if (RTISA(proxy,Gv2Transform)) {
		Gv2Transform *g = (Gv2Transform *) proxy;
		Point newP(p);

		Matrix m;
		if (g->getMatrix(m)) {
			m.Invert();
			newP = m * p;
		}

		return isPointInside(state,g->children,newP);

	}
	else if (RTISA(proxy,Gv2Group)) {
		Gv2Group *g = (Gv2Group *) proxy;
		return isPointInside(state,g->children,p);

	}
	else {
		// switch etc
		GvNode *n= proxy->Eval(state);
		if (n != proxy) return isPointInside(state,n,p);
	}
	TRACE("GvOcclusion::isInside bad node type %s \n",proxy->ClassName());
	return FALSE;
}

// must be outside of all group elements
GvBool GvInclusion::isPointInside(GTraversal &state, GvMFNode &proxy,const Point &p) const
{
	for(int i=0; i <proxy.getNum(); i++) {
		if (isPointInside(state,proxy.get1(i),p)) return TRUE;
	}
	return FALSE;
}



//
// GvMetaverse
//

GV_NODE_SOURCE_IMP(GvMetaverse,Gv2Group);

GvMetaverse::GvMetaverse() : initialized(FALSE), 
			start(0,0,0),end(1,0,1),spacing(1,0,1), 
			urlPrefix(0),
			currentTile(-1,-1,-1)
{
    GV_NODE_CONSTRUCTOR(GvOcclusion);
    isBuiltIn = FALSE;

	// from Gv2group
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);

    
	GV_NODE_ADD_FIELD(start);
	GV_NODE_ADD_FIELD(end);
	GV_NODE_ADD_FIELD(spacing);

	GV_NODE_ADD_FIELD(proxy);

	GV_NODE_ADD_FIELD(urlPrefix);

    //GV_NODE_ADD_FIELD(enabled);

    // from Gv2group
    GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);

//    GV_NODE_ADD_EVENT_OUT(isActive);
//    GV_NODE_ADD_EVENT_OUT(enterTime);
//    GV_NODE_ADD_EVENT_OUT(exitTime);

}


GvMetaverse::~GvMetaverse()
{
	FlushAll();
}

// flush 
void GvMetaverse::FlushAll()
{
	children.deleteAll();
	for(int i=0; i<tileCache.Length(); i++) {
		GTileEntry * e = tileCache[i];
		tileCache[i]=NULL;
		if (e) delete e;
	}
	tileCache.Flush();
	currentTile  = Pointi(-1,-1,-1);
}

	// store entry in cache
void GvMetaverse::putCache(GTileEntry *entry)
{
	tileCache.Append(entry);
}
	
// find entry in cache
GTileEntry *GvMetaverse::getCache(Pointi coord)
{
	int l= tileCache.Length();
	if (l<=0) return NULL;

	GTileEntry **start = tileCache.DataRW();
	GTileEntry **end = tileCache.DataRW()+l;

	while (start<end) {
		if ((*start)->coord == coord)
			return *start;
		start++;
	}
	return NULL;
}

	// generate the scene graph for coordinate
GvNode *GvMetaverse::generateTileNode(Pointi coord)
{

	Gv2Inline *inl=NULL;
	
	inl = new Gv2Inline();

	generateTileUrl(coord,inl->url);
	
	// compute bbox ???

	return inl;
}



BOOL GvMetaverse::computeTileCoord(const Point &p,Pointi &coord)
{
	Point x=p-start;

	float s= spacing[0];
	if (s>0.0f) {
		if (p.x >= end[0]) return FALSE;
		x.x /= s;
		if (x.x<0) return FALSE;
		coord.x = x.x;
	}
	else coord.x = 0;

	s= spacing[1];
	if (s>0.0f) {
		if (p.y >= end[1]) return FALSE;
		x.y /= s;
		if (x.y<0) return FALSE;
		coord.y = x.y;
	}
	else coord.y = 0;

	s= spacing[2];
	if (s>0.0f) {
		if (p.z >= end[2]) return FALSE;
		x.z /= s;
		if (x.z<0) return FALSE;
		coord.z = x.z;
	}
	else coord.z = 0;

	return TRUE;
}

// get the Inline url field	
void GvMetaverse::generateTileUrl(Pointi coord,GvMFString &url)
{
	char buf[100];
	GvString s;
	sprintf(buf,"%d_%d_%d.wrl",coord.x,coord.y,coord.z);
	if (urlPrefix.getNum()>0) {
		s = urlPrefix.get1(0);
	}
	s+=buf;
	url.makeRoom(1);
	url.set1(0,s);
}



// add the tile node to the scene graph
BOOL GvMetaverse::addTileNode(GTileEntry *entry)
{
	children.add(entry->node); // or append 
	entry->inScene  = TRUE;
	return TRUE;
}

// add the tile node to the scene graph
void GvMetaverse::removeTileNode(GTileEntry *entry)
{
	children.remove(entry->node);
	entry->inScene  = FALSE;
}


int GvMetaverse::Do(GglRenderTraversal &state)
{
	int ret = 0; // return code 


	Point eye = state.physicalCamera->position;
	
	// eye *= state.GetMatrixInverse();

	Pointi eyei;

	if (!computeTileCoord(eye,eyei)) {
		// viewer outside of metaverse
		return ret;
	}

	// viewer moved to different tile 
	if (eyei != currentTile) {

		GTileEntry *eyeTile; // the tile the viewer is in
		eyeTile = getCache(eyei);
	
		if (eyeTile == NULL) { // not loaded, create tile 
			eyeTile = new GTileEntry(eyei);
			eyeTile->node = generateTileNode(eyei);
			putCache(eyeTile);
		}
	
		if (!eyeTile->inScene) {
			addTileNode(eyeTile);
		}
		currentTile = eyei;
	}

	return Gv2Group::Do(state);

}


int GvMetaverse::Do(GRaySelectTraversal &state)
{

	return Gv2Group::Do(state);
}


