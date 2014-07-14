/******************************************************************************
@doc

@module GKatmai.cpp - blaxxun CC3D Katmai Transform, Light & Utility routines|

Copyright (c) 1998	blaxxun interactive 
All rights reserved

Purpose:
Intel Katmai Transform Light & Utilities

Classes:

KState - Katmai global lighting state


Notes:
Parts of this module are based on samples and ideas from Intel

Changes:

$Revision: 1.2 $
$Log: gkatmai_soa.cpp,v $
Revision 1.2  1998/09/29 11:48:28  holger
empty message

Revision 1.1  1998/09/22 16:37:30  holger
empty message


Todo :

******************************************************************************/

// #include "stdafx.h"

#include "d3dtypes.h"

#include <math.h>
#include <Point.h>

#include "gkatmai.h"

// for scaling colors to RGBA_MAKE

#define KRGB_SCALE 255.0f 



typedef struct  Vertex_Tag  { float x; 
							  float y;
						 	  float z;
							  float w; 
							} Vertex; 



#ifdef _KATMAI



// requires Intel compiler


__declspec(align(16)) float M[4][4]; 


#include <xmmintrin.h> 
#include <fvec.h>


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
void setKVec3(KVec3 &v,Point val)
{
	v.x = F32vec4(val.x),
	v.y = F32vec4(val.y),
	v.z = F32vec4(val.z);
}

void setKVec3(KVec3 &v,float x,float y,float z)
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

// _MM_ALIGN16

class KLightState 
{
public :
	KVec3 direction; // - direction of directional light

	KVec3 ambient;	// material specular color multiplied with light color

	KVec3 diffuse;	// material diffuse color multiplied with light color
	
	KVec3 specular;	// material specular color multiplied with light color

};



typedef struct 
{
	KMatrix m; // object to screen transformation
	KVec3	viewer; // viewer position in local coordinate space
	
	KVec3	ambient;
	KVec3	emissive;	// current material emissive color
	F32vec4 power;

	KLightState lights[16]; // diffuse lights
	int numLights;
	int alpha;


} KState;

void KSetMatrix(KState *state,Matrix &m);


_MM_ALIGN16 

	KState state;		// the lighting context 


// see what code IC is generating
int KF2int(float f) 
{ return f; }

unsigned int KF2uint(float f) 
{ return f; }


// set number of used lights 
void KSetNumLights(int num)
{
	state.numLights = num;
}


void KSetLightDirection(int i,Point dir)
{
	setKVec3(state.lights[i].direction,dir);
}

// set light color (multiplied with material color 
void KSetLightColors(int i,
					 D3DCOLORVALUE ambient,D3DCOLORVALUE diffuse,D3DCOLORVALUE specular)
{
	setKVec3(state.lights[i].ambient,ambient.r*KRGB_SCALE,ambient.g*KRGB_SCALE,ambient.b*KRGB_SCALE);
	setKVec3(state.lights[i].diffuse,diffuse.r*KRGB_SCALE,diffuse.g*KRGB_SCALE,diffuse.b*KRGB_SCALE);
	setKVec3(state.lights[i].specular,specular.r*KRGB_SCALE,specular.g*KRGB_SCALE,specular.b*KRGB_SCALE);
}

// set  material emissive color , alpha & power
void KSetBasicMaterial( D3DCOLORVALUE emissive,float alpha,float power)
{
	setKVec3(state.emissive,emissive.r*KRGB_SCALE,emissive.g*KRGB_SCALE,emissive.b*KRGB_SCALE);
	state.alpha = alpha * KRGB_SCALE;
	state.power = F32vec4(power);

}

// set position of viewer in local coordinates
void KSetViewerPosition(Point position)
{
	setKVec3(state.viewer,position);
}






// constants 

_MM_ALIGN16 
static const F32vec4 zero(0.0f);
static const F32vec4 twofivefive(255.0f);



// diffuse lighting for directional light
static inline void KLightDiffuse(KLightState *l,
								   F32vec4 nx,F32vec4 ny,F32vec4 nz,
								   KVec3 &color
								   ) 
{

	F32vec4 f =	nx * l->direction.x + ny * l->direction.y + nz * l->direction.z ;

	f=simd_max(f,zero);

	// addin diffuse part 

	color.x += l->diffuse.x*f,color.y += l->diffuse.y*f,color.z += l->diffuse.z*f;

}

// approximation for shininess
#define SPOW(dot,shin) dot*rcp(shin-shin*dot+dot)

// diffuse/spec lighting for directional light
static inline void KLightDiffuseSpecular(KLightState *l,

								   F32vec4 nx,F32vec4 ny,F32vec4 nz,
								   F32vec4 toViewerx, F32vec4 toViewery, 
								   F32vec4 toViewerz,
								   F32vec4 power,
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
	f = rsqrt_nr(f);
	tmpx *=f,tmpy *=f,tmpz *=f;


		f = nx * tmpx + ny * tmpy + nz * tmpz; // dotprod

		f=simd_max(f,zero);


		f= SPOW(f,power);

		// add in specularColor part 

		specularColor.x += l->specular.x*f,specularColor.y += l->specular.y*f,specularColor.z += l->specular.z*f;

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
	
    for (int i=0; i<n; i++) 
	{
			_mm_prefetch((char *)(&x[i+2]), _MM_HINT_NTA); 
		    _mm_prefetch((char *)(&y[i+2]), _MM_HINT_NTA); 
		    _mm_prefetch((char *)(&z[i+2]), _MM_HINT_NTA);

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
D3DMATRIX KatmaiMatrixMult_Int( const float a[4][4], const float b[4][4] )
{
	//
	// Use a union of an Fvec32 with a D3DMATRIX so the D3DMATRIX
	// gets paragraph aligned
	//
	union
	{
		__m128 fret[4];
		//float ret[4][4]
		D3DMATRIX ret;
	} Result;

	F32vec4 fa[4], fb;

	int i,j;

	// Load all of the 'a' matrix into the 'fa' array
	// one row at a time like this:
	//			-------------------
	// fa[i] =  | mi0 mi1 mi2 mi3 |
	//			-------------------
	// See the file xmmfunc.h for info on _mm_loadu_ps and all the
	// intrinsics
	for( i=0; i<4; i++ )
		fa[i] = _mm_loadu_ps( (float *)(&a[i][0]) );

	// Result.ret = ZeroMatrix();

	// Compute the result
	for( i=0; i<4; i++ )
	{
		for( j=0; j<4; j++)
		{
			fb = _mm_load_ss( (float *) &b[i][j] );
			Result.fret[i] =  ( //_mm_add_ps( Result.fret[i], 
										_mm_mul_ps( fa[j], 
											_mm_shuffle_ps( fb, fb, 0 ) ) );
		}
	}
	return Result.ret;
} // MatrixMult_SIMDIntrinsics



/////////////////////////////////////////////////////////////////////////////

void MatVec_x87 (Vertex A, Vertex *B)
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


inline void MatVec_Int (float *A /*[4] */ ,  float * B /*[4] */)
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

		


void KatmaiTransformScreen(const float *x,
						   const float *y,
						   const float *z,
						   int n,
						   float m[4][4],
						   float w,float h,
						   D3DTLVERTEX *vtlp)
{
//	n = (n+3)/4;

	for(int i=0; i < n; i++) {
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

	for(int i=0; i < n; i++,p++) {
	
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
			   float w,float h,
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
	
	F32vec4 VK_X = w;
	F32vec4 VK_Y = h;

	// F32vec4 One   = (1., 1., 1., 1.);
	
	//F32vec4 sum4  = (0., 0., 0., 0.);

	F32vec4 curX, curY, curZ, curW; 
	F32vec4 Xout,       Yout,       Zout;	
	
	n = (n+3) >> 2;

	
    for (int i=0; i<n; i++) 
	{
			_mm_prefetch((char *)(&x[2]), _MM_HINT_NTA); 
		    _mm_prefetch((char *)(&y[2]), _MM_HINT_NTA); 
		    _mm_prefetch((char *)(&z[2]), _MM_HINT_NTA);

			curX = *x++; 
			curY = *y++; 
			curZ = *z++;

			// use rcp_nr : to do test & profile
			curW = rcp_nr(((curX*mat30)+(curY*mat31)+(curZ*mat32)+mat33));
			// curW = (One/((curX*mat30)+(curY*mat31)+(curZ*mat32)+mat33));
	
			Xout=((((curX*mat00)+(curY*mat01)+(curZ*mat02)+mat03)*curW));
			Xout = VK_X+ Xout * VK_X;

			Yout=((((curX*mat10)+(curY*mat11)+(curZ*mat12)+mat13)*curW));
			Yout = VK_Y - Yout * VK_Y;

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
void KatmaiTransformScreen_Int(
			   int n, float  *vec_,
			   float m[4][4],
			   float w,float h,
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
	
	F32vec4 VK_X = w;
	F32vec4 VK_Y = h;

	// F32vec4 One   = (1., 1., 1., 1.);
	
	//F32vec4 sum4  = (0., 0., 0., 0.);

	F32vec4 curX, curY, curZ, curW; 
	F32vec4 Xout,       Yout,       Zout;	
	
	n = (n+3) >> 2;

	
    for (int i=0; i<n; i++) 
	{

			curX = *vec++; 
			curY = *vec++; 
			curZ = *vec++;

			// use rcp_nr : to do test & profile
			curW = rcp_nr(((curX*mat30)+(curY*mat31)+(curZ*mat32)+mat33));
			// curW = (One/((curX*mat30)+(curY*mat31)+(curZ*mat32)+mat33));

			_mm_prefetch((char *)(&vec[3]), _MM_HINT_NTA); 
	
			Xout=((((curX*mat00)+(curY*mat01)+(curZ*mat02)+mat03)*curW));
			Xout = VK_X+ Xout * VK_X;

			Yout=((((curX*mat10)+(curY*mat11)+(curZ*mat12)+mat13)*curW));
			Yout = VK_Y - Yout * VK_Y;

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



#define GETXYZ()	curX = *x++;curY = *y++; curZ = *z++
#define GETNXNYNZ()	curNX = *nx++;curNY = *ny++; curNZ = *nz++

#define SETUPXYZ() \
	__m128 *x =(__m128 *) x_; \
	__m128 *y =(__m128 *) y_; \
	__m128 *z =(__m128 *) z_;

#define SETUPNXNYNZ() \
	__m128 *nx =(__m128 *) nx_; \
	__m128 *ny =(__m128 *) ny_; \
	__m128 *nz =(__m128 *) nz_;


#define SETUPMAT() \
	F32vec4 mat30 = m[0][3], mat31 = m[1][3]; F32vec4 mat32 = m[2][3], mat33 = m[3][3]; \
	F32vec4 mat00 = m[0][0], mat01 = m[1][0]; F32vec4 mat02 = m[2][0], mat03 = m[3][0]; \
	F32vec4 mat10 = m[0][1], mat11 = m[1][1]; F32vec4 mat12 = m[2][1], mat13 = m[3][1]; \
	F32vec4 mat20 = m[0][2], mat21 = m[1][2]; F32vec4 mat22 = m[2][2], mat23 = m[3][2]; \
	F32vec4 VK_X = w; 	F32vec4 VK_Y = h 



#define TRANSFORMXYZ() \
			curW = rcp_nr(((curX*mat30)+(curY*mat31)+(curZ*mat32)+mat33)); \
			Xout=((((curX*mat00)+(curY*mat01)+(curZ*mat02)+mat03)*curW)); \
			Xout = VK_X+ Xout * VK_X; \
			Yout=((((curX*mat10)+(curY*mat11)+(curZ*mat12)+mat13)*curW)); \
			Yout = VK_Y - Yout * VK_Y; \
			Zout=((((curX*mat20)+(curY*mat21)+(curZ*mat22)+mat23)*curW)); \

#define UNPACKXYZ() \
			vtlp->sx = Xout[0],vtlp->sy = Yout[0],vtlp->sz = Zout[0],vtlp->rhw = curW[0]; \
			vtlp ++; \
			vtlp->sx = Xout[1],vtlp->sy = Yout[1],vtlp->sz = Zout[1],vtlp->rhw = curW[1]; \
			vtlp ++; \
			vtlp->sx = Xout[2],vtlp->sy = Yout[2],vtlp->sz = Zout[2],vtlp->rhw = curW[2]; \
			vtlp ++; \
			vtlp->sx = Xout[3],vtlp->sy = Yout[3],vtlp->sz = Zout[3],vtlp->rhw = curW[3]; \
			vtlp ++; 


#define UNPACKXYZRGB() \
			vtlp->sx = Xout[0],vtlp->sy = Yout[0],vtlp->sz = Zout[0],vtlp->rhw = curW[0]; \
			vtlp->color= RGBA_MAKE((unsigned int)color.x[0],(unsigned int)color.y[0],(unsigned int)color.z[0],state.alpha); vtlp->specular=0; \
			vtlp ++; \
			vtlp->sx = Xout[1],vtlp->sy = Yout[1],vtlp->sz = Zout[1],vtlp->rhw = curW[1]; \
			vtlp->color= RGBA_MAKE((unsigned int)color.x[1],(unsigned int)color.y[1],(unsigned int)color.z[1],state.alpha); vtlp->specular=0; \
			vtlp ++; \
			vtlp->sx = Xout[2],vtlp->sy = Yout[2],vtlp->sz = Zout[2],vtlp->rhw = curW[2]; \
			vtlp->color= RGBA_MAKE((unsigned int)color.x[2],(unsigned int)color.y[2],(unsigned int)color.z[2],state.alpha); vtlp->specular=0;\
			vtlp ++; \
			vtlp->sx = Xout[3],vtlp->sy = Yout[3],vtlp->sz = Zout[3],vtlp->rhw = curW[3]; \
			vtlp->color= RGBA_MAKE((unsigned int)color.x[3],(unsigned int)color.y[3],(unsigned int)color.z[3],state.alpha); vtlp->specular=0; \
			vtlp ++; 

#define UNPACKXYZRGBSPEC() \
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



// F32vec4ToInt
// Is32vec2 F32vec4ToIs32vec2 (const F32vec4 &a)

// UnpackRgb
// _mm_cvtt_ps2pi(a);


// SOA, 4 points a time 
void KatmaiTransformScreen_(
			   int n, float  *x_, float *y_, float *z_,
			   float m[4][4],
			   float w,float h,
			   D3DTLVERTEX *vtlp
			)
{

	SETUPMAT();
	
	SETUPXYZ();


	n = (n+3) >> 2;

	F32vec4 curX, curY,curZ,curW; 
	F32vec4 Xout,Yout, Zout;	

	
    for (int i=0; i<n; i++) 
	{

			GETXYZ();

			TRANSFORMXYZ();

			UNPACKXYZ();
	}
		
}		

// diffuse lighting with 1 directional light

#define DOLIGHT() KLightDiffuse(&state.lights[0],curNX,curNY,curNZ,color); \
					color.x += state.lights[0].ambient.x,color.y += state.lights[0].ambient.y,color.z += state.lights[0].ambient.z;


void KatmaiTransformLight_1_dir(
			   int n, 
			   
			   float  *x_, float *y_, float *z_,
			   float  *nx_, float *ny_, float *nz_,
			   
			   float m[4][4],

			   float w,float h,

			  

			   D3DTLVERTEX *vtlp
			)
{

	SETUPMAT();
	
	SETUPXYZ();

	SETUPNXNYNZ();


	n = (n+3) >> 2;

	F32vec4 curX, curY,curZ,curW; 
	F32vec4 Xout,Yout, Zout;	

	F32vec4 curNX, curNY,curNZ; 
	
	KVec3 color;

	
    for (int i=0; i<n; i++) 
	{

			GETXYZ();

			TRANSFORMXYZ();

			GETNXNYNZ();

			color.x = state.emissive.x, color.y = state.emissive.y, color.z = state.emissive.z;

			DOLIGHT();

			// clamp
			color.x = simd_min(color.x,twofivefive);
			color.y = simd_min(color.y,twofivefive);
			color.z = simd_min(color.z,twofivefive);



			UNPACKXYZRGB();
	}
		
}		

#undef DOLIGHT()


// 1 dir light with specular
#define DOLIGHT() KLightDiffuseSpecular(&state.lights[0],curNX,curNY,curNZ,toViewerx,toViewery,toViewerz,state.power,color,specularColor); \
					color.x += state.lights[0].ambient.x,color.y += state.lights[0].ambient.y,color.z += state.lights[0].ambient.z;


void KatmaiTransformLight_1_dir_spec(
			   int n, 
			   
			   float  *x_, float *y_, float *z_,
			   float  *nx_, float *ny_, float *nz_,
			   
			   float m[4][4],

			   float w,float h,

			  

			   D3DTLVERTEX *vtlp
			)
{

	SETUPMAT();
	
	SETUPXYZ();

	SETUPNXNYNZ();


	n = (n+3) >> 2;

	F32vec4 curX, curY,curZ,curW; 
	F32vec4 Xout,Yout, Zout;	

	F32vec4 curNX, curNY,curNZ; 
	
	KVec3 color;
	KVec3 specularColor;

	
    for (int i=0; i<n; i++) 
	{

			GETXYZ();

			TRANSFORMXYZ();

			GETNXNYNZ();

			color.x = state.emissive.x, color.y = state.emissive.y, color.z = state.emissive.z;
			specularColor.x=specularColor.y=specularColor.z = F32vec4(0.0f);

			F32vec4 toViewerx=state.viewer.x-curX; 
			F32vec4 toViewery=state.viewer.y-curY; 
			F32vec4 toViewerz=state.viewer.z-curZ;


			// normalize (toViewerz,toViewery,toViewerz) ***
			F32vec4 f = toViewerx*toViewerx;
			f += toViewery*toViewery;
			f += toViewerz*toViewerz;
			f = rsqrt_nr(f);
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
		
}		




#endif



void KatmaiMatrixMul(const float a[4][4],const float b[4][4],float result[4][4]);
void KatmaiMatrixTranspose(const float a[4][4],float result[4][4]);
void KatmaiMatrixInvert(const float a[4][4],float result[4][4]);

// Point : Len() len2() .Normalize()
// void KatmaiNormalize(float v[4],float result[4]) = l = (sqrt(


void KatmaiTransformTextureCoord(const float *x,
						   const float *y,
						   int n,
						   float m[4][4],
						   D3DTLVERTEX *vtlp)
{

	for(int i=0; i < n; i++) {
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

	for(int i=0; i < n; i++) {
			vtlp->tu = *x++;
			vtlp->tv=  *y++;
			vtlp ++;

	}

}
