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
#ifndef  _GV_MF_VEC3F_
#define  _GV_MF_VEC3F_

#include <gvsubfield.h>
class PointArray;
class AStream;


class GvMFVec3f : public GvMField 
#ifdef _COM
	,public CComDualImpl<EventInMFVec3f, &IID_EventInMFVec3f, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutMFVec3f, &IID_EventOutMFVec3f, &LIBID_GLViewVrmlLib>
#endif


{

    GV_MFIELD_HEADER(GvMFVec3f,float,3);

  public:
    float *values;


   	operator float *() { return(values); }
    float* operator[] (int i) { return(&values[i*3]);}

	static GvFieldClass getClassFieldType() { return (MFVEC3F); }
	GvFieldClass getFieldType() const { return (MFVEC3F); }
	GvFieldClass getSFFieldType() const { return (SFVEC3F); }


	int io1Value(AStream &a, int index);
	int get1(GvString &value, int index);
	int set1(const char* value, int index);

	int get(PointArray &vals) const;
	GvBool getBBox(BBox &box) const;



	// direct reference 
    const float  *get1(int index) const  { return &values[index*3]; }
    float  *get1(int index) { return &values[index*3]; }

	Point& get1(int index, Point &value) const;
    
	Point&  get1Point(int index) { return *((Point *) &values[index*3]);}

    void set1(int index, const float value[3]);
    void set1(int index, const Point &value);

    const float  *get() const { return values; }

	int set(const PointArray &vals);

	GvField* get1Field(int index);

	GvBool operator==(const GvMFVec3f &f) const; 


    //! Interpolate 1 value  result = A * ta + B * tb
    void Interpolate (float *result, const float *A, float ta, const float *B,float tb) 
    { 
        result[0] = (A[0] * ta + B[0] *tb); 
        result[1] = (A[1] * ta + B[1] *tb); 
        result[2] = (A[2] * ta + B[2] *tb); 
    }


    //! Interpolate cnt values result = A * ta + B * tb
    void Interpolate (float *result, const float *A, float ta, const float *B,float tb, int cnt) 
    { 
        while (cnt >0 ) {
            Interpolate(result,A,ta,B,tb);
            result +=3; A+=3; B+=3; cnt--;
        }
    }

    // for normal interpolator 
	// to do: is this correct (interpolating on sphere) 
    void InterpolateNormal (float *result, const float *A, float ta, const float *B,float tb)
    { 
        Point n((A[0] * ta + B[0] *tb),(A[1] * ta + B[1] *tb),A[2] * ta + B[2] *tb); 
		n.Normalize();
		result[0]= n.x,result[1]= n.y,result[2]= n.z;
    }


    //! Interpolate cnt values result = A * ta + B * tb
    void InterpolateNormal (float *result, const float *A, float ta, const float *B,float tb, int cnt) 
    { 
        while (cnt >0 ) {
            InterpolateNormal(result,A,ta,B,tb);
            result +=3; A+=3; B+=3; cnt--;
        }
    }

#ifdef _COM
	BEGIN_COM_MAP(GvMFVec3f)
		COM_INTERFACE_ENTRY(EventInMFVec3f)
		COM_INTERFACE_ENTRY(EventOutMFVec3f)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvMFVec3f)

	COM_DEFAULT_SFIELD_METHODS()

    /* EventIn methods */
    STDMETHOD(setValue)(int cnt,float *newValue) 
	{ 		
			cnt /= 3;
			if (cnt != getNum()) makeRoom(cnt);
			memcpy(values,newValue,cnt*3*sizeof(float));
			setDefault(FALSE);
			OnChanged();
			return S_OK; 
	}

    STDMETHOD(set1Value)(int index,float *newValue) 
	{ 
			if (index >=getNum()) makeRoom(index+1);
			set1(index,newValue); 
			return S_OK; 
	}

	/* EventOut methods */ 
    STDMETHOD(getSize)(int *cnt) { *cnt = getNum();  return S_OK; }
	STDMETHOD(getValue)(int cnt,float *v) 
	{ 
			cnt /= 3;
			if (cnt > getNum()) cnt = getNum();
			memcpy(v,get(),cnt*3*sizeof(float)); 
			return S_OK;  
	}
	STDMETHOD(get1Value)(int index,float *v) 
	{ 
			if (index >=getNum()) return S_FALSE;
			float *vi = (*this)[index]; 
			v[0]=vi[0],v[1]=vi[1],v[2]=vi[2]; 
			return S_OK;  
	}
#endif

};



//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//								GvMFVec4f	
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/


class GvMFVec4f : public GvMField 

{

    GV_MFIELD_HEADER(GvMFVec4f,float,4);

  public:
    float *values;


   	operator float *() { return(values); }
    float* operator[] (int i) { return(&values[i*4]);}

	static GvFieldClass getClassFieldType() { return (MFVEC4F); }
	GvFieldClass getFieldType() const { return (MFVEC4F); }
	GvFieldClass getSFFieldType() const { return (SFVEC4F); }

	int io1Value(AStream &a, int index);
	int get1(GvString &value, int index);
	int set1(const char* value, int index);

	int get(PointArray &vals) const;
	GvBool getBBox(BBox &box) const;



	// direct reference 
    const float  *get1(int index) const  { return &values[index*4]; }
    float  *get1(int index) { return &values[index*4]; }

	//Point& get1(int index, Point &value) const;
    
	//Point&  get1Point(int index) { return *((Point *) &values[index*4]);}

    void set1(int index, const float value[4]);
    //void set1(int index, const Point &value);

    const float  *get() const { return values; }

	int set(const PointArray &vals);

	GvField* get1Field(int index);

    // Interpolate 1 value  result = A * ta + B * tb
    void Interpolate (float *result, const float *A, float ta, const float *B,float tb) 
    { 
        result[0] = (A[0] * ta + B[0] *tb); 
        result[1] = (A[1] * ta + B[1] *tb); 
        result[2] = (A[2] * ta + B[2] *tb); 
        result[3] = (A[3] * ta + B[3] *tb); 
    }


    // Interpolate cnt values result = A * ta + B * tb
    void Interpolate (float *result, const float *A, float ta, const float *B,float tb, int cnt) 
    { 
        while (cnt >0 ) {
            Interpolate(result,A,ta,B,tb);
            result +=4; A+=4; B+=4; cnt--;
        }
    }

/*
    // for normal interpolator 
	// to do: is this correct (interpolating on sphere) 
    void InterpolateNormal (float *result, const float *A, float ta, const float *B,float tb)
    { 
        Point n((A[0] * ta + B[0] *tb),(A[1] * ta + B[1] *tb),A[2] * ta + B[2] *tb); 
		n.Normalize();
		result[0]= n.x,result[1]= n.y,result[2]= n.z;
    }


    // Interpolate cnt values result = A * ta + B * tb
    void InterpolateNormal (float *result, const float *A, float ta, const float *B,float tb, int cnt) 
    { 
        while (cnt >0 ) {
            InterpolateNormal(result,A,ta,B,tb);
            result +=4; A+=4; B+=4; cnt--;
        }
    }
*/

};


#endif /* _GV_MF_VEC3F_ */
