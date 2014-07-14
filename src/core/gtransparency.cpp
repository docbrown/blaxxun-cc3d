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

#include "gclass.h"
#include "gshell.h"

#include "grender.h"

#include <gvtraverse.h>

#include "gtransparency.h"

#if 0
int GNode::Do(GglTransparencyRenderTraversal &state) // declared in GClass.h
{
   return Do(*((GTraversal *) &state));		
}
#endif



//
// class GglTransparencyRenderTraversal
//
RTIMP(GglTransparencyRenderTraversal,0,GGvglRenderTraversal);

void GglTransparencyRenderTraversal::InitializeAttributes()
{	
 	GGvglRenderTraversal::InitializeAttributes();
}

GglTransparencyRenderTraversal::~GglTransparencyRenderTraversal()
{
}


 // fully process the transparent elements, and clean list at end
int GglTransparencyRenderTraversal::RenderTransparentElements()
{
	return -1;
}


