/*
 * VsFunction.h
 *
 *      Declaration of classes:
 *        VsFunctionDef
 *        VsFunctionCall
 *	  VsArgOrLocal
 *
 * Function support
 * 
 * Copyright (C) 1996, Silicon Graphics,  Inc.
 */
#ifndef _VS_FUNCTION_
#define _VS_FUNCTION_

#include "vsexpr.h"

#include <gvstring.h>
#include <gvfield.h>

#include "vsfields.h" // fieldClass


class VsParseTree;
class GvScript;

// browser
class GView;

//
// Functions are split into two classes:  function definitions, which
// can appear only at the top-level of a file, and function calls,
// which are expressions and can appear wherever an expression
// appears.
//
// Function definition class.
// 
class VsFunctionDef {
  public:
    // FunctionDef constructor.  Called during parsing; gotta keep
    // track of current function so that we know if identifiers found
    // inside the function are function arguments, local variables, or
    // global variables.  The VsParseTree argument is needed because
    // global variables are kept by the ParseTree class.
    VsFunctionDef(const char *name, VsParseTree *parent);

    // Again, used during parsing, adds arguments.  It is assumed that
    // this is called BEFORE findIdentifier() is called.
    static void		addArg(const char *argName);

    // Returns the local variable, argument, or global variable with
    // the given name.  A new local variable is automatically created
    // and returned if the name isn't already known in this function.
    static VsVar *findIdentifier(const char *name,gbool createIfNotExists=gfalse);
	
	// create a new local var for the var statement 
	static VsVar * makeLocalVar(const char *name);


    // And when function is done, this cleans everything up and
    // remembers what statements are part of this function definition:
    void		endDefinition(VsStatementList *stmnts);

    // Returns number of local vars and args.  Determines how many
    // VsValues need to be pushed onto stack at a function call:
    inline int			getNumLocals() { return numArgs+numLocals; }

    // Returns number of arguments.  Event in functions need to
    // have two arguments, so we need to test for this.
    inline int			getNumArgs() { return numArgs; }

    // Returns name of this function definition
    const char *	getName() { return name; }
    
    // Statements that make up this function.  Used by VsFunctionCall.
    VsStatementList *statements;

    virtual ~VsFunctionDef();

  // private:
  public:
    // When parsing, function being defined:
    static VsFunctionDef *curFunction;

    char *name;			// This function's name
    int numArgs;		// Number of arguments
    int numLocals;		// Number of local variables
    
	VbPList argOrLocalNames;	// Names of arguments and local vars
    VsParseTree *parent;	// Script we're defined in
	
	// get the script node context
	GvScript *getScriptNode(); 


};


class VsFunctionCall : public VsExpr {
  public:
    
    VsFunctionCall(VsFunctionDef *, VsExprList &exprs);

	// get a string describing the location/name of the function
	const char * getContextString();


    //! Evaluate method.
    virtual void evaluate(VsValue &ret, VsFunctionCall *parent);

    // The enclosing VsFunctionCall is passed to all expressions and
    // statements so that a return statement can turn around and call
    // this:
    void		setReturnValue(const VsValue &val);

    // Argument getting/setting routines, used by VsArgOrLocal class.

    //VsValue		getArg(int whichArg);

	inline void getArg(VsValue &ret, int whichArg)
	{
		if (whichArg >= function->getNumLocals()) {
			// Reference to un-passed param, return NULL value
			ret = VsValue::nothing;
		} else 
			ret = locals[whichArg];
	}

    void		setArgOrLocal(int whichArg, const VsValue &newVal);
    
    static int		isAnUnimplementedFunction(const char* fName);

  //protected:
  public:
    virtual ~VsFunctionCall();

//  private:

    VsFunctionDef *function;	// Function definition contains statements
    VsExprList args;		// Argument expressions
    VsValue *locals;		// Arguments and local variables
    VsValue returnValue;
}; 


class VsFunctionCallByName : public VsFunctionCall {
  public:
    
    VsFunctionCallByName(const char *name, VsExprList &exprs);

	//! get a string describing the location/name of the function
	const char * getContextString();

	//! lookup function name in list of defined functions
	gbool resolve(VsParseTree *parent); 

    //! Evaluate method.
    virtual void evaluate(VsValue &ret,VsFunctionCall *parent);


    

  protected:
    virtual ~VsFunctionCallByName();

    char *name;			// This function's name
	
	VsVar *browser;		// pointer to browser var for print function

  private:
}; 


class VsNewCall : public VsExpr {
  public:
    
    VsNewCall(const char *className, VsExprList &exprs);

    // Evaluate method.
    virtual void evaluate(VsValue &ret, VsFunctionCall *parent);


  protected:
    virtual ~VsNewCall();
  private:
    VsExprList args;		// Argument expressions
	
	VsFieldClass fieldClass;
}; 



class VsMethodCall : public VsExpr {
    public:
	enum Function {
	    // Math library functions
	    fABS, 
	    fACOS, 
	    fASIN, 
	    fATAN, 
	    fCEIL, 
	    fCOS, 
	    fEXP, 
	    fFLOOR, 
	    fLOG, 
	    fMAX, 
	    fMIN, 
	    fPOW, 
	    fRANDOM, 
	    fROUND, 
	    fSIN, 
	    fSQRT, 
	    fTAN, 
// hg 
		fATAN2,
		fGAUSSIAN,
	    fCOSH, 
	    fSINH, 
	    fTANH, 
		fNOISE, 
		fNOISE3, 
		fDNOISE, 

	    fUNKNOWN, 
	};

	VsMethodCall(VsExpr* object,  const char* funcName,  VsExprList &exprs);
	   
    //! Evaluate method.
    virtual void evaluate(VsValue &ret, VsFunctionCall *);
	
	// evalute browser method call
	//VsValue browserMethodEvaluate(GView *browser,VsFunctionCall *parent);


    protected:
	virtual ~VsMethodCall();
	
    private:
	VsExpr*   obj;
	Function func;
	VsExprList args;

	GvName memberName; // name of member function 
	
	//VsValue returnValue;
};

class VsMemberAccess : public VsVar {
    public:
	enum Member {
		// field members 
		mX,
		mY,
		mZ,
	    // Math library members
	    mE, 
	    mLN10, 
	    mLN2, 
	    mPI, 
	    mSQRT1_2, 
	    mSQRT2, 
	    mUNKNOWN, 
	};

	VsMemberAccess(VsExpr* obj,  const char* memberName);

	//! create a expr for certain member accesses 
	static VsExpr * NewMemberAccess(VsExpr* obj, const char* memName);
	   
    //! Evaluate method.
    virtual void evaluate(VsValue &ret, VsFunctionCall *);
	
	// (appearance.material).diffuseColor 
	virtual VsVar *evaluateVar(VsFunctionCall *, VsValue &value);

    virtual VsValue		assign(const VsValue &, VsFunctionCall *);

    protected:
	virtual ~VsMemberAccess();
	
    private:

	VsExpr* obj;
	Member member;
	GvName memberName;

	
	// VsValue returnValue; ??
};


class VsNodeMemberAccess : public VsVar {
    public:

	VsNodeMemberAccess(VsExpr* obj,  const char* memberName);

    // Evaluate method.
    virtual void evaluate(VsValue &ret, VsFunctionCall *);
    virtual VsValue		assign(const VsValue &, VsFunctionCall *);

    protected:
	virtual ~VsNodeMemberAccess();
	
    private:

	VsExpr* obj;	// should evaluate to node 			
	GvName member;
	
	VsValue returnValue;
};

//
// Function argument or local variable (they're both implemented in
// exactly the same way-- as an integer that refers to a VsValue
// created at a VsFunctionCall).
//
class VsArgOrLocal : public VsVar {
  public:
    VsArgOrLocal(int whichArg);

    virtual void evaluate(VsValue &ret, VsFunctionCall *);
    virtual VsValue		assign(const VsValue &, VsFunctionCall *);
	
	// returns field if value is field (compare to VsExpr implementation )
	virtual VsVar *     evaluateVar(VsFunctionCall *fcall, VsValue &value);


  protected:
    virtual ~VsArgOrLocal();
  private:
    int whichArg;
};

#endif /* _VS_FUNCTION_ */
