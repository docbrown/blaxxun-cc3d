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
/******************************************************************************
@doc

@module GvMFields.cpp - Implementation for all GvMF -Fields|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:
parts based on QvLib from SGI

Changes:

Todo :

******************************************************************************/

#include "quaternation.h"


#include <gvsensors.h>

// need for parsing MFNode from string 
#include <gvscene.h>


// GvMFColor.cpp

#include <gvmfcolor.h>
#include <gvsfcolor.h>

GV_MFIELD_SOURCE(GvMFColor, float, 3);

GvBool
GvMFColor::read1Value(GvInput *in, int index)
{
    float *valuePtr = values + index * 3;
	
	ASSERT(index < getNum());

    return (in->read(valuePtr[0]) &&
	    in->read(valuePtr[1]) &&
	    in->read(valuePtr[2]));
}


int
GvMFColor::io1Value(AStream &a, int index)
{
    float *valuePtr = values + index * 3;

    return (a.io(valuePtr[0]) &&
	    a.io(valuePtr[1]) &&
	    a.io(valuePtr[2]));
}

int GvMFColor::get1(GvString &s,int index)
{
  float *valuePtr = values + index * 3;
  int ret=sprintf(s.getBuffer(3*15),"%g %g %g",valuePtr[0],valuePtr[1],valuePtr[2]);
  s.releaseBuffer();
  return(ret > 0);
}


int GvMFColor::set1(const char* s,int index)
{
  float *valuePtr = values + index * 3;

  int ret=sscanf(s,"%f %f %f",&valuePtr[0],&valuePtr[1],&valuePtr[2]);
  setDefault(FALSE);
  OnChanged();
  return(ret == 3);
}



int GvMFColor::get(PointArray &vals) const
{ int l;
  vals.SetLength(l=Length());
  float *vp = values;
  Point *pp = vals.DataRW();
  while (l>0){
  	pp->x = *vp++;
  	pp->y = *vp++;
  	pp->z = *vp++;
	pp++;
	l--;
  }
  return(vals.Length());
}

int GvMFColor::set(const PointArray &vals)
{ int l;
  makeRoom(l=vals.Length());

  float *vp = values;
  const Point *pp = vals;
  while (l>0){
  	*vp++ = pp->x;
  	*vp++ = pp->y;
  	*vp++ = pp->z;
	pp++;
	l--;
  }
  setDefault(FALSE);
  OnChanged();

  return(Length());
}


Point& GvMFColor::get1(int i, Point &color) const 
{
	if (Length() == 0) {
		color.Zero();
   		return (color);
	}

	if (i>=num) i = num-1;
	i*=3;
	color.Set(values[i+0],values[i+1],values[i+2]);; 
	return (color);
}

int GvMFColor::set(const Point val)
{ 
  makeRoom(1);
  float *vp = values;
  const Point *pp = &val;
  *vp++ = pp->x;
  *vp++ = pp->y;
  *vp++ = pp->z;
 
  setDefault(FALSE);
  OnChanged();

  return(Length());
}

void GvMFColor::set1(int index, const float value[3])
{
  float *p = &values[index*3];
  *p++ = value[0];
  *p++ = value[1];
  *p++ = value[2];
  setDefault(FALSE);
  OnChanged();
}

GvField* GvMFColor::get1Field(int index)
{
	if (index<0 || index >= getNum()) return NULL;

	GvSFColor *f = new GvSFColor();
	f->set(&values[index*3]);
	return f;
}

GvBool GvMFColor::operator==(const GvMFColor &f) const 
{
	int cnt = getNum();
	if (f.getNum() != cnt) return gfalse;
	cnt*=3;
	const float *a=values;
	const float *b=f.values;

    while (cnt >0 ) {
		if (*a != *b) return gfalse;
		a++;b++;
		cnt--;
	};
	return gtrue;
}



// interpolate in Hsv space 
void GvMFColor::InterpolateHsv (float *result, const float *A, float ta, const float *B,float tb) 
{ 
	float x[3],y[3],temp[3];
	Rgb2Hsv(A,x[0],x[1],x[2]);
	Rgb2Hsv(B,y[0],y[1],y[2]);
    temp[0] = (x[0] * ta + y[0] *tb); 
    temp[1] = (x[1] * ta + y[1] *tb); 
    temp[2] = (x[2] * ta + y[2] *tb); 
	Hsv2Rgb(temp[0],temp[1],temp[2],result);
}







#include <gvmffloat.h>
#include <gvsffloat.h>

GV_MFIELD_SOURCE(GvMFFloat, float, 1);

//GV_MFIELD_SOURCE_EQ(GvMFFloat, float, 1);

GvBool
GvMFFloat::read1Value(GvInput *in, int index)
{
    return in->read(values[index]);
}

int GvMFFloat::io1Value(AStream &a, int index)
{
    float *valuePtr = values + index;
    return (a.io(valuePtr[0]));
}


int GvMFFloat::get1(GvString &s,int index)
{
  int ret=sprintf(s.getBuffer(15),"%g",values[index]);
  s.releaseBuffer();
  return(ret >0);

}


int GvMFFloat::set1(const char* s,int index)
{
  int ret=sscanf(s,"%f",&values[index]);
  setDefault(FALSE);
  OnChanged();
  return(ret == 1);
}


int GvMFFloat::set(float val)
{ 
  makeRoom(1);
  values[0]=val;
  setDefault(FALSE);
  OnChanged();
  return(Length());
}

void GvMFFloat::set1(int index, float value)
{
  values[index] = value;
  setDefault(FALSE);
  OnChanged();
}


GvField* GvMFFloat::get1Field(int index)
{
	if (index<0 || index >= getNum()) return NULL;
	GvSFFloat *f = new GvSFFloat();
	f->set(values[index]);
	return f;
}

#include <gvmftime.h>
#include <gvsftime.h>

GV_MFIELD_SOURCE(GvMFTime, double, 1);


GvBool
GvMFTime::read1Value(GvInput *in, int index)
{
    return in->read(values[index]);
}

int GvMFTime::io1Value(AStream &a, int index)
{
    double *valuePtr = values + index;
    return (a.io(valuePtr[0]));
}


int GvMFTime::get1(GvString &s,int index)
{
  int ret=sprintf(s.getBuffer(30),"%.16g",values[index]);
  s.releaseBuffer();
  return(ret >0);

}


int GvMFTime::set1(const char* s,int index)
{
  int ret=sscanf(s,"%f",&values[index]);
  setDefault(FALSE);
  OnChanged();
  return(ret == 1);
}



int GvMFTime::set(double val)
{ 
  makeRoom(1);
  values[0]=val;
  setDefault(FALSE);
  OnChanged();
  return(Length());
}

void GvMFTime::set1(int index, double value)
{
  values[index] = value;
  setDefault(FALSE);
  OnChanged();
}

GvField* GvMFTime::get1Field(int index)
{
	if (index<0 || index >= getNum()) return NULL;
	GvSFTime *f = new GvSFTime();
	f->set(values[index]);
	return f;
}


#include <gvmflong.h>
#include <gvsflong.h>

GV_MFIELD_SOURCE(GvMFLong, long, 1);
//GV_MFIELD_SOURCE_EQ(GvMFLong, long, 1);

GvBool
GvMFLong::read1Value(GvInput *in, int index)
{
    return in->read(values[index]);
}

int GvMFLong::io1Value(AStream &a, int index)
{
    long *valuePtr = values + index;
    return (a.io(valuePtr[0]));
}


int GvMFLong::get1(GvString &s,int index)
{
  int ret=sprintf(s.getBuffer(15),"%ld",values[index]);
  s.releaseBuffer();
  return(ret > 0);

}


int GvMFLong::set1(const char* s,int index)
{
  int ret=sscanf(s,"%ld",&values[index]);
  setDefault(FALSE);
  OnChanged();
  return(ret == 1);
}

int GvMFLong::set(long val)
{ 
  makeRoom(1);
  values[0]=val;
  setDefault(FALSE);
  OnChanged();
  return(Length());
}

void GvMFLong::set1(int index, long value)
{
  values[index] = value;
  setDefault(FALSE);
  OnChanged();
}

GvField* GvMFLong::get1Field(int index)
{
	if (index<0 || index >= getNum()) return NULL;
	GvSFLong *f = new GvSFLong();
	f->set(values[index]);
	return f;
}



#include <gvmfint32.h>
#include <gvsfint32.h>

RTIMP(GvMFInt32,0,GvMFLong);

GvField* GvMFInt32::get1Field(int index)
{
	if (index<0 || index >= getNum()) return NULL;
	GvSFInt32 *f = new GvSFInt32();
	f->set(values[index]);
	return f;
}





#include <gvmfrotation.h>
#include <gvsfrotation.h>

GV_MFIELD_SOURCE(GvMFRotation, Rotation, 1);

GvBool
GvMFRotation::read1Value(GvInput *in, int index)
{
    return values[index].readValue(in);
}

int GvMFRotation::io1Value(AStream &a, int index)
{
    Rotation *valuePtr = values + index;
    return (valuePtr[0].ioValue(a));
}


int GvMFRotation::get1(GvString &s,int index)
{
  return (values[index].get(s));
}


int GvMFRotation::set1(const char* s,int index)
{
  int ret = values[index].set(s);
  setDefault(FALSE);
  OnChanged();
  return(ret);
}


void GvMFRotation::set1(int index, const Rotation &newValue)
{
  values[index] = newValue;
  setDefault(FALSE);
  OnChanged();
}

GvField* GvMFRotation::get1Field(int index)
{
	if (index<0 || index >= getNum()) return NULL;
	GvSFRotation *f = new GvSFRotation();
	f->set(values[index]);
	return f;
}




// Interpolate rotation 
void GvMFRotation::Interpolate(Rotation *result, const Rotation *A, float ta, const Rotation *B,float tb) 
{ 
        //result[0] = (A[0] * ta + B[0] *tb); 
    Quaternation a;
    Quaternation b;
    Quaternation c;
    A->get(a);
    B->get(b);
	c.Interpolate(a,b,tb); //  double alpha, int spin=0) { QuaternationSlerp(alpha,a,b,*this,spin); }
    
    result->set(c);
}







//GvMFVec2f.cpp

#include <gvmfvec2f.h>
#include <gvsfvec2f.h>

GV_MFIELD_SOURCE(GvMFVec2f, float, 2);
GV_MFIELD_SOURCE_EQ_SF(GvMFVec2f,GvSFVec2f)

GvBool
GvMFVec2f::read1Value(GvInput *in, int index)
{
    float *valuePtr = values + index * 2;

    return (in->read(valuePtr[0]) &&
	    in->read(valuePtr[1]));
}


int GvMFVec2f::io1Value(AStream &a, int index)
{
    float *valuePtr = values + index * 2;

    return (a.io(valuePtr[0]) &&
	    a.io(valuePtr[1]));
}

int GvMFVec2f::get1(GvString &s,int index)
{
  float *valuePtr = values + index * 2;

  int ret=sprintf(s.getBuffer(2*15),"%g %g",valuePtr[0],valuePtr[1]);
  s.releaseBuffer();
  return(ret >0);
}


int GvMFVec2f::set1(const char* s,int index)
{
  float *valuePtr = values + index * 2;

  int ret=sscanf(s,"%f %f",&valuePtr[0],&valuePtr[1]);
  setDefault(FALSE);
  OnChanged();
  return(ret == 2);
}

Point& GvMFVec2f::get1(int i, Point &value) const 
{
	if (i>=num) i = num-1;
	i*=2;
	value.Set(values[i+0],values[i+1],0);
	return(value);
}



int GvMFVec2f::get(PointArray &vals) const
{ int l;
  vals.SetLength(l=Length());
  float *vp = values;
  Point *pp = vals.DataRW();
  while (l>0){
  	pp->x = *vp++;
  	pp->y = *vp++;
	pp->z = 0.0;
	pp++;
	l--;
  }
  return(vals.Length());
}

int GvMFVec2f::set(const PointArray &vals) 
{ int l;
  makeRoom(l=vals.Length());

  float *vp = values;
  const Point *pp = vals;
  while (l>0){
  	*vp++ = pp->x;
  	*vp++ = pp->y;
	pp++;
	l--;
  }
  setDefault(FALSE);
  OnChanged();
  return(Length());
}

void GvMFVec2f::set1(int index, const float value[2])
{
  float *p = &values[index*2];
  *p++ = value[0];
  *p++ = value[1];
  setDefault(FALSE);
  OnChanged();
}

void GvMFVec2f::set1(int index, const Point &value)
{
  float *p = &values[index*2];
  *p++ = value.x;
  *p++ = value.y;
  setDefault(FALSE);
  OnChanged();
}

GvField* GvMFVec2f::get1Field(int index)
{
	if (index<0 || index >= getNum()) return NULL;
	GvSFVec2f *f = new GvSFVec2f();
	f->set(&values[index*23]);
	return f;
}

//-------------------------------------------------------------------
//
//						GvMFVec3f.cpp
//
//-------------------------------------------------------------------

#include <gvmfvec3f.h>
#include <gvsfvec3f.h>

GV_MFIELD_SOURCE(GvMFVec3f, float, 3);
GV_MFIELD_SOURCE_EQ_SF(GvMFVec3f,GvSFVec3f)

GvBool
GvMFVec3f::read1Value(GvInput *in, int index)
{
    float *valuePtr = values + index * 3;

    return (in->read(valuePtr[0]) &&
	    in->read(valuePtr[1]) &&
	    in->read(valuePtr[2]));
}

int
GvMFVec3f::io1Value(AStream &a, int index)
{
    float *valuePtr = values + index * 3;

    return (a.io(valuePtr[0]) &&
	    a.io(valuePtr[1]) &&
	    a.io(valuePtr[2]));
}

int GvMFVec3f::get1(GvString &s,int index)
{
  float *valuePtr = values + index * 3;
  int ret=sprintf(s.getBuffer(3*15),"%g %g %g",valuePtr[0],valuePtr[1],valuePtr[2]);
  s.releaseBuffer();
  return(ret > 0);
}


int GvMFVec3f::set1(const char* s,int index)
{
  float *valuePtr = values + index * 3;

  int ret=sscanf(s,"%f %f %f",&valuePtr[0],&valuePtr[1],&valuePtr[2]);
  setDefault(FALSE);
  OnChanged();
  return(ret == 3);
}


int GvMFVec3f::get(PointArray &vals) const
{ int l;
  vals.SetLength(l=Length());
  const float *vp = values;
  Point *pp = vals.DataRW();
  while (l>0){
  	pp->x = *vp++;
  	pp->y = *vp++;
  	pp->z = *vp++;
	pp++;
	l--;
  }
  return(vals.Length());
}

// compute the bounding box 
GvBool GvMFVec3f::getBBox(BBox &box) const
{
  const Point *vp,*vpend;
  Point mi,ma;

  if (Length()<=0) return(FALSE);

  vp= (const Point *) values;
  vpend=&vp[Length()];

  mi= ma= *vp++;
  for (; vp<vpend;vp++) {
		vp->MinMax(mi,ma);
  }

  box.mi = mi;
  box.ma = ma;
  return TRUE;
}

  



int GvMFVec3f::set(const PointArray &vals)
{ int l;
  makeRoom(l=vals.Length());

  float *vp = values;
  const Point *pp = vals;
  while (l>0){
  	*vp++ = pp->x;
  	*vp++ = pp->y;
  	*vp++ = pp->z;
	pp++;
	l--;
  }
  setDefault(FALSE);
  OnChanged();
  return(Length());
}


Point& GvMFVec3f::get1(int i, Point &value) const 
{
	if (i>=num) i = num-1;
	i*=3;
	value.Set(values[i+0],values[i+1],values[i+2]);
	return(value);
}

void GvMFVec3f::set1(int index, const float value[3])
{
  float *p = &values[index*3];
  *p++ = value[0];
  *p++ = value[1];
  *p++ = value[2];
  setDefault(FALSE);
  OnChanged();
}

void GvMFVec3f::set1(int index, const Point &value)
{
  float *p = &values[index*3];
  *p++ = value.x;
  *p++ = value.y;
  *p++ = value.z;
  setDefault(FALSE);
  OnChanged();
}



GvField* GvMFVec3f::get1Field(int index)
{
	if (index<0 || index >= getNum()) return NULL;
	GvSFVec3f *f = new GvSFVec3f();
	f->set(&values[index*3]);
	return f;
}

GvBool GvMFVec3f::operator==(const GvMFVec3f &f) const 
{
	int cnt = getNum();
	if (f.getNum() != cnt) return gfalse;
	cnt*=3;
	const float *a=values;
	const float *b=f.values;

    while (cnt >0 ) {
		if (*a != *b) return gfalse;
		a++;b++;
		cnt--;
	};
	return gtrue;
}


//-------------------------------------------------------------------
//
//						GvMFVec4f
//
//-------------------------------------------------------------------

GV_MFIELD_SOURCE(GvMFVec4f, float, 4);
GV_MFIELD_SOURCE_EQ_SF(GvMFVec4f,GvSFVec4f)



int GvMFVec4f::set1(const char* s,int index)
{
  float *valuePtr = values + index * 4;

  int ret=sscanf(s,"%f %f %f %f",&valuePtr[0],&valuePtr[1],&valuePtr[2],&valuePtr[3]);
  setDefault(FALSE);
  OnChanged();
  return(ret == 4);
}


int GvMFVec4f::get1(GvString &s,int index)
{
  float *valuePtr = values + index * 4;
  int ret=sprintf(s.getBuffer(4*15),"%g %g %g %g",valuePtr[0],valuePtr[1],valuePtr[2],valuePtr[3]);
  s.releaseBuffer();
  return(ret > 0);
}


int
GvMFVec4f::io1Value(AStream &a, int index)
{
    float *valuePtr = values + index * 4;

    return (a.io(valuePtr[0]) &&
	    a.io(valuePtr[1]) &&
	    a.io(valuePtr[2]) &&
	    a.io(valuePtr[3]));
}

GvBool
GvMFVec4f::read1Value(GvInput *in, int index)
{
    float *valuePtr = values + index * 4;

    return (in->read(valuePtr[0]) &&
	    in->read(valuePtr[1]) &&
	    in->read(valuePtr[2]) &&
	    in->read(valuePtr[3]));
}

/*


int
GvMFVec4f::io1Value(AStream &a, int index)
{
    float *valuePtr = values + index * 4;

    return (a.io(valuePtr[0]) &&
	    a.io(valuePtr[1]) &&
	    a.io(valuePtr[2]));
}




int GvMFVec4f::get(PointArray &vals) const
{ int l;
  vals.SetLength(l=Length());
  const float *vp = values;
  Point *pp = vals.DataRW();
  while (l>0){
  	pp->x = *vp++;
  	pp->y = *vp++;
  	pp->z = *vp++;
	pp++;
	l--;
  }
  return(vals.Length());
}

// compute the bounding box 
GvBool GvMFVec4f::getBBox(BBox &box) const
{
  const Point *vp,*vpend;
  Point mi,ma;

  if (Length()<=0) return(FALSE);

  vp= (const Point *) values;
  vpend=&vp[Length()];

  mi= ma= *vp++;
  for (; vp<vpend;vp++) {
		vp->MinMax(mi,ma);
  }

  box.mi = mi;
  box.ma = ma;
  return TRUE;
}
  
	
	  
int GvMFVec4f::set(const PointArray &vals)
{ int l;
  makeRoom(l=vals.Length());

  float *vp = values;
  const Point *pp = vals;
  while (l>0){
  	*vp++ = pp->x;
  	*vp++ = pp->y;
  	*vp++ = pp->z;
	pp++;
	l--;
  }
  setDefault(FALSE);
  OnChanged();
  return(Length());
}


Point& GvMFVec4f::get1(int i, Point &value) const 
{
	if (i>=num) i = num-1;
	i*=4;
	value.Set(values[i+0],values[i+1],values[i+2]);
	return(value);
}


void GvMFVec4f::set1(int index, const Point &value)
{
  float *p = &values[index*4];
  *p++ = value.x;
  *p++ = value.y;
  *p++ = value.z;
  setDefault(FALSE);
  OnChanged();
}

*/

void GvMFVec4f::set1(int index, const float value[4])
{
  float *p = &values[index*4];
  *p++ = value[0];
  *p++ = value[1];
  *p++ = value[2];
  *p++ = value[3];
  setDefault(FALSE);
  OnChanged();
}

GvField* GvMFVec4f::get1Field(int index)
{
	if (index<0 || index >= getNum()) return NULL;
	GvSFVec4f *f = new GvSFVec4f();
	f->set(&values[index*4]);
	return f;
}


//-------------------------------------------------------------------
//
//						GvMFString
//
//-------------------------------------------------------------------


#include <gvmfstring.h>
#include <gvsfstring.h>

// need constructor /destructor call
// GV_MFIELD_SOURCE(GvMFString, GvString, 1);
RTIMP(GvMFString,0,GvMField)

GV_MFIELD_SOURCE_EQ_FIELD(GvMFString);

GvMFString::GvMFString(int cnt)	
{							
    values = NULL;			
    allocValues(cnt);							     
}									     

// Destructor
									     
GvMFString::~GvMFString()							     
{									     
    if (values != NULL)							     
	    delete [] values;
}									     
									     
void									     
GvMFString::allocValues(int newNum)					     
{									     
    if (values == NULL) {						     
	    if (newNum > 0)							     
	        values = new GvString[newNum]();
    }					
	else if (newNum>0 && newNum<=maxNum) {
	    num = newNum; 
		return;
	}
    else {								     
	if (newNum > 0) {							     
	    
	    GvString *newValues = new GvString[newNum]();

        int copyNum = min(newNum,num);
	    
	    for (int i=0; i< copyNum; i++) newValues[i]=values[i];
	    
	    delete [] values;
		values = newValues;

	} else {								     
	    delete [] values;
	    values = NULL;						     
	}								     
    }									     
    num = maxNum = newNum;						     
}


GvBool
GvMFString::read1Value(GvInput *in, int index)
{
    return in->read(values[index]);
}

int GvMFString::io1Value(AStream &a, int index)
{
    GvString *valuePtr = values + index;
    
    if (a.IsInput()) {
   		TRACE("To Do %s::io1Value \n",ClassName());
		return(0);

   } else 
   {
   		return(a.wstr(valuePtr->getString()));
   }
   return (0);
}


int GvMFString::get1(GvString &s,int index)
{
  GvString *valuePtr = values + index;
  s = *valuePtr;
  return(1);

}

void GvMFString::set1(int index, const GvString &newValue)
{
   values[index]=newValue;
   setDefault(FALSE);
   OnChanged();
}



int GvMFString::set1(const char* s,int index)
{
  GvString *valuePtr = values + index;
  *valuePtr = s;
  return(1);
}


int GvMFString::set(int newNum,const GvString *newValues) 
{ 
	num=0; // do not copy old values 
	allocValues(newNum); 
    for(int i= 0 ; i<num; i++) values[i] = newValues[i];

    setDefault(FALSE);
    OnChanged();

    return(num>0);
}

GvField* GvMFString::get1Field(int index)
{
	if (index<0 || index >= getNum()) return NULL;
	GvSFString *f = new GvSFString();
	f->set_(values[index]);
	return f;
}

GvBool GvMFString::operator==(const GvMFString &f) const 
{
	if (f.getNum() != getNum()) return FALSE;
	for (int i=0; i < getNum(); i++) {
		if (! ( values[i] == f.values[i]))
			return FALSE;
	}
	return TRUE;
}


// GvMFNode 
#include <gvsfnode.h>
#include <gvmfnode.h>
#include <gvnode.h>
#include <gvdirectionallight.h>

// need constructor /destructor call

//GV_MFIELD_SOURCE_0(GvMFNode, GvNode *, 1);
//GV_MFIELD_SOURCE_0(GvMFNode, GvNodeHandle, 1);

RTIMP(GvMFNode,0,GvMField)
GV_MFIELD_SOURCE_EQ_FIELD(GvMFNode);



GvMFNode::GvMFNode(int cnt) 
{
    values = NULL;
    /* Make room for cnt value to start */				      
    allocValues(cnt);							      
}

// Destructor
									     
GvMFNode::~GvMFNode()							     
{									     
    if (values != NULL)	{
        // unreference nodes !!!!!!!!!!!!! is this needed ?
        // for(int i=0; i<num; i++) values[i] = NULL;
	    delete [] values;
    }
}			

// get the class id of the corresponding SF field class
TypeId GvMFNode::getSFType()
{
    return GvSFNode::ClassId();
}
						     
									     
void									     
GvMFNode::allocValues(int newNum)					     
{	int i;
								     
    if (values == NULL) {						     
	    if (newNum > 0)							     
	        values = new GvNodeHandle[newNum]();
    }									     
	else if (newNum>0 && newNum<=maxNum) {
	    for (i=newNum; i< num; i++) // release nodes past new length
            values[i].destruct();

	    num = newNum; 
		return;
	}
	else if (newNum > 0) {							     
        
	    GvNodeHandle *newValues = new GvNodeHandle[newNum]();

//      ASSERT(newValues[newNum-1] == (GvNode *) NULL);

        int copyNum = min(newNum,num);
	    
	    for (i=0; i< copyNum; i++) 
            newValues[i]=(GvNode *)values[i];

   	    delete [] values; // destructor will get called 

		values = newValues;

	} else {								     
        // for(i=0; i<num; i++)     values[i].destruct();
	    delete [] values;
	    values = NULL;						     
	}								     
    num = maxNum = newNum;						     
}


GvBool
GvMFNode::read1Value(GvInput *in, int index)

{	GvNode *newNode = NULL;
    GvBool ret = GvNode::read(in, newNode);


	values[index] = newNode; // will ref it

	if (newNode) {
		ASSERT(newNode->getBrowser() != NULL);			
	}

	if (!ret) {
		if (newNode) // remove node from nameSpace 
			if (!(!newNode->getName()))
				in->removeReference(newNode->getName(),newNode); // new 11.12.97

		values[index] = NULL; // read error, but node might be allocated
		// free not done by GvMField::readValueList
	}
	else {
		// if (!GvNode::readRoutesOrProtos(in))  return(FALSE);

		ret = GvNode::readRoutesOrProtos(in);	// 15.11.99 hg, return failure

	}

	return(ret);
}


void GvMFNode::set(GvNode *node)
{
	makeRoom(1);
	set1(0,node);
	setDefault(FALSE);
	OnChanged();
}


int GvMFNode::set(const char* s)
{
	//delete contents 
	makeRoom(0);

	if (strcmp(s,"[]" /* OPEN_CLOSE_BRACE*/) == 0) { // [] empty
		setDefault(TRUE);
		OnChanged();
  		return(1);
	} 

	//TRACE("Parsing an MFNode from :%s\n",s);
	// create a parser 
	GvInput in;

    in.setString(s);
	in.version = 2.0;
	in.vrml2 = 1;

	GvScene *scene = new GvScene();
	scene->ref();
	
	in.setScene(scene);

	if (getContainer()) 
		scene->setBrowser(getContainer()->getBrowser());

/*	
	GReporter *reporter = &Reporter;
	if (reporter) {
	   in.setReporter(reporter);
	   in.reportErrors = TRUE;
	}	
*/

	// read 
	int ret = this->readValue(&in);
	
	scene->unref();

	setDefault(FALSE);
	OnChanged();

	return ret;
}




int GvMFNode::set(int newNum,const GvNodeHandle *newValues) 
{ 
    int i;

	if (values && values !=newValues) // clean out node handles
		for(i=0; i<num; i++)   values[i].destruct();
	
	num=0;

    allocValues(newNum); 

    for( i= 0 ; i<num; i++) values[i] = newValues[i];
    
	setDefault(FALSE);
    OnChanged();

    return(num>0);
}


// get SFNode copy from element at index i
GvField* GvMFNode::copy1(int i) 
{   
   if ( i<0 || i>=num) return NULL;
   return new GvSFNode(values[i]); 
}

GvNode* GvMFNode::get1(int i) 
{  
   if ( i<0 || i>=num) return NULL;
   return(values[i]); 
}

// replace child at index i
GvBool GvMFNode::set1(int index, GvNode *node)
{
   if (index<0 || index >= num) return(FALSE);

   values[index] = node;

   if (node && container)
   if (node->m_browser == NULL) // tmp workaround for browser 
			node->m_browser = container->m_browser;

   setDefault(FALSE);
   OnChanged();
   return(TRUE);
}


// replace child at index i
GvBool GvMFNode::set1(int index, GvSFNode *node)
{
   if (index<0 || index >= num) return(FALSE);

   values[index] = node->value;
   if (node->value && container)
   if (node->value->m_browser == NULL) // tmp workaround for browser 
			node->value->m_browser = container->m_browser;


   setDefault(FALSE); 
   OnChanged();
   return(TRUE);
}

GvField* GvMFNode::get1Field(int index)
{
	if (index<0 || index >= getNum()) return NULL;
	GvSFNode *f = new GvSFNode();
	f->set(values[index]);
	return f;
}





// append a new child
void GvMFNode::append(GvNode *node)
{
    int i=num;
    makeRoom(num+1),
    values[i]= node;
    setDefault(FALSE);
    flags.modified = TRUE; 
    // ?? OnChanged();
}

// insert child at index
int  GvMFNode::insert(int index,GvNode *node)
{
    int i=num;
    if (index<0 || index> num) return(FALSE);
    
    makeRoom(num+1);

    for(int n=i; n>index; n--)
        values[n] = values[n-1];
    values[index]= node;
	flags.modified = TRUE; 
    return(TRUE);
}


// add a new child, if not already in list
GvBool GvMFNode::add(GvNode *node)
{
    for(int i= 0 ; i<num; i++) 
        if (values[i] == node) return (FALSE);

	if (node == NULL) return FALSE;

	if (RTISA(node,GvLight)) {
		insert(0,node);
	}
    else append(node);
    return(TRUE);
}

// add new childs, if not already in list
void GvMFNode::add(GvMFNode *nodes)

{
    for(int i= 0 ; i<nodes->num; i++) 
        add(nodes->values[i]);

}

// remove  child, if in list
// Return TRUE if found
GvBool GvMFNode::remove(GvNode *node)

{
    for(int i= 0 ; i<num; i++)  {
        if (values[i] == node) {
            deleteElement(i);
            return (TRUE);
        }
    }

    return(FALSE);
}

// remove new childs, if in list
void GvMFNode::remove(GvMFNode *nodes)

{
    for(int i= 0 ; i<nodes->num; i++) 
        remove(nodes->values[i]);

}


// delete child at index
GvBool GvMFNode::deleteElement(int index)
{
    if (index>=num || index<0) return FALSE;
    
    for (int i=index+1; i<num; i++)
         values[i-1]=values[i];

    values[num-1].destruct();
    num--;
	
	flags.modified = TRUE; 
    return(TRUE);
}

// delete child at index with replacement swap (value[index]=value[num-1]
GvBool GvMFNode::deleteElementSwap(int index)
{
    if (index>=num || index<0) return FALSE;
    
	if (index == (num -1)) {
		values[num-1]= (GvNode *) NULL;
	    num--;

	}
	else {	// replace with last 
        values[index]=values[num-1];
		values[num-1].destruct();
	    num--;
	}
	
	flags.modified = TRUE; 
    return(TRUE);
}

void
GvMFNode::deleteAll()
{
   // done in allocValues
   // int n=num;
   // for(int i=0; i<n; i++)  values[i].destruct();
   makeRoom(0);
   flags.modified = TRUE; 
}


// remove all nodes from list with a refCnt <= refCnt 
// return number of removed nodes 
int 
GvMFNode::removeUnused(int refcnt)
{
   int n=num;
   int cnt=0;
   for(int i=n-1; i>=0; i--)  {
	   GvNode *n = values[i];	
	   if (n == NULL || n->refCnt <= refcnt) {
			deleteElement(i);
			cnt++;
	   }	
   }
   if (cnt >0) {
	   TRACE("%d unused nodes removed from MFNode \n",cnt);
   }	
   return cnt;
}


// find a child by name, NULL if not found 
GvNode *GvMFNode::find(const GvName &name) 
{
    for(int i= 0 ; i<num; i++)  {
		GvNode *n=values[i];
        if (n && n->getName() == name) {
            return(n);
        }
    }
    return((GvNode*) NULL);
}
	
// find a child by type (IsA), NULL if not found  
GvNode *GvMFNode::find(TypeId type)
{
    for (int i= 0 ; i<num; i++)  {
		GvNode *n=values[i];
        if (n && n->IsA(type)) {
            return(n);
        }
    }
    return((GvNode*) NULL);
}


// move some nodes up as first childs,
// workaround, some nodes are triggered into state during rendering

GvBool GvMFNode::CheckLights()
{  GvBool ret=FALSE;
   GvBool atStart = TRUE;
   for (int i= 0 ; i<num; i++)  {
        if (values[i] && ( 
				RTISA(values[i],GvLight)   || 
				RTISA(values[i],GvProximitySensor) // triggered during rendering, move up 
				|| RTISA(values[i],GvVisibilitySensor) // triggered during rendering, move up 			)
			)
			&& !atStart) { // move node to beginning

			ret  = TRUE;
			GvNodeHandle l= values[i];
			deleteElement(i);
			insert(0,l);
        } else {
			atStart = FALSE;
		}
    }
	return (ret);

}


// find a child by index, NULL if not found 
GvNode *GvMFNode::find(int index)
{
   if (index<0 || index >= num) return (GvNode*) NULL;
   return (values[index]); 
}


// find a child by ptr, return index, -1  if not found
int  GvMFNode::find(GvNode * node)
{
    for(int i= 0 ; i<num; i++)  {
        if (values[i] == node) {
            return(i);
        }
    }
    return(-1);
}



// computes copyOnInstance flag from any child nodes flag set
GvBool 
GvMFNode::computeCopyOnInstance(GvCopyContext &context)
{
   if (num <=0) return FALSE; 
   GvBool flag = FALSE;

   for(int i=0; i<num; i++) {
	  GvNode *n=values[i];
	  if (n == NULL) continue;	

	  if (n == container) { // simple self reference avoidance
			ASSERT(0);
			if (n->getCopyOnInstance()) flag = TRUE;
			continue;
	  }	

       // force computation of flag
       if (n->computeCopyOnInstance(context))
            flag = TRUE; 
   } 
   return(flag);
}

#if 0

// getCopyOnInstance flag from any child nodes flag copyOnInstance flag set
GvBool 
GvMFNode::getCopyOnInstance()
{
   if (num <=0) return FALSE; 
   GvBool flag = FALSE;

   for(int i=0; i<num; i++) {
       if (values[i]->getCopyOnInstance())
            return(TRUE);
   } 
   return(FALSE);
}
#endif


#ifdef _COM

#include "gtrace.h"

/* EventIn methods */
STDMETHODIMP GvMFNode::setValue(int cnt,IDispatch* *newValue) 
{ 
		EAI_TRACE("MfNode::setvalue %d \n",cnt);
		if (newValue == NULL) {
			makeRoom(0);
			setDefault(FALSE);
			OnChanged();
			return S_OK;
		}
		if (cnt != getNum()) makeRoom(cnt);

		for(int i=0; i< cnt;i++)	{
			GvNode *n=NULL;
			if (newValue[i]) {
				Node *v;
				if (newValue[i]->QueryInterface(IID_Node, (void **)&v) == S_OK) {
					v->getNative((long *) &n);
					v->Release();
				}
			}
			values[i] = n;
		}

		setDefault(FALSE);
		OnChanged();
		return S_OK; 
}

STDMETHODIMP GvMFNode::set1Value(int index,Node* newValue) 
{ 
		EAI_TRACE("MfNode::set1value %d \n",index);

		if (index >=getNum()) makeRoom(index+1);
		GvNode *n=NULL;
		if (newValue) newValue->getNative((long *) &n);
		set1(index,n); 
		return S_OK; 
}

/* EventOut methods */ 
STDMETHODIMP GvMFNode::getSize(int *cnt) { *cnt = getNum();  return S_OK; }
STDMETHODIMP GvMFNode::getValue(int cnt,IDispatch* *v) 
{ 
	EAI_TRACE("MfNode::getvalue %d \n",cnt);
			if (cnt > getNum()) cnt = getNum();
			for(int i=0; i<cnt;i++)	{
				GvNode *value = get1(i);
				if (value == NULL) v[i] = NULL;
				else  value->QueryInterface(IID_IDispatch, (void **)&v[i]);
			}
			return S_OK;  
}

   
STDMETHODIMP GvMFNode::get1Value(int index,Node**v) 
{ 
	EAI_TRACE("MfNode::get1value %d \n",index);
			if (index >=getNum()) return S_FALSE;
			GvNode *value = get1(index);
			if (value == NULL) *v = NULL;
			else  value->QueryInterface(IID_Node, (void **)v);
			return S_OK;  
}


#endif



