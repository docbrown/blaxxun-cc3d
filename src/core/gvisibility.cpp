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

gvisibility.cpp

Copyright (c) 1997 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

$Revision: 1.24 $
$Log: gvisibility.cpp,v $
Revision 1.24  1999/11/29 20:30:16  holger
empty message

Revision 1.22  1999/07/06 16:55:15  tom
*** empty log message ***



Todo :

******************************************************************************/


#include "stdafx.h"
//#include "point.h"
//#include "matrix.h"




#include "gvisibility.h"

// is bbox b transformed by m outside a ?
gbool BoxBoxOutside(const BBox &a, const Matrix &m,const BBox &b)
{
	switch (m.contents) {
	case 0 : // identity 
				return( (b.ma.x < a.mi.x) || (b.mi.x	> a.ma.x)
						|| (b.ma.y < a.mi.y) || (b.mi.y > a.ma.y)
						|| (b.ma.z < a.mi.z) || (b.mi.z > a.ma.z));

	case Matrix::HAS_TRANSLATION : 
	case Matrix::HAS_SCALE :  // only positive
	case Matrix::HAS_SCALE | Matrix::HAS_TRANSLATION :  // only positive
				{
				Point mi = m * b.mi;
				Point ma = m * b.ma;

				return( (ma.x < a.mi.x) || (mi.x	> a.ma.x)
						|| (ma.y < a.mi.y) || (mi.y > a.ma.y)
						|| (ma.z < a.mi.z) || (mi.z > a.ma.z));

			  }

	default : {
				BBox x = m * b;
				// check only mi & ma transformed 
				return a.Outside(x);

			  }
	}

}


// INSIDE, OUTSIDE ON
int BoxPlaneStatus(const BBox &box, const Plane &plane) 
{ Point p;
  float temp;

  p.x= (plane.n.x >= 0.0) ? box.ma.x : box.mi.x;
  p.y= (plane.n.y >= 0.0) ? box.ma.y : box.mi.y;
  p.z= (plane.n.z >= 0.0) ? box.ma.z : box.mi.z;

  temp= DotProd(p, plane.n)+plane.d;

  if (temp < BEPS /* 0 */ ) return INSIDE;

  // temp >= 0

  p.x= (plane.n.x <= 0.0) ? box.ma.x : box.mi.x;
  p.y= (plane.n.y <= 0.0) ? box.ma.y : box.mi.y;
  p.z= (plane.n.z <= 0.0) ? box.ma.z : box.mi.z;

  temp= DotProd(p, plane.n)+plane.d;
  
  //TRACE("%g %g %g %g",p.x,p.y,p.z,temp);	
  if (temp >= -BEPS /* 0.0*/ ) 
	  return OUTSIDE;

  return OVERLAP;

/*

  if (iszero(temp)) { // the corner is on 
	// both points of bbox on plane 
	float a= DotProd(box.mi, plane.n)+plane.d;
	float b= DotProd(box.ma, plane.n)+plane.d;
	if (iszero(a) && iszero(b)) return ON;
  }
  
  return OUTSIDE;
*/

//  if (iszero(temp)) return(2); /* corner on */
//  return(temp<0);
}

// return TRUE if box fully outside of box 
gbool BoxPlaneOutside(const BBox &box, const Plane &plane) 
{ Point p;
  float temp;


  p.x= (plane.n.x <= 0.0) ? box.ma.x : box.mi.x;
  p.y= (plane.n.y <= 0.0) ? box.ma.y : box.mi.y;
  p.z= (plane.n.z <= 0.0) ? box.ma.z : box.mi.z;

  temp= DotProd(p, plane.n)+plane.d;

  if (temp >= -BEPS  ) return TRUE;

  return FALSE;

}


// test a box against a set of plane 
// returns only OUTSIDE & OVERLAP 
int BoxPlaneStatus(const BBox &box,const Array<Plane> &planes)
{
	int status;
	for (int i=0; i<planes.Length(); i++) {
		status = BoxPlaneStatus(box,planes[i]);
		if (status == OUTSIDE) 
			return OUTSIDE;
	}
	return OVERLAP;
}

// test a box against 5 planes of view frustrum 
// returns true if box outside off all planes 
// planes are defined by the view point position and 4 points on far plane
// return also if resulting object need clipping to view frustrum
gbool BoxViewVolumeOutside(const BBox &box,const Point &viewpoint,PointArray &viewVolume,BOOL &needClip)
{
	Plane p;

	int s; // status
	int icnt=0; // inside count
	//TRACE_BBOX(box);
	ASSERT(viewVolume.Length() == 4); 
	// far 
	p.Set(viewVolume[0],viewVolume[1],viewVolume[2]);
	if ((s = BoxPlaneStatus(box, p)) == OUTSIDE)	return gtrue;
	if (s == INSIDE) icnt++;

	p.Set(viewpoint,viewVolume[1],viewVolume[0]);
	if ((s = BoxPlaneStatus(box, p)) == OUTSIDE)	return gtrue;
	if (s == INSIDE) icnt++;

	p.Set(viewpoint,viewVolume[2],viewVolume[1]);
	if ((s = BoxPlaneStatus(box, p)) == OUTSIDE)	return gtrue;
	if (s == INSIDE) icnt++;

	p.Set(viewpoint,viewVolume[3],viewVolume[2]);
	if ((s = BoxPlaneStatus(box, p)) == OUTSIDE)	return gtrue;
	if (s == INSIDE) icnt++;

	p.Set(viewpoint,viewVolume[0],viewVolume[3]);
	if ((s = BoxPlaneStatus(box, p)) == OUTSIDE)	return gtrue;
	if (s == INSIDE) icnt++;
	
	needClip = (icnt !=5); // if inside of all planes, don't need clipping
	//TRACE("Inside %d \n",needClip);

	return gfalse;
}

// test a box against 5 planes of view frustrum 
// returns true if box outside off all planes 
// planes are defined by the view point position and 4 points on far plane
gbool BoxViewVolumeOutside(const BBox &box,const Point &viewpoint,PointArray &viewVolume)
{
	Plane p;

	// far 
	p.Set(viewVolume[0],viewVolume[1],viewVolume[2]);
	if (BoxPlaneOutside(box,p))	return gtrue;

	p.Set(viewpoint,viewVolume[1],viewVolume[0]);
	if (BoxPlaneOutside(box,p))	return gtrue;

	p.Set(viewpoint,viewVolume[2],viewVolume[1]);
	if (BoxPlaneOutside(box,p))	return gtrue;

	p.Set(viewpoint,viewVolume[3],viewVolume[2]);
	if (BoxPlaneOutside(box,p))	return gtrue;

	p.Set(viewpoint,viewVolume[0],viewVolume[3]);
	if (BoxPlaneOutside(box,p))	return gtrue;

	return gfalse;
}



// test a list of points against plane 
// can return all 4 cases 
int PlaneStatus(const PointArray &v, const Plane &plane)
{
	int n = v.Length();

	if (n == 0) return OVERLAP; // strange object 

	int status;

	// test first point with plane 
	float temp= DotProd(v[0], plane.n)+plane.d;

	if (temp<-BEPS) status = INSIDE;
	else if (temp>BEPS) status = OUTSIDE;
	else status  = ON;

	//test rest, early exit if OVERLAP 
	for (int i=1; i<n; i++) {

		temp= DotProd(v[i], plane.n)+plane.d;

		switch (status) {
		case INSIDE: if (temp>BEPS) return OVERLAP; break;
		case OUTSIDE: if (temp<-BEPS) return OVERLAP; break;
		case ON : {
					if (temp<0) status = INSIDE;
					else if (temp>0) status = OUTSIDE;
					else status  = ON;
					break;
				  }
		}

	}
	return (status);
}

int PlaneStatus(int np, const Point *p, const Plane &plane)
{

	if (np == 0) return OVERLAP; // strange object 

	int status;

	// test first point with plane 
	float temp= DotProd(*p, plane.n)+plane.d;

	if (temp<-BEPS) status = INSIDE;
	else if (temp>BEPS) status = OUTSIDE;
	else status  = ON;

	np--;
	p++;

	//test rest, early exit if OVERLAP 
	while(np>0) {

		temp= DotProd(*p, plane.n)+plane.d;

		switch (status) {
		case INSIDE: if (temp>BEPS) return OVERLAP; break;
		case OUTSIDE: if (temp<-BEPS) return OVERLAP; break;
		case ON : {
					if (temp<0) status = INSIDE;
					else if (temp>0) status = OUTSIDE;
					else status  = ON;
					break;
				  }
		}
		np--;
		p++;

	}
	return (status);
}

int SpherePlaneStatus(const Point &center,float radius, const Plane &plane)
{
	int status;
	// test first point with plane 
	float temp= DotProd(center, plane.n)+plane.d;

	if (temp< -(radius+BEPS)) status = INSIDE;
	else if (temp>(radius  + BEPS)) status = OUTSIDE;
	else status  = ON;

	return (status);
}



// Compute a separation Planes 
gbool ComputeSeparationPlane(const BBox &a,const BBox &b, Plane &plane, int &frontIndex,int &backIndex)
{
	 // a x + b y + c z + d = 0
	// a = 1 b = c =0
	// d = - a x 
	if (a.ma.x <= b.mi.x) { 
		plane.Set(1,0,0, -(a.ma.x + b.mi.x)*0.5); frontIndex = 0, backIndex = 1; return TRUE; 
	}
	if (a.mi.x >= b.ma.x) { 
		plane.Set(1,0,0, -(a.mi.x + b.ma.x)*0.5); frontIndex = 1, backIndex = 0; return TRUE; 
	}

	if (a.ma.y <= b.mi.y) { 
		plane.Set(0,1,0, -(a.ma.y + b.mi.y)*0.5); frontIndex = 0, backIndex = 1; return TRUE; 
	}
	if (a.mi.y >= b.ma.y) { 
		plane.Set(0,1,0, -(a.mi.y + b.ma.y)*0.5); frontIndex = 1, backIndex = 0; return TRUE; 
	}

	if (a.ma.z <= b.mi.z) { 
		plane.Set(0,0,1, -(a.ma.z + b.mi.z)*0.5); frontIndex = 0, backIndex = 1; return TRUE; 
	}
	if (a.mi.z >= b.ma.z) { 
		plane.Set(0,0,1, -(a.mi.z + b.ma.z)*0.5); frontIndex = 1, backIndex = 0; return TRUE; 
	}

	return FALSE;
}


/*

  compute a separation plane splitting a box
  along the dominant axis

*/

gbool ComputeSeparationPlane(const BBox &a, Plane &plane)
{
	Point size = a.Size();
	
	//TRACE_BBOX(a);
	
	int i= size.Dominant();

	switch (i) {
	case 0 : plane.Set(1,0,0,-(a.ma.x + a.mi.x)*0.5); return TRUE;
	case 1 : plane.Set(0,1,0,-(a.ma.y + a.mi.y)*0.5); return TRUE;
	case 2 : plane.Set(0,0,1,-(a.ma.z + a.mi.z)*0.5); return TRUE;
	default : return FALSE;
	}


}

//
// GConvexVolume
// 
GConvexVolume::GConvexVolume(const Matrix &m,const Matrix &minverse,const PointArray &v_, const Array<Plane> &planes_) 
{
	Set(m,minverse,v_,planes_);
}


void GConvexVolume::Set(const Matrix &m,const Matrix &minverse,const PointArray &v_, const Array<Plane> &planes_) 
{
	int n=v_.Length();
	int i;

	v.SetLength(n);
	// transform vertices & bounding box
	for(i=0;i<n;i++)
		v[i]= m * v_[i];

	UpdateBBox();

	// transform planes
	n=planes_.Length();

	planes.SetLength(n);

    // I sure hope that matrix is invertible...
    // Matrix use = Transpose(Invert(tform));
	const int planepts[]=  { 0,0,3,3,4,4};
    // return RotateOnly(use, p);

	Point center = bbox.Center();

	for(i=0;i<n;i++) {
		// TRACE("Plane %d \n",i);
		// TRACE_PLANE((planes_[i]));
		planes[i]= minverse * planes_[i];
/*
		if (::PlaneStatus(1, &center, planes[i]) != INSIDE) {
				planes[i].n = -planes[i].n; 
				planes[i].d = -planes[i].d; 
				TRACE("Error");
		}
*/
		//
		//Vector n= RotateOnly(minverse,planes_[i].n);
		//planes[i].Set(v[planepts[i]],n);

		// TRACE_PLANE((planes[i]));

	}
}

// for Bsp Tree, test transformed plane against volume
int GConvexVolume::PlaneStatus(const Matrix &m,const Matrix &minverse, const Plane &plane)
{
	Plane p =  m * plane; // m is the matrix, from volume to space, so m is the inverse
	return BoxPlaneStatus(bbox,p);
}



		int GConvexVolume::ComputePlaneMasks(const PointArray &v,Array<GPlaneMask> &masks) 
		{
			int nv =v.Length();
			
			ASSERT(planes.Length() <= GMAX_PLANES);
			masks.SetLength(nv);
			GPlaneMask ormask=0;
			GPlaneMask andmask= (GPlaneMask) -1;

			for (int i=0; i<nv;i++) {
				GPlaneMask m=0; 

				for (int j=0; j<planes.Length();j++) {
					// test point with plane
					
					Plane &plane = planes[j];

					float temp= DotProd(v[i], plane.n)+plane.d;

					if (temp> - BEPS) { // OUTSIDE;
						m |= (1<<j);
					}

				}
				masks[i] = m;

				ormask |= m;
				andmask &= m;
			}

			if (andmask !=0 ) return OUTSIDE;
			if (ormask == 0 ) return INSIDE;

			return OVERLAP;
		}

gbool GConvexVolume::ComputePenetration(int n, const int *fp, const PointArray &v, const Array<GPlaneMask> &masks,GCollisionResponse &response) const 
{
	ASSERT(n>0);
	gbool atLeastOnePointInside = gfalse;
	gbool newCollision = gfalse;

	while (n>0) {
		int vi = *fp++;
		GPlaneMask m;

		m = masks[vi];
		n--;
		if (m == 0) { // point is inside 
				atLeastOnePointInside = gtrue;
		
				for (int j=0; j<planes.Length();j++) {
					// test point with plane
					
					const Plane &plane = planes[j];

					float temp= DotProd(v[vi], plane.n)+plane.d;

					if (temp < response.currentMinDistance) {
						// this vertex is more inside
						response.currentMinDistance = temp;
						response.currentMinPlaneIndex = j;
						response.currentMinVertexIndex = vi;
						response.collision = gtrue;
						newCollision = gtrue;
					}
				} // for all planes
		}
				
	}
	// all points inside ?


	if (!atLeastOnePointInside) {
		TRACE("Polygon overlaps collision volume \n");
		// intersect edges with volume ... ??
	}
	return newCollision;

}

