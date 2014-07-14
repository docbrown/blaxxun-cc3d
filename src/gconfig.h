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
GConfig.h		-  config info

$Revision: 1.5 $
$Log: gconfig.h,v $
Revision 1.5  1999/05/05 14:37:43  tom
empty message

Revision 1.1  1999/03/22 16:43:27  kristof
Initial revision



******************************************************************************/



//#define _GLVIEW_OCX
//#define _GLVIEW_IE // for Internet explorer
//#define _GLVIEW_DLL

#define _COM
#define _GV
#define _VRML
#define _WWW


#define _G_VRML1		// VRML 1.0 Nodes
#define _G_INVENTOR		// Inventor VRML 1.0 style nodes
#define _G_VRML2		// VRML 2.0 Nodes
#define _G_MPEG4		// additional nodes from MPEG4



// GLView light 
#ifdef _GLVIEW_LE

	#define _3DV
	#undef _DXF
	#undef _RAW
	#undef _GEO
	#undef _HMF
	#undef _OBJ
	#undef _POV
	#undef _STL
	#undef _RGB

#elif defined(_GLVIEW_IE)

	#undef _3DV
	#undef _DXF
	#undef _RAW
	#undef _GEO
	#undef _HMF
	#undef _OBJ
	#undef _POV
	#undef _STL

	#define _RGB
	#define _GV_MINIMAL

#else

	#define _DXF
	#define _RAW
	#define _GEO
	#define _OBJ
	#define _STL
	#define _POV
	#define _RGB
	#define _3DV

#endif





#ifdef WIN32

// sound
// RSX 
#define G_RSX

// Direct Sound
#define G_DSOUND

// Active Movie 
#define G_AMOVIE


// extra CC3D cache stuff
#define G_CACHE

// trimmed NURBS
#define G_T_NURBS

// the navigation panel
//#define G_PANEL


#endif WIN32


// image 
#define _IMAGE
#define _RGB
#define _JPEG
#define _PNG
#define _TARGA
#define _GIF
#define _DIB
#define _PPM

#define _PCX
#undef _PCX

