/*
 * VsStatement.h
 *
 *      Declaration of classes:
 *	  VsValue
 *        VsStatement
 *        VsReturn
 *        VsIf
 *        VsFor
 *        VsStatementList
 *
 * Statement support.  Functions are lists of statements that get
 * executed.
 * 
 * Copyright (C) 1996, Silicon Graphics,  Inc.
 */

#ifndef _VS_STATEMENT_
#define _VS_STATEMENT_

#include <gvbasic.h>

#ifdef __sgi
#include <Inventor/SbPList.h>
#include <Inventor/SbString.h>
#else
#include "vbplist.h"
#endif

class VsStatementList;
class VsFunctionCall;
class VsExpr;
class VsVar;

class GvString;
class GvNode;
class GvField;

class VsField;


//
// VsValues are passed around and represent values of various types.
// Since JavaScript is pretty much untyped, having one value class
// makes things a lot cleaner.  KEEP THIS STRUCTURE AS SMALL AS
// POSSIBLE!  VsValues tend to be passed around by value, so should be kept
// as lightweight as possible.
//

struct VsFloatArray {
    float *array;
    int32_t length;
};

struct VsInt32Array {
    int32_t *array;
    int32_t length;
};

struct VsVoidArray {
    void **array;
    int32_t length;
};

struct VsStringArray {
    GvString *array;
    int32_t length;
};

/*! the basic datatype returned by vrmlscript evaluations
   can hold basic datatypes or references to objects

*/
class VsValue {

public:

    VsValue(double num) 
	{
	    type = vNUMBER;
		number = num;
	}

    VsValue(float num) 
	{
	    type = vNUMBER;
		number = num;
	}
 

    VsValue(int num) 
	{
	    type = vNUMBER;
		number = num;
	}

    VsValue(long num) 
	{
	    type = vNUMBER;
		number = num;
	}

    VsValue(unsigned long num) 
	{
	    type = vNUMBER;
		number = num;
	}

	// warning pointers are references only, no value is copied
    VsValue(void *ptr)
	{
		type = vVOID;
		data = ptr;
	}

    VsValue(char *str)
	{
		type = vSTRING;
		string = str;
	}

    VsValue(const char *str)
	{
		type = vSTRING;
		string = (char *) str;
	}

    VsValue(GvNode * node);


	VsValue(const VsValue &b);

	VsValue(VsField	 *f);


    VsValue() { data = NULL; type = vVOID; };

	~VsValue();

	inline void construct() { 	type = vVOID; } 

	// destruct value, unref pointers 
	void destruct();

	VsValue &operator=(const VsValue &b);


    GvBool		getBool() const;  // Returns value as boolean (0 or 1)
    GvBool		getString(GvString &s) const;  // Returns value as string  (0 or 1)
	
	inline GvBool		get(int &value) 
	{
		switch (type) {
			case vNUMBER: value = (int) number; return(TRUE); // may be overflow
			default: break;
		}
		return FALSE;
	}

	inline GvBool		get(double &value) 
	{
		switch (type) {
			case vNUMBER: value = number; return(TRUE);
			default: break;
		}
		return FALSE;
	}
	
	inline GvBool		get(float &value) 
	{
		switch (type) {
			case vNUMBER: value  = (float) number; return(TRUE); // may be overflow
			default: break;
		}
		return FALSE;
	}

	// get a string 
	const char *getString() const;

    GvNode*	getNode() const;  // Returns value as node 


    union {
		double number;
		char  *string;
		void  *data;
		VsFloatArray *floatArray;
		VsInt32Array *int32Array;
		VsVoidArray  *voidArray;
		VsStringArray *stringArray;

		GvString *gvString;
		GvNode	 *node;

		VsField	 *field;

    };

    enum Type {
		vUNKNOWN, vVOID,
		vNUMBER, vSTRING,
		vOBJECT, vFUNCTION, 

/* obsolete, done via vFIELD 
		vFLOAT_ARRAY,
		vINT32_ARRAY,
		vSTRING_ARRAY,
		vVOID_ARRAY,
		vROTATION,
*/

		vGVSTRING,		// string alloacted 
		vNODE,			// VRML node 
		vFIELD			// VRML field
		
    };
	
	inline gbool isString() const { return type == vSTRING || type == vGVSTRING; }

    unsigned char type; // One of the above ENUM values

    static const VsValue nothing;
	static const VsValue zero;
};    

//
// Statement base class:
//
class VsStatement {
  public:
    // These status codes are returned from statements to tell the
    // enclosing controls structure (loop, if-test, function) what to
    // do:
    enum Status {
		NORMAL,   // Normal return, continue executing statements
		BREAK,    // Break out of enclosing loop/if
		CONTINUE, // Continue enclosing loop/if
		RETURN,   // Return from enclosing function
    };

    VsStatement();
    virtual ~VsStatement();

    // Statement subclasses should override this to do whatever they
    // do and return a Status code.  References to function arguments
    // (which are buried down inside expressions) need to know what
    // function they're executing inside of, which is why a
    // VsFunctionCall is passed in to this routine:
    virtual Status	execute(VsFunctionCall *) = 0;
};

//
// Return statement.
//
class VsReturn : public VsStatement {
  public:
    VsReturn(VsExpr *expr);
    virtual ~VsReturn();

    // Standard execute.  Evalutes expression (if any given), returns
    // a status of RETURN.
    virtual VsStatement::Status	execute(VsFunctionCall *);

  private:
    VsExpr *expression;
};

//
// If statement.
//
class VsIf : public VsStatement {
  public:
    // Pass in expression for condition, if statements, and else
    // statements (NULL if just a "bare" if).
    VsIf(VsExpr *condition, VsStatementList *ifStatements,
	 VsStatementList *elseStatements);
    virtual ~VsIf();

    // Evaluates.  Depending on condition expression, either if or
    // else statements are executed.
    virtual VsStatement::Status	execute(VsFunctionCall *);

  private:
    VsExpr *condition;
    VsStatementList *ifStatements, *elseStatements;
};

//
// For statement.
//
class VsFor : public VsStatement {
  public:
    VsFor(VsExpr *initial, VsExpr *condition, VsExpr *increment, 
	  VsStatementList *body);
    virtual ~VsFor();

    // Execute statements repeatedly until terminate expression is
    // true.
    virtual VsStatement::Status	execute(VsFunctionCall *);

  private:
    VsExpr *initial, *increment, *condition;
    VsStatementList *body;
};

//
// For..In statement.
//
class VsForIn : public VsStatement {
  public:
    VsForIn(VsVar *variable, VsExpr *object, VsStatementList *body);
    virtual ~VsForIn();

    virtual VsStatement::Status	execute(VsFunctionCall *);
    
  private:
    VsVar  *variable;
    VsExpr *object;
    VsStatementList *body;
};

//
// While statement.
//
class VsWhile : public VsStatement {
  public:
    VsWhile(VsExpr *condition, VsStatementList *body);
    virtual ~VsWhile();

    // Execute statements repeatedly until terminate expression is
    // true.
    virtual VsStatement::Status	execute(VsFunctionCall *);

  private:
    VsExpr *condition;
    VsStatementList *body;
};

//
// Break statement.
//
class VsBreak : public VsStatement {
  public:
    VsBreak();

    virtual ~VsBreak();

    // Execute statements repeatedly until terminate expression is
    // true.
    virtual VsStatement::Status	execute(VsFunctionCall *);

};

//
// Continue statement.
//
class VsContinue : public VsStatement {
  public:
    VsContinue();
    virtual ~VsContinue();

    // Execute statements repeatedly until terminate expression is
    // true.
    virtual VsStatement::Status	execute(VsFunctionCall *);

};

//
// With statement.
//
class VsWith : public VsStatement {
  public:
    VsWith(VsExpr *object, VsStatementList *body );

    virtual ~VsWith();

    // Execute statements repeatedly until terminate expression is
    // true.
    virtual VsStatement::Status	execute(VsFunctionCall *);

    private:
	VsExpr* object;
	
	VsStatementList* body;
};

//
// Little wrapper class for VbPList that bundles up some annoying casting:
//
class VsStatementList : public VbPList {
  public:
    VsStatementList()                                 {}
    VsStatementList(int initSize) : VbPList(initSize) {}
    ~VsStatementList();	// Destructor deletes all statements

    // Useful routine, executes all while return status is NORMAL:
    VsStatement::Status	execute(VsFunctionCall *);

    void    append(VsStatement *statement)
        	{ if (statement != NULL) { this->VbPList::append((void *)statement); }}
    int     find(VsStatement *statement)
	        { return this->VbPList::find((void *)statement); }
    void    insert(VsStatement *statement, int addBefore)
	        { if (statement != NULL) {this->VbPList::insert((void *)statement, addBefore); }}
    VsStatement* &operator [](int i) const
	        { return (VsStatement * &)(this->VbPList::operator[](i)); }
};


//
// VAR statement.
//
class VsVarStatement : public VsStatement {
  public:
    VsVarStatement(VsStatementList *body);
    virtual ~VsVarStatement();

    // Execute statements repeatedly until terminate expression is
    // true.
    virtual VsStatement::Status	execute(VsFunctionCall *);

  private:
    VsStatementList *body;
};

#endif /* _VS_STATEMENT_ */
