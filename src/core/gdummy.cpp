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

// GDdummy.cpp
#include "stdafx.h"
#include "gclass.h"


// Dummys for some traversal actions


int GNode::Do(GioTraversal &state) {return(0); }	// implemented in GioTraversal.cpp 
int GNode::Do(GPovIoTraversal &state) {return(0); }	// implemented in GioTraversal.cpp 
int GNode::Do(GBuildShellTraversal &state) {return(0); } // implemented in GBuildShell.cpp
int GNode::Do(GRaySelectTraversal &state) {return(0); } // implemented in GRaySelect.cpp
int GNode::Do(GLoadInlinesTraversal &state) {return(0); } // implemented in qvTraverse.cpp
int GNode::Do(GDisplayTreeTraversal &state) {return(0); }  // implemented in GDisplayTree.cpp


// Retreive current scene url or url home directory
const char* GTraversal::GetUrl() {return(NULL); }		// implemented in GvTraverse.cpp
const char* GTraversal::GetHomeUrl() {return(NULL); }	

#include "gshell.h"
int GShell::Do(GRaySelectTraversal &state) {return(0); } // implemented in GRaySelect.cpp
int GShell::IntersectFaces(GRaySelectTraversal &traversal,RayRange &ray, GHitInfo &info)
{return(0); } // declared in GShell.h implmented in GRaySelect

