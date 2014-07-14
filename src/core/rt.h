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
#ifndef _RT_H
#define _RT_H

//
// Runtime Type information
// after Stroustrup Page 444

// additons (c) 1995 Holger Grahn

#ifndef str
#define str1(XX,YY) XX ## YY
#define strq(XX)  # XX
#define str(XX,YY) str1(XX,YY)
#endif

class RTRoot;


/*! \class TypeId
 *  \brief holds a reference to a actual Typeinfo of a class
 *  intented as a abstract type for general use
 */

class TypeId {

public:

	friend class TypeInfo;

	const TypeInfo *info;		//!< points to class info structure

	TypeId(const TypeInfo *p) : info(p) { };

	//! get class info structure
	const TypeInfo *Info()  const{ return (info); }

	int operator==(TypeId p) const { return(info == p.info); }

    //! create a new instance of this type , or NULL if abstract
	RTRoot *New() const ;

    //! is A ?
    gbool IsA(TypeId T) const;

};


/*! \class TypeInfo
 *  \brief class descriptor
 *  each class with RT-Type information gets a static member of this type
 */

class TypeInfo {

public:
	const char *name;		//!< Class Name
	const char *ioName;		//!< name for input/output of the object, normally eq name

	int id;					//!< unique identification Number for class
	const TypeInfo ** bases;//!< list of base classes

	TypeInfo *next;			//!< link together on static initialization

	//!constructor 
	TypeInfo(const char *Name,int Id=0, const TypeInfo* Bases[]=0,
			int Size=0,RTRoot* (*new_func)() = 0);
	
    //! get class name
	const char *Name() const { return (name); }
	
    //! get IO name 
	const char *IoName() const { return (ioName); }
	
	//! set new IoName, IoName passed by ref !
	void SetIoName(const char *NewIoName) { ioName = NewIoName; }

	//! is this derived from T  or of same type ?
	gbool IsA(const TypeInfo *T) const;

	//! sizeof(class)
	int size;
	
	//! Pointer to function creating a object of that class (factory)
	RTRoot * (*New)();		

//	static int cnt;
	static TypeInfo *root;	  //!< linked list of all instantiated TypeInfos
	int valid;

//	const char *file; __FILE__
// per class data
// e.g. constructor function
// reference counter
// freelist
// parser information ????
// memberinformation

};



// For actual typeinformation two macros need to be used::
// class myClass : public baseClass {
//  RT(baseClass)
//}
// RTIMP(myClass,baseClass)

// CLASS:ClassId();
// object->Id();

/*! \def RT(BASECLASS) 
 *	Macro defining Runtime type information in the declaration of a class
 *  the baseclass parameter is currently not needed
 */

#define RT(BASECLASS)										\
public:														\
		static const TypeInfo info;							\
		virtual TypeId Id() const;							\
		static TypeId ClassId() { return(&info); }			\
		static const TypeInfo* baseInfo[2];					\
		static RTRoot *New()

/*! \def RTIMP(CLASS,ID,BASECLASS)
*	Macro defining Runtime type information in the implementation of a class
*/

#define RTIMP(CLASS,ID,BASECLASS)								\
const TypeInfo* CLASS::baseInfo[2]= { &BASECLASS::info, NULL };	\
const TypeInfo	CLASS::info(strq(CLASS),ID,baseInfo,sizeof(CLASS),CLASS::New);	\
TypeId CLASS::Id() const { return &info; }						\
RTRoot* CLASS::New() { return new CLASS(); }				

/*! \class RTRoot
 *  \brief Root class with runtime type information
 *  derived classes must ovveride type information using the #RT(BASECLASS) and #RTIMP(CLASS,ID,BASECLASS) macros.
 */

class RTRoot {
public:
		static const TypeInfo info;
		
		//! get the TypeId of this object 
		virtual TypeId Id() const;

		// ! reuturn the TypeId of the class
		static TypeId ClassId() { return(&info); }

		//! is this object of same or derived type as T ?
		gbool IsA(TypeId T) const;

		//! is this object of same or derived type as TypeId of T ?
		gbool IsA(RTRoot *T) const { return(IsA(T->Id())); }

		//! get a pointer to the classname of this class
		const char * ClassName() const;
		
		//! virtual destructor
		virtual ~RTRoot() {}

};

//! \def RTIS(P,CLASS) 
//! Is object (*p) of class class ?
#define RTIS(P,CLASS) ((P)->TypeId == CLASS::ClassId())

//! \def RTISA(P,CLASS)
// Is object (*p) of class class or derived from it ?
#define RTISA(P,CLASS) ((P)->IsA(CLASS::ClassId()))

//! \def RTCAST(P,CLASS)
// save cast object point p to CLASS, return 0 if p is not of class CLASS or derived from it
#define RTCAST(P,CLASS) (RTISA(P,CLASS) ? (CLASS *) P : 0)


//! lookup for class typeName
TypeInfo* FindType(const char *typeName);

//! allocate a object given the classname by string
RTRoot* NewType(const char *typeName);


#endif 



