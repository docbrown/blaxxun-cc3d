/*
 * VsParse.c++
 *
 *     Implementation of classes:
 *        VsParseTree
 *
 * The whole enchilada parse tree
 * 
 * Copyright (C) 1996, Silicon Graphics,  Inc.
 */
#include "stdafx.h"

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "vsparse.h"
#include "vsfunction.h"

#include "greporter.h"


extern int yyparse(); /* The YACC-generated parser */

VsParseTree *VsParseTree::current = NULL;

VsParseTree::VsParseTree(GvScript *script) : scriptNode(script), reporter(NULL)
{
    functions.truncate(0);
    current = NULL;

    curParseString = NULL;
	curParseStringLen=0;

    scriptName = NULL;

#ifdef __sgi
    errorCB = NULL;
    cbUserData = NULL;
#endif
}

VsParseTree::~VsParseTree()
{
    int i;
    for (i = 0; i < functions.getLength(); i++) {
		VsFunctionDef *f = (VsFunctionDef *)functions[i];
		delete f;
    }
    for (i = 0; i < globalVars.getLength(); i+=2) {
		VsVar *var = (VsVar *)globalVars[i+1];
		ASSERT(var != NULL /*&& (var->refCount > 0)*/);
		var->unref();
    }
	// statementlist currentlz doesn"t use refCnt"s
    for (i = 0; i < unresolvedFunctionCalls.getLength(); i++) {
		//VsFunctionCallByName *var = (VsFunctionCallByName *)unresolvedFunctionCalls[i];
		//ASSERT(var != NULL /*&& (var->refCount > 0)*/);
		//var->unref();
		unresolvedFunctionCalls[i]= NULL;
    }

    if (scriptName != NULL) {
		free((void *)scriptName);
    }
}

int
VsParseTree::parse(const char *buffer, const char *name)
{
    assert(current == NULL);

	if (!buffer) return -1;

    current = this;
    curParseString = buffer;
	curParseStringLen = strlen(buffer);
    parsePosition = 0;
    lineNumber = 0;
    numParseErrors = 0;
    
    if (name != NULL) {
        scriptName = strdup(name);
    }

    int result = yyparse();

    current = NULL;
    curParseString = NULL;
	curParseStringLen=0;

    return (getNumErrors() == 0);
}

void
VsParseTree::addGlobalVar(const char *name, VsVar *var)
{
	ASSERT(var != NULL);    
	globalVars.append((void *)name);
    globalVars.append((void *)var);
    var->ref();

}

VsVar *
VsParseTree::getGlobalVar(const char *name)
{
    // Look through globals:
    for (int i = 0; i < globalVars.getLength(); i+= 2) {
		const char *n = (const char *)globalVars[i];
		if (strcmp(n, name) == 0) {
	    	VsVar *v = (VsVar *)globalVars[i+1];
	    	return v;
		}
    }
    return NULL;
}

/* static */
VsFunctionDef *
VsParseTree::beginFunction(const char *name)
{
    VsFunctionDef *result = new VsFunctionDef(name, current);

	if (!current) return result; // HG should not happen

    // ??? Warn if two functions with same name ???
    current->functions.append(result);
	
    return result;
}


// add a function call to be unresolved,
// try to resolve after parsing
void
VsParseTree::addUnresolvedFunctionCall(VsFunctionCallByName *fcall)
{
	// fcall->ref();
	// statementlist currently doesn"t use refCnt's

    unresolvedFunctionCalls.append(fcall);
}

// resolve open function calls

gbool
VsParseTree::resolveFunctionCalls()
{
    for (int i = 0; i < unresolvedFunctionCalls.getLength(); i++) {
	   VsFunctionCallByName *f = (VsFunctionCallByName *)unresolvedFunctionCalls[i];
	   f->resolve(this);
	}
	return TRUE;
}


VsFunctionDef *
VsParseTree::findFunction(const char *name)
{
    for (int i = 0; i < functions.getLength(); i++) {
	VsFunctionDef *f = (VsFunctionDef *)functions[i];
	if (strcmp(f->getName(), name) == 0)
	    return f;
    }
    return NULL;
}


void
VsParseTree::yyerror(const char *errmsg)
{
	ASSERT(current !=NULL);
	if (!current) return;


    current->numParseErrors++;

    // Ok, give the user some context about the error:
    // Find beginning and end of current line:
    const char *buf = current->curParseString;
    int lineBegin, lineEnd;
    lineEnd = current->parsePosition;
    lineBegin = lineEnd-1;
    while (lineBegin >= 0 && isprint(buf[lineBegin]))
        --lineBegin;
    ++lineBegin;

    while (isprint(buf[lineEnd])) ++lineEnd;
    --lineEnd;

    int charsInLine = lineEnd-lineBegin + 1;
    int charsInScriptName = 0;
    if (current->scriptName != NULL) {
		charsInScriptName = strlen(current->scriptName);
    }
    char *msg = (char *)malloc(strlen(errmsg)+charsInLine+charsInScriptName+50);

#ifdef __sgi    
    if (current->errorCB == NULL) {
        sprintf(msg, "Script node: %s: line \"", errmsg);
    } else if (current->scriptName != NULL) {
        sprintf(msg, "Script node %s: %s: line %d \"", 
		current->scriptName, 
		errmsg, current->lineNumber+1);	  
    } else {
        sprintf(msg, "Unnamed Script node: %s: line %d \"", errmsg, current->lineNumber+1);
    }
#else 
    sprintf(msg, "Script node %s: %s: line %d \"",current->scriptName, 
		errmsg, current->lineNumber+1);	
#endif
    int msglen = strlen(msg);

    for (int i = 0; i < charsInLine; i++) {
        msg[msglen+i] = buf[lineBegin+i];
    }
    msg[msglen+i] = '"';  // Add close-quote:
   // msg[msglen+i+1] = '\n'; // Add new line
    msg[msglen+i+1] = 0; // And end-of-string

#ifndef __sgi
     //::MessageBox((HWND)NULL, msg, "VrmlScript syntax error", MB_OK );

	if (current->reporter) current->reporter->Error(msg);
	else Reporter.Error(msg);	

#else
    if (current->errorCB != NULL)
        (*(current->errorCB))(current->cbUserData, current->lineNumber+1, msg);
    else
        fprintf(stderr, msg);
#endif

    free(msg);
}
extern "C" {
int yywrap() { return 1; }
};


/* 
	unput

	replace the passed char as the next char in the input stream
*/
void
VsParseTree::unput(int c)
{
    ASSERT(current->parsePosition > 0);
	if (current->parsePosition == 0) return;

    --current->parsePosition;

    if (c == '\n' || c == '\f') --current->lineNumber;

    // Assertion: always put back what we just got:
    ASSERT(current->curParseString[current->parsePosition] == c);
}

/* 
    input
    get chars from buffer, update wLineNo
*/
int
VsParseTree::input()
{
	
	if (current->parsePosition>= current->curParseStringLen) {
		//ASSERT(0);
		return 0;

	}
    char result = current->curParseString[current->parsePosition];
    current->parsePosition++;

    if (result == '\n' || result == '\f') ++current->lineNumber;

    return result;
}
