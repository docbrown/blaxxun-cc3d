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
#ifndef  _GV_SF_LONG_
#define  _GV_SF_LONG_
/******************************************************************************
@doc

@module GvSFLong.h - VRML 1.0 SFLong field|

Copyright (c) 1996	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvSFLong>

Notes:
parts based on QvLib from SGI

Changes:

setChecked() assign only if changed

Todo :

******************************************************************************/


#include <gvsubfield.h>

class GvSFLong : public GvSField {
  public:
    long value;
    
    GV_SFIELD_HEADER(GvSFLong);
    
    GvSFLong(long defaultValue=0) : value(defaultValue) {}

	static GvFieldClass getClassFieldType() { return (SFLONG); }
	GvFieldClass getFieldType() const { return (SFLONG); }

	
	operator long&() { return value; }
    long get() const { return value;}

	void set(long newValue) { value = newValue; setDefault(FALSE); OnChanged(); }
	void set(int newValue) { value = newValue; setDefault(FALSE); OnChanged(); }

	void setChecked(long newValue) { if (value != newValue) { value = newValue; setDefault(FALSE); OnChanged(); } }

    void get(long &Value) const { Value = value; }

    const GvSFLong& operator= (const GvSFLong &f) { set(f.value);  return(f); }
    GvBool operator==(const GvSFLong &f) const { return(value == f.value); }

	int ioValue(AStream &a); 
	int get(GvString &value);
	int set(const char* value);

};

#endif /* _GV_SF_LONG_ */
