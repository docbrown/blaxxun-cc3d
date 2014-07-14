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
#ifndef  _GV_SF_BOOL_
#define  _GV_SF_BOOL_
/******************************************************************************
@doc

@module GvSFBool.h - VRML SFBool field|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvSFBool>

Notes:

Changes:
$Revision: 1.18 $
$Log: gvsfbool.h,v $
Revision 1.18  1999/07/06 16:35:22  tom
*** empty log message ***

Revision 1.17  1999/05/10 13:54:13  tom
*** empty log message ***


setChecked() assign only if changed
18.10.98 HG default value is FALSE !

Todo :

******************************************************************************/


#include <gvsubfield.h>

class GvSFBool : public GvSField 
#ifdef _COM
	,public CComDualImpl<EventInSFBool, &IID_EventInSFBool, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutSFBool, &IID_EventOutSFBool, &LIBID_GLViewVrmlLib>
#endif

{
  public:
    GvBool value;
    GV_SFIELD_HEADER(GvSFBool);

    GvSFBool(GvBool defaultValue=FALSE) : value(defaultValue) {}

	static GvFieldClass getClassFieldType() { return (SFBOOL); }
	GvFieldClass getFieldType() const { return (SFBOOL); }


	operator GvBool() const { return(value); }
	operator GvBool&() { return value; }

    GvBool get() const  { return value; }
	
    const GvSFBool& operator= (const GvSFBool &f) { set(f.value); return(f); }
    GvBool operator==(const GvSFBool &f) const { return(value == f.value); }
    GvBool operator==(GvBool f) const { return(value == f); }

	void get(GvBool &Value) const { Value = value; }

    void set(GvBool newValue) { value = newValue; setDefault(FALSE);  OnChanged(); }
    void set(int newValue) { set(newValue !=0); } // convert int to GvBool
    void setChecked(GvBool newValue) { if (value != newValue) {  value = newValue; setDefault(FALSE);  OnChanged(); } }

	int ioValue(AStream &a); 
    int get(GvString &value);
	int set(const char* value);

#ifdef _COM
	BEGIN_COM_MAP(GvSFBool)
		COM_INTERFACE_ENTRY(EventInSFBool)
		COM_INTERFACE_ENTRY(EventOutSFBool)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvSFBool)

	COM_DEFAULT_SFIELD_METHODS()

    /* EventIn methods */
    STDMETHOD(setValue)(VARIANT_BOOL  newValue) { set((newValue? TRUE: FALSE)); COM_EXTERN_TOUCH(); return S_OK; }

	/* EventOut methods */ 
	STDMETHOD(getValue)(VARIANT_BOOL *v) { *v= (get() ? VARIANT_TRUE : VARIANT_FALSE); return S_OK;  }


#endif

};

#endif /* _GV_SF_BOOL_ */
