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
#ifndef  _GV_SF_VEC3F_
#define  _GV_SF_VEC3F_


#include <gvsubfield.h>

class GvSFVec3f : public GvSField 

#ifdef _COM
	,public CComDualImpl<EventInSFVec3f, &IID_EventInSFVec3f, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutSFVec3f, &IID_EventOutSFVec3f, &LIBID_GLViewVrmlLib>
#endif


{
  public:
    
	float value[3];

    GV_SFIELD_HEADER(GvSFVec3f);


    GvSFVec3f(float x=0.0,float y=0.0,float z=0.0) { value[0]=x,value[1]=y,value[2]=z; };
	GvSFVec3f(const Point &p) { value[0]=p.x,value[1]=p.y,value[2]=p.z; }

	static GvFieldClass getClassFieldType() { return (SFVEC3F); }
	GvFieldClass getFieldType() const { return (SFVEC3F); }

	operator Point()  const { return(Point(value[0],value[1],value[2])); }
	Point getPoint() const { return(Point(value[0],value[1],value[2])); }
	operator float*() { return(value); }
    
	const float* get() const  { return value;}
	int get(Point &val) const { val.Set(value[0],value[1],value[2]); return (1); }

	void set(const Point &p) { value[0]=p.x,value[1]=p.y,value[2]=p.z; setDefault(FALSE); OnChanged(); }
	void set(float x,float y, float z) { value[0]=x,value[1]=y,value[2]=z; setDefault(FALSE); OnChanged(); }
    void set(const float v[3]) { value[0] = v[0],value[1] = v[1],value[2] = v[2]; setDefault(FALSE); OnChanged(); }

	float get1(int index) const		{ return value[index]; }
	void set1(int index, float v)	{ value[index] = v; setDefault(FALSE); OnChanged(); }

	float x() const		{ return value[0]; }
	float y() const		{ return value[1]; }
	float z() const		{ return value[2]; }

	void setx(float v)	{ set1(0,v); }
	void sety(float v)	{ set1(1,v); }
	void setz(float v)	{ set1(2,v); }


    const GvSFVec3f& operator= (const GvSFVec3f &f) { set(f.value); return(f); } 
    GvBool operator==(const GvSFVec3f &f) const { return(value[0] == f.value[0] && value[1] == f.value[1] && value[2] == f.value[2]); }

	int ioValue(AStream &a);

	int get(GvString &value);
	int set(const char* value);

#ifdef _COM
	BEGIN_COM_MAP(GvSFVec3f)
		COM_INTERFACE_ENTRY(EventInSFVec3f)
		COM_INTERFACE_ENTRY(EventOutSFVec3f)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvSFVec3f)

	COM_DEFAULT_SFIELD_METHODS()

    /* EventIn methods */
    STDMETHOD(setValue)(float *newValue) { set(newValue[0],newValue[1],newValue[2]); COM_EXTERN_TOUCH(); return S_OK; }

	/* EventOut methods */ 
	STDMETHOD(getValue)(float *v) { v[0]=value[0],v[1]=value[1],v[2]=value[2]; return S_OK;  }


#endif

};



class PointArray;
class AStream;


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//								GvSFVec4f	
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/


class GvSFVec4f : public GvSField 
{
  public:
    
	float value[4];

    GV_SFIELD_HEADER(GvSFVec4f);


    GvSFVec4f(float x=0.0,float y=0.0,float z=0.0, float w=0.0) { value[0]=x,value[1]=y,value[2]=z,value[3]=w; };
//	GvSFVec4f(const Point &p) { value[0]=p.x,value[1]=p.y,value[2]=p.z,value[3]=p.w; }

	static GvFieldClass getClassFieldType() { return (SFVEC4F); }
	GvFieldClass getFieldType() const { return (SFVEC4F); }

//	operator Point()  const { return(Point(value[0],value[1],value[2],value[3])); }
//	Point getPoint() const { return(Point(value[0],value[1],value[2],value[3])); }
	operator float*() { return(value); }
    
	const float* get() const  { return value;}
//	int get(Point &val) const { val.Set(value[0],value[1],value[2],value[3]); return (1); }

//	void set(const Point &p) { value[0]=p.x,value[1]=p.y,value[2]=p.z,value[3]=p.w; setDefault(FALSE); OnChanged(); }
	void set(float x,float y, float z, float w) { value[0]=x,value[1]=y,value[2]=z,value[3]=w; setDefault(FALSE); OnChanged(); }
    void set(const float v[4]) { value[0] = v[0],value[1] = v[1],value[2] = v[2],value[3] = v[3]; setDefault(FALSE); OnChanged(); }

	float get1(int index) const		{ return value[index]; }
	void set1(int index, float v)	{ value[index] = v; setDefault(FALSE); OnChanged(); }

	float x() const		{ return value[0]; }
	float y() const		{ return value[1]; }
	float z() const		{ return value[2]; }
	float w() const		{ return value[3]; }

	void setx(float v)	{ set1(0,v); }
	void sety(float v)	{ set1(1,v); }
	void setz(float v)	{ set1(2,v); }
	void setw(float w)	{ set1(3,w); }


    const GvSFVec4f& operator= (const GvSFVec4f &f) { set(f.value); return(f); } 
    GvBool operator==(const GvSFVec4f &f) const { return(value[0] == f.value[0] && value[1] == f.value[1] && value[2] == f.value[2] && value[3] == f.value[3]); }

	int ioValue(AStream &a);

	int get(GvString &value);
	int set(const char* value);
};


#endif /* _GV_SF_VEC3F_ */
