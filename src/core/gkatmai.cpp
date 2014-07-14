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
/******************************************************************************
@doc

@module GKatmai.cpp - blaxxun CC3D Katmai Transform, Light & Utility routines|

Copyright (c) 1998 - 1999	blaxxun interactive 
All rights reserved

Purpose:
Intel Katmai (Pentium III) Transform Light & Utilities

Classes:

KState - Katmai global lighting state


Notes:
Parts of this module are based on samples and ideas from Intel
// compiler options need to changed for Kamtai source compiled with ICL

# ADD CPP /O2 /Ob2 /Qvec /Qpf /D "_USE_INTEL_COMPILER" -Qoption,c,-ip_inline_max_total_stats=20000

Changes:

removed F32Vec4 static constants, problem on non Katmai systems

using now _mm_rsqrt_ps(a) for normal normalization
grouped together transform & coordinate packing 
KBuildFaceList - Katmai backface culling 


$Revision: 1.22 $
$Log: gkatmai.cpp,v $
Revision 1.22  1999/07/06 16:54:31  tom
*** empty log message ***


Todo :

******************************************************************************/

#include "stdafx.h"

//******************************

//#include "d3dtypes.h"

#if 0

#include <math.h>
#include "Point.h"
#include "array.h"
#include "rt.h"

#include "PointA.h"

#endif


#include "gkatmai.h"

// Open GL-required Headerfiles for WINDOWS NT/95

#ifdef _OGL
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifdef _D3D
#include <d3d.h>
#endif


#include "gnurbs.h"


typedef struct  Vertex_Tag  { float x; 
							  float y;
						 	  float z;
							  float w; 
							} Vertex; 

// global flags

BOOL isKatmaiProcessor = FALSE; // FLAG do we run on Katmai
BOOL useKatmai = FALSE; // FLAG use Katmai codepath 

// function variables 
void  (*KMatrixMult)( /*const*/ float a[4][4], /*const*/ float b[4][4], float ret[4][4]);
void (*KMatrixVec) (/*const*/ float m[4][4],/*const*/ Point &A, Point &B);
void (*KMatrixVecN) (/*const*/ float m[4][4],int n, /*const*/ Point* A, Point *B);


#ifdef _KATMAI



// requires Intel compiler


// __declspec(align(16)) float M[4][4]; 


#include <xmmintrin.h> 
#include <fvec.h>




// constants 

/*

constructors uses intrinsic, Crash on non-Katmai system 

_MM_ALIGN16 
static const F32vec4 zero(0.0f);
static const F32vec4 neps(-0.001);
static const F32vec4 eps(0.1);
static const F32vec4 one(1.0f);
static const F32vec4 twofivefive(255.0f);
static const F32vec4 fvecf0pt5(0.5f);
static const F32vec4 fvecf3pt0(3.0f);

*/

// Constants

#define zero F32vec4(0.0f)
#define neps F32vec4(-0.001f)
#define eps F32vec4(0.1f)
#define one F32vec4(1.0f)
#define minusone F32vec4(-1.0f)
#define twofivefive F32vec4(255.0f)
#define fvecf0pt5 F32vec4(0.5f)
#define fvecf3pt0 F32vec4(3.0f)

_MM_ALIGN16 

// negative 0, to be used for xor
static F32vec4  negmask;




#define M128(a) (*(__m128*)&a)

#define SELECT(i3,i2,i1,i0) ((int) ((i3<<6)|(i2<<4)|(i1<<2)|i0))

__inline __m128 sum4(__m128 a)
{
	a = _mm_add_ps(_mm_shuffle_ps(a, a, SELECT(1,0,3,2)), a);
	return _mm_add_ss(_mm_shuffle_ps(a, a, SELECT(3,2,0,1)), a);
}


// xyz expanded 
class KVec3 {
public:
	F32vec4 x,y,z;
};

// initialize KVec3
// one time in memorz
static inline void setKVec3(KVec3 &v,Point val)
{
	v.x = F32vec4(val.x),
	v.y = F32vec4(val.y),
	v.z = F32vec4(val.z);
}

static inline void setKVec3(KVec3 &v,float x,float y,float z)
{
	v.x = F32vec4(x),
	v.y = F32vec4(y),
	v.z = F32vec4(z);
}


class KVec4 {
public :
	F32vec4 v[3];
};

class KMatrix {
public :
	F32vec4 v[4][4];
};

// _MM_ALIGN16;

/*
State information for light,
lighting 4 vertices a time 

*/
class KLightState 
{
public :
	KVec3 direction; // - direction of directional light

	KVec3 ambient;	// material specular color multiplied with light color

	KVec3 diffuse;	// material diffuse color multiplied with light color
	
	KVec3 specular;	// material specular color multiplied with light color

	KVec3 position; // - position of point/spotlight 
	
	KVec3 attenuation;	// distance attenuation
	
	F32vec4 radius;		// max radius 
	F32vec4 cosPhi;		// spot light 
	F32vec4 cosTheta;
};

class KFogState 
{
public :
	KVec3 color; // - fog color
	F32vec4 start;	
	F32vec4 end;
	F32vec4 deltaRcp;	
};


/*
State information for light,
lighting 1 vertex a time 

*/
class KLightState1 
{
public :
	F32vec4 direction;	// direction of directional light

	F32vec4 ambient;	// material specular color multiplied with light color

	F32vec4 diffuse;	// material diffuse color multiplied with light color
	
	F32vec4 specular;	// material specular color multiplied with light color

	F32vec4 position;	// position of point/spotlight 
	
	F32vec4 attenuation;	// distance attenuation
	
	float radius;		// max radius 
	float cosPhi;		// spot light 
	float cosTheta;
	float dummy;
};





typedef struct 
{
	KMatrix m;			// object to screen transformation
	KVec3	viewer;		// viewer position in local coordinate space
	
	//KVec3	ambient;	// global ambient light (currentl not used, ambient per light)	
	KVec3	emissive;	// current material emissive color

	F32vec4 power;		// material specular exponent
	
	F32vec4 alpha_;		// material alpha 4 * AAAA, scaled by 255


	KLightState lights[KMAX_LIGHT]; // table of all lights
	//KLightState1 lights1[KMAX_LIGHT]; // table of all lights for 1 vertex lighting (NURBS)

	//F32vec4	ambient_;	// global ambient light (currentl not used, ambient per light)	
	F32vec4	emissive_;		// current material emissive color
	KFogState fog;			// fog state

	// non aligned data
	int lightTypes[KMAX_LIGHT];	// type of each light 

	float   power_;		// material specular exponent

	size_t	numLights;		// number of lights, 0  .. KMAX_LIGHT
	int alpha;			// original material alpha (0..1)
	
	D3DCOLOR alphaMask; //
	
	int doSpecular;			// do specular lighting 


} KState;



_MM_ALIGN16 

	KState state;		// the lighting context 



// allocate Katmai state 
void *KatmaiStateAlloc()
{
	return  (KState*) _mm_malloc(sizeof(KState), 32);
}

// free Katmai state 
void KatmaiStateFree(void *state)
{
	_mm_free((char *) state);
}


// see what code IC is generating
int KF2int(float f) 
{ return f; }

unsigned int KF2uint(float f) 
{ return f; }

void  KatmaiMatrixMult_x87( /*const*/ float A[4][4], /*const*/ float B[4][4], float ret[4][4] )
{
	// save data if needed 
	if (A==ret) {
		float tmp[4][4];
	    for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			tmp[i][j] = A[i][j];
		KatmaiMatrixMult_x87(tmp,B,ret);
		return;
	}
	if (B==ret) {
		float tmp[4][4];
	    for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			tmp[i][j] = B[i][j];
		KatmaiMatrixMult_x87(A,tmp,ret);
		return;
	}
    for (int i = 0; i < 4; i++)	{
		ret[i][0] = A[i][0] * B[0][0] + A[i][1] * B[1][0] + A[i][2] * B[2][0] + A[i][3] * B[3][0];
        ret[i][1] = A[i][0] * B[0][1] + A[i][1] * B[1][1] + A[i][2] * B[2][1] + A[i][3] * B[3][1];
		// illegal instructions here with /QVec , VC can"t dissasemble ????
        ret[i][2] = A[i][0] * B[0][2] + A[i][1] * B[1][2] + A[i][2] * B[2][2] + A[i][3] * B[3][2];
        ret[i][3] = A[i][0] * B[0][3] + A[i][1] * B[1][3] + A[i][2] * B[2][3] + A[i][3] * B[3][3];
	}
}



//Checking for REAL Katmai New Instruction support - i.e. no emulation
BOOL cpuid_KatmaiHWSpt()
{
	BOOL KatmaiHWSpt = FALSE;	
	int one_ = 1;					//Variable used as a Boolean

	_asm{
		mov		eax,one_			//Move the number "1" into eax - this will move the 
									// feature bits into EDX when a CPUID is issued, that
									// is, EDX will then hold the key to the q:
									// Does this processor have Katmai New Instruction support?)
		cpuid						//Perform CPUID
		shr		edx,0x1A			//Shift the bits in edx to the right by 26, thus bit 25 (the Katmai New
									// Instruction bit) is now in the CF bit in the EFLAGS register.
		xor		eax,eax				//Clear eax
		//cmovc	eax,one				//If the CF flag is a one, then move a "1" into eax
									// Note: The cmov will only work on a PPro architecture type of processor
									// If using a Pentium processor, then use "jc" mnemonic.
		jc carryflagset				//If the CF is set (i.e. a 1), then jump to "carryflagset", otherwise...
		mov		[KatmaiHWSpt],eax	//Set the variable KatmaiHWSpt to false if bit 25 of the EDX is "0" 
									// (i.e. the CF flag is 0).
carryflagset:
		mov		[KatmaiHWSpt],eax	//Set the variable KatmaiHWSpt to true (i.e. 1) if bit 25 of EDX is
									// a 1 (i.e. the CF flag is a 1).	
	}

	return KatmaiHWSpt;
}

//Checking for Kamai New Instruction emulation support - i.e. a Katmai SDK is on the machine.
BOOL cpuid_KatmaiEmulation()
{
	BOOL KatmaiEmulation = TRUE;	
	
	// constructore executes intrinsicss!!!
	//F32vec4 pNormal = (1.0, 2.0, 3.0, 4.0);
	// ::F32vec4 pZero = 0.0;
	
	//Checking for Katmai New Instruction HW emulation
	__try
	{

		__m128 pNormal,pZero;

		_asm{
			movups  xmm1,[pNormal]	//Issue a move instruction that will cause exception 
									// w/out HW support emulation
			//divps	xmm1,[pZero]	//Issue a computational instruction will cause exception 
			mulps	xmm1,[pZero]	//Issue a computational instruction will cause exception 
									// w/out HW support emulation
			//In addition to the above instructions one might issue
			// a fxsave and fxrstor ASM instruction. Then write a function to determine whether 
			// an exception occurred (i.e. an exception handler that will check to see whether or not
			// the exception that was generated was an Invalid Opcode exception).  Then set the return value
			// of the fucntion accordingly.  This would also be valid for checking actual OS support for
			// Katmai New Instructions.
		}
	}
	//If there's an exception, then set the emulation variable to False.
	__except(EXCEPTION_EXECUTE_HANDLER){KatmaiEmulation = FALSE;}
	
	return KatmaiEmulation;
}



// Initiallize any Katmai state and return true if Katmai support
BOOL KatmaiInit()
{

	union {
		unsigned int xi;
		float xf;
	} tmp;

	KMatrixMult = KatmaiMatrixMult_x87;

	// Set FPU precision control to use 24-bit significand
	// _controlfp( _PC_24, _MCW_PC );
	if (!cpuid_KatmaiEmulation()) return FALSE;
	
	isKatmaiProcessor = TRUE;
	useKatmai=TRUE;

	DWORD scw = _mm_getcsr();
	scw |= 1<<15; // flush to zero bit
	_mm_setcsr(scw);

	//TRACE("Katmai init: setting csr to %x \n",scw);
	// set function pointers
	KMatrixMult = KatmaiMatrixMult_Int;
	KMatrixVec = KatmaiMatVec_int;
	KMatrixVecN = KatmaiMatVecN_int; 

	// init constants 
	tmp.xi = 0x80000000;

	negmask = F32vec4(tmp.xf);

	return TRUE;

}

// enable / disable Katmai Code 
BOOL KatmaiEnable(BOOL flag) 
{
	if (!isKatmaiProcessor) return FALSE;
	if (flag) {
		useKatmai = TRUE;
		KMatrixMult = KatmaiMatrixMult_Int;
		KMatrixVec = KatmaiMatVec_int;
		KMatrixVecN = KatmaiMatVecN_int; 

	} else {
		useKatmai = FALSE;
		KMatrixMult = KatmaiMatrixMult_x87;

	}
	return useKatmai;

}

void KatmaiTerm()
{
}



// set model to screen matrix in Katmai state
void KSetMatrix(const float a[4][4])
{
	state.m.v[0][0] = F32vec4(a[0][0]);
	state.m.v[0][1] = F32vec4(a[0][1]);
	state.m.v[0][2] = F32vec4(a[0][2]);
	state.m.v[0][3] = F32vec4(a[0][3]);

	state.m.v[1][0] = F32vec4(a[1][0]);
	state.m.v[1][1] = F32vec4(a[1][1]);
	state.m.v[1][2] = F32vec4(a[1][2]);
	state.m.v[1][3] = F32vec4(a[1][3]);

	state.m.v[2][0] = F32vec4(a[2][0]);
	state.m.v[2][1] = F32vec4(a[2][1]);
	state.m.v[2][2] = F32vec4(a[2][2]);
	state.m.v[2][3] = F32vec4(a[2][3]);

	state.m.v[3][0] = F32vec4(a[3][0]);
	state.m.v[3][1] = F32vec4(a[3][1]);
	state.m.v[3][2] = F32vec4(a[3][2]);
	state.m.v[3][3] = F32vec4(a[3][3]);



}


// set number of used lights 
void KSetNumLights(int num)
{
	if (num >=0)
		state.numLights = num;
}

// set light type 
void KSetLightType(int i,int type)
{
	state.lightTypes[i] = type;
}



// set light direction for directional lights
void KSetLightDirection(int i,Point dir)
{
	setKVec3(state.lights[i].direction,dir);
	//state.lights1[i].direction = F32vec4(dir.x,dir.y,dir.z,0);
}

// set light position for point / spot lights
void KSetLightPosition(int i,Point pos)
{
	setKVec3(state.lights[i].position,pos);
}

// set light attenuation for point / spot lights
void KSetLightAttenuation(int i,BOOL doAtten,float a0,float a1,float a2)
{
	setKVec3(state.lights[i].attenuation,a0,a1,a2);
	if (doAtten) 
		state.lightTypes[i] |= KATTENUATION_MASK;

}
// set sotlight cone values 
void KSetLightSpotCone(int i, float cosPhi,float cosTheta)
{
	state.lights[i].cosPhi = F32vec4(cosPhi);
	state.lights[i].cosTheta = F32vec4(cosTheta);
}

// set light radius
void KSetLightRadius(int i, float radius)
{
	state.lights[i].radius = F32vec4(radius);
}




// set light colors (pre-multiplied with material color) 
void KSetLightColors(int i,
					 D3DCOLORVALUE ambient,D3DCOLORVALUE diffuse,D3DCOLORVALUE specular,BOOL doSpecular)
{
	setKVec3(state.lights[i].ambient,ambient.r*KRGB_SCALE,ambient.g*KRGB_SCALE,ambient.b*KRGB_SCALE);
	setKVec3(state.lights[i].diffuse,diffuse.r*KRGB_SCALE,diffuse.g*KRGB_SCALE,diffuse.b*KRGB_SCALE);
	setKVec3(state.lights[i].specular,specular.r*KRGB_SCALE,specular.g*KRGB_SCALE,specular.b*KRGB_SCALE);
	
	// for 1 vertex / nurbs lighting
	//state.lights1[i].ambient = F32vec4(ambient.r*KRGB_SCALE,ambient.g*KRGB_SCALE,ambient.b*KRGB_SCALE,0);
	//state.lights1[i].diffuse = F32vec4(diffuse.r*KRGB_SCALE,diffuse.g*KRGB_SCALE,diffuse.b*KRGB_SCALE,0);
	//state.lights1[i].specular = F32vec4(specular.r*KRGB_SCALE,specular.g*KRGB_SCALE,specular.b*KRGB_SCALE,0);

	if (doSpecular)
		state.lightTypes[i] |= KSPECULAR_MASK;
	else 
		state.lightTypes[i] &= ~KSPECULAR_MASK;
}


void KSetMaterial(D3DCOLORVALUE ambient,D3DCOLORVALUE diffuse,D3DCOLORVALUE specular,BOOL doSpecular)
{
}


// set  material emissive color , alpha & power
void KSetBasicMaterial( D3DCOLORVALUE emissive,float alpha,float power)
{
	setKVec3(state.emissive,emissive.r*KRGB_SCALE,emissive.g*KRGB_SCALE,emissive.b*KRGB_SCALE);

	state.emissive_ = F32vec4(emissive.r*KRGB_SCALE,emissive.g*KRGB_SCALE,emissive.b*KRGB_SCALE,0);

	state.alpha = alpha * KRGB_SCALE;
	state.alpha_ = F32vec4(alpha * KRGB_SCALE);
	state.power = F32vec4(power);
	state.power_ = power;

}

// set Linear Fog 
void KSetFogLinear(D3DCOLORVALUE color,float start,float end)
{
	setKVec3(state.fog.color,color.r*KRGB_SCALE,color.g*KRGB_SCALE,color.b*KRGB_SCALE);
	state.fog.start = F32vec4(start);
	state.fog.end = F32vec4(end);
	state.fog.deltaRcp = F32vec4(1.0f/ (end-start));

}

// set Linear Fog off
void KSetFogOff() 
{
}

// set position of viewer in local coordinates
void KSetViewerPosition(Point position)
{
	setKVec3(state.viewer,position);
}





// functions for normalizing normals
// more exact
static inline F32vec4 rsqrt_normal_(__m128 a)
{
		F32vec4 Ra0 = _mm_rsqrt_ps(a);
		return (fvecf0pt5 * Ra0) * (fvecf3pt0 - (a * Ra0) * Ra0);
	
}
// 1/sqrt(x) used for namal normalization
// faster
static inline F32vec4 rsqrt_normal(__m128 a)
{
		return _mm_rsqrt_ps(a);

}

inline __m128 QTransformVertex( __m128 vertx, __m128 verty, __m128 vertz, __m128 vertw, __m128 *pQView )
{
	__m128 result[4];
	result[0] = _mm_mul_ps( vertx, pQView[0] );
	result[1] = _mm_mul_ps( verty, pQView[1] );
	result[2] = _mm_mul_ps( vertz, pQView[2] );
	result[3] = _mm_mul_ps( vertw, pQView[3] );
	return ( _mm_add_ps( _mm_add_ps( _mm_add_ps( result[0], result[1] ), result[2] ), result[3] ) );
}

inline __m128 QNegate( __m128 vec1 )
{
	return _mm_mul_ps( vec1, minusone );
} // DotP_SIMDintrinsics


inline __m128 QClampUpper( __m128 vec )
{
	// a > b ? b : a
	return _mm_min_ps( vec, one );
//	__m128 mask = _mm_cmpgt_ps( vec, QOne );
//	return _mm_or_ps( _mm_and_ps( mask, QOne ), _mm_andnot_ps( mask, vec) );
}

//-----------------------------------------------------------------------------
// DotProduct
//   float dot = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
//-----------------------------------------------------------------------------
static inline __m128 QDotProduct( __m128 vec1, __m128 vec2 )
{
	__m128 result = _mm_mul_ps( vec1, vec2 );
	__m128 immed1 = _mm_unpacklo_ps( result, result );
	__m128 immed2 = _mm_add_ps( immed1, result );
	result = _mm_unpackhi_ps( result, result );
	result = _mm_add_ps( immed2, result );
	result = _mm_shuffle_ps( result, result, _MM_SHUFFLE(2,2,2,2) );
	return result;
} 

//	float distance = (float)sqrt(dv.x*dv.x + dv.y*dv.y + dv.z*dv.z);
static inline __m128 QLength( __m128 dv )
{
	return _mm_sqrt_ps( QDotProduct( dv, dv ) );
}

static inline __m128 QNormalize( __m128 vert )
{
	__m128 result = _mm_mul_ps( vert, vert );
	__m128 immed1 = _mm_unpacklo_ps( result, result );
	__m128 immed2 = _mm_add_ps( immed1, result );

	result = _mm_unpackhi_ps( result, result );
	result = _mm_add_ps( immed2, result );
	result = _mm_shuffle_ps( result, result, _MM_SHUFFLE(2,2,2,2) );

	__m128 Ra0 = _mm_rsqrt_ps( result );
	result = _mm_mul_ps( vert, 
						 _mm_mul_ps( _mm_mul_ps( fvecf0pt5, Ra0 ), 
									 _mm_sub_ps( fvecf3pt0,
												 _mm_mul_ps( _mm_mul_ps( result, Ra0 ), 
															 Ra0 
														   )
											   )
								   )
					   );
	return result;
}

// Implements Graphics Gems IV approximation routine for x^y.
// x^y ~= x / (y - xy + 1)
static inline __m128 QPow( __m128 x, __m128 y )
{
	return _mm_mul_ps( x, _mm_rcp_ps( _mm_add_ps( _mm_sub_ps( y, _mm_mul_ps( x, y ) ), one ) ) );
}

static inline D3DCOLOR QPackColor( __m128 color )
{
	__m128 result;
	long r, g, b;

	// color = _mm_mul_ps( color, twofivefive ); already done

	result = _mm_shuffle_ps( color, color, _MM_SHUFFLE(0,0,0,0) );
	r = _mm_cvtt_ss2si( result );			// truncate or not truncate???
	result = _mm_shuffle_ps( color, color, _MM_SHUFFLE(1,1,1,1) );
	g = _mm_cvtt_ss2si( result );			// truncate or not truncate???
	result = _mm_shuffle_ps( color, color, _MM_SHUFFLE(2,2,2,2) );
	b = _mm_cvtt_ss2si( result );			// truncate or not truncate???
	
	//return (alphaMask | (r << 16) | (g << 8) | b);
	
	return (0xff000000 | (r << 16) | (g << 8) | b);
}




static inline 
void KPackXYZ(__m128 Xout,__m128 Yout,__m128 Zout,__m128 curW,D3DTLVERTEX *vtlp)
{
			// unpack 
		__m128 Clow1  = _mm_unpacklo_ps (Xout, Yout);
		_mm_storel_pi ((__m64*)&((vtlp)->sx), Clow1);
		_mm_storeh_pi ((__m64*)&((vtlp+1)->sx), Clow1);
		__m128 Clow2  = _mm_unpacklo_ps (Zout, curW);		
		_mm_storel_pi ((__m64*)&((vtlp)->sz), Clow2);
		_mm_storeh_pi ((__m64*)&((vtlp+1)->sz ), Clow2);


		__m128 Chigh1 = _mm_unpackhi_ps (Xout, Yout);
		__m128 Chigh2 = _mm_unpackhi_ps (Zout, curW);

//		_mm_storel_pi ((__m64*)&((vtlp+2)->sx), Chigh1);
//		_mm_storeh_pi ((__m64*)&((vtlp+3)->sx), Chigh1);
//		_mm_storel_pi ((__m64*)&((vtlp+2)->sz), Chigh2);
//		_mm_storeh_pi ((__m64*)&((vtlp+3)->sz), Chigh2);

		__m128 Cf1 = _mm_shuffle_ps (Chigh1, Chigh2, _MM_SHUFFLE(1,0,1,0)); 
		__m128 Cf2 = _mm_shuffle_ps (Chigh1, Chigh2, _MM_SHUFFLE(3,2,3,2));

//		_mm_stream_ps (&((vtlp+2)->sx), Cf1); 
//		_mm_stream_ps (&((vtlp+3)->sx), Cf2); 

		_mm_store_ps (&((vtlp+2)->sx), Cf1);  // keep in cache 
		_mm_store_ps (&((vtlp+3)->sx), Cf2); 
}

static inline 
void KPackColor(__m128 r,__m128 g,__m128 b,__m128 a,
							  D3DTLVERTEX *vtlp)
{
 // we  rgba == argb
	__m128 Cint1, Cint2, Cint3, Cint4; 
	__m128 r1, r2, r3, r4; 

	__m64 mm4, mm5, mm0; 

	Cint1 = _mm_shuffle_ps (r, a, SELECT (1,0,1,0) );  // a2,a1, r2 r1
	Cint2 = _mm_shuffle_ps (b, g, SELECT (1,0,1,0) ); // g2 g1 b2 b1 

//	Cint1 = _mm_shuffle_ps (a, r, SELECT (1,0,1,0) );  // a2,a1, r2 r1
//	Cint2 = _mm_shuffle_ps (g, b, SELECT (1,0,1,0) ); // g2 g1 b2 b1 

	r1 = _mm_shuffle_ps (Cint1, Cint2, SELECT (2,0,2,0) );  // a r g b (1)

	r2 = _mm_shuffle_ps (Cint1, Cint2, SELECT (3,1,3,1) ); 

//	Cint3 = _mm_shuffle_ps (a, r, SELECT (3,2,3,2) );
//	Cint4 = _mm_shuffle_ps (g, b, SELECT (3,2,3,2) );
	Cint3 = _mm_shuffle_ps (r, a, SELECT (3,2,3,2) );
	Cint4 = _mm_shuffle_ps (b, g, SELECT (3,2,3,2) );

	r3 = _mm_shuffle_ps (Cint3, Cint4, SELECT (2,0,2,0) ); 
	r4 = _mm_shuffle_ps (Cint3, Cint4, SELECT (3,1,3,1) ); 

	mm4 = _mm_cvtt_ps2pi (r1); // mm4 = g1 b1
	
	r1 = _mm_shuffle_ps (r1, r1, SELECT(3,2,3,2) ); 

	mm5 = _mm_cvtt_ps2pi (r1);
//m4 = _m_packssdw (mm4, mm5);  // 16 bit A R G B
	mm4 = _m_packssdw (mm5, mm4);  // 16 bit A R G B
	mm0 = _m_packuswb (mm4, mm4);  // 8BIT A R G B 
	(vtlp+0)->color = _m_to_int (mm0); 

	mm4 = _mm_cvtt_ps2pi (r2);
	r2 = _mm_shuffle_ps (r2, r2, SELECT(3,2,3,2) ); 
	mm5 = _mm_cvtt_ps2pi (r2);
//m4 = _m_packssdw (mm4, mm5); 
	mm4 = _m_packssdw (mm5, mm4);  // 16 bit A R G B
	mm0 = _m_packuswb (mm4, mm4); 
	(vtlp+1)->color = _m_to_int (mm0);
	
	mm4 = _mm_cvtt_ps2pi (r3);
	r3 = _mm_shuffle_ps (r3, r3, SELECT(3,2,3,2) ); 
	mm5 = _mm_cvtt_ps2pi (r3);
//	mm4 = _m_packssdw (mm4, mm5); 
	mm4 = _m_packssdw (mm5, mm4);  // 16 bit A R G B

	mm0 = _m_packuswb (mm4, mm4); 
	(vtlp+2)->color = _m_to_int (mm0);

	mm4 = _mm_cvtt_ps2pi (r4);
	r4 = _mm_shuffle_ps (r4, r4, SELECT(3,2,3,2) ); 
	mm5 = _mm_cvtt_ps2pi (r4);
//	mm4 = _m_packssdw (mm4, mm5); 
	mm4 = _m_packssdw (mm5, mm4);  // 16 bit A R G B

	mm0 = _m_packuswb (mm4, mm4); 
	(vtlp+3)->color = _m_to_int (mm0);

}
// nurbs version, vtlp stepping 2
static inline 
void KPackColor2(__m128 r,__m128 g,__m128 b,__m128 a,
							  D3DTLVERTEX *vtlp)
{
	__m128 Cint1, Cint2, Cint3, Cint4; 
	__m128 r1, r2, r3, r4; 

	__m64 mm4, mm5, mm0; 

	Cint1 = _mm_shuffle_ps (r, a, SELECT (1,0,1,0) );  // a2,a1, r2 r1
	Cint2 = _mm_shuffle_ps (b, g, SELECT (1,0,1,0) ); // g2 g1 b2 b1 

	r1 = _mm_shuffle_ps (Cint1, Cint2, SELECT (2,0,2,0) );  // a r g b (1)

	r2 = _mm_shuffle_ps (Cint1, Cint2, SELECT (3,1,3,1) ); 

	Cint3 = _mm_shuffle_ps (r, a, SELECT (3,2,3,2) );
	Cint4 = _mm_shuffle_ps (b, g, SELECT (3,2,3,2) );

	r3 = _mm_shuffle_ps (Cint3, Cint4, SELECT (2,0,2,0) ); 
	r4 = _mm_shuffle_ps (Cint3, Cint4, SELECT (3,1,3,1) ); 

	mm4 = _mm_cvtt_ps2pi (r1); // mm4 = g1 b1
	
	r1 = _mm_shuffle_ps (r1, r1, SELECT(3,2,3,2) ); 

	mm5 = _mm_cvtt_ps2pi (r1);
	mm4 = _m_packssdw (mm5, mm4);  // 16 bit A R G B
	mm0 = _m_packuswb (mm4, mm4);  // 8BIT A R G B 
	(vtlp+0)->color = _m_to_int (mm0); 

	mm4 = _mm_cvtt_ps2pi (r2);
	r2 = _mm_shuffle_ps (r2, r2, SELECT(3,2,3,2) ); 
	mm5 = _mm_cvtt_ps2pi (r2);
	mm4 = _m_packssdw (mm5, mm4);  // 16 bit A R G B
	mm0 = _m_packuswb (mm4, mm4); 
	(vtlp+2)->color = _m_to_int (mm0);
	
	mm4 = _mm_cvtt_ps2pi (r3);
	r3 = _mm_shuffle_ps (r3, r3, SELECT(3,2,3,2) ); 
	mm5 = _mm_cvtt_ps2pi (r3);
	mm4 = _m_packssdw (mm5, mm4);  // 16 bit A R G B

	mm0 = _m_packuswb (mm4, mm4); 
	(vtlp+4)->color = _m_to_int (mm0);

	mm4 = _mm_cvtt_ps2pi (r4);
	r4 = _mm_shuffle_ps (r4, r4, SELECT(3,2,3,2) ); 
	mm5 = _mm_cvtt_ps2pi (r4);
	mm4 = _m_packssdw (mm5, mm4);  // 16 bit A R G B

	mm0 = _m_packuswb (mm4, mm4); 
	(vtlp+6)->color = _m_to_int (mm0);

}



static inline 
void KPackSpecularColor(__m128 r,__m128 g,__m128 b,__m128 a,
							  D3DTLVERTEX *vtlp)
{
 // we  rgba == argb
	__m128 Cint1, Cint2, Cint3, Cint4; 
	__m128 r1, r2, r3, r4; 

	__m64 mm4, mm5, mm0; 

	Cint1 = _mm_shuffle_ps (r, a, SELECT (1,0,1,0) );  // a2,a1, r2 r1
	Cint2 = _mm_shuffle_ps (b, g, SELECT (1,0,1,0) ); // g2 g1 b2 b1 

//	Cint1 = _mm_shuffle_ps (a, r, SELECT (1,0,1,0) );  // a2,a1, r2 r1
//	Cint2 = _mm_shuffle_ps (g, b, SELECT (1,0,1,0) ); // g2 g1 b2 b1 

	r1 = _mm_shuffle_ps (Cint1, Cint2, SELECT (2,0,2,0) );  // a r g b (1)

	r2 = _mm_shuffle_ps (Cint1, Cint2, SELECT (3,1,3,1) ); 

//	Cint3 = _mm_shuffle_ps (a, r, SELECT (3,2,3,2) );
//	Cint4 = _mm_shuffle_ps (g, b, SELECT (3,2,3,2) );
	Cint3 = _mm_shuffle_ps (r, a, SELECT (3,2,3,2) );
	Cint4 = _mm_shuffle_ps (b, g, SELECT (3,2,3,2) );

	r3 = _mm_shuffle_ps (Cint3, Cint4, SELECT (2,0,2,0) ); 
	r4 = _mm_shuffle_ps (Cint3, Cint4, SELECT (3,1,3,1) ); 

	mm4 = _mm_cvtt_ps2pi (r1); // mm4 = g1 b1
	
	r1 = _mm_shuffle_ps (r1, r1, SELECT(3,2,3,2) ); 

	mm5 = _mm_cvtt_ps2pi (r1);
//m4 = _m_packssdw (mm4, mm5);  // 16 bit A R G B
	mm4 = _m_packssdw (mm5, mm4);  // 16 bit A R G B
	mm0 = _m_packuswb (mm4, mm4);  // 8BIT A R G B 
	(vtlp+0)->specular = _m_to_int (mm0); 

	mm4 = _mm_cvtt_ps2pi (r2);
	r2 = _mm_shuffle_ps (r2, r2, SELECT(3,2,3,2) ); 
	mm5 = _mm_cvtt_ps2pi (r2);
//m4 = _m_packssdw (mm4, mm5); 
	mm4 = _m_packssdw (mm5, mm4);  // 16 bit A R G B
	mm0 = _m_packuswb (mm4, mm4); 
	(vtlp+1)->specular = _m_to_int (mm0);
	
	mm4 = _mm_cvtt_ps2pi (r3);
	r3 = _mm_shuffle_ps (r3, r3, SELECT(3,2,3,2) ); 
	mm5 = _mm_cvtt_ps2pi (r3);
//	mm4 = _m_packssdw (mm4, mm5); 
	mm4 = _m_packssdw (mm5, mm4);  // 16 bit A R G B

	mm0 = _m_packuswb (mm4, mm4); 
	(vtlp+2)->specular = _m_to_int (mm0);

	mm4 = _mm_cvtt_ps2pi (r4);
	r4 = _mm_shuffle_ps (r4, r4, SELECT(3,2,3,2) ); 
	mm5 = _mm_cvtt_ps2pi (r4);
//	mm4 = _m_packssdw (mm4, mm5); 
	mm4 = _m_packssdw (mm5, mm4);  // 16 bit A R G B

	mm0 = _m_packuswb (mm4, mm4); 
	(vtlp+3)->specular = _m_to_int (mm0);

}
// NURBS version 
static inline 
void KPackSpecularColor2(__m128 r,__m128 g,__m128 b,__m128 a,
							  D3DTLVERTEX *vtlp)
{
 // we  rgba == argb
	__m128 Cint1, Cint2, Cint3, Cint4; 
	__m128 r1, r2, r3, r4; 

	__m64 mm4, mm5, mm0; 

	Cint1 = _mm_shuffle_ps (r, a, SELECT (1,0,1,0) );  // a2,a1, r2 r1
	Cint2 = _mm_shuffle_ps (b, g, SELECT (1,0,1,0) ); // g2 g1 b2 b1 


	r1 = _mm_shuffle_ps (Cint1, Cint2, SELECT (2,0,2,0) );  // a r g b (1)

	r2 = _mm_shuffle_ps (Cint1, Cint2, SELECT (3,1,3,1) ); 

	Cint3 = _mm_shuffle_ps (r, a, SELECT (3,2,3,2) );
	Cint4 = _mm_shuffle_ps (b, g, SELECT (3,2,3,2) );

	r3 = _mm_shuffle_ps (Cint3, Cint4, SELECT (2,0,2,0) ); 
	r4 = _mm_shuffle_ps (Cint3, Cint4, SELECT (3,1,3,1) ); 

	mm4 = _mm_cvtt_ps2pi (r1); // mm4 = g1 b1
	
	r1 = _mm_shuffle_ps (r1, r1, SELECT(3,2,3,2) ); 

	mm5 = _mm_cvtt_ps2pi (r1);
	mm4 = _m_packssdw (mm5, mm4);  // 16 bit A R G B
	mm0 = _m_packuswb (mm4, mm4);  // 8BIT A R G B 
	(vtlp+0)->specular = _m_to_int (mm0); 

	mm4 = _mm_cvtt_ps2pi (r2);
	r2 = _mm_shuffle_ps (r2, r2, SELECT(3,2,3,2) ); 
	mm5 = _mm_cvtt_ps2pi (r2);
	mm4 = _m_packssdw (mm5, mm4);  // 16 bit A R G B
	mm0 = _m_packuswb (mm4, mm4); 
	(vtlp+2)->specular = _m_to_int (mm0);
	
	mm4 = _mm_cvtt_ps2pi (r3);
	r3 = _mm_shuffle_ps (r3, r3, SELECT(3,2,3,2) ); 
	mm5 = _mm_cvtt_ps2pi (r3);
	mm4 = _m_packssdw (mm5, mm4);  // 16 bit A R G B

	mm0 = _m_packuswb (mm4, mm4); 
	(vtlp+4)->specular = _m_to_int (mm0);

	mm4 = _mm_cvtt_ps2pi (r4);
	r4 = _mm_shuffle_ps (r4, r4, SELECT(3,2,3,2) ); 
	mm5 = _mm_cvtt_ps2pi (r4);
	mm4 = _m_packssdw (mm5, mm4);  // 16 bit A R G B

	mm0 = _m_packuswb (mm4, mm4); 
	(vtlp+6)->specular = _m_to_int (mm0);

}





// diffuse lighting for directional light
static inline 
void KLightDiffuse(KLightState *l,
				   F32vec4 nx,F32vec4 ny,F32vec4 nz,
				   KVec3 &color
				   ) 
{

	F32vec4 f =	nx * l->direction.x + ny * l->direction.y + nz * l->direction.z ;

	//if ((allNegative(f)) return;
	f=simd_max(f,zero);

	// addin diffuse part 

	color.x += l->diffuse.x*f,color.y += l->diffuse.y*f,color.z += l->diffuse.z*f;

}

static inline 
void KLightDiffuse(KLightState1 *l,
				   F32vec4 n,
				   F32vec4 &color
				   ) 
{

	// Now see what is the angle between the vertex normal and light.
	__m128 f = QNegate( QDotProduct(l->direction, n) );
	
	// if ( _mm_comile_ss( f, zero )) return;
	f=simd_max(f,zero);

	//if ((allNegative(f)) return;
	f = QClampUpper( f);

	//f=simd_max(f,zero);
	// addin diffuse part 
	color = _mm_add_ps( color, _mm_mul_ps( l->diffuse, f ));

}

// approximation for shininess
#define SPOW(dot,shin) (dot*rcp(shin-shin*(dot)+(dot)))

// diffuse/spec lighting for directional light
static inline 
void KLightDiffuseSpecular(KLightState *l,

								   F32vec4 nx,F32vec4 ny,F32vec4 nz,
								   F32vec4 toViewerx, F32vec4 toViewery, 
								   F32vec4 toViewerz,
								   F32vec4 &power,
								   KVec3 &color,
								   KVec3 &specularColor
								   ) 
{

	F32vec4 f =	nx * l->direction.x + ny * l->direction.y + nz * l->direction.z ;

	f=simd_max(f,zero);

	// addin diffuse part 

	color.x += l->diffuse.x*f,color.y += l->diffuse.y*f,color.z += l->diffuse.z*f;


	F32vec4 tmpx = l->direction.x + toViewerx;
	F32vec4 tmpy = l->direction.y + toViewery;
	F32vec4 tmpz = l->direction.z + toViewerz;



	// normalize (tmpx,tmpy,tmpz) ***
	f = tmpx*tmpx;
	f += tmpy*tmpy;
	f += tmpz*tmpz;
	f = rsqrt_normal(f);

	// tmpx *=f,tmpy *=f,tmpz *=f;


	f = f * (nx * tmpx + ny * tmpy + nz * tmpz); // dotprod & normalize
		
		//if ((allNegative(f)) return;


		f=simd_max(f,zero);


		f= SPOW(f,power);

		// add in specularColor part 
		//f=simd_min(f,one); // NAN problem without ???????????

		specularColor.x += l->specular.x*f;
		specularColor.y += l->specular.y*f;
		specularColor.z += l->specular.z*f;

}


// diffuse specular lighting for pointlight
static inline 
void KLightPointDiffuse(KLightState *l,
								   F32vec4 px,F32vec4 py,F32vec4 pz,
								   F32vec4 nx,F32vec4 ny,F32vec4 nz,
								   F32vec4 toViewerx, F32vec4 toViewery, 
								   F32vec4 toViewerz,
								   F32vec4 &power,
								   KVec3 &color,
								   KVec3 &specularColor
								   ) 
{

	KVec3 toLight;
	toLight.x = l->position.x - px;
	toLight.y = l->position.y - py;
	toLight.z = l->position.z - pz;
	
	// normalize (toLight.x,toLight.y,toLight.z) ***
	F32vec4 f = toLight.x*toLight.x;
	f += toLight.y*toLight.y;
	f += toLight.z*toLight.z;
	f = rsqrt_normal(f);

	// to do: clamp with light radius and exit

	// normalize to light
	toLight.x *=f,toLight.y *=f,toLight.z *=f;

	// diffuse part 
	f =	nx * toLight.x + ny * toLight.y + nz * toLight.z ;


	f=simd_max(f,zero);

	// addin diffuse part 

	color.x += l->diffuse.x*f,color.y += l->diffuse.y*f,color.z += l->diffuse.z*f;

	// ambient 
	color.x += l->ambient.x,color.y += l->ambient.y,color.z += l->ambient.z;

/*

	// specular part 
	F32vec4 tmpx = toLight.x + toViewerx;
	F32vec4 tmpy = toLight.y + toViewery;
	F32vec4 tmpz = toLight.z + toViewerz;


	// normalize (tmpx,tmpy,tmpz) ***
	f = tmpx*tmpx;
	f += tmpy*tmpy;
	f += tmpz*tmpz;
	f = rsqrt_normal(f);
	// tmpx *=f,tmpy *=f,tmpz *=f;


		f = f * (nx * tmpx + ny * tmpy + nz * tmpz); // dotprod
	
		//if ((allNegative(f)) return;

		f=simd_max(f,zero);


		f= SPOW(f,power);

		// add in specularColor part 

		specularColor.x += l->specular.x*f,specularColor.y += l->specular.y*f,specularColor.z += l->specular.z*f;
*/

}

// diffuse specular lighting for pointlight
static inline 
void KLightPointDiffuseSpecular(KLightState *l,
								   F32vec4 px,F32vec4 py,F32vec4 pz,
								   F32vec4 nx,F32vec4 ny,F32vec4 nz,
								   F32vec4 toViewerx, F32vec4 toViewery, 
								   F32vec4 toViewerz,
								   F32vec4 &power,
								   KVec3 &color,
								   KVec3 &specularColor
								   ) 
{

	KVec3 toLight;
	toLight.x = l->position.x - px;
	toLight.y = l->position.y - py;
	toLight.z = l->position.z - pz;
	
	// normalize (toLight.x,toLight.y,toLight.z) ***
	F32vec4 f = toLight.x*toLight.x;
	f += toLight.y*toLight.y;
	f += toLight.z*toLight.z;
	f = rsqrt_normal(f);

	// to do: clamp with light radius and exit

	// normalize to light
	toLight.x *=f,toLight.y *=f,toLight.z *=f;

	// diffuse part 
	f =	nx * toLight.x + ny * toLight.y + nz * toLight.z ;


	f=simd_max(f,zero);

	// addin diffuse part 

	color.x += l->diffuse.x*f,color.y += l->diffuse.y*f,color.z += l->diffuse.z*f;

	// ambient 
	color.x += l->ambient.x,color.y += l->ambient.y,color.z += l->ambient.z;

	// specular part 
	F32vec4 tmpx = toLight.x + toViewerx;
	F32vec4 tmpy = toLight.y + toViewery;
	F32vec4 tmpz = toLight.z + toViewerz;


	// normalize (tmpx,tmpy,tmpz) ***
	f = tmpx*tmpx;
	f += tmpy*tmpy;
	f += tmpz*tmpz;
	f = rsqrt_normal(f);
	// tmpx *=f,tmpy *=f,tmpz *=f;


		f = f * (nx * tmpx + ny * tmpy + nz * tmpz); // dotprod
	
		//if ((allNegative(f)) return;

		f=simd_max(f,zero);


		f= SPOW(f,power);

		// add in specularColor part 

		specularColor.x += l->specular.x*f,specularColor.y += l->specular.y*f,specularColor.z += l->specular.z*f;


}

static inline 
void KLightPointDiffuseSpecularAtt(KLightState *l,
								   F32vec4 px,F32vec4 py,F32vec4 pz,
								   F32vec4 nx,F32vec4 ny,F32vec4 nz,
								   F32vec4 toViewerx, F32vec4 toViewery, 
								   F32vec4 toViewerz,
								   F32vec4 &power,
								   KVec3 &colorOut,
								   KVec3 &specularColor,
								   bool doSpecular
								   ) 
{

	// compute vector to light position 
	KVec3 toLight;

	toLight.x = l->position.x - px;
	toLight.y = l->position.y - py;
	toLight.z = l->position.z - pz;
	
	F32vec4 dist;
	KVec3 color;

	// compute distance and  normalize (toLight.x,toLight.y,toLight.z) ***
	F32vec4 f = toLight.x*toLight.x;
	f += toLight.y*toLight.y;
	f += toLight.z*toLight.z;

	F32vec4 atten;
	
	if (1) {
		dist = sqrt(f); // costly ?

		// attenuation
		atten  = l->attenuation.x+ dist* (l->attenuation.y + l->attenuation.z * dist);
		atten = simd_min(atten,one);
		atten = rcp_nr(atten);

		// to do: clamp with light radius and exit
		//atten *=  select_lt(dist,l->radius,one,zero);

		// exit if atten 0 
		// f = rcp_nr(dist);
		f=rsqrt_normal(f);	
	} else {
		atten = one;
		f=rsqrt_normal(f);	
	}


	// normalze to light
	toLight.x *=f,toLight.y *=f,toLight.z *=f;



	// diffuse part 
	f =	nx * toLight.x + ny * toLight.y + nz * toLight.z ;

	f=simd_max(f,zero);

	// addin diffuse part 

	color.x = l->diffuse.x*f,color.y = l->diffuse.y*f,color.z = l->diffuse.z*f;

	// ambient 
	color.x += l->ambient.x,color.y += l->ambient.y,color.z += l->ambient.z;


    // add to out color 
	colorOut.x += atten * color.x;
	colorOut.y += atten * color.y;
	colorOut.z += atten * color.z;

	
	// specular part 
	if (doSpecular) {
	F32vec4 tmpx = toLight.x + toViewerx;
	F32vec4 tmpy = toLight.y + toViewery;
	F32vec4 tmpz = toLight.z + toViewerz;





	// normalize (tmpx,tmpy,tmpz) ***
	f = tmpx*tmpx;
	f += tmpy*tmpy;
	f += tmpz*tmpz;
	f = rsqrt_normal(f);
	tmpx *=f,tmpy *=f,tmpz *=f;


		f = nx * tmpx + ny * tmpy + nz * tmpz; // dotprod

		f=simd_max(f,zero);


		f= SPOW(f,power);

		// add in specularColor part 
		f *= atten;

		specularColor.x += l->specular.x*f,specularColor.y += l->specular.y*f,specularColor.z += l->specular.z*f;
	}

}
								   
								   
static inline 

void KLightSpotDiffuseSpecularAtt(KLightState *l,
								   F32vec4 px,F32vec4 py,F32vec4 pz,
								   F32vec4 nx,F32vec4 ny,F32vec4 nz,
								   F32vec4 toViewerx, F32vec4 toViewery, 
								   F32vec4 toViewerz,
								   F32vec4 power,
								   KVec3 &colorOut,
								   KVec3 &specularColor,
								   bool doSpecular
								   ) 
{

	// compute vector to light position 
	KVec3 toLight;

	toLight.x = l->position.x - px;
	toLight.y = l->position.y - py;
	toLight.z = l->position.z - pz;
	
	F32vec4 dist;
	KVec3 color;

	// compute distance and  normalize (toLight.x,toLight.y,toLight.z) ***
	F32vec4 f = toLight.x*toLight.x;
	f += toLight.y*toLight.y;
	f += toLight.z*toLight.z;

	F32vec4 atten;
	
	if (0) {
		dist = sqrt(f); // costly ?

		// attenuation
		atten  = l->attenuation.x+ dist* (l->attenuation.y + l->attenuation.z * dist);
		atten = simd_min(atten,one);
		atten = rcp_nr(atten);

		// to do: clamp with light radius and exit
		//atten *=  select_lt(dist,l->radius,one,zero);

		// exit if atten 0 
		// f = rcp_nr(dist);
		f=rsqrt_normal(f);	
	} else {
		atten = one;
		f=rsqrt_normal(f);	
	}


	// normalize to light
	toLight.x *=f,toLight.y *=f,toLight.z *=f;


	// spot part 
	F32vec4 spotdot  =	(toLight.x * l->direction.x + toLight.y * l->direction.y + toLight.z * l->direction.z);

	F32vec4 fac= select_lt(spotdot,l->cosPhi,zero,one);

	// to to inner angle 
	atten *= fac;



	// diffuse part 
	f =	nx * toLight.x + ny * toLight.y + nz * toLight.z ;

	f=simd_max(f,zero);

	// addin diffuse part 

	color.x = l->diffuse.x*f,color.y = l->diffuse.y*f,color.z = l->diffuse.z*f;

	// ambient 
	color.x += l->ambient.x,color.y += l->ambient.y,color.z += l->ambient.z;


    // add to out color 
	colorOut.x += atten * color.x;
	colorOut.y += atten * color.y;
	colorOut.z += atten * color.z;

	
	// specular part 
	if (doSpecular) {
	F32vec4 tmpx = toLight.x + toViewerx;
	F32vec4 tmpy = toLight.y + toViewery;
	F32vec4 tmpz = toLight.z + toViewerz;





	// normalize (tmpx,tmpy,tmpz) ***
	f = tmpx*tmpx;
	f += tmpy*tmpy;
	f += tmpz*tmpz;
	f = rsqrt_normal(f);
	tmpx *=f,tmpy *=f,tmpz *=f;


		f = nx * tmpx + ny * tmpy + nz * tmpz; // dotprod

		f=simd_max(f,zero);


		f= SPOW(f,power);

		// add in specularColor part 
		f *= atten;

		specularColor.x += l->specular.x*f,specularColor.y += l->specular.y*f,specularColor.z += l->specular.z*f;
	}

}






static inline 

void KFogLinear(const KFogState *l,
								   F32vec4 dist,
								   F32vec4 &fogFactor,
								   
								   KVec3 &color,
								   KVec3 &specularColor
				)
{
	
	// fog stored in a part of specularColor

	F32vec4 d = l->end-dist;
	d = d * l->deltaRcp;
	d = simd_min(d,one);
	d = simd_max(d,zero);
	fogFactor = d*twofivefive;

/*
	F32vec4 d = dist-l->start;
	F32vec4 d1=one-d;
	color.x = d1*color.x + l->color.x*d;
	specularColor.x = d1*specularColor.x + l->color.x*d;
	color.y = d1*color.y + l->color.y*d;
	specularColor.y = d1*specularColor.y + l->color.y*d;
	color.z = d1*color.z + l->color.z*d;
	specularColor.z = d1*specularColor.z + l->color.z*d;
*/
}


// generic light routine 

static inline 
void KLightGenericDiffuseSpecular(
								   int ltype,	
								   F32vec4 zdist, // for fog 
								   KLightState *l,
								   F32vec4 px,F32vec4 py,F32vec4 pz,
								   F32vec4 nx,F32vec4 ny,F32vec4 nz,
								   F32vec4 toViewerx, F32vec4 toViewery, 
								   F32vec4 toViewerz,
								   F32vec4 &power,
								   KVec3 &color,
								   KVec3 &specularColor,
								   F32vec4 &fogFactor
								   ) 
{
	switch (ltype) {
	case  KDIFFUSE_LIGHT :
			KLightDiffuse(l,nx,ny,nz,color); 
			color.x += l->ambient.x,color.y += l->ambient.y,color.z += l->ambient.z;
		break;
	case  (KDIFFUSE_LIGHT | KSPECULAR_MASK) :
			KLightDiffuseSpecular(l,nx,ny,nz,toViewerx,toViewery,toViewerz,power,color,specularColor); 
			color.x += l->ambient.x,color.y += l->ambient.y,color.z += l->ambient.z;
		break;

	case  KPOINT_LIGHT : 
		  KLightPointDiffuse(l,px,py,pz,nx,ny,nz,toViewerx,toViewery,toViewerz,power,color,specularColor); 
		  break;

	case  (KPOINT_LIGHT | KSPECULAR_MASK) :
		  KLightPointDiffuseSpecular(l,px,py,pz,nx,ny,nz,toViewerx,toViewery,toViewerz,power,color,specularColor); 
		  break;

	case  (KPOINT_LIGHT | KATTENUATION_MASK) :
	case  (KPOINT_LIGHT | KSPECULAR_MASK | KATTENUATION_MASK) :
			// KLightPointDiffuseSpecular(l,px,py,pz,nx,ny,nz,toViewerx,toViewery,toViewerz,state.power,color,specularColor); 
			KLightPointDiffuseSpecularAtt(l,px,py,pz,nx,ny,nz,toViewerx,toViewery,toViewerz,power,color,specularColor,true); 
		break;

	case  KSPOT_LIGHT :
	case  (KSPOT_LIGHT | KSPECULAR_MASK) :
	case  (KSPOT_LIGHT | KATTENUATION_MASK) :
	case  (KSPOT_LIGHT | KSPECULAR_MASK | KATTENUATION_MASK) :

		KLightSpotDiffuseSpecularAtt(l,px,py,pz,nx,ny,nz,toViewerx,toViewery,toViewerz,power,color,specularColor,true); 
		break;
	case KFOG_LINEAR : {
			KFogLinear(&state.fog,zdist,fogFactor,color,specularColor);
			break;
			}
#ifdef _DEBUG
	default :
			_m_empty(); 
			assert(0);
			break;
#endif
	
	}
}




/*

transform with normalization
4 * x 4* y  a time

*/

void KatmaiMult(int n, F32vec4 *x, F32vec4 *y, F32vec4 *z,float m[4][4])
{
	F32vec4 mat30 = m[3][0], mat31 = m[3][1];
	F32vec4 mat32 = m[3][2], mat33 = m[3][3];
	F32vec4 mat00 = m[0][0], mat01 = m[0][1];
	F32vec4 mat02 = m[0][2], mat03 = m[0][3];
	F32vec4 mat10 = m[1][0], mat11 = m[1][1];
	F32vec4 mat12 = m[1][2], mat13 = m[1][3];
	F32vec4 mat20 = m[2][0], mat21 = m[2][1];
	F32vec4 mat22 = m[2][2], mat23 = m[2][3];
	
	F32vec4 VX_X  = (1., 1., 1., 1.), VX_Y  = (1., 1., 1., 1.), VX_Z  = (1., 1., 1., 1.);
	F32vec4 One   = (1., 1., 1., 1.);
	F32vec4 sum4  = (0., 0., 0., 0.);
	F32vec4 curX, curY, curZ, curW; 
	F32vec4 Xout,       Yout,       Zout;	
	
    for (size_t i=0; i<n; i++) 
	{
			// _mm_prefetch((char *)(&x[i+2]), _MM_HINT_NTA); 
		    // _mm_prefetch((char *)(&y[i+2]), _MM_HINT_NTA); 
		    // _mm_prefetch((char *)(&z[i+2]), _MM_HINT_NTA);

			curX = x[i]; 
			curY = y[i]; 
			curZ = z[i];
			curW = (One/((curX*mat30)+(curY*mat31)+(curZ*mat32)+mat33));
	
			Xout=((((curX*mat00)+(curY*mat01)+(curZ*mat02)+mat03)*curW)+VX_X);
			Yout=((((curX*mat10)+(curY*mat11)+(curZ*mat12)+mat13)*curW)+VX_Y);
			Zout=((((curX*mat20)+(curY*mat21)+(curZ*mat22)+mat23)*curW)+VX_Z);

			//Output.x[i]=Xout; Output.y[i]=Yout; Output.z[i]=Zout; 
			//Output.w[i]=curW;
		}
		
}		



//-----------------------------------------------------------------------------
// MatrixMult_SIMDIntrinsics - Matrix multiply using the Fvec32 class and SIMD
// intrinsics.  Multiply two 4x4 matricies (remember, D3DMATRIX is a 4x4) by 
// one another using the Fvec class for SIMD intrinsics.  You'll notice we're 
// really doing this slightly backwards, since we're transversing the b matrix.
//
// Inputs: a, b (both D3DMATIX)
// Outputs: Result.ret (the result matrix)
// Return Value: Result.ret (the result matrix)
//-----------------------------------------------------------------------------
/*
    for (int i = 0; i < 4; i++)	{
        ret.x[i][0] = A.x[i][0] * B.x[0][0] + A.x[i][1] * B.x[1][0] + A.x[i][2] * B.x[2][0] + A.x[i][3] * B.x[3][0] ;
        ret.x[i][1] = A.x[i][0] * B.x[0][1] + A.x[i][1] * B.x[1][1] + A.x[i][2] * B.x[2][1] + A.x[i][3] * B.x[3][1] ;
        ret.x[i][2] = A.x[i][0] * B.x[0][2] + A.x[i][1] * B.x[1][2] + A.x[i][2] * B.x[2][2] + A.x[i][3] * B.x[3][2] ;
        ret.x[i][3] = A.x[i][0] * B.x[0][3] + A.x[i][1] * B.x[1][3] + A.x[i][2] * B.x[2][3] + A.x[i][3] * B.x[3][3] ;
	}
*/
void  KatmaiMatrixMult_Int( /*const*/ float a[4][4], /*const*/ float b[4][4], float ret[4][4] )
{
	F32vec4 fa[4],fb;
	int i;

	for( i=0; i<4; i++ )
		fa[i] = _mm_loadu_ps( (float *)(&b[i][0]) );


	__m128 fret[4];

	for( i=0; i<4; i++ )
	{
		__m128 tmp;
		__m128 fb;
		fb = _mm_load_ss((float *) &a[i][0]);
		tmp = _mm_mul_ps( fa[0],_mm_shuffle_ps( fb, fb, 0 ));
		fb = _mm_load_ss((float *) &a[i][1]);
		tmp = _mm_add_ps( tmp,_mm_mul_ps( fa[1],_mm_shuffle_ps( fb, fb, 0 )));
		fb = _mm_load_ss((float *) &a[i][2]);
		tmp = _mm_add_ps( tmp,_mm_mul_ps( fa[2],_mm_shuffle_ps( fb, fb, 0 )));
		fb = _mm_load_ss((float *) &a[i][3]);
		tmp = _mm_add_ps( tmp,_mm_mul_ps( fa[3],_mm_shuffle_ps( fb, fb, 0 )));
		fret[i]=tmp;
	}

	for( i=0; i<4; i++ )
		_mm_storeu_ps(&ret[i][0],fret[i]);


/*
	// Compute the result
	for( i=0; i<4; i++ )
	{
			fb = _mm_load_ss( (float *) &b[i][0] );
			fret =  _mm_mul_ps( fa[0],_mm_shuffle_ps( fb, fb, 0 ) );
			fb = _mm_load_ss( (float *) &b[i][1] );
			fret =  _mm_add_ps( fret,_mm_mul_ps( fa[1],_mm_shuffle_ps( fb, fb, 0 ) ) );
			fb = _mm_load_ss( (float *) &b[i][2] );
			fret =  _mm_add_ps( fret,_mm_mul_ps( fa[2],_mm_shuffle_ps( fb, fb, 0 ) ) );
			fb = _mm_load_ss( (float *) &b[i][3] );
			fret =  _mm_add_ps( fret,_mm_mul_ps( fa[3],_mm_shuffle_ps( fb, fb, 0 ) ) );
			_mm_storeu_ps( (float *)(&ret[i][0]),fret);

	}
*/
} 


//-----------------------------------------------------------------------------
// MatrixMult_SIMDASM - Matrix multiply using Katmai assembly code.  Multiply 
// two 4x4 matricies (remember, D3DMATRIX is a 4x4) by one another using the 
// Fvec class for SIMD intrinsics.  You'll notice we're really doing this 
// slightly backwards, since we're transversing the b matrix.
// 
// Unlike MatrixMult_SIMDc and MatrixMult_SIMDIntrinsics, this version unrolls the 
// inner-most loop.  MatrixMult_SIMDc and MatrixMult_SIMDIntrinsincs could be 
// optimized to do the same.  Notice we're using movups b/c we're unsure about 
// the data alignment.
//  
// Inputs: a, b (both D3DMATIX)
// Outputs: Result.ret (the result matrix)
// Return Value: ret (the result matrix)
//-----------------------------------------------------------------------------
// Doesn"t work correctly ?
void MatrixMult_SIMDasm( const D3DMATRIX &a, const D3DMATRIX &b, D3DMATRIX &ret)
{

	_asm
	{
		push esi
		push edi

		mov edi, a
		mov esi, b
		mov edx, ret

		mov ecx, 4

		movups xmm4, [edi]		// a00 a01 a02 a03
		movups xmm5, [edi+16]	// a10 a11 a12 a13
		movups xmm6, [edi+32]	// a20 a21 a22 a23
		movups xmm7, [edi+48]	// a30 a31 a32 a33

Row_Loop:

		movss xmm0, [esi]		// ??? ??? ??? bx0 where x goes from 0 to 3 in the loop
		shufps xmm0, xmm0, 0	// bx0 bx0 bx0 bx0
		mulps xmm0, xmm4		// xmm0 now contains 
								// [ bx0 * a00 ] [ bx0 * a01 ] [ bx0 * a02 ] [ bx0 * a03 ]

		movss xmm1, [esi+4]		// ??? ??? ??? bx1 where x goes from 0 to 3 in the loop
		shufps xmm1, xmm1, 0	// bx1 bx1 bx1 bx1
		mulps xmm1, xmm5		// xmm1 now contains
								// [ bx1 * a10 ] [ bx1 * a11 ] [ bx1 * a12 ] [ bx1 * a13 ]
		addps xmm0, xmm1		// xmm0 now contains four partial dot products
								// [ bx0*a00+bx1*a10 ] [ bx0*a01+bx1*a11 ] . . .
		movss xmm1, [esi+8]
		shufps xmm1, xmm1, 0
		mulps xmm1, xmm6
		addps xmm0, xmm1

		movss xmm1, [esi+12]
		shufps xmm1, xmm1, 0
		mulps xmm1, xmm7
		addps xmm0, xmm1		// xmm0 now contains four dot products
								// [ bx0*a00+bx1*a10+bx2*a20+bx3*a30 ] [ bx0*a01+bx1*a11+...

		movups [edx], xmm0
		add edx, 16
		add esi, 16
		dec ecx
		jne Row_Loop

		pop edi
		pop esi
	}
	
} // MatrixMult_SIMDasm



/////////////////////////////////////////////////////////////////////////////

void MatVec_x87 (float M[4][4], Vertex A, Vertex *B)
{

	B->x = M[0][0]*A.x + M[0][1]*A.y + M[0][2]*A.z + M[0][3]*A.w; 
	B->y = M[1][0]*A.x + M[1][1]*A.y + M[1][2]*A.z + M[1][3]*A.w;
	B->z = M[2][0]*A.x + M[2][1]*A.y + M[2][2]*A.z + M[2][3]*A.w;
	B->w = M[3][0]*A.x + M[3][1]*A.y + M[3][2]*A.z + M[3][3]*A.w;

//	B->x = M[0][0]*A.x + M[1][0]*A.y + M[2][0]*A.z + M[3][0]*A.w; 
//	B->y = M[0][1]*A.x + M[1][1]*A.y + M[2][1]*A.z + M[3][1]*A.w;
//	B->z = M[0][2]*A.x + M[1][2]*A.y + M[2][2]*A.z + M[3][2]*A.w;
//	B->w = M[0][3]*A.x + M[1][3]*A.y + M[2][3]*A.z + M[3][3]*A.w;


} 
void MatVec_x87 (/*const*/ float M[4][4], Point A, Point *B)
{
    float denom = A.x * M[0][3] + A.y * M[1][3] + A.z * M[2][3] + M[3][3];
    if (denom != 1.0)
		denom = (1.0f / denom);

	B->x = denom*(M[0][0]*A.x + M[0][1]*A.y + M[0][2]*A.z + M[0][3]); 
	B->y = denom*(M[1][0]*A.x + M[1][1]*A.y + M[1][2]*A.z + M[1][3]);
	B->z = denom*(M[2][0]*A.x + M[2][1]*A.y + M[2][2]*A.z + M[2][3]);

} 



inline void MatVec_Int (float M[4][4], float *A /*[4] */ ,  float * B /*[4] */)
{
	__m128 c, c0, c1, c2, c3;
//	float ret; 

	c = _mm_load_ps(&A[0]);

	c0 = _mm_mul_ps(c, M128(M[0]));
	c1 = _mm_mul_ps(c, M128(M[1]));
	c2 = _mm_mul_ps(c, M128(M[2]));
	c3 = _mm_mul_ps(c, M128(M[3]));

	c0 = sum4(c0); 
	c1 = sum4(c1);
	c2 = sum4(c2);	
	c3 = sum4(c3); 

	_mm_store_ss (&B[0], c0); 
	_mm_store_ss (&B[1], c1); 
	_mm_store_ss (&B[2], c2); 
	_mm_store_ss (&B[3], c3); 

//    _mm_store_ss (&ret, result); 


}

void KatmaiMatVec_int (/*const*/ float m[4][4], /*const*/ Point &A, Point &B)
{
	/*register*/ __m128 m0,m1,m2,m3; // matrix on stack aligned 
	register __m128 tmp;
	register __m128 res;
	float vt[4];


	m0 = _mm_loadu_ps(&m[0][0]);
	m1 = _mm_loadu_ps(&m[1][0]);
	m2 = _mm_loadu_ps(&m[2][0]);
	m3 = _mm_loadu_ps(&m[3][0]);

		tmp = _mm_load_ps1(&A.x);
		res = _mm_mul_ps(m0,tmp);

		tmp = _mm_load_ps1(&A.y);
		res = _mm_add_ps(res,_mm_mul_ps(m1,tmp));

		tmp = _mm_load_ps1(&A.z);

		res = _mm_add_ps(res,_mm_mul_ps(m2,tmp));

		res = _mm_add_ps(res,m3);

#if 0	
		//get  w=vt[3];
		_mm_store_ps(&vt[0],res);

		tmp = _mm_load_ps1(&vt[3]);
		// to do : get res[3] and spread over tmp

		// res =  _mm_div_ps(res,tmp);

		res =  _mm_mul_ps(res,rcp_nr(tmp));


// 		tmp = _mm_rcp_ps(tmp); // 1.0 / vt[3]

//??		_mm_store_ps1(&vtlp->rhw,tmp); // exception here 
//		_mm_store_ps1(&tmpinvv[0],tmp); // exception here 

//		res =  _mm_mul_ps(res,tmp);
		_mm_store_ps(&vt[0],res);
		B->x = vt[0]; 
		B->y = vt[1];
		B->z = vt[2];


#else
		_mm_store_ps(&vt[0],res);
		float tmpinv = 1.0f / vt[3];
		B.x = vt[0]*tmpinv; 
		B.y = vt[1]*tmpinv;
		B.z = vt[2]*tmpinv;
#endif

} 

void KatmaiMatVecN_int (/*const*/ float m[4][4],int n, /*const*/ Point *A, Point *B)
{
	/*register*/ __m128 m0,m1,m2,m3; // matrix on stack aligned 
	register __m128 tmp;
	register __m128 res;
	float vt[4];


	m0 = _mm_loadu_ps(&m[0][0]);
	m1 = _mm_loadu_ps(&m[1][0]);
	m2 = _mm_loadu_ps(&m[2][0]);
	m3 = _mm_loadu_ps(&m[3][0]);

	for (size_t i=0; i<n; i++) {

		tmp = _mm_load_ps1(&A->x);
		res = _mm_mul_ps(m0,tmp);

		tmp = _mm_load_ps1(&A->y);
		res = _mm_add_ps(res,_mm_mul_ps(m1,tmp));

		tmp = _mm_load_ps1(&A->z);

		res = _mm_add_ps(res,_mm_mul_ps(m2,tmp));

		res = _mm_add_ps(res,m3);

#if 0	
		//get  w=vt[3];
		_mm_store_ps(&vt[0],res);

		tmp = _mm_load_ps1(&vt[3]);
		// to do : get res[3] and spread over tmp

		// res =  _mm_div_ps(res,tmp);

		res =  _mm_mul_ps(res,rcp_nr(tmp));


// 		tmp = _mm_rcp_ps(tmp); // 1.0 / vt[3]

//??		_mm_store_ps1(&vtlp->rhw,tmp); // exception here 
//		_mm_store_ps1(&tmpinvv[0],tmp); // exception here 

//		res =  _mm_mul_ps(res,tmp);
		_mm_store_ps(&vt[0],res);
		B->x = vt[0]; 
		B->y = vt[1];
		B->z = vt[2];


#else
		_mm_store_ps(&vt[0],res);
		float tmpinv = 1.0f / vt[3];
		B->x = vt[0]*tmpinv; 
		B->y = vt[1]*tmpinv;
		B->z = vt[2]*tmpinv;
#endif
	A++;
	B++;
	}

} 



/////////////////////////////////////////////////////////////////////////////


void MatMat_x87 (const float M1[4][4], const float M2[4][4], float M3[4][4])
{

	int i, j, k;
	float V; 

	for (i=0; i<4; i++)
	{	for (k=0; k<4; k++)
		{	V = 0.; 
			for (j=0; j<4; j++)
			{	V = V + M1[i][j]*M2[j][k];
			} 
			M3[i][k] = V; 
		}
	}
} 


inline void MatMat_Int (float M1[4][4], float M2[4][4], float M3[4][4])

{

	int i; 
	__m128  M1in[4]; 
	__m128  M2in[4], M2out[4]; 
	__m128  M3in[4], M3out[4]; 
	__m128  Cint1, Cint2, Cint3, Cint4, Cint5, Cint6; 
//
// Transpose second 
//
	M2in[0] = _mm_load_ps (&M2[0][0]); 
	M2in[1] = _mm_load_ps (&M2[1][0]);
	M2in[2] = _mm_load_ps (&M2[2][0]);
	M2in[3] = _mm_load_ps (&M2[3][0]);

	Cint1 = _mm_shuffle_ps(M2in[0], M2in[1], SELECT(1, 0, 1, 0)); 
	Cint2 = _mm_shuffle_ps(M2in[2], M2in[3], SELECT(1, 0, 1, 0));

	M2out[0] = _mm_shuffle_ps(Cint1, Cint2, SELECT(2, 0, 2, 0));
	M2out[1] = _mm_shuffle_ps(Cint1, Cint2, SELECT(3, 1, 3, 1));

	Cint3 = _mm_shuffle_ps(M2in[0], M2in[1], SELECT(3, 2, 3, 2)); 
	Cint4 = _mm_shuffle_ps(M2in[2], M2in[3], SELECT(3, 2, 3, 2));

	M2out[2] = _mm_shuffle_ps(Cint3, Cint4, SELECT(2, 0, 2, 0));
	M2out[3] = _mm_shuffle_ps(Cint3, Cint4, SELECT(3, 1, 3, 1));
//
// Load M1 
//
	M1in[0] = _mm_load_ps (&M1[0][0]); 
	M1in[1] = _mm_load_ps (&M1[1][0]);
	M1in[2] = _mm_load_ps (&M1[2][0]);
	M1in[3] = _mm_load_ps (&M1[3][0]);

	for (i=0; i<4; i++)
	{	M3in[0] = _mm_mul_ps (M1in[i], M2out[0]);
		M3in[1] = _mm_mul_ps (M1in[i], M2out[1]); 
		M3in[2] = _mm_mul_ps (M1in[i], M2out[2]); 
		M3in[3] = _mm_mul_ps (M1in[i], M2out[3]); 

		Cint1 = sum4(M3in[0]); 
		Cint2 = sum4(M3in[1]);
		Cint3 = sum4(M3in[2]);
		Cint4 = sum4(M3in[3]);

		Cint5 = _mm_shuffle_ps(Cint1, Cint2, SELECT(0, 0, 0, 0));
		Cint6 = _mm_shuffle_ps(Cint3, Cint4, SELECT(0, 0, 0, 0));

		M3out[i] = _mm_shuffle_ps(Cint5, Cint6, SELECT(2, 0, 2, 0));
	}



	_mm_store_ps (&M3[0][0], M3out[0]); 
	_mm_store_ps (&M3[1][0], M3out[1]);
	_mm_store_ps (&M3[2][0], M3out[2]);
	_mm_store_ps (&M3[3][0], M3out[3]);


}

inline  float DotProd_x87 (Vertex &A, Vertex &B)
{
	
	return A.x * B.x + A.y * B.y + A.z * B.z;
/*	_asm{
		fld         A.x
		fmul        B.x
		fld         A.y
		fmul        B.y
		faddp       st(1),st
		fld         A.z
		fmul        B.z
		faddp       st(1),st
		}
	*/
} 


inline float DotProd_Int (__m128 A, __m128 B)
{
	__m128 result;
	float ret; 

	result = _mm_mul_ps (A, B);
	result = sum4(result); 
    _mm_store_ss (&ret, result); 
	return ret; 
}

inline __m128 DotProd_Intx (__m128 &A, __m128 &B)
{
	__m128 result;

	result = _mm_mul_ps (A, B);
	result = sum4(result); 
	return result; 
}



inline float Norm_x87 (Vertex A, Vertex B)
{
	return sqrt(A.x * B.x + A.y * B.y + A.z * B.z);
} 

inline float Norm_Int (__m128 *A, __m128 *B)
{
	__m128 result; 
	float ret;

	result = _mm_set_ps (1.f, 1.f, 1.f, 1.f);
	result = _mm_mul_ps (*A, *B);
	result = sum4(result); 
	result = _mm_sqrt_ss (result); 
    _mm_store_ss (&ret, result); 
	return ret; 
}

inline float NormInv_x87 (Vertex A, Vertex B)
{
	return 1./sqrt(A.x * B.x + A.y * B.y + A.z * B.z);
} 

inline float NormInv_Int (__m128 *A, __m128 *B)
{
	__m128 result; 
	__m128 One4;
	float ret;

	One4   = _mm_set_ps (1.f, 1.f, 1.f, 1.f); 
	result = _mm_mul_ps (*A, *B);
	result = sum4(result); 
	result = _mm_sqrt_ss (result);
	result = _mm_div_ss (One4, result); 
    _mm_store_ss (&ret, result); 
	return ret; 
}

inline float NormInvRec_Int (__m128 *A, __m128 *B)
{
	__m128 result; 
	float ret;
 
	result = _mm_mul_ps (*A, *B);
	result = sum4(result); 
	result = _mm_rsqrt_ss (result);
    _mm_store_ss (&ret, result); 
	return ret; 
}




#endif





#if 0

__m128;

/*
 * Untransformed/lit vertices
 */
typedef struct _D3DLVERTEX {
    union {
    D3DVALUE     x;             /* Homogeneous coordinates */
    D3DVALUE     dvX;
    };
    union {
    D3DVALUE     y;
    D3DVALUE     dvY;
    };
    union {
    D3DVALUE     z;
    D3DVALUE     dvZ;
    };
    DWORD            dwReserved;
    union {
    D3DCOLOR     color;         /* Vertex color */
    D3DCOLOR     dcColor;
    };
    union {
    D3DCOLOR     specular;      /* Specular component of vertex */
    D3DCOLOR     dcSpecular;
    };
    union {
    D3DVALUE     tu;            /* Texture coordinates */
    D3DVALUE     dvTU;
    };
    union {
    D3DVALUE     tv;
    D3DVALUE     dvTV;
    };

#endif

		

#ifdef _D3D
void KatmaiTransformScreen(const float *x,
						   const float *y,
						   const float *z,
						   int n,
						   float m[4][4],
						   float w,float h,
						   D3DTLVERTEX *vtlp)
{
//	n = (n+3)/4;

	for(size_t i=0; i < n; i++) {
		// vertex 
		const Point p(x[i],y[i],z[i]);

		float vt[4];
		vt[0] = p.x * m[0][0] + p.y * m[1][0] + p.z * m[2][0] + m[3][0];
		vt[1] = p.x * m[0][1] + p.y * m[1][1] + p.z * m[2][1] + m[3][1];
		vt[2] = p.x * m[0][2] + p.y * m[1][2] + p.z * m[2][2] + m[3][2];
		vt[3] = p.x * m[0][3] + p.y * m[1][3] + p.z * m[2][3] + m[3][3];
		
		float tmp = 1.0f / vt[3];
		vt[0]  *=tmp; 
		vt[1]  *=tmp;
		vt[2]  *=tmp;


		vtlp->sx = w+vt[0]*w;
		vtlp->sy = h-vt[1]*h;
		vtlp->sz = vt[2];
		vtlp->rhw = vt[3]; // tmp; // to do : clamp
		
		vtlp ++;

	}


}

#endif


#ifdef _KATMAI

void KatmaiTransformScreen_Int(int n,Point *p,
						   float m[4][4],
						   float w,float h,
						   D3DTLVERTEX *vtlp)
{
	register __m128 m0,m1,m2,m3; // matrix on stack aligned 
	register __m128 tmp;
	register __m128 res;

	float vt[4];
	float tmpinvv[4];

	m0 = _mm_loadu_ps(&m[0][0]);
	m1 = _mm_loadu_ps(&m[1][0]);
	m2 = _mm_loadu_ps(&m[2][0]);
	m3 = _mm_loadu_ps(&m[3][0]);

	for(size_t i=0; i < n; i++,p++) {
	
		tmp = _mm_load_ps1(&p->x);
		res = _mm_mul_ps(m0,tmp);

		tmp = _mm_load_ps1(&p->y);
		res = _mm_add_ps(res,_mm_mul_ps(m1,tmp));

		tmp = _mm_load_ps1(&p->z);

		res = _mm_add_ps(res,_mm_mul_ps(m2,tmp));

		res = _mm_add_ps(res,m3);
/*
		vt[0] = p.x * m[0][0] + p.y * m[1][0] + p.z * m[2][0] + m[3][0];
		vt[1] = p.x * m[0][1] + p.y * m[1][1] + p.z * m[2][1] + m[3][1];
		vt[2] = p.x * m[0][2] + p.y * m[1][2] + p.z * m[2][2] + m[3][2];
		vt[3] = p.x * m[0][3] + p.y * m[1][3] + p.z * m[2][3] + m[3][3];
*/		
		_mm_store_ps(&vt[0],res);

#if 0	
		float w=vt[3];

		tmp = _mm_load_ps1(&vt[3]);

		res =  _mm_div_ps(res,tmp);


// 		tmp = _mm_rcp_ps(tmp); // 1.0 / vt[3]

//??		_mm_store_ps1(&vtlp->rhw,tmp); // exception here 
//		_mm_store_ps1(&tmpinvv[0],tmp); // exception here 
//		vtlp->rhw =tmpinvv[0]; // rcp lookss bad !!!!!!!!!!11

//		res =  _mm_mul_ps(res,tmp);

		_mm_store_ps(&vt[0],res);
		vtlp->rhw = 1.0/w;

#else
		float tmpinv = 1.0f / vt[3];
		vt[0]  *=tmpinv; 
		vt[1]  *=tmpinv;
		vt[2]  *=tmpinv;

		vtlp->rhw = tmpinv; 

#endif


		vtlp->sx = w+vt[0]*w;
		vtlp->sy = h-vt[1]*h;
		vtlp->sz = vt[2];
		
		vtlp ++;

	}


}


// SOA, 4 points a time 
void KatmaiTransformScreen_Int(
			   int n, float  *x_, float *y_, float *z_,
			   float m[4][4],
			   float sx,float tx, float sy,float ty, // 2D viewpoint transform
			   D3DTLVERTEX *vtlp
			)
{
	__m128 *x =(__m128 *) x_;
	__m128 *y =(__m128 *) y_;
	__m128 *z =(__m128 *) z_;

	F32vec4 mat30 = m[0][3], mat31 = m[1][3];
	F32vec4 mat32 = m[2][3], mat33 = m[3][3];
	F32vec4 mat00 = m[0][0], mat01 = m[1][0];
	F32vec4 mat02 = m[2][0], mat03 = m[3][0];
	F32vec4 mat10 = m[0][1], mat11 = m[1][1];
	F32vec4 mat12 = m[2][1], mat13 = m[3][1];
	F32vec4 mat20 = m[0][2], mat21 = m[1][2];
	F32vec4 mat22 = m[2][2], mat23 = m[3][2];
	
	F32vec4 VK_SX = sx;
	F32vec4 VK_TX = tx;
	F32vec4 VK_SY = sy;
	F32vec4 VK_TY = ty;


	F32vec4 curX, curY, curZ, curW; 
	F32vec4 Xout,Yout,Zout;	
	
	n = (n+3) >> 2;

	
    for (size_t i=0; i<n; i++) 
	{
			// _mm_prefetch((char *)(&x[2]), _MM_HINT_NTA); 
		    // _mm_prefetch((char *)(&y[2]), _MM_HINT_NTA); 
		    // _mm_prefetch((char *)(&z[2]), _MM_HINT_NTA);

			curX = *x++; 
			curY = *y++; 
			curZ = *z++;

			// use rcp_nr : to do test & profile
			curW = rcp_nr(((curX*mat30)+(curY*mat31)+(curZ*mat32)+mat33));
			// curW = (One/((curX*mat30)+(curY*mat31)+(curZ*mat32)+mat33));
	
			Xout=((((curX*mat00)+(curY*mat01)+(curZ*mat02)+mat03)*curW));
			Xout = VK_TX+ Xout * VK_SX;

			Yout=((((curX*mat10)+(curY*mat11)+(curZ*mat12)+mat13)*curW));
			Yout = VK_TY - Yout * VK_SY;

			Zout=((((curX*mat20)+(curY*mat21)+(curZ*mat22)+mat23)*curW));
#if 0 
			_MM_TRANSPOSE4_PS(curX, curY, curZ, curW);
			storeu(&vtlp->sx,curX); vtlp++;
			storeu(&vtlp->sx,curY); vtlp++;
			storeu(&vtlp->sx,curZ); vtlp++;
			storeu(&vtlp->sx,curW); vtlp++;
#else			
			// unpack 
			vtlp->sx = Xout[0],vtlp->sy = Yout[0],vtlp->sz = Zout[0],vtlp->rhw = curW[0];
			vtlp ++;
			vtlp->sx = Xout[1],vtlp->sy = Yout[1],vtlp->sz = Zout[1],vtlp->rhw = curW[1];
			vtlp ++;
			vtlp->sx = Xout[2],vtlp->sy = Yout[2],vtlp->sz = Zout[2],vtlp->rhw = curW[2];
			vtlp ++;
			vtlp->sx = Xout[3],vtlp->sy = Yout[3],vtlp->sz = Zout[3],vtlp->rhw = curW[3];
			vtlp ++;
#endif
		}
		
}		

// SOS, 4 points a time 
// currently used 
void KatmaiTransformScreen_Int(
			   int n, float  *vec_,
			   float m[4][4],
			   float sx,float tx, float sy,float ty, // 2D viewpoint transform
			   D3DTLVERTEX *vtlp
			)
{
	__m128 *vec  =(__m128 *) vec_;

	F32vec4 mat30 = m[0][3], mat31 = m[1][3];
	F32vec4 mat32 = m[2][3], mat33 = m[3][3];
	F32vec4 mat00 = m[0][0], mat01 = m[1][0];
	F32vec4 mat02 = m[2][0], mat03 = m[3][0];
	F32vec4 mat10 = m[0][1], mat11 = m[1][1];
	F32vec4 mat12 = m[2][1], mat13 = m[3][1];
	F32vec4 mat20 = m[0][2], mat21 = m[1][2];
	F32vec4 mat22 = m[2][2], mat23 = m[3][2];
	
	F32vec4 VK_SX = sx;
	F32vec4 VK_TX = tx;
	F32vec4 VK_SY = sy;
	F32vec4 VK_TY = ty;

	F32vec4 curX, curY, curZ, curW; 
	F32vec4 Xout,       Yout,       Zout;	
	
	n = (n+3) >> 2;

	// _mm_prefetch(((char *)(vtlp)) + 32, _MM_HINT_T0); 
	
    for (size_t i=0; i<n; i++) 
	{
//MVo
			_mm_prefetch((char *)(&vec[5]), _MM_HINT_NTA);
			_mm_prefetch((char *)(&vec[7]), _MM_HINT_NTA);
			curX = *vec++; 
			curY = *vec++; 
			curZ = *vec++;

			// use rcp_nr : to do test & profile
			curW = rcp_nr(((curX*mat30)+(curY*mat31)+(curZ*mat32)+mat33));
			// curW = (One/((curX*mat30)+(curY*mat31)+(curZ*mat32)+mat33));

//			_mm_prefetch((char *)(&vec[0]), _MM_HINT_NTA); 
	
			Xout=((((curX*mat00)+(curY*mat01)+(curZ*mat02)+mat03)*curW));
			Xout = VK_TX+ Xout * VK_SX;
//			_mm_prefetch(((char *)(vtlp)) + 32, _MM_HINT_T0); 


			Yout=((((curX*mat10)+(curY*mat11)+(curZ*mat12)+mat13)*curW));
			Yout = VK_TY - Yout * VK_SY;

//			_mm_prefetch(((char *)(&vec[0])) + 32, _MM_HINT_NTA); 


			Zout=((((curX*mat20)+(curY*mat21)+(curZ*mat22)+mat23)*curW));
//			_mm_prefetch(((char *)(vtlp)) + 64, _MM_HINT_T0); 


#if 0 
			_MM_TRANSPOSE4_PS(curX, curY, curZ, curW);
			storeu(&vtlp->sx,curX); vtlp++;
			storeu(&vtlp->sx,curY); vtlp++;
			storeu(&vtlp->sx,curZ); vtlp++;
			storeu(&vtlp->sx,curW); vtlp++;
#else			
#if 1 
			// unpack 
		__m128 Clow1  = _mm_unpacklo_ps (Xout, Yout);
		_mm_storel_pi ((__m64*)&((vtlp)->sx), Clow1);
		_mm_storeh_pi ((__m64*)&((vtlp+1)->sx), Clow1);
//		_mm_prefetch(((char *)(vtlp)) + 96, _MM_HINT_T0); 

		__m128 Clow2  = _mm_unpacklo_ps (Zout, curW);		

		_mm_storel_pi ((__m64*)&((vtlp)->sz), Clow2);
		_mm_storeh_pi ((__m64*)&((vtlp+1)->sz ), Clow2);


		__m128 Chigh1 = _mm_unpackhi_ps (Xout, Yout);
		__m128 Chigh2 = _mm_unpackhi_ps (Zout, curW);
//		_mm_storel_pi ((__m64*)&((vtlp+i+2)->sx), Chigh1);
//		_mm_storeh_pi ((__m64*)&((vtlp+i+3)->sx), Chigh1);
//		_mm_storel_pi ((__m64*)&((vtlp+i+2)->sz), Chigh2);
//		_mm_storeh_pi ((__m64*)&((vtlp+i+3)->sz), Chigh2);

//		_mm_prefetch(((char *)(vtlp)) + 128, _MM_HINT_T0); 

		__m128 Cf1 = _mm_shuffle_ps (Chigh1, Chigh2, _MM_SHUFFLE(1,0,1,0)); 
		__m128 Cf2 = _mm_shuffle_ps (Chigh1, Chigh2, _MM_SHUFFLE(3,2,3,2));
		_mm_stream_ps (&((vtlp+2)->sx), Cf1); 
		_mm_stream_ps (&((vtlp+3)->sx), Cf2); 

//		_mm_store_ps (&((vtlp+2)->sx), Cf1);  // vtl now aligned ! and keep vtl in cache 
//		_mm_store_ps (&((vtlp+3)->sx), Cf2); 

		// compiler seems to put all prefetch instructions together !!!!!!!!!!!!!!!!
		vtlp+=4;
		// _mm_prefetch(((char *)(vtlp)), _MM_HINT_T0); 


#else
			vtlp->sx = Xout[0],vtlp->sy = Yout[0],vtlp->sz = Zout[0],vtlp->rhw = curW[0];
			vtlp ++;
			vtlp->sx = Xout[1],vtlp->sy = Yout[1],vtlp->sz = Zout[1],vtlp->rhw = curW[1];
			vtlp ++;
			vtlp->sx = Xout[2],vtlp->sy = Yout[2],vtlp->sz = Zout[2],vtlp->rhw = curW[2];
			vtlp ++;
			vtlp->sx = Xout[3],vtlp->sy = Yout[3],vtlp->sz = Zout[3],vtlp->rhw = curW[3];
			vtlp ++;
#endif

#endif
		}
		
}		



#define GETXYZ() \
					_mm_prefetch((char *) &v[4], _MM_HINT_NTA); \
					_mm_prefetch((char *) &v[6], _MM_HINT_NTA); \
					curX = *v++; curY = *v++; curZ = *v++; 



#define GETNXNYNZ()	\
					_mm_prefetch((char *) &vn[4], _MM_HINT_NTA); \
					_mm_prefetch((char *) &vn[6], _MM_HINT_NTA); \
					curNX = *vn++; curNY = *vn++; curNZ = *vn++;

#define SETUPXYZ() \
	__m128 *v =(__m128 *) v_; \
	__m128 zdist;


//    _mm_prefetch((char *)  v, _MM_HINT_NTA);

#define PREFETCHXYZ() \
    _mm_prefetch((char *)  v, _MM_HINT_NTA); \
    _mm_prefetch(((char *)  v)+32, _MM_HINT_NTA); 

#define SETUPNXNYNZ() \
	__m128 *vn =(__m128 *) vn_; \

//    _mm_prefetch((char *)  vn, _MM_HINT_NTA);

#define PREFETCHNXNYZN() \
    _mm_prefetch((char *)  vn, _MM_HINT_NTA); \
    _mm_prefetch(((char *)  vn)+32, _MM_HINT_NTA); 


#define SETUPMAT() \
	F32vec4 mat30 = m[0][3], mat31 = m[1][3]; F32vec4 mat32 = m[2][3], mat33 = m[3][3]; \
	F32vec4 mat00 = m[0][0], mat01 = m[1][0]; F32vec4 mat02 = m[2][0], mat03 = m[3][0]; \
	F32vec4 mat10 = m[0][1], mat11 = m[1][1]; F32vec4 mat12 = m[2][1], mat13 = m[3][1]; \
	F32vec4 mat20 = m[0][2], mat21 = m[1][2]; F32vec4 mat22 = m[2][2], mat23 = m[3][2]; \
	F32vec4 VK_SX = sx; F32vec4 VK_TX = tx; F32vec4 VK_SY = sy; F32vec4 VK_TY = ty


// compiler doesn't inline if function gets to complex
#define RCP_NR(a) \
		__m128 Ra0 = _mm_rcp_ps(a); \
		a =  _mm_sub_ps(_mm_add_ps(Ra0, Ra0), _mm_mul_ps(_mm_mul_ps(Ra0, a), Ra0)); 


#define TRANSFORMXYZ() \
			curW = rcp_nr(((curX*mat30)+(curY*mat31)+(curZ*mat32)+mat33)); \
			/* _mm_prefetch((char *) vtlp, _MM_HINT_T1); */ \
			Xout=((((curX*mat00)+(curY*mat01)+(curZ*mat02)+mat03)*curW)); \
			Xout = VK_X+ Xout * VK_X; \
			/* _mm_prefetch((char *)(vtlp+1), _MM_HINT_T1); */ \
			Yout=((((curX*mat10)+(curY*mat11)+(curZ*mat12)+mat13)*curW)); \
			Yout = VK_Y - Yout * VK_Y; \
			/* _mm_prefetch((char *)(vtlp+2), _MM_HINT_T1); */ \
			Zout=((((curX*mat20)+(curY*mat21)+(curZ*mat22)+mat23)*curW)); \
			/* _mm_prefetch((char *)(vtlp+3), _MM_HINT_T1); */ \

#define TRANSFORMPACKXYZ() \
			{ F32vec4 curW = (((curX*mat30)+(curY*mat31)+(curZ*mat32)+mat33)); \
			RCP_NR(curW); \
			/* _mm_prefetch((char *) vtlp, _MM_HINT_T1); */ \
			F32vec4  Xout=((((curX*mat00)+(curY*mat01)+(curZ*mat02)+mat03)*curW)); \
			Xout = VK_TX+ Xout * VK_SX; \
			/* _mm_prefetch((char *)(vtlp+1), _MM_HINT_T1); */ \
			F32vec4 Yout=((((curX*mat10)+(curY*mat11)+(curZ*mat12)+mat13)*curW)); \
			Yout = VK_TY - Yout * VK_SY; \
			/* _mm_prefetch((char *)(vtlp+2), _MM_HINT_T1); */ \
			zdist = ((curX*mat20)+(curY*mat21)+(curZ*mat22)+mat23); \
			F32vec4 Zout=((zdist*curW)); \
			/* _mm_prefetch((char *)(vtlp+3), _MM_HINT_T1); */ \
			KPackXYZ(Xout,Yout,Zout,curW,vtlp); \
			} \



#define UNPACKXYZ() \
			KPackXYZ(Xout,Yout,Zout,curW,vtlp); \
			vtlp += 4;

#define UNPACKXYZ_() \
			vtlp->sx = Xout[0],vtlp->sy = Yout[0],vtlp->sz = Zout[0],vtlp->rhw = curW[0]; \
			vtlp ++; \
			vtlp->sx = Xout[1],vtlp->sy = Yout[1],vtlp->sz = Zout[1],vtlp->rhw = curW[1]; \
			vtlp ++; \
			vtlp->sx = Xout[2],vtlp->sy = Yout[2],vtlp->sz = Zout[2],vtlp->rhw = curW[2]; \
			vtlp ++; \
			vtlp->sx = Xout[3],vtlp->sy = Yout[3],vtlp->sz = Zout[3],vtlp->rhw = curW[3]; \
			vtlp ++; 

#define UNPACKXYZRGB() \
			/* KPackXYZ(Xout,Yout,Zout,curW,vtlp);*/ \
			KPackColor(color.x,color.y,color.z,state.alpha_,vtlp); \
			vtlp[0].specular=vtlp[1].specular= vtlp[2].specular = vtlp[3].specular=0; \
			vtlp += 4;

// old version, slow generic
#define UNPACKXYZRGB__() \
			/*vtlp->sx = Xout[0],vtlp->sy = Yout[0],vtlp->sz = Zout[0],vtlp->rhw = curW[0];*/ \
			vtlp->color= RGBA_MAKE((unsigned int)color.x[0],(unsigned int)color.y[0],(unsigned int)color.z[0],state.alpha); vtlp->specular=0; \
			vtlp ++; \
			/*vtlp->sx = Xout[1],vtlp->sy = Yout[1],vtlp->sz = Zout[1],vtlp->rhw = curW[1];*/ \
			vtlp->color= RGBA_MAKE((unsigned int)color.x[1],(unsigned int)color.y[1],(unsigned int)color.z[1],state.alpha); vtlp->specular=0; \
			vtlp ++; \
			/*vtlp->sx = Xout[2],vtlp->sy = Yout[2],vtlp->sz = Zout[2],vtlp->rhw = curW[2];*/ \
			vtlp->color= RGBA_MAKE((unsigned int)color.x[2],(unsigned int)color.y[2],(unsigned int)color.z[2],state.alpha); vtlp->specular=0;\
			vtlp ++; \
			/*vtlp->sx = Xout[3],vtlp->sy = Yout[3],vtlp->sz = Zout[3],vtlp->rhw = curW[3];*/ \
			vtlp->color= RGBA_MAKE((unsigned int)color.x[3],(unsigned int)color.y[3],(unsigned int)color.z[3],state.alpha); vtlp->specular=0; \
			vtlp ++; 

#define UNPACKXYZRGBSPEC() \
			/* KPackXYZ(Xout,Yout,Zout,curW,vtlp); */ \
			KPackColor(color.x,color.y,color.z,state.alpha_,vtlp); \
			KPackSpecularColor(specularColor.x,specularColor.y,specularColor.z,fogFactor,vtlp); \
			vtlp += 4;

#define UNPACKXYZRGBSPEC___() \
			vtlp->sx = Xout[0],vtlp->sy = Yout[0],vtlp->sz = Zout[0],vtlp->rhw = curW[0]; \
			vtlp->color= RGBA_MAKE((unsigned int)color.x[0],(unsigned int)color.y[0],(unsigned int)color.z[0],state.alpha); \
			vtlp->specular= RGBA_MAKE((unsigned int)specularColor.x[0],(unsigned int)specularColor.y[0],(unsigned int)specularColor.z[0],255);  \
			vtlp ++; \
			vtlp->sx = Xout[1],vtlp->sy = Yout[1],vtlp->sz = Zout[1],vtlp->rhw = curW[1]; \
			vtlp->color= RGBA_MAKE((unsigned int)color.x[1],(unsigned int)color.y[1],(unsigned int)color.z[1],state.alpha); \
			vtlp->specular= RGBA_MAKE((unsigned int)specularColor.x[1],(unsigned int)specularColor.y[1],(unsigned int)specularColor.z[1],255);  \
			vtlp ++; \
			vtlp->sx = Xout[2],vtlp->sy = Yout[2],vtlp->sz = Zout[2],vtlp->rhw = curW[2]; \
			vtlp->color= RGBA_MAKE((unsigned int)color.x[2],(unsigned int)color.y[2],(unsigned int)color.z[2],state.alpha); \
			vtlp->specular= RGBA_MAKE((unsigned int)specularColor.x[2],(unsigned int)specularColor.y[2],(unsigned int)specularColor.z[2],255);  \
			vtlp ++; \
			vtlp->sx = Xout[3],vtlp->sy = Yout[3],vtlp->sz = Zout[3],vtlp->rhw = curW[3]; \
			vtlp->color= RGBA_MAKE((unsigned int)color.x[3],(unsigned int)color.y[3],(unsigned int)color.z[3],state.alpha);  \
			vtlp->specular= RGBA_MAKE((unsigned int)specularColor.x[3],(unsigned int)specularColor.y[3],(unsigned int)specularColor.z[3],255);  \
			vtlp ++; 

// NURBS version
#define UNPACKRGBSPEC2() \
			KPackColor2(color.x,color.y,color.z,state.alpha_,vtlp); \
			KPackSpecularColor2(specularColor.x,specularColor.y,specularColor.z,zero,vtlp); \
			vtlp += 4*2;


// F32vec4ToInt
// Is32vec2 F32vec4ToIs32vec2 (const F32vec4 &a)

// UnpackRgb
// _mm_cvtt_ps2pi(a);


// SOS, 4 points a time 
void KatmaiTransformScreen_(
			   int n, 
			   float  *v_,
			   float m[4][4],
			   float sx,float tx, float sy,float ty, // 2D viewpoint transform
			   D3DTLVERTEX *vtlp
			)
{

	
	SETUPXYZ();
/*
	_asm {
		// prefetchnta BYTE PTR [v+32]
		prefetchnta v
	}
*/
	SETUPMAT();


	n = (n+3) >> 2;

	// F32vec4 Xout,Yout, Zout;	
#if 0
	// unroll 2
	size_t i=0;
    for (; i<(n-1); i+=2) 
	{
			GETXYZ();
			TRANSFORMPACKXYZ();
			PREFETCHXYZ();

			vtlp+=4;
			GETXYZ();
			TRANSFORMPACKXYZ();
			PREFETCHXYZ();
			vtlp+=4;

	}

#endif
	F32vec4 curX, curY,curZ; 
	GETXYZ();
	
    for (size_t i=0; i<n; i++) 
	{
/*
			_asm {
				prefetchnta v
				prefetchnta v+16
			}

*/			//PREFETCHXYZ();
			TRANSFORMPACKXYZ();
			GETXYZ();
			vtlp+=4;
			//_mm_prefetch((char *)(&v[0]), _MM_HINT_NTA);
			//_mm_prefetch((char *)(&v[2]), _MM_HINT_NTA);
	}
		
}		

// diffuse lighting with 1 directional light
/*
#define DOLIGHT() KLightDiffuse(&state.lights[0],curNX,curNY,curNZ,color); \
					color.x += state.lights[0].ambient.x,color.y += state.lights[0].ambient.y,color.z += state.lights[0].ambient.z;
*/

#define SETUPLIGHT() KVec3 ldirection = state.lights[0].direction; \
					 KVec3 ldiffuse = state.lights[0].diffuse; 	


#define DOLIGHT() \
	F32vec4 f =	curNX * ldirection.x + curNY * ldirection.y + curNZ * ldirection.z; \
	f=simd_max(f,zero); \
	color.x += ldiffuse.x*f,color.y += ldiffuse.y*f,color.z += ldiffuse.z*f; \



void KatmaiTransformLight_1_dir(
			   int n, 
			   float  *v_,
			   float  *vn_,
			   float m[4][4],
			   float sx,float tx, float sy,float ty, // 2D viewpoint transform
			   D3DTLVERTEX *vtlp
			)
{

	SETUPMAT();
	
	SETUPXYZ();

	SETUPNXNYNZ();

	SETUPLIGHT();
					 KVec3  startColor; 	
					 startColor.x = state.emissive.x + state.lights[0].ambient.x; 
					 startColor.y = state.emissive.y + state.lights[0].ambient.y; 
					 startColor.z = state.emissive.z + state.lights[0].ambient.z; 

	n = (n+3) >> 2;


	/* unstripping already done 
int max;
int left;
max=n/256;
left=n-256*max;

for (int j=0; j<max; j++)
		
//        P.F

    for (size_t i=0; i<256; i++)  // I replaced i<n by i<256  P.F 
*/
    for (size_t i=0; i<n; i++) 
		
	{
		F32vec4 curX, curY,curZ; 

		F32vec4 curNX, curNY,curNZ; 
	
		KVec3 color;

			GETXYZ();

			TRANSFORMPACKXYZ();

			GETNXNYNZ();

			color = startColor;
			// color.x = state.emissive.x, color.y = state.emissive.y, color.z = state.emissive.z;

			DOLIGHT();

			// clamp
			color.x = simd_min(color.x,twofivefive);
			color.y = simd_min(color.y,twofivefive);
			color.z = simd_min(color.z,twofivefive);


			UNPACKXYZRGB();

	}
	
/*	
	 for (size_t i=0; i<left; i++) 
	 {

			GETXYZ();

			TRANSFORMPACKXYZ();

			GETNXNYNZ();

			color.x = state.emissive.x, color.y = state.emissive.y, color.z = state.emissive.z;

			DOLIGHT();

			// clamp
			color.x = simd_min(color.x,twofivefive);
			color.y = simd_min(color.y,twofivefive);
			color.z = simd_min(color.z,twofivefive);



			UNPACKXYZRGB();
	}
*/
	_m_empty(); 

}		

#undef DOLIGHT()
#undef SETUPLIGHT()


#define SETUPCOLOR() \
					KVec3 emissive(state.emissive); \
					F32vec4 power(state.power); \
					F32vec4 fogFactor(twofivefive); 


#define SETUPLIGHT() \
					KLightState light =	state.lights[0]; 

#define CLEARCOLOR() color.x = emissive.x, color.y = emissive.y, color.z = emissive.z; \
					 specularColor.x=specularColor.y=specularColor.z =  zero; \
					 fogFactor = twofivefive;

#define CLEARCOLORDIFFUSE() color.x = emissive.x, color.y = emissive.y, color.z = emissive.z; 


// 1 dir light with specular
#define DOLIGHT() KLightDiffuseSpecular(&light,curNX,curNY,curNZ,toViewerx,toViewery,toViewerz,power,color,specularColor); \
					color.x += light.ambient.x,color.y += light.ambient.y,color.z += light.ambient.z;


void KatmaiTransformLight_1_dir_spec(
			   int n, 
			   float  *v_,
			   float  *vn_,
			   float m[4][4],
			   float sx,float tx, float sy,float ty, // 2D viewpoint transform
			   D3DTLVERTEX *vtlp
			)
{

	SETUPMAT();
	
	SETUPXYZ();

	SETUPNXNYNZ();


	n = (n+3) >> 2;

	F32vec4 curX, curY,curZ,curW; 

	F32vec4 curNX, curNY,curNZ; 
	
	KVec3 color;
	KVec3 specularColor;

	SETUPCOLOR();

	SETUPLIGHT();

/*
//        P.F

int max;
int left;
max=n/256;
left=n-256*max;

for (int j=0; j<max; j++)
		
//        P.F

    for (size_t i=0; i<256; i++)  // I replaced i<n by i<256  P.F 
*/
    for (size_t i=0; i<n; i++) 
	{

			GETXYZ();

			TRANSFORMPACKXYZ();

			GETNXNYNZ();

			F32vec4 toViewerx=state.viewer.x-curX; 
			F32vec4 toViewery=state.viewer.y-curY; 
			F32vec4 toViewerz=state.viewer.z-curZ;


			// normalize (toViewerx,toViewery,toViewerz) ***
			F32vec4 f = toViewerx*toViewerx;
			f += toViewery*toViewery;
			f += toViewerz*toViewerz;
			f = rsqrt_normal(f);
			toViewerx *=f,toViewery *=f,toViewerz *=f;
			
			CLEARCOLOR();

			DOLIGHT();

			// clamp
			color.x = simd_min(color.x,twofivefive);
			color.y = simd_min(color.y,twofivefive);
			color.z = simd_min(color.z,twofivefive);

			specularColor.x = simd_min(specularColor.x,twofivefive);
			specularColor.y = simd_min(specularColor.y,twofivefive);
			specularColor.z = simd_min(specularColor.z,twofivefive);



			UNPACKXYZRGBSPEC();
	}

	
/*
    for (size_t i=0; i<left; i++)  // I replaced i<n by i<256  P.F 
	{

			GETXYZ();

			TRANSFORMPACKXYZ();

			GETNXNYNZ();

			color.x = state.emissive.x, color.y = state.emissive.y, color.z = state.emissive.z;
			specularColor.x=specularColor.y=specularColor.z = F32vec4(0.0f);

			F32vec4 toViewerx=state.viewer.x-curX; 
			F32vec4 toViewery=state.viewer.y-curY; 
			F32vec4 toViewerz=state.viewer.z-curZ;


			// normalize (toViewerx,toViewery,toViewerz) ***
			F32vec4 f = toViewerx*toViewerx;
			f += toViewery*toViewery;
			f += toViewerz*toViewerz;
			f = rsqrt_normal(f);
			toViewerx *=f,toViewery *=f,toViewerz *=f;

			DOLIGHT();

			// clamp
			color.x = simd_min(color.x,twofivefive);
			color.y = simd_min(color.y,twofivefive);
			color.z = simd_min(color.z,twofivefive);

			specularColor.x = simd_min(specularColor.x,twofivefive);
			specularColor.y = simd_min(specularColor.y,twofivefive);
			specularColor.z = simd_min(specularColor.z,twofivefive);



			UNPACKXYZRGBSPEC();
	}
*/
	_m_empty(); 

}		


#undef DOLIGHT()
// diffuse lighting with 1 directional light

// 1 dir light with specular
#define DOLIGHT() KLightDiffuse(&light,curNX,curNY,curNZ,color); \
					color.x += light.ambient.x,color.y += light.ambient.y,color.z += light.ambient.z;


void KatmaiTransformLight_1_dir_twosided(
			   int n, 
			   float  *v_,
			   float  *vn_,
			   float m[4][4],
			   float sx,float tx, float sy,float ty, // 2D viewpoint transform
			   D3DTLVERTEX *vtlp
			)
{

	SETUPMAT();
	
	SETUPXYZ();

	SETUPNXNYNZ();


	n = (n+3) >> 2;

	F32vec4 curX, curY,curZ,curW; 

	F32vec4 curNX, curNY,curNZ; 
	
	KVec3 color;
	
	SETUPCOLOR();

	SETUPLIGHT();

    for (size_t i=0; i<n; i++) 
		
	{

			GETXYZ();

			TRANSFORMPACKXYZ();

			GETNXNYNZ();

			F32vec4 toViewerx=state.viewer.x-curX; 
			F32vec4 toViewery=state.viewer.y-curY; 
			F32vec4 toViewerz=state.viewer.z-curZ;

			// normalize (toViewerx,toViewery,toViewerz) ***
			F32vec4 f = toViewerx*toViewerx;
			f += toViewery*toViewery;
			f += toViewerz*toViewerz;
			f = rsqrt_normal(f);
			toViewerx *=f,toViewery *=f,toViewerz *=f;

			// dotprod normal to viewer
			f = curNX * toViewerx + curNY * toViewery + curNZ * toViewerz;

			// scale negative parts 
#if 1
			F32vec4 mask = _mm_cmple_ps(f,zero);
			mask &= negmask;


			curNX = _mm_xor_ps (curNX,mask);
			curNY = _mm_xor_ps (curNY,mask);
			curNZ = _mm_xor_ps (curNZ,mask);
#else
			// curNX = select_lt(f,zero,-curNX,curNX);
			F32vec4 mask = _mm_cmpgt_ps(f,zero);						

			// is there a better way to flip signs ??
			curNX = ((mask & curNX) | F32vec4((_mm_andnot_ps(mask,zero-curNX))));
			curNY = ((mask & curNY) | F32vec4((_mm_andnot_ps(mask,zero-curNY))));
			curNZ = ((mask & curNZ) | F32vec4((_mm_andnot_ps(mask,zero-curNZ))));
#endif


			CLEARCOLORDIFFUSE(); // diffuse only

			DOLIGHT();

			// clamp
			color.x = simd_min(color.x,twofivefive);
			color.y = simd_min(color.y,twofivefive);
			color.z = simd_min(color.z,twofivefive);


			UNPACKXYZRGB();
	}
	
	_m_empty(); 

}		


#undef DOLIGHT()


// 1 dir light with specular
#define DOLIGHT() KLightDiffuseSpecular(&light,curNX,curNY,curNZ,toViewerx,toViewery,toViewerz,power,color,specularColor); \
					color.x += light.ambient.x,color.y += light.ambient.y,color.z += light.ambient.z;

void KatmaiTransformLight_1_dir_spec_twosided(
			   int n, 
			   float  *v_,
			   float  *vn_,
			   float m[4][4],
			   float sx,float tx, float sy,float ty, // 2D viewpoint transform
			   D3DTLVERTEX *vtlp
			)
{

	SETUPMAT();
	
	SETUPXYZ();

	SETUPNXNYNZ();


	n = (n+3) >> 2;

	F32vec4 curX, curY,curZ,curW; 
	// F32vec4 Xout,Yout, Zout;	

	F32vec4 curNX, curNY,curNZ; 
	
	KVec3 color;
	KVec3 specularColor;
	
	SETUPCOLOR();

	SETUPLIGHT();

    for (size_t i=0; i<n; i++) 
	{

			GETXYZ();

			TRANSFORMPACKXYZ();

			GETNXNYNZ();

			F32vec4 toViewerx=state.viewer.x-curX; 
			F32vec4 toViewery=state.viewer.y-curY; 
			F32vec4 toViewerz=state.viewer.z-curZ;

			// normalize (toViewerx,toViewery,toViewerz) ***
			F32vec4 f = toViewerx*toViewerx;
			f += toViewery*toViewery;
			f += toViewerz*toViewerz;
			f = rsqrt_normal(f);
			toViewerx *=f,toViewery *=f,toViewerz *=f;

			// dotprod normal to viewer
			f = curNX * toViewerx + curNY * toViewery + curNZ * toViewerz;

			// scale negative parts 
			// curNX = select_lt(f,zero,-curNX,curNX);
			F32vec4 mask = _mm_cmple_ps(f,zero);
			mask &= negmask;

			// is there a better way to flip signs ??
			//curNX = ((mask & curNX) | F32vec4((_mm_andnot_ps(mask,zero-curNX))));
			//curNY = ((mask & curNY) | F32vec4((_mm_andnot_ps(mask,zero-curNY))));
			//curNZ = ((mask & curNZ) | F32vec4((_mm_andnot_ps(mask,zero-curNZ))));

			curNX = _mm_xor_ps (curNX,mask);
			curNY = _mm_xor_ps (curNY,mask);
			curNZ = _mm_xor_ps (curNZ,mask);

			CLEARCOLOR();

			DOLIGHT();

			// clamp
			color.x = simd_min(color.x,twofivefive);
			color.y = simd_min(color.y,twofivefive);
			color.z = simd_min(color.z,twofivefive);

			specularColor.x = simd_min(specularColor.x,twofivefive);
			specularColor.y = simd_min(specularColor.y,twofivefive);
			specularColor.z = simd_min(specularColor.z,twofivefive);



			UNPACKXYZRGBSPEC();
	}

	
	_m_empty(); 

}		


#undef DOLIGHT()



//


// n lights with specular
#define DOLIGHT()  \
			unsigned int lmax = state.numLights; \
			for(unsigned int j=0; j<lmax; j++) \
		        /* _mm_prefetch((char *) &state.lights[j+1], _MM_HINT_T0); */ \
				KLightGenericDiffuseSpecular(state.lightTypes[j],zdist,&state.lights[j], \
				curX,curY,curZ,curNX,curNY,curNZ,toViewerx,toViewery,toViewerz,power,color,specularColor,fogFactor); 


void KatmaiTransformLight_generic_spec(
		 
			   int n, 
			   float  *v_,
			   float  *vn_,
			   float m[4][4],
			   float sx,float tx, float sy,float ty, // 2D viewpoint transform
			   D3DTLVERTEX *vtlp
			)
{

	SETUPMAT();
	
	SETUPXYZ();

	SETUPNXNYNZ();


	n = (n+3) >> 2;

	F32vec4 curX, curY,curZ,curW; 
	//F32vec4 Xout,Yout, Zout;	

	F32vec4 curNX, curNY,curNZ; 
	
	KVec3 color;
	KVec3 specularColor;
	
	SETUPCOLOR();
	
    for (size_t i=0; i<n; i++) 
	{

			GETXYZ();

			TRANSFORMPACKXYZ();

			GETNXNYNZ();
			
			F32vec4 toViewerx=state.viewer.x-curX; 
			F32vec4 toViewery=state.viewer.y-curY; 
			F32vec4 toViewerz=state.viewer.z-curZ;


			// normalize (toViewerx,toViewery,toViewerz) ***
			F32vec4 f = toViewerx*toViewerx;
			f += toViewery*toViewery;
			f += toViewerz*toViewerz;
			f = rsqrt_normal(f);
			toViewerx *=f,toViewery *=f,toViewerz *=f;

			CLEARCOLOR();

			DOLIGHT();

			// clamp
			color.x = simd_min(color.x,twofivefive);
			color.y = simd_min(color.y,twofivefive);
			color.z = simd_min(color.z,twofivefive);

			specularColor.x = simd_min(specularColor.x,twofivefive);
			specularColor.y = simd_min(specularColor.y,twofivefive);
			specularColor.z = simd_min(specularColor.z,twofivefive);



			UNPACKXYZRGBSPEC();
	}

	_m_empty(); 
		
}		


void KatmaiTransformLight_generic_spec_twosided(

			   int n, 
			   float  *v_,
			   float  *vn_,
			   float m[4][4],
			   float sx,float tx, float sy,float ty, // 2D viewpoint transform
			   D3DTLVERTEX *vtlp
			)
{

	SETUPMAT();
	
	SETUPXYZ();

	SETUPNXNYNZ();


	n = (n+3) >> 2;

	F32vec4 curX, curY,curZ,curW; 

	F32vec4 curNX, curNY,curNZ; 
	
	KVec3 color;
	KVec3 specularColor;

	SETUPCOLOR();
	
    for (size_t i=0; i<n; i++) 
	{

			GETXYZ();

			TRANSFORMPACKXYZ();

			GETNXNYNZ();

			F32vec4 toViewerx=state.viewer.x-curX; 
			F32vec4 toViewery=state.viewer.y-curY; 
			F32vec4 toViewerz=state.viewer.z-curZ;


			// normalize (toViewerx,toViewery,toViewerz) ***
			F32vec4 f = toViewerx*toViewerx;
			f += toViewery*toViewery;
			f += toViewerz*toViewerz;
			f = rsqrt_normal(f);
			toViewerx *=f,toViewery *=f,toViewerz *=f;

			// dotprod normal to viewer
			f = curNX * toViewerx + curNY * toViewery + curNZ * toViewerz;

			// scale negative parts 
#if 1
			F32vec4 mask = _mm_cmple_ps(f,zero);
			mask &= negmask;


			curNX = _mm_xor_ps (curNX,mask);
			curNY = _mm_xor_ps (curNY,mask);
			curNZ = _mm_xor_ps (curNZ,mask);
#else
			// curNX = select_lt(f,zero,-curNX,curNX);
			F32vec4 mask = _mm_cmpgt_ps(f,zero);						

			// is there a better way to flip signs ??
			curNX = ((mask & curNX) | F32vec4((_mm_andnot_ps(mask,zero-curNX))));
			curNY = ((mask & curNY) | F32vec4((_mm_andnot_ps(mask,zero-curNY))));
			curNZ = ((mask & curNZ) | F32vec4((_mm_andnot_ps(mask,zero-curNZ))));
#endif

			CLEARCOLOR();


			DOLIGHT();

			// clamp
			color.x = simd_min(color.x,twofivefive);
			color.y = simd_min(color.y,twofivefive);
			color.z = simd_min(color.z,twofivefive);

			specularColor.x = simd_min(specularColor.x,twofivefive);
			specularColor.y = simd_min(specularColor.y,twofivefive);
			specularColor.z = simd_min(specularColor.z,twofivefive);



			UNPACKXYZRGBSPEC();
	}

	_m_empty(); 
		
}		
void KatmaiLight_generic_spec_twosided(
			   int n, 
			   float  *v_,
			   float  *vn_,
			   D3DTLVERTEX *vtlp
			)
{

	
	SETUPXYZ();

	SETUPNXNYNZ();


	n = (n+3) >> 2;

	F32vec4 curX, curY,curZ,curW; 

	F32vec4 curNX, curNY,curNZ; 
	
	KVec3 color;
	KVec3 specularColor;

	SETUPCOLOR();
	
    for (size_t i=0; i<n; i++) 
	{

			GETXYZ();


			GETNXNYNZ();
			F32vec4 f = curNX*curNX;
			f += curNY*curNY;
			f += curNZ*curNZ;
			f = rsqrt_normal(f);

			curNX *=f,curNY *=f,curNZ *=f;

			F32vec4 toViewerx=zero-curX; 
			F32vec4 toViewery=zero-curY; 
			F32vec4 toViewerz=zero-curZ;


			// normalize (toViewerx,toViewery,toViewerz) ***
			f = toViewerx*toViewerx;
			f += toViewery*toViewery;
			f += toViewerz*toViewerz;
			f = rsqrt_normal(f);
			toViewerx *=f,toViewery *=f,toViewerz *=f;

			// dotprod normal to viewer
			f = curNX * toViewerx + curNY * toViewery + curNZ * toViewerz;

			// scale negative parts 
			F32vec4 mask = _mm_cmple_ps(f,zero);
			mask &= negmask;

			curNX = _mm_xor_ps (curNX,mask);
			curNY = _mm_xor_ps (curNY,mask);
			curNZ = _mm_xor_ps (curNZ,mask);


			CLEARCOLOR();


			DOLIGHT();

			// clamp
			color.x = simd_min(color.x,twofivefive);
			color.y = simd_min(color.y,twofivefive);
			color.z = simd_min(color.z,twofivefive);

			specularColor.x = simd_min(specularColor.x,twofivefive);
			specularColor.y = simd_min(specularColor.y,twofivefive);
			specularColor.z = simd_min(specularColor.z,twofivefive);



			UNPACKRGBSPEC2();
	}

	_m_empty(); 
		
}		


/*
Do backface culling and create triangle outlist

*/



int KatmaiBuildFaceList(const int *fp_,const int *fpend_, 
						float *fn_,float *fv_,
						WORD *fpout)
{
	__m128 *fv =(__m128 *) fv_;
	__m128 *fn =(__m128 *) fn_;

	_mm_prefetch((char *)(&fv[0]), _MM_HINT_NTA);

	WORD *fpi = fpout;

	const int *fp = fp_;
	const int *fpend = fpend_;

	_mm_prefetch((char *)(&fp[0]), _MM_HINT_NTA);
	

	while (fp < fpend) 
	{

			//_mm_prefetch((char *)(&fp[0]), _MM_HINT_NTA);
			//_mm_prefetch((char *)(&fp[8]), _MM_HINT_NTA);

			_mm_prefetch((char *)(&fn[3]), _MM_HINT_NTA);
			_mm_prefetch((char *)(&fn[5]), _MM_HINT_NTA);

			F32vec4 toViewerx=state.viewer.x-fv[0]; 
			F32vec4 toViewery=state.viewer.y-fv[1];
			F32vec4 toViewerz=state.viewer.z-fv[2];

			fv+=3;
			_mm_prefetch((char *)(&fv[0]), _MM_HINT_NTA);
			_mm_prefetch((char *)(&fv[3]), _MM_HINT_NTA);
/*
			F32vec4 f = toViewerx * toViewerx + toViewery * toViewery + toViewerz * toViewerz;
			f = rsqrt_normal(f);
			toViewerx *=f,toViewery *=f,toViewerz *=f;
*/

			F32vec4 dot =  (toViewerx * fn[0] + toViewery * fn[1]  + toViewerz * fn[2]);
			
			fn+=3;

			int m;
			
			m = _mm_movemask_ps(dot); // get the 4 sign bits
			// m |= _mm_movemask_ps(_mm_cmpeq_ps(dot,zero));
			//m = _mm_movemask_ps(_mm_cmplt_ps(dot,eps));

			unsigned int j=4; // for 4 polygons 
			// do 

			for (j=0; j<4;j++)
			{
			_mm_prefetch((char *)(&fp[8]), _MM_HINT_NTA);

			int flen = *fp++;
			
			if ( (m & (1)) && (flen>2) ) { // not culled store it 
					int p1= fp[0]; int p2= fp[1];
					size_t i=2;
					for (i=2; i<flen; i++)
					//do 
					{
						fpi[0] = p1; fpi[1] = p2; 
						p2 = fp[i];
						fpi[2] = p2;
						fpi+=3; 
						//i++;
					} //while (i<flen);
			}
			
			fp+=flen;
			
			if (fp>=fpend) goto ret;
			m = m>>1;
			//j--;
			} 
			//while (j>0);

	}
ret:
	return (fpi-fpout); // number of WORDS stored at fpout

}


// build face list with marking of used vertices 
int KatmaiBuildFaceListMark(const int *fp_,const int *fpend_, 
						float *fn_,float *fv_,
						int *vertexMark,
						WORD *fpout)
{
	__m128 *fv =(__m128 *) fv_;
	__m128 *fn =(__m128 *) fn_;

	WORD *fpi = fpout;

	const int *fp = fp_;
	const int *fpend = fpend_;

	
	F32vec4 curX, curY,curZ; 
	
	F32vec4 curNX, curNY,curNZ; 

	while (fp < fpend) 
	{
			curX = *fv++;curY = *fv++; curZ = *fv++;
	
			curNX = *fn++;curNY = *fn++; curNZ = *fn++;

			F32vec4 toViewerx=state.viewer.x-curX; 
			F32vec4 toViewery=state.viewer.y-curY; 
			F32vec4 toViewerz=state.viewer.z-curZ;

			F32vec4 dot = toViewerx * curNX + toViewery *curNY  + toViewerz * curNZ;

			int m;
			
			m = _mm_movemask_ps(dot); // get the 4 sign bits

			int j=4; // for 4 polygons 
			// do 
			int flen;

			for (j=0; j<4;j++)
			{

			flen = *fp++;
			if ( !(m & 1) && (flen>2) ) { // not culled store it 
					int p1= fp[0]; int p2= fp[1];
					size_t i=2;

					vertexMark[p1]= p1;
					vertexMark[p2]= p2;

					for (i=2; i<flen; i++)
					//do 
					{
						fpi[0] = p1; fpi[1] = p2; 
						p2 = fp[i];
	
						vertexMark[p2]= p2;

						fpi[2] =p2;
						fpi+=3; 
						//i++;
					} //while (i<flen);
			}
			
			fp+=flen;
			
			if (fp>=fpend) goto ret;
			m = m>>1;
			//j--;
			} 
			//while (j>0);

	}
ret:
	return (fpi-fpout);

}


int KatmaiRayIntersect(const int *fp,const int *fpend, 
						float *fn_,float *fv_,
						Point direction,Point position,
						WORD *fpout)
{
	__m128 *fv =(__m128 *) fv_;
	__m128 *fn =(__m128 *) fn_;

	KVec3 dir;
	KVec3 pos;
	setKVec3(dir,direction);
	setKVec3(pos,position);



	WORD *fpi = fpout;

	while (fp < fpend) 
	{
/*
			
			F32vec4 tdot = dir.x * fn[0] + dir.y *fn[1]  + dir.z * fn[2];
			// if near zero ==> perpendicular

			// parameter d of plane eq.
			F32vec4 tdist ; = fv[0] * fn[0] + fv[1] * fn[1]  + fv[2] * fn[2];
			F32vec4 tdot1 = pos.x * fn[0] + pos.y *fn[1]  + pos.z * fn[2];

			// compute the t-parameter
			F32vec4 t = (tdist - tdot1) / tdot;
*/

			// eval point on ray
			// check if point is in triangle

/*			// clamp to to range 

			F32vec4 toViewerx=state.viewer.x-fv[0]; 
			F32vec4 toViewery=state.viewer.y-fv[1]; 
			F32vec4 toViewerz=state.viewer.z-fv[2];

			F32vec4 dot = toViewerx * fn[0] + toViewery *fn[1]  + toViewerz *fn[2];
			fn+=3,fv+=3;
			int m;
			
			m = _mm_movemask_ps(dot); // get the 4 sign bits

			int j=4; // for 4 polygons 
			do {

			int flen = *fp++;

			if (m&1) { // not culled store it 
					WORD p1= fp[0]; WORD p2= fp[1];
					size_t i=2;
					do {
						fpi[0] = p1; fpi[1] = p2; 
						p2 = fpi[2] = fp[i];
						fpi+=3; i++;
					} while (i<flen);
			}
			fp+=flen;
			
			if (fp>fpend) break;
			m = m>>1;
			j--;
			} while (j>0);
*/
	}

	return (fpi-fpout);

}


void KatmaiComputeVertexNormals(const int *f,int flength, // face list
								const KGVertexI *verts,
								const Point *fn, // face normals
								int vnLength,
								Point *vn // computed vertex normals	
								)
{



	// zero out Vertex normals
   { Point *p = vn,*pend = &p[vnLength];
		while (p < pend) {
			p->x=0;
			p->y=0;
			p->z=0;
			p++;
		}
   }

   const int *fp = f;			   // face list stepping pointers
   const int *fpend= fp + flength;

   int fi=0;	// face index
   
   do { 		// for all faces
		 int flen = *fp++; // length of this face
		 if (flen<0) flen=-flen;

		 const Point &fn_ = fn[fi];	

		 for (size_t i=0; i< flen; i++) {
				int vi = *fp++; // vertex index
				vn[verts[vi].n] += fn_;	// add face normal to vn slot
		 }

		 fi++ ;
   } while (fp <fpend);


   // Normalize vn array
   { Point *p = vn,*pend = &p[vnLength];
		while (p < pend) {
			// p->Normalize();

			F32vec4 n(p->x,p->y,p->z,0.0);

			F32vec4 dot = n * n;
			// rsqrt_normal_
			n = n * _mm_rsqrt_ps(F32vec4(add_horizontal(dot)));

			p->x=n[3];
			p->y=n[2];
			p->z=n[1];

			

			p++;
		}
   }
}

#endif



void KatmaiMatrixMul(const float a[4][4],const float b[4][4],float result[4][4]);
void KatmaiMatrixTranspose(const float a[4][4],float result[4][4]);
void KatmaiMatrixInvert(const float a[4][4],float result[4][4]);

// Point : Len() len2() .Normalize()
// void KatmaiNormalize(float v[4],float result[4]) = l = (sqrt(

#ifdef _D3D
void KatmaiTransformTextureCoord(const float *x,
						   const float *y,
						   int n,
						   float m[4][4],
						   D3DTLVERTEX *vtlp)
{

	for(size_t i=0; i < n; i++) {
			// vertex 
			const Point p(x[i],y[i],0.0);

			vtlp->tu = p.x * m[0][0] + p.y * m[1][0] /* + p.z * m[2][0] */ + m[3][0];
			vtlp->tv=  p.x * m[0][1] + p.y * m[1][1] /* + p.z * m[2][1] */+ m[3][1];
			vtlp ++;

	}

}

void KatmaiCopyTextureCoord(const float *x,
						   const float *y,
						   int n,
						   D3DTLVERTEX *vtlp)
{

	for(size_t i=0; i < n; i++) {
			vtlp->tu = *x++;
			vtlp->tv=  *y++;
			vtlp ++;

	}

}

#endif



#ifndef _KATMAI

#ifdef _D3D
HRESULT GNurbsShell::D3DRenderKatmai(RenderState &state, 
		const Matrix &m_MxProjection, // projection 
	    float sx,float tx, float sy,float ty, // 2D viewpoint transform
		DWORD clipFlags	// for DrawPrimitive
		)
{
	return S_FALSE;
}

void GNurbsShell::TransformControlPointsKatmai(const Matrix &m)
{

	TransformControlPoints(m);

}

#endif  _D3D


#else  // _KATMAI



void GNurbsShell::TransformControlPointsKatmai(const Matrix &m)
{

//	TransformControlPoints(m);
//	return;
/*
	float zMin;
	float rhw;

	zMin = FLT_MAX;
*/
	
	// __m128t *pControlPoints =	(__m128 *) m_ControlPoints.Data();
	HPoint *pControlPoints = (HPoint *) m_ControlPoints.Data();

	__m128 *pTxControlPoints = (__m128 *)m_TxControlPoints.Data();

	int imax=(m_iUControlPoints * m_iVControlPoints);
	
	 __m128 m0,m1,m2,m3; // matrix on stack aligned 

	m0 = _mm_loadu_ps((float *)&m.x[0][0]);
	m1 = _mm_loadu_ps((float *)&m.x[1][0]);
	m2 = _mm_loadu_ps((float *)&m.x[2][0]);
	m3 = _mm_loadu_ps((float *)&m.x[3][0]);

	//for( int i=imax; i>0; i-- )
	__m128 tmp,res,x,y,z,w;

	for( int i=0; i<imax; i++ )
	{
		//_mm_set_ps1
		x = _mm_load_ss(&pControlPoints->x);
		x = _mm_shuffle_ps( x, x, 0 );

		res = _mm_mul_ps(m0,x);

		y = _mm_load_ss(&pControlPoints->y);
		y = _mm_shuffle_ps( y, y, 0 );

		res = _mm_add_ps(res,_mm_mul_ps(m1,y));

		z = _mm_load_ss(&pControlPoints->z);
		z = _mm_shuffle_ps( z, z, 0 );

		res = _mm_add_ps(res,_mm_mul_ps(m2,z));

		w = _mm_load_ss(&pControlPoints->w);
		w = _mm_shuffle_ps( w, w, 0 );

		res = _mm_add_ps(res,_mm_mul_ps(m3,w));
		
		// *pTxControlPoints++ = res;

		_mm_store_ps((float *) pTxControlPoints,res);

		pTxControlPoints++;
		pControlPoints++;



	}

#if 0			
	Point TxbboxMin, TxbboxMax;
	
	TxbboxMin.x = 
				bboxMin.x * m.x[0][0] +
				bboxMin.y * m.x[1][0] +
				bboxMin.z * m.x[2][0] +
				m.x[3][0] ;
	TxbboxMin.y = 
				bboxMin.x * m.x[0][1] +
				bboxMin.y * m.x[1][1] +
				bboxMin.z * m.x[2][1] +
				m.x[3][1];
			TxbboxMin.z = 
				bboxMin.x * m.x[0][2] +
				bboxMin.y * m.x[1][2] +
				bboxMin.z * m.x[2][2] +
				m.x[3][2];
			rhw = 1/(
				bboxMin.x * m.x[0][3] +
				bboxMin.y * m.x[1][3] +
				bboxMin.z * m.x[2][3] +
				m.x[3][3]);	
	TxbboxMin.x *= rhw;
	TxbboxMin.y *= rhw;
	TxbboxMin.z *= rhw;

			
			TxbboxMax.x = 
				bboxMax.x * m.x[0][0] +
				bboxMax.y * m.x[1][0] +
				bboxMax.z * m.x[2][0] +
				m.x[3][0];
			TxbboxMax.y = 
				bboxMax.x * m.x[0][1] +
				bboxMax.y * m.x[1][1] +
				bboxMax.z * m.x[2][1] +
				m.x[3][1];
			TxbboxMax.z = 
				bboxMax.x * m.x[0][2] +
				bboxMax.y * m.x[1][2] +
				bboxMax.z * m.x[2][2] +
				m.x[3][2];
			rhw = 1/(
				bboxMax.x * m.x[0][3] +
				bboxMax.y * m.x[1][3] +
				bboxMax.z * m.x[2][3] +
				m.x[3][3]);
	TxbboxMax.x *= rhw;
	TxbboxMax.y *= rhw;
	TxbboxMax.z *= rhw;

		

			Point Length;
			Length = (TxbboxMax - TxbboxMin);
			m_fBBSurface = Length.Length();
			TRACE("TxbboxMin.z %f durchmesser der bbox %f\n", TxbboxMin.z, m_fBBSurface);
			m_fBBSurface  /= TxbboxMin.z;
			m_fBBSurface *= m_fBBSurface;
			TRACE("bbsurface %f\n", m_fBBSurface);
#endif
}



// Katmai lighting
#define xx 
//#undef xx 

HRESULT GNurbsShell::D3DRenderKatmai(RenderState &state, 
		const Matrix &m_MxProjection,
		// float w,float h, // projection & viewport information
	    float sx,float tx, float sy,float ty, // 2D viewpoint transform

		DWORD clipFlags	// for DrawPrimitive
		)
{
	
	// constants 
	const __m128 m0pt5 = _mm_set_ps1( 0.5f );
	const __m128 m3pt0 = _mm_set_ps1( 3.0f );

	unsigned int u, v;
	unsigned int k, l;

	int uKnot, vKnot;
	__m128 *UTemp = (__m128 *)m_UTemp.Data(), *dUTemp = (__m128 *)m_dUTemp.Data();
	__m128 QPw, QTanU, QTanV, QNorm;
	HPoint *Pw = (HPoint *)&QPw, 
			*TanU = (HPoint *)&QTanU, *TanV = (HPoint *)&QTanV, 
			*Norm = (HPoint *)&QNorm;
	__m128 *UBasis = (__m128 *)m_UBasis.Data();
	__m128 *dUBasis = (__m128 *)m_dUBasis.Data();
	__m128 *VBasis = (__m128 *)m_VBasis.Data();
	__m128 *dVBasis = (__m128 *)m_dVBasis.Data();
	__m128 tVBasis, tdVBasis;
	float rhw;
	float Len;
	unsigned int iVertices;
	__m128 *pTxPoints = (__m128 *)m_TxControlPoints.Data();
	//int iTxOffset;

	// tbd : copy on stack page ??
	// unsigned int iVDegree= m_iVDegree;

	int iSet = 0;
	unsigned int idx, uidx;

	__m128 Qtemp, Qtemp2, Qresult, Qrhw, Ra0, Qimmed1, Qimmed2;

	if( !m_bInitialized || !m_iUTessellations || !m_iVTessellations )
		return DD_OK;

	if( (m_iUTessellations != m_iLastUTess) ||
		(m_iVTessellations != m_iLastVTess) )
	{
		EvaluateBasisFunctions();
		m_iLastUTess = m_iUTessellations;
		m_iLastVTess = m_iVTessellations;
	}

	iVertices = 2 * (m_iVTessellations + 1);

//	float m_fHalfX = w;
//	float m_fHalfY = h;

	float Proj00 = m_MxProjection.x[0][0] * sx; // m_fHalfX;
	float Proj11 = -m_MxProjection.x[1][1]* sy; // m_fHalfY;
	float Proj22 = m_MxProjection.x[2][2];
	float Proj23 = m_MxProjection.x[2][3];
	float Proj32 = m_MxProjection.x[3][2];

	__m128 QProj23 = _mm_set_ps1( Proj23 );
	__m128 QProj1 = _mm_set_ps( 0.0f, Proj22, Proj11, Proj00 );
	__m128 QProj2 = _mm_set_ps( 1.0f, Proj32, 0.0f, 0.0f );
//	__m128 QProj3 = _mm_set_ps( 0.0f, 0.0f, m_fHalfY, m_fHalfX );
	__m128 QProj3 = _mm_set_ps( 0.0f, 0.0f, ty, tx);


	D3DTLVERTEX *m_pScreenVertices = GetScreenVertices(iVertices);
//	__m128 *pVertexPos = (__m128 *)m_pVertexPos.Data();

#ifndef xx
	HPoint *pVertexPos = (HPoint *)m_pVertexPos.Data();
#endif

	// xxxxxx PrepLightsKatmai( pd3dDevice );

	float Fuinc = 1.0f/(float)m_iUTessellations;
	float Fvinc = 1.0f/(float)m_iVTessellations;
	float Fu = 0.0f;
	float Fv;

	// Lighting stuff
	gbool culling = state.backfaceCulling; // backface culling enabled ?
	gbool singlsidelighting = culling;


	gbool doVp = state.textureDefined;
	gbool doVn =  !state.Unlit();
	gbool doVc = gfalse;

	if (doVp) { // transform texture coordinates
		// recreate texture coordinates	
		textureMatrixVersion = state.GetTextureMatrixVersion();
		
		if (state.textureMatrixIdentity) {
			doVp = gfalse;
		}
	}



	//
	// Step over the U and V coordinates and generate triangle strips to render
	//
	for( u=0; u<=m_iUTessellations; u++ )
	{
		uKnot = m_TessUKnot[u];
		// TRACE(" u %d %d \n",u,m_iUTessellations);
		uidx = u * m_iUOrder;
		vKnot = -1;
		idx = iSet;
		Fv = 0.0f;
		unsigned int vidx=0;
		unsigned int iTxOffset0 = (uKnot - m_iUDegree) * m_iVControlPoints;

		for( v=0; v<=m_iVTessellations; v++, idx+=2, vidx += m_iVOrder )
		{
			if( vKnot != m_TessVKnot[v] )
			{
				vKnot = m_TessVKnot[v];
				unsigned int iTxOffset1 = iTxOffset0 + (vKnot - m_iVDegree);

				for( k=0; k<=m_iVDegree; k++)
				{
					//iTxOffset = (uKnot - m_iUDegree) * m_iVControlPoints + (vKnot - m_iVDegree);
					
					unsigned int iTxOffset = iTxOffset1;
					
					//_mm_prefetch((char *)(&pTxPoints[ iTxOffset + k + m_iVControlPoints ]), _MM_HINT_NTA);

					UTemp[k] = _mm_mul_ps( UBasis[uidx], pTxPoints[ iTxOffset + k ] );
					dUTemp[k] = _mm_mul_ps( dUBasis[uidx], pTxPoints[ iTxOffset + k ] );
					for( l=1; l<=m_iUDegree; l++ )
					{
						iTxOffset += m_iVControlPoints;
						_mm_prefetch((char *)(&pTxPoints[ iTxOffset + k + (m_iVControlPoints*2) ]), _MM_HINT_NTA);
						UTemp[k] = _mm_add_ps( UTemp[k], 
										_mm_mul_ps( UBasis[uidx + l], 
													pTxPoints[ iTxOffset + k ] ) );
						dUTemp[k] = _mm_add_ps( dUTemp[k], 
							_mm_mul_ps( dUBasis[uidx + l],
										pTxPoints[ iTxOffset + k ] ) );
					}
					// _mm_prefetch((char *)(&m_TessVKnot[v+4]), _MM_HINT_NTA);
				}

			}

			tVBasis = VBasis[ vidx ];
			tdVBasis = dVBasis[ vidx ];
			QPw = _mm_mul_ps( tVBasis, UTemp[0] );
			QTanU = _mm_mul_ps( tVBasis, dUTemp[0] );
			
			_mm_prefetch((char *)(& VBasis[ (vidx + 2) ]), _MM_HINT_NTA);

			QTanV = _mm_mul_ps( tdVBasis, UTemp[0] );

			for( k=1; k<=m_iVDegree; k++ )
			{
				_mm_prefetch((char *)(& dVBasis[ (vidx + k + 2) ]), _MM_HINT_NTA);

				tVBasis = VBasis[ (vidx + k) ];
				tdVBasis = dVBasis[ (vidx + k) ];

				QPw = _mm_add_ps( QPw, _mm_mul_ps( tVBasis, UTemp[k] ) );
				QTanU = _mm_add_ps( QTanU, _mm_mul_ps( tVBasis, dUTemp[k] ) );
				
				_mm_prefetch((char *)(& VBasis[ (vidx + k + 2) ]), _MM_HINT_NTA);

				QTanV = _mm_add_ps( QTanV, _mm_mul_ps( tdVBasis, UTemp[k] ) );
			}

			//
			// Divide through by w
			//
			Qtemp = _mm_shuffle_ps( QPw, QPw, _MM_SHUFFLE(3,3,3,3) );
			Ra0 = _mm_rcp_ps( Qtemp );
			Qrhw = _mm_sub_ps(_mm_add_ps(Ra0, Ra0), 
							  _mm_mul_ps( _mm_mul_ps(Ra0, Qtemp), Ra0));
			QPw = _mm_mul_ps( QPw, Qrhw );

			//_mm_prefetch((char *)(& VBasis[ (vidx + 1) ]), _MM_HINT_NTA);

			//
			// Convert the Tangents from Homogenous to Euclidean space
			//
			Qtemp = _mm_shuffle_ps( QTanU, QTanU, _MM_SHUFFLE(3,3,3,3) ); // Broadcast TanU.w component into Qtemp
			QTanU = _mm_mul_ps( _mm_sub_ps( QTanU, _mm_mul_ps( Qtemp, QPw ) ), Qrhw );
			Qtemp2 = _mm_shuffle_ps( QTanV, QTanV, _MM_SHUFFLE(3,3,3,3) );
			QTanV = _mm_mul_ps( _mm_sub_ps( QTanV, _mm_mul_ps( Qtemp2, QPw ) ), Qrhw );

			//_mm_prefetch((char *)(&dVBasis[ (vidx + 1) ]), _MM_HINT_NTA);

			//
			// Determine the normal from the cross product of the two tangents
			//
			Qtemp = _mm_shuffle_ps( QTanU, QTanU, _MM_SHUFFLE(3,0,2,1 ) );
			Qtemp = _mm_mul_ps( QTanV, Qtemp );
			Qtemp2 = _mm_shuffle_ps( QTanV, QTanV, _MM_SHUFFLE(3,0,2,1 ) );
			Qtemp2 = _mm_mul_ps( QTanU, Qtemp2 );

			Qresult = _mm_sub_ps( Qtemp, Qtemp2 );
			QNorm = _mm_shuffle_ps( Qresult, Qresult, _MM_SHUFFLE(3,0,2,1) );

#ifndef xx
			//
			// Normalize the normal
			//
			Qresult = _mm_mul_ps( QNorm, QNorm );
			Qimmed1 = _mm_unpacklo_ps( Qresult, Qresult );
			Qimmed2 = _mm_add_ps( Qimmed1, Qresult );
			Qresult = _mm_unpackhi_ps( Qresult, Qresult );
			Qresult = _mm_add_ps( Qimmed2, Qresult );
			Qresult = _mm_shuffle_ps( Qresult, Qresult, _MM_SHUFFLE(1,1,1,1) );

			Ra0 = _mm_rsqrt_ps( Qresult );
			QNorm = _mm_mul_ps( QNorm, 
							   _mm_mul_ps( _mm_mul_ps( m0pt5, Ra0 ), 
										   _mm_sub_ps( m3pt0,
													   _mm_mul_ps( _mm_mul_ps( Qresult, Ra0 ), 
																   Ra0 
																 )
													 )
										 )
							  );
#endif
			//
			// Save out the vertex position (in View space )
			//
#ifdef xx 
			//int index =  (v>>2)*P4_STRIDE + (v & 3);
			//kv[index] = Pw->x,	kv[index+4] = Pw->y,	kv[index+8] = Pw->z;
			kv.Set1(v,Pw->x,Pw->y,Pw->z);
#else
	
			pVertexPos[v].x = Pw->x;
			pVertexPos[v].y = Pw->y;
			pVertexPos[v].z = Pw->z;
		
			//pVertexPos[v] = *QPw;
#endif
			//
			// Now calculate the projected vertex in screen space
			//
			Qrhw = QProj23;
			Qtemp = _mm_mul_ps( Qrhw, _mm_shuffle_ps( QPw, QPw, _MM_SHUFFLE(2,2,2,2)) );
			Ra0 = _mm_rcp_ps( Qtemp );
			Qrhw = _mm_sub_ps(_mm_add_ps(Ra0, Ra0), 
							  _mm_mul_ps(_mm_mul_ps(Ra0, Qtemp), Ra0));

			QPw = _mm_mul_ps( QPw, QProj1 );
			QPw = _mm_mul_ps( _mm_add_ps( QPw, QProj2 ), Qrhw );
			QPw = _mm_add_ps( QPw, QProj3 );

			*(__m128 *)&m_pScreenVertices[idx].sx = QPw; // HG streaming write ?

			m_pScreenVertices[ idx ].tu = Fu;
			m_pScreenVertices[ idx ].tv = Fv;
#ifdef xx 

			kvn.Set1(v,Norm->x,Norm->y,Norm->z);
#else
			m_pNormals[v].x = Norm->x; // HG Katmai ??
			m_pNormals[v].y = Norm->y;
			m_pNormals[v].z = Norm->z;
#endif
			Fv += Fvinc;
		}
		Fu += Fuinc;

		//if (flipNormals) {
		//} 

#ifdef xx 
		if (doVp) { // transform texture coordinates by texture matrix
				D3DTLVERTEX *vtlp= m_pScreenVertices+iSet;

				const Matrix &M = state.textureMatrix;
				for(unsigned int i = 0;  i <= m_iVTessellations; i++) {
					float x =vtlp->tu;
					float y =vtlp->tv;
					vtlp->tu = x * M.x[0][0] + y * M.x[1][0] /* + p.z * M.x[2][0] */ + M.x[3][0];
					vtlp->tv=  x * M.x[0][1] + y * M.x[1][1] /* + p.z * M.x[2][1] */+ M.x[3][1];
					vtlp +=2;
				}
		}
		
		KatmaiLight_generic_spec_twosided(m_iVTessellations+1,kv.Data(),kvn.Data(), m_pScreenVertices+iSet);
#else

		//xxxxxxxxxxxxx LightVerticesKatmai( pd3dDevice, pd3dSurfMaterial, iSet );
		LightVertices(state,m_iVTessellations+1,(HPoint *)(pVertexPos),m_pNormals, m_pScreenVertices+iSet,2);
		//LightVerticesKatmai(state,m_iVTessellations+1,kv.Data(),kvn.Data(), m_pScreenVertices+iSet,2);

#endif

		if( u > 0 )
		{
			if (triStrip)
				state.lpD3DDevice2->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, D3DVT_TLVERTEX,
							  m_pScreenVertices, iVertices, // number of vertices 
							  m_iVertexIndices[iSet], iVertices, // number of indices
							  clipFlags );
			else 
				state.lpD3DDevice2->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DVT_TLVERTEX,
							  m_pScreenVertices, iVertices, // number of vertices 
							  m_iVertexIndices[iSet],3* 2 * (m_iVTessellations ),
							  // number of indices
							  clipFlags );
		}
		iSet = 1 - iSet;
	}

	return DD_OK;
}

#endif // Katmai







