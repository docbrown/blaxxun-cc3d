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

@module GvHandle.cpp - GLView VRML edit handle nodes implementation|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvHandle>

Notes:

Changes:

$Revision: 1.9 $
$Log: gvhandle.cpp,v $
Revision 1.9  1999/07/06 16:45:46  tom
*** empty log message ***

Revision 1.8  1999/05/10 13:54:54  tom
*** empty log message ***



Todo :

******************************************************************************/

#include "stdafx.h"

#include <gvhandle.h>

#include "gcamera.h"


GV_NODE_SOURCE_IMP(GvHandle,Gv2Group);

GvHandle::GvHandle() 
{
    GV_NODE_CONSTRUCTOR(GvHandle);
    isBuiltIn = FALSE;
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);
    GV_NODE_ADD_FIELD(children);

    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);
}

GvHandle::~GvHandle()
{
}





GV_NODE_SOURCE_IMP(GvBoxHandle,GvHandle);

GvBoxHandle::GvBoxHandle() 
{
    GV_NODE_CONSTRUCTOR(GvBoxHandle);
    isBuiltIn = FALSE;
    GV_NODE_ADD_FIELD(bboxSize);
    GV_NODE_ADD_FIELD(bboxCenter);
    GV_NODE_ADD_FIELD(children);

    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);
}

GvBoxHandle::~GvBoxHandle()
{
}

GvBool GvBoxHandle::createProxy(const BBox &box)
{
	float c[2][3];
	int k,i,j,pi;
	if (box.IsEmpty()) return FALSE;
	
	Point p[8];

	for (i=0; i<3;i++) { c[0][i]= box.mi[i]; c[0][i]= box.ma[i]; }

	pi=0;
	for (k=0;k<=1;k++) {
		 for (i=0;i<=1;i++) {
				for (j=0;j<=1;j++) {
				  {
						p[pi].Set(c[j][0],c[i][1],c[k][2]);
						pi++;
				  }
				  
				}
		 } 
   }

#define Edge(i1,i2) ;
	//addLine(p[i1],p[i2])

   Edge(0,1);			
   Edge(1,3);			
   Edge(2,3);			
   Edge(0,2);			

   Edge(4,5);			
   Edge(5,7);			
   Edge(6,7);			
   Edge(4,6);			

   Edge(0,4);			
   Edge(1,5);			
   Edge(2,6);			
   Edge(3,7);			



   return TRUE;
}

#if 1
#include "grender.h"

#include "image.h"
#include "glutils.h"
#include "gshell.h"
#include <gvnodes.h>
#include "goptimizetraversal.h"

/*

  create a node for image

*/

GvNode *TextureToNode(int fileFormat,const char *fileName,int op)
{

	GvNode *node = NULL;
	GvString url;
	int ret=0;

	url = fileName;


		int formats =  (1<<IM_RGBA) | (1<<IM_RGB) | (1<<IM_GRAYA) | (1<<IM_GRAY);

		if (op == OP_ELEVATION_GRID_COLORED || (op == OP_PIXEL_TEXTURE)) {
		}
		else 
			formats |=  (1<<IM_MAPPED8);
	

		set_supported_image_formats(formats);

		// image library image structure 
		image im;
		image_zero(&im);

		im.supported_fmts = formats;

		ret= ReadTextureImageOpt(fileFormat,fileName, &im,1,1,1024,1024,formats,
			/* OPT_SCALE_2POWER */ OPT_SCALE_FILTERD);



		if (im.image) {
#if 0			
			isGrayscale = image_is_gray(&im);
				if (im.fmt != IM_MAPPED8 ) 
					ret = image_convert_image_mapped8(&im);
				if (im.fmt != IM_RGB) {
						ret=image_convert_to_rgb(&im);
				}
#endif

			switch (im.fmt) {
			case IM_BW:
			case IM_GRAY:
			case IM_GRAYA:
			case IM_MAPPED4:
			case IM_MAPPED8:
					break;
			case IM_RGB:
			case IM_RGBA:
					break;
			default :
				break;

			}
	}

	switch (op) {
	case OP_IMAGE_TEXTURE: 
	{	GvImageTexture *n = new GvImageTexture();
		n->url.allocValues(1);
		n->url.set1(0,url);
		node = n;
		break;
	}
	case OP_MOVIE_TEXTURE: 
	{	GvMovieTexture *n = new GvMovieTexture();
		n->url.allocValues(1);
		n->url.set1(0,url);
		n->loop.set(TRUE);
		node = n;
		break;
	}
	case OP_BACKGROUND: 
	{	GvBackground *n = new GvBackground();
		
		n->leftUrl.allocValues(1);
		n->rightUrl.allocValues(1);
		n->frontUrl.allocValues(1);
		n->backUrl.allocValues(1);

		n->leftUrl.set1(0,url);
		n->rightUrl.set1(0,url);
		n->frontUrl.set1(0,url);
		n->backUrl.set1(0,url);

		node = n;
		break;
	}

	case OP_MOVIE_RECT:
	case OP_TEXTURED_RECT: 
	{
		#define T1  (511.0f/512.0f)
		#define TXEPS (1.0f/512.0f)

		static point_ params[] = { {0.0,0.0, 0.0 }, { T1,0.0, 0.0 }, { T1, T1, 0.0 }, { 0.0, T1, 0.0 } };
		static point_ center[] = { {-1.0,-1.0, 0.0 }, { 1.0,-1.0, 0.0 }, { 1.0, 1.0, 0.0 }, { -1.0,1.0, 0.0 } };

		PointArray p(4,(const Point *) center);
		float ratio = 1.0;

		if (im.height >0) {
			ratio = (float) im.width / (float) im.height;
				p[0].x = p[3].x = -ratio;
				p[1].x = p[2].x = ratio;
		}


		GvShape *shape = new GvShape();
		GvAppearance *a = new GvAppearance();
		//GvImageTexture *n = new GvImageTexture();
		if (op == OP_TEXTURED_RECT) {
			GvImageTexture *t = new GvImageTexture();
			t->url.allocValues(1);
			t->url.set1(0,url);
			a->texture.set(t);
		} else 
		if (op == OP_MOVIE_RECT) {
			GvMovieTexture *t = new GvMovieTexture();
			t->url.allocValues(1);
			t->url.set1(0,url);
			t->loop.set(TRUE);
			a->texture.set(t);
		}

		shape->appearance.set(a);
		GShell *shell = new GPolygon(4, p);
		shell->SetVP(4,(const Point *) params);
		shell->SetFlat();

		
		node = CreateIndexedFaceSet(shell, FALSE, NULL,NULL);
		shape->geometry.set(node);
		delete shell;

		node = shape;
		break;
    }
	default :
		break;
	}	// op 
	
	image_free_all(&im);
	return node;
}

#endif

