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
#ifndef  _GV_SFInt32_
#define  _GV_SFInt32_
/******************************************************************************
@doc

@module GvSFFInt32.h - VRML 2.0 SFInt32 field|

Copyright (c) 1996 - 1999	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvSFInt32>

Notes:

Changes:

Todo :

******************************************************************************/


#include <gvsubfield.h>
#include <gvsflong.h>



//
// GvSFInt32
// 
// simply derived from SFLong


class GvSFInt32 : public GvSFLong 

#ifdef _COM
	,public CComDualImpl<EventInSFInt32, &IID_EventInSFInt32, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutSFInt32, &IID_EventOutSFInt32, &LIBID_GLViewVrmlLib>
#endif
{

        
public:
    RT(GvSFInt32);

    GvSFInt32(long defaultValue=0) : GvSFLong(defaultValue) {};

	static GvFieldClass getClassFieldType() { return (SFINT32); }
	GvFieldClass getFieldType() const { return (SFINT32); }


#ifdef _COM
	BEGIN_COM_MAP(GvSFInt32)
		COM_INTERFACE_ENTRY(EventInSFInt32)
		COM_INTERFACE_ENTRY(EventOutSFInt32)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvSFInt32)

	COM_DEFAULT_SFIELD_METHODS()

    /* EventIn methods */
    STDMETHOD(setValue)(int  newValue) { set(newValue); COM_EXTERN_TOUCH(); return S_OK; }

	/* EventOut methods */ 
	STDMETHOD(getValue)(int *v) { *v=get(); return S_OK;  }


#endif




};





#endif /* _GV_SFInt32_ */
