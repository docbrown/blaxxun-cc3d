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

Point.cpp

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:
Generic Point + Vector (= Normal) class	

Classes:
Point
	Vector

Plane

BBox

Ray

Notes:

Changes:

$Revision: 1.13 $
$Log: point.cpp,v $
Revision 1.13  1999/07/06 16:55:18  tom
*** empty log message ***

Revision 1.12  1999/03/10 10:21:55  tom
*** empty log message ***



Todo :
   Expand Ray BBbox + Plane functionality

******************************************************************************/


#include "stdafx.h"
//#include "point.h"
//#include "matrix.h"


void Point::Rotate(const Point& AxisDir, float angle)
{	 Matrix M = RotationAxisMatrix(AxisDir,angle);
	 Set(M * *this);
}

void Point::Rotate(const Point& AxisPt, const Point& AxisDir,float angle)
{	 Matrix M = RotationAxisMatrix(AxisPt,AxisDir,angle);
	 Set(M * *this);
}

int Point::io(AStream &a) 
{ 
  if (a.fmt == IV_FMT) {
  	 a.io(x); a.io(y); a.io(z);
  } else {
  	a.Sep('('); a.io(x); a.io(y); a.io(z); a.Sep(')'); 
  }
  return(a.stat); 
}

Plane Plane3(const Point &p0,const Point &p1,const Point &p2)
{
		Plane p;
		// two vectors
		float ux = p1.x-p0.x;
		float uy = p1.y-p0.y;
		float uz = p1.z-p0.z;
		float vx = p2.x-p0.x;
		float vy = p2.y-p0.y;
		float vz = p2.z-p0.z;
		// crossprod is normal
		p.n.Set(uy*vz-uz*vy,
				uz*vx-ux*vz,
				ux*vy-uy*vx);
		p.n.Normalize();
		p.d = -DotProd(p.n,p0);				// compute plane constant
		return p;
}


///////////////////////////////////////////////////////////////////////////////

/*
ostream& operator<< (ostream& os,const Point& a)
{
   os << "(" << a.x<< " " << a.y<< " " << a.z << ")";
   return(os);
}
*/

///////////////////////////////////////////////////////////////////////////////

int Plane::io(AStream &a) 
{ a.Sep('('); n.io(a); a.io(d); a.Sep(')'); return(a.stat); }


///////////////////////////////////////////////////////////////////////////////


int BBox::io(AStream &a) { a.Sep('('); mi.io(a); ma.io(a); a.Sep(')'); return(a.stat); }

BBox operator* (const Matrix& m , const BBox& b)
{
   Point p1 = m * b.mi;
 
   BBox result(p1);
   
   p1.Set(b.mi.x,b.ma.y,b.mi.z); result.Union(m*p1);
   p1.Set(b.mi.x,b.mi.y,b.ma.z); result.Union(m*p1);
   p1.Set(b.mi.x,b.ma.y,b.ma.z); result.Union(m*p1);

   p1.Set(b.ma.x,b.mi.y,b.mi.z); result.Union(m*p1);
   p1.Set(b.ma.x,b.mi.y,b.ma.z); result.Union(m*p1);
   p1.Set(b.ma.x,b.ma.y,b.mi.z); result.Union(m*p1);
   result.Union(m * b.ma);

   return (result);
}

BBox& BBox::operator*= (const Matrix &m)
{
	*this = m * *this;

	return(*this);
}

// >0 if BBOX inside the space of the plane , 2 a point i on plane
int BBox::InsidePlane(const Plane &plane) const
{ Point p;
  float temp;
  p.x= (plane.n.x >= 0.0) ? ma.x : mi.x;
  p.y= (plane.n.y >= 0.0) ? ma.y : mi.y;
  p.z= (plane.n.z >= 0.0) ? ma.z : mi.z;

  temp= DotProd(p, plane.n)+plane.d;
  if (iszero(temp)) return(2); /* corner on */
  return(temp<0);
}


///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/*
void PointArray::resize(unsigned N)
{
   NumericArray<float>::resize(N);
}
*/
#if 0
int PointArray::io(AStream &a)
{ int i;

  Point newpoint;

  a.Sep('(');

   if (a.isOutput()) {
     for(i=0;i<np;i++) {
	if (i>1) a.Sep(' ');
	if ((i%5) == 4) a.nl();
	p[i].io(a);
     }
   } else {
     np=0;
     /* read while ( until closing paren */
     while (a.Skip() == '(') {
	newpoint.io(a);
	append(newpoint);
     }
  }

  a.Sep(')');
  return (a.stat);
}

#endif

