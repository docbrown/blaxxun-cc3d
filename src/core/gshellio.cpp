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

@module GShellIO.cpp - GLView additional GShell IO functions implementation |

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Additional member functions for GShell GMesh (GShell.cpp) are
implemented here, but defined in GShell.h.
GShell
	// additional io routines implemented in GShellIO.CPP
#ifdef _HMF
	// write shell to AStream in HMF format
	virtual int WriteHmf(AStream &a);
#endif

	// write to GEO polygon format (optional with normals)
	int WriteGeo(AStream &a,int outputNormals=0);

	// write to STL triangle format
	int WriteStl(AStream &a)

	// write to POV Ray-tracer format
	virtual int WritePov(AStream &a);
	
	// write to RAW triangle format
	virtual int WriteRaw(AStream &a);
#ifdef _VRML
	// write to AStream in VRML format
	virtual int WriteVrml(AStream &a);
#endif

GMesh
	// additional io routines implemented in GShellIO.CPP
#ifdef _HMF
	// write mesh to AStream in HMF format
	int WriteHmf(AStream &a);
#endif
#ifdef _VRML
	// write to AStream in VRML format
	virtual int WriteVrml(AStream &a);
#endif

GShellI

#ifdef _VRML
	// write to AStream in VRML format
	virtual int WriteVrml(AStream &a);
#endif




Changes:

Todo :

******************************************************************************/

#include "stdafx.h"
#include "gversion.h"

#include "gclass.h"
#include "gshell.h"
#include "gshellop.h"
#include "gshelli.h"

#include "gshellio.h"

#ifdef _VRML
#include "gvnode.h"
#include "gvstring.h"
#include "giotraversal.h"
#endif



#ifdef _HMF
// write shell to AStream in HMF format
int GShell::WriteHmf(AStream &a)
{
 a.BeginBlock();
 a.io("Geometry"); 

 a.BeginBlock();
   a.io("Shell"); a.nl();
   v.io(a);
   a.nl();
   f.io(a);
 a.EndBlock(); // Shell
 a.EndBlock(); // Geometry
 return(a.stat);
}

GMesh::WriteHmf(AStream &a)
{
 a.BeginBlock();
   a.io("Mesh"); a.nl();

   int r,c;
   int vi=0;
   a.BeginBlock();

   for (r=0; r<nrows; r++) {
	a.BeginBlock();
	  for (c=0; c<ncols; c++) {
		v[vi].io(a);
		vi++;
	  }
	a.EndBlock();
	a.nl();
   }
   a.EndBlock();
 a.EndBlock();
 a.nl();
 return(a.stat);
}
#endif

#ifdef _RAW
// write to RAW triangle format
int
GShell::WriteRaw(AStream &a)
{
  GFaceList tris;
  IntArray tri2face;
  int ntris=ComputeTriangles(tris,tri2face); // compute all triangles for object
  int *fp = tris;
  int *fpend = fp+tris.Length();
  int vi; // vertex index
  GVertex v; // vertex data
  while (fp<fpend) {
	  int n= *fp++;	// #3
	  int i;
	  for(i=0;i<n;i++) {
		 vi = *fp++;  GetVertex(vi,v); fprintf(a.f," %g %g %g",v.v.x,v.v.y,v.v.z);
	  }
	  fprintf(a.f,"\n");
  }
  return(a.stat);
}
#endif

#define VECX(XX) (XX).x,(XX).y,(XX).z

#ifdef _GEO
// write to GEO polygon format, ok for GShell, GMesh, GShellI
int
GShell::WriteGeo(AStream &a,int outputNormals)
{
  GFaceList *flist = &f;
  GFaceList tris;
  IntArray tri2face;
  // to do GShellI ???????????????
  
  if ((flist->Length() == 0) || (flags & SHELL_HAS_HOLE_FACES)) {
     int ntris=ComputeTriangles(tris,tri2face); // compute all triangles for object
	 flist = &tris;
  }

  int *fp = *flist;
  int *fpend = fp+ flist->Length();
  int vi; // vertex index
  int nv = v.Length();
  int i;


  // vertex & face count 
  fprintf(a.f,"%d %d 1\n",nv,flist->GetFaceCount());

  // vertices
  for(i=0; i<nv;i++) {
    GVertex v; // vertex data
	GetVertex(i,v); fprintf(a.f," %g %g %g",v.v.x,v.v.y,v.v.z);
  	if (outputNormals) fprintf(a.f,"  %g %g %g",v.v.x,v.n.y,v.n.z);
  	fprintf(a.f,"\n");
  }

  // faces
  while (fp<fpend) {
	  int  n= *fp++;	
	  int i;
	  if (n<0) n = -n;
      
      fprintf(a.f,"%d",n); // vertex cnt in this face

	  for(i=0;i<n;i++) {
		 vi = *fp++; 
	  	 fprintf(a.f," %d",vi+1);
	  }
	  fprintf(a.f,"\n");
  }
  return(a.stat);
}

#endif


#ifdef _STL
// write to STL triangle format

int
GShell::WriteStl(AStream &a)
{
  GFaceList tris;
  IntArray tri2face;
  int ntris=ComputeTriangles(tris,tri2face); // compute all triangles for object
  int *fp = tris;
  int *fpend = fp+tris.Length();
  int vi; // vertex index
  int facei=0;
  GVertex v; // vertex data
  
  fprintf(a.f,"solid %s\n","one");

  while (fp<fpend) {
	  int n= *fp++;	// #3
	  int i;
      fprintf(a.f," facet ");

	  if (fn) {
	    int fni = tri2face[facei];
	    fprintf(a.f,"normal %g %g %g\n",fn[fni].x,fn[fni].y,fn[fni].z);
	  }

      fprintf(a.f,"  outer loop\n");

	  for(i=0;i<n;i++) {
		 vi = *fp++;  GetVertex(vi,v); fprintf(a.f,"   vertex %g %g %g\n",v.v.x,v.v.y,v.v.z);
	  }
      fprintf(a.f,"  endloop\n");


      fprintf(a.f," endfacet\n");
	  facei++;
  }
  fprintf(a.f,"end solid\n");

  return(a.stat);
}


#if 0
// read a STL Ascii Triangle file 
int ReadStlFile(const char *FileName) 
{
  FILE *f;
  f = fopen(FileName,"r");
  if (f == NULL) return(-1);

  int inSolid = 0;
  int inFacet = 0;
  char partName[160];
  
  int pcnt=0;	
  int pi=0;
  Point p[4];
  Point n;

  Point * pbuf;		  // vertices	
  int pbufCnt = 0;	 // counter of all vertices
  int pbufMax = 4096;	// allocated size
  
  pbuf = (Point *) malloc(pbufMax * sizeof(Point));


  while (!feof(f)) {
	char tok[80];
 
 	if	(fscanf(f,"%s",tok) != 1) 	continue; 
	strlwr(tok);

	switch (tok[0]) {
	case 's' :
	if (streq("tok","solid")) {
	   inSolid = 1;
	     // partname
   	    if	(fscanf(f,"%s",partName) != 1) 	continue; 
		strlwr(tok);
	break;

	} 
	
	case 'e' :
	if (streq("tok","endsolid")) {
		inSolid = 0;
	break;
	} 
	else if (streq("tok","end")) {
   	    if	(fscanf(f,"%s",tok) != 1) 	continue; 
		strlwr(tok);
		if (streq(tok,"solid")) inSolid = 0;
	break;
	} 
	else if (streq("tok","endfacet")) {
		// store the 3 coordinates
		if ((pbufMax + 3) >= pbufCnt) {
			pbufMax +=1024;
			pbuf = (Point *) realloc(pbuf,pbufMax * sizeof(Point));

		}

		pbuf[pbufCnt] = p[0]; pbufCnt ++;
		pbuf[pbufCnt] = p[1]; pbufCnt ++;
		pbuf[pbufCnt] = p[2]; pbufCnt ++;

		inFacet = 0;
	break;
	} 
	else if (streq("tok","endloop")) {
	break;
	} 

	case 'f' :

	if (streq("tok","facet")) {
	   inFacet = 1;
	   pi=0;
	   break;
	} 

	case 'l' :
	if (streq("tok","loop")) {
	break;
	} 

	case 'v' :
	if (streq("tok","vertex")) {
		 if (pi >= 3) pi=0; // to many data, should not occur
		 fscanf(f,"%f %f %f",&p[pi].x,&p[pi].y,&p[pi].z);
		 pi++;
		 break;
	} 
	case 'n' :

	if (streq("tok","normal")) {
		 fscanf(f,"%f %f %f",&n.x,&n.y,&n.z);
		 break;
	} 
	
	default :
	{
		TRACE("Unknown Token : %s \n",tok);
	}
	break;
	} // switch 

  }

  TRACE("%s Triangles read : %d \n",pbufCnt / 3 );

  // build facelist 
  {
    int *f; //Facelist
	int fcnt = (pbufCnt/3) * 4;

	f = (int *) malloc(fcnt*sizeof(int));
	int *fp=f;
	int i=0;

	while (i<pbufCnt) {
		*fp++=3;
		*fp++ = i++;
		*fp++ = i++;
		*fp++ = i++;
	}

	if (1) {  // minimize 
      int npout;
	  Point *pout = (Point *) malloc(pbufCnt * sizeof(Point));
      int nfout;
	  int *fout = (int *) malloc(fcnt * sizeof(int));

      HC_Compute_Minimized_Shell(pbufCnt,pbuf,fcnt,f,"orphan elemination = ??",
      				&npout,pout,&nfout,fout,NULL,NULL);
	  HC_Insert_Shell(npout,pout,nfout,fout);
	  free(fout); free(pout);

	} else {
	  HC_Insert_Shell(pbufCnt,pbuf,fcnt,f);
	}
	
	free(f);  
  }
  free(pbuf);
  fclose(f);
  
  return(0);
}

#endif
#endif


//*****************************************************************************************

#ifdef _POV

void pov_BEGIN_OBJECT(AStream &a)  
{ fprintf(a.f,"object ");
  a.BeginBlock();
}

void pov_END_OBJECT(AStream &a) 
{ 
  a.EndBlock();
  a.nl();
}

void pov_color(AStream &a,float r,float g,float b)
{
   fprintf(a.f,"\tcolor red %g green %g blue %g\n",r,g,b);
}

void pov_translate(AStream &a,float x,float y,float z)
{
   fprintf(a.f,"\tTRANSLATE <%g, %g, %g>\n",x,y,z);
}

void pov_sphere(AStream &a,float x,float y,float z,float r)
{
   fprintf(a.f,"\tsphere { <%g, %g, %g> %g }\n",x,y,z,r);
}

void pov_box(AStream &a,Point *p1, Point *p2)
{
   fprintf(a.f,"\tbox { <%g, %g, %g> <%g, %g, %g>  }\n",VECX(*p1),VECX(*p2));
}

void pov_bounding_box(AStream &a,Point *p1, Point *p2)
{
   fprintf(a.f,"\tbounded_by { box { <%g, %g, %g> <%g, %g, %g>  }}\n",VECX(*p1),VECX(*p2));
}

void pov_point_light(AStream &a,Point *p,Point *c)
{
  pov_BEGIN_OBJECT(a);
    fprintf(a.f,"\tlight_source {");
    fprintf(a.f," <%g, %g, %g>\n",VECX(*p)); /* position */
    pov_color(a,VECX(*c));
/*
    fprintf(povout,"\tAMBIENT 1.0");
    fprintf(povout,"\tDIFFUSE 0.0\n");
*/
    fprintf(a.f,"}");
  pov_END_OBJECT(a);
}



void pov_flat_triangle(AStream &a,const Point &p1,const Point &p2,const Point &p3)
{
   fprintf(a.f,"\ttriangle { <%g, %g, %g> <%g, %g, %g> <%g, %g, %g> }",VECX(p1),VECX(p2),VECX(p3));
   a.nl();
}

void pov_smooth_triangle(AStream &a,const Point &p1,const Point &n1,const Point &p2,const Point &n2,const Point &p3,const Point &n3)
{
   fprintf(a.f,"\tsmooth_triangle { <%g, %g, %g> <%g, %g, %g> <%g, %g, %g> <%g, %g, %g> <%g, %g, %g> <%g, %g, %g> }",
      VECX(p1),VECX(n1),VECX(p2),VECX(n2),VECX(p3),VECX(n3));
   a.nl();
}


// write to POV Ray-tracer format
int
GShell::WritePov(AStream &a)
{
  GFaceList tris;
  IntArray tri2face;
  
  int ntris=ComputeTriangles(tris,tri2face); // compute all triangles for object
  int *fp = tris;
  int *fpend = fp+tris.Length();
  int vi; // vertex index
  GVertex v[4]; // vertex data

  int smooth = vn.Length()>0;
  while (fp<fpend) {	// for all faces
	  int n= *fp++;	// #3
	  int i;
	  for(i=0;i<n;i++) {
		 vi = *fp++;  GetVertex(vi,v[i]); 
	  }
	  if (smooth)	// with vertex normals
	  pov_smooth_triangle(a, v[0].v,v[0].n,v[1].v,v[1].n,v[2].v,v[2].n);
	  else pov_flat_triangle(a, v[0].v,v[1].v,v[2].v);
  }
  return(a.stat);
}

#endif


#ifdef _HMF
int WriteShellHmf(GShell *s,const char *FileName)
{
  AStream a(FileName,"w");
  a.SetBlockSep('(',')');
  a.SetComment(';');
  a.SetListSep('(',')',' ');

  if (!a.atEof()) {
	a.comment("; HMF V1.20 TEXT");
	a.comment("HMF written by GLView Version 1.1");
	s->WriteHmf(a);
  }
  return(a.stat);
}
#endif

#ifdef _GEO

int WriteShellGeo(GShell *s,const char *FileName,int outputNormals)
{
  AStream a(FileName,"w");
  a.SetComment(';');

  if (!a.atEof()) {
	/* a.comment("GEO written by GLView Version 1.11"); */
	s->WriteGeo(a,outputNormals);
  }
  return(a.stat);
}
#endif

#ifdef _STL

int WriteShellStl(GShell *s,const char *FileName)
{
  AStream a(FileName,"w");
  a.SetBlockSep('{','}');
  a.SetComment(';');
  a.SetListSep('(',')',' ');

  if (!a.atEof()) {
	// 29.04.96 a.comment("STL written by GLView Version 1.11");
	s->WriteStl(a);
  }
  return(a.stat);
}
#endif


#ifdef _POV
int WriteShellPov(GShell *s,const char *FileName)
{
  AStream a(FileName,"w");
  a.SetBlockSep('(',')');
  a.SetComment(';');
  a.SetListSep('(',')',' ');

  if (!a.atEof()) {
	a.comment("POV written by GLView Version 2.09");
	s->WritePov(a);
  }
  return(a.stat);
}
#endif

#ifdef _RAW

int WriteShellRaw(GShell *s,const char *FileName)
{
  AStream a(FileName,"w");

  if (!a.atEof()) {
	a.comment("RAW written by GLView Version 3.05");
	s->WriteRaw(a);
  }
  return(a.stat);
}

#endif



#ifdef _VRML
static char* VrmlHeader = "VRML V1.0 ascii";
static char* Vrml2Header = "VRML V2.0 utf8";

int WriteShellVrml(GShell *s,const char *FileName,gbool vrml2)
{
  AStream a(FileName,"w");
  
  a.SetBlockSep('{','}');
  a.SetComment('#');
  a.SetListSep('[',']',',');
  a.fmt = IV_FMT;
  if (vrml2) {
	 // a.fmt = VRML_FMT;
  }		



  if (!a.atEof()) {
	if (vrml2) a.comment(Vrml2Header);
	else a.comment(VrmlHeader);

	a.comment("written by " _PROGRAM _PROGRAM_TYPE " " _VERSION " " URL_HOME);

	if (!vrml2) {
  		a.tok("Shape"); 
  		a.BeginBlockTab();
		// a.tok("geometry"); 

		a.tok("geometry"); 
	}
	else {
  		a.tok("Separator"); 
  		a.BeginBlockTab();
	}

    // a.io("Info {string \"VRML written by GLView\"}"); a.nl(); 
    //a.io("DirectionalLight { direction 0.2 0 -1 }"); a.nl();
    //a.io("Material {diffuseColor 0.6 0.5 0.3 }"); a.nl();  
	
	if (!vrml2)
		a.io("ShapeHints {shapeType	SOLID  faceType	CONVEX }"); a.nl();

	s->WriteVrml(a ,vrml2);

	a.EndBlockTab();

  }
  return(a.stat);
}

int WriteVrmlCoordinate3(AStream &a,PointArray &v,gbool vrml2)
{
	if (vrml2) {
		a.tok("coord");
		a.tok("Coordinate");  
	} else 	a.tok("Coordinate3");  

	a.BeginBlockTab();
		a.tok("point");
		v.io(a); 
	a.EndBlockTab();
	a.nl();
	return(a.stat);
}

int WriteVrmlNormal3(AStream &a,PointArray &vn,gbool vrml2)
{
	if (vrml2) {
		a.tok("normal");
		a.tok("Normal");  
	} else  a.tok("Normal");  
	a.BeginBlockTab();
	a.tok("vector");
		vn.io(a); 
	a.EndBlockTab();
	a.nl();
	return(a.stat);
}

int WriteVrmlDiffuseColor(AStream &a,PointArray &vc,BOOL vrml2)
{
	if (vrml2) {
		a.tok("color");
		a.tok("Color");  
	} else a.tok("Material");  
	a.BeginBlockTab();
	
	if (vrml2) 
		a.tok("color");
	else a.tok("diffuseColor");
	
	vc.io(a); 
	a.EndBlockTab();
	a.nl();
	return(a.stat);
}


int WriteVrmlTextureCoordinate2(AStream &a,PointArray &vp,BOOL vrml2)
{

	if (vrml2) {
		a.tok("texCoord");
		a.tok("TextureCoordinate");  
	} else 	a.tok("TextureCoordinate2");  

	a.BeginBlockTab();
		a.tok("point");
		a.BeginList(); 
				const Point *p = vp;
				const Point *pend = p + vp.Length();
				while (p<pend) {
					a.w(p->x);	a.w(p->y); a.Sep(',');
					a.nl();
					p++;
				}
		a.EndList();


	a.EndBlockTab();
	a.nl();


	return(a.stat);
}


int GShell::WriteVrml(AStream &a,gbool vrml2)
{
	BOOL ils = FALSE;
	if (vrml2) {
		if (RTISA(this,GPolyline)) {
			a.tok("IndexedLineSet");
			ils = TRUE;
		} else 
		  a.tok("IndexedFaceSet");
  
	} else 	a.tok("Separator"); 
  
	a.BeginBlockTab();

		char buf[256];
		sprintf(buf," Shell Vertices = %d, Faces = %d",v.Length(),nfaces);
		a.comment(buf);
		{ 
		  BBox & b= GetBBox();
		  Point center(b.Center()), size(b.Size());
		  sprintf(buf,"bboxCenter %f %f %f bboxSize %f %f %f",center.x,center.y,center.z,size.x,size.y,size.z);
		  a.comment(buf);
		}


		if (v.Length()) WriteVrmlCoordinate3(a,v,vrml2);  
		if (!ils) {
		if (vn.Length() && !Flat()) {  
			WriteVrmlNormal3(a,vn,vrml2);  
			if (vrml2) 	{ 
				a.io("normalPerVertex TRUE"); a.nl(); 
				a.io("creaseAngle 3.14");
			}	
			else a.io("NormalBinding { value PER_VERTEX_INDEXED }");
			a.nl();
		} else if (fn.Length()) {  
			WriteVrmlNormal3(a,fn,vrml2);  
			if (vrml2) 	a.io("normalPerVertex FALSE");
			else a.io("NormalBinding { value PER_FACE }");
			a.nl();
		} 
		}
		if (vc.Length()) {  
			WriteVrmlDiffuseColor(a,vc,vrml2);  
			if (vrml2) a.io("colorPerVertex TRUE");
			else a.io("MaterialBinding { value PER_VERTEX_INDEXED }");
			a.nl();
		} else if (fc.Length()) {  
			WriteVrmlDiffuseColor(a,fc,vrml2);  
			if (vrml2)	a.io("colorPerVertex FALSE");
			else a.io("MaterialBinding { value PER_FACE }");
			a.nl();
		} 

		if (!(ils && vrml2)) 
		if (vp.Length()) {  
			WriteVrmlTextureCoordinate2(a,vp,vrml2);
			if (!vrml2)
				a.io("TextureCoordinateBinding { value PER_VERTEX_INDEXED }");
			a.nl();
		}

		// facelist f
		if (f.Length()) { 
			if (vrml2)  { }
			else {

				if (RTISA(this,GPolyline)) {
					a.tok("IndexedLineSet");
					ils = TRUE;
				} else 
				  a.tok("IndexedFaceSet");
				a.BeginBlockTab();
			}

				  a.tok("coordIndex");
				  a.BeginList(); 
						  int *fp=f,*fpend=fp+f.Length();
						  int needSep = 0;
						  while (fp<fpend) {
								int n=*fp++;
								if (needSep) a.ListSep();

								a.nl(); // Each facet on new line ?
								if (n<0) n=-n;
								while (n>0) { 
									a.io(*fp); a.ListSep(); fp++; n--; 
								}
								a.w(-1);	// EndMarker 
								needSep = 1;
						  }
					a.EndList();

					// HG 02.03.96 disabled
					if (0) { // default value of -1 should work, but some viewers dosn´t seem
					// to support it
					// normalIndex
					if (vn.Length()>0) {
				  		a.tok("normalIndex");
				  		a.BeginList(); 
						  const int *fp=f,*fpend=fp+f.Length();
						  int needSep = 0;
						  while (fp<fpend) {
								int n=*fp++;
								if (needSep) a.ListSep();
								a.nl(); // Each facet on new line ?
								if (n<0) n=-n;
								while (n>0) { 
									a.w(*fp); a.ListSep(); fp++; n--; 
								}
								a.w(-1);	// EndMarker 
								needSep = 1;
						  }
						a.EndList();
					
					} //VN
					// material index
					if (vc.Length()>0) {
						if (vrml2) a.tok("colorIndex");
				  		else a.tok("materialIndex");

				  		a.BeginList(); 
						  const int *fp=f,*fpend=fp+f.Length();
						  int needSep = 0;
						  while (fp<fpend) {
								int n=*fp++;
								if (needSep) a.ListSep();
								a.nl(); // Each facet on new line ?
								if (n<0) n=-n;
								while (n>0) { 
									a.w(*fp); a.ListSep(); fp++; n--; 
								}
								a.w(-1);	// EndMarker 
								needSep = 1;
						  }
						a.EndList();
					
					} //VC
					// textureCoordinateIndex
					if (!(ils && vrml2)) 
					if (vp.Length()>0) {
				  		if (vrml2)
							a.tok("texCoordIndex");
				  		else a.tok("textureCoordIndex");
				  		a.BeginList(); 
						  const int *fp=f,*fpend=fp+f.Length();
						  int needSep = 0;
						  while (fp<fpend) {
								int n=*fp++;
								if (needSep) a.ListSep();
								a.nl(); // Each facet on new line ?
								if (n<0) n=-n;
								while (n>0) { 
									a.w(*fp); a.ListSep(); fp++; n--; 
								}
								a.w(-1);	// EndMarker 
								needSep = 1;
						  }
						a.EndList();


					} //VP

					}
					if (!vrml2)
						a.EndBlockTab();
				   }

  a.EndBlockTab();
  return(a.stat);
}

int GShell::Do(GioTraversal &state) 
{
  return (WriteVrml(*state.astream,state.vrml2));
}

int GMesh::WriteVrml(AStream &a,gbool vrml2)
{
   gbool hasFlist = f.Length()>0;
   if (!hasFlist) ComputeFaceList();
   int ret = GShell::WriteVrml(a,vrml2);
   if (!hasFlist) f.Flush();
   return(ret);

}

int GShellI::WriteVrml(AStream &a,gbool vrml2)
{
	gbool ils = FALSE;
	if (vrml2) {
		if (RTISA(this,GPolyline)) {
			ils = TRUE;
			a.tok("IndexedLineSet");
		}
		else if (RTISA(this,GPolylineI)) {
			ils = TRUE;
			a.tok("IndexedLineSet");
		} else 
		  a.tok("IndexedFaceSet");
  
	} else 	a.tok("Separator"); 
  
  a.BeginBlockTab();
		char buf[256];
		sprintf(buf," ShellI Vertices = %d, Faces = %d",v.Length(),nfaces);
		a.comment(buf);
		{ 
		  BBox &b= GetBBox();
		  Point center(b.Center()), size(b.Size());
		  sprintf(buf,"bboxCenter %f %f %f bboxSize %f %f %f",center.x,center.y,center.z,size.x,size.y,size.z);
		  a.comment(buf);
		}

		if (v.Length()) WriteVrmlCoordinate3(a,v,vrml2);  
		if (!ils) {
		if (vn.Length() && !Flat()) {  
			WriteVrmlNormal3(a,vn,vrml2);  
			if (vrml2) {
				a.io("normalPerVertex TRUE"); a.nl(); 
				a.io("creaseAngle 3.14");
			} else a.io("NormalBinding { value PER_VERTEX_INDEXED }");
			a.nl();

		} else if (fn.Length()) {  
			WriteVrmlNormal3(a,fn,vrml2);  
			if (vrml2) 	a.io("normalPerVertex FALSE");
			else a.io("NormalBinding { value PER_FACE }");
			a.nl();
		} 
		}
		if (vc.Length()) {  
			WriteVrmlDiffuseColor(a,vc,vrml2);  
			if (vrml2) a.io("colorPerVertex TRUE");
			else a.io("MaterialBinding { value PER_VERTEX_INDEXED }");
			a.nl();
		} else if (fc.Length()) {  
			WriteVrmlDiffuseColor(a,fc,vrml2);  
			if (vrml2)	a.io("colorPerVertex FALSE");
			else a.io("MaterialBinding { value PER_FACE }");
			a.nl();
		} 

		if (!(ils && vrml2)) 
		if (vp.Length()) {  
			WriteVrmlTextureCoordinate2(a,vp,vrml2);
			if (!vrml2)
				a.io("TextureCoordinateBinding { value PER_VERTEX_INDEXED }");
			a.nl();
		}

		// facelist f
		if (f.Length()) { 

			if (!vrml2) {
				if (RTISA(this,GPolyline)|| RTISA(this,GPolylineI))	{
					a.tok("IndexedLineSet");
					ils = TRUE;
				} else 
				  a.tok("IndexedFaceSet");
				a.BeginBlockTab();
			}
				  a.tok("coordIndex");
				  a.BeginList(); 
						  const int *fp=f,*fpend=fp+f.Length();
						  int needSep = 0;
						  while (fp<fpend) {
								int n=*fp++;
								if (needSep) a.ListSep();
								a.nl(); // Each facet on new line ?
								if (n<0) n=-n;
								while (n>0) { 
									a.w(verts[*fp].v); a.ListSep(); fp++; n--; 
								}
								a.w(-1);	// EndMarker 
								needSep = 1;
						  }
					a.EndList();
					// normalIndex
					if (vn.Length()>0) {
				  		a.tok("normalIndex");
				  		a.BeginList(); 
						  const int *fp=f,*fpend=fp+f.Length();
						  int needSep = 0;
						  while (fp<fpend) {
								int n=*fp++;
								if (needSep) a.ListSep();
								a.nl(); // Each facet on new line ?
								if (n<0) n=-n;
								while (n>0) { 
									a.w(verts[*fp].n); a.ListSep(); fp++; n--; 
								}
								a.w(-1);	// EndMarker 
								needSep = 1;
						  }
						a.EndList();
					
					} //VN
					// material index
					if (vc.Length()>0) {
				  		if (vrml2) a.tok("colorIndex");
				  		else a.tok("materialIndex");
				  		a.BeginList(); 
						  const int *fp=f,*fpend=fp+f.Length();
						  int needSep = 0;
						  while (fp<fpend) {
								int n=*fp++;
								if (needSep) a.ListSep();
								a.nl(); // Each facet on new line ?
								if (n<0) n=-n;
								while (n>0) { 
									a.w(verts[*fp].c); a.ListSep(); fp++; n--; 
								}
								a.w(-1);	// EndMarker 
								needSep = 1;
						  }
						a.EndList();
					
					} //VC
					// to do face group ???
					// textureCoordIndex
					if (!(ils && vrml2)) 
					if (vp.Length()>0) {
				  		if (vrml2)
							a.tok("texCoordIndex");
				  		else a.tok("textureCoordIndex");
				  		a.BeginList(); 
						  const int *fp=f,*fpend=fp+f.Length();
						  int needSep = 0;
						  while (fp<fpend) {
								int n=*fp++;
								if (needSep) a.ListSep();
								a.nl(); // Each facet on new line ?
								if (n<0) n=-n;
								while (n>0) { 
									a.w(verts[*fp].p); a.ListSep(); fp++; n--; 
								}
								a.w(-1);	// EndMarker 
								needSep = 1;
						  }
						a.EndList();


					} //VP
				if (!vrml2)
				a.EndBlockTab();
				   
				} // f.Length();

  a.EndBlockTab();
  return(a.stat);
}



#endif
