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

#include <gvswitch.h>

#ifdef _G_VRML1


GV_NODE_SOURCE_IMP(GvSwitch,GvGroup);

GvSwitch::GvSwitch()
{
    GV_NODE_CONSTRUCTOR(GvSwitch);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(whichChild);

    whichChild.value = GV_SWITCH_NONE;
}

GvSwitch::~GvSwitch()
{
}

#endif _G_VRML1

//
// Gv2Switch
//

GV_NODE_SOURCE_IMP(Gv2Switch,GvNode);

Gv2Switch::Gv2Switch() : whichChoice(GV_SWITCH_NONE),choice(0)
{
    GV_NODE_CONSTRUCTOR(Gv2Switch);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(whichChoice);
    GV_NODE_ADD_FIELD(choice);

}

Gv2Switch::~Gv2Switch()
{
}

// set redraw bit if a field changed 
int Gv2Switch::OnFieldChanged(GvField *field)
{
	setRedraw();
	return GvNode::OnFieldChanged(field);
}



int Gv2Switch::Do(GTraversal &state)
{   

	if (state.traverseAll) { return(choice.Traverse(state)); }

    int which = whichChoice.value;

    if (which == GV_SWITCH_NONE)
	    return(0);
    else if (which == GV_SWITCH_ALL) // extension 
        return(choice.Traverse(state));
    else if (which <0) {
        return(0); 
    }
	else if (which < choice.Length() && ((GvNode*)choice.values[which] !=NULL))
	    return choice.values[which]->Traverse(state);
    return(0);
}

int Gv2Switch::Do(GPathTraversal &state)
{   
	int ret = 0;

    int which = whichChoice.value;


	GPUSH_PATH(state,this);
		if (state.traverseAll || which == GV_SWITCH_ALL) // extension 
			ret = choice.Traverse(state);
		else {

			if (which == GV_SWITCH_NONE)
				ret=0;
			else if (which <0) {
				ret = 0; 
			}
			else if (which < choice.Length() && ((GvNode*)choice.values[which] !=NULL)) {
				GSET_PATH_FIELD(state,choice);
				GSET_PATH_INDEX(state,which);
				ret = choice.values[which]->Traverse(state);
			}

		}
	GPOP_PATH(state);

    return(ret);
}

//eval node to the actual value node of node,
GvNode* Gv2Switch::Eval(GTraversal &state)
{
    int which = whichChoice.value;

    if (which == GV_SWITCH_NONE)
	    return(NULL);
    else if (which == GV_SWITCH_ALL) // extension 
        return(this);
    else if (which <0) {
        return(NULL); 
    }
	else if (which < choice.Length())
	    return choice.values[which];
    return(NULL);
}


