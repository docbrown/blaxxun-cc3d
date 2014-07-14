/*
 * VsStatement.cpp
 *
 *     Implementation of classes:
 *	  VsValue
 *        VsStatement
 *        VsReturn
 *        VsIf
 *        VsFor
 *
 * Copyright (C) 1996, Silicon Graphics,  Inc.
 */

#include "stdafx.h"

#include <assert.h>
#include "vsexpr.h"
#include "vsfunction.h"

#include <gvnode.h>
#include <gvfield.h>

#include "vsfields.h"
#include "vsstatement.h"


const VsValue VsValue::nothing(0.0);
const VsValue VsValue::zero(0.0);


	VsValue::VsValue(GvNode * newNode)
	{
		type = vNODE;
		node = newNode;
		if (node) node->ref();
	}


	VsValue::VsValue(VsField	 *newField)
	{
		type = vFIELD;
		field = newField;
		if (field) field->ref();
	}




VsValue::~VsValue()
{
	// VC generates linear comparisson & no jump table
	switch(type) {
	case vFIELD :		// HG : major case first 
		if (field) field->unref();
		break;
	case vGVSTRING :
		if (gvString) delete gvString;
		break;
	case vNODE :
		if (node) node->unref();
		break;
	default :
		break;
	}
	// type = vVOID; not really needed, no more ref
}

void VsValue::destruct()
{
	switch(type) {
	case vFIELD :
		if (field) field->unref();
		break;

	case vGVSTRING :
		if (gvString) delete gvString;
		break;
	case vNODE :
		if (node) node->unref();
		break;

	default :
		break;
	}
	type = vVOID;
	data = NULL;
}



VsValue::VsValue(const VsValue &b)
{
	data = NULL; 
	type = b.type;

	switch(b.type) {
	case vNUMBER :
		 number = b.number;
		 break;
	case vGVSTRING :
		if (b.gvString) 
			gvString = new GvString(*b.gvString);
		else gvString = NULL;
		break;
	case vNODE :
		if (b.node) {
			node = b.node;
			node->ref();
		} else node = NULL;
		break;
	case vFIELD :
		if (b.field) {
			field = b.field;
			field->ref();
		}
		else field = NULL;
		break;
	default :
		data = b.data;
		break;
	}
}


VsValue & VsValue::operator=(const VsValue &b)
{

	switch(type) {
	case vGVSTRING :
		if (gvString) delete gvString;
		break;
	case vNODE :
		if (node) node->unref();
		break;
	case vFIELD :
		if (field) field->unref();
	default :
		break;
	}
//	type = vVOID;

	data = NULL; 

	type = b.type;

	switch(b.type) {
	case vNUMBER :
		 number = b.number;
		 break;
	case vGVSTRING :
		if (b.gvString) 
			gvString = new GvString(*b.gvString);
		else gvString = NULL;
		break;
	case vNODE :
		if (b.node) {
			node = b.node;
			node->ref();
		} else node = NULL;
		break;
	case vFIELD :
		if (b.field) {
			field = b.field;
			field->ref();
		}
		else field = NULL;
		break;
	default :
		data = b.data;
		break;
	}
	return(*this);
}


GvBool

VsValue::getBool() const
{
	switch(type) {

    case vNUMBER:
		if (number != 0.0) return TRUE;
		break;
	case vSTRING : 	// Null or empty string is FALSE:
		if (string != NULL && string[0] != '\0') return TRUE;
		return(0);

	case vGVSTRING : 	// Null or empty string is FALSE:
		if (gvString != NULL && gvString->getString()[0] != '\0') return 1;
		return(0);
	
	case vFIELD:
		if (field == NULL) return 0;
		switch (field->getFieldType()) {
		case SFNODE :
			return ((VsSFNode*)field)->evaluateBool();
		case SFSTRING :
			return ((VsSFString*)field)->evaluateBool();
		case MFNODE:
		case MFSTRING:
		case MFLONG:
		case MFINT32:
		case MFFLOAT:
		case MFROTATION:
		case MFVEC2F:
		case MFVEC3F:
			return (((VsMField*)field)->getLength())  >0;

		}
		return 1;
		break;

	case vNODE:
		return (node != NULL);
	default :
		break;
    }
    return 0;
}

// transform value into string 
GvBool 
VsValue::getString(GvString &s) const
{
	switch(type) {
	case vNUMBER :
		{
			char *str= s.getBuffer(60);
			sprintf(str,"%.16g",number);
			s.releaseBuffer();
		}
		break;

	case vSTRING :
		if (string) 
			s = string;
		else s = "";
		break;

	case vGVSTRING :
		if (gvString) 
			s = *gvString;
		else s = "";
		break;

	case vFIELD:  	
		if (SFSTRING  == field->getFieldType()) {
			s = ((VsSFString *)field)->getField()->get();
		} 
		else if (MFSTRING  == field->getFieldType() && ((VsMFString *)field)->getLength()  == 1) {
			s = ((VsMFString *)field)->getField()->get1(0);
		} 
		else {	
			// implicit call toString 
			GvField *v=field->getField();
			if (!v) return FALSE;
			v->get(s); 
			return TRUE;
		}
		break;


	default :
		return FALSE;
		break;
	}
	return(TRUE);
}

/*
// get a string 
const char *VsValue::getString() const
{
	switch(type) {
	case vSTRING :
		return(string);
		break;

	case vGVSTRING :
		if (gvString) 
			return gvString->getString();
		break;
	case vFIELD:  	
		if (SFSTRING  == field->getFieldType()) {
			return ((VsSFString *)field)->getField()->get();
		} 
		else if (MFSTRING  == field->getFieldType() && ((VsMFString *)field)->getLength()  == 1) {
			return ((VsMFString *)field)->getField()->get1(0);
		} 
		break;

	default :
		break;
	}
	return(NULL);
}

*/

GvNode*	VsValue::getNode() const  // Returns value as node 
{
	switch(type) {
	case vNODE :
		return(node);
		break;
	case vFIELD:  	
		if (SFNODE  == field->getFieldType()) {
			return ((VsSFNode *)field)->getField()->get();
		} 
		else if (MFNODE  == field->getFieldType() && ((VsMFNode *)field)->getLength()  == 1) {
			return ((VsMFNode *)field)->getField()->get1(0);
		} 
		else return FALSE;
		break;
	default :
		break;
	}
	return(NULL);
}



VsStatement::VsStatement()
{
}

VsStatement::~VsStatement()
{
}

VsReturn::VsReturn(VsExpr *expr)
{
    expression = expr;
    if (expression != NULL) {
	expression->ref();
    }
}

VsReturn::~VsReturn()
{
    if (expression != NULL) {
	expression->unref();
    }
}

VsStatement::Status
VsReturn::execute(VsFunctionCall *f)
{
    if (expression != NULL) {
		//f->setReturnValue(expression->evaluate(f));
		expression->evaluate(f->returnValue,f);
    }
    return VsStatement::RETURN;
}

VsIf::VsIf(VsExpr *_condition, VsStatementList *_if,
	   VsStatementList *_else)
{
    assert(_condition != NULL);

    condition = _condition;
    if (condition) condition->ref();
    ifStatements = _if;
    elseStatements = _else;
}

VsIf::~VsIf()
{
    if (condition) condition->unref();
    if (ifStatements != NULL) delete ifStatements;
    if (elseStatements != NULL) delete elseStatements;
}

VsStatement::Status
VsIf::execute(VsFunctionCall *f)
{
    VsStatement::Status result = VsStatement::NORMAL;
	VsValue v;

    // do either if or else statements, executing all the statements
    // on the list unless we hit a return, break, or continue statement.
    if (condition && (condition->evaluate(v,f), v.getBool())) {
		if (ifStatements != NULL) {
			result = ifStatements->execute(f);
		}
    }
    else {
		if (elseStatements != NULL) {
			result = elseStatements->execute(f);
		}
    }

    return result;
}

VsFor::VsFor(VsExpr *_initial, VsExpr *_condition, VsExpr *_increment,
	   VsStatementList *_body)
{
    assert(_body != NULL);

    body = _body;
    initial = _initial;
    if (initial) initial->ref();
    condition = _condition;
    if (condition) condition->ref();
    increment = _increment;
    if (increment) increment->ref();
}

VsFor::~VsFor()
{
    delete body;
    if (initial != NULL) initial->unref();
    if (increment != NULL) increment->unref();
    if (condition != NULL) condition->unref();
}

VsStatement::Status
VsFor::execute(VsFunctionCall *f)
{

	VsValue v;

	if (initial) 
		initial->evaluate(v,f);

    for	(;
		condition ? (condition->evaluate(v,f), v.getBool()) : 1; // ??
	  ) {

	VsStatement::Status s = body->execute(f);

	if (s == VsStatement::CONTINUE) { 
		if (increment) increment->evaluate(v,f);
	    continue;
	}
	if (s == VsStatement::BREAK) {
	    break;
	}
	if (s == VsStatement::RETURN) {
	    return s;
	}
	if (increment) increment->evaluate(v,f);
    }
    return VsStatement::NORMAL;
}

VsForIn::VsForIn(VsVar *_variable, VsExpr *_object, VsStatementList *_body)
{
    assert(_body != NULL);

    body = _body;
    variable = _variable;
    if (variable) variable->ref();
    object = _object;
    if (object) object->ref();
}

VsForIn::~VsForIn()
{
    delete body;
    if (variable != NULL) variable->unref();
    if (object != NULL)   object->unref();
}

VsStatement::Status
VsForIn::execute(VsFunctionCall *f)
{
    // NOT IMPLEMENTED YET - DO NOTHING

    return VsStatement::NORMAL;
}

VsWhile::VsWhile(VsExpr *_condition, VsStatementList *_body)
{
    assert(_body != NULL && _condition != NULL);

    body = _body;

    condition = _condition;
    condition->ref();
}

VsWhile::~VsWhile()
{
    assert (condition != NULL);

    delete body;
    condition->unref();
}

VsStatement::Status
VsWhile::execute(VsFunctionCall *f)
{
    assert (condition != NULL);
	VsValue v;

    while ( (condition->evaluate(v,f), v.getBool())) {
	VsStatement::Status s = body->execute(f);

	if (s == VsStatement::CONTINUE) { 
	    continue;
	}
	if (s == VsStatement::BREAK) {
	    break;
	}
	if (s == VsStatement::RETURN) {
	    return s;
	}
    }   

    return VsStatement::NORMAL;
}

VsBreak::VsBreak()
{
}

VsBreak::~VsBreak()
{
}

VsStatement::Status
VsBreak::execute(VsFunctionCall *f)
{
    return VsStatement::BREAK;
}

VsContinue::VsContinue()
{

}

VsContinue::~VsContinue()
{
}

VsStatement::Status
VsContinue::execute(VsFunctionCall *f)
{
    return VsStatement::CONTINUE;
}

VsWith::VsWith(VsExpr *_object, VsStatementList* _body)
{
    object = _object;
    if (object) object->ref();

    assert(_body != NULL);

    body = _body;

}

VsWith::~VsWith()
{
    if (object) object->unref();

    delete body;
}

VsStatement::Status
VsWith::execute(VsFunctionCall *f)
{
    // CURRENTLY UNIMPLEMENTED - DO NOTHING

    return VsStatement::NORMAL;
}

VsStatementList::~VsStatementList()
{
    for (int i = 0; i < getLength(); i++) {
	if ((*this)[i] != NULL) {
    	    delete (*this)[i];
	}
    }
}

VsStatement::Status
VsStatementList::execute(VsFunctionCall *f)
{
    VsStatement::Status result = VsStatement::NORMAL;

    for (int i = 0; i < getLength() && result == VsStatement::NORMAL; i++) {
		result = (*this)[i]->execute(f);
    }
    return result;
}




VsVarStatement::VsVarStatement(VsStatementList *_body)
{
    assert(_body != NULL );
    body = _body;
}

VsVarStatement::~VsVarStatement()
{
    delete body;
}


VsStatement::Status
VsVarStatement::execute(VsFunctionCall *f)
{
    VsStatement::Status result = VsStatement::NORMAL;

	VsStatement::Status s = body->execute(f);
    
	return s;
}

