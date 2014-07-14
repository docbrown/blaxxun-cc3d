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
@doc

@module GFindNode.cpp - GLView node finding  traversal routines	|

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:

Classes:
GFindNodeTraversal

Notes:

Changes:

Todo :

******************************************************************************/

#include "stdafx.h"

#include "gclass.h"
#include "gshell.h"
#include "gshelli.h"
#include "ggeom.h"
#include "gcamera.h"
#include <gvfields.h>


#include "gfindnode.h"




//
// class GFindNodeTraversal
//
RTIMP(GFindNodeTraversal,0,GPathTraversal);

void GFindNodeTraversal::InitializeAttributes()
{	
 	GPathTraversal::InitializeAttributes();
}

GFindNodeTraversal::~GFindNodeTraversal()
{
}

int GFindNodeTraversal::Do(GNode *n) 
{	
	
	if (n == nodeToFind) {
		int ret;
		GFindNodeTraversal &state = *this;
		GPUSH_PATH(state,n);
			ret = Found(n);
		GPOP_PATH(state);
		stop = 1;
		return(ret);

	}
	return (n->Do(*this)); 

}


int GFindNodeTraversal::Found(GNode *node) 
{	
	
	TRACE("Found : %s \n",node->ClassName());
	found = TRUE;
	foundPath.SetByList(topPath);
	return 0;
}

