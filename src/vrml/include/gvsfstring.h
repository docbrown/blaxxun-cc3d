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
#ifndef  _GV_SF_STRING_
#define  _GV_SF_STRING_

#include <gvsubfield.h>

class GvSFString : public GvSField 
#ifdef _COM
	,public CComDualImpl<EventInSFString, &IID_EventInSFString, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutSFString, &IID_EventOutSFString, &LIBID_GLViewVrmlLib>
#endif

{
  public:

    GvString value;

    GV_SFIELD_HEADER(GvSFString);

    GvSFString() {}
    GvSFString(const char *defaultValue) : value(defaultValue) {}
    GvSFString(const GvString &defaultValue) : value(defaultValue) {}

	static GvFieldClass getClassFieldType() { return (SFSTRING); }
	GvFieldClass getFieldType() const { return (SFSTRING); }

	operator const char *() const { return(value.getString()); }
    const GvString& get() const { return value;}

	void set(const GvString &Value) { value = Value; setDefault(FALSE); OnChanged(); }
	void set_(const char * Value);


	int ioValue(AStream &a);
	// to string, from string 
	int get(GvString &value);
	int set(const char* value);
    
    const GvSFString& operator= (const GvSFString &f) { value = f.value;  setDefault(FALSE); OnChanged(); return(f); }  
    const char * operator= (const char *s) { value = s; setDefault(FALSE); OnChanged();  return(s); }  
    
    GvBool operator==(const GvSFString &f) const { return(value == f.value); }
    GvBool operator==(const char *s) const { return(value == s); }

#ifdef _COM
	BEGIN_COM_MAP(GvSFString)
		COM_INTERFACE_ENTRY(EventInSFString)
		COM_INTERFACE_ENTRY(EventOutSFString)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvSFString)

	COM_DEFAULT_SFIELD_METHODS()

    /* EventIn methods */
    STDMETHOD(setValue)(BSTR newValue);

	/* EventOut methods */ 
	STDMETHOD(getValue)(BSTR *v);


#endif




};

#endif /* _GV_SF_STRING_ */
