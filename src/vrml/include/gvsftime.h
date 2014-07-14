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
#ifndef  _GV_SF_TIME_
#define  _GV_SF_TIME_
/******************************************************************************
@doc

@module GvSFTime.h - VRML 2.0 SFTime Field|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvSFTime>

Notes:

Changes:
$Revision: 1.15 $
$Log: gvsftime.h,v $
Revision 1.15  1999/07/06 16:35:26  tom
*** empty log message ***

Revision 1.14  1999/05/10 13:54:18  tom
*** empty log message ***


18.10.98 HG default value is -1


Todo :

******************************************************************************/

#include <gvsubfield.h>


class GvSFTime : public GvSField
#ifdef _COM
	,public CComDualImpl<EventInSFTime, &IID_EventInSFTime, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutSFTime, &IID_EventOutSFTime, &LIBID_GLViewVrmlLib>
#endif


 {
  public:

    double value;

    GV_SFIELD_HEADER(GvSFTime);
    
    GvSFTime(double defaultValue=-1.0) : value(defaultValue) {}

	static GvFieldClass getClassFieldType() { return (SFTIME); }
	GvFieldClass getFieldType() const { return (SFTIME); }


	//operator double&() { return value; }
	operator double() const { return value; }

	void get(double &Value) const { Value = value; }
	double get() const { return( value);}
    void set(double newValue) { value = newValue; setDefault(FALSE);  OnChanged(); }

	int ioValue(AStream &a); 
	int get(GvString &value);
	int set(const char* value);


    const GvSFTime& operator= (const GvSFTime &f) { set(f.value); return(f); }
    GvBool operator==(const GvSFTime &f) const { return(value == f.value); }

#ifdef _COM
	BEGIN_COM_MAP(GvSFTime)
		COM_INTERFACE_ENTRY(EventInSFTime)
		COM_INTERFACE_ENTRY(EventOutSFTime)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvSFTime)

	COM_DEFAULT_SFIELD_METHODS()

    /* EventIn methods */
    STDMETHOD(setValue)(double  newValue) { set(newValue); COM_EXTERN_TOUCH(); return S_OK; }

	/* EventOut methods */ 
	STDMETHOD(getValue)(double *v) { *v=get(); return S_OK;  }


#endif

};

#endif /* _GV_SF_TIME_ */
