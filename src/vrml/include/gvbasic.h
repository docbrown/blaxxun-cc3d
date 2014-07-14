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
#ifndef  _GV_BASIC_
#define  _GV_BASIC_

/******************************************************************************
@doc

@module GvBasic.h - basic VRML types|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:
parts based on QvLib from SGI

Changes:


Todo :

******************************************************************************/


#ifdef WIN32
typedef unsigned long u_long;
#define M_PI 3.14159265358979323846
#define M_PI_4 (M_PI/4.0)

// HG
// #pragma warning(disable : 4244)     // X86	VC2.0 const double to float

#endif

#include <sys/types.h>
#ifndef WIN32
//#include <libc.h>
#else
#include <stdlib.h>
#endif /* WIN32 */
#include <stdio.h>

#ifndef FALSE
#   define FALSE	0
#   define TRUE		1
#endif

//typedef int GvBool;

typedef gbool	GvBool; // use core boolean type
typedef gfloat	GvFloat;

//typedef double	GvTime;


// implement some common extensions
#define GV_EXTENSIONS


// This uses the preprocessor to quote a string
#if defined(__STDC__) || defined(__ANSI_CPP__)		/* ANSI C */
#  define GV__QUOTE(str)	#str
#else							/* Non-ANSI C */
#ifdef WIN32
#  define GV__QUOTE(str)	#str
#else
#  define GV__QUOTE(str)	"str"
#endif
#endif

// This uses the preprocessor to concatenate two strings
#if defined(__STDC__) || defined(__ANSI_CPP__)		/* ANSI C */
#   define GV__CONCAT(str1, str2)	str1##str2
#else							/* Non-ANSI C */
#   define GV__CONCAT(str1, str2)	str1/**/str2
#endif

#endif /* _GV_BASIC_ */

