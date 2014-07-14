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
#ifndef  _GV_MF_VEC2F_
#define  _GV_MF_VEC2F_

#include <gvsubfield.h>

class PointArray;
class AStream;
class GvSFVec2f;

class GvMFVec2f : public GvMField 
#ifdef _COM
	,public CComDualImpl<EventInMFVec2f, &IID_EventInMFVec2f, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutMFVec2f, &IID_EventOutMFVec2f, &LIBID_GLViewVrmlLib>
#endif

{

    GV_MFIELD_HEADER(GvMFVec2f,float,2);
	

  public:
    
    float *values;

   	operator float *() { return(values); }
    float* operator[] (int i) { return(&values[i*2]);}

	static GvFieldClass getClassFieldType() { return (MFVEC2F); }
	GvFieldClass getFieldType() const { return (MFVEC2F); }
	GvFieldClass getSFFieldType() const { return (SFVEC2F); }


    const float  *get() const { return values; }
	int get(PointArray &vals) const;

    const float  *get1(int index) const  { return &values[index*2]; }
    
	float  *get1(int index) { return &values[index*2]; }
	
	Point& get1(int index, Point &value) const;

	int set(const PointArray &vals);
    void set1(int index, const float value[2]);
    void set1(int index, const Point &value);



	int io1Value(AStream &a, int index);
	int get1(GvString &value, int index);
	int set1(const char* value, int index);

	GvField* get1Field(int index);

    //const GvMFVec2f& operator= (const GvMFVec2f &f) { set(f.value); return(f); } 
    
	GvBool operator==(const GvMFVec2f &f) const 
	{
			int cnt = getNum();
			if (f.getNum() != cnt) return gfalse;
			cnt*=2;
			const float *a=values;
			const float *b=f.values;

	        while (cnt >0 ) {
				cnt--;
				if (*a != *b) return gfalse;
				a++;b++;
				cnt--;
			};
			return gtrue;
	}


    void Interpolate (float *result, const float *A, float ta, const float *B,float tb) 
    { 
        result[0] = (A[0] * ta + B[0] *tb); 
        result[1] = (A[1] * ta + B[1] *tb); 
    }

    // Interpolate cnt values result = A * ta + B * tb
    void Interpolate (float *result, const float *A, float ta, const float *B,float tb, int cnt) 
    { 
        while (cnt >0 ) {
            Interpolate(result,A,ta,B,tb);
            result+=2;
            A+=2;
            B+=2;
            cnt--;
        }
    }

#ifdef _COM
	BEGIN_COM_MAP(GvMFVec2f)
		COM_INTERFACE_ENTRY(EventInMFVec2f)
		COM_INTERFACE_ENTRY(EventOutMFVec2f)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvMFVec2f)

	COM_DEFAULT_SFIELD_METHODS()

    /* EventIn methods */
    STDMETHOD(setValue)(int cnt,float *newValue) 
	{ 
			cnt /= 2;
			if (cnt != getNum()) makeRoom(cnt);
			memcpy(values,newValue,cnt*2*sizeof(float));
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
				cnt/=2;
				if (cnt > getNum()) cnt = getNum();
				memcpy(v,get(),cnt*2*sizeof(float)); 
				return S_OK;  
	}
	STDMETHOD(get1Value)(int index,float *v) 
	{ 
				if (index >=getNum()) return S_FALSE;
				float *vi = (*this)[index]; 
				v[0]=vi[0],v[1]=vi[1]; 
				return S_OK;  
	}
#endif

};

#endif /* _GV_MF_VEC2F_ */
