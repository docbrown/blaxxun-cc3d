/*
 * VsParse.h
 *
 *      Declaration of classes:
 *        VsParseTree
 *
 * Parser and interpreter for a language that looks a lot like
 *   JavaScript(tm)
 * 
 * Copyright (C) 1996, Silicon Graphics,  Inc.
 *
 *   (JavaScript is a registered trademark of NetScape...)
 */

#ifndef _VS_PARSE_
#define _VS_PARSE_

#ifdef __sgi
#include <Inventor/SbPList.h>
#else
#include "vbplist.h"
#endif

class VsFunctionDef;
class VsVar;
class VsFunctionCallByName;

class GvScript;
class GReporter;



#ifdef __sgi
typedef void VsErrorCB(void *userData, int lineNumber, const char *errorMsg);
#endif

class VsParseTree {
  public:

	// Add a global variable.  Called by Script node to register
    // eventOuts/fields as global variables.
    void			addGlobalVar(const char *name, VsVar *var);
    VsVar *			getGlobalVar(const char *name);

    // Parse the given input buffer.  Returns 0 if everything was read
    // OK, 1 if there was an error.
    int				parse(const char *buffer,  const char* scriptName);

    // Called during parse() to add a top-level function to our list:
    static VsFunctionDef *	beginFunction(const char *name);

    // Returns a reference to a top-level function definition
    VsFunctionDef *		findFunction(const char *name);

	// add a function call to be unresolved,
	// try to resolve after parsing
	void addUnresolvedFunctionCall(VsFunctionCallByName *fcall);

	// resolve open function calls
	gbool resolveFunctionCalls();



    // Returns 'current' parse tree.  Used only during parsing.
    static VsParseTree *	getCurrent() { return current; }
    
#ifdef __sgi
    // Set a callback to be called when syntax errors are encoutered during parsing
    void			setErrorCallback(VsErrorCB *cb, void *ud)
                                        { errorCB = cb; cbUserData = ud; }
#endif

    int  getNumErrors()	   { return numParseErrors; }

    VsParseTree(GvScript *script);
    ~VsParseTree();

    // Used by yylex/yyparse:
    static void			unput(int c);
    static int			input();
    static void			yyerror(const char *errmsg);

//  private:
  public: 
    //! Parse tree currently being built:
    static VsParseTree *current;

    //! And string being parsed:
    const char *curParseString;

	//! length of curParseString
	int curParseStringLen;

    //! Position in string being parsed:
    int parsePosition;

    //! And which line in string is being parsed:
    int lineNumber;

    //! Global variables for this script:
    VbPList	globalVars;

    // Parse tree consists of a series of functions definitions, each
    // of which in turn contains a series of statements.
    VbPList	functions;

	// function calls not yet resolved
    VbPList	unresolvedFunctionCalls;
    
    int numParseErrors;
    
    const char* scriptName;

	GvScript *scriptNode; // back pointer to script 
	GvScript *getScriptNode() { return scriptNode; }

	GReporter *reporter;
    
#ifdef __sgi
    VsErrorCB *	errorCB;
    void *	cbUserData;
#endif
};

#endif /* _VS_PARSE_ */

