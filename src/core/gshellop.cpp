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

@module GShellOP.cpp - GLView GShell input routines	|

Copyright (c) 1995 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

$Revision: 1.8 $
$Log: gshellop.cpp,v $
Revision 1.8  1999/07/06 16:54:57  tom
*** empty log message ***



Todo :

******************************************************************************/


#include "stdafx.h"

#include <stdlib.h>
//#include <string.h>

#include "gutils.h"
//#include "astream.h"
//#include "point.h"
//#include "pointa.h"
#include "gclass.h"
#include "gshell.h"
#include "gshelli.h"
#include "gshellop.h"
#include "www.h"

#ifdef _HMF

// Read a shell in HOOPS Metafile Format
GShell* ReadFromMetafile(AStream &a,GShell *the_shell)

{ GShell *s = NULL;
 PointArray p;

 if (a.isInput()) {
	 s=the_shell;
	 if (s== NULL) s=new GShell;

	 // read vertex list ( (x y z) ....)

	 p.io(a);

	 s->SetV(p);
	 a.Sep('(');
	 // read face list ( i i i i)
	 { int *flist = NULL;
	   int nflist=0,flist_max=0;
	   /* read until closing paren */
	   while (a.Skip() != ')') {
		  int i;
		  a.r(i);
		  append(flist,nflist,flist_max,&i,1);
	   }
	   s->SetFaceList(nflist,flist);
	   if (flist) FREE(flist);
	 }
	 a.Sep(')');
 }
 return(s);
}

// Read a mesh in HOOPS Metafile Format
// ( ((P0) ..(P1)) ... )
GMesh* ReadFromMetafile(AStream &a,GMesh *the_shell)
{ GMesh *s;
 PointArray p,row;
 int cols=0,rows=0;

 if (a.isInput()) {
	 s=the_shell;

	 if (s== NULL) s=new GMesh;
	 a.Sep('(');


	 if ( a.Skip() == '(')  {

	 // read first row = vertex list ( (x y z) ....)
	 p.io(a);

	 cols = p.Length();
	 rows = 1;

	 while (a.Skip() != ')') {
	 row.io(a);
		 if (row.Length() != cols) {
		   TRACE("Warning::ReadMesh row %d length %d != colums %d\n",row,row.Length(),cols);
		   row.Resize(cols);
		 }
		 p.Append(row);
		 rows++;
		 row.Resize(0);
	 }

	 s->Set(cols,rows,p);
	 s->SetVertexParametersNative();
	 }

	 a.Sep(')');
 }
 return(s);
}


// Parse stream until matching closing paren is found
int HMFSkipUntilClose(AStream &a)
{ int level = 1;
  int c;

  do {
	 c = a.Skip();
	 if (c == a.block_open)  level ++;
	 else if (c == a.block_close)  level --;
	 c = fgetc(a.f);
  } while ((level>0) && !a.AtEof());
  return a.stat;
}



// Read a shell from a HOOPS Metafile

GShell* ReadFromHmfFile(const char *FileName)
{ GShell *s=NULL;
  int in_geometry=0;

  AStream a(FileName,"r");


  
  a.SetBlockSep('(',')');
  a.SetListSep('(',')',' ');
  a.SetComment(';');

  if (!a.atEof()) {
	  if (a.Skip() == '(') {
		char tok[256];
		char tok1[256];
		char tok2[256];

		a.Sep('(');
		a.iotok(tok);
		if (streq(tok,"geometry")) {
			in_geometry=1;

			if (a.Skip() == '(') {
				a.Sep('(');
				a.iotok(tok);
			}
		}

		if (streq(tok,"shell")) {
		  s=ReadFromMetafile(a,(GShell *) NULL);
		  a.Sep(')'); // closing paren from (Shell
		}
		else if (streq(tok,"mesh")) {
		  s=ReadFromMetafile(a,(GMesh *) NULL);
		  a.Sep(')'); // closing paren from (Mesh
		}

		if (in_geometry) {
		  while (a.Skip() == '(') {
				a.Sep('(');
				a.BeginTok();
				if (a.tok("vertex")) {
					int i;
					Point c;
					a.r(i);
//					TRACE("Vertex #%d\n",i);
					while (a.Skip() == '(') {   // (
					a.Sep('(');
					a.BeginTok();
						if (a.tok("color_by_value"))  {
						  a.iotok(tok1); a.iotok(tok2);

						  a.r(c.x); a.r(c.y); a.r(c.z);

						  if (streq(tok1,"\"face\"") && streq(tok2,"\"rgb\"")) {
//							printf("Vertex Face color %g %g %g \n",c.x,c.y,c.z);
							if (s) {
							  if (s->vc.Length() == 0) { // allocate vc array
								 s->SetVC(1,&c);
							  }
							  if (i<s->v.Length()) s->vc[i]=c;
							}
						 }
						}
						else if (a.tok("parameter"))  {
							c.io(a);
//							printf("Vertex Parameter %g %g %g \n",c.x,c.y,c.z);
							if (s) {
							  if (s->vp.Length() == 0) { // allocate vp array
								 s->SetVP(1,&c);
							  }
							  if ((i<s->v.Length()) && (s->vp.Length()>0)) s->vp[i]=c;
							}
						}
						else if (a.tok("normal"))  {
						//	c.io(a);
							a.io(c.x); a.io(c.y); a.io(c.z);
//							printf("Vertex Normal %g %g %g \n",c.x,c.y,c.z);
							if (s) {
							  if (s->vn.Length() == 0) { // allocate vn array
									// trigger no recomputation of normals
									s->UnSetEditFlag(SHELL_VERTICES_CHANGED);
									s->UnSetEditFlag(SHELL_FACELIST_CHANGED);
									s->ComputeVertexNormals();
									// compute default normals
							  //   s->SetVN(1,&c);
							  }
							  if ((i<s->v.Length()) && (s->vn.Length()>0)) s->vn[i]=c;
							}
						}
						else  {
							// error
							// skip until )
						}
						a.EndTok();
						a.Sep(')');

					}  // while  )
				} // vertex
				else if (a.tok("face")) {
					int i;
					Point c;
					a.r(i);
//					TRACE("Face #%d\n",i);
					while (a.Skip() == '(') {   // (
					a.Sep('(');
					a.BeginTok();
						if (a.tok("color_by_value"))  {
						  a.iotok(tok1); a.iotok(tok2);

						  a.r(c.x); a.r(c.y); a.r(c.z);

						  if (streq(tok1,"\"face\"") && streq(tok2,"\"rgb\"")) {
//							printf("Face color %g %g %g \n",c.x,c.y,c.z);
							if (s) {
							  if (s->fc.Length() == 0) { // allocate vc array
								 if (s->nfaces == 0) s->nfaces = s->CountFaces();
								 s->SetFC(1,&c);
							  }
							  if (i<s->nfaces) s->fc[i]=c;
							}
						 }
						}
						else if (a.tok("normal"))  {
						//	c.io(a);
							a.io(c.x); a.io(c.y); a.io(c.z);
//							printf("Face Normal %g %g %g \n",c.x,c.y,c.z);
							if (s) {
							  if (s->fn.Length() == 0) { // allocate vn array
								 if (s->nfaces == 0) s->CountFaces();
									// trigger no recomputation of normals
									s->UnSetEditFlag(SHELL_VERTICES_CHANGED);
									s->UnSetEditFlag(SHELL_FACELIST_CHANGED);
									s->ComputeFaceNormals(); // ?? compute default normals
							  //   s->SetFN(1,&c);
							  }
							  if ((i<s->nfaces) && (s->fn.Length()>0)) s->fn[i]=c;
							}
						}
						else  {
							// error
							// skip until )
						}
						a.EndTok();
						a.Sep(')');

					}  // while  )
				} // face
				// edge
				else {
				 // error unknown attribute
				}
				// a.EndTok();
				a.Sep(')');

		  } // while
		  a.Sep(')');
		}  // geometry

	  } // (
  }
  return(s);
}

#endif

// Read a shell from simple geo format file
GShell* ReadFromGeoFile(const char *FileName)
{ GShell *s=NULL;
  FILE *f;
  Point *points_ptr,*points;

  int		i,cur_poly, vertice_count, vertice_num;
  int		number_of_points, polycount, scale;


  if ((f=fopen(FileName,"r")) == 0) {
	   return s;
  }
  /* Read the 1st line from file: # of points, # of polygons, scale. */
  fscanf (f, "%d %d %d", &number_of_points, &polycount, &scale);

  /* Allocate enough memory for reading in vertices */
  points_ptr = points  = new Point[number_of_points];

  /* Read in Point Values from .geo file */
  for (i=0;  i<number_of_points;  i++, points_ptr++)
	fscanf (f, "%f %f %f", &(points_ptr->x), &(points_ptr->y), &(points_ptr->z));

   int *flist = NULL;
   int nflist=0,flist_max=0;

  /* Read in connectivity info */

  for (cur_poly=0; cur_poly<polycount; cur_poly++) {
	   fscanf (f, "%d", &vertice_count);    /* Count of vertices */
	   append(flist,nflist,flist_max,&vertice_count,1);

	   for (i=0; i<vertice_count; i++) {
	  /* Read vertex number (vertex 1 is the 1st point read in).*/
	  fscanf (f, "%d", &vertice_num);
	  vertice_num--;
	 append(flist,nflist,flist_max,&vertice_num,1);
	  }
  }

  s = new GShell(number_of_points,points,nflist,flist);

  if (flist) FREE(flist);
  if (points) delete points;
  fclose(f);

  return(s);
};


#ifdef _3DV


// write a shell to a 3dv File

int WriteShell3DV(GShell *s,const char *FileName)
{
  AStream a(FileName,"w");
  a.block_open = '{';
  a.block_close = '}';


  if (!a.atEof()) {
	a.comment("GLView Version 1.0 Object");
	s->io(a);
  }
  return(a.stat);
}

// Read a shell from a 3dv file

int ReadShell3DV(const char *FileName,GShell *&s)
{

  AStream a(FileName,"r");

  if (!a.atEof()) {
		a.BeginTok();
		if (a.IsInput()) {
		   if (s == NULL) {
			  if (a.tok("shell"))   s = new GShell();
			  else if (a.tok("mesh"))   s = new GMesh();
			  else if (a.tok("shellindexed"))   s = new GShellI();
			  else if (a.tok("polygon"))   s = new GPolygon();
			  else if (a.tok("polyline"))   s = new GPolyline();
			  else if (a.tok("polylineindexed"))   s = new GPolylineI();
			  //else if (a.tok("nurbssurface"))   s = new GNurbsSurface();
			  else {
					a.Error("ReadShell:: Unknown type \n");
					TRACE("ReadShell3DV: unknown type %s \n",a.curtok);
					return(-1); // bad file type 
				}

		   }
		}

	s->io(a);
  }
  return(a.stat);
}

#endif

#ifdef _RAW

// Read a shell from a raw file
// raw format has a single triangle per line
int ReadShellRaw(const char *FileName,GShell *&s)
{

  char buf[256];
  
  AStream a(FileName,"r");

  if (!a.atEof()) {
	   Point p[3];
	   if (s == NULL) s = new GShell();

    while (!a.atEof()) {
	 int ret;
	 int c=a.Skip();	// skip comments, nl ...

     if (fgets(buf,256,a.f) == NULL) break;
     a.line++;


     // ignore lines not starting with number or . 2.06.96
     c = buf[0];
    // if (!(isdigit(c) || c  == '.')) {
//        continue;
//     }

	 ret=sscanf(buf,"%g %g %g %g %g %g %g %g %g",
		&p[0].x,&p[0].y,&p[0].z,
		&p[1].x,&p[1].y,&p[1].z,
		&p[2].x,&p[2].y,&p[2].z);

	 if (ret != 9) {

	 } else 
	   s->AppendTriangle(p[0],p[1],p[2]);
  }
    
  }
  return(a.stat);
}

#endif _RAW 


// Read shell from basice lowlevel file formats 
int ReadShell(const char *FileName,int fmt,GShell * &newShell)
{  int ret=-1;

	newShell = NULL;

	if (0) {} 
#ifdef _GEO
   else if (fmt == GEO_FMT) {
       newShell = ReadFromGeoFile(FileName);
	   if (newShell == NULL) ret=-1;
   }	
#endif
#ifdef _RAW
   else if (fmt == RAW_FMT)
       ret=ReadShellRaw(FileName,newShell);
#endif
#ifdef _3DV
   else if (fmt == D3V_FMT)
       ret=ReadShell3DV(FileName,newShell);
#endif
#ifdef _OBJ
   else if (fmt == OBJ_FMT) {
	   GShellI *theShellI=NULL;  	
       ret=ReadShellWavefront(FileName,theShellI);
	   newShell = theShellI;
   }
#endif
#ifdef _HMF
   else { 
	   newShell = ReadFromHmfFile(FileName);
   	   if (newShell == NULL) ret=-1;
   }	

#endif
   
   return ret;

}




