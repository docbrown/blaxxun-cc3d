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

@module GLRender.cpp - OpenGL RenderState classes implementation |

Copyright (c) 1995 - 1999	by Holger Grahn
All rights reserved

Purpose:
see GLRender.h

Classes:
glMatrix
RGBAColor
glMaterial
glTextureParameters
glTextureGeneration
glFog
renderState

Notes:

Changes:

Todo :

******************************************************************************/

// implementation of glrender class
// glrender.cpp

					
#include "stdafx.h"

#ifdef _OGL


#include "gutils.h"
#include "glrender.h"
#include "glutils.h"

#include "matrix.h"

#include "gcamera.h"


// check for current gl error 
int glCheckError_(const char *file, int line)
{ 
   GLenum error;
   error = glGetError();
   if (error != GL_NO_ERROR) {
      const char *s = (const char *) gluErrorString(error);
      TRACE("(%s:%d)GL Error : %s (%d)  \n",file,line,s, error);
      return(1);
   }
   return(0);
}


//
// glTextureParametersr
//

glTextureParameters::glTextureParameters() 
{
	min_filter = GL_NEAREST;
	mag_filter = GL_NEAREST;
	wrap_s = GL_REPEAT;
	wrap_t = GL_REPEAT;
	border_color.Gray(0.5);	// Gl Default is Black(); or track diffuse color ?
	gen_mipmaps=0;
	set_diffuse_white = 1;
	max_size_x=1024,max_size_y=1024;
	current_max_size_x = 1024, current_max_size_y= 1024;
	min_size_x=1,min_size_y=1; // ok ? 
	// will be update by GetLimits
	square_size = TRUE;
	border = TRUE; 
	supported_image_fmts = (1<<IM_RGBA) | (1<<IM_RGB) | (1<<IM_GRAYA) | (1<<IM_GRAY) ; // to be initialized depending on texture formats

	blend = GL_DECAL;
	defaultBlend = GL_MODULATE;
	unlitBlend = GL_DECAL;
};


int glTextureParameters::SetMaxSize(int size_x,int size_y)
{	int iPow2;

	max_size_x = size_x;
	
	iPow2 = log((double)size_x) / log((double)2.0);

    max_size_x = 1 << iPow2;

	max_size_y = size_y;
	iPow2 = log((double)size_y) / log((double)2.0);
    max_size_y = 1 << iPow2;

	current_max_size_x=max_size_x;
	current_max_size_y=max_size_y;

	return(1);
}

int glTextureParameters::SetCurrentMaxSize(int size_x,int size_y)
{	int iPow2;

	int max_size_x,max_size_y;

	max_size_x = size_x;
	
	iPow2 = log((double)size_x) / log((double)2.0);

    max_size_x = 1 << iPow2;

	max_size_y = size_y;
	iPow2 = log((double)size_y) / log((double)2.0);
    max_size_y = 1 << iPow2;

	current_max_size_x=min(this->max_size_x,max_size_x);
	current_max_size_y=min(this->max_size_y,max_size_y);


	return(1);
}

void glTextureParameters::GetMaxSize(int &size_x,int &size_y)
{ 
  size_x = max_size_x;size_y = max_size_y;
}

void glTextureParameters::GetCurrentMaxSize(int &size_x,int &size_y)
{ 
  size_x = current_max_size_x;size_y = current_max_size_y;
}

int glTextureParameters::SetMinSize(int size_x,int size_y)
{	int iPow2;

	min_size_x = size_x;
	
	iPow2 = log((double)size_x) / log((double)2.0);

    min_size_x = 1 << iPow2;

	min_size_y = size_y;
	iPow2 = log((double)size_y) / log((double)2.0);
    min_size_y = 1 << iPow2;

	return(1);
}

void glTextureParameters::GetMinSize(int &size_x,int &size_y)
{ 
  size_x = min_size_x;size_y = min_size_y;
}


void glTextureGeneration::glSet()
{
		if (isOff) { glDisable_(); return; }

		if (enable[S]) {
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,pname[S]);
			glTexGenfv(GL_S,ptype[S],params[S]);
			glEnable(GL_TEXTURE_GEN_S);
		} else glDisable(GL_TEXTURE_GEN_S);

		if (enable[T]) {
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,pname[T]);
			glTexGenfv(GL_T,ptype[T],params[T]);
			glEnable(GL_TEXTURE_GEN_T);
	    } else glDisable(GL_TEXTURE_GEN_T);

		if (enable[R]) {
			glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,pname[R]);
			glTexGenfv(GL_R,ptype[R],params[R]);
			glEnable(GL_TEXTURE_GEN_R);
		 } else glDisable(GL_TEXTURE_GEN_R);
		if (enable[Q]) {
			glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,pname[Q]);
			glTexGenfv(GL_Q,ptype[Q],params[Q]);
			glEnable(GL_TEXTURE_GEN_Q);
		 } else glDisable(GL_TEXTURE_GEN_Q);
}

   // Set by string to some predefined modes
int glTextureGeneration::Set(const char *type)
  { int ret=0;
     strcpy(parameter,type);
	 if (streq(type,"sphere")) 	SetSphere();
	 else if (streq(type,"xyz")) SetPlane(0,0, 0,1,2);
	 else if (streq(type,"yzx")) SetPlane(0,0, 1,2,0);
	 else if (streq(type,"xzy")) SetPlane(0,0, 0,2,1);
	 else if (streq(type,"eye xyz")) SetPlane(1,1, 0,1,2);
	 else if (streq(type,"eye yzx")) SetPlane(1,1, 1,2,0);
	 else if (streq(type,"eye xzy")) SetPlane(1,1, 0,2,1);
	 else if (streq(type,"off")) SetOff();
	 else if (streq(type,"none")) SetOff();
	 else {
		SetOff();
		ret = -1;
		strcpy(parameter,"<invalid>");
	 }
	 return(ret);
}

   // Eval a planar mapping
float glTextureGeneration::EvalPlane(int i, Point &p,float q)  {
			return(params[i][0]*p.x + params[i][1]*p.y +params[i][2]*p.z+params[i][3]*q);
}

// Evaluate float
float glFog::Eval(float z) {
	   float f= (end - z) /  ( end-start);
	   if (mode == GL_EXP) f = exp(-density * z);
	   else if (mode == GL_EXP2) f = exp(sqr(-density * z));
//		   f = clamp(f,1.0,0.0);
	   return f;
	   // return f= blend( f,c, (1-f),color);
}



const IS glTextureParameters::filters[] = {
			{-1,"<Invalid>"},
			{GL_NEAREST,"Nearest"},
			{GL_LINEAR,"Linear"},
			{GL_NEAREST_MIPMAP_NEAREST,"Mipmap_Nearest_Nearest"},
			{GL_LINEAR_MIPMAP_NEAREST,"Mipmap_Linear_Nearest"},
			{GL_NEAREST_MIPMAP_LINEAR,"Mipmap_Nearest_Linear"},
			{GL_LINEAR_MIPMAP_LINEAR,"Mipmap_Linear_Linear"},
			{0,NULL},
			};

//glTexEnv
const IS glTextureParameters::blends[] = {
			{-1,"<Invalid>"},
			{GL_DECAL,"Decal"},
			{GL_MODULATE,"Modulate"},
			{GL_BLEND,"Blend"},
			{GL_REPLACE,"Replace"},	 // ??
			{GL_DECAL,"Copy"},
			{0,NULL},
			};


const char * GlFilterLookup(int v)
{ return Lookup(glTextureParameters::filters,v); }

int GlFilterLookup(const char *v) 
{ return Lookup(glTextureParameters::filters,v); }

const IS glFog::modes [] = {
			{-1,"<Invalid>"},
			{GL_LINEAR,"linear"},
			{GL_EXP,"exp"},
			{GL_EXP2,"exp2"},
// vrml types
			{GL_LINEAR,"LINEAR"},
			{GL_EXP,"EXPONENTIAL"},
			{GL_EXP2,"EXPONENTIAL2"},

			{0,NULL},
			};

const char * GFogModeLookup(int v)
{ return Lookup(glFog::modes,v); }

int GFogModeLookup(const char *v)
{ return Lookup(glFog::modes,v); }

static const IS GL_RenderModesS [] = {
			{-1,"<Invalid>"},
			{ GRenderVertices,"Vertices" },
			{ GRenderBoundingBoxes,"BoundingBoxes" },
			{ GRenderWireframe,"Wireframe" },
			{ GRenderSolid,"Solid" },
			{ GRenderNonLighted,"NonLighted" },
			{ GRenderFlat,"Flat" },
			{ GRenderGouraud,"Gouraud" },

            // aliases lowercase 
			{ GRenderVertices,"vertices" },
			{ GRenderBoundingBoxes,"boundingboxes" },
			{ GRenderWireframe,"wireframe" },
			{ GRenderSolid,"solid" },
			{ GRenderNonLighted,"nonlighted" },
			{ GRenderFlat,"flat" },
			{ GRenderGouraud,"gouraud" },

			{0,NULL},
			};


const char * GRenderModeLookup(int v)
{ for (int i=0; GL_RenderModesS[i].s != NULL; i++)
      if (GL_RenderModesS[i].i == v) return(GL_RenderModesS[i].s);
  return 0;
}

int GRenderModeLookup(const char *v)
{ for (int i=0; GL_RenderModesS[i].s != NULL; i++)
      if (streq(GL_RenderModesS[i].s,v)) return(GL_RenderModesS[i].i);
  return -1;
}

static const IS GL_DrawStyleS [] = {
			{-1,"<Invalid>"},
			{ GAsDefault,"default" },
			{ GAsSphere,"sphere" },
			{ GAsCube,"cube" },
			{ GAsCylinder,"cylinder" },
			{ GAsCone,"cone" },
			{ GAsDCone,"dcone" },
			{ GAsDisc,"disc" },
			{0,NULL},
			};

const char * GDrawStyleLookup(int v)
{ for (int i=0; GL_DrawStyleS[i].s != NULL; i++)
      if (GL_DrawStyleS[i].i == v) return(GL_DrawStyleS[i].s);
  return 0;
}

int GDrawStyleLookup(const char *v)
{ for (int i=0; GL_DrawStyleS[i].s != NULL; i++)
      if (streq(GL_DrawStyleS[i].s,v)) return(GL_DrawStyleS[i].i);
  return -1;
}

//
// RGBAColor
//


// Read/Write Material to File

int RGBAColor::io(AStream &a,int fmt)
{
 if (fmt == 1)	{ a.io(r); a.io(g); a.io(b); }
 else { a.Sep('('); a.io(r); a.io(g); a.io(b); a.Sep(')'); }
 return(a.stat);
}

#if 0
Example
#
Inventor V1.0 ascii

Material {
    ambientColor	0.0706087 0.0212897 0.0336154
    diffuseColor	0.282435 0.0851587 0.134462
    specularColor	0.276305 0.11431 0.139857
    emissiveColor	0 0 0
    shininess	0.127273
    transparency	0
}
	RGBAColor ambient;
	RGBAColor diffuse;
	RGBAColor specular;
	RGBAColor emission;

	float shininess;
	float alpha;
#endif


//
// glMaterial
//



// Read Material
//

int glMaterial::io(AStream &a)
{  int ret;

   char tok[256];
   a.Sep('{');
   if (a.isInput()) {
   while ( (a.Skip() != '}'))  {
	   a.r(tok);	strlwr(tok);
	   if (streq(tok,"ambientcolor")) 	{	ret = ambient.io(a,1); }
	   else if (streq(tok,"diffusecolor")) {ret = diffuse.io(a,1); }
	   else if (streq(tok,"specularcolor")) {ret = specular.io(a,1); }
	   else if (streq(tok,"emissivecolor")) {ret = emission.io(a,1); }
	   else if (streq(tok,"shininess")) { float r; ret=a.io(r);
										      shininess=r*128.0;
									     }
	   else if (streq(tok,"alpha") ||
				streq(tok,"transparency")) { float r; ret=a.io(r);
										      SetAlpha(r);
								}
	   else {
			a.Error("Bad keyword in reading material ");
			break;
			}
   }
   } else
   {
		// TBD
   }
   a.Sep('}');
   return(a.stat);
}

#if 0
int glMaterial::io(AStream &a)
{  int ret;

   char tok[256];
   a.Sep('{');
   if (a.isInput()) {
   while ( (a.Skip() != '}'))  {
	   a.r(tok);	strlwr(tok);
	   if (streq(tok,"ambientcolor")) 	{	ret = ambient.io(a,1); }
	   else if (streq(tok,"diffusecolor")) {ret = diffuse.io(a,1); }
	   else if (streq(tok,"specularcolor")) {ret = specular.io(a,1); }
	   else if (streq(tok,"emissivecolor")) {ret = emission.io(a,1); }
	   else if (streq(tok,"shininess")) { float r; ret=a.io(r);
										      shininess=r*128.0;
									     }
	   else if (streq(tok,"alpha") ||
				streq(tok,"transparency")) { float r; ret=a.io(r);
										      SetAlpha(r);
								}
	   else {
			a.error("Bad keyword in reading material ");
			break;
			}
   }
   } else
   {
		// TBD
   }
   a.Sep('}');
   return(a.stat);
}
#endif

int glMaterial::ReadFromInventorFile(const char *FileName)
{ int ret;

  AStream a(FileName,"r");

  a.comment_char = '#';
  ret = 0;

  if (!a.atEof()) {
	    a.Skip();
		char tok[256];
		a.r(tok);	strlwr(tok);
		if (streq(tok,"material")) {
			ret=io(a);
		}
  }
  return(ret);
}

//



// GExtendedDrawStyle
void GExtendedDrawStyle::RenderVertex(Point &p,Point &n) {
			glPushMatrix();
			glTranslatef(p.x,p.y,p.z);
			glRender();
			glPopMatrix();
			
}

void GExtendedDrawStyle::RenderNormal(Point &p,Point &n) {
			glPushMatrix();
			glTranslatef(p.x,p.y,p.z);
			glRender();
			glPopMatrix();
}

void GExtendedDrawStyle::RenderEdge(Point &p1,Point &n1,Point &p2,Point &n2) {
			Point d = p2-p1;
			Point v1(0,0,1);
			Point v2 = Normalize(d);
			
			Point v = CrossProd(v1,v2);
			double cosw = DotProd(v1,v2);
  			double sinw = v.Len();
  			v *= 1.0/sinw;	// normalize v
  			
  			double angle = atan2(cosw,sinw);
			angle*=Rad2Deg;
			glPushMatrix();
					glScalef(1.0,1.0,d.Len());
					
					glRotatef(angle,v.x,v.y,v.z);
					glTranslatef(p1.x,p1.y,p1.z);

					glRender();
		
			glPopMatrix();
}

// Setup object to be rendered
void GExtendedDrawStyle::glSetup() {

			if (lighted) {
			       normals = smooth ? GLU_SMOOTH : GLU_FLAT;
			}
			else normals = GL_NONE; 

			if (type == GAsDefault) return;
			
			GLUquadricObj * q = gluNewQuadric();
			
			gluQuadricNormals(q,(GLenum) normals);
			gluQuadricTexture(q,textured);
			gluQuadricOrientation(q,(GLenum)GLU_OUTSIDE);
			gluQuadricDrawStyle(q,(GLenum)drawStyle);

			cache.Begin();
			switch(type) {
			case GAsCylinder :
			     gluCylinder(q,scale1,scale1,1.0,xsteps,2); break;
			case GAsCone :
			     gluCylinder(q,scale1,scale2,1.0,xsteps,2); break;
			case GAsDCone :
			     gluCylinder(q,scale2,scale1,0.5,xsteps,2); 
				 glTranslatef(0.0,0.0,0.5);
			     gluCylinder(q,scale1,scale2,0.5,xsteps,2); 
			     break;
			case GAsDisc :
			     gluDisk(q,scale2,scale1,xsteps,2); break;
			case GAsSphere :
			     gluSphere(q,scale1,xsteps,ysteps); break;
			case GAsCube :
			     gluSphere(q,scale1,4,3); break;
			}
			cache.End();

			if (q) gluDeleteQuadric(q);
}
//
// class RenderState
//

RenderState::RenderState()
{
		
	// disable for now, not working yet
	doTextureDownscale = gfalse;

	// GRenderer2D :: tess2d
	tess2D = NULL;
	inPolygon = inContour = gfalse;

	//
	nurbsMode = GGL_NURBS_SOFTWARE;

	maxMmatrixStack=0;
	maxTmatrixStack=0;
	maxLights=0;


	canCull	= gtrue;
	canDoRgbLighting = gtrue;
	canDoFog= gtrue;
	canDoAlphaBlend = gtrue;
	canDoMipMap = gtrue;
	canDoAlphaStipple = gfalse;
	canDoAlphaTest = gtrue;
	maxMipfilter = GL_LINEAR_MIPMAP_LINEAR;
	fogEnabled = gfalse;

	
	textures = NULL;
	currentTexture = NULL;
	textureMatrixIdentity = gtrue;

	zbuffer = FALSE; 
	zwrite = TRUE;

	depthFunc = GL_LESS; 

	dither = TRUE; 
	sbuffer=0;
	renderMode = GRenderGouraud;
	LightMode = 0;
	
	//unlit =FALSE;
	litMaterial = TRUE;

	vertices.visible = 0;
	normals.visible =0;
	bboxes.visible =0;
	
	lighting=FALSE;
	lightmodel = GL_SMOOTH;
	localColors=0;
	localColorsMode = GL_DIFFUSE; // GL_AMBIENT_AND_DIFFUSE;

	backfaceCulling = 0;
	cullFront = 0;
	frontFaceCW=0;
	texturing=0;
	textureDefined = 0;
	twosidedLighting=0;
	localViewer=gtrue; // new 22.10.98
//	ambient.Gray(0.2);
	ambient.Gray(0.3); // 1.03.97
	
	ambient.Gray(0.0); // 22.10.99, light as ambient color too !

	lineSmooth=0;
	computePrimitiveList=0;

	blend = FALSE;
	blendFromMaterial = FALSE;

	blendSrcFunc = GL_ONE;
	blendDestFunc = GL_ZERO;

	alphaTest = FALSE;
	alphaTestFromTexture=FALSE;
	alphaFunc = GL_ALWAYS;
	alphaRef = 0.0;
	alpha = 1.0;

	normalize = FALSE;
	lightScale = 1.0;
    
    textureEnvMode = GL_DECAL;
	
	vertexColor.Set(1.0,1.0,0.0);
	edgeColor.White();
	faceColor.Gray(0.5);
	normalColor.Set(1.0,0.0,0.0);
	bboxColor.Set(0.0,1.0,0.0);
	
	doAbortCheck=0;
	AbortCheckerData = 0;
	AbortChecker=0;

	vertexDrawStyle = 0;
	edgeDrawStyle = 0;
	normalDrawStyle = 0;
	bboxDrawStyle = 0;
	
	pointSize = 1.0;
    lineWidth= 1.0;
    lineStipple = 0;
    lineFactor = 1;
    linePattern = (unsigned short) -1;


    perspectiveCorrectionHint=GL_NICEST; // GL_FASTEST; // new ?? GL_DONT_CARE; GL_FASTEST 

    pointSmoothHint=GL_DONT_CARE;
    lineSmoothHint=GL_DONT_CARE;
    polygonSmoothHint=GL_DONT_CARE;

    //fogHint=GL_DONT_CARE;
	//fogEnabled = 0;
	
	bufferMode = 0; // swapbuffers
	FlushCurrent();

	useVertexArray = gfalse;

	/* EXT_vertex_array */
	useVertexArrayEXT = 0;
	hasVertexArrayEXT = 0;

	// GL_EXT_compiled_vertex_array
	hasCompiledVertexArrayEXT=0;
	useCompiledVertexArrayEXT=0;

	/* WIN_swap_hint */
	useWinSwapHintEXT=0;
	hasWinSwapHintEXT=0;
	
	hasTobjEXT = 0;	
  	useTobjEXT = 0;	

	/* GL_EXT_texture_env_add */
	has_texture_env_add_EXT=0;	
  	use_texture_env_add_EXT=0;	

/* GL_EXT_texture_env_combine */
	has_texture_env_combine_EXT=0;	
  	use_texture_env_combine_EXT=0;	


	/* ARB_multitexture */
  	hasARB_multitextureEXT = 0;	
  	useARB_multitextureEXT = 0;
	maxEnabledTexUnit = 0;
	maxTexUnit = 0;




	// 2 D Renderer interface 
	initRenderer2D();


};

RenderState::~RenderState()
{
	// to do: drawstyle
  	DeleteUnusedTextures();  // DeleteTextures();
	ASSERT(textures == NULL); // if this fires an external app has handle to texture


   // 2 D Renderer interface 
   termRenderer2D();

}



// set camera as current GL projection matrix 
void RenderState::SetCamera( GCamera *camera) 
{
    camera->glSet();
    // could save projection & view matrix  here

   	modelMatrix.glGet(GL_MODELVIEW_MATRIX);
   	projectionMatrix.glGet(GL_PROJECTION_MATRIX);

}



// set all cached glstate parameters 
void RenderState::SetContextAll() 
{
         SetZBuffer();
         SetZWrite();
         SetPointSize();
         SetLineWidth();
         SetLineStipple();

         SetNormalize();
         SetLighting();
         SetLightModel();
         SetTwoSidedLighting();
         SetLocalViewer();
         SetMaterial();
         SetBackgroundColor();
         SetFog();
         SetAmbientColor();
         SetLocalColors();
         SetBackfaceCulling();
         SetFrontFaceCW();
         SetTexturing();
         
		 // SetTexturingDiffuseColor ????

         SetLineAntialiasing();
         // ??SetTextureTransform();
         SetTextureMatrixIdentity();

    	 SetTextureBlend();

    	 glHint(GL_PERSPECTIVE_CORRECTION_HINT,perspectiveCorrectionHint);
    	 glHint(GL_POINT_SMOOTH_HINT,pointSmoothHint);
    	 glHint(GL_LINE_SMOOTH_HINT,lineSmoothHint);
    	 glHint(GL_POLYGON_SMOOTH_HINT,polygonSmoothHint);
		 
		 // SetLocalViewer : glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1); // new 22.10.98
    	 // glHint(GL_FOG_HINT,fogHint);
}


// set global render mode to mode (enum)
int RenderState::SetRenderMode(GRenderMode newMode)
{
  switch (newMode)  {
  case GRenderWireframe:
	faces.visible = 0;
	edges.visible = 1;

	edges.lighted = LightMode;
	edges.smooth = LightMode || edges.colored;
	edges.useMaterialColor = 1;

	vertices.visible = 0;
	bboxes.visible = 0;

	SetZBuffer(LightMode);  /* lighted edges are better with z-buffering */
	SetLighting(LightMode);
	SetLitMaterial(LightMode);
	if (edges.smooth)
	   SetLightModel(GL_SMOOTH);
	else SetLightModel(GL_FLAT);

	break;

  case GRenderVertices:
	faces.visible = 0;
	edges.visible = 0;
	bboxes.visible = 0;
	vertices.visible = 1;
	vertices.lighted = LightMode;

	SetZBuffer(0);
	if (vertexDrawStyle && vertexDrawStyle->enabled) {
	 	SetZBuffer(1);
	}
	SetLighting(LightMode);
	SetLitMaterial(LightMode);

	break;

  case GRenderBoundingBoxes:
	faces.visible = 0;
	edges.visible = 0;
	vertices.visible = 0;
	bboxes.visible = 1;

	SetZBuffer(0);
	if (bboxDrawStyle && bboxDrawStyle->enabled) {
	 	SetZBuffer(1);
		SetLighting(LightMode);
		SetLitMaterial(LightMode);
	} else {
	  SetLighting(0);
	  SetLitMaterial(0);
    }
	break;

  case GRenderSolid:
	faces.visible = 1;
	faces.lighted = 0;
	edges.visible = 1;
	edges.lighted = 0;
	edges.useMaterialColor = 0;
	vertices.visible = 0;
	bboxes.visible = 0;

	SetZBuffer(1);
	SetLighting(0);
	SetLitMaterial(0);
//	theRenderState.SetLightModel(GL_FLAT);
	SetTextureBlend(GL_DECAL);
	// theRenderState.sbuffer = 1; /* use stencil planes */
	break;
  case GRenderNonLighted:
	faces.visible = 1;
	faces.lighted = 0;
	edges.visible = 0;
	edges.lighted = 0;
	edges.useMaterialColor = 0;
	vertices.visible = 0;
	bboxes.visible = 0;
	SetZBuffer(1);
	SetLighting(0);
	SetLitMaterial(0);
	SetTextureBlend(GL_DECAL);
	break;

  case GRenderFlat:
	faces.visible = 1;
	faces.lighted = 1;
	faces.smooth = 0;
	edges.visible = 0;
	vertices.visible = 0;
	bboxes.visible = 0;
//	SetLightModel(GL_FLAT);
	SetZBuffer(1);
	SetLighting(1);
	SetLitMaterial(1);
	SetTextureBlend(GL_MODULATE);
	break;
  case GRenderGouraud :
	faces.visible = 1;
	faces.lighted = 1;
	faces.smooth = 1;
	edges.visible = 0;
	vertices.visible = 0;
	bboxes.visible = 0;
	SetLightModel(GL_SMOOTH);
	SetZBuffer(1);
	SetLighting(1);
	SetLitMaterial(1);
	SetTextureBlend(GL_MODULATE);
	break;

 }
 renderMode = newMode;
 return(0);
}



int RenderState::SetRenderFlags(RenderFlags &flags)
{
  	if (!flags.visible) return(0);
  	SetLighting(flags.lighted && litMaterial);
	SetTexturing(flags.textured && textureDefined);
	SetLightModel(flags.smooth ? GL_SMOOTH : GL_FLAT);
	SetLocalColors(flags.colored);
	return(1);

}

// Set current color, also change material color
void RenderState::SetColor(const RGBAColor &c) {
	  	const GLenum face = GL_FRONT_AND_BACK;

		currentColor = c;

		if (lighting) {
		   //glMaterialfv(face,GL_AMBIENT,&c.r);
		   glMaterialfv(face,GL_DIFFUSE,&currentColor.r);
		}
		glColor(currentColor);

}

// Set Rendering Attributes to vertex
int RenderState::SetVertexAttributes() 
{
	if (SetRenderFlags(vertices)) {
	   if (!vertices.useMaterialColor) SetColor(vertexColor);
	   return(1);
	}
	return(0);
}

// Set Rendering Attributes to edge
int RenderState::SetEdgeAttributes() 
{
	if (SetRenderFlags(edges)) {
	   if (!edges.useMaterialColor) SetColor(edgeColor);
	   return(1);
	}
	return(0);
}

// Set Rendering Attributes to face
int RenderState::SetFaceAttributes() 
{
	if (SetRenderFlags(faces)) {
	   if (!faces.useMaterialColor) SetColor(faceColor);
	   return(1);
	}
	return(0);
}
// Set Rendering Attributes to normal
int RenderState::SetNormalAttributes() 
{
	if (SetRenderFlags(normals)) {
	   if (!normals.useMaterialColor) SetColor(normalColor);
	   return(1);
	}
	return(0);
}

int RenderState::SetBBoxAttributes() 
{
	if (SetRenderFlags(bboxes)) {
	   if (!bboxes.useMaterialColor) SetColor(bboxColor);
	   return(1);
	}
	return(0);
}

	 // get GL limits of interest for our renderer
void RenderState::GetLimits() {
	 GLint i;

	 glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH,&i); maxMmatrixStack=i;
	 glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH,&i); maxTmatrixStack=i;
	 glGetIntegerv(GL_MAX_LIGHTS,&i); maxLights=i;

     GLint glMaxTexDimX;
     GLint glMaxTexDimY;
     glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDimX);
     glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDimY);
	 textureParameters.SetMaxSize(glMaxTexDimX,glMaxTexDimY);

};

void RenderState::Clear() {
	glClear (GL_COLOR_BUFFER_BIT | (zbuffer ? GL_DEPTH_BUFFER_BIT : 0));

};

// clear buffers
void RenderState::Clear(gbool colorBuffer,gbool zBuffer,gbool stencilBuffer,gbool accumBuffer) 
{
	glClear (  (colorBuffer ? GL_COLOR_BUFFER_BIT : 0) 
			 | (zBuffer ? GL_DEPTH_BUFFER_BIT : 0)
			 | (stencilBuffer ? GL_STENCIL_BUFFER_BIT : 0)
			 | (accumBuffer ? GL_ACCUM_BUFFER_BIT : 0)
			);
}


// set current viewport
void RenderState::SetViewport(int x,int y,int width, int height) 
{
	
	if (x==0 && y==0 && width == sizex && height == sizey) {
		SetViewport();
		return;
	}
	glViewport(x,y,width,height); // lower-left corner of the viewport rectangle, in pixels
	glEnable(GL_SCISSOR_TEST);
	glScissor(x,y,width,height); //lower-left corner 
}

void RenderState::SetViewport() 
{
	int x=0,y=0,width,height;
	
	//GLint v[4];
	//glGetIntegerv(GL_MAX_VIEWPORT_DIMS,v);  // is 2048
	//width=v[0],height=v[1];
	
	width = sizex, height = sizey;


	glViewport(x,y,width,height); // left top
	glScissor(x,y,width,height);
	glDisable(GL_SCISSOR_TEST);
}
 
// get current viewport
void RenderState::GetViewport(int &x,int &y,int &width, int &height) 
{
	GLint v[4];
	glGetIntegerv(GL_VIEWPORT,v); // GL_MAX_VIEWPORT_DIMS 
	x=v[0],y=v[1],width=v[2],height=v[3];
}

 

void RenderState::SetBufferMode(int mode)
{
	
	bufferMode = mode;
	switch (mode) {
	case 0:
		/* Draw into the back buffer */
		SetRenderSize(sizex,sizey);
		glDrawBuffer(GL_BACK);
		break;
	case 1:
		/* Draw into the front buffer */
		SetRenderSize(sizex,sizey);
		glDrawBuffer(GL_FRONT);
		break;
	case 2:
		/* Draw into the back buffer */
		glDrawBuffer(GL_BACK);
		//glViewport(0, 0, renderSizex, renderSizey);
		SetRenderSize(sizex/2,sizey/2);
		break;
	}
}

int RenderState::SwapBuffers()
{
	switch (bufferMode) {
	case 0:
		{
#ifdef _WGL
    	HDC hdc = ::wglGetCurrentDC();
		::SwapBuffers(hdc);
#else

#pragma message("To DO swap ");

#endif
		}
		return(1);

		break;
	case 1:
		break;
	case 2:
		{
		gbool saveZBuffer = zbuffer;
		gbool saveLighting = lighting;

		/* Copy from the back buffer to the front buffer */
		glDrawBuffer(GL_FRONT);
		glReadBuffer(GL_BACK);
		glViewport(0, 0, sizex, sizey);

		// HG Push state
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		/* Set an ortho viewport for the blit */
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, sizex, 0.0, sizey, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		SetZBuffer(0);
		SetLighting(0);
		glDisable(GL_DITHER);

		/* Copy the backbuffer viewport with a 2X zoom.
	 	* Don't forget to disable depth test, blending,lighting, and
	 	* other modes which slow down pixel operations!
	 	*/
		glRasterPos2i(0, 0);
		glPixelZoom(2.0, 2.0);
		glCopyPixels(0, 0, renderSizex, renderSizey, GL_COLOR);

		// HG Pop state
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		/* Draw into the back buffer */
		glDrawBuffer(GL_BACK);
		glViewport(0, 0, renderSizex, renderSizey);
		SetZBuffer(saveZBuffer);
		SetLighting(saveLighting);
		glEnable(GL_DITHER);
		}
		break;

	}
  return(0);
}

// copy back to front buffer // implementation dependent
int RenderState::UpdateFrontBuffer()
{
	switch (bufferMode) {
	case 1:
		break;
	case 0:
	case 2:
		{
		gbool saveZBuffer = zbuffer;
		gbool saveLighting = lighting;

		/* Copy from the back buffer to the front buffer */
		glDrawBuffer(GL_FRONT);
		glReadBuffer(GL_BACK);
		glViewport(0, 0, sizex, sizey);

		// HG Push state
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		/* Set an ortho viewport for the blit */
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, sizex, 0.0, sizey, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		SetZBuffer(0);
		SetLighting(0);
		//glDisable(GL_DITHER);

		/* Copy the backbuffer viewport with a 2X zoom.
	 	* Don't forget to disable depth test, blending,lighting, and
	 	* other modes which slow down pixel operations!
	 	*/
		glRasterPos2i(0, 0);
		if (bufferMode == 2) glPixelZoom(2.0, 2.0);
		else glPixelZoom(1.0, 1.0);
		
		glCopyPixels(0, 0, renderSizex, renderSizey, GL_COLOR);

		// HG Pop state
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glEnable(GL_DITHER);

		/* Draw into the back buffer */
		glDrawBuffer(GL_BACK);
		glViewport(0, 0, renderSizex, renderSizey);
		SetZBuffer(saveZBuffer);
		SetLighting(saveLighting);
		}
		break;

	}
  return(0);
}


void RenderState::SetSize(int sizeX,int sizeY)
{
	sizex = sizeX,sizey = sizeY;
	if (bufferMode == 2)
	   SetRenderSize(sizex / 2,sizey / 2);
	else SetRenderSize(sizex,sizey);
}

void RenderState::SetRenderSize(int sizeX,int sizeY)
{
	renderSizex = sizeX,renderSizey = sizeY;
	glViewport(0, 0, renderSizex, renderSizey);
}


// Query Windows GL extensions and intialize function pointers, if present
int RenderState::InitializeExtensions()
{
  const char * ext = (const char *)glGetString(GL_EXTENSIONS);

#ifdef _WGL

#ifdef GL_VERSION_1_1    

  {const char * sext = "GL_EXT_bgra";
    if (strstr(ext,sext)) {
  	     hasBgraEXT = 1;	
    }
  }
  {const char * sext = "GL_EXT_paletted_texture";
    if (strstr(ext,sext)) {
  	     hasPaletedTextureEXT = 1;	
         glColorTableEXT = (PFNGLCOLORTABLEEXTPROC)   wglGetProcAddress("glColorTableEXT");
         glColorSubTableEXT = (PFNGLCOLORSUBTABLEEXTPROC)   wglGetProcAddress("glColorSubTableEXT");
         glGetColorTableEXT = (PFNGLGETCOLORTABLEEXTPROC)   wglGetProcAddress("glGetColorTableEXT");
         glGetColorTableParameteriEXT = (PFNGLGETCOLORTABLEPARAMETERIVEXTPROC)   wglGetProcAddress("glGetColorTableParameterivEXT");
         glGetColorTableParameterfEXT = (PFNGLGETCOLORTABLEPARAMETERFVEXTPROC)   wglGetProcAddress("glGetColorTableParameterfvEXT");
    }
  }

#endif

  const char * vext = "GL_EXT_vertex_array";

  if (strstr(ext,vext)) {
  	 hasVertexArrayEXT = 1;	
  	 useVertexArrayEXT = 1;	// hg test 
     glArrayElementEXT = (PFNGLARRAYELEMENTEXTPROC) 	  wglGetProcAddress("glArrayElementEXT");
	 glDrawArraysEXT	= (PFNGLDRAWARRAYSEXTPROC		) wglGetProcAddress("glDrawArraysEXT");
	 glVertexPointerEXT	= (PFNGLVERTEXPOINTEREXTPROC	) wglGetProcAddress("glVertexPointerEXT");  
	 glNormalPointerEXT = (PFNGLNORMALPOINTEREXTPROC	) wglGetProcAddress("glNormalPointerEXT"); 
	 glColorPointerEXT  = (PFNGLCOLORPOINTEREXTPROC		) wglGetProcAddress("glColorPointerEXT"); 
	 glIndexPointerEXT  = (PFNGLINDEXPOINTEREXTPROC		) wglGetProcAddress("glIndexPointerEXT");  
	 glTexCoordPointerEXT=(PFNGLTEXCOORDPOINTEREXTPROC	) wglGetProcAddress("glTexCoordPointerEXT");
	 glEdgeFlagPointerEXT=(PFNGLEDGEFLAGPOINTEREXTPROC	) wglGetProcAddress("glEdgeFlagPointerEXT");
  	 glGetPointerVertexEXT=(PFNGLGETPOINTERVEXTPROC		) wglGetProcAddress("glGetPointerVertexEXT"); 
  } else {
#ifdef GL_VERSION_1_1    
//        glArrayElementEXT = glArrayElement;
#endif
  }
  const char * vcext = "GL_EXT_compiled_vertex_array";
  if (strstr(ext,vcext)) {
	     hasCompiledVertexArrayEXT=1;
		 useCompiledVertexArrayEXT=1;
  		 glLockArraysEXT = (glLockArrays_t)	 wglGetProcAddress("glLockArraysEXT");
  		 glUnlockArraysEXT = (glUnlockArrays_t)	 wglGetProcAddress("glUnlockArraysEXT");

  }
  


  const char * sext = "GL_WIN_swap_hint";
  if (strstr(ext,sext)) {
  	 hasWinSwapHintEXT = 1;	
     glAddSwapHintRectWin= (PFNGLADDSWAPHINTRECTWINPROC) wglGetProcAddress("glAddSwapHintRectWin");
  }
  
  {const char * sext = "GL_EXT_texture_object";
    if (strstr(ext,sext)) {
  	     hasTobjEXT = 1;	
  	     useTobjEXT = 1;	
		 GglTextureHandle::glAreTexturesResidentEXT= glAreTexturesResidentEXT = (glAreTexturesResident_t)	 wglGetProcAddress("glAreTexturesResidentEXT");
		 GglTextureHandle::glBindTextureEXT= glBindTextureEXT = (glBindTexture_t)	  wglGetProcAddress("glBindTextureEXT");
		 GglTextureHandle::glGenTexturesEXT= glGenTexturesEXT = (glGenTextures_t)	  wglGetProcAddress("glGenTexturesEXT");
		 GglTextureHandle::glDeleteTexturesEXT= glDeleteTexturesEXT = (glDeleteTextures_t)	  wglGetProcAddress("glDeleteTexturesEXT");
		 GglTextureHandle::glIsTextureEXT= glIsTextureEXT = (glIsTexture_t)		wglGetProcAddress("glIsTextureEXT");
		 GglTextureHandle::glPrioritizeTexturesEXT= glPrioritizeTexturesEXT = (glPrioritizeTextures_t)	 wglGetProcAddress("glPrioritizeTexturesEXT");
		 GglTextureHandle::hasTobjEXT = hasTobjEXT;

    }
	else {
		 // OGL 1.
  	     hasTobjEXT = 1;	
  	     useTobjEXT = 1;	
		 GglTextureHandle::glAreTexturesResidentEXT= glAreTexturesResidentEXT = glAreTexturesResident;
		 GglTextureHandle::glBindTextureEXT= glBindTextureEXT = glBindTexture;
		 GglTextureHandle::glGenTexturesEXT= glGenTexturesEXT = glGenTextures;
		 GglTextureHandle::glDeleteTexturesEXT= glDeleteTexturesEXT = glDeleteTextures;
		 GglTextureHandle::glIsTextureEXT= glIsTextureEXT = glIsTexture;
		 GglTextureHandle::glPrioritizeTexturesEXT= glPrioritizeTexturesEXT = glPrioritizeTextures;
		 GglTextureHandle::hasTobjEXT = hasTobjEXT;

	}
  }

  {const char * sext = "GL_EXT_texture_env_add";
  if (strstr(ext,sext)) {
  	 has_texture_env_add_EXT = 1;	
  	 use_texture_env_add_EXT = 1;	
  }
  }
  {const char * sext = "GL_EXT_texture_env_combine";
  if (strstr(ext,sext)) {
  	 has_texture_env_combine_EXT = 1;	
  	 use_texture_env_combine_EXT = 1;	
  }
  }
  const char * amext = "GL_ARB_multitexture";

  if (strstr(ext,amext)) {
  	 hasARB_multitextureEXT = 1;	
  	 useARB_multitextureEXT = 1;	// hg test 
     //glMultiTexCoord1dARB = (PFNGLMULTITEXCOORD1DARBPROC)  wglGetProcAddress("glMultiTexCoord1dARB");
     glMultiTexCoord2fvARB = (PFNGLMULTITEXCOORD2FVARBPROC)  wglGetProcAddress("glMultiTexCoord2fvARB");
     glMultiTexCoord3fvARB = (PFNGLMULTITEXCOORD3FVARBPROC)  wglGetProcAddress("glMultiTexCoord3fvARB");

	 glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
	 glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");
	 // to do :
	 GLint i;

	 glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&i); maxTexUnit=i;
  }

#else _WGL

#ifdef GL_VERSION_1_1    
	 useVertexArray = gtrue;

  	 hasVertexArrayEXT = 1;	
  	 useVertexArrayEXT = 1;	// hg test 
  	 hasVertexArrayEXT = 0;	
  	 useVertexArrayEXT = 0;	// Mesa 3.1 ?????????????
glArrayElementEXT =  (PFNGLARRAYELEMENTEXTPROC)	 
glArrayElement; 	 glDrawArraysEXT	=
(PFNGLDRAWARRAYSEXTPROC		) glDrawArrays; 	
glVertexPointerEXT	= (PFNGLVERTEXPOINTEREXTPROC	) glVertexPointer;  
	 glNormalPointerEXT = (PFNGLNORMALPOINTEREXTPROC	)
glNormalPointer;  	 glColorPointerEXT  = (PFNGLCOLORPOINTEREXTPROC
	) glColorPointer;  	 glIndexPointerEXT  =
(PFNGLINDEXPOINTEREXTPROC		) glIndexPointer;   	
glTexCoordPointerEXT=(PFNGLTEXCOORDPOINTEREXTPROC	) glTexCoordPointer;
	 glEdgeFlagPointerEXT=(PFNGLEDGEFLAGPOINTEREXTPROC	)
glEdgeFlagPointer;   	 glGetPointerVertexEXT=(PFNGLGETPOINTERVEXTPROC
	) glGetPointerv; // glGetPointerVertex; 

	     hasTobjEXT = 1;	
  	     useTobjEXT = 1;	
		 GglTextureHandle::glAreTexturesResidentEXT= glAreTexturesResidentEXT = (glAreTexturesResident_t)	glAreTexturesResident;
		 GglTextureHandle::glBindTextureEXT= glBindTextureEXT = (glBindTexture_t)	  glBindTexture;
		 GglTextureHandle::glGenTexturesEXT= glGenTexturesEXT = (glGenTextures_t)	  glGenTextures;
		 GglTextureHandle::glDeleteTexturesEXT= glDeleteTexturesEXT = (glDeleteTextures_t) glDeleteTextures;
		 GglTextureHandle::glIsTextureEXT= glIsTextureEXT = (glIsTexture_t)	glIsTexture;
		 GglTextureHandle::glPrioritizeTexturesEXT= glPrioritizeTexturesEXT = (glPrioritizeTextures_t) glPrioritizeTextures;
		 GglTextureHandle::hasTobjEXT = hasTobjEXT;


#endif

#endif _WGL


  return(0);
}

//
// glLight
//


void glLight::glSet(int light) 
{
	 	
	 	glLightfv((GLenum)light,GL_AMBIENT,ambient);
	 	glLightfv((GLenum)light,GL_DIFFUSE,diffuse);
	 	glLightfv((GLenum)light,GL_SPECULAR,specular);
		
		GLfloat v[4];
		v[0]=position.x,v[1]=position.y,v[2]=position.z;
		if (directional) v[3]= 0.0; else v[3]= 1.0; // positional
		glLightfv((GLenum)light,GL_POSITION,v);
		
		if (spot) {
			v[0]=direction.x,v[1]=direction.y,v[2]=direction.z;
			glLightfv((GLenum)light,GL_SPOT_DIRECTION,v);
			glLightf((GLenum)light,GL_SPOT_EXPONENT,SpotExponent);
			glLightf((GLenum)light,GL_SPOT_CUTOFF,SpotCutoff);
		 }
		 else glLighti((GLenum)light,GL_SPOT_CUTOFF,180);

		 glLightf((GLenum)light,GL_CONSTANT_ATTENUATION,Attenuation.x);
		 glLightf((GLenum)light,GL_LINEAR_ATTENUATION,Attenuation.y);
		 glLightf((GLenum)light,GL_QUADRATIC_ATTENUATION,Attenuation.z);
};


// primitive Rendering 
void RenderState::RenderVertex(Point &p){}
void RenderState::RenderLine(Point &p1,Point &p2){}
void RenderState::RenderBBox(Point &p1,Point &p2)
{
  RenderCubeEdges(p1,p2);
}
void RenderState::RenderCylinder(Point &p, float r, float h){}
void RenderState::RenderCone(Point &p, float r, float h){}
//	 void RenderState::RenderSphere(Point &p,float r){}



/* render a wireframe cube */
void RenderState::RenderCubeEdges(double x0, double y0,double z0,
				double x1,double y1,double z1)
{ gbool saveLighting = lighting;
  SetLighting(0);
  glBegin (GL_LINES);  /* vertical */
    glVertex3f (x0, y0, z0);  glVertex3f (x0, y0, z1);
    glVertex3f (x0, y1, z0);  glVertex3f (x0, y1, z1);
    glVertex3f (x1, y0, z0);  glVertex3f (x1, y0, z1);
    glVertex3f (x1, y1, z0);  glVertex3f (x1, y1, z1);
  glEnd ();

  glBegin (GL_LINE_LOOP);  /* bottom */
    glVertex3f (x0, y0, z0);
    glVertex3f (x0, y1, z0);
    glVertex3f (x1, y1, z0);
    glVertex3f (x1, y0, z0);
  glEnd ();
  glBegin (GL_LINE_LOOP);  /* top */
    glVertex3f (x0, y0, z1);
    glVertex3f (x0, y1, z1);
    glVertex3f (x1, y1, z1);
    glVertex3f (x1, y0, z1);
  glEnd ();
  SetLighting(saveLighting);
} 

void RenderState::RenderCubeEdges(const Point &p0,const Point &p1)
{
  RenderCubeEdges(p0.x,p0.y,p0.z,p1.x,p1.y,p1.z);
}

// create a new texture handle with a ref cnt of 1 
GglTextureHandle* RenderState::CreateTexture()
{
    GglTextureHandle *m = new GglTextureHandle();
    if (!textures) textures = m;
    else  {
      m->next = textures->next;
      m->prev =  textures;
      if (textures->next)
          textures->next->prev = m;
      textures->next = m;
    }
    m->refCnt = 1;
    return(m);
}

// delete an no longer used Texture
void RenderState::DeleteTexture(GTextureHandle *m)
{
    if (m == textures) {  // must be first
        textures= (GglTextureHandle*) m->Next();
        if (textures) textures->prev = NULL;
    } else {
      m->prev->next = m->next;
      if (m->next) m->next->prev = m->prev;
    }
    delete m;
}

// release all Texture handles
void RenderState::ReleaseTextures()
{
    GTextureHandle *i= textures;
    
    while(i != NULL) 
    {
        i->Release();
        i= i->Next();
    }
	currentTexture = NULL;
}

// release textures not rendered in the last frame
gbool RenderState::ReleaseUnrenderedTextures(int max)
{
	gbool ret = FALSE;
    GTextureHandle *i= textures;
    
    while(i != NULL) 
    {
		if (!i->rendered)
		{
			if (i->Ok() && i->CheckInVideoMem()) {
				i->Release();
				ret = TRUE;
				max--;
				if (max <0) return ret;
			}
			else i->Release();
		}
		i->rendered = FALSE;
        i= i->Next();
    }

	//hTex = 0;  
	currentTexture = NULL;

	return ret;
}

// release large textures 
gbool RenderState::ReleaseLargeTextures(int max)
{
	gbool ret = FALSE;

    GTextureHandle *i= textures;
    
    while(i != NULL) 
    {
		if ((i->sizeX > textureParameters.current_max_size_x)
		 || (i->sizeY > textureParameters.current_max_size_y))
		{
			if (i->Ok() && i->CheckInVideoMem()) {
				i->Release();
				ret = TRUE;
				max--;
				if (max <0) return ret;
			}
			else i->Release();
		}
	 

        i= i->Next();
    }

	//hTex = 0; 
	currentTexture = NULL;

	return ret;
}



// Delete all Texture handles
// should only be called if all references deleted !!!!!!!!!!!!1
void RenderState::DeleteTextures()
{
    GTextureHandle *i = textures,*next;
    
    while(i != NULL) 
    {
        next = i->Next();
        delete i;
        i = next;
    }
    textures = NULL;
	currentTexture = NULL;
}

// Delete all Texture handles not in use 
gbool RenderState::DeleteUnusedTextures()
{
    GTextureHandle *i= textures,*next;
	gbool ret = FALSE;
    
    
    while(i != NULL) 
    {
        next = i->Next();
	    if (i->refCnt==0) {
			if (i->Ok() && i->CheckInVideoMem()) {
				ret = TRUE;
			}

            DeleteTexture(i);
		}
        i = next;
    }
	return ret;
}

// update texture parameters
void RenderState::UpdateTextures()
{
    GglTextureHandle *i= textures;
    
    while(i != NULL) 
    {
		i->Update(textureParameters);
        i= i->Next();
    }
}

// texture memory overflow 
// if result TRUE, try to reload texture in progress
gbool RenderState::OnTextureMemoryOverflow(GTextureHandle &overflowHandle)
{	int sx=0,sy=0,bpp;
	
	TRACE("RenderState::OnTextureMemoryOverflow \n");

	if (DeleteUnusedTextures()) {
		TRACE("Found unused texture \n");
		return TRUE;
	}
	
	// reduce format ??
	
		
	GetMaxUsedTextureSize(sx,sy,bpp);

	sx = max((short)sx,overflowHandle.sizeX);
	sy = max((short)sy,overflowHandle.sizeY);

	if (ReleaseLargeTextures(2)) 
		return TRUE;

	if (!doTextureDownscale) 
		return FALSE;



	// reduce texture size 
	if ((sx>textureParameters.min_size_x) || (sy>textureParameters.min_size_y)) {
		int s=max(sx,sy);
		s = s/2;
		textureParameters.SetCurrentMaxSize(min(s,textureParameters.max_size_x),min(s,textureParameters.max_size_y));
		TRACE("New max texture size is %d %d \n",textureParameters.current_max_size_x,textureParameters.current_max_size_y);
		//if (overflowHandle.sizeX > textureParameters.current_max_size_x) return TRUE; // try with new size 
		if (ReleaseLargeTextures(1))
			return TRUE;
	}
//	allTexturesInVideo = FALSE;
	return FALSE;

}

gbool RenderState::GetMaxUsedTextureSize(int &sizeX,int &sizeY, int &bytesPerPix)
{
	gbool ret = FALSE;

    GglTextureHandle *i= textures;
    
    while(i != NULL) 
    {
		if (i->Ok()) {
			sizeX = max((short)sizeX,i->sizeX);
			sizeY = max((short)sizeY,i->sizeY);
			ret = TRUE;
		}
        i= i->Next();
    }

	return ret;
}



// set Texture properties
gbool RenderState::SetTexture(GTextureRef &ref,
							LPCTSTR fileName,
							LPCTSTR fileUrl,
							int format,gbool reload) 
{
	//ASSERT(ref.handle == NULL);; // HG diag
	ref.Release();

		if (ref.handle == NULL) 
		{
			// find if a texture with this file name exists 
		    GTextureHandle *i= textures;
	        GTextureHandle *found =  NULL;
	        GTextureHandle *firstFree =  NULL;
    
			while(i != NULL) 
			{
				if (i->MatchFileName(fileName)) {
					found = i;
					break;
				}
				if (i->refCnt == 0)
					firstFree = i;

				i = i->Next();
			}

			if (found) {
				ref.Release();

				ref.handle = found;
				found->Ref();	 // inc ref count 
				
				TRACE("Sharing texture %s \n",fileName);
				
				// downcast to GL specific version 
				GglTextureHandle *h = (GglTextureHandle *) ref.handle; // downcast 
				if (!h->Ok()) {
					reload = gtrue;
				}

				if (reload) {
					GglTextureHandle *h = (GglTextureHandle *) ref.handle; // downcast  
					if (h->Set(*this,fileName,format,fileUrl)) {
						SetTexture(h); // ??? D3D not
						return TRUE;
					}
					return FALSE;
				}
				SetTexture(h); // was uncommented 
				return(TRUE);
			}

			// create a new one 
			if (firstFree) {
				ref.handle = firstFree;
				firstFree->Ref(); 
			}
            else ref.handle = CreateTexture();
	  }

	  GglTextureHandle *h = (GglTextureHandle *) ref.handle; // downcast  

       // set Texture properties
	   if (h && h->Set(*this,fileName,format,fileUrl)) {
           SetTexture(h); // ??? D3D not
           return(TRUE);
       } else {
		   SetTexture(NULL);
		   return(FALSE);
	   }
}

// find if a texture with this URL already loaded 
gbool RenderState::FindTextureByUrl(GTextureRef &ref,LPCTSTR fileUrl)
{

	//ASSERT(ref.handle == NULL);; // HG diag
    //if (ref.handle == NULL) 

	{
			// find if a texture with this file name exists 
		    GTextureHandle *i= textures;
	        GTextureHandle *found =  NULL;
	        GTextureHandle *firstFree =  NULL;
    
			while(i != NULL) 
			{
				if (i->MatchFileUrl(fileUrl)) {
					found = i;
					break;
				} else
				
				if (i->refCnt == 0)
					firstFree = i;

				i = i->Next();
			}

			if (found) {
				ref.Release();
				ref.handle = found;
				found->Ref();	 // inc ref count 
				TRACE("Sharing texture by url %s \n",fileUrl);

				return(TRUE);
			}

	}
	return FALSE;
}


// set by image data 
gbool RenderState::SetTexture(GTextureRef &ref,int glFormat,int width,int height,int numComponents,unsigned char *data)
{
/*
	if (ref.handle) {
		ref.Release(); // unref old 
	}
*/
	// create a new one 
    if (!ref.handle)
	    ref.handle = CreateTexture();
	
	ASSERT(ref.handle->refCnt ==1);
	
	GglTextureHandle *h = (GglTextureHandle *) ref.handle; // downcast  


    // set Texture properties
	if (h && h->Set(*this,glFormat,width,height,numComponents,data)) {
        SetTexture(h);
        return(TRUE);
    } else {
	    SetTexture(NULL);
	    return(FALSE);
    }
}


// set texture by image structure 
gbool RenderState::SetTexture(GTextureRef &ref,image *im,gbool checkScale)
{

//	if (ref.handle) {
//		ref.Release(); // unref old 
//	}
	// create a new one 
    if (!ref.handle)
		ref.handle = CreateTexture();

	GglTextureHandle *h = (GglTextureHandle *) ref.handle; // downcast  

    // set Texture properties
	if (h && h->Set(*this,im,checkScale)) {
        // alreday set bei Set ?? not if not tobj extension
		SetTexture(h);
        return(TRUE);
    } else {
	    SetTexture(NULL);
	    return(FALSE);
    }
}



static int renderSphereVN = 1;
static int renderSphereFN = 0;

void glRenderSphereSub(int level,const Point &p1,const Point &p2,const Point &p3)
{

	if  (level<=0) { 	
		if (renderSphereFN) {
		   glNormal(NormCrossProd(p2-p1,p3-p1));
		}
 		if (renderSphereVN) glNormal(p1);
 		glVertex(p1);
 		if (renderSphereVN) glNormal(p2);
 		glVertex(p2);
 		if (renderSphereVN) glNormal(p3);
 		glVertex(p3);
		return;
	}

	Point p12((p1+p2)*0.5); p12.Normalize();
	Point p23((p2+p3)*0.5); p23.Normalize();
	Point p31((p3+p1)*0.5);	p31.Normalize();
	{
	  level--;
	  glRenderSphereSub(level,p1,p12,p31);
	  glRenderSphereSub(level,p12,p23,p31);
	  glRenderSphereSub(level,p31,p23,p3);
	  glRenderSphereSub(level,p12,p2,p23);
	}
}

void RenderState::RenderSphere(Point &center, float r,int level, int mask, int VN,int FN)
{
	renderSphereVN=VN;
	renderSphereFN=FN;

/*
   Icosahedron
   from Open Gl programming guide, pg. 56
*/
#define X .525731112119133606
#define Z .850650808352039932
//static Point icosahedron_pts[12]= { {-X,0.0,Z}, {X,0.0,Z},{-X,0.0,-Z}, {X,0.0,-Z},
//						  {0.0,Z,X}, {0.0,Z,-X},{0.0,-Z,X}, {0.0,-Z,-X},
//						  {Z,X,0.0}, {-Z,X,0.0},{Z,-X,0.0}, {-Z,-X,0.0} };

static point_ icosahedron_ptsp[12]= {
						  {-X,0.0,Z}, {X,0.0,Z},{-X,0.0,-Z}, {X,0.0,-Z},
						  {0.0,Z,X}, {0.0,Z,-X},{0.0,-Z,X}, {0.0,-Z,-X},
						  {Z,X,0.0}, {-Z,X,0.0},{Z,-X,0.0}, {-Z,-X,0.0}
						  };
static int icosahedron_connectivity[] = {
		3,0,4,1,  3,0,9,4,		3,9,5,4,  3,4,5,8,		3,4,8,1,
		3,8,10,1, 3,8,3,10, 3,5,3,8,  3,5,2,3,	3,2,7,3,
		3,7,10,3, 3,7,6,10, 3,7,11,6, 3,11,0,6, 3,0,1,6,
		3,6,1,10, 3,9,0,11, 3,9,11,2, 3,9,2,5,	3,7,2,11
		};

	Point *icosahedron_pts = (Point *) icosahedron_ptsp;
	glPushMatrix();
	glScalef(r,r,r);
	glTranslate(center);
	
	// normalize should be on
	glBegin(GL_TRIANGLES);
	const int *fp =icosahedron_connectivity; 
	const int *fpEnd =icosahedron_connectivity+ELEMENTS(icosahedron_connectivity);
	
	while (fp<fpEnd) {
		int n=*fp++;
		int i1=*fp++;
		int i2=*fp++;
		int i3=*fp++;

		glRenderSphereSub(level,icosahedron_pts[i1],icosahedron_pts[i2],icosahedron_pts[i3]);
	}
	glEnd();
	glPopMatrix();
}


//@class
// wrapper for gl pixel image functions

class glImage {
public :
	GLubyte *data;
	int sizeX, sizeY;
	GLenum format;
	GLenum type;
	// derived
	int bytesPerPixel;
	int bytesPerRow;
    

	glImage();

    void Set(GLubyte *newData,int newSizeX, int newSizeY,int newFormat = GL_RGB,int newType =GL_UNSIGNED_BYTE);
	int Scale(int newSizeX, int newSizeY);
	int ScaleNearest2Power();
	
	int SetScaled(glImage &src,int newSizeX, int newSizeY);

    int ComponentsPerPixel();
	int BytesPerComponent();

	void glRender(void);
	void glSetTexImage2d();

};

glImage::glImage() 
{
	data = NULL;
	Set(NULL,0,0);
}

void glImage::Set(GLubyte *newData,int newSizeX, int newSizeY,int newFormat,int newType)
{
  if (data) free(data);
  data=newData;
  sizeX = newSizeX,
  sizeY = newSizeY;
  format = (GLenum) newFormat;
  type = (GLenum) newType;
  bytesPerPixel = ComponentsPerPixel()*BytesPerComponent();
  bytesPerRow = bytesPerPixel * sizeX;
}

int glImage::ComponentsPerPixel() {
	switch (format) {
	case  GL_RGB : return(3);
	case  GL_RGBA : return(4);
	case GL_LUMINANCE_ALPHA : return(2);
	default : return(1);
	}
}

int glImage::BytesPerComponent() {
	switch (type) {
	case GL_FLOAT : return(sizeof(GLfloat));
	case GL_INT : 
	case GL_UNSIGNED_INT : return(sizeof(GLint));
	case GL_SHORT : 
	case GL_UNSIGNED_SHORT : return(sizeof(GLshort));
	case GL_BYTE : 
	case GL_UNSIGNED_BYTE : return(sizeof(GLbyte));
	case GL_BITMAP:return(0);
	}
	return(0);
}


int glImage::Scale(int newSizeX, int newSizeY)
{	int ret;

    GLubyte *buf;
	if ((sizeX == newSizeX) && (sizeY == newSizeY)) return(0);

    buf = (GLubyte *)malloc(bytesPerPixel*newSizeX*newSizeY);
	if (!buf) return(-1);
    
    ret=gluScaleImage(format, sizeX, sizeY, type,
                  data, newSizeX, newSizeY, type, buf);
    Set(buf,newSizeX,newSizeY,format,type);
	return(ret);
}

int glImage::ScaleNearest2Power()
{
    int imageSizeX = (int)pow(2.0f, (float)((int)(log((float)sizeX)/log(2.0f))));
    int imageSizeY = (int)pow(2.0f, (float)((int)(log((float)sizeY)/log(2.0f))));
	return(Scale(imageSizeX,imageSizeY));
}

int glImage::SetScaled(glImage &src,int newSizeX, int newSizeY)
{
	int ret;

    GLubyte *buf;

    buf = (GLubyte *)malloc(bytesPerPixel*newSizeX*newSizeY);
	if (!buf) return(-1);
   
    ret=gluScaleImage(src.format, src.sizeX, src.sizeY, src.type,
                  src.data, newSizeX, newSizeY, src.type, buf);
    Set(buf,newSizeX,newSizeY,src.format,src.type);
	return(ret);
}


void glImage::glRender(void)
{
    glRasterPos2i(0, 0);
    glDrawPixels(sizeX, sizeY, GL_RGB, GL_UNSIGNED_BYTE,data);
}

void glImage::glSetTexImage2d()
{
   glTexImage2D(GL_TEXTURE_2D, 0 /* level*/, ComponentsPerPixel(), 
   			sizeX, sizeY, 0 /* border 0/1 */, format, type,data);
}


#if 0
    gluOrtho2D(0, sizeX, 0, sizeY);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
#endif


//
// compute target texture format size 
//

void RenderState::GetTextureTargetSize(int &width,int &height,int roundUpLimit)
{
	get_image_2power(width,height,
		textureParameters.min_size_x,textureParameters.min_size_y,
		textureParameters.current_max_size_x,textureParameters.current_max_size_y,
		textureParameters.square_size,roundUpLimit);

}

//
// GglTextureHandle
//
int GglTextureHandle::hasTobjEXT=0;
	
glAreTexturesResident_t GglTextureHandle::glAreTexturesResidentEXT;
glBindTexture_t GglTextureHandle::glBindTextureEXT;
glGenTextures_t GglTextureHandle::glGenTexturesEXT;
glDeleteTextures_t GglTextureHandle::glDeleteTexturesEXT;
glIsTexture_t GglTextureHandle::glIsTextureEXT;
glPrioritizeTextures_t GglTextureHandle::glPrioritizeTexturesEXT;

gbool GglTextureHandle::ReloadTextureSurf(){ return( gfalse ); };
gbool GglTextureHandle::LoadTextureSurf(){ return( gfalse ); };

// set texture by texture filename 
gbool GglTextureHandle::Set(RenderState &state,LPCTSTR fileName,int format, LPCTSTR fileUrl)
{	int ret;
	int numTrys = 0;

Retry: numTrys++;



#ifdef _GL_TOBJ
	if (!Ok()) {
		New();	
		glCheckError();
	}


	glSet();

	glCheckError();
	glEnable(GL_TEXTURE_2D);
	state.textureParameters.glSet();
	state.currentTexture = this;

#else
	//if (cache.IsValid()) Release();
	if (Ok()) { Release(); }

	if (hasTobjEXT) {
		New();
		glSet();
		glEnable(GL_TEXTURE_2D);

		state.textureParameters.glSet();
		state.currentTexture = this;

	}
	else cache.Begin();

#endif
		//ret = ReadTextureFile(fileName,format,state,"Texture2",hasAlpha);
	   BOOL alpha;
	   BOOL colorKey;
	   BOOL gray;

	   cannotLoad = FALSE;

  	   ret = InitImageTexture(format,fileName,0, 
		   state.textureParameters.NeedMipmaps(),
		   state.textureParameters.current_max_size_x,state.textureParameters.current_max_size_y,
		   alpha,colorKey,gray,components,sizeX,sizeY,textureFormat);

	   this->fileName=fileName;
	   this->fileUrl=fileUrl;
	   fileFormat = format;
	
	   hasAlpha = alpha;
	   hasColorKey = colorKey;
	   isGrayscale = gray;
	   inVideoMem=1;

  	   glCheckError();
	   if (ret == 0) ret = -1;

#ifdef _GL_TOBJ

#else
	if (!hasTobjEXT) cache.End(); 
	else {

		GLint params[2];
		params[0]=1;
	    glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_RESIDENT,params );

  	    glCheckError();
  
 		//GLuint textures[2];
		//GLboolean residences[2]; 
		//textures[0]=texture;

		// if (!glAreTexturesResident(1,textures,residences)) 

		if (!params[0])
		{
			TRACE("Texture not resident \n");
			if (state.OnTextureMemoryOverflow(*this)) {
				if (numTrys < 5) goto Retry;
			}
		}			
	
	}; 
	
#endif
	
	if (ret<0) {
		Release();
		cannotLoad = TRUE;
	}

	return (ret >= 0);
}

// set texture surface from image data
gbool GglTextureHandle::Set(RenderState &state,int glFormat,int width,int height,int numComponents,unsigned char *data, char *colorTable)
{	int ret;

	fileName.Empty();
	fileUrl.Empty();
	fileFormat = 0;
    cannotLoad = FALSE;


#ifdef _GL_TOBJ
	if (!Ok()) New();	
	glCheckError();

	glSet();

	glCheckError();
	state.currentTexture = this;

#else
	//if (cache.IsValid()) Release();
	if (Ok()) { Release(); }

	if (hasTobjEXT) {
		New();
		glSet();
		glEnable(GL_TEXTURE_2D);
		state.textureParameters.glSet();
		state.currentTexture = this;
	}
	else cache.Begin();

#endif

	
	hasAlpha = (glFormat == GL_RGBA) || (glFormat == GL_LUMINANCE_ALPHA);
	hasColorKey = 0;
	isGrayscale = (glFormat == GL_LUMINANCE) || (glFormat == GL_LUMINANCE_ALPHA);
	components = numComponents;

	ret = glSetTexture((GLenum)glFormat,width,height,numComponents,data, 
					state.textureParameters.NeedMipmaps(),
					state.textureParameters.current_max_size_x,state.textureParameters.current_max_size_y,
					sizeX,sizeY);
    if (ret == 0) ret = -1;

#ifdef _GL_TOBJ

#else
	if (!hasTobjEXT) 
		cache.End();
	else {

		GLint params[2];
	    glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_RESIDENT,params );
  
 		//GLuint textures[2];
		//GLboolean residences[2]; 
		//textures[0]=texture;

		// if (!glAreTexturesResident(1,textures,residences)) 

		if (!params[0])
		{
			TRACE("Texture not resident \n");
		}			
	
	}; 
  


#endif

	
	if (ret<0) {
		Release();
		cannotLoad = TRUE;
	}

	return (ret >= 0);

}


// set texture by image
gbool GglTextureHandle::Set(RenderState &state,image *im,gbool checkScale,gbool checkFormat)
{	int ret;

	fileName.Empty();
	fileUrl.Empty();
	fileFormat = 0;
    cannotLoad = FALSE;


	int supported_fmts = state.GetSupportedImageFormats();

#ifdef _GL_TOBJ
	if (!Ok()) {
		New();	
		glCheckError();
	}


	glSet();

	glCheckError();
	glEnable(GL_TEXTURE_2D);
	state.textureParameters.glSet();
	state.currentTexture = this;


#else
	//if (cache.IsValid()) Release();

	if (hasTobjEXT) {
		if (!Ok()) { 
			Release(); 
			New();
		}
		glSet(); // glBindTexture
		glEnable(GL_TEXTURE_2D);
		state.textureParameters.glSet();
		state.currentTexture = this;
	}
	else {
		Release();  // release old displaylist
		cache.Begin();
	}

#endif

    hasColorKey = FALSE;	
	isGrayscale = image_is_gray(im);

	// color key to RGBA 
  	if ( (im->fmt == IM_MAPPED8  && im->colorKey) && (!(supported_fmts & (1<<IM_MAPPED8)))&& ((supported_fmts & (1<<IM_RGBA)))) {
		   hasColorKey = TRUE;	
		   ret=image_convert_to_rgba(im);
  	}

	if ((im->fmt == IM_MAPPED8) && (!(supported_fmts & (1<<IM_MAPPED8)))) {
      ret=image_convert_to_rgb(im);
   	}

	int glFormat= GL_RGB;
	int numComponents = im->bytesperpixel;
	switch (im->fmt) {
	default :
	case IM_RGB  : glFormat = GL_RGB; break;
	case IM_RGBA : glFormat = GL_RGBA; break;
	case IM_GRAY : glFormat = GL_LUMINANCE; break;
	case IM_GRAYA : glFormat = GL_LUMINANCE_ALPHA; break;
	}

	hasAlpha = (glFormat == GL_RGBA) || (glFormat == GL_LUMINANCE_ALPHA);
	components = numComponents;

	ret = glSetTexture((GLenum)glFormat,im->width,im->height,numComponents,im->image, 
					state.textureParameters.NeedMipmaps(),
					state.textureParameters.current_max_size_x,state.textureParameters.current_max_size_y,
					sizeX,sizeY
					);
	
  	 glCheckError();
	 if (ret == 0) ret = -1;

#ifdef _GL_TOBJ

#else
	if (!hasTobjEXT) 
		cache.End();
	else {

		GLint params[2];
	    glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_RESIDENT,params );
  
 		//GLuint textures[2];
		//GLboolean residences[2]; 
		//textures[0]=texture;

		// if (!glAreTexturesResident(1,textures,residences)) 

		if (!params[0])
		{
			TRACE("Texture not resident \n");
		}			
	
	}; 
#endif
	
	if (ret<0) {
		Release();
		cannotLoad = TRUE;
	}
	return (ret >= 0);
}


// 2D stuff

//! get a 2D renderer to current state
GRenderer2D* RenderState::getRenderer2D() 
{
  return this;
}






// actual GLU call back functions 

void __stdcall _tesselateBegin(GLenum mode) 
{
	TRACE("glbegin %d  \n",mode);
	glBegin(mode);
}


void  __stdcall _tesselateVertex3dv(const GLdouble *v)
{ 
	ASSERT(v);
	if (!v) return;

	glTexCoord2dv(v);
	glVertex3dv(v);
	TRACE("%g %g %g \n",v[0],v[1],v[2]);
}

void  __stdcall _tesselateEnd() {
	glEnd();
}

void  __stdcall _tesselateEdgeFlag (GLboolean flag)
{
}

void  __stdcall _tesselateError (GLenum error)
{
#ifdef _OGL
	const char* msg = (const char *)gluErrorString(error);
	TRACE("Tesselate error : %s\n",msg);
#endif

}

// new calls 
void __stdcall _tesselateCombine(GLdouble coords[3],
                                    void* verts[4], 
                                    GLfloat w[4],
                                    int* dataOut )
{


}


#ifndef GLU_VERSION_1_2
void gluTessNormal(
    GLUtesselator       *tess,
    GLdouble            x,
    GLdouble            y,
    GLdouble            z ){};
#endif



// 2 D Renderer interface 
void RenderState::initRenderer2D()
{
#if 1
	inPolygon=gfalse;
	inContour=gfalse;

	// GLU entry points

	_gluNewTess = gluNewTess;
	_gluDeleteTess = gluDeleteTess;

	_gluTessCallback = gluTessCallback;
	_gluBeginPolygon = gluBeginPolygon;
	_gluNextContour = gluNextContour;
	_gluEndPolygon = gluEndPolygon;
	_gluTessVertex = gluTessVertex;
	_gluTessNormal = gluTessNormal;

#ifdef GLU_VERSION_1_2 	
	_gluTessBeginContour = gluTessBeginContour;
	_gluTessEndContour = gluTessEndContour;
#endif




    tess2D = (*_gluNewTess)();  /* create and initialize a GLU tesselator */
//	gluTessProperty(tess2D,GLU_TESS_BOUNDARY_ONLY,GL_TRUE);

	//gluTessProperty(tess2D,GLU_TESS_WINDING_RULE ,GLU_TESS_WINDING_NEGATIVE);
/*	
	(*_gluTessCallback)(tess2D, GLU_TESS_BEGIN, (void(CALLBACK *)(void)) glBegin);  
	(*_gluTessCallback)(tess2D, GLU_TESS_VERTEX, (void(CALLBACK *)(void))glVertex3dv);  
	(*_gluTessCallback)(tess2D, GLU_TESS_END, (void(CALLBACK *)(void))glEnd);  
	//(*_gluTessCallback)(tess, GLU_TESS_EDGE_FLAG , glEdgeFlag);  
*/
    (*_gluTessCallback)(tess2D, GLU_BEGIN, (void(CALLBACK *)(void)) (GLUtessBeginProc) _tesselateBegin);
    (*_gluTessCallback)(tess2D, GLU_VERTEX, (void(CALLBACK *)(void)) (GLUtessVertexProc)_tesselateVertex3dv); 
    (*_gluTessCallback)(tess2D, GLU_END, (void(CALLBACK *)(void))(GLUtessEndProc)_tesselateEnd);
    (*_gluTessCallback)(tess2D, GLU_EDGE_FLAG, (void(CALLBACK *)(void))(GLUtessEdgeFlagProc)_tesselateEdgeFlag);
    (*_gluTessCallback)(tess2D, GLU_ERROR, (void(CALLBACK *)(void))(GLUtessErrorProc)_tesselateError);

#ifdef GLU_VERSION_1_2
	(*_gluTessCallback)(tess2D, GLU_TESS_COMBINE, (void(__stdcall *)(void))(GLUtessCombineProc)_tesselateCombine);
	//gluTessNormal(tess2D, 0 ,0 ,1);      

#endif


	
#endif

}

void RenderState::termRenderer2D()
{
	inPolygon=gfalse;
	if (tess2D) {
		(*_gluDeleteTess)(tess2D);
		tess2D = NULL;
	}
}

//!render 2D polyline,
void RenderState::DrawPolyline(int _np, const Point *_pts)
{
	int np=_np;
	ASSERT(np>=2);

	const Point *pts=_pts;

	if (inContour) {
	    GLdouble vertex[3];
		vertex[2]=0;

		while (np-- > 0) {
			vertex[0]=pts->x;
			vertex[1]=pts->y;
			vertex[2]=pts->z; // ?
			// (*_gluTessVertex)(tess2D,vertex,(void *)pts);
			(*gluTessVertex)(tess2D,vertex,(void *)NULL);
		    pts++;
		}

		return;
	}

	np=_np;
	pts=_pts;

	glBegin(GL_LINE_STRIP);
	while (np-- > 0) {
	   //glVertex2fv( &pts->x); pts++;
	   glVertex3fv( &pts->x); pts++;
	}
 	glEnd();

}


	// ! draw 3D polygon with vertex coordinates 
int RenderState::DrawPolygon3D(
					int vertexFlags,
					const float *v,	// f3 vertex coordinates
					const float *vc,
					const float *vn,
					int nvp, const float * vp[],	//Array of texture coordinates
					const long *coordIndex,int numCoordIndex,
					const Plane &plane		// plane of polygon 
				)
{
    gbool flipNormals = frontFaceCW && !twosidedLighting;

	ASSERT(numCoordIndex>=3);

	gbool mulTex = useARB_multitextureEXT && (nvp>1);

	//glActiveTextureARB( GL_TEXTURE1_ARB );
	//glDisable( GL_TEXTURE_2D );

	//glActiveTextureARB( GL_TEXTURE0_ARB );


	glBegin(GL_POLYGON);
	
	if (!vn) {
		if (flipNormals) 
			glNormal3f(-plane.n.x,-plane.n.y,-plane.n.z);
		else glNormal3f(plane.n.x,plane.n.y,plane.n.z);
	}	

	while (numCoordIndex >0) {
		long index = *coordIndex++;
		if (index <0) break;

		unsigned int i3= ((unsigned int) index)*3;
		if (vc) glColor3fv(&vc[i3]);
		if (vn) { if (flipNormals) glNormal3f(-vn[i3],-vn[i3+1],-vn[i3+2]); else glNormal3fv(&vn[i3]); }
		
		if (nvp>0) { // texture coordinate
			unsigned int i2= ((unsigned int) index)*2;
			if (vp[0]) glTexCoord2fv(&(vp[0][i2]));

			if (mulTex) 
				for (int i=1; i<nvp;i++) 
					if (vp[i]) glMultiTexCoord2fvARB((GLenum)(GL_TEXTURE0_ARB + i),&(vp[i][i2]));

		}

		glVertex3fv(&v[i3]);
	
		numCoordIndex--;

	}

	glEnd();
	return(0);

}



//!render 2D polygon
void RenderState::DrawPolygon(int np, const Point *pts)
{
	ASSERT(np>=3);
	if (inContour) {
		return;
	}

	glBegin(GL_POLYGON);
	while (np-- > 0) {
	   glVertex2fv( &pts->x); pts++;
	}
 	glEnd();

}


//!render 2D points
void RenderState::DrawPoint(int np, const Point *pts)
{
	ASSERT(np>=1);
	if (inContour) {
		return;
	}

	glBegin(GL_POINTS);
	while (np-- > 0) {
	   //glVertex2fv( &pts->x); pts++;
	   glVertex3fv( &pts->x); pts++;
	}
 	glEnd();
}

//!render 2D grid 
void RenderState::DrawGrid(const Point &pmin,const Point &pmax,int xsteps,int ysteps,gbool lines) 
{
	float u,v;
	float uinc = (pmax.x - pmin.x) / (float) (xsteps);
	float vinc = (pmax.y - pmin.y) / (float) (ysteps);

	int i,j;

	// horizontal 
	for (j=0,v=pmin.y;j<=ysteps;j++,v+=vinc) {

		if (lines)
			glBegin(GL_LINE_STRIP);
		else glBegin(GL_POINTS);


		for (i=0,u=pmin.x;i<=xsteps;i++,u+=uinc) {
			glVertex2f(u,v);
		}
		glEnd();

	}

	// vertical 
	if (lines)
	for (i=0,u=pmin.x;i<=xsteps;i++,u+=uinc) {

		if (lines)
			glBegin(GL_LINE_STRIP);
		else glBegin(GL_POINTS);

		for (j=0,v=pmin.y;j<=ysteps;j++,v+=vinc) {
			glVertex2f(u,v);
		}
		glEnd();

	}


}



//! begin a new contour within the current polygon 
void RenderState::BeginContour() 
{
	if (!tess2D) return; // error

	inContour = gtrue;
#ifdef GLU_VERSION_1_2 	
	//(*_gluTessBeginContour)(tess2D);
	_gluTessBeginContour(tess2D);
#else
	(*_gluNextContour)(tess2D,GLU_UNKNOWN);
#endif


}

//!end the current contour within the current polygon 
void RenderState::EndContour() 
{
	inContour = gfalse;
	if (!tess2D) return; // error

	glDisable(GL_LIGHTING);
	glColor3f(1,1,1);
	glNormal3f(0,0,1);

#ifdef GLU_VERSION_1_2 	
	(*_gluTessEndContour)(tess2D);
	//(*_gluNextContour)(tess2D,GLU_UNKNOWN);
#endif

	glCheckError();
}

//!begin a new polygon 
void RenderState::BeginPolygon()
{
	if (!tess2D) return; // error

	inPolygon = gtrue;
	
	(*_gluBeginPolygon)(tess2D);
	glCheckError();
}


//!end the current open polygon 
void RenderState::EndPolygon()
{
	if (!tess2D) return; // error

	inPolygon = gfalse;
	
	(*_gluEndPolygon)(tess2D);
	
	
	glCheckError();

}


// attributes
void RenderState::SetEdgeColor(const RGBAColor &color)
{
	edgeColor = color;

}
/*
void RenderState::SetColor(const RGBAColor &color)
{
}
*/

void RenderState::SetDrawStyle(int style)
{

}
//! set the point size 
//void RenderState::SetPointSize(float radius) 
//{
//}





#endif
