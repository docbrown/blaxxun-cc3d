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
/******************************************************************************
@doc

@module GvInterpolators.cpp - Implementation of Interpolator Nodes|

Copyright (c) 1996 - 1998	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:


Changes:

$Revision: 1.10 $
$Log: gvinterpolators.cpp,v $
Revision 1.10  1999/07/06 16:45:48  tom
*** empty log message ***

Revision 1.9  1999/05/10 13:54:56  tom
*** empty log message ***



Todo :

******************************************************************************/


#include <gvinterpolators.h>



/*
Interpolators nodes are designed for linear keyframed animation. That is, an interpolator node defines a piecewise linear
function, f(t), on the interval (-infinity, infinity). The piecewise linear function is defined by n values of t, called key, and the n
corresponding values of f(t), called keyValue. The keys must be monotonic non-decreasing and are not restricted to any
interval. An interpolator node evaluates f(t) given any value of t (via the set_fraction eventIn). 

Let the n keys k0, k1, k2, ..., k(n-1) partition the domain (-infinity, infinity) into the n+1 subintervals given by (-infinity, k0),
[k0, k1), [k1, k2), ... , [k(n-1), infinity). Also, let the n values v0, v1, v2, ..., v(n-1) be the values of an unknown function,
F(t), at the associated key values. That is, vj = F(kj). The piecewise linear interpolating function, f(t), is defined to be

     f(t) = v0,     if t < k0,
          = v(n-1), if t > k(n-1),
          = vi,     if t = ki for some value of i, where -1<i<n,
          = linterp(t, vj, v(j+1)), if kj < t < k(j+1),

where linterp(t,x,y) is the linear interpolant, and -1< j < n-1. The third conditional value of f(t) allows the defining of multiple
values for a single key, i.e. limits from both the left and right at a discontinuity in f(t).The first specified value will be used as the
limit of f(t) from the left, and the last specified value will be used as the limit of f(t) from the right. The value of f(t) at a multiply
defined key is indeterminate, but should be one of the associated limit values. 
*/
#if 1

// binary search 

int findKey(float t,int n, float *k,int &i1, int &i2, float &f)
{

    if (t<=k[0]) {
        i1=0;
        i2=0;
        f = 0.0;
        return(0);
    }
    if (t >= k[n-1] || (n == 1) ) {
        i1 = n-1;
        i2 = n-1;
        f = 0.0;
        return(2);
    }

	int l = 0; // left
	int r = n-1;  // right 

    int i; // mid element

	// binary search  terminate if interval below 1
	while ((l+1) < r) {

		i = (l+r) / 2;

		if (t < k[i]) {
			r = i;
			continue;
		}
		else if (t > k[i]) {
			l = i;
			continue;
		}
		else {
			i1 = i;
			i2 = i;
			f = 0.0;
			return(2);
		}
	}
   
    i1=l;
    i2=l+1;

    float d = k[i2] - k[i1];
    if (d > 0.0)  f = (t - k[i1]) / d;
    else f = 0.0;

    return(1);
}


#else
// linear search 
int findKey(float t,int n, float *k,int &i1, int &i2, float &f)
{
    if (k == NULL) {
    }
    int i=0;
    if (t<=k[0]) {
        i1=0;
        i2=0;
        f = 0.0;
        return(0);
    }
    while (i<n && (t >= k[i])) {
        i++;
    }
    if (i >= n ) {
        i1 = n-1;
        i2 = n-1;
        f = 0.0;
        return(2);
    }
   
    i1=i-1;
    i2=i;

    float d = k[i2] - k[i1];
    if (d> 0.0)  f = (t - k[i1]) / d;
    else f = 0.0;

    return(1);
}
#endif

// map f 0..1 linear to keys i1 and i2 
void findKey(float f,int nVals, int &i1, int &i2, float &frac)
{
            // no key frames, map linear 
            f= f * (nVals-1);
            if (f<0.0f) f = -f ;

            frac = fmod(f,1.0f);
            i1 = (int) f;
            if (i1 >= (nVals -1)) {
                i1 = i2 = nVals -1;
                frac = 0.0;
            } else {
                i2 = i1+1;
            }
}

GV_NODE_SOURCE_IMP(GvInterpolator,GvNode);

GvInterpolator::GvInterpolator() : key(0)
{
    GV_NODE_CONSTRUCTOR(GvInterpolator);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(key);

    //  event in
    GV_NODE_ADD_FIELD_EVENT_IN(fraction);

    evaluating = 0; 

    
}



GvInterpolator::~GvInterpolator()
{
}


// assume f from 0 to 1 
int GvInterpolator::OnEvaluate(float f)
{
    evaluating++;
    TRACE("%s::OnEvaluate not implemented \n",ClassName());
    evaluating--;
    return(0);
}



// check for fraction_changed eventIn
// outValue will be changed, beware of infinite loop 
int GvInterpolator::OnFieldChanged(GvField *field)
{
	if (field == &fraction) { // its the set_fraction eventIn
	    if (!Evaluating()) {
		    return(OnEvaluate(fraction.value));
		}
		else return 0;
	}
    return(0); // early exit, do we ever want to notify owners ?
	// return GvNode::OnFieldChanged(field); // ??
}

// something changed 
// outValue will be changed, beware of infinite loop 

int GvInterpolator::OnChanged()
{
    return(0); // early exit, do we ever want to notify owners ?

    if (!Evaluating()) {

		/// if reevaluation for each change : return(OnEvaluate(fraction.value));

    }
	return GvNode::OnChanged();
}

// do nothing 
int GvInterpolator::Do(GTraversal &state)
{
  return(0);	
}

int GvInterpolator::Do(GglRenderTraversal &state)
{
  return(0);	
}

//
// GvColorInterpolator
//

GV_NODE_SOURCE_IMP(GvColorInterpolator,GvInterpolator);

GvColorInterpolator::GvColorInterpolator() : keyValue(0)
{
    GV_NODE_CONSTRUCTOR(GvColorInterpolator);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(key);
    GV_NODE_ADD_FIELD(keyValue);
    GV_NODE_ADD_FIELD_EVENT_IN(fraction);
    GV_NODE_ADD_EVENT_OUT(value);
}



GvColorInterpolator::~GvColorInterpolator()
{
}

int GvColorInterpolator::OnEvaluate(float f)
{   
	int nVals = keyValue.Length();
    if (nVals <=0) return(0);

    int i1,i2;
    float frac;

    evaluating++;
/*
	if ( (f> 1.0f) || (f <0.01)) {
			TRACE("GvColorInterpolator::Extrem fraction %g \n",f);

	}
*/

	if (EvaluateKey(f,nVals,i1,i2, frac) <= 0) {
		value.set(keyValue[i1]);
	}
	else {
        keyValue.InterpolateHsv(value,keyValue[i1],1.0-frac,keyValue[i2],frac);
        value.touch();
	}
    evaluating--;
	return(1);
}






//
// GvCoordinateInterpolator
//
GV_NODE_SOURCE_IMP(GvCoordinateInterpolator,GvInterpolator);

GvCoordinateInterpolator::GvCoordinateInterpolator() : keyValue(0)
{
    GV_NODE_CONSTRUCTOR(GvCoordinateInterpolator);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(key);
    GV_NODE_ADD_FIELD(keyValue);
    GV_NODE_ADD_FIELD_EVENT_IN(fraction);
    
    GV_NODE_ADD_EVENT_OUT(value);
    
}



GvCoordinateInterpolator::~GvCoordinateInterpolator()
{
}

// assume f from 0 to 1 
int GvCoordinateInterpolator::OnEvaluate(float f)
{   int nVals = keyValue.Length();
    if (nVals <=0) return(0);
    evaluating++;

    int nKeys = key.Length();
    int i1,i2;
    float frac;
    int multiple = 0;

    if (nVals == 1) {
        value.set(1,keyValue[0]);
        value.touch();
        return(0);
    }

    if (nKeys > 1) {
        findKey(f,nKeys, key.values,i1,i2, frac);
        multiple =  nVals / nKeys;
    } else {
        // no key frames, map linear 
        findKey(f,nVals,i1,i2,frac);
        multiple = 1;
    }

    //TRACE("Coordinate interpolater f=%f i1 %d i2 %d frac = %f  m=%d \n",f,i1,i2,frac,multiple);

    value.makeRoom(multiple);
    i1*=multiple;
    i2*=multiple;
    ASSERT(i1 <= (nVals-multiple));
    ASSERT(i2 <= (nVals-multiple));
    keyValue.Interpolate(value,keyValue[i1],1.0-frac,keyValue[i2],frac,multiple);
    value.touch();
        
    evaluating--;

    return(1);

}

//
// GvCoordinateInterpolator
//
GV_NODE_SOURCE_IMP(GvCoordinate2Interpolator,GvInterpolator);

GvCoordinate2Interpolator::GvCoordinate2Interpolator() : keyValue(0)
{
    GV_NODE_CONSTRUCTOR(GvCoordinate2Interpolator);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(key);
    GV_NODE_ADD_FIELD(keyValue);
    GV_NODE_ADD_FIELD_EVENT_IN(fraction);
    
    GV_NODE_ADD_EVENT_OUT(value);
    
}



GvCoordinate2Interpolator::~GvCoordinate2Interpolator()
{
}

// assume f from 0 to 1 
int GvCoordinate2Interpolator::OnEvaluate(float f)
{   int nVals = keyValue.Length();
    if (nVals <=0) return(0);
    evaluating++;

    int nKeys = key.Length();
    int i1,i2;
    float frac;
    int multiple = 0;

    if (nVals == 1) {
        value.set(1,keyValue[0]);
        value.touch();
        evaluating--;
        return(0);
    }

    if (nKeys > 1) {
        findKey(f,nKeys, key.values,i1,i2, frac);
        multiple =  nVals / nKeys;
    } else {
        // no key frames, map linear 
        findKey(f,nVals,i1,i2,frac);
        multiple = 1;
    }

    //TRACE("Coordinate interpolater f=%f i1 %d i2 %d frac = %f  m=%d \n",f,i1,i2,frac,multiple);

        value.makeRoom(multiple);
        i1*=multiple;
        i2*=multiple;
        ASSERT(i1 <= (nVals-multiple));
        ASSERT(i2 <= (nVals-multiple));
        keyValue.Interpolate(value,keyValue[i1],1.0-frac,keyValue[i2],frac,multiple);
        value.touch();
        
    evaluating--;
  
    return(1);

}

//
// GvPositionInterpolator
//

GV_NODE_SOURCE_IMP(GvPositionInterpolator,GvInterpolator);

GvPositionInterpolator::GvPositionInterpolator() : keyValue(0)
{
    GV_NODE_CONSTRUCTOR(GvPositionInterpolator);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(key);
    GV_NODE_ADD_FIELD(keyValue);
    GV_NODE_ADD_FIELD_EVENT_IN(fraction);
    
    GV_NODE_ADD_EVENT_OUT(value);
}



GvPositionInterpolator::~GvPositionInterpolator()
{
}

#if 0
int GvPositionInterpolator::OnEvaluate(float f)
{   int nVals = keyValue.Length();

    if (nVals <=0) return(0);
    evaluating++;

    int nKeys = key.Length();
    int i1,i2;
    float frac;

    if (nVals == 1) {
        value.set(keyValue[0]); 
        evaluating--;
        return(0);
    }

    if (nKeys > 1) {
        findKey(f,nKeys, key.values,i1,i2, frac);
    } else {   // no key frames, map linear 
        findKey(f,nVals,i1,i2,frac);
    }
    if (frac < FRAC_EPS) value.set(keyValue[i1]);
    else  {
        keyValue.Interpolate(value,keyValue[i1],1.0-frac,keyValue[i2],frac);
        value.touch();
    }
    evaluating--;
    return(1);

}
#endif

int GvPositionInterpolator::OnEvaluate(float f)
{   int nVals = keyValue.Length();
    if (nVals <=0) return(0);

    int i1,i2;
    float frac;

    evaluating++;

	if (EvaluateKey(f,nVals,i1,i2, frac) <= 0) {
		value.set(keyValue[i1]);
	}
	else {
        keyValue.Interpolate(value,keyValue[i1],1.0-frac,keyValue[i2],frac);
        value.touch();
	}
	//TRACE("PI %f (%f %f %f) \n",f,value.value[0],value.value[1],value.value[2]);

    evaluating--;
	return(1);
}


//
// GvNormalInterpolator
//

GV_NODE_SOURCE_IMP(GvNormalInterpolator,GvInterpolator);

GvNormalInterpolator::GvNormalInterpolator() : keyValue(0)
{
    GV_NODE_CONSTRUCTOR(GvPositionInterpolator);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(key);
    GV_NODE_ADD_FIELD(keyValue);
    GV_NODE_ADD_FIELD_EVENT_IN(fraction);
    
    GV_NODE_ADD_EVENT_OUT(value);
}



GvNormalInterpolator::~GvNormalInterpolator()
{
}

// multi 
int GvNormalInterpolator::OnEvaluate(float f)
{   int nVals = keyValue.Length();
    if (nVals <=0) return(0);
    evaluating++;

    int nKeys = key.Length();
    int i1,i2;
    float frac;
    int multiple = 0;

    if (nVals == 1) {
        value.set(1,keyValue[0]);
        value.touch();
        return(0);
    }

    if (nKeys > 1) {
        findKey(f,nKeys, key.values,i1,i2, frac);
        multiple =  nVals / nKeys;
    } else {
        // no key frames, map linear 
        findKey(f,nVals,i1,i2,frac);
        multiple = 1;
    }

    //TRACE("Coordinate interpolater f=%f i1 %d i2 %d frac = %f  m=%d \n",f,i1,i2,frac,multiple);

    value.makeRoom(multiple);
    i1*=multiple;
    i2*=multiple;
    ASSERT(i1 <= (nVals-multiple));
    ASSERT(i2 <= (nVals-multiple));
    keyValue.InterpolateNormal(value,keyValue[i1],1.0-frac,keyValue[i2],frac,multiple);
    value.touch();
        
    evaluating--;

    return(1);

}


//
// GvOrientationInterpolator
//

GV_NODE_SOURCE_IMP(GvOrientationInterpolator,GvInterpolator);

GvOrientationInterpolator::GvOrientationInterpolator() : keyValue(0)
{
    GV_NODE_CONSTRUCTOR(GvOrientationInterpolator);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(key);
    GV_NODE_ADD_FIELD(keyValue);
    GV_NODE_ADD_FIELD_EVENT_IN(fraction);
    
    GV_NODE_ADD_EVENT_OUT(value);
}



GvOrientationInterpolator::~GvOrientationInterpolator()
{
}

int GvOrientationInterpolator::OnEvaluate(float f)
{   int nVals = keyValue.Length();
    if (nVals <=0) return(0);

    int i1,i2;
    float frac;

    evaluating++;
	if (EvaluateKey(f,nVals,i1,i2, frac) <= 0) {
		value.set(keyValue[i1]);
	}
	else {
        keyValue.Interpolate(&value.value,&keyValue[i1],1.0-frac,&keyValue[i2],frac);
        value.touch();
	}
    evaluating--;
	return(1);
}

//
// GvScalarInterpolator
//

GV_NODE_SOURCE_IMP(GvScalarInterpolator,GvInterpolator);

GvScalarInterpolator::GvScalarInterpolator() : keyValue(0)
{
    GV_NODE_CONSTRUCTOR(GvScalarInterpolator);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(key);
    GV_NODE_ADD_FIELD(keyValue);
    GV_NODE_ADD_FIELD_EVENT_IN(fraction);
    
    GV_NODE_ADD_EVENT_OUT(value);
}



GvScalarInterpolator::~GvScalarInterpolator()
{
}

int GvScalarInterpolator::OnEvaluate(float f)
{   int nVals = keyValue.Length();
    if (nVals <=0) return(0);

    int i1,i2;
    float frac;

    evaluating++;
	if (EvaluateKey(f,nVals,i1,i2, frac) <= 0) {
		value.set(keyValue[i1]);
	}
	else {
        keyValue.Interpolate(&value.value,&keyValue[i1],1.0-frac,&keyValue[i2],frac);
        value.touch();
	}
    evaluating--;
	return(1);
}




