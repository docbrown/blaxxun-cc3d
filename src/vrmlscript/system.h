//  -*- C++ -*-

/*
 * Copyright (C) 1996   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Description:
 |	This file contains basic Vb library definitions.
 |
 |   Author(s)		: 
 |       (for the PC)   : Jim Kent, Alan Norton, Daniel Woods
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _VB_SYSTEM_H_
#define  _VB_SYSTEM_H_



#if 0  // HG


#ifndef WIN32
  #define AFX_EXT_CLASS 

  #define AFX_EXT_API
#else
  //#include <stdafx.h>

  #include <windows.h>


  #ifdef _DEBUG
	#include <crtdbg.h>

    void G__cdecl VRMLTRACE(const char *, ...);
  #else

    #define VRMLTRACE() (void(0))

  #endif



  #define TRACE(expr) (void(0))

  #define NO_MFC_CLASSES


  #ifdef AFX_EXT_CLASS

    #undef AFX_EXT_CLASS

  #endif /* AFX_EXT_CLASS */

  #define AFX_EXT_CLASS



  #ifdef AFX_EXT_API 

    #undef AFX_EXT_API  

  #endif /* AFX_EXT_API */

  #define AFX_EXT_API

#endif /* WIN32 */

#endif 



// I hate efficiency where it isn't needed.  And I hate macros.

// The hates cancel.  

#ifdef WIN32

#define  SET_POINT_FROM_DWORD(point, dword)		    \
		SHORT  *__array = (SHORT *)(&dword);	    \
		point.x = (LONG) __array[0];		    \
		point.y = (LONG) __array[1]

#else

#define  MAKE_POINT_FROM_DWORD(point, dword)		    \
	/* Something to fix if this compiles */		    \
	ERROR: you must implement this functionality.

#endif /* WIN32 */


#endif /* _VB_SYSTEM_H_ */
