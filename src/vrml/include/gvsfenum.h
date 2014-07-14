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
#ifndef  _GV_SF_ENUM_
#define  _GV_SF_ENUM_

#include <gvstring.h>
#include <gvsubfield.h>

class GvSFEnum : public GvSField {
  public:
    int value;

    GV_SFIELD_HEADER(GvSFEnum);

    GvSFEnum(int defaultValue=0);

	static GvFieldClass getClassFieldType() { return (SFENUM); }
	GvFieldClass getFieldType() const { return (SFENUM); }

    // Sets up value/name correspondances
    void setEnums(int num, const int vals[], const GvName names[])
	{ numEnums = num; enumValues = vals; enumNames = names; }

    int			numEnums;	// Number of enumeration values
    const int		*enumValues;	// Enumeration values
    const GvName	*enumNames;	// Mnemonic names of values

    // Looks up enum name, returns value. Returns FALSE if not found.
    GvBool		findEnumValue(const GvName &name, int &val) const;

	// get string for enum value
	const char * Get() const;
    GvBool Set(const char* name);

    int get() const  { return value;}
	void get(int& i) const { i = value;}
	void set(int i) { value = i; setDefault(FALSE); OnChanged(); }

    const GvSFEnum& operator= (const GvSFEnum &f) { value = f.value; setDefault(FALSE); OnChanged(); return(f); }

    GvBool operator==(int f) const { return(value == f); }
    GvBool operator==(const GvSFEnum &f) const { return(value == f.value); }


	//operator int() const { return(value); }
	operator int&() { return(value); }
	
	int ioValue(AStream &a);
	int get(GvString &value);
	int set(const char* value);


};

#define	GV_NODE_SET_SF_ENUM_TYPE(fieldName, enumType)			      \
    do {								      \
	int _so_sf_enum_num;						      \
	const int *_so_sf_enum_vals;					      \
	const GvName *_so_sf_enum_names;				      \
	fieldData->getEnumData(GV__QUOTE(enumType),			      \
			       _so_sf_enum_num,				      \
			       _so_sf_enum_vals,			      \
			       _so_sf_enum_names);			      \
	fieldName.setEnums(_so_sf_enum_num,				      \
			   _so_sf_enum_vals,				      \
			   _so_sf_enum_names);				      \
    } while (0)

#endif /* _GV_SF_ENUM_ */
