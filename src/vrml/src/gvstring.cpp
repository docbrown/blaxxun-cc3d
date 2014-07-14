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
#include <gvstring.h>

GvString::~GvString()
{
    if (string != staticStorage)
		delete [] string;
}

void
GvString::expand(int bySize)
{
    int newSize = _tcsclen(string) + bySize + 1;

    if (newSize >= GV_STRING_STATIC_STORAGE_SIZE && (string == staticStorage || newSize > storageSize)) {

	_TCHAR *newString = new _TCHAR[newSize];

	_tcscpy(newString, string);

	if (string != staticStorage)
	    delete [] string;

	string      = newString;
	storageSize = newSize;
    }
}

u_long
GvString::hash(const char *s)
{
    u_long	total, shift;

    total = shift = 0;
    while (*s) {
	total = total ^ ((*s) << shift);
	shift+=5;
	if (shift>24) shift -= 24;
	s++;
    }

    return( total );
}

void
GvString::makeEmpty(GvBool freeOld)
{
    if (string != staticStorage) {
		if (freeOld)
	    	delete [] string;
		string = staticStorage;
    }
    string[0] = '\0';
}

GvString &
GvString::operator=(const char *str)
{
    int size = str ? (strlen(str) + 1) : 1; // HG

    if (str >= string &&
		str < string + (string != staticStorage ? storageSize :
			GV_STRING_STATIC_STORAGE_SIZE)) {

		GvString tmp = str;
		*this = tmp;
		return *this;
    }

    if (size < GV_STRING_STATIC_STORAGE_SIZE) {
		if (string != staticStorage)
	    	makeEmpty();
    }

    else if (string == staticStorage)
		string = new char[size];

    else if (size > storageSize) {
		delete [] string;
		string = new char[size];
    }

    if (str) _tcscpy(string, str);
	else string[0]=0;
    storageSize = size;
    return *this;
}

GvString &
GvString::operator +=(LPCTSTR str)
{
	if (str) { // HG
    	expand(strlen(str));
    	_tcscat(string, str);
	} 
    return *this;
}

GvString &
GvString::operator +=(_TCHAR c)
{
    _TCHAR str[2];
	str[0]=c; str[1]= 0;
    expand(1); 
    _tcscat(string, str);
    return *this;
}

GvString operator+(const GvString& string1, const GvString& string2)
{ return GvString(string1,string2); }

GvString operator+(const GvString& string, TCHAR c)
{
    _TCHAR str[2];
	str[0]=c; str[1]= 0;
	return GvString(string,str); 
}

//GvString operator+(TCHAR ch, const GvString& string);

GvString operator+(const GvString& string1, LPCTSTR string2)
{ return GvString(string1,string2); }

GvString operator+(LPCTSTR string1, const GvString& string2)
{ return GvString(string1,string2); }



// similar to cstring, allocate buffer
char *GvString::getBuffer(int size)
{
	makeEmpty();

	expand(size);

	return(string);
}

void GvString::releaseBuffer( int nNewLength)
{
//  if (nNewLength == -1)
//  else
	// could resize string here to actual size 
}



GvBool
operator ==(const GvString &str, const char *s)
{
    return (str.string[0] == s[0] && ! strcmp(str.string, s));
}

GvBool
operator !=(const GvString &str, const char *s)
{
    return (str.string[0] != s[0] || strcmp(str.string, s));
}


// vrmlscript methods

int 
GvString::indexOf(const GvString &search,int fromIndex)
{
	int index = -1;
	if ((fromIndex>=0)  && (fromIndex<getLength())) {
		const char *p= string;
		const char *pp= strstr(p + fromIndex,search);
		if (pp) index = pp-p;
	}
	return index;
}


int GvString::lastIndexOf(const GvString &search,int fromIndex)
{
	int index = -1;

	int searchLen = search.getLength();
	if (fromIndex <0) fromIndex = getLength();

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



GvString& GvString::setSubstring(const GvString &s, int index1, int index2)
{
	int index = -1;
	int l = s.getLength();
	if (index1 > index2) { // swap
		int t = index1;
		index1 = index2;
		index2 = t;
	}


	if (index1>=0  && index1<l) {
		
		index2 = min(l,index2);

		int l= index2-index1; // not including
		if (l>0) {
		
			char *x=getBuffer(l+1);

			if (x) {		
				strncpy(x,s.getString()+index1,l);
				x[l]=0;
			}
		
			releaseBuffer();
		}
		else 
			makeEmpty();
	}
	else {
		makeEmpty();
	}
	return *this;

}

GvString& GvString::toLowerCase()
{
	_strlwr(string);
	return *this;
}

GvString& GvString::toUpperCase() 
{
	_strupr(string);
	return *this;
}



#ifdef _COM
GvString& 
GvString::operator =(BSTR str)
{
	const char *vs;
	USES_CONVERSION;

	vs = OLE2CT(str);
	
	*this = vs;
    return *this;
}

BSTR GvString::SysAllocString() const
{ 
	USES_CONVERSION;
	return  ::SysAllocString(T2COLE(string));
}


#endif

