/*
 * VsExpr.c++
 *
 *     Implementation of classes:
 *        VsExpr
 *        VsConstant
 *        VsAssign
 *        VsArith
 *        VsLogicOp
 *	  VsUArithOp
 *	  VsULogicOp
 *	  VsUIncDec
 *	  VsComma
 *	  VsArrayVar
 *
 * Expressions
 * 
 * Copyright (C) 1996, Silicon Graphics,  Inc.
 */
#include "stdafx.h"

#include <assert.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>


#include "vsexpr.h"
#include "vsfields.h"

#include <gvstring.h>
#include "greporter.h"


class VsFunctionDef;
class VsFunctionCall;

#include "vsfunction.h"

class VsNewCall;
#include "y_tab.h"  /* For the PLUS/etc #defines */

VsExpr::VsExpr()
{
    refCount = 0;
}

void
VsExpr::ref()
{
    ++refCount;
}

void
VsExpr::unref()
{
    --refCount;
    if (refCount <= 0) delete this;
}

VsExpr::~VsExpr()
{
}

 // returns this 
VsVar *VsExpr::evaluateVar(VsFunctionCall *fcall, VsValue &ret)
{

	// let's evaluate and see if it evaluates to field
	// could be new call, so keep reference 
	evaluate(ret,fcall);

	switch (ret.type) {

		case VsValue::vFIELD:
		//	ASSERT(ret.field->refCount >1);
			// otherwise it was a temp field ???

			return(ret.field);

		default :
			break;
	}


	return NULL;
}



VsValue
VsVar::assign(const VsValue &v, int32_t, VsFunctionCall *f)
{
	Reporter.Error("array assignment not supported, uninitalized variable ?");
	return VsValue::nothing;
    //return assign(v, f);
}

// Object support 
// assign to an member 
VsValue 
VsVar::assignMember(const VsMember &member, const VsValue &v, VsFunctionCall *)
{
	Reporter.Error("member assignment not supported %s, uninitalized variable ?",member.getString());
	return VsValue::nothing;
}

// evaluate an member 
VsValue
VsVar::evaluateMember(const VsMember &member, VsFunctionCall *)
{
	Reporter.Error("member not supported %s",member.getString());
	return VsValue::nothing;
}

// invoke member function 
VsValue VsVar::callMember(const VsMember &member,VsExprList *args, VsFunctionCall *)
{
	Reporter.Error("member function not supported %s",member.getString());
	return VsValue::nothing;
}




 // returns this 
VsVar *VsVar::evaluateVar(VsFunctionCall *, VsValue &ret)
{
	return this;
}

// evaluate element  
void
VsVar::evaluate(VsValue &ret,int32_t index, VsFunctionCall *)
{
	Reporter.Error("array acess not supported");
	ret = VsValue::nothing;

}

// evaluate element L-value 
VsVar*	VsVar::evaluateVar(int32_t index, VsFunctionCall * /*, VsValue &ret*/)
{
	Reporter.Error("array acess not supported");
	return NULL;
}

// get Var representing the objects member 
VsVar* VsVar::evaluateMemberVar(const VsMember &member, VsFunctionCall *fcall)
{
	Reporter.Error("property not supported %s",member.getString());
	return NULL;
}




VsStatement::Status
VsExpr::execute(VsFunctionCall *f)
{
	VsValue ret;
    evaluate(ret,f); 
    return VsStatement::NORMAL;
}

	

VsConstant::VsConstant(const char *string)
{
    myValue.type = VsValue::vSTRING;
    myValue.string = (char *)string;	// string is alloc'ed in scan.lpp
					// can be freed in here
}

VsConstant::VsConstant(double number)
{
    myValue.type = VsValue::vNUMBER;
    myValue.number = number;
}

VsConstant::VsConstant(const VsValue &value) : myValue(value) 
{
}

VsConstant::~VsConstant()
{
    if (myValue.type == VsValue::vSTRING) {
		delete[] myValue.string;
		// or free ?????
    }
}

void
VsConstant::evaluate(VsValue &ret,VsFunctionCall *)
{
    ret  = myValue;
}

///////////////////////////////////////////////////////////////////////////

VsArrayVar::VsArrayVar(VsExpr *_array, VsExpr *_subscript)
{
    array     = _array;  // for now, assume this is a Var
    subscript = _subscript;

    if (array != NULL) {
        array->ref();
    }
    if (subscript != NULL) {
        subscript->ref();	
    }
}

VsArrayVar::~VsArrayVar()
{
    if (array != NULL) {
        array->unref();
    }
    if (subscript != NULL) {
        subscript->unref();
    }
}

void
VsArrayVar::evaluate(VsValue &ret,VsFunctionCall *f)
{
    if ((array == NULL) || (subscript == NULL)) {
		ret = VsValue::nothing;
		return;
    }

    VsValue sub;
	subscript->evaluate(sub,f);

    int32_t index = (int32_t)sub.number;

	// standard case 
	VsValue value;

	VsVar *var = array->evaluateVar(f,value);

	if (var) {
		var->ref();
			var->evaluate(ret,index, f);
		var->unref();
		return;

    }
	

    VsValue arr;
	array->evaluate(arr,f);

    if (arr.type == VsValue::vFIELD) {
		arr.field->evaluate(ret,index, f);
		return;
    } else 

/* obsolete ??
    if ((arr.type == VsValue::vFLOAT_ARRAY) ||
		(arr.type == VsValue::vROTATION)) {
		return(arr.floatArray->array[index]);
    } else if (arr.type == VsValue::vINT32_ARRAY) {
		return(arr.int32Array->array[index]);
    } else if (arr.type == VsValue::vSTRING_ARRAY) {
		return((char *)(arr.stringArray->array[index].getString()));
    } else if (arr.type == VsValue::vVOID_ARRAY) {
		return(arr.voidArray->array[index]);
    } else 
*/

	{
		Reporter.Error("Bad array variable");
    }

  	ret = VsValue::nothing;
}

VsValue
VsArrayVar::assign(const VsValue &v, VsFunctionCall* f)
{
    if ((array == NULL) || (subscript == NULL)) {
		// ERROR
		return VsValue::nothing;
    }     

    VsValue ss; 
	subscript->evaluate(ss,f);

    int32_t index = (int32_t)ss.number;

	// standard case 
	VsValue value;
	VsVar *var = array->evaluateVar(f,value);

	if (var) {
		VsValue ret;
		var->ref();
			ret = var->assign(v, index, f);
		var->unref();
		return ret;

    }

	// old case ????? 
	VsValue arr;
	array->evaluate(arr,f);

    if (arr.type == VsValue::vFIELD) {
		return arr.field->assign(v, index, f);
    } else 
/*
    if ((arr.type == VsValue::vFLOAT_ARRAY) ||
        (arr.type == VsValue::vROTATION)) {
		array->assign(v, index, f);
        return(arr.floatArray->array[index]);
    } else if (arr.type == VsValue::vINT32_ARRAY) {
		array->assign(v, index, f);
        return(arr.int32Array->array[index]);
    } else if (arr.type == VsValue::vSTRING_ARRAY) {
		array->assign(v, index, f);
        return((char *)(arr.stringArray->array[index].getString()));
    } else if (arr.type == VsValue::vVOID_ARRAY) {
		array->assign(v, index, f);
        return(arr.voidArray->array[index]);
    } else 
*/
	{
		Reporter.Error("Bad array variable");
    }

    return VsValue(0.0);
}

// (x[y] . member = z
// x[y][z]
VsVar *VsArrayVar::evaluateVar(VsFunctionCall *f, VsValue &value)
{ VsVar *ret;

    if ((array == NULL) || (subscript == NULL)) {
		return NULL;
    }

    VsValue sub;
	subscript->evaluate(sub,f);

    int32_t index = (int32_t)sub.number;

	// VsValue valueA;
	VsVar *var = array->evaluateVar(f,value);

	if (var) {
		var->ref();
		    ret = var->evaluateVar(index,f /*,value*/);
		var->unref();
    }


	return ret;
}



VsAssign::VsAssign(VsVar *v, VsExpr *e)
{
//    assert(v != NULL && e != NULL);

    var = v;
    if (var != NULL) {
        var->ref();
    }
    expr = e;
    
    if (expr != NULL) {
        expr->ref();
    }
}

VsAssign::~VsAssign()
{
    if (var != NULL) {	
        var->unref();
    }
    if (expr != NULL) {
        expr->unref();
    }
}

void
VsAssign::evaluate(VsValue &ret, VsFunctionCall *f)
{
    // This is the normal scalar assignment.
    if ((var != NULL) && (expr != NULL)) {
		expr->evaluate(ret,f);
        ret = var->assign(ret, f); // tbd check this 
    } else {
		ret = VsValue::zero;
    }
}

VsArith::VsArith(VsExpr *e1, VsExpr *e2, int op)
{
//    assert(e1 != NULL && e2 != NULL);

    expr1 = e1;
    if (expr1 != NULL) {
        expr1->ref();
    }
    expr2 = e2;
    if (expr2 != NULL) {
        expr2->ref();
    }
    operation = op;
}

VsArith::~VsArith()
{
    if (expr1 != NULL) {
        expr1->unref();
    }
    if (expr2 != NULL) {
        expr2->unref();
    }
}

void
VsArith::evaluate(VsValue &result, VsFunctionCall *f)
{
    if ((expr1 == NULL) || (expr2 == NULL)) {
		result = VsValue::nothing;
		return;
    }

    VsValue v1; expr1->evaluate(v1,f);
    VsValue v2; expr2->evaluate(v2,f);

    int32_t tempInt;

    if (v1.type == VsValue::vNUMBER && v2.type == VsValue::vNUMBER) {
	result.destruct();
	result.type = VsValue::vNUMBER;

	switch (operation) {
	  case tPLUS:
	    result.number = v1.number + v2.number;
	    break;
	  case tMINUS:
	    result.number = v1.number - v2.number;
	    break;
	  case tMULTIPLY:
	    result.number = v1.number * v2.number;
	    break;
	  case tDIVIDE:
	    result.number = v1.number / v2.number;
	    break;
	  case tMOD:
	    result.number = ((int32_t)v1.number) % ((int32_t)v2.number);
	    break;
	  case tAND:
	    result.number = ((uint32_t)v1.number) & ((uint32_t)v2.number);
	    break;
	  case tOR:
	    result.number = ((uint32_t)v1.number) | ((uint32_t)v2.number);
	    break;
	  case tXOR:
	    result.number = ((uint32_t)v1.number) ^ ((uint32_t)v2.number);
	    break;
	  case tLSHIFT:
	    result.number = ((uint32_t)v1.number) << ((uint32_t)v2.number);
	    break;
	  case tRSHIFT:
	    result.number = ((uint32_t)v1.number) >> ((uint32_t)v2.number);
	    break;
	  case tRSHIFTFILL:
	    // make sure high order bit is zero before shifting
	    if (v2.number >= 1) {
		tempInt = (uint32_t)v1.number & 0x7FFFFFFF;
	    } else {
		tempInt = (uint32_t)v1.number;
	    }
	    result.number = (tempInt >> ((int32_t)v2.number));
	    break;
	  default:
		TRACE("VsScript : unimplemented op %d %d %d \n",v1.type,operation,v2.type);
		Reporter.Error("Bad numeric operation in %s",f->getContextString()); 
	    assert(0);
	    break;
	}
	}
    else 
	if (1) /*v1.isString() && v2.isString())*/ {
		// result.type = VsValue::vSTRING;

		switch (operation) {
		case tPLUS: {
			
			GvString vs,vs2;

			v1.getString(vs);
			v2.getString(vs2);

			vs += vs2;

			result = VsValue(new VsSFString(vs));
			return;

			}
			break;
		default:
			Reporter.Error("Bad numeric operation for this type in %s",f->getContextString()); 
			TRACE("Script : unimplemented op %d %d %d \n",v1.type,operation,v2.type);
			assert(0);
			break;
		}

    } // else 
	else {
		TRACE("VsScript : unimplemented op %d %d %d \n",v1.type,operation,v2.type);
		Reporter.Error("Bad numeric operation in %s",f->getContextString()); 
		assert(0);

	}
   
}

VsCondExpr::VsCondExpr(VsExpr *t, VsExpr *e1, VsExpr *e2)
{
//    assert(t != NULL && e1 != NULL && e2 != NULL);

    test = t;
    if (test != NULL) {
        test->ref();
    }
    expr1 = e1;
    if (expr1 != NULL) {
        expr1->ref();
    }
    expr2 = e2;
    if (expr2 != NULL) {   
        expr2->ref();
    }
}

VsCondExpr::~VsCondExpr()
{
    if (test != NULL) {
        test->unref();
    }
    if (expr1 != NULL) {
        expr1->unref();
    }
    if (expr2 != NULL) {
        expr2->unref();
    }
}

void
VsCondExpr::evaluate(VsValue &ret,VsFunctionCall *f)
{
    if (test == NULL) {
		ret = VsValue::nothing;
		return;
    }
	test->evaluate(ret,f);

    if (ret.getBool()) {
		if (expr1 == NULL) {
			ret = VsValue::zero;
		}
		else expr1->evaluate(ret,f);
    }
    else {
		if (expr2 == NULL) {
			ret = VsValue::zero;
		}
		else expr2->evaluate(ret,f);
    }
}

VsIncDec::VsIncDec(VsExpr *_var, int _increment, int _prefix)
{
    ASSERT(_var != NULL);

    var = _var;
    if (var != NULL) {
        var->ref();
    }

    increment = _increment;
    prefix    = _prefix;
    
}

VsIncDec::~VsIncDec()
{
    if (var != NULL) {
        var->unref();
    }
}

void
VsIncDec::evaluate(VsValue &ret,VsFunctionCall *f)
{
    if (var == NULL) {
		ret = VsValue::zero;
		return;
    }

    VsValue initval;
	var->evaluate(initval,f);

    VsValue newval = initval;

    if (initval.type == VsValue::vNUMBER) {
		if (increment) {
			newval.number += 1;
		} else {
			newval.number -= 1;
		}
    } else {
		// JavaScript only allows increment/decrement for numeric types
		// ??? warn if not numeric ???
		Reporter.Error("Need numeric expression for increment operator in %s",f->getContextString()); 
    }

    // This is the normal scalar assignment.
    //var->assign(newval, f);

	VsValue value;
	VsVar *var = this->var->evaluateVar(f,value);
	//could be SFFloat SFInt32 etc 

	if (var) {
		var->ref();
			var->assign(newval, f);
		var->unref();
    }  else {
		// could be local var 
		//this->var->assign(newval, f);
		Reporter.Error("++:Cant get variable");
	}


    if (prefix) {
		ret = newval;
    } else {
		ret = initval;
    }
}

VsUArithOp::VsUArithOp(VsExpr *e, int op)
{
//    assert(e != NULL);

    expr = e;
    if (expr != NULL) {
        expr->ref();
    }

    operation = op;
}

VsUArithOp::~VsUArithOp()
{
    if (expr != NULL) {
        expr->unref();
    }
}

void 
VsUArithOp::evaluate(VsValue &result,VsFunctionCall *f)
{
    if (expr == NULL) {
		result = VsValue::nothing;
		return;
    }

    VsValue v;
	expr->evaluate(v,f);

    if (v.type == VsValue::vNUMBER) {

	switch (operation) {
	  case tMINUS:
	    result = VsValue(-v.number); 
		return;
	  case tONESCOMP:
	    result = VsValue(~((int32_t)v.number));
		return;
	  default:
		Reporter.Error("Need numeric expression for increment operator in %s",f->getContextString()); 
	    assert(0);
	    break;
	}
    } else {
  	    Reporter.Error("Need numeric expression for unary operator in %s",f->getContextString()); 

	}	

    result = VsValue::nothing;
}

VsLogicOp::VsLogicOp(VsExpr *e1, VsExpr *e2, int op)
{
//    assert(e1 != NULL && e2 != NULL);

    expr1 = e1;
    if (expr1 != NULL) {
        expr1->ref();
    }
    expr2 = e2;
    if (expr2 != NULL) {
        expr2->ref();
    }
    operation = op;
}

VsLogicOp::~VsLogicOp()
{
    if (expr1 != NULL) {
        expr1->unref();
    }
    if (expr2 != NULL) {
        expr2->unref();
    }
}

void 
VsLogicOp::evaluate(VsValue &result,VsFunctionCall *f)
{
    if (expr1 == NULL) {
		result = VsValue::zero;
		return;
    }

    VsValue v1;
	expr1->evaluate(v1,f);

    result.destruct();

    result.type = VsValue::vNUMBER;

    // short circuit evaluation
    // if operation is LAND or LOR, don't always evaluate v2
    if ((operation == tLAND) && (v1.getBool() == FALSE)) {
		result.number = FALSE;
		return;
    } else if ((operation == tLOR) && (v1.getBool() == TRUE)) {
		result.number = TRUE;
		return;
    }

    if (expr2 == NULL) {
		result = VsValue::zero;
		return;
    }

    VsValue v2;
	expr2->evaluate(v2,f);

    if (v1.type == VsValue::vNUMBER && v2.type == VsValue::vNUMBER) {
	switch (operation) {
	  case tEQ:
	    result.number = (v1.number == v2.number);
	    break;
	  case tNE:
	    result.number = (v1.number != v2.number);
	    break;
	  case tLT:
	    result.number = (v1.number < v2.number);
	    break;
	  case tLE:
	    result.number = (v1.number <= v2.number);
	    break;
	  case tGT:
	    result.number = (v1.number > v2.number);
	    break;
	  case tGE:
	    result.number = (v1.number >= v2.number);
	    break;
	  case tLAND:
	    result.number = v1.number && v2.number;
	    break;
	  case tLOR:
	    result.number = v1.number || v2.number;
	    break;
	  default:
  	    Reporter.Error("Bad operation in %s",f->getContextString()); 
	    assert(0);
	    break;
	}
    }
    else  {
		GvString s1;
		GvString s2;
		v1.getString(s1);
		v2.getString(s2);

		if (1) {
	int cmp = strcmp(s1, s2);

	switch (operation) {
	  case tEQ:
	    result.number = (cmp == 0);
	    break;
	  case tNE:
	    result.number = (cmp != 0);
	    break;
	  case tLT:
	    result.number = (cmp < 0);
	    break;
	  case tLE:
	    result.number = (cmp <= 0);
	    break;
	  case tGT:
	    result.number = (cmp > 0);
	    break;
	  case tGE:
	    result.number = (cmp >= 0);
	    break;
	  default:
  	    Reporter.Error("Bad operation in %s",f->getContextString()); 
	    assert(0);
	    break;
	}
	}
    }
}

VsULogicOp::VsULogicOp(VsExpr *e, int op)
{
//    assert(e != NULL);

    expr = e;
    if (expr != NULL) {
        expr->ref();
    }
    operation = op;
}

VsULogicOp::~VsULogicOp()
{
    if (expr != NULL) {
        expr->unref();
    }
}

void 
VsULogicOp::evaluate(VsValue &result,VsFunctionCall *f)
{
    if (expr == NULL) {
		result = VsValue::zero;
		return;
    }

    VsValue v;
	expr->evaluate(v,f);


	result.destruct();
    result.type = VsValue::vNUMBER;

    if (v.type == VsValue::vNUMBER) {
	switch (operation) {
	  case tNOT:
	    result.number = ! (v.number != 0.0);
	    break;
	  default:
	    assert(0);
	    break;
	}
    }
    else if (v.type == VsValue::vSTRING) {
        if (v.string && v.string[0] != '\0')
            result.number = 0.0;
        else
            result.number = 1.0;
    }
  	else  Reporter.Error("Bad operation in %s",f->getContextString()); 
}

VsComma::VsComma(VsExpr *e1, VsExpr *e2)
{
    expr1 = e1;
    if (expr1 != NULL) {
        expr1->ref();
    }
    expr2 = e2;
    if (expr2 != NULL) {
        expr2->ref();
    }
}

VsComma::~VsComma()
{
    if (expr1 != NULL) {
        expr1->unref();
    }
    if (expr2 != NULL) {
        expr2->unref();
    }
}

void 
VsComma::evaluate(VsValue &ret,VsFunctionCall *f)
{
    if (expr1 != NULL) {
		expr1->evaluate(ret,f);
    }

    if (expr2 != NULL) {
		expr2->evaluate(ret,f);
    } else {
		ret = VsValue::zero;
    }
}
