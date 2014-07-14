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
#ifndef  _GVInterpolators_
#define  _GVInterpolators_

#include <gvsubnode.h>
#include <gvnode.h>


#include <gvsffloat.h>
#include <gvmffloat.h>

#include <gvsfcolor.h>
#include <gvmfcolor.h>

#include <gvsfvec2f.h>
#include <gvmfvec2f.h>

#include <gvsfvec3f.h>
#include <gvmfvec3f.h>

#include <gvsfrotation.h>
#include <gvmfrotation.h>




#define FRAC_EPS 1E-8

int findKey(float t,int n, float *k,int &i1, int &i2, float &f);
// map f 0..1 linear to keys i1 and i2 
void findKey(float f,int nVals, int &i1, int &i2, float &frac);

//
// GvInterpolator - ROOT for VRML 2. Interpolator nodes
// 

class GvInterpolator : public GvNode {

    GV_NODE_HEADER(GvInterpolator);

public:

    // Fields
    GvMFFloat		key;		// [] keyframe Times

    //Event IN
    GvSFFloat fraction;

    int evaluating;		// we are currently in OnEvaluate
    
    // this interpolator is currently evaluating
    GvBool Evaluating() { return(evaluating >0); }
    
    // Evaluate interpolator output
    virtual int OnEvaluate(float f);

	// check for fraction_changed eventIn
	int OnFieldChanged(GvField *field);

    // something changed , do nothing
	int OnChanged();

	// find key, 0-1 do nothing 0 use i1, >0 use i2 && i2 + frac

	int EvaluateKey(float f, int nVals, int &i1, int &i2, float &frac) const 
	{   
	// int nVals = keyValue.Length();

    if (nVals <=0) return(-1);

    int nKeys = key.Length();

    if (nVals == 1) {
        i1=0;
        return(0);
    }

    if (nKeys > 1) {
        findKey(f,nKeys, key.values,i1,i2, frac);
    } else {   // no key frames, map linear 
        findKey(f,nVals,i1,i2,frac);
    }
    if (frac < FRAC_EPS) return(0); // use i1 
    else  {
	    return(1);
    }
	}

	// do nothing 
	int Do(GTraversal &state);
    int Do(GglRenderTraversal &state);



};




class GvCoordinateInterpolator : public GvInterpolator{

    GV_NODE_HEADER(GvCoordinateInterpolator);

  public:
    // Fields
    GvMFVec3f		keyValue;		// [] keyframe values

    // Event OUT 
    GvMFVec3f       value;


    // Event IN
    // set_fraction SFFloat

    int OnEvaluate(float f);

};

class GvPositionInterpolator : public GvInterpolator{

    GV_NODE_HEADER(GvPositionInterpolator);

  public:
    // Fields
    GvMFVec3f		keyValue;		// [] keyframe values

    // Event OUT   
    GvSFVec3f  value;


    // Event IN
    // set_fraction SFFloat

    int OnEvaluate(float f);

};


class GvNormalInterpolator : public GvInterpolator{

    GV_NODE_HEADER(GvNormalInterpolator);

  public:

    // Fields
    GvMFVec3f		keyValue;		// [] keyframe values

    // Event OUT 
    GvMFVec3f  value; // normalized vector   

    
    // Event IN
    // set_fraction SFFloat

    int OnEvaluate(float f);

};


class GvCoordinate2Interpolator : public GvInterpolator{

    GV_NODE_HEADER(GvCoordinate2Interpolator);

  public:

    // Fields
    GvMFVec2f		keyValue;		// [] keyframe values

    // Event OUT 
    GvMFVec2f  value;

    // Event IN
    // set_fraction SFFloat
  
    int OnEvaluate(float f);

};


class GvColorInterpolator : public GvInterpolator {

    GV_NODE_HEADER(GvColorInterpolator);

  public:

     // Fields
    GvMFColor		keyValue;		// [] keyframe values
    
    // Event OUT  
    GvSFColor  value;
    // no MFColor out ??

    // Event IN
    // set_fraction SFFloat


    int OnEvaluate(float f);
};


class GvOrientationInterpolator : public GvInterpolator {

    GV_NODE_HEADER(GvOrientationInterpolator);

  public:

     // Fields
    GvMFRotation		keyValue;		// [] keyframe values
    
    // Event OUT  
    GvSFRotation  value;
    
    // no MFOrientation out ??

    // Event IN
    // set_fraction SFFloat


    int OnEvaluate(float f);
};



class GvScalarInterpolator : public GvInterpolator{

    GV_NODE_HEADER(GvScalarInterpolator);

  public:

    // Fields
    GvMFFloat		keyValue;		// [] keyframe values

    // Event OUT   
    GvSFFloat  value;

    // Event IN
    // set_fraction SFFloat


    int OnEvaluate(float f);
};



#endif /* _GVInterpolators_ */
