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
#ifndef _GLRENDER_H
#define _GLRENDER_H
/******************************************************************************
@doc

@module GLRender.h - OpenGL RenderState classes |

Copyright (c) 1995-1999	by Holger Grahn
All rights reserved

Purpose:
OpenGL Rendering depends on many different glStates. All major OpenGL attributes
are encapsulated in a class. This allows easyier state changing, easier state checking,
less OpenGL state change calls.
Render State is the major class, other classes are maintaining the state of a light source or the material setting.


Classes:
<c glCache>
<c glMatrix>
<c RGBAColor>
<c glMaterial>
<c glTextureParameters>
<c glTextureGeneration>
<c glFog>
<c glLight>
<c glBackground>
<c GExtendedDrawStyle>
<c renderState>

Notes:

Changes:

Todo :

******************************************************************************/

#ifdef _OGL

#if defined(WIN32) && !defined (MESA)
	#define _WGL

#endif




// Mesa 3.2 defines this 
#if !defined(GLCALLBACK)
#define GLCALLBACK CALLBACK
#endif

#if !defined(APIENTRY)
#define APIENTRY
#endif

// typedefs of extensions
#include "glext.h"

// gl texture object
// use OGL 1.1 texture object 
//#define _GL_TOBJ

// callbacks for "GL_EXT_texture_object"
typedef	GLboolean (APIENTRY *glAreTexturesResident_t) (GLsizei n, const GLuint *textures, GLboolean *residences);
typedef	void (APIENTRY *glBindTexture_t) (GLenum target, GLuint texture);
typedef	void (APIENTRY *glGenTextures_t) (GLsizei n, GLuint *textures);
typedef	void (APIENTRY *glDeleteTextures_t) (GLsizei n, const GLuint *textures);
typedef	GLboolean (APIENTRY *glIsTexture_t) (GLuint texture);
typedef	void (APIENTRY *glPrioritizeTextures_t) (GLsizei n, const GLuint *textures, const GLclampf *priorities);


typedef void (APIENTRY *glLockArrays_t) (GLint first, GLsizei count);
typedef void (APIENTRY* glUnlockArrays_t)(void);

typedef void (APIENTRY * PFNGLARRAYELEMENTARRAYEXTPROC)(GLenum mode, GLsizei count, const GLvoid* pi);



#if !defined(_WGL)
// types defined in WIN32 OGL

/* EXT_vertex_array */
typedef void (APIENTRY * PFNGLARRAYELEMENTEXTPROC) (GLint i);
typedef void (APIENTRY * PFNGLDRAWARRAYSEXTPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRY * PFNGLVERTEXPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
typedef void (APIENTRY * PFNGLNORMALPOINTEREXTPROC) (GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
typedef void (APIENTRY * PFNGLCOLORPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
typedef void (APIENTRY * PFNGLINDEXPOINTEREXTPROC) (GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
typedef void (APIENTRY * PFNGLTEXCOORDPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
typedef void (APIENTRY * PFNGLEDGEFLAGPOINTEREXTPROC) (GLsizei stride, GLsizei count, const GLboolean *pointer);
typedef void (APIENTRY * PFNGLGETPOINTERVEXTPROC) (GLenum pname, GLvoid* *params);
typedef void (APIENTRY * PFNGLARRAYELEMENTARRAYEXTPROC)(GLenum mode, GLsizei count, const GLvoid* pi);

/* WIN_swap_hint */
typedef void (APIENTRY * PFNGLADDSWAPHINTRECTWINPROC)  (GLint x, GLint y, GLsizei width, GLsizei height);

/* EXT_paletted_texture */
typedef void (APIENTRY * PFNGLCOLORTABLEEXTPROC)
    (GLenum target, GLenum internalFormat, GLsizei width, GLenum format,
     GLenum type, const GLvoid *data);
typedef void (APIENTRY * PFNGLCOLORSUBTABLEEXTPROC)
    (GLenum target, GLsizei start, GLsizei count, GLenum format,
     GLenum type, const GLvoid *data);
typedef void (APIENTRY * PFNGLGETCOLORTABLEEXTPROC)
    (GLenum target, GLenum format, GLenum type, GLvoid *data);
typedef void (APIENTRY * PFNGLGETCOLORTABLEPARAMETERIVEXTPROC)
    (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY * PFNGLGETCOLORTABLEPARAMETERFVEXTPROC)
    (GLenum target, GLenum pname, GLfloat *params);


/*
 // MESA ????
    void gluTessNormal(
        GLUtesselator       *tess,
        GLdouble            x,
        GLdouble            y,
        GLdouble            z );

#define GLU_TESS_COMBINE    1000
*/

#endif



#include "image.h"

#include "grenderbase.h"


// Forward
class GCamera;
class RenderState;



#ifdef _DEBUG
// Check gl error code of last operation 
#define    glCheckError() glCheckError_(__FILE__,__LINE__)
#else
#define    glCheckError() 0
#endif


int glCheckError_(const char *file="", int line=0);


inline void	glScale(const Point &scale) { glScalef(scale.x,scale.y,scale.z); };

inline void glRotate(const Point &rotate)
		{ glRotatef(rotate.x,1.0,0.0,0.0);
		  glRotatef(rotate.y,0.0,1.0,0.0);
		  glRotatef(rotate.z,0.0,0.0,1.0);
		};

inline void glTranslate(const Point &translate) { glTranslatef(translate.x,translate.y,translate.z); }

inline void glSet(const Transform3 &m) 
{
	glScale(m.scale);
	glRotate(m.rotate);
	glTranslate(m.translate);
}

/*!

  Wrapper for glCalllist, Record glCalls for playback

*/

class glCache {
public:
 //! the OpenGL calllist id, 0 means invalid
 GLuint id;

 //! constructore
 glCache(int Id=0) { id=Id; }

 //! set the callist id 
 void Set(int Id=0) {id =Id; }
 
 //! true if callist id != 0
 int IsValid() { return(id != 0); }
 
 //! true if callist id == 0
 int Invalid() { return(id == 0); }

 //! Create a new call list
 int New() { 	if (IsValid()) Delete();
 			id=glGenLists(1);
 			return(id);	
 		}

 //! open call list, (most) following GL calls will be stored in list
 void Begin(GLenum mode = GL_COMPILE) { 
 			if (!IsValid()) New();
 			glNewList(id,mode); 
 		}
 
 //!	 close call list (valid only after Begin()
 void End() { glEndList(); }
 
  //! execute a call list
  void Call() { if( IsValid() ) glCallList(id); }	
 
 //! delete call list
 void Delete() { if( IsValid() ) glDeleteLists(id,1);  id = 0; }
 
 //! invalidate call list, i.e. after scene has changed
 void Invalidate() {Delete(); }

 //! destructor, will call delete
  ~glCache() { Delete(); }
};

//
// @class a OpenGL Matrix
//
class glMatrix {
public :
	
	//! OpenGL matrix representation
	GLfloat m[16];
	//! flag if current matrix is identity matrix
	int identity;
	
	//! constructor, set identity flag
	glMatrix() { identity = 1; }

	//!  Access matrix vector 
	operator GLfloat* () { return &m[0]; }
	
	//!  Access matrix vector element
	GLfloat&  operator[] (int element) { return(m[element]);}
	
	//!  Set identity flag
	void SetIdentity() { identity = 1; }

	//!  Clear identity flag
	void ClearIdentity() { identity = 0; }
	
	//! assign glMatrix by a <c Matrix> object
	glMatrix& Set(const Matrix &mat) {
		 for (int i=0; i<4;i++) {
		 for (int j=0; j<4;j++) {
			M(i,j) = mat.x[i][j];
//			M(j,i) = mat.x[j][i];
		 }
		 }
		 identity=0;
		 return(*this);
	}
	//! get glMatrix as a <c Matrix> object
	Matrix& Get(Matrix &mat) {
		 for (int i=0; i<4;i++) {
		 for (int j=0; j<4;j++) {
//			mat.x[j][i]= M(j,i) ;
			mat.x[i][j]= M(i,j) ;
		 }
		 }
		 return(mat);
	}
	
	//!  Access matrix element
//	GLfloat & M(int row,int col) { return(m[col*4+row]); }
	GLfloat & M(int row,int col) { return(m[row*4+col]); }
  /// ????????????#define M(row,col)  m[col*4+row]



	//! Set gl Matrix (glLoadMatrix / glLoadIdentity
	void glSet() {
		if (identity) glLoadIdentity();
		else glLoadMatrixf(m);
	}

	//! multiply gl Matrix (glMultMatrix)
	void glMult() {
		if (identity) ;
		else glMultMatrixf(m);
	}

	//! get glMatrix back from GL state
	void glGet(GLenum pname = GL_MODELVIEW_MATRIX) {
		glGetFloatv(pname,m);
		identity=0;
	}

	//! debug only, trace matrix contents
	void Trace() {
		TRACE("glMatrix identity=%d (%g %g %g %g) (%g %g %g %g) (%g %g %g %g) (%g %g %g %g)\n",
		identity,m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8],m[9],m[10],m[11],m[12],m[13],m[14],m[15]);
	}


	//!  get current gl matrix as newly allocated glMatrix object
	glMatrix* Save() {
		glMatrix *m= new glMatrix();
		m->glGet();
		return(m);
	}
	
	void Restore() {
		glSet();
	}
};


#if 0

//@class GL-Style Red Green Blue Alpha color
class RGBAColor {
public:
	//! the color components (range 0..1)
	float r,g,b,a;

	RGBAColor(float R=0.0,float G=0.0, float B=0.0,float A=1.0) { r=R,g=G,b=B,a=A; };

	RGBAColor(const Point &p,float A=1.0) { r=p.x,g=p.y,b=p.z,a=A; };

	RGBAColor& Set(float R,float G, float B,float A=1.0) { r=R,g=G,b=B,a=A; return(*this); };

	RGBAColor& Set(const Point &p,float A=1.0) { r=p.x,g=p.y,b=p.z,a=A; return(*this); };
	RGBAColor& Get(Point &p) { p.Set(r,g,b); return(*this); };
	RGBAColor& SetAlpha(float A=1.0) { a=A; return(*this); };

	RGBAColor& White() { r=g=b=1.0; a=1.0; return (*this); };
	RGBAColor& Black() { r=g=b=0.0; a=1.0; return (*this); };
	RGBAColor& Gray(float gray) { r=g=b=gray; a=1.0; return (*this);};

	//! set GL Color
	void glColor() const { glColor4fv(&r); } ;

	//! set GL Color (glColor)
	void glSet() const { glColor4fv(&r); };
	
	operator GLfloat* () {return &r;};		// type convert to GLfloat * (GLfloat[4]) 

	// input/ouput
	int io(AStream &a,int fmt=0);

};
#endif

// set RGBA Color as current glColor

inline void glColor(const RGBAColor &color)  { glColor4fv(&color.r); }


//@class GL-Style Material
class glMaterial {
public:
	RGBAColor ambient;
	RGBAColor diffuse;
	RGBAColor specular;
	RGBAColor emission;

	float shininess;
	float alpha;

    RGBAColor& Ambient() { return ambient; }
    RGBAColor& Diffuse() { return diffuse; }
    RGBAColor& Specular() { return specular; }
    RGBAColor& Emissive() { return emission; }
    RGBAColor& Emission() { return emission; }


	glMaterial & Get(Point &Ambient, Point &Diffuse, Point &Specular,Point &Emission,float &Shininess,float &Alpha) {
		 ambient.Get(Ambient);
		 diffuse.Get(Diffuse);
		 specular.Get(Specular);
		 emission.Get(Emission);
		 Shininess = shininess;
		 Alpha = alpha;
		 return(*this);
		};

	// set material component wise as "Points "
	glMaterial & Set(const Point &Ambient, const Point &Diffuse,
				     const Point &Specular,const Point &Emission,
				     float Shininess,float Alpha) {
		 ambient.Set(Ambient);
		 diffuse.Set(Diffuse);
		 specular.Set(Specular);
		 emission.Set(Emission);
		 shininess = Shininess;
		 alpha = Alpha;
		 return(*this);
		};

	glMaterial & glSet(GLenum face = GL_FRONT_AND_BACK) {
			glMaterialfv(face,GL_AMBIENT,&ambient.r);
			glMaterialfv(face,GL_DIFFUSE,&diffuse.r);
			glMaterialfv(face,GL_SPECULAR,&specular.r);
			glMaterialfv(face,GL_EMISSION,&emission.r);
			glMaterialfv(face,GL_SHININESS,&shininess);
			return(*this);
	};

	glMaterial & Default() {
		ambient.Gray(0.2);
		diffuse.Gray(0.8);
		specular.Black();
		emission.Black();
		shininess=0.0;
		alpha = 1.0;
		return(*this);
   };

   glMaterial & SetAlpha(float a) {
		ambient.SetAlpha(a);
		diffuse.SetAlpha(a);
		specular.SetAlpha(a);
		emission.SetAlpha(a);
		alpha = a;
		return(*this);
   };
   
   float GetAlpha() {
		return (alpha);
   }

   int io(AStream &a);
   int ReadFromInventorFile(const char *FileName);

};

extern const char * GFilterLookup(int v);
extern int GFilterLookup(const char *v);

//@class GL-Texture mapping options

class glTextureParameters {
public :
	GLenum min_filter;
	GLenum mag_filter;
	GLenum wrap_s;
	GLenum wrap_t;
	gbool set_diffuse_white;
	int max_size_x,max_size_y;
	int current_max_size_x,current_max_size_y; // current allowed texture size , idea scale down textures if few texture memory
	int min_size_x,min_size_y; //  min allowed minimum texture size , idea scale down textures if few texture memory

	gbool square_size;			// true, only square texture size allowed
	gbool border;				// can do border 



	RGBAColor border_color;


    GLenum blend;			// blending function
    GLenum defaultBlend;	// default blend function
    GLenum unlitBlend;		// blend function for unlit textures 


	gbool gen_mipmaps;
	int supported_image_fmts;		// supported image formts for texture images ( image.h)

    static const IS filters[]; // table of filter names 
    static const IS blends[];  // table of texture blend names 

	void SetWrap(gbool s,gbool t) {
	      wrap_s = s ?  GL_REPEAT : GL_CLAMP;
	      wrap_t = t ?  GL_REPEAT : GL_CLAMP;
	} ;

	void GetWrap(gbool &s,gbool &t) {
	      s = wrap_s ==  GL_REPEAT;
	      t = wrap_t ==  GL_REPEAT;
	};

	void SetMinFilter(const char *filter) {
		min_filter = (GLenum) Lookup(filters,filter);
	};

	void SetMagFilter(const char *filter) {
		mag_filter = (GLenum) Lookup(filters,filter);
	};

	const char * GetMinFilter() { return Lookup(filters,min_filter); }
	const char * GetMagFilter() { return Lookup(filters,mag_filter); }

	void SetBlend(const char *filter) {
        int v = Lookup(blends,filter);
        if (v>=0) blend = defaultBlend = (GLenum) v;
	};
	
	const char *GetBlend(){ return Lookup(blends,blend); }

	void SetUnlitBlend(const char *filter) {
        int v = Lookup(blends,filter);
        if (v>=0) unlitBlend = (GLenum)v;
	};

	const char * GetUnlitBlend(){ return Lookup(blends,unlitBlend); }

	GLenum GetMaxBlend() { return GL_REPLACE; }


	glTextureParameters();

	// Are mipmap definitions required
	gbool NeedMipmaps() { return( (min_filter > GL_LINEAR ) || (gen_mipmaps)); }
	gbool IsMipmaped() { return min_filter >= GL_NEAREST_MIPMAP_NEAREST; }

	void glSet(GLenum target = GL_TEXTURE_2D) {
		glTexParameteri(target,GL_TEXTURE_MIN_FILTER,min_filter);
		glTexParameteri(target,GL_TEXTURE_MAG_FILTER,mag_filter);
		glTexParameteri(target,GL_TEXTURE_WRAP_S,wrap_s);
		glTexParameteri(target,GL_TEXTURE_WRAP_T,wrap_t);
		glTexParameterfv(target,GL_TEXTURE_BORDER_COLOR,&border_color.r);
       };

	//! set the maximum texture size
	int SetMaxSize(int size_x,int size_y);

	//! get max texture size
	void GetMaxSize(int &size_x,int &size_y);

	//! set the current maximal texture size
	int SetCurrentMaxSize(int size_x,int size_y);

	//! get current max texture size
	void GetCurrentMaxSize(int &size_x,int &size_y);

	//! set min texture size
	int SetMinSize(int size_x,int size_y);

	//! get min texture size
	void GetMinSize(int &size_x,int &size_y);


};


//@class GL Texture parameters generation (glTexGen)
class glTextureGeneration
{
enum TextureCoordinate { S,T,R,Q };

public :
   int enable[4];	// glEnable[GL_S .. GL_Q]
   int pname[4];	// GL_TEXTURE_GEN_MODE : GL_OBJECT_LINE,GL_EYE_LINEAR,GL_SPHERE_MAP
   GLenum ptype[4];	// plane type :: GL_OBJECT_PLANE GL_EYE_PLANE
   float params[4][4];

   char parameter[60];
   gbool isOff;		// enable[*] == 0

   glTextureGeneration() {
		for (int i=S; i<=Q; i++) { enable[i]=0; pname[i]=GL_OBJECT_PLANE; ptype[i]=GL_OBJECT_PLANE;	}
		parameter[0]=0;
		isOff=1;

   };


   void glDisable_() {
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
		glDisable(GL_TEXTURE_GEN_Q);
   };

   void glEnable_() {
		if (isOff) return;
		if (enable[S]) glEnable(GL_TEXTURE_GEN_S); else 	glDisable(GL_TEXTURE_GEN_S);
		if (enable[T]) glEnable(GL_TEXTURE_GEN_T); else 	glDisable(GL_TEXTURE_GEN_T);
		if (enable[R]) glEnable(GL_TEXTURE_GEN_R); else 	glDisable(GL_TEXTURE_GEN_R);
		if (enable[Q]) glEnable(GL_TEXTURE_GEN_Q); else 	glDisable(GL_TEXTURE_GEN_Q);
   };

   void glSet();

   void SetSphere() {
			enable[S]=enable[T]=1;
			enable[Q]=enable[R]=0;
			pname[S]=pname[Q]=GL_SPHERE_MAP;
			isOff=0;
   };

   void SetPlane(int eye,int eyePlane,int s=0,int t=1,int r=2) {
		int i,j;
		for (i=S; i<=Q; i++) for(j=0; j<4;j++)
		   params[i][j]=0;
		if (s >=0)
		{  enable[S]=1; params[S][s]=1.0;
		   if (eye) pname[S]=GL_EYE_LINEAR;else pname[S]=GL_OBJECT_LINEAR;
		   if (eyePlane) ptype[S]=GL_EYE_PLANE;else ptype[S]=GL_OBJECT_PLANE;
		}
		else enable[S]=0;
		if (t >=0)
		{  enable[T]=1; params[T][t]=1.0;
		   if (eye) pname[T]=GL_EYE_LINEAR;else pname[T]=GL_OBJECT_LINEAR;
		   if (eyePlane) ptype[T]=GL_EYE_PLANE;else ptype[T]=GL_OBJECT_PLANE;
		}
		else enable[T]=0;
		if (r >=0)
		{  enable[R]=1; params[R][r]=1.0;
		   if (eye) pname[R]=GL_EYE_LINEAR;else pname[R]=GL_OBJECT_LINEAR;
		   if (eyePlane) ptype[R]=GL_EYE_PLANE;else ptype[R]=GL_OBJECT_PLANE;
		}
		else enable[R]=0;
		enable[Q]=0;

		isOff=0;
   };

   void SetOff() {
		for (int i=S; i<=Q; i++) { enable[i]=0; }
		strcpy(parameter,"off");
		isOff=1;

   };

   // Set by string to some predefined modes
   int Set(const char *type) ;

   // Eval a planar mapping
   float EvalPlane(int i, Point &p,float q=1.0);

};


const char * GFogModeLookup(int v);
int GFogModeLookup(const char *v);

//@class GL Fog/Depth Cueing options (glFog)
class glFog {
public :
	gbool enable;  // glEnable

	int mode;
	float start,end;
	float density;

	float index;

	int useBackgroundColor;
	RGBAColor color;

	GLenum fogHint;

    static const IS modes[]; // table of filter names 


	void SetMode(const char *Mode) {
		mode = GFogModeLookup(Mode);
	};
	const char * GetMode() { return GFogModeLookup(mode); }



	glFog() {
			enable = 0;
			mode = GL_EXP;
			density = 1.0;
			start= 0.0;
			end = 1.0;
			index = 0.0;
			useBackgroundColor = 0;
			color.Gray(0.0);	// Gl Default is Black();
			fogHint = GL_DONT_CARE;
			};

	void SetQuality(int i) { if (i) fogHint = GL_NICEST; else fogHint = GL_FASTEST; }
	int  GetQuality() { return (fogHint == GL_NICEST) ? 1 : 0; }

	void glSet() {
	  	if (enable) {
		 glEnable(GL_FOG);
		 glFogi(GL_FOG_MODE,mode);
		 glFogf(GL_FOG_DENSITY,density);
		 glFogf(GL_FOG_START,start);
		 glFogf(GL_FOG_END,end);
		 // glFogf(GL_FOG_INDEX,index); // CIMODE only
		 if (!useBackgroundColor) {
			glFogfv(GL_FOG_COLOR,&color.r);
		 }
		 glHint(GL_FOG_HINT,fogHint);
	    } else glDisable(GL_FOG);
		};
       	
       	// Evaluate float
       	float Eval(float z);

};

// @class Background rendering (color gradient)

class glBackground
{
public :
    gbool enable;

    char type[60];
    int ncolors;
    Point colors[10];
	
	glCache cache;

    glBackground() { enable = 0; }

};


// class Renderflags : contains rendering style flags
//
class RenderFlags {
public:
		int visible;		// item is visible
		int lighted;		// item is affected by lights (so pass normals to GL)
		int smooth;			// item should be smooth (gouaud) shaded
		int textured;		// item should be textured, if parameters available
		int colored;		// item should use local colors, if available
		int useMaterialColor;	// use Material color (instead of item color )

		RenderFlags(int Visible=1,int Lighted=1,int Smooth=1,int Colored=0,int Textured=0,int UseMaterialColor=1) {
		   visible=Visible,lighted=Lighted,smooth=Smooth;
		   colored=Colored;
		   textured=Textured;
		   useMaterialColor=UseMaterialColor;
		}
};

//@enum GRenderMode | predefined render modes
enum GRenderMode { 
			       GRenderVertices,		//@emem render only the vertices
			       GRenderBoundingBoxes,	//@emem render bounding box of object only
				   GRenderWireframe, 	//@emem render only the edges
			       GRenderSolid,		//@emem render faces and edges with no lighting
				   GRenderNonLighted,	//@emem render faces with no lighting
				   GRenderFlat,			//@emem render faces with flag lighting
				   GRenderGouraud,		//@emem render faces with smooth lightings
				   GRenderBBox,			//@emem render only bounding box of object 
				   GRenderCustom 		//@emem mode is set to a custom mode
	};

//@enum GExtendedDrawStyles | Extended Drawstyle ,renders a object for vertices,edges, normals ...
enum GExtendDrawStyles {
		GAsDefault,
		GAsCylinder,
		GAsSphere,
		GAsCone,
		GAsDCone,
		GAsCube,
		GAsDisc
		};

extern const char * GRenderModeLookup(int v);
extern int GRenderModeLookup(const char *v);

extern const char * GDrawStyleLookup(int v);
extern int GDrawStyleLookup(const char *v);


//@class render a obect e.g. a sphere for a vertex, edge or normal
class GExtendedDrawStyle {
public : 
		gbool enabled;
		int type;
		int orient;	// orient with respect to normal
		int xsteps,ysteps;
		float scale1,scale2;

		gbool smooth;
		gbool textured;
		gbool lighted;

		/*GLenum*/ int normals;
		/*GLenum*/ int drawStyle;
		glCache cache;

		GExtendedDrawStyle (int Type = GAsDefault) {
				enabled=1;
				type =  Type;
				xsteps=8; ysteps=4;
				scale1 = 0.5; scale2 = 0.5;
				drawStyle = GLU_LINE;
				normals= GL_NONE;
				textured=0;
				smooth=1;
				lighted=1;
				orient=1;
		};

		void SetDrawStyle(int Style) {
			  if (Style <=0) type = GAsDefault;
			  else type = Style;
			  cache.Invalidate();
		}
		void SetChanged() {cache.Invalidate();}
		
		void SetDrawStyle(const char *style) { SetDrawStyle(GDrawStyleLookup(style)); }
		const char * GetDrawStyle() { return(GDrawStyleLookup(type)); }

		void SetScale(float Scale1,float Scale2) {
				scale1= Scale1; scale2 = Scale2;
			     cache.Invalidate();
		}
		
		void SetSteps(int Xsteps, int Ysteps) {
				xsteps = Xsteps; ysteps = Ysteps;
			    cache.Invalidate();
		}
		
		void SetTextured(gbool Textured) {
		  	  if (textured != Textured) {textured = Textured;  cache.Invalidate(); }
			  }

		void SetSmooth(gbool Smooth) {
		  	  if (smooth != Smooth) {smooth = Smooth;  cache.Invalidate(); }
			  }

		void SetLighted(gbool Lighted) {
		  	  if (lighted != Lighted) {lighted = Lighted;  cache.Invalidate(); }
			  }
		
		void SetFlags(gbool Lighted,gbool Textured,gbool Smooth)
		{ 
			 lighted = Lighted;
	   	     textured = Textured;
			 smooth = Smooth;
			 cache.Invalidate();
		};
		
		void SetRenderMode(GRenderMode m) {
			switch (m) {
			case GRenderWireframe : drawStyle =GLU_LINE; break;
			case GRenderVertices : drawStyle =GLU_POINT; break;
			default :
				drawStyle = GLU_FILL; break;
			}
			cache.Invalidate();
		}
		
		//! Render a vertex
		void RenderVertex(Point &p,Point &n);
		
		//! Render a normal
		void RenderNormal(Point &p,Point &n);

		//! Render a edge
		void RenderEdge(Point &p1,Point &n1,Point &p2,Point &n2);

		//! Render the object,transformation already setup
		void glRender() {
			  if (cache.Invalid())  glSetup();
			  cache.Call();
		}

		//! Setup object to be rendered
		void glSetup();

};

//
// @class GL light information
//

class glLight {
public:
	int enabled;

	RGBAColor ambient; // @cmember ambient color  of light
	RGBAColor diffuse;
	RGBAColor specular;

    int directional;
	int spot;  			// is this a spot light ?
	Point position;
	Point direction;

	float SpotExponent; // [0..128]
	float SpotCutoff; // [ 0 .. 90 ]  or 180 special value
	Point Attenuation;	// Constant, Linear , Quadratic

	glLight() {
		ambient.Set(0,0,0, 1.0);
		diffuse.Set(1,1,1, 1.0);// gl default is ::	diffuse.Set(0,0,0, 1.0);
		specular.Set(0,0,0, 1.0);
		directional = 1;  
		spot=0;
		position.Set(0,0,1);
		direction.Set(0,0,-1);;
		SpotExponent = 0;
		SpotCutoff = 180;
		Attenuation.Set(1,0,0);
	 };

	 void glSet(int light);
};



//
// @class maintains a handle to a OpenGL  Texture
//
class GglTextureHandle : public GTextureHandle  {
public: 


	GglTextureHandle() {  
			texture = 0;
			}
	~GglTextureHandle() { Release(); }

	GglTextureHandle *Next() { return (GglTextureHandle *) next; }

	//! the OpenGL texture object name, 0 is invalid
	GLuint texture;	// texture object name , 0 is invalid

	//! OpenGL callist for texture
	glCache  cache; 

	//! OpenGL texture object extension function pointers

	static int hasTobjEXT;
	
	static glAreTexturesResident_t glAreTexturesResidentEXT;
	static glBindTexture_t glBindTextureEXT;
	static glGenTextures_t glGenTexturesEXT;
	static glDeleteTextures_t glDeleteTexturesEXT;
	static glIsTexture_t glIsTextureEXT;
	static glPrioritizeTextures_t glPrioritizeTexturesEXT;



#ifdef _GL_TOBJ

	// Opengl 1.1 use texture objects 

	void  New() {
		glGenTextures(1,&texture);	
	}

    // Is definition valid ???
	gbool Ok() { return(texture !=0); }

	// set the texture as current GL texture
	void glSet() {  

		glBindTexture(GL_TEXTURE_2D,texture);
		rendered = 1;	
	}
    
    // release (free)  texture
    void Release() { 
		glDeleteTextures(1,&texture);	
		texture = 0;
	}
	
	void Update(glTextureParameters &textureParameters) 
	{
		if (texture !=0) {
			glBindTexture(GL_TEXTURE_2D,texture);
			textureParameters.glSet();
		}
	}

	GLboolean IsTexture() const { return glIsTexture(texture); 	}
 


#else

	// fallback with OpenGL 1.0 call list, and optional use of textureObjectExtension

    // Is definition valid ???
	gbool Ok() { return( (texture !=0) || cache.IsValid()); }

	// create new texture object 
	void New() {
		if (hasTobjEXT) 
			glGenTexturesEXT(1,&texture);	
	}

	// set the texture as current GL texture
	void glSet() {  
		if (texture !=0) //
			glBindTextureEXT(GL_TEXTURE_2D,texture);
		else cache.Call();  
		rendered=1;
	}

	void Update(glTextureParameters &textureParameters) 
	{
		if (texture !=0) {
			glBindTexture(GL_TEXTURE_2D,texture);
			textureParameters.glSet();
		}
	}
    
    // release (free)  texture
    void Release() { 
		if (texture !=0) {
			glDeleteTexturesEXT(1,&texture);	
			texture = 0;
		}
		else cache.Delete();
		inVideoMem=0;
	}

#endif

    void Ref() { refCnt++; }

    void UnRef() { refCnt--;  if (refCnt == 0) Release(); }

	// set texture by texture filename 
	gbool Set(RenderState &state, LPCTSTR fileName,int format, LPCTSTR fileUrl=NULL);

	// set texture from image data
	gbool Set(RenderState &state,int format,int width,int height,int numComponents,unsigned char *data, char *colorTable=NULL);

	// set by image 
	gbool Set(RenderState &state,image *im,gbool checkScale=TRUE,gbool checkFormat=TRUE);


	// reload 
	gbool ReloadTextureSurf();

	// load surface from fileName
	gbool LoadTextureSurf();			 

};

typedef enum 
{
  GGL_NURBS_SOFTWARE,	//	use own software
  GGL_NURBS_GLU,		//	use GLU nurbs triangulator
  GGL_NURBS_GLU_CALL_LIST, // " and use a call llist
  GGL_NURBS_OFF			// no Nurbs at all

} GGlNurbsMode;




// GLRenderState
// @class class caching all OpenGL render attributes and current GL states
//
class RenderState  : 
	public GRenderState,
	public GRenderer2D
{
public:
	 // a frame counter to measure texture oldiness

	 DWORD					currentFrame; // the current frame number
	 
	 LARGE_INTEGER			currentFrameStartCounter; // QueryPerformanceCounter

	 void IncrementFrame()   { 
					currentFrame++;
					lastFramePrimsDrawn = primsDrawn;
					primsDrawn = 0;
					lastFrameTrisDrawn=trisDrawn;
					trisDrawn = 0;
					}

	 inline DWORD GetCurrentFrame() { return currentFrame; }
	 
	 void ResetFrame()		 {
		 lastFramePrimsDrawn = primsDrawn = 0;
		 lastFrameTrisDrawn=trisDrawn = 0;
		 currentFrame = 2; 
	 }

	 int primsDrawn;
	 int trisDrawn;
	 int lastFramePrimsDrawn;
	 int lastFrameTrisDrawn;

	 int currentTriBudget; //!< estimated target triangle budget for scene

	 GGlNurbsMode nurbsMode;	// NURBS Mode

	 gbool IsOldFrame(DWORD nr,DWORD delta) const { return (nr<(currentFrame-delta)); }

	 gbool canCull;				//!< is culling supported (otherwise assume CCW culling done)
	 gbool canDoRgbLighting;	//!< RGB Mode lighting ? 
	 gbool canDoFog;			//!< fog ??
	 gbool canDoAlphaBlend;		//!< ALphablending of Materials & Textues
	 gbool canDoMipMap;			//!< any mim-map formt ?
	 gbool canDoAlphaStipple;
	 gbool canDoAlphaTest;		//!< alpha testing working ?
	 
	 GLenum maxMipfilter;		//!< highest level of mip-map min filter supported
	
	 void FlushState() {};


    GglTextureHandle* textures;	//!< list of all texture handles in use 

    GglTextureHandle* currentTexture; //!< the current texture

    // Texture 

	// get allowable image format mask
	int GetSupportedImageFormats() { return textureParameters.supported_image_fmts; }

    void SetTexture(GglTextureHandle *handle) 
	{ 
		if (handle == currentTexture) {
			TRACE("Same texture handle %p \n",handle);
			return;
		}
		if (handle) handle->glSet();
		currentTexture = handle;
	} 

    // Set Texture, if result FALSE, need (re) definition of Texture 
    gbool SetTexture(GTextureRef &ref)
    {
        if (!ref.Ok()) return(FALSE);
		GglTextureHandle *h = (GglTextureHandle *) ref.handle; // downcast 
        SetTexture(h);
		return(TRUE);
    }

	// Reload Texture, if it has been flushed 
    gbool ReloadTexture(GTextureRef &ref)
    {	gbool ret = FALSE;
		if (ref.handle == NULL) return (FALSE); // never defined
        if (!ref.handle->Ok()) {
		   if (ref.handle->CantLoad()) 
			   return (FALSE); // file lost !!!
		   GglTextureHandle *h = (GglTextureHandle *) ref.handle; // downcast 
		   ret = h->LoadTextureSurf();
		}

        return(ret);
    }

	// set Texture properties
	// set texture by filename
	gbool SetTexture(GTextureRef &ref,LPCTSTR fileName,LPCTSTR fileUrl,int format=0,gbool reload=gfalse); 
	
	// find if a texture with this URL already loaded 
	gbool FindTextureByUrl(GTextureRef &ref,LPCTSTR fileUrl); 

	
	// set by image data 
	gbool SetTexture(GTextureRef &ref,int glFormat,int width,int height,int numComponents,unsigned char *data);

	// set texture by image structure 
	gbool SetTexture(GTextureRef &ref,image *im,gbool checkScale=TRUE);


#if 0
	gbool SetDefaultTexture(const char *fileName,int format = 0) 
    {
        return(SetTexture(defaultTexture,fileName,format));
    }

    void ReleaseDefaultTexture() 
    {
      defaultTexture.Release();
    }
#endif

	//! compute target texture format size 
	void GetTextureTargetSize(int &width,int &height,int roundUpLimit=128);

	//! get max texture size
	void GetTextureMaxSize(int &size_x,int &size_y) { textureParameters.GetMaxSize(size_x,size_y); }

	//! get current max texture size
	void GetTextureCurrentMaxSize(int &size_x,int &size_y) { textureParameters.GetCurrentMaxSize(size_x,size_y); }

	//! set current max texture size
	void SetTextureCurrentMaxSize(int size_x,int size_y) { textureParameters.SetCurrentMaxSize(size_x,size_y); }

	//! get min texture size
	void GetTextureMinSize(int &size_x,int &size_y)  { textureParameters.GetMinSize(size_x,size_y); }



    
    //! create a new Texture handle with a ref cnt of 1 
    GglTextureHandle* CreateTexture();

    //! delete an no longer used Texture
    void DeleteTexture(GTextureHandle *m);

    //! release all Texture handles
    void ReleaseTextures();

    //! release textures not rendered in the last frame
    gbool ReleaseUnrenderedTextures(int max=1);

    //! release large textures 
    gbool ReleaseLargeTextures(int max=1);

	//! compute the size of the largest textures 
	gbool GetMaxUsedTextureSize(int &sizeX,int &sizeY, int &bytesPerPix);


    //! Delete all Texture handles
    //! should only be called if all references deleted !!!!!!!!!!!!1
    void DeleteTextures();

    //! Delete all Texture handles no longer in use 
    gbool DeleteUnusedTextures();

	//! texture memory overflow 
	//! if result TRUE, try to reload texture in progress
	gbool OnTextureMemoryOverflow(GTextureHandle &overflowHandle);

	//! update gl texture objects with texture parameters
	void UpdateTextures();




	 // flags keeping track of current gl-state
	 gbool zbuffer;          // zbuffer enabled ?
	 GLenum depthFunc;      // depth func 
	 gbool zwrite;          // zbuffer write enabled ?

	 gbool dither;           // dithering enabled ?
	 gbool sbuffer;
	 gbool lighting;          // lighting needed

	 GLenum lightmodel;        // GL_SMOOTH / FLAT etc 
	 gbool  localColors;       // use local face/vertex colors of object
	 GLenum localColorsMode;  // what property to replace 	 


	 gbool backfaceCulling;
	 int cullFront; // ??
	 gbool frontFaceCW;		// glFrontFace

	 gbool texturing;         // using texturing 
	 gbool textureDefined;	// texture has been set
     int textureEnvMode;    //

	 gbool twosidedLighting;
	 gbool localViewer;       // gl local viewer
	 int lineSmooth;        // line smooth
	 int colorMaterial;		// glColorMaterial
	 int colorMaterialBack;
	 int computePrimitiveList;	// compute tristrips for shells

	 gbool blend;				// blend enabled
	 gbool blendFromMaterial;	// blend test enabled from material

	 GLenum blendSrcFunc,blendDestFunc;

	 gbool alphaTest;		// alpha test enabled
	 gbool alphaTestFromTexture;		// alpha test enabled from texture

	 GLenum alphaFunc;
	 GLfloat alphaRef;


	 float alpha;		// alpha value from material

	 gbool normalize;     // need normalization of normals
	 float lightScale;	// global light scale factor 

     // Hints 
	 //was int
     GLenum perspectiveCorrectionHint;
     GLenum pointSmoothHint;
     GLenum lineSmoothHint;
     GLenum polygonSmoothHint;
     
     // int fogHint; part of glFog 
	 gbool fogEnabled;				// fog  enabled

	 glFog fog;						// fog settings

	 glBackground backgroundOptions;	// background settings

	 glTextureParameters textureParameters;
	 glTextureGeneration textureGeneration;

	 glMatrix modelMatrix;
	 glMatrix projectionMatrix;

	 // obsolete 
	 //glMatrix textureMatrix;

     // set view and projection matrix from camera 
     void SetCamera(GCamera *camera);



	 Transform3 textureTransform3;

	 //gbool textureMatrixIdentiy;		  // current texture matrix is identity	

	 // set texture matrix
	 inline void SetTextureMatrix(const Matrix &m) { 
		// textureMatrix = m; textureMatrixVersion++; 
		textureMatrixIdentity = FALSE; 
 		glMatrixMode(GL_TEXTURE);
		glLoadMatrixf(&m[0][0]);
		glMatrixMode(GL_MODELVIEW);
	 }

	 // set texture matrix, together with version ID, ID = 0, matrix is identity
	 void SetTextureMatrix(const Matrix &m, unsigned long version) 
	 { SetTextureMatrix(m); textureMatrixVersion=version; textureMatrixIdentity = (version == 0); }


	 // set texture matrix
	 inline void AppendTextureMatrix(const Matrix &m) { 
		// textureMatrix = m; textureMatrixVersion++; 
		textureMatrixIdentity = FALSE; 
 		glMatrixMode(GL_TEXTURE);
		glMultMatrixf(&m[0][0]);
		glMatrixMode(GL_MODELVIEW);
	 }

	 // set texture matrix, together with version ID, ID = 0, matrix is identity
	 // void SetTextureMatrix(const Matrix &m, unsigned long version) { textureMatrix = m; textureMatrixVersion=version; textureMatrixIdentiy = (version == 0); }

	 // set texture matrix to identity
	 void SetTextureMatrixIdentity() { 
		 if (!textureMatrixIdentity) { 
			// textureMatrix.Identity();  
			textureMatrixIdentity = TRUE; 
			glMatrixMode(GL_TEXTURE);
				glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
		 } 
	 }


#if 0

	 void SetTextureTransform(const Transform3 &t) {
			textureTransform3 = t;
			glMatrixMode(GL_TEXTURE);
				glLoadIdentity();
				textureTransform3.glSet();
			glMatrixMode(GL_MODELVIEW);
			textureMatrixIdentiy = FALSE; 
			}

     void SetTextureTransform() {
			glMatrixMode(GL_TEXTURE);
				glLoadIdentity();
				textureTransform3.glSet();
			glMatrixMode(GL_MODELVIEW);
	}


	 void UnSetTextureTransform() {
			textureTransform3.Identity();
			glMatrixMode(GL_TEXTURE);
				glLoadIdentity();
				textureMatrixIdentiy = TRUE; 
			glMatrixMode(GL_MODELVIEW);
			}

#endif



	 // limits
	 int maxMmatrixStack;
	 int maxLights;
	 int maxTmatrixStack;

	 RGBAColor background;
	 RGBAColor ambient;		// ambient light color

	 glMaterial material;
	 glMaterial backMaterial;

	 
	 // renderstyle flags 

	 RenderFlags vertices;
	 RenderFlags normals;
	 RenderFlags edges;
	 RenderFlags faces;
	 RenderFlags bboxes;

	 RGBAColor currentColor; // current unlit color


	 RGBAColor vertexColor;
	 RGBAColor edgeColor;
	 RGBAColor faceColor;
	 RGBAColor normalColor;
	 RGBAColor bboxColor;

	 float pointSize;
	 float lineWidth;
     int   lineStipple; // glenable();   
     int   lineFactor;
     GLushort linePattern; 

	 gbool	litMaterial; // flag TRUE if current material is lighted

	 GRenderMode renderMode;
	 gbool LightMode;  // lighted wireframe ...

 	 // Drawing extensions ......
 	 GExtendedDrawStyle *vertexDrawStyle,
	 					*edgeDrawStyle,
	 					*normalDrawStyle,
						*bboxDrawStyle;


	 RenderState();
	 
	 ~RenderState();


	 // clear buffers
	 void Clear();
	 
	 // clear buffers
	 void Clear(gbool colorBuffer,gbool zBuffer,gbool stencilBuffer=gfalse,gbool accumBuffer=gfalse); 

	 // set current viewport rectangle
	 void SetViewport(int x,int y,int width, int height);

	 // set current viewport to max rectangle
	 void SetViewport();

	 // get current viewport
	 void GetViewport(int &x,int &y,int &width, int &height);

	 // set global render mode to mode (enum)
     int SetRenderMode(GRenderMode newMode);

	 int SetRenderFlags(RenderFlags &flags);

	 // Set current color, also change material color
	 void SetColor(const RGBAColor &c);

	 // set current color // e.g. material diffuse, needed for unlit execute buffers
	 void SetCurrentColor(const RGBAColor &Color) {
	     SetColor(Color);
	 };

	 void SetCurrentColor(const Point &Color) {
	     currentColor = Color;
		 SetColor(currentColor);
	 };

	 void SetCurrentColor(float r,float g,float b, float a=1.0f) {
	     currentColor.Set(r,g,b,a);
		 SetColor(currentColor);
	 };

 	 void SetFogEnable(gbool flag) 
	 {
		 if (fogEnabled == flag) return;
		 fogEnabled = flag;
	  	 if (flag) 
			 glEnable(GL_FOG);
		 else glDisable(GL_FOG);
	 }		 


	// apply the fogsettings described in the Fog structure
	void SetFog() {
		if (fog.enable) {
		    fogEnabled = TRUE;
			if (fog.useBackgroundColor) {
				glFogfv(GL_FOG_COLOR,&background.r);
			}
			fog.glSet();
		}
		else {
			SetFogEnable(FALSE);
		}

	}

 
	 // Set Rendering Attributes to vertex
	 int SetVertexAttributes();

	 // Set Rendering Attributes to edge
	 int SetEdgeAttributes();

	 // Set Rendering Attributes for faces
	 int SetFaceAttributes(); 

	 // Set Rendering Attributes for normals
	 int SetNormalAttributes(); 

	 // Set Rendering Attributes for bounding boxes
	 int SetBBoxAttributes(); 

	 // set texture flags for faces edges vertices
	 int SetAnyTexturing(int state)
	 {	
		faces.textured = state;
		edges.textured = state;
		vertices.textured = state;

		// disable alpha test if enabled only for texture
		if (!state && alphaTestFromTexture) {
			SetAlphaTestFromTexture(FALSE);
		}
		return(state);
	 }

	 int SetAnyLighting(int state)
	 {	
		faces.lighted = state;
		edges.lighted = state;
		vertices.lighted = state;
		return(state);
	 }


	 
	 int SetFaceTexturing(int state)
	 {	
		faces.textured = state;
		if (state && faces.visible)
			edges.textured = 0;
		return(state);
	 }

	 int SetEdgeTexturing(int state)
	 {	
		edges.textured = state;
		if (state && edges.visible)
			vertices.textured = 0;
		return(state);
	 }

	 int SetEdgeLighting(int state)
	 {	
		edges.lighted = state;
		return(state);
	 }


     // set all cached glstate parameters 
     void SetContextAll();
	 
	 // Set Z-Buffer Mode  on/off
	void SetZBuffer(gbool Zbuffer) {
		if (zbuffer == Zbuffer) return;
		zbuffer = Zbuffer;
        SetZBuffer();
    }

    void SetZBuffer() {
		if (zbuffer) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);
	};

	 // Set Z-Buffer Mode  on/off
	void SetZWrite(gbool ZWrite) {
		if (zwrite == ZWrite) return;
		zwrite = ZWrite;
        SetZWrite();
    }

    void SetZWrite() {
		glDepthMask(zwrite);
	};

    void SetDepthFunc(GLenum DepthFunc) {
		depthFunc= DepthFunc;
		glDepthFunc(depthFunc);
	};

    void SetDepthFunc(int DepthFunc) {
		SetDepthFunc((GLenum) DepthFunc);
	};

	// Set Dither Mode  on/off
	void SetDither(gbool Dither) {
		if (dither == Dither) return;
		dither = Dither;
        SetDither();
    }

    void SetDither() {
		if (dither) glEnable(GL_DITHER);
		else glDisable(GL_DITHER);
	};

	// Set Alpha Stipple  on/off
	void SetAlphaStippleDither(gbool /*AlphaStipple*/) { }

    void SetAlphaStipple() { }


	void SetBlend(gbool Blend) {
		if (blend == Blend) return;
		blend = Blend;
        SetBlend();
    }

    void SetBlend() {
		if (blend) glEnable(GL_BLEND);
		else glDisable(GL_BLEND);
	};

	void SetBlendFromMaterial(gbool Blend) {
		if (blend == Blend) return;
		blend = Blend;
		blendFromMaterial = blend;
        SetBlend();
    }

	void SetLitMaterial(gbool Lit) {
		if (Lit == litMaterial) return;
		litMaterial = Lit;
		if (!Lit) SetLighting(0);
    }


    void SetBlendFunc(GLenum BlendSrcFunc,GLenum BlendDestFunc) 
	{
		blendSrcFunc=BlendSrcFunc,blendDestFunc=BlendDestFunc; 
		glBlendFunc(blendSrcFunc,blendDestFunc);
	}
    
	void SetBlendFunc(int BlendSrcFunc,int BlendDestFunc) 
	{
		SetBlendFunc((GLenum) BlendSrcFunc,(GLenum) BlendDestFunc);
	}


	void SetMaterialAlpha(float Alpha) 
	{
		alpha=Alpha;

		if (alpha < 1.0) {
			SetBlendFromMaterial(TRUE);
		}
		else SetBlendFromMaterial(FALSE);
	}

	void SetMaterialAlphaOff() 
	{
		alpha=1.0;
		SetBlendFromMaterial(FALSE);
	}


	void SetAlphaTest(gbool AlphaTest) {
		if (alphaTest == AlphaTest) return;
		alphaTest = AlphaTest;
        SetAlphaTest();
    }

    void SetAlphaTest() {
		if (alphaTest) glEnable(GL_ALPHA_TEST);
		else glDisable(GL_ALPHA_TEST);
	};

	void SetAlphaTestFromTexture(gbool AlphaTest) {
		if (alphaTest == AlphaTest) return;
		alphaTest = AlphaTest;
		alphaTestFromTexture = alphaTest;
        SetAlphaTest();
    }

    void SetAlphaFunc(GLenum AlphaFunc,GLfloat AlphaRef) 
	{
		alphaFunc=AlphaFunc;
	    alphaRef=AlphaRef;
		glAlphaFunc(alphaFunc,alphaRef);
	}

    void SetAlphaFunc(int AlphaFunc,float AlphaRef) 
	{
		SetAlphaFunc((GLenum) AlphaFunc,AlphaRef);
	}

	void SetPointSize(float PointSize) {
		pointSize = PointSize;
		glPointSize(pointSize);
	}

	void SetPointSize() {
		glPointSize(pointSize);
	}

    void SetLineWidth(float width) {
		lineWidth = width;
		glLineWidth(lineWidth);
	}
    void SetLineWidth() {
		glLineWidth(lineWidth);
	}

    void SetLineStipple()
    {
        if (lineStipple) {
            glLineStipple(lineFactor,linePattern);
            glEnable(GL_LINE_STIPPLE);
        }
        else {
            glDisable(GL_LINE_STIPPLE);
        }
    }

	 // Set Auto normalize  on/off
	 void SetNormalize(gbool Normalize) {
			if (normalize == Normalize) return;
			normalize = Normalize;
			if (normalize) glEnable(GL_NORMALIZE);
			else glDisable(GL_NORMALIZE);
	};

	 void SetNormalize() {
			if (normalize) glEnable(GL_NORMALIZE);
			else glDisable(GL_NORMALIZE);
	 };

	 // Set Light Mode on/off
	 void SetLighting(gbool Lighting) {
			if (lighting == Lighting) return;
			lighting = Lighting;
			if (lighting) glEnable(GL_LIGHTING);
			else glDisable(GL_LIGHTING);
	 };
	 
     void SetLighting() {
			if (lighting) glEnable(GL_LIGHTING);
			else glDisable(GL_LIGHTING);
	 };

	 void SetLightModel(int Lightmodel) {
			if (lightmodel == (GLenum) Lightmodel) return;
			lightmodel = (GLenum) Lightmodel;
			glShadeModel(lightmodel);
	 };
	 
	 void SetLightModel() {
			glShadeModel(lightmodel);
	 };

	
	// set / get lightScale factor
	
	void SetLightScale(float factor) { lightScale = factor; }
	
	float GetLightScale() { return(lightScale); }


	 void SetTwoSidedLighting(gbool Lighting) {
			if (Lighting == twosidedLighting) return;
			twosidedLighting = Lighting;
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,twosidedLighting);
	 };

     void SetTwoSidedLighting() {
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,twosidedLighting);
	 };

	 // mode = 0 specular reflection is computed fom the origin of eye coordinate sys.
	 // otherwise simply in direction of - z
	 void SetLocalViewer(gbool mode) {
			if (mode == localViewer) return;
			localViewer = mode;
			glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,mode);
	 };
	 
     void SetLocalViewer() {
			glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,localViewer);
	 };


	 void SetMaterial(const glMaterial &Material) {
			material = Material;
			material.glSet();
			if (localColors) glColor(material.diffuse); 
	 };

	 void SetMaterial() {
			material.glSet();
			if (localColors) glColor(material.diffuse);
	 };

    // Set Material
    gbool SetMaterial( /* G3MaterialRef &ref, */			
            // material parameters
			float ambient[3],float diffuse[3],float specular[3],float emissive[3],
			float alpha,float power,gbool whiteDiffuse);


	 // set background color = gl clear color
	 void SetBackgroundColor(const RGBAColor &Color) {
	     background = Color;
		 glClearColor(background.r,background.g,background.b,background.a);
		 // ACCUM-BUffer  clear color
	 };
	 void SetBackgroundColor() {
		 glClearColor(background.r,background.g,background.b,background.a);
	 };


	 // set global ambient light color
	 void SetAmbientColor(const RGBAColor &Color) {
	     ambient = Color;
		 glLightModelfv(GL_LIGHT_MODEL_AMBIENT,&ambient.r);
	 };
	 void SetAmbientColor() {
		 glLightModelfv(GL_LIGHT_MODEL_AMBIENT,&ambient.r);
	 };

	 // Enable display of local vertex colors in lighted mode
	 void SetLocalColors(gbool mode) {
		  if (mode == localColors) return;
		  localColors = mode;
          SetLocalColors();
     }

	 // mode true && lights == false ==> replace emissive
	 void SetLocalColors(gbool mode,gbool lighted) {
		  localColors = mode;
          if (localColors ) {
			 if (lighted) glColorMaterial(GL_FRONT_AND_BACK,localColorsMode);
			 else glColorMaterial(GL_FRONT_AND_BACK,GL_EMISSION);
			 glEnable(GL_COLOR_MATERIAL);
		  }
		  else glDisable(GL_COLOR_MATERIAL);
     }

     void SetLocalColors() {
          if (localColors ) {
			 glColorMaterial(GL_FRONT_AND_BACK,localColorsMode);
			 glEnable(GL_COLOR_MATERIAL);
		  }
		  else glDisable(GL_COLOR_MATERIAL);
	 };

     // disable / enable backface culling 
	 void SetBackfaceCulling(gbool mode) {
		  if (mode == backfaceCulling) return;
		  backfaceCulling = mode;

		  if (backfaceCulling) {
			 glEnable(GL_CULL_FACE);
		  }
		  else glDisable(GL_CULL_FACE);
	 };
	 
     void SetBackfaceCulling() {
		  if (backfaceCulling) {
			 glEnable(GL_CULL_FACE);
		  }
		  else glDisable(GL_CULL_FACE);
	 };

	 // mode 0 Front Facing polyongs are oriented counter clock wise
	 // mode 1   " 	   "						  clock wise
	 void SetFrontFaceCW(gbool mode) {
		  if (mode == frontFaceCW) return;
		  glFrontFace(mode ? GL_CW : GL_CCW);
		  frontFaceCW = mode;
	 };
	 void SetFrontFaceCW() {
		  glFrontFace(frontFaceCW ? GL_CW : GL_CCW);
	 };

	 void SetCulling(gbool cull, gbool modeCW) {
         SetBackfaceCulling(cull);
         SetFrontFaceCW(modeCW);
		 SetTwoSidedLighting(!cull); // new 22.10.98
     }


     // disable/enable texturing 
	 void SetTexturing(gbool mode) {
		  if (mode == texturing) return;
		  if (mode ) {
			 glEnable(GL_TEXTURE_2D);
			 textureGeneration.glEnable_();
		  }
		  else {
		    glDisable(GL_TEXTURE_2D);
			textureGeneration.glDisable_();
		  };
		  texturing = mode;
	 };

	 void SetTexturing() {
		  if (texturing) {
			 glEnable(GL_TEXTURE_2D);
			 textureGeneration.glEnable_();
		  }
		  else {
		    glDisable(GL_TEXTURE_2D);
			textureGeneration.glDisable_();
		  };
	 };
     // Texture options 
	 void SetTextureParameters() 
        {  textureParameters.glSet();
        }

	 // if flag set_diffuse_white is set, set WHite on Texturing
	 void SetTexturingDiffuseColor(gbool setBorder=gtrue) {
		if (texturing && textureParameters.set_diffuse_white)
		{   RGBAColor w;
			w.White();
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,&w.r);
			glColor(w);
			if (setBorder) glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,&material.diffuse.r);
		}
	 };
	 
	 void SetTextureWrap(BOOL repeatS,BOOL repeatT) {
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,repeatS ?  GL_REPEAT : GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,repeatT ?  GL_REPEAT : GL_CLAMP);
	 }

	void SetTextureBlend(int blend_) 
	{
		GLenum blend = (GLenum) blend_; 

		if (blend != textureParameters.blend) {;
			textureEnvMode = textureParameters.blend = blend;
	        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,textureParameters.blend);

		}
	}

	void SetTextureBlend_(int blend_) {
		GLenum blend = (GLenum) blend_;
		textureEnvMode = textureParameters.blend = blend;
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,textureParameters.blend);

	}

	void SetTextureBlend() {
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,textureParameters.blend);
	}




	 void SetLineAntialiasing(int mode) {
         lineSmooth = mode;
         SetLineAntialiasing();

     }    
	 
     void SetLineAntialiasing() {
         if (lineSmooth) {
				glEnable(GL_LINE_SMOOTH);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
				glHint(GL_LINE_SMOOTH_HINT,lineSmoothHint);
				glLineWidth(1.5);
			 } else {
				glDisable(GL_LINE_SMOOTH);
				glDisable(GL_BLEND);
				glLineWidth(1.0);
			 }
			 
	 };
     
     void SetPerspectiveCorrectionHint(int hint) {
         perspectiveCorrectionHint =  (GLenum) hint;
    	 glHint(GL_PERSPECTIVE_CORRECTION_HINT,perspectiveCorrectionHint);

     }   

     void SetComputePrimitiveList(int flag) { computePrimitiveList = flag; }


	 // primitive Rendering 
	 void RenderVertex(Point &p);
	 void RenderLine(Point &p1,Point &p2);
	 void RenderBBox(Point &p1,Point &p2);
	 void RenderCylinder(Point &p, float r, float h);
	 void RenderCone(Point &p, float r, float h);
	 //void RenderSphere(Point &p,float r);

	 // get GL limits of interest for our renderer
	 void GetLimits();


	 // End of rendering to this frame
	 void Finish()
	 {
		glFinish();
	 };

 	 // cache
	 glCache modelCache;
	 glCache textureCache;


	 int (*AbortChecker)(RenderState &s);
	 void * AbortCheckerData;
	 int doAbortCheck;

	 float LastFrameTime; // Time needed to do last update
	 
	 // Check if redraw should be aborted
	 int Abort() {  if (doAbortCheck)  return(AbortChecker(*this));
	 				return(0);
			    }
	// 
	int bufferMode; // 0 swapbuffers, 1 front buffer, 2 scale

	void SetBufferMode(int mode);
	int GetBufferMode() { return(bufferMode); }
	int SwapBuffers();
	
	int UpdateFrontBuffer();


	//! viewport (window) size in pixel
	int sizex,sizey;
	
	//! rendering viewport size in pixel
	int renderSizex,renderSizey;

	void SetSize(int sizeX,int sizeY);
	void SetRenderSize(int sizex,int sizey);

	// Primitives
	void RenderSphere(Point &center, float r=1.0,int level=1, int mask = 1, int VN=1, int FN=0);
	
	// render a wireframe cube 
	void RenderCubeEdges(double x0, double y0,double z0,double x1,double y1,double z1);
	void RenderCubeEdges(const Point &p0,const Point &p1);

	enum RenderElements {
		RVertices = 1,
		REdges = 2,
		RFaces = 4,
		RNormals = 8,
		RBBox	= 16
	};

	// caching help for qv
	// the material node, which recently has been set
	void * currentMaterialNode;
	void * currentBaseColorNode;
	int currentMaterialIndex;
	void *currentTexture2Node;
	void *currentCameraNode;
	int currentMaxLight;

	void FlushCurrent() {
		currentMaterialNode = 0;
		currentBaseColorNode = 0;
		currentMaterialIndex = 0;
		currentTexture2Node = 0;
		currentCameraNode = 0;
		currentMaxLight=0;
	}

	//! use OGL 1.1 vertex arrys, Warning EXT functions are a bit different ?????
    gbool useVertexArray;

	/* EXT_vertex_array */
    gbool useVertexArrayEXT;
    gbool hasVertexArrayEXT;

    PFNGLARRAYELEMENTEXTPROC glArrayElementEXT; 	// (GLint i);
    PFNGLDRAWARRAYSEXTPROC	glDrawArraysEXT; 		// (GLenum mode, GLint first, GLsizei count);
    PFNGLVERTEXPOINTEREXTPROC glVertexPointerEXT; 	// (GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
    PFNGLNORMALPOINTEREXTPROC glNormalPointerEXT; 	//	 (GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
    PFNGLCOLORPOINTEREXTPROC glColorPointerEXT;	// (GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
    PFNGLINDEXPOINTEREXTPROC glIndexPointerEXT;	//(GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
    PFNGLTEXCOORDPOINTEREXTPROC glTexCoordPointerEXT;	//(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
    PFNGLEDGEFLAGPOINTEREXTPROC glEdgeFlagPointerEXT;	//(GLsizei stride, GLsizei count, const GLboolean *pointer);
    PFNGLGETPOINTERVEXTPROC		glGetPointerVertexEXT;	// (GLenum pname, GLvoid* *params);
	// ?? PFNGLARRAYELEMENTARRAYEXTPROC // (GLenum mode, GLsizei count, const GLvoid* pi);



    /* GL_EXT_compiled_vertex_array */
    gbool useCompiledVertexArrayEXT;
    gbool hasCompiledVertexArrayEXT;

    
	glLockArrays_t glLockArraysEXT;
    glUnlockArrays_t glUnlockArraysEXT;


    /* WIN_swap_hint */
    int useWinSwapHintEXT;
    int hasWinSwapHintEXT;

    PFNGLADDSWAPHINTRECTWINPROC	glAddSwapHintRectWin;	//  (GLint x, GLint y, GLsizei width, GLsizei height);


    /* GL_EXT_texture_object */
    int hasTobjEXT;
    int useTobjEXT;
	glAreTexturesResident_t glAreTexturesResidentEXT;
	glBindTexture_t glBindTextureEXT;
	glGenTextures_t glGenTexturesEXT;
	glDeleteTextures_t glDeleteTexturesEXT;
	glIsTexture_t glIsTextureEXT;
	glPrioritizeTextures_t glPrioritizeTexturesEXT;



    // OGL 1.1
#ifdef GL_VERSION_1_1    
	 gbool hasBgraEXT;	
  	 gbool hasPaletedTextureEXT;	

    /* EXT_paletted_texture */
     PFNGLCOLORTABLEEXTPROC glColorTableEXT; // (GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const GLvoid *data);
     PFNGLCOLORSUBTABLEEXTPROC glColorSubTableEXT; //   (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
     PFNGLGETCOLORTABLEEXTPROC glGetColorTableEXT; //  (GLenum target, GLenum format, GLenum type, GLvoid *data);
     PFNGLGETCOLORTABLEPARAMETERIVEXTPROC glGetColorTableParameteriEXT; // (GLenum target, GLenum pname, GLint *params);
     PFNGLGETCOLORTABLEPARAMETERFVEXTPROC glGetColorTableParameterfEXT; // (GLenum target, GLenum pname, GLfloat *params);

	  // WINGDIAPI void APIENTRY glBindTexture (GLenum target, GLuint texture);
	 // WINGDIAPI void APIENTRY glGenTextures (GLsizei n, GLuint *textures);
	 //WINGDIAPI void APIENTRY glDeleteTextures (GLsizei n, const GLuint *textures);


 

#endif    

/* GL_EXT_texture_env_add */
	gbool has_texture_env_add_EXT;	
  	gbool use_texture_env_add_EXT;	

/* GL_EXT_texture_env_combine */
	gbool has_texture_env_combine_EXT;	
  	gbool use_texture_env_combine_EXT;	


/* ARB_multitexture */
	gbool hasARB_multitextureEXT;	
  	gbool useARB_multitextureEXT;	

	int maxEnabledTexUnit; // maximum enabled texture unit
	int maxTexUnit;			// maximum available texture units

/*
	PFNGLMULTITEXCOORD1DARBPROC	glMultiTexCoord1dARB; //GLenum target, GLdouble s);
	PFNGLMULTITEXCOORD1DVARBPROC glMultiTexCoord1dvARB	; //GLenum target, const GLdouble *v);
	PFNGLMULTITEXCOORD1FARBPROC	glMultiTexCoord1fARB; //GLenum target, GLfloat s);
	PFNGLMULTITEXCOORD1FVARBPROC glMultiTexCoord1fvARB	; //GLenum target, const GLfloat *v);
	PFNGLMULTITEXCOORD1IARBPROC	glMultiTexCoord1iARB; //GLenum target, GLint s);
	PFNGLMULTITEXCOORD1IVARBPROC glMultiTexCoord1ivARB	; //GLenum target, const GLint *v);
	PFNGLMULTITEXCOORD1SARBPROC	glMultiTexCoord1sARB; //GLenum target, GLshort s);
	PFNGLMULTITEXCOORD1SVARBPROC glMultiTexCoord1svARB	; //GLenum target, const GLshort *v);

	PFNGLMULTITEXCOORD2DARBPROC	 glMultiTexCoord2dARB; //GLenum target, GLdouble s, GLdouble t);
	PFNGLMULTITEXCOORD2DVARBPROC glMultiTexCoord2dvARB	; //GLenum target, const GLdouble *v);
	PFNGLMULTITEXCOORD2FARBPROC	 glMultiTexCoord2fARB; //GLenum target, GLfloat s, GLfloat t);
*/	PFNGLMULTITEXCOORD2FVARBPROC glMultiTexCoord2fvARB	; //GLenum target, const GLfloat *v);
/*	PFNGLMULTITEXCOORD2IARBPROC	 glMultiTexCoord2iARB; //GLenum target, GLint s, GLint t);
	PFNGLMULTITEXCOORD2IVARBPROC glMultiTexCoord2ivARB	; //GLenum target, const GLint *v);
	PFNGLMULTITEXCOORD2SARBPROC	 glMultiTexCoord2sARB; //GLenum target, GLshort s, GLshort t);
	PFNGLMULTITEXCOORD2SVARBPROC glMultiTexCoord2svARB	; //GLenum target, const GLshort *v);

	PFNGLMULTITEXCOORD3DARBPROC	 glMultiTexCoord3dARB; //GLenum target, GLdouble s, GLdouble t, GLdouble r);
	PFNGLMULTITEXCOORD3DVARBPROC glMultiTexCoord3dvARB	; //GLenum target, const GLdouble *v);
	PFNGLMULTITEXCOORD3FARBPROC	 glMultiTexCoord3fARB; //GLenum target, GLfloat s, GLfloat t, GLfloat r);
*/	PFNGLMULTITEXCOORD3FVARBPROC glMultiTexCoord3fvARB	; //GLenum target, const GLfloat *v);
/*	PFNGLMULTITEXCOORD3IARBPROC	 glMultiTexCoord3iARB; //GLenum target, GLint s, GLint t, GLint r);
	PFNGLMULTITEXCOORD3IVARBPROC glMultiTexCoord3ivARB	; //GLenum target, const GLint *v);
	PFNGLMULTITEXCOORD3SARBPROC	 glMultiTexCoord3sARB; //GLenum target, GLshort s, GLshort t, GLshort r);
	PFNGLMULTITEXCOORD3SVARBPROC glMultiTexCoord3svARB	; //GLenum target, const GLshort *v);

	PFNGLMULTITEXCOORD4DARBPROC	 glMultiTexCoord4dARB; //GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
	PFNGLMULTITEXCOORD4DVARBPROC glMultiTexCoord4dvARB	; //GLenum target, const GLdouble *v);
	PFNGLMULTITEXCOORD4FARBPROC	 glMultiTexCoord4fARB; //GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
	PFNGLMULTITEXCOORD4FVARBPROC glMultiTexCoord4fvARB	; //GLenum target, const GLfloat *v);
	PFNGLMULTITEXCOORD4IARBPROC	 glMultiTexCoord4iARB; //GLenum target, GLint s, GLint t, GLint r, GLint q);
	PFNGLMULTITEXCOORD4IVARBPROC glMultiTexCoord4ivARB	; //GLenum target, const GLint *v);
	PFNGLMULTITEXCOORD4SARBPROC	 glMultiTexCoord4sARB; //GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
	PFNGLMULTITEXCOORD4SVARBPROC glMultiTexCoord4svARB	; //GLenum target, const GLshort *v);
*/	
	PFNGLACTIVETEXTUREARBPROC	glActiveTextureARB; //GLenum target);
	PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB; //GLenum target);


	// ! draw 3D polygon with vertex coordinates 
	int DrawPolygon3D(
					int vertexFlags,
					const float *v,	// f3 vertex coordinates
					const float *vc,
					const float *vn,
					int nvp, const float * vp[],	//Array of texture coordinates
					const long *coordIndex,int numCoordIndex,
					const Plane &plane		// plane of polygon 
				);


    // get functions pointers for extensions
    int InitializeExtensions();

	//! get a 2D renderer to current state
	virtual GRenderer2D* getRenderer2D();

	// 2 D Renderer interface 
	void initRenderer2D();
	void termRenderer2D();

	gbool inPolygon;
	gbool inContour;

	//RGBAColor edgeColor;
	RGBAColor fillColor;

	// def _G_GLU_TESSELATOR_DYN

	GLUtriangulatorObj * tess2D;

	// function entrys 
	GLUtriangulatorObj*  (APIENTRY *_gluNewTess)(void);

	void (APIENTRY *_gluDeleteTess)(GLUtriangulatorObj *tess );

	void (APIENTRY  *_gluTessCallback)(     
		GLUtriangulatorObj       *tess,
		GLenum              which, 
		void                (GLCALLBACK *fn)());
	
	void  (APIENTRY *_gluTessVertex)(       
		GLUtriangulatorObj       *tess,
		GLdouble            coords[3], 
		void                *data );

	void (APIENTRY  *_gluTessNormal)(       
		GLUtriangulatorObj       *tess, 
		GLdouble            x,
		GLdouble            y, 
		GLdouble            z );


	void (APIENTRY *_gluBeginPolygon)( GLUtriangulatorObj *tess );
	void (APIENTRY *_gluNextContour)(  GLUtriangulatorObj *tess, GLenum        type );
	void (APIENTRY *_gluEndPolygon)(   GLUtriangulatorObj *tess );
	void (APIENTRY  *_gluTessBeginContour)(GLUtriangulatorObj       *tess );
	void (APIENTRY  *_gluTessEndContour)(GLUtriangulatorObj       *tess );


	//!render 2D polyline,
	virtual void DrawPolyline(int np, const Point *pts);

	//!render 2D polygon
	virtual void DrawPolygon(int np, const Point *pts);

	//!render 2D points
	virtual void DrawPoint(int np, const Point *pts);

	//!render 2D grid 
	virtual void DrawGrid(const Point &pmin,const Point &pmax,int xsteps,int ysteps,gbool lines=gtrue);


	//!begin a new contour within the current polygon 
	virtual void BeginContour();
	//!end the current contour within the current polygon 
	virtual void EndContour();

	//!begin a new polygon 
	virtual void BeginPolygon();

	//!end the current open polygon 
	virtual void EndPolygon();

	// attributes
	// set the edge color
	virtual void SetEdgeColor(const RGBAColor &color);
	//virtual void SetColor(const RGBAColor &color);


	virtual void SetDrawStyle(int style);

	//! set the point size 
	//virtual void SetPointSize(float radius);





};




inline void glVertex(const Point &p) { glVertex3fv(&p.x); }
inline void glNormal(const Point &p) { glNormal3fv(&p.x); }
inline void glColor(const Point &p)  { glColor3fv(&p.x); }
inline void glTexCoord(const Point &p) { glTexCoord3fv(&p.x); }


#endif


#endif

