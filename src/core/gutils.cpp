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
@doc

@module GUtils.cpp - GLView utility routines implementation|

Copyright (c) 1995-1998	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

$Revision: 1.9 $
$Log: gutils.cpp,v $
Revision 1.9  1999/07/06 16:55:12  tom
*** empty log message ***



Todo :

******************************************************************************/

#include "stdafx.h"

#include <stdlib.h>
#include <string.h>

#include <math.h>


#include "gutils.h"



// floating point control
// init floating point control 
void GFPInit()
{
#ifdef _M_IX86
	TRACE( "Control fp is : 0x%.4x default is 0x%.4x \n", _control87( 0, 0 ) ,_CW_DEFAULT );
   
   //all to default 	
   // _control87(_CW_DEFAULT ,0xfffff );
#endif

   GFPSetControl();

   // 53 bits	
   //_controlfp(_PC_53, _MCW_PC);
   // 24 bits
   //_control87(_PC_24, _MCW_PC);

	// Test :
   // _control87(0, _MCW_EM); // enable all exceptions
   // denable divide by 0
	// _control87(_EM_INVALID + _EM_OVERFLOW + _EM_UNDERFLOW + _EM_INEXACT + _EM_DENORMAL, _MCW_EM); // ); 
   // TRACE(" Test test 1/0 == ", ((float)x) / yz);


}

// terminitate floating point control
void GFPTerm()
{

}


// reset floating point control word, might be changed by other components
void GFPSetControl()
{

#ifdef _M_IX86

#if GFLOAT_IS_FLOAT
	// 24 bits of prec
   // _control87(_PC_24, _MCW_PC); this does *not* work we need double for timing & scripting :-(
#endif

	// disable all exceptions
//	_control87(_EM_INVALID + _EM_ZERODIVIDE + _EM_OVERFLOW + _EM_UNDERFLOW + _EM_INEXACT + _EM_DENORMAL, _MCW_EM);
// Debug : get /0 
	//_control87(_EM_INVALID + _EM_OVERFLOW + _EM_UNDERFLOW + _EM_INEXACT + _EM_DENORMAL, _MCW_EM);

#endif

}





// @func Copy a list of integers
void copylist(
	int *dest,		//@parm destination pointer
	const int *src,	//@parm source pointer 
	int cnt			//@parm number of elements to copy
	) 
{ memcpy(dest,src,cnt*sizeof(int)); }

// append appendlist to list,
// array will be reallocated accordingnly
//	list 	pointer to array
//  nlist	current elements
//	listmax	allocated size
//   elements to append

int append(int * &list,int & nlist, int & listmax, const int *appendlist,int nappendlist)
{
	    if (((nlist + nappendlist)> listmax) || (list==NULL)) {
		int n=max((listmax*3)/2,listmax+nappendlist);
		if (list==NULL) { n++;
		    list=(int *) CALLOC(n,sizeof(int));
		} else list= (int *) REALLOC(list,n*sizeof(int));
		if (list==NULL) return (-1);
		   listmax=n;
	    }
	    if (appendlist != NULL) {
	       memcpy(&((list)[nlist]),appendlist,nappendlist*sizeof(int));
	       nlist+=nappendlist;
	    }
	    return(0);
}



// string utils


// check if string prefix occurs at the beggin of str
// return 1 if prefix, 0 if not 
int strprefix(const char *str, const char * prefix)
{
	 int prefixLen = strlen( prefix );
 	 if (strncmp( str, prefix,prefixLen) == 0) return(1); 
	 return(0);
}

int striprefix(const char *str, const char * prefix)
{
	 int prefixLen = strlen( prefix );
 	 if (strnicmp( str, prefix,prefixLen) == 0) return(1); 
	 return(0);
}

// return true if c is the last char of s
int strsuffix(const char *s,char c)
{
	int l=strlen(s);
	if (l==0) return(0);
	return(s[l-1] == c);
}


// return entry in s, where suffix is at end
// null if s has not suffix

const char* strsuffix(const char *s,const char *suffix)
{
	int l=strlen(s);
	int ls = strlen(suffix);
	if (ls>l) return 0;
	if (l==0) return(0);
	if (strcmp(s+l-ls,suffix) == 0)
		return s+l-ls;
	else return NULL;
}

const char* strisuffix(const char *s,const char *suffix)
{
	int l=strlen(s);
	int ls = strlen(suffix);
	if (ls>l) return 0;
	if (l==0) return(0);
	if (stricmp(s+l-ls,suffix) == 0)
		return s+l-ls;
	else return NULL;
}


// get the first index of search in string, -1 if not found
int indexOf(const char *string,const char *search,int fromIndex)
{
	int index = -1;
	if ((fromIndex>=0)  && (fromIndex<strlen(string))) {
		const char *p= string;
		const char *pp= strstr(p + fromIndex,search);
		if (pp) index = pp-p;
	}
	return index;
}

// get the last index of search in string, -1 if not found
int lastIndexOf(const char *string,const char *search,int fromIndex)
{
	int index = -1;

	int searchLen = strlen(search);
	if (fromIndex <0) fromIndex = strlen(string);

	const char *p= string;
	const char *pp= &p[fromIndex];

	while (pp>=p) {

		if (strncmp(pp,search,searchLen) == 0) {
			return pp-p;
		}
		pp--;
	}
	
	return (-1);


}



// set out to the subsing of s[index1:index2]
// out must be large enought, empty string if bad data
char * setSubstring(const char *s, int index1, int index2,char *out)
{
	int index = -1;
	int l = strlen(s);

	if (index1 > index2) { // swap
		int t = index1;
		index1 = index2;
		index2 = t;
	}


	if (index1>=0  && index1<l) {
		
		index2 = min(l,index2);

		int l= index2-index1; // not including
		if (l>0) {
		
			char *x=out;

			if (x) {		
				strncpy(x,s+index1,l);
				x[l]=0;
			}
		
		}
		else 
			out[0]=0;
	}
	else {
		out[0]=0;
	}
	return out;

}

// replace max cnt occurences of search in string by replace 
// out must be large enough
char * replace(const char *string,
			   const char *search,const char *replace,
			   int cnt,
			   char *out)
{
	
	return out;

}

char * toLowerCase(char *string)
{
	_strlwr(string);
	return string;
}

char * toUpperCase(char *string) 
{
	_strupr(string);
	return string;
}


// parse a boolean strings
// true TRUE on ON ==> true, else false 
gbool parseBool(const gchar* s)
{

   int ret = 1;
   if (streq(s,"TRUE") || streq(s,"true")|| streq(s,"on") || streq(s,"ON")) {
		return gtrue;
   }
   else
   if (streq(s,"FALSE") || streq(s,"false")) {
		return gfalse;
   }
    
   return(gfalse);

}

// Lookup string for value in table 
// return NULL if value not found 
const char * Lookup(const IS *table,int v)
{ for (int i=0; table[i].s != NULL; i++)
      if (table[i].i == v) return(table[i].s);
  return 0;
}

// Lookup value given string
// Result -1 if not string not found
int Lookup(const IS *table,const char *v)
{ for (int i=0; table[i].s != NULL; i++)
      if (streq(table[i].s,v)) return(table[i].i);
  return -1;
}
/* Example : 
const IS G3TextureParameters::filters[] = {
			{-1,"<Invalid>"},
			{D3DFILTER_NEAREST,"Nearest"},
			{D3DFILTER_LINEAR,"Linear"},
			{D3DFILTER_MIPNEAREST,"Mipmap_Nearest_Nearest"},
			{D3DFILTER_MIPLINEAR,"Mipmap_Linear_Nearest"},
			{D3DFILTER_LINEARMIPNEAREST,"Mipmap_Nearest_Linear"},
			{D3DFILTER_LINEARMIPLINEAR,"Mipmap_Linear_Linear"},
			{0,NULL},
			};

*/



// derived from 
// http://www.whizkidtech.net/i18n/

// libutf8 
/*
 * Copyright (c) 1999 G. Adam Stanislav
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	sget.utf-8.c v.1
 */


#define	INVALID	UTF_INVALID

#define	get(c)	c = *strptr++; \
	if (chars) (*chars)++; \
	if ((c) == 0) return (unsigned int)EOF

/*!
reads a sequence of one or more characters from a UTF-8 formatted string,
which it converts to a single UTC-4 (Unicode) value.
*/
// hg const
unsigned int sgetu8(const unsigned char *strptr, int *chars) 
{
	unsigned int c;
	int i, iterations;
	unsigned char ch;

	if (chars) *chars = 0;

	if (strptr == NULL)
		return (unsigned int)EOF;

	get(c);

	if ((c & 0xFE) == 0xFC) {
		c &= 0x01;
		iterations = 5;
	}
	else if ((c & 0xFC) == 0xF8) {
		c &= 0x03;
		iterations = 4;
	}
	else if ((c & 0xF8) == 0xF0) {
		c &= 0x07;
		iterations = 3;
	}
	else if ((c & 0xF0) == 0xE0) {
		c &= 0x0F;
		iterations = 2;
	}
	else if ((c & 0xE0) == 0xC0) {
		c &= 0x1F;
		iterations = 1;
	}
	else if ((c & 0x80) == 0x80)
		return INVALID;
	else return c;

	for (i = 0; i < iterations; i++) {
		get(ch);
		if ((ch & 0xC0) != 0x80)
			return INVALID;
		c <<= 6;
		c |= ch & 0x3F;
	}

	return c;
}


// holger
//! get next character from utf8 encoded string and increment pointer 
unsigned int utf8nextcinc(const unsigned char * &strptr) 
{
	int chars;
	unsigned int c = sgetu8(strptr,&chars);
	strptr+=chars;
	return c;
}

//! get length of utf8 encoded string 
int utf8strlen(const unsigned char * strptr) 
{
	int chars;
	unsigned int c;
	int len=0;

	while (1) {
		c = sgetu8(strptr,&chars);
		if (c == (unsigned int)EOF)
			return len;
		if (c == (unsigned int)INVALID)
			return len;
		strptr+=chars;
		len ++;
	}
	return c;
}


#define	bits(c)	(0x80 | ((c) & 0x3F))
#define	put(c)	*strptr++ = (c);
#define	putbits(c)	put(bits(c))
#define	finish()	*strptr = '\0'

/*
function of utf-8 library converts a UTC-4 (31-bit Unicode) integer to a sequence 
of one or more characters representing its UTF-8 value, and writes the result to 
a buffer (character string). It appends a NUL at the end of the buffer, so it can be
processed as a standard C string.
*/

char * sputu8(unsigned int c, char *strptr) 
{
	if (strptr != NULL) {
		if (c < 0x80) {
			put(c);
			finish();
		}
		else if (c < 0x800) {
			put(0xC0 | (c >>  6));
			putbits(c);
			finish();
		}
		else if (c < 0x10000) {
			put(0xE0 | (c >> 12));
			putbits(c >>  6);
			putbits(c);
			finish();
		}
		else if (c < 0x200000) {
			put(0xF0 | (c >> 18));
			putbits(c >> 12);
			putbits(c >>  6);
			putbits(c);
			finish();
		}
		else if (c < 0x400000) {
			put(0xF8 | (c >> 24));
			putbits(c >> 18);
			putbits(c >> 12);
			putbits(c >>  6);
			putbits(c);
			finish();
		}
		else if (c < 0x80000000) {
			put(0xFC | (c >> 30));
			putbits(c >> 24);
			putbits(c >> 18);
			putbits(c >> 12);
			putbits(c >>  6);
			putbits(c);
			finish();
		}
		else {	/* Not a valid Unicode "character" */
			finish();
		}
	}

	return strptr;
}


