#if !defined(AFX_STDAFX_H__7CE086A4_3E78_11D0_AE6B_00C04FD7D06E__INCLUDED_)
#define AFX_STDAFX_H__7CE086A4_3E78_11D0_AE6B_00C04FD7D06E__INCLUDED_

/******************************************************************************
@doc

@module stdafx.h - adapted for cygwin / MESA |

Copyright (c) 19998 - 1999 by blaxxun interactive - Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:
 

 stdafx.h : include file for standard system include files,
            or project specific include files that are used frequently,
			but are changed infrequently


Changes:

$Revision: 1.1 $
$Log: stdafx.h,v $
Revision 1.1  1999/11/29 20:32:46  holger
empty message

Revision 1.16  1999/07/06 17:02:07  tom


Todo :

******************************************************************************/

#define _GLVIEW_DLL         hello, this is peter
#define _GLMESA             hello, this is peter


#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define STRICT

#define Linux

#ifdef __Linux__
#define Linux
#endif

#ifdef _Linux_
#define Linux
#endif



#include "gconfig.h"

#ifdef G_RMA_MOVIE
// Real G2 SDK problem , conflicting declarations in MSVC 6.0, see comment in pntypes.h !

#if (defined(_MSC_VER) && (_MSC_VER >= 1200)) // VC6 
#include "pntypes.h"
#endif

#endif


// ATL 1.1/2.0 COM VRML Interface 

#ifdef _COM

#include "stdafx_atl.h"

#endif // _COM


//  C-Library 
#include <math.h>
#include <float.h>
#include <string.h>
#include <ctype.h>

#include <time.h>

#include <stdlib.h>
#include <stdarg.h>

#ifdef WIN32

#include <windows.h>

#define HAS_vsnprintf

#else 

#define HAS_vsnprintf

#define _vsnprintf vsnprintf
#define _snprintf  snprintf
#define _alloca    alloca

#ifndef _MSC_VER
#include <sys/unistd.h>
#include <sys/time.h>
#include <sys/param.h>

#define G__cdecl /* */

#define __stdcall
#define ZeroMemory  bzero

#else 

#define G__cdecl __cdecl
typedef unsigned long u_long;
#define M_PI 3.14159265358979323846
#define M_PI_4 (M_PI/4.0)
typedef unsigned long uint32_t;
typedef long          int32_t;


#endif


#include <sys/types.h>
#include <sys/timeb.h>

// some win types 
#ifndef VOID
#define VOID void
typedef char CHAR;
typedef short SHORT;
typedef long LONG;

typedef LONG HRESULT;

typedef void				*LPVOID;
#endif

typedef int                 INT;
typedef unsigned int        UINT;

#ifndef __TCHAR_DEFINED
typedef char            _TCHAR;
typedef signed char     _TSCHAR;
typedef unsigned char   _TUCHAR;
typedef unsigned char   _TXCHAR;
typedef unsigned int    _TINT;

#define _T( X ) X

#define __TCHAR_DEFINED

/* String functions */
#define _tcscat     strcat
#define _tcscpy     strcpy
#define _tcslen     strlen
#define _tcsxfrm    strxfrm
#define _tcsdup     _strdup


#define _tcschr     strchr
#define _tcscspn    strcspn
#define _tcsncat    strncat
#define _tcsncpy    strncpy
#define _tcspbrk    strpbrk
#define _tcsrchr    strrchr
#define _tcsspn     strspn
#define _tcsstr     strstr
#define _tcstok     strtok

#define _tcsnset    _strnset
#define _tcsrev     _strrev
#define _tcsset     _strset

#define _tcscmp     strcmp
#define _tcsicmp    _stricmp
#define _tcsnccmp   strncmp
#define _tcsncmp    strncmp
#define _tcsncicmp  _strnicmp
#define _tcsnicmp   _strnicmp

#define _tcscoll    strcoll
#define _tcsicoll   _stricoll
#define _tcsnccoll  _strncoll
#define _tcsncoll   _strncoll
#define _tcsncicoll _strnicoll
#define _tcsnicoll  _strnicoll

#define stricmp     strcasecmp
#define _stricmp    strcasecmp
#define strnicmp    strncasecmp
#define _strnicmp   strncasecmp

/* "logical-character" mappings */

#define _tcsclen    strlen
#define _tcsnccat   strncat
#define _tcsnccpy   strncpy
#define _tcsncset   _strnset

#endif

#ifndef _TCHAR_DEFINED
typedef char            TCHAR;
#define _TCHAR_DEFINED
#endif

#define LPCTSTR const TCHAR *
#define LPCSTR const TCHAR *

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif


// WIN32 Critical section
// need if multiple plugins are sharing global data
typedef int CRITICAL_SECTION;
typedef CRITICAL_SECTION* LPCRITICAL_SECTION;


inline void InitializeCriticalSection( LPCRITICAL_SECTION lpCriticalSection  )

{}

inline void EnterCriticalSection( LPCRITICAL_SECTION lpCriticalSection)
{}

inline void LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{}

inline long InterlockedDecrement(long * l) { l--; return *l; }
inline long InterlockedIncrement(long * l) { l++; return *l;  }

//typedef __int64 LONGLONG; // 
typedef double LONGLONG; // 

typedef union _LARGE_INTEGER {
/*    struct {
        DWORD LowPart;
        LONG HighPart;
    };
*/
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER;

inline BOOL
QueryPerformanceCounter(
    LARGE_INTEGER *lpPerformanceCount
    )
{ return FALSE; }

inline BOOL
QueryPerformanceFrequency(
    LARGE_INTEGER *lpFrequency
    )
{ return FALSE; }


//The timeGetTime function retrieves the system time, in milliseconds. 
inline DWORD timeGetTime() { return (1000l*clock()) / CLOCKS_PER_SEC  ; }

#define _timeb timeb
#define _ftime ftime
#define _strlwr strlwr
#define _strupr strupr

#endif


#ifndef _MAX_PATH
#define _MAX_PATH MAXPATHLEN
#endif

#define HTREEITEM  void *

#define HANDLE void *
#define HWND void *
#define HDC  void *
typedef struct tagRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} RECT, *PRECT;

typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT, *PPOINT;

/*
 * Key State Masks for Mouse Messages
 */
#define MK_LBUTTON          0x0001
#define MK_RBUTTON          0x0002
#define MK_SHIFT            0x0004
#define MK_CONTROL          0x0008
#define MK_MBUTTON          0x0010



// Open GL-required Headerfiles for WINDOWS NT/95

#ifdef _OGL

// mesa test
#undef __CYGWIN32__

#include <GL/gl.h>
#include <GL/glu.h>

// mesa GLU is outdated
// try some workarounds


#include "gluemul.h"

#ifndef GLU_VERSION_1_2 
#endif


#endif _OGL


#ifdef _D3D // Direct 3D 
#include <ddraw.h>
#include <d3d.h>
#include "d3dmacs.h"
#endif _D3D


#if (defined(_MSC_VER))

#define  BEGIN_PACKED_STRUCT struct  { // GCC does not like this ?
		//int mInverseOk:1;	// flag if mInverse is valide
		//int viewpointLocalOk:1;
#define  END_PACKED_STRUCT };


#else

#define	BEGIN_PACKED_STRUCT
#define	END_PACKED_STRUCT


#endif



#if (defined(_MSC_VER_XXX) && defined(_DEBUG))

// MFC diagnostics

#define new DEBUG_NEW

// Debug memory allocation routines
#include <CRTDBG.H>

#define REALLOC(PTR,SIZE) ::_realloc_dbg(PTR,SIZE,_NORMAL_BLOCK,__FILE__,__LINE__)
#define CALLOC(NUM,SIZE) ::_calloc_dbg(NUM,SIZE,_NORMAL_BLOCK,__FILE__,__LINE__)
#define MALLOC(SIZE) ::_malloc_dbg(SIZE,_NORMAL_BLOCK,__FILE__,__LINE__)
#define FREE(PTR) ::_free_dbg(PTR,_NORMAL_BLOCK)

#else

// standard C routines

#define REALLOC(PTR,SIZE) ::realloc(PTR,SIZE)
#define CALLOC(NUM,SIZE) ::calloc(NUM,SIZE)
#define MALLOC(SIZE) ::malloc(SIZE)
#define FREE(PTR) ::free(PTR)

#endif

#define _VC50

#if defined(_WIN32)

// disable data conversion warnings of Microsoft C
// VC 5.0
#pragma warning(disable : 4056)     // overflow in floating-point constant arithmetic
#pragma warning(disable : 4244)     // conversion from 'double' to 'float', possible loss of data
#pragma warning(disable : 4305)     // truncation from 'const double' to 'float

#ifdef _DEBUG
#pragma warning(disable : 4100)     //  'n' : unreferenced formal parameter
#endif

#endif


#ifndef _MFC 

/////////////////////////////////////////////////////////////////////////////
// Diagnostic support

#ifdef _DEBUG_XXXX

BOOL AfxAssertFailedLine(LPCSTR lpszFileName, int nLine);
void G__cdecl AfxTrace(LPCTSTR lpszFormat, ...);

#define TRACE              ::SysTrace
#define THIS_FILE          __FILE__
#define ASSERT(f) \
	do \
	{ \
	if (!(f) && AfxAssertFailedLine(THIS_FILE, __LINE__)) \
		SysDebugBreak(); \
	} while (0) \

#define VERIFY(f)          ASSERT(f)
#define DEBUG_ONLY(f)      (f)

// The following trace macros are provided for backward compatiblity
//  (they also take a fixed number of parameters which provides
//   some amount of extra error checking)
#define TRACE0(sz)              ::AfxTrace(_T("%s"), _T(sz))
#define TRACE1(sz, p1)          ::AfxTrace(_T(sz), p1)
#define TRACE2(sz, p1, p2)      ::AfxTrace(_T(sz), p1, p2)
#define TRACE3(sz, p1, p2, p3)  ::AfxTrace(_T(sz), p1, p2, p3)

// These AFX_DUMP macros also provided for backward compatibility
#define AFX_DUMP0(dc, sz)   dc << _T(sz)
#define AFX_DUMP1(dc, sz, p1) dc << _T(sz) << p1

#else   // _DEBUG

#define ASSERT(f)          ((void)0)
#define VERIFY(f)          ((void)(f))
#define DEBUG_ONLY(f)      ((void)0)
inline void G__cdecl AfxTrace(LPCTSTR, ...) { }

#define AfxIsValidAddress(a,b,c)

#define TRACE              1 ? (void)0 : ::AfxTrace
#define TRACE0(sz)
#define TRACE1(sz, p1)
#define TRACE2(sz, p1, p2)
#define TRACE3(sz, p1, p2, p3)

#endif // !_DEBUG

#define ASSERT_POINTER(p, type) ASSERT(((p) != NULL) && AfxIsValidAddress((p), sizeof(type), FALSE))

#define ASSERT_NULL_OR_POINTER(p, type) ASSERT(((p) == NULL) || AfxIsValidAddress((p), sizeof(type), FALSE))


#ifdef _DEBUG
#define UNUSED(x)
#else
#define UNUSED(x) x
#endif
#define UNUSED_ALWAYS(x) x

#define _strlwr strlwr
extern void strlwr(char * s);


#define _strupr strupr
extern void strupr(char * s);


// emulation for MFC CString 
class CString
{
	char *s;
public:

	// Constructors
	CString() : s(NULL) {};

	CString(const CString &stringSrc);
	
	CString(LPCTSTR lpsz);
	CString(LPCTSTR lpsz,int count ) ;
	

	~CString() { Empty(); };

	// Attributes & Operations
	// as an array of characters
	inline int GetLength() const { if (s) return strlen(s); else return 0; }

	inline BOOL IsEmpty() const { return ((s== NULL) || (!s[0])); }

	void Empty();

	inline TCHAR GetAt(int nIndex) const { return s[nIndex]; };      // 0 based
	inline TCHAR operator[](int nIndex) const { return s[nIndex]; } ; // same as GetAt
	
	inline void SetAt(int nIndex, TCHAR ch) { s[nIndex]=ch; }

	operator LPCTSTR() const { return s; }           // as a C string

    char * GetBuffer( int l ){
       	Empty();
        s = (char*)malloc( l + 1 );
        *s = 0;
        return( s );
    }

    void ReleaseBuffer(){ };

	// overloaded assignment
	const CString& operator=(const CString& stringSrc);

//	const CString& operator=(TCHAR ch);

	const CString& operator=(LPCTSTR lpsz);


	void MakeUpper(){ if( s ) strupr( s ); };
	void MakeLower(){ if( s ) strlwr( s ); };


	// string concatenation
	const CString& operator+=(LPCTSTR lpsz);


	const CString& operator+=(const CString& string)
    {
        if (!string.IsEmpty()) *this += string.s;
		return *this;
    };


	const CString& operator+=(TCHAR ch);

/*
	friend CString AFXAPI operator+(const CString& string, TCHAR ch);
	friend CString AFXAPI operator+(TCHAR ch, const CString& string);
#ifdef _UNICODE
	friend CString AFXAPI operator+(const CString& string, char ch);
	friend CString AFXAPI operator+(char ch, const CString& string);
#endif
*/
	friend CString operator+(const CString& string1,const CString& string2);
	friend CString operator+(const CString& string, LPCTSTR lpsz);
	friend CString operator+(LPCTSTR lpsz, const CString& string);

	// string comparison
	int Compare(LPCTSTR lpsz) const { return strcmp(s,lpsz);  } 	// straight character
	friend bool operator==(const CString& string, LPCTSTR lpsz);
	friend bool operator==(const CString& string1,const CString& string2);

/*
	int CompareNoCase(LPCTSTR lpsz) const;   // ignore case
	int Collate(LPCTSTR lpsz) const;         // NLS aware

	// simple sub-string extraction
	CString Mid(int nFirst, int nCount) const;
	CString Mid(int nFirst) const;
	CString Left(int nCount) const;
	CString Right(int nCount) const;

	CString SpanIncluding(LPCTSTR lpszCharSet) const;
	CString SpanExcluding(LPCTSTR lpszCharSet) const;

	// upper/lower/reverse conversion
	void MakeUpper();
	void MakeLower();
	void MakeReverse();

*/
	// trimming whitespace (either side)
	void TrimRight();

	void TrimLeft();


	// look for a specific sub-string
	int Find(LPCTSTR lpszSub) const;        // like "C" strstr
	int Find(TCHAR ch) const;                // like "C" strchr


    int ReverseFind(TCHAR ch) const;                // like "C" strchr

    CString Left( int i )
    {
       return CString( s, i );
    }

    CString Mid( int i )
    {
       if( i <= strlen( s ))
           return CString( s + i );
       else
           return CString();
    }

/*
	// searching (return starting index, or -1 if not found)
	// look for a single character match
	int ReverseFind(TCHAR ch) const;
	int FindOneOf(LPCTSTR lpszCharSet) const;


	// simple formatting
	void AFX_CDECL Format(LPCTSTR lpszFormat, ...);
	void AFX_CDECL Format(UINT nFormatID, ...);
*/





};


#include "arraynew.h"  

class CStringArray {

public:

	ArrayNew<CString> v;

	void RemoveAll();

    int GetSize() { return( v.Length()); };
    CString &GetAt( int i ) { return( v[ i ] ); };

    void Add( const CString & s ) { v.Append( s ); };
};



class CTime
{
public:

// Constructors
	static CTime GetCurrentTime(){ return CTime( time( NULL )); };

	CTime(){m_time = 0; };
	CTime(time_t time):m_time(time){} ;
/*
	CTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,
		int nDST = -1);
	CTime(WORD wDosDate, WORD wDosTime, int nDST = -1);
	CTime(const CTime& timeSrc);

	CTime(const SYSTEMTIME& sysTime, int nDST = -1);
	CTime(const FILETIME& fileTime, int nDST = -1);
	const CTime& operator=(const CTime& timeSrc);
	const CTime& operator=(time_t t);

// Attributes
	struct tm* GetGmtTm(struct tm* ptm = NULL) const;
	struct tm* GetLocalTm(struct tm* ptm = NULL) const;
*/

	time_t GetTime() const{ return m_time; };
/*

	int GetYear() const;
	int GetMonth() const;       // month of year (1 = Jan)
	int GetDay() const;         // day of month
	int GetHour() const;
	int GetMinute() const;
	int GetSecond() const;
	int GetDayOfWeek() const;   // 1=Sun, 2=Mon, ..., 7=Sat

// Operations
	// time math
	CTimeSpan operator-(CTime time) const;
	CTime operator-(CTimeSpan timeSpan) const;
	CTime operator+(CTimeSpan timeSpan) const;
	const CTime& operator+=(CTimeSpan timeSpan);
	const CTime& operator-=(CTimeSpan timeSpan);
*/

	BOOL operator==(CTime time) const{ return m_time == time.m_time; };
	BOOL operator!=(CTime time) const{ return m_time != time.m_time; };
	BOOL operator<(CTime time) const{ return m_time < time.m_time; };
	BOOL operator>(CTime time) const{ return m_time > time.m_time; };
	BOOL operator<=(CTime time) const{ return m_time <= time.m_time; };
	BOOL operator>=(CTime time) const{ return m_time >= time.m_time; };

	/*
	// formatting using "C" strftime
	CString Format(LPCTSTR pFormat) const;
	CString FormatGmt(LPCTSTR pFormat) const;
	CString Format(UINT nFormatID) const;
	CString FormatGmt(UINT nFormatID) const;
*/



private:
	time_t m_time;
};

/////////////////////////////////////////////////////////////////////////////
// CPoint - A 2-D point, similar to Windows POINT structure.

class CPoint : public tagPOINT
{
public:

// Constructors
	CPoint() {} 

	CPoint(int initX, int initY) { x = initX; y = initY; }

	CPoint(POINT initPt) { *(POINT*)this = initPt; }

/*

	CPoint(SIZE initSize);
	CPoint(DWORD dwPoint);
// Operations
	void Offset(int xOffset, int yOffset);
	void Offset(POINT point);
	void Offset(SIZE size);
	BOOL operator==(POINT point) const;
	BOOL operator!=(POINT point) const;
	void operator+=(SIZE size);
	void operator-=(SIZE size);
	void operator+=(POINT point);
	void operator-=(POINT point);

// Operators returning CPoint values
	CPoint operator+(SIZE size) const;
	CPoint operator-(SIZE size) const;
	CPoint operator-() const;
	CPoint operator+(POINT point) const;

// Operators returning CSize values
	CSize operator-(POINT point) const;

// Operators returning CRect values
	CRect operator+(const RECT* lpRect) const;
	CRect operator-(const RECT* lpRect) const;
*/

};

class CRect : public tagRECT
{
public:

// Constructors
	CRect() { } 
	CRect(int l, int t, int r, int b) { left = l; top = t; right = r; bottom = b; }
/*
	CRect(const RECT& srcRect);
	CRect(LPCRECT lpSrcRect);
	CRect(POINT point, SIZE size);
	CRect(POINT topLeft, POINT bottomRight);
*/
// Attributes (in addition to RECT members)
	int Width() const 	{ return right - left; }
	int Height() const 	{ return bottom - top; }

/*
	CSize Size() const;
	CPoint& TopLeft();
	CPoint& BottomRight();
	const CPoint& TopLeft() const;
	const CPoint& BottomRight() const;
	CPoint CenterPoint() const;

	// convert between CRect and LPRECT/LPCRECT (no need for &)
	operator LPRECT();
	operator LPCRECT() const;

	BOOL IsRectEmpty() const;
	BOOL IsRectNull() const;
	BOOL PtInRect(POINT point) const;

// Operations
	void SetRect(int x1, int y1, int x2, int y2);
	void SetRect(POINT topLeft, POINT bottomRight);
	void SetRectEmpty();
	void CopyRect(LPCRECT lpSrcRect);
	BOOL EqualRect(LPCRECT lpRect) const;

	void InflateRect(int x, int y);
	void InflateRect(SIZE size);
	void InflateRect(LPCRECT lpRect);
	void InflateRect(int l, int t, int r, int b);
	void DeflateRect(int x, int y);
	void DeflateRect(SIZE size);
	void DeflateRect(LPCRECT lpRect);
	void DeflateRect(int l, int t, int r, int b);

	void OffsetRect(int x, int y);
	void OffsetRect(SIZE size);
	void OffsetRect(POINT point);
	void NormalizeRect();

	// operations that fill '*this' with result
	BOOL IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2);
	BOOL UnionRect(LPCRECT lpRect1, LPCRECT lpRect2);
	BOOL SubtractRect(LPCRECT lpRectSrc1, LPCRECT lpRectSrc2);

// Additional Operations
	void operator=(const RECT& srcRect);
	BOOL operator==(const RECT& rect) const;
	BOOL operator!=(const RECT& rect) const;
	void operator+=(POINT point);
	void operator+=(SIZE size);
	void operator+=(LPCRECT lpRect);
	void operator-=(POINT point);
	void operator-=(SIZE size);
	void operator-=(LPCRECT lpRect);
	void operator&=(const RECT& rect);
	void operator|=(const RECT& rect);

// Operators returning CRect values
	CRect operator+(POINT point) const;
	CRect operator-(POINT point) const;
	CRect operator+(LPCRECT lpRect) const;
	CRect operator+(SIZE size) const;
	CRect operator-(SIZE size) const;
	CRect operator-(LPCRECT lpRect) const;
	CRect operator&(const RECT& rect2) const;
	CRect operator|(const RECT& rect2) const;
	CRect MulDiv(int nMultiplier, int nDivisor) const;
*/
};



/////////////////////////////////////////////////////////////////////////////
// CCriticalSection

class CCriticalSection 
{

// Constructor
public:
	CCriticalSection() {::InitializeCriticalSection(&m_sect); }

// Attributes
public:
	operator CRITICAL_SECTION*()
	{ return (CRITICAL_SECTION*) &m_sect; }

	CRITICAL_SECTION m_sect;

// Operations
public:
	BOOL Unlock()
	{ ::LeaveCriticalSection(&m_sect); return TRUE; }
	
	BOOL Lock()
	{ ::EnterCriticalSection(&m_sect); return TRUE; }
	
	BOOL Lock(DWORD dwTimeout)
	{ return Lock(); }

// Implementation
public:
	~CCriticalSection() {}
};

#endif

// tools

#include "gutils.h"

#include "rt.h"
#include "rtabst.h"
#include "astream.h"

// template classes
#include "array.h"  
#include "arraynew.h"  
#include "stack.h"


// support classes
#include "point.h"
#include "pointa.h"
#include "matrix.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__7CE086A4_3E78_11D0_AE6B_00C04FD7D06E__INCLUDED)

