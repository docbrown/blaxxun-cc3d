/*
 * VsFunction.c++
 *
 *     Implementation of classes:
 *        VsFunctionDef
 *        VsFunctionCall
 *	  VsMethodCall
 *	  VsMemberAccess
 *
 * Copyright (C) 1996, Silicon Graphics,  Inc.

$Revision: 1.9 $
$Log:

 */
#include "stdafx.h"

#include <assert.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include "vsfunction.h"
#include "vsstatement.h"
#include "vsparse.h"
#include "vsfields.h"
#include "jsmath.h"

#include "greporter.h"

#include "grender.h"
#include "gmodel.h"
#include "gvnode.h"

double JsMath::nextNextGaussian=0;
gbool  JsMath::haveNextNextGaussian = gfalse;

// hg 

#include "gperlinnoise.h"

#ifdef __sgi
double JsMath::E       = M_E;
double JsMath::LN10    = M_LN10;
double JsMath::LN2     = M_LN2;
double JsMath::PI      = M_PI;
double JsMath::SQRT1_2 = M_SQRT1_2;
double JsMath::SQRT2   = M_SQRT2;
#else
double JsMath::E       = 2.7182818284590452354;
double JsMath::LN10    = 2.30258509299404568402;
double JsMath::LN2     = 0.69314718055994530942;
double JsMath::PI      = 3.14159265358979323846;
double JsMath::SQRT1_2 = 0.70710678118654752440;
double JsMath::SQRT2   = 1.41421356237309504880;
#endif

#ifdef __sgi
#include <alloca.h> /* For alloca() on SGI */
#endif              /* (alloca() is defined in <malloc.h> on PC) */

// While parsing, function currently being defined:
VsFunctionDef *VsFunctionDef::curFunction = NULL;

VsFunctionDef::VsFunctionDef(const char *_name, VsParseTree *_parent)
{
    name = strdup(_name);
    numLocals = numArgs = 0;
    statements = NULL;
    parent = _parent;
    assert(curFunction == NULL);
    curFunction = this;
}

VsFunctionDef::~VsFunctionDef()
{
    int i;
    free(name);
    for (i = 0; i < argOrLocalNames.getLength(); i++) {
		char *n = (char *)argOrLocalNames[i];
		free(n);
    }
    if (statements != NULL)
		delete statements;

	// was undefed __sgi HG 
    // Make sure curFunction is set to NULL.  If there was a syntax
    // error inside a function, curFunction may be left in an unknown
    // state.  Make sure it is cleared here.
    curFunction = NULL;
}

void
VsFunctionDef::endDefinition(VsStatementList *stmnts)
{
    assert(curFunction == this);
    statements = stmnts;
    curFunction = NULL;
}

void
VsFunctionDef::addArg(const char *argName)
{
    assert(curFunction->numLocals == 0);

    ++curFunction->numArgs;
    curFunction->argOrLocalNames.append(strdup(argName));
}

VsVar *
VsFunctionDef::findIdentifier(const char *name, gbool createIfNotExists)
{
    int i;
	assert(curFunction != NULL);

    // Ok, look through local vars and arguments, last-defined first:
    for (i = curFunction->getNumLocals()-1; i >= 0; i--) {
	const char *n = (const char *)curFunction->argOrLocalNames[i];
	if (strcmp(n, name) == 0) {
	    return new VsArgOrLocal(i);
	}
    }

    VsVar *result = curFunction->parent->getGlobalVar(name);

    if (result == NULL) {
		if (!createIfNotExists) {
// not for now		   Reporter.Warning("script error: Variable %s used before defined or assigned",name);
		}
		// Not found, create a new local variable:
		++curFunction->numLocals;
		curFunction->argOrLocalNames.append(strdup(name));
		result = new VsArgOrLocal(curFunction->getNumLocals()-1);
    }

    return result;
}

// create a new local var for the var statement 
VsVar *
VsFunctionDef::makeLocalVar(const char *name)
{
    assert(curFunction != NULL);

#if 0
    // Ok, look through local vars and arguments, last-defined first:
    for (int i = curFunction->getNumLocals()-1; i >= 0; i--) {
	const char *n = (const char *)curFunction->argOrLocalNames[i];
	if (strcmp(n, name) == 0) {
	    return new VsArgOrLocal(i);
	}
    }

    VsVar *result = curFunction->parent->getGlobalVar(name);

    if (result == NULL) {
	}
#endif

	VsVar *result= NULL;	

	if (curFunction != NULL) {
		// warning if already declared ??

		// Not found, create a new local variable:
		++curFunction->numLocals;
		curFunction->argOrLocalNames.append(strdup(name));
		result = new VsArgOrLocal(curFunction->getNumLocals()-1);
	}
	else {
	
	}

    return result;
}

	
GvScript * VsFunctionDef::getScriptNode()
{ 
	if (parent) return parent->getScriptNode();
	return NULL; 
}


//
// VsFunctionCall
//

VsFunctionCall::VsFunctionCall(VsFunctionDef *_func, VsExprList &exprs)
{
    function = _func; // do we need a ref ?

    for (int i = 0; i < exprs.getLength(); i++) {
		args[i] = exprs[i];
		ASSERT(args[i]!=NULL);
		if (args[i]) args[i]->ref();
    }
    locals = NULL;
}

VsFunctionCall::~VsFunctionCall()
{
    for (int i = 0; i < args.getLength(); i++) {
		if (args[i]) args[i]->unref();	  //crash here arg[i] == null
    }
}


const char * VsFunctionCall::getContextString()
{
	if (!function) return "";
	return function->getName();
}

void 
VsFunctionCall::evaluate(VsValue &ret,VsFunctionCall *parent)
{
    int i;             
    //ref();

	returnValue.destruct();

    VsValue *callStack;  // Previous args, locals, needed if recursing

    // Gotta allocate new locals[].  Use alloca() for speed.
    callStack = locals;
    
	int numL = function->getNumLocals();

    if (numL == 0)
		locals = NULL;
    else {
        locals = (VsValue *)alloca(sizeof(VsValue)*numL);

        // give all  locals/args a VOID value:
        for (i = 0; i < numL; i++) {
			locals[i].construct();
        }

        // Evaluate arguments:
		for (i = 0; i < function->getNumArgs(); i++) {
			if (args[i] != NULL) // HG argument not passed 
				args[i]->evaluate(locals[i],parent);
			// else locals[i].type = VsValue::vVOID;  // ??? UNINIT ???
		}

        // And give other locals/args a VOID value:
        //for (i = function->getNumArgs(); i < function->getNumLocals(); i++) {
		//	locals[i].type = VsValue::vVOID;  // ??? UNINIT ???
        //}
    }

    // Execute statements until we get a return:
    VsStatementList *stmnts = function->statements;
    for (i = 0; stmnts != NULL && i < stmnts->getLength(); i++) {
		VsStatement *stmt = (*stmnts)[i];
		VsStatement::Status s = stmt->execute(this);
		if (s == VsStatement::RETURN) {
			break;
		}
    }

    // ??? Delete any string-valued args or locals ???
    // give all  locals/args a VOID value:

	// destruct 
    for (i = 0; i < numL; i++) {
		locals[i].destruct();
     }


    // Unwind call stack
    locals = callStack;
	
	//unref();

    ret = returnValue;
	// ?? returnValue.destruct();

}

void
VsFunctionCall::setReturnValue(const VsValue &val)
{
    returnValue = val;
}
/* inline 

VsValue
VsFunctionCall::getArg(int whichArg)
{
    if (whichArg >= function->getNumLocals()) {
		// Reference to un-passed param, return NULL value
		return VsValue::nothing;
    }
    return locals[whichArg];
}
*/

void
VsFunctionCall::setArgOrLocal(int which, const VsValue &newVal)
{
    if (which < function->getNumLocals()) {
		locals[which] = newVal;
    }
}

int		
VsFunctionCall::isAnUnimplementedFunction(const char* fName)
{
/*
    if (!strcmp(fName, "SFBool")) {
	return 1;
    } else if (!strcmp(fName, "SFColor")) {
	return 1;
    } else if (!strcmp(fName, "SFFloat")) {
	return 1;
    } else if (!strcmp(fName, "SFImage")) {
	return 1;
    } else if (!strcmp(fName, "SFInt32")) {
	return 1;
    } else if (!strcmp(fName, "SFNode")) {
	return 1;
    } else if (!strcmp(fName, "SFRotation")) {
	return 1;
    } else if (!strcmp(fName, "SFString")) {
	return 1;
    } else if (!strcmp(fName, "SFTime")) {
	return 1;
    } else if (!strcmp(fName, "SFVec2f")) {
	return 1;
    } else if (!strcmp(fName, "SFVec3f")) {
	return 1;
    } else if (!strcmp(fName, "MFColor")) {
	return 1;
    } else if (!strcmp(fName, "MFFloat")) {
	return 1;
    } else if (!strcmp(fName, "MFInt32")) {
	return 1;
    } else if (!strcmp(fName, "MFNode")) {
	return 1;
    } else if (!strcmp(fName, "MFRotation")) {
	return 1;
    } else if (!strcmp(fName, "MFString")) {
	return 1;
    } else if (!strcmp(fName, "MFVec2f")) {
	return 1;
    } else if (!strcmp(fName, "MFVec3f")) {
	return 1;
    } else if (!strcmp(fName, "parseInt")) {
	return 1;
    } else if (!strcmp(fName, "parseFloat")) {
	return 1;
    }
*/
    return 0;
}


VsFunctionCallByName::VsFunctionCallByName(const char *name_, VsExprList &exprs) : VsFunctionCall(NULL,exprs),browser(NULL),name(NULL)
{

	if (name_) name = strdup(name_);
}


VsFunctionCallByName::~VsFunctionCallByName()
{
	if (name) free(name);
#ifdef _DEBUG
	name = NULL;
#endif
}


const char * VsFunctionCallByName::getContextString()
{
	if (!function) return name;
	return function->getName();
}

// lookup function name in list of defined functions
gbool VsFunctionCallByName::resolve(VsParseTree *parent) 
{


	function = parent->findFunction(name);
	if (function !=NULL) return gtrue;
	browser = parent->getGlobalVar("Browser");
	return gfalse;
}


void
VsFunctionCallByName::evaluate(VsValue &ret, VsFunctionCall *parent)
{
	int i;

	returnValue.destruct();

	// already resolved to other script function
	if (function) {
		VsFunctionCall::evaluate(ret,parent);
		return;
	}


	// built in function ?
    VsValue *callStack;  // Previous args, locals, needed if recursing

    // Gotta allocate new locals[].  Use alloca() for speed.
    callStack = locals;
    
	int numL = args.getLength();

    if (numL == 0)
		locals = NULL;
    else {
        locals = (VsValue *)alloca(sizeof(VsValue)*numL);

        // give all  locals/args a VOID value:
        for (i = 0; i < numL; i++) {
			locals[i].construct();
        }

        // Evaluate arguments:
		for (i = 0; i < args.getLength(); i++) {
			if (args[i] != NULL) // HG argument not passed 
				args[i]->evaluate(locals[i],parent);
		}

    }

    // Execute function
	if ((streq(name,"print") || streq(name,"trace")) && (numL == 1)) {
		if (browser) {
			GView *view = (GView *) (((VsSFNode*)(browser))->getField()->get());
			GvString s;
			locals[0].getString(s);
			if (view) view->Print(s);
		    returnValue = VsValue::nothing;
		}	
	}
	else 
		if (streq(name,"parseInt") && (numL>=1)) { // (string [,radix])
		if (numL > 0) {
			GvString s;
			int base = 10;
			char *endptr=NULL;  
			long value = 0;
			locals[0].getString(s);
			if (numL>1) {
				locals[1].get(base);
			}
			value = strtol(s, &endptr, base );
			returnValue = value;
		}
	}
	else if (streq(name,"parseFloat")  && (numL==1)) {
		if (numL > 0) {
			GvString s;
			char *endptr=NULL;  
			double value = 0.0;
			locals[0].getString(s);
			value = strtod(s, &endptr);
			returnValue = value;
		}
	}
	else if (streq(name,"toString") && (numL==1)) {
		if (numL > 0) {
			GvString s;
			char *endptr=NULL;  
			locals[0].getString(s);
			//returnValue = s;
			returnValue = VsValue(new VsSFString(s));
		}
	}
	else {
		GReporter *reporter= &Reporter;

		if (browser) {
			GView *view = (GView *) (((VsSFNode*)(browser))->getField()->get());
			if (view) reporter = view->GetReporter();
		}	

		reporter->Warning("script error: function %s with %d arguments not defined in %s",name,numL,parent->getContextString());
	}


	// destruct 
    for (i = 0; i < numL; i++) {
		locals[i].destruct();
     }


    // Unwind call stack
    locals = callStack;

    ret = returnValue;
}



VsNewCall::VsNewCall(const char *fName,VsExprList &exprs)
{
    for (int i = 0; i < exprs.getLength(); i++) {
		args[i] = exprs[i];
		args[i]->ref();
    }
    if (!strcmp(fName, "SFBool")) {
		fieldClass = SFBOOL;
    } else if (!strcmp(fName, "SFColor")) {
		fieldClass = SFCOLOR;
    } else if (!strcmp(fName, "SFFloat")) {
		fieldClass = SFFLOAT;
    } else if (!strcmp(fName, "Float")) {
		fieldClass = SFFLOAT;
    } else if (!strcmp(fName, "SFImage")) {
		fieldClass = SFIMAGE;
    } else if (!strcmp(fName, "SFInt32")) {
		fieldClass = SFINT32;
    } else if (!strcmp(fName, "Int")) {
		fieldClass = SFINT32;
    } else if (!strcmp(fName, "SFNode")) {
		fieldClass = SFNODE;
    } else if (!strcmp(fName, "SFRotation")) {
		fieldClass = SFROTATION;
    } else if (!strcmp(fName, "SFString")) {
		fieldClass = SFSTRING;
    } else if (!strcmp(fName, "String")) {
		fieldClass = SFSTRING;
    } else if (!strcmp(fName, "SFTime")) {
		fieldClass = SFTIME;
    } else if (!strcmp(fName, "Double")) {
		fieldClass = SFTIME;
    } else if (!strcmp(fName, "SFVec2f")) {
		fieldClass = SFVEC2F;
    } else if (!strcmp(fName, "SFVec3f")) {
		fieldClass = SFVEC3F;
    } else if (!strcmp(fName, "MFColor")) {
		fieldClass = MFCOLOR;
    } else if (!strcmp(fName, "MFFloat")) {
		fieldClass = MFFLOAT;
    } else if (!strcmp(fName, "MFInt32")) {
		fieldClass = MFINT32;
    } else if (!strcmp(fName, "MFNode")) {
		fieldClass = MFNODE;
    } else if (!strcmp(fName, "MFRotation")) {
		fieldClass = MFROTATION;
    } else if (!strcmp(fName, "MFString")) {
		fieldClass = MFSTRING;
    } else if (!strcmp(fName, "MFVec2f")) {
		fieldClass = MFVEC2F;
    } else if (!strcmp(fName, "MFVec3f")) {
		fieldClass = MFVEC3F;
    } else if (!strcmp(fName, "SFMatrix")) {
		fieldClass = SFMATRIX;
    } else if (!strcmp(fName, "VrmlMatrix")) {
		fieldClass = SFMATRIX;
	} else if (!strcmp(fName, "MFTime")) {
		fieldClass = MFTIME;
	} else if (!strcmp(fName, "Array")) {
		fieldClass = MFVALUE;
	} else if (!strcmp(fName, "Date")) {
		fieldClass = SFDATE;
    } 
	else  {
		Reporter.Error("script error: unkown class for 'new %s' \n",fName);
		fieldClass = UnknownType;
	}

}

VsNewCall::~VsNewCall()
{

    for (int i = 0; i < args.getLength(); i++) {
		args[i]->unref();
    }
}

void 
VsNewCall::evaluate(VsValue &ret,VsFunctionCall *parent)
{
	VsField *f=  VsField::create(fieldClass, VsField::RW,NULL,0);
	if (f == NULL) { 
		ret = VsValue::nothing;
		return; 
	}
    // init all will return VsValue(f) 
	ret = f->initCall(parent,&args);


}


VsArgOrLocal::VsArgOrLocal(int _whichArg)
{
    whichArg = _whichArg;
}

VsArgOrLocal::~VsArgOrLocal()
{
}

void 
VsArgOrLocal::evaluate(VsValue &ret, VsFunctionCall *f)
{
     f->getArg(ret,whichArg);
}

// returns field if any (compare to VsExpr implementation )
VsVar *VsArgOrLocal::evaluateVar(VsFunctionCall *f,VsValue &ret)
{
	// let's evaluate and see if it evaluates to field
	f->getArg(ret,whichArg);

	switch (ret.type) {

		case VsValue::vFIELD:
		//	ASSERT(ret.field->refCount >1);
			// otherwise it was a temp field ???
			return(ret.field);

		default :
			return this; // it's itself a variable which can be assigned 
			break;
	}


	return NULL;
}


VsValue
VsArgOrLocal::assign(const VsValue &v, VsFunctionCall *f)
{
    f->setArgOrLocal(whichArg, v);
    return v;
}

VsMethodCall::VsMethodCall(VsExpr* _obj, // by ref 
			     const char* funcName, // by value
				 VsExprList &exprs) : memberName(funcName)	// by value !!!!
{
    obj = _obj;
	if (obj) obj->ref();


	int numArgs = exprs.getLength();
	

    if (obj == NULL) {	    // MATH object is a special case
	if (!strcmp(funcName, "abs")) {
	    func = fABS;
	} else if (!strcmp(funcName, "acos")) {
	    func = fACOS;
	} else if (!strcmp(funcName, "asin")) {
	    func = fASIN;
	} else if (!strcmp(funcName, "atan")) {
	    func = fATAN;
	} else if (!strcmp(funcName, "ceil")) {
	    func = fCEIL;
	} else if (!strcmp(funcName, "cos")) {
	    func = fCOS;
	} else if (!strcmp(funcName, "exp")) {
	    func = fEXP;
	} else if (!strcmp(funcName, "floor")) {
	    func = fFLOOR;
	} else if (!strcmp(funcName, "log")) {
	    func = fLOG;
	} else if ((numArgs == 2) && !strcmp(funcName, "max")) {
	    func = fMAX;
	} else if ((numArgs == 2) && !strcmp(funcName, "min")) {
	    func = fMIN;
	} else if ( (numArgs == 2) && !strcmp(funcName, "pow")) {
	    func = fPOW;
	} else if (!strcmp(funcName, "random")) {
	    func = fRANDOM;
	} else if (!strcmp(funcName, "round")) {
	    func = fROUND;
	} else if (!strcmp(funcName, "sin")) {
	    func = fSIN;
	} else if (!strcmp(funcName, "sqrt")) {
	    func = fSQRT;
	} else if (!strcmp(funcName, "tan")) {
	    func = fTAN;
// hg 
	} else if ((numArgs == 2) && !strcmp(funcName, "atan2")) {
	    func = fATAN2;
	} else if (!strcmp(funcName, "randomGaussian")) {
	    func = fGAUSSIAN;
	} else if (!strcmp(funcName, "sinh")) {
	    func = fSINH;
	} else if (!strcmp(funcName, "cosh")) {
	    func = fCOSH;
	} else if (!strcmp(funcName, "tanh")) {
	    func = fTANH;
	} else if (!strcmp(funcName, "noise")) {
	    func = fNOISE;
	} else {
	    Reporter.Error("script error: Unknown function Math.%s with %d arguments",funcName,numArgs);
	    func = fUNKNOWN;
	}
    }

    for (int i = 0; i < exprs.getLength(); i++) {
		args[i] = exprs[i];
		ASSERT(args[i]!=NULL);
		args[i]->ref();
    }
}

VsMethodCall::~VsMethodCall()
{
    for (int i = 0; i < args.getLength(); i++) {
		args[i]->unref();
    }
	if (obj) obj->unref();
}

void 
VsMethodCall::evaluate(VsValue &returnValue, VsFunctionCall *parent)
{
    if (obj == NULL) {	// MATH
	returnValue.destruct();
	VsValue v1;
	
	if (args.getLength() >0) args[0]->evaluate(v1,parent);

	switch (func) {
	    case fABS:
		returnValue.number = JsMath::abs(v1.number);
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case fACOS:
		returnValue.number = JsMath::acos(v1.number);
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case fASIN:
		returnValue.number = JsMath::asin(v1.number);
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case fATAN:
		returnValue.number = JsMath::atan(v1.number);
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case fCEIL:
		returnValue.number = JsMath::ceil(v1.number);
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case fCOS:
		returnValue.number = JsMath::cos(v1.number);
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case fEXP:
		returnValue.number = JsMath::exp(v1.number);
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case fFLOOR:
		returnValue.number = JsMath::floor(v1.number);
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case fLOG:
		returnValue.number = JsMath::log(v1.number);
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case fMAX:
			{	VsValue v2; if (args.getLength() >1) args[1]->evaluate(v2,parent); 
				returnValue.number = JsMath::max(v1.number,v2.number);
				returnValue.type   = VsValue::vNUMBER;
			}
		break;
	    case fMIN:
			{	VsValue v2; if (args.getLength() >1) args[1]->evaluate(v2,parent); 
				returnValue.number = JsMath::min(v1.number,v2.number);
				returnValue.type   = VsValue::vNUMBER;
			}
		break;
	    case fPOW:
			{	VsValue v2; if (args.getLength() >1) args[1]->evaluate(v2,parent); 
				returnValue.number = JsMath::pow(v1.number,v2.number);
				returnValue.type   = VsValue::vNUMBER;
			}
		break;
	    case fRANDOM:
		returnValue.number = JsMath::random();
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case fROUND:
		returnValue.number = JsMath::round(v1.number);
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case fSIN:
		returnValue.number = JsMath::sin(v1.number);
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case fSQRT:
		returnValue.number = JsMath::sqrt(v1.number);
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case fTAN:
		returnValue.number = JsMath::tan(v1.number);
		returnValue.type   = VsValue::vNUMBER;
		break;
//hg 
	    case fCOSH:
		returnValue.number = JsMath::cosh(v1.number);
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case fSINH:
		returnValue.number = JsMath::sinh(v1.number);
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case fTANH:
		returnValue.number = JsMath::tanh( v1.number);
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case fNOISE: // hg 
			returnValue.number = Noise1(v1.number);
			returnValue.type   = VsValue::vNUMBER;
		break;

		case fGAUSSIAN:
			returnValue.number = JsMath::randomGaussian();
			returnValue.type   = VsValue::vNUMBER;
		break;


		case fATAN2:
		{	VsValue v2; if (args.getLength() >1) args[1]->evaluate(v2,parent); 
			returnValue.number = JsMath::atan2(v1.number,v2.number);
			returnValue.type   = VsValue::vNUMBER;
		}
		break;

	    case fUNKNOWN:
			returnValue.type = VsValue::vVOID;
		break;
	}
    } else {

		//xxx TRACE("Method call : %s \n",memberName.getString());
		VsValue objValue;

		VsVar *v = obj->evaluateVar(parent,objValue);

		if (objValue.isString()) {
			if (v) {
				v->ref();
				v->unref();
			}
			GvString s;
			objValue.getString(s);
			v = new VsSFString(s); // create a tmp string object 
		}

		if (v==NULL) {
/*			if (objValue.type == vSTRING) {
			}

*/
			// to String on simple types ??? 	
			if (args.getLength() == 0 && streq(memberName.getString(),"toString")) {
				obj->evaluate(objValue,parent);
				GvString s;
				objValue.getString(s);
				returnValue = VsValue(new VsSFString(s));
			    return;
			}

		    Reporter.Error("script error: Can't get object for method %s in %s",memberName.getString(),parent->getContextString());
		    returnValue = VsValue::nothing;
			return;
		}
		//ASSERT(v->refCount >0); // assume for now 
		v->ref();
			returnValue = v->callMember(memberName,&args,parent);
		v->unref();
	}

}



//
//
//

// create a expr for certain member accesses 
VsExpr *VsMemberAccess::NewMemberAccess(VsExpr* obj, const char* memName)
{
	VsExpr *ret = NULL;

    if (obj == NULL) {	    // MATH object
		if (!strcmp(memName, "E")) { ret= new VsConstant(JsMath::E); 
		} else if (!strcmp(memName, "LN10")) {ret= new VsConstant(JsMath::LN10); 
		} else if (!strcmp(memName, "LN2")) {ret= new VsConstant(JsMath::LN2); 
		} else if (!strcmp(memName, "PI")) {ret= new VsConstant(JsMath::PI); 
		} else if (!strcmp(memName, "SQRT1_2")) {ret= new VsConstant(JsMath::SQRT1_2); 
		} else if (!strcmp(memName, "SQRT2")) {ret= new VsConstant(JsMath::SQRT2); 
		} else {
			Reporter.Error("script error: %s no such member",memName);
		}
    }
	else {
		if (!strcmp(memName, "x")) { ret = new VsArrayVar(obj,new VsConstant(0.0f)); }
 		else if (!strcmp(memName, "y")) { ret = new VsArrayVar(obj,new VsConstant(1.0f)); }
 		else if (!strcmp(memName, "z")) { ret = new VsArrayVar(obj,new VsConstant(2.0f)); }
 		//else Reporter.Error("script error: %s no such member",memName);
		else ret = new VsNodeMemberAccess(obj,memName);

	}
	return ret;
}



VsMemberAccess::VsMemberAccess(VsExpr* _obj,
			     const char* memName)
{

    obj = _obj;
	memberName = memName;
	if (obj) obj->ref();


    if (obj == NULL) {	    // MATH object
	
		if (!strcmp(memName, "E")) {
			member = mE;
		} else if (!strcmp(memName, "LN10")) {
			member = mLN10;
		} else if (!strcmp(memName, "LN2")) {
			member = mLN2;
		} else if (!strcmp(memName, "PI")) {
			member = mPI;
		} else if (!strcmp(memName, "SQRT1_2")) {
			member = mSQRT1_2;
		} else if (!strcmp(memName, "SQRT2")) {
			member = mSQRT2;
		} 
		else {
			Reporter.Error("script error: %s no such member",memName);
		}
    }
	else {

	}
}



VsMemberAccess::~VsMemberAccess()
{
	if (obj) obj->unref();
 
}

void 
VsMemberAccess::evaluate(VsValue &returnValue, VsFunctionCall *parent)
{
    //returnValue.type = VsValue::vVOID;

    if (obj == NULL) {	// MATH object
	returnValue.destruct();
	switch (member) {
	    case mE:
		returnValue.number = JsMath::E;
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case mLN10:
		returnValue.number = JsMath::LN10;
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case mLN2:
		returnValue.number = JsMath::LN2;
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case mPI:
		returnValue.number = JsMath::PI;
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case mSQRT1_2:
		returnValue.number = JsMath::SQRT1_2;
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case mSQRT2:
		returnValue.number = JsMath::SQRT2;
		returnValue.type   = VsValue::vNUMBER;
		break;
	    case mUNKNOWN:
		returnValue.type = VsValue::vVOID;
		break;
	}
    }
	else {
		VsValue parentValue;
		VsVar *v = obj->evaluateVar(parent,parentValue);
		if (v==NULL) {
		   Reporter.Error("script error: Can't get object for member %s in %s",memberName.getString(),parent->getContextString());
		   returnValue = VsValue::nothing;
		   return;
		}
		v->ref();
			returnValue = v->evaluateMember(memberName,parent);
		v->unref();
	}


}



VsValue
VsMemberAccess::assign(const VsValue &newValue, VsFunctionCall *fcall) 
{

    VsValue returnValue;

    if (obj == NULL) {
		returnValue = VsValue::nothing;
	} else {
		VsValue parentValue;
		VsVar *v = obj->evaluateVar(fcall,parentValue);
		if (v==NULL) {
		   Reporter.Error("script error: Can't get object for member %s in %s",memberName.getString(),fcall->getContextString());
		   return VsValue::nothing;
		}
		v->ref();
			v->assignMember(memberName,newValue,fcall);
		v->unref();
		return newValue;

	}
    return returnValue;

}

// (appearance.material).diffuseColor 
VsVar *VsMemberAccess::evaluateVar(VsFunctionCall *fcall, VsValue &value)
{ VsVar *ret;
    if (obj == NULL) {
		return NULL;
	} else {

		VsValue parentValue;
		VsVar *v = obj->evaluateVar(fcall,parentValue);

		if (v==NULL) {
		   Reporter.Error("script error: Can't get object for member %s in %s",memberName.getString(),fcall->getContextString());
		   return NULL;
		}
		v->ref();
			ret = v->evaluateMemberVar(memberName,fcall);
		v->unref();
		return ret;
	}
}



VsNodeMemberAccess::VsNodeMemberAccess(VsExpr* _obj,
			     const char* memName)
{
	obj = _obj;
	obj->ref();
	member = memName;

}


VsNodeMemberAccess::~VsNodeMemberAccess()
{
	obj->unref();
	obj = NULL;
 
}
#include <gvnode.h>


void 
VsNodeMemberAccess::evaluate(VsValue &ret, VsFunctionCall *parent)
{
    //returnValue.type = VsValue::vVOID;
	
	VsValue nodeValue;
	obj->evaluate(nodeValue,parent);
	
	ret = VsValue::nothing;

    //return returnValue;
}


VsValue
VsNodeMemberAccess::assign(const VsValue &value, VsFunctionCall *f) 
{
	VsValue nodeValue;

	obj->evaluate(nodeValue,f);
	if (nodeValue.type == VsValue::vNODE ) {

		//nodeValue.node->SendEventIn(member,value.getField());

	}

    returnValue.type = VsValue::vVOID;
    return returnValue;

}

