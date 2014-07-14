

#include "stdafx.h"

void strlwr(char * s)
{ 
    while(*s)
  	{
         if( isupper(*s)) *s = tolower(*s);
         s++;
     }
}

void strupr(char * s)
{ 
	while(*s) {
       	if( islower(*s)) *s = toupper(*s);
        s++;
    }
}
 

// emulation for MFC CString 
CString::CString(LPCTSTR lpsz) 
{ 	s = NULL;
	if (lpsz)
        s = strdup(lpsz);
}

CString::CString(const CString &stringSrc)
{ 	s = NULL;
	if (!stringSrc.IsEmpty())
        s = strdup(stringSrc.s);
}



CString::CString(LPCTSTR lpsz, int count ) 
{ s = NULL;
  if (lpsz)
  {
      s = (char*) malloc ( count + 1 );
      strncpy( s, lpsz, count );
      s[ count ] = 0;
  }
}



void CString::Empty()

{

   if (s)

       free(s);

    s = NULL;

} // free up the data



const CString& CString ::operator=(const CString& stringSrc)
{ Empty();
  if (!stringSrc.IsEmpty())
      s = strdup(stringSrc.s);
  return *this;
}


const CString& CString::operator=(LPCTSTR lpsz)
{ 

	Empty();
	if (lpsz)
          s = strdup(lpsz);
    return *this; 

}

// string concatenation
const CString& CString::operator+=(LPCTSTR lpsz)
{
		char * p;
        int    ls = 0, lp = 0;

        if( !lpsz )
        {
            return *this;
        }
        else
        {
            lp = strlen( lpsz );
        }

        if( s )
        {
            ls = strlen( s );
        }

        p = (char*)malloc( ls + lp + 1 );
        if( p )
        {
            if( ls )
            {
                strncpy( p, s, ls );
            }
            if( lp )
            {
                strncpy( p + ls, lpsz, lp );
            }
            p[ ls + lp ] = 0;
        }

        Empty();

        s = p;

        return *this;
            
}


const CString& CString::operator+=(TCHAR ch)
{
    char p[ 2 ];
    p [ 0 ] = ch;
    p [ 1 ] = 0;
    return ( *this += p );
};


// trimming whitespace (either side)
void CString::TrimRight()

{ char * p;

  if( !s )
      return;

  p = s + strlen( s ) - 1;
  while( p >= s && isspace( *p ))
  {
     *p = 0;
	 p--;
  }
}

void CString::TrimLeft()

{ 

  char * t = s;
  char * p = s;

  if( !s )
      return;

  while( *p && isspace( *p ))
      p++;

  if( t == p )
      return;

  while( *p )
      *t++ = *p++;

  *t = 0;
}





// look for a specific sub-string
int  CString::Find(LPCTSTR lpszSub) const        // like "C" strstr
{
    char * p;

    if( !s )
    {
        return( -1 );
    }

    p = strstr( s, lpszSub );
    if( p )
    {
        return( p - s );
    }
    return( -1 );
}


int CString::Find(TCHAR ch) const                // like "C" strchr
{
    char * p;

    if( !s )
    {
        return( -1 );
    }

    p = strchr( s, ch );
    if( p )
    {
        return( p - s );
    }
    return( -1 );
}
int CString::ReverseFind(TCHAR ch) const                // like "C" strchr
{
    char * p;

    if( !s )
    {
        return( -1 );
    }

    p = strrchr( s, ch );
    if( p )
    {
        return( p - s );
    }
    return( -1 );
}



CString operator+(const CString& string1,
		          const CString& string2)
{
    CString s(string1);
    if (!string2.IsEmpty()) s += string2.s;
    return( s );
}

CString operator+(const CString& string1, LPCTSTR string2)
{
    CString s(string1);
    if (string2) s += string2;
    return( s );
}

CString operator+(LPCTSTR string1, const CString& string2)
{
    CString s(string1);
    if (!string2.IsEmpty()) s += string2;
    return( s );
}

bool  operator==(const CString& string, LPCTSTR lpsz)
{ 
    return string.s== lpsz || (strcmp(string.s,lpsz)==0); 
}


bool  operator==(const CString& string1,const CString& string2)
{ 
    return string1.s== string2.s || (strcmp(string1.s,string2.s)==0); 
}


