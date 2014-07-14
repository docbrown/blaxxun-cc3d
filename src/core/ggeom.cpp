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

@module GGeom.cpp - GLView geometry creation routines |

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

Todo :

******************************************************************************/

#include "stdafx.h"

#include "gutils.h"

#include "gclass.h"
#include "gshell.h"
#include "gshellop.h"
#include "gshelli.h"

#include "grender.h"
#include "ggeom.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


RTIMP(GSphereSubdiv,0,GShell);

GSphereSubdiv::GSphereSubdiv(const Point &Center, float R,int Level,int compute) : 
		center(Center), r(R), level(Level)  
{ 
  viewDependent = 0;  
  computed=0; 
  if (compute) Compute();
};

	
void GSphereSubdiv::ComputeBBox(BBox &box)
{
  box.mi =center-Point(r,r,r),box.ma=center + Point(r,r,r);
}

void GSphereSubdiv::ComputeCenter(Point &Center)
{
 Center = center;
}

#ifdef _OGL
void GSphereSubdiv::RenderGlFaces(RenderState &state,int FN, int VN, int VC, int VP)
{
  if (computed) GShell::RenderGlFaces(state,FN,VN,VC,VP);
  else state.RenderSphere(center,r,level,0,VN,FN);
}
	

void GSphereSubdiv::RenderGlEdges(RenderState &state,int VN,int VC,int VP)
{
  if (computed) GShell::RenderGlEdges(state,VN,VC,VP);
  else {
  	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  	state.RenderSphere(center,r,level,0,VN);
  	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }
}

void GSphereSubdiv::RenderGlVertices(RenderState &state,int VN,int VC)
{
  if (computed) GShell::RenderGlVertices(state,VN,VC);
  else { 
  	glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
  	state.RenderSphere(center,r,level,0,VN);
  	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }
}

void GSphereSubdiv::RenderGlNormals(RenderState &state,float scale,int VN,int FN)
{
  if (computed) GShell::RenderGlNormals(state,VN,FN);
  else ;
}
#endif


// subdivide a triangle 
void GSphereSubdiv::TriangleSub(int level,int p1,int p2,int p3)
{


	if  (level<=0) { 	
		f.AppendTri(p1,p2,p3);
		return;
	}

	int p12= AppendV(Normalize((v[p1]+v[p2])*0.5));
	int p23= AppendV(Normalize((v[p2]+v[p3])*0.5));
	int p31= AppendV(Normalize((v[p3]+v[p1])*0.5));


	{
	  level--;
	  TriangleSub(level,p1,p12,p31);
	  TriangleSub(level,p12,p23,p31);
	  TriangleSub(level,p31,p23,p3);
	  TriangleSub(level,p12,p2,p23);
	}
}



int GSphereSubdiv::ComputeSphere(const Point &center, float r,int level,int VP)
{
//	renderSphereVN=VN;
//	renderSphereFN=FN;

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


	SetV(ELEMENTS(icosahedron_ptsp),(const Point*) icosahedron_ptsp);
	
	f.Flush();
	
	f.SetMaxLength(ELEMENTS(icosahedron_connectivity) * (level+1)*2);


	
	const int *fp =icosahedron_connectivity; 
	const int *fpEnd =icosahedron_connectivity+ELEMENTS(icosahedron_connectivity);
	
	while (fp<fpEnd) {
		int n=*fp++;
		int i1=*fp++;
		int i2=*fp++;
		int i3=*fp++;

		TriangleSub(level,i1,i2,i3);
	}


	// transform to radius and center
	for (int i=0; i< v.Length(); i++) {
		v[i]*=r;
		v[i]+=center;

	}
	if (VP) {
		vp.SetLength(v.Length());
		GetVertexParametersNative(v.Length(),vp.DataRW());
	}
	computed = 1;
	return(0);
}


int
GSphereSubdiv::ComputeTriangles(GFaceList &tri,IntArray &tri2face,int useTriangulator)
{
  if (!computed) Compute();
  return GShell::ComputeTriangles(tri,tri2face,useTriangulator);
}

// get "native" vertex parameters
// implementation  simply returns vertex coordinates
void GSphereSubdiv::GetVertexParametersNative(int n,Point *vp)
{
  int i=0;
  for (i=0; i < v.Length(); i++) {
		 
	Point p(v[i]-center);

	p /= r;

	float u,v = p.z;

	if (v <= -1.0) { u = 0.0; v = 0.0; }
	else if (v >= 1.0)  { u=0.0; v = 1.0; }
	else {
	  v=	(PI*0.5+acos(v)) / PI;
	  u = (atan2(p.y,p.x)+PI) / TWOPI;
	}
	vp[i].Set(u,v,0.0);
		 
	 
		 
	// vp[i] = v[i];
	if (i>=n) break;
  }
}

/*  Evaluate object as object of type "type"
  if return = this, don´t destroy result, otherwise destroy */
GEvalAs_TYPE*  GSphereSubdiv::EvalAs(TypeId type,int copyAttributes)
{
  if (type == Id()) return(this); // already indentical
#if 0
  if (IsA(type)) {
	  // is of derived types
	  return(this); // or copy ??
  }
#endif
  if (type == GShell::ClassId()) {  // convert to shell
  	 if (!computed) Compute();

	 GShell *s = new GShell();
	 s->SetFaceList(f);
	 s->SetV(v);
	 if (copyAttributes) {
		 if (vn.Length() > 0) s->SetVN(vn);
		 if (vc.Length() > 0) s->SetVC(vc);
		 if (vp.Length() > 0) s->SetVP(vp);
		 if (fn.Length() > 0) s->SetFN(fn);
		 if (fc.Length() > 0) s->SetFC(fc);
	 }
	 return(s);
  }
// else cannot convert
  return(NULL);
}




int GRotSweep::Compute()
{
  return(0);
}

int GTransSweep::Compute()
{
  return(0);
}


//@func Create a GPolygon primitive with circle data
GPolygon *NewCircle(float r,float cx,float cy,int n)
{
        const float cz=0.0;

        int     i;
        float   div, phi;

//        if (n<=0) n=CIRCLEPTS;

		PointArray poly(n);

        div = TWOPI /n;
        for (i=0; i<n; i++) {
                phi = div * i;
                poly[i].x= cx+r* (cos(phi));
                poly[i].y= cy+r* (sin(phi));
                poly[i].z= cz;
        }
        return(new GPolygon(poly.Length(),poly.Data()));
}

//@func create a shell with sphere data

GShell * NewSphere(float r,int usteps,int vsteps) 
{

GShell *arc = NewCircle(r,0.0,0.0,vsteps);
GRotSweep sweep(arc);

GShell * result;
sweep.vsteps=usteps;
sweep.apexBottom = 1;
sweep.apexTop = 1;

sweep.Compute();
result = sweep.result;
sweep.result = 0;
return result;

}

// Invert facelist by adding faces as holes to fin, and than append holes in fin
// as normal faces to fout
// fout should contain the one master face at input
int ComputeInvertedFaceList(const GFaceList &fin,GFaceList &fout)
{
 
 const int * fp=fin.Data();
 const int * fpend=fp+fin.Length();

 // pass 1 : add normal faces as holes
 while (fp <fpend) {
    int n = *fp++;
	if (n<0) {
	   n=-n;
	 } else {
	   fout.Append(-n);
	   fout.Append(n,fp);
	}
	fp+=n;
 }

 // pass 2 : add holes as normal faces 
 fp=fin.Data();

 while (fp <fpend) {
    int n = *fp++;
	if (n<0) {
	   n=-n;
	   fout.Append(n);
	   fout.Append(n,fp);
	}
	fp+=n;
 }
 return(0);
}


int compute_sweeped_facelist(istyle style,
                                int nf,const int *f,   /* input face list */
                                int max_level,int level[],
                                int *noutf,int *outf)
{
  const int *fp,*fpend=f+nf;
  int *outp= outf;

#define put(a,b,c,d) *outp++ = 4; *outp++ = (a); *outp++ = (b); *outp++ = (c); *outp++ = (d);

  int i;
  int l;   /* level */
  int ihard = BIT(style,IHARDTOPBOTTOM) && (BIT(style,ITOP||IBOTTOM));

  *noutf=0;

  for(fp=f; fp<fpend; )                 /* for all faces */
  {   int ishole=0;
      int n;
      int p0,p1,p2;
      n = *fp++;

      if (n<0)
        { ishole = 1; n = -n; }


      for(l=0; l<max_level; l++)  { /* for all sweep levels */

      p0 = p1 = fp[0];
      for(i=1;i<n;i++) {
        p2 = fp[i];
        if (p1!=p2) 
           put(level[l]+p1,level[l]+p2,level[l+1]+p2,level[l+1]+p1);
        p1=p2;
      }
      if ((n>2) && BIT(style,ICLOSE)) {
         p2=p0;
         if (p1!=p2) {
           put(level[l]+p1,level[l]+p2,level[l+1]+p2,level[l+1]+p1);
		 }
      }
      }  /* level */

      fp +=n;  /* to next subface */

  }
  *noutf = outp-outf;
  return (*noutf);
}


// list of offsets 
static Point *extrusionOffsets;
static float extrusionVParameter[20];
static float extrusionContourOffsets[20];

/* compute from linear point array all sweeped points */
void compute_sweeped_points(int np,Point *p,int nf,int *f,
                int max_level,int *level,Point *pparam)
{ 
  int i,l;
  Point pt;
  for (i=0; i <np;i++) {
     for (l=1; l<=max_level; l++) {
        pt = p[i];

		pt += extrusionOffsets[l];
        
        p[level[l]+i] = pt;
		
		if (pparam) { 
		   pparam[level[l]+i].Set(pparam[level[0]+i].x,extrusionVParameter[l],l / (float) max_level); 
		}
     }
  }
}


/* extract n points from index list */
static void extract_points(int n,const int *fp,const Point *p,
                Point *pbuf,int *fbuf,int ioffset)
{  int i;

   for (i=0; i < n; i++)  {
      pbuf[i] = p[fp[i]];
      if (fbuf) fbuf[i]= i + ioffset;
   }
}


GShell * ComputeExtrudedShell(
				istyle style,
                int np,const Point *p,
                int nf, const int *f,
                int max_level)
{
    const int *fp,*fpend= f+nf;

    int pcnt;
    
	// to do make dynamic
    PointArray pbuf(1000);		// Pointbuffer original + extruded
    PointArray pbufParam(1000);

    IntArray fbuf(2000);
    IntArray outfbuf(12000);

    Point *pbufp;
    int   *fbufp;

    int pbufcnt,fbufcnt;
    int noutfbuf;
    int level[100];


    GShell *s;
	if (BIT(style,ILINES)) s = new GPolyline;
	else 	s = new GShell();
	GShell *k;


	// for each face
	for (fp=f; fp<fpend; )  {
      int subfaces;
      int n;

      /* extract next face with hole(s) */
      pbufp = pbuf;
      fbufp = fbuf;
      subfaces=0;
      do {
         n = *fp++;
         *fbufp++ = n;

         if (n<0) n = -n;

		 //  resize pbuf ??
		 {	int l = (pbufp-pbuf) + n;
			int pbufi = (pbufp-pbuf);

			if (l>= pbuf.Length()) {
				pbuf.SetLength(l);
			    pbufp = pbuf+pbufi;

			}
			if (l>= pbufParam.Length()) {
				pbufParam.SetLength(l);
			}

		 }	
         extract_points(n,fp,p,pbufp,fbufp,pbufp-pbuf);

		 if (BIT(style,IPARAMETERS))		// parametrisise face using edgelength

		      //   if (p1!=p2) 
         {
		    pbufParam[pbufp-pbuf].Set(0.0,extrusionVParameter[0],0.0);
		    for(int i=1; i<n; i++) {

				Point dist = pbufp[i]-pbufp[i-1];
		  		pbufParam[(pbufp-pbuf)+i].Set(pbufParam[(pbufp-pbuf)+i-1].x + dist.Len(),extrusionVParameter[0],0.0);

		  }
		 }
         pbufp+=n; fbufp+=n;
         fp+=n;
         subfaces++;
      } while ((fp<fpend) && (*fp<0));

      pbufcnt = pbufp-pbuf;
      fbufcnt = fbufp-fbuf;

      level[0]=0;
      pcnt=pbufcnt;
      
      for (n=1; n<=max_level; n++) {
        level[n] = level[n-1] + pbufcnt;
        pcnt+=pbufcnt;
      }

      TRACE("Face pts %d facelength %d subfaces %d \n",pbufcnt, fbufcnt,subfaces);
      
      compute_sweeped_points(pbufcnt,pbuf,fbufcnt,fbuf,max_level,level,
              (BIT(style,IPARAMETERS) ? pbufParam.DataRW(): NULL));

      compute_sweeped_facelist(style,
                                fbufcnt,fbuf,
                                max_level,level,
                                &noutfbuf,outfbuf);
      
      TRACE("Result shell  pts %d facelength %d \n",pcnt, noutfbuf);
	  
	  k= new GShell(pcnt,pbuf,noutfbuf,outfbuf);
	  if (BIT(style,IPARAMETERS)) {
	     k->SetVP(pcnt,pbufParam);
	  }

      s->Append(*k,0);
	  delete k;
      

      if (BIT(style,ITOP)) {
          k = new GShell(pbufcnt,&pbuf[level[max_level]],fbufcnt,fbuf);
          if (BIT(style,IPARAMETERS)) {
	     		k->SetVP(k->v);
	  	  }

          s->Append(*k,0);

		  delete k;
      }
      if (BIT(style,IBOTTOM)) {
          /* reverse */
		  k = new GShell(pbufcnt,pbuf,fbufcnt,fbuf);
          if (BIT(style,IPARAMETERS)) {
	     		k->SetVP(k->v);
				k->vp *= Point(-1.0,1.0,1.0);
	  	  }

          s->Append(*k,0);
		  delete k;
      }

  }  /* for */
  return (s);
}


//@func compute extruded version of shell
GShell * ComputeExtrudedShell(
				istyle style,
				GShell &s,
				PointArray &offsets)

{
  extrusionOffsets = offsets;
  if (BIT(style,IPARAMETERS))		// parameter values in extrusion direction
  for (int l=0; l<offsets.Length(); l++)
  {
  	extrusionVParameter[l]= offsets[l].Len();
  }

  return (ComputeExtrudedShell(style,s.v.Length(),s.v,s.f.Length(),s.f,offsets.Length()-1));
}
  
#if 0
int PointInPolygon2D(float x,float y,int n,const Point *poly)
{
	const Point *v1,*v2;
	int i;
	float xp1,yp1,xp2,yp2;
	int tst;

	tst=0;
	v1=&poly[0];
	v2=&poly[n-1];
	for(i=0; i < n; i++)
	{
		xp1=v1->x;
		yp1=v1->y;
		xp2=v2->x;
		yp2=v2->y;
		if ((((yp1<=y) && (y<yp2)) || ((yp2<=y)  && (y<yp1))) &&
			(x < (xp2 - xp1) * (y - yp1) /  (yp2 - yp1) + xp1))
				tst = (tst==0 ? 1 : 0);
		v2=v1;
		v1=++t;
	}
	return tst;
}

#endif
