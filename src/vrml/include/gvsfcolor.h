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
#ifndef  _GV_SF_COLOR_
#define  _GV_SF_COLOR_
/******************************************************************************
@doc

@module GvSFColor.h - VRML SFColor field|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvSFColor>

Notes:

Changes:
$Revision: 1.15 $
$Log: gvsfcolor.h,v $
Revision 1.15  1999/07/06 16:35:22  tom
*** empty log message ***

Revision 1.14  1999/05/10 13:54:14  tom
*** empty log message ***


18.10.98 HG default value is 0 0 0 in VRML 97 !

Todo :

******************************************************************************/


#include <gvsubfield.h>

class AStream;

class GvSFColor : public GvSField 
#ifdef _COM
	,public CComDualImpl<EventInSFColor, &IID_EventInSFColor, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutSFColor, &IID_EventOutSFColor, &LIBID_GLViewVrmlLib>
#endif

{
  public:
    float value[3];
    GV_SFIELD_HEADER(GvSFColor);

    GvSFColor(float r=0.0f,float g=0.0f,float b=0.0f) { value[0]=r,value[1]=g,value[2]=b; };

	static GvFieldClass getClassFieldType() { return (SFCOLOR); }
	GvFieldClass getFieldType() const { return (SFCOLOR); }

	operator Point()  const { return(Point(value[0],value[1],value[2])); }
	Point getPoint() const { return(Point(value[0],value[1],value[2])); }
	operator float* () { return(value);}
    const float *get() const  { return value;}

	
	// get as 4 component vector
	float * GetRGBA(float *rgba,float a=1.0) { 
		rgba[0]=value[0],rgba[1]=value[1],
		rgba[2]=value[2],rgba[3]=a; 
		return (rgba);
	}

	int ioValue(AStream &a);
	int get(GvString &value);
	int set(const char* value);

	void get(Point &val) const { val.Set(value[0],value[1],value[2]); }
	void set(const Point &val)   { value[0] = val.x,value[1] = val.y,value[2] = val.z; setDefault(FALSE); OnChanged(); }
	void set(float r,float g, float b) { value[0]=r,value[1]=g,value[2]=b; setDefault(FALSE); OnChanged(); }
    void set(const float v[3]) { value[0] = v[0],value[1] = v[1],value[2] = v[2]; setDefault(FALSE); OnChanged(); }

	void setHsv(const Point &val);
	void getHsv(Point &val);

	float get1(int index)			{ return value[index]; }
	void set1(int index, float v)	{ value[index] = v; setDefault(FALSE); OnChanged(); }


    const GvSFColor& operator= (const GvSFColor &f) { set(f.value); return(f); } 
    GvBool operator==(const GvSFColor &f) const { return(value[0] == f.value[0] && value[1] == f.value[1] && value[2] == f.value[2]); }

	// set value to interpolation from two same fields 
	int interpolateFrom(GvSFColor *a,GvSFColor *b, float t);
	virtual int interpolateFrom(GvField *a,GvField *b, float t);

#ifdef _COM
	BEGIN_COM_MAP(GvSFColor)
		COM_INTERFACE_ENTRY(EventInSFColor)
		COM_INTERFACE_ENTRY(EventOutSFColor)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvSFColor)

	COM_DEFAULT_SFIELD_METHODS()

    /* EventIn methods */
    STDMETHOD(setValue)(float *newValue) { set(newValue); COM_EXTERN_TOUCH(); return S_OK; }

	/* EventOut methods */ 
	STDMETHOD(getValue)(float *v) { v[0]=value[0],v[1]=value[1],v[2]=value[2]; return S_OK;  }


#endif

	    
};


// convert color from hsv to rgb 
void Hsv2Rgb( float h, float s, float v, float vec[3]);

//
// convert rgb to HSV 
//
void Rgb2Hsv(const float vec[3], float &h, float &s, float &v);



#endif /* _GV_SF_COLOR_ */
