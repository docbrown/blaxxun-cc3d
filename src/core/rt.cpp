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
#include "stdafx.h"

#include "rt.h"



// create a new instance of this type , or NULL if abstract
RTRoot *
TypeId::New() const
{
  if (info->New == NULL) return (NULL);
  return info->New();
}

gbool 
TypeId::IsA(TypeId t) const
{
    return Info()->IsA(t.Info());
}



TypeInfo* TypeInfo::root;	// do no init explicitely ???
// Problem what is the order of calls for static initializers ??????


// TypeInfo constructor
TypeInfo::TypeInfo(const char *Name,int Id, const TypeInfo* Bases[],int Size,RTRoot* (*new_func)())
{
  name=Name;
  ioName=Name;
  id=Id;
  bases = Bases;
  size = Size;
  New = new_func;
//  cnt ++;
  // link together
  next = root;  
  root = this;
  valid = 1234567;
}


gbool 
TypeInfo::IsA(const TypeInfo *T) const 
{
  if (this == T) return(1);

  const TypeInfo* *super = bases;
  if (!super) return(0);		/* at root, no more base classes */

  while( *super) {		// For all super classes
    if ((*super)->IsA(T)) return(1); // check recursively
	super ++;
  }
  return(0);
}

//
// class RTRoot
//
// the Root TypeInfo
const TypeInfo	RTRoot::info("RtRoot",0,0,sizeof(RTRoot));

// return TypeId of class
// this must be overwritten by a virtual function for each and every derived class
// so that static info of thet class is returned

TypeId RTRoot::Id() const { return &info; }

// Query if object is identical or derived from Type with TypeInfo T
gbool RTRoot::IsA(TypeId T)	const
{
		return Id().Info()->IsA(T.Info());
};

// get the class name string of this function

const char * RTRoot::ClassName() const 
{ return Id().Info()->Name(); };


//const char * RTRoot::ClassIoName() const 
//{ return Id().Info()->IoName(); };


//
// lookup for class typeName
TypeInfo* FindType(const char *typeName)
{
TypeInfo *t=TypeInfo::root;
while (t) {
  if ((strcmp(typeName,t->name) == 0)
  	 || (strcmp(typeName,t->ioName) == 0))
  	 return(t);
  t=t->next;


}
return(NULL);
}

// allocate a object given the classname by string
RTRoot* NewType(const char *typeName)
{
TypeInfo *t = FindType(typeName);
if (!t) return(NULL);
if (t->New == 0) return(NULL);		// abstract type
return t->New();
}

// Build in class support
class Int {
	RT(RTRoot);
public:
	int v;
	Int(int V = 0) { v=V; }
	operator int() { return(v); }
};
class Float {
	RT(RTRoot);
public:
	float v;
	Float(float V = 0) { v=V; }
	operator float() { return(v); }
};


#if 0

template <class Restype,A1> 
class FunctionCall {
public:

	ResType (*func)(A1);

	// call function, args are binary packed into argbuf
	// result is copied binary into resbuf	
	virtual void CallIt(char * argbuf,char * resbuf)) {
		A1 *a1;

		a1 =  (A1 *) argbuf;
		argbuf += sizeof(A1);
		* ((Restype *) resbuf)=  func(*a1);
	}

};
#endif


#if 0
// Test
// Holds information about a function call
class FunctionInfo {
public:
	const char *name;
	int nargs;
	TypeId res;
	TypeId *args;
	
	void  (*call) (char * argbuf,char * resbuf);
	static FunctionInfo *root;	  // linked list of all instantiated FunctionInfos
	FunctionInfo *next;

	FunctionInfo(TypeId Res,const char *Name,int NArgs =0,TypeId *Args=0,
		void  (*Call) (char * argbuf,char * resbuf)=0) :
			res(Res), name(Name), nargs(NArgs), call(Call) {
					args = Args;
 				    next = root;  root = this;

				};
};

// Define a static function info
#define DEFFUNC(NAME,RESTYPE,A1) 								\
static TypeId str(NAME,InfoArgs)[] = { A1::ClassId() };			\
static void  str(NAME,Call)(char * argbuf,char * resbuf) { 		\
		* ((RESTYPE *) resbuf) =  NAME(*((A1*)argbuf)); 			\
	}															\
FunctionInfo	str(NAME,Info) (RESTYPE::ClassId(),strq(name),1,str(NAME,InfoArgs),str(NAME,Call));

#define DEFFUNC2(NAME,RESTYPE,A1,A2) 								\
static TypeId str(NAME,InfoArgs)[] = { A1::ClassId(),A2::ClassId() };	\
static void  str(NAME,Call)(char * argbuf,char * resbuf) { 		\
		* ((RESTYPE *) resbuf) =  NAME(*((A1*)argbuf),*((A2*)(argbuf+sizeof(A1)))); \
	}															\
FunctionInfo	str(NAME,Info) (RESTYPE::ClassId(),strq(name),2,str(NAME,InfoArgs),str(NAME,Call));

//		A1 *a1;		a1 =  (A1 *) argbuf;		argbuf += sizeof(A1);

RTRoot f1(RTRoot x) { return(x); }

DEFFUNC(f1,RTRoot,RTRoot)

FunctionInfo* FunctionInfo::root;	// do no init explicitely ???

#include <math.h>
DEFFUNC(sin,Float,Float)
DEFFUNC(cos,Float,Float)
DEFFUNC(tan,Float,Float)
DEFFUNC2(atan2,Float,Float,Float)


void print_functions() 
{
  FunctionInfo* f= FunctionInfo::root;

  while (f != 0 ) {
  	f = f->next;
  }
}

#endif
