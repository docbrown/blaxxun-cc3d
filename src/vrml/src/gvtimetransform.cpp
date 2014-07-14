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

#include <gvtimetransform.h>

GV_NODE_SOURCE_IMP(GvTimeTransform,GvNode);

GvTimeTransform::GvTimeTransform()
{
    GV_NODE_CONSTRUCTOR(GvTimeTransform);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(function);
    GV_NODE_ADD_FIELD(translation);
    GV_NODE_ADD_FIELD(scaleFactor);

    function.value = TRANSFORM;
	translation.value = 0.0;
	scaleFactor.value = 1.0;

    GV_NODE_DEFINE_ENUM_VALUE(Function, TRANSFORM);
    GV_NODE_DEFINE_ENUM_VALUE(Function, IDENTITY);
    GV_NODE_DEFINE_ENUM_VALUE(Function, CLAMP);
    GV_NODE_DEFINE_ENUM_VALUE(Function, CYCLE);
    GV_NODE_DEFINE_ENUM_VALUE(Function, WRAP);

    GV_NODE_DEFINE_ENUM_VALUE(Function, MIRROR);
    GV_NODE_DEFINE_ENUM_VALUE(Function, SIN);

    GV_NODE_DEFINE_ENUM_VALUE(Function, SQR);
    GV_NODE_DEFINE_ENUM_VALUE(Function, CUB);
    GV_NODE_DEFINE_ENUM_VALUE(Function, RND);
    GV_NODE_DEFINE_ENUM_VALUE(Function, EASE_IN_OUT);

    GV_NODE_SET_SF_ENUM_TYPE(function, Function);
}


GvTimeTransform::~GvTimeTransform()
{
}


static float f_tria(float x)
{ if (x<=0.0) return (0);
  if (x>=1.0) return (0.0);
  if (x<=0.5) return (x*2.0);
  return(1.0- (x-0.5)*2.0);
}
static float f_lin_i(float x) { return(1.0-x); }

static float f_linclamp(float x)
{ if (x<=0.0) return (0.0);
  if (x>=1.0) return (1.0);
  return(x);
}

static float f_rect(float x)
{ if (x<0.5) return (0.0);
  return(1.0);
}

static float f_saw(float x)
{ float t0=0.3333333333;
  float t1=2*t0;
  float dt=t1-t0;
  if (x<t0) return (0.0);
  if (x>=t1) return (0.0);
  /* triangle */
  if (x<=0.5) return ((x-t0)/ (0.5-t0));
  return(1.0- (x-0.5)/(t1-0.5));
}


static float f_sqr(float x) { return(x*x); }
static float f_cub(float x) { return(x*x*x); }
static float f_sin(float x) { return(sin(x*PI)); }
static float f_smooth(float x) { return((-cos(x*PI)+1)*0.5); }
static float f_rnd(float x) { return((float)rand() / (float)RAND_MAX); }


float GvTimeTransform::Transform(GTraversal &state, float t)
{

  switch (function) {
  case TRANSFORM : return(translation+t*scaleFactor);
  case CLAMP : return(translation+f_linclamp(t/scaleFactor)*scaleFactor);
  case CYCLE : return(translation+f_tria(t/scaleFactor)*scaleFactor);
  case WRAP : return(translation+fmod(t,scaleFactor));

  case MIRROR : return(translation+f_lin_i(t/scaleFactor)*scaleFactor);
  
  case SIN : return(translation+f_sin(t/scaleFactor)*scaleFactor);
  case SQR : return(translation+f_sqr(t/scaleFactor)*scaleFactor);
  case CUB : return(translation+f_tria(t/scaleFactor)*scaleFactor);
  case RND : return(translation+f_rnd(t/scaleFactor)*scaleFactor);

  case EASE_IN_OUT : return(translation+f_smooth(t/scaleFactor)*scaleFactor);

  default : return(t);
  }
  return(t);
}

#endif _G_VRML1


