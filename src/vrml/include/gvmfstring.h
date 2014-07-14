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
#ifndef  _GV_MF_STRING_
#define  _GV_MF_STRING_

#include <gvsubfield.h>

class GvMFString : public GvMField 
#ifdef _COM
	,public CComDualImpl<EventInMFString, &IID_EventInMFString, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutMFString, &IID_EventOutMFString, &LIBID_GLViewVrmlLib>
#endif

{
  public:

    GvString *values; // array of pointer to strings 

    GV_MFIELD_HEADER(GvMFString,GvString,1);

	static GvFieldClass getClassFieldType() { return (MFSTRING); }
	GvFieldClass getFieldType() const { return (MFSTRING); }
	GvFieldClass getSFFieldType() const { return (SFSTRING); }

	// set(string) conflicts with GvField::set(char)

	void set_(const GvString &Value) { 
		makeRoom(1);
		set1(0,Value); 
	}

	void set_(const char *Value) { 
		makeRoom(1);
		set1(0,Value); 
	}


    void set1(int index, const GvString &newValue);

	const GvString *get() const  { return values; }

	operator GvString *() { return(values); }

	int io1Value(AStream &a, int index);
	int get1(GvString &value, int index);
	int set1(const char* value, int index);

	GvString &get1(int i) { if (i>=num) i = num-1; return(values[i]); }

	GvField* get1Field(int index);
    GvBool operator==(const GvMFString &f) const ;



#ifdef _COM
	BEGIN_COM_MAP(GvMFString)
		COM_INTERFACE_ENTRY(EventInMFString)
		COM_INTERFACE_ENTRY(EventOutMFString)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvMFString)

	COM_DEFAULT_SFIELD_METHODS()

    /* EventIn methods */
    STDMETHOD(setValue)(int cnt,BSTR *newValue) 
	{ 
			if (cnt !=getNum()) makeRoom(cnt);
			for(int i=0; i<cnt;i++)
				values[i] = newValue[i];

			setDefault(FALSE);
			OnChanged();
			return S_OK; 
	}

    STDMETHOD(set1Value)(int index,BSTR newValue) 
	{ 
			if (index >=getNum()) makeRoom(index+1);
			set1(index,newValue); 
			return S_OK; 
	}

	/* EventOut methods */ 
    STDMETHOD(getSize)(int *cnt) { *cnt = getNum();  return S_OK; }
	STDMETHOD(getValue)(int cnt,BSTR *v) 
	{ 
				if (cnt > getNum()) cnt = getNum();
				for(int i=0; i<cnt;i++)
					v[i] = values[i].SysAllocString();

				return S_OK;  
	}

	STDMETHOD(get1Value)(int index,BSTR *v) 
	{ 
				if (index >=getNum()) return S_FALSE;
				*v = values[index].SysAllocString(); 
				return S_OK;  
	}


#endif


};

#endif /* _GV_MF_STRING_ */
