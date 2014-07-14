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
#include "stdafx.h"
/******************************************************************************
@doc

@module GvInput.cpp - Implementation of VRML ascii parser|

Copyright (c) 1995 - 1998	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvInput>


Notes:


Changes:

$Revision: 1.30 $
$Log: gvinput.cpp,v $
Revision 1.30  1999/07/06 16:45:47  tom
*** empty log message ***

Revision 1.29  1999/05/10 13:54:55  tom
*** empty log message ***



Todo :

******************************************************************************/

#include <ctype.h>
//#include <stdlib.h>
//#include <stdio.h>
#include <gvinput.h>
#include <gvdebugerror.h>
#include <gvreaderror.h>
#include <gvnode.h>

#include <gvnamespace.h>
#include <gvscene.h>

#include "gzreadstream.h"


#define CURVERSION 1.0 			// Current version of file format
#define COMMENT_CHAR '#'

struct headerStorage 
{
    const char *string;
    float version;
};

static const int numValidASCIIHeaders = 7;
static const headerStorage ASCIIHeader[7] = {
    { "#VRML V1.0 ascii",   1.0 },
    { "#VRML V1.1 utf8",	1.1 },
	{ "#Inventor V2.0 ascii", -2.0 }, 
	{ "#Inventor V2.1 ascii", -2.1 }, 
	{ "#Inventor V2.2 ascii", -2.2 }, 
    { "#VRML Draft #2 V2.0 utf8",2.0},
    { "#VRML V2.0 utf8",2.0}
};


// 2.07.96 check only for prefix, after header
// additional chars are allowed 
float
GvInput::isASCIIHeader(const char *string)
{
    for (int i = 0; i < numValidASCIIHeaders; i++) {
//	if (strcmp(ASCIIHeader[i].string, string) == 0)
	if (strncmp(ASCIIHeader[i].string, string,strlen(ASCIIHeader[i].string)) == 0)
	    return ASCIIHeader[i].version;
    }
    return FALSE;
}

GvInput::GvInput() : scene(NULL),reporter(NULL)
{
//  setFilePointer(stdin);
    setFilePointer(NULL);
	setSrc(NULL);
	reportErrors = TRUE;
    vrml2 = FALSE;
  
    // ev. nameSpace is beeing passed as argument
    nameSpace = new GvNameSpace();
    currentProto = NULL;
	lastErrorLineNum = -1;
	errorCode = 0;

}

GvInput::~GvInput()
{
    if (nameSpace)
        delete nameSpace;

}

void
GvInput::setFilePointer(GZReadStream *newFP)
{
    fp = newFP;
    lineNum = 1;
	lastErrorLineNum = -1;
	errorCode = 0;
    version = CURVERSION;
    readHeader = FALSE;
    headerOk = TRUE;
    backBufIndex = -1;		
}

void
GvInput::setString(const char *string,GvBool needHeader)
{
    fp = NULL;
    lineNum = 1;
	errorCode = 0;
    version = CURVERSION;
    readHeader = !needHeader;
    headerOk = TRUE;
    backBufIndex = -1;		
    putBack(string);
}


float
GvInput::getVersion()
{
    if (! readHeader)
		checkHeader();

    return version;
}


// get next char from backBuf or File, return FALSE if EOF 
GvBool
GvInput::get(char &c)
{
    GvBool ret;
    if (backBufIndex >= 0) {
		c = backBuf.getString()[backBufIndex++];

		if (c != '\0')
	    	return TRUE;

		backBuf.makeEmpty();
		backBufIndex = -1;
    }

    if (! readHeader && ! checkHeader())
		return FALSE;

    if (eof()) {
		c = (char)EOF;
		ret = FALSE;
    }

    else {
		// int i = getc(fp);
		int i = fp->getc();

		if (i == EOF) {
	    	c = (char)EOF;  // its -1 !!!
	    	ret = FALSE;
		}
		else {
			c = (char) i;
			
			if (i == '\r') { // CR == 10 
				int j = fp->getc(); // get next 
				if (j == '\n') { // CR LF sequence, report only LF 
					c = (char) j;
				} else { // unget this char 
					// bad its a CR delimited MAC File 
					if (fp->ungetc(j) == EOF) {
						backBuf = c;
						backBufIndex = 0;
					}
				}
			}
			ret = TRUE;
		}
    }

    return ret;
}

GvBool
GvInput::read(char &c)
{
    return (skipWhiteSpace() && get(c));
}

GvBool
GvInput::read(GvString &s)
{
    if (!skipWhiteSpace())
		return FALSE;

    GvBool      quoted;
    char        c;
    char        bufStore[256*4];
    char        *buf;
    int         bytesLeft;

    s.makeEmpty();

    if (! get(c))
		return FALSE;

    quoted = (c == '\"');
    
	if (! quoted)
		putBack(c);

    do {

	buf       = bufStore;
	bytesLeft = sizeof(bufStore) - 1;

	while (bytesLeft > 0) {

	    if (!get(*buf))
			break;
		
		c = *buf;

	    if (quoted) {
			if (c == '\"')
				break;

			if (c == '\\') {		// other escapes ???????????
				if ((get(c)))
					switch (c) {  // escape sequences ??

					case '\\' : *buf = '\\'; break;
					case '\"' : *buf = '\"'; break;
					case '\n' : *buf = '\n'; break;
					case '\t' : *buf = '\t'; break;

					default :
						putBack(c);
						break;
					}
			}
			else 
			if ((c == '\n') || (c == '\r')) {  // new CRLF

				lineNum++;

				// got cr lf, store only '\n'
				if (*buf == '\n' && (buf>bufStore) && (buf[-1] ==  '\r')) {
					buf--;
					bytesLeft++;
					*buf = '\n';
					lineNum--;
				}

			}	
		} // quoted
	    else if (isspace(*buf)) {
		   putBack(*buf);
		   break;
	    }
        else if ( (*buf == '}') ||  (*buf == ']') || (*buf == ',') || (*buf == '#') ) { // HG Break on delimiter 4.4.96
		   putBack(*buf);
		   break;
	    }
	    buf++;
	    bytesLeft--;
	}
	*buf = '\0';

	s += bufStore;

    } while (bytesLeft == 0);

	if ((s.getLength() == 0) && !quoted) { // syntax error
        // if (verbose) 
		GvReadError::post(this,GV_RE_SYNTAX_ERROR, "Identifier expected %c",c);
		return FALSE;
    } return TRUE;
}

#define get(c)   (*p ? (c) = *p++: 0)
#define putBack(c) (p--)

int skipWhiteSpace(const char * &p) {
   while (*p && isspace(*p)) p++;
   return(1);
}


GvBool
UnQuote(GvString &src, GvString &s)
{
    const char *p= src;


    GvBool      quoted;
    char        c;
    char        bufStore[256*4];
    char        *buf;
    int         bytesLeft;
	int         lineNum = 0;

    s.makeEmpty();
    if (!skipWhiteSpace(p)) return FALSE;

    if (! get(c)) 
		return FALSE;

    quoted = (c == '\"');
    if (! quoted)
		putBack(c);

    do {
	buf       = bufStore;
	bytesLeft = sizeof(bufStore) - 1;

	while (bytesLeft > 0) {

	    if (! get(*buf))
		break;

	    if (quoted) {
			if (*buf == '\"')
		    	break;

			if (*buf == '\\') {
		    	if ((get(c)) && c == '\"')
					*buf = '\"';
		    	else
					putBack(c);
			}

			if ((*buf == '\n') || (*buf == '\r'))
		    	lineNum++;
	    }

	    else if (isspace(*buf)) {
			putBack(*buf);
		break;
	    }

	    buf++;
	    bytesLeft--;
	}
	*buf = '\0';

	s += bufStore;

    } while (bytesLeft == 0);

    return TRUE;
}

#undef putBack
#undef get

GvBool
GvInput::read(GvName &n, GvBool validIdent)
{
    GvBool	gotChar;

    if (! skipWhiteSpace())
		return FALSE;

    if (!validIdent) {
		GvString s;

		if (! read(s))
			return FALSE;

		n = s;
		return TRUE;
    }

    else {
		char	buf[256];
		char	*b = buf;
		char	c;


		if ((gotChar = get(c)) && GvName::isIdentStartChar(c,vrml2)) {
			*b++ = c;

			while ((gotChar = get(c)) && GvName::isIdentChar(c,vrml2)) {
				if (b - buf < 255)
					*b++ = c;
			}
		}
		*b = '\0';

		if (gotChar)
			putBack(c);

		n = buf;
		return(buf[0] != 0); 
	}

}


// read a nodename for DEF USE ROUTE etc
// should stop with whitespace or . etc
// could start with number
// was  read before 
GvBool
GvInput::readNodeName(GvName &n)
{
    GvBool	gotChar;

    if (! skipWhiteSpace())
		return FALSE;

		char	buf[256];
		char	*b = buf;
		char	c;


		if ((gotChar = get(c)) && GvName::isIdentChar(c,vrml2)) {
			*b++ = c;

			while ((gotChar = get(c)) && GvName::isIdentChar(c,vrml2)) {
			if (b - buf < 255)
				*b++ = c;
	    }
	
	*b = '\0';

	if (gotChar)
	    putBack(c);

	n = buf;
    }

    return TRUE;
}

#define READ_NUM(reader, readType, num, type)				      \
    GvBool ok;								      \
    if (! skipWhiteSpace())						      \
    ok = FALSE;								      \
    else {								      \
	readType _tmp;							      \
	ok = reader(_tmp);						      \
	if (ok)								      \
	    num = (type) _tmp;						      \
    }									      \
    return ok

#define READ_INTEGER(num, type)						      \
    READ_NUM(readInteger, long, num, type)

#define READ_UNSIGNED_INTEGER(num, type)				      \
    READ_NUM(readUnsignedInteger, unsigned long, num, type)

#define READ_REAL(num, type)						      \
    READ_NUM(readReal, double, num, type)

GvBool
GvInput::read(int &i)
{
    READ_INTEGER(i, int);
}

GvBool
GvInput::read(unsigned int &i)
{
    READ_UNSIGNED_INTEGER(i, unsigned int);
}

GvBool
GvInput::read(short &s)
{
    READ_INTEGER(s, short);
}

GvBool
GvInput::read(unsigned short &s)
{
    READ_UNSIGNED_INTEGER(s, unsigned short);
}

GvBool
GvInput::read(long &l)
{
    READ_INTEGER(l, long);
}

GvBool
GvInput::read(unsigned long &l)
{
    READ_UNSIGNED_INTEGER(l, unsigned long);
}

GvBool
GvInput::read(float &f)
{
    READ_REAL(f, float);
}

GvBool
GvInput::read(double &d)
{
    READ_REAL(d, double);
}

GvBool
GvInput::eof() const
{
    if (fp == NULL) return(TRUE);
    return !!fp->Eof();
}

void
GvInput::getLocationString(GvString &string) const
{
    char buf[128];
    sprintf(buf, "line %3d", lineNum);
    string = buf;
}

void
GvInput::putBack(char c)
{
    if (c == (char) EOF)
		return;

    if (backBufIndex >= 0)
		--backBufIndex;
    else
	if (fp) { // tried twice !!!!!!!!!!!1
		if (fp->ungetc(c)==EOF) 
		{
			backBuf = c;
			backBufIndex = 0;

		}
	}
}

void
GvInput::putBack(const char *string)
{
	int l = strlen(string);

    if (backBufIndex >0) {
		//assert that buffer content matches string 
		// get prob here in dump stringTRACE("put back %s|%s",&(backBuf.getString()[backBufIndex-l+1]), string);
		backBufIndex -= l;
	} else {
		backBuf = string;
		backBufIndex = 0;
	}
}

GvBool
GvInput::checkHeader()
{
    char	c;

    readHeader = TRUE;

    if (get(c)) {
    	if (c == COMMENT_CHAR) {
	    char	buf[256];
	    int		i = 0;

	    buf[i++] = c;
		while (get(c) && (c != '\n') && (c != 13) && (i<255)) {
			buf[i++] = c;
		}
	    
	    buf[i] = '\0';
	    if ((c == '\n') ||  (c == 13))
		   lineNum++;
		
		//HG to do : check for CR limited files ??????

		if ((i>0) && (buf[i-1] == '\r')) buf[i-1]=0; // Cybergate bug 22.02.96

	    if ((version = isASCIIHeader(buf))) {
            if (version >= 2.0) 
                vrml2 = TRUE;
            else vrml2 = FALSE;
#ifndef _G_VRML1
			if (!vrml2) {
				GvReadError::post(this,GV_RE_INVLAID_HEADER, "VRML 1.0 support is not available in this version");
				headerOk = FALSE;
				return FALSE;
			}		
#endif 
		
        }
		return TRUE;
	}

	else
	    putBack(c);
    }

    GvReadError::post(this, GV_RE_INVLAID_HEADER, "File does not have a valid header string");
    headerOk = FALSE;
    return FALSE;
}


GvBool 
GvInput::isWhiteSpace(char c)
{
	if (vrml2) {
		if (c == '\x20')	return TRUE; //   space
		if (c == '\x09')	return TRUE; //   tab
		if (c == '\x2c')	return TRUE; //   comma !!!!!!!!! strange
		if (c == '\x0d')	return TRUE; //   carriage return
		if (c == '\x0a')	return TRUE; //   newline return
		return FALSE;
	}
	// VRML 1.0
	return(isspace(c)!=0);
}


GvBool
GvInput::skipWhiteSpace()
{
    char	c;
    GvBool	gotChar;

    if (! readHeader && ! checkHeader())
	return FALSE;

    while (TRUE) {

	while ((gotChar = get(c)) && isWhiteSpace(c)) {
	    if ((c == '\n') || (c == '\r'))
		      lineNum++;
    }

	if (! gotChar)
	    break;

	if (c == COMMENT_CHAR) {
		while (get(c) && (c != '\n') && (c != '\r') )  // or CR ? only to do:
		;

	    if (eof())
	  	    /* HG 03.04.96 message not needed */
          //GvReadError::post(this, "EOF reached before end of comment");
          break;  

	    else
			lineNum++;
	}
	else if (c == 0) {
          //if (verbose) GvReadError::post(this, "BAD 0 character in file");
	}	
	else {	// not whitespace/comment char 
	    putBack(c);
	    break;				    
	}
    } // WHILE true 

    return TRUE;
}

GvBool
GvInput::readInteger(long &l)
{
    char str[64];
    char *s = str;

    if (readChar(s, '-') || readChar(s, '+'))
		s++;

    if (! readUnsignedIntegerString(s,62))
		return FALSE;

    l = strtol(str, NULL, 0);

    return TRUE;
}

GvBool
GvInput::readUnsignedInteger(unsigned long &l)
{
    char	str[64];		

    if (! readUnsignedIntegerString(str,63))
		return FALSE;

    l = strtoul(str, NULL, 0);

    return TRUE;
}

GvBool
GvInput::readUnsignedIntegerString(char *str,int maxCnt)
{
    int  minSize = 1;
    char *s = str;

    if (readChar(s, '0')) {
		if (readChar(s + 1, 'x')) {
			s += 2 + readHexDigits(s + 2,maxCnt-2);
			minSize = 3;
		}
		else if (readChar(s + 1, 'X')) {
			s += 2 + readHexDigits(s + 2,maxCnt-2);
			minSize = 3;
		}
		else
			s += 1 + readDigits(s + 1,maxCnt-1);
		}

    else
		s += readDigits(s,maxCnt);

    if (s - str < minSize)
		return FALSE;

    if ((s - str) >= maxCnt) // too long 
		return FALSE;

    *s = '\0';

    return TRUE;

}

/*
see Precision Specification
Value
 Output
 
+ infinity
 1.#INFrandom-digits
 
 -infinity
 1.#INFrandom-digits
 
Indefinite (same as quiet NaN)
 digit.#INDrandom-digits
 
NAN 
 digit.#NANrandom-digits
 
*/



GvBool
GvInput::readReal(double &d)
{
    char	str[80];
    int		n;
    char	*s = str;
    GvBool	gotNum = FALSE;

	// to do: NAN etc 

    n = readChar(s, '-');

    if (n == 0)
		n = readChar(s, '+');

    s += n;

    if ((n = readDigits(s,32)) > 0) {
		if (n==32) { // too long number
			return FALSE;
		}
		gotNum = TRUE;
		s += n;
    }

    if (readChar(s, '.') > 0) {
		s++;

		if ((n = readDigits(s,26)) > 0) { // DBL_DIG
			if (n==26) { // too long number
				return FALSE;
			}
			gotNum = TRUE;
			s += n;
		}
    }

    if (! gotNum)
		return FALSE;

    n = readChar(s, 'e');
    if (n == 0)
		n = readChar(s, 'E');

    if (n > 0) {
		s += n;

		n = readChar(s, '-');
		if (n == 0)
			n = readChar(s, '+');
		s += n;

		if ((n = readDigits(s,5)) > 0) {
			if (n==5) { // too long number
				return FALSE;
			}
			s += n;
		}

		else
			return FALSE;		    
	}

    *s = '\0';

    d = atof(str);

    return TRUE;
}

int
GvInput::readDigits(char *string,int maxCnt)
{
    char c, *s = string;
	int cnt=0;

    while (get(c)) {

		if (isdigit(c)) {
			cnt++;
			if (cnt>maxCnt) 
				break;
			*s++ = c;
		}
		else {
			putBack(c);
			break;
		}
    }

    return s - string;
}

int
GvInput::readHexDigits(char *string,int maxCnt)
{
    char c, *s = string;
	int cnt=0;

    while (get(c)) {

		if (isxdigit(c)) {
			cnt++;
			if (cnt>maxCnt) 
				break;

			*s++ = c;
		}

		else {
			putBack(c);
			break;
		}
    }

    return s - string;
}

int
GvInput::readChar(char *string, char charToRead)
{
    char	c;
    int		ret;

    if (!get(c))
	    ret = 0;

    else if (c == charToRead) {
	    *string = c;
	    ret = 1;
    }

    else {
		putBack(c);
		ret = 0;
    }

    return ret;
}



// push a local name space 

void 
GvInput::pushNameSpace(GvNameSpace *newNameSpace)
{
    ASSERT(newNameSpace!=NULL);
    newNameSpace->next = nameSpace;
    nameSpace = newNameSpace;
}


GvNameSpace* 
GvInput::popNameSpace() 
{
    GvNameSpace *oldNameSpace = nameSpace;
    ASSERT(oldNameSpace!=NULL);
    nameSpace = oldNameSpace->next;
	oldNameSpace->next = NULL; 
    return(oldNameSpace);
}


GvBool
GvInput::addReference(const GvName &name, GvNode *node)
{
    GvBool res= getNameSpace()->addNode(name,node);
    node->setName(name,vrml2);
    return(res);
}

GvBool
GvInput::removeReference(const GvName &name, GvNode *node)
{
    GvBool res= getNameSpace()->removeNode(name,node);
    return(res);
}

GvNode*
GvInput::findReference(const GvName &name) const
{
    return(getNameSpace()->findNode(name));
}

GvBool
GvInput::addProto(const GvName &name, GvProto *node)
{
    GvBool res= getNameSpace()->addProto(name,node); 
    return(res);
}

GvProto*
GvInput::findProto(const GvName &name) const
{
    return(getNameSpace()->findProto(name));
}

