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

#include <gvmaterialbinding.h>

GV_NODE_SOURCE_IMP(GvMaterialBinding,GvNode);


GvMaterialBinding::GvMaterialBinding()
{
    GV_NODE_CONSTRUCTOR(GvMaterialBinding);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(value);

    value.value = DEFAULT;

    GV_NODE_DEFINE_ENUM_VALUE(Binding, DEFAULT);
    GV_NODE_DEFINE_ENUM_VALUE(Binding, NONE);
    GV_NODE_DEFINE_ENUM_VALUE(Binding, OVERALL);
    GV_NODE_DEFINE_ENUM_VALUE(Binding, PER_PART);
    GV_NODE_DEFINE_ENUM_VALUE(Binding, PER_PART_INDEXED);
    GV_NODE_DEFINE_ENUM_VALUE(Binding, PER_FACE);
    GV_NODE_DEFINE_ENUM_VALUE(Binding, PER_FACE_INDEXED);
    GV_NODE_DEFINE_ENUM_VALUE(Binding, PER_VERTEX);
    GV_NODE_DEFINE_ENUM_VALUE(Binding, PER_VERTEX_INDEXED);

    GV_NODE_SET_SF_ENUM_TYPE(value, Binding);
}


GvMaterialBinding::~GvMaterialBinding()
{
}
#endif _G_VRML1

