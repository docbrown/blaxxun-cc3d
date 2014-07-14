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
#ifndef _GFont_h
#define _Gfont_h
/******************************************************************************
@doc

@module GFont.h - GLView TrueType font support	|

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

$Revision: 1.5 $
$Log: gfont.h,v $
Revision 1.5  1999/07/06 16:54:27  tom
*** empty log message ***

Revision 1.4  1999/03/10 10:22:07  tom
*** empty log message ***



Todo :

******************************************************************************/

class GShell;


#ifdef WIN32

float ComputeGlyphLineHeight();


/*
 @func compute a polygonal outline for string text
 from a true tpye font

*/
int ComputeGlyphOutline(
	HDC hDC,			// a Windows DC with a TrueType font selected
 	
	const  _TCHAR *text, // the input text, to compute a polygon outline for

 	float &x, float &y, // starting x,y position, will be updated after the call to new position
	float spacing, // 0.0 extra character spacing
	float height,	// = 1.0,

	float dirstepx, // 1.0
	float dirstepy,// -1.0

	float lineSpacing,	// = 1.0,
	int align, 
	float length,		// 0.0  targeted string length
 	gbool duplicateHardPts,	// if true all vertices of line segments will be duplicated, to create hard edges for 3d extrsuion
 	GShell &s,		// the output <c GShell> ("2d"), will contain faces with holes
	float &resultMaxLength	// max length of the line 
	);


#endif

#endif
