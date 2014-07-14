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
#include "stdafx.h"
#include <gvfontstyle.h>

#ifdef _G_VRML1

GV_NODE_SOURCE_IMP(GvFontStyle,GvNode);

GvFontStyle::GvFontStyle()
{
    GV_NODE_CONSTRUCTOR(GvFontStyle);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(size);
    GV_NODE_ADD_FIELD(family);
    GV_NODE_ADD_FIELD(style);

    family.value = "";
    size.value = 1;
    style.value = NONE;

    GV_NODE_DEFINE_ENUM_VALUE(Style, NONE);
    GV_NODE_DEFINE_ENUM_VALUE(Style, BOLD);
    GV_NODE_DEFINE_ENUM_VALUE(Style, ITALIC);
    GV_NODE_DEFINE_ENUM_VALUE(Style, UNDERLINE);
    GV_NODE_DEFINE_ENUM_VALUE(Style, STRIKEOUT);

    GV_NODE_SET_SF_ENUM_TYPE(style, Style);
}

GvFontStyle::~GvFontStyle()
{
}
#endif _G_VRML1

//
// Gv2FontStyle
//

GV_NODE_SOURCE_IMP(Gv2FontStyle,GvNode);

Gv2FontStyle::Gv2FontStyle() : 
		family(1),
		horizontal(TRUE),
		justify(1),leftToRight(TRUE),
		size(1.0),spacing(1.0),
		topToBottom(TRUE)
{
    GV_NODE_CONSTRUCTOR(Gv2FontStyle);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(family);
    GV_NODE_ADD_FIELD(horizontal);
    GV_NODE_ADD_FIELD(justify);
    GV_NODE_ADD_FIELD(language);
    GV_NODE_ADD_FIELD(leftToRight);
    GV_NODE_ADD_FIELD(size);
    GV_NODE_ADD_FIELD(spacing);
    GV_NODE_ADD_FIELD(style);
    GV_NODE_ADD_FIELD(topToBottom);

    // family.value = "SERIF"; ISO
    family.values[0]= "SERIF";
    justify.values[0] = "BEGIN";
    language.value = "";
    style.value = "PLAIN";
}

Gv2FontStyle::~Gv2FontStyle()
{
}
