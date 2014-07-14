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
#ifndef  _GV_SF_VEC2F_
#define  _GV_SF_VEC2F_
/******************************************************************************
@doc

@module GvVec2f.h - VRML SFVec2f field|

Copyright (c) 1996 - 1998	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvSFVec2f>

Notes:

Changes:
$Revision: 1.15 $
$Log: gvsfvec2f.h,v $
Revision 1.15  1999/07/06 16:35:26  tom
*** empty log message ***

Revision 1.14  1999/05/10 13:54:20  tom
*** empty log message ***


Todo :

******************************************************************************/



#include <gvnode.h>


#include <gvsubfield.h>

class GvSFVec2f : public GvSField 
#ifdef _COM
	,public CComDualImpl<EventInSFVec2f, &IID_EventInSFVec2f, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutSFVec2f, &IID_EventOutSFVec2f, &LIBID_GLViewVrmlLib>
#endif

{
  public:
    GvFloat value[2];

    GV_SFIELD_HEADER(GvSFVec2f);

    GvSFVec2f(GvFloat x=0.0,GvFloat y=0.0) { value[0]=x,value[1]=y; };
	GvSFVec2f(const Point &p) { value[0]=p.x,value[1]=p.y; }
	
	static GvFieldClass getClassFieldType() { return (SFVEC2F); }
	GvFieldClass getFieldType() const { return (SFVEC2F); }


	operator Point()  const { return(Point(value[0],value[1],0.0)); }
	Point getPoint() const  { return(Point(value[0],value[1],0.0)); }

	operator GvFloat*() { return(value); }
    const GvFloat* get() const  { return value;}

	
	void set(const Point &p) { value[0]=p.x,value[1]=p.y; setDefault(FALSE); OnChanged(); }
	void set(GvFloat a, GvFloat b) { value[0]=a,value[1]=b; setDefault(FALSE); OnChanged(); }
    void set(const GvFloat v[2]) { value[0] = v[0],value[1] = v[1]; setDefault(FALSE); OnChanged(); }

    int get(Point &val) const { val.Set(value[0],value[1], 0.0f); return (1); }

	float get1(int index) const			{ return value[index]; }
	void set1(int index, GvFloat v)	{ value[index] = v; setDefault(FALSE); OnChanged(); }

	float x() const		{ return value[0]; }
	float y() const		{ return value[1]; }
	float z() const		{ return 0.0; }

	void setx(float v)	{ set1(0,v); }
	void sety(float v)	{ set1(1,v); }

 //  const GvSFVec3f& operator= (const GvSFVec3f &f) { set(f.value); return(f); } 
    const GvSFVec2f& operator= (const GvSFVec2f &f) { set(f.value); return(f); } 
    GvBool operator==(const GvSFVec2f &f) const { return(value[0] == f.value[0] && value[1] == f.value[1] ); }


	int ioValue(AStream &a);
	int get(GvString &value);
	int set(const char* value);

#ifdef _COM
	BEGIN_COM_MAP(GvSFVec2f)
		COM_INTERFACE_ENTRY(EventInSFVec2f)
		COM_INTERFACE_ENTRY(EventOutSFVec2f)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvSFVec2f)

	COM_DEFAULT_SFIELD_METHODS()

    /* EventIn methods */
    STDMETHOD(setValue)(float *newValue) { set(newValue[0],newValue[1]); COM_EXTERN_TOUCH(); return S_OK; }

	/* EventOut methods */ 
	STDMETHOD(getValue)(float *v) { v[0]=value[0],v[1]=value[1]; return S_OK;  }


#endif



};

#endif /* _GV_SF_VEC2F_ */
