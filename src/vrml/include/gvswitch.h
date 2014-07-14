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
#ifndef  _GV_SWITCH_
#define  _GV_SWITCH_


#include <gvsfint32.h>
#include <gvmfnode.h>

#include <gvsubnode.h>


#define GV_SWITCH_NONE		(-1)	/* Don't traverse any children	*/
#define GV_SWITCH_ALL		(-3)	/* Traverse all children	*/

#ifdef _G_VRML1

#include <gvsflong.h>
#include <gvgroup.h>

class GvSwitch : public GvGroup {

    GV_NODE_HEADER(GvSwitch);

  public:
    // Fields
    GvSFLong		whichChild;	// Child to traverse


	int Do(GTraversal &state);   	


};
#endif _G_VRML1


class Gv2Switch : public GvNode {

    GV_NODE_HEADER(Gv2Switch);

  public:
    // Fields
    GvSFInt32		whichChoice;	// -1 Child to traverse
    GvMFNode        choice;

    //eval node to the actual value node of node,
    GvNode* Eval(GTraversal &state);

    // call by subfield on touch
	int OnFieldChanged(GvField *field);



	int Do(GTraversal &state);   	
	int Do(GPathTraversal &state);

	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp




};

#endif /* _GV_SWITCH_ */
