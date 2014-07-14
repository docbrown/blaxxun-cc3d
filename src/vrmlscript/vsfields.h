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
#ifndef _VS_Fields_
#define _VS_Fields_


#include "vsexpr.h"

#include <rt.h>

#include <mallocdbg.h>

#include <gvfields.h>

//typedef GvFieldClass VsFieldClass;
typedef int VsFieldClass;


// 
#define setHasBeenSet setModified



class VsField : public VsVar {
  public:
    enum Permission {
		RW, // For eventOuts/fields
        RO, // For eventIns
    };

    //! Creation method, given a field type Does The Right
    // Thing.
    static VsField * create(VsFieldClass t, Permission p,GvField *field,int byRef=1);

	VsField(GvField *f=NULL,Permission p =RW, int byRef=1);

	//! get the field type 
	virtual VsFieldClass getFieldType() const = 0;

	//! get the field, note: not virtual 
	GvField *getField()  { return field; }

	//! get the length if viewed as array 
	virtual int getLength() { return(1); }


	void setField(GvField *f,int byRef=1);

	//! evalutes to field 
	virtual void evaluate(VsValue &ret,VsFunctionCall *);

	//! evaluates to this 
	VsVar* evaluateVar(VsFunctionCall *, VsValue &value);

	//! evalutes to nothing
	virtual void evaluate(VsValue &ret,int32_t index, VsFunctionCall *);

	virtual VsValue evaluateMember(const VsMember &member, VsFunctionCall *fcall);
	virtual VsValue assignMember(const VsMember &member, const VsValue &v, VsFunctionCall *fcall);

	//! invoke member function , supported toString
	virtual VsValue callMember(const VsMember &member,VsExprList *args, VsFunctionCall *fcall);


	//! assign if v evaluates to same field type 
	virtual VsValue assign(const VsValue &v, VsFunctionCall *);

	// do nothing 
	virtual VsValue		assign(const VsValue &, int32_t index, VsFunctionCall *);

	//< new function - constructor init call 
	virtual VsValue initCall(VsFunctionCall *parent,VsExprList *args);




  protected:

    Permission permission;
	int byRef;			//!< if false, field is alloced and must be destroyed 

	GvField *field;		//!< the field used by this VsField

    virtual ~VsField();

};

class VsMField : public VsField
{
public:

		VsMField(GvField *field=NULL,Permission perm=RW,int byRef=1) : VsField(field,perm,byRef) { }
		GvMField*	getField() { return (GvMField*) field; }

		int getLength() { return( getField()->Length()); }
		
		void setLength(int l) { getField()->makeRoom(l); }

		void checkLength(int l) { if (l>=getField()->Length()) 
										getField()->makeRoom(l); 
								}
		VsFieldClass getSFieldType() { return	getField()->getSFFieldType(); }

		virtual void		evaluate(VsValue &ret,int32_t index, VsFunctionCall *);
		virtual VsVar*		evaluateVar(int32_t index, VsFunctionCall *);

		virtual VsValue		assign(const VsValue &, int32_t index, VsFunctionCall *);

		// member length 
		VsValue evaluateMember(const VsMember &member, VsFunctionCall *fcall);
		VsValue assignMember(const VsMember &member, const VsValue &v, VsFunctionCall *fcall);
		
		// constructor init call 
		VsValue initCall(VsFunctionCall *parent,VsExprList *args);


};


class VsSFInt32 : public VsField {
  public:

	VsSFInt32(GvField *field=NULL,Permission p=RW,int byRef=1);

    virtual void		evaluate(VsValue &ret,VsFunctionCall *);
    virtual VsValue		assign(const VsValue &, VsFunctionCall *);
    
	VsFieldClass getFieldType() const  { return  SFINT32; }
	GvSFInt32*	getField() { return (GvSFInt32*) field; }

	static VsField *newField(GvField *field=NULL,Permission p=RW,int byRef=1) {
			if (field == NULL && !byRef) return new VsSFInt32( new GvSFInt32(),p,0);
			else return new VsSFInt32( field,p,byRef);
	}

};

//	VS_SFIELD_HEADER(VsSFString,GvSFString,SFSTRING,GvString &)
//	RT(CLASSTYPE); 

#define VS_SFIELD_HEADER(CLASSTYPE,GVCLASS,TYPEDID,VALTYPE) \
	public : \
	CLASSTYPE(GvField *field=NULL,Permission perm=RW,int byRef=1) : VsField(field,perm,byRef) { }\
	VsFieldClass getFieldType() const  { return  TYPEDID; } \
	GVCLASS*	getField() { return (GVCLASS*) field; } \
	static VsField *newField(GvField *field=NULL,Permission p=RW,int byRef=1) { \
			if (field == NULL && !byRef) return new CLASSTYPE(new GVCLASS(),p,0); \
			else return new CLASSTYPE( field,p,byRef); \
	}

#define VS_MFIELD_HEADER(CLASSTYPE,GVCLASS,TYPEDID,VALTYPE) \
	public : \
	CLASSTYPE(GvField *field=NULL,Permission perm=RW,int byRef=1) : VsMField(field,perm,byRef) { }\
    VsValue		assign(const VsValue &, int32_t index, VsFunctionCall *); \
    void evaluate(VsValue &ret,int32_t index, VsFunctionCall *); \
	VsFieldClass getFieldType() const  { return  TYPEDID; } \
	GVCLASS*	getField() { return (GVCLASS*) field; } \
	static VsField *newField(GvField *field=NULL,Permission p=RW,int byRef=1) { \
			if (field == NULL && !byRef) return new CLASSTYPE(new GVCLASS(0),p,0); \
			else return new CLASSTYPE( field,p,byRef); \
	}


class VsSFNode : public VsField {
	VS_SFIELD_HEADER(VsSFNode,GvSFNode,SFNODE, GvNode*)

public:
	// alternate constructors 
	VsSFNode(GvNode *n,Permission perm=RW) : VsField(new GvSFNode(n),perm,0) {};


	void evaluate(VsValue &ret,VsFunctionCall *); 

	// if != NULL
	GvBool evaluateBool();

    VsValue		assign(const VsValue &, VsFunctionCall *); 
	// assign, evaluate evenIns/Outs 
	VsValue		assignMember(const VsMember &member, const VsValue &v, VsFunctionCall *fcall);

	VsValue		evaluateMember(const VsMember &member, VsFunctionCall *fcall);

	// get Var representing the node's EventOut 
	VsVar* evaluateMemberVar(const VsMember &member, VsFunctionCall *fcall);

	// constructor init call 
	VsValue initCall(VsFunctionCall *parent,VsExprList *args);

	// invoke member function  of browser 
	VsValue callMember(const VsMember &member,VsExprList *args, VsFunctionCall *);


};

class VsSFBool : public VsField {
	VS_SFIELD_HEADER(VsSFBool,GvSFBool,SFBOOL,GvBool)

public:

	void evaluate(VsValue &ret,VsFunctionCall *); 
    VsValue		assign(const VsValue &, VsFunctionCall *); 
	GvBool evaluateBool();

};

class VsSFFloat : public VsField {
	VS_SFIELD_HEADER(VsSFFloat,GvSFFloat,SFFLOAT,float)
public:

	void evaluate(VsValue &ret,VsFunctionCall *); 
    VsValue		assign(const VsValue &, VsFunctionCall *); 
};

class VsSFTime : public VsField {
	VS_SFIELD_HEADER(VsSFTime,GvSFTime,SFTIME,double)
public:

	void evaluate(VsValue &ret,VsFunctionCall *); 
    VsValue		assign(const VsValue &, VsFunctionCall *); 
};

class VsSFString : public VsField {
	VS_SFIELD_HEADER(VsSFString,GvSFString,SFSTRING,GvString &)

public:
	// alternate constructors 
	VsSFString(const GvString &s,Permission perm=RW) : VsField(new GvSFString(s),perm,0) {};

	void evaluate(VsValue &ret,VsFunctionCall *); 
	VsValue		evaluateLogicOp(VsFunctionCall *f,int operation,VsValue &v2);

    VsValue		assign(const VsValue &, VsFunctionCall *); 

	// members : length
	VsValue evaluateMember(const VsMember &member, VsFunctionCall *fcall);

	// if length >0 
	GvBool evaluateBool();


	// evaluate char 
    void evaluate(VsValue &ret,int32_t index, VsFunctionCall *fcall);

	VsValue	assign(const VsValue &, int32_t index, VsFunctionCall *fcall);

	// constructor init call 
	VsValue initCall(VsFunctionCall *parent,VsExprList *args);

	// invoke member function 
	VsValue callMember(const VsMember &member,VsExprList *args, VsFunctionCall *fcall);



};

class VsSFVec2f : public VsField {
	VS_SFIELD_HEADER(VsSFVec2f,GvSFVec2f,SFVEC2F,float *)

public:
	// alternate constructors 
	VsSFVec2f(float x,float y,Permission perm=RW) : VsField(new GvSFVec2f(x,y),perm,0) {};

	int getLength() { return(2); }

    void evaluate(VsValue &ret,int32_t index, VsFunctionCall *fcall);
	VsValue	assign(const VsValue &, int32_t index, VsFunctionCall *fcall);

	// members : x y 
	VsValue evaluateMember(const VsMember &member, VsFunctionCall *fcall);
	VsValue assignMember(const VsMember &member, const VsValue &v, VsFunctionCall *fcall);

	// invoke member function 
	VsValue callMember(const VsMember &member,VsExprList *args, VsFunctionCall *);

};

class VsSFVec3f : public VsField {
	VS_SFIELD_HEADER(VsSFVec3f,GvSFVec3f,SFVEC3F,float *)

public:
	// alternate constructors 
	VsSFVec3f(float x,float y,float z,Permission perm=RW) : VsField(new GvSFVec3f(x,y,z),perm,0) {};
	// alternate constructors 
	VsSFVec3f(const Point &p,Permission perm=RW) : VsField(new GvSFVec3f(p.x,p.y,p.z),perm,0) {};

	int getLength() { return(3); }

    void evaluate(VsValue &ret,int32_t index, VsFunctionCall *fcall);
	VsValue	assign(const VsValue &, int32_t index, VsFunctionCall *fcall);
	// members : x y z
	VsValue evaluateMember(const VsMember &member, VsFunctionCall *fcall);
	VsValue assignMember(const VsMember &member, const VsValue &v, VsFunctionCall *fcall);

	// invoke member function 
	VsValue callMember(const VsMember &member,VsExprList *args, VsFunctionCall *);

};

class VsSFColor : public VsField {
	VS_SFIELD_HEADER(VsSFColor,GvSFColor,SFCOLOR,float *)

public:
	// alternate constructors 
	VsSFColor(float x,float y,float z,Permission perm=RW) : VsField(new GvSFColor(x,y,z),perm,0) {};

	int getLength() { return(3); }

    void evaluate(VsValue &ret,int32_t index, VsFunctionCall *fcall);
	VsValue	assign(const VsValue &, int32_t index, VsFunctionCall *fcall);

	// members : r g b
	VsValue evaluateMember(const VsMember &member, VsFunctionCall *fcall);
	VsValue assignMember(const VsMember &member, const VsValue &v, VsFunctionCall *fcall);

	// invoke member function 
	VsValue callMember(const VsMember &member,VsExprList *args, VsFunctionCall *);

	

};

class VsSFRotation : public VsField {
	VS_SFIELD_HEADER(VsSFRotation,GvSFRotation,SFROTATION,float *)
public:

	// alternate constructors 
	VsSFRotation(const Rotation &r,Permission perm=RW) : VsField(new GvSFRotation(r),perm,0) {};

	int getLength() { return(4); }

    void		evaluate(VsValue &ret,int32_t index, VsFunctionCall *fcall);
	VsValue	assign(const VsValue &, int32_t index, VsFunctionCall *fcall);
	// members : x y z angle 
	VsValue evaluateMember(const VsMember &member, VsFunctionCall *fcall);
	VsValue assignMember(const VsMember &member, const VsValue &v, VsFunctionCall *fcall);
	VsValue callMember(const VsMember &member,VsExprList *args, VsFunctionCall *fcall);
	VsValue initCall(VsFunctionCall *parent,VsExprList *args);


};

class VsSFMatrix : public VsField {
	VS_SFIELD_HEADER(VsSFMatrix,GvSFMatrix,SFMATRIX,float *)

public:
	VsSFMatrix(Matrix &m,Permission perm=RW);
	int getLength() { return(4*4); }

	// constructor init call 
	VsValue  initCall(VsFunctionCall *parent,VsExprList *args);

	// invoke member function 
	VsValue callMember(const VsMember &member,VsExprList *args, VsFunctionCall *fcall);

	// m[x][y] ))
	//virtual VsVar*		evaluateVar(int32_t index, VsFunctionCall *);


};

class VsSFImage : public VsField {
	VS_SFIELD_HEADER(VsSFImage,GvSFImage,SFIMAGE,int *)
public:
	// constructor init call 
	VsValue  initCall(VsFunctionCall *parent,VsExprList *args);

	int getLength() { return( getField()->getDataLength()); }


    void		evaluate(VsValue &ret,int32_t index, VsFunctionCall *fcall);

	VsValue	assign(const VsValue &, int32_t index, VsFunctionCall *fcall);
	// members : x y z
	VsValue evaluateMember(const VsMember &member, VsFunctionCall *fcall);
	VsValue assignMember(const VsMember &member, const VsValue &v, VsFunctionCall *fcall);

	// invoke member function 
	VsValue callMember(const VsMember &member,VsExprList *args, VsFunctionCall *);

};


class VsMFInt32 : public VsMField {
  public:

	
	VsMFInt32(GvField *field=NULL,Permission perm=RW,int byRef=1) : VsMField(field,perm,byRef) { }

    //virtual VsValue		evaluate(VsFunctionCall *);
    virtual void		evaluate(VsValue &ret,int32_t index, VsFunctionCall *);
    //virtual VsVar*		evaluateVar(int32_t index, VsFunctionCall *);
    virtual VsValue		assign(const VsValue &, int32_t index, VsFunctionCall *);

	
	VsFieldClass getFieldType() const  { return  MFINT32; }
	VsFieldClass getSFieldType() const  { return  SFINT32; }

	GvMFInt32*	getField() { return (GvMFInt32*) field; }

	static VsField *newField(GvField *field=NULL,Permission p=RW,int byRef=1) {
			if (field == NULL && !byRef ) return new VsMFInt32( new GvMFInt32(0),p,0);
			else return new VsMFInt32(field,p,byRef);
	}


};

class VsMFNode : public VsMField {
	VS_MFIELD_HEADER(VsMFNode,GvMFNode,MFNODE,GvNode*)
	virtual VsVar*		evaluateVar(int32_t index, VsFunctionCall *);
};

/*
class VsMFBool : public VsMField {
	VS_MFIELD_HEADER(VsMFBool,GvMFBool,MFBOOL,GvBool)
};
*/

class VsMFFloat : public VsMField {
	VS_MFIELD_HEADER(VsMFFloat,GvMFFloat,MFFLOAT,float)
};

class VsMFTime : public VsMField {
	VS_MFIELD_HEADER(VsMFTime,GvMFTime,MFTIME,double)
};

class VsMFString : public VsMField {
	VS_MFIELD_HEADER(VsMFString,GvMFString,MFSTRING,GvString &)
	
	virtual VsValue assign(const VsValue &v, VsFunctionCall *fcall);
	virtual VsVar*		evaluateVar(int32_t index, VsFunctionCall *);

};

class VsMFVec2f : public VsMField {
	VS_MFIELD_HEADER(VsMFVec2f,GvMFVec2f,MFVEC2F,float *)
	virtual VsVar*		evaluateVar(int32_t index, VsFunctionCall *);
};

class VsMFVec3f : public VsMField {
	VS_MFIELD_HEADER(VsMFVec3f,GvMFVec3f,MFVEC3F,float *)
	virtual VsVar*		evaluateVar(int32_t index, VsFunctionCall *);
	virtual VsValue callMember(const VsMember &member,VsExprList *args, VsFunctionCall *fcall);

};

class VsMFColor : public VsMField {
	VS_MFIELD_HEADER(VsMFColor,GvMFColor,MFCOLOR,float *)
	virtual VsVar*		evaluateVar(int32_t index, VsFunctionCall *);
};

class VsMFRotation : public VsMField {
	VS_MFIELD_HEADER(VsMFRotation,GvMFRotation,MFROTATION,float *)
	virtual VsVar*		evaluateVar(int32_t index, VsFunctionCall *);
};


/*

   Represents a field  
   MF-field[index] variable 

*/


class VsFieldMF1 : public VsField {
public:

	VsMField *fieldVar;	// reference to field var, reference counted
	
	int index;	// the array index

	VsFieldMF1(VsMField *fieldVar,int index);
	~VsFieldMF1();
	
	GvField*	getField() { return fieldVar->getField(); }

	VsFieldClass getFieldType() const  { return fieldVar->getFieldType(); }

	//VsFieldClass getType() const  { fieldVar->getFieldSFType(); }

	// delegate to fieldVar

    virtual void		evaluate(VsValue &ret,VsFunctionCall *);
    virtual VsValue		assign(const VsValue &, VsFunctionCall *);


};

// float, int32, time are directly evaluated
// no member functions needed 


class VsSFStringMF1 : public VsSFString {
public:

	VsMFString *fieldVar;
	int index;

	VsSFStringMF1(VsMFString *fieldVar,int index,Permission perm=RW);
	~VsSFStringMF1();

	void store();
	
    VsValue	assign(const VsValue &, VsFunctionCall *);
	VsValue assign(const VsValue &v,int32_t index, VsFunctionCall *fcall);


};

// reference to a MFVec3f element at index 
// create by VsMFVec3f::evaluteVar(index)
class VsSFVec3fMF1 : public VsSFVec3f {
public:

	VsMFVec3f *fieldVar;
	int index;

	VsSFVec3fMF1(VsMFVec3f *fieldVar,int index,Permission perm=RW);
	~VsSFVec3fMF1();

	void store();
	
    VsValue	assign(const VsValue &, VsFunctionCall *);
	VsValue	assign(const VsValue &, int32_t index, VsFunctionCall *fcall);
	// assignMember falls back to assign(,index,)

};

class VsSFVec2fMF1 : public VsSFVec2f {
public:

	VsMFVec2f *fieldVar;
	int index;

	VsSFVec2fMF1(VsMFVec2f *fieldVar,int index,Permission perm=RW);
	~VsSFVec2fMF1();

	void store();
	
    VsValue	assign(const VsValue &, VsFunctionCall *);
	VsValue	assign(const VsValue &, int32_t index, VsFunctionCall *fcall);
	// assignMember falls back to assign(,index,)

};

class VsSFColorMF1 : public VsSFColor {
public:

	VsMFColor *fieldVar;
	int index;

	VsSFColorMF1(VsMFColor *fieldVar,int index,Permission perm=RW);
	~VsSFColorMF1();

	void store();
	
    VsValue	assign(const VsValue &, VsFunctionCall *);
	VsValue	assign(const VsValue &, int32_t index, VsFunctionCall *fcall);
	// assignMember falls back to assign(,index,)

};

class VsSFRotationMF1 : public VsSFRotation {
public:

	VsMFRotation *fieldVar;
	int index;

	VsSFRotationMF1(VsMFRotation *fieldVar,int index,Permission perm=RW);
	~VsSFRotationMF1();

	void store();
	
    VsValue	assign(const VsValue &, VsFunctionCall *);
	VsValue	assign(const VsValue &, int32_t index, VsFunctionCall *fcall);
	// assignMember falls back to assign(,index,)

};

class VsSFNodeMF1 : public VsSFNode {
public:

	VsMFNode *fieldVar;
	int index;

	VsSFNodeMF1(VsMFNode *fieldVar,int index,Permission perm=RW);
	~VsSFNodeMF1();

	void store();
	
    VsValue	assign(const VsValue &, VsFunctionCall *);
	// assignMember ? 

};

#include <gvsubfield.h>

#define MFVALUE (MFTIME+1)  

#define SFDATE (MFTIME+2)  

class GvMFValue : public GvMField 

{
  public:

    VsValue *values; 

    GV_MFIELD_HEADER(GvMFValue,VsValue,1);

	static VsFieldClass getClassFieldType() { return (MFVALUE); }
	
	GvFieldClass getFieldType() const { return (UnknownType); }
	GvFieldClass getSFFieldType() const { return (UnknownType); }

	//VsFieldClass getFieldType() const { return (MFVALUE); }
	//VsFieldClass getSFFieldType() const { return (UnknownType); }


    int set(const VsValue &newValue);
    void set1(int index, const VsValue &newValue);

	const VsValue *get() const  { return values; }

	operator VsValue *() { return(values); }

	int io1Value(AStream &a, int index);
	int get1(VsValue &value, int index);
	int set1(const VsValue &value, int index);

	VsValue &get1(int i) { if (i>=num) i = num-1; return(values[i]); }

	GvField* get1Field(int index) { return NULL;}
    GvBool operator==(const GvMFValue &f) const ;

};

class VsArray : public VsMField {
	VS_MFIELD_HEADER(VsArray,GvMFValue,MFVALUE,VsValue)
};

class VsDate : public VsField {
	VS_SFIELD_HEADER(VsDate,GvSFTime,SFDATE,double)
public:

	void		evaluate(VsValue &ret,VsFunctionCall *); 
    VsValue		assign(const VsValue &, VsFunctionCall *); 

	// constructor init call 
	VsValue initCall(VsFunctionCall *parent,VsExprList *args);

	// invoke member function  of browser 
	VsValue callMember(const VsMember &member,VsExprList *args, VsFunctionCall *);



};



#endif // 



