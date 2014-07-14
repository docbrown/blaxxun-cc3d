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
#include "gclass.h"

#include "goptimizetraversal.h"


#include <gvnodes.h>
#include <gvproto.h>
#include "gshell.h"
#include "gshelli.h"

#include "gnurbs.h"


#include <gvbsp.h>
#include <gvelement.h>
#include <gvnurbs.h>
#include <gvnurbsinventor.h>
#include <gvscript.h>

#include "gversion.h"

Point GRound(const Point &p,float scale,float xscale)
{ return Point(GRound(p.x,scale,xscale),GRound(p.y,scale,xscale),
			   GRound(p.z,scale,xscale)); }


void GRound(PointArray &p,float scale,float xscale)
{
  if (p.Length() <=0) return;

  for(int i=0;i<p.Length();i++) {
	  GRoundEq(p[i],scale,xscale);
  }	
 
}



#ifndef _GV_MINIMAL
#define GOPT
#endif
///////////////////////////////////////

// create Coordinate node from points, transformed by m
GvCoordinate *CreateCoordinate(const PointArray &v,Matrix *m= NULL)
{
	GvCoordinate *n = new  GvCoordinate();
	if (m) {
		PointArray tv(v);
		tv.Mult(*m);
		n->point.set(tv);
	} else n->point.set(v);

	return n;
}

// like above, but uses subface only 
GvCoordinate *CreateCoordinate(const PointArray &v,int cnt,const int f[], Matrix *m= NULL)
{
	PointArray p(cnt);
	for (int i=0; i<cnt;i++) {
		p[i] = v[f[i]];
	}
	return CreateCoordinate(p,m);
}

// like above, but uses subface only 
GvCoordinate *CreateCoordinate(const PointArray &v,int cnt,const int f[], Matrix *m,const GVertexI verts[])
{
	PointArray p(cnt);
	for (int i=0; i<cnt;i++) {
		p[i] = v[verts[f[i]].v];
	}
	return CreateCoordinate(p,m);
}


// create Coordinate node from points, transformed by m
Gv2Normal *CreateNormal(PointArray &v,Matrix *m= NULL)
{									
	Gv2Normal *n = new  Gv2Normal();
	if (m) {
		Matrix use = Transpose(Invert(*m));
		// Plane Rotate  ?? Foley pg 1108
		use.x[3][0]= use.x[3][1] = use.x[3][2] = 0;

		PointArray tv(v);
		tv.Mult(use);
		n->vector.set(tv);
	} else n->vector.set(v);

	return n;
}

// like above, but uses subface only 
Gv2Normal *CreateNormal(const PointArray &v,int cnt,const int f[], Matrix *m= NULL)
{
	PointArray p(cnt);
	for (int i=0; i<cnt;i++) {
		p[i] = v[f[i]];
	}
	return CreateNormal(p,m);
}

Gv2Normal *CreateNormal(const PointArray &v,int cnt,const int f[], Matrix *m,const GVertexI verts[])
{
	PointArray p(cnt);
	for (int i=0; i<cnt;i++) {
		p[i] = v[verts[f[i]].n];
	}
	return CreateNormal(p,m);
}


GvColor *CreateColor(const PointArray &v)
{
	GvColor *n = new  GvColor();
	n->color.set(v);
	return n;
}

// like above, but uses subface only 
GvColor *CreateColor(const PointArray &v,int cnt,const int f[])
{
	PointArray p(cnt);
	for (int i=0; i<cnt;i++) {
		p[i] = v[f[i]];
	}
	return CreateColor(p);
}

// like above, but uses subface only 
GvColor *CreateColor(const PointArray &v,int cnt,const int f[],const GVertexI verts[])
{
	PointArray p(cnt);
	for (int i=0; i<cnt;i++) {
		p[i] = v[verts[f[i]].c];
	}
	return CreateColor(p);
}

/*
Gv2Normal *CreateNormal(PointArray &v)
{
	Gv2Normal *n = new  Gv2Normal();
	n->vector.set(v);
	return n;
}
// like above, but uses subface only 
Gv2Normal *CreateNormal(PointArray &v,int cnt,const int f[])
{
	PointArray p(cnt);
	for (int i=0; i<cnt;i++) {
		p[i] = v[f[i]];
	}
	return CreateNormal(p);
}
*/


GvTextureCoordinate* CreateTextureCoordinate(const PointArray &v,Matrix *m= NULL)
{
	GvTextureCoordinate *n = new GvTextureCoordinate();
	if (m) {
		PointArray tv(v);
		tv.Mult(*m);
		n->point.set(tv);
	} else n->point.set(v);
	return n;
}

// like above, but uses subface only 
GvTextureCoordinate *CreateTextureCoordinate(const PointArray &v,int cnt,const int f[], Matrix *m= NULL)
{
	PointArray p(cnt);
	for (int i=0; i<cnt;i++) {
		p[i] = v[f[i]];
	}
	return CreateTextureCoordinate(p,m);
}

// like above, but uses subface only 
GvTextureCoordinate *CreateTextureCoordinate(const PointArray &v,int cnt,const int f[], Matrix *m,const GVertexI verts[])
{
	PointArray p(cnt);
	for (int i=0; i<cnt;i++) {
		p[i] = v[verts[f[i]].p]; 
	}
	return CreateTextureCoordinate(p,m);
}


// create a IndexedFaceSet out of shell, wrap == TRUE, flip faces
// optional transformation and texture matrix 

Gv2IndexedFaceSet* CreateIndexedFaceSet(GShell *s, GvBool wrap,  Matrix *m,Matrix * tm,GvBool isSolid)
{
  Gv2IndexedFaceSet *fs= new Gv2IndexedFaceSet;

  if (!isSolid)
	  fs->solid.set(isSolid);
 
  // coordinates
  fs->coord.set(CreateCoordinate(s->v,m));
  
  // vertex colors
  if (s->vc.Length()>0) {
	 fs->color.set(CreateColor(s->vc));
	 fs->colorPerVertex.set(TRUE);
  }	else
  if (s->fc.Length()>0) {
	 fs->color.set(CreateColor(s->fc));
	 fs->colorPerVertex.set(FALSE);
  }	

  // vertex parameters 
  if (s->vp.Length()>0)
	 fs->texCoord.set(CreateTextureCoordinate(s->vp,tm));

  if (s->Flat()) { } // fs->normalPerVertex.set(FALSE);
  else fs->creaseAngle.set(3.16);

  // vertex normals

  // convert face list 
  const int *fp = s->f,*fpend=s->f+s->f.Length();
  
  fs->coordIndex.makeRoom(s->f.Length());
  long *dest=fs->coordIndex.values;

  int fi=0;
  while ( fp < fpend ) {
	 int n = *fp++;
	 if (n<0) n=-n; // hole

	 // store the face indices 
	 if (wrap) {
		 for(int i=n-1; i>=0; i--) {  *dest ++= fp[i];  }
		 fp += n;
	 }	
	 else {
		 while (n>0) {  *dest ++= *fp++;  n--; 	 }
	 }
	 *dest++ = -1;
	 fi++;
  }
  fs->coordIndex.setDefault(FALSE);

  return(fs);

}

// create a IndexedFaceSet out of shell, wrap == TRUE, flip faces
// optional transformation and texture matrix 

GvPolygon* CreatePolygon(GShell *s, GvBool wrap,  Matrix *m,Matrix * tm,GvBool isSolid)
{
  GvPolygon *fs= new GvPolygon;

  if (!isSolid)
	  fs->solid.set(isSolid);
 
  // coordinates
  fs->coord.set(CreateCoordinate(s->v,m));
  
  // vertex colors
  if (s->vc.Length()>0) {
	 fs->color.set(CreateColor(s->vc));
  }

  // vertex parameters 
  if (s->vp.Length()>0)
	 fs->texCoord.set(CreateTextureCoordinate(s->vp,tm));

  if (s->Flat()) {  // fs->normalPerVertex.set(FALSE);
  } else {	
  
	// vertex normals
  }	

  // convert face list 
  const int *fp = s->f,*fpend=s->f+s->f.Length();
  
  fs->coordIndex.makeRoom(s->f.Length());
  long *dest=fs->coordIndex.values;

  int fi=0;
  while ( fp < fpend ) {
	 int n = *fp++;
	 if (n<0) n=-n; // hole

	 // store the face indices 
	 if (wrap) {
		 for(int i=n-1; i>=0; i--) {  *dest ++= fp[i];  }
		 fp += n;
	 }	
	 else {
		 while (n>0) {  *dest ++= *fp++;  n--; 	 }
	 }
	 *dest++ = -1;
	 fi++;
  }
  fs->coordIndex.setDefault(FALSE);

  return(fs);

}


// create a new material node, and optionally copy settings from m
Gv2Material * NewMaterial(Gv2Material *m)
{
	Gv2Material *result;
	result = new Gv2Material();
	if (m == ((GvNode *) -1)) return result;

	if (m) {
		if (!m->ambientIntensity.isDefault()) result->ambientIntensity = m->ambientIntensity;
		if (!m->diffuseColor.isDefault()) result->diffuseColor = m->diffuseColor;
		if (!m->specularColor.isDefault()) result->specularColor = m->specularColor;
		if (!m->emissiveColor.isDefault()) result->emissiveColor = m->emissiveColor;
		if (!m->shininess.isDefault()) result->shininess = m->shininess;
		if (!m->transparency.isDefault()) result->transparency = m->transparency;
		
		if (result->emissiveColor[0]==0.0f && result->emissiveColor[0]==0.0f && result->emissiveColor[0]==0.0f) result->emissiveColor.setDefault(TRUE);
		if (result->specularColor[0]==0.0f && result->specularColor[0]==0.0f && result->specularColor[0]==0.0f) result->specularColor.setDefault(TRUE);

		if (result->ambientIntensity ==0.2f) result->ambientIntensity.setDefault(TRUE);
		if (result->shininess ==0.2f) result->shininess.setDefault(TRUE);
		if (result->transparency ==0.0f) result->transparency.setDefault(TRUE);
		result->checkForDefaultValues(1);
	}
	return result;
}





Gv2IndexedFaceSet* CreateIndexedFaceSet(GShellI *s, GvBool wrap,  Matrix *m,Matrix * tm, GvBool isSolid, GvBool bx3d)
{
  Gv2IndexedFaceSet *fs= new Gv2IndexedFaceSet;

  int doNormalIndex=0;
  int doTextureIndex=0;
  int doColorIndex=0;

  if (!isSolid)
	  fs->solid.set(isSolid);
 
  // coordinates
  fs->coord.set(CreateCoordinate(s->v,m));

  if (!bx3d) {
  // vertex colors
  if (s->vc.Length()>0) {
	 fs->color.set(CreateColor(s->vc));
	 fs->colorPerVertex.set(TRUE);

	 // need indices if not matching vertex indices 
	 for(int i=0; i<s->verts.Length(); i++) {
		GVertexI &v = s->verts[i];
		if (v.v != v.c) {
		   doColorIndex = 1;
		   break;
		}
	 }	

  }	else
  if (s->fc.Length()>0) {
	 fs->color.set(CreateColor(s->fc));
	 fs->colorPerVertex.set(FALSE);
  }	
  } // bx3d

  // vertex parameters 
  if (s->vp.Length()>0) {
	 fs->texCoord.set(CreateTextureCoordinate(s->vp,tm));

	 // need indices if not matching vertex indices 
	 for(int i=0; i<s->verts.Length(); i++) {
		GVertexI &v = s->verts[i];
		if (v.v != v.p) {
		   doTextureIndex = 1;
		   break;
		}
	 }	
  }

  if (s->Flat()) {} // fs->normalPerVertex.set(FALSE);
  else {  
	 fs->creaseAngle.set(3.16);

	 if (!bx3d) {
	 // need indices if not matching vertex indices 
	 for(int i=0; i<s->verts.Length(); i++) {
		GVertexI &v = s->verts[i];
		if (v.v != v.n) {
		   doNormalIndex = 1;
		   break;
		}
	 }	
	 }
  }

  // vertex normals

  // convert face list 
  const int *fp = s->f,*fpend=s->f+s->f.Length();
  
  fs->coordIndex.makeRoom(s->f.Length());
  long *dest=fs->coordIndex.values;

  // check which indices to be written 
  long *normalIndex = NULL;
  if (doNormalIndex) {
	 fs->normalIndex.makeRoom(s->f.Length());
	 normalIndex=fs->normalIndex.values;
	 fs->normalIndex.setDefault(FALSE);

  }	

  long *colorIndex = NULL;
  if (doColorIndex) {
	 fs->colorIndex.makeRoom(s->f.Length());
	 colorIndex=fs->colorIndex.values;
	 fs->colorIndex.setDefault(FALSE);
  }	

  long *textureIndex = NULL;
  if (doTextureIndex) {
	 fs->texCoordIndex.makeRoom(s->f.Length());
	 textureIndex=fs->texCoordIndex.values;
	 fs->texCoordIndex.setDefault(FALSE);

  }	

  int fi=0;
  while ( fp < fpend ) {
	 int n = *fp++;
	 if (n<0) n=-n; // hole

	 // store the face indices 
	 if (wrap) {
		 for(int i=n-1; i>=0; i--) { 
			 GVertexI &v = s->verts[fp[i]];
			 *dest++ = v.v;  

			 if (doNormalIndex)	 *normalIndex++ = v.n;
			 if (doColorIndex)	 *colorIndex++ = v.c;
			 if (doTextureIndex) *textureIndex++ = v.p;

		 }
		 fp += n;
	 }	
	 else {
		 while (n>0) {  

			 GVertexI &v = s->verts[*fp++];
			 *dest++ = v.v;  

			 if (doNormalIndex)	 *normalIndex++ = v.n;
			 if (doColorIndex)	 *colorIndex++ = v.c;
			 if (doTextureIndex) *textureIndex++ = v.p;
			 n--; 	 

		 }
	 }
	 // terminate face 
	 *dest++ = -1;
	 if (doNormalIndex)	 *normalIndex++ = -1;
	 if (doColorIndex)	 *colorIndex++ = -1;
	 if (doTextureIndex) *textureIndex++ = -1;
	 fi++;
  }
  fs->coordIndex.setDefault(FALSE);

  return(fs);

}


//  create a IndexedFaceSet out of face of shell, wrap == TRUE, flip faces
// optional transformation and texture matrix 
Gv2IndexedFaceSet* NewIfsFromFace(GTraversal &state,GShell *s, int fcnt, const int fp[], int faceNum, 
								  GvBool wrap, GvBool solid, Matrix *m,Matrix * tm)
{

  Gv2IndexedFaceSet *fs= new Gv2IndexedFaceSet;

  // coordinates
  fs->coord.set(CreateCoordinate(s->v,fcnt,fp,m));

  if (!solid) fs->solid.set(FALSE);

  if (s->Flat()) fs->normalPerVertex.set(FALSE);
  else {
	fs->creaseAngle.set(3.16);

	// vertex normals

	if (s->vn.Length()>0) {
		fs->normal.set(CreateNormal(s->vn,fcnt,fp,m));
		fs->normalPerVertex.set(TRUE);
	}	
  
  }
  
  // vertex colors
  if (s->vc.Length()>0) {
	 fs->color.set(CreateColor(s->vc,fcnt,fp));
	 fs->colorPerVertex.set(TRUE);
  }	
/*
  else
  if (s->fc.Length()>0) {
	 fs->color.set(CreateColor(s->fc[faceNum]));
	 fs->colorPerVertex.set(FALSE);
  }	
*/
  // vertex parameters 
  if (s->vp.Length()>0)
	 fs->texCoord.set(CreateTextureCoordinate(s->vp,fcnt,fp,tm));


  // convert face list 

  fs->coordIndex.makeRoom(fcnt+1);

  long *dest=fs->coordIndex.values;

    int n = fcnt;

	 // store the face indices 	 , now linear ! 
	 if (wrap) {
		 for(int i=n-1; i>=0; i--) {  *dest ++= i;  }
	 }	
	 else {
		 for(int i=0; i<n; i++) {  *dest ++= i; }
	 }
	 *dest++ = -1;

	 fs->coordIndex.setDefault(FALSE);

	 fs->BuildShell(state); // we need info for BSP 

  return(fs);
}

Gv2IndexedFaceSet* NewIfsFromFace(GTraversal &state,GShellI *s, int fcnt, const int fp[], int faceNum, 
								  GvBool wrap, GvBool solid, Matrix *m,Matrix * tm)
{

  Gv2IndexedFaceSet *fs= new Gv2IndexedFaceSet;

  // coordinates
  fs->coord.set(CreateCoordinate(s->v,fcnt,fp,m,s->verts));

  if (!solid) fs->solid.set(FALSE);

  if (s->Flat()) fs->normalPerVertex.set(FALSE);
  else {
	fs->creaseAngle.set(3.16);

	// vertex normals

	if (s->vn.Length()>0) {
		fs->normal.set(CreateNormal(s->vn,fcnt,fp,m,s->verts));
		fs->normalPerVertex.set(TRUE);
	}	
  
  }
  
  // vertex colors
  if (s->vc.Length()>0) {
	 fs->color.set(CreateColor(s->vc,fcnt,fp,s->verts));
	 fs->colorPerVertex.set(TRUE);
  }	
/*
  else
  if (s->fc.Length()>0) {
	 fs->color.set(CreateColor(s->fc[faceNum]));
	 fs->colorPerVertex.set(FALSE);
  }	
*/
  // vertex parameters 
  if (s->vp.Length()>0)
	 fs->texCoord.set(CreateTextureCoordinate(s->vp,fcnt,fp,tm,s->verts));


  // convert face list 

  fs->coordIndex.makeRoom(fcnt+1);

  long *dest=fs->coordIndex.values;

    int n = fcnt;

	 // store the face indices 	 , now linear ! 
	 if (wrap) {
		 for(int i=n-1; i>=0; i--) {  *dest ++= i;  }
	 }	
	 else {
		 for(int i=0; i<n; i++) {  *dest ++= i; }
	 }
	 *dest++ = -1;

	 fs->coordIndex.setDefault(FALSE);

	 fs->BuildShell(state); // we need info for BSP 

  return(fs);
}



// expand object into singl faces IFS
GvBool ExpandIndexedFaceSet(GTraversal &state,GShell *s, GvBool wrap, GvBool solid,  Matrix *m,Matrix * tm, GvMFNode &result)
{

  GvBool colorPerFaces =  (s->fc.Length()>0); 
  GvBool isShellI = FALSE;
  
  if (RTISA(s,GShellI)) {
		 isShellI= TRUE;
  }


  // step face list 
  const int *fp = s->f,*fpend=s->f+s->f.Length();
  

  int fi=0;
  while ( fp < fpend ) {
	 int n = *fp++;
	 if (n<0) n=-n; // hole
	 if (n>=3) {	
		if (isShellI) result.add(NewIfsFromFace(state,(GShellI*) s, n,fp,fi,wrap,solid,m,tm));
		else result.add(NewIfsFromFace(state,s, n,fp,fi,wrap,solid,m,tm));
	 }
	 fp +=n;

	 fi++;
  }
  return(result.getNum()>0);

}

class GFEntry {
public:
	Point c;
	
	GShellI *s;
	
	GvSFNode material;
	
	GFEntry *next;

	GFEntry () {s = NULL;} 
	~GFEntry () { delete s;} 

	// find entry with the material matching fc
	static GFEntry *findEntry(GFEntry * &root,const Point &fc) {
	  GFEntry *m = root;
	  while (m != NULL) {
		  if (m->c == fc) return m;
		  m = m->next;
	  }	
	  return NULL;	
	}
};

/*
  split shell with fc into separate sets 
  need root material

*/

gbool ExpandFaceColors(GShellI *s, 
					  // for appearance creation 
					  Gv2Material *mat,
					  GvNode *texture,GvNode *textureTranform,
					  
					  gbool solid,gbool flip,
					  
					  gbool replaceEmissive,
					  GvMFNode &result)
{

  GvBool isShellI = FALSE;
  
  if (RTISA(s,GShellI)) {
		 isShellI= TRUE;
  }
  
  GFEntry *root = NULL;


  // step face list 
  const int *fp = s->f,*fpend=s->f+s->f.Length();
  

  int fi=0;
  while ( fp < fpend ) {
	  ASSERT(s->fc.Data() != NULL);
	  ASSERT(fi < s->fc.Length());
	 
	  GFEntry *e = GFEntry::findEntry(root,s->fc[fi]);

	  if (!e) { // create new entry, with new materail
		  e = new GFEntry();
		  
		  if (!e) break;

		  e->next = root; root = e;
		  e->c = s->fc[fi];
		  
		  Gv2Material *lm=NewMaterial(mat);
		  if (replaceEmissive)
		  lm->emissiveColor.set(s->fc[fi]);
		  else lm->diffuseColor.set(s->fc[fi]);

		  e->material.set(lm);
		  e->s = new GShellI();
		  // create a copy 	
		  s->ComputeShell(e->s,1);
		  
		  e->s->SetFaceList(0,NULL);

		  e->s->fc.SetLength(0);
		  e->s->fn.SetLength(0);

	  }	
	  
	  const int *faceStart = fp;
	  int n = *fp++;
	

	  if (n<0) n=-n; // hole
  	  fp +=n;
	  
	  while (fp <fpend && *fp <0){
		  fp+= 1 + - *fp;
	  }		
	 
	  e->s->f.Append(fp-faceStart,faceStart);

	  fi++;
  }

  GFEntry *m = root;
  
  while (m != NULL) 
  {
	  GFEntry *mnext = m->next;

	  GvShape *s;
	  s = new GvShape();
	  GvAppearance *a;
	  a = new GvAppearance();
	  a->material.set(m->material.get());	

	  if ( (texture != NULL) && texture != ((GvNode *) -1)) 
	  		a->texture.set(texture);

	  if ( (textureTranform != NULL) && textureTranform != ((GvNode *) -1)) 
		  a->textureTransform.set(textureTranform);

	  s->appearance.set(a);
	  
	  // remove unused verts 
	  GShellI *newS = CompressShell(m->s,NULL,1,0);

	  Gv2IndexedFaceSet *ifs =
		  CreateIndexedFaceSet(newS,flip); // eShell,!ccw, m,tm);

	  if (!solid) ifs->solid.set(FALSE);
	  
	  s->geometry.set(ifs);

	  delete newS;


	  result.add(s);
	  delete m;

	  m = mnext;
  }	

  return(result.getNum()>0);

}


///////////////////////////////////////



#ifndef GOPT

int GNode::Do(GOptimizeTraversal &state) // declared in GClass.h
{
	return Do(*((GTraversal *) &state));		
}

int GvNode::Do(GOptimizeTraversal &state) // declared in GvNode.h
{
	if (state.removeNodeNames && getName().getLength()>0) { 
		if (!hasRoutes() || state.ignoreRoutes)
			setName(GvName(""));
	}
	return GNode::Do(state); 
}

int GvGeometrySensor::Do(GOptimizeTraversal &state) 
{	
	return GvNode::Do(state); 
}

int GvTimeSensor::Do(GOptimizeTraversal &state) 
{	
	return GvNode::Do(state); 
}

int GvProtoInstance::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

// VRML 1.0 

#ifdef _G_VRML1
int GvCoordinate3::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int GvTransform::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

#ifdef _G_INVENTOR
int GvNurbsProfile::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }
int GvLinearProfile::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }
int GvProfileCoordinate2::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }
int GvProfileCoordinate3::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }
int GvCoordinate4::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }


#endif


int GvTextureCoordinate2::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }
int GvTextureCoordinateBinding::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }
int GvNormal::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }
int GvNormalBinding::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }
int GvMaterial::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }
int GvMaterialBinding::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }
int GvMatrixTransform::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }
int GvRotation::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }
int GvRotationXYZ::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }
int GvScale::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }
int GvTranslation::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int GvGroup::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }
int GvSeparator::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }
int GvWWWAnchor::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int GvShapeHints::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int GvInfo::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }


#endif _G_VRML1

//
int Gv2Group::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }


int Gv2LOD::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }


int Gv2Switch::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int Gv2Billboard::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int GvHUD::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int Gv2Transform::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int Gv2MatrixTransform::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int Gv2Collision::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int Gv2Inline::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int Gv2Anchor::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int GvShape::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int GvAppearance::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int Gv2Material::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int GvImageTexture::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }


int Gv2IndexedFaceSet::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int GvPolygon::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int GvNodeShell::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int GvNurbsSurface::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int GvTrimmedSurface::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int Gv2TextureTransform::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int GvViewpoint::Do(GOptimizeTraversal &state)
{	return GvNode::Do(state); }

int GvBspTree::Do(GOptimizeTraversal &state) 
{	return GvNode::Do(state); }


#else

#define NAME_CHECK \
if (state.removeNodeNames && getName().getLength()>0) {  \
		if (!hasRoutes() || state.ignoreRoutes) \
			setName(GvName("")); \
	}



int GNode::Do(GOptimizeTraversal &state) // declared in GClass.h
{
	return Do(*((GTraversal *) &state));		
}

int GvNode::Do(GOptimizeTraversal &state) // declared in GvNode.h
{
	if (state.removeNodeNames && getName().getLength()>0) { 
		if (!hasRoutes() || state.ignoreRoutes)
			setName(GvName(""));
	}

	// to think, remove unneeded nodes names starting with _
	state.AppendResult(this,GOptimizeTraversal::HAS_UNKNOWN_NODE);
	return(0);
	//return Do(*((GTraversal *) &state));		// don´t do default traversal
}

int GvGeometrySensor::Do(GOptimizeTraversal &state) 
{
	if (state.ignoreRoutes) return 0; // ignore 

	if (state.bx3d) 
		return 0; // ignore 

	state.AppendResult(this, (hasRoutes() ? GOptimizeTraversal::HAS_ROUTES :0) | GOptimizeTraversal::KEEP_GROUP);
	return(0);
}

int GvTimeSensor::Do(GOptimizeTraversal &state) 
{
	if (state.ignoreRoutes) return 0; // ignore 

	if (!hasRoutes()) return 0; // ignore 
	state.AppendResult(this, (hasRoutes() ? GOptimizeTraversal::HAS_ROUTES :0) );
	return(0);
}

// expand protoinstance 
int GvProtoInstance::Do(GOptimizeTraversal &state)
{	int ret = 0;

	if (!state.bx3d) // ignore 
	if (!state.doInlines) {
		// has mostly routes because of IS bad .........
		if (hasRoutes() && !state.ignoreRoutes) {
			state.ReturnNode(this,GOptimizeTraversal::HAS_ROUTES);
			return(0);
		}
	}
	// keep it ??

    if (!instanced)  
        Instance(state);

    ret = children.Traverse(state);

	return(ret);
}

// VRML 1.0 
int GvCoordinate3::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}
int GvTransform::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}


#ifdef _G_INVENTOR

int GvNurbsProfile::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}
int GvLinearProfile::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}

int GvProfileCoordinate2::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}

int GvProfileCoordinate3::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}
int GvCoordinate4::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}

#endif


int GvTextureCoordinate2::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}
int GvTextureCoordinateBinding::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}

int GvNormal::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}
int GvNormalBinding::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}

int GvMaterial::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}

int GvMaterialBinding::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}

int GvShapeHints::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}

int GvInfo::Do(GOptimizeTraversal &state)
{	
	if (state.vrml2) {
		GvWorldInfo *n=new GvWorldInfo();
		n->_info.set_(this->string);
		n->setName(getName());

		state.ReturnNode(n,0);
		return 1;

	}
	return GvNode::Do(state); 
}


int GvMatrixTransform::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}
int GvRotation::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}
int GvRotationXYZ::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}
int GvScale::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}
int GvTranslation::Do(GOptimizeTraversal &state)
{ 
	Do(*((GTraversal *) &state));		
	return(0);
}

int GvGroup::Do(GOptimizeTraversal &state)
{	int ret = 0;

    for (int i = 0; i < getNumChildren(); i++) {
		ret = getChild(i)->Traverse(state);
	}
	return(ret);
}

int GvSeparator::Do(GOptimizeTraversal &state)
{	int ret = 0;

	if (state.vrml2) {
	    state.Push();

		GvMFNode oldResult(0);

		int	oldResultMask = state.GetResult(oldResult);
	
		Gv2Group *g = new Gv2Group();
		g->setName(getName());


		for (int i = 0; i < getNumChildren(); i++) {
			ret = getChild(i)->Traverse(state);
		}


		state.GetResultBsp(g->children);

		state.SetResult(oldResult,oldResultMask);

		state.AppendResult(g,GOptimizeTraversal::HAS_GROUP);

		state.Pop();


		return 1;

	}

    state.Push();

    for (int i = 0; i < getNumChildren(); i++) {
		ret = getChild(i)->Traverse(state);
	}
	
	state.Pop();
	return(ret);
}

int GvWWWAnchor::Do(GOptimizeTraversal &state)
{	int ret = 0;

	// keep it 
    state.Push();

	GvMFNode oldResult(0);

	int oldResultMask = state.GetResult(oldResult);
	Gv2Anchor *g = new Gv2Anchor();


    for (int i = 0; i < getNumChildren(); i++) {
		ret = getChild(i)->Traverse(state);
	}

	state.SetResult(oldResult,oldResultMask);
					  //GvMFString
	// copy fields
	g->description.set(description);
	g->parameter.set_(target);
	g->url.set_(name);
	g->homeUrl = homeUrl;
	g->homeUrlInit = homeUrlInit;
	state.GetResultBsp(g->children);

	state.AppendResult(g,GOptimizeTraversal::HAS_GROUP);

	state.Pop();
	return(ret);
}


GvNode* GOptimizeTraversal::MakeGroup(GvMFNode &nodes)
{

  Gv2Group *g=NULL;

  if (buildBspTree) {

		GvMFNode nodesForBsp(0);
		GvMFNode ret(0);

		// filter out non BSP nodes 
		for (int i= 0; i<nodes.getNum(); i++) {
			GvNode *n= nodes[i];

			if (UseNodeForBsp(n))
				nodesForBsp.add(n);
			else
				ret.add(n);
		}

		if (nodesForBsp.getNum()>0) {
			ret.add(BuildBspTree(nodesForBsp));
		}  

		// create result 
		if (g == NULL) {
			if (ret.getNum() == 0) return NULL;
/*
			if (ret.getNum() == 1) {
//	 !!!    	nodes in ret looses refcnt 
				return ret[0];
			}
*/
		}

		if (g == NULL) g = new Gv2Group();

		g->children.set(ret);

		return g;

		// GvNode *n = BuildBspTree(nodes);
		//return n;
  }	 else {
	if (g == NULL) g = new Gv2Group();
	g->children.set(nodes);
	return g;
  }
}

GvBool KeepGroup(GvMFNode &shapes)
{
	for (int i=0; i<shapes.getNum(); i++) {
		GvNode *s = shapes[i];
		if (s == NULL) continue;
		if (RTISA(s,GvGeometrySensor))
			return TRUE;
		if (RTISA(s,GvDirectionalLight))
			return TRUE;
	}
	return FALSE;
}


										
int Gv2Group::Do(GOptimizeTraversal &state) 
{
	int ret = 0;

	if (hasRoutes() && !state.ignoreRoutes) {
		// problem if transforms are beeing evaluated 
		state.ReturnNode(this,GOptimizeTraversal::HAS_ROUTES | GOptimizeTraversal::HAS_GROUP);
		return(0);
	}

	// ignore if empty
	if (children.getNum() == 0) return(1);

/*	
	if (!( this->Id() == Gv2Group::ClassId())) { // keep if not exact class 
		return(0);
	}
*/

    state.Push();
	ret = ApplyGeometrySensors(state);



	GvBool keepGroup = KeepGroup(children);

	if (keepGroup) {
		GvMFNode newChildren(0);
		int ret = state.Do(children,newChildren,TRUE);
		GvNode *g= state.MakeGroup(newChildren);
		state.AppendResult(g, GOptimizeTraversal::HAS_GROUP);
		ret = 1;
	}
	else ret = state.Do(children); // expand children  

	state.Pop();

	return(ret);
	
	
#if 0	
	int ret = state.Do(children);


	GvBool doCopy = (state.resultMask & GOptimizeTraversal::KEEP_GROUP) !=0;

	if (doCopy) {
		GvMFNode newChildren(0);

		//Gv2Group *g = new Gv2Group();
		int mask = state.GetResultBsp(newChildren);
		
		GvNode *g= state.MakeGroup(newChildren);
		state.AppendResult(g, GOptimizeTraversal::HAS_GROUP);
		ret=1;
	}

	return(ret);
#endif 
}

int Gv2LOD::Do(GOptimizeTraversal &state) 
{
	if (hasRoutes() && !state.ignoreRoutes) {
		// problem if transforms are beeing evaluated 
		state.ReturnNode(this,GOptimizeTraversal::HAS_ROUTES | GOptimizeTraversal::HAS_GROUP);
		return(0);
	}

	if (!state.expandLods) {
	   TRACE("TO DO \n");
	}
	
	int nChilds = level.Length();
    if (nChilds == 0) return(0);

	int ret = level[nChilds-1]->Traverse(state);


	return(ret);
}

int Gv2Switch::Do(GOptimizeTraversal &state) 
{

	if (hasRoutes() && !state.ignoreRoutes) {
		// problem if transforms are beeing evaluated 
		state.ReturnNode(this,GOptimizeTraversal::HAS_ROUTES | GOptimizeTraversal::HAS_GROUP);
		return(0);
	}


    int which = whichChoice.value;

    if (which == GV_SWITCH_NONE)
	    return(0);
    else if (which == GV_SWITCH_ALL) // extension 
        return(choice.Traverse(state));
    else if (which <0) {
        return(0); 
    }
	else if (which < choice.Length() && ((GvNode*)choice.values[which] !=NULL))
	    return choice.values[which]->Traverse(state);
    return(0);

	return(0);
}

int Gv2Billboard::Do(GOptimizeTraversal &state) 
{
	state.ReturnNode(this,GOptimizeTraversal::HAS_GROUP);
	return(0);
}

int GvHUD::Do(GOptimizeTraversal &state) 
{
	state.ReturnNode(this,GOptimizeTraversal::HAS_GROUP);
	return(0);
}

int GvViewpoint::Do(GOptimizeTraversal &state) 
{
	state.ReturnNode(this,GOptimizeTraversal::HAS_GROUP);
	return(0);
}

int GvBspTree::Do(GOptimizeTraversal &state) 
{
	int ret;
	if (hasRoutes() && !state.ignoreRoutes) {
		state.ReturnNode(this,GOptimizeTraversal::HAS_ROUTES | GOptimizeTraversal::HAS_GROUP);
		return(0);
	}
	// simply traverse all

	ret = front.Traverse(state);
	ret = overlap.Traverse(state);
   	ret = back.Traverse(state);
	return ret;

}


// check transform fields for default values 
GvBool CheckForDefaults(Gv2Transform *g)
{	GvSFVec3f	zero(0,0,0);
	GvSFVec3f	one(1,1,1);
	GvSFRotation	zeroRot; zeroRot.value.set(0,0,1,0);

	GvBool allDefaults = TRUE;
	g->checkForDefaultValues(1);
	if (g->translation == zero) g->translation.setDefault(TRUE); else allDefaults = FALSE;
	if (g->scale == one) g->scale.setDefault(TRUE); else allDefaults = FALSE;
	if (g->center == zero) g->center.setDefault(TRUE); else allDefaults = FALSE;

	if (g->rotation == zeroRot) g->rotation.setDefault(TRUE); else allDefaults = FALSE;
	if (g->scaleOrientation == zeroRot) g->scaleOrientation.setDefault(TRUE); else allDefaults = FALSE;

	return(allDefaults);
}

void PushMatrix(GOptimizeTraversal &state,Matrix &m,GvBool replace=FALSE) 
{
        GMatrixStack *current,*prev;
	    prev = (GMatrixStack*) state.Get(GMatrixStack::attrId);
	    current = (GMatrixStack*) prev->NewInstance();
	    if (replace)
			current->SetMatrix(m);
		else current->m.Multiply(m,prev->m);

	    state.Push(current);
		state.currentMatrix = current;

}

int Gv2Transform::Do(GOptimizeTraversal &state) 
{
	if (hasRoutes() && !state.ignoreRoutes) {
		
		state.animatedTransforms.add(this);
		// return(0);

		state.ReturnNode(this,GOptimizeTraversal::HAS_ROUTES | GOptimizeTraversal::HAS_GROUP);
		return(0);
	}

	// ignore if empty
	if (children.getNum() == 0) return(1);

	int ret;

    state.Push();
	ret = ApplyGeometrySensors(state);



	PushMatrix(state,Transform());

	//state.ReturnNode(this,GOptimizeTraversal::HAS_GROUP);
	//return(0);

	// check for keep transform 
	if (CheckForDefaults(this)|| state.applyTransforms) { // eliminate transform 

		ret = state.Do(children);

		GvBool doCopy = (state.resultMask & GOptimizeTraversal::KEEP_GROUP) !=0;

		if (doCopy) {
			Gv2Group *g = new Gv2Group();
			int mask = state.GetResult(g->children);
			state.AppendResult(g, GOptimizeTraversal::HAS_GROUP);
			ret=1;
		}
	}
	else {

		// keep node 

		Gv2Transform *g = new Gv2Transform();
		// copy fields
		g->translation = translation;
		g->rotation = rotation;
		g->scale = scale;
		g->scaleOrientation = scaleOrientation;
		g->center = center;
		CheckForDefaults(g);


		ret=state.Do(children,g->children,TRUE);
	
		state.AppendResult(g, GOptimizeTraversal::HAS_GROUP);
	}
	
	state.Pop();

	return(ret);


}
int Gv2MatrixTransform::Do(GOptimizeTraversal &state) 
{
	if (hasRoutes() && !state.ignoreRoutes) {
		
		state.animatedTransforms.add(this);
		// return(0);

		state.ReturnNode(this,GOptimizeTraversal::HAS_ROUTES | GOptimizeTraversal::HAS_GROUP);
		return(0);
	}

	// ignore if empty
	if (children.getNum() == 0) return(1);

	int ret;

    state.Push();
	ret = ApplyGeometrySensors(state);



	PushMatrix(state,Transform());

	//state.ReturnNode(this,GOptimizeTraversal::HAS_GROUP);
	//return(0);

	// check for keep transform 
	if (state.applyTransforms) { // eliminate transform 

		ret = state.Do(children);

		GvBool doCopy = (state.resultMask & GOptimizeTraversal::KEEP_GROUP) !=0;

		if (doCopy) {
			Gv2Group *g = new Gv2Group();
			int mask = state.GetResult(g->children);
			state.AppendResult(g, GOptimizeTraversal::HAS_GROUP);
			ret=1;
		}
	}
	else {

		// keep node 

		Gv2MatrixTransform *g = new Gv2MatrixTransform();
		// copy fields
		g->matrix = matrix;

		ret=state.Do(children,g->children,TRUE);
	
		state.AppendResult(g, GOptimizeTraversal::HAS_GROUP);
	}
	
	state.Pop();

	return(ret);


}

int Gv2Collision::Do(GOptimizeTraversal &state) 
{
	// ignore if empty
	if (children.getNum() == 0) return(1);

	int ret;


	if (state.bx3d) {
		Gv2Group *g = new Gv2Group();


		ret=state.Do(children,g->children,TRUE);
	
		state.AppendResult(g, GOptimizeTraversal::HAS_GROUP);
		return ret;

	}

	if (hasRoutes() && !state.ignoreRoutes) {
		state.ReturnNode(this,GOptimizeTraversal::HAS_ROUTES | GOptimizeTraversal::HAS_GROUP);
		return(0);
	}


    state.Push();
	ret = ApplyGeometrySensors(state);


	if (collide.get() == TRUE && (GvNode *) proxy == (GvNode *)NULL) {
		// EXPAND 
		ret =  state.Do(children);
	} else {

	// keep node 
	Gv2Collision *g = new Gv2Collision();

	// copy fields
	g->collide = collide;

	state.Do(proxy,g->proxy);

	ret=state.Do(children,g->children,TRUE);
	
	state.AppendResult(g, GOptimizeTraversal::HAS_GROUP);
	}

	state.Pop();

	return(ret);
}


int Gv2Inline::Do(GOptimizeTraversal &state) 
{
	if (hasRoutes() && !state.ignoreRoutes) {
		state.ReturnNode(this,GOptimizeTraversal::HAS_ROUTES | GOptimizeTraversal::HAS_GROUP);
		return(0);
	}
	
	if (!state.doInlines) {
		state.ReturnNode(this,GOptimizeTraversal::HAS_GROUP);
		return(0);
	}



	int ret;

    if (!loader.Ok()) { // we need to check the Loader state 
		if (state.loadInlines >= GTraversal::LOAD_IF_NEEDED) {
			Load(state);
		}
	}	


    state.Push();
	ret = ApplyGeometrySensors(state);

	// EXPAND 
	ret = state.Do(children);
	
	state.Pop();

	return(ret);
}


/*
  if there are sensors or anchors in group,
  put result node in anchor node
  or grpup node containing the sensors

*/


GvNode* GOptimizeTraversal::BuildAnchorOrSensorGroup(GvNode *n,int &mask)
{

	GvNode *result = n;

	if ( RTISA(n,Gv2Group) || RTISA(n,GvGroup) || RTISA(n,GvShape)) {

	} else return n;

	Gv2Group *resultGroup = NULL;

	Gv2Anchor *anchor = NULL;
	GvMFNode sensors(0);

		GvWWWAnchorElement *current;
		current = (GvWWWAnchorElement*) this->Get(GvWWWAnchorElement::attrId);
		int myLevel = current->level;

		// store all elements at this level !!!!!
		if (current)  {

			do {
				if (current->level != myLevel) break;
			   
				if (current->data) {
					if (RTISA(current->data,Gv2Anchor))
						anchor = (Gv2Anchor *) current->data;
					else if (bx3d) {
						if (RTISA(current->data,GvTouchSensor)) 
							sensors.append(current->data);
					}
					else sensors.append(current->data);
				}
					
					// follow next sensor / 
				current = (GvWWWAnchorElement*) current->prevThisAttr;


				} while(current != NULL);
			}
		
		if (anchor != NULL) {
			Gv2Anchor *g = new Gv2Anchor();
			// copy fields
			g->description = anchor->description;
			g->parameter = anchor->parameter;
			g->url = anchor->url;
			g->homeUrl = anchor->homeUrl;
			g->homeUrlInit = anchor->homeUrlInit;
			resultGroup = g;
			mask |= GOptimizeTraversal::ANCHOR_APPLIED;
		}

		if (sensors.getNum()>0) {
			if (resultGroup == NULL) resultGroup = new Gv2Group();
			mask |= GOptimizeTraversal::ANCHOR_APPLIED;
			resultGroup->children.add(&sensors);
		}
		
		if (resultGroup==NULL) return result;
		resultGroup->children.append(result);
		return resultGroup;
}


int Gv2Anchor::Do(GOptimizeTraversal &state) 
{
	if (hasRoutes() && !state.ignoreRoutes) {
		state.ReturnNode(this,GOptimizeTraversal::HAS_ROUTES | GOptimizeTraversal::HAS_GROUP);
		return(0);
	}

	// ignore if empty
	if (children.getNum() == 0) return(1);

	int ret = 0;

    state.Push();

		GvWWWAnchorElement *current,*prev;
		prev = (GvWWWAnchorElement*) state.Get(GvWWWAnchorElement::attrId);
		if (prev) {
			current = (GvWWWAnchorElement *) prev->NewInstance();								
			current->data = this;
			state.Push(GvWWWAnchorElement::attrId,current);	
		
			if (!homeUrlInit) { // get the URL of current scene / inline 
				homeUrl = state.GetHomeUrl();
				homeUrlInit = TRUE;
			}
		}

	ret = ApplyGeometrySensors(state);

	GvBool keepGroup = TRUE;

	//keepGroup = FALSE; // EXPAND IT for now 
	
	keepGroup = !state.expandAnchors || KeepGroup(children);

	if (!keepGroup) {
		ret= state.Do(children); // expand children
	}

	else {
		Gv2Anchor *g = new Gv2Anchor();

		// copy fields
		g->description = description;
		g->parameter = parameter;
		g->url = url;
		g->homeUrl = homeUrl;
		g->homeUrlInit = homeUrlInit;
		if ( state.applyTransforms) {
			 Matrix m;
			 m.Identity();

			 state.Push();
				PushMatrix(state,m,TRUE); //  new coordspace
				//ret = ApplyGeometrySensors(state);
	 			ret=state.Do(children,g->children,TRUE);
			state.Pop();

		} else {
	
			ret=state.Do(children,g->children,TRUE);
		}
	
		state.AppendResult(g, GOptimizeTraversal::HAS_GROUP |  GOptimizeTraversal::ANCHOR_APPLIED);
	}

	state.Pop();
	return(ret);
}


//
// class GOptimizeTraversal
//
RTIMP(GOptimizeTraversal,0,GRenderTraversal);


GOptimizeTraversal::GOptimizeTraversal() : GRenderTraversal()
,result(0),resultMask(0),imageTextures(0),materials(0),appearances(0),
coordinates(0),textureCoordinates(0),normals(0),colors(0),
animatedTransforms(0) 
{ 
  needShapes = TRUE; // we proabably need them 
  removeNodeNames = FALSE;
  doInlines = TRUE; 
  applyTransforms = TRUE; 
  expandAnchors = TRUE;

  expandAnchors = FALSE; // hg 

  expandLods = FALSE;
  buildBspTree = TRUE;

  buildBspTree = FALSE;
  keepPrimitives = FALSE;

  decomposeIfs  = FALSE;
  compressIfs  = TRUE;
  optimizeIfs  = TRUE;
  expandFcIfs = FALSE;
  unlitTextures = FALSE;

  bx3d=FALSE;
  vrml2=FALSE;

  doCoordRound= TRUE;
  coordRound = 0.001; // 1 mm

  doNormalRound= TRUE;
  normalRound = 0.01; 


  doTexCoordRound= TRUE;
  texCoordRound = 0.01; 

  doColorRound= TRUE;
  colorRound = 0.01; 

  doLookupCoord = FALSE;
  usePolygon = FALSE;

  loadInlines = GTraversal::LOAD_IF_NEEDED;

  updateBBox = TRUE;

  //xxx   decomposeIfs  = TRUE;

  includeInline = FALSE;	  // inline exapanded	
  includeExternProtos=FALSE;  // externProto ==> PROTO
  includeImageTextures=FALSE;  // ImageTexture ==> PixelTexture

  smoothCreaseAngle = 3.14;
  decomposeVolumeLimit = 1.0;

}


void GOptimizeTraversal::InitializeAttributes()
{	
 	GRenderTraversal::InitializeAttributes();
}

GOptimizeTraversal::~GOptimizeTraversal()
{
}

// append a resulting node to result list 
void GOptimizeTraversal::AppendResult(GvNode *node,int mask) 
{ 

	GvNode* n =node;
	if (! (mask & ANCHOR_APPLIED)) n= BuildAnchorOrSensorGroup(n,mask);
	result.append(n); 
	resultMask |= mask; 
}


BOOL IsIdentity(const Matrix &m) 
{
   for (int i = 0; i < 4; i++)
	for (int j = 0; j < 4; j++)
	    if ( !  (m.x[i][j] == ((i == j) ? 1.0 : 0.0))) return FALSE;
    return TRUE;
}


// return node 
int GOptimizeTraversal::ReturnNode(GvNode *node,int mask)
{	Matrix m;

	BOOL needTransform = FALSE;

	needTransform = !(mask & TRANSFORM_APPLIED); // && affectedByTransform(node)

	if (needTransform) {
        // Optimize using stack storage ??? 
        GMatrixStack *current;
	    current = (GMatrixStack*) this->Get(GMatrixStack::attrId);
	    m=current->m;

		needTransform = !IsIdentity(m);
	}


	if (needTransform) {
		Gv2Transform *t = new Gv2Transform();
		
		t->setMatrix(m);
		CheckForDefaults(t);

		t->children.set(node);
	    AppendResult(t,mask | TRANSFORM_APPLIED);

	}
	else AppendResult(node,mask);
	return(0);
}

// return node 
int GOptimizeTraversal::ReturnReplacedNode(GvNode *oldNode,GvNode *newNode,int mask)

{
	AppendResult(newNode,mask);
	return(0);
}

// return false if Node not supported by bx3d

GvBool GOptimizeTraversal::checkForBx3dNode(GNode *n)
{
	if (n == NULL) return FALSE;

	if (RTISA(n,GvScript)) return FALSE;
	if (RTISA(n,GvNormalInterpolator)) return FALSE;
	if (RTISA(n,GvNormal)) return FALSE;
	if (RTISA(n,Gv2TextureTransform)) return FALSE;
	if (RTISA(n,GvPointLight)) return FALSE;
	if (RTISA(n,GvSpotLight)) return FALSE;
	if (RTISA(n,GvBspGroup)) return FALSE;
	if (RTISA(n,GvBspTree)) return FALSE;
	if (RTISA(n,GvProto)) return FALSE;
	if (RTISA(n,GvExternProto)) return FALSE;
	if (RTISA(n,GvPixelTexture)) return FALSE;
	if (RTISA(n,GvMovieTexture)) return FALSE;

	if (ignoreRoutes && RTISA(n,GvSensor)) 
		return 0; // ignore

	return TRUE;
}



// true if node should go in bsp optimizer 
GvBool GOptimizeTraversal::UseNodeForBsp(GvNode *n)
{
	if (n == NULL) return FALSE;
	if (RTISA(n,GvShape)) return TRUE;
	if (RTISA(n,GvNodeShell)) return TRUE;
	if (RTISA(n,Gv2Group)) return TRUE; // anchor / collision / group 
	if (RTISA(n,Gv2LOD)) return TRUE;
	if (RTISA(n,Gv2Switch)) return TRUE;

	if (RTISA(n,GvGroup)) return TRUE;
	if (RTISA(n,GvVisibilitySensor)) return TRUE;
	return FALSE;
}


// do we need to keep the group e.g. if it contains
// geometry sensors or directional lights 
GvBool GOptimizeTraversal::NeedGroup(GvMFNode &in)
{
	return FALSE;
}


// store result as single node, and return mask
int GOptimizeTraversal::GetResult(GvNodeHandle &ret) 
{
		int retMask = resultMask;
		if (result.getNum() == 1) ret = result.get1(0);
		else if (result.getNum() == 0) ret = NULL;
		else {
			GvNode *n;
			n = MakeGroup(result);
			ret = n;
			retMask |= HAS_GROUP;
		}
		FlushResult();
		return(retMask);
}

// append result to MFNode, and return mask
// includes BSP tree building 
int GOptimizeTraversal::GetResultBsp(GvMFNode &ret) 
{
	int retMask = resultMask;


	if (buildBspTree) {
		GvMFNode nodesForBsp(0);

		for (int i= 0; i<result.getNum(); i++) {
			GvNode *n= result[i];

			if (UseNodeForBsp(n))
				nodesForBsp.add(n);
			else
				ret.add(n);
		}

		if (nodesForBsp.getNum()>0) {
			ret.add(BuildBspTree(nodesForBsp));
		}  
	}
	else 
		ret.add(&result);

	FlushResult();
	return(retMask);
}

int GOptimizeTraversal::GetResult(GvMFNode &ret) 
{
	int retMask = resultMask;
	ret.add(&result);
	FlushResult();
	return(retMask);
}


// process MFNode field and append all results 
int GOptimizeTraversal::Do(GvMFNode &n)
{
		int ret=0;
		int curIndex=0;

		while (curIndex < n.Length()) {
			GvNode *node = n.values[curIndex]; 
			if (node) 
				ret=node->Traverse(*this);
			curIndex ++;
		}
		return ret;
}

// process MFNode field and append all results to result
int GOptimizeTraversal::Do(GvMFNode &n,GvMFNode &result,GvBool buildBsp)
{

		GvMFNode oldResult(0);
		int oldResultMask;

		oldResultMask = GetResult(oldResult);

		int ret=0;
		int curIndex=0;

		while (curIndex < n.Length()) {
			GvNode *node = n.values[curIndex]; 
			if (node) 
				ret=node->Traverse(*this);
			curIndex ++;
		}

		if (buildBsp)
			ret=GetResultBsp(result);
		else ret=GetResult(result);

		SetResult(oldResult,oldResultMask);

		return ret;
}



int GOptimizeTraversal::Do(GvSFNode &n,GvSFNode &result)
{
		GvMFNode oldResult(0);
		int oldResultMask;

		oldResultMask = GetResult(oldResult);

		int ret=n.Traverse(*this);

		GetResult(result);

		SetResult(oldResult,oldResultMask);
		return ret;
}

int GOptimizeTraversal::Do(GvSFNode &n,GvMFNode &result)
{
		GvMFNode oldResult(0);
		int oldResultMask;

		oldResultMask = GetResult(oldResult);

		int ret=n.Traverse(*this);

		GetResult(result);

		SetResult(oldResult,oldResultMask);
		return ret;
}



// test if appearances matches for sorting 
int AppearanceMatch(GvAppearance *a,GvAppearance *b)
{
	if (a == b) return(1);
	if (a == NULL) return(0);
	if (b == NULL) return(0);

	if (a->texture != NULL && a->texture == b->texture) return(2);

	return(0);
}

int ShapeAppearanceMatch(GvNode *a,GvAppearance *b)
{
	if (RTISA(a,GvShape)) {
		return AppearanceMatch( (GvAppearance *) (GvNode*) (((GvShape *)a)->appearance),b);
	}
	else return(0);
}


// check if appearance already defined, and return looked up appearance
GvBool GOptimizeTraversal::Lookup(GvAppearance *old,GvAppearance **new_)
{

	// lookup matching appearance 

	for(int i=0; i<appearances.getNum();i++) {
		GvAppearance *m = (GvAppearance *) appearances.get1(i);
		if (old == m || 
			(old->material == m->material		// test for node equality 
			 && old->texture == m->texture
			 && old->textureTransform == m->textureTransform
			 )) 
		{
			*new_=m;			

			return(gtrue);
		}

	}
	appearances.append(old);
	*new_ = old;
	return gfalse;

}

GvBool GOptimizeTraversal::Lookup(GvImageTexture *old,GvImageTexture **new_)
{

	// lookup imageTexture 
	for(int i=0; i<imageTextures.getNum();i++) {
		GvImageTexture *m = (GvImageTexture *) imageTextures.get1(i);
		if (old == m || 
			(old->url == m->url
			 && old->repeatS == m->repeatS
			 && old->repeatT == m->repeatT
			 )) {
			*new_=m;			

			return(gtrue);
		}

	}

	// return this and add to list 
	imageTextures.append(old);

	*new_ = old;
	return gfalse;
}

GvBool GOptimizeTraversal::Lookup(GvCoordinate *old,GvCoordinate **new_)
{
	if (!old) { *new_ = old;return gfalse; }
	// lookup GvCoordinate 
	for(int i=0; i<coordinates.getNum();i++) {
		GvCoordinate *m = (GvCoordinate *) coordinates.get1(i);
		if (old == m || (old->point == m->point)) {
			*new_=m;			
			return(gtrue);
		}
	}

	// return old and add to list 
	coordinates.append(old);
	*new_ = old;
	return gfalse;
}
GvBool GOptimizeTraversal::Lookup(GvTextureCoordinate *old,GvTextureCoordinate **new_)
{
	if (!old) { *new_ = old;return gfalse; }
	// lookup GvTextureCoordinate 
	for(int i=0; i<textureCoordinates.getNum();i++) {
		GvTextureCoordinate *m = (GvTextureCoordinate *) textureCoordinates.get1(i);
		if (old == m || (old->point == m->point)) {
			*new_=m;			
			return(gtrue);
		}
	}

	// return old and add to list 
	textureCoordinates.append(old);
	*new_ = old;
	return gfalse;
}

GvBool GOptimizeTraversal::Lookup(GvNormal *old,GvNormal **new_)
{
	if (!old) { *new_ = old;return gfalse; }
	// lookup GvNormal 
	for(int i=0; i<normals.getNum();i++) {
		GvNormal *m = (GvNormal *) normals.get1(i);
		if (old == m || (old->vector == m->vector)) {
			*new_=m;			
			return(gtrue);
		}
	}

	// return old and add to list 
	normals.append(old);
	*new_ = old;
	return gfalse;
}
GvBool GOptimizeTraversal::Lookup(GvColor *old,GvColor **new_)
{
	if (!old) { *new_ = old;return gfalse; }
	// lookup GvColor 
	for(int i=0; i<colors.getNum();i++) {
		GvColor *m = (GvColor *) colors.get1(i);
		if (old == m || (old->color == m->color)) {
			*new_=m;			
			return(gtrue);
		}
	}

	// return old and add to list 
	colors.append(old);
	*new_ = old;
	return gfalse;
}

GvBool GOptimizeTraversal::LookupCoord(Gv2IndexedFaceSet *ifs)
{
	if (!doLookupCoord) 
		return gfalse;

	GvCoordinate *new_c= (GvCoordinate *) ifs->coord.get();
	
	if (Lookup(new_c,&new_c))
		ifs->coord.set(new_c);

	GvTextureCoordinate *new_vp= (GvTextureCoordinate *) ifs->texCoord.get();
	
	if (Lookup(new_vp,&new_vp))
		ifs->texCoord.set(new_vp);

	GvNormal *new_n = (GvNormal *) ifs->normal.get();
	
	if (Lookup(new_n,&new_n))
		ifs->normal.set(new_n);

	GvColor *new_vc= (GvColor *) ifs->color.get();
	
	if (Lookup(new_vc,&new_vc))
		ifs->color.set(new_vc);


	return gtrue;
}


int GvShape::Do(GOptimizeTraversal &state) 
{
	if (hasRoutes() && !state.ignoreRoutes) {
		state.AppendResult(this,GOptimizeTraversal::HAS_ROUTES);
		return(0);
	}

	// ignore if empty geometry field
	if (geometry.value == (GvNode *) NULL) 
		return(0);

	int ret = 0;
	state.Push(); // texture transform could get pushed 
	state.shape = this;


	if (state.decomposeIfs) { // IFS will be expanded !
		GvSFNode app;
		GvMFNode geoms(0);
		
		state.Do(appearance,app);
		
		ret = state.Do(geometry,geoms);

		int retmask = 0;
		if (ret & GOptimizeTraversal::TRANSFORM_APPLIED)
			retmask = GOptimizeTraversal::TRANSFORM_APPLIED;


		TRACE("Expanding shape into %d shapes \n",geoms.getNum());

		for (int i=0; i<geoms.getNum(); i++) {
			GvNode *a = app; 
			GvNode *n = geoms[i];
			if (RTISA(n,Gv2IndexedFaceSet))
			{
				Gv2IndexedFaceSet *f= (Gv2IndexedFaceSet *) n;
				if (!f->colorPerVertex.value && f->color.get() != (GvNode *) NULL)
				{
					// create a new appearance node
					// GvAppearance *newApp = new GvAppearance();

				}
			}

			if (RTISA(n,GvShape)) {
				state.AppendResult(n, GOptimizeTraversal::HAS_SHAPE|retmask);
			} else {
				GvShape *g;
				g = new GvShape();
				g->appearance.set(a);
				g->geometry.set(n);
				state.AppendResult(g, GOptimizeTraversal::HAS_SHAPE|retmask);
			}
		}
		ret = geoms.getNum()>0;

	} else {

	// keep node 
	GvShape *g = new GvShape();

	// copy fields
	state.Do(appearance,g->appearance);

#if 0	
	GvMFNode oldResult(0);
	int oldResultMask;
	oldResultMask = state.GetResult(oldResult);
	
	ret=geometry.Traverse(state);

	state.GetResult(result);

	state.SetResult(oldResult,oldResultMask);
#endif
//Do(GvSFNode &n,GvMFNode &result)
	ret = state.Do(geometry,g->geometry);

	int retmask = 0;

	if (ret & GOptimizeTraversal::TRANSFORM_APPLIED)
		retmask = GOptimizeTraversal::TRANSFORM_APPLIED;
	
	GvNode *geom = g->geometry.get();

	if (geom == NULL) {
		TRACE("Goptimize empty geometry in shape \n");
		delete  g;
		g = NULL;

	} else
	if (RTISA(geom,GvNodeShell)) {

	if (1) {
		// insert Shape after shape with same Appearance
		for(int i=0; i<  state.result.getNum();i++) {
			if (ShapeAppearanceMatch(state.result.get1(i),(GvAppearance *) (GvNode*) g->appearance)>0) {
				state.result.insert(i+1,g);
				state.resultMask |= GOptimizeTraversal::HAS_SHAPE | retmask;
				g = NULL;
				break;
			}
		}
		if (g) state.AppendResult(g, GOptimizeTraversal::HAS_SHAPE | retmask);
	}	
	else state.AppendResult(g, GOptimizeTraversal::HAS_SHAPE|retmask);
	}
	else 
	{	// something more complex 
		
		state.AppendResult(geom,retmask);
		delete  g;
		g = NULL;
	}

	state.shape = g;
	
	}

	state.shape = NULL;

	state.Pop();
	return(ret);
}



int GvAppearance::Do(GOptimizeTraversal &state) 
{
	if (state.appearance != this) {
		state.appearance = this;
		state.appearanceApplied = 0;
		// set node pointers
		state.material = (Gv2Material *) material.Eval(state);
		state.texture = (GvTexture* )texture.Eval(state);

	}
	
	if (material.get() == (GvNode*)NULL) material.setDefault(TRUE);
	if (texture.get() == (GvNode*)NULL) texture.setDefault(TRUE);
	if (textureTransform.get() == (GvNode*)NULL) textureTransform.setDefault(TRUE);


	if (hasRoutes() && !state.ignoreRoutes) {
		state.AppendResult(this,GOptimizeTraversal::HAS_ROUTES | GOptimizeTraversal::TRANSFORM_APPLIED);
		return(0);
	}


	// keep node 
	GvAppearance *g = new GvAppearance();
	
	g->ref();

	// copy fields
	state.Do(material,g->material);
	state.Do(texture,g->texture);
	state.Do(textureTransform,g->textureTransform);

	if (g->material.get() == (GvNode*)NULL) g->material.setDefault(TRUE);
	if (g->texture.get() == (GvNode*)NULL) g->texture.setDefault(TRUE);
	if (g->textureTransform.get() == (GvNode*)NULL) g->textureTransform.setDefault(TRUE);

	//checkForDefaultValues();

    if (state.applyTransforms || state.bx3d) {
		g->textureTransform.set((GvNode*)NULL);
		g->textureTransform.setDefault(TRUE);
    }



	// lookup matching appearance 

	for(int i=0; i<state.appearances.getNum();i++) {
		GvAppearance *m = (GvAppearance *) state.appearances.get1(i);
		if (this == m || g == m || 
			(g->material == m->material		// test for node equality 
			 && g->texture == m->texture
			 && g->textureTransform == m->textureTransform
			 )) {
			
			state.AppendResult(m,GOptimizeTraversal::TRANSFORM_APPLIED);
			g->unref();

			return(1);
		}

	}


	state.appearances.append(g);
	state.AppendResult(g,GOptimizeTraversal::TRANSFORM_APPLIED);

	if (state.appearance != g) {
		state.appearance = g;
		state.appearanceApplied = 0;	
		// set node pointers
		state.material = (Gv2Material *) material.Eval(state);
		state.texture = (GvTexture* )texture.Eval(state);

	}
	g->unref();
	return(1);
}


int Gv2Material::Do(GOptimizeTraversal &state) 
{

	if (state.bx3d) {
		const Point zeroPoint(0,0,0);
		const Point gray(0.8,0.8,0.8);

		ambientIntensity.set(0.2); ambientIntensity.setDefault(TRUE);
	
		if (diffuseColor.getPoint() == gray) diffuseColor.setDefault(TRUE);

		emissiveColor.set(zeroPoint); emissiveColor.setDefault(TRUE);
		specularColor.set(zeroPoint); specularColor.setDefault(TRUE);

		shininess.set(0.2); shininess.setDefault(TRUE);
		if (transparency.get() == 0.0) transparency.setDefault(TRUE);
	} else {
		checkForDefaultValues(0);
	}

	if (hasRoutes() && !state.ignoreRoutes) {
		state.AppendResult(this,GOptimizeTraversal::HAS_ROUTES | GOptimizeTraversal::TRANSFORM_APPLIED);
		return(0);
	}

#ifdef _D3D
	handle.Release();
#endif


	// lookup material 

	for(int i=0; i<state.materials.getNum();i++) {
		Gv2Material *m = (Gv2Material *) state.materials.get1(i);
		if (this == m || 
			(this->ambientIntensity == m->ambientIntensity
			 && this->ambientIntensity == m->ambientIntensity
			 && this->diffuseColor == m->diffuseColor
			 && this->specularColor == m->specularColor
			 && this->emissiveColor == m->emissiveColor
			 && this->shininess == m->shininess
			 && this->transparency == m->transparency
			 )) {
			state.AppendResult(m);
			return(1);
		}

	}

	// return this and add to list 
	state.materials.append(this);
	state.AppendResult(this);

	return(1);
}

int GvImageTexture::Do(GOptimizeTraversal &state) 
{
	if (hasRoutes() && !state.ignoreRoutes) {
		state.AppendResult(this,GOptimizeTraversal::HAS_ROUTES);
		return(0);
	}

	if (state.bx3d) {
		repeatS.set(TRUE); repeatS.setDefault(TRUE);
		repeatT.set(TRUE); repeatT.setDefault(TRUE);
	}
	else {
		if (repeatS.get()) repeatS.setDefault(TRUE);
		if (repeatT.get()) repeatT.setDefault(TRUE);

	}

	// lookup imageTexture 

	for(int i=0; i<state.imageTextures.getNum();i++) {
		GvImageTexture *m = (GvImageTexture *) state.imageTextures.get1(i);
		if (this == m || 
			(this->url == m->url
			 && this->repeatS == m->repeatS
			 && this->repeatT == m->repeatT
			 )) {
			state.AppendResult(m);
			return(1);
		}

	}

	// return this and add to list 
	state.imageTextures.append(this);
	state.AppendResult(this);

	return(1);
}



int GvPolygon::Do(GOptimizeTraversal &state)
{	
	if (hasRoutes() && !state.ignoreRoutes) {
		state.AppendResult(this,GOptimizeTraversal::HAS_ROUTES | GOptimizeTraversal::KEEP_TRANSFORM);
		return(0);
	}

	// ignore if empty 
	if (coordIndex.getNum() <=1) return(0);

	if (!theShell) BuildShell(state);
	if (!theShell) return (0);

	Gv2IndexedFaceSet *result=NULL; // resulting node 

	Matrix *m= NULL;
	Matrix *tm= NULL;

	if (state.applyTransforms) {
		m = state.GetMatrix();
		tm = state.GetTextureMatrix();
	}

	TRACE("Shell #nfaces %d  nverts %d %s \n",theShell->GetFaceCount(),theShell->v.Length(),theShell->ClassName());

	if (state.decomposeIfs) {
		if (RTISA(theShell,GShell)) {
			ExpandIndexedFaceSet(state,(GShell*) theShell, !ccw, solid, m,tm, state.result);
			state.resultMask |=  GOptimizeTraversal::HAS_SHAPE | (state.applyTransforms ?  GOptimizeTraversal::TRANSFORM_APPLIED : 0);
			return(1);
		}
	}

	if (RTISA(theShell,GShellI)) {
		GShellI *shellI = (GShellI*) theShell;

		if (state.optimizeIfs && shellI->fc.Length() == 0) {
			shellI->f.Optimize();
		}
		if (state.doCoordRound) {
			GRound(shellI->v,state.coordRound,1.0f/state.coordRound);
		}


		if (state.compressIfs) {
			GShellI *newS = NULL;
			TRACE("Compressing shell : facelist length = %d, verts %d v %d n %d p%d c%d \n",
				shellI->f.Length(),shellI->verts.Length(),shellI->v.Length(),shellI->vn.Length(),shellI->vp.Length(),shellI->vc.Length());
			newS = CompressShell(shellI,NULL,1,state.doCoordRound);
			SetShell(newS);
			shellI = (GShellI*) theShell;
			TRACE("Compressed Shell : facelist length = %d, verts %d v %d n %d p%d c%d \n",
				shellI->f.Length(),shellI->verts.Length(),shellI->v.Length(),shellI->vn.Length(),shellI->vp.Length(),shellI->vc.Length());
		}	
		result = CreateIndexedFaceSet((GShellI*) theShell,!ccw, m,tm,solid,state.bx3d);
	}
	else if (RTISA(theShell,GShell)) {
		GShell *s = (GShell*) theShell;

		if (state.doCoordRound) {
			GRound(s->v,state.coordRound,1.0f/state.coordRound);

			// relookkup ???????????
		}
		
		if (state.optimizeIfs && s->fc.Length() == 0) {
			s->f.Optimize();
		}
		result = CreateIndexedFaceSet(s,!ccw, m,tm);
	}

	if (result) {
		if (!solid) result->solid.set(FALSE);
		// if (!ccw) result->ccw.set(FALSE); was wrapped above 
		// shell is triangulated so not convex 
		// creaseAngle 
		
		if (state.doLookupCoord) 
			state.LookupCoord(result); // xxx

		if (state.buildBspTree) { // we need the shell 
			result->BuildShell(state);
		}

	}

	state.AppendResult(result, GOptimizeTraversal::HAS_SHAPE | (state.applyTransforms ?  GOptimizeTraversal::TRANSFORM_APPLIED : 0));

	return(1);

}



int Gv2IndexedFaceSet::Do(GOptimizeTraversal &state) 
{
	if (hasRoutes() && !state.ignoreRoutes) {
		state.AppendResult(this,GOptimizeTraversal::HAS_ROUTES | GOptimizeTraversal::KEEP_TRANSFORM);
		return(0);
	}

	// ignore if empty 
	if (coordIndex.getNum() <=1) return(0);

	if (!theShell) BuildShell(state);
	if (!theShell) return (0);

	Gv2IndexedFaceSet *result=NULL; // resulting node 

	Matrix *m= NULL;
	Matrix *tm= NULL;

	if (state.applyTransforms) {
		m = state.GetMatrix();
		tm = state.GetTextureMatrix();
	}

	TRACE("Shell #nfaces %d  nverts %d %s \n",theShell->GetFaceCount(),theShell->v.Length(),theShell->ClassName());

	if (state.decomposeIfs) {
		if (RTISA(theShell,GShell)) {
			ExpandIndexedFaceSet(state,(GShell*) theShell, !ccw, solid, m,tm, state.result);
			state.resultMask |=  GOptimizeTraversal::HAS_SHAPE | (state.applyTransforms ?  GOptimizeTraversal::TRANSFORM_APPLIED : 0);
			return(1);
		}
	}

	if (RTISA(theShell,GShellI)) {
		GShellI *shellI = (GShellI*) theShell;

		if (state.optimizeIfs && shellI->fc.Length() == 0) {
			shellI->f.Optimize();
		}
		if (state.doCoordRound) {
			GRound(shellI->v,state.coordRound,1.0f/state.coordRound);
		}



		if (!colorPerVertex && (shellI->fc.Length()>0) && state.expandFcIfs) {
			shellI->vc.SetLength(0); // flush computed vertex colors

			ExpandFaceColors(shellI, state.material,state.texture,state.textureTransform, solid,!ccw,RTISA(theShell,GPolylineI),state.result);

			state.resultMask |= GOptimizeTraversal::HAS_SHAPE | (state.applyTransforms ?  GOptimizeTraversal::TRANSFORM_APPLIED : 0);
			return 1;
		}

		if (state.compressIfs) {
			GShellI *newS = NULL;
			TRACE("Compressing shell : facelist length = %d, verts %d v %d n %d p%d c%d \n",
				shellI->f.Length(),shellI->verts.Length(),shellI->v.Length(),shellI->vn.Length(),shellI->vp.Length(),shellI->vc.Length());
			newS = CompressShell(shellI,NULL,1,state.doCoordRound);
			SetShell(newS);
			shellI = (GShellI*) theShell;
			TRACE("Compressed Shell : facelist length = %d, verts %d v %d n %d p%d c%d \n",
				shellI->f.Length(),shellI->verts.Length(),shellI->v.Length(),shellI->vn.Length(),shellI->vp.Length(),shellI->vc.Length());
		}	
		result = CreateIndexedFaceSet((GShellI*) theShell,!ccw, m,tm,solid,state.bx3d);
	}
	else if (RTISA(theShell,GShell)) {
		GShell *s = (GShell*) theShell;

		if (state.doCoordRound) {
			GRound(s->v,state.coordRound,1.0f/state.coordRound);

			// relookkup ???????????
		}
		
		if (state.optimizeIfs && s->fc.Length() == 0) {
			s->f.Optimize();
		}

		if (state.usePolygon && (s->GetFaceCount() == 1 )) {  // convert to Polygon node 

			GvPolygon *result = CreatePolygon(s,!ccw, m,tm,solid);
			if (result) {
				// state.LookupCoord(result); // xxx
				
				if (state.doLookupCoord) 
				for (int i=0; i<result->texCoord.getNum();i++) {
					GvTextureCoordinate *new_vp= (GvTextureCoordinate *) result->texCoord.get1(i);
	
					if (state.Lookup(new_vp,&new_vp))
						result->texCoord.set1(i,new_vp);
				}


				if (state.buildBspTree) { // we need the shell 
					result->BuildShell(state);
				}

				state.AppendResult(result, GOptimizeTraversal::HAS_SHAPE | (state.applyTransforms ?  GOptimizeTraversal::TRANSFORM_APPLIED : 0));

				return(1);

			}
		}
		result = CreateIndexedFaceSet(s,!ccw, m,tm);

	}
	if (result) {
		if (!solid) result->solid.set(FALSE);
		// if (!ccw) result->ccw.set(FALSE); was wrapped above 
		// shell is triangulated so not convex 
		// creaseAngle 
		
		state.LookupCoord(result); // xxx

		if (state.buildBspTree) { // we need the shell 
			result->BuildShell(state);
		}

	}

	state.AppendResult(result, GOptimizeTraversal::HAS_SHAPE | (state.applyTransforms ?  GOptimizeTraversal::TRANSFORM_APPLIED : 0));

	return(1);
}

int GvNurbsSurface::Do(GOptimizeTraversal &state) 
{
	if (hasRoutes() && !state.ignoreRoutes) {
		state.AppendResult(this,GOptimizeTraversal::HAS_ROUTES | GOptimizeTraversal::KEEP_TRANSFORM);
		return(0);
	}

	if (state.keepPrimitives) {
		Shapify(state,this,GOptimizeTraversal::KEEP_TRANSFORM);
		//state.AppendResult(s, GOptimizeTraversal::KEEP_TRANSFORM);
		return(0);
	}

	// ignore if empty 
	if (controlPoint.getNum() <=1) return(0);

	if (!theShell) BuildShell(state);
	if (!theShell) return (0);

	Gv2IndexedFaceSet *result=NULL; // resulting node 

	Matrix *m= NULL;
	Matrix *tm= NULL;

	if (state.applyTransforms) {
		m = state.GetMatrix();
		tm = state.GetTextureMatrix();
	}

	TRACE("Shell #nfaces %d  nverts %d %s \n",theShell->GetFaceCount(),theShell->v.Length(),theShell->ClassName());

	if (state.decomposeIfs) {
		if (RTISA(theShell,GNurbsShell)) {
			GNurbsShell *s = (GNurbsShell *) theShell;

			// use actual tessellation 
			if (!s->IsShellTessellationOk())
				s->ComputeShell();

			ExpandIndexedFaceSet(state,(GShell*) theShell, !ccw, solid, m,tm, state.result);
			state.resultMask |=  GOptimizeTraversal::HAS_SHAPE | (state.applyTransforms ?  GOptimizeTraversal::TRANSFORM_APPLIED : 0);
			return(1);

		} else
		if (RTISA(theShell,GShell)) {
			ExpandIndexedFaceSet(state,(GShell*) theShell, !ccw, solid, m,tm, state.result);
			state.resultMask |=  GOptimizeTraversal::HAS_SHAPE | (state.applyTransforms ?  GOptimizeTraversal::TRANSFORM_APPLIED : 0);
			return(1);
		}
	}
		if (RTISA(theShell,GNurbsShell)) {
			GNurbsShell *s = (GNurbsShell *) theShell;

			// use actual tessellation 
			if (!s->IsShellTessellationOk())
				s->ComputeShell();
		}

	result = CreateIndexedFaceSet(theShell,!ccw, m,tm);

	if (result) {
		if (!solid) result->solid.set(FALSE);
		// if (!ccw) result->ccw.set(FALSE); was wrapped above 
		// shell is triangulated so not convex 
		// creaseAngle 
		
		state.LookupCoord(result); // xxx

		if (state.buildBspTree) { // we need the shell 
			result->BuildShell(state);
		}

	}

	state.AppendResult(result, GOptimizeTraversal::HAS_SHAPE | (state.applyTransforms ?  GOptimizeTraversal::TRANSFORM_APPLIED : 0));

	return(1);
}

int GvTrimmedSurface::Do(GOptimizeTraversal &state) 
{
	if (hasRoutes() && !state.ignoreRoutes) {
		state.AppendResult(this,GOptimizeTraversal::HAS_ROUTES | GOptimizeTraversal::KEEP_TRANSFORM);
		return(0);
	}

	// ignore if empty 
	if (!surface) return(0);

	//if (state.keepPrimitives) 
	{
		Shapify(state,this,GOptimizeTraversal::KEEP_TRANSFORM);
		//state.AppendResult(s, GOptimizeTraversal::KEEP_TRANSFORM);
		return(1);
	}


}


// create VRML 2.0 Material out of VRML 1 one 
static int  CreateMaterial(GOptimizeTraversal &state, GvMaterial *me,GvSFNode  &mat)
{ 
	Point am,di,sp,em;
	float a;

	if (me == NULL) return 0;

	if (me->transparency.Length()>0) {
		a = me->transparency[0];
	} else a = 0.0;
	float power;
	if (me->shininess.Length()>0) 
        power = me->shininess[0];
    else power = 0.03; // ??


	me->ambientColor.get1(0,am);
	me->diffuseColor.get1(0,di);
	me->specularColor.get1(0,sp);
	me->emissiveColor.get1(0,em);

	float ambientIntensity=0.0;

	if (di.x >0) ambientIntensity =  am.x / di.x;
	else if (di.y >0) ambientIntensity =  am.y / di.y;
	else if (di.z >0) ambientIntensity =  am.z / di.z;

	// lookup material 
	for(int i=0; i<state.materials.getNum();i++) {
		Gv2Material *m = (Gv2Material *) state.materials.get1(i);
		if (ambientIntensity == m->ambientIntensity
			 && di == m->diffuseColor
			 && sp == m->specularColor
			 && em == m->emissiveColor
			 && power == m->shininess
			 && a == m->transparency
			 ) {
				mat = m;
				return(1);
		}

	}

	Gv2Material *m = new Gv2Material();
	m->ambientIntensity.set(ambientIntensity);
	m->diffuseColor.set(di);
	m->specularColor.set(sp);
	m->emissiveColor.set(em);
	m->transparency.set(a);
	m->shininess.set(power);
	m->checkForDefaultValues(1);

	// return this and add to list 
	state.materials.append(m);
	mat = m;
	
	return 1;

}



// check for VRML 1 shapes & wrap into Shape
//
void Shapify(GOptimizeTraversal &state,GvNode *result,int flags)
{
	if ( (state.shape == NULL) || (state.shape == (GvNode*) -1) ) { // VRML 1.0, no Shape 

		G_GETSTATE(state,matB,GvMaterialBinding,	GvMaterialBindingElement)
		G_GETSTATE(state,shapeHints,GvShapeHints,	GvShapeHintsElement)
		G_GETSTATE(state,texture2,GvTexture2,	GvTexture2Element)


		GvMaterial *material=NULL; 
		GvMaterialElement *materialTop = (GvMaterialElement*) state.Get(GvMaterialElement::attrId); 
		if (materialTop) material = (GvMaterial *) materialTop->data;

		GvShape *g;

		g = new GvShape();

		GvAppearance *a = state.appearance;
		
		if (a == NULL) {			
			a= new GvAppearance();

			if (state.applyTransforms) {
				a->textureTransform.set((GvNode*)NULL);
				a->textureTransform.setDefault(TRUE);
			}

			if (material)
				CreateMaterial(state, material,a->material);
			if (texture2) { 
				GvImageTexture *n = new GvImageTexture();
				n->url.set_(texture2->filename);
				if (texture2->wrapS == GvTexture2::CLAMP) n->repeatS.set(FALSE);
				if (texture2->wrapT == GvTexture2::CLAMP) n->repeatT.set(FALSE);
				n->ref();
				GvImageTexture *new_a;

				state.Lookup(n,&new_a);
				
				if (new_a) a->texture.set(new_a); 
				n->unref();


			}
		// TO DO: texture, LOOK UP APPEARANCE 
			a->ref();
		} else a->ref();
		
		GvAppearance *new_a;

		state.Lookup(a,&new_a);

		if (new_a) g->appearance.set(new_a); 
		
		a->unref();

		g->geometry.set(result);
		state.ReturnNode(g, GOptimizeTraversal::HAS_SHAPE| flags);
	}
	else 
		state.ReturnNode(result, GOptimizeTraversal::HAS_SHAPE| flags);
}



int GvNodeShell::Do(GOptimizeTraversal &state) 
{

	// could be builtin node, eg Sphere / VRML 1.0 node etc
	// 
	if (hasRoutes() && !state.ignoreRoutes) {
		state.AppendResult(this,GOptimizeTraversal::HAS_ROUTES| GOptimizeTraversal::KEEP_TRANSFORM);
		return(0);
	}

	// ignore if empty 

	if (!theShell) BuildShell(state);
	if (!theShell) return (0);

	Gv2IndexedFaceSet *result=NULL; // resulting node 

	Matrix *m= NULL;
	Matrix *tm= NULL;

	if (state.applyTransforms) {
		m = state.GetMatrix();
		tm = state.GetTextureMatrix();
	}

	if (RTISA(theShell,GShellI)) {
		GShellI *s = (GShellI*) theShell;
		if (state.doCoordRound) {
			//Round(s->v,coordRound);
		}
		
		if (state.optimizeIfs && s->fc.Length() == 0) {
			s->f.Optimize();
		}
		result = CreateIndexedFaceSet(s,state.cw, m,tm);
	}
	else if (RTISA(theShell,GNurbsShell)) {
			GNurbsShell *s = (GNurbsShell *) theShell;

			// use actual tessellation 
			if (!s->IsShellTessellationOk())
				s->ComputeShell();
			
			result = CreateIndexedFaceSet(s,state.cw, m,tm);

	}
	else if (RTISA(theShell,GMesh)) {
		((GMesh*) theShell)->ComputeFaceList();

		result = CreateIndexedFaceSet((GShell*) theShell,state.cw, m,tm);
	}
	else if (RTISA(theShell,GShell)) {
		GShell *s = (GShell*) theShell;
		
		if (state.optimizeIfs && s->fc.Length() == 0) {
			s->f.Optimize();
		}

		result = CreateIndexedFaceSet(s,state.cw, m,tm);
	}
	if (result) {
		if (!state.solid) result->solid.set(FALSE);
		// if (!ccw) result->ccw.set(FALSE); was wrapped above 
		// shell is triangulated so not convex 
		// creaseAngle 
		
		state.LookupCoord(result); // xxx

		if (state.buildBspTree) { // we need the shell 
			result->BuildShell(state);
		}

	}

	// TBD: decompose where ??? 

	Shapify(state,result, GOptimizeTraversal::HAS_SHAPE| (state.applyTransforms ?  GOptimizeTraversal::TRANSFORM_APPLIED : 0));

	return(1);
}


// Gv2TextureTransform
int Gv2TextureTransform::Do(GOptimizeTraversal &state)   
{ 

	int apply = state.bx3d; 

	if (!apply && hasRoutes() && !state.ignoreRoutes) {
		state.AppendResult(this,GOptimizeTraversal::HAS_ROUTES | GOptimizeTraversal::KEEP_TRANSFORM);
		return(0);
	}
	
	if (!apply && !state.applyTransforms) {
		state.AppendResult(this);

	} else {



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
	current->m.Multiply(m,prev->m);

	state.Push(current);
	}
	return(1);
}

#endif // GOPT 
