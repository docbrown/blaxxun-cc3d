/*
 * VsExpr.h
 *
 *      Declaration of classes:
 *        VsExpr
 *        VsConstant
 *	  VsVar
 *        VsAssign
 *        VsArith
 *        VsUArithOp
 *        VsLogicOp
 *        VsULogicOp
 *	  VsIncDec
 *	  VsComma
 *	  VsExprList
 *
 * Expression support
 * 
 * Copyright (C) 1996, Silicon Graphics,  Inc.
 */



/* hg

  14.12.99 changed

    virtual VsValue		evaluate(VsFunctionCall *) = 0;

to 

  virtual void		evaluate(VsValue &ret, VsFunctionCall *) = 0;

  because temporary VsValue copy / destructor calls cause a serious amount of time


*/

#ifndef _VS_EXPR_
#define _VS_EXPR_

class VsExprList;

#include "vsstatement.h"


#include "vbplist.h"

#include <gvstring.h>

#define VsMember GvName

class VsFunctionCall;
class VsMethodCall;
class VsMemberAccess;

//
// Expression base class:
//
class VsExpr : public VsStatement {
  public:
    // Constructor, just initializes reference count to zero
    VsExpr();

    // Expressions are also statements, and so must have an execute()
    // method.  The execute() method just calls evaluate() and returns
    // a NORMAL status
    virtual VsStatement::Status	execute(VsFunctionCall *);

    //! Evaluates this expression, returning resulting value.
    virtual void evaluate(VsValue &ret, VsFunctionCall *) = 0;

    //! Evaluates this expression, returning an variable (Lvalue)
	virtual VsVar *evaluateVar(VsFunctionCall *, VsValue &value);


    // Expressions must be reference counted because variables may be
    // multiply instanced in the parse tree
    void	ref();
    void	unref();

  protected:
    virtual ~VsExpr();
  private:
    short refCount;
};

//
// Constant numbers and/or strings:
//
class VsConstant : public VsExpr {
  public:
    VsConstant(const char *string); // String is copied in
    VsConstant(double number);
    
	VsConstant(const VsValue &value);

    //! Evaluation just returns constant value:
    virtual void evaluate(VsValue &ret, VsFunctionCall *);

  protected:
    virtual ~VsConstant();
  private:
    VsValue myValue;
};

//
// Abstract base class:
//
class VsVar : public VsExpr {
  public:
    // Assignment operator.
    virtual VsValue		assign(const VsValue &, VsFunctionCall *) = 0;
    
    // Assign to a component of an array.
    virtual VsValue		assign(const VsValue &, int32_t index, VsFunctionCall *);

	// array support
	//! evaluate element 
	virtual void evaluate(VsValue &ret,int32_t index, VsFunctionCall *);

	//! evaluate element L-value 
	// may return temporary object !! ref() unref()
	virtual VsVar*		evaluateVar(int32_t index, VsFunctionCall * /*, VsValue &value*/);



    // Note: derived classes will also need to define an execute()
    // function to return the var's value.

	// Object support 
	// assign to an member 
	virtual VsValue assignMember(const VsMember &member, const VsValue &v, VsFunctionCall *);

	// evaluate an member 
	virtual VsValue evaluateMember(const VsMember &member, VsFunctionCall *);

	// invoke member function 
	virtual VsValue callMember(const VsMember &member,VsExprList *args, VsFunctionCall *);

    // returns the corresponding variable object, usually this 
	virtual VsVar *evaluateVar(VsFunctionCall *, VsValue &value);

	// get Var representing the objects member 
	// may return temporary object 
	virtual VsVar* evaluateMemberVar(const VsMember &member, VsFunctionCall *fcall);



};

//
// Assignment expression:
//
class VsAssign: public VsExpr {
  public:

    // Pass variable = expression.  Destructor will delete expression,
    // the enclosing scope must delete the variable (since it may
    // appear more than once).
    VsAssign(VsVar *, VsExpr *);
    
    // Evaluate.  Assigns to variable, result is value of variable.
    virtual void evaluate(VsValue &ret, VsFunctionCall *);

  protected:
    virtual ~VsAssign();
  private:
    VsVar *var;
    VsExpr *expr;
};

//
// Handles arrays on the right hand side of an expression:
//
class VsArrayVar : public VsVar {
  public:

    VsArrayVar(VsExpr *array, VsExpr *subscript);

    //! Evaluation
    virtual void	evaluate(VsValue &ret, VsFunctionCall *);
    
    //! Assignment operator.
    virtual VsValue		assign(const VsValue &, VsFunctionCall *);

	//! x[y][z] // evalute var for array element 
	VsVar *evaluateVar(VsFunctionCall *fcall, VsValue &value);



  protected:
    virtual ~VsArrayVar();
    
  private:
    VsExpr  *array;	    
    VsExpr  *subscript;
};

//
// Binary arithmetic operators.  One class is used for all of them to
// cut down on code bloat.
//
class VsArith: public VsExpr {
  public:
    VsArith(VsExpr *e1, VsExpr *e2, int op);

    virtual void evaluate(VsValue &ret, VsFunctionCall *);

  protected:
    virtual ~VsArith();
  private:
    VsExpr *expr1, *expr2;
    int operation;
};

//
// Increment/decrement operators.  Separate class to allow
// for correct prefix/postfix behavior
//
class VsIncDec: public VsExpr {
  public:
    VsIncDec(VsExpr *var, int inc, int pref);

    virtual void evaluate(VsValue &ret, VsFunctionCall *);

  protected:
    virtual ~VsIncDec();
  private:
    VsExpr *var;
    int	  increment;
    int	  prefix;
};

//
// Comma operator.  Separate class to allow for expression of
// mixed type
//
class VsComma: public VsExpr {
  public:
    VsComma(VsExpr *e1, VsExpr *e2);

    virtual void evaluate(VsValue &ret, VsFunctionCall *);

  protected:
    virtual ~VsComma();
  private:
    VsExpr *expr1, *expr2;
};

//
// Conditional expression operator.
//
class VsCondExpr: public VsExpr {
  public:
    VsCondExpr(VsExpr *test, VsExpr *e1, VsExpr *e2);

    virtual void evaluate(VsValue &ret, VsFunctionCall *);

  protected:
    virtual ~VsCondExpr();
  private:
    VsExpr *test,  *expr1, *expr2;
};

//
// Unary minus operator
//
class VsUArithOp: public VsExpr {
  public:
    VsUArithOp(VsExpr *e, int op);

    virtual void evaluate(VsValue &ret, VsFunctionCall *);

  protected:
    virtual ~VsUArithOp();
  private:
    VsExpr *expr;
    
    int	operation;
};

//
// Binary logic operators.  One class is used for all of them to
// cut down on code bloat.
//
class VsLogicOp: public VsExpr {
  public:
    VsLogicOp(VsExpr *e1, VsExpr *e2, int op);

    virtual void evaluate(VsValue &ret, VsFunctionCall *);

  protected:
    virtual ~VsLogicOp();
  private:
    VsExpr *expr1, *expr2;
    int operation;
};

//
// Unary logic operators.  One class is used for all of them to
// cut down on code bloat.
//
class VsULogicOp: public VsExpr {
  public:
    VsULogicOp(VsExpr *e, int op);

    virtual void evaluate(VsValue &ret, VsFunctionCall *);

  protected:
    virtual ~VsULogicOp();
  private:
    VsExpr *expr;
    int operation;
};

//
// Little wrapper class for VbPList that bundles up some annoying casting:
//
class VsExprList : public VbPList {
  public:
    VsExprList()                                 {}
    VsExprList(int initSize) : VbPList(initSize) {}

    void    append(VsExpr *expr)
        	{ this->VbPList::append((void *)expr); }
    int     find(VsExpr *expr)
	        { return this->VbPList::find((void *)expr); }
    void    insert(VsExpr *expr, int addBefore)
	        { this->VbPList::insert((void *)expr, addBefore); }
    VsExpr* &operator [](int i) const
	        { return (VsExpr * &)(this->VbPList::operator[](i)); }
};

#endif /* _VS_EXPR_ */
