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
#ifndef  _GV_MF_FLOAT_
#define  _GV_MF_FLOAT_
/******************************************************************************
@doc

@module GvMFFloat.h - VRML MFFloat field|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvMFFloat>

Notes:

Changes:
$Revision: 1.15 $
$Log: gvmffloat.h,v $
Revision 1.15  1999/07/06 16:35:05  tom
*** empty log message ***

Revision 1.14  1999/05/10 13:53:53  tom
*** empty log message ***



Todo :

******************************************************************************/


#include <gvsubfield.h>

class GvMFFloat : public GvMField 
#ifdef _COM
	,public CComDualImpl<EventInMFFloat, &IID_EventInMFFloat, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutMFFloat, &IID_EventOutMFFloat, &LIBID_GLViewVrmlLib>
#endif


{

    GV_MFIELD_HEADER(GvMFFloat,float,1);

  public:

	float *values;
    

	operator float*() { return(values); }

	static GvFieldClass getClassFieldType() { return (MFFLOAT); }
	GvFieldClass getFieldType() const { return (MFFLOAT); }
	GvFieldClass getSFFieldType() const { return (SFFLOAT); }


    void set1(int index, float value);
    const float  *get() const { return values; }


	float get1(int i) const { 
				if (Length() == 0) { return(0.0); }
				if (i>=num) i = num-1; return(values[i]); 
			}

	int io1Value(AStream &a, int index);
	int get1(GvString &value, int index);
	int set1(const char* value, int index);

    int set(float val);

	int set(const FloatArray &a) { return set(a.Length(),a.Data()); }
	
	GvField* get1Field(int index);


    //GvBool operator==(const GvMFFloat &f) const { return(value == f.value); }

    void Interpolate (float *result, const float *A, float ta, const float *B,float tb) { *result = (*A * ta+ *B *tb); }

    // Interpolate cnt values result = A * ta + B * tb
    void Interpolate (float *result, const float *A, float ta, const float *B,float tb, int cnt) 
    { 
        while (cnt >0 ) {
            Interpolate(result,A,ta,B,tb);
            result ++; A++; B++; cnt--;
        }
    }

#ifdef _COM
	BEGIN_COM_MAP(GvMFFloat)
		COM_INTERFACE_ENTRY(EventInMFFloat)
		COM_INTERFACE_ENTRY(EventOutMFFloat)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvMFFloat)

	COM_DEFAULT_SFIELD_METHODS()

    /* EventIn methods */
    STDMETHOD(setValue)(int cnt,float *newValue) 
	{ 
			if (cnt !=getNum()) makeRoom(cnt);
			memcpy(values,newValue,cnt*1*sizeof(float));
			setDefault(FALSE);
			OnChanged();
			return S_OK; 
	}

    STDMETHOD(set1Value)(int index,float newValue) 
	{ 
			if (index >=getNum()) makeRoom(index+1);
			set1(index,newValue); 
			return S_OK; 
	}

	/* EventOut methods */ 
    STDMETHOD(getSize)(int *cnt) { *cnt = getNum();  return S_OK; }
	STDMETHOD(getValue)(int cnt,float *v) 
	{ 
				if (cnt > getNum()) cnt = getNum();
				memcpy(v,get(),cnt*1*sizeof(float)); 
				return S_OK;  
	}
	STDMETHOD(get1Value)(int index,float *v) 
	{ 
				if (index >=getNum()) return S_FALSE;
				*v = values[index]; 
				return S_OK;  
	}


#endif



};

#endif /* _GV_MF_FLOAT_ */
