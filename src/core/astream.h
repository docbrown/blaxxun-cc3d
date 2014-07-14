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
#ifndef _astream_h
#define _astream_h

/******************************************************************************
@doc
@module AStream.h - ascii input/output stream with some parsing support|

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:
ascii input/output stream with some parsing support

Classes:
AStream

******************************************************************************/


#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
//#include <iostream.h>


// grrrrrrrrr some stdio define them as macros !

#ifdef getc
#undef getc
#endif

#ifdef putc
#undef putc
#endif



// formatting indicator
//
// inventor 
#define IV_FMT 1
#define LISP_FORMAT 2


#define A_INPUT 1
#define A_OUTPUT 2

//@class AScii input/output stream with formating and tab
class AStream {
   public :

	FILE *f;		/* the FILE */
	long fsize;		/* for doing % status */

	int  stat;		/* fscanf / fprintf status / EOF */
	int  eof;		/* eof indicator */
	int  fmt;		/* application defined format identification */

	int  mode;		/* INPUT/OUTPUT mode */
	char name[_MAX_PATH];	/* file name */
	int  line;		/* current line number */

	int minimizeSize; // not tabs etc 


	int tablevel;	// current indent level
	int tabsize;	// tabsize
	int noTab;      // don' use tabs
	int fprec;		// float prec 

	char comment_char;	/* char for end of line comment */
	char string_char;	/* char for enclosing strings */

	char block_open;	/* chars for enclosing a block */
	char block_close;

	char list_open;	/* chars for enclosing a list */
	char list_close;
	char list_sep;  /* separator for listelements */


	char curtok[256];		// lookahead token for BeginTok Tok EndTok

	/* Constructor & destructor */

	AStream(const char *fname,const char *fmode = "r");

	virtual ~AStream();
	void Close();
	void Flush();

	gbool isInput() { return(mode == A_INPUT); }
	gbool isOutput() { return(mode == A_OUTPUT); }
	gbool atEof() { return ((stat == EOF) || (f == NULL)); }

	gbool IsInput() { return(mode == A_INPUT); }

	gbool IsOutput() { return(mode == A_OUTPUT); }
	gbool AtEof() { return ((stat == EOF) || (f == NULL)); }

	/* skip spaces, new lines, eof line comments, return next char */
	int Skip();
	int getc() { return(fgetc(f)); }
	int putc( int c) { return fputc(c,f); }

	// check if newline or tab must be written 
	void CheckSep();  


	/* simple read functions */

	int r(int &v) {Skip(); return (stat=fscanf(f,"%d",&v)); }
	int w(int v)  {needSep=1; return (stat=fprintf(f," %d",v)); }
	int w(int v,const char *format)	{needSep=1; return (stat=fprintf(f,format,v)); }

	int r(short &v) {Skip(); return (stat=fscanf(f,"%hd",&v)); }
	int w(short v)  {needSep=1; return (stat=fprintf(f," %hd",v)); }


	int r(long &v) {Skip(); return (stat=fscanf(f,"%ld",&v)); }
	int w(long v)  { CheckSep(); needSep=1; return (stat=fprintf(f,"%ld",v)); }

	int r(float &v) {Skip(); return (stat=fscanf(f,"%f",&v)); }
	int w(float v)	{CheckSep(); needSep=1; return (stat=fprintf(f,"%g",v)); }
	int w(float v,const char *format)	{ CheckSep(); needSep=1; return (stat=fprintf(f,format,v)); }

	int r(double &v) {Skip(); return (stat=fscanf(f,"%lf",&v)); }
	int w(double v)	{CheckSep(); needSep=1; return (stat=fprintf(f,"%lg",v)); }

	// process a scanf/printf image
	int rim(const char *v) {Skip(); return (stat=fscanf(f,v)); }
	int wim(const char *v)	{needSep=1; return (stat=fprintf(f,v)); }

	int r(char *v) {Skip(); return (stat=fscanf(f,"%s",v)); }
	int w(const char *v)  {
							CheckSep();
						    needSep=1; return (stat=fprintf(f,"%s",v)); 
						   }
	// write a quoted string 
	int wstr(const char *v);


	int wnl()  {return (stat=fprintf(f,"\n")); }
	int rnl()  {return (stat=fscanf(f,"\n")); }

	int gets(char *s,int n=256)  {
	    stat= (fgets(s,n,f)== NULL) ? EOF : 0;
	    return (stat); }

	int puts(char *s)  {
	    stat= (fputs(s,f)== 0) ? 0 : EOF;
	    return (stat); }

	/* function with automatic input / ouput control */
	/* write / match separator character */

	int Sep(char c);

	int needSep;	// need separtor before writing string, value ...
	int needNl;	// need newline  before writing string, value ...

	int BeginBlock() { return Sep(block_open); }
	int EndBlock() { return Sep(block_close); }
	int BeginList() { return Sep(list_open); }
	int EndList() { return Sep(list_close); }
	int ListSep() { return Sep(list_sep); }

	int BeginBlockTab(){ int s=Sep(block_open); TabIn(); return(s); }
	int EndBlockTab()  { TabOut(); int s=Sep(block_close);  return(s);  }
	int BeginListTab() { int s=Sep(list_open); TabIn(); return(s);  }
	int EndListTab()   { TabOut(); int s=Sep(list_close);  return(s);  }


	void SetBlockSep(char open = '{', char close = '}') {
  		block_open = open; block_close = close;
	}
	
	void SetListSep(char open = '{', char close = '}',char sep = ' ') {
  		list_open = open; list_close = close; list_sep = sep;
	}
	
	void SetComment(char comment = ';') { comment_char = comment; }

	int io (int &v);	/* io int */
	int io (short &v);	/* io short */
	int io (long &v);	/* io long */
	int io (float &v);	/* io float */
	int io (double &v);	/* io double */
	int io (char *v);	/* io token*/
	
	int iotok (char *v);	/* io token, v is lowercase on input */
	int iostr(char *v);		/* io string quoted with quote characters */

	int io (const char *v);	/* must match io token*/

	int curtokMatched;

	int BeginTok();		// init token
		int tok(const char *tok);
		int io (const char *tok,int &v);	/* io int */
		int io (const char *tok,float &v);	/* io float */
		int io (const char *tok, char *v);	/* io token*/
		int iotok (const char *tok,char *v);	/* io token, v is lowercase on input */
		int iostr(const char *tok,char *v);		/* io string quoted with quote characters */
	int EndTok();

	/*  usage
	  if (BeginTok()) {
		do {
		    iotok("x",x);
		    iotok("Option",option);
		    if (tok("x")) io(x);
		while (!EndTok());
	 */
	/* output comment */
	int comment(const char *com);

	/* print error */
	//int error(const char *com);
	int G__cdecl Error(const char *formatString, ...);


	/* output newline , indent as needed */
	int nl();

	// mark that newline is needed 
	void SetNl() { needNl = 1; }

	int Tab(int col);  // tab with spaces
	int TabTab(int col); // tab with tabs
	void SetTabSize(int TabSize) {tabsize=TabSize; }

	void TabIn() { tablevel ++; }
	void TabOut() { tablevel --; }


};
#endif

