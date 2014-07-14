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

#include <gvblinker.h>

GV_NODE_SOURCE_IMP(GvBlinker,GvSwitch);

GvBlinker::GvBlinker()
{
    GV_NODE_CONSTRUCTOR(GvBlinker);
    isBuiltIn = TRUE;

    isBuiltIn = FALSE; // extension node

    GV_NODE_ADD_FIELD(whichChild);

    whichChild.value = GV_SWITCH_NONE;

    GV_NODE_ADD_FIELD(speed); speed.value = 1.0;
    GV_NODE_ADD_FIELD(offset); offset.value = 0.0;
    GV_NODE_ADD_FIELD(on); on.value = TRUE;
    GV_NODE_ADD_FIELD(upto); upto.value = FALSE;
    GV_NODE_ADD_FIELD(times); times.values[0] = -1.0;


}

GvBlinker::~GvBlinker()
{
}
#endif _G_VRML1

