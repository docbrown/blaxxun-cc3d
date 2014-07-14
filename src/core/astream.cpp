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
/******************************************************************************

AStream.cpp

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:
ascii input/output  stream with some parsing support

Classes:

AStream

Notes:

Changes:

Todo :
Error reporting using dialog for MFC

******************************************************************************/


#include "stdafx.h"
//#include "astream.h"
#include "greporter.h"


AStream::AStream(const char *fname,const char *fmode)
{
  stat = 0;
  eof = 0;
  line = 1;
  fmt = 0;
  
  minimizeSize = 0; // not tabs etc 
  comment_char = ';';
  string_char = '"';

  block_open = '{';
  block_close = '}';

  list_open = '{';
  list_close = '}';
  list_sep = ' ';
  needSep=0;	// need separtor before writing string, value ...
  needNl=0;		// need newline    """" 


  if (fname)  strcpy(name,fname);
  else name[0]=0;

  tablevel = 0;
  tabsize = 4;
  noTab=0;

  if (fmode[0]=='r') mode = A_INPUT;
  else if (fmode[0]=='w') mode = A_OUTPUT;

  if (name[0] == 0) {
     if (mode == A_INPUT)  f = stdin;
     else if (mode == A_OUTPUT)  f = stdout;
  } else {
     f=fopen(fname,fmode);
  }
  if (f == NULL) stat=EOF;
  else {
  	// set buffering to larger bufer 64 k
  	if (setvbuf(f,NULL,_IOFBF,1024*64) !=0) {
	   Error("Can´t set stream buffer size to %d",1024*64);
  	}
  }

  curtok[0]=0;

}

AStream::~AStream()
{
  if (f!=NULL) fclose(f);
  f = NULL;
  stat=EOF;
  mode = 0;
  eof=1;
}

// Close Stream
void AStream::Close()
{
  if (f!=NULL) fclose(f);
  f = NULL;
  stat=EOF;
  eof = 1;
}

// Flush any buffers
void AStream::Flush() {
  if (f!=NULL) fflush(f);
}

/* skip spaces and newlines, and eof line comments */
int AStream::Skip()
{ int c;

  while (1) {
  c= fgetc(f);

  if (c == EOF) {
     stat=EOF;
	 eof = 1;
     return(c);
  }

  if (c == comment_char) {
     do
       c=fgetc(f);
     while ((c != EOF) && (c != '\n'));

  }

  if (c == '\n')
     line++;
  else if (isspace(c)) {
  }
  else {
     ungetc(c,f);
     return(c);
  }
  }
}

// check if newline or tab must be written 
void AStream::CheckSep()  
{
	if (needNl) nl();
	else if (needSep) {
		fputc(' ',f);
		needSep=0;
	}
}



/*
  input / output separator
*/

int AStream::Sep(char c)
{ int curc;

  if (mode == A_INPUT) {
      curc=Skip();

      if (curc==c) {
	     curc=fgetc(f);
	     return(curc);
      }
      Error("%c expected but %c found",c,curc);
      return(EOF);
  }
  if (mode == A_OUTPUT) {
	if (needNl) nl(); 
	needSep=0;
	return fputc(c,f);
  }
  return(-1);
}

// Input- Output of basic types
int AStream::io(int &v)
{
  if (mode == A_INPUT) {
      return(r(v));
  } else
  if (mode == A_OUTPUT) {
      return (w(v));
  }
  return(-1);
}

int AStream::io(short &v)
{
  if (mode == A_INPUT) {
      return(r(v));
  } else
  if (mode == A_OUTPUT) {
      return (w(v));
  }
  return(-1);
}

int AStream::io(long &v)
{
  if (mode == A_INPUT) {
      return(r(v));
  } else
  if (mode == A_OUTPUT) {
      return (w(v));
  }
  return(-1);
}

int AStream::io(float &v)
{
  if (mode == A_INPUT) {
      return(r(v));
  } else
  if (mode == A_OUTPUT) {
      return (w(v));
  }
  return(-1);
}

int AStream::io(double &v)
{
  if (mode == A_INPUT) {
      return(r(v));
  } else
  if (mode == A_OUTPUT) {
      return (w(v));
  }
  return(-1);
}

int AStream::io (char *v)	/* io token*/
{
  if (mode == A_INPUT) {
      return(r(v));
  } else
  if (mode == A_OUTPUT) {
      return (w(v));
  }
  return(-1);
}

int AStream::iotok (char *v)	/* io token, v is lowercase on input */
{ int ret;
	if (mode == A_INPUT) {
		ret=r(v);
		strlwr(v);
		return(ret);
	} else
	if (mode == A_OUTPUT) {
		return (w(v));
	}
	return(-1);
}

int AStream::io (const char *v)	/* must match io token*/
{ int ret;
  if (mode == A_INPUT) {
      char buf[256];
      ret=r(buf);
      // ??? printf("Read only Token read = %s\n",buf);
      return(ret);
  } else
  if (mode == A_OUTPUT) {
      return (w(v));
  }
  return(-1);
}

/* io string quoted */
int AStream::wstr (const char *v)	
{ int ret;
  if (needNl) nl(); 
  else if (needSep) fputc(' ',f);
#if 0
  fputc(string_char,f);
  if (v!= NULL) ret=fprintf(f,"%s",v); else ret=0;
  fputc(string_char,f);
#else
	const char *p=v;
	const char *p1;
	ret = 1;
    fputc(string_char,f);

	do {
		p1 = strchr(p,'\n'); // find newline
		if (!p1) p1 = p+strlen(p);
	  

		while(p<p1) {
  			if (*p==string_char) fputc('\\',f); // ESCAPE character
  			fputc(*p,f);
			p++;

		}
  		
		if (*p == 0) break;
		
		// no tab in multiline string nl();
		fprintf(f,"\n"); line++;

		p++; // skip newline character 

	}  while(1);
	fputc(string_char,f);

#endif

  return(ret);
}


/* io string quoted */
int AStream::iostr (char *v)
{ int ret;
  if (mode == A_INPUT) {
	  char fmt[]="%[^X]";
	  int curc;
	  if (Skip() == string_char) {
		curc=fgetc(f);
		fmt[3]=string_char;
		ret=fscanf(f,fmt,v);
		curc=fgetc(f);
		if (curc != string_char) {
			Error("Closing string quote expected ");
		}
	  	//to do: no closing before nl, appending of strings
	  }	else
	     ret=r(v);
	  return(ret);
  } else
  if (mode == A_OUTPUT) {
	    if (needNl) nl(); 
		else if (needSep) fputc(' ',f);
		
		fputc(string_char,f);

		if (v!= NULL) ret=fprintf(f,"%s",v); else ret=0;
		fputc(string_char,f);
	
  }
  return(ret);
}

/* output comment */
int AStream::comment(const char *com)
{ if (mode == A_OUTPUT) { stat=fprintf(f,"%c%s\n",comment_char,com); line++; } return(stat);}

/* print error */

int G__cdecl AStream::Error(const char *formatString, ...)
{
    char	buf[10000];
    va_list	ap;

    va_start(ap, formatString);
    vsprintf(buf, formatString, ap);
    va_end(ap);
   
   // stat=fprintf(stderr,"%c Error %s\n",comment_char,com);

 	return(Reporter.Error("%s:%d Error %s\n",name,line,buf));
}


/* output newline */
int AStream::nl()
{ if (mode == A_OUTPUT) {
		stat=fprintf(f,"\n"); 
		line++;
		needNl=0; 
		if (!noTab) {
			// Tab(tablevel*tabsize);
			TabTab(tablevel); // tab by tabs
		}
   }
   needSep=0;
   return(stat);
}

int AStream::Tab(int col)
{ int res;
   if (mode == A_OUTPUT) {
	   if (!minimizeSize) {
			while (col >0)	{ res = fputc(' ',f); col--; }
			if (res == EOF) stat = EOF;
			else stat = 0;
			needSep = 0;
	   }
   }
   return(stat);
}

int AStream::TabTab(int col)
{ int res;
   if (mode == A_OUTPUT) {
	   if (!minimizeSize) {
			while (col >0)	{ res = fputc('\t',f); col--; }
			if (res == EOF) stat = EOF;
			else stat = 0;
			needSep=0;
		}
   }
   return(stat);
}



int AStream::BeginTok()
{
  if (isInput()) {
      curtokMatched = 0;
      iotok(curtok);
      return (stat != EOF);
  }
  else  {
    TabIn();
    return(1);
  }
 }

int AStream::EndTok()
{
  if (isInput()) {
      if (!curtokMatched) {
	   	Error("Unhandled token %s",curtok);
	   return(0);	// don't continue Loop
      }

      int next = Skip();
      // parse delimters if needed

      if (!isalnum(next)) {
		return(0); // no keyword following
      }

      // get new current token and repeat loop

      iotok(curtok);

      curtokMatched = 0;
      return (stat != EOF);
  }
  else {
   	TabOut();
	return(1);
  }
}


// Match token on input
// write it on output
int AStream::tok(const char *token)
{
  if (isInput()) {
	  if (curtokMatched) {
//		BeginTok();		// sequential mode, read next token
	  }
      if (strcmp(token,curtok) == 0) {	// Return 1 if tok matches curren token
	  	curtokMatched=1;
	  	return(1);
      }
      return(0);
  }
  else {
    
	// HG 27.12.95 nl();  // should be option

    //iotok(token); // write token out
	w(token);
	fputc(' ',f);
	needSep = 0;

    return(1);
  }
}

int AStream::io (const char *token,int &v)
{
   if (tok(token)) { io(v); nl(); return(1); }
   return(0);
}

int AStream::io (const char *token,float &v)	/* io float */
{
   if (tok(token)) { io(v); nl(); return(1); }
   return(0);
}

int AStream::io (const char *token, char *v)	/* io token*/
{
   if (tok(token)) { io(v); nl(); return(1); }
   return(0);
}
int AStream::iotok (const char *token,char *v)	/* io token, v is lowercase on input */
{
   if (tok(token)) { iotok(v); nl(); return(1); }
   return(0);
}

int AStream::iostr(const char *token,char *v)		/* io string quoted with quote characters */
{
   if (tok(token)) { iostr(v); nl(); return(1); }
   return(0);
}

