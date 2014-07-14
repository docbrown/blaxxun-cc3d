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

#ifdef _G_VRML1

#include <gvcomplexity.h>

GV_NODE_SOURCE_IMP(GvComplexity,GvNode);

GvComplexity::GvComplexity()
{
    GV_NODE_CONSTRUCTOR(GvComplexity);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(type);
    GV_NODE_ADD_FIELD(value);
    GV_NODE_ADD_FIELD(textureQuality);

    type.value = OBJECT_SPACE;
    value.value = 0.5;
    textureQuality.value = 0.5;

    GV_NODE_DEFINE_ENUM_VALUE(Complexity, SCREEN_SPACE);
    GV_NODE_DEFINE_ENUM_VALUE(Complexity, OBJECT_SPACE);
    GV_NODE_DEFINE_ENUM_VALUE(Complexity, BOUNDING_BOX);

    GV_NODE_SET_SF_ENUM_TYPE(type, Complexity);
}

GvComplexity::~GvComplexity()
{
}
#endif _G_VRML1

