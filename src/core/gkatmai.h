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

#ifndef _GKATMAIxx_h
#define _GKATMAIxx_h


extern BOOL isKatmaiProcessor; // FLAG do we run on Katmai
extern BOOL useKatmai; // FLAG use Katmai codepath 

// enable / disable Katmai Code 
extern BOOL KatmaiEnable(BOOL flag); 


// function variables
// initialized at runtime 
// ret = a * b
// ret can overlap a or b 
// having problems with name mangling with Intel compiler
extern void  (*KMatrixMult)( /*const*/ float a[4][4], /*const*/ float b[4][4], float ret[4][4]);
extern void (*KMatrixVec) (/*const*/ float m[4][4],/*const*/ Point &A, Point &B);
extern void (*KMatrixVecN) (/*const*/ float m[4][4],int n, /*const*/ Point* A, Point *B);



// from crt/heap.h 

/*
 * Unit of allocation. All allocations are of size n * _GRANULARITY. Note
 * that _GRANULARITY must be a power of 2, or it cannot be used with the
 * _ROUND2 macro.
 */

#if defined (_M_MRX000) || defined (_M_ALPHA) || defined (_M_PPC) || defined (_M_MPPC)
#define _GRANULARITY    8
#else  /* defined (_M_MRX000) || defined (_M_ALPHA) || defined (_M_PPC) || defined (_M_MPPC) */
#define _GRANULARITY    4
#endif  /* defined (_M_MRX000) || defined (_M_ALPHA) || defined (_M_PPC) || defined (_M_MPPC) */

#define _BLOCKSIZE 4
#define _BLOCKSIZE_BYTES (_BLOCKSIZE *sizeof(float))
#define _EXTRA_ALLOC_BYTES (_BLOCKSIZE_BYTES - _GRANULARITY)
#define _EXTRA_ALLOC (_EXTRA_ALLOC_BYTES /  _BLOCKSIZE)

// ALIGN float * to 16 byte boundary 
#define PALIGN(P) \
	(float *) _ROUND2((DWORD) (p),_BLOCKSIZE_BYTES )



void *  calloc16(size_t, size_t);
void    free16(void *);
void *  malloc16(size_t);
void *  malloc16_ofs(size_t,size_t);
void *  realloc16(void *, size_t);

// for scaling colors to RGBA_MAKE
#define KRGB_SCALE 255.0f 


#define KMAX_LIGHT 64

enum KLightType {
	KDIFFUSE_LIGHT = 0,
	KPOINT_LIGHT = 1,
	KSPOT_LIGHT = 2,

	KFOG_LINEAR = 3, // for now

	KSPECULAR_MASK = 4,		// has specular ored in 
	KATTENUATION_MASK = 8,	// attenuation mask 
	KRANGE_MASK = 16,		// range mask

};

// Initiallize any Katmai state and return true if Katmai support
extern BOOL KatmaiInit();
extern void KatmaiTerm();



// matrix * matrix
extern void KatmaiMatrixMult_x87( /*const*/ float A[4][4], /*const*/ float B[4][4], float ret[4][4]);
extern void KatmaiMatrixMult_Int( /*const*/ float a[4][4], /*const*/ float b[4][4], float ret[4][4] );

// matrix * vector3 
extern void KatmaiMatVec_int (/*const*/ float m[4][4], /*const*/ Point &A, Point &B);
extern void KatmaiMatVecN_int (/*const*/ float m[4][4],int n, /*const*/ Point *A, Point *B);


#ifdef _D3D
extern void MatrixMult_SIMDasm( const D3DMATRIX &a, const D3DMATRIX &b, D3DMATRIX &ret);



extern void KatmaiTransformScreen(const float *x,
						   const float *y,
						   const float *z,
						   int n,
						   float m[4][4],
						   float sx,float tx, float sy,float ty, // 2D viewpoint transform
						   D3DTLVERTEX *vtlp);


extern void KatmaiTransformTextureCoord(
						   const float *x,
						   const float *y,
						   int n,
						   float m[4][4],
						   D3DTLVERTEX *vtlp);

extern void KatmaiTransformScreen_Int(int n,Point *p,
						   float m[4][4],
						   float w,float h,
						   D3DTLVERTEX *vtlp);

// SOA, 4 points a time 
extern void KatmaiTransformScreen_Int(
			   int n, float  *x_, float *y_, float *z_,
			   float m[4][4],
			   float sx,float tx, float sy,float ty, // 2D viewpoint transform
			   D3DTLVERTEX *vtlp
			);

// SOS
extern void KatmaiTransformScreen_Int(
			   int n, float  *vx_,
			   float m[4][4],
			   float sx,float tx, float sy,float ty, // 2D viewpoint transform
			   D3DTLVERTEX *vtlp
			);

extern void KatmaiTransformScreen_(
			   int n, float  *vx_,
			   float m[4][4],
			   float sx,float tx, float sy,float ty, // 2D viewpoint transform
			   D3DTLVERTEX *vtlp
			);

// set number of used lights 
extern void KSetNumLights(int num);

// set light type 
extern void KSetLightType(int i,int type);


// set light direction of vector
extern void KSetLightDirection(int i,Point dir);

// set light position for point / spot lights
extern void KSetLightPosition(int i,Point pos);


// set light color (multiplied with material color 
extern void KSetLightColors(int i,
				 D3DCOLORVALUE ambient,D3DCOLORVALUE diffuse,D3DCOLORVALUE specular,BOOL doSpecular);

// set light attenuation for point / spot lights
extern void KSetLightAttenuation(int i,BOOL doAtten=FALSE,float a0=1.0f,float a1=0.0f,float a2=0.0f);

// set sotlight cone values 
extern void KSetLightSpotCone(int i, float cosPhi,float cosTheta);

// set light radius
extern void KSetLightRadius(int i, float radius=0.0f);


// set  material emissive color , alpha & power
extern void KSetBasicMaterial(D3DCOLORVALUE emissive,float alpha,float power);

// set Linear Fog 
extern void KSetFogLinear(D3DCOLORVALUE color,float start,float end);

// set Linear Fog off
extern void KSetFogOff();

// set position of viewer in local coordinates
extern void KSetViewerPosition(Point position);

// for per vertex color lighting we need the material values
extern void KSetMaterial(D3DCOLORVALUE ambient,D3DCOLORVALUE diffuse,D3DCOLORVALUE specular,BOOL doSpecular);




// 1 dir light 
extern void KatmaiTransformLight_1_dir(
			   int n, 
			   float  *v_,
			   float  *vn_,
			   float m[4][4],
			   float sx,float tx, float sy,float ty, // 2D viewpoint transform
			   D3DTLVERTEX *vtlp
			);


// 1 dir light with specular

extern void KatmaiTransformLight_1_dir_spec(
			   int n, 
			   float  *v_,
			   float  *vn_,
			   float m[4][4],
			   float sx,float tx, float sy,float ty, // 2D viewpoint transform
	  		   D3DTLVERTEX *vtlp
			);
// 1 dir light 
extern void KatmaiTransformLight_1_dir_twosided(
			   int n, 
			   float  *v_,
			   float  *vn_,
			   float m[4][4],
			   float sx,float tx, float sy,float ty, // 2D viewpoint transform
			   D3DTLVERTEX *vtlp
			);


// 1 dir light with specular

extern void KatmaiTransformLight_1_dir_spec_twosided(
			   int n, 
			   float  *v_,
			   float  *vn_,
			   float m[4][4],
			   float sx,float tx, float sy,float ty, // 2D viewpoint transform
	  		   D3DTLVERTEX *vtlp
			);

// generic 


extern void KatmaiTransformLight_generic_spec(
			   int n, 
			   float  *v_,
			   float  *vn_,
			   float m[4][4],
			   float sx,float tx, float sy,float ty, // 2D viewpoint transform
			   D3DTLVERTEX *vtlp
			);

extern void KatmaiTransformLight_generic_spec_twosided(
			   int n, 
			   float  *v_,
			   float  *vn_,
			   float m[4][4],
			   float sx,float tx, float sy,float ty, // 2D viewpoint transform
			   D3DTLVERTEX *vtlp
			);

extern int KatmaiBuildFaceList(const int *fp,const int *fpend, 
						float *fn_,float *fv_,
						WORD *fpout);

// build face list with marking of used vertices 
extern int KatmaiBuildFaceListMark(const int *fp_,const int *fpend_, 
						float *fn_,float *fv_,
						int *vertexMark,
						WORD *fpout);

class KGVertexI {
public :
		//@cmember the vertex coordinate index
		int v;	
		//@cmember the vertex normal index
		int n;	
		//@cmember the vertex color index
		int c;	
		//@cmember the vertex parameter index
		int p;	

		//@cmember Zero out values to -1
		void Zero() { v=n=c=p=-1; }

		//@cmember compare for equality
		int operator== ( const KGVertexI &b) 
			{
				return(v == b.v && n == b.n && c == b.c && p == b.p); 
			}

};


extern void KatmaiComputeVertexNormals(const int *f,int flength, // face list
								const KGVertexI *verts,
								const Point *fn, // face normals
								int vnLength,
								Point *vn // computed vertex normals	
								);




#endif


#endif

