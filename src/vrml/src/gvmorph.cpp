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

#include <gvmorph.h>


GV_NODE_SOURCE_IMP(GvMorph,GvGroup);

GvMorph::GvMorph()
{
    GV_NODE_CONSTRUCTOR(GvMorph);

    isBuiltIn = FALSE; // extension node

    GV_NODE_ADD_FIELD(type);
    
    GV_NODE_ADD_FIELD(local);
    GV_NODE_ADD_FIELD(transform);
    
    GV_NODE_ADD_FIELD(rotation);
    
    GV_NODE_ADD_FIELD(hermite);
    GV_NODE_ADD_FIELD(intersect);

    GV_NODE_ADD_FIELD(speed);
    GV_NODE_ADD_FIELD(on);

    GV_NODE_ADD_FIELD(cycle);
    GV_NODE_ADD_FIELD(t0);
    GV_NODE_ADD_FIELD(t1);

    GV_NODE_ADD_FIELD(rscale);
    GV_NODE_ADD_FIELD(nscale1);
    GV_NODE_ADD_FIELD(nscale2);

    GV_NODE_ADD_FIELD(wave_nwaves);
    GV_NODE_ADD_FIELD(wave_offset);
    GV_NODE_ADD_FIELD(wave_radius);

    GV_NODE_ADD_FIELD(swirl_nwaves);

    GV_NODE_ADD_FIELD(amplitude1);
    GV_NODE_ADD_FIELD(amplitude2);
    GV_NODE_ADD_FIELD(phaseShift1);
    GV_NODE_ADD_FIELD(phaseShift2);
    GV_NODE_ADD_FIELD(phaseScale1);
    GV_NODE_ADD_FIELD(phaseScale2);

	 GV_NODE_DEFINE_ENUM_VALUE(MorphType, NONE);
	 GV_NODE_DEFINE_ENUM_VALUE(MorphType, MELT);
	 GV_NODE_DEFINE_ENUM_VALUE(MorphType, SPHERE);
	 GV_NODE_DEFINE_ENUM_VALUE(MorphType, CYLINDER);
	 GV_NODE_DEFINE_ENUM_VALUE(MorphType, CONE);
	 GV_NODE_DEFINE_ENUM_VALUE(MorphType, BOX);
	 GV_NODE_DEFINE_ENUM_VALUE(MorphType, HANTEL);
	 GV_NODE_DEFINE_ENUM_VALUE(MorphType, WAVE);
	 GV_NODE_DEFINE_ENUM_VALUE(MorphType, WAVING);
	 GV_NODE_DEFINE_ENUM_VALUE(MorphType, SWIRL);

     GV_NODE_DEFINE_ENUM_VALUE(MorphType, PINCH);
	 GV_NODE_DEFINE_ENUM_VALUE(MorphType, TAPER);
	 GV_NODE_DEFINE_ENUM_VALUE(MorphType, TWIST);
	 GV_NODE_DEFINE_ENUM_VALUE(MorphType, BEND);

	 GV_NODE_DEFINE_ENUM_VALUE(MorphType, DNOISE);
    
    GV_NODE_SET_SF_ENUM_TYPE(type,MorphType);

	local.value = FALSE;

    rotation.value.set(0,1,0,0);

    transform.value  = NULL;

	speed.value = 1.0;
	on.value = TRUE;
	cycle.value = TRUE;
   
    t0.value = 0.0;
    t1.value = 1.0;

	init = 0;
	has_map_matrix = 0;
	ResetParams();


}
// Moved to GMorph
/*
GvMorph::~GvMorph()
{
}
*/

#endif _G_VRML1

