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
/******************************************************************************
@doc

@module VSFields.cpp - GLView VRMLSCRIPT vrml field objects|

Copyright (c) 1997-1999	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:
 15.11.99 HG VsDate 
 03.07.97 HG fixed Problem of evaluation of MFVec3f[index] MF1 constructors inits SF shadown value 

$Revision: $
$Log:

Todo :

  implement VsDate fromstring / to string function
  check if VsDate is correct
  check VsArray object

  static methods on VsDate

******************************************************************************/


#include <assert.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "point.h"
#include "quaternation.h"


#include "gvnode.h"
#include "gvfielddata.h"
#include "vsfields.h"
#include "gvsensors.h"
#include "gvbsp.h"
#include "gvscene.h"
#include "gvscript.h"

#include "gvmatrixtransform.h"

#include "vsfunction.h"
// #include "vsexpr.h"

#include "vsfields.h"
#include "gperlinnoise.h"

#include "sys/timeb.h"


#include "greporter.h"
#include "y_tab.h"  /* For the PLUS/etc #defines */


enum VsStaticMember {
	VS_UNKNOWN,
	VS_X,
	VS_Y,
	VS_Z,
	VS_R,
	VS_G,
	VS_B,
	VS_angle,
	VS_length,
	VS_toString,
	VS_add,
	VS_cross,
	VS_divide,
	VS_dot,
	VS_multiply,
	VS_negate,
	VS_normalize,
	VS_subtract
};


// similar to GvKeyword
class VsNames {
public :
	static GvName x,y,z;
	static GvName r,g,b;
	static GvName angle;
	static GvName length;
	static GvName toString;
	static GvName add,cross,divide,dot,multiply,negate,normalize,subtract;
	static GvName comp,array;

	// Browser

#define VS_NAME(XX) static GvName XX;
	VS_NAME(getName)
	VS_NAME(getVersion)
	VS_NAME(getCurrentSpeed)
	VS_NAME(getCurrentFrameRate)
	VS_NAME(getCurrentTime)
	VS_NAME(getTime)
	VS_NAME(getWorldStartTime)
	VS_NAME(getWorldURL)
	VS_NAME(getWorldBaseURL)
	VS_NAME(setDescription)
	VS_NAME(print)
	VS_NAME(getBaseURL)
	VS_NAME(createVrmlFromString)
	VS_NAME(createVrmlFromURL)
	VS_NAME(loadURL)
	VS_NAME(loadURLrel)
	VS_NAME(isKeyPressed)
// Date method names

	VS_NAME(getDate)
	VS_NAME(getDay)
	VS_NAME(getHours)
	VS_NAME(getMinutes)
	VS_NAME(getMonth)
	VS_NAME(getSeconds)
	//VS_NAME(getTime)
	VS_NAME(getTimezoneOffset)
	VS_NAME(getYear)
	VS_NAME(parse)
	VS_NAME(setDate)
	VS_NAME(setHours)
	VS_NAME(setMinutes)
	VS_NAME(setMonth)
	VS_NAME(setSeconds)
	VS_NAME(setTime)
	VS_NAME(setYear)
	VS_NAME(toGMTString)
	VS_NAME(toLocaleString)
	VS_NAME(UTC)



#undef VS_NAME
	
	static VsStaticMember lookup(GvName *n);

};


GvName VsNames::x("x");
GvName VsNames::y("y");
GvName VsNames::z("z");
GvName VsNames::r("r");
GvName VsNames::g("g");
GvName VsNames::b("b");
GvName VsNames::angle("angle");
GvName VsNames::length("length");
GvName VsNames::toString("toString");
GvName VsNames::add("add");
GvName VsNames::cross("cross");
GvName VsNames::divide("divide");
GvName VsNames::dot("dot");
GvName VsNames::multiply("multiply");
GvName VsNames::negate("negate");
GvName VsNames::normalize("normalize");
GvName VsNames::subtract("subtract");
GvName VsNames::comp("comp");
GvName VsNames::array("array");

#define VS_NAME(XX) GvName VsNames::XX(strq(XX));
// define the statics

	VS_NAME(getName)
	VS_NAME(getVersion)
	VS_NAME(getCurrentSpeed)
	VS_NAME(getCurrentFrameRate)
	VS_NAME(getCurrentTime)
	VS_NAME(getTime)
	VS_NAME(getWorldStartTime)
	VS_NAME(getWorldURL)
	VS_NAME(getWorldBaseURL)
	VS_NAME(setDescription)
	VS_NAME(print)
	VS_NAME(getBaseURL)
	VS_NAME(createVrmlFromString)
	VS_NAME(createVrmlFromURL)
	VS_NAME(loadURL)
	VS_NAME(loadURLrel)
	VS_NAME(isKeyPressed)
// Date method names

	VS_NAME(getDate)
	VS_NAME(getDay)
	VS_NAME(getHours)
	VS_NAME(getMinutes)
	VS_NAME(getMonth)
	VS_NAME(getSeconds)
	//VS_NAME(getTime)
	VS_NAME(getTimezoneOffset)
	VS_NAME(getYear)
	VS_NAME(parse)
	VS_NAME(setDate)
	VS_NAME(setHours)
	VS_NAME(setMinutes)
	VS_NAME(setMonth)
	VS_NAME(setSeconds)
	VS_NAME(setTime)
	VS_NAME(setYear)
	VS_NAME(toGMTString)
	VS_NAME(toLocaleString)
	VS_NAME(UTC)


#undef VS_NAME
#define VS_NAME_EQ(AA,XX) (AA == VsNames::XX)

// helpers 
#define VS_RET_INDEX_ERROR(index,fcall) { Reporter.Error("script error:Bad index %d in %s",index,(fcall)->getContextString());  return VsValue::nothing;  } 
#define VS_RET_INDEX_ERROR_RET(index,fcall) { Reporter.Error("script error:Bad index %d in %s",index,(fcall)->getContextString());  ret = VsValue::nothing; return; } 
#define VS_RET_INDEX_ERROR_VAR(index,fcall) { Reporter.Error("script error:Bad index %d in %s",index,(fcall)->getContextString());  return NULL; } 

#define VS_RO_ERROR()  { Reporter.Error("script error: can not assign to readonly variable \n"); } 
#define VS_RO_ERROR1(fcall) { Reporter.Error("script error: can not assign to readonly variable in %s",(fcall)->getContextString()); } 
#define VS_ARG_TYPE_ERROR(fcall,v) { Reporter.Error("script error: bad argument value %s %s",(fcall)->getContextString(),v); } 
#define VS_ARG_TYPE_ERRORI(fcall,i,v) { Reporter.Error("script error: bad argument value %d, %s in %s ",i,v,(fcall)->getContextString()); } 

// create newly allocated MFString outof value 
GvMFString* newMFString(VsValue &v,VsFunctionCall *fcall)
{
	GvMFString *f = new GvMFString(0);

	switch(v.type) {
/* obsolete 
	case VsValue::vSTRING_ARRAY:
		{
			f->set(v.stringArray->length,v.stringArray->array);
		}
		break;
*/

	case VsValue::vFIELD:  	
			if (MFSTRING  == v.field->getFieldType()) {
				((GvField *)f)->set((v.field)->getField());
			}
			else if (SFSTRING  == v.field->getFieldType()) {
				f->set_(( (VsSFString *)v.field)->getField()->get());
			}
			else Reporter.Error("script error: Bad MFString expression in %s",fcall->getContextString());
			break;

	case VsValue::vSTRING :
	case VsValue::vGVSTRING :
	default : {
			GvString s;
			v.getString(s);
			f->set_(s);
		}
		break;
	}
	return(f);
}

// create newly allocated MFNode outof value 
GvMFNode* newMFNode(VsValue &v, VsFunctionCall *fcall)
{
	GvMFNode *f = new GvMFNode(0);

	switch(v.type) {
	case VsValue::vNODE :
		f->set(v.node);  
		break;
	case VsValue::vFIELD:  	
		if (SFNODE  == v.field->getFieldType()) {
			f->set((GvNode*)((VsSFNode *)v.field)->getField()->get());
		} 
		else if (MFNODE  == v.field->getFieldType()) {
			f->set(*((VsMFNode *)v.field)->getField());
		} 
		else {
			Reporter.Error("script error: Bad MFNode expression in %s",fcall->getContextString());
		}	
	break;
	default :
		Reporter.Error("vrmlscript:Bad MFNode expression in %s",fcall->getContextString());		
		break;
	}
	return(f);
};


//
// VsField
// 

//RTIMP(VsField,0,VsVar);

static int fieldCnt= 0;


VsField::VsField(GvField *field_,Permission p,int byRef_) : field(field_), permission(p), byRef(byRef_)
{
	if (!byRef)	fieldCnt++;
}


VsField *
VsField::create(VsFieldClass t, Permission p,GvField *field,int byRef)
{
    VsField *result = NULL;

    switch(t) {
      case SFINT32: result = VsSFInt32::newField(field,p,byRef); break;
      case SFBOOL:	result = VsSFBool::newField(field,p,byRef); break;
      case SFFLOAT: result = VsSFFloat::newField(field,p,byRef); break;
	  case SFTIME: 	result = VsSFTime::newField(field,p,byRef); break;
      case SFVEC2F: result = VsSFVec2f::newField(field,p,byRef); break;
      case SFVEC3F: result = VsSFVec3f::newField(field,p,byRef); break;
      case SFCOLOR: result = VsSFColor::newField(field,p,byRef); break;
      case SFROTATION:result = VsSFRotation::newField(field,p,byRef); break;
      case SFSTRING:result = VsSFString::newField(field,p,byRef); break;
      case SFNODE:  result = VsSFNode::newField(field,p,byRef); break;
      case SFIMAGE: result = VsSFImage::newField(field,p,byRef); break;
      case SFMATRIX:result = VsSFMatrix::newField(field,p,byRef); break;

      case MFINT32: result = VsMFInt32::newField(field,p,byRef); break;
      case MFFLOAT: result = VsMFFloat::newField(field,p,byRef); break;
      case MFVEC2F: result = VsMFVec2f::newField(field,p,byRef); break;
      case MFVEC3F: result = VsMFVec3f::newField(field,p,byRef); break;
      case MFCOLOR: result = VsMFColor::newField(field,p,byRef); break;
      case MFROTATION: result = VsMFRotation::newField(field,p,byRef); break;
      case MFSTRING: result = VsMFString::newField(field,p,byRef); break;
      case MFNODE:  result = VsMFNode::newField(field,p,byRef); break;
      case MFTIME: result = VsMFTime::newField(field,p,byRef); break;
      
	  case MFVALUE: result = VsArray::newField(field,p,byRef); break;
	  case SFDATE: result = VsDate::newField(field,p,byRef); break;
	  
	  default :
		  TRACE("VrmlScript:Unhandled field type \n");
		  break;
    }
    return result;
}

// invoke member function , supported toString
VsValue VsField::callMember(const VsMember &member,VsExprList *args, VsFunctionCall *fcall)
{	
 	if (member == VsNames::toString) { 	
		GvField *v=getField();
		GvString vs;
		v->get(vs);
/*
	    VsValue result;
		GvString *vs = new GvString();
		v->get(*vs);
		result.type = VsValue::vGVSTRING;
		result.gvString = vs;
		return result;
*/
		return VsValue(new VsSFString(vs));

	}
	return VsVar::callMember(member,args,fcall);
}




//TOOL functions 

// get one argument as SFString
GvBool getArg(VsExprList &args, int i, VsFunctionCall *fcall,VsValue &v,GvSFString *&vf)
{
  if (i >= args.getLength()) return FALSE;
  
  args[i]->evaluate(v,fcall);

  switch (v.type) {
	case VsValue::vFIELD:
		if (GvSFString::getClassFieldType() == v.field->getFieldType()) {
				vf = ((VsSFString *)v.field)->getField();
				return TRUE;
		}

	default: break;
  }
  VS_ARG_TYPE_ERRORI(fcall,i,"SFString expected");
  return FALSE;
}

// get one argument as String
GvBool getArg(VsExprList &args, int i, VsFunctionCall *fcall,GvString &vf)
{
  VsValue v;

  if (i >= args.getLength()) return FALSE;
  
  args[i]->evaluate(v,fcall);

  switch (v.type) {
	case VsValue::vSTRING:
	case VsValue::vGVSTRING:
	case VsValue::vNUMBER:
	case VsValue::vFIELD:
		{ return v.getString(vf);}
/*
	case VsValue::vFIELD:
		if (GvSFString::getClassFieldType() == v.field->getFieldType()) {
				vf = ((VsSFString *)v.field)->getField()->get();
				return TRUE;
		}

*/
	default: break;
  }
  VS_ARG_TYPE_ERRORI(fcall,i,"String expected");
  return FALSE;
}


// get one argument of SFVec2f
GvBool getArg(VsExprList &args, VsFunctionCall *fcall,VsValue &v,GvSFVec2f *&vf)
{
  if (args.getLength() != 1) return FALSE;
  
  args[0]->evaluate(v,fcall);

  switch (v.type) {
	case VsValue::vFIELD:
		if (GvSFVec2f::getClassFieldType() == v.field->getFieldType()) {
				vf =  ((VsSFVec2f *)v.field)->getField();
				return TRUE;
		}

	default: break;
  }
  VS_ARG_TYPE_ERRORI(fcall,0,"SFVec2f expected");
  return FALSE;
}


// get one argument of SFVec3f
GvBool getArg(VsExprList &args, VsFunctionCall *fcall,VsValue &v,GvSFVec3f *&vf)
{
  if (args.getLength() != 1) return FALSE;
  
  args[0]->evaluate(v,fcall);

  switch (v.type) {
	case VsValue::vFIELD:
		if (GvSFVec3f::getClassFieldType() == v.field->getFieldType()) {
				vf =  ((VsSFVec3f *)v.field)->getField();
				return TRUE;
		}

	default: break;
  }
  VS_ARG_TYPE_ERRORI(fcall,0,"SFVec3f expected");
  return FALSE;
}

GvBool getArg(VsExprList &args, int i, VsFunctionCall *fcall,VsValue &v,GvSFVec3f *&vf)
{
  if (i >= args.getLength()) return FALSE;
  
  args[i]->evaluate(v,fcall);

  switch (v.type) {
	case VsValue::vFIELD:
		if (GvSFVec3f::getClassFieldType() == v.field->getFieldType()) {
				vf =  ((VsSFVec3f *)v.field)->getField();
				return TRUE;
		}

	default: break;
  }

  VS_ARG_TYPE_ERRORI(fcall,i,"SFVec3f expected");

  return FALSE;
}


// get one argument at SFVec3f
GvBool getArg(VsExprList &args, int i, VsFunctionCall *fcall,Point &vp)
{
  VsValue v;

  if (i >= args.getLength()) return FALSE;
  
  args[i]->evaluate(v,fcall);

  switch (v.type) {
	case VsValue::vFIELD:
		if (GvSFVec3f::getClassFieldType() == v.field->getFieldType()) {
				vp =  ((VsSFVec3f *)v.field)->getField()->getPoint();
				return TRUE;
		}

	default: break;
  }
  VS_ARG_TYPE_ERRORI(fcall,i,"SFVec3f expected");

  return FALSE;
}
// set one argument as variable of type  SFVec3f
GvBool setArg(VsExprList &args, int i, VsFunctionCall *fcall,const Point &vp)
{
  VsValue newValue;
  if (i >= args.getLength()) return FALSE;

  VsValue argvalue;
  VsVar *v = args[i]->evaluateVar(fcall,argvalue);

  if (!v) return FALSE;

  // do a generic assign 
  newValue = new VsSFVec3f(vp.x,vp.y,vp.z);

  v->ref();
	v->assign(newValue,fcall);
  v->unref();
  return TRUE;

}

// get one argument as Rotation from SFRotation 
GvBool getArg(VsExprList &args, int i, VsFunctionCall *fcall,Rotation &vr)
{
  VsValue v;

  if (i >= args.getLength()) return FALSE;

  args[i]->evaluate(v,fcall);

  switch (v.type) {
	case VsValue::vFIELD:
		if (GvSFRotation::getClassFieldType() == v.field->getFieldType()) {
				vr =  ((VsSFRotation *)v.field)->getField()->get();
				return TRUE;
		}

	default: break;
  }
  VS_ARG_TYPE_ERRORI(fcall,i,"SFRotation expected");
  return FALSE;
}

GvBool getArg(VsExprList &args, int i, VsFunctionCall *fcall,VsValue &v,GvSFRotation *&vf)
{
  if (i >= args.getLength()) return FALSE;
  
  args[i]->evaluate(v,fcall);

  switch (v.type) {
	case VsValue::vFIELD:
		if (GvSFRotation::getClassFieldType() == v.field->getFieldType()) {
				vf =  ((VsSFRotation*)v.field)->getField();
				return TRUE;
		}

	default: break;
  }
  VS_ARG_TYPE_ERRORI(fcall,i,"SFRotation expected");
  return FALSE;
}
GvBool getArg(VsExprList &args, int i, VsFunctionCall *fcall,VsValue &v,GvMFInt32 *&vf)
{
  if (i >= args.getLength()) return FALSE;
  
  args[i]->evaluate(v,fcall);

  switch (v.type) {
	case VsValue::vFIELD:
		if (GvMFInt32::getClassFieldType() == v.field->getFieldType()) {
				vf =  ((VsMFInt32*)v.field)->getField();
				return TRUE;
		}

	default: break;
  }
  VS_ARG_TYPE_ERRORI(fcall,i,"MFInt32 expected");
  return FALSE;
}

GvBool getArg(VsExprList &args, int i, VsFunctionCall *fcall,VsValue &v,GvMFVec3f *&vf)
{
  if (i >= args.getLength()) return FALSE;
  
  args[i]->evaluate(v,fcall);

  switch (v.type) {
	case VsValue::vFIELD:
		if (GvMFVec3f::getClassFieldType() == v.field->getFieldType()) {
				vf =  ((VsMFVec3f*)v.field)->getField();
				return TRUE;
		}

	default: break;
  }
  VS_ARG_TYPE_ERRORI(fcall,i,"MFVec3f expected");
  return FALSE;
}


GvBool getArg(VsExprList &args, int i, VsFunctionCall *fcall,VsValue &v,GvMFRotation *&vf)
{
  if (i >= args.getLength()) return FALSE;
  
  args[i]->evaluate(v,fcall);

  switch (v.type) {
	case VsValue::vFIELD:
		if (GvMFRotation::getClassFieldType() == v.field->getFieldType()) {
				vf =  ((VsMFRotation*)v.field)->getField();
				return TRUE;
		}

	default: break;
  }
  VS_ARG_TYPE_ERRORI(fcall,i,"MFRotation expected");
  return FALSE;
}



// set one argument as variable of type  SFRotation
GvBool setArg(VsExprList &args, int i, VsFunctionCall *fcall,Rotation &vr)
{
  VsValue newValue;
  if (i >= args.getLength()) return FALSE;

  VsValue argvalue;
  VsVar *v = args[i]->evaluateVar(fcall,argvalue);
  if (!v) return FALSE;

  // do a generic assign 
  newValue = new VsSFRotation(vr);

  v->ref();
	v->assign(newValue,fcall);
  v->unref();
  return TRUE;

}


// get one argument as Matrix 
GvBool getArg(VsExprList &args, int i, VsFunctionCall *fcall,Matrix &m)
{
  VsValue v;

  m.Identity();
  if (i >= args.getLength()) return FALSE;
  
  args[i]->evaluate(v,fcall);

  switch (v.type) {
	case VsValue::vFIELD:
		if (GvSFMatrix::getClassFieldType() == v.field->getFieldType()) {
				((VsSFMatrix *)v.field)->getField()->get(m);
				return TRUE;
		}

	default: break;
  }
  VS_ARG_TYPE_ERRORI(fcall,i,"Matrix expected");
  return FALSE;
}

// compute a matrix similar to the Transform node 
void setTransform(Matrix &m,const Point &translation,const Rotation &rotation,
				  const Point &scale,const Rotation &scaleOrientation,const Point &center)
{
	 m.TranslationMatrix(center+translation);
	 if (rotation.Angle() != 0.0f) {
		m.MultiplyLeft(RotationAxisMatrix(rotation.Axis(),rotation.Angle()));
	 }
	 if (1) {
		 if ( scaleOrientation.Angle()==0.0f) {
				m.ScaleByLeft(scale);
		 } else {
			m.MultiplyLeft(RotationAxisMatrix(scaleOrientation.Axis(),scaleOrientation.Angle()));
				m.ScaleByLeft(scale);
			m.MultiplyLeft(RotationAxisMatrix(scaleOrientation.Axis(),-scaleOrientation.Angle()));
		 }
	 }
	 m.MultiplyLeft(TranslationMatrix(-center));
}


GvBool getArg(VsExprList &args, int i, VsFunctionCall *fcall,float &fv)
{
  VsValue v;

  if (i >= args.getLength()) return FALSE;

  
  args[i]->evaluate(v,fcall);
  switch (v.type) {
		case VsValue::vNUMBER: 	fv=v.number; return(TRUE);
		default: break;
  }
  VS_ARG_TYPE_ERRORI(fcall,i,"Number expected");
  return FALSE;
}

GvBool getArg(VsExprList &args, int i, VsFunctionCall *fcall,double &fv)
{
  VsValue v;

  if (i >= args.getLength()) return FALSE;

  
  args[i]->evaluate(v,fcall);
  switch (v.type) {
		case VsValue::vNUMBER: 	fv=v.number; return(TRUE);
		default: break;
  }
  VS_ARG_TYPE_ERRORI(fcall,i,"Number expected");
  return FALSE;
}


GvBool getArg(VsExprList &args, int i, VsFunctionCall *fcall,int &fv)
{
  VsValue v;

  if (i >= args.getLength()) return FALSE;

  
  args[i]->evaluate(v,fcall);
  switch (v.type) {
		case VsValue::vNUMBER: 	fv=v.number; return(TRUE);
		default: break;
  }
  VS_ARG_TYPE_ERRORI(fcall,i,"integer Number expected");
  return FALSE;
}

GvBool getArg(VsExprList &args, VsFunctionCall *fcall)
{
  return args.getLength() == 0;
}




#define BAD_ARG goto bad_arg




//
void
VsField::setField(GvField *f,int byRef) 
{
	if (!this->byRef && field && field !=f) delete field;
	field = f;
	this->byRef = byRef;

}

VsField::~VsField()
{
	if (!byRef && field!=NULL) {
		fieldCnt--;
		delete field;
	}
}



// constructor init call 
VsValue VsField::initCall(VsFunctionCall *parent,VsExprList *args)
{
	int l= getLength();

	int numArgs = min(l,args->getLength());
	
	VsValue v;


	if (l == 1 && numArgs >0) {
		if ((*args)[0] != NULL)	{//  argument not passed 
			(*args)[0]->evaluate(v,parent);
			((VsVar*)this)->assign(v,parent);
		}
	}
	else  {
		// Evaluate arguments:
		for (int i = 0; i < numArgs; i++) {
			if ((*args)[i] != NULL)	{ //  argument not passed 
				(*args)[i]->evaluate(v,parent);
				((VsVar*)this)->assign(v,i,parent);
			}
		}
	}

	return VsValue(this);
}

VsValue VsField::evaluateMember(const VsMember &member, VsFunctionCall *fcall)
{
	Reporter.Error("script error: Unimplemented property %s in %s",member.getString(),fcall->getContextString());
	return VsValue::nothing;
}

VsValue
VsField::assignMember(const VsMember &member, const VsValue &v, VsFunctionCall *fcall)
{
	Reporter.Error("script error: Unimplemented property assign %s in %s",member.getString(),fcall->getContextString());
	return VsValue::nothing;
}

#if 0

VsValue 
VsField::evaluateLogicOp(VsFunctionCall *f,int operation,VsValue &v2 )
{ 
	GvBool result=gfalse;
	const GvString &s1=getField()->get();

	GvString s2;
	v2.getString(s2);


	if (1) {
	int cmp = strcmp(s1, s2);

	switch (operation) {
	  case tEQ:
	    result = getField;
	    break;
	  case tNE:
	    result = (cmp != 0);
	    break;
	  case tLT:
	    result = (cmp < 0);
	    break;
	  case tLE:
	    result = (cmp <= 0);
	    break;
	  case tGT:
	    result = (cmp > 0);
	    break;
	  case tGE:
	    result = (cmp >= 0);
	    break;
	  default:
  	    Reporter.Error("Bad operation in %s",f->getContextString()); 
	    assert(0);
	    break;
	}
	}
	return VsValue(result);
}

#endif


void
VsField::evaluate(VsValue &ret, VsFunctionCall *)
{
    ret=VsValue(this);
}

VsVar*
VsField::evaluateVar(VsFunctionCall *, VsValue &value)
{
    value = VsValue(this);
	return this;
}


void
VsField::evaluate(VsValue &ret,int32_t index, VsFunctionCall *fcall)
{
	Reporter.Error("script error: Unimplemented array in %s",fcall->getContextString());
    ret=VsValue::nothing;
}

VsValue
VsField::assign(const VsValue &v, VsFunctionCall *fcall)
{
    if (permission == VsField::RW) {
		switch (v.type) {
		case VsValue::vFIELD:
			if (getFieldType() == v.field->getFieldType()) {
				getField()->set(v.field->getField());
				//field->setHasBeenSet((VbBool) TRUE);
				return v;
			}
/*
			if (getSFFieldType() == v.field->getFieldType()) {
				getField()->makeRoom(1);
				getField()->set1(0,v.field->getField());
				return v;
			}
*/
			TRACE("Error :%s::assign(v,) bad type  \n","Field");
			Reporter.Error("script error: Variable Assigment, type mismatch in %s",fcall->getContextString());
			break;
		case VsValue::vSTRING: 	getField()->set(v.string); 	return(v);
		case VsValue::vGVSTRING: getField()->set(*v.gvString); 	return(v);
		default:
   			TRACE("Error :%s::assign(v,) bad type  \n","Field");
			Reporter.Error("script error: Variable Assigment, bad expression in %s",fcall->getContextString());
			break;
	}

	}
	else VS_RO_ERROR1(fcall);

	return VsValue::nothing;
}
VsValue
VsField::assign(const VsValue &v, int32_t index, VsFunctionCall *fcall)
{
	Reporter.Error("script error: Unimplemented property array assign in %s",fcall->getContextString());
	return VsValue::nothing;
}


void
VsMField::evaluate(VsValue &ret,int32_t index, VsFunctionCall *fcall)
{
   	TRACE("Error :%s::evalute(int,) not implemented \n","MField");
	Reporter.Error("Unimplemented array in %s",fcall->getContextString());
	ret = VsValue::nothing;
}

VsValue
VsMField::assign(const VsValue &v, int32_t index, VsFunctionCall *fcall)
{
   	TRACE("Error :%s::assign(v,int,) not implemented \n","MField");
	Reporter.Error("Unimplemented array assign in %s",fcall->getContextString());
	return VsValue::nothing;
}

// evaluate reference to element at offset
VsVar*	VsMField::evaluateVar(int32_t index, VsFunctionCall *)
{
//	Reporter.Warning("MField evaluate var");
	return new VsFieldMF1(this,index);
}

// evaluate a member of field 
VsValue
VsMField::evaluateMember(const VsMember &member, VsFunctionCall *fcall)
{
	if (member == VsNames::length) {
		return VsValue(getLength());
	}
	return VsField::evaluateMember(member,fcall);
}

VsValue
VsMField::assignMember(const VsMember &member, const VsValue &v, VsFunctionCall *fcall)
{
 	if (member == VsNames::length) {
		if (v.type == VsValue::vNUMBER) {
			int l=v.number;
			if (l>=0) setLength(l);
			return VsValue(getLength());
		}
		Reporter.Error("length Assigment, bad expression in %s",fcall->getContextString());
	}
	return VsField::assignMember(member,v,fcall);
}


// constructor init call 
VsValue VsMField::initCall(VsFunctionCall *parent,VsExprList *args)
{

	int numArgs = args->getLength();

	{
		VsValue v;
		setLength(numArgs);
		// Evaluate arguments:
		for (int i = 0; i < numArgs; i++) {
			if ((*args)[i] != NULL)	{//  argument not passed 
				(*args)[i]->evaluate(v,parent);
				assign(v,i,parent);
			}
		}
	}

	return VsValue(this);
}




//
// VsSFInt 
//


VsSFInt32::VsSFInt32(GvField *field,Permission perm,int byRef) : VsField(field,perm,byRef)
{
}


void
VsSFInt32::evaluate(VsValue &ret,VsFunctionCall *)
{
    ret = VsValue(getField()->get());
}

VsValue
VsSFInt32::assign(const VsValue &v, VsFunctionCall *fcall)
{
    if (permission == VsField::RW) {
		switch (v.type) {
		case VsValue::vNUMBER: 	getField()->set((long)v.number); 	return(v);
		default: return VsField::assign(v,fcall);
		}
	}

	return VsValue::nothing;
}

// VsSFBool

void
VsSFBool::evaluate(VsValue &ret,VsFunctionCall *)
{
    ret = VsValue(getField()->get());
}

GvBool VsSFBool::evaluateBool()
{
	return (getField()->get());
}

VsValue VsSFBool::assign(const VsValue &v, VsFunctionCall *fcall)
{
    if (permission == VsField::RW) {
		switch (v.type) {
		case VsValue::vNUMBER: 	getField()->set((int)v.number); 	return(v);
		default: return VsField::assign(v,fcall);
		}
	}
	return VsValue::nothing;
}
// VsSFFloat

void
VsSFFloat::evaluate(VsValue &ret, VsFunctionCall *)
{
    ret = VsValue(getField()->get());
}

VsValue VsSFFloat::assign(const VsValue &v, VsFunctionCall *fcall)
{
    if (permission == VsField::RW) {
		switch (v.type) {
		case VsValue::vNUMBER: 	getField()->set(v.number); 	return(v);
		default: return VsField::assign(v,fcall);
		}
	}
	return VsValue::nothing;
}

// VsSFTime

void
VsSFTime::evaluate(VsValue &ret,VsFunctionCall *)
{
    ret=VsValue(getField()->get());
}

VsValue VsSFTime::assign(const VsValue &v, VsFunctionCall *fcall)
{
    if (permission == VsField::RW) {
		switch (v.type) {
		case VsValue::vNUMBER: 	getField()->set(v.number); 	return(v);
		default: return VsField::assign(v,fcall);
		}
	}
	return VsValue::nothing;
}

// VsSFString

void
VsSFString::evaluate(VsValue &ret,VsFunctionCall *)
{
    ret=VsValue(this); // evalute to self, so that string member functions work
	// return VsValue(getField()->get()); // ?? eval to self 
}

VsValue 
VsSFString::evaluateLogicOp(VsFunctionCall *f,int operation,VsValue &v2 )
{ 
	GvBool result=gfalse;
	const GvString &s1=getField()->get();

	GvString s2;
	v2.getString(s2);


	if (1) {
	int cmp = strcmp(s1, s2);

	switch (operation) {
	  case tEQ:
	    result = (cmp == 0);
	    break;
	  case tNE:
	    result = (cmp != 0);
	    break;
	  case tLT:
	    result = (cmp < 0);
	    break;
	  case tLE:
	    result = (cmp <= 0);
	    break;
	  case tGT:
	    result = (cmp > 0);
	    break;
	  case tGE:
	    result = (cmp >= 0);
	    break;
	  default:
  	    Reporter.Error("Bad operation in %s",f->getContextString()); 
	    assert(0);
	    break;
	}
	}
	return VsValue(result);
}


VsValue VsSFString::assign(const VsValue &v, VsFunctionCall *fcall)
{
    if (permission == VsField::RW) {
		{ GvString s; v.getString(s); getField()->set_(s); }	// allways try to string convert 
		return(v);
/*
		switch (v.type) {	   
		case VsValue::vNUMBER: { GvString s; v.getString(s); getField()->set_(s); }	return(v);
		case VsValue::vSTRING:	getField()->set_(v.string); return(v);
		case VsValue::vGVSTRING: getField()->set_(*v.gvString); return(v);
		default: return VsField::assign(v,fcall);
		}
*/
	}
	return VsValue::nothing;
}

VsValue VsSFString::evaluateMember(const VsMember &member, VsFunctionCall *fcall)
{
	if (member == VsNames::length) return VsValue(getField()->get().getLength());
	return VsField::evaluateMember(member,fcall);
}

GvBool VsSFString::evaluateBool()
{
	return (getField()->get().getLength())>0;
}


void
VsSFString::evaluate(VsValue &ret,int32_t index, VsFunctionCall *fcall)
{									   
	const GvString &s=	getField()->get();
	if (index >=0 && index < s.getLength()) {
		GvString v(s[index]);
		//return VsValue(v);
		ret = VsValue(new VsSFString(v));

	}	
	else ret = VsValue::nothing;
}

VsValue	VsSFString::assign(const VsValue &v, int32_t index, VsFunctionCall *fcall)
{
	const GvString &s=	getField()->get();

    if (permission == VsField::RW && (index >=0 && index <  s.getLength())) {
		GvString c; 
		v.getString(c);
		
		// replace character 
		GvString news(s);
		if (c.getLength()>0) {
			news.set1(index,c[0]);
		}
		
		getField()->set_(news);
		return v;
	}
	else VS_RO_ERROR1(fcall);
	return VsValue::nothing;
}


// constructor init call 
VsValue VsSFString::initCall(VsFunctionCall *parent,VsExprList *args)
{
	int l= getLength();

	int numArgs = args->getLength();

	if (numArgs == 0) {

	}
	if (numArgs == 1) {
		VsValue v;
		if ((*args)[0] != NULL)	{ //  argument not passed 
			(*args)[0]->evaluate(v,parent);
			((VsVar*)this)->assign(v,parent);
		}
	}
#define MAX_ARG  8
	else if (numArgs <= MAX_ARG) {
		VsValue a[MAX_ARG];
		
		GvString as[MAX_ARG];

		GvString s;
		GvString result;
		int maxl;

		if (getArg(*args,0,parent,s)) { // get format string , must contain only %s

			maxl = s.getLength() + numArgs * 100;


			// Evaluate arguments:
			for (int i = 1; i < numArgs; i++) {
				if ((*args)[i] != NULL) {	//  argument not passed 
					 (*args)[i]->evaluate(a[i-1],parent);
					a[i-1].getString(as[i-1]);
				}
			}
			char *x=result.getBuffer(maxl+1);

			switch (numArgs) {
			case 1 : _snprintf(x,maxl,s.getString()); break;
			case 2 : _snprintf(x,maxl,s.getString(),as[0].getString()); break;
			case 3 : _snprintf(x,maxl,s.getString(),as[0].getString(),as[1].getString()); break;
			case 4 : _snprintf(x,maxl,s.getString(),as[0].getString(),as[1].getString(),as[2].getString()); break;
			case 5 : _snprintf(x,maxl,s.getString(),as[0].getString(),as[1].getString(),as[2].getString(),as[3].getString()); break;
			case 6 : _snprintf(x,maxl,s.getString(),as[0].getString(),as[1].getString(),as[2].getString(),as[3].getString(),as[4].getString()); break;
			case 7 : _snprintf(x,maxl,s.getString(),as[0].getString(),as[1].getString(),as[2].getString(),as[3].getString(),as[4].getString(),as[5].getString()); break;
			case 8 : _snprintf(x,maxl,s.getString(),as[0].getString(),as[1].getString(),as[2].getString(),as[3].getString(),as[4].getString(),as[5].getString(),as[6].getString()); break;
			case 9 : _snprintf(x,maxl,s.getString(),as[0].getString(),as[1].getString(),as[2].getString(),as[3].getString(),as[4].getString(),as[5].getString(),as[6].getString(),as[7].getString()); break;
			default : 
			
				Reporter.Error("string constructor not implemented in %s",parent->getContextString());
	
				break;
			}
			result.releaseBuffer();

			this->getField()->set(result);

		}
    }
#undef MAX_ARG
	else  {
		Reporter.Error("string constructor not implemented in %s",parent->getContextString());
	}

	return VsValue(this);
}

// invoke member function 
VsValue VsSFString::callMember(const VsMember &member,VsExprList *args, VsFunctionCall *fcall)
{	GvSFString *v1=getField();
 	if (member == "charAt") { 
		float l=0.0;
		if (getArg(*args,0,fcall,l)) {
			GvString v;
			int i=l;
			if (i>=0  && i<v1->value.getLength())
				v = v1->value[i];
			return VsValue(new VsSFString(v));
		} else BAD_ARG;
	}

	
	if (member == "indexOf") { 	
		GvString v2;
		if (getArg(*args,0,fcall,v2)) {
			int fromIndex=0;
			float l=0.0;
			int index = -1; // result index 

			if (getArg(*args,1,fcall,l)) {
				fromIndex = l;
			}
			index = v1->value.indexOf(v2,fromIndex);
			
			return VsValue(index);
		}
		else BAD_ARG;
	}
	if (member == "lastIndexOf") { 	
		GvString v2;
		if (getArg(*args,0,fcall,v2)) {
			int fromIndex=0;
			float l=0.0;
			int index = -1; // result index 

			if (getArg(*args,1,fcall,l)) {
				fromIndex = l;
			}
			else fromIndex = v1->value.getLength();

			index = v1->value.lastIndexOf(v2,fromIndex);
			
			return VsValue(index);
		}
		else BAD_ARG;
	}
	if (member == "substring") { 	
		float index1=0,index2=-1;
		float l;

		if (getArg(*args,0,fcall,l)) {
			index1=l;
			GvString s;

			if (getArg(*args,1,fcall,l)) {
				index2 = l;
			}
			else index2 = v1->value.getLength();

			s.setSubstring(v1->get(),index1,index2);
			return VsValue(new VsSFString(s));

		}
		else BAD_ARG;
	}
	if (member == "toLowerCase") { 	
		if (getArg(*args,fcall)) {
			
			GvString s;
			s = v1->get();
			s.toLowerCase();

			return VsValue(new VsSFString(s));
		}
		else BAD_ARG;
	}
	if (member == "toUpperCase") { 	
		if (getArg(*args,fcall)) {
			
			GvString s;
			s = v1->get();
			s.toUpperCase();

			return VsValue(new VsSFString(s));
		}
		else BAD_ARG;
	}
	return VsVar::callMember(member,args,fcall);

bad_arg:
	Reporter.Error("SFString:Bad Argument for function %s . (Num Args = %d) in %s",member.getString(),args->getLength(),fcall->getContextString()); 
	return VsValue::nothing; 
}


/* from VRMLScript doku, need to check out JavaScript String object


5.9 SFString Object

The SFString object corresponds to a VRML 2.0 SFString field.

Instance Creation Method(s)



•sfStringObjectName = new SFString(number) 
•number is any scalar expression 
•sfStringObjectName = new SFString(format-string, expr, expr, ...) 
•format-string describes the format of the string, using the special % characters to serve as placeholders for the values of expressions, just like in the C stdio library function printf. •expr, expr, ... are the variable number of expression arguments that serve as the values substituted into the placeholders. For example, the following results in the SFString with value "http://foo29.wrl" being created and assigned to a: 


   b = 29;
   a = SFString("http://foo%d.wrl", b);






Properties



•length 
•An integer containing the number of characters in the string 




Methods



charAt(index) 
Returns a SFString containing the character of the string at position index in the string. index is an integer-valued expressio between 0 and length-1, where length is the number of characters in the string 

indexOf(string, [fromIndex]) 
Returns the index of the first occurrence of string in the object, 
starting the search from fromIndex. 
fromIndex must be between 0 and length -1, where length is the number of characters in the string. If fromIndex is not specified, 
the search will start with character 0. 

lastIndexOf(string, [fromIndex]) 
Returns the index of the first occurrence of string in the object, 
starting the search backwards from fromIndex. 
fromIndex must be between 0 and length -1, where length is the number of characters in the string. If fromIndex is not specified, 
the search will start with character length - 1. 

substring(index1, index2) 
Returns a SFString containing the desired substring. 
If index1 is less than index2, the substring returned starts 
with the character at index1 and ends with the character before index2. 
If index1 is greater than index2, the substring returned starts with the character at index2 and ends with the character before index1. If index1 equals index2, the empty string is returned. index1 and index2 are any integer-valued expressions where 0<=index1, index2<length, where length is the number of characters in the string 

toLowerCase() 
Returns a SFString with all alphabetic characters of the string converted to lower case. 

toUpperCase() 
Returns a SFString with all alphabetic characters of the string converted to upper case. 




Special Operators



•+ (addition) 

*/


// GvSFVec2f
void
VsSFVec2f::evaluate(VsValue &ret, int32_t index, VsFunctionCall *fcall)
{									   
	if (index >=0 && index < getLength()) ret = VsValue(getField()->get1(index));
	else ret = VsValue::nothing;

}

VsValue	VsSFVec2f::assign(const VsValue &v, int32_t index, VsFunctionCall *fcall)
{
    if (permission == VsField::RW && (index >=0 && index < getLength())) {
		switch (v.type) {
		case VsValue::vNUMBER: 	getField()->set1(index,v.number); return(v);
		default: break;
		}
	}
	return VsValue::nothing;
}


VsValue VsSFVec2f::evaluateMember(const VsMember &member, VsFunctionCall *fcall)
{
	VsValue ret;
	if (member == VsNames::x) { evaluate(ret,0,fcall); return ret; }
	else if (member == VsNames::y) { evaluate(ret,1,fcall); return ret; }
	return VsField::evaluateMember(member,fcall);

}

VsValue	VsSFVec2f::assignMember(const VsMember &member, const VsValue &v, VsFunctionCall *fcall)
{
 	if (member == VsNames::x) 	return assign(v,0,fcall);
	else if (member == VsNames::y) return assign(v,1,fcall);
	return VsField::assignMember(member,v,fcall);

}

/*
SFVec2f add(SFVec2f vec)Returns the value of the passed value added, component-wise, to the object.
SFVec2f divide(numeric n)Returns the value of the object divided by the passed value.
numeric dot(SFVec2f vec)Returns the dot product of this vector and the passed value.
numeric length( )Returns the geometric length of this vector.
SFVec2f multiply(numeric n)Returns the value of the object multiplied by the passed value.
SFVec2f normalize( )Returns the object converted to unit length .
SFVec2f subtract(SFVec2f vec)Returns the value of the passed value subtracted, component-wise, from the object.
String toString( )Returns a String containing the VRML 97 utf8 encoded value of x and y.

*/
// invoke member function 
VsValue VsSFVec2f::callMember(const VsMember &member,VsExprList *args, VsFunctionCall *fcall)
{	GvSFVec2f *v1=getField();
	GvSFVec2f *v2;
	VsValue v;

 	if (member == VsNames::add) { 	
		if (getArg(*args,fcall,v,v2)) 
			return VsValue(new VsSFVec2f(v1->value[0]+v2->value[0],v1->value[1]+v2->value[1]));
		else BAD_ARG;
	}
 	if (member == VsNames::subtract) { 	
		if (getArg(*args,fcall,v,v2)) 
			return VsValue(new VsSFVec2f(v1->value[0]-v2->value[0],v1->value[1]-v2->value[1]));
		else BAD_ARG;
	}
 	if (member == VsNames::dot) { 	
		if (getArg(*args,fcall,v,v2)) 
			return VsValue(v1->value[0]*v2->value[0]+v1->value[1]*v2->value[1]);
		else BAD_ARG;
	}
 	if (member == VsNames::length) { 	
		if (getArg(*args,fcall)) { 
			double l= v1->value[0]*v1->value[0]+v1->value[1]*v1->value[1];
			return VsValue(sqrt(l));
		} else BAD_ARG;
	}
 	if (member == VsNames::normalize) { 	
		if (getArg(*args,fcall)) {
			double l= v1->value[0]*v1->value[0]+v1->value[1]*v1->value[1];
			l = sqrt(l);
			if (l!=0.0) l = 1.0/l;
			return VsValue(new VsSFVec2f(l*v1->value[0],l*v1->value[1]));
		} else BAD_ARG;
	}
 	if (member == VsNames::negate) { 	
		if (getArg(*args,fcall)) {
			const float l = -1.0;
			return VsValue(new VsSFVec2f(l*v1->value[0],l*v1->value[1]));
		} else BAD_ARG;
	}
 	if (member == VsNames::multiply) { 
		float l=1.0;
		if (getArg(*args,0,fcall,l)) {
			return VsValue(new VsSFVec2f(l*v1->value[0],l*v1->value[1]));
		} else BAD_ARG;
	}
 	if (member == VsNames::divide) { 
		float l=1.0;
		if (getArg(*args,0,fcall,l)) {
			if (l!=0.0) l = 1.0/l;
			return VsValue(new VsSFVec2f(l*v1->value[0],l*v1->value[1]));
		} else BAD_ARG;
	}
	return VsVar::callMember(member,args,fcall);

bad_arg:
	Reporter.Error("SFVec2f:Bad Argument for function %s in %s",member.getString(),fcall->getContextString()); 
	return VsValue::nothing; 
}


// GvSFVec3f
void 
VsSFVec3f::evaluate(VsValue &ret, int32_t index, VsFunctionCall *fcall)
{									   
	if (index >=0 && index < getLength()) 
		ret =VsValue(getField()->get1(index));
	else ret = VsValue::nothing;

};
VsValue	VsSFVec3f::assign(const VsValue &v, int32_t index, VsFunctionCall *fcall)
{
    if (permission == VsField::RW && (index >=0 && index < getLength())) {
		switch (v.type) {
		case VsValue::vNUMBER: 	getField()->set1(index,v.number); return(v);
		default: break;
		}
	}
	return VsValue::nothing;
}


VsValue VsSFVec3f::evaluateMember(const VsMember &member, VsFunctionCall *fcall)
{
	VsValue ret;
	if (member == VsNames::x) { evaluate(ret,0,fcall);  return ret; }
	else if (member == VsNames::y) { evaluate(ret,1,fcall);  return ret; }
	else if (member == VsNames::z) { evaluate(ret,2,fcall);  return ret; }
	return VsField::evaluateMember(member,fcall);

}

VsValue	VsSFVec3f::assignMember(const VsMember &member, const VsValue &v, VsFunctionCall *fcall)
{
 	if (member == VsNames::x) 	return assign(v,0,fcall);
	else if (member == VsNames::y) return assign(v,1,fcall);
	else if (member == VsNames::z) return assign(v,2,fcall);
	return VsField::assignMember(member,v,fcall);

}



/*
SFVec3f add(SFVec3f vec)Returns the value of the passed value added, component-wise, to the object.
SFVec3f cross(SFVec3f vec)Returns the cross product of the object and the passed value.
SFVec3f divide(numeric n)Returns the value of the object divided by the passed value.
numeric dot(SFVec3f vec)Returns the dot product of this vector and the passed value.
numeric length( )Returns the geometric length of this vector.
SFVec3f multiply(numeric n)Returns the value of the object multiplied by the passed value.
SFVec3f negate( )Returns the value of the component-wise negation of the object.
SFVec3f normalize( )Returns the object converted to unit length .
SFVec3f subtract(SFVec3f vec)Returns the value of the passed value subtracted, component-wise, from the object.
String toString( )Returns a String containing the VRML 97 utf8 encoded value of x, y, and z.
*/


// invoke member function 
VsValue VsSFVec3f::callMember(const VsMember &member,VsExprList *args, VsFunctionCall *fcall)
{	GvSFVec3f *v1=getField();
	GvSFVec3f *v2;
	VsValue v; 

	//TRACE(" Field by ref in use : %d \n",fieldCnt);


 	if (member == VsNames::add) { 	
		if (getArg(*args,fcall,v,v2)) 
			return VsValue(new VsSFVec3f(v1->value[0]+v2->value[0],v1->value[1]+v2->value[1],v1->value[2]+v2->value[2]));
		else BAD_ARG;
	}
 	if (member == VsNames::subtract) { 	
		if (getArg(*args,fcall,v,v2)) 
			return VsValue(new VsSFVec3f(v1->value[0]-v2->value[0],v1->value[1]-v2->value[1],v1->value[2]-v2->value[2]));
		else BAD_ARG;
	}
	if (member == VsNames::dot) { 	 // DotProd
		if (getArg(*args,fcall,v,v2)) 
			return VsValue(v1->value[0]*v2->value[0]+v1->value[1]*v2->value[1]+v1->value[2]*v2->value[2]);
		else BAD_ARG;
	}
 	if (member == VsNames::length) { 	
		if (getArg(*args,fcall)) { 
			double l= v1->value[0]*v1->value[0]+v1->value[1]*v1->value[1]+v1->value[2]*v1->value[2];
			return VsValue(sqrt(l));
		} else BAD_ARG;
	}
 	if (member == VsNames::normalize) { 	
		if (getArg(*args,fcall)) {
			double l= v1->value[0]*v1->value[0]+v1->value[1]*v1->value[1]+v1->value[2]*v1->value[2];
			l = sqrt(l);
			if (l!=0.0) l = 1.0/l;
			return VsValue(new VsSFVec3f(l*v1->value[0],l*v1->value[1],l*v1->value[2]));
		} else BAD_ARG;
	}
 	if (member == VsNames::negate) { 	
		if (getArg(*args,fcall)) {
			const float l = -1.0;
			return VsValue(new VsSFVec3f(l*v1->value[0],l*v1->value[1],l*v1->value[2]));
		} else BAD_ARG;
	}
 	if (member == VsNames::multiply) { 
		float l=1.0;
		if (getArg(*args,0,fcall,l)) {
			return VsValue(new VsSFVec3f(l*v1->value[0],l*v1->value[1],l*v1->value[2]));
		} else BAD_ARG;
	}
 	if (member == VsNames::divide) { 
		float l=1.0;
		if (getArg(*args,0,fcall,l)) {
			if (l!=0.0) l = 1.0/l;
			return VsValue(new VsSFVec3f(l*v1->value[0],l*v1->value[1],l*v1->value[2]));
		} else BAD_ARG;
	}
 	if (member == VsNames::cross) { 
		if (getArg(*args,fcall,v,v2)) 
			return VsValue(new VsSFVec3f(CrossProd(*v1,*v2)));
		else BAD_ARG;
	}
 	if (member == "noise") { 	
		if (getArg(*args,fcall)) {
			float f=Noise3(v1->value[0]);
			return VsValue(f);
		} else BAD_ARG;
	}
 	if (member == "turbulence") { 
		float oktaves=4;
		float f;
		getArg(*args,0,fcall,oktaves);

		f=turbulence(v1->value,oktaves);
		return VsValue(f);
	}

	return VsVar::callMember(member,args,fcall);

bad_arg:
	Reporter.Error("SFVec3f:Bad Argument for function %s . (Num Args = %d) in %s",member.getString(),args->getLength(),fcall->getContextString()); 
	return VsValue::nothing; 
}


// VsSFColor
void
VsSFColor::evaluate(VsValue &ret, int32_t index, VsFunctionCall *fcall)
{									   
	if (index >=0 && index < getLength()) 
		ret = VsValue(getField()->get1(index));
	else ret = VsValue::nothing;

};
VsValue	VsSFColor::assign(const VsValue &v, int32_t index, VsFunctionCall *fcall)
{
    if (permission == VsField::RW && (index >=0 && index < getLength())) {
		switch (v.type) {
		case VsValue::vNUMBER: 	getField()->set1(index,v.number); return(v);
		default: break;
		}
	}
	return VsValue::nothing;
}


VsValue VsSFColor::evaluateMember(const VsMember &member, VsFunctionCall *fcall)
{
	VsValue ret;
	if (member == VsNames::r) { evaluate(ret,0,fcall); return ret; }
	else if (member == VsNames::g) { evaluate(ret,1,fcall); return ret; }
	else if (member == VsNames::b) { evaluate(ret,2,fcall); return ret; }
	return VsField::evaluateMember(member,fcall);

}

VsValue	VsSFColor::assignMember(const VsMember &member, const VsValue &v, VsFunctionCall *fcall)
{
 	if (member == VsNames::r) 	return assign(v,0,fcall);
	else if (member == VsNames::g) return assign(v,1,fcall);
	else if (member == VsNames::b) return assign(v,2,fcall);
	return VsField::assignMember(member,v,fcall);

}



//void setHSV(float h, float s, float v)  
//numeric[3] getHSV( ) 
//String toString( ) 
// invoke member function 
VsValue VsSFColor::callMember(const VsMember &member,VsExprList *args, VsFunctionCall *fcall)
{	GvSFColor *v1=getField();

	if (member == "setHSV") {
		// ARGCHECK(3)

		float h=1.0,s=0,v=0;
		float rgb[3];
		getArg(*args,0,fcall,h);
		getArg(*args,1,fcall,s);
		getArg(*args,2,fcall,v);
		Hsv2Rgb(h,s,v,rgb);

		  // do a generic assign 
		VsValue newValue(new VsSFColor(rgb[0],rgb[1],rgb[2]));

		((VsVar *)this)->assign(newValue,fcall);
  
		return VsValue::nothing;
	}
	if (member == "getHSV") {
		// ARGCHECK(0)
		float hsv[3];

		Rgb2Hsv(v1->get(),hsv[0],hsv[1],hsv[2]);

		return VsValue(new VsSFVec3f(hsv[0],hsv[1],hsv[2]));
	}


	return VsVar::callMember(member,args,fcall);
/*
bad_arg:
	Reporter.Error("SFColor:Bad Argument for function %s . (Num Args = %d)\n",member.getString(),args->getLength()); 
	return VsValue::nothing; 
*/
}


// VsSFRotation
void VsSFRotation::evaluate(VsValue &ret, int32_t index, VsFunctionCall *fcall)
{									   
	if (index >=0 && index < getLength()) 
		ret = VsValue(getField()->get1(index));
	else ret = VsValue::nothing;

};
VsValue	VsSFRotation::assign(const VsValue &v, int32_t index, VsFunctionCall *fcall)
{
    if (permission == VsField::RW && (index >=0 && index < getLength())) {
		switch (v.type) {
		case VsValue::vNUMBER: 	getField()->set1(index,v.number); return(v);
		default: break;
		}
	}
	return VsValue::nothing;
}


VsValue VsSFRotation::evaluateMember(const VsMember &member, VsFunctionCall *fcall)
{
	VsValue ret;
	if (member == VsNames::x) { evaluate(ret,0,fcall); return ret; }
	else if (member == VsNames::y) { evaluate(ret,1,fcall); return ret; }
	else if (member == VsNames::z) { evaluate(ret,2,fcall); return ret; }
	else if (member == VsNames::angle) { evaluate(ret,3,fcall); return ret; }
	return VsField::evaluateMember(member,fcall);

}


VsValue	VsSFRotation::assignMember(const VsMember &member, const VsValue &v, VsFunctionCall *fcall)
{
 	if (member == VsNames::x) 	return assign(v,0,fcall);
	else if (member == VsNames::y) return assign(v,1,fcall);
	else if (member == VsNames::z) return assign(v,2,fcall);
	else if (member == VsNames::angle) return assign(v,3,fcall);
	return VsField::assignMember(member,v,fcall);

}
/*
SFVec3f getAxis( )Returns the axis of rotation.
SFRotation inverse( )Returns the inverse of this object's rotation.
SFRotation multiply(SFRotation rot)Returns the object multiplied by the passed value.
SFVec3f multVec(SFVec3f vec)Returns the value of vec multiplied by the matrix corresponding to this object's rotation.void setAxis(SFVec3f vec)Sets the axis of rotation to the value passed in vec.SFRotation slerp(SFRotation dest, numeric t) Returns the value of the spherical linear interpolation between this object's rotation and dest at value 0 <= t <= 1. For t = 0, the value is this object's rotation. For t = 1, the value is dest.
String toString( )Returns a String containing the VRML 97 utf8 encoded value of x, y, z, and angle.
*/

// invoke member function 
VsValue VsSFRotation::callMember(const VsMember &member,VsExprList *args, VsFunctionCall *fcall)
{
	if (member == "getAxis") {
		// ARGCHECK(0)
		const Rotation &r =getField()->value;
		return VsValue(new VsSFVec3f(r.Axis()));
	}
	if (member == "setAxis") {
		// ARGCHECK(1)
		Rotation r =getField()->value;
		Point p(0,1,0);
		getArg(*args,0,fcall,p);
		r.setAxis(p);
		  // do a generic assign 
		VsValue newValue(new VsSFRotation(r));

		((VsVar *)this)->assign(newValue,fcall);
  
		return VsValue::nothing;
	}

	if (member == "inverse") {
		// ARGCHECK(0)
		const Rotation &r =getField()->value;
		Quaternation q;
		r.get(q);
		Rotation r2;
		r2.set(Invert(q));
		return VsValue(new VsSFRotation(r2));
	}
	if (member == "multiply") {
		// ARGCHECK(1)
		const Rotation &r =getField()->value;
		Rotation r1; getArg(*args,0,fcall,r1);

		Quaternation q,q1;
		r.get(q); r1.get(q1);
		Rotation r2;
		//r2.set(q*q1);
		r2.set(q1*q); // 6.05.98 Big Problem with FOLLOW PROTO 

		return VsValue(new VsSFRotation(r2));
	}
	if (member == "slerp") {
		// ARGCHECK(2)
		const Rotation &r =getField()->value;
		Rotation r1; getArg(*args,0,fcall,r1);
		float t=0.5;   getArg(*args,1,fcall,t);

		Quaternation q,q1,q2;
		r.get(q); r1.get(q1);
		Rotation r2;
		q2.Interpolate(q,q1,t);
		r2.set(q2);

		return VsValue(new VsSFRotation(r2));
	}
	if (member == "multVec") {
		// ARGCHECK(1)
		Matrix m;
		Point p(1,1,1);
		getField()->get(m);
		getArg(*args,0,fcall,p);
		p= m * p; 
		return VsValue(new VsSFVec3f(p));
	}
	return VsVar::callMember(member,args,fcall);
}

VsValue VsSFRotation::initCall(VsFunctionCall *parent,VsExprList *args)
{
	int numArgs = args->getLength();

	if (numArgs == 0) {
		// default 
	}
	else if (numArgs >= 2) {
		if ((*args)[0] != NULL)	{ //  argument not passed 
			VsValue v;
			(*args)[0]->evaluate(v,parent);
			if (v.type == VsValue::vFIELD && (SFVEC3F == v.field->getFieldType())) {
				// first argument is SFVEC3F, special form 
				VsValue v1;
				Point axis;

				getArg(*args,0,parent,axis);

				if ((*args)[1] != NULL) (*args)[1]->evaluate(v1,parent);
				if (v1.type == VsValue::vFIELD && (SFVEC3F == v1.field->getFieldType())) {
					// two vectors 
					Point axis1;
					getArg(*args,1,parent,axis1);
					Quaternation q;
					q.Set(axis,axis1);
					getField()->set(q);
				}
				else  {
					float angle=0.0;
					// axis angle 
					getArg(*args,1,parent,angle);
					getField()->set(Rotation(axis.x,axis.y,axis.z,angle));

				}

			} else {
				return VsField::initCall(parent,args); // array member wise 
				// must return value immediately, other wise this will be destroyed by temp  deref to 0
			}
		}
	}
	else  {
		Reporter.Error("bad SFRotation constructor in %s",parent->getContextString());
	}

	return VsValue(this);
}


/*
C.6.5 SFImage object

C.6.5.1 Description

The SFImage object corresponds to a VRML SFImage field.

C.6.5.2 Instance creation method

sfImageObjectName = new SFImage(numeric x, numeric y, numeric comp, MFInt32 array) 

where

x is the x-dimension of the image. y is the y-dimension of the image. comp is the number of components of the image (1 for greyscale, 2 for greyscale+alpha, 3 for rgb, 4 for rgb+alpha). Array contains the x x y values for the pixels of the image. Format of each pixel is the same as the PixelTexture file format. 

C.6.5.3 Properties

The properties of the SFImage object are listed in Table C.4.

Table C.4: SFImage properties


PropertyDescriptionnumeric 
xx dimension of the imagenumeric yy dimension of the imagenumeric comp 

number of components of the image: 

1: greyscale 
2: greyscale + alpha 
3: rgb 
4: rgb + alpha 

MFInt32 arrayimage data


SFImage method


MethodDescriptionString toString( )Returns a String containing the VRML 97 UTF-8 encoded value of x, y, comp and array. 
*/

VsValue VsSFImage::initCall(VsFunctionCall *parent,VsExprList *args)
{
	int numArgs = args->getLength();
	int x=2;
	int y=2;
	int comp=1;
	GvMFInt32 *data = NULL;

	if (numArgs == 0) {
		// default 
	}
	else if (numArgs >= 2) 
	{
		if ( getArg(*args,0,parent,x)
			 && getArg(*args,1,parent,y)
			 && getArg(*args,2,parent,comp)
			 ) {
			if ((*args)[3] != NULL)	{ //  argument passed 
				VsValue v;
				(*args)[3]->evaluate(v,parent);
				if (v.type == VsValue::vFIELD && (MFINT32 == v.field->getFieldType())) {
					data =  (GvMFInt32 *) v.field->getField();
					if (data->getNum()>= (x*y)) {
						this->getField()->set(x,y,comp,( const unsigned long *)data->get());
						return VsValue(this);
					}
				}
				if (v.type == VsValue::vFIELD && (MFFLOAT == v.field->getFieldType())) {
					GvMFFloat * data =  (GvMFFloat *) v.field->getField();
					if (data->getNum()>= (x*y*comp)) {
						this->getField()->set(x,y,comp,data->get());
						return VsValue(this);
					}
				}
			}
		}
		if (x>0 && y >0 && comp>0) {
			this->getField()->set(x,y,comp,(const unsigned long *)NULL);
			return VsValue(this);
		}
	}
	else  {
		Reporter.Error("bad SFImage constructor in %s",parent->getContextString());
	}

	return VsValue(this);
}


void
VsSFImage::evaluate(VsValue &ret, int32_t index, VsFunctionCall *fcall)
{									   
	if (index >=0 && index < getLength()) 
		ret = VsValue(getField()->get1(index));
	else ret =  VsValue::nothing;

};
VsValue	VsSFImage::assign(const VsValue &v, int32_t index, VsFunctionCall *fcall)
{
    if (permission == VsField::RW && (index >=0 && index < getLength())) {
		switch (v.type) {
		case VsValue::vNUMBER: 	getField()->set1(index,(unsigned long) v.number); return(v);
		default: break;
		}
	}
	return VsValue::nothing;
}


VsValue VsSFImage::evaluateMember(const VsMember &member, VsFunctionCall *fcall)
{
	GvSFImage *im = getField();
	if (member == VsNames::x) return VsValue(im->size[0]);
	else if (member == VsNames::y) return VsValue(im->size[1]);
	else if (member == VsNames::comp) return VsValue(im->numComponents);
	else if (member == VsNames::array) {
		int l= im->size[0]*im->size[1];
		GvMFInt32 *v= new GvMFInt32(l);
		im->getData(l, (unsigned long *) v->get());
		return VsValue(new VsMFInt32(v,VsField::RW,0));
	}
	else if (member == VsNames::length) return VsValue(im->getDataLength());
	return VsField::evaluateMember(member,fcall);

}

VsValue	VsSFImage::assignMember(const VsMember &member, const VsValue &v, VsFunctionCall *fcall)
{
	GvSFImage *im = getField();
	int val;
	if (member == VsNames::x) {
		if (v.type == VsValue::vNUMBER && v.number >0) {
			val=v.number;
			if (im->getDataLength() < im->getDataLength(val,im->size[1],im->numComponents)) 
				im->FlushData();
			im->size[0]=val;
			im->OnChanged();
			return VsValue(im->size[0]);

		}	
	}
	else if (member == VsNames::y) { 
		if (v.type == VsValue::vNUMBER && v.number >0) {
			val=v.number;
			if (im->getDataLength() < im->getDataLength(val,im->size[1],im->numComponents)) 
				im->FlushData();
			im->size[1]=val;
			im->OnChanged();
			return VsValue(im->size[1]);

		}	
	}
	else if (member == VsNames::comp) {
		if (v.type == VsValue::vNUMBER && v.number >0) {
			val=v.number;
			if (im->getDataLength() < im->getDataLength(val,im->size[1],im->numComponents)) 
				im->FlushData();
			im->numComponents=val;
			im->OnChanged();

			return VsValue(im->numComponents);
		}
	}
	else if (member == VsNames::array) {
			if (v.type == VsValue::vFIELD && (MFINT32 == v.field->getFieldType())) {
				GvMFInt32 *data =  (GvMFInt32 *) v.field;
				if (data->getNum()>= (im->size[0]*im->size[0])) {
					this->getField()->set(im->size[0],im->size[1],im->numComponents,( const unsigned long *)data->get());
					return VsValue(this);
					}
				}
	}

	return VsField::assignMember(member,v,fcall);

}

// invoke member function 
VsValue VsSFImage::callMember(const VsMember &member,VsExprList *args, VsFunctionCall *fcall)
{
	return VsVar::callMember(member,args,fcall);

}

//
//
//




VsSFMatrix::VsSFMatrix(Matrix &m,Permission perm): VsField(new GvSFMatrix(),perm,0)
{

	getField()->set(m);
}

// constructor init call 
VsValue 
VsSFMatrix::initCall(VsFunctionCall *parent,VsExprList *args)
{
	int l= getLength();

	int numArgs = args->getLength();
	
	Matrix m;

	m.Identity();

	if (numArgs == 0) { // identity matrix if no argument 
		getField()->set(m);
	}
	else  {
		if (numArgs > 16) numArgs = 16;
		VsValue v;

		// Evaluate arguments:
		for (int i = 0; i < numArgs; i++) {
			if ((*args)[i] != NULL)	{//  argument not passed 
				(*args)[i]->evaluate(v,parent);
				assign(v,i,parent);
			}
		}

	}

	return VsValue(this);
}


// invoke member function 
VsValue VsSFMatrix::callMember(const VsMember &member,VsExprList *args, VsFunctionCall *fcall)
{
	if (member == "setTransform") {
		Point translation(0,0,0),scale(1,1,1),center(0,0,0);
		Rotation rotation,scaleOrientation;
		Matrix m;
		getArg(*args,0,fcall,translation);
		getArg(*args,1,fcall,rotation);
		getArg(*args,2,fcall,scale);
		getArg(*args,3,fcall,scaleOrientation);
		getArg(*args,4,fcall,center);

		setTransform(m,translation,rotation,scale,scaleOrientation,center);
		getField()->set(m);
		return VsValue::nothing;
	}
	if (member == "getTransform") {
		Point translation(0,0,0),scale(1,1,1),shear;
		Rotation rotation;
		Matrix m;
		getField()->get(m);

		DecomposeMatrix(m,scale,shear,rotation,translation);
		
		setArg(*args,0,fcall,translation);
		setArg(*args,1,fcall,rotation);
		setArg(*args,2,fcall,scale);

		return VsValue::nothing;
	}
	if (member == "inverse") {
		Matrix m;
		getField()->get(m);
		m.Invert();
		return VsValue(new VsSFMatrix(m));
	}
	if (member == "transpose") {
		Matrix m;
		getField()->get(m);
		m.Transpose();
		return VsValue(new VsSFMatrix(m));
	}
	if (member == "multLeft") {
		Matrix m,m1;
		getField()->get(m);

		// ARGCHECK(1)
		getArg(*args,0,fcall,m1);
		m = m1 * m;
		return VsValue(new VsSFMatrix(m));
	}
	if (member == "multRight") {
		Matrix m,m1;
		getField()->get(m);

		// ARGCHECK(1)
		getArg(*args,0,fcall,m1);
		m = m * m1;
		return VsValue(new VsSFMatrix(m));
	}
	if (member == "multVecMatrix") {
		Matrix m;
		getField()->get(m);
		Point p(0,0,0);

		// ARGCHECK(1)
		getArg(*args,0,fcall,p);
		p= m * p;
		return VsValue(new VsSFVec3f(p));
	}
	if (member == "multMatrixVec") {
		Matrix m;
		getField()->get(m);
		Point p(0,0,0);

		// ARGCHECK(1)
		getArg(*args,0,fcall,p);
		p= m * p; // ????? to do: check 
		return VsValue(new VsSFVec3f(p));
	}


	return VsVar::callMember(member,args,fcall);
}



/*
VrmlMatrixObjectName = new VrmlMatrix(
                                                                      numeric f11, numeric f12, numeric f13, numeric f14,
                                                                      numeric f21, numeric f22, numeric f23, numeric f24, 
                                                                      numeric f31, numeric f32, numeric f33, numeric f34, 
                                                                      numeric f41, numeric f42, numeric f43, numeric f44) 

A new matrix initialized with the values in f11 through f44 is created and returned. The translation values will be f41, f42, and f43.

VrmlMatrixObjectName = new VrmlMatrix( ) 

A new matrix initialized with the identity matrix is created and returned. 

*/


/*
MethodDescription
void setTransform(SFVec3f translation,
                        SFRotation rotation,
                        SFVec3f scale, 
                        SFRotation scaleOrientation, 
                        SFVec3f center) Sets the VrmlMatrix to the passed values. Any of the rightmost parameters may be omitted. The method has 0 to 5 parameters. For example, specifying 0 parameters results in an identity matrix while specifying 1 parameter results in a translation and specifying 2 parameters results in a translation and a rotation. Any unspecified parameter is set to its default as specified for the Transform node. 
void getTransform(SFVec3f translation,
                         SFRotation rotation, 
                         SFVec3f scale) 
		Decomposes the VrmlMatrix and returns the components in the passed translation, rotation, and scale objects. 
		The types of these passed objects is the same as the first three arguments to setTransform. 
		If any passed object is not sent, or if the null object is sent for any value, that value is not returned. 
		Any projection or shear information in the matrix is ignored. 
VrmlMatrix inverse( ) Returns a VrmlMatrix whose value is the inverse of this object. 
VrmlMatrix transpose( ) Returns a VrmlMatrix whose value is the transpose of this object. 
VrmlMatrix multLeft(VrmlMatrix matrix) Returns a VrmlMatrix whose value is the object multiplied by the passed matrix on the left. 
VrmlMatrix multRight(VrmlMatrix matrix) Returns a VrmlMatrix whose value is the object multiplied by the passed matrix on the right. 
SfVec3f multVecMatrix(SFVec3f vec) Returns an SFVec3f whose value is the object multiplied by the passed row vector. 
SFVec3f multMatrixVec(SFVec3f vec) Returns an SFVec3f whose value is the object multiplied by the passed column vector. 
String toString( ) 

  */
// VsSFFNode

void VsSFNode::evaluate(VsValue &ret, VsFunctionCall *)
{
//    return VsValue(getField()->get());
    ret = VsValue(this); // now return as VsSFNode !
}

GvBool VsSFNode::evaluateBool()
{
	return (getField()->get()) != (GvNode *) NULL;
}


VsValue VsSFNode::assign(const VsValue &v, VsFunctionCall *fcall)
{
    if (permission == VsField::RW) {
		switch (v.type) {
		case VsValue::vVOID:
			getField()->set((GvNode*)NULL);	return(v);
		case VsValue::vNUMBER:
			getField()->set((GvNode*)NULL);	return(v);

		case VsValue::vNODE: 	getField()->set(v.node);	return(v);
 
		default: return VsField::assign(v,fcall);
		}
	}
	return VsValue::nothing;
}


VsValue
VsSFNode::assignMember(const VsMember &member, const VsValue &v, VsFunctionCall *fcall)
{

	GvNode *to = getField()->get();	// get the target node 

	if (to == NULL) return VsValue::nothing;

	GvField *fromField = NULL;
	VsFieldClass fromFieldType;

	if (v.type == VsValue::vFIELD) {
		fromField = v.field->getField();
		fromFieldType = v.field->getFieldType();

	}

    GvFieldData* toData = to->getFieldData();

	int toIndex = toData->getEventInIndex(to,member);
	
	if (toIndex<0) {
		Reporter.Error("Node %s has no eventIn %s in %s",to->ClassName(),member.getString(),fcall->getContextString());
		return VsValue::nothing;
	}
    
	GvFieldEntry *toFieldEntry = toData->getFieldEntry(toIndex);
    GvField *toField = to->getField(toIndex);


	VsField *tmpField=NULL;

	if (fromField == NULL) { // we have to field 
		tmpField = VsField::create(toFieldEntry->fieldClass,RW,NULL,0);
		tmpField->ref();

		tmpField->assign(v,fcall);

		fromField = tmpField->getField();
	}
	else if (toFieldEntry->fieldClass != fromFieldType) { // type mismatch 
		Reporter.Error("type mismatch for member %s in %s ",member.getString(), fcall->getContextString());
		return VsValue::nothing;
	}

	// verify types 						GvField

    ASSERT(fromField);

    if (!fromField) {
		Reporter.Error("Node %s.%s bad value in %s",to->ClassName(),member.getString(),fcall->getContextString());
		return VsValue::nothing;
	}


    switch(toFieldEntry->type) {
    case GV_NODE_EVENT_IN: 
			{
            GvNodeMethodConnection *c = new GvNodeMethodConnection(to, (GvNodeEventInMethod ) (toFieldEntry->method));
            c->OnTrigger(fromField);
			delete c;
			}
            break;
    default:
        {
            ASSERT(toField);
			toField->set(fromField);
        }
        break;
    }

    if (tmpField) 
		tmpField->unref(); // discard tempfield 

	return(v);
}

VsValue		
VsSFNode::evaluateMember(const VsMember &member, VsFunctionCall *fcall)
{
	GvNode *to = getField()->get();	// get the target node 

	if (to == NULL) return VsValue::nothing;


    GvFieldData* toData = to->getFieldData();

	int toIndex = toData->getEventOutIndex(to,member);
	
	if (toIndex<0) {
		Reporter.Error("Node %s has no eventOut %s in %s",to->ClassName(),member.getString(),fcall->getContextString());
		return VsValue::nothing;
	}
    
	GvFieldEntry *toFieldEntry = toData->getFieldEntry(toIndex);
    GvField *toField = to->getField(toIndex);

	// evaluate simple types ?? SFBool SFInt 

	ASSERT(toField != NULL);

	switch(toFieldEntry->fieldClass) {
	case SFFLOAT :  return VsValue(((GvSFFloat*) toField)->get());
	case SFTIME :  return VsValue(((GvSFTime*) toField)->get());
	case SFINT32 :  return VsValue(((GvSFInt32*) toField)->get());
	case SFLONG :  return VsValue(((GvSFLong*) toField)->get());
	case SFBOOL :  return VsValue(((GvSFBool*) toField)->get());
	//case SFSTRING :  return VsValue(((GvSFString*) toField)->get());


	default : break;
	}


	VsField *tmpField=NULL;

	// create field by ref 
	tmpField = VsField::create(toFieldEntry->fieldClass,RW,toField,1);
	return VsValue(tmpField);

}

// get Var representing the node's EventOut 
VsVar* 
VsSFNode::evaluateMemberVar(const VsMember &member, VsFunctionCall *fcall)
{
	GvNode *to = getField()->get();	// get the target node 
	if (to == NULL) return NULL;

    GvFieldData* toData = to->getFieldData();

	int toIndex = toData->getEventOutIndex(to,member);
	
	if (toIndex<0) {
		Reporter.Error("Node %s has no eventOut %s in %s",to->ClassName(),member.getString(),fcall->getContextString());
		return NULL;
	}
    
	GvFieldEntry *toFieldEntry = toData->getFieldEntry(toIndex);
    GvField *toField = to->getField(toIndex);
	VsField *tmpField=NULL;
	// create field by ref 
	tmpField = VsField::create(toFieldEntry->fieldClass,RW,toField,1);
	return tmpField;

}

// constructor init call 
VsValue VsSFNode::initCall(VsFunctionCall *parent,VsExprList *args)
{
	int numArgs = args->getLength();

	if (numArgs == 0) {
		// NULL
	}
	else if (numArgs == 1) {
		if ((*args)[0] != NULL)	{ //  argument  passed 
			VsValue v;
			(*args)[0]->evaluate(v,parent);
			if (v.type == VsValue::vFIELD && (SFNODE == v.field->getFieldType())) {
				((VsVar*)this)->assign(v,parent);
				// getField()->set(((VsSFNode)v.field)->getField()->get());
			} else {

				GvNode *owner = parent->function->getScriptNode();

				GvSFNode *f= getField();
				f->setContainer(owner);

				GvString s;
				v.getString(s); // think of proper browser context
				f->set(s); // set by VRML syntax string
				f->setContainer(NULL);
			}
		}
	}
	else  {
		Reporter.Error("bad SFNode constructor in %s",parent->getContextString());
	}

	return VsValue(this);
}



#include "grender.h"
#include "gmodel.h"

// create newly allocated MFString outof value 
GvMFString* newMFString(VsValue &v,VsFunctionCall *fcall);

// evalute browser method call

VsValue
browserMethodEvaluate(GView *browser,const VsMember &memberName,VsExprList &args,VsFunctionCall *parent)

{
	if (VS_NAME_EQ(memberName,isKeyPressed)) { // 4.1 extra
       float v=1; 
	   if (args.getLength()==1 && getArg(args, 0, parent,v)) {
			return VsValue(browser->isKeyPressed(v >0.0));
	   }
   }	
   else if (VS_NAME_EQ(memberName,getName)) {
	   return VsValue(new VsSFString(browser->getName()));
   }
   else if (VS_NAME_EQ(memberName,getVersion)) {
	   return VsValue(new VsSFString(browser->getVersion()));
   }
   else if (VS_NAME_EQ(memberName,getCurrentSpeed)) {
	   return VsValue(browser->getCurrentSpeed());
   }
   else if (VS_NAME_EQ(memberName,getCurrentFrameRate)) {
	   return VsValue(browser->getCurrentFrameRate());
   }
   // extra
   else if (VS_NAME_EQ(memberName,getCurrentTime)) {
	   return VsValue(GTime::GetGlobalTime());
   }
   else if (VS_NAME_EQ(memberName,getTime)) {
	   return VsValue(browser->theTime);
   }
   else if (VS_NAME_EQ(memberName,getWorldStartTime)) {
	   return VsValue(browser->loadTime);
   }
   else if (VS_NAME_EQ(memberName,getWorldURL)) {
	   return VsValue(new VsSFString(browser->getWorldURL()));
   }
   else if (VS_NAME_EQ(memberName,getWorldBaseURL)) { // 4.1
	   return VsValue(new VsSFString(browser->GetHomeUrl()));
   }
   else if (VS_NAME_EQ(memberName,setDescription)) {
	   if (args.getLength()==1) {
			VsValue  arg0v;
			args[0]->evaluate(arg0v,parent);
			GvString s;
			arg0v.getString(s);
			browser->setDescription(s);
		    return VsValue::nothing;
	   }
   }	
   else if (VS_NAME_EQ(memberName,print)) {
	   if (args.getLength() ==1) {
			VsValue  arg0v;
			args[0]->evaluate(arg0v,parent);
			GvString s;
			arg0v.getString(s);
			browser->Print(s);
		    return VsValue::nothing;
	   }
   }	
   else if (VS_NAME_EQ(memberName,getBaseURL)) {
	   if (args.getLength()>=0) {
			GvScript *script = parent->function->getScriptNode();
			GvString s;
			if (script)
				s=script->GetBaseUrl();
			return VsValue(new VsSFString(s));
	   }
   }	
   else if (VS_NAME_EQ(memberName,createVrmlFromString)) {
	   if (args.getLength()>=1) {
			VsValue  arg0v;
			args[0]->evaluate(arg0v,parent);
			GvString s;
			arg0v.getString(s);
			GvMFNode *ret= NULL;

			GvScript *script = parent->function->getScriptNode();

			ret=browser->createVrmlFromString(s,script); 
			if (ret == NULL) {
				Reporter.Warning("Browser.createVrmlFromString(%s) failed ", (const char *) s);
				return VsValue::nothing;
			}
			return VsValue(new VsMFNode(ret,VsField::RW,0));
	   }
   }
   else if (VS_NAME_EQ(memberName,createVrmlFromURL)) {
	   if (args.getLength()>=1) {
			VsValue  arg0v;
			args[0]->evaluate(arg0v,parent);
			VsValue  arg1v;
			VsValue  arg2v;
			if (args.getLength()>=2) args[1]->evaluate(arg1v,parent);
			if (args.getLength()>=3) args[2]->evaluate(arg2v,parent);

			GvMFString *arg0 = NULL;
			arg0 = newMFString(arg0v,parent);

			GvNodeHandle arg1 = arg1v.getNode();
			
			GvString arg2;
			arg2v.getString(arg2);
			GvScript *script = parent->function->getScriptNode();


			browser->createVrmlFromURL(arg0,arg1,arg2,script);
			if (arg0) delete arg0;

		    return VsValue::nothing;

	   }
    }
	else if ((VS_NAME_EQ(memberName,loadURL)) || (VS_NAME_EQ(memberName,loadURLrel))) {
	   if (args.getLength()>=1) {
			VsValue  arg0v;
			args[0]->evaluate(arg0v,parent);
			VsValue  arg1v;

			if (args.getLength()>=2) args[1]->evaluate(arg1v,parent);

			GvMFString *arg0 = NULL;
			GvMFString *arg1 = NULL;
			arg0 = newMFString(arg0v,parent);
			arg1 = newMFString(arg1v,parent);
			GvScript *script =NULL;
			// Cosmo keeps loadURL requests relative to top worlds !
			if (VS_NAME_EQ(memberName,loadURLrel))
				script = parent->function->getScriptNode();

			browser->loadURL(arg0,arg1,script);

			if (arg0) delete arg0;
			if (arg1) delete arg1;
		    return VsValue::nothing;

	   }
   }	
   else if (memberName == "addRoute") {
	   if (args.getLength()== 4) {
			VsValue  arg0v; args[0]->evaluate(arg0v,parent);
			VsValue  arg1v; args[1]->evaluate(arg1v,parent);
			VsValue  arg2v; args[2]->evaluate(arg2v,parent);
			VsValue  arg3v; args[3]->evaluate(arg3v,parent);

			GvNodeHandle arg0 = arg0v.getNode();
			GvNodeHandle arg2 = arg2v.getNode();
			
			GvString arg1,arg3;
			arg1v.getString(arg1);
			arg3v.getString(arg3);

			browser->addRoute(arg0,arg1,arg2,arg3);
		    return VsValue::nothing;
	   }
   }
   else if (memberName == "deleteRoute") {
	   if (args.getLength()== 4) {
			VsValue  arg0v; args[0]->evaluate(arg0v,parent);
			VsValue  arg1v; args[1]->evaluate(arg1v,parent);
			VsValue  arg2v; args[2]->evaluate(arg2v,parent);
			VsValue  arg3v; args[3]->evaluate(arg3v,parent);

			GvNodeHandle arg0 = arg0v.getNode();
			GvNodeHandle arg2 = arg2v.getNode();
			
			GvString arg1,arg3;
			arg1v.getString(arg1);
			arg3v.getString(arg3);

			browser->deleteRoute(arg0,arg1,arg2,arg3);
		    return VsValue::nothing;
	   }
   }
	else if (memberName == "replaceWorld") {
	   if (args.getLength()==1) {
			VsValue  arg0v;

			args[0]->evaluate(arg0v,parent);

			GvMFNode *arg0 = NULL;
			arg0 = newMFNode(arg0v,parent);
			browser->replaceWorld(arg0);
			if (arg0) delete arg0;
		    return VsValue::nothing;

	   }
    }	
	// extras 

	else if (memberName == "computeRayHit") {

		Point p1,p2;
		GvRayHitInfo *info=NULL;
		VsValue  arg2v;
		GvNodeHandle startNode;
		int options=0;

		// p1 - ray start position , p2 - end position 
		if (getArg(args, 0, parent,p1) && getArg(args, 1, parent,p2)) {
			info = new GvRayHitInfo();
			info->setBrowser(browser);
			
			if (args.getLength()>=3) { // starting node 
				args[2]->evaluate(arg2v,parent);
				startNode = arg2v.getNode();
			}
			if (getArg(args, 3, parent,options)) {
			}



			if (info->OnTrigger(browser,p1,p2,startNode)) {
				GvSFNode *f = new GvSFNode();
				f->set(info);
				return VsValue(new VsSFNode(f,VsField::RW,0));
			}
			else {
				delete info;
				return VsValue::nothing;
			}


		}	

	}
	else if (memberName == "computeCollision") {

		GvCollisionInfo *info=NULL;
		VsValue  arg0v, arg2v;
		GvNodeHandle startNode;
		GvNodeHandle sceneGraph;
		Matrix ma,mb;
		gbool hasMa,hasMb;
		int options;


		if (args.getLength()>=1) {
			
			info = new GvCollisionInfo();
			info->setBrowser(browser);
			
			// test node (geometry )
			if (args.getLength()>=1) { // starting node 
				args[0]->evaluate(arg0v,parent);
				startNode = arg0v.getNode();
			}
			if (getArg(args, 1, parent,ma)) {
				hasMa=gtrue;
			} else { hasMa = gfalse; ma.Identity(); }

			// scene graph 

			if (args.getLength()>=3) { 
				args[2]->evaluate(arg2v,parent);
				sceneGraph = arg2v.getNode();
			}
			if (getArg(args, 3, parent,mb)) {
				hasMb=gtrue;
			} else { hasMb = gfalse; mb.Identity(); }

			if (getArg(args, 4, parent,options)) {
			}

			// compute 
			if (info->OnTrigger(browser,startNode,&ma,sceneGraph,&mb,options)) {
				GvSFNode *f = new GvSFNode();
				f->set(info);
				return VsValue(new VsSFNode(f,VsField::RW,0));
			}
			else {
				delete info;
				return VsValue::nothing;
			}


		}	

	}
	else if (memberName == "prefetch") { // 4.3 prefetch nodes
	   if (args.getLength()>=1) {
			VsValue  arg0v; args[0]->evaluate(arg0v,parent);
			GvMFNode *arg0 = NULL;
			arg0 = newMFNode(arg0v,parent);
			int flag1=1; int flag2=1;
			getArg(args, 1, parent,flag1);
			getArg(args, 2, parent,flag2);



			int ret = browser->PrefetchNodes(*arg0,gfalse,flag1,flag2);
			if (arg0) delete arg0;
		    return VsValue(ret);

	   }
    }	
	else if (memberName == "getWorld") { // 4.3 get the world , security check ??
			GvSFNode *f = new GvSFNode();
			f->set(browser->GetScene()); // bad back reference 
			return VsValue(new VsSFNode(f,VsField::RW,0));

	}
	else if (memberName == "getScript") { // 4.3 get the script 
			GvSFNode *f = new GvSFNode();
			f->set(parent->function->getScriptNode()); // bad back reference 
			return VsValue(new VsSFNode(f,VsField::RW,0));

	}
	

	// extras 
	else if (memberName == "setViewpointByValue") {

		Point p;
		Rotation r;
		int mode=0;

		VsValue  arg2v;

		if (getArg(args, 0, parent,p) && getArg(args, 1, parent,r)) {
			
			if (args.getLength()>=3) {
				if (getArg(args, 2, parent,mode)) {
				}
			}

			browser->setViewpointByValue(p,r,mode);
		    return VsValue::nothing;

		}	
	    return VsValue::nothing;

	}
	else if (memberName == "getViewpointByValue") {

		GvSFVec3f *p=NULL;
		GvSFRotation *r=NULL;
		
		Point position;
		Rotation orientation;
		int mode=0;

		VsValue  arg1v,arg2v;

		if (getArg(args, 0, parent, arg1v, p) && getArg(args, 1, parent, arg2v, r)) {
			if (args.getLength()>=3) {
				if (getArg(args, 2, parent,mode)) {
				}
			}

			browser->getViewpointByValue(position,orientation,mode);
			p->set(position);
			r->set(orientation);
		    return VsValue::nothing;
		}	
	    return VsValue::nothing;

	}
	// extras 
	else if (memberName == "setMyAvatar") {

		VsValue  arg0v;
		GvNodeHandle n;

		if (args.getLength()==1) {
			args[0]->evaluate(arg0v,parent);
			n = arg0v.getNode();
			browser->BindAvatar(n);
			return VsValue::nothing;
		}

	}
	else if (memberName == "showMyAvatar" || memberName == "setThirdPersonView") {

       int flag=1;
	   if (args.getLength()==1 && getArg(args, 0, parent,flag)) {
			browser->SetThirdPersonView(flag>0);
			int ret= browser->ShowAvatar(flag>0);
			return VsValue(ret);
		}

	}
	else if (memberName == "setRenderMode") {
	   if (args.getLength()==1) {
			VsValue  arg0v; args[0]->evaluate(arg0v,parent);
			GvString arg0;
			arg0v.getString(arg0);
			browser->SetRenderMode(arg0);
		    return VsValue::nothing;
	   }
    }	
	else if (memberName == "setNavigationMode") {
	   if (args.getLength()==1 ) {
			VsValue  arg0v; args[0]->evaluate(arg0v,parent);
			GvString arg0;
			arg0v.getString(arg0);
			browser->SetNavigationMode(arg0);
		    return VsValue::nothing;
	   }
   }	
   else if (memberName == "setCollisionDetection") {
       float v=1; 
	   if (args.getLength()==1 && getArg(args, 0, parent,v)) {
			browser->setCollisionDetection(v >0.0);
		    return VsValue::nothing;
	   }
   }	
   else if (memberName == "setGravity") {
       float v=1; 
	   if (args.getLength()==1 && getArg(args, 0, parent,v)) {
			browser->setGravity(v >0.0); 
		    return VsValue::nothing;
	   }
   }	
   else if (memberName == "setHeadlight") {
       float v=1; 
	   if (args.getLength()==1 && getArg(args, 0, parent,v)) {
			browser->setHeadlight(v >0.0);
		    return VsValue::nothing;
	   }
   }	
   else if (memberName == "setSoundEnabled") {
       float v=1; 
	   if (args.getLength()==1 && getArg(args, 0, parent,v)) {
			browser->SetUseSound(v >0.0);
		    return VsValue::nothing;
	   }
   }	
   else if (memberName == "setViewpointAnimation") {
       float v=1; 
	   if (args.getLength()==1 && getArg(args, 0, parent,v)) {
			browser->setViewpointAnimation(v >0.0);
		    return VsValue::nothing;
	   }
   }	
   else if (memberName == "setAvatarHeight") {
       float v=1.65; 
	   if (args.getLength()==1 && getArg(args, 0, parent,v)) {
			browser->setAvatarHeight(v);
		    return VsValue::nothing;
	   }
   }	
   else if (memberName == "setCollisionDistance") {
       float v=0.5; 
	   if (args.getLength()==1 && getArg(args, 0, parent,v)) {
			browser->setCollisionDistance(v);
		    return VsValue::nothing;
	   }
   }	
   else if (memberName == "setVisibilityLimit") {
       float v=0.0; 
	   if (args.getLength()==1 && getArg(args, 0, parent,v)) {
			browser->setVisibilityLimit(v);
		    return VsValue::nothing;
	   }
   }	
   else if (memberName == "setWalkSpeed") {
       float v=1.0; 
	   if (args.getLength()==1 && getArg(args, 0, parent,v)) {
			browser->setWalkSpeed(v);
		    return VsValue::nothing;
	   }
   }	
   else if (memberName == "setBspMode") {
       int v=TRAVERSE_FRONT_TO_BACK; 
	   if (args.getLength()>=1 && getArg(args, 0, parent,v)) {
			browser->traversalOrder = v;
		    return VsValue::nothing;
	   }
   }	
   else if (memberName == "setBspLoadingMode") {
       int v=TRAVERSE_FRONT_TO_BACK; 
	   if (args.getLength()>=1 && getArg(args, 0, parent,v)) {
			browser->traversalInlineLoadingOrder = v;
		    return VsValue::nothing;
	   }
   }	
   else if (memberName == "getCap") {
       int v=0; 
	   if (args.getLength()>=1 && getArg(args, 0, parent,v)) {
			return VsValue(browser->GetCap(v));
	   }
   }	
   else if (memberName == "getOption") { // 4.3 x
	   if (args.getLength()==1) {
			GvString v; 	
			VsValue  arg0v; args[0]->evaluate(arg0v,parent);
			GvString arg0;
			arg0v.getString(arg0);
		    browser->GetOption(arg0,v);
			return VsValue(new VsSFString(v));
	   }
   }	
   else if (memberName == "setOption") { // 4.3 x
	   if (args.getLength()==2) {
			
			VsValue  arg0v; args[0]->evaluate(arg0v,parent);
			GvString arg0;
			arg0v.getString(arg0);
			VsValue  arg1v; args[1]->evaluate(arg1v,parent);
			GvString arg1;
			arg1v.getString(arg1);
		    GvBool x=browser->SetOption(arg0,arg1);
			return VsValue(x);
	   }
   }	
   else if (memberName == "getNavigationMode") {
       CString v; 
	   if (args.getLength()>=0) {
		    browser->GetNavigationMode(v);
			return VsValue(new VsSFString((const char *)v));
	   }
   }	
   else if (memberName == "getInstallDirectory") { // 4.3
	   return VsValue(new VsSFString((const char *) browser->GetInstallDirectory()));
   }	
   else if (VS_NAME_EQ(memberName,getName)) {
	   return VsValue(new VsSFString(browser->getName()));
   }
   else if (memberName == "setUnloadMode") {
       float v1= 0.05;
       int v=1000;
	   if (args.getLength()>=1 && getArg(args, 0, parent,v)) {
			browser->activeInlinesLimit = v;

			if (args.getLength()>=2 && getArg(args, 1, parent,v1)) {
				browser->activeInlinesPurge= v1;
			}
			return VsValue::nothing;
	   }
   }	

   else if (memberName == "setFocus") {
	    //browser->pView->SetFocus(); // new  for 		andreas
	    return VsValue::nothing;
   }	

   else if (memberName == "getWindowSizeX") {
	   return VsValue(browser->getWindowSizeX());
   }
   else if (memberName == "getWindowSizeY") {
	   return VsValue(browser->getWindowSizeY());
   }
   else if (memberName == "getWindowAspect") {
	   return VsValue(browser->getWindowAspect());
   }
   else if (memberName == "getGravity") {
	   return VsValue(browser->getGravity());
   }
   else if (memberName == "getCollisionDetection") {
	   return VsValue(browser->getCollisionDetection());
   }
   else if (memberName == "getHeadLight") {
	   return VsValue(browser->GetHeadLight());
   }
   else if (memberName == "getViewpointAnimation") {
	   return VsValue(browser->getViewpointAnimation());
   }
   else if (memberName == "getVisibilityLimit") {
	   return VsValue(browser->getVisibilityLimit());
   }
   else if (memberName == "getCollisionDistance") {
	   return VsValue(browser->getCollisionDistance());
   }
   else if (memberName == "getAvatarHeight") {
	   return VsValue(browser->getAvatarHeight());
   }
   else if (memberName == "getStepOverSize") {
	   return VsValue(browser->getStepOverSize());
   }
   else if (memberName == "getZNear") {
	   return VsValue(browser->getZNear());
   }
   else if (memberName == "getZFar") {
	   return VsValue(browser->getZFar());
   }
   else if (memberName == "getThirdPersonView") {
	   return VsValue(browser->thirdPersonView);
   }
   else if (memberName == "getSoundEnabled") {
	   return VsValue(browser->GetUseSound());
   }


   Reporter.Error("script error: Bad Browser method call %s in %s",memberName.getString(),parent->getContextString());	
   return VsValue::nothing;
}


// invoke member function 
VsValue VsSFNode::callMember(const VsMember &memberName,VsExprList *args, VsFunctionCall *fcall)
{
	GvNode *to = getField()->get();	// get the target node 
	if (to == NULL) 
		return VsValue::nothing;
		//VsValue val = obj->evaluate(parent);
			if (RTISA(to,GView)) {
				GView *view = (GView *) to;
				//TRACE("Browser Method call : %s \n",member.getString());
				return browserMethodEvaluate(view,memberName,*args,fcall);

			}
   if (memberName == "getType") {
		return VsValue(new VsSFString(to->GetType()));
   } else 	
   if (memberName == "getName") {
		return VsValue(new VsSFString(to->getName().getString()));
   } else 	
	   if (memberName == "copy") { // 4.1
		return VsValue(new VsSFNode(to->copy()));
   } else 	
   if (memberName == "getEventIn") {
		GvString v;
		if (getArg(*args, 0, fcall,v)) {  
		    GvFieldData* toData = to->getFieldData();
			int toIndex = toData->getEventInIndex(to,v);
			if (toIndex<0) {
				//Reporter.Error("Node %s has no eventOut %s in %s",to->ClassName(),member.getString(),fcall->getContextString());
				return VsValue::nothing;
			}
			GvFieldEntry *toFieldEntry = toData->getFieldEntry(toIndex);
			GvField *toField = to->getField(toIndex);
			
			if (toField == NULL) {
				return VsValue::nothing;
			}

			VsField *tmpField=NULL;

			// create field by ref 
			tmpField = VsField::create(toFieldEntry->fieldClass,RW,toField,1);
			return VsValue(tmpField);

		}	
	}
   else if (memberName == "hasEventIn") { // 4.1
		GvString v;
		if (getArg(*args, 0, fcall,v)) {  
		    GvFieldData* toData = to->getFieldData();
			int toIndex = toData->getEventInIndex(to,v);
			return VsValue( (toIndex >=0)); // 4,1
		}	
	}

   else if (memberName == "getEventOut") { // 4.1
		GvString v;
		if (getArg(*args, 0, fcall,v)) {  
		    GvFieldData* toData = to->getFieldData();
			int toIndex = toData->getEventOutIndex(to,v);
			if (toIndex<0) {
				//Reporter.Error("Node %s has no eventOut %s in %s",to->ClassName(),member.getString(),fcall->getContextString());
				return VsValue::nothing;
			}
			GvFieldEntry *toFieldEntry = toData->getFieldEntry(toIndex);
			GvField *toField = to->getField(toIndex);
			
			if (toField == NULL) {
				return VsValue::nothing;
			}

			VsField *tmpField=NULL;

			// create field by ref 
			tmpField = VsField::create(toFieldEntry->fieldClass,RW,toField,1);
			return VsValue(tmpField);

		}	
	}
   else if (memberName == "hasEventOut") { // 4.1
		GvString v;
		if (getArg(*args, 0, fcall,v)) {  
		    GvFieldData* toData = to->getFieldData();
			int toIndex = toData->getEventOutIndex(to,v);
			return VsValue((toIndex >=0)); // 4,1
		}	
	}
    else if (memberName == "getMatrix") { // 4.3
		Matrix m;
		if (to->getMatrix(m)) {
		} else m.Identity();
		return VsValue(new VsSFMatrix(m));
	}
    else if (memberName == "setMatrix") { // 4.3
		Matrix m;
		int ret = 0;
		if (getArg(*args,0,fcall,m)) {
			if (RTISA(to,Gv2Transform)) {
				((Gv2Transform *)to)->setMatrix(m);  ret = 1;  
			}
			else if (RTISA(to,GvMatrixTransform)) {
				((GvMatrixTransform *)to)->setMatrix(m);  ret = 1;  
			}
			else if (RTISA(to,Gv2MatrixTransform)) {
				((Gv2MatrixTransform *)to)->setMatrix(m);  ret = 1;  
			}
		}
		return VsValue(ret);
	}
	return VsVar::callMember(memberName,args,fcall);
}




//
//	VsMFInt32
//

void
VsMFInt32::evaluate(VsValue &ret, int32_t index, VsFunctionCall *fcall)
{
	if (index <0 || index >= getLength())  {
		VS_RET_INDEX_ERROR_RET(index,fcall);
	}
    else ret = VsValue(getField()->get1(index));
}

VsValue
VsMFInt32::assign(const VsValue &v, int32_t index, VsFunctionCall *fcall)
{
	if (index < 0) VS_RET_INDEX_ERROR(index,fcall);

    if (permission == VsField::RW) {
		checkLength(index+1);
		switch (v.type) {
		case VsValue::vNUMBER: 	getField()->set1(index, v.number); return(v);
		case VsValue::vFIELD:  	
			if (SFINT32 == v.field->getFieldType()) {
				getField()->set1(index, ((VsSFInt32*)v.field)->getField()->get());
				return v;
			}
			break;
		default: return VsMField::assign(v,index,fcall);
		}
	}
	return VsValue::nothing;
}

//
//	VsMFFloat
//

void
VsMFFloat::evaluate(VsValue &ret, int32_t index, VsFunctionCall *fcall)
{
	if (index <0 || index >= getLength()) VS_RET_INDEX_ERROR_RET(index,fcall);
    ret = VsValue(getField()->get1(index));
}

VsValue
VsMFFloat::assign(const VsValue &v, int32_t index, VsFunctionCall *fcall)
{
	if (index < 0) VS_RET_INDEX_ERROR(index,fcall);

    if (permission == VsField::RW) {
		checkLength(index+1);
		switch (v.type) {
		case VsValue::vNUMBER: 	getField()->set1(index, v.number); return(v);
		case VsValue::vFIELD:  	
			if (SFFLOAT == v.field->getFieldType()) {
				getField()->set1(index, ((VsSFFloat*)v.field)->getField()->get());
				return v;
			}
			// break;
		default: return VsMField::assign(v,index,fcall);
		}
	}
	return VsValue::nothing;
}

//
//	VsMFTime
//

void
VsMFTime::evaluate(VsValue &ret, int32_t index, VsFunctionCall *fcall)
{
	if (index <0 || index >= getLength()) VS_RET_INDEX_ERROR_RET(index,fcall);
    ret = VsValue(getField()->get1(index));
}

VsValue
VsMFTime::assign(const VsValue &v, int32_t index, VsFunctionCall *fcall)
{
	if (index < 0) VS_RET_INDEX_ERROR(index,fcall);

    if (permission == VsField::RW) {
		checkLength(index+1);
		switch (v.type) {
		case VsValue::vNUMBER: 	getField()->set1(index, v.number); return(v);
		case VsValue::vFIELD:  	
			switch(v.field->getFieldType()) {
			case SFFLOAT :
				getField()->set1(index, ((VsSFFloat*)v.field)->getField()->get());
				return v;
			case SFTIME :
				getField()->set1(index, ((VsSFTime*)v.field)->getField()->get());
				return v;
			default : break;
			}
			// break;
		default: return VsMField::assign(v,index,fcall);
		}
	}
	return VsValue::nothing;
}


//
//	VsMFString
//

void
VsMFString::evaluate(VsValue &ret, int32_t index, VsFunctionCall *fcall)
{
	if (index <0 || index >= getLength()) VS_RET_INDEX_ERROR_RET(index,fcall);
    ret= VsValue(getField()->get1(index));
}

VsValue
VsMFString::assign(const VsValue &v, int32_t index, VsFunctionCall *fcall)
{
	if (index < 0) VS_RET_INDEX_ERROR(index,fcall);

    if (permission == VsField::RW) {
		checkLength(index+1);
		// convert argument to string 
		GvString s; v.getString(s);
		getField()->set1(index,s); 
		return(v);
/*
		switch (v.type) {
		case VsValue::vNUMBER: { GvString s; v.getString(s); getField()->set1(index,s); }	return(v);
		case VsValue::vSTRING:	getField()->set1(index,v.string); return(v);
		case VsValue::vGVSTRING: getField()->set1(index,*v.gvString); return(v);
		case VsValue::vFIELD:  	
			if (SFSTRING == v.field->getFieldType()) {
				getField()->set1(index, ((VsSFString*)v.field)->getField()->get());
				return v;
			}
			break;
		default: return VsMField::assign(v,index,fcall);
		}
*/
	}
	return VsValue::nothing;
}

// evaluate reference to element at offset
VsVar*	VsMFString::evaluateVar(int32_t index, VsFunctionCall *)
{
	return new VsSFStringMF1(this,index,permission);
}


VsValue VsMFString::assign(const VsValue &v, VsFunctionCall *fcall)
{
    if (permission == VsField::RW) {
		
		switch (v.type) {	   

		case VsValue::vFIELD: 
			if (getFieldType() == v.field->getFieldType()) // same type 
			{
				((GvField *)getField())->set(v.field->getField()); // 
				return v; 
			}
			// MFFLoat ==> MFString ?? 

		case VsValue::vNUMBER: 
		case VsValue::vSTRING:
		case VsValue::vGVSTRING:
		default: { 
				GvString s; v.getString(s);  // set to single length MF
				getField()->set_(s); 
			}	// allways try to string convert 
		}
		return(v);
	}
	return VsValue::nothing;

}
//
//	VsMFNode
//

void
VsMFNode::evaluate(VsValue &ret, int32_t index, VsFunctionCall *fcall)
{
	if (index <0 || index >= getLength()) VS_RET_INDEX_ERROR_RET(index,fcall);
	GvSFNode *f = new GvSFNode();
	f->set(getField()->get1(index));
	ret = VsValue(new VsSFNode(f,RW,0));
//    return VsValue(getField()->get1(index));
}
VsValue
VsMFNode::assign(const VsValue &v, int32_t index, VsFunctionCall *fcall)
{
	if (index < 0) VS_RET_INDEX_ERROR(index,fcall);

    if (permission == VsField::RW) {
		checkLength(index+1);
		switch (v.type) {
		case VsValue::vNODE: 	getField()->set1(index, v.node); return(v);
		case VsValue::vFIELD:  	
			if (SFNODE  == v.field->getFieldType()) {
				getField()->set1(index, ((VsSFNode*)v.field)->getField()->get());
				return v;
			}
			break;
		default: return VsMField::assign(v,index,fcall);
		}
	}
	return VsValue::nothing;
}

// evaluate reference to element at offset
VsVar*	VsMFNode::evaluateVar(int32_t index, VsFunctionCall *)
{
	return new VsSFNodeMF1(this,index,permission);
}


//
//	VsMFVec2f
//						

void
VsMFVec2f::evaluate(VsValue &ret, int32_t index, VsFunctionCall *fcall)
{
	if (index <0 || index >= getLength()) VS_RET_INDEX_ERROR_RET(index,fcall);
	GvSFVec2f *f = new GvSFVec2f();
	f->set(getField()->get1(index));
	ret = VsValue(new VsSFVec2f(f,RW,0));
}

VsValue
VsMFVec2f::assign(const VsValue &v, int32_t index, VsFunctionCall *fcall)
{
	if (index < 0) VS_RET_INDEX_ERROR(index,fcall);

    if (permission == VsField::RW) {
		checkLength(index+1);
		switch (v.type) {
		case VsValue::vFIELD:  	
			if (SFVEC2F == v.field->getFieldType()) {
				getField()->set1(index, ((VsSFVec2f*)v.field)->getField()->get());
				return v;
			}
			if (SFVEC3F == v.field->getFieldType()) {
				getField()->set1(index, ((VsSFVec3f*)v.field)->getField()->get());
				return v;
			}
			if (SFCOLOR == v.field->getFieldType()) {
				getField()->set1(index, ((VsSFColor*)v.field)->getField()->get());
				return v;
			}
			break;
		default: return VsMField::assign(v,index,fcall);
		}
	}
	return VsValue::nothing;
}

// evaluate reference to element at offset
VsVar*	VsMFVec2f::evaluateVar(int32_t index, VsFunctionCall *)
{
	return new VsSFVec2fMF1(this,index,permission);
}


//
//	VsMFVec3f
//						

void
VsMFVec3f::evaluate(VsValue &ret, int32_t index, VsFunctionCall *fcall)
{
	if (index <0 || index >= getLength()) VS_RET_INDEX_ERROR_RET(index,fcall);
	GvSFVec3f *f = new GvSFVec3f();
	f->set(getField()->get1(index));
	ret = VsValue(new VsSFVec3f(f,RW,0));
}

VsValue
VsMFVec3f::assign(const VsValue &v, int32_t index, VsFunctionCall *fcall)
{
	if (index < 0) VS_RET_INDEX_ERROR(index,fcall);

    if (permission == VsField::RW) {
		checkLength(index+1);
		switch (v.type) {
		case VsValue::vFIELD:  	
			if (SFVEC3F == v.field->getFieldType()) {
				getField()->set1(index, ((VsSFVec3f*)v.field)->getField()->get());
				return v;
			}
			if (SFCOLOR == v.field->getFieldType()) {
				getField()->set1(index, ((VsSFColor*)v.field)->getField()->get());
				return v;
			}
			break;
		default: return VsMField::assign(v,index,fcall);
		}
	}
	return VsValue::nothing;
}

// evaluate reference to element at offset
VsVar*	VsMFVec3f::evaluateVar(int32_t index, VsFunctionCall *fcall)
{
	if (index <0 ) VS_RET_INDEX_ERROR_VAR(index,fcall);
	return new VsSFVec3fMF1(this,index,permission);
}

// invoke member function 

// MFVec3f::setVertexTransform(srcVertex,srcIndex,translation,rotation,scale)

VsValue VsMFVec3f::callMember(const VsMember &member,VsExprList *args, VsFunctionCall *fcall)
{
	if (member == "setByVertexTransform") {

		GvMFVec3f *srcVertex=NULL;
		GvMFInt32 *srcIndex=NULL;
		GvMFVec3f *translation=NULL;
		GvMFVec3f *scale=NULL;
		GvMFRotation *rotation=NULL;
		GvMFRotation *scaleOrientation=NULL;
		GvMFVec3f *center=NULL;
		VsValue vtmp[7];

		// get the arguments 
		getArg(*args,0,fcall,vtmp[0],srcVertex); if (!srcVertex) BAD_ARG;
		getArg(*args,1,fcall,vtmp[1],srcIndex);  if (!srcIndex) BAD_ARG;
		getArg(*args,2,fcall,vtmp[2],translation);
		getArg(*args,3,fcall,vtmp[3],rotation);
		getArg(*args,4,fcall,vtmp[4],scale);
		getArg(*args,5,fcall,vtmp[5],scaleOrientation);
		getArg(*args,6,fcall,vtmp[6],center);
		const long *srci=srcIndex->get();
		const Point*srcv= (const Point *) srcVertex->get();
		int g=-1;
		if (srci) {
			const long *srciEnd=srci+srcIndex->getNum();

			// the matrix stack
			Array<Matrix> mstack(32);

			int stacki=-1;

			// me, the target 
			GvMFVec3f *targetVertex= getField();
			
			if (targetVertex->getNum() == 0) targetVertex->set(*srcVertex); // init by copy

			Point* destv= (Point *) targetVertex->get();
			int destvn= targetVertex->getNum();

			Matrix m;
			m.Identity();

			while (srci < srciEnd) {

				g=g+1; // get group index

				int stacki = *srci++; // get group stack index

				// get vertex start, end index
				int vstart = *srci++;
				int vend = *srci++;
      
				if (g>=0) {

					// push
					if (mstack.MaxLength() <= stacki) 
						mstack.SetMaxLength(stacki+16);
			

					// compute local transform matrix
					Matrix &m = mstack[stacki];

					if (srci > srciEnd || vend>destvn || stacki<0) {
						// bad data 
						BAD_ARG;
						//break; 
					}

					// compute matrix 
					if (translation)  {
						if (center)
							m.TranslationMatrix(center->get1Point(g)+translation->get1Point(g));
						else m.TranslationMatrix(/*center+*/translation->get1Point(g));
					}
					else m.Identity();

					if (rotation) {
						const Rotation& r=rotation->get1(g);
						m.MultiplyLeft(RotationAxisMatrix(r.Axis(),r.Angle()));
					}

					if (scale) {
						if (!scaleOrientation) {
							m.ScaleByLeft(scale->get1Point(g));
						} else {
							const Rotation& r=scaleOrientation->get1(g);

							m.MultiplyLeft(RotationAxisMatrix(r.Axis(),r.Angle()));
							m.ScaleByLeft(scale->get1Point(g));
							m.MultiplyLeft(RotationAxisMatrix(r.Axis(),-r.Angle()));
						}
					}
					if (center) m.MultiplyLeft(TranslationMatrix(-center->get1Point(g)));
  
					// combine with parent matrix if exists
					if(stacki != 0) {
						//m.MultiplyLeft(mstack[stacki-1]);
						m.MultiplyRight(mstack[stacki-1]);
						//m = mstack[stacki-1]*m;
					} 

					// transform vertices 
					for (int i=vstart; i<vend; i++) {
						destv[i] = m * srcv[i];
					}
				} 
			}
			// trigger
			targetVertex->setDefault(FALSE);
			targetVertex->OnChanged();

		}

		return VsValue::nothing;
	}
#if 1
	if (member == "setByVertexTransformPop") {

		GvMFVec3f *srcVertex=NULL;
		GvMFInt32 *srcIndex=NULL;
		GvMFVec3f *translation=NULL;
		GvMFVec3f *scale=NULL;
		GvMFRotation *rotation=NULL;
		GvMFRotation *scaleOrientation=NULL;
		GvMFVec3f *center=NULL;
		VsValue vtmp[6];

		// get the arguments 
		getArg(*args,0,fcall,vtmp[0],srcVertex); if (!srcVertex) BAD_ARG;
		getArg(*args,1,fcall,vtmp[1],srcIndex);  if (!srcIndex) BAD_ARG;
		getArg(*args,2,fcall,vtmp[2],translation);
		getArg(*args,3,fcall,vtmp[3],rotation);
		getArg(*args,4,fcall,vtmp[4],scale);
		getArg(*args,5,fcall,vtmp[5],scaleOrientation);
		const long *srci=srcIndex->get();
		const Point*srcv= (const Point *) srcVertex->get();

		if (srci) {
			const long *srciEnd=srci+srcIndex->getNum();

			// the matrix stack
			Array<Matrix> mstack(32);

			int stacki=-1;

			// me, the target 
			GvMFVec3f *targetVertex= getField();
			
			if (targetVertex->getNum() == 0) targetVertex->set(*srcVertex); // init by copy

			Point* destv= (Point *) targetVertex->get();
			int destvn= targetVertex->getNum();


			while (srci < srciEnd) {
				int g= *srci++; // get group index, or pop code
				if (g>=0) {
					// push
					stacki++;
					if (mstack.MaxLength() <= stacki) 
						mstack.SetMaxLength(stacki+16);
					
					Matrix &m = mstack[stacki];

					// get vertex start, end index
					int vstart = *srci++;
					int vend = *srci++;
					if (srci > srciEnd || vend>destvn) {
						// bad data 
						BAD_ARG;
					}

					// compute matrix 
					if (translation) 
						m.TranslationMatrix(/*center+*/translation->get1Point(g));
					else m.Identity();

					if (rotation) {
						const Rotation& r=rotation->get1(g);
						m.MultiplyLeft(RotationAxisMatrix(r.Axis(),r.Angle()));
					}

					if (scale) {
						if (!scaleOrientation) {
							m.ScaleByLeft(scale->get1Point(g));
						} else {
							const Rotation& r=scaleOrientation->get1(g);

							m.MultiplyLeft(RotationAxisMatrix(r.Axis(),r.Angle()));
							m.ScaleByLeft(scale->get1Point(g));
							m.MultiplyLeft(RotationAxisMatrix(r.Axis(),-r.Angle()));
						}
					}
					if (center) m.MultiplyLeft(TranslationMatrix(-center->get1Point(g)));

					// combine with parent matrix 
					//m.MultiplyLeft(mstack[stacki]);
					// m = mstack[stacki]*m;
					
					if (stacki>0)
						m.MultiplyRight(mstack[stacki-1]);

					// transform vertices 
					for (int i=vstart; i<vend; i++) {
						destv[i] = m * srcv[i];
					}


				} else {
					// pop
					stacki += g;
					if (stacki<-1) stacki=-1;

				}
			}
			// trigger
			targetVertex->setDefault(FALSE);
			targetVertex->OnChanged();

		}

		return VsValue::nothing;
	}
#endif

#if 0
	if (member == "getTransform") {
		Point translation(0,0,0),scale(1,1,1),shear;
		Rotation rotation;
		Matrix m;
		getField()->get(m);

		DecomposeMatrix(m,scale,shear,rotation,translation);
		
		setArg(*args,0,fcall,translation);
		setArg(*args,1,fcall,rotation);
		setArg(*args,2,fcall,scale);

		return VsValue::nothing;
	}
	if (member == "inverse") {
		Matrix m;
		getField()->get(m);
		m.Invert();
		return VsValue(new VsSFMatrix(m));
	}
	if (member == "transpose") {
		Matrix m;
		getField()->get(m);
		m.Transpose();
		return VsValue(new VsSFMatrix(m));
	}
	if (member == "multLeft") {
		Matrix m,m1;
		getField()->get(m);

		// ARGCHECK(1)
		getArg(*args,0,fcall,m1);
		//m = m1 * m;
		m.MultLeft(m1);
		return VsValue(new VsSFMatrix(m));
	}
	if (member == "multRight") {
		Matrix m,m1;
		getField()->get(m);

		// ARGCHECK(1)
		getArg(*args,0,fcall,m1);
		// m = m * m1;
		m.MultRight(m1);

		return VsValue(new VsSFMatrix(m));
	}
	if (member == "multVecMatrix") {
		Matrix m;
		getField()->get(m);
		Point p(0,0,0);

		// ARGCHECK(1)
		getArg(*args,0,fcall,p);
		p = m * p;
		return VsValue(new VsSFVec3f(p));
	}
	if (member == "multMatrixVec") {
		Matrix m;
		getField()->get(m);
		Point p(0,0,0);

		// ARGCHECK(1)
		getArg(*args,0,fcall,p);
		p= m * p; // ????? to do: check 
		return VsValue(new VsSFVec3f(p));
	}
#endif


	return VsMField::callMember(member,args,fcall);
bad_arg:
	Reporter.Error("MFVec3f:Bad Argument for function %s . (Num Args = %d) in %s",member.getString(),args->getLength(),fcall->getContextString()); 
	return VsValue::nothing; 

}

//
//	VsMFColor
//						

void
VsMFColor::evaluate(VsValue &ret, int32_t index, VsFunctionCall *fcall)
{
	if (index <0 || index >= getLength()) VS_RET_INDEX_ERROR_RET(index,fcall);
	GvSFColor *f = new GvSFColor();
	f->set(getField()->get1(index));
	ret = VsValue(new VsSFColor(f,RW,0));
}

VsValue
VsMFColor::assign(const VsValue &v, int32_t index, VsFunctionCall *fcall)
{
	if (index < 0) VS_RET_INDEX_ERROR(index,fcall);

    if (permission == VsField::RW) {
		checkLength(index+1);
		switch (v.type) {
		case VsValue::vFIELD:  	
			if (SFCOLOR == v.field->getFieldType()) {
				getField()->set1(index, ((VsSFColor*)v.field)->getField()->get());
				return v;
			}
			if (SFVEC3F == v.field->getFieldType()) {
				getField()->set1(index, ((VsSFVec3f*)v.field)->getField()->get());
				return v;
			}
			break;
		default: return VsMField::assign(v,index,fcall);
		}
	}
	return VsValue::nothing;
}

// evaluate reference to element at offset
VsVar*	VsMFColor::evaluateVar(int32_t index, VsFunctionCall *)
{
	return new VsSFColorMF1(this,index,permission);
}

//
//	VsMFRotation
//						

void
VsMFRotation::evaluate(VsValue &ret, int32_t index, VsFunctionCall *fcall)
{
	if (index <0 || index >= getLength()) VS_RET_INDEX_ERROR_RET(index,fcall);
	GvSFRotation *f = new GvSFRotation();
	f->set(getField()->get1(index));
	ret=VsValue(new VsSFRotation(f,RW,0));
}
								
VsValue
VsMFRotation::assign(const VsValue &v, int32_t index, VsFunctionCall *fcall)
{
	if (index < 0) VS_RET_INDEX_ERROR(index,fcall);

    if (permission == VsField::RW) {
		checkLength(index+1);
		switch (v.type) {
		case VsValue::vFIELD:  	
			if (SFROTATION == v.field->getFieldType()) {
				getField()->set1(index, ((VsSFRotation*)v.field)->getField()->get());
				return v;
			}
			break;
		default: return VsMField::assign(v,index,fcall);
		}
	}
	return VsValue::nothing;
}

// evaluate reference to element at offset
VsVar*	VsMFRotation::evaluateVar(int32_t index, VsFunctionCall *)
{
	return new VsSFRotationMF1(this,index,permission);
}


//
// VsFieldMF1
//


VsFieldMF1::VsFieldMF1(VsMField *fieldVar,int index) : fieldVar(fieldVar), index(index)
{
	if (fieldVar) fieldVar->ref();
}

VsFieldMF1::~VsFieldMF1()
{
	if (fieldVar) fieldVar->unref();
}



void
VsFieldMF1::evaluate(VsValue &ret, VsFunctionCall *fcall)
{
    (fieldVar)->evaluate(ret,index,fcall);
}

VsValue
VsFieldMF1::assign(const VsValue &v, VsFunctionCall *fcall)
{
	return (fieldVar)->assign(v,index,fcall);
}


/*

  evalutateMember

  assignMember ??? 

*/

//
// VsSFStringMF1
//


VsSFStringMF1::VsSFStringMF1(VsMFString *fieldVar,int index,Permission perm) :
	 VsSFString(new GvSFString(),perm,0),fieldVar(fieldVar), index(index)
{
	if (fieldVar) fieldVar->ref();
	if (index >=0 && index < fieldVar->getLength())
	   getField()->set(fieldVar->getField()->get1(index)); // set initial value 
}

VsSFStringMF1::~VsSFStringMF1()
{
	if (fieldVar) fieldVar->unref();
}

void 
VsSFStringMF1::store()
{
    if (permission == VsField::RW) {
		fieldVar->checkLength(index+1);
		fieldVar->getField()->set1(index,getField()->value);
	}
	else VS_RO_ERROR();

}


VsValue
VsSFStringMF1::assign(const VsValue &v, VsFunctionCall *fcall)
{
	VsValue ret= VsSFString/*VsField*/::assign(v,fcall);
	store();
	return ret;
}

VsValue
VsSFStringMF1::assign(const VsValue &v,int32_t index, VsFunctionCall *fcall)
{
	VsValue ret= VsSFString::assign(v,index,fcall);
	store();
	return ret;
}



//
// VsSFVec3fMF1
//


VsSFVec3fMF1::VsSFVec3fMF1(VsMFVec3f *fieldVar,int index,Permission perm) :
 VsSFVec3f(new GvSFVec3f(),perm,0),fieldVar(fieldVar), index(index)
{
	if (fieldVar) fieldVar->ref();
	if (index >=0 && index < fieldVar->getLength())
	   getField()->set(fieldVar->getField()->get1(index)); // set initial value 
}

VsSFVec3fMF1::~VsSFVec3fMF1()
{
	if (fieldVar) fieldVar->unref();
}

void 
VsSFVec3fMF1::store()
{
    if (permission == VsField::RW) {
		fieldVar->checkLength(index+1);
		fieldVar->getField()->set1(index,getField()->value);
	}
	else VS_RO_ERROR();

}


VsValue
VsSFVec3fMF1::assign(const VsValue &v, VsFunctionCall *fcall)
{
	VsValue ret= /*VsSFVec3f::*/ VsField::assign(v,fcall);
	store();
	return ret;
}

VsValue
VsSFVec3fMF1::assign(const VsValue &v,int32_t index, VsFunctionCall *fcall)
{
	VsValue ret= VsSFVec3f::assign(v,index,fcall);
	store();
	return ret;
}

//
// VsSFVec2fMF1
//


VsSFVec2fMF1::VsSFVec2fMF1(VsMFVec2f *fieldVar,int index,Permission perm) :
 VsSFVec2f(new GvSFVec2f(),perm,0),fieldVar(fieldVar), index(index)
{
	if (fieldVar) fieldVar->ref();
	if (index >=0 && index < fieldVar->getLength())
	   getField()->set(fieldVar->getField()->get1(index)); // set initial value 
}

VsSFVec2fMF1::~VsSFVec2fMF1()
{
	if (fieldVar) fieldVar->unref();
}


void 
VsSFVec2fMF1::store()
{
    if (permission == VsField::RW) {
		fieldVar->checkLength(index+1);
		fieldVar->getField()->set1(index,getField()->value);
	}
	else VS_RO_ERROR();

}


VsValue
VsSFVec2fMF1::assign(const VsValue &v, VsFunctionCall *fcall)
{
	VsValue ret= /*VsSFVec2f*/ VsField::assign(v,fcall);
	store();
	return ret;
}

VsValue
VsSFVec2fMF1::assign(const VsValue &v,int32_t index, VsFunctionCall *fcall)
{
	VsValue ret= VsSFVec2f::assign(v,index,fcall);
	store();
	return ret;
}


//
// VsSFColorMF1
//


VsSFColorMF1::VsSFColorMF1(VsMFColor *fieldVar,int index,Permission perm) :
	 VsSFColor(new GvSFColor(),perm,0),fieldVar(fieldVar), index(index)
{
	if (fieldVar) fieldVar->ref();
	if (index >=0 && index < fieldVar->getLength())
		getField()->set(fieldVar->getField()->get1(index)); // set initial value 
}

VsSFColorMF1::~VsSFColorMF1()
{
	if (fieldVar) fieldVar->unref();
}

void 
VsSFColorMF1::store()
{

    if (permission == VsField::RW) {
		fieldVar->checkLength(index+1);
		fieldVar->getField()->set1(index,getField()->value);
	}
	else VS_RO_ERROR();

}


VsValue
VsSFColorMF1::assign(const VsValue &v, VsFunctionCall *fcall)
{
	VsValue ret= VsField::assign(v,fcall);
	store();
	return ret;
}

VsValue
VsSFColorMF1::assign(const VsValue &v,int32_t index, VsFunctionCall *fcall)
{
	VsValue ret= VsSFColor::assign(v,index,fcall);
	store();
	return ret;
}

//
// VsSFRotationMF1
//


VsSFRotationMF1::VsSFRotationMF1(VsMFRotation *fieldVar,int index,Permission perm) :
 VsSFRotation(new GvSFRotation(),perm,0),fieldVar(fieldVar), index(index)
{
	if (fieldVar) fieldVar->ref();
	if (index >=0 && index < fieldVar->getLength())
		getField()->set(fieldVar->getField()->get1(index)); // set initial value 
}

VsSFRotationMF1::~VsSFRotationMF1()
{
	if (fieldVar) fieldVar->unref();
}


void 
VsSFRotationMF1::store()
{
    if (permission == VsField::RW) {
		fieldVar->checkLength(index+1);
		fieldVar->getField()->set1(index,getField()->value);
	}
	else VS_RO_ERROR();

}


VsValue
VsSFRotationMF1::assign(const VsValue &v, VsFunctionCall *fcall)
{
	VsValue ret= /*VsSFRotation*/ VsField::assign(v,fcall);
	store();
	return ret;
}

VsValue
VsSFRotationMF1::assign(const VsValue &v,int32_t index, VsFunctionCall *fcall)
{
	VsValue ret= VsSFRotation::assign(v,index,fcall);
	store();
	return ret;
}

//
// VsSFNodeMF1
//


VsSFNodeMF1::VsSFNodeMF1(VsMFNode *fieldVar,int index,Permission perm) :
	 VsSFNode(new GvSFNode(),perm,0),fieldVar(fieldVar), index(index)
{
	if (fieldVar) fieldVar->ref();
	if (index >=0 && index < fieldVar->getLength())
		 getField()->set(fieldVar->getField()->get1(index)); // set initial value 
}

VsSFNodeMF1::~VsSFNodeMF1()
{
	if (fieldVar) fieldVar->unref();
}

void 
VsSFNodeMF1::store()
{
    if (permission == VsField::RW) {
		fieldVar->checkLength(index+1);
		fieldVar->getField()->set1(index,getField()->value);
	}
	else VS_RO_ERROR();

}


VsValue
VsSFNodeMF1::assign(const VsValue &v, VsFunctionCall *fcall)
{
	VsValue ret= VsSFNode::assign(v,fcall);
	store();
	return ret;
}

///

// GV_MFIELD_SOURCE(GvMFValue, VsValue, 1);
// need constructor /destructor call

RTIMP(GvMFValue,0,GvMField)

GV_MFIELD_SOURCE_EQ_FIELD(GvMFValue);
GvMFValue::GvMFValue(int cnt)	
{							
    values = NULL;			
    allocValues(cnt);							     
}									     

// Destructor
									     
GvMFValue::~GvMFValue()							     
{									     
    if (values != NULL)							     
	    delete [] values;
}									     


void									     
GvMFValue::allocValues(int newNum)					     
{	
	int i;
    if (values == NULL) {						     
	    if (newNum > 0)							     
	        values = new VsValue[newNum]();
    }					
	else if (newNum>0 && newNum<=maxNum) {
	    for (i=newNum; i< num; i++) // release  past
            values[i].destruct();

	    num = newNum; 
		return;
	}
    else {								     
	if (newNum > 0) {							     
	    
	    VsValue *newValues = new VsValue[newNum]();

        int copyNum = min(newNum,num);
	    
	    for (i=0; i< copyNum; i++) newValues[i]=values[i];
	    
	    delete [] values;
		values = newValues;

	} else {								     
	    delete [] values;
	    values = NULL;						     
	}								     
    }									     
    num = maxNum = newNum;						     
}

int GvMFValue::set(int newNum,const VsValue *newValues) 
{ 
    int i;

	if (values && values !=newValues) // clean out handles
		for(i=0; i<num; i++)   values[i].destruct();
	num=0;

    allocValues(newNum); 

    for( i= 0 ; i<num; i++) values[i] = newValues[i];
    
	setDefault(FALSE);
    OnChanged();

    return(num>0);
}



GvBool
GvMFValue::read1Value(GvInput *in, int index)
{
	return FALSE;
    //return in->read(values[index]);
}

int GvMFValue::io1Value(AStream &a, int index)
{
    return -1;

	//double *valuePtr = values + index;
    //return (a.io(valuePtr[0]));
}

#if 0


int GvMFValue::get1(GvString &s,int index)
{
  int ret=sprintf(s.getBuffer(30),"%.16g",values[index]);
  s.releaseBuffer();
  return(ret >0);

}


int GvMFValue::set1(const char* s,int index)
{
  int ret=sscanf(s,"%f",&values[index]);
  setDefault(FALSE);
  OnChanged();
  return(ret == 1);
}

#endif


int GvMFValue::set(const VsValue &value)
{ 
  makeRoom(1);
  values[0]=value;
  setDefault(FALSE);
  OnChanged();
  return(Length());
}

void GvMFValue::set1(int index,  const VsValue &value)
{
  values[index] = value;
  setDefault(FALSE);
  OnChanged();
}

/*
GvField* GvMFValue::get1Field(int index)
{
	if (index<0 || index >= getNum()) return NULL;
	GvSFTime *f = new GvSFTime();
	f->set(values[index]);
	return f;
}
*/

//
//	VsArray
//

void
VsArray::evaluate(VsValue &ret,int32_t index, VsFunctionCall *fcall)
{
	if (index <0 || index >= getLength()) VS_RET_INDEX_ERROR_RET(index,fcall);
    ret = getField()->get1(index);
}

VsValue
VsArray::assign(const VsValue &v, int32_t index, VsFunctionCall *fcall)
{
	if (index < 0) VS_RET_INDEX_ERROR(index,fcall);

    if (permission == VsField::RW) {
		checkLength(index+1);
		getField()->set1(index, v );
		return(v);

	}
	return VsValue::nothing;
}


// VsDate

void 
VsDate::evaluate(VsValue &ret,VsFunctionCall *)
{
    ret = VsValue(getField()->get());
}

VsValue VsDate::assign(const VsValue &v, VsFunctionCall *fcall)
{
    if (permission == VsField::RW) {
		switch (v.type) {
		case VsValue::vNUMBER: 	getField()->set(v.number); 	return(v);
		default: return VsField::assign(v,fcall);
		}
	}
	return VsValue::nothing;
}


/*
var newDateObj = new Date()
var newDateObj = new Date(year, month, day)
var newDateObj = new Date(year, month, day, hours, minutes, seconds)
var newDateObj = new Date("month day, year hours:minutes:seconds")


--------------------------------------------------------------------------------
getDate method
getDay method
getHours method
getMinutes method
getMonth method
getSeconds method
getTime method
getTimezoneOffset method
getYear method
parse method
setDate method
setHours method
setMinutes method
setMonth method
setSeconds method
setTime method
setYear method
toGMTString method
toLocaleString method
UTC method



   struct _timeb timebuffer;
   char *timeline;

   _ftime( &timebuffer );
   timeline = ctime( & ( timebuffer.time ) );

   printf( "The time is %.19s.%hu %s", timeline, timebuffer.millitm, &timeline[20] );

	struct tm *localtime( const time_t *timer );


}


  
    time( &ltime );

*/

VsValue VsDate::initCall(VsFunctionCall *parent,VsExprList *args)
{
	int l= getLength();

	int numArgs = args->getLength();

	double  tt=0;

	if (numArgs == 0) {
		struct _timeb t;
		_ftime(&t);
		tt=(double ) t.time + ((double) t.millitm) / 1000.0;
		TRACE("Time %d %d \n",t.time,(int) t.millitm); 
		this->getField()->set(tt);
	} else 

	if (numArgs == 1) { // string ??
		if ((*args)[0] != NULL)	{ //  argument not passed 
			VsValue v;
			(*args)[0]->evaluate(v,parent);
			((VsVar*)this)->assign(v,parent);
		}
	}
	else if (numArgs <= 7) {

		// year, month, day, hours, minutes, seconds
		// extra milli

		struct tm t;
		double msec=0;

		ZeroMemory(&t,sizeof(t));

		// Evaluate arguments:
		for (int i = 0; i < numArgs; i++) {
			int v=0;

			if (getArg(*args,0,parent,v)) { 

			switch (i) {
			case 0 : t.tm_year = v; break;
			case 1 : t.tm_mon = v; break;
			case 2 : t.tm_mday = v; break;
			case 3 : t.tm_hour = v; break;
			case 4 : t.tm_min = v; break;
			case 5 : t.tm_sec = v; break;
			case 6 : msec = ((double) v)/1000; break;
			default : 
			
				Reporter.Error("date constructor not implemented in %s",parent->getContextString());
	
				break;
			}
			}


		}
		struct _timeb tstruct; // TO DO: is this ok ?

		_ftime( &tstruct );
		//t.tm_min += tstruct.timezone; 
		tt = mktime(&t);
		tt+= msec;

		this->getField()->set(tt);

    }

	else  {
		Reporter.Error("date constructor not implemented in %s",parent->getContextString());
	}

	return VsValue(this);
}

// invoke member function 
VsValue VsDate::callMember(const VsMember &memberName,VsExprList *args, VsFunctionCall *parent)
{
	
	double tt=getField()->get();

	struct tm *t;
	time_t tlocal = tt;

	t = localtime( &tlocal );
	double milli = tt-tlocal;

	if (VS_NAME_EQ(memberName,getDate)) { //Returns the day of the month as stored in a Date object
			return VsValue(t->tm_mday);
	}	
	if (VS_NAME_EQ(memberName,getDay)) { //Retrieves the day of the week represented by the date stored in a Date object. 
			return VsValue(t->tm_wday);
	}	
	if (VS_NAME_EQ(memberName,getHours)) { //Retrieves the number of hours that have elapsed since midnight
			return VsValue(t->tm_hour);
	}	
	if (VS_NAME_EQ(memberName,getMinutes)) { // Retrieve the minutes value stored in a Date object
			return VsValue(t->tm_min);
	}	
	if (VS_NAME_EQ(memberName,getMonth)) { // Retrieves the month value of the Date object.
			return VsValue(t->tm_mon);
	}	
	if (VS_NAME_EQ(memberName,getSeconds)) { // Retrieves the number of seconds stored in a Date object
			return VsValue(t->tm_sec);
	}	
	if (VS_NAME_EQ(memberName,getTime)) { // Retrieves the time stored in a Date object
			return VsValue(tt); //tlocal
	}	
	if (VS_NAME_EQ(memberName,getTimezoneOffset)) { // Determine the difference in minutes between the time on the host computer and Greenwich Mean Time (GMT).			
			struct _timeb tstruct;
			_ftime( &tstruct );
			return VsValue(tstruct.timezone );
	}	
	if (VS_NAME_EQ(memberName,getYear)) { // Retrieve the year stored in the specified Date object
			return VsValue(t->tm_year);
	}	
	// static :
	if (VS_NAME_EQ(memberName,parse)) { // Parses a string containing a date, and returns the number of milliseconds between that date and midnight, January 1, 1970
		float v=1; 
		if (args->getLength()==1 && getArg(*args, 0, parent,v)) {
			return VsValue(t->tm_wday);
		}


		//parse xxx
			return VsValue(tlocal);
	}	


	//
	if (VS_NAME_EQ(memberName,setHours)) { //Modifies the hours stored in the Date object
       int v=0; 
	   if (args->getLength()>=1 && getArg(*args, 0, parent,v)) {
		   t->tm_hour = v;  tlocal = mktime(t);
		   getField()->set(tlocal+milli);
		   return VsValue::nothing;
	   } else BAD_ARG;
    }	
	if (VS_NAME_EQ(memberName,setMinutes)) { 
       int v=0; 
	   if (args->getLength()>=1 && getArg(*args, 0, parent,v)) {
		   t->tm_min = v;  tlocal = mktime(t);
		   getField()->set(tlocal+milli);
		   return VsValue::nothing;
	   } else BAD_ARG;
    }	
	if (VS_NAME_EQ(memberName,setMonth)) { 
       int v=0; 
	   if (args->getLength()>=1 && getArg(*args, 0, parent,v)) {
		   t->tm_mon = v;  tlocal = mktime(t);
		   getField()->set(tlocal+milli);
		   return VsValue::nothing;
	   } else BAD_ARG;
    }	
	if (VS_NAME_EQ(memberName,setSeconds)) { 
       int v=0; 
	   if (args->getLength()>=1 && getArg(*args, 0, parent,v)) {
		   t->tm_sec = v;  tlocal = mktime(t);
		   getField()->set(tlocal+milli);
		   return VsValue::nothing;
	   } else BAD_ARG;
    }	
	if (VS_NAME_EQ(memberName,setTime)) { 
       double v=0; 
	   if (args->getLength()>=1 && getArg(*args, 0, parent,v)) {
		   getField()->set(v);
		   return VsValue::nothing;
	   } else BAD_ARG;
    }	
	if (VS_NAME_EQ(memberName,setYear)) { 
       int v=0; 
	   if (args->getLength()>=1 && getArg(*args, 0, parent,v)) {
		   t->tm_year = v;  tlocal = mktime(t);
		   getField()->set(tlocal+milli);
		   return VsValue::nothing;
	   } else BAD_ARG;
    }	
	if (VS_NAME_EQ(memberName,toGMTString)) { 
       int v=0; 
	   if (args->getLength()==0 ) {
		GvString vs;
		// timeline = ctime(&tlocal);
		t=gmtime(&tlocal);

		const char * timeline = asctime(t);
		vs = timeline;
		vs+= " GMT";
		// to do: fixformat
		//The format of the return value is as follows: "05 Jan 1996 00:00:00 GMT". 
		// return "01/05/96 00:00:00" for January 5, but in Europe, it may return "05/01/96 00:00:00
	   	return VsValue(new VsSFString(timeline));

	   } else BAD_ARG;
    }	
	if (VS_NAME_EQ(memberName,toLocaleString)) { 
       int v=0; 
	   if (args->getLength()==0 ) {
		GvString vs;
		// timeline = ctime(&tlocal);

		const char * timeline = asctime(t);
		vs = timeline;
		// return "01/05/96 00:00:00" for January 5, but in Europe, it may return "05/01/96 00:00:00
	   	return VsValue(new VsSFString(timeline));

	   } else BAD_ARG;
    }	
//	static :
	if (VS_NAME_EQ(memberName,UTC)) { 
       int v=0; 
		// year, month, day, hours, minutes, seconds
		// extra milli

		struct tm t;
		double msec=0;

		ZeroMemory(&t,sizeof(t));
		int numArgs = args->getLength();


		// Evaluate arguments:
		for (int i = 0; i < numArgs; i++) {
			int v=0;

			if (getArg(*args,0,parent,v)) { 

			switch (i) {
			case 0 : t.tm_year = v; break;
			case 1 : t.tm_mon = v; break;
			case 2 : t.tm_mday = v; break;
			case 3 : t.tm_hour = v; break;
			case 4 : t.tm_min = v; break;
			case 5 : t.tm_sec = v; break;
			case 6 : msec = ((double) v)/1000; break;
			default : 
				BAD_ARG;
				break;
			}
			}


		}
		//struct _timeb tstruct; // TO DO: is this ok ?
		//_ftime( &tstruct );
		//t.tm_min -= tstruct.timezone; 
		tt = mktime(&t);
		// if (tt == -1) 
		tt+= msec;
		return VsValue(tt);
    }	
  
	
	return VsVar::callMember(memberName,args,parent);
bad_arg:
	Reporter.Error("Date:Bad Argument for function %s . (Num Args = %d) in %s",memberName.getString(),args->getLength(),parent->getContextString()); 
	return VsValue::nothing; 



}





