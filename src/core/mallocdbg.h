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
// MALLOC macros replaces malloc calloc free, for debugging

#if   defined(WIN32) && defined(_DEBUG)

// MFC diagnostics

// for MFC only #define new DEBUG_NEW

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
