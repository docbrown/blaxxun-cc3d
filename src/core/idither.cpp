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

#include "image.h"

//#include "glutils.h"

/* convert a image to rgb */
int image_convert_to_rgb(image *im);
int image_convert_to_rgba(image *im);



// IDither



/* Range values for lum, cr, cb. */
static int CR_RANGE=8;
static int CG_RANGE=8;
static int CB_RANGE=4;
static int CA_RANGE=2;

int SetCRange(int cr,int cg, int cb)
{
  CR_RANGE=cr;
  CG_RANGE=cg;
  CB_RANGE=cb;
  return(0);
}


/* Arrays holding quantized value ranged for r, g, b channels. */

static int *cr_values=NULL;
static int *cg_values=NULL;
static int *cb_values=NULL;
static int *ca_values=NULL;


/*
 *--------------------------------------------------------------
 *
 * InitColor --
 *
 *	Initialized cr, cg, and cb quantized range value arrays.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

void
InitColor(void)
{
  int i;

  cr_values = (int *) malloc(CR_RANGE*sizeof(int));
  cg_values = (int *) malloc(CG_RANGE*sizeof(int));
  cb_values = (int *) malloc(CB_RANGE*sizeof(int));
  ca_values = (int *) malloc(CA_RANGE*sizeof(int));

  for (i=0; i<CR_RANGE; i++) {
    //cr_values[i] = ((i * 256) / (CR_RANGE)) + (256/(CR_RANGE*2));
    cr_values[i] = ((i * 255 + (CR_RANGE / 2)-1) / (CR_RANGE-1)); // HG
  }

  for (i=0; i<CG_RANGE; i++) {
    //cg_values[i] = ((i * 256) / (CG_RANGE)) + (256/(CG_RANGE*2));
    cg_values[i] = ((i * 255 + (CG_RANGE / 2)-1) / (CG_RANGE-1)); // HG
  }

  for (i=0; i<CB_RANGE; i++) {
   // cb_values[i] = ((i * 256) / (CB_RANGE)) + (256/(CB_RANGE*2));
    cb_values[i] = ((i * 255 + (CB_RANGE / 2)-1) / (CB_RANGE-1)); // HG
  }

  for (i=0; i<CA_RANGE; i++) {
   // ca_values[i] = ((i * 256) / (CA_RANGE)) + (256/(CA_RANGE*2));
    ca_values[i] = ((i * 255 + (CA_RANGE / 2)-1) / (CA_RANGE-1)); // HG
  }
}

/*
void InitColormap(void)
{
  int ncolors = CR_RANGE*CG_RANGE*CR_RANGE;
  int i, cr_num, cg_num, cb_num;
  unsigned char r, g, b;


  for (i=0; i<ncolors; i++) {
    cr_num = (i / (CR_RANGE*CG_RANGE))%CR_RANGE;
    cg_num = (i / CG_RANGE)%CR_RANGE;
    cb_num = i % CG_RANGE;
  }
}
*/

void
TermColor(void)
{
  if (cr_values) free(cr_values); cr_values = NULL;
  if (cg_values) free(cg_values); cg_values = NULL;
  if (cb_values) free(cb_values); cb_values = NULL;
  if (ca_values) free(ca_values); ca_values = NULL;
}


/*
 * Copyright (c) 1992 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

/* Arrays containing error values for floyd-steinberg dithering. */

static int deltay[256];
static int deltau[256];
static int deltav[256];
static int deltaa[256];
static int deltay2[256];
static int deltau2[256];
static int deltav2[256];
static int deltaa2[256];



/* Definitions governing number of bits used for crinance, cr, and cb. */

#define CR_BITS 3
#define CG_BITS 3
#define CB_BITS 2

#define CA_BITS 1

/* Masks for proper quantization of cr, cr, and cb values. */

#define CR_MASK 0xe0
#define CG_MASK 0xe0 
#define CB_MASK 0xc0

/*
 *--------------------------------------------------------------
 *
 * InitFS2FastDither --
 *
 *	Initializes structures and arrays neeeded for fast implementation
 *	of two error F-S dithering.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

void InitFS2FastDither()
{
  int i;
  int cr_num, cg_num, cb_num, ca_num;

  for (i=0; i<256; i++) {
    cr_num = (i >> (8-CR_BITS));
    cg_num = (i >> (8-CG_BITS));
    cb_num = (i >> (8-CB_BITS));
    ca_num = (i >> (8-CA_BITS));

    /* These arrays contain the error values propogated for each pixel value
       for each channel.
    */

    deltay[i] = (i - ((int) cr_values[cr_num])) / 2;
    deltau[i] = (i-((int) cg_values[cg_num])) / 2;
    deltav[i] = (i-((int) cb_values[cb_num])) / 2;
    deltaa[i] = (i-((int) ca_values[ca_num])) / 2;

    deltay2[i] = (i - ((int) cr_values[cr_num])) - deltay[i];
    deltau2[i] = (i - ((int) cg_values[cg_num])) - deltau[i];
    deltav2[i] = (i - ((int) cb_values[cb_num])) - deltav[i];
    deltaa2[i] = (i - ((int) ca_values[ca_num])) - deltaa[i];

  }

}

/*
 *--------------------------------------------------------------
 *
 * TermFS2FastDither --
 *
 *	Free structures and arrays neeeded for fast implementation
 *	of two error F-S dithering.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */



void TermFS2FastDither()
{
 
}


/*
 *--------------------------------------------------------------
 *
 * DitherImage --
 *
 *	Dithers an image using floyd-steinberg.
 *	Assumptions made:
 *	  1) The color space is allocated r:g:b = 8:8:4
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */
void
FS2FastDitherImage (unsigned char *in, unsigned char *out, int h, int w)
{
    int i, j, idx;
    int y, u, v;
    int dr, dg, db;
    int code;
    int *ye1, *ue1, *ve1;
    int *ye2, *ue2, *ve2;

    unsigned char *l,*o;

	int *yerr1=NULL;
    int *yerr2=NULL;
    int *uerr1=NULL;
    int *uerr2=NULL;
    int *verr1=NULL;
    int *verr2=NULL;

    /*  allocate error arrays. */
    yerr1 = (int *) _alloca((w+5)*sizeof(int));
    yerr2 = (int *) _alloca((w+5)*sizeof(int));
    uerr1 = (int *) _alloca((w+5)*sizeof(int));
    uerr2 = (int *) _alloca((w+5)*sizeof(int));
    verr1 = (int *) _alloca((w+5)*sizeof(int));
    verr2 = (int *) _alloca((w+5)*sizeof(int));
    
    /*
     * Init error arrays and variables.
     */
    memset ((char *)yerr1, 0, (w+5)*sizeof(int));
    memset ((char *)yerr2, 0, (w+5)*sizeof(int));
    memset ((char *)uerr1, 0, (w+5)*sizeof(int));
    memset ((char *)uerr2, 0, (w+5)*sizeof(int));
    memset ((char *)verr1, 0, (w+5)*sizeof(int));
    memset ((char *)verr2, 0, (w+5)*sizeof(int));

    dg = db = dr = 0;

	int twopass=0;		// second pass is wrong, bad colors !!!!!!!!!!!!1
    
	for (j=0; j<h; j+=1+twopass) {
	ye1 = yerr1;
	ue1 = uerr1;
	ve1 = verr1;
	ye2 = yerr2;
	ue2 = uerr2;
	ve2 = verr2;

	idx = j*w;

	l = in+idx*3;
	o = out+idx;

	/* Do the top row in forward order. */
	for (i=0; i<w; i++) {
	    /* Do left side of this pair... */
	    y = *l++ + dr + *ye1++;
	    u = *l++ + dg + *ue1++;
	    v = *l++ + db + *ve1++;

	    if (y < 0) y = 0;
	    else if (y > 255) y = 255;
	    if (u < 0) u = 0;
	    else if (u > 255) u = 255;
	    if (v < 0) v = 0;
	    else if (v > 255) v = 255;

	    /*
	     * Construct a code using:
	     *	high order 3 bits of y,
	     *	high order 2 bits of u,
	     *	high order 2 bits of v
	     */
	    code = (((y & CR_MASK) | ((u & CG_MASK) >> CR_BITS) | 
					(v >> (CR_BITS+CG_BITS)))
						>> (8-(CR_BITS+CG_BITS+CB_BITS)));
	    *o++ = code;

	    *ye2++ = deltay[y];
	    *ue2++ = deltau[u];
	    *ve2++ = deltav[v];

	    dr = deltay2[y];
	    dg = deltau2[u];
	    db = deltav2[v];
	}

	if (!twopass) continue;


	ye1 = yerr1+w-1;
	ue1 = uerr1+w-1;
	ve1 = verr1+w-1;
	ye2 = yerr2+w-1;
	ue2 = uerr2+w-1;
	ve2 = verr2+w-1;


	l += (w-1)*3;
	o += w-1;
	dr = dg = db = 0;

	/* Do bottom part of row, in right to left order. */
	for (i=w-1; i>=0; i--) {  // hg was i>0
	    /* Do right side of this pair... */
	    v = *l-- + db + *ve2--;
	    u = *l-- + dg + *ue2--;
	    y = *l-- + dr + *ye2--;

	    if (y < 0) y = 0;
	    else if (y > 255) y = 255;
	    if (u < 0) u = 0;
	    else if (u > 255) u = 255;
	    if (v < 0) v = 0;
	    else if (v > 255) v = 255;

	    /*
	     * Construct a code using:
	     *	high order 3 bits of y,
	     *	high order 2 bits of u,
	     *	high order 2 bits of v
	     */
	    code = (((y & CR_MASK) | ((u & CG_MASK) >> CR_BITS) | (v >> (CR_BITS+CG_BITS)))
		    >> (8-(CR_BITS+CG_BITS+CB_BITS)));
	    *o-- = code;
	    *ye1-- = deltay[y];
	    *ue1-- = deltau[u];
	    *ve1-- = deltav[v];
	    dr = deltay2[y];
	    dg = deltau2[u];
	    db = deltav2[v];
	}
    }
}

void
FS2FastDitherImageRGBA_8 (unsigned char *in, unsigned char *out, int h, int w, int transparentCode)
{
    int i, j, idx;
    int y, u, v, a;
    int dr, dg, db, da ;
    int code;
    int *ye1, *ue1, *ve1, *ae1;
    int *ye2, *ue2, *ve2, *ae2;

    unsigned char *l,*o;

	int *yerr1=NULL;
    int *yerr2=NULL;
    int *uerr1=NULL;
    int *uerr2=NULL;
    int *verr1=NULL;
    int *verr2=NULL;
    int *aerr1=NULL;
    int *aerr2=NULL;

    /*  allocate error arrays. */
    yerr1 = (int *) _alloca((w+5)*sizeof(int));
    yerr2 = (int *) _alloca((w+5)*sizeof(int));
    uerr1 = (int *) _alloca((w+5)*sizeof(int));
    uerr2 = (int *) _alloca((w+5)*sizeof(int));
    verr1 = (int *) _alloca((w+5)*sizeof(int));
    verr2 = (int *) _alloca((w+5)*sizeof(int));
    aerr1 = (int *) _alloca((w+5)*sizeof(int));
    aerr2 = (int *) _alloca((w+5)*sizeof(int));
    
    /*
     * Init error arrays and variables.
     */
    memset ((char *)yerr1, 0, (w+5)*sizeof(int));
    memset ((char *)yerr2, 0, (w+5)*sizeof(int));
    memset ((char *)uerr1, 0, (w+5)*sizeof(int));
    memset ((char *)uerr2, 0, (w+5)*sizeof(int));
    memset ((char *)verr1, 0, (w+5)*sizeof(int));
    memset ((char *)verr2, 0, (w+5)*sizeof(int));
    memset ((char *)aerr1, 0, (w+5)*sizeof(int));
    memset ((char *)aerr2, 0, (w+5)*sizeof(int));

    dg = db = dr = da = 0;

	int twopass=0;		// second pass is wrong, bad colors !!!!!!!!!!!!1
    
	for (j=0; j<h; j+=1+twopass) {
	ye1 = yerr1;
	ue1 = uerr1;
	ve1 = verr1;
	ae1 = aerr1;
	ye2 = yerr2;
	ue2 = uerr2;
	ve2 = verr2;
	ae2 = aerr2;

	idx = j*w;

	l = in+idx*4;
	o = out+idx;

	/* Do the top row in forward order. */
	for (i=0; i<w; i++) {
	    /* Do left side of this pair... */
	    y = *l++ + dr + *ye1++;
	    u = *l++ + dg + *ue1++;
	    v = *l++ + db + *ve1++;

		a = *l++ + da + *ae1++;

	    if (y < 0) y = 0;
	    else if (y > 255) y = 255;
	    if (u < 0) u = 0;
	    else if (u > 255) u = 255;
	    if (v < 0) v = 0;
	    else if (v > 255) v = 255;
	    if (a < 0) a = 0;
	    else if (a > 255) a = 255;

		if (a<128) 
			code = transparentCode;
		else {
	    /*
	     * Construct a code using:
	     *	high order 3 bits of y,
	     *	high order 2 bits of u,
	     *	high order 2 bits of v
	     */
	    code = (((y & CR_MASK) | ((u & CG_MASK) >> CR_BITS) | 
					(v >> (CR_BITS+CG_BITS)))
						>> (8-(CR_BITS+CG_BITS+CB_BITS)));
		}

		*o++ = code;

	    *ye2++ = deltay[y];
	    *ue2++ = deltau[u];
	    *ve2++ = deltav[v];
	    *ae2++ = deltaa[a];

	    dr = deltay2[y];
	    dg = deltau2[u];
	    db = deltav2[v];
	    da = deltaa2[a];

	}

	if (!twopass) continue;


	ye1 = yerr1+w-1;
	ue1 = uerr1+w-1;
	ve1 = verr1+w-1;
	ae1 = aerr1+w-1;

	ye2 = yerr2+w-1;
	ue2 = uerr2+w-1;
	ve2 = verr2+w-1;
	ae2 = aerr2+w-1;


	l += (w-1)*4;
	o += w-1;
	dr = dg = db = da = 0;

	/* Do bottom part of row, in right to left order. */
	for (i=w-1; i>=0; i--) {  // hg was i>0
	    /* Do right side of this pair... */
	    v = *l-- + db + *ve2--;
	    u = *l-- + dg + *ue2--;
	    y = *l-- + dr + *ye2--;
	    a = *l-- + da + *ae2--;

	    if (y < 0) y = 0;
	    else if (y > 255) y = 255;
	    if (u < 0) u = 0;
	    else if (u > 255) u = 255;
	    if (v < 0) v = 0;
	    else if (v > 255) v = 255;
	    if (a < 0) a = 0;
	    else if (a > 255) a = 255;

		if (a<128) 
			code = transparentCode;
		else {

	    /*
	     * Construct a code using:
	     *	high order 3 bits of y,
	     *	high order 2 bits of u,
	     *	high order 2 bits of v
	     */
	    code = (((y & CR_MASK) | ((u & CG_MASK) >> CR_BITS) | (v >> (CR_BITS+CG_BITS)))
		    >> (8-(CR_BITS+CG_BITS+CB_BITS)));
		}

	    *o-- = code;
	    *ye1-- = deltay[y];
	    *ue1-- = deltau[u];
	    *ve1-- = deltav[v];
	    *ae1-- = deltaa[a];

	    dr = deltay2[y];
	    dg = deltau2[u];
	    db = deltav2[v];
	    da = deltaa2[v];
	}
    }
}





/* convert RGB image to FS dither mapped image */
int image_convert_image_mapped8(image *im /*,useRGB8Pal*/)
{	int ret;

	if (im->fmt == IM_MAPPED8) return(0); // already has palette
	gbool useRGB8Pal = TRUE; // use FS 332 color table 

	if (!useRGB8Pal && im->fmt == IM_GRAY) {
		if (im->cmap == NULL) {
		  ret = image_alloc_cmap(im,256);
		  for (int i=0; i<256;i++) image_set_cmap(im,i,i,i,i);
		}	
		im->fmt = IM_MAPPED8;
		return(1);
    }
	else if (!useRGB8Pal && im->fmt == IM_GRAYA) { // strip alpha
		image irgb;
		if (im->cmap == NULL) {
		  ret = image_alloc_cmap(im,256);
		  for (int i=0; i<256;i++) image_set_cmap(im,i,i,i,i);
		}	
		image_zero(&irgb);
	    ret=image_alloc(&irgb,IM_GRAY,im->width,im->height);
		if (ret <0) return ret;
	    ret=image_graya_to_gray(im,&irgb);
		image_free(im);

		im->image = irgb.image;
		image_set_fmt(im,irgb.fmt,irgb.width,irgb.height);
		irgb.image = NULL;
		image_free_all(&irgb);
		im->fmt = IM_MAPPED8;
		return(1);
    }

	if (im->fmt == IM_GRAYA) { // GRAYA ==> RGBA 
		ret=image_convert_to_rgba(im);
		if (ret<0) return (ret);
	}

	if (im->fmt == IM_RGBA) {
			// OK 
		useRGB8Pal = TRUE;
	}
	else if (im->fmt != IM_RGB) {
		
		ret=image_convert_to_rgb(im);
		
		if (ret<0) return (ret);
		
		if (im->fmt != IM_RGB) return(IME_NOTIMP);

	}


	   /* dither to mapped 8 */
//	   SetCRange(8,8,4);

	   IBYTE *imageb = (IBYTE *) malloc(im->width* im->height);

	   if (!imageb) { 
		   ret = IME_NOMEM;
  		   return(ret);
	   }

#ifdef _D3D
	// try first if image only contains up to 256 distinct colors
	if (!useRGB8Pal) {
	IBYTE *src = im->image;
	IBYTE *dest = imageb;

	int color_count = 0;    /* number of colors in image*/
	int width = im->width; 
	int height = im->height;
    D3DCOLOR colors[256];
    D3DCOLOR c;


    
	for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int r,g,b,k;
			// get the pixel
            r = *src++;
            g = *src++;
            b = *src++;
            c = RGB_MAKE(r, g, b);

            /*
             * Search for this color in a table of colors              */
            for (k = 0; k < color_count; k++)
                if (c == colors[k]) break;
			if (k == color_count) {
					/*
					* This is a new color, so add it to the list
					*/
					color_count++;
					/*
					* More than 256 and we fail (8-bit) 
					*/
					if (color_count > 256) {
	                    goto dither;
		            }
			        colors[k] = c;
			}
            *dest++=k;
            }
	   }
	
	   // if we come here, it fits !!!!!!!!!!!1
	   // create color table 
	   image_alloc_cmap(im,color_count);
	   for (int i=0; i<color_count;i++) {
		   im->cmap[i].r= RGB_GETRED(colors[i]);
		   im->cmap[i].g= RGB_GETGREEN(colors[i]);
		   im->cmap[i].b= RGB_GETBLUE(colors[i]);
	   }

	   /* replace image */
	   //free(im->image);
	   image_free(im);
	   im->image = imageb; imageb = NULL;
	   ret=image_set_fmt(im,IM_MAPPED8,im->width,im->height);
	   return(1);
    }


dither:
#endif

	   InitColor();
	   InitFS2FastDither();
	   if (im->fmt == IM_RGBA) {
		  // use black as colorkey 
		  FS2FastDitherImageRGBA_8 (im->image, imageb, im->height, im->width,0);
		  im->colorKey=1;
		  im->colorKeyIndex = 0;

		  im->colorKeyRgb.r = 0;
		  im->colorKeyRgb.g = 0;
		  im->colorKeyRgb.b = 0;

	   } else FS2FastDitherImage (im->image, imageb, im->height, im->width);

	   TermFS2FastDither();

	    // InitColormap(void)
  	    int ncolors = CR_RANGE*CG_RANGE*CB_RANGE;
  		// int i, cr_num, cg_num, cb_num;
  		// unsigned char 
		int r, g, b;

		image_alloc_cmap(im,ncolors);

		int d= 255;
		//int r,g,b;
		for (r=0; r< CR_RANGE; r++)
		for (g=0; g< CG_RANGE; g++)
		for (b=0; b< CB_RANGE; b++)	{
			int i =  (r << (CB_BITS + CG_BITS)) | (g<<CB_BITS) | b;
//			im->cmap[i].r=(r*d+2) / 7, im->cmap[i].g=(g*d+2) / 7 , im->cmap[i].b=(b*d+1)/3;
			im->cmap[i].r=cr_values[r], im->cmap[i].g=cg_values[g] , im->cmap[i].b= cb_values[b];
		}

	   TermColor();


	   /* replace image */
	   image_free(im);
	   im->image = imageb; imageb = NULL;
	   ret=image_set_fmt(im,IM_MAPPED8,im->width,im->height);

 	return(ret);
}










