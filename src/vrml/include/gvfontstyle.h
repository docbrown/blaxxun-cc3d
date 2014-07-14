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
#ifndef  _GvFontStyle_
#define  _GvFontStyle_

#include <gvsfenum.h>
#include <gvsfbitmask.h>
#include <gvsffloat.h>
#include <gvsfstring.h>
#include <gvsubnode.h>

#include <gvsfbool.h>
#include <gvmfstring.h>

#ifdef _G_VRML1


class GvFontStyle : public GvNode 
{

    GV_NODE_HEADER(GvFontStyle);

  public:

    enum Style {
	NONE	= 0x0,
	BOLD	= 0x1,
	ITALIC	= 0x2,	
	UNDERLINE	= 0x4,	
	STRIKEOUT	= 0x8	
    };

    // Fields
    GvSFFloat		size;
	GvSFString		family;
    GvSFBitMask		style;	


	int Do(GTraversal &state);

};

#endif _G_VRML1

/*

The family field specifies a case-sensitive SFString value that may be "SERIF" (the default)
for a serif font such as Times Roman; "SANS" for a sans-serif font such as Helvetica; or
"TYPEWRITER" for a fixed-pitch font such as Courier. A family value of empty quotes,
"", is identical to "SERIF".

The style field specifies a case-sensitive SFString value that may be "PLAIN" (the default)
for default plain type; "BOLD" for boldface type; "ITALIC" for italic type; or
"BOLDITALIC" for bold and italic type. A style value of empty quotes, "", is identical to
"PLAIN".

*/

class Gv2FontStyle : public GvNode 
{

    GV_NODE_HEADER(Gv2FontStyle);

  public:

    // Fields

	//GvSFString family pre iso;       //Default "SERIF"
	GvMFString family;       //Default "SERIF"
	GvSFBool   horizontal;   //Default TRUE
	GvMFString justify;      //Default "BEGIN"
	GvSFString language;     //Default ""
	GvSFBool   leftToRight;  //Default TRUE
	GvSFFloat  size;         //Default 1.0
	GvSFFloat  spacing;      //Default 1.0
	GvSFString style;        //Default "PLAIN"
	GvSFBool   topToBottom;  //Default TRUE

};

#endif /* _GvFontStyle_ */
