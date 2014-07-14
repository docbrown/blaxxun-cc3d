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
// HG source file for all Transfomations nodes + texture transformation nodes + animated textures
///

/*
to consider for animated transfroms:
	add a input t field, if not connected use global t
	check for offset field and connect 


*/
#include "stdafx.h"
#include "grender.h"

#ifdef _G_VRML1

#include <gvmatrixtransform.h>



GV_NODE_SOURCE_IMP(GvMatrixTransform,GvNode);


GvMatrixTransform::GvMatrixTransform()
{
    GV_NODE_CONSTRUCTOR(GvMatrixTransform);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(matrix);

    for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++)
	    matrix.value[i][j] = (i == j ? 1.0 : 0.0);
}

GvMatrixTransform::~GvMatrixTransform()
{
}


#include <gvtransform.h>

GV_NODE_SOURCE_IMP(GvTransform,GvNode);

GvTransform::GvTransform()
{
    GV_NODE_CONSTRUCTOR(GvTransform);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(translation);
    GV_NODE_ADD_FIELD(rotation);
    GV_NODE_ADD_FIELD(scaleFactor);
    GV_NODE_ADD_FIELD(scaleOrientation);
    GV_NODE_ADD_FIELD(center);

    translation.value[0] = translation.value[1] = translation.value[2] = 0.0;
    rotation.value.axis[0] = 0.0;
    rotation.value.axis[1] = 0.0;
    rotation.value.axis[2] = 1.0;
    rotation.value.angle = 0.0;
    
    scaleFactor.value[0] = scaleFactor.value[1] = scaleFactor.value[2] = 1.0;
    scaleOrientation.value.axis[0] = 0.0;
    scaleOrientation.value.axis[1] = 0.0;
    scaleOrientation.value.axis[2] = 1.0;
    scaleOrientation.value.angle = 0.0;
    center.value[0] = center.value[1] = center.value[2] = 0.0;
	cached = 0;
}


int GvTransform::OnChanged()
{
   cached = 0;
   return(1);
}

GvTransform::~GvTransform()
{
}

#if 1
void GvTransform::Get(Matrix &m) 

{
#if 0
		 m = IdentityMatrix();
		 //	int isDefault() { return(matrix.isDefault()); }
  
  		 m = TranslationMatrix(center+translation) * m;
		 m = RotationAxisMatrix(rotation.Axis(),rotation.Angle()) * m;
		 if (!scaleFactor.isDefault()) {
		 m = RotationAxisMatrix(scaleOrientation.Axis(),scaleOrientation.Angle()) * m;
		 m = ScaleMatrix(scaleFactor) * m;
		 m = RotationAxisMatrix(scaleOrientation.Axis(),-scaleOrientation.Angle()) * m;
		 }
		 m = TranslationMatrix(-(Point)center) * m;
#else
  		 m = TranslationMatrix(center+translation);
		 m = RotationAxisMatrix(rotation.Axis(),rotation.Angle()) * m;
		 if (!scaleFactor.isDefault()) {
			m = RotationAxisMatrix(scaleOrientation.Axis(),scaleOrientation.Angle()) * m;
			m = ScaleMatrix(scaleFactor) * m;
			m = RotationAxisMatrix(scaleOrientation.Axis(),-scaleOrientation.Angle()) * m;
		 }
		 m = TranslationMatrix(-center.getPoint()) * m;

#endif
}   

#else
void GvTransform::get (Matrix &m)

{
		 m = IdentityMatrix();
		 //	int isDefault() { return(matrix.isDefault()); }
  
  		 m *= TranslationMatrix(center+translation);
		 m *= RotationAxisMatrix(rotation.Axis(),rotation.Angle());
		 if (!scaleFactor.isDefault()) {
		 m *= RotationAxisMatrix(scaleOrientation.Axis(),scaleOrientation.Angle());
		 m *= ScaleMatrix(scaleFactor);
		 m *= RotationAxisMatrix(scaleOrientation.Axis(),-scaleOrientation.Angle());
		 }
		 m *= TranslationMatrix(-(Point)center);
}   
#endif

// GvRotation
#include <gvrotation.h>

GV_NODE_SOURCE_IMP(GvRotation,GvNode);

GvRotation::GvRotation()
{
    GV_NODE_CONSTRUCTOR(GvRotation);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(rotation);

    rotation.value.axis[0] = 0.0;
    rotation.value.axis[1] = 0.0;
    rotation.value.axis[2] = 1.0;
    rotation.value.angle = 0.0;
}

GvRotation::~GvRotation()
{
}


GV_NODE_SOURCE_IMP(GvRotationXYZ,GvNode);

GvRotationXYZ::GvRotationXYZ()
{
    GV_NODE_CONSTRUCTOR(GvRotationXYZ);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(axis);
    GV_NODE_ADD_FIELD(angle);

    GV_NODE_DEFINE_ENUM_VALUE(Axis, X);
    GV_NODE_DEFINE_ENUM_VALUE(Axis, Y);
    GV_NODE_DEFINE_ENUM_VALUE(Axis, Z);

    axis.value  = X;
    angle.value = 0.0;
    GV_NODE_SET_SF_ENUM_TYPE(axis, Axis);

}

void GvRotationXYZ::get(Matrix &m) {
	 if (axis == (int)X) m = RotationXMatrix(angle);
	 else if (axis == (int)Y) m = RotationYMatrix(angle);
	 else m = RotationZMatrix(angle);
}


GvRotationXYZ::~GvRotationXYZ()
{
}

// GvScale
#include <gvscale.h>

GV_NODE_SOURCE_IMP(GvScale,GvNode);

GvScale::GvScale()
{
    GV_NODE_CONSTRUCTOR(GvScale);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(scaleFactor);

    scaleFactor.value[0] = scaleFactor.value[1] = scaleFactor.value[2] =1.0;
}

GvScale::~GvScale()
{
}

// GvTranslation
#include <gvtranslation.h>

GV_NODE_SOURCE_IMP(GvTranslation,GvNode);

GvTranslation::GvTranslation()
{
    GV_NODE_CONSTRUCTOR(GvTranslation);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(translation);

    translation.value[0] = translation.value[1] = translation.value[2] = 0.0;
}

GvTranslation::~GvTranslation()
{
}



// GvRotor
#include <gvrotor.h>

GV_NODE_SOURCE_IMP(GvRotor,GvRotation);

GvRotor::GvRotor()
{
    // Problem how are derived fields handled
    GV_NODE_CONSTRUCTOR(GvRotor);
    isBuiltIn = FALSE; // extension node


    GV_NODE_ADD_FIELD(rotation);
    GV_NODE_ADD_FIELD(speed);
    GV_NODE_ADD_FIELD(on);
    GV_NODE_ADD_FIELD(offset);

    rotation.value.axis[0] = 0.0;
    rotation.value.axis[1] = 0.0;
    rotation.value.axis[2] = 1.0;
    rotation.value.angle = 2.0 * M_PI;
	speed.value = 1.0;
	on.value = TRUE;
	offset.value = 0.0;
}

// get type class of node
int GvRotor::GetNodeType()
{
   return(GvRotation::GetNodeClassType() /*|TIME_DEPENDENT_NODE|GLVIEW_NODE*/ );
}

GvRotor::~GvRotor()
{
}

//GvPedulum
#include <gvpendulum.h>

GV_NODE_SOURCE_IMP(GvPendulum,GvRotation);

GvPendulum::GvPendulum()
{
    // Problem how are derived fields handled
    GV_NODE_CONSTRUCTOR(GvPendulum);
    isBuiltIn = TRUE;
    isBuiltIn = FALSE; // extension node

	// in herited
    GV_NODE_ADD_FIELD(rotation);
    GV_NODE_ADD_FIELD(rotation0);
    GV_NODE_ADD_FIELD(rotation1);
    GV_NODE_ADD_FIELD(speed);
    GV_NODE_ADD_FIELD(on);
    GV_NODE_ADD_FIELD(offset);

    rotation.value.axis[0] = 0.0;
    rotation.value.axis[1] = 0.0;
    rotation.value.axis[2] = 1.0;
    rotation.value.angle = 2.0 * M_PI;
	rotation0 = rotation;
	rotation1 = rotation;
	speed.value = 1.0;
	on.value = TRUE;
	offset.value = 0.0;
}

GvPendulum::~GvPendulum()
{
}

// get type class of node
int GvPendulum::GetNodeType()
{
   return(GvRotation::GetNodeClassType() /* |TIME_DEPENDENT_NODE|GLVIEW_NODE*/);
}

#include "quaternation.h"

void GvPendulum::Get(Matrix &m) 
{
	       rotation.get(m);
}

void GvPendulum::Get(Matrix &m,float t) 
{
	t *= speed;
	t += offset;
	t = fmod(t,1.0f);
	QuaternationWithSpin q1,q2;
	q1.SetAxisRotation(rotation0.value.axis[0],rotation0.value.axis[1],rotation0.value.axis[2],rotation0.value.angle);
	q2.SetAxisRotation(rotation1.value.axis[0],rotation1.value.axis[1],rotation1.value.axis[2],rotation1.value.angle);

	QuaternationWithSpin q;
	q.Interpolate(q1,q2,t);
	// to do: set rotation field
	q.GetMatrix(m);

}


// GvShuttle

#include <gvshuttle.h>

GV_NODE_SOURCE_IMP(GvShuttle,GvTranslation);

GvShuttle::GvShuttle()
{
    GV_NODE_CONSTRUCTOR(GvShuttle);
    isBuiltIn = TRUE;
    isBuiltIn = FALSE; // extension node

	// inherited 
    GV_NODE_ADD_FIELD(translation);
    GV_NODE_ADD_FIELD(translation0);
    GV_NODE_ADD_FIELD(translation1);
    GV_NODE_ADD_FIELD(speed);
    GV_NODE_ADD_FIELD(on);

    translation.value[0] = translation.value[1] = translation.value[2] = 0.0;
    translation0.value[0] = translation0.value[1] = translation0.value[2] = 0.0;
    translation1.value[0] = translation1.value[1] = translation1.value[2] = 0.0;
	speed.value = 1.0;
	on.value = TRUE;

}

GvShuttle::~GvShuttle()
{
}

void GvShuttle::Get(Matrix &m) {
	      m = TranslationMatrix(translation);

}

void GvShuttle::Get(Matrix &m,float t) 
{
	t *= speed;
	t = fmod(t,1.0f);
	Point tvec = Interpolate(translation0,(1.0-t),translation1,(t));
	translation.set(tvec);
    m = TranslationMatrix(translation);
}



// GvTexture2Transform
#include <gvtexture2transform.h>

//
// GvTexture2Transform
//
GV_NODE_SOURCE_IMP(GvTexture2Transform,GvNode);

GvTexture2Transform::GvTexture2Transform()
{
    GV_NODE_CONSTRUCTOR(GvTexture2Transform);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(translation);
    GV_NODE_ADD_FIELD(rotation);
    GV_NODE_ADD_FIELD(scaleFactor);
    GV_NODE_ADD_FIELD(center);

    translation.value[0] = translation.value[1] = 0.0;
    rotation.value = 0.0;
    scaleFactor.value[0] = scaleFactor.value[1] = 1.0;
    center.value[0] = center.value[1] = 0.0;
	changed = 0;

	mVersion = 0;  // need a new unique  matrix
}

GvTexture2Transform::~GvTexture2Transform()
{
}

// something changed 
int GvTexture2Transform::OnChanged()
{
	changed = 1;
	setRedraw();
    return GvNode::OnChanged();
}


void GvTexture2Transform::Get(Matrix &m)

{
	 m = IdentityMatrix();
	 m = TranslationMatrix(center+translation) * m;
	 m = RotationZMatrix(rotation) * m;
	 if (!scaleFactor.isDefault()) {
		 m = ScaleMatrix(scaleFactor) * m;
	 }
	 m = TranslationMatrix(-center.getPoint()) * m;
}   

// apply setting to renderstate
int GvTexture2Transform::Apply(GglRenderTraversal &state)
{
	Matrix m;
	Get(m); 
	state.glState->SetTextureMatrix(m);
	return(1);
}

#endif

// GvTexture2Transform
#include <gvtexture2transform.h>

//
// Gv2TextureTransform // VRML 2.0
//
GV_NODE_SOURCE_IMP(Gv2TextureTransform,GvNode);

Gv2TextureTransform::Gv2TextureTransform()
{
    GV_NODE_CONSTRUCTOR(Gv2TextureTransform);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(center);
    GV_NODE_ADD_FIELD(rotation);
    GV_NODE_ADD_FIELD(scale);
    GV_NODE_ADD_FIELD(translation);


    //translation.value[0] = translation.value[1] = 0.0;
    //rotation.value = 0.0;
    scale.value[0] = scale.value[1] = 1.0;
    //center.value[0] = center.value[1] = 0.0;
	changed = 0;

	transformCached = 0;


	mVersion = 0;  // need a new unique  matrix

}

Gv2TextureTransform::~Gv2TextureTransform()
{
}

int Gv2TextureTransform::checkForDefaultValues(int mode)
{
    if (translation.value[0] == 0 &&  translation.value[1] == 0) translation.setDefault(TRUE);
    if (rotation.value == 0.0) rotation.setDefault(TRUE);
    if (scale.value[0] == 1.0 &&  scale.value[1] == 1.0) scale.setDefault(TRUE);
    if (center.value[0] == 0 &&  center.value[1] == 0.0) center.setDefault(TRUE);
	return 0;

}


// something changed 
int Gv2TextureTransform::OnChanged()
{
	changed = 1;
	transformCached = 0;
    setRedraw();	 // done in appearance but not if in PROTO :-(
    return GvNode::OnChanged();

}


void Gv2TextureTransform::get(Matrix &m)
{
#if 0
	 m = TranslationMatrix(center+translation); 
	 if (rotation != 0.0f) m = RotationZMatrix(rotation) * m;
	 if (!scale.isDefault()) {
		 m = ScaleMatrix(scale) * m;
	 }
	 m = TranslationMatrix(-center.getPoint()) * m;
#else

	m.TranslationMatrix(-center.getPoint());
	if (!scale.isDefault()) {
		 //m = ScaleMatrix(scale) * m;
		 m.ScaleByLeft(scale);
	}
	if (rotation != 0.0f) 
		m.MultiplyLeft(RotationZMatrix(rotation));
	m.MultiplyLeft(TranslationMatrix(center+translation));

#endif

}   



// apply setting to renderstate
int Gv2TextureTransform::Apply(GglRenderTraversal &state)
{

/*
#ifdef _OGL
	Matrix m;
	Get(m); 
	state.glState->SetTextureMatrix(Transform());
#endif
*/

	// D3D Problem, no indep. texture transform
	// transforms is evaluated into execute buffer
	// sharing IFS with different TT big Problem
	// to minimize changes, stamp each TT node with a unique id
	// to be able to detect changes

//#ifdef _D3D
	if (changed) {
		mVersion = 0;
		changed = 0;
	}
	if (mVersion == 0) {
		mVersion=  state.glState->GetNewTextureMatrixVersion();
	}
	
	state.glState->SetTextureMatrix(Transform(),mVersion);
//#endif


	return(1);
}

int Gv2TextureTransform::Do(GTraversal &state)   
{ 
	// state approache really not needed for VRML 2.0 rendering 

	GTextureMatrixStack *current,*prev;
	
	prev = (GTextureMatrixStack*) state.Get(GTextureMatrixStack::attrId);
	if (!prev) return(0);

    Matrix &m=Transform();

	if (prev->level == state.level) { // already pushed
       prev->AppendLeft(m);
	   prev->Set(state);
	   return(1);
	}																	
	current = (GTextureMatrixStack*) prev->NewInstance();
	current->m = m* prev->m;
	state.Push(current);
	return(1);
}




#ifdef _G_VRML1

// GvTextureRotor
#include <gvtexturerotor.h>

GV_NODE_SOURCE_IMP(GvTextureRotor,GvRotation);

GvTextureRotor::GvTextureRotor()
{
    // Problem how are derived fields handled
    GV_NODE_CONSTRUCTOR(GvTextureRotor);
    isBuiltIn = FALSE; // extension node

    GV_NODE_ADD_FIELD(rotation);
    GV_NODE_ADD_FIELD(speed);
    GV_NODE_ADD_FIELD(on);
    GV_NODE_ADD_FIELD(offset);

    rotation.value.axis[0] = 0.0;
    rotation.value.axis[1] = 0.0;
    rotation.value.axis[2] = 1.0;
    rotation.value.angle = 2.0 * M_PI;
	speed.value = 1.0;
	on.value = TRUE;
	offset.value = 0.0;
}

GvTextureRotor::~GvTextureRotor()
{
}

// GvTextureShuttle

#include <gvtextureshuttle.h>

GV_NODE_SOURCE_IMP(GvTextureShuttle,GvTranslation);

GvTextureShuttle::GvTextureShuttle()
{
    GV_NODE_CONSTRUCTOR(GvTextureShuttle);
    isBuiltIn = TRUE;
    isBuiltIn = FALSE; // extension node

	// inherited 
    GV_NODE_ADD_FIELD(translation);
    GV_NODE_ADD_FIELD(translation0);
    GV_NODE_ADD_FIELD(translation1);
    GV_NODE_ADD_FIELD(speed);
    GV_NODE_ADD_FIELD(on);

    translation.value[0] = translation.value[1] = translation.value[2] = 0.0;
    translation0.value[0] = translation0.value[1] = translation0.value[2] = 0.0;
    translation1.value[0] = translation1.value[1] = translation1.value[2] = 0.0;
	speed.value = 1.0;
	on.value = TRUE;

}

GvTextureShuttle::~GvTextureShuttle()
{
}

void GvTextureShuttle::Get(Matrix &m) {
     m = TranslationMatrix(translation);
}

void GvTextureShuttle::Get(Matrix &m,float t) 
{
	t *= speed;
	t = fmod(t,1.0f);
	Point tvec = Interpolate(translation0,(1.0f-t),translation1,(t));
	translation.set(tvec);
    m = TranslationMatrix(translation);
}

#endif
