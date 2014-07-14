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
#ifndef _GLUTILS_H
#define _GLUTILS_H
/******************************************************************************
@doc

@module GLUtils.h - GL utility routines |

Copyright (c) 1995 - 1998  by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:
texturing helper routines


Changes:

Todo :

******************************************************************************/

// texture quality level
enum {
    TEX_LOW = 0,
    TEX_MID,
    TEX_HIGH
};

int get_supported_image_formats(void);
int set_supported_image_formats(int fmts);



//@func load a image texture
//int InitImageTexture(int format, const char *bmpfile, int wflag = 0, int TextureQuality = TEX_LOW, int maxSizeX=0,int maxSizeY=0);

int InitImageTexture(int format, const char *bmpfile, int wflag, int buildMipmaps, int maxSizeX,int maxSizeY,
					 BOOL &hasAlpha,BOOL &hasColorKey,BOOL &isGrayscale,
				 	 unsigned char			&components,			// number of components 
				 	 short					&sizeX,		// current size 
				 	 short					&sizeY,		// current size 
					 short					&textureFormat		// index into texture format // D3D
);


// set gl texture including 2 power scaling and mip mapping
// format eg. GL_RGB ==> components = 3
int glSetTexture(GLenum format,int sizeX,int sizeY, int components, 
				 unsigned char *data, int buildMipmaps, int maxSizeX,int maxSizeY,
			 	 short					&cur_sizeX,		// current size 
			 	 short					&cur_sizeY		// current size 
				 );



// from TK
/*
** RGB Image Structure
*/

typedef struct _TK_RGBImageRec {
    GLenum format;
    GLint sizeX, sizeY;
    GLint components;
    unsigned char *data;
} TK_RGBImageRec;

#define AUX_RGBImageRec TK_RGBImageRec

AUX_RGBImageRec *
LoadImage(int format, const char *bmpfile, int wflag=0,int scale_2power=0);


TK_RGBImageRec *tkDIBImageLoadAW(const char *fileName, BOOL bUnicode);
TK_RGBImageRec *tkRGBImageLoadAW(const char *fileName, BOOL bUnicode);

int  ReadImage(int format, const char *theFile,AUX_RGBImageRec **imAUX,BOOL &hasColorKey,BOOL &isGrayscale);





#define MESSAGEBOX(a, b, c, d)   MessageBoxA((a), (b), (c), (d))

#ifdef _IMAGE_H

/* convert a mapped 8 image to rgb */
int image_convert_to_rgb(image *im);

/* convert a mapped 8/gray/graya image to rgba */
int image_convert_to_rgba(image *im);

/* convert a mapped 8/gray/graya/rgba image to rgb */
int image_convert_to_rgb(image *im);



/*
   scale the image to a power of two and respect maximum texture dimension
   maxSizeX maxSizeY
   Result 1, ok,
   else error code from an image operation

*/
int image_scale_2power(image *im, int minSizeX,int minSizeY, int maxSizeX,int maxSizeY,gbool square=FALSE,gbool filteredScale=TRUE);


gbool get_image_2power(int &width,int &height, int maxSizeX,int maxSizeY,gbool square=FALSE,int roundUpLimt=128);
gbool get_image_2power(int &width,int &height, int minSizeX,int minSizeY,  int maxSizeX,int maxSizeY,gbool square=FALSE,int roundUpLimt=128);


/*
   scale the image 

*/

int image_scale(image *im, int xSize2,int ySize2,gbool filteredScale=TRUE);


int  ReadImage(int format,const char *thefile,const char *options,image *im);


/*
	 Read an image File into im
	 scale to power of 2 if needed
	 and convert a mapped8 image to rgb if needed.
*/

int  ReadTextureImage(int format,const char *theFile,image *im, 
					  int minSizeX,int minSizeY,int maxSizeX,int maxSizeY,gbool square,gbool filteredScale,
					  int supported_fmts);


enum ReadTextureOptions {
	OPT_SCALE_2POWER=1,
	OPT_SCALE_2POWER_SQUARE=2,
	OPT_UPSCALE=4,
	OPT_SCALE_FILTERD=8,
};

/*
	 Read an image File into im
	 process according to options 
*/

int  ReadTextureImageOpt(int format,const char *theFile,image *im, 
					  int minSizeX,int minSizeY,int maxSizeX,int maxSizeY,
					  int supported_fmts,int options);



#endif



#endif
