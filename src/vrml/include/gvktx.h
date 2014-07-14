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
#ifndef  _GvKtx_
#define  _GvKtx_


#ifdef _G_VRML1


#include <gvsfenum.h>
#include <gvsfbitmask.h>
#include <gvsffloat.h>
#include <gvsfvec2f.h>

#include <gvsfstring.h>
#include <gvmfstring.h>
#include <gvmffloat.h>
#include <gvmfvec3f.h>
#include <gvmfnode.h>
#include <gvsubnode.h>
#include <gvnodeshell.h>

#include <gvmodifier.h>



// www.ktx.com 3dstudio extension nodes

class GvAnimationFrames_ktx_com : public GvNode 
{

    GV_NODE_HEADER(GvAnimationFrames_ktx_com);
public:


        GvSFLong length;        // -1 
        GvSFLong segmentStart;  // -1 
        GvSFLong segmentEnd;    // -1 
        GvSFLong current;       // -1 
        GvSFFloat rate;         // 30.0

};


class GvAnimationStyle_ktx_com : public GvNode 
{

    GV_NODE_HEADER(GvAnimationStyle_ktx_com);
public:

    enum SplineUse {  // is this ok ?????????????????????
   	NONE = 0,
    CONTINUITY = 1,
   	BIAS = 2,
   	EASETO = 4,
   	EASEFROM = 8,
	TENSION = 16,
    ALL = 1 + 2 + 4 + 8 + 16
    };



        GvSFBool loop;  // FALSE
        GvSFBitMask splineUse;  // () 
        GvSFFloat tension;  // 0
        GvSFFloat continuity; // 0
        GvSFFloat bias; // 0
        GvSFFloat easeTo; // 0

        GvSFFloat easeFrom; // 0
        GvSFVec3f pivotOffset; // 0 0 0

};

class GvPositionKey_ktx_com : public GvNode 
{

    GV_NODE_HEADER(GvPositionKey_ktx_com);

public:

    
        GvSFLong frame; // 0
        GvSFVec3f translation; // 0 0 0


};

class GvRotationKey_ktx_com : public GvNode 
{

    GV_NODE_HEADER(GvRotationKey_ktx_com);

public:

    
        GvSFLong frame; // 0
        GvSFRotation rotation; // 0 0 1 0


};

class GvHideKey_ktx_com : public GvNode 
{

    GV_NODE_HEADER(GvHideKey_ktx_com);

public:

    
        GvSFLong frame; // 0


};

class GvScaleKey_ktx_com : public GvNode 
{

    GV_NODE_HEADER(GvScaleKey_ktx_com);

public:

    
        GvSFLong frame; // 0
        GvSFVec3f scale; // 1 1 1


};


class GvColorKey_ktx_com : public GvNode 
{

    GV_NODE_HEADER(GvColorKey_ktx_com);

public:

    
        GvSFLong frame; // 0
        GvSFColor color; // 1 1 1


};

class GvAimTarget_ktx_com : public GvNode 
{

    GV_NODE_HEADER(GvAimTarget_ktx_com);

public:

    
        GvSFString aimer; // ""


};


// proximity nodes ....................

//
//
class GvReferenceMark_ktx_com : public GvSeparator 
{

    GV_NODE_HEADER(GvReferenceMark_ktx_com);

public:
    
        GvSFString description; // ""
};


#endif

#endif /* _GvKtx_ */
