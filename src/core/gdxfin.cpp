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
/*
	gdxfin.cpp from tsview/dxfin.cpp 03/95

	DXFIN.C

	AUTOCAD DXF reader for _GLVIEW	  
	01.11.89 - 1992 (c) Holger Grahn 


DXF reader
==============================


AutoCAD Versions supported: 10,11
===========================

Sections:
=========
	BLOCK, LAYER, ENTITIES

Attributes:
===========
	LAYER, COLOR, EDGE VISIBILITY

Entities:
=========
	BLOCK, INSERT
	POINT, LINE, SOLID, 3DFACE, CIRCLE, ARC
	POLYLINE, TEXT

Support for _GLVIEW 3.0 shell primitves:
======================================
	circles with height are stored as a shell
	polylines with height are stored as a shell
	Multiface polylines are transformed in a _GLVIEW shell with
	individual face colors and edge visibilities
	polyline meshes are transformed into a _GLVIEW mesh

AME Support:
===========
	AME CSG solid models which are multiface POLYLINEs are
	correctly displayed


Source:
=======
	ANSI-C (KR C requires a few modifications)
	tested with MetaWare High C, BORLAND Turbo C, HP 300 / 700, SG Indigo

Testprogram:
============
	DXF2HMF.C  read's a AutoCAD DXF-Files and writes out a _GLVIEW-HMF file

To do:
======
	Elevations other than the z-direction (DXF-Fields 210 220 230)
	not supported for all primitives
	Polylines with width and height (contour polylines)
	Linestyle attribute
	User definable fonts
	other primitves like dimension, attribute, paper space ...

Comments to Entity translation :
================================
DXF 	_GLVIEW
=== 	=====

LINE		LINE
		POLYGON if elevated

CIRCLE		CIRCLE
		shell cylinder if elevated

ARC 	Circular Arc

POLYLINE
		POLYLINE
		POLYGON if closed
		SHELL if elevated


		MESH	if mesh polyline
		SHELL	if multi face polyline



SOLID		POLYGON
		4 POLYGONS if elevated

3DFACE		POLYGON


BLOCK		Segment
		with Modelling Matrix
		and Include_Segment to Block

		List of Segments if a GRID Block Insert

TEXT		Segment with stroked Text Primitive
		and Modelling Matrix for Scaling, Rotation

LAYER		Top-Level Segmentname

COLOR		Face and Line Color by Value (R,G,B)
		or Color By Index
		several Entities with the same color are
		grouped into a separate layer subsegment

With additional tool box routines, it's possible to build a
_GLVIEW shell from a set of faces.

Changes :
	01.03.95 CPP Version
	20.01.94 Fixname, remove special characters from ACAD-Blocks
		started to add style segments for layers
	08.08.93 HG code 999 comment in dxf_parse

*/

#include "stdafx.h"

#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include <string.h>

#include "gclass.h"
#include "gshell.h"
#include "gshelli.h"
#include "gdxfin.h"
// to do: reenable //XX world->Current()->Insert(new

/***************************************************************************/
/* global define switches */

/* Parser generates _GLVIEW output */
#ifndef _GLVIEW
#define _GLVIEW
#endif

/* Parser generates printf output */
/*#undef _GLVIEW	*/

/* Parser works with AutoCAD ADS interface	*/
#undef ADS

/* Debug printouts */
#undef DEBUG

/* #define DEBUG	 */

// to do: replace
typedef char segstr[1024];
#define vector Point

typedef float tmatrix[4][4];


#define  VECSET(AA,A,B,C) VECOP2_3(AA,=,A,B,C)
#define  VECZERO(AA)  (AA).x=(AA).y=(AA).z=0.0
#define  VECADD(AA,A,B) VECOP3(AA,=,A,+,B)
#define  VECSUB(AA,A,B) VECOP3(AA,=,A,-,B)

#define VECOP2(AAA,OPOP,BBB) \
		{(AAA).x OPOP (BBB).x; (AAA).y OPOP (BBB).y; (AAA).z OPOP (BBB).z;}
#define VECOP2B(AAA,OPOP,BBB,BOP) \
		(((AAA).x OPOP (BBB).x) BOP ((AAA).y OPOP (BBB).y) BOP ((AAA).z OPOP (BBB).z))
#define VECOP2C(AAA,OPOP,BBB) \
		{(AAA).x OPOP BBB; (AAA).y OPOP BBB; (AAA).z OPOP BBB;}
#define VECOP2CX(AAA,OPOP,BBB) \
		(AAA).x OPOP BBB, (AAA).y OPOP BBB, (AAA).z OPOP BBB
#define VECOP2_3(AAA,OPOP,A,B,C) \
		{(AAA).x OPOP A; (AAA).y OPOP B; (AAA).z OPOP C;}


/* Insertstyle for inserting generated objects */
#define IPOLYS 1
#define ILINES 2
#define IPOLYSEGMENTS 4
#define ISEGMENTS 8
#define ISHELLS   16
#define ISELF 32
#define IDESCRIPTION 32

/* DXF-input */
#define ILAYER 64
#define ICOLOR 128
#define ICOLORBYINDEX 256

/* for sweep solids */
#define ITOP  256
#define IBOTTOM  512
#define ILEFT  1024
#define IRIGHT	2048
#define ICLOSE	(2048*2)
#define ICLOSE_COLS (2048*2)	/* close colums of a mesh */
#define ICLOSE_ROWS (2048*16)	/* close rows of a mesh */
#define IHARDTOPBOTTOM	(2048*4) /* make hard edge */

#define IQUAD	(2048*8)	/* use quadrilaterals (instead of triangles) */
#define IPARAMETERS (2048*16)	/* set default vertex parameters */

typedef int istyle;


long insert_plate(
	istyle style,
	int np,
	const Point * p,
	int n,
	int p1start) { return(0); }

int insert_circle_width (
	istyle style,
	float cx,
	float cy,
	float cz,
	float r,
	float width,
	int n) {return 0;};

long insert_cylinder_3d(
	istyle style,
	const Point * center,
	float radius,
	float height,
	int n) {return 0;}

#if 0
extern int compute_contour(
	const Point * p,
	int pmax,
	int ispolygon,
	float dist,
	Point * pout);
#endif

int compute_contour3d(
	const Point * p,
	int pmax,
	const Point * pnormal,
	int ispolygon,
	float dist,
	Point * pout) {return 0;}


/***************************************************************************/
/* usefull macros */

#define BIT(v, mask)	(((v) & (mask)) != 0)

#ifndef CALLOC
#define CALLOC(NELEMENTS,OFTYPE)  (OFTYPE *) calloc(NELEMENTS, sizeof(OFTYPE))
#endif

#define isstr(STR) (strcmp(curval.rstring,STR) == 0)

#ifndef streq
#define streq(STR1,STR2) (strcmp(STR1,STR2) == 0)
#endif


/***************************************************************************/
/* ADS, DXF declarations */
#ifdef ADS

#	include "ads.h"

#else

#ifdef _GLVIEW
typedef float ads_real;
#else
typedef double ads_real;
#endif


typedef ads_real ads_point[3];
typedef long ads_name[2];


/* The coordinates of a Point */
#define X  0
#define Y  1
#define Z  2


/* Binary data stream structure */

struct ads_binary { 		  /* Binary data chunk structure */
	short clen; 			  /* length of chunk in bytes */
	char *buf;				  /* binary data */
};

/*	Union for storing different ADS data types. */

union ads_u_val {
   ads_real rreal;
   ads_real rpoint[3];
   short rint;
   char *rstring;
   long rlname[2];
   long rlong;
   struct ads_binary rbinary;
};

struct resbuf {
	struct resbuf *rbnext;		  /* Allows them to be "linked" */
	short restype;
	union ads_u_val resval;
};



#include "adscodes.h"

#endif

/* color support */
#ifdef ADS
#include "color.h"

#else

/***************************************************************************/
/*
  DXF color stuff
*/

struct r_g_b {				  /* RGB colour description */
	ads_real red, green, blue;
};

/* AutoCAD standard color palette */

#define 	BLACK		0
#define 	RED 	1
#define 	YELLOW		2
#define 	GREEN		3
#define 	CYAN		4
#define 	BLUE		5
#define 	MAGENTA 	6
#define 	WHITE		7

#define 	SAT 	1.0

/* copyright for the following function */
/*
	 ________________________________________________________________________

	  Copyright (C) 1990 by Autodesk, Inc.

	  Permission to use, copy, modify, and distribute this software and its
	  documentation for any purpose and without fee is hereby granted.

	  THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
	  ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
	  MERCHANTABILITY ARE HEREBY DISCLAIMED.
	 ________________________________________________________________________

	  DESCRIPTION:

	AutoCAD colour manipulation functions

	Designed and implemented in October of 1989 by John Walker
*/
/*	ACADRGB  --  Takes	an	AutoCAD  colour  number in hsv and returns
		 red, green, and blue intensities in rgp in the  range
		 0.0 to 1.0 */

static void acadrgb(int hsv, struct r_g_b *rgp)
{
	static ads_real brightfac[5] = {  /* Brightness levels */
	   1.0, 0.65, 0.5, 0.3, 0.15
	  }, halfsat = .5;			  /* Halfway saturation */
	int ih, vs;
	ads_real h, s, f, value;

	switch (hsv) {
	case BLACK:
	rgp->red   = 0.0;
	rgp->blue  = 0.0;
	rgp->green = 0.0;
	value = 0.0;
	break;

	case RED:
	rgp->red   = SAT;
	rgp->green = 0.0;
	rgp->blue  = 0.0;
	value = 1.0;
	break;

	case YELLOW:
	rgp->red   = SAT;
	rgp->green = SAT;
	rgp->blue  = 0.0;
	value = 1.0;
	break;

	case GREEN:
	rgp->red   = 0.0;
	rgp->green = SAT;
	rgp->blue  = 0.0;
	value = 1.0;
	break;

	case CYAN:
	rgp->red   = 0.0;
	rgp->green = SAT;
	rgp->blue  = SAT;
	value = 1.0;
	break;

	case BLUE:
	rgp->red   = 0.0;
	rgp->green = 0.0;
	rgp->blue  = SAT;
	value = 1.0;
	break;

	case MAGENTA:
	rgp->red   = SAT;
	rgp->green = 0.0;
	rgp->blue  = SAT;
	value = 1.0;
	break;

	case WHITE:
	case 8:
	case 9:
	rgp->red   = SAT;
	rgp->green = SAT;
	rgp->blue  = SAT;
	value = 1.0;
	break;

	default:

	/*	The chromatic colors.  The	hue  resulting	from  an
		AutoCAD color 10-249 will be determined by its first
		two digits, and the saturation and	value  from  the
		last digit, as follows:

		Hues:

		 10 -- Red
		 50 -- Yellow
		 90 -- Green
		130 -- Cyan
		170 -- Blue
		210 -- Magenta

		Between  each  of these are three intermediate hues,
		e.g., between red and yellow, we have:

		 20 -- reddish orange
		 30 -- orange
		 40 -- yellowish orange

		To each hue number, 0, 2, 4, 6, or 8 can be added to
		give a different "value", or brightness, with 0  the
		brightest  and	8  the	weakest.   Finally, 1 can be
		added to  produce  a  "half-saturated",  or  pastel,
		color.	For example, color 18 is the dimmest red and
		10 the brightest red.  19 is the dimmest pink and 11
		the brightest pink.
	*/

	if (hsv > 9 && hsv < 250) {

		/* Apply the algorithm from Foley & van Dam to turn
		   HSV into RGB values */

		ih = (hsv - 10) / 10;	  /* Integer hue value. */
		if (ih >= 24)		  /* Range is 0-23. */
		ih -= 24;
		vs = hsv % 10;		  /* Encoded value and saturation */
		h = ih / 4.;		  /* Map into range [0.0,6.0) */
		ih = h; 		  /* The integer part. */
		f = h - ih; 		  /* Fractional part. */
		value = brightfac[vs >> 1]; /* Value in [0,1] */
		s = vs & 1 ? halfsat : 1.0; /* Saturation */

		switch (ih) {
		case 0:
		rgp->red   = 1.0;
		rgp->green = (ads_real) (1.0 - s * (1.0 - f));
		rgp->blue  = (ads_real) (1.0 - s);
		break;

		case 1:
		rgp->red   = (ads_real) (1.0 - s * f);
		rgp->green = 1.0;
		rgp->blue  = (ads_real) (1 - s);
		break;

		case 2:
		rgp->red   = (ads_real) (1.0 - s);
		rgp->green = 1.0;
		rgp->blue  = (ads_real) (1.0 - s *(1.0 - f));
		break;

		case 3:
		rgp->red   = (ads_real) (1.0 - s);
		rgp->green = (ads_real) (1.0 - s * f);
		rgp->blue  = 1.0;
		break;

		case 4:
		rgp->red   = (ads_real) (1.0 - s * (1.0 - f));
		rgp->green = (ads_real) (1.0 - s);
		rgp->blue  = 1.0;
		break;

		case 5:
		rgp->red   = 1.0;
		rgp->green = (ads_real) (1.0 - s);
		rgp->blue  = (ads_real) (1.0 - s * f);
		break;
		}
	} else {
		/* Define some extra colours from dark grey to white
		   in the 250 to 255 slots */
		value = 0.33 + (hsv - 250) * 0.134;
		rgp->red   = 1.0;
		rgp->green = 1.0;
		rgp->blue  = 1.0;
	}
	break;				  /* Default */
	}
	rgp->red   *= value;		  /* Apply lightness scale factor */
	rgp->green *= value;		  /* to components resulting from */
	rgp->blue  *= value;		  /* hue and saturation. */
}


#endif

/***************************************************************************/


#define RTEOF RTNIL
typedef short rttype;

/* copy ads_point */
#define capt(A,B) {(A)[0]=(B)[0];(A)[1]=(B)[1];(A)[2]=(B)[2];}
#define zeroapt(A) {(A)[0]=0;(A)[1]=0;(A)[2]=0;}

/* copy ads_point to _GLVIEW Point */
#define apt2vec(A,B) {A.x=(B)[0]*scalex;A.y=(B)[1]*scaley;A.z=(B)[2]*scalez;}


typedef int groupcode;

/***************************************************************************/
/* static data section */

static FILE* InFile;				/* DXF-Input File */

static groupcode	curgroup;		/* current group */
static union ads_u_val	curval; 		/* current value */
static rttype		curgroupt=RTNONE;	/* current DXF group type */
static char 	groupstr[1025];

static int polycnt=0;				/* number of polys generated */

static double scalex=1.0, scaley=1.0,scalez=1.0;

/* scale factor for doubles */
/*
static double scalex=1.0/1E6, scaley=1.0/1E6,scalez=1.0;
*/

#ifdef ADS
 struct resbuf *ebuf,*ebufp;
#endif

			/* Insert Style,which elements should be preserved */
static istyle		dxfistyle = ISHELLS|IDESCRIPTION|IPOLYS|IPOLYSEGMENTS|ILAYER;

#ifdef _GLVIEW


class GDXFReader {
public :
};

//static segstr 	  blocksroot;	  /* where the BLOCKS are located */

static GShell *curShell=NULL;
static char curShellLayer[80];	// the layer curShell is inserted
static GGroup *blocksroot;
static GWorld *world;


int takeColor=1;
int takeLayer=1;


#endif



/***************************************************************************/
/* read next dxfgroup from InFile into curgroup+curval */

groupcode getgroup(void)
{
groupcode g;
char c;
#ifdef ADS
	if (ebufp == NULL) g=RRTEOF;
	else ebufp=ebufp->rbnext;
	if (ebufp == NULL) g=RRTEOF;
	else g=eb->restype;
#else
	if (fscanf(InFile,"%d\n",&g)==EOF) {
		curgroup=0;
		curgroupt=RTSTR;
		strcpy(curval.rstring,"EOF");
		fprintf(stderr,"DXFIN:Unexepected end of file \n");

	}
#endif
	curgroup = g;
	curgroupt =RTNONE;
	if (((g>=0) && (g<=9)) || ((g>=999) && (g<=1009))) {
		curgroupt = RTSTR;
	} else {
	if (g>1000) {  /* in xdata */
		if (g==1071) curgroupt = RTLONG;
		else if (g==1004) g=0; /* empty ??? */
		else g %= 1000;

	}

	/* find DXF datatype for groupcode curgroupt */
	switch (g/10) {
	case 0 :
		curgroupt = RTSTR;
		break;
		/* 1004 = XDATA */
#ifdef ADS
	case 1:
/*		  if (etype = ET_VIEW)
		   curgroupt = RTPOINT;
		else if (etype = ET_VPORT && (g<=15))
		   curgroupt = RTPOINT;
		else
*/
		   curgoupt = RT3DPOINT;
		break;
#else
	case 1:

#endif

#ifdef ads
	case 2: case 3: case 4:
		curgroupt = RTREAL;
		break;

	case 5:
		curgroupt = RTANG;
		break;
	case 21: case 22: case 23:
		curgroupt = RT3DPOINT;
		break;
#else
	case 2: case 3: case 4: case 5:
	case 21: case 22: case 23:
		curgroupt = RTREAL;
		break;
#endif

	case 6: case 7:
		if (curgroupt==RTNONE) curgroupt = RTSHORT;
		break;
	default:
		if ((g==-1) || (g==-2)) curgroupt = RTENAME;
		else if (g==-3) curgroupt = RTSHORT;
		else curgroupt = RTNONE;
		break;
	}
	}
	switch (curgroupt) {
	case RTNONE:

	case RTSTR:
	  curval.rstring = &groupstr[0];
	  if ((c=fgetc(InFile)) == '\n')
		  curval.rstring[0]='\0';
	  else {
		 ungetc(c,InFile);
		 fgets(curval.rstring,1024,InFile); /* Turbo C kludge , TC doesn't read empty lines */
		 if (strlen(curval.rstring) > 0) {
		curval.rstring[strlen(curval.rstring)-1]='\0';
		  }
	  }

	  break;

	case RTREAL:
	case RTANG:
	  fgets(&groupstr[0],1024,InFile);
	  if (sizeof(ads_real)==sizeof(float)) { double d=0.0;
		   sscanf(groupstr,"%lf",&d);
		   if (d>=FLOAT_MAX) {
		  fprintf(stderr,"dxfin: number exceeds float range : %lf \n",d);
		  d=FLOAT_MAX;

		   } else
		   if (d<= -FLOAT_MAX) {
		  fprintf(stderr,"dxfin: number exceeds float range : %lf \n",d);
		  d=-FLOAT_MAX;
		   }
		   curval.rreal=d;
	  }
	  else sscanf(groupstr,"%lf",&curval.rreal);
	  break;

	case RTSHORT:
	  fgets(&groupstr[0],1024,InFile);
	  sscanf(groupstr,"%hd\n",&curval.rint);
	  break;

	case RTLONG:
	  fgets(&groupstr[0],1024,InFile);
	  sscanf(groupstr,"%ld\n",&curval.rlong);
	  break;
	default:
	  printf("dxfin.getgroup:don't know how to read in type %d \n",curgroupt);
	  break;
	}

	return(curgroup);
}

/***************************************************************************/
/* print out curgroup for debugging */
void printcurgroup(void)
{
	printf("%d\t:",curgroup);
	switch (curgroupt) {
	case RTSTR:
	  printf("%s\n",curval.rstring);
	  break;

	case RTANG:
	case RTREAL:
	  if (sizeof(ads_real)==sizeof(float))
		 printf("%f\n",curval.rreal);
	  else printf("%lf\n",curval.rreal);
	  break;

	case RTPOINT:
	  if (sizeof(ads_real)==sizeof(float))
		  printf("(%f, %f)\n",curval.rpoint[X],curval.rpoint[Y]);
	  else printf("(%lf, %lf)\n",curval.rpoint[X],curval.rpoint[Y]);
	  break;

	case RT3DPOINT:
	  if (sizeof(ads_real)==sizeof(float))
		 printf("(%f, %f, %f)\n",curval.rpoint[X],curval.rpoint[Y],curval.rpoint[Z]);
	  else printf("(%lf, %lf, %lf)\n",curval.rpoint[X],curval.rpoint[Y],curval.rpoint[Z]);
	  break;

	case RTSHORT:
	  printf("%d\n",curval.rint);
	  break;

	case RTLONG:
	  printf("%ld\n",curval.rint);
	  break;
	case RTENAME:
	  printf("<Entity name :%8lx>\n",curval.rlname[0]);
	  break;
	default:
	  printf("<Type %d>\n",curgroupt);
	  break;
	}
}


/***************************************************************************/
/*
  static attribute settings for parsing
*/
static char secname[80],		/* current section name */
		name[80];

/* common object variables	*/
static char oltyp[80];		/* linetype */
static char olayer[80]; 	/* layer */
static ads_real ozoffset,ozheight;	/* elevation */
static int	ohasheight; 	/* object has eleveation, i.e. is solid */
static ads_point ozdir; 		/* object elevation/EKS vector, group 210-230 */
static int	ohaseks;		/* object has EKS property */
static int	ocolor; 		/* object DXF color index */
static int	netocolor;		/* net DXF color index */
static int	ogenend_close=0;	/* flag for closing segment ogenend */

static ads_point opts[4];		/* 4 Object definition points */



#ifdef _GLVIEW

void FlushCurrentShell() {
	curShell = 0;
}

GShell* GetCurrentShell()
{
   
   if (!streq(olayer,curShellLayer)) {
      FlushCurrentShell();
   }
   
   if (curShell == NULL) {
       curShell = new GShell();
	   //XX world->Current()->Insert(curShell);
	   strcpy(curShellLayer,olayer);	// save cur layer
   }
   return (curShell);
}

#endif

/* compute a transform mapping points to eks */

int compute_to_eks_transform(double nx,double ny,double nz,Matrix &m)
{ Point xdir,ydir,zdir;
  double neps=1.0/64.0;
/*	tmatrix m; */

  VECSET(zdir,nx,ny,nz);
  zdir.Normalize();

  /* find x + y axis for given normal = zdir */
  if ((fabs(zdir.x)<neps) && (fabs(zdir.y)<neps)) {
	 VECSET(ydir,0,1,0);
	 xdir = CrossProd(ydir,zdir);
  } else {
	 VECSET(ydir,0,0,1);
	 xdir = CrossProd(ydir,zdir);
  }
  xdir.Normalize();
  ydir = CrossProd(zdir,xdir);
  ydir.Normalize();
/// ???????????  VECOP2C(ydir,*=,-1);

  m[0][0]= xdir.x; m[0][1]= ydir.x; m[0][2]= zdir.x; m[0][3]= 0.0;
  m[1][0]= xdir.y; m[1][1]= ydir.y; m[1][2]= zdir.y; m[1][3]= 0.0;
  m[2][0]= xdir.z; m[2][1]= ydir.z; m[2][2]= zdir.z; m[2][3]= 0.0;
  m[3][0]= 0.0; m[3][1]= 0.0; m[3][2]= 0.0; m[3][3]= 1.0;

/*	return(HC_Compute_Matrix_Inverse(m,mtoeks)); */
//  return(HC_Compute_Matrix_Inverse(m,m));
/*	return(1); */
// ?????????	m.Invert();
	return(1);
}

int get_elevation_vector(double height,vector *v)
{
  VECSET(*v,height*ozdir[X],height*ozdir[Y],height*ozdir[Z]);
  return 1;
}

/***************************************************************************/
/* start of a new object */
void ostart(void)
{
  /* Reset attributes */
  oltyp[0]='\0';
  olayer[0]='\0';
  ozdir[X]=ozdir[Y]=ozoffset=ozheight=0.0;
  ozdir[Z]=1.0;
  ohaseks=0;
  ohasheight=0;
  ocolor=-1;
  netocolor=-1;
/*	zeroapt(ozdir); */
  zeroapt(opts[0]);
  zeroapt(opts[1]);
  zeroapt(opts[2]);
  zeroapt(opts[3]);

#if 0
/*def DEBUG */
  printf("Object ");
  printcurgroup();
#endif
}

/***************************************************************************/
/* parse common DXF groups : linetype, layer, elevation, color
   and object points 1-4  into global variables
*/

int oparse1(void) {
  switch (curgroup) {
  /* case 6: Referenzwert hex */
  case 6 : strcpy(oltyp,curval.rstring); break;
  case 8 : strcpy(olayer,curval.rstring); break;
  case 38 : ozoffset=curval.rreal; break;
  case 39 : ozheight=curval.rreal; ohasheight=1; break;
  case 210 : ozdir[X]=curval.rreal; ohaseks = ohaseks || (ozdir[X] != 0.0);  break;
  case 220 : ozdir[Y]=curval.rreal; ohaseks = ohaseks || (ozdir[Y] != 0.0); break;
  case 230 : ozdir[Z]=curval.rreal; ohaseks = ohaseks || (ozdir[Z] != 1.0); break;
  case 62 : ocolor=curval.rint; break;
/*	case 67 : omodell=curval.rint; break; */
  case 10: case 11: case 12: case 13: opts[curgroup-10][X]=curval.rreal; break;
  case 20: case 21: case 22: case 23: opts[curgroup-20][Y]=curval.rreal; break;
  case 30: case 31: case 32: case 33: opts[curgroup-30][Z]=curval.rreal; break;
  default : return(0);
  }
  return(1);
}

/***************************************************************************/
/* end of object */
void oend(void)
{
}

#ifdef _GLVIEW

/***************************************************************************/
static char sbuf[160];

/* remove _GLVIEW Wildcards out of AutoCAD names	*/

char *fixname(char *s) {
   char *p=sbuf;
   strcpy(sbuf,s);
   if (*p == 0) printf("Warning emptyname = %s \n",s);

   while (*p != '\0') {
	if (*p> 128) *p = '#';
	else switch (*p) {
	case '.':       /* _GLVIEW Meta Characters */
	case '/':
	case '*':
	case '?':
	case '(':
	case ')':
	case '<':
	case '>':
	case '\'':
	case '"':
		*p='_';
		break;
	default :
		break;
	}
	p++;
   }
/*	 printf("Fixname(%s) = %s \n",s,sbuf); */
   return (sbuf);
}
/***************************************************************************/
/*
  apply eks transformation
*/
int need_eks_transform(void)
{
  return (!(iszero(ozdir[X]) && iszero(ozdir[Y]) && iszero (1.0-ozdir[Z])));
}

int append_eks_matrix(void)
{ Matrix m;
  Point dir;
  if (!(iszero(ozdir[X]) && iszero(ozdir[Y]) && iszero (1.0-ozdir[Z]))) {
/*	  apt2vec(dir,ozdir);
	compute_toz_rotation(&dir,m);
*/
	printf(" append eks transformation for normal : %f %f %f \n",ozdir[X],ozdir[Y],ozdir[Z]);
	if (compute_to_eks_transform(ozdir[X],ozdir[Y],ozdir[Z],m))
	  //HC_Append_Modelling_Matrix(m);	  to do
	  world->Current()->Set(new GMatrix(m));
	return(1);
 }
 return(0);
}


#endif

/***************************************************************************/
/* take acad ocolor
   acad colors ignored
   -1 = invalid
   0 = from block ? black
   7 = default white (on black)
   256 = from layer ?
*/
int takeacadcolor(int ocolor)
{
#ifdef _GLVIEW
 return (takeColor && (ocolor>0) && (ocolor<256) && (ocolor!=7));
#else
 return 0;
#endif
}

static int style_layers=1;

int open_layer_segment(char *olayer)
{ char * layer;
#ifdef _GLVIEW
	if ((olayer[0]!='\0') && (takeLayer)) {
	   layer=fixname(olayer);
#ifdef _TODO
	   if (style_layers) {
		  if (!exists_segment(layer)) {
			 segstr style;
			 strcpy(style,blocksroot);
		 	strcat(style,"/layers/");
		 	strcat(style,layer);
		 	world->Open(layer);
			if (!exists_segment(style)) {
		   		world->Open(style);
			 	printf("Creating style %s \n",style);
		   		world->Close();
			}
			HC_Style_Segment(style);
		 world->Close();
	  	}
	   }
#endif
	   world->Open(layer);
	   return(1);
	}
#endif
  return(0);
}


/***************************************************************************/
/*
 start of object generation
 if LAYERS are requested, open layer segment,
 if each object should get its own segment, create segment
 set color
*/

void ogenstart(void)
{ int c;
  char tmp[80];
/* printf ("Ogen start : ocolor = %d \n",ocolor); */

  c=takeacadcolor(ocolor);
  ogenend_close=0;

#ifdef _GLVIEW
/*
	if ((olayer[0]!='\0') && (takeLayer))
	world->Open(fixname(olayer));
*/
	open_layer_segment(olayer);

	if (c) { /* create color subsegment */
	   struct r_g_b rgb;
	   sprintf(tmp,"_c%d",ocolor);
	   world->Open(tmp);
	if (BIT(dxfistyle,ICOLORBYINDEX)) {
/*		HC_Set_Color_By_Index("text,faces,lines",ocolor); */
//	  HC_Set_Color_By_Index("geometry",ocolor);
	} else {
	   acadrgb(ocolor,&rgb);   /* convert ACAD color to rgb */
/*		 HC_Set_Color_By_Value("text,faces,lines","rgb",rgb.red,rgb.green,rgb.blue); */
//	   HC_Set_Color_By_Value("geometry","rgb",rgb.red,rgb.green,rgb.blue);
	   world->current->Set(new GFaceColor(Point(rgb.red,rgb.green,rgb.blue)));

	}
	/* alternate implementations :
	   Color By Index, and a ACAD colormap
	   set edge colors too
	*/
	netocolor=ocolor;
	ogenend_close++;
	}
	if (BIT(dxfistyle,IPOLYSEGMENTS)) {
	   world->Open("");
	   ogenend_close++;
	}
#endif
}

void olayerstart(void)
{ int c;
  char tmp[80];

  c=takeacadcolor(ocolor);
  ogenend_close=0;

#ifdef _GLVIEW
	if ((olayer[0]!='\0') && (takeLayer))
	world->Open(fixname(olayer));

// _TODO
	if (c) { /* set color */
	   struct r_g_b rgb;
	if (BIT(dxfistyle,ICOLORBYINDEX)) {
/*		HC_Set_Color_By_Index("text,faces,lines",ocolor); */
//	  HC_Set_Color_By_Index("geometry",ocolor);
	} else {
	   acadrgb(ocolor,&rgb);   /* convert ACAD color to rgb */
/*		 HC_Set_Color_By_Value("text,faces,lines","rgb",rgb.red,rgb.green,rgb.blue); */
//	   HC_Set_Color_By_Value("geometry","rgb",rgb.red,rgb.green,rgb.blue);
	   world->current->Set(new GFaceColor(Point(rgb.red,rgb.green,rgb.blue)));
	}
	/* alternate implementations :
	   Color By Index, and a ACAD colormap
	   set edge colors too
	*/
	netocolor=ocolor;
	}
#endif
}

/***************************************************************************/
/*
  start
   of a new object subface
*/

void ofacestart(void)
{ int c;
  c=takeacadcolor(ocolor);

#ifdef _GLVIEW
// _TODO
	if (BIT(dxfistyle,IPOLYSEGMENTS) || c)
		world->Open("");
	if (c) {
		struct r_g_b rgb;
		if (netocolor!=ocolor) {
	 		acadrgb(ocolor,&rgb);
/*	 		HC_Set_Color_By_Value("faces","rgb",rgb.red,rgb.green,rgb.blue); */
//		 	HC_Set_Color_By_Value("geometry","rgb",rgb.red,rgb.green,rgb.blue);
	   		world->current->Set(new GFaceColor(Point(rgb.red,rgb.green,rgb.blue)));
		}
	}
#endif
}

void oeksstart(void)
{
  if (ohaseks) {
#ifdef _GLVIEW
	 world->Open("");
	 append_eks_matrix();	/* apply modelling coordinate system */
#endif
  }
}

void oeksend(void)
{
  if (ohaseks) {
#ifdef _GLVIEW
	 world->Close();
#endif
  }
}

/***************************************************************************/
/*
 start of object generation
 if LAYERS are requested, open layer segment,
 if descriptions are requested, store description in User_Options
 if each object should get its own segment, create segment
 set color
*/



void objgenstart(char *obj)
{ int c;
  c=takeacadcolor(ocolor);

  ogenend_close=0;

#ifdef _GLVIEW
	if ((olayer[0]!='\0') && (takeLayer)) {
	   ogenend_close++;
	   /* world->Open(fixname(olayer)); */
	   open_layer_segment(olayer);
	}
//  _TODO
	if (BIT(dxfistyle,ISEGMENTS) || (c)) {
		world->Open("");
		ogenend_close++;

	if (BIT(dxfistyle,IDESCRIPTION)) {
// to do		HC_Set_User_Options(obj);
	}
	if (c) {
		struct r_g_b rgb;
		if (BIT(dxfistyle,ICOLORBYINDEX)) {
/*		HC_Set_Color_By_Index("text,faces,lines",ocolor); */
//	  	HC_Set_Color_By_Index("geometry",ocolor);
		} else {
			acadrgb(ocolor,&rgb);
/*	  		HC_Set_Color_By_Value("faces,lines","rgb",rgb.red,rgb.green,rgb.blue); */
			//HC_Set_Color_By_Value("geometry","rgb",rgb.red,rgb.green,rgb.blue);
 	    	world->current->Set(new GFaceColor(Point(rgb.red,rgb.green,rgb.blue)));

	}
	netocolor=ocolor;
  	}
	}
#endif
}

/***************************************************************************/
/*
  end of object generation
  close object segment and layer segment

*/

void ogenend(void)
{
#ifdef _GLVIEW

	while (ogenend_close>0) {
		world->Close();
		ogenend_close--;
	}
	if ((olayer[0]!='\0') && (takeLayer))
	world->Close();
#endif
}

void olayerend(void)
{
#ifdef _GLVIEW
	if ((olayer[0]!='\0') && (takeLayer))
		world->Close();
#endif
}

/***************************************************************************/
/*
   end of subface
*/

void ofaceend(void)
{
#ifdef _GLVIEW
	if (BIT(dxfistyle,IPOLYSEGMENTS)||(takeacadcolor(ocolor))) world->Close();
#endif
}

void objgenend(void)
{
#ifdef _GLVIEW
	while (ogenend_close>0) {
		world->Close();
		ogenend_close--;
	}
#endif
}

#ifdef _GLVIEW

/***************************************************************************/
/* generate poly */

int genpoly(ads_point *pts,int cnt,int flags)
{	int i;
	int ret=-1;
	ads_point *p=pts;
	PointArray v(cnt);

/*	printf("\n Poly #%d :",cnt); */
/*	for (i=0;i<cnt;i++,p++) printf("(%lf, %lf, %lf) ",*p[0],*p[1],*p[2]); */

	if (v.Data() != NULL) {

	for (i=0;i<cnt;i++,p++) {apt2vec(v[i],*p);}

#ifdef DEBUG
/*	  printf("\n POLY "); */
/*	  for (i=0;i<cnt;i++) printf("(%lf, %lf, %lf) ",v[i].x,v[i].y,v[i].z); */
#endif

	ofacestart();

	polycnt++;
	if (BIT(flags,1)) {
//		 ret=HC_KInsert_Polygon(cnt,v);
	   //XX world->Current()->Insert(new GPolygon(cnt,v));

//XX		 GetCurrentShell()->AppendPolygon(v,1);
	} else 
	   //XX world->Current()->Insert(new GPolyline(cnt,v));
	//	ret=HC_KInsert_Polyline(cnt,v);  /* 15.02.92 */

	ofaceend();

	}
	return(ret);

}

// add a polygon to current shell with lookup
int addshellface(ads_point *pts,int cnt)
{	int i;
	int ret=-1;
	ads_point *p=pts;
	PointArray v(cnt);

	if (v.Data() != NULL) {

	for (i=0;i<cnt;i++,p++) {apt2vec(v[i],*p);}
	
	polycnt++;
    GetCurrentShell()->AppendPolygon(v,1);
	
	}
	return(ret);

}



/***************************************************************************/
/* insert 3d polyline */

int genpolyheight(ads_point *pts,int npts,int flags,ads_real *height)
{ int i;
  int ret=-1;
  int n=npts;
  int np=npts*2;
  int istyle;
  PointArray p(np);
  vector elevation;


/*	printf("\n Poly #%d :",cnt); */
/*	for (i=0;i<cnt;i++,p++) printf("(%lf, %lf, %lf) ",*p[0],*p[1],*p[2]); */

  if (p.Data() !=NULL) {
	 for(i=0;i<n;i++)
	 {	apt2vec(p[i],pts[i]);
		p[i+n]=p[i];
		get_elevation_vector(height[i],&elevation);
		p[i+n] += elevation; /* elevated Point */
	 }

	ofacestart();

	polycnt++;
	istyle=dxfistyle;
	if (BIT(flags,1)) istyle=istyle|ICLOSE|ITOP|IBOTTOM; /* closed */

	ret=insert_plate(istyle,np,p,npts,npts);

	ofaceend();

   }
   return(ret);
}



/***************************************************************************/
/* insert 3d polyline */
/* p[0..4*pmax-1]
*/
long insert_polygon3d_width(int pmax,Point p[],int ispolygon,int is2d,float wall_width1,float wall_width2,
		/*Vector*/ Point *elevation)
{ Point *cp1p,*cp2p;
  int i,pmax2;
  int fpmax;
  int *fp,*fpi;
  long key=-1;

  PointArray cp(pmax*2);

	if (cp.Data()!=NULL) {
/*	   show definition points
	   if (ispolygon) curSegment->Insert(new GPolygon(pmax,p));
	   else curSegment->Insert(new GPolyline(pmax,p));
	   HC_Update_Display();
*/
	   if (!iszero(wall_width1)) { /* compute outer contour */
	   cp1p=cp;
	   compute_contour3d(p,pmax,NULL,ispolygon,wall_width1,cp1p);
	   if (!ispolygon && is2d) {
#if 0
		  key=HC_KInsert_Polyline(pmax,cp1p); /* insert outer countour */
#endif
	   }
	   } else { cp1p=cp; memcpy(cp1p,p,pmax*sizeof(Point)); }

	   if (!iszero(wall_width2)) { /* compute inner contour */
	   cp2p=&cp[pmax];
	   compute_contour3d(p,pmax,NULL,ispolygon,wall_width2,cp2p);
#if 0
	   if (!ispolygon && is2d) {
		   key=HC_KInsert_Polyline(pmax,cp2p); /* insert inner countour */
	   }
#endif
	   } else { cp2p=&cp[pmax]; memcpy(cp2p,p,pmax*sizeof(Point)); }

	   if (is2d) {
	   if (ispolygon) {
	   /* insert shell with two polygons, subtract second from first */
	   int i,fpmax=2*(pmax+1);
	   int *fp;
	   fp=CALLOCT(fpmax,int);
	   if (fp!=NULL) { /* create shell face list */
		  fp[0]=pmax; fp[0+pmax+1]=-pmax;
		  for (i=0;i<pmax;i++) {fp[i+1]=i; fp[i+1+pmax+1]=i+pmax; }

//		  key=HC_KInsert_Shell(pmax*2,cp,fpmax,fp);
		  GShell *obj=new GShell(pmax*2,cp,fpmax,fp);
		  //XX world->Current()->Insert(obj);
		  FREE(fp);
	   }
	   }
	   else {
	  Point ptemp;
	  int j;
	  /* polyline, reverse second contour, and generate polygon */
	  for(i=0,j=pmax-1;i<j;i++,j--) {
		 ptemp=cp[pmax+i];
		 cp[pmax+i]=cp[pmax+j];
		 cp[pmax+j]=ptemp;
	  }
//	  key=HC_KInsert_Polygon(pmax*2,cp);
	  //XX world->Current()->Insert(new GPolygon(pmax*2,cp));
	   }
	   }

	memcpy(&p[0],&cp[0],pmax*2*sizeof(Point)); /* replace out points */
  }

  if (!is2d) { /* generate a shell with elevation */
  pmax2=pmax*2;

  /* compute elavated (top) points */
  for (i=0; i<(pmax2);i++) p[i+pmax2] = p[i] + *elevation;

  /* create shell face indices ,
	assume left handed coordinates
  */
  fpmax=(pmax- (!ispolygon))*5*4 + (!ispolygon)*2*5;

#define putface4(P,A,B,C,D) { *P=4; P++; *P=A; P++; *P=B; P++; *P=C; P++; *P=D; P++; }
#define putface4b(P,D,C,B,A) { *P=4; P++; *P=A; P++; *P=B; P++; *P=C; P++; *P=D; P++; }

  fp=CALLOCT(fpmax,int);
  if (fp!=NULL) { fpi=fp;
	  if (!ispolygon) putface4(fpi,0,pmax,pmax2+pmax,pmax2); /* first front face */
	  for(i=1;i<pmax;i++) {
		putface4b(fpi,i-1,i,i+pmax,i+pmax-1); /* bottom */
		putface4(fpi,pmax2+i-1,pmax2+i,pmax2+i+pmax,pmax2+i+pmax-1); /* top */
		putface4(fpi,i-1,i,pmax2+i,pmax2+i-1); /* left */
		putface4b(fpi,pmax+i-1,pmax+i,pmax2+pmax+i,pmax2+i+pmax-1); /* right */
	  }
	  if (!ispolygon) { putface4b(fpi,pmax-1,pmax2-1,pmax2+pmax2-1,pmax2+pmax-1); }
		  /* last back face for polyline wall */
	  else { /* closing wall for polygon wall */
		i=0;
		putface4b(fpi,pmax-1,i,i+pmax,pmax+pmax-1); /* bottom */
		putface4(fpi,pmax2+pmax-1,pmax2+i,pmax2+i+pmax,pmax2+pmax+pmax-1); /* top */
		putface4(fpi,pmax-1,i,pmax2+i,pmax2+pmax-1); /* left */
		putface4b(fpi,pmax+pmax-1,pmax+i,pmax2+pmax+i,pmax2+pmax+pmax-1); /* right */
	  }
	  //HC_Set_Visibility("Markers=off");
	  //key=HC_KInsert_Shell(pmax*4,p,fpmax,fp);
	  //XX world->Current()->Insert(new GShell(pmax*4,p,fpmax,fp));

	  FREE(fp); fp=NULL;
  }
  }
#undef putface4
#undef putface4b
  return(key);
}

int genpoly_width_height(ads_point *pts,int npts,int flags,float width,ads_real *height,ads_point *oz)
{ int i;
  int ret=-1;
  int n=npts;
  int np=npts*2;
  int is2d;
  PointArray p(np*4);
  vector elevation;


/*	printf("\n Poly #%d :",cnt); */
/*	for (i=0;i<cnt;i++,p++) printf("(%lf, %lf, %lf) ",*p[0],*p[1],*p[2]); */


  get_elevation_vector(height[0],&elevation);

  is2d = (height == NULL) || iszero(height[0]);

#ifdef DEBUG
  printf("genpoly width height #npts %d, flags =%d, is2d %d, width %f v = (%f %f %f) \n",npts,flags,is2d,width,VECX(elevation));
#endif

  if (p.Data() != NULL) {
	 for(i=0;i<n;i++)
	 {	apt2vec(p[i],pts[i]);
#if 0
	p[i+n]=p[i];
	get_elevation_vector(height[i],&elevation);
	VECOP2(p[i+n],+=,elevation); /* elevated Point */
#endif
	 }


   ofacestart();
	 insert_polygon3d_width(npts,p,BIT(flags,1),is2d,-width*0.5,width*0.5,&elevation);
   ofaceend();
   ret=0;
   }
   return(ret);
}


#else

int genpoly(ads_point *pts,int cnt,int flags)
{ int i;
  ads_point *p=pts;
  printf("\n Poly #%d :",cnt);
  for (i=0;i<cnt;i++,p++) printf("(%lf, %lf, %lf) ",(*p)[0],(*p)[1],(*p)[2]);
  return(0);
}

int genpolyheight(ads_point *pts,int cnt,int flags,ads_real *height)
{ int i;
  ads_point *p=pts;
  printf("\n Poly #%d :",cnt);
  for (i=0;i<cnt;i++,p++) printf("(%lf, %lf, %lf) ",(*p)[0],(*p)[1],(*p)[2]);
  return(0);
}

#endif

/***************************************************************************/
/*
  generate polygons out of 4 separate points
*/
void genquadp(ads_point *a,ads_point *b,ads_point *c,ads_point *d)
{ads_point p[4];
 capt(p[0],*a);
 capt(p[1],*b);
 capt(p[2],*c);
 capt(p[3],*d);
 genpoly(&p[0],4,1);
}

/* in Z direction elevated line  */
void genquadpw(ads_point *a,ads_point *b,ads_real wa,ads_real wb)
{ads_point p[4];
 capt(p[0],*a);
 capt(p[1],*b);
 capt(p[2],*b);
 capt(p[3],*a);
 p[2][X]+=ozdir[X]*wa; p[2][Y]+=ozdir[Y]*wa; p[2][Z]+=ozdir[Z]*wa;
 p[3][X]+=ozdir[X]*wb; p[3][Y]+=ozdir[Y]*wb; p[3][Z]+=ozdir[Z]*wb;
 genpoly(&p[0],4,1);
}

/***************************************************************************/
/* Shell data */
static int s_face_n;				/* number of shell faces */
static GFaceList s_faces;			/* shell face list */
static IntArray s_edgevis;			/* unvisible edges list */
static IntArray s_facecolor;		/* face color list  (face index, face color) */

//static int s_faces_max,s_faces_cnt; /* allocated, current length */
//static int s_edgevis_max,s_edgevis_cnt;
//static int s_facecolor_max,s_facecolor_cnt;


/***************************************************************************/
/* start generating a shell */
void shellgenstart(int faceCnt)
{
   s_face_n=0;
   s_faces.Flush();
   s_faces.SetMaxLength(faceCnt*5); // pre allocate
   s_edgevis.Flush();
   s_facecolor.Flush();
}

/***************************************************************************/
/* end generating a shell */
void shellgenend(void)
{
   s_faces.Flush();
   s_edgevis.Flush();
   s_facecolor.Flush();
 }

/***************************************************************************/
/* store a face as a polygon , 3 <= picnt <=4 */

int genface(ads_point *pts,int *pi,int picnt,int flags)
{ ads_point p[10],*pp;
  int i,j;
  for (i=0;i<picnt;i++) {
	j=abs(*(pi+i))-1; /* get Point index, < 0 = invisible edge */
	pp=pts+j;
	capt(p[i],(*pp));
  }
  genpoly(&p[0],picnt,1);
  return(0);
}

/***************************************************************************/
/* store a face into shell facelist  */

int genface_s(ads_point *pts,int *pi,int picnt,int flags)
{ ads_point p[10],*pp;
  int myfacelist[10];
  int myinvisedges[20];
  int invis=0;
  int i,j;

  /* Build _GLVIEW facelist */
  myfacelist[0]=picnt;
  for (i=0;i<picnt;i++) {
	j=abs(*(pi+i))-1; /* get Point index, < 0 = unvisible edge */
	if (pi[i]<0) {
	  myinvisedges[invis]=j;
	  myinvisedges[invis+1] =( ((i+1)>=picnt) ? myfacelist[1] : abs(*(pi+i+1))-1);
	  invis+=2;
	}
	myfacelist[i+1]=j;
  }
  s_face_n++; /* increment number of faces cnt */
  
  /* append the new face to s_faces */
  s_faces.Append(picnt+1,myfacelist);
  /* append the edge visibilitys to s_edgevis */
  // to do: not supported in GShell if (invis>0) s_edgevis.Append(invis,myinvisedges);
  return(0);
}

/***************************************************************************/
/*
  create a GLVIEW shell
*/

int genshell(ads_point *pts,int npts)
{  long key;
   int i;

#ifdef DEBUG
	printf("Shell : Points : %d, Faces : %d List length : %d\n",npts,s_face_n,s_faces_cnt);
	printf("%d Edgevisibilitys, %d facecolors \n",s_edgevis_cnt/2,s_facecolor_cnt/2);
#endif

#ifdef _GLVIEW
   if (sizeof(ads_point) != sizeof(Point))
   {
	   printf("dxfin.genshell : Bad Point format \n");
	   return(-1);
   }

   GShell *obj = new GShell(npts, (Point *) pts, s_faces.Length(),s_faces.Data());
   
   if (s_facecolor.Length()>0) {		/* set face colors */
   
   obj->SetFC(obj->GetFaceCount(),NULL);
	{	struct r_g_b rgb;
		acadrgb(netocolor,&rgb);  /* convert ACAD color to rgb */
		for (i=0; i<obj->fc.Length(); i++) obj->fc[i].Set(rgb.red,rgb.green,rgb.blue);
	}
   
   for (i=0; i< s_facecolor.Length(); i+=2) {
		{	struct r_g_b rgb;
			acadrgb(s_facecolor[i+1],&rgb);  /* convert ACAD color to rgb */
			obj->fc[s_facecolor[i]].Set(rgb.red,rgb.green,rgb.blue);
		}
	 }
  }
   
   //XX world->Current()->Insert(obj);

#if 0
   key=HC_KInsert_Shell(npts,(Point *) pts, s_faces_cnt,s_faces);
   HC_Set_Visibility("markers=off");

   /* process local geometry */
   HC_Open_Geometry(key);
	 if (s_edgevis!=NULL) { 	/* set edge visibilitys */
	for (i=0;i<s_edgevis_cnt;i+= 2) {
		if (s_edgevis[i]<s_edgevis[i+1])
		   HC_Open_Edge(s_edgevis[i],s_edgevis[i+1]);
		else
		   HC_Open_Edge(s_edgevis[i+1],s_edgevis[i]);

		   HC_Set_Visibility("edges=off");
		HC_Close_Edge();
		}
	 }
 	if (s_facecolor!=NULL) {		/* set face colors */
	for (i=0;i<s_facecolor_cnt;i+=2) {
		if (s_facecolor[i+1] != netocolor) {  /* reduce colors */
		HC_Open_Face(s_facecolor[i]);
		{	struct r_g_b rgb;
		acadrgb(s_facecolor[i+1],&rgb);  /* convert ACAD color to rgb */
		HC_Set_Color_By_Value("face","rgb",rgb.red,rgb.green,rgb.blue);
		 }
		HC_Close_Face();
		}
		}
	 }
   HC_Close_Geometry();
#endif

#endif

 return(0);
}


/***************************************************************************/
/*
  store a m*n DXF mesh
*/
int genmesh(ads_point *pts,int m,int n, int flags)
{
  ads_point *a,*b,*a1,*a2,*b1,*b2;
  int rstep=n;	/* rowstep */
  int mi,ni;
  a=pts;
  for (mi=1;mi<m;mi++) {
	b=a+rstep;
	a1=a; b1=b;
	for (ni=1;ni<n;ni++) {
	 a2=a1+1; b2=b1+1;
	 genquadp(a1,b1,b2,a2);
	 a1=a2; b1=b2;
	}
	a=b;
  }
  if (BIT(flags,1)) { /* close m */
	a=pts+(m-1)*rstep;
	b=pts;
	a1=a; b1=b;
	for (ni=1;ni<n;ni++) {
	  a2=a1+1; b2=b1+1;
	  genquadp(a1,b1,b2,a2);
	  a1=a2; b1=b2;
	}
  }
  if (BIT(flags,32)) { /* close n */
	a1=pts+n-1;
	b1=pts;
	for (mi=1;mi<m;mi++) {
	 a2=a1+rstep; b2=b1+rstep;
	 genquadp(a1,b1,b2,a2);
	 a1=a2; b1=b2;
	}
	if (BIT(flags,1)) genquadp(pts+(m*n)-1,pts+n-1,pts,pts+(n-1)*rstep);
  }
  return(0);
}

/***************************************************************************/
int genmesh__GLVIEW(ads_point *pts,int m,int n, int flags)
{
  ads_point *a;
  int rstep=n;	/* rowstep */
  int mi,ni;
  int rows,cols;
  Point *mesh,*meshp; /* _GLVIEW mesh points */
  int meshrstep;

  rows=m+(BIT(flags,1)!=0);
  meshrstep=cols=n+(BIT(flags,32)!=0);

#ifdef DEBUG
  printf("genmesh m=%d n=%d flags=%d ==> rows=%d cols=%d \n",m,n,flags,rows,cols);
#endif

  mesh=CALLOCT(rows*cols,Point);

  if (mesh==NULL) return(-1);

  a=pts;
  meshp=mesh;
  for (mi=0;mi<m;mi++) {
	for (ni=0;ni<n;ni++) {
	 	apt2vec(meshp[ni],a[ni]); /* copy to mesh */
	}
	if (BIT(flags,32)) { meshp[n]=meshp[0]; }	/* close m */
	a+=rstep;
	meshp+=meshrstep;
  }

  if (BIT(flags,1)) { /* close m */
	 memcpy(&mesh[m*cols],&mesh[0],cols*sizeof(Point));
  }

#ifdef _GLVIEW
  {long k;
//    world->Current()->Insert(new GMesh(cols,rows,mesh));
    //XX world->Current()->Insert(new GShell(cols,rows,mesh));

  //k=HC_KInsert_Mesh(rows,cols,mesh);
  //HC_Set_Visibility("edges=(generics=off)");
  //HC_Set_Visibility("markers=off");
  }
#endif

  FREE(mesh);
  return(0);
}


/***************************************************************************/
/* DXF parsing routines */

int dxf_get_polyline(void)
{
int pcntrf=0,pflags=0;
ads_real pabreite=0.0,pebreite=0.0;  /* start width, end width */
ads_real pwidth =0.0;

int pnetzm=0,pnetz_cnt=0,pnetzn=0,pgnetzm=0, pgnetzn=0,pgtyp=0;
int ppnts=0;

//Array<ads_point> netzp;				// meshp
ads_point *netzp=NULL,  *pnetzp=NULL;	/* mesh points + Pointer */
ads_point *polyp=NULL, *ppolyp=NULL;	/* Polyline points + Pointer */

static ads_point  polybuf[300]; 	/* static simple polyline buffer */
static ads_real   polyheight[300];
ads_real *ppolyh;

int phasheight=0;			/* polygon has height */

ads_real vx=0.0, vy=0.0, vz=0.0;
ads_real vabreite=0.0,vebreite=0.0; /* start and end width	*/
ads_real vwidth =0.0;
int vflags=0;
int vi[4];				/* one shell face */
int vimax=0;
char player[80];			/* Polygon layer */

int  end=0;

 ostart();

 /* read DXF-polyline header */
 while(!end) {
	getgroup();
	switch (curgroup) {
	case 66: pcntrf = curval.rint; break;
	case 70: pflags = curval.rint; break;
	case 39: vwidth = pwidth= curval.rreal; break;

	case 40: vabreite= pabreite = curval.rreal; break;
	case 41: vebreite= pebreite = curval.rreal; break;
	case 71: pnetzm = curval.rint; break;
	case 72: pnetzn = curval.rint; break;
	case 73: pgnetzm = curval.rint; break;
	case 74: pgnetzn = curval.rint; break;
	case 75: pgtyp = curval.rint; break;
	case 0 : end=1; break;
	default :
		oparse1();
		break;
	}
	 }
	 oend();
#ifdef DEBUG
	 if (ohaseks) printf("Polyline has eks \n");
	 if (ohasheight) printf("Polyline has highth \n");

	 printf("Width %f %f %f %f \n",vabreite,vebreite,pabreite,pebreite);
#endif

	 strcpy(player,olayer); 	/* save layer */

	 if (pcntrf==1) {			/* read pnts */

	   end=0;
	   ppolyp=polyp=&polybuf[0];
	   ppolyh=&polyheight[0];

	   if (BIT(pflags,16)) {		/* mesh */
	   char descr[120];
#ifdef DEBUG
	  	printf("3D-Netz %d * %d, type=%d \n",pnetzm,pnetzn,pgtyp);
#endif

	  	netzp = CALLOCT(pnetzm*pnetzn,ads_point);
	  	// xxxx netzp.SetLength(pnetzm*pnetzn);
	  	pnetzp=netzp;
	  	sprintf(descr,"self=3DMESH,OPT=%d:%d:%d:%d",pnetzm,pnetzn,pflags,pgtyp);
	  	objgenstart(descr);
	   } else
	   if (BIT(pflags,64)) {		/* multiface mesh */
	   char descr[120];
#ifdef DEBUG
	  	printf("3D-Face Mesh Pts %d , Polys  %d \n",pnetzm,pnetzn);
#endif
	  	sprintf(descr,"self=3DPOLYNET,OPT=%d:%d:%d",pnetzm,pnetzn,pflags);

	  	pnetz_cnt=0;		/* true counter of points */
	  	if (pnetzm==0) pnetzm=1000; /* AME doesn't store group 71  !!!! */
	  	netzp = CALLOCT(pnetzm,ads_point);
	  	//netzp.SetLength(pnetzm);

	  	pnetzp=netzp; 	/* pointer into netzp */

	  objgenstart(descr);

	  if (BIT(dxfistyle,ISHELLS)) {
		shellgenstart(pnetzn);  /* store as shell */
	  }
	   } else {
	 ogenstart(); }

	   ocolor=-1;	/* delete color,  objgenstart has done	*/

	   while (!end) {
	getgroup();

	switch (curgroup) {
	case 0:
		oend();
		if (BIT(vflags,64)) {	/* mesh control Point */
		  if (pnetzp!=NULL) {
		  	//if (pnetz_cnt>= netzp.Length()) xxxxxxxxxxxxxxxxxxxxxx
		  	
		  	(*pnetzp)[0]=vx; (*pnetzp)[1]=vy; (*pnetzp)[2]=vz;
		  	pnetzp++;
		  }
		  pnetz_cnt++;
		} else
		if (BIT(vflags,128) && ! BIT(vflags,64)) { /* face */

		   if (BIT(dxfistyle,ISHELLS)) { /* store as shell */

			genface_s(netzp,&vi[0],vimax,vflags);

			if ((ocolor>0) && takeColor && (ocolor != netocolor)) { /* color */
			int face_color[2];
			face_color[0]=s_face_n-1; /* face number */
			face_color[1]=ocolor; /* face color  */
			/* append the face color to s_facecolor */
			s_facecolor.Append(2, face_color);
			}
		  } else
			genface(netzp,&vi[0],vimax,vflags);
		  vimax=0;
		}
		else
	/*		if (BIT(vflags,32))  */
		{	/* 3D-Polyline control Point  */
		  if (ppolyp!=NULL){
		  /* to do: pointlist overflow */
		  (*ppolyp)[0]=vx; (*ppolyp)[1]=vy; (*ppolyp)[2]=vz;
		  ppolyp++;
		  *ppolyh=ozheight; /* store height */
		  ppolyh++;
		  phasheight+=(ohasheight);
		  ppnts++;
		}}

#ifdef DEBUG
		printf("V %lf, %lf, %lf %d \n",vx,vy,vz,vflags);
#endif
		end=((curgroup==0) && isstr("SEQEND"));
		vx=vy=vz=0.0; vabreite=pabreite; vebreite=pebreite;
		vwidth=pwidth;
		vflags=0; vi[0]=vi[1]=vi[2]=vi[3]=0;
		{ int savenetcolor;
		  savenetcolor =netocolor;
		  ostart();  /* clear variables */
		  netocolor = savenetcolor;
		}

		break;
	case 10: vx = curval.rreal; break;
	case 20: vy = curval.rreal; break;
	case 30: vz = curval.rreal; break;

	case 39: vwidth = curval.rreal; break;
	case 40: vabreite = curval.rreal; break;
	case 41: vebreite = curval.rreal; break;
	case 70: vflags = curval.rint; break;
	case 71:
	case 72:
	case 73:
	case 74: vi[curgroup-71] = curval.rint;
		 vimax=curgroup-70;
		 break;

/*	case 8: strcpy(vlayer,curval.rstring); break; */
	default :
		 oparse1(); /* color .. */
		 break;
	}

	   }

	   strcpy(olayer,player); /* restore layer */

	   if (BIT(pflags,16)) { /* ************ store mesh */

	  		pnetzp=netzp;
	  		if (BIT(dxfistyle,ISHELLS))  /* store as _GLVIEW mesh  */
		 		genmesh__GLVIEW(netzp,pnetzm,pnetzn,pflags);
	  		else genmesh(netzp,pnetzm,pnetzn,pflags);

	  		FREE(netzp);
	  		objgenend();
	   } else
	   if (BIT(pflags,64)) { /* ***************** multi face mesh */

	   		if (BIT(dxfistyle,ISHELLS)) { /* store as shell */
				genshell(netzp,pnetz_cnt);
				shellgenend(); /* shell data */
	   		}
	   
	   		FREE(netzp);
	   		objgenend();

	   } else {
	   if (ppnts>0) {
		if (pabreite != 0.0) {
#ifdef _GLVIEW
		   genpoly_width_height(polyp,ppnts,pflags,pabreite,polyheight,&ozdir);
#endif
		} else {
		  if (phasheight)
		 	genpolyheight(polyp,ppnts,pflags,polyheight); /* 3d polygon */
		  else genpoly(polyp,ppnts,pflags);  /* simple polygon */
		}
	   }
	   ogenend();
	 }
	 }
   return(0);
}

/***************************************************************************/
void dxf_line(void)
{
  ostart();
  curgroup=-1;
  while (curgroup!=0) {
	 getgroup();
	 oparse1();
  }
  oend();
#ifdef _GLVIEW
  if (ohasheight) {
	 objgenstart("self=line");
	 genquadpw(&opts[0],&opts[1],ozheight*scalez,ozheight*scalez);
	 objgenend();
  } else {
	ogenstart();

	GetCurrentShell()->AppendEdge( Point( opts[0][X]*scalex,opts[0][Y]*scaley,opts[0][Z]*scalez),
			Point(opts[1][X]*scalex,opts[1][Y]*scaley,opts[1][Z]*scalez));
	ogenend();
  }
#endif
}

/***************************************************************************/
void dxf_point(void)
{
  ostart();
  curgroup=-1;
  while (curgroup!=0) {
	 getgroup();
	 oparse1();
  }
  oend();
#ifdef _GLVIEW
  ogenstart();
	// append a single Vertex coordinate
	GetCurrentShell()->AppendV(Point( opts[0][X]*scalex,opts[0][Y]*scaley,opts[0][Z]*scalez));
  ogenend();
#endif
}

/***************************************************************************/
void dxf_circle(void)
{ ads_real r=1.0;
  ostart();
  curgroup=-1;
  while (curgroup!=0) {
	 getgroup();
	 if (curgroup==40) r=curval.rreal;
	 else oparse1();
  }
  oend();
#ifdef _GLVIEW
	if (ohasheight) {
	Point center;
	objgenstart("self=cylinder");
	if (!BIT(dxfistyle,ISHELLS)) {
	  insert_circle_width(0 /*dxfistyle*/,opts[0][X]*scalex,opts[0][Y]*scaley,opts[0][Z]*scalez,r*scalex,ozheight*scalez,0);
	}
	else { /* store as mesh with 2 polygons */
	  apt2vec(center,opts[0]);
	  insert_cylinder_3d(dxfistyle & (IDESCRIPTION),&center,r,ozheight*scalez,0);
	}
	append_eks_matrix();   /* apply modelling coordinate system */
	objgenend();
	} else {
	Point p[3];
	ogenstart();
	apt2vec(p[0],opts[0]); p[0].x -= r*scalex;
	apt2vec(p[1],opts[0]); p[1].y += r*scaley;
	apt2vec(p[2],opts[0]); p[2].x += r*scalex;
	oeksstart();
#ifdef _TODO
	HC_Insert_Circle(&p[0],&p[1],&p[2]);
#endif

#ifdef DEBUG
	printf(" Circle \n");
#endif
	/*	 insert_circle(0 ,opts[0][X],opts[0][Y],opts[0][Z],r,0); */
	oeksend();
	ogenend();
	}
#endif
}

/***************************************************************************/
void dxf_arc(void)
{ ads_real r=1.0,aw=0.0,mw=0.0,ew=0.0;
  Point center;
  ostart();
  curgroup=-1;
  while (curgroup!=0) {
	 getgroup();
	 if (curgroup==40) r=curval.rreal;
	 else if (curgroup==50) aw=curval.rreal;
	 else if (curgroup==51) ew=curval.rreal;
	 else oparse1();
  }
  oend();
#ifdef _GLVIEW
	if (ohasheight) {

	objgenstart("self=cylinder");
	if (!BIT(dxfistyle,ISHELLS)) {
	  /* to do : arc width */
	  insert_circle_width(0 /*dxfistyle*/,opts[0][X]*scalex,opts[0][Y]*scaley,
				opts[0][Z]*scalez,r*scalex,ozheight*scalez,0);
	}
	else { /* store as mesh with 2 polygons */
	  apt2vec(center,opts[0]);
	  insert_cylinder_3d(dxfistyle & (IDESCRIPTION),&center,r*scalex,ozheight*scalez,0);
#ifdef _GLVIEW
//	  HC_Set_Visibility("markers=off");
#endif
	}
	append_eks_matrix();   /* apply modelling coordinate system */
	objgenend();
	} else {
	Point p[3];
	aw=DEG2RAD(aw);
	ew=DEG2RAD(ew);
	mw=(aw+ew)*0.5;
	apt2vec(center,opts[0]);
	ogenstart();
	p[0].x = opts[0][0] + cos(aw)*r*scalex; p[0].y = opts[0][1] + sin(aw)*r*scaley;
	p[0].z = opts[0][2]*scalez;
	p[1].x = opts[0][0] + cos(mw)*r*scalex; p[1].y = opts[0][1] + sin(mw)*r*scaley;
	p[1].z = opts[0][2]*scalez;
	p[2].x = opts[0][0] + cos(ew)*r*scalex; p[2].y = opts[0][1] + sin(ew)*r*scaley;
	p[2].z = opts[0][2]*scalez;
	oeksstart();
#ifdef _TODO
	HC_Insert_Circular_Arc(&p[0],&p[1],&p[2]);
#endif
#ifdef DEBUG
	printf(" Circular arc  Angles a %f m %f e %f \n",aw,mw,ew);
#endif
	oeksend();
	ogenend();
	}
#endif
}

/***************************************************************************/
/*
  parse and define a text
*/

void dxf_text(void)
{ char value[255],*p;
  ads_real h=0.0, xfac=1.0, rotw = 0.0, slantw =0.0;
  int gflag=0,alignx=0,aligny=0;
  Point ref;
  const char *halignx = "<*>***";
  const char *haligny = "vv*^";
  char align[3];

  value[0]='\0';

  ostart();
  curgroup=-1;
  while (curgroup!=0) {
	 getgroup();
	 switch(curgroup) {
	 case 1 : strcpy(value,curval.rstring); break;
	 case 40 : h=curval.rreal; break;
	 case 41 : xfac=curval.rreal; break;
	 case 50 : rotw=curval.rreal; break;
	 case 51 : slantw=curval.rreal; break;
	 case 71 : gflag=curval.rint; break;
	 case 72 : alignx=curval.rint; break;
	 case 73 : aligny=curval.rint; break;
	 default : oparse1(); break;
	 }
  }
  oend();
/*
  printf("TEXT  %s  \n Refpt (%lf, %lf, %lf) Refpt2 (%lf, %lf, %lf) \n h %f rot %f xfac %f slant %f %d %d %d \n",value,
  opts[0][X],opts[0][Y],opts[0][Z],opts[1][X],opts[1][Y],opts[1][Z],
			h, rotw, xfac, slantw, gflag, alignx, aligny);
*/
  apt2vec(ref,opts[0]);
#ifdef _TODO
  while ((p=strstr(value,"%%253")) !=NULL)  {
	strndel(p,0,5); strins(p,0,"\262"); } /* square sign */

  while ((p=strstr(value,"%%")) !=NULL)  {
	  *p= atoi(p+2); strndel(p+1,0,4);
  }

  if ((alignx >0) || (aligny >0)) apt2vec(ref,opts[1]); /* reference Point */

  ogenstart();
#ifdef _GLVIEW
	 world->Open("");
	  /* xfac *= 1.05; xxxxx */
	  if ((!iszero(rotw)) || (!iszero(1.0-xfac)) ) {
	HC_Translate_Object(-ref.x,-ref.y,-ref.z);
	if (!iszero(1.0-xfac)) HC_Scale_Object(xfac,1.0,1.0);
	if (!iszero(rotw)) HC_Rotate_Object(0.0,0.0,rotw);
	HC_Translate_Object(+ref.x,+ref.y,+ref.z);
	  }
	  if (1) {	  /* !iszero(1.0-h)) { */
	 char tmp[60];
	 sprintf(tmp,"size=%f wru",h*0.70); /* 0.74 xxxx */
	 HC_Set_Text_Font(tmp);
	  }
	  if ((alignx >0) || (aligny >0)) {
	align[0]=halignx[alignx];
	align[1]=haligny[aligny];
	align[2]=0;
	HC_Set_Text_Alignment(align);
	  }
	  HC_Insert_Text(ref.x,ref.y,ref.z,value);
	  append_eks_matrix();	 /* apply modelling coordinate system */
	 world->Close();
#endif
  ogenend();
#endif
}


/***************************************************************************/
void dxf_solid(void)
{
  ostart();
  curgroup=-1;
  while (curgroup!=0) {
	 getgroup();
	 oparse1();
  }
  oend();
  if (ohasheight) {
	 objgenstart("self=solid");
	 /* to do : build shell */
	 genquadpw(&opts[0],&opts[2],ozheight*scalez,ozheight*scalez);
	 genquadpw(&opts[2],&opts[3],ozheight*scalez,ozheight*scalez);
	 genquadpw(&opts[3],&opts[1],ozheight*scalez,ozheight*scalez);
	 genquadpw(&opts[1],&opts[0],ozheight*scalez,ozheight*scalez);
	 append_eks_matrix();	/* apply modelling coordinate system */
	 objgenend();
  } else {
	 ogenstart();
	 oeksstart();
	 genquadp(&opts[0],&opts[2],&opts[3],&opts[1]);
	 oeksend();
	 ogenend();
  }
}

/***************************************************************************/
void dxf_3dface(void)
{
  ostart();
  curgroup=-1;
  while (curgroup!=0) {
	 getgroup();
	 oparse1();
  }
  oend();
#ifdef _GLVIEW
  ogenstart();
	// to do : if (opts[2] == opts[3]) 
	addshellface(opts,4);
  ogenend();

#else
  ogenstart();
  genquadp(&opts[0],&opts[1],&opts[2],&opts[3]);
  ogenend();
#endif
}




/***************************************************************************/
static int sect=0;		/* section */

static ads_real x[10],y[10],z[10];

/*
  simple dxf groupcode pre-processing
*/

void dxf_parse1(void) {
	groupcode g=curgroup;
	   switch (g) {
	   case 0:
		  if (isstr("EOF")) { curgroupt=RTEOF; }
		  else if (isstr("SECTION")) { sect++; }
		  else if (isstr("ENDSEC")) { sect--; }
		  break;

	   case 2:
		  strcpy(name,curval.rstring);
		  break;


	   default:
		  if (g>=10 && g<=18) { x[g-10]=curval.rreal;}
		  else if (g>=20 && g<=28) { y[g-20]=curval.rreal;}
		  else if (g>=30 && g<=38) { z[g-30]=curval.rreal;}
		  break;
	   }
}
#define zero(XXXX) ((XXXX)==0.0)

/***************************************************************************/
/*
  parse and define a block
*/
void dxf_block(void)
{ char bname[255];
  int btyp=0;
  bname[0]='\0';
  ostart();
  curgroup=-1;
  while (curgroup!=0) {
	 getgroup();
	 switch(curgroup) {
	 case 2 : strcpy(bname,curval.rstring); break;
	 case 70 : btyp=curval.rint; break;
	 default : oparse1(); break;
	 }
  }
  oend();
#ifdef DEBUG
  printf("BLOCK  %s Refpt (%lf, %lf, %lf) \n",bname,opts[0][X],opts[0][Y],opts[0][Z]);
#endif
#ifdef _GLVIEW
  world->Open(fixname(bname));

  /* Translate to reference Point */
  if (!(zero(opts[0][X]) && zero(opts[0][Y]) && zero (opts[0][Z]))) {
	 Matrix m = TranslationMatrix(-opts[0][X]*scalex,-opts[0][Y]*scaley,-opts[0][Z]*scalez);
	 world->Current()->Set(new GMatrix(m));
  }
#endif
}

/***************************************************************************/
void dxf_endblock(void)
{
#ifdef DEBUG
  printf("BLOCK-END \n");
#endif

#ifdef _GLVIEW
  world->Close();
#endif
  getgroup();
}

/***************************************************************************/
/*
  read insert request
*/

void dxf_insert(void)
{ char bname[1024];
  int btyp=0;
  ads_point fac;
  ads_real rotate=0.0;
  int rows=1, cols=1, r,c;
  ads_real drow=0.0,dcol=0.0;
  int facf=0;
  bname[0]='\0';

  fac[X]=fac[Y]=fac[Z]=1.0;

  ostart();
  curgroup=-1;
  while (curgroup!=0) {
	 getgroup();
	 switch(curgroup) {
	 case 2 : strcpy(bname,curval.rstring); break;
	 case 41: case 42: case 43: fac[curgroup-41]=curval.rreal; facf=1;break;
	 case 50 : rotate=curval.rreal; break;
	 case 70 : rows=curval.rint; break;
	 case 71 : cols=curval.rint; break;
	 case 44 : drow=curval.rreal; break;
	 case 45 : dcol=curval.rreal; break;
	 case 66 : btyp=curval.rint; break;
	 default : oparse1(); break;
	 }
  }
  oend();

#ifdef DEBUG
  printf("INSERT BLOCK  %s Scale (%lf, %lf, %lf) Rot %lf Refpt (%lf, %lf, %lf)  RPT %d %d\n",
	bname,
	fac[X],fac[Y],fac[Z],
	rotate,
	opts[0][X],opts[0][Y],opts[0][Z],rows,cols);
#endif


#ifdef _GLVIEW
  ogenstart();
  world->Open(""); /* create a segment for transformation */


   /* find _GLVIEW segment, which contains the BLOCK */
   world->Open(blocksroot);
	 world->Open(fixname(bname));
		//HC_Show_Pathname_Expansion(".",bname);
		
		GGroup *theBlock = world->Current();

	 world->Close();
   world->Close();

  if ((rows >1 ) || (cols >1)) { /* grid */
#ifdef _TODO
	  HC_Set_User_Options("self=grid insert");
#endif

	  for (r=1; r<=rows; r++)
	  for (c=1; c<=cols; c++)  {
	world->Open("");
		//HC_Include_Segment(bname);
	
		world->Current()->Insert(new GInstance(theBlock));
		Matrix m;
		m.Identity();

		if (facf) m.Scale(fac[X],fac[Y],fac[Z]);
		if (!zero(rotate)) m.Rotate(0.0,0.0,DEG2RAD(rotate));
		// to do append_eks_matrix();  /* apply modelling coordinate system */
		m.Translate((float)(c-1)* dcol, (float)(r-1)* drow ,0.0);
		if (!(zero(opts[0][X]) && zero(opts[0][Y]) && zero (opts[0][Z])))
		   m.Translate(opts[0][X]*scalex,opts[0][Y]*scaley,opts[0][Z]*scalez);
		
		world->Current()->Set(new GMatrix(m));

	world->Close();
	  }

  } else {	/* simple include */

	//HC_Include_Segment(bname);
	world->Current()->Insert(new GInstance(theBlock));

	Matrix m;
	m.Identity();


	if (facf) m.Scale(fac[X],fac[Y],fac[Z]);
	if (!zero(rotate)) m.Rotate(0.0,0.0,DEG2RAD(rotate));
	if (! (zero(opts[0][X]) && zero(opts[0][Y]) && zero (opts[0][Z])))
	   m.Translate(opts[0][X]*scalex,opts[0][Y]*scaley,opts[0][Z]*scalez);
	//xx to do append_eks_matrix();  /* apply modelling coordinate system */
	world->Current()->Set(new GMatrix(m));
  }

  world->Close();

  ogenend();

#endif

}

/***************************************************************************/
/*
  process DXF entitiy section
*/

void dxf_entities(void)
{
	int eof=0;
#ifdef DEBUG
	   printf("********* Entities *************************\n");
#endif


	getgroup();
	while ((curgroupt!=RTEOF) && ( eof == 0)) {

#ifdef DEBUG
	   printcurgroup();
#endif
	   if (curgroup==0) {
		 if (isstr("POLYLINE"))
		 dxf_get_polyline();
		else if (isstr("LINE"))
		 dxf_line();
		else if (isstr("SOLID"))
		  dxf_solid();
		else if (isstr("3DFACE"))
		  dxf_3dface();
		else if (isstr("POINT"))
		  dxf_point();
		else if (isstr("CIRCLE"))
		  dxf_circle();
		else if (isstr("ARC"))
		  dxf_arc();
		else if (isstr("TEXT"))
		  dxf_text();
		else if (isstr("BLOCK"))
		  dxf_block();
		else if (isstr("ENDBLK"))
		  dxf_endblock();
		else if (isstr("INSERT"))
		  dxf_insert();
		else if (isstr("ENDSEC"))
		 eof=1;
		else getgroup();
#ifdef DEBUG
		 /* show_open_segments(); */
#endif
	   }
	   else getgroup();

	}
#ifdef DEBUG
	   printf("********* Entities EOF ***********************\n");
#endif
}

/***************************************************************************/
/*
  process DXF block section
*/


void dxf_blocks(void) {

#ifdef DEBUG
	   printf("********* Blocks *************************\n");
#endif


#ifdef _GLVIEW
	world->Open(blocksroot);
/*	  world->Open("BLOCKS"); */
/*	  HC_Show_Pathname_Expansion(".",blocksroot); */
//	HC_Set_Visibility("off");
//	HC_Set_Selectability("off");
//	if (BIT(dxfistyle,IDESCRIPTION)) {
//		HC_Set_User_Options("self=BLOCK");
//	}
#endif
	dxf_entities();
#ifdef _GLVIEW
	world->Close();
#endif


}

/***************************************************************************/
/*
  process DXF layer section
  define layers as _GLVIEW segments and set default colors
*/
void dxf_layer(void)
{ int eof=0;
  char lname[255];
  int ltyp=0;
  int lcnt=0;

#ifdef DEBUG
	   printf("********* Layers *************************\n");
#endif



  lname[0]='\0';

  curgroup=-1;
  while (!eof) {
	 getgroup();
	 switch(curgroup) {
	case 0:

#ifdef DEBUG
		printcurgroup();
#endif

		if isstr("EOF") eof = 1;
		else if isstr("ENDTAB") eof = 1;
		else if isstr("ENDSEC") {eof=1;}
		else if isstr("LAYER") {
		}

		if (lcnt>0) {
		  oend();

		  strcpy(olayer,lname); /* set layer */
		  olayerstart();
#ifdef DEBUG
  printf("LAYER %s ,color : %d flags : %d \n",lname,ocolor,ltyp);
#endif

#ifdef _TODO

		if (ocolor<0) HC_Set_Visibility("off");

		  if (takeLayer && BIT(dxfistyle,IDESCRIPTION)) {
			char descr[256];
			sprintf(descr,"self=layer,layer=%s",lname);
			HC_Set_User_Options(descr);
		  }
#endif

		  olayerend();
		 }
		  lcnt++;

		  if (!eof) {ostart();}

		break;

	 case 2 : strcpy(lname,curval.rstring); break;
	 case 70 : ltyp=curval.rint; break;
	 default : oparse1(); break;
	 }
  } /* while */
#ifdef DEBUG
	   printf("********* Layers END *********************\n");
#endif

}



void dxf_tables(void) {
	int eof=0;
	int tab=0;

#ifdef DEBUG
	   printf("********* TABLES *************************\n");
#endif

	while ((curgroupt!=RTEOF) && ( eof == 0)) {
	getgroup();
	dxf_parse1();
	switch (curgroup) {
	case 0:

#ifdef DEBUG
		printcurgroup();
#endif
		if isstr("EOF") eof = 1;
		else if isstr("ENDSEC") {eof=1;}
		else if isstr("TABLE") tab++;
		else if isstr("ENDTAB") tab--;
		break;
	case 2:
#ifdef DEBUG
		printcurgroup();
#endif
		if ((tab>0) && isstr("LAYER")) dxf_layer();
		break;

	default:

/*		 getgroup(); */
		   break;
	}
	}
#ifdef DEBUG
	   printf("********* TABLES END **********************\n");
#endif

}

/***************************************************************************/
void dxf_header(void) {
	int eof=0;

#ifdef DEBUG
	   printf("********* HEADER *************************\n");
#endif

	while ((curgroupt!=RTEOF) && ( eof == 0)) {
	getgroup();
	dxf_parse1();
	switch (curgroup) {
	case 0:

#ifdef DEBUG
		printcurgroup();
#endif
		if isstr("EOF") eof = 1;
		else if isstr("ENDSEC") {eof=1;}
		break;
	case 2:
#ifdef DEBUG
		printcurgroup();
#endif
		break;

	default:
/*		 getgroup(); */
		   break;
	}
	}
#ifdef DEBUG
	   printf("********* HEADER END *********************\n");
#endif
}



/***************************************************************************/
void dxf_parse(groupcode untilcode,char * untiltype) {
	int eof=0;

	while ((curgroupt!=RTEOF) && ( eof == 0)) {
	getgroup();
	dxf_parse1();
	switch (curgroup) {
	case 0:
#ifdef DEBUG
		printcurgroup();
#endif
		if isstr("EOF") eof = 1;
		break;
	case 2:
#ifdef DEBUG
		printcurgroup();
#endif
		if isstr("BLOCKS") dxf_blocks();
		else if isstr("ENTITIES") dxf_entities();
		else if isstr("TABLES") dxf_tables();
		else if isstr("HEADER") dxf_header();
		if ((curgroup==0) && isstr("EOF")) eof = 1;
		break;
	case 999:		/* comment */
#ifdef DEBUG
		printcurgroup();
#endif
		break;
	default:
		   fprintf(stderr,"Bad DXF File: Section expected !");
		   return;
/*		 getgroup();
		   break; */
	}
	}
}

#ifdef _GLVIEW
/***************************************************************************/
/*
  Insert the geometric data of AutoCAD DXF file "dxfile"
  into the currently open _GLVIEW segment

  Store DXF Block definitions in a _GLVIEW tree starting at "blockseg"

  Options :
	style:	BITVALUE
	Possible values, may be ored together :

	IPOLYSEGMENTS : store each entity in a separate unnamed _GLVIEW Segment
	IDESCRIPTION  : usefull if IPOLYSEGMENTS
			store short description of entity in User_Options
	ICOLOR		  : use DXF color information


  Result:
	<0 : error
	-1 : file error
	0  : success
*/
int ReadDxfFile(const char *dxfile,GWorld *theWorld)
{ int ret=-1;
//	   dxfistyle = style;
	   world = theWorld;
	   if (!world->objects) { 
	   		world->objects = new GGroup();
			world->objects->SetName("\\BLOCKS");
			}
	   if (!world->styles) {
	   		 world->styles = new GGroup();
			 world->styles->SetName("\\STYLES");
	   }

	   blocksroot = world->objects;
	   
	   curShell = NULL;
	   curShellLayer[0]=0;


	   InFile = fopen(dxfile,"r");
	   if (InFile != NULL) {
		  segstr tmp;
		  curgroup=-1;
		  curgroupt=RTNONE;

  		  // set buffering to larger bufer 64 k
  		  if (setvbuf(InFile,NULL,_IOFBF,1024*64) !=0) {
	         fprintf(stderr,"gdxfin::Can´t set stream buffer size");
  		  }


		  /* default attributes */
//		  HC_Set_Text_Alignment("<v");
//		  HC_Set_Text_Font("transforms=on,size=1.0 wru");

		  world->Open("/model");
		  	dxf_parse(0,"EOF");
		  world->Close();

		  fclose(InFile);
#ifdef _TODO
		  HC_Begin_Segment_Search("ame_*");
			/* delete unneeded ame stuff */
			if (HC_Find_Segment(tmp))
		   	HC_Delete_Segment("ame_*");
		  HC_End_Segment_Search();
#endif

		  ret=0;
	   }
#ifdef DEBUG
	printf("DXFIN: %s ready ret = %d \n",dxfile,ret);
#endif
  return(ret);
}

#else
/***************************************************************************/
/*
  dxfin parse as standalone programm
*/
int dxfin(int argc,char * argv[]) {
	int ret=1;
	if (argc>=2) {
	   InFile = fopen(argv[1],"rt");
	   if (InFile != NULL) {
		  curgroup=-1;
		  curgroupt=RTNONE;
		  dxf_parse(0,"EOF");
		  fclose(InFile);
		  ret=0;
	   }
	}
#ifndef _GLVIEW
	if (ret==1) {
	  printf("Usage : %s DXFILENAME.DXF \n",argv[0]);
	  printf("Extracts 3D data from AUTOCAD R. 11 DXF-Files \n");
	}
#endif
	return(ret);
}

#endif
/***************************************************************************/
