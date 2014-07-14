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
#ifndef  _GV_MF_COLOR_
#define  _GV_MF_COLOR_

#include <gvsubfield.h>

class PointArray;
class Point;

class AStream;

class GvMFColor : public GvMField
#ifdef _COM
	,public CComDualImpl<EventInMFColor, &IID_EventInMFColor, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutMFColor, &IID_EventOutMFColor, &LIBID_GLViewVrmlLib>
#endif

 {
    GV_MFIELD_HEADER(GvMFColor,float,3);

  public:
    float *values;			// 3 per color

	
	operator float* () { return(values);}
    float* operator[] (int i) { return(&values[i*3]);}

	static GvFieldClass getClassFieldType() { return (MFCOLOR); }
	GvFieldClass getFieldType() const { return (MFCOLOR); }
	GvFieldClass getSFFieldType() const { return (SFCOLOR); }


	
	// get as 4 component vector
	float * GetRGBA(float *rgba,float a=1.0) { 
		if (Length() == 0) {
			rgba[0]=rgba[1]=rgba[2]=0.0,rgba[3]=a; 
    		return (rgba);

		}
		rgba[0]=values[0],rgba[1]=values[1],
		rgba[2]=values[2],rgba[3]=a; 
		return (rgba);
	}

	float * GetRGBA(int i,float *rgba,float a=1.0) { 
		if (Length() == 0) {
			rgba[0]=rgba[1]=rgba[2]=0.0,rgba[3]=a; 
    		return (rgba);
		}

		if (i>=num) i = num-1;
		i*=3;
		rgba[0]=values[i+0],rgba[1]=values[i+1],
		rgba[2]=values[i+2],rgba[3]=a; 
		return (rgba);
	}



    // int set(int count, const float * newValues); in  declard MFIELD_HEADER
	int set(const PointArray &vals);
	int set(const Point val);

    void set1(int index, const float value[3]);

    const float  *get() const  { return values; }
	int get(PointArray &vals) const;

	Point& get1(int index, Point &color) const;
    const float  *get1(int index) const  { return &values[index*3]; }


	int io1Value(AStream &a, int index);
	int get1(GvString &value, int index);
	int set1(const char* value, int index);
	
	GvField* get1Field(int index);
	
	GvBool operator==(const GvMFColor &f) const; 




    void Interpolate (float *result, const float *A, float ta, const float *B,float tb) 
    { 
        result[0] = (A[0] * ta + B[0] *tb); 
        result[1] = (A[1] * ta + B[1] *tb); 
        result[2] = (A[2] * ta + B[2] *tb); 
    }

    void InterpolateHsv (float *result, const float *A, float ta, const float *B,float tb); 

    // Interpolate cnt values result = A * ta + B * tb
    void Interpolate (float *result, const float *A, float ta, const float *B,float tb, int cnt) 
    { 
        while (cnt >0 ) {
            Interpolate(result,A,ta,B,tb);
            result +=3; A+=3; B+=3; cnt--;
        }
    }

    // Interpolate cnt values result = A * ta + B * tb
    void InterpolateHsv (float *result, const float *A, float ta, const float *B,float tb, int cnt) 
    { 
        while (cnt >0 ) {
            InterpolateHsv(result,A,ta,B,tb);
            result +=3; A+=3; B+=3; cnt--;
        }
    }

#ifdef _COM
	BEGIN_COM_MAP(GvMFColor)
		COM_INTERFACE_ENTRY(EventInMFColor)
		COM_INTERFACE_ENTRY(EventOutMFColor)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvMFColor)

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

#endif /* _GV_MF_COLOR_ */
