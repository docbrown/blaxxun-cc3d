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
#if !defined(AFX_STDAFX_H__7CE086A4_3E78_11D0_AE6B_00C04FD7D06E__INCLUDED_)
#define AFX_STDAFX_H__7CE086A4_3E78_11D0_AE6B_00C04FD7D06E__INCLUDED_

/******************************************************************************
@doc

@module stdafx.h - blaxxunCC3D OCX Control standard header file|

Copyright (c) 1997 - 1999 by blaxxun interactive - Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:
 

 stdafx.h : include file for standard system include files,
            or project specific include files that are used frequently,
			but are changed infrequently


Changes:

$Revision: 1.16 $
$Log: stdafx.h,v $
Revision 1.16  1999/07/06 17:02:07  tom
*** empty log message ***

Revision 1.15  1999/05/05 14:36:43  tom
empty message



Todo :

******************************************************************************/


#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define STRICT

#include "gconfig.h"

#ifdef G_RMA_MOVIE
// Real G2 SDK problem , conflicting declarations in MSVC 6.0, see comment in pntypes.h !

#if (defined(_MSC_VER) && (_MSC_VER >= 1200)) // VC6 
#include "pntypes.h"
#endif

#endif

// MFC Stuff 
#include <afxctl.h>         // MFC support for OLE Controls
#include <afxmt.h>			// AFX MT support 

#ifdef _AFX     
	#define _MFC
#endif

 


// ATL 1.1/2.0 COM VRML Interface 

#ifdef _COM

#include "stdafx_atl.h"

#endif // _COM


//  C-Library 
#include <math.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

// new placement operator

#include <new.h>

// WIN specific
#define G__cdecl __cdecl

#ifdef WIN32

#include <windows.h>

#define HAS_vsnprintf

#endif


// Open GL-required Headerfiles for WINDOWS NT/95

#ifdef _OGL
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifdef _D3D // Direct 3D 
#include <ddraw.h>
#include <d3d.h>
#include "d3dmacs.h"
#endif _D3D


// packed anon structures

#define  BEGIN_PACKED_STRUCT struct  { // GCC does not like this ?
		//int mInverseOk:1;	// flag if mInverse is valide
		//int viewpointLocalOk:1;
#define  END_PACKED_STRUCT };



#if defined(WIN32) && defined(_DEBUG)


// MS-VC memory diagnostics

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

#if defined (_MSC_VER)
#define _VC50

// disable data conversion warnings of Microsoft C
// VC 5.0
#pragma warning(disable : 4056)     // overflow in floating-point constant arithmetic
#pragma warning(disable : 4244)     // conversion from 'double' to 'float', possible loss of data
#pragma warning(disable : 4305)     // truncation from 'const double' to 'float
#pragma warning(disable : 4800)     // 'int' : forcing value to bool 'true' or 'false' (performance warning)


#ifdef _DEBUG
#pragma warning(disable : 4100)     //  'n' : unreferenced formal parameter
#endif

#endif


// tools

#include "gutils.h"

#include "rt.h"
#include "rtabst.h"
#include "astream.h"

// template classes
#include "array.h"  
#include "stack.h"


// support classes
#include "point.h"
#include "pointa.h"
#include "matrix.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__7CE086A4_3E78_11D0_AE6B_00C04FD7D06E__INCLUDED)

