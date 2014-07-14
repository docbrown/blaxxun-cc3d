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

@module GLUtils.cpp - GLView utility routines for images & textures |

Copyright (c) 1995 - 1999 by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:
	20.09.96 HG D3D routines (avoid gl specific functions)

 Todo :


******************************************************************************/

#include "stdafx.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <math.h>
#include <sys/types.h>
#include <time.h>

#ifdef _D3D
#include "grender.h"
#endif

#include "gconfig.h"


#include "gversion.h"


// for format specifiers
#include "www.h"



#include "image.h"
#include "idither.h"

#include "glutils.h"


#ifdef _IMAGE_H

#ifdef _JPEG
#include "jpegread.h"
//#include "jpegwrit.h"
#endif

#ifdef _PNG
#include "png_read.h"
#endif

#ifdef _TARGA
#include "targaio.h"
#endif

#ifdef _GIF
//#include "gifread.h"
#include "gifread.h"
#endif

#ifdef _DIB
#include "dibio.h"
#endif

#endif



// formats supported by opengl 
static int supported_fmts = (1<<IM_RGB24) | (1<<IM_RGBA) | (1<<IM_GRAY) | (1<<IM_GRAYA);


int get_supported_image_formats(void) { return (supported_fmts); }

int set_supported_image_formats(int fmts) {
  supported_fmts = fmts;
  return (supported_fmts);
}

/* convert a mapped 8/gray/graya image to rgba */
int image_convert_to_rgba(image *im)
{ image irgb;
  int ret;

  if (im->fmt == IM_RGBA) return(0);

  /* TRACE("Converting to RGBA \n"); */

  image_zero(&irgb);

  ret=image_alloc(&irgb,IM_RGBA,im->width,im->height);
  
  if (ret<0) return(ret);

  ret = IME_NOTIMP;

  if (im->fmt == IM_MAPPED8)  
	ret=image_mapped8_to_rgba(im,im->colorKeyIndex,0,255,&irgb);
  else if (im->fmt == IM_GRAYA)  
	ret=image_graya_to_rgba(im,&irgb);

  if (ret<0) {
     image_free_all(&irgb);
     return(ret);
  }

  image_free(im);
  
  im->image = irgb.image;
  image_set_fmt(im,irgb.fmt,irgb.width,irgb.height);
  irgb.image = NULL;
  image_free_all(&irgb);
  return(ret);
}


/* convert a mapped 8/gray/graya/rgba image to rgb */
int image_convert_to_rgb(image *im)
{ image irgb;
  int ret;
  if (im->fmt == IM_RGB24) return(0);
/*   printf("Converting to RGB \n"); */

  if (im->fmt == IM_GRAY) { 
	  if (im->cmap == NULL) {
		  image_alloc_cmap(im,256);
		  for (int i=0; i<256;i++) image_set_cmap(im,i,i,i,i);
	  }	
	  im->fmt = IM_MAPPED8;
  }	

  
  image_zero(&irgb);

  ret=image_alloc(&irgb,IM_RGB24,im->width,im->height);
  
  if (ret<0) return(ret);
  
  ret = IME_NOTIMP;

  if (im->fmt == IM_MAPPED8) 
	 ret=image_mapped8_to_rgb(im,&irgb);
  else if (im->fmt == IM_GRAYA) 
	 ret=image_graya_to_rgb(im,&irgb);
  else if (im->fmt == IM_RGBA) 
	 ret=image_rgba_to_rgb(im,&irgb);

  if (ret<0) {
     image_free_all(&irgb);
     return(ret);
  }

  image_free(im);

  im->image = irgb.image;
  image_set_fmt(im,irgb.fmt,irgb.width,irgb.height);
  irgb.image = NULL;
  image_free_all(&irgb);

  return(ret);
}

/*
   scale the image to a power of two and respect maximum texture dimension
   maxSizeX maxSizeY
   Result 1, ok,
   else error code from an image operation

*/

gbool get_image_2power(int &sizeX,int &sizeY, int maxSizeX,int maxSizeY,gbool square,int roundUpLimt)
{
	return get_image_2power(
						sizeX,sizeY,	// the input size / modified on output
					    1, 1,
						maxSizeX,maxSizeY, // min/ max allowd dimensions
						square,	// only square allowed
						roundUpLimt	// limit where to up-round to not loose image detail
						);

}


gbool get_image_2power(int &sizeX,int &sizeY,	// the input size / modified on output
					    int minSizeX,int minSizeY,int maxSizeX,int maxSizeY, // min/ max allowd dimensions
						gbool square,	// only square allowed
						int roundUpLimt	// limit where to up-round to not loose image detail
						)
{
    double xPow2, yPow2;
    int ixPow2, iyPow2;
    int xSize2, ySize2;
    GLint glMaxTexDimX=maxSizeX;
    GLint glMaxTexDimY=maxSizeY;


    if (sizeX <= glMaxTexDimX)
		xPow2 = log((double)sizeX) / log((double)2.0);
    else
		xPow2 = log((double)glMaxTexDimX) / log((double)2.0);

    if (sizeY <= glMaxTexDimY)
		yPow2 = log((double)sizeY) / log((double)2.0);
    else
		yPow2 = log((double)glMaxTexDimY) / log((double)2.0);

    ixPow2 = (int)(xPow2+0.5); // round 
    iyPow2 = (int)(yPow2+0.5); // round 

    xSize2 = 1 << ixPow2;
    ySize2 = 1 << iyPow2;


#if 1
    
	// round up 
	if (xSize2<roundUpLimt && (glMaxTexDimX >= roundUpLimt))
//    if (xPow2 != (double)ixPow2)

		if ( (float) xPow2  >  (float)ixPow2)
			ixPow2++;

	if (ySize2<roundUpLimt && (glMaxTexDimY >= roundUpLimt))
//    if (yPow2 != (double)iyPow2)

		if ( (float) yPow2  >  (float)iyPow2)
			iyPow2++;
#endif



	if ( (ixPow2 != iyPow2) && square) {
		// need square texture, take minimum 

		if (ixPow2 < iyPow2) iyPow2 = ixPow2;
		else ixPow2= iyPow2;
	}

				      
    xSize2 = 1 << ixPow2;
    ySize2 = 1 << iyPow2;

	if (xSize2 < minSizeX) xSize2 = minSizeX; 
	if (ySize2 < minSizeY) ySize2 = minSizeY;


	if ((xSize2 == sizeX) && (ySize2 == sizeY)) {
		// don´t need a scale 
		return FALSE;
	} else {
		sizeX = xSize2;
		sizeY = ySize2;
		return TRUE;
	}
}

int image_scale_2power(image *im, int minSizeX,int minSizeY, int maxSizeX,int maxSizeY,gbool square,gbool filteredScale)
{
    double xPow2, yPow2;
    int ixPow2, iyPow2;
    int xSize2, ySize2;
    
	int sizeX = im->width;
	int sizeY = im->height;

    GLint glMaxTexDimX=maxSizeX;
    GLint glMaxTexDimY=maxSizeY;


    if (!im->image )	return IME_BADIMAGE; // failure


    if (sizeX <= glMaxTexDimX)
		xPow2 = log((double)sizeX) / log((double)2.0);
    else
		xPow2 = log((double)glMaxTexDimX) / log((double)2.0);

    if (sizeY <= glMaxTexDimY)
		yPow2 = log((double)sizeY) / log((double)2.0);
    else
		yPow2 = log((double)glMaxTexDimY) / log((double)2.0);

    ixPow2 = (int)(xPow2+0.5); // round 
    iyPow2 = (int)(yPow2+0.5); // round 

    xSize2 = 1 << ixPow2;
    ySize2 = 1 << iyPow2;

#if 1
    // hg 04.08.97, small sizes supported in D3D
	//  13.03.98 but worse quality 
	int roundUpLimt = 128;

	// round up 
	if (xSize2<roundUpLimt && (glMaxTexDimX >= roundUpLimt))
//    if (xPow2 != (double)ixPow2)

    if ( (float) xPow2  >  (float)ixPow2)
		ixPow2++;

	if (ySize2<roundUpLimt && (glMaxTexDimY >= roundUpLimt))
//    if (yPow2 != (double)iyPow2)

	if ( (float) yPow2  >  (float)iyPow2)
		iyPow2++;
#endif


/*
	if ( (ixPow2 != iyPow2) && square) {
		// need square texture, take minimum 

		if (ixPow2 < iyPow2) iyPow2 = ixPow2;
		else ixPow2= iyPow2;
	}
*/
				      
    xSize2 = 1 << ixPow2;
    ySize2 = 1 << iyPow2;

	if (xSize2 < minSizeX) xSize2 = minSizeX; 
	if (ySize2 < minSizeY) ySize2 = minSizeY;

	if ( (xSize2 != ySize2) && square) {
		// need square texture, take maximum

		if (xSize2 < ySize2) xSize2 = ySize2;
		else ySize2= xSize2;
	}
	

	if ((xSize2 == sizeX) && (ySize2 == sizeY)) {
		// don´t need a scale 
	} else {

		image iscaled;
		int ret;

												                       
		TRACE("Scaling image from  %d * %d to %d * %d  (fmt = %d )\n",sizeX,sizeY,xSize2,ySize2,im->fmt);

		// allocte new sized image 
		image_zero(&iscaled);
		ret=image_alloc(&iscaled,im->fmt,xSize2,ySize2);
		if (ret<0) return(ret);

		if (im->fmt == IM_MAPPED8 || !filteredScale) // quick scale 
			ret = image_scale_copy(im,&iscaled); 
		else ret = image_scale_copy_filtered(im,&iscaled);

		if (ret<0) {
			image_free(&iscaled);
			return(ret);
		}
		free(im->image);
		im->image = iscaled.image;
		image_set_fmt(im,iscaled.fmt,iscaled.width,iscaled.height);
		iscaled.image = NULL;
		image_free_all(&iscaled);
	}
	return 0; // success
}


/*
   scale the image 

*/

int image_scale(image *im, int xSize2,int ySize2,gbool filteredScale)
{
    
	int sizeX = im->width;
	int sizeY = im->height;

	if ((xSize2 == sizeX) && (ySize2 == sizeY)) {
		// don´t need a scale 
	} else {

		image iscaled;
		int ret;

												                       
		TRACE("Scaling image from  %d * %d to %d * %d  (fmt = %d )\n",sizeX,sizeY,xSize2,ySize2,im->fmt);

		// allocte new sized image 
		image_zero(&iscaled);
		ret=image_alloc(&iscaled,im->fmt,xSize2,ySize2);
		if (ret<0) return(ret);

		if (im->fmt == IM_MAPPED8 || !filteredScale)
			ret = image_scale_copy(im,&iscaled); 
		else ret = image_scale_copy_filtered(im,&iscaled);

		if (ret<0) {
			image_free(&iscaled);
			return(ret);
		}
		free(im->image);
		im->image = iscaled.image;
		image_set_fmt(im,iscaled.fmt,iscaled.width,iscaled.height);
		iscaled.image = NULL;
		image_free_all(&iscaled);
	}
	return 1; // success
}


int  hasextension(const char *fileName,const char *ext) 
{
   	const char *extp;
 	extp = strrchr(fileName,'.');
	if (!extp) return(0);
	extp++;
	char buf[80];
	strcpy(buf,extp); strlwr(buf);
	return(streq(buf,ext));
}

/*

  load a PPM RGB file into memory
  ret = 1 if ok
  <=0 on error
*/

int ppm_read(image *im,const char *fileName)
{
	char buf[256];
	int ret=0;
	size_t cnt;
	int width,height;


    /*
     * Find the image file and open it
     */
	FILE *fp = fopen(fileName,"rb");

    if (fp == NULL) {
        TRACE("Cannot find %s.\n", fileName);
        return IME_NOFILE;
    }
    /*
     * Is it a PPM file?
     */
    fgets(buf, sizeof buf, fp);
    if (strcmp(buf, "P6\n")) {
        fclose(fp);
        TRACE("%s is not a PPM file.\n", fileName);
        return IME_BADFILE;
    }
    /*
     * Skip any comments
     */
    do {
        fgets(buf, sizeof buf, fp);
    } while (buf[0] == '#');
    /*
     * Read the width and height
     */
    sscanf(buf, "%d %d\n", &width, &height);
    fgets(buf, sizeof buf, fp); /* skip next line */
	if (width<1 || height <1) {
		ret = IME_BADFILE;
		goto error_exit;
	}

	cnt = width*height * 3;
	ret=image_alloc(im,IM_RGB24,width,height);
	if (ret<0) goto error_exit;


	if (fread(im->image,1,cnt,fp) != cnt) {
        image_free(im);
		ret = IME_FILEIO;
		goto error_exit;
	}

	ret = 1;

error_exit:
	fclose(fp);
	return(ret);
}


/*
  Read an image File into im
  scale to twopower if needed
  and convert a mapped8 image to rgb if needed.
*/


int  ReadImage(int format,const char *thefile,const char *options,image *im)
{ int ret;
  ret = IME_NOTIMP;

  switch (format) {

#ifdef _JPEG
  case JPEG_FMT:	
     {
     ret=jpeg_read(im,thefile,0,NULL);
     // new version is flipped: if (ret>=0) image_vflip(im,NULL); 
	 }
	 break;
#endif

#ifdef _PNG
  case PNG_FMT:	
     {
     ret=png_read(thefile,im);
     // if (ret>=0) image_vflip(im,NULL); 
	 }
	 break;
#endif


#ifdef _TARGA
  case TGA_FMT:	
     {
     ret=targa_read(im,thefile);
     if (ret>=0) image_vflip(im,NULL); 
	 }
	 break;
#endif

#ifdef _WIN32
#ifdef _DIB
  case DIB_FMT:	
   ret=dib_read(im,thefile);
   if (ret>= 0) {
       image_rgb_swap(im);  /* BMP RGB is BGR */
  //     image_vflip(im,NULL); /* BMP is bottom up */
   }
   break;
#endif
#endif

#ifdef _GIF
  case GIF_FMT:	
  { 
	  //ret=gif_read(im,thefile);
	  // new reader 
	 ret = ReadGif(im, thefile);
     if (ret>=0) image_vflip(im,NULL); 
  }
  break;
#endif

#ifdef _PCX
  case PCX_FMT:	
  {
    ret=pcx_read(im,thefile);
    if (ret>=0) image_vflip(im,NULL); 
  }
  break;
#endif
#ifdef _WIN32
#ifdef _RGB
  case RGB_FMT:	
  {
	AUX_RGBImageRec *imAUX=NULL;
    imAUX = tkRGBImageLoadAW((const char *)thefile,0);
	if (imAUX != NULL) {
	    image_set_fmt(im,IM_RGB,imAUX->sizeX,imAUX->sizeY);
		im->image = imAUX->data; // take over data
		imAUX->data = NULL;
		free(imAUX);
	} else ret = IME_BADFILE;
  }
  break;
#endif
#endif
  case PPM_FMT:	
  {
    ret=ppm_read(im,thefile);
    // if (ret>=0) image_vflip(im,NULL); 
  }
  break;
  } // switch 	

  return (ret);
}



/*
	 Read an image File into im
	 scale to power of 2 if needed
	 and convert a mapped8 image to rgb if needed.
*/

int  ReadTextureImage(int format,const char *theFile,image *im, 
					  int minSizeX,int minSizeY,int maxSizeX,int maxSizeY,gbool square,gbool filteredScale,
					  int supported_fmts)
{
	int ret;
	im->supported_fmts = supported_fmts;

	ret=ReadImage(format, theFile,"",im);
	if (ret <0) {
		TRACE("Error %d reading texture file %s \n",ret,theFile);
	}
	if (ret>=0) {
		ret = image_scale_2power(im,minSizeX,minSizeY,maxSizeX,maxSizeY,square,filteredScale);

/*
   		if (format == GIF_FMT && (im->fmt == IM_MAPPED8) && (!(supported_fmts & (1<<IM_MAPPED8)))&& ((supported_fmts & (1<<IM_RGBA)))) {
		   ret=image_convert_to_rgba(im);
   		}
		else 
*/
   		if ((im->fmt == IM_MAPPED8) && (!(supported_fmts & (1<<IM_MAPPED8)))) {
		   ret=image_convert_to_rgb(im);
   		}
   		else if ((im->fmt == IM_GRAY) && (!(supported_fmts & (1<<IM_GRAY)))) {
		   ret=image_convert_to_rgb(im);
   		}
	}
	return(ret);
}


/*
	Read an image File into im
	 scale to twopower if needed
	 and convert a mapped8 image to rgb if needed.
*/

int  ReadTextureImageOpt(int format,const char *theFile,image *im, 
					  int minSizeX,int minSizeY,int maxSizeX,int maxSizeY,
					  int supported_fmts,int options)
{
	int ret;
	im->supported_fmts = supported_fmts;
	

	ret=ReadImage(format, theFile,"",im);

	if (ret>=0) {
		
		if (options & OPT_SCALE_2POWER)
			ret = image_scale_2power(im,minSizeX,minSizeY,maxSizeX,maxSizeY,(options & OPT_SCALE_2POWER_SQUARE) !=0,(options & OPT_SCALE_FILTERD) !=0  );

/*
   		if (format == GIF_FMT && (im->fmt == IM_MAPPED8) && (!(supported_fmts & (1<<IM_MAPPED8)))&& ((supported_fmts & (1<<IM_RGBA)))) {
		   ret=image_convert_to_rgba(im);
   		}
		else 
*/
		switch (im->fmt) {
		case IM_MAPPED8:

   				if (!(supported_fmts & (1<<IM_MAPPED8))) {
					ret=image_convert_to_rgb(im);
   				}
				break;

		case  IM_GRAY :
			if (!(supported_fmts & (1<<IM_GRAY))) {
				if (supported_fmts & (1<<IM_MAPPED8))
					ret = image_convert_image_mapped8(im);
				else ret=image_convert_to_rgb(im);
   			}
			break;
		case  IM_GRAYA :
			if (!(supported_fmts & (1<<IM_GRAYA))) {
				if (supported_fmts & (1<<IM_RGBA))
					ret=image_convert_to_rgba(im);
				else if (supported_fmts & (1<<IM_MAPPED8))
					ret = image_convert_image_mapped8(im);
				else ret=image_convert_to_rgb(im);
   			}
			break;

		case IM_RGBA:
			if ((!(supported_fmts & (1<<IM_RGBA)))) {
				ret=image_convert_to_rgb(im);
   			}
		case IM_RGB:

			// if image formt not support, and we support mapped 8	
			if ((supported_fmts & (1<<IM_MAPPED8)) && (!(supported_fmts & (1<<im->fmt)))) {
				/* convert RGB image to FS dither mapped image */
				ret = image_convert_image_mapped8(im);
			}
			break;
		} // switch
	}
	return(ret);
}





int ReadImage(int format, const char *theFile,AUX_RGBImageRec **imAUX,BOOL &hasColorKey,BOOL &isGrayscale)
{
  int ret;

#ifdef _WIN32

    if (format == RGB_FMT) {
       *imAUX = tkRGBImageLoadAW((char *)theFile,0);
	   ret = imAUX != NULL;
    } else if (format == DIB_FMT) {
       *imAUX = tkDIBImageLoadAW((char *)theFile,0);
	   ret = imAUX != NULL;
	}
	else

#endif

    {

  image im;
  image_zero(&im);
  im.supported_fmts = get_supported_image_formats();

  ret=ReadImage(format, theFile,"",&im);

  if (ret>=0) {
	
	isGrayscale = image_is_gray(&im);


  	if ((im.fmt == IM_MAPPED8  && im.colorKey) && (!(supported_fmts & (1<<IM_MAPPED8)))&& ((supported_fmts & (1<<IM_RGBA)))) {
		   ret=image_convert_to_rgba(&im);
		   hasColorKey = TRUE;
  	}
	else 
		// to do: check for grayscale 
	if ((im.fmt == IM_MAPPED8) && (!(supported_fmts & (1<<IM_MAPPED8)))) {
      ret=image_convert_to_rgb(&im);
   	}
    
    *imAUX = (AUX_RGBImageRec *)malloc(sizeof(AUX_RGBImageRec));

	// to do: color table

	// to do : refine 
	(*imAUX)->components = im.bytesperpixel;
	switch (im.fmt) {
	default :
	case IM_RGB  : (*imAUX)->format = GL_RGB; break;
	case IM_RGBA : (*imAUX)->format = GL_RGBA; break;
	case IM_GRAY : (*imAUX)->format = GL_LUMINANCE; break;
	case IM_GRAYA : (*imAUX)->format = GL_LUMINANCE_ALPHA; break;
	}

    (*imAUX)->sizeX = im.width;
    (*imAUX)->sizeY = im.height;
    (*imAUX)->data = im.image;
    
    im.image = NULL; // take over data
	ret = 1;
  }
  image_free_all(&im);
	}

  return(ret);

}



// creates a texture from iamge file 

int InitImageTexture(int format, const char *bmpfile, int wflag, 
					 int buildMipmaps, int maxSizeX,int maxSizeY,
					 BOOL &hasAlpha,BOOL &hasColorKey,BOOL &isGrayscale,
				 	 unsigned char			&components,			// number of components 
				 	 short					&sizeX,		// current size 
				 	 short					&sizeY,		// current size 
					 short					&textureFormat		// index into texture format // D3D
)
{
    AUX_RGBImageRec *image = (AUX_RGBImageRec *) NULL;
	int ret=0;

	hasColorKey = FALSE;

    // load the bitmap file
#ifdef _WIN32

    if (format == RGB_FMT) {
       image = tkRGBImageLoadAW((char *)bmpfile,wflag);
	   isGrayscale = (image->components == 1) || (image->components == 2);
    } else if (format == DIB_FMT) {
       image = tkDIBImageLoadAW((char *)bmpfile,wflag);
	   isGrayscale = (image->components == 1) || (image->components == 2);
	}
	else

#endif

    {
		ret = ReadImage(format, bmpfile,&image,hasColorKey,isGrayscale);

		if (ret<=0) {
			if (image) goto exit;
		}
	}
    if( !image )	return ret; // failure


	hasAlpha = (image->components == 2) || (image->components == 4);
	TRACE("texture image %s %d * %d * %d  a%d c%d g%d \n",bmpfile,   		
				image->sizeX,image->sizeY,image->components,hasAlpha,hasColorKey,isGrayscale);

	ret = 0;

    ret = glSetTexture(image->format,
   					image->sizeX,image->sizeY,
   					image->components , 
				 	image->data,
				 	buildMipmaps,maxSizeX,maxSizeY,sizeX,sizeY);

	components = image->components;
	textureFormat = image->format;

	// ret= 1; // success
exit:
    free(image->data); 
    image->data = NULL;
	free(image); 

    return ret; 
}


// set gl texture including 2 power scaling and mip mapping
// format eg. GL_RGB ==> components = 3

int glSetTexture(GLenum format,int sizeX,int sizeY, int components, 
				 unsigned char *data, int buildMipmaps, int maxSizeX,int maxSizeY,
			 	 short					&cur_sizeX,		// current size 
			 	 short					&cur_sizeY		// current size 
				 )
{

    if (! data )	return 0; // failure

#ifdef _OGL

    double xPow2, yPow2;
    int ixPow2, iyPow2;
    int xSize2, ySize2;
    
    BYTE *pData = NULL; // scaled image
    
   
    GLint glMaxTexDimX;
    GLint glMaxTexDimY;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    glPixelStorei(GL_PACK_ALIGNMENT, 1); 


    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDimX);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDimY);
	
	if (maxSizeX>0) glMaxTexDimX= min(maxSizeX,glMaxTexDimX);
	if (maxSizeY>0) glMaxTexDimY= min(maxSizeY,glMaxTexDimY);


    if (sizeX <= glMaxTexDimX)
		xPow2 = log((double)sizeX) / log((double)2.0);
    else
		xPow2 = log((double)glMaxTexDimX) / log((double)2.0);

    if (sizeY <= glMaxTexDimY)
		yPow2 = log((double)sizeY) / log((double)2.0);
    else
		yPow2 = log((double)glMaxTexDimY) / log((double)2.0);

    ixPow2 = (int)(xPow2+0.5); // round
    iyPow2 = (int)(yPow2+0.5); // round

    xSize2 = 1 << ixPow2;
    ySize2 = 1 << iyPow2;

#if 1
    // hg 04.08.97, small sizes supported in D3D
	//  13.03.98 but worse quality 
	int roundUpLimt = 128;

	// round up 
	if (xSize2<roundUpLimt && (glMaxTexDimX >= roundUpLimt))
    //if (xPow2 != (double)ixPow2)    // this test fails even if both are the same !!!!
	if ( (xPow2 - ixPow2)  > EPS) 
		ixPow2++;

	if (ySize2<roundUpLimt && (glMaxTexDimY >= roundUpLimt))
    //if (yPow2 != (double)iyPow2)
	if ( (yPow2 - iyPow2)  > EPS) 
		iyPow2++;
#endif

	gbool square = gtrue; 

	if ( (ixPow2 != iyPow2) && square) {
		// need square texture, take minimum 

		if (ixPow2 < iyPow2) iyPow2 = ixPow2;
		else ixPow2= iyPow2;

	}




    xSize2 = 1 << ixPow2;
    ySize2 = 1 << iyPow2;

    cur_sizeX = xSize2;
    cur_sizeY = ySize2;

	if ((xSize2 == sizeX) && (ySize2 == sizeY)) {
		// don´t need a scale 
	} else 
	if (!buildMipmaps)
	{
    	pData = (unsigned char *)  malloc(xSize2 * ySize2 *  components * sizeof(BYTE));
    
    	if (!pData) return 0;
	
		TRACE("Scaling %d * %d to %d * %d Mipmaps %d \n",sizeX,sizeY,xSize2,ySize2,buildMipmaps);

		// scale is slow
    	gluScaleImage(format, sizeX, sizeY,
			  	GL_UNSIGNED_BYTE, data,
		  		xSize2, ySize2, GL_UNSIGNED_BYTE,
		  		pData);
	
	};

    if (buildMipmaps) {
		// does the scale for us
		gluBuild2DMipmaps(GL_TEXTURE_2D, components, sizeX, sizeY,
			  format, GL_UNSIGNED_BYTE, data);
    }
    else {
       cur_sizeX = xSize2;
       cur_sizeY = ySize2;

	   glTexImage2D(GL_TEXTURE_2D, 0, components, xSize2, ySize2,
			 0, format, GL_UNSIGNED_BYTE, (pData ?  pData : data));
    }

    if (pData) free(pData);

    return 1; // success
#endif

#ifdef _D3D
	return(0);
#endif
}



// load a image file, and return image record

AUX_RGBImageRec *
LoadImage(int format, const char *bmpfile, int wflag,int scale_2power )
{
    int ret;

    AUX_RGBImageRec *image = (AUX_RGBImageRec *) NULL;
	BOOL hasColorKey;
	BOOL isGrayscale;

    // load the bmp file
#ifdef _WIN32

    if (format == RGB_FMT)
       image = tkRGBImageLoadAW((char *)bmpfile,wflag);
    else if (format == DIB_FMT) {
       image = tkDIBImageLoadAW((char *)bmpfile,wflag);
	}
    else

#endif

		ret = ReadImage(format,bmpfile, &image,hasColorKey,isGrayscale);

    if( !image )
		return 0; // failure

#ifdef _OGL
    if (scale_2power) {
    double xPow2, yPow2;
    int ixPow2, iyPow2;
    int xSize2, ySize2;
    BYTE *pData;
    xPow2 = log((double)image->sizeX) / log((double)2.0);

    yPow2 = log((double)image->sizeY) / log((double)2.0);

    ixPow2 = (int)xPow2;
    iyPow2 = (int)yPow2;

     if ( (xPow2 - ixPow2)  > EPS) 
		ixPow2++;

   	if ( (yPow2 - iyPow2)  > EPS) 
		iyPow2++;

    xSize2 = 1 << ixPow2;
    ySize2 = 1 << iyPow2;

    pData = (unsigned char *) malloc(xSize2 * ySize2 * image->components * sizeof(BYTE));
    if (!pData)
	return 0;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // new 11.11.95
    glPixelStorei(GL_PACK_ALIGNMENT, 1); // new 11.11.95


    gluScaleImage(image->format, image->sizeX, image->sizeY,
		  GL_UNSIGNED_BYTE, image->data,
		  xSize2, ySize2, GL_UNSIGNED_BYTE,
		  pData);
    
    free(image->data);
    image->data = pData;
    image->sizeX = xSize2;
    image->sizeY = ySize2;
    }
#endif
    return image; // success
}
