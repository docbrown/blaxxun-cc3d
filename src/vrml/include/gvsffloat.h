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
#ifndef  _GV_SF_FLOAT_
#define  _GV_SF_FLOAT_
/******************************************************************************
@doc

@module GvSFFloat.h - VRML 1.0/2.0 SFFloat Field|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvSFFloat>

Notes:
parts based on QvLib from SGI


Changes:
$Revision: 1.15 $
$Log: gvsffloat.h,v $
Revision 1.15  1999/05/10 13:54:15  tom
*** empty log message ***

Revision 1.13  1999/03/10 10:26:57  tom
*** empty log message ***


setChecked() assign only if changed


Todo :

******************************************************************************/


#include <gvsubfield.h>

class GvSFFloat : 
	public GvSField
#ifdef _COM
	,public CComDualImpl<EventInSFFloat, &IID_EventInSFFloat, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutSFFloat, &IID_EventOutSFFloat, &LIBID_GLViewVrmlLib>
#endif

{

  public:
    float value;		// the value 

    GV_SFIELD_HEADER(GvSFFloat);

    GvSFFloat(float defaultValue=0.0) : value(defaultValue) {}

	static GvFieldClass getClassFieldType() { return (SFFLOAT); }
	GvFieldClass getFieldType() const { return (SFFLOAT); }


//	operator float() const { return value; }
	operator float&() { return value; }
    float get() const  { return value;}
	void get(float &Value) const { Value = value; }

    void set(float newValue) { value = newValue; setDefault(FALSE);  OnChanged();  }
    void setChecked(float newValue) { if (value != newValue) {  value = newValue; setDefault(FALSE);  OnChanged(); } }

	//const char * printfImage = "%g";

	int ioValue(AStream &a); 
	int get(GvString &value);
	int set(const char* value);

    const GvSFFloat& operator= (const GvSFFloat &f) { set(f.value); return(f); }
    GvBool operator==(const GvSFFloat &f) const { return(value == f.value); }
    GvBool operator==(float f) const { return(value == f); }

#ifdef _COM
	BEGIN_COM_MAP(GvSFFloat)
		COM_INTERFACE_ENTRY(EventInSFFloat)
		COM_INTERFACE_ENTRY(EventOutSFFloat)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvSFFloat)

    /* IUnknown methods */
	STDMETHOD_(ULONG, AddRef)() { return InternalAddRef();  }
	STDMETHOD_(ULONG, Release)() { return InternalRelease();  }
	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
	{return _InternalQueryInterface(iid, ppvObject);}

    /* EventIn methods */
    STDMETHOD(getType)(THIS_ int FAR* type) { return GvField::getType(type); }
	STDMETHOD(setValueFromString)( /* [in] */ BSTR value) { return GvField::setValueFromString(value); } 

    STDMETHOD(setValue)(float value) { set(value); return S_OK; }

	/* EventOut methods */ 
    STDMETHOD(advise)(EventOutObserver *observer, IDispatch* userData) { return GvSField::advise(observer,userData); }
    STDMETHOD(unAdvise)(EventOutObserver *observer) { return GvSField::unAdvise(observer); }
	STDMETHOD(toString) (/* [retval][out] */ BSTR  *value) { return GvField::toString(value); } 

	STDMETHOD(getValue)(float *value) { *value = get(); return S_OK;  }

#endif



};

#endif /* _GV_SF_FLOAT_ */
