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
#ifndef  _GV_MFRotation_
#define  _GV_MFRotation_

#include <gvsubfield.h>

#include <gvsfrotation.h>

class GvMFRotation : public GvMField 
#ifdef _COM
	,public CComDualImpl<EventInMFRotation, &IID_EventInMFRotation, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutMFRotation, &IID_EventOutMFRotation, &LIBID_GLViewVrmlLib>
#endif

{

    GV_MFIELD_HEADER(GvMFRotation,Rotation,1);

  public:
   
   
    Rotation *values;

	operator Rotation  *() { return(values); }
    Rotation& operator[] (int i) { return(values[i]);}

	static GvFieldClass getClassFieldType() { return (MFROTATION); }
	GvFieldClass getFieldType() const { return (MFROTATION); }
	GvFieldClass getSFFieldType() const { return (SFROTATION); }


    const Rotation  *get() const { return values; }
    Rotation& get1(int i) { if (i>=num) i = num-1; return(values[i]); }
    
	void set1(int index, const Rotation &value);
	void set1(int index, const float value[4]) { set1(index,Rotation(value)); }

    // get element as Matrix
    void get1(int i, Matrix &m) { if (i>=num) i = num-1; values[i].get(m);}

	int io1Value(AStream &a, int index);

	int get1(GvString &value, int index);
	int set1(const char* value, int index);

	GvField* get1Field(int index);



    // Interpolate 1 value  result = A * ta + B * tb
    void Interpolate (Rotation *result, const Rotation *A, float ta, const Rotation *B,float tb);

    // Interpolate cnt values result = A * ta + B * tb
    void Interpolate (Rotation *result, const Rotation *A, float ta, const Rotation *B,float tb, int cnt) 
    { 
        while (cnt >0 ) {
            Interpolate(result,A,ta,B,tb);
            result ++; A++; B++; cnt--;
        }
    }

#ifdef _COM
	BEGIN_COM_MAP(GvMFRotation)
		COM_INTERFACE_ENTRY(EventInMFRotation)
		COM_INTERFACE_ENTRY(EventOutMFRotation)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvMFRotation)

	COM_DEFAULT_SFIELD_METHODS()

    /* EventIn methods */
    STDMETHOD(setValue)(int cnt,float *newValue) 
	{ 
			cnt /= 4;
			if (cnt != getNum()) makeRoom(cnt);
			for(int i=0; i<cnt;i++)
				values[i].set(&newValue[i*4]);
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
				cnt /= 4;
				if (cnt > getNum()) cnt = getNum();
				for(int i=0; i<cnt;i++)	{
					Rotation &vi = (*this)[i]; 
					*v++ = vi[0];
					*v++ = vi[1];
					*v++ = vi[2];
					*v++ = vi[3];
				}
				return S_OK;  
	}
	STDMETHOD(get1Value)(int index,float *v) 
	{ 
				if (index >=getNum()) return S_FALSE;
				Rotation &vi = (*this)[index]; 
				v[0]=vi[0],v[1]=vi[1],v[2]=vi[2],v[3]=vi[3]; 
				return S_OK;  
	}
#endif


};

#endif /* _GV_MFRotation_ */
