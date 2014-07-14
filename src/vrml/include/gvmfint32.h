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
#ifndef  _GV_MFInt32_
#define  _GV_MFInt32_
/******************************************************************************
@doc

@module GvMFInt32.h - VRML 2.0 MFInt32 field|

Copyright (c) 1996	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvMFInt32>

Notes:

Changes:

Todo :

******************************************************************************/


#include <gvsubfield.h>
#include <gvmflong.h>


// simply derived from MFLong


class GvMFInt32 : public GvMFLong 
#ifdef _COM
	,public CComDualImpl<EventInMFInt32, &IID_EventInMFInt32, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutMFInt32, &IID_EventOutMFInt32, &LIBID_GLViewVrmlLib>
#endif

{

        
public:
    RT(GvMFInt32);
    
    GvMFInt32(int cnt=1) : GvMFLong(cnt) {};

	static GvFieldClass getClassFieldType() { return (MFINT32); }
	GvFieldClass getFieldType() const { return (MFINT32); }
	GvFieldClass getSFFieldType() const { return (SFINT32); }

	GvField* get1Field(int index);


#ifdef _COM
	BEGIN_COM_MAP(GvMFInt32)
		COM_INTERFACE_ENTRY(EventInMFInt32)
		COM_INTERFACE_ENTRY(EventOutMFInt32)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvMFInt32)

	COM_DEFAULT_SFIELD_METHODS()

    /* EventIn methods */
    STDMETHOD(setValue)(int cnt,int *newValue) 
	{ 
			if (cnt !=getNum()) makeRoom(cnt);
			memcpy(values,newValue,cnt*1*sizeof(int));
			setDefault(FALSE);
			OnChanged();
			return S_OK; 
	}

    STDMETHOD(set1Value)(int index,int newValue) 
	{ 
			if (index >=getNum()) makeRoom(index+1);
			set1(index,newValue); 
			return S_OK; 
	}

	/* EventOut methods */ 
    STDMETHOD(getSize)(int *cnt) { *cnt = getNum();  return S_OK; }
	STDMETHOD(getValue)(int cnt,int *v) 
	{ 
				if (cnt > getNum()) cnt = getNum();
				memcpy(v,get(),cnt*1*sizeof(int)); 
				return S_OK;  
	}
	STDMETHOD(get1Value)(int index,int *v) 
	{ 
				if (index >=getNum()) return S_FALSE;
				*v = values[index]; 
				return S_OK;  
	}


#endif


};



#endif /* _GV_MFInt32_ */
