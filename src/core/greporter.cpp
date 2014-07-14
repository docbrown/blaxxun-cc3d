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

@module GReporter.h - GLView Error reporter object|

Copyright (c) 1995 - 1997	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GReporter>

Notes:

Changes:

Todo :
	think about log file support 

******************************************************************************/

#include "stdafx.h"


#include "greporter.h"


GReporter * GReporter::current = NULL;
void * GReporter::currentData = NULL;

GReporter * GReporter::defaultReporter = NULL;
void * GReporter::defaultData = NULL;


static CRITICAL_SECTION fileSection;  
static BOOL fileSectionInit = FALSE;

void GReporter::Lock() 
{
	// Initialize the critical section.
	if (!fileSectionInit) { 
		InitializeCriticalSection(&fileSection); 
		fileSectionInit = TRUE;
	}
	//TRACE("Enter section\n");

    EnterCriticalSection(&fileSection); 
	locked++;

}

BOOL GReporter::TryLock() 
{
	// Initialize the critical section.
	if (!fileSectionInit) { 
		InitializeCriticalSection(&fileSection); 
		fileSectionInit = TRUE;
	}
	//TRACE("Enter section\n");

	if (locked) return FALSE;
	Lock();
	return TRUE;

    //return TryEnterCriticalSection(&fileSection); only NT

}


void GReporter::UnLock()
{
  
  LeaveCriticalSection(&fileSection);
  locked--;
}


int GReporter::Warning1(const char * message)
{
  TRACE("Warning : %s\n",message);
  if (ignoreWarnings) return -1;


#ifdef WIN32
  if (!TryLock()) return -1;

		if (AfxMessageBox(message,MB_OKCANCEL) != IDOK) {
		   ignoreWarnings= TRUE;
		}

  
  UnLock();
#else
  	fprintf(stderr, "VRML Warning: %s",message);
#endif


  return(0);
}

int GReporter::Status1(const char * message)
{
  TRACE("%s\n",message);
  return(0);
}


int GReporter::Error1(const char * message)
{ int ret = 0;
  TRACE("Error : %s",message);
  
  if (ignoreErrors) return -1;

#ifdef WIN32
  if (!TryLock()) return -1;

  //Lock();
	// ret = AfxMessageBox(message);

	if (AfxMessageBox(message,MB_OKCANCEL) != IDOK) {
		   ignoreErrors= TRUE;
	}
  
  UnLock();
#else
  	fprintf(stderr, "VRML Error: %s",message);
#endif


  return(0);
}

int GReporter::Trace1(const char * message)
{
  TRACE("%s\n",message);
  return(0);
}
	

int G__cdecl GReporter::Warning(const char *formatString, ...)
{
    char	buf[10000];
    va_list	ap;
	int cnt;


    va_start(ap, formatString);

#ifdef HAS_vsnprintf
	cnt = _vsnprintf(buf,sizeof(buf), formatString,ap); // win32
#else
    cnt = vsprintf(buf, formatString, ap);
#endif

    va_end(ap);
	if (cnt <=0) {
		ASSERT(0);
		return (0);
	}

	return(Warning1(buf));
}

int G__cdecl GReporter::Status(const char *formatString, ...)
{
    char	buf[10000];
	int cnt;
    va_list	ap;
    va_start(ap, formatString);

#ifdef HAS_vsnprintf
	cnt = _vsnprintf(buf,sizeof(buf), formatString,ap); // win32
#else
    cnt = vsprintf(buf, formatString, ap);
#endif

    va_end(ap);

	if (cnt <=0) {
		ASSERT(0);
		return (0);
	}

	return(Status1(buf));
}

int G__cdecl GReporter::Error(const char *formatString, ...)
{
    char	buf[10000];
    va_list	ap;
	int cnt;

    va_start(ap, formatString);
#ifdef HAS_vsnprintf
	cnt = _vsnprintf(buf,sizeof(buf), formatString,ap); // win32
#else
    cnt = vsprintf(buf, formatString, ap);
#endif
    va_end(ap);
	if (cnt <=0) {
		ASSERT(0);
		return (0);
	}
	if (cnt <= 120) Status1(buf);
	return(Error1(buf));
}

int G__cdecl GReporter::Trace(const char *formatString, ...)
{
    char	buf[10000];
    va_list	ap;
	int cnt;
    va_start(ap, formatString);

#ifdef HAS_vsnprintf
	cnt= _vsnprintf(buf,sizeof(buf), formatString,ap); // win32
#else
    cnt = vsprintf(buf, formatString, ap);
#endif


    va_end(ap);

	if (cnt <=0) {
		ASSERT(0);
		return (0);
		
	}


	return(Trace1(buf));

}

