//  -*- C++ -*-

/*
 * Copyright (C) 1996   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.2 $
 |
 |   Description:
 |	This file contains basic Vb library definitions.
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson
 |       (for the PC)   : Jim Kent, Daniel Woods
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _VB_BASIC_
#define  _VB_BASIC_

#include "system.h"

/*
 * Types are such a pain in the butt.
 */
#ifdef WIN32
        typedef unsigned long u_long;
		typedef unsigned long uint32_t;
        typedef long          int32_t;

	#ifndef M_PI
        #define M_PI 3.1415926536
	#endif

        #define M_PI_4 (M_PI/4.0)
		#define M_SQRT2 1.41421356237309504880

#else
	typedef unsigned long uint32_t;
#endif

#ifndef FALSE
#   define FALSE	0
#   define TRUE		1
#endif

typedef int	VbBool;

// This uses the preprocessor to quote a string
#if defined(__STDC__) || defined(__ANSI_CPP__)          /* ANSI C */
#  define VB__QUOTE(str)        #str
#else                                                   /* Non-ANSI C */
#ifdef WIN32
#  define VB__QUOTE(str)        #str
#else
#  define VB__QUOTE(str)        "str"
#endif
#endif

// This uses the preprocessor to concatenate two strings
#if defined(__STDC__) || defined(__ANSI_CPP__)          /* ANSI C */
#   define VB__CONCAT(str1, str2)       str1##str2
#else                                                   /* Non-ANSI C */
#   define VB__CONCAT(str1, str2)       str1/**/str2
#endif


#endif /* _VB_BASIC_ */
