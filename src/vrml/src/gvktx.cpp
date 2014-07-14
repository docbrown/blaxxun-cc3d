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

#include <gvnodes.h>

#include <gvktx.h>

#include "gshell.h"
#include "gcamera.h"


// derivs.c from Compuserve AMMEDIA 
/* 
 Example 3D Studio r3 key interpolation code for KXP 
 IPAS3 developers.  Note that position keys are handled just
 like any other track type except for rotations.  Those are
 handled as described below.  

 This is unsupported code, so use at your own risk.
------------------------------------------------------------*/

static void CompElementDeriv( float pp, float p, float pn, float *ds, float *dd,
			      float ksm, float ksp, float kdm, float kdp )
{
	float delm, delp;

	delm = p - pp;
	delp    = pn - p;
	*ds  = ksm*delm + ksp*delp;
	*dd  = kdm*delm + kdp*delp;
}

#define X 0
#define Z 2

class PosKey {
public:
    long frame;
    float pos[3];
    float ds[3]; // source derivate
    float dd[3]; // destination derivate 
    float cont;
    float tens;
    float bias;
};

class RotKey {
public:
    long frame;
    float axis[3];
    float angle;
    float cont;
    float tens;
    float bias;
    
    float quat[3];
};

#if 0
qlog
qcopy
qlndif
qexp
qmul
#endif



/*-------------------------------------------------------
 This computes the derivative at key, as a weighted average of
 the linear slopes into and out of key, the weights being determined
 by the tension and continuity parameters.      Actually two derivatives
 are computed at key:
	"ds" is the "source derivative", or "arriving derivative" 
	"dd" is the "destination derivative" or "departing derivative"
-------------------------------------------------------------*/
static void CompDeriv( PosKey *keyp, PosKey *key, PosKey *keyn )
{
	int i;
	/* Full TCB computation */
	float tm,cm,cp,bm,bp,tmcm,tmcp,ksm,ksp,kdm,kdp,/*delm,delp,*/c;
	float dt,fp,fn;
	
	/* fp,fn apply speed correction when continuity is 0.0 */
	dt = .5 * (float)( keyn->frame - keyp->frame );
	fp = ( (float)( key->frame - keyp->frame ) ) / dt;
	fn = ( (float)( keyn->frame - key->frame ) ) / dt;
	c  = fabs( key->cont );
	fp = fp + c - c * fp;
	fn = fn + c - c * fn;
	cm = 1.0 - key->cont;   
	tm = 0.5 * ( 1.0 - key->tens );
	cp = 2.0 - cm;
	bm = 1.0 - key->bias;
	bp = 2.0 - bm;      
	tmcm = tm*cm;   tmcp = tm*cp;
	ksm = tmcm*bp*fp;       ksp = tmcp*bm*fp;
	kdm = tmcp*bp*fn;       kdp = tmcm*bm*fn;
	
	for( i = X; i <= Z; i++ ) {
		CompElementDeriv( keyp->pos[i], key->pos[i], keyn->pos[i], 
				  &key->ds[i], &key->dd[i], ksm, ksp, kdm, kdp );
	}
					
}

#if 0
/* -----------------------------------------------------------
   Compute the "a" and "b" terms at key "cur", which determine
   the incoming and outgoing tangents (in quaternion space ) 
 -----------------------------------------------------------*/
static int CompAB( RotKey *prev, RotKey *cur, RotKey *next ) 
{
	int i;
	float qprev[4],qnext[4],q[4],qzero[4];
	float qp[4],qm[4],qa[4],qb[4],qae[4],qbe[4];
	float tm,cm,cp,bm,bp,tmcm,tmcp,ksm,ksp,kdm,kdp,c;
	float dt,fp,fn;
	
	if( prev != NULL ) {
		if( cur->angle > TWOPI-.00001 ) {
			COPY_POINT3( q, cur->axis );
			q[3] = 0;
			qlog( q,qm );
		} else {
			qcopy( qprev, prev->quat );
			if( qdot( qprev, cur->quat ) < 0 ) qnegate( qprev );
			qlndif( qprev, cur->quat, qm );
		}
	}
	
	if( next != NULL ) {
		if( next->angle > TWOPI-.00001 ) {
			COPY_POINT3( q, next->axis );
			q[3] = 0;
			qlog( q, qp );
		} else { 
			qcopy( qnext, next->quat );
			if( qdot( qnext, cur->quat ) < 0 ) qnegate( qnext );
			qlndif( cur->quat, qnext, qp );
		}
	}
	
	if( prev == NULL ) qcopy( qm, qp );
	if( next == NULL ) qcopy( qp, qm );

	fp = fn = 1.0;
	cm = 1.0 - cur->cont;
	if( prev && next ) {
		dt = 0.5 * (float)(next->frame - prev->frame );
		fp = ((float)(cur->frame - prev->frame))/dt;
		fn = ((float)(next->frame - cur->frame))/dt;
		c = fabs( cur->cont );
		fp = fp + c - c * fp;
		fn = fn + c - c * fn;
	}

	tm = .5*(1.0 - cur->tens);
	cp = 2.0 - cm;
	bm = 1.0 - cur->bias;
	bp = 2.0 - bm;      
	tmcm = tm * cm; 
	tmcp = tm * cp;
	ksm  = 1.0 - tmcm * bp * fp;    
	ksp  = -tmcp * bm * fp;
	kdm  = tmcp * bp * fn;  
	kdp  = tmcm * bm * fn - 1.0;
	
	for( i = 0; i < 4; i++ ) {
		qa[i] = .5 * ( kdm * qm[i] + kdp * qp[i] );
		qb[i] = .5 * ( ksm * qm[i] + ksp * qp[i] );
	}

	qexp( qa, qae );
	qexp( qb, qbe );
	
	qmul( cur->quat, qae, cur->a );
	qmul( cur->quat, qbe, cur->b );

	return TRUE;
}
#endif


/*
Subject: Re: Question re derivs.c (fwd)

It looks like deriv.c didn't contain the ease function -- just 
the geometric derivatives.  In that case, here's the ease function:
*/

/*  Ease: remap parameter between two keys to apply eases

Call this with 
   a = easeFrom of key[n], 
   b = easeTo of key[n+1], and 
 u = (t-t[n]/(t[n+1]-t[n])
-------------------------------------*/

static  float Ease(float u, float a, float b) {
 float k;
 float s = a+b;
 if (s==0.0) return(u);
 if (s>1.0) {
    a = a/s;
    b = b/s;
  }
 k = 1.0/(2.0-a-b); 
 if (u<a) return((k/a)*u*u);
 else if (u<1.0-b) return( k*(2*u - a)); 
 else {
    u = 1.0-u;
    return(1.0-(k/b)*u*u);
  }
 }


////////////////////////////////////////////////////////////////


GV_NODE_SOURCE_IMP(GvAnimationFrames_ktx_com,GvNode);

GvAnimationFrames_ktx_com::GvAnimationFrames_ktx_com()
{
    GV_NODE_CONSTRUCTOR(GvAnimationFrames_ktx_com);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(length);
    GV_NODE_ADD_FIELD(segmentStart);
    GV_NODE_ADD_FIELD(segmentEnd);
    GV_NODE_ADD_FIELD(current);
    GV_NODE_ADD_FIELD(rate);
    length.value = -1;
    segmentStart.value = -1;
    segmentEnd.value = -1;
    current.value = -1;
    rate.value = 30.0;

}

GvAnimationFrames_ktx_com::~GvAnimationFrames_ktx_com()
{
}


////////////////////////////////////////////////////////////////////////////////////////

GV_NODE_SOURCE_IMP(GvAnimationStyle_ktx_com,GvNode);

GvAnimationStyle_ktx_com::GvAnimationStyle_ktx_com()
{
    GV_NODE_CONSTRUCTOR(GvAnimationStyle_ktx_com);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(loop);
    GV_NODE_ADD_FIELD(splineUse);
    GV_NODE_ADD_FIELD(tension);
    GV_NODE_ADD_FIELD(continuity);
    GV_NODE_ADD_FIELD(bias);
    GV_NODE_ADD_FIELD(easeTo);
    GV_NODE_ADD_FIELD(easeFrom);
    GV_NODE_ADD_FIELD(pivotOffset);

    GV_NODE_DEFINE_ENUM_VALUE(SplineUse, NONE);
    GV_NODE_DEFINE_ENUM_VALUE(SplineUse, CONTINUITY);
    GV_NODE_DEFINE_ENUM_VALUE(SplineUse, BIAS);
    GV_NODE_DEFINE_ENUM_VALUE(SplineUse, EASETO);
    GV_NODE_DEFINE_ENUM_VALUE(SplineUse, EASEFROM);
    GV_NODE_DEFINE_ENUM_VALUE(SplineUse, TENSION);
    GV_NODE_DEFINE_ENUM_VALUE(SplineUse, ALL);
    GV_NODE_SET_SF_ENUM_TYPE(splineUse, SplineUse);

    loop.value = FALSE;
    splineUse.value = NONE;
    tension.value = 0.0;
    continuity.value = 0.0;
    bias.value = 0.0;
    easeTo.value = 0.0;
    easeFrom.value = 0.0;
    pivotOffset.value[0] =pivotOffset.value[1] = pivotOffset.value[2]  = 0.0;

}

GvAnimationStyle_ktx_com::~GvAnimationStyle_ktx_com()
{
}


////////////////////////////////////////////////////////////////////////////////////////

GV_NODE_SOURCE_IMP(GvPositionKey_ktx_com,GvNode);

GvPositionKey_ktx_com::GvPositionKey_ktx_com()
{
    GV_NODE_CONSTRUCTOR(GvPositionKey_ktx_com);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(frame);
    GV_NODE_ADD_FIELD(translation);
    frame.value = 0;
    translation.value[0] =translation.value[1] = translation.value[2]  = 0.0;
}

GvPositionKey_ktx_com::~GvPositionKey_ktx_com()
{
}

////////////////////////////////////////////////////////////////////////////////////////



GV_NODE_SOURCE_IMP(GvRotationKey_ktx_com ,GvNode);

GvRotationKey_ktx_com ::GvRotationKey_ktx_com ()
{
    GV_NODE_CONSTRUCTOR(GvPositionKey_ktx_com);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(frame);
    GV_NODE_ADD_FIELD(rotation);
    frame.value = 0;
    rotation.value.set(0,1,0,0);
}

GvRotationKey_ktx_com ::~GvRotationKey_ktx_com ()
{
}

////////////////////////////////////////////////////////////////////////////////////////

GV_NODE_SOURCE_IMP(GvHideKey_ktx_com ,GvNode);

GvHideKey_ktx_com ::GvHideKey_ktx_com ()
{
    GV_NODE_CONSTRUCTOR(GvHideKey_ktx_com);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(frame);
    frame.value = 0;
}

GvHideKey_ktx_com ::~GvHideKey_ktx_com ()
{
}

////////////////////////////////////////////////////////////////////////////////////////

GV_NODE_SOURCE_IMP(GvScaleKey_ktx_com,GvNode);

GvScaleKey_ktx_com::GvScaleKey_ktx_com()
{
    GV_NODE_CONSTRUCTOR(GvScaleKey_ktx_com);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(frame);
    GV_NODE_ADD_FIELD(scale);
    frame.value = 0;
    scale.value[0] =scale.value[1] = scale.value[2]  = 1.0;
}

GvScaleKey_ktx_com::~GvScaleKey_ktx_com()
{
}

////////////////////////////////////////////////////////////////////////////////////////


GV_NODE_SOURCE_IMP(GvColorKey_ktx_com,GvNode);

GvColorKey_ktx_com::GvColorKey_ktx_com()
{
    GV_NODE_CONSTRUCTOR(GvColorKey_ktx_com);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(frame);
    GV_NODE_ADD_FIELD(color);
    frame.value = 0;
    color.value[0] =color.value[1] = color.value[2]  = 1.0;
}

GvColorKey_ktx_com::~GvColorKey_ktx_com()
{
}

////////////////////////////////////////////////////////////////////////////////////////

GV_NODE_SOURCE_IMP(GvAimTarget_ktx_com,GvNode);

GvAimTarget_ktx_com::GvAimTarget_ktx_com()
{
    GV_NODE_CONSTRUCTOR(GvAimTarget_ktx_com);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(aimer);
    aimer.value = "";
}

GvAimTarget_ktx_com::~GvAimTarget_ktx_com()
{
}

////////////////////////////////////////////////////////////////////////////////////////

GV_NODE_SOURCE_IMP(GvReferenceMark_ktx_com,GvSeparator);

GvReferenceMark_ktx_com::GvReferenceMark_ktx_com()
{
    GV_NODE_CONSTRUCTOR(GvReferenceMark_ktx_com);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(description);
    description.value = "";
}

GvReferenceMark_ktx_com::~GvReferenceMark_ktx_com()
{
}

////////////////////////////////////////////////////////////////////////////////////////


#endif _G_VRML1



