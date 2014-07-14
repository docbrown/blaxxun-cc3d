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
#ifndef  _GV_SUB_FIELD_
#define  _GV_SUB_FIELD_

#include <gvfield.h>
#include <gvinput.h>

// HG RT
/////////////////////////////////////////////////////////////////////////////

#define GV_SFIELD_HEADER(className)					      \
  public:								      \
    virtual ~className();						      \
    virtual GvBool readValue(GvInput *in);	    \
    virtual const GvField& operator = (const GvField &f); \
	RT(className)				

/////////////////////////////////////////////////////////////////////////////

#define GV_MFIELD_HEADER(className, valueType,numValues)					      \
  public:								      \
    className(int cnt=1);							      \
    virtual ~className();						      \
    virtual GvBool	read1Value(GvInput *in, int index);      \
    void		   allocValues(int newNum);	\
    virtual const GvField& operator = (const GvField &f);  \
    int set(int newNum,const valueType *newValues);   \
    int set(const className &f) { set(f.num,f.values); return(num);} \
    const className & operator= (const className &f) { set(f.num,f.values); return(f);} \
	void set1(int index, GvField *f); \
	RT(className);				


#define GV_MFIELD_HEADER_EQ(className, valueType)     \
    int set(int newNum,const valueType *newValues);   \
	int set(const className &f) { set(f.num,f.values); return(num);} \
    const className & operator= (const className &f) { set(f.num,f.values); return(f); } \
	void set1(int index, GvField *f); 

/////////////////////////////////////////////////////////////////////////////


#define GV_SFIELD_SOURCE(className)					      \
									      \
RTIMP(className,0,GvSField)				\
className::~className()							      \
{									      \
}                                                           \
\
const GvField & className::operator=(const GvField &f) { \
    *this = * (const className *) &f;   \
    return(f);  \
}   \


#define GV_SFIELD_SOURCE_EQ(className)    \
const GvField & className::operator=(const GvField &f) { \
    *this = * (const className *) &f;   \
    return(f);  \
}   \

#define GV_MFIELD_SOURCE_EQ_SF(className, valueType)     \
void className::set1(int index, GvField *f)	\
{ set1(index, ((valueType *)f)->value); } 




/////////////////////////////////////////////////////////////////////////////

#define GV_MFIELD_SOURCE(className, valueType, numValues)		      \
									      \
RTIMP(className,0,GvMField)				\
className::className(int cnt)							      \
{									      \
    values = NULL;							      \
    /* Make room for cnt values to start */				      \
    allocValues(cnt);							      \
}									      \
									      \
className::~className()							      \
{									      \
    if (values != NULL)							      \
	    FREE((char *) values);						      \
}									      \
									      \
void									      \
className::allocValues(int newNum)					      \
{									      \
    if (values == NULL) {						      \
	if (newNum > 0)							      \
	    values = (valueType *)					      \
		MALLOC(numValues * sizeof(valueType) * newNum);		      \
    }									      \
    else {								      \
	if (newNum > 0)							      \
	    values = (valueType *)					      \
		REALLOC(values, numValues * sizeof(valueType) * newNum);      \
	else {								      \
	    FREE(values);					      \
	    values = NULL;						      \
	}								      \
    }									      \
    num = maxNum = newNum;						      \
}\
const GvField & className::operator=(const GvField &f) { \
    *this = * (const className *) &f;   \
    return(f);  \
}   \
int className::set(int newNum,const valueType *newValues) { allocValues(newNum);  \
		memcpy(values,newValues,num*numValues*sizeof(valueType)); \
		setDefault(FALSE); OnChanged(); \
		return(num>0);}


#define GV_MFIELD_SOURCE_EQ_FIELD(className)    \
const GvField & className::operator=(const GvField &f) { \
    *this = * (const className *) &f;   \
    return(f);  \
}   


#define GV_MFIELD_SOURCE_EQ(className, valueType, numValues)		      \
int className::set(int newNum,const valueType *newValues) { allocValues(newNum); \
memcpy(values,newValues,num*numValues*sizeof(valueType)); \
setDefault(FALSE); OnChanged(); \
return(num>0);}




#define GV_MFIELD_SOURCE_0(className, valueType, numValues)		      \
									      \
RTIMP(className,0,GvMField)				\
void									      \
className::allocValues(int newNum)					      \
{									      \
    if (values == NULL) {						      \
	if (newNum > 0)							      \
	    values = (valueType *)					      \
		MALLOC(numValues * sizeof(valueType) * newNum);		      \
    }									      \
    else {								      \
	if (newNum > 0)							      \
	    values = (valueType *)					      \
		REALLOC(values, numValues * sizeof(valueType) * newNum);      \
	else {								      \
	    FREE(values);					      \
	    values = NULL;						      \
	}								      \
    }									      \
    num = maxNum = newNum;						      \
}



#endif /* _GV_SUB_FIELD_ */
