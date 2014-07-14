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


   Copyright (c) 1990 - 1993, 1994  by Holger Grahn

   Purpose :
		image library core routines

   Functionality :
		allocation, deallocation of images

   Changes :

   Todo :

	 for mapped 8 + mapped 16, lookup for actually used colors

******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "image.h"

int image_debug=0;

const char * image_error_msg[]= { NULL,
		   "Image error",
		   "Out of memory during image operation",
		   "Bad image format",
		   "Image format is not compatible",
		   "Bad image file",
		   "image file not found",
		   "error during image io",
		   "image operation not implmented",
		   "image has no color map",
		   "bad image",
		   "Parameters out of range",
		   "Bad operation for image"
		 };

const char * get_image_error_message(int im_err)
{
   if (im_err >=0) return(NULL);
   if (im_err < IME_NOCMAP) return(NULL);
   return(image_error_msg[-im_err]);
}

/*
  zero out image structure,
  warning shouldn't contain allocated pointers
*/

void image_zero(image *im)
{ char *p;
  int cnt;
/* memchr(im,0,sizeof(image));
*/
 p = (char *)im;
 cnt = sizeof(image);
 while (cnt-- >0) *p++ = 0;
 im->backgroundIndex = -1; /* not assigned */ 
}


/*
   allocate a image
*/

int image_alloc(image *im,int fmt,int width,int height)
{
  im->width = width;
  im->height = height;
  im->fmt = fmt;
  switch (fmt) {
  case IM_BW :			/* 1 bit per pixel */
       im->bytesperpixel=0;
       im->bytesperscan= (im->width +7) / (8  * sizeof(IBYTE));
       im->bytesperimage = im->height * im->bytesperscan;
       im->image = (IBYTE *) malloc(im->bytesperimage);
       return (im->image == NULL) ? IME_NOMEM : 0;
  case IM_MAPPED4 :		/* 4 bit per pixel */
       im->bytesperpixel=0;
       im->bytesperscan= (im->width +1) / (2  * sizeof(IBYTE));
       im->bytesperimage = im->height * im->bytesperscan;
       im->image = (IBYTE *) malloc(im->bytesperimage);
       return (im->image == NULL) ? IME_NOMEM : 0;
  case IM_GRAY:
	  im->isGray = 1;
  case IM_MAPPED8:
	im->bytesperpixel=1;
	break;
  case IM_GRAYA:
	im->isGray = 1;
	im->bytesperpixel=2;
	break;
  case IM_MAPPED16:
  case IM_RGB15:
  case IM_RGB16:
	im->bytesperpixel=2;
	break;
  case IM_RGB24:
	im->bytesperpixel=3;
	break;
  case IM_RGB32:
  case IM_RGBA:
	im->bytesperpixel=4;
	break;
  case IM_RGB_SHORT:
	im->bytesperpixel=3*sizeof(ISHORT);
	break;
  case IM_RGBA_SHORT:
	im->bytesperpixel=4*sizeof(ISHORT);
	break;
  default :
	im->bytesperpixel=0;
	return(IME_BADFMT);
  }

  im->bytesperscan= im->width * im->bytesperpixel * sizeof(IBYTE);
  im->bytesperimage = im->height * im->bytesperscan;
  im->image = (IBYTE *) malloc(im->bytesperimage);
  if (image_debug) printf("Image alloc: %dx%d Fmt = %d  bytes = %ld \n",im->width,im->height,im->fmt,im->bytesperimage);

  return (im->image == NULL) ? IME_NOMEM : 0;
}

/*
   set image format
*/

int image_set_fmt(image *im,int fmt,int width,int height)
{
  im->width = width;
  im->height = height;
  im->fmt = fmt;
  switch (fmt) {
  case IM_BW :
	   im->isGray = 1;
       im->bytesperpixel=0;
       im->bytesperscan= (im->width +7) / (8  * sizeof(IBYTE));
       im->bytesperimage = im->height * im->bytesperscan;
       return (im->image == NULL) ? IME_NOMEM : 0;
  case IM_MAPPED4 :		/* 4 bit per pixel */
       im->bytesperpixel=0;
       im->bytesperscan= (im->width +1) / (2  * sizeof(IBYTE));
       im->bytesperimage = im->height * im->bytesperscan;
       return (im->image == NULL) ? IME_NOMEM : 0;
  case IM_GRAY:
	   im->isGray = 1;

  case IM_MAPPED8:
	im->bytesperpixel=1;
	break;
  case IM_GRAYA:
	   im->isGray = 1;
	   im->bytesperpixel=2;
	   break;
  case IM_MAPPED16:
  case IM_RGB15:
  case IM_RGB16:
	im->bytesperpixel=2;
	break;
  case IM_RGB24:
	im->bytesperpixel=3;
	break;
  case IM_RGB32:
  case IM_RGBA:
	im->bytesperpixel=4;
	break;
  case IM_RGB_SHORT:
	im->bytesperpixel=3*sizeof(ISHORT);
	break;
  case IM_RGBA_SHORT:
	im->bytesperpixel=4*sizeof(ISHORT);
	break;
  default :
	im->bytesperpixel=0;
	return(IME_BADFMT);
  }

  im->bytesperscan= im->width * im->bytesperpixel * sizeof(IBYTE);
  im->bytesperimage = im->height * im->bytesperscan;
  if (image_debug) printf("Image set fmt: %dx%d Fmt = %d  bytes = %ld \n",im->width,im->height,im->fmt,im->bytesperimage);

  return (im->image == NULL) ? IME_NOMEM : 0;
}


int image_set_fmt_by_ref(
	image * im,
	int fmt,
	int width,
	int height,
	IBYTE *imageData	   /* image data */
	)
{
	int ret;
	im->image = imageData;
	im->imageByRef = 1;

	ret = image_set_fmt(im,fmt,width,height);

	return(ret);

}



void image_free(image *im)
{
  if (im->image != NULL && !im->imageByRef) free(im->image);
  im->image = NULL;
  im->imageByRef = 0;
}

/*
   create a image info
   representing a subspace of a image

   never user any image_free routines on subim

   if (width <im->width) you must use sumbim->bytesperscan to correctly
   access the right scanline
*/

int image_set_subimage(image *im,int x,int y,int width,int height,image *subim)
{
  if (im->bytesperpixel == 0) return (IME_BADFMT);
  *subim = *im;

  subim->width = width;
  subim->height = height;
  subim->fmt = im->fmt;
  subim->image = IPIX(im,x,y); /* set pointer to first byte */
  subim->bytesperpixel=im->bytesperpixel;
  subim->bytesperscan=im->bytesperscan;
  subim->bytesperimage=subim->bytesperscan*subim->height;
  return(0);
}

// is image a grayscal image, for mapped 8 images test on palette 
int image_is_gray(
	image * im
	)
{
	if (im->isGray) return(1);

	if (im->fmt == IM_GRAY || im->fmt == IM_GRAYA) return(1);
	
	if (im->fmt == IM_MAPPED8 && im->cmap!= NULL) {
		int i;
		for(i=0; i<im->ncolors; i++) {
			irgb_color *c;
			c = &im->cmap[i];
			if (c->r == c->g  &&  c->r == c->b) {
			} 
			else return 0;
		}
		im->isGray = 1;
		return(1);
	}	
	
	return(0);

}



/* float color map */
int image_alloc_fcmap(image *im,int ncolors)
{
 im->ncolors = ncolors;
 im->fcmap = (frgb_color *) calloc(ncolors,sizeof(frgb_color));
 return (im->fcmap == NULL) ? IME_NOMEM : 0;
}

void image_free_fcmap(image *im)
{
  if (im->fcmap != NULL) free(im->fcmap);
  im->fcmap = NULL;
}

/* set color map entry i to fcolor r,g,b */

int image_set_fcmap(image *im,int i,float r,float g,float b)
{ frgb_color *c;
 if (i>=im->ncolors) return(-1);
 c=&im->fcmap[i];
 c->r = r, c->g =g, c->b = b;
 return(0);
}


int image_alloc_cmap(image *im,int ncolors)
{
 im->ncolors = ncolors;
 im->cmap = (irgb_color *) calloc(ncolors,sizeof(irgb_color));
 return (im->cmap == NULL) ? IME_NOMEM : 0;
}

void image_free_cmap(image *im)
{
  if (im->cmap != NULL) free(im->cmap);
  im->cmap = NULL;
}


/* set color map entry i to color r,g,b */

int image_set_cmap(image *im,int i,int r,int g,int b)
{ irgb_color *c;
 if ((im->cmap == NULL) || (i<0) || (i>=im->ncolors)) return(-1);
 c=&im->cmap[i];
 c->r = r, c->g =g, c->b = b;
 return(0);
}

/* get color map entry i, set r,g,b */

int image_get_cmap(image *im,int i,int *r,int *g,int *b)
{ irgb_color *c;
 if ((i<0) || (i>=im->ncolors)) return(-1);
 c=&im->cmap[i];
 *r= c->r , *g= c->g, *b= c->b;
 return(0);
}


void image_free_all(image *im)
{
  if (im) {
    image_free_fcmap(im);
    image_free_cmap(im);

#if 0
    if (im->filename ) free(im->filename);
    if (im->name ) free(im->name);
    if (im->comment) free(im->comment);
#endif

    image_free(im);
    if (image_debug)
       printf("Image free all : %dx%d Fmt = %d  bytes = %ld \n",im->width,im->height,im->fmt,im->bytesperimage);
  }
}



/* construct float color map from i color map */

int image_itof_cmap(image *im)
{ int ret;
  int i;
  float cscale;

  if (im->cmap == NULL) return(IME_NOCMAP); /* no colormap present */

  if (im->fcmap == NULL) {
     ret=image_alloc_fcmap(im,im->ncolors);
     if (ret < 0) return (ret);
  }

  cscale = 1.0f/255.0f;

  for (i=0; i<im->ncolors;i++) {
      image_set_fcmap(im,i,im->cmap[i].r * cscale,im->cmap[i].g * cscale,im->cmap[i].b * cscale);
  }
  return(0);
}

int image_ftoi_cmap(image *im)
{ int ret;
  int i;
  float cscale;

  if (im->fcmap == NULL) return(IME_NOCMAP);
  if (im->cmap == NULL) {
     ret=image_alloc_cmap(im,im->ncolors);
     if (ret < 0) return (ret);
  }

  cscale = 255.0f;

  for (i=0; i<im->ncolors;i++) {
      image_set_cmap(im,i,(int) (im->fcmap[i].r * cscale),
			  (int) (im->fcmap[i].g * cscale),
			  (int) (im->fcmap[i].b * cscale));
  }
  return(0);
}

/*
  copy colormap from imsrc to im
*/

int image_copy_cmap(const image *imsrc,image *im)
{  int ret,i;
    if (imsrc->cmap == NULL) return(IME_NOCMAP);

   image_free_fcmap(im);
   image_free_cmap(im);
   if (im->cmap == NULL) {
      ret=image_alloc_cmap(im,imsrc->ncolors);
      if (ret < 0) return (ret);
   }
   for (i=0; i<imsrc->ncolors;i++) {
      image_set_cmap(im,i,imsrc->cmap[i].r,imsrc->cmap[i].g,imsrc->cmap[i].b);
   }
   return(0);
}

/*
  simple image scaling
  without any resampling / filtering
  usefull for upscaling

  both images must be in same format and in memory

*/

int image_scale_copy(image *iin,image *iout)
{ IBYTE *o,*orow,*i;
  int sx,sy;
  int x,y;
  int bytesperpix;
  int prevsy;
  long xtmp;

/*
  float yscale;
  float xscale;
  yscale = iout->height / iin->height;
  xscale = iout->width / iin->width;
*/

  bytesperpix = iin->bytesperpixel;

  if (bytesperpix==0) return(IME_BADFMT);

  prevsy=-100;
  for ( y=0, o=iout->image;

	y < iout->height;

	y++,o+=iout->bytesperscan, prevsy=sy

	)
  {

    sy=  ((long) (y* iin->height)) / iout->height;

    if (prevsy == sy) {
		/* replicate output row */
		memcpy(o,o-iout->bytesperscan,iout->bytesperscan);
	continue;
    }

    /* input row for sy */
    i = iin->image+ sy * iin->bytesperscan;

	xtmp=0; // to do : use DDA etc 
	switch (bytesperpix) {
	case 0: break;
	case 1 : 
		for (x=0,orow=o; x < iout->width; x++,xtmp+=iin->width,orow+=1) {
			IBYTE *ix;
			ix =  i+ (xtmp / iout->width)*1; 
			orow[0]=ix[0]; 
		}
		break;
	case 2 : 
		for (x=0,orow=o; x < iout->width; x++,xtmp+=iin->width,orow+=2) {
			IBYTE *ix;
			ix =  i+ (xtmp / iout->width)*2; 
			*((IWORD*)orow) = *((IWORD*)ix); // orow[0]=ix[0]; orow[1]=ix[1]; 
		}
		break;
	case 3 : 
		for (x=0,orow=o; x < iout->width; x++,xtmp+=iin->width,orow+=3) {
			IBYTE *ix;
			ix =  i+ (xtmp / iout->width)*3; 
			orow[0]=ix[0]; orow[1]=ix[1]; orow[2]=ix[2];
		}
		break;
	case 4 : 
		for (x=0,orow=o; x < iout->width; x++,xtmp+=iin->width,orow+=4) {
			IBYTE *ix;
			ix =  i+ (xtmp / iout->width)*4; 
			*((ILONG*)orow) = *((ILONG*)ix); //orow[0]=ix[0]; orow[1]=ix[1]; orow[2]=ix[2]; ; orow[3]=ix[3];
		}
		break;
	default : 
		for (x=0,orow=o; x < iout->width; x++,xtmp+=iin->width,orow+=bytesperpix) {
			// sx=  ((long) (x* iin->width)) / iout->width;
			sx=  (xtmp / iout->width);
			memcpy(orow,i+sx*bytesperpix,bytesperpix);
		}
		break;
	} // bytesperpix

  }
  return(0);
}

/*
  simple image scaling
  without any resampling / filtering
  usefull for upscaling

  both images must be in same format and in memory

*/
#define EPSILON 0.001f

int image_scale_copy_filtered(image *iin,image *iout)
{ int bytesperpix;

  int widthin=iin->width;
  int heightin=iin->height;
  const IBYTE *tempin = iin->image;

  int widthout=iout->width;
  int heightout=iout->height;
  IBYTE *tempout = iout->image;

  // scaling factors 
  float sx = (float) widthin / (float) widthout;
  float sy = (float) heightin / (float) heightout;

  int i,j;

  bytesperpix = iin->bytesperpixel;

  if (bytesperpix==0) return(IME_BADFMT);


/*#define POINT_SAMPLE*/
#ifdef POINT_SAMPLE
   for (i=0;i<heightout;i++) {
      int ii = i * sy;
      for (j=0;j<widthout;j++) {
		int jj = j * sx;

		IBYTE *src = tempin + (ii * widthin + jj) * bytesperpix;
		IBYTE *dst = tempout + (i * widthout + j) * bytesperpix;

		for (k=0;k<bytesperpix;k++) {
			*dst++ = *src++;
		}
      }
   }
#else
   if (sx<1.0f && sy<1.0f) {
      /* magnify both width and height:  use weighted sample of 4 pixels */
      int i0, i1;
	  const IBYTE *srcA, *srcB;

      const IBYTE *src00, *src01, *src10, *src11;
      float s1, s2;
      IBYTE *dst;

      for (i=0;i<heightout;i++) {
		float alpha;
		float alpha1;

		i0 = i * sy;
		//i1 = (i+1) * sy - EPSILON;
		//alpha = i*sy - i0;
		// hg 
		i1=i0+1;
		if (i1>=heightin) i1 = heightin-1;

		alpha = i*sy - i0; // fract part
		alpha1 = 1.0f-alpha; // inverted 

		// set source row pointers
		srcA = tempin + (i0 * widthin) * bytesperpix;
		srcB = tempin + (i1 * widthin) * bytesperpix;

		// set destination row pointer 
		dst = tempout + (i * widthout) * bytesperpix;


		for (j=0;j<widthout;j++) {
			int k;
			int j0,j1;
			float beta, beta1;

			j0 = j * sx;
			j1=j0+1;
			if (j1>=widthin) j1 = widthin-1;

			//j1 = (j+1) * sx - EPSILON;
			beta = j*sx - j0;

			/* bilinear interpolation in rect (i0,j0)-(i1,j1) */
			src00 = srcA + j0 * bytesperpix;
			src01 = srcA + j1 * bytesperpix;
			src10 = srcB + j0 * bytesperpix;
			src11 = srcB + j1 * bytesperpix;

			beta1=(1.0f-beta);

			// unroll + int arithmetic  ??
			for (k=0;k<bytesperpix;k++) {
				s1 = *src00++ * beta1 + *src01++ * beta;
				s2 = *src10++ * beta1 + *src11++ * beta;
				*dst++ = s1 * alpha1 + s2 * alpha;
			}
		}
      }
   }
   else  {
      /* shrink width and/or height:  use an unweighted box filter */
      int i0, i1;
      int ii, jj;

	  IBYTE *dst;

      for (i=0;i<heightout;i++) {
		i0 = i * sy;
		i1 = (i+1) * sy - EPSILON;
		
		dst = tempout + (i * widthout) * bytesperpix;
		
		for (j=0;j<widthout;j++) {
			int k;
			int j0,j1;
			float sumScale;
			j0 = j * sx;
			j1 = (j+1) * sx - EPSILON;

			sumScale = 1.0f / (float) ((j1-j0+1) * (i1-i0+1));

			/* compute average of pixels in the rectangle (i0,j0)-(i1,j1) */
			for (k=0;k<bytesperpix;k++) {
				float sum = 0.0;
				for (ii=i0;ii<=i1;ii++) {
					for (jj=j0;jj<=j1;jj++) {
						sum += *(tempin + (ii * widthin + jj) * bytesperpix + k);
					}
				}
				sum *= sumScale;
				*dst++ = (IBYTE) sum; // truncate to byte
			}
		}
      }
   }
#endif

  return(0);
}


/*
  flip image vertical

  if iout == NULL or iout = iin
     replace iin with flipped version

  otherwise iout should be allocated and should have save dimensions as iin

*/

int image_vflip(image *iin,image *iout)
{ IBYTE *o,*i;
  int y;

  if (iin->bytesperscan ==0) return(IME_BADFMT);

  if ((iout == NULL) || (iout == iin)) {
	/* swap scanlines from outside to center */
	int y2;

    for ( y=0, y2=iin->height-1,
	i = iin->image + ((iin->height-1)*iin->bytesperscan),
	o=iin->image;

	y < y2;

	y++,y2--,
	i-=iin->bytesperscan, o+=iin->bytesperscan
      )
    { IBYTE t;	     /* flip one row */
      IBYTE *irow,*orow;
      int x;
      x = iin->bytesperscan;
      irow = i; orow=o;
      while (x-- > 0) {
	 t= *irow;
	 *irow = *orow;
	 *orow = t;
	 irow++; orow++;
      }
    }

  } else {

  for ( y=0,
	i = iin->image + ((iin->height-1)*iin->bytesperscan),
	o=iout->image;

	y < iin->height;

	y++,i-=iin->bytesperscan, o+=iout->bytesperscan

      )
  {
    memcpy(o,i,iin->bytesperscan); /* copy one row */
  }
  }
  return(0);
}


/*
   convert a mapped 8 image to a RGB24 image
   iout image may be larger.
*/

int image_mapped8_to_rgb(image *iin,image *iout)
{ IBYTE *o,*irow,*i,*orow;
  int x,y;

  if (iin->cmap == NULL)  return(IME_NOCMAP);

  for ( y = iin->height,
	i = iin->image,
	o = iout->image;

	y>0;

	y--,i+=iin->bytesperscan, o+=iout->bytesperscan

      )
  {
     for (x=iin->width,orow=o,irow=i; x>0; x--) {

	int ci;
	irgb_color *c;
	ci = *irow++;

	/* if ((c== NULL) || (ci<0) || (ci>=im->ncolors)) return(-1); */

	c=&iin->cmap[ci];
	*orow++ =c->r;
	*orow++ =c->g;
	*orow++ =c->b;
     }
  }
  return(0);
}

/*
   convert a mapped 8 image to a RGBA image
   iout image may be larger.
*/


int image_mapped8_to_rgba(image *iin,IBYTE alphaColor, IBYTE alpha1, IBYTE alpha2, image *iout)
{ IBYTE *o,*irow,*i,*orow;
  int x,y;
						
  if (iin->cmap == NULL)  return(IME_NOCMAP);

  for ( y = iin->height,
	i = iin->image,
	o = iout->image;

	y>0;

	y--,i+=iin->bytesperscan, o+=iout->bytesperscan

      )
  {
     for (x=iin->width,orow=o,irow=i; x>0; x--) {

	int ci;
	irgb_color *c;
	ci = *irow++;

	/* if ((c== NULL) || (ci<0) || (ci>=im->ncolors)) return(-1); */

	c=&iin->cmap[ci];
	*orow++ =c->r;
	*orow++ =c->g;
	*orow++ =c->b;
	if (ci == alphaColor) *orow++ = alpha1;
	else *orow++ = alpha2;
     }
  }
  return(0);
}


int image_rgb_to_rgba(image *iin,image *iout,IBYTE alpha)
{ IBYTE *o,*irow,*i,*orow;
  int x,y;

  for ( y = iin->height,
	i = iin->image,
	o = iout->image;

	y>0;

	y--,i+=iin->bytesperscan, o+=iout->bytesperscan

      )
  {
     for (x=iin->width,orow=o,irow=i; x>0; x--) {
		*orow++ = *irow++;
		*orow++ = *irow++;
		*orow++ = *irow++;
		*orow++ = alpha;
     }
  }
  return(0);
}

int image_rgba_to_rgb(image *iin,image *iout)
{ IBYTE *o,*irow,*i,*orow;
  int x,y;

  for ( y = iin->height,
	i = iin->image,
	o = iout->image;

	y>0;

	y--,i+=iin->bytesperscan, o+=iout->bytesperscan

      )
  {
     for (x=iin->width,orow=o,irow=i; x>0; x--) {
		*orow++ = *irow++;
		*orow++ = *irow++;
		*orow++ = *irow++;
		irow++; // skip a 
     }
  }
  return(0);
}

int image_graya_to_rgba(image *iin,image *iout)
{ IBYTE *o,*irow,*i,*orow;
  int x,y;

  for ( y = iin->height,
	i = iin->image,
	o = iout->image;

	y>0;

	y--,i+=iin->bytesperscan, o+=iout->bytesperscan

      )
  {
     for (x=iin->width,orow=o,irow=i; x>0; x--) {
		IBYTE g;
		g = *irow++;
		*orow++ = g;
		*orow++ = g; 
		*orow++ = g;
		*orow++ = *irow++;
     }
  }
  return(0);
}

int image_graya_to_rgb(image *iin,image *iout)
{ IBYTE *o,*irow,*i,*orow;
  int x,y;

  for ( y = iin->height,
	i = iin->image,
	o = iout->image;

	y>0;

	y--,i+=iin->bytesperscan, o+=iout->bytesperscan

      )
  {
     for (x=iin->width,orow=o,irow=i; x>0; x--) {
		IBYTE g;
		g = *irow++;
		*orow++ = g;
		*orow++ = g; 
		*orow++ = g;
		irow++; // skip A 
     }
  }
  return(0);
}

int image_graya_to_gray(image *iin,image *iout)
{ IBYTE *o,*irow,*i,*orow;
  int x,y;

  for ( y = iin->height,
	i = iin->image,
	o = iout->image;

	y>0;

	y--,i+=iin->bytesperscan, o+=iout->bytesperscan

      )
  {
     for (x=iin->width,orow=o,irow=i; x>0; x--) {
		*orow++ = *irow++;
		irow++; // skip alpha 
     }
  }
  return(0);
}


void mapped4_to_mapped8(IBYTE *a, int cnt, IBYTE *b)
{
   while (--cnt >=0) {
     *b++ = (*a) >> 4;
     *b++ = (*a) & 0X0F;
     a++;
   }
}

void mapped4_to_mapped8_rev(IBYTE *a, int cnt, IBYTE *b)
{
   b+= (cnt *2) -1;
   a+= cnt -1;
   while (--cnt >=0) {
     *b-- = (*a) & 0X0F;
     *b-- = (*a) >> 4;
     a--;
   }
}

void mapped8_to_mapped4(IBYTE *a, int cnt, IBYTE *b)
{ IBYTE t;
   while (--cnt >=0) {
     t = (*a++)<<4; ;
     t |= (*a++) & 0x0F;
     *b++ = t;
   }
}

/* expand bw data to mapped 8 */
void packed_to_mapped8(const IBYTE *a, int cnt, IBYTE *b)
{
   while (--cnt >=0) {
     register IBYTE x;
     x = *a++;
#if 0
     *b++ = (x & 1) != 0;
     *b++ = (x & 2) != 0;
     *b++ = (x & 4) != 0;
     *b++ = (x & 8) != 0;
     *b++ = (x & 16) != 0;
     *b++ = (x & 32) != 0;
     *b++ = (x & 64) != 0;
     *b++ = (x & 128) != 0;
#else
     *b++ = (x & 128) != 0;
     *b++ = (x & 64) != 0;
     *b++ = (x & 32) != 0;
     *b++ = (x & 16) != 0;
     *b++ = (x & 8) != 0;
     *b++ = (x & 4) != 0;
     *b++ = (x & 2) != 0;
     *b++ = (x & 1) != 0;
#endif
   }
}

void packed_to_mapped8_rev(const IBYTE *a, int cnt, IBYTE *b)
{
   a+=cnt-1;
   b+=cnt*8-1;
   while (--cnt >=0) {
     register IBYTE x;
     x = *a--;
#if 1
     *b-- = (x & 1) != 0;
     *b-- = (x & 2) != 0;
     *b-- = (x & 4) != 0;
     *b-- = (x & 8) != 0;
     *b-- = (x & 16) != 0;
     *b-- = (x & 32) != 0;
     *b-- = (x & 64) != 0;
     *b-- = (x & 128) != 0;
#else
     *b-- = (x & 128) != 0;
     *b-- = (x & 64) != 0;
     *b-- = (x & 32) != 0;
     *b-- = (x & 16) != 0;
     *b-- = (x & 8) != 0;
     *b-- = (x & 4) != 0;
     *b-- = (x & 2) != 0;
     *b-- = (x & 1) != 0;
#endif
   }
}

/*
   convert a packed image to a mapped 8 image
   iout will be allocated
*/

int image_packed_to_mapped8(image *iin,image *iout)
{ int ret;
  image_free_all(iout);
  ret=image_alloc(iout,IM_MAPPED8,iin->bytesperscan*8,iin->height);
  if (ret<=0) return(ret);

  image_alloc_cmap(iout,2);
  image_set_cmap(iout,0, 0,0,0);
  image_set_cmap(iout,1, 255,255,255);
  packed_to_mapped8(iin->image,iin->bytesperimage,iout->image);
  return(0);
}

/******************************************************************************/

/* Runlength encoding / decoding like TGA codeing
*/
int pack_byte_run_length(int cols, const IBYTE *pixel,IBYTE *dest_start)
{
   const IBYTE *pixel_end= pixel+cols;
   IBYTE curpixel;
   IBYTE *dest=dest_start;
   IBYTE *cur_cnt=NULL;
   do {
       unsigned cnt;
       curpixel = *pixel++;
       cnt=0;

       while ((pixel<pixel_end) && (cnt<127) && (*pixel == curpixel))
	 { cnt++; pixel++; }

       if (cnt == 0 ) {
	  if ((cur_cnt != NULL) && (*cur_cnt<127))
	     { (*cur_cnt)++;	/* increment previous unencoded scan */
	     }
	  else {
	    cur_cnt = dest;
	    *dest++ = 0;
	  }
       } else {
	 *dest++ = 0x80 + cnt;
	 cur_cnt = NULL;
       }
       *dest++ = curpixel;
   } while (pixel < pixel_end);
   return(dest-dest_start);
}

int unpack_byte_run_length(int cols, const IBYTE *src,IBYTE *dest_start)
{
   const IBYTE *dest_end= dest_start+cols;
   const IBYTE *src_org= src;
   IBYTE curpixel;
   IBYTE *dest=dest_start;
   int cnt;
   do {
       cnt = *src++;
       if (cnt & 0x80) {  /* RLE */
	   cnt = (cnt & 0x07F) ;
	   curpixel=*src++;
	   do {
	     *dest++ = curpixel;
	   }  while (cnt-- >0);
       } else {
	   do {
	     *dest++ = *src++;
	   }  while (cnt-- >0);
       }
   } while (dest < dest_end);
   return(src-src_org);
}


/* ********* RGB RLE */

#define PUT_RGB_PIXEL(buf,RGB)	*buf++ = (RGB).r,*buf++ = (RGB).g,*buf++ = (RGB).b
#define GET_RGB_PIXEL(buf,RGB)	{(RGB).r = *buf++; (RGB).g = *buf++; (RGB).b = *buf++;}
#define RGB_PIXEL_EQ(A,B)  (((A).r == (B).r) && ((A).g == (B).g) && ((A).b == (B).b))

int pack_rgb_run_length(int cols, /* const */ irgb_pixel *pixel,IBYTE *dest_start)
{
   const irgb_pixel *pixel_end= pixel+cols;
   irgb_pixel curpixel;
   IBYTE *dest=dest_start;
   IBYTE *cur_cnt=NULL;
   do {
       unsigned cnt;
       curpixel = *pixel;
       pixel++;
       cnt=0;

       while ((RGB_PIXEL_EQ(*pixel,curpixel))
		&& (pixel<pixel_end) && (cnt<127) )
	 { cnt++; pixel++; }

       if (cnt == 0 ) {
	  if ((cur_cnt != NULL) && (*cur_cnt<127))
	     { (*cur_cnt)++;	/* increment previous unencoded scan */
	     }
	  else {
	    cur_cnt = dest;
	    *dest++ = 0;
	  }
       } else {
	 *dest++ = 0x80 + cnt;
	 cur_cnt = NULL;
       }
      PUT_RGB_PIXEL(dest,curpixel);
   } while (pixel < pixel_end);
   return(dest-dest_start);
}

int unpack_rgb_run_length(int cols, const IBYTE *src,
			irgb_pixel *dest_start)
{
   const irgb_pixel *dest_end= dest_start+cols;
   const IBYTE *src_org= src;
   irgb_pixel curpixel;
   irgb_pixel *dest=dest_start;
   int cnt;
   do {
       cnt = *src++;
       if (cnt & 0x80) {  /* RLE */
	   cnt = (cnt & 0x07F) ;
	   GET_RGB_PIXEL(src,curpixel);
	   do {
	     *dest++ = curpixel;
	   }  while (cnt-- >0);
       } else {

	    cnt++; memcpy(dest,src,cnt*sizeof(curpixel)); dest +=cnt;
	       src += cnt*sizeof(curpixel);
	   /*
	   do {
	     GET_WORD_PIXEL(src,curpixel);
	     *dest++ = curpixel;
	   }  while (cnt-- >0);
	   */
       }
   } while (dest < dest_end);
   return(src-src_org);
}

/* ********* WORD RLE */


#define PUT_WORD_PIXEL(buf,RGB)	*buf++ = (RGB) & 0xFF,*buf++ = RGB >> 8
#define GET_WORD_PIXEL(buf,RGB) RGB= *buf++, RGB |= (*buf++ << 8)

int pack_word_run_length(int cols, const IWORD *pixel,IBYTE *dest_start)
{
   const IWORD *pixel_end= pixel+cols;
   IWORD curpixel;
   IBYTE *dest=dest_start;
   IBYTE *cur_cnt=NULL;
   do {
       unsigned cnt;
       curpixel = *pixel++;
       cnt=0;

       while ((*pixel == curpixel)
		&& (pixel<pixel_end) && (cnt<127) )
	 { cnt++; pixel++; }

       if (cnt == 0 ) {
	  if ((cur_cnt != NULL) && (*cur_cnt<127))
	     { (*cur_cnt)++;	/* increment previous unencoded scan */
	     }
	  else {
	    cur_cnt = dest;
	    *dest++ = 0;
	  }
       } else {
	 *dest++ = 0x80 + cnt;
	 cur_cnt = NULL;
       }
      PUT_WORD_PIXEL(dest,curpixel);
   } while (pixel < pixel_end);
   return(dest-dest_start);
}

int unpack_word_run_length(int cols, const IBYTE *src,
			IWORD *dest_start)
{
   const IWORD *dest_end= dest_start+cols;
   const IBYTE *src_org= src;
   IWORD curpixel;
   IWORD *dest=dest_start;
   int cnt;
   do {
       cnt = *src++;
       if (cnt & 0x80) {  /* RLE */
	   cnt = (cnt & 0x07F) ;
	   GET_WORD_PIXEL(src,curpixel);
	   do {
	     *dest++ = curpixel;
	   }  while (cnt-- >0);
       } else {

	    cnt++; memcpy(dest,src,cnt*sizeof(curpixel)); dest +=cnt;
	       src += cnt*sizeof(curpixel);
	   /*
	   do {
	     GET_WORD_PIXEL(src,curpixel);
	     *dest++ = curpixel;
	   }  while (cnt-- >0);
	   */
       }
   } while (dest < dest_end);
   return(src-src_org);
}

/* ********* LONG RLE */


#define PUT_LONG_PIXEL(buf,RGB)	*buf++ = (RGB) & 0xFF,*buf++ = RGB >> 8,*buf++ = RGB >> 16,*buf++ = RGB >> 24
#define GET_LONG_PIXEL(buf,RGB) RGB= *buf++; RGB |= (*buf++ << 8); RGB |= (*buf++ << 16);RGB |= (*buf++ << 24)

int pack_long_run_length(int cols, const ILONG *pixel,IBYTE *dest_start)
{
   const ILONG *pixel_end= pixel+cols;
   ILONG curpixel;
   IBYTE *dest=dest_start;
   IBYTE *cur_cnt=NULL;
   do {
       unsigned cnt;
       curpixel = *pixel++;
       cnt=0;

       while ((*pixel == curpixel)
		&& (pixel<pixel_end) && (cnt<127) )
	 { cnt++; pixel++; }

       if (cnt == 0 ) {
	  if ((cur_cnt != NULL) && (*cur_cnt<127))
	     { (*cur_cnt)++;	/* increment previous unencoded scan */
	     }
	  else {
	    cur_cnt = dest;
	    *dest++ = 0;
	  }
       } else {
	 *dest++ = 0x80 + cnt;
	 cur_cnt = NULL;
       }
      PUT_LONG_PIXEL(dest,curpixel);
   } while (pixel < pixel_end);
   return(dest-dest_start);
}

int unpack_long_run_length(int cols, const IBYTE *src,
			ILONG *dest_start)
{
   const ILONG *dest_end= dest_start+cols;
   const IBYTE *src_org= src;
   ILONG curpixel;
   ILONG *dest=dest_start;
   int cnt;
   do {
       cnt = *src++;
       if (cnt & 0x80) {  /* RLE */
	   cnt = (cnt & 0x07F) ;
	   GET_LONG_PIXEL(src,curpixel);
	   do {
	     *dest++ = curpixel;
	   }  while (cnt-- >0);
       } else {

	    cnt++; memcpy(dest,src,cnt*sizeof(curpixel)); dest +=cnt;
	       src += cnt*sizeof(curpixel);
	   /*
	   do {
	     GET_LONG_PIXEL(src,curpixel);
	     *dest++ = curpixel;
	   }  while (cnt-- >0);
	   */
       }
   } while (dest < dest_end);
   return(src-src_org);
}


/******************************************************************************/
/*
  rle compress image iin to image iiout
  iin must be in a 1 2 3 or 4 bytes per pixel format
  iout should have same format and same number of rows,
  for savety cols should be larger

*/

int image_pack_rle(/* const */image *iin,image *iout)
{ IBYTE *o,*i;
  int y;
  int cnt;
  long sum;

  if (iin->bytesperscan ==0) return(IME_BADFMT);
  if (iout->image == NULL) image_alloc(iout,iin->fmt,(iin->width*12)/10,iin->height);
  if (iout->image == NULL) return(IME_BADIMAGE);

  sum =0;

  for ( y=0,
	i = iin->image,
	o=iout->image;

	y < iin->height;

	y++,i+=iin->bytesperscan
      )
  {
     switch (iin->bytesperpixel) {
     case 4:
	cnt=pack_long_run_length(iin->bytesperscan / 4, (ILONG *) i, o);
	break;
     case 3:
	cnt=pack_rgb_run_length(iin->bytesperscan / 3, (irgb_pixel *) i, o);
	break;
    case 2:
	cnt=pack_word_run_length(iin->bytesperscan / 2, (IWORD *) i, o);
	break;
     case 1:
	cnt=pack_byte_run_length(iin->bytesperscan, i, o);
	break;
     default :
	return(IME_NOTIMP);
     }
     o+=cnt;
     sum+=cnt;
     if (sum > iout->bytesperimage) {
	fprintf(stderr,"image_pack_rle:Compressed size %d exceeds raw size %d \n",sum,iout->bytesperimage);
	break;
     }
  }
  iout->compression = IM_RLE_COMPRESSION;
  iout->compressed_bytesperimage = sum;
/*  printf("image_pack_rle: Size before : %ld after : %ld Ratio : %f \n",iin->bytesperimage,sum,100.0 * (float ) sum / (float) iin->bytesperimage ); */
  return(0);
}


int image_unpack_rle( /* const */ image *iin,image *iout)
{ IBYTE *o,*i;
  int y;
  int cnt;
  long sum;

  if (iin->bytesperscan ==0) return(IME_BADFMT);
  if (iout->image == NULL) image_alloc(iout,iin->fmt,iin->width,iin->height);
  if (iout->image == NULL) return(IME_BADIMAGE);

  sum =0;

  for ( y=0,
	i = iin->image,
	o=iout->image;

	y < iin->height;

	y++,o+=iout->bytesperscan
      )
  {
     switch (iin->bytesperpixel) {
     case 4:
	cnt=unpack_long_run_length(iout->bytesperscan / 4,i ,(ILONG *) o);
	break;
     case 3:
	cnt=unpack_rgb_run_length(iout->bytesperscan / 3,i ,(irgb_pixel *) o);
	break;
    case 2:
	cnt=unpack_word_run_length(iout->bytesperscan / 2, i, (IWORD *) o);
	break;
     case 1:
	cnt=unpack_byte_run_length(iout->bytesperscan, i, o);
	break;
     default :
	return(IME_NOTIMP);
     }

     i+=cnt;
     sum+=cnt;
/*
     if (sum > iout->bytesperimage) {
	printf("unCompressed size exceeds raw size \n");
	break;
     }
*/
  }
/*  printf("Unpack RLE Size before : %ld after : %ld \n",iin->bytesperimage,sum); */
  return(0);
}


/******************************************************************************/
/*
  get a pixel at (x,y) in rgba format */
int image_get_rgb_color(image *im,int x,int y,irgb_color *p)
{ IBYTE *i;
  i= IPIX(im,x,y);
  switch (im->fmt) {
  case IM_RGB24:
  case IM_RGB32:
	p->r = *i++;
	p->g = *i++;
	p->b = *i++;
	break;
  case IM_RGBA:
	p->r = *i++;
	p->g = *i++;
	p->b = *i++;
	p->a = *i++;
	break;
  case IM_MAPPED8:
	{ irgb_color *ci;
	if (!im->cmap) return(IME_NOCMAP);
	ci = &im->cmap[*i];
	p->r = ci->r;
	p->g = ci->g;
	p->b = ci->b;
	p->a = ci->a;
	}
	break;
  case IM_GRAY:
	p->r = p->g  = p->b = *i;
	break;
  case IM_GRAYA:
	p->r = p->g  = p->b = *i++;
	p->a = *i++;
	break;
 default :
	return(IME_NOTIMP);
 }
 return(0);
}

/*
  apply operation on RGBA type image
*/
/*
int (*f)(int x,int y,IBYTE *pix,
*/

int image_alpha_op(image *im,int op,irgb_color *p)
{ IBYTE *irow;
  int cnt;
#define BPERPIX 4
  if (im->bytesperpixel == 0) return(IME_BADFMT);
  if (im->bytesperpixel != BPERPIX) return(IME_BADFMT);
  /* for all rows */
  for (	irow = im->image,cnt= im->height;
	cnt >0;
	irow+= im->bytesperscan,cnt--)
  {
    IBYTE *i;
    int x;
    switch (op) {
    case IMOP_NOOP: break;
    case IMOP_SETR : for (i=irow,x=im->width; x>0; i+=BPERPIX, x--) { i[0]=p->r; } break;
    case IMOP_SETG : for (i=irow,x=im->width; x>0; i+=BPERPIX, x--) { i[1]=p->g; } break;
    case IMOP_SETB : for (i=irow,x=im->width; x>0; i+=BPERPIX, x--) { i[2]=p->b; } break;
    case IMOP_SETA : for (i=irow,x=im->width; x>0; i+=BPERPIX, x--) { i[3]=p->a; } break;
    case IMOP_SETRGB : for (i=irow,x=im->width; x>0; i+=BPERPIX, x--) {i[0]=p->r; i[1]=p->g;i[2]=p->b; } break;
    case IMOP_SETRGBA : for (i=irow,x=im->width; x>0; i+=BPERPIX, x--) {i[0]=p->r; i[1]=p->g;i[2]=p->b;i[3]=p->a; } break;

    case IMOP_SETA_IF_RGBEQ :
	for (i=irow,x=im->width; x>0; i+=BPERPIX, x--) {int eq;
	    eq = (p->r == i[0]) && (p->g == i[1]) && (p->g == i[2]);
	    if (eq) i[3]=p->a;
	}
	break;
    default :
      return (IME_BADOP);
    }
  }
  return(0);
}
#undef BPERPIX

/*
  change order of bytes
  or set image channels to a constant value

  to do:
     add a gray channel
     complement channel ....
*/

int image_channel_map(image *im,IBYTE *map,IBYTE *channel)
{ IBYTE *irow;
  int cnt;
  if (im->bytesperpixel == 0) return(IME_BADFMT);
  /* for all rows */
  for (	irow = im->image,cnt= im->height;
	cnt >0;
	irow+= im->bytesperscan,cnt--)
  {
    IBYTE *i;
    int x;
    int bperpix= im->bytesperpixel;
    for (i=irow,x=im->width; x>0; i+=bperpix, x--) {
	switch (bperpix) {
	case 6 : channel[5]=i[5];
	case 5 : channel[4]=i[4];
	case 4 : channel[3]=i[3];
	case 3 : channel[2]=i[2];
	case 2 : channel[1]=i[1];
	case 1 : channel[0]=i[0];
	break;
	}

	switch (bperpix) {
	case 6 : i[5]=channel[map[5]];
	case 5 : i[4]=channel[map[4]];
	case 4 : i[3]=channel[map[3]];
	case 3 : i[2]=channel[map[2]];
	case 2 : i[1]=channel[map[1]];
	case 1 : i[0]=channel[map[0]];
	break;
	}

   }
  }
  return(0);
}

int image_rgb_swap(image *im)
{ IBYTE channel[4];
  IBYTE map[4]= {2,1,0,3};

  if (im->bytesperpixel < 3) return(IME_BADFMT);
  if (im->bytesperpixel > 4) return(IME_BADFMT);

  return (image_channel_map(im,	map,channel));
}


/*
  Copy image subrectangle

  iout[xdest:width,ydest:height] = iin[xstart:width,ystart:height]
  iout + iin should have same depth
  no error checking !

  color key support if MAPPED8 format
*/

int image_copy(	image *iin, // src image
				int xstart,int ystart, int width, int height,
				image *iout, // destination image
				int xdest,int ydest,
				int useCkey	// don"t write transparent pixels of iin
				)
{ IBYTE *o,*i;
  int y,xsize;
  IBYTE key = iin->colorKeyIndex;

  int colorKey = useCkey &&(iin->colorKey>0 && iin->fmt == IM_MAPPED8);

  if ((xdest + width) > iout->width) { // clamp 
	  width = iout->width-xdest;
  }		
  if (width <=0) return -1;

  if ((ydest + height) > iout->height) { // clamp 
	  height = iout->height-ydest;
  }		
  if (height <=0) return -1;


  xsize = width * iin->bytesperpixel;
  for ( y = height,
		i = IPIX(iin,xstart,ystart),
		o = IPIX(iout,xdest,ydest);

		y>0;

		y--,i+=iin->bytesperscan, o+=iout->bytesperscan

      )
  {
		if (colorKey) {
			int j;
			for (j=0; j<xsize;j++) 
				if (i[j]!=key) o[j]=i[j];

		} else memcpy(o,i,xsize);
  }
  return(0);
}

/*
 set subrectangle
 to constant color
*/


int image_set_8(
	image *iout,	// destination image
	int xdest,int ydest, // position
	int width, int height, // size
	IBYTE color	// color
	)

{ IBYTE *o;
  int y,xsize;

  if ((xdest + width) > iout->width) { // clamp 
	  width = iout->width-xdest;
  }		
  if (width <=0) return -1;

  if ((ydest + height) > iout->height) { // clamp 
	  height = iout->height-ydest;
  }		
  if (height <=0) return -1;


  xsize = width * iout->bytesperpixel;

  for ( y = height,
		o = IPIX(iout,xdest,ydest);

		y>0;

		y--, o+=iout->bytesperscan

      )
  {
		memset(o,color,xsize);
  }
  return(0);
}

