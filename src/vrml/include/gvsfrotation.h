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
#ifndef  _GV_SF_ROTATION_
#define  _GV_SF_ROTATION_

#include <gvnode.h>
#include <gvsubfield.h>

class Quaternation;


// a single rotation
// axis need not to be normalized 

class Rotation {
  public:
    float axis[3];	// rotation axis, need not to be normalized 
    float angle;	// rotation angle in radians

    Rotation(float x=0.0,float y=0.0,float z=1.0,float Angle = 0) {
         axis[0]=x,axis[1]=y,axis[2]=z; angle = Angle;
    }
	Rotation(const float v[4]) { axis[0]=v[0],axis[1]=v[1],axis[2]=v[2]; angle = v[3]; }
    Rotation(const Rotation &r) { axis[0]=r.axis[0],axis[1]=r.axis[1],axis[2]=r.axis[2]; angle = r.angle; }
	Rotation (const Quaternation &q) { set(q); }

    void set(float x=0.0,float y=1.0,float z=0.0,float Angle = 0) {
         axis[0]=x,axis[1]=y,axis[2]=z; angle = Angle;
    }
    
    void set(const Rotation &r) { axis[0]=r.axis[0],axis[1]=r.axis[1],axis[2]=r.axis[2]; angle = r.angle; }
    
	void set(const float v[4]) { axis[0]=v[0],axis[1]=v[1],axis[2]=v[2]; angle = v[3]; }

    Point Axis() const { return (Point(axis[0],axis[1],axis[2])); }
	float Angle() const  { return(angle); }

    void setAxis(const Point &p) { axis[0]=p.x,axis[1]=p.y,axis[2]=p.z; }
    void setAngle(float angle) { this->angle = angle; }

	// cast to 4 float 	
	const float *get() const { return(&axis[0]); }  
	
	// cast to 4 float 	
	operator const float *() { return(&axis[0]); }  

    void get(float &x,float &y,float &z,float &Angle) {
         x=axis[0],y=axis[1],z=axis[2]; Angle = angle;
    }

	// get 1 element 0..2 axis, 3 amgöe 
	float get1(int index)			{ if (index >= 3) return angle;
									  return axis[index]; 
									}

	void set1(int index, float v)	{ if (index >= 3)  angle = v;
									  else axis[index]=v; 
									}


	// get rotation matrix 
	void get(Matrix &m) const
	{
	       m = RotationAxisMatrix(Point(axis[0],axis[1],axis[2]), angle);
	}

    // convert to quaternation
    void get(Quaternation &q) const;

    // convert from quaternation
    void set(const Quaternation &q) ;

   
    GvBool readValue(GvInput *in);

	int ioValue(AStream &a); 
	int get(GvString &value);
	int set(const char* value);

    GvBool operator==(const Rotation &f) const { return(axis[0] == f.axis[0] && axis[1] == f.axis[1] && axis[2] == f.axis[2]  && angle == f.angle); }

};


// decompose matrix into parts 
int DecomposeMatrix(const Matrix &m, // input matrix 
					Point &scale,
					Point &shear, 
					Rotation &rotation, 
					Point &translate);




class GvSFRotation : public GvSField 
#ifdef _COM
	,public CComDualImpl<EventInSFRotation, &IID_EventInSFRotation, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutSFRotation, &IID_EventOutSFRotation, &LIBID_GLViewVrmlLib>
#endif

{
  public:

	// the rotation value (4 floats)  
    Rotation value;

    GV_SFIELD_HEADER(GvSFRotation);

	// constructors 

    GvSFRotation(float x=0.0,float y=0.0,float z=1.0,float angle = 0) : value(x,y,z,angle) {}
	GvSFRotation(const float v[4]) : value(v) {}
    GvSFRotation(const Rotation &r) :value(r) {}
    GvSFRotation(const Quaternation &r) { value.set(r); }

	static GvFieldClass getClassFieldType() { return (SFROTATION); }
	GvFieldClass getFieldType() const { return (SFROTATION); }

	// get the value 
	const Rotation& get() const  { return value; }

	// get the axis of rotation
    Point Axis() const { return value.Axis(); }

	// get the angle of rotation in radians
    float Angle() const  { return value.Angle(); }

	// get the rotation as matrix 
	void get(Matrix &m) const  { value.get(m); }

    // convert to quaternation
    void get(Quaternation &q) const { value.get(q);}

	// output 
    int ioValue(AStream &a) { return value.ioValue(a); } 
	
	// get as string 
	int get(GvString &valueS) { return value.get(valueS); } 

	// set by string 
    int set(const char* valueS) { int ret=value.set(valueS); setDefault(FALSE); OnChanged(); return ret; } 
    
	// set functions 
    // convert from quaternation
    void set(const Quaternation &q) { value.set(q); setDefault(FALSE); OnChanged(); }
    void set(float x,float y,float z,float angle = 0) { value.set(x,y,z,angle); setDefault(FALSE); OnChanged();  }
    void set(const Rotation &r) { value.set(r); setDefault(FALSE); OnChanged();  }
    void set(const float values[4]) { value.set(values); setDefault(FALSE); OnChanged();  }

	float get1(int index)			{ return value.get1(index); }
	void set1(int index, float v)	{ value.set1(index,v); setDefault(FALSE); OnChanged(); }


    const GvSFRotation& operator=(const GvSFRotation &f) { value = f.value;  setDefault(FALSE); OnChanged();  return(f); }
    GvBool operator==(const GvSFRotation &f) const { return(value == f.value); }

#ifdef _COM
	BEGIN_COM_MAP(GvSFRotation)
		COM_INTERFACE_ENTRY(EventInSFRotation)
		COM_INTERFACE_ENTRY(EventOutSFRotation)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvSFRotation)

	COM_DEFAULT_SFIELD_METHODS()

    /* EventIn methods */
    STDMETHOD(setValue)(float *newValue) { set(newValue); return S_OK; }

	/* EventOut methods */ 
	STDMETHOD(getValue)(float *v) { v[0]=value[0],v[1]=value[1],v[2]=value[2],v[3]=value[3]; return S_OK;  }


#endif



};

#endif /* _GV_SF_ROTATION_ */
