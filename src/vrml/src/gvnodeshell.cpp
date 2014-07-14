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

#include <gvnode.h>

#include <gvnodes.h>
#include "gshell.h"

#include <gvnodeshell.h>

GV_NODE_SOURCE_IMP(GvNodeShell,GvNode);

GvNodeShell::GvNodeShell() : theShapes(0)
{
    GV_NODE_CONSTRUCTOR(GvNodeShell);
    isBuiltIn = TRUE; // ??

	theShell = NULL;
}

GvNodeShell::~GvNodeShell()
{
   if (theShell) theShell->unref();
   theShell = NULL;
}

int GvNodeShell::BuildShell(GTraversal &state)
{
  return(0);
}
	
void GvNodeShell::SetShell(GShell *newShell)
{
   if (theShell != newShell) {
      if (theShell) theShell->unref();
      theShell = newShell;
      if (theShell) theShell->ref();
   }
   if (theShell) {


#if 0 // for now _D3D	
	gbool computeSubMeshes = (theShell->f.Length() < theShell->v.Length())
		||  (theShell->v.Length() > GShell::maxSubMeshVertices);


	if (computeSubMeshes && theShell->subMeshes == NULL)
		theShell->CreateSubMeshes();
#endif
	
	   
	   
	   
	   theShell->Update(); // ??? for bounding Box 

   }
}

// is p outside the object
// may fall back to very simply outside bbox computation
// needed for Occlusion node 
GvBool GvNodeShell::isOutside(const Point &p) const
{

//   if (!theShell) BuildShell();
   if (!theShell) return FALSE;
   return theShell->isOutside(p);
}

// is the a planar object ?
GvBool GvNodeShell::isPlanar() const
{
   Plane p;
   if (!theShell) return FALSE;
 
   // text is planar !! 
   return theShell->isPlanar(p)==1;

}

// is this a solid  i.e. closed ?
GvBool GvNodeShell::isSolid() const
{
    if (!theShell) return FALSE;
    return theShell->GetFlag(SHELL_CLOSED);
}


// is this a convex object ?
GvBool GvNodeShell::isConvex() const
{
    if (!theShell) return theShell->isConvex();
	return FALSE;
}



int GvNodeShell::OnFieldChanged(GvField *field)
{
 	SetShell(NULL);
	setRedraw();  // Any change will trigger redraw / and rebuilding of shell
	return GvNode::OnFieldChanged(field);
}
