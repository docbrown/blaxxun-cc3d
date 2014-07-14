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

#include <gvnodes.h>

#include <gvwebfx.h>

#include "gshell.h"
#include "gcamera.h"

#ifdef _G_VRML1


#if 0
    Spin {
        objectname    "tire1"
        yangle        -1
        axes          LOCAL
    }
   DEF MainHeart SpinGroup {
      rotation 0 1 0 .2
      local    FALSE

      Spin {fields [
                SFString objectname,
                SFFloat yangle,
                SFEnum axes ]
                objectname  "GasWorld"
                yangle      -.01732
                axes        GLOBAL  }


#endif




// all WebFX / Live 3d extension nodes



GV_NODE_SOURCE_IMP(GvAxisAlignment,GvNode);

GvAxisAlignment::GvAxisAlignment()
{
    GV_NODE_CONSTRUCTOR(GvAxisAlignment);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(alignment);


    GV_NODE_DEFINE_ENUM_VALUE(Alignment, ALIGNAXISXYZ);
    GV_NODE_SET_SF_ENUM_TYPE(alignment, Alignment);
	alignment.value = ALIGNAXISXYZ;
}

GvAxisAlignment::~GvAxisAlignment()
{
}

////////////////////////////////////////////////////////////////////////////////////////


GV_NODE_SOURCE_IMP(GvBrowserHints,GvNode);

GvBrowserHints::GvBrowserHints():headlight(FALSE)
{
    GV_NODE_CONSTRUCTOR(GvBrowserHints);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(headlight);

}

GvBrowserHints::~GvBrowserHints()
{
}


////////////////////////////////////////////////////////////////////////////////////////


GV_NODE_SOURCE_IMP(GvAnimator,GvNode);

GvAnimator::GvAnimator():filename(""),loop(TRUE),reverse(FALSE),
		startframe(-1),endframe(-1)
{
    GV_NODE_CONSTRUCTOR(GvAnimator);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(filename);
    GV_NODE_ADD_FIELD(loop);
    GV_NODE_ADD_FIELD(reverse);
    GV_NODE_ADD_FIELD(startframe);
    GV_NODE_ADD_FIELD(endframe);

}

GvAnimator::~GvAnimator()
{
}

////////////////////////////////////////////////////////////////////////////////////////



GV_NODE_SOURCE_IMP(GvCollideStyle,GvNode);

GvCollideStyle::GvCollideStyle() : collide(FALSE)
{
    GV_NODE_CONSTRUCTOR(GvCollideStyle);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(collide);


}

GvCollideStyle::~GvCollideStyle()
{
}



////////////////////////////////////////////////////////////////////////////////////////



GV_NODE_SOURCE_IMP(GvCollisionDetection,GvNode);

GvCollisionDetection::GvCollisionDetection() : collision(FALSE)
{
    GV_NODE_CONSTRUCTOR(GvCollisionDetection);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(collision);


}

GvCollisionDetection::~GvCollisionDetection()
{
}


////////////////////////////////////////////////////////////////////////////////////////


GV_NODE_SOURCE_IMP(GvSprite,GvNode);

GvSprite::GvSprite()
{
    GV_NODE_CONSTRUCTOR(GvSprite);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(texture);
}

GvSprite::~GvSprite()
{
}


////////////////////////////////////////////////////////////////////////////////////////


GV_NODE_SOURCE_IMP(GvSpinGroup,GvGroup);

GvSpinGroup::GvSpinGroup()
{
    GV_NODE_CONSTRUCTOR(GvSpinGroup);
    isBuiltIn = FALSE; // extension node


    GV_NODE_ADD_FIELD(rotation);
    GV_NODE_ADD_FIELD(local);

    rotation.value.axis[0] = 0.0;
    rotation.value.axis[1] = 0.0;
    rotation.value.axis[2] = 1.0;
    rotation.value.angle = 2.0 * M_PI;
	local.value = TRUE;
}

GvSpinGroup::~GvSpinGroup()
{

}

int GvSpinGroup::Do(GTraversal &state)   
{ 

    state.Push();

	GMatrixStack *current,*prev;
	prev = (GMatrixStack*) state.Get(GMatrixStack::attrId);
	if (prev) {
	GTimeStack  *ts = (GTimeStack*) state.Get(GTimeStack::attrId);

	if (!ts || !ts->time.On())  { 
	} else {
	
	float t=ts->time.t;
	
	Matrix m;
	Get(m,t);

	if (!local) {
		Point p(0,0,0);
		p *= prev->m;
		m = TranslationMatrix(-p) * m * TranslationMatrix(p);
	}

	if (prev->level == state.level) { // already pushed
      prev->AppendLeft(m);
	   prev->Set(state);
	} else {																	
		current = (GMatrixStack*) prev->NewInstance();
		current->m = m * prev->m;
		state.Push(current);
	}
	}
	}
	int ret = TraverseChildren(state);
	state.Pop();

	return(1);
}

////////////////////////////////////////////////////////////////////////////////////////


GV_NODE_SOURCE_IMP(GvSpin,GvNode);

GvSpin::GvSpin()
{
    GV_NODE_CONSTRUCTOR(GvSpin);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(objectname);
    GV_NODE_ADD_FIELD(xangle);
    GV_NODE_ADD_FIELD(yangle);
    GV_NODE_ADD_FIELD(zangle);
    GV_NODE_ADD_FIELD(axes);


    GV_NODE_DEFINE_ENUM_VALUE(Axis, LOCAL);
    GV_NODE_DEFINE_ENUM_VALUE(Axis, GLOBAL);
    GV_NODE_SET_SF_ENUM_TYPE(axes, Axis);
	 axes.value = LOCAL;

}

GvSpin::~GvSpin()
{
}

////////////////////////////////////////////////////////////////////////////////////////


GV_NODE_SOURCE_IMP(GvDirectedSound,GvNode);

GvDirectedSound::GvDirectedSound() :
		intensity(1.0),
		minFrontRange(1.0),maxFrontRange(0.0),
		minBackRange(1.0),maxBackRange(0.0),
		loop(TRUE),start(0.0),pause(0.0)
{
    GV_NODE_CONSTRUCTOR(GvDirectedSound);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(name);
    GV_NODE_ADD_FIELD(description);
    GV_NODE_ADD_FIELD(intensity);
    GV_NODE_ADD_FIELD(location);
    GV_NODE_ADD_FIELD(direction);
    GV_NODE_ADD_FIELD(minFrontRange);
    GV_NODE_ADD_FIELD(maxFrontRange);
    GV_NODE_ADD_FIELD(minBackRange);
    GV_NODE_ADD_FIELD(maxBackRange);
    GV_NODE_ADD_FIELD(loop);
    GV_NODE_ADD_FIELD(start);
    GV_NODE_ADD_FIELD(pause);

    location.value[0] = 0.0;
    location.value[1] = 0.0;
    location.value[2] = 0.0;
    direction.value[0] = 0.0;
    direction.value[1] = 0.0;
    direction.value[2] = 1.0;



}

GvDirectedSound::~GvDirectedSound()
{
}



////////////////////////////////////////////////////////////////////////////////////////


GV_NODE_SOURCE_IMP(GvPointSound,GvNode);

GvPointSound::GvPointSound() :
		intensity(1.0),
		minRange(1.0),maxRange(0.0),
		loop(TRUE),start(0.0),pause(0.0)
{
    GV_NODE_CONSTRUCTOR(GvPointSound);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(name);
    GV_NODE_ADD_FIELD(description);
    GV_NODE_ADD_FIELD(intensity);
    GV_NODE_ADD_FIELD(location);
    GV_NODE_ADD_FIELD(minRange);
    GV_NODE_ADD_FIELD(maxRange);
    GV_NODE_ADD_FIELD(loop);
    GV_NODE_ADD_FIELD(start);
    GV_NODE_ADD_FIELD(pause);

    location.value[0] = 0.0;
    location.value[1] = 0.0;
    location.value[2] = 0.0;
}

GvPointSound::~GvPointSound()
{
}


////////////////////////////////////////////////////////////////////////////////////////


GV_NODE_SOURCE_IMP(GvColorGradient,GvGroup);

GvColorGradient::GvColorGradient()
{
    GV_NODE_CONSTRUCTOR(GvColorGradient);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(colors);
    GV_NODE_ADD_FIELD(keys);

    GV_NODE_ADD_FIELD(axis);
    
    GV_NODE_ADD_FIELD(transform);
    
    GV_NODE_ADD_FIELD(cycle);
    GV_NODE_ADD_FIELD(scaleFactor);


    GV_NODE_DEFINE_ENUM_VALUE(Axis, X);
    GV_NODE_DEFINE_ENUM_VALUE(Axis, Y);
    GV_NODE_DEFINE_ENUM_VALUE(Axis, Z);
    GV_NODE_DEFINE_ENUM_VALUE(Axis, RADIAL2);
    GV_NODE_DEFINE_ENUM_VALUE(Axis, RADIAL);
    GV_NODE_DEFINE_ENUM_VALUE(Axis, SINXY);
    GV_NODE_SET_SF_ENUM_TYPE(axis, Axis);

	colors.values[0] = colors.values[1] = colors.values[2] = 0.0;
	keys.values[0] = 0;
	
	scaleFactor.value = 1.0;
	axis.value = Y;
    cycle.value = FALSE;

}

GvColorGradient::~GvColorGradient()
{
}


Point GvColorGradient::Get(float t) 
{ int n = colors.Length();
  float torg = t;
  
  t *= (n-1);
  
  t = fmod(t,(n-1));
  
  if (t<0) t+=(n-1);

  int i1 = t;
  t = t-i1;
  if (i1>=n) {
     i1 = 0;
  }
  int i2 = i1+1;
  if (i2>=n) i2 = 0;

  Point c1,c2,res;
  //t = -1.980310 i1 -1 I2 0 f= -0.960620 
  //TRACE("t = %f i1 %d I2 %d f= %f \n",torg,i1,i2,t);

  
  Interpolate(colors.get1(i1,c1),1.0-t, colors.get1(i2,c2), t,res);
  return(res);

}


int GvColorGradient::Do(GTraversal &state)   
{ 

#if 0
   GStackableAttribute* prev = state.Get(GvModifierElement::attrId);  
   if (!prev) {
	   GvModifierElement *e = GvModifierElement::NewElement;
	   e->modifier = this;
	   e->node = this;
	   state.Set(e);
   } else {
	   GvModifierElement *e = (GvModifierElement *) prev->NewInstance();
    	e->modifier = this;
	   e->node = this;

	   state.Push(e);
   }
	return(1);
#else
   return(0);
#endif

}


int GvColorGradient::Do(GglRenderTraversal &state)   
{ 
   GStackableAttribute* prev = state.Get(GvModifierElement::attrId);
   
   if (!prev) {
	   GvModifierElement *e = GvModifierElement::NewElement();
	   e->modifier = this;
	   e->node = this;
      e->t = 0.0;

      if (cycle) {
         GTimeStack  *ts = (GTimeStack*) state.Get(GTimeStack::attrId);
	      if (ts && ts->time.On()) 
	      	e->t=ts->time.t;
      }
   
	   state.Set(e);
   } else {
	   GvModifierElement *e = (GvModifierElement *) prev->NewInstance();
    	e->modifier = this;
	   e->node = this;
      e->t = 0.0;

      if (cycle) {
         GTimeStack  *ts = (GTimeStack*) state.Get(GTimeStack::attrId);
	      if (ts && ts->time.On()) 
	      	e->t=ts->time.t;
      }

	   state.Push(e);
   }
	return(1);

}

GvNode *GvColorGradient::Apply(GTraversal &state, GvNode * node)

{

	if (!RTISA(node,GShell)) return(node);

   float time = 0.0;

   if (cycle) {
         GTimeStack  *ts = (GTimeStack*) state.Get(GTimeStack::attrId);
	      if (ts && ts->time.On()) 
	      	time=ts->time.t;
   }

	GShell *s = (GShell *) node;

	int n= s->v.Length();
	int ax = axis.value;


	PointArray vc(n);

	for(int i=0; i<n; i++) 
	{
		const Point &p =  s->v[i];
		float t;
      // apply mapping 
      switch (ax) {
      case X:  t = p[ax];         break;
      case Y:  t = p[ax];         break;
      case Z:  t = p[ax];         break;
      case RADIAL2:  t = sqrt(sqr(p.x) + sqr(p.y)); break;
      case RADIAL:  t = sqrt(sqr(p.x) + sqr(p.y) + sqr(p.z)); break;
      case SINXY:  t = sin(p.x * TWOPI) * sin(p.y*TWOPI); break;
      default:
               t = p.x;
               break;
      }
      
      t += time;

		t *= scaleFactor;
		vc[i]= Get(t);
    }

	s->SetVC(vc);

	// destory

	return(s);

}

#endif
