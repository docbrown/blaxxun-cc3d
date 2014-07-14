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

@module GShellTriang.cpp - GLView core graphical primitves |

Copyright (c) 1995 - 1999 by Holger Grahn
All rights reserved

Purpose:
Implementation of graphical primitives GShell, GMesh ...
																	  
Classes:

Notes:

Changes:


  added vertex interolation for GLU tesselator

$Revision: 1.5 $
$Log: gshelltriang.cpp,v $
Revision 1.5  1999/11/29 20:30:14  holger
empty message

Revision 1.3  1999/05/05 14:37:45  tom
empty message

Revision 1.17  1998/08/20 17:22:12  holger
empty message



Todo :

******************************************************************************/

#include "stdafx.h"

#include "gversion.h"


//#include <math.h>
//#include <string.h>
//#include <stdlib.h>
//#include "point.h"
//#include "glh.h"

#include "gutils.h"

#include "greporter.h"

#include "gclass.h"
#include "gshell.h"

#include "gshelli.h" 
#include "gshelltriang.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//



#ifdef _OGL
#define _G_GLU_TESSELATOR
#endif


#ifndef _OGL
//triangulate a facelist into triangles using
extern int triangulate_tesselateFaceList( GFaceList &f, //@parm input facelist
						PointArray &v, //@parm corresponding vertices for indices in f
						IntArray *vmap, // @parm optional vertex map list
						GFaceList &triangles, //@parm output: triangulated facelist
						IntArray *new2old,	//@parm optional mapping from new triangles to old face indices
                        GShell * theShell  //@parm optional shell to append vertices too
					  );
#endif

#ifdef _D3D

#define _G_GLU_TESSELATOR
#define _G_GLU_TESSELATOR_DYN

#undef GLenum
#undef GLint
#undef GLdouble
#undef GLint

#include <GL/gl.h>
#include <GL/glu.h>


#endif




// ********* Tesselation 

#ifdef _G_GLU_TESSELATOR

class GTriangulateGLU {
public :


	static GLUtriangulatorObj *tobj;

	// temporary statics needed for GLU tesselation callback functions

	static int tesselateV[3];
	static int tesselateVi;	// max index in tesselateV
	static int tesselateFi; // face index
	static int tesselateMode;

	static GFaceList *tesselateF;
	static PointArray *tesselateP;

	static GShell *tesselateShell; // to append a vertex


	static IntArray *tesselateNew2Old;

	// glu callbacks 
	static void __stdcall tesselateBegin(GLenum mode); 
	static void  __stdcall tesselateVertex3dv(const GLdouble *v);
	static void  __stdcall tesselateEnd();
	static void  __stdcall tesselateEdgeFlag (GLboolean flag);
	static void  __stdcall tesselateError (GLenum error);
	// new calls 
	static void __stdcall tesselateCombine(GLdouble coords[3],
                                    void* verts[4], 
                                    GLfloat w[4],
                                    int* dataOut );
	
	//triangulate a facelist into triangles using the GLU triangulator
	static int tesselateFaceList( GFaceList &f, //@parm input facelist
						PointArray &v, //@parm corresponding vertices for indices in f
						IntArray *vmap, // @parm optional vertex map list
						GFaceList &newf, //@parm output: triangulated facelist
						IntArray *new2old,	//@parm optional mapping from new triangles to old face indices
                        GShell * theShell  //@parm optional shell to append vertices too
					  );

// GLU_VERSION_1_2


	// def _G_GLU_TESSELATOR_DYN

	// function entrys 
	static GLUtesselator*  (APIENTRY *_gluNewTess)(void);
	static void (APIENTRY *_gluDeleteTess)(GLUtesselator *tess );


/*
   GLUAPI void APIENTRY gluTessCallback( GLUtriangulatorObj *tobj,
                                      GLenum which,
                                      void (CALLBACK *fn)() );

// From Mesa 1.1
 */
#define G_GLU_CALLBACK CALLBACK
#define G_GLU_CALLBACK_T void (CALLBACK *)()

	//#define G_GLU_CALLBACK_T void (CALLBACK *)(...)

    static void (APIENTRY  *_gluTessCallback)(
                 GLUtesselator       *tess,
                 GLenum              which,
                 void                (CALLBACK *fn)(
					// Mesa ...
				 )
               );

	
	static void  (APIENTRY *_gluTessVertex)(       
		GLUtesselator       *tess,
		GLdouble            coords[3], 
		void                *data );

	static void (APIENTRY  *_gluTessNormal)(       
		GLUtesselator       *tess, 
		GLdouble            x,
		GLdouble            y, 
		GLdouble            z );


	static void   (APIENTRY *_gluBeginPolygon)( GLUtesselator *tess );

	static void   (APIENTRY *_gluNextContour)(  GLUtesselator *tess, 
                                 GLenum        type );

	static void   (APIENTRY *_gluEndPolygon)(   GLUtesselator *tess );

	
	static gbool gluInit;
	static gbool gluLoaded;

#ifdef _WIN32
	static HINSTANCE gluLib;


	static FARPROC get(const char *entry) {
		FARPROC p= GetProcAddress(gluLib,entry);
		ASSERT(p != NULL);
		if (p == NULL) gluLoaded = gfalse;
		return p;

	}

	static gbool LoadGLU() 
	{
		gluLib = LoadLibrary("glu32.dll"); // LoadLibrary
		gluInit = gtrue;
		if (gluLib == NULL) {
			gluLoaded = gfalse;
			TRACE("Can't load library 'glu32.dll'");
			return FALSE;
		}
		gluLoaded = gtrue;
		* ((FARPROC*) &_gluNewTess) = get("gluNewTess");
		* ((FARPROC*) &_gluDeleteTess) = get("gluDeleteTess");
		* ((FARPROC*) &_gluTessCallback) = get("gluTessCallback");
		* ((FARPROC*) &_gluBeginPolygon) = get("gluBeginPolygon");
		* ((FARPROC*) &_gluNextContour) = get("gluNextContour");
		* ((FARPROC*) &_gluEndPolygon) = get("gluEndPolygon");

		* ((FARPROC*) &_gluTessVertex) = get("gluTessVertex");
		* ((FARPROC*) &_gluTessNormal) = get("gluTessNormal");

		return gluLoaded;


	}

#endif

};

// the hole code 

int GShell_GLUTriangulationHoleCode=GLU_INTERIOR;


// temporary statics needed for GLU tesselation callback functions

GLUtriangulatorObj * GTriangulateGLU::tobj=NULL;

 int GTriangulateGLU::tesselateV[3];
 int GTriangulateGLU::tesselateVi;	// max index in tesselateV
 int GTriangulateGLU::tesselateFi; // face index
 int GTriangulateGLU::tesselateMode;

 GFaceList *GTriangulateGLU::tesselateF=NULL;
 PointArray *GTriangulateGLU::tesselateP=NULL;

 GShell *GTriangulateGLU::tesselateShell=NULL; // to append a vertex


 IntArray *GTriangulateGLU::tesselateNew2Old;


// dynamic globals

gbool GTriangulateGLU::gluInit=gfalse;
gbool GTriangulateGLU::gluLoaded=gfalse;

#ifdef _WIN32
HINSTANCE GTriangulateGLU::gluLib=NULL;

#endif

	// function entrys 
GLUtesselator*  (APIENTRY *GTriangulateGLU::_gluNewTess)(void);

void (APIENTRY *GTriangulateGLU::_gluDeleteTess)(GLUtesselator *tess );

void (APIENTRY  *GTriangulateGLU::_gluTessCallback)(     
    GLUtesselator       *tess,
    GLenum              which, 
    void                (CALLBACK *fn)());
	
void  (APIENTRY *GTriangulateGLU::_gluTessVertex)(       
		GLUtesselator       *tess,
		GLdouble            coords[3], 
		void                *data );

void (APIENTRY  *GTriangulateGLU::_gluTessNormal)(       
		GLUtesselator       *tess, 
		GLdouble            x,
		GLdouble            y, 
		GLdouble            z );


void   (APIENTRY *GTriangulateGLU::_gluBeginPolygon)( GLUtesselator *tess );

void   (APIENTRY *GTriangulateGLU::_gluNextContour)(  GLUtesselator *tess, 
                                 GLenum        type );

void   (APIENTRY *GTriangulateGLU::_gluEndPolygon)(   GLUtesselator *tess );




// actual GLU call back functions 

void __stdcall GTriangulateGLU::tesselateBegin(GLenum mode) 
{
  tesselateVi=0;
  tesselateMode = mode;
}


void  __stdcall GTriangulateGLU::tesselateVertex3dv(const GLdouble *v)
{ 
  int vi = (int) v;

  switch (tesselateMode) {
  case GL_TRIANGLE_FAN:
  		tesselateV[tesselateVi++]=vi;
		if (tesselateVi == 3) {
		   tesselateF->AppendTri(tesselateV[0],tesselateV[1],tesselateV[2]);
  	       if (tesselateNew2Old) tesselateNew2Old->Append(tesselateFi);
		  
		  tesselateV[1]=tesselateV[2];
		  tesselateVi=2;
		}
  		break;

  case GL_TRIANGLE_STRIP:
  		tesselateV[tesselateVi++]=vi;
		if (tesselateVi == 3) {
		  tesselateF->AppendTri(tesselateV[0],tesselateV[1],tesselateV[2]);
  	       if (tesselateNew2Old) tesselateNew2Old->Append(tesselateFi);
		  tesselateV[0]=tesselateV[1];
		  tesselateV[1]=tesselateV[2];
		  tesselateVi=2;
		}
  		TRACE("tesselate bad mode \n");
		break;
  case GL_TRIANGLES:
  		tesselateV[tesselateVi++]=vi;
		if (tesselateVi == 3) {
		  tesselateF->AppendTri(tesselateV[0],tesselateV[1],tesselateV[2]);
  	      if (tesselateNew2Old) tesselateNew2Old->Append(tesselateFi);
		  tesselateVi=0;
		}
  		break;
  }
  
}

void  __stdcall GTriangulateGLU::tesselateEnd() {
}

void  __stdcall GTriangulateGLU::tesselateEdgeFlag (GLboolean flag)
{
}

void  __stdcall GTriangulateGLU::tesselateError (GLenum error)
{
#ifdef _OGL
	const char* msg = (const char *)gluErrorString(error);
	TRACE("Tesselate error : %s\n",msg);
#endif

}

// new calls 
void __stdcall GTriangulateGLU::tesselateCombine(GLdouble coords[3],
                                    void* verts[4], 
                                    GLfloat w[4],
                                    int* dataOut )
{

	Point newP(coords[0],coords[1],coords[2]);
	int newI;
    if (tesselateShell) {

#if 0
        newI = tesselateShell->AppendVertex(newP,0);
#else 
		GVertex vnew;
        // interpolate 
		vnew.v=newP;

		const int *d = (int *) &verts[0];

		// lookup offsets 
		GVertexI vi[4];
		for (int i=0; i<4;i++) tesselateShell->GetVertexI(d[i],vi[i]);

		// compute new vertex attributes
		if (tesselateShell->vn.Length()>0) {
			   const PointArray &vn=tesselateShell->vn;	
		       vnew.n = w[0]*vn[vi[0].n] + w[1]*vn[vi[1].n] + w[2]*vn[vi[2].n] + w[3]*vn[vi[3].n];
			   vnew.n.Normalize();
		}

		if (tesselateShell->vp.Length()>0) {
			   const PointArray &vp=tesselateShell->vp;	
		       vnew.p = w[0]*vp[vi[0].p] + w[1]*vp[vi[1].p] + w[2]*vp[vi[2].p] + w[3]*vp[vi[3].p];
		}
		if (tesselateShell->vc.Length()>0) {
			   const PointArray &vc=tesselateShell->vc;	
		       vnew.c = w[0]*vc[vi[0].c] + w[1]*vc[vi[1].c] + w[2]*vc[vi[2].c] + w[3]*vc[vi[3].c];
		}

		newI=tesselateShell->AppendVertex(vnew);
#endif

    } else {
        newI = tesselateP->Length();
	    tesselateP->Append(newP);
    }

	*dataOut = newI; // thats the new index

	
	// TRACE("Tesselate combine new vertex %d \n",newI);


#if 0
		    void myCombine( GLdouble coords[3],	int *d[4],


				    GLfloat w[4], VERTEX **dataOut )


		    {


		       VERTEX *new = new_vertex();

		       new->x =	coords[0];
		       new->y =	coords[1];
		       new->z =	coords[2];
		       new->r =	w[0]*d[0]->r + w[1]*d[1]->r + w[2]*d[2]->r
				+ w[3]*d[3]->r;


		       new->g =	w[0]*d[0]->g + w[1]*d[1]->g + w[2]*d[2]->g
				+ w[3]*d[3]->g;

		       new->b =	w[0]*d[0]->b + w[1]*d[1]->b + w[2]*d[2]->b
				+ w[3]*d[3]->b;

		       new->a =	w[0]*d[0]->a + w[1]*d[1]->a + w[2]*d[2]->a
				+ w[3]*d[3]->a;
		       *dataOut	= new;


#endif


}

/*
typedef void (CALLBACK* GLUtessBeginDataProc)    (GLenum, void *);
typedef void (CALLBACK* GLUtessEdgeFlagDataProc) (GLboolean, void *);
typedef void (CALLBACK* GLUtessVertexDataProc)   (void *, void *);
typedef void (CALLBACK* GLUtessEndDataProc)      (void *);
typedef void (CALLBACK* GLUtessErrorDataProc)    (GLenum, void *);
typedef void (CALLBACK* GLUtessCombineDataProc)  (GLdouble[3],
                                                  void*[4], 
                                                  GLfloat[4],
                                                  void**,
                                                  void* );



void           APIENTRY  gluTessProperty(     GLUtesselator *tess,
                                              GLenum        which, 
                                              GLdouble      value );
 
void           APIENTRY  gluTessNormal(       GLUtesselator *tess, 
                                              GLdouble      x,
                                              GLdouble      y, 
                                              GLdouble      z );


void           APIENTRY  gluGetTessProperty(  GLUtesselator *tess,
                                              GLenum        which, 
                                              GLdouble      *value );

*/





//triangulate a facelist into triangles using the GLU triangulator
int GTriangulateGLU::tesselateFaceList( 
						GFaceList &f, //@parm input facelist
						PointArray &v, //@parm corresponding vertices for indices in f
						IntArray *vmap, // @parm optional vertex map list
						GFaceList &newf, //@parm output: triangulated facelist
						IntArray *new2old,	//@parm optional mapping from new triangles to old face indices
                        GShell * theShell  //@parm optional shell to append vertices too
					  )
{
  //static GLUtriangulatorObj *tobj = NULL;

  if (tobj == NULL) {
#ifdef _OGL
	_gluNewTess = gluNewTess;
	_gluDeleteTess = gluDeleteTess;

	_gluTessCallback = gluTessCallback;
	_gluBeginPolygon = gluBeginPolygon;
	_gluNextContour = gluNextContour;
	_gluEndPolygon = gluEndPolygon;
	_gluTessVertex = gluTessVertex;

#ifdef GLU_VERSION_1_2
	_gluTessNormal = gluTessNormal;
#endif


#endif

    tobj = (*_gluNewTess)();  /* create and initialize a GLU
                             polygon tesselation object */
    (*_gluTessCallback)(tobj, (GLenum) GLU_BEGIN,	(G_GLU_CALLBACK_T)tesselateBegin);
    (*_gluTessCallback)(tobj, (GLenum) GLU_VERTEX,  (G_GLU_CALLBACK_T)tesselateVertex3dv); 
    (*_gluTessCallback)(tobj, (GLenum) GLU_END,		(G_GLU_CALLBACK_T)tesselateEnd);
    (*_gluTessCallback)(tobj, (GLenum) GLU_EDGE_FLAG, (G_GLU_CALLBACK_T)tesselateEdgeFlag);
    (*_gluTessCallback)(tobj, (GLenum) GLU_ERROR,	(G_GLU_CALLBACK_T)tesselateError);

#ifdef GLU_VERSION_1_2
    (*_gluTessCallback)(tobj, (GLenum) GLU_TESS_COMBINE, (G_GLU_CALLBACK_T)tesselateCombine);
#endif

  }

  GLdouble vertex[3];

  const int *fp = f;
  const int *fpend=fp+f.Length();
  int fi=-1;

  int beginPolygon=0;
  
  int ftricnt= f.GetTriangleCount();
  
  tesselateF= &newf;
  tesselateP = &v; // combine might add new vertices !!!!
  tesselateShell = theShell;

  
  tesselateF->SetLength(0);
  
  tesselateF->SetMaxLength(ftricnt*(4+1));

  tesselateNew2Old =new2old;
  if (new2old) {
     new2old->SetLength(0);
     new2old->SetMaxLength(ftricnt);
  } 
	 

  // for each face / face hole 
  while ( fp < fpend ) {
	 int n = *fp++;

	 if (n>0)  {
	    if (beginPolygon)  (*_gluEndPolygon)(tobj);

		// face counter
		fi++;
		tesselateFi = fi;
		
		if (n <= 2) { // ignore edges
		   fp += n;
		   beginPolygon=0;
		   continue;
		}
	    (*_gluBeginPolygon)(tobj);
		beginPolygon=1;
	 }
	 else { 
	     //(*_gluNextContour(tobj,GLU_INTERIOR);
	     (*_gluNextContour)(tobj,(GLenum) GShell_GLUTriangulationHoleCode);
		 n= - n;
	 }
	 int vold = -1;
	 int voldmapped;
	 while (n>0) {
	  int vi= *fp++;
	  if (vi>=v.Length()) {
	  }
	  int vimapped = vi;
	  if (vmap) vimapped = (*vmap)[vi];

	  vertex[0]= v[vimapped].x,vertex[1]= v[vimapped].y,vertex[2]= v[vimapped].z;

//	  if ((vold >=0) && (v[vi]== v[vold])) {
	  if ((vold >=0) && (v[vimapped] == v[voldmapped])) {  // 15.12.95
	  	// resulting from sweep code to get "hard" edges
	    // TRACE("identical vertex %d %d\n",vold,vi); 
	  } else 
	    (*_gluTessVertex)(tobj,vertex,(void *)vi);
	  vold = vi;
	  voldmapped = vimapped;
	  n--;
	 }
  }
  if (beginPolygon)  (*_gluEndPolygon)(tobj);

  // TRACE("triangulation gives %d triangles\n",newf.Length() / 4);
  return(0);
}

#if 0

int tesselateFace(GLfloat data[][2], unsigned int dataSize,
  GLdouble thickness, GLuint side, GLuint edge, GLuint whole)
{
  static GLUtriangulatorObj *tobj = NULL;
  GLdouble vertex[3], dx, dy, len;

  if (tobj == NULL) {
    tobj = gluNewTess();  /* create and initialize a GLU
                             polygon * * tesselation object */
    gluTessCallback(tobj, GLU_BEGIN, glBegin);
    gluTessCallback(tobj, GLU_VERTEX, glVertex2fv);  /* semi-tricky 
                                                      */
    gluTessCallback(tobj, GLU_END, glEnd);
  }
  
  gluBeginPolygon(tobj);
  for (i = 0; i < count; i++) {
    vertex[0] = data[i][0];
    vertex[1] = data[i][1];
    vertex[2] = 0;
    gluTessVertex(tobj, vertex, data[i]);
  }
  gluEndPolygon(tobj);
}

#endif

#endif // _OGL

#ifndef _OGL
	//#define _MESA_TESS
#endif



#ifdef _MESA_TESS

#undef GLint 
#undef GLenum
#undef GLfloat 
#undef GLdouble

#include "mesa/tess.h"
// temporary statics needed for GLU tesselation callback functions

static int tesselateV[3];
static int tesselateVi;	// max index in tesselateV
static int tesselateFi; // face index
static int tesselateMode;

static GFaceList *tesselateF;
static PointArray *tesselateP;

static GShell *tesselateShell; // to append a vertex


static IntArray *tesselateNew2Old;


void /*__stdcall*/ tesselateBegin(GLenum mode) 
{
  tesselateVi=0;
  tesselateMode = mode;
}


void  /*__stdcall*/ tesselateVertex3dv(const GLdouble *v)
{ int vi = (int) v;

  switch (tesselateMode) {
  case GL_TRIANGLE_FAN:
  		tesselateV[tesselateVi++]=vi;
		if (tesselateVi == 3) {
		   tesselateF->AppendTri(tesselateV[0],tesselateV[1],tesselateV[2]);
  	       if (tesselateNew2Old) tesselateNew2Old->Append(tesselateFi);
		  
		  tesselateV[1]=tesselateV[2];
		  tesselateVi=2;
		}
  		break;

  case GL_TRIANGLE_STRIP:
  		tesselateV[tesselateVi++]=vi;
		if (tesselateVi == 3) {
		  tesselateF->AppendTri(tesselateV[0],tesselateV[1],tesselateV[2]);
  	       if (tesselateNew2Old) tesselateNew2Old->Append(tesselateFi);
		  tesselateV[0]=tesselateV[1];
		  tesselateV[1]=tesselateV[2];
		  tesselateVi=2;
		}
  		TRACE("tesselate bad mode \n");
		break;
  case GL_TRIANGLES:
  		tesselateV[tesselateVi++]=vi;
		if (tesselateVi == 3) {
		  tesselateF->AppendTri(tesselateV[0],tesselateV[1],tesselateV[2]);
  	      if (tesselateNew2Old) tesselateNew2Old->Append(tesselateFi);
		  tesselateVi=0;
		}
  		break;
  }
  
}

void  /*__stdcall*/ tesselateEnd() {
}

void  /*__stdcall*/ tesselateEdgeFlag (GLboolean flag)
{
}

void  /*__stdcall*/ tesselateError (GLenum error)
{
	//const char* msg = (const char *)gluErrorString(error);
//	TRACE("Tesselate error : %s\n",msg);
	TRACE("Tesselate error : %d\n",error);
}
// new calls 
void /*__stdcall*/ tesselateCombineXXXXX need to updated (GLdouble coords[3],
                                    void* verts[4], 
                                    GLfloat w[4],
                                    int* dataOut )
{

	Point newP(coords[0],coords[1],coords[2]);
	int newI;
    if (tesselateShell) {
        // to do: interpolate 
        newI = tesselateShell->AppendVertex(newP,0);


    } else {
        newI = tesselateP->Length();
	    tesselateP->Append(newP);
    }

	*dataOut = newI;

	
	// TRACE("Tesselate combine new vertex %d \n",newI);


#if 0
		    void myCombine( GLdouble coords[3],	int *d[4],


				    GLfloat w[4], VERTEX **dataOut )


		    {


		       VERTEX *new = new_vertex();





		       new->x =	coords[0];
		       new->y =	coords[1];
		       new->z =	coords[2];
		       new->r =	w[0]*d[0]->r + w[1]*d[1]->r + w[2]*d[2]->r
				+ w[3]*d[3]->r;


		       new->g =	w[0]*d[0]->g + w[1]*d[1]->g + w[2]*d[2]->g
				+ w[3]*d[3]->g;

		       new->b =	w[0]*d[0]->b + w[1]*d[1]->b + w[2]*d[2]->b
				+ w[3]*d[3]->b;

		       new->a =	w[0]*d[0]->a + w[1]*d[1]->a + w[2]*d[2]->a
				+ w[3]*d[3]->a;
		       *dataOut	= new;


#endif


}

//triangulate a facelist into triangles using the GLU triangulator
int mesa_tesselateFaceList( GFaceList &f, //@parm input facelist
						PointArray &v, //@parm corresponding vertices for indices in f
						IntArray *vmap, // @parm optional vertex map list
						GFaceList &newf, //@parm output: triangulated facelist
						IntArray *new2old,	//@parm optional mapping from new triangles to old face indices
                        GShell * theShell  //@parm optional shell to append vertices too
					  )
{
  // static GLUtriangulatorObj *tobj = NULL;
  GLUtriangulatorObj obj;

  GLUtriangulatorObj *tobj = &obj;
  tobj->contours=tobj->last_contour=NULL;
  //init_callbacks(&tobj->callbacks);
  tobj->error=GLU_NO_ERROR;
  tobj->current_polygon=NULL;
  tobj->contour_cnt=0;

  {
    //tobj = gluNewTess();  
							/* create and initialize a GLU
                             polygon tesselation object */
    gluTessCallback(tobj, GLU_BEGIN, (void(__stdcall *)(void))tesselateBegin);
    gluTessCallback(tobj, GLU_VERTEX, (void(__stdcall *)(void))tesselateVertex3dv); 
    gluTessCallback(tobj, GLU_END, (void(__stdcall *)(void))tesselateEnd);
    gluTessCallback(tobj, GLU_EDGE_FLAG, (void(__stdcall *)(void))tesselateEdgeFlag);
    gluTessCallback(tobj, GLU_ERROR, (void(__stdcall *)(void))tesselateError);

	// gluTessCallback(tobj, GLU_TESS_COMBINE, (void(__stdcall *)(void))tesselateCombine);
  }

  GLdouble vertex[3];

  const int *fp = f;
  const int *fpend=fp+f.Length();
  int fi=-1;

  int beginPolygon=0;
  
  int ftricnt= f.GetTriangleCount();
  
  tesselateF= &newf;
  tesselateP = &v; // combine might add new vertices !!!!
  tesselateShell = theShell;

  
  tesselateF->SetLength(0);
  
  tesselateF->SetMaxLength(ftricnt*(4+1));

  tesselateNew2Old =new2old;
  if (new2old) {
     new2old->SetLength(0);
     new2old->SetMaxLength(ftricnt);
  } 
	 

  // for each face / face hole 
  while ( fp < fpend ) {
	 int n = *fp++;

	 if (n>0)  {
	    if (beginPolygon)  gluEndPolygon(tobj);

		// face counter
		fi++;
		tesselateFi = fi;
		
		if (n <= 2) { // ignore edges
		   fp += n;
		   beginPolygon=0;
		   continue;
		}
	    gluBeginPolygon(tobj);
		beginPolygon=1;
	 }
	 else { 
	     //gluNextContour(tobj,GLU_INTERIOR);
	     gluNextContour(tobj,GLU_UNKNOWN /* GShell_GLUTriangulationHoleCode*/ );
		 n= - n;
	 }
	 int vold = -1;
	 int voldmapped;
	 while (n>0) {
	  int vi= *fp++;
	  if (vi>=v.Length()) {
	  }
	  int vimapped = vi;
	  if (vmap) vimapped = (*vmap)[vi];

	  vertex[0]= v[vimapped].x,vertex[1]= v[vimapped].y,vertex[2]= v[vimapped].z;

//	  if ((vold >=0) && (v[vi]== v[vold])) {
	  if ((vold >=0) && (v[vimapped] == v[voldmapped])) {  // 15.12.95
	  	// resulting from sweep code to get "hard" edges
	    // TRACE("identical vertex %d %d\n",vold,vi); 
	  } else 
	    gluTessVertex(tobj,vertex,(void *)vi);
	  vold = vi;
	  voldmapped = vimapped;
	  n--;
	 }
  }
  if (beginPolygon)  gluEndPolygon(tobj);

  // TRACE("triangulation gives %d triangles\n",newf.Length() / 4);
  return(0);
}

#if 0

int tesselateFace(GLfloat data[][2], unsigned int dataSize,
  GLdouble thickness, GLuint side, GLuint edge, GLuint whole)
{
  static GLUtriangulatorObj *tobj = NULL;
  GLdouble vertex[3], dx, dy, len;

  if (tobj == NULL) {
    tobj = gluNewTess();  /* create and initialize a GLU
                             polygon * * tesselation object */
    gluTessCallback(tobj, GLU_BEGIN, glBegin);
    gluTessCallback(tobj, GLU_VERTEX, glVertex2fv);  /* semi-tricky 
                                                      */
    gluTessCallback(tobj, GLU_END, glEnd);
  }
  
  gluBeginPolygon(tobj);
  for (i = 0; i < count; i++) {
    vertex[0] = data[i][0];
    vertex[1] = data[i][1];
    vertex[2] = 0;
    gluTessVertex(tobj, vertex, data[i]);
  }
  gluEndPolygon(tobj);
}


#endif
#endif // mesa triangulator




//triangulate a facelist into triangles 
int tesselateFaceList( 
						GFaceList &f, //@parm input facelist
						PointArray &v, //@parm corresponding vertices for indices in f
						IntArray *vmap, // @parm optional vertex map list
						GFaceList &newf, //@parm output: triangulated facelist
						IntArray *new2old,	//@parm optional mapping from new triangles to old face indices
                        GShell * theShell  //@parm optional shell to append vertices too
					  )

{

 
#ifdef _OGL
	return GTriangulateGLU::tesselateFaceList(f,v,vmap,newf,new2old,theShell); 
#else

	
	 if (!GTriangulateGLU::gluInit)
		 GTriangulateGLU::LoadGLU();
	 if (GTriangulateGLU::gluLoaded)
		return GTriangulateGLU::tesselateFaceList(f,v,vmap,newf,new2old,theShell); 

	return triangulate_tesselateFaceList(f,v,vmap,newf,new2old,theShell); 

#endif

}
		









