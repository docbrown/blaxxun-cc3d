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
#ifndef _IMAGE_H
#define _IMAGE_H

#ifdef _DEBUG
#ifdef WIN32

// MS-VC Runtime MEM debugging
#include <crtdbg.h>

#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

/*
  generic image storage object
*/

#define IBITSET(v, thebit)	(((v) & (1<<thebit)) != 0)
/* word long flipping */
#define IFLIPS(A) { A = ( (unsigned short)(A) >> 8) | (((unsigned short)(A) & 0XFF) << 8); }
#define IFLIPEDS(A) (( (unsigned short)(A) >> 8) | (((unsigned short)(A) & 0XFF) << 8))
#define IFLIPL(A) { A = ( ((unsigned long)(A) >> 24) | (((unsigned long)(A) >> 8) & 0X00FF00l)	\
	| (((unsigned long)(A) << 8) & 0X00FF0000l) | (((unsigned long)(A) << 24) & 0XFF000000l)); }


/* out of error */
#define IME_ERROR -1
#define IME_NOMEM -2
#define IME_BADFMT -3
#define IME_INCOMPATIBLEFMT -4
#define IME_BADFILE -5
#define IME_NOFILE -6
#define IME_FILEIO -7
#define IME_NOTIMP -8
#define IME_NOCMAP -9
#define IME_BADIMAGE -10
#define IME_RANGE -11
#define IME_BADOP -12




/* image format types */
#define IM_BW 1
#define IM_GRAY 2
#define IM_MAPPED4 3
#define IM_MAPPED8 4
#define IM_MAPPED16 5
#define IM_RGB15 6
#define IM_RGB16 7
#define IM_RGB24 8
#define IM_RGB 8
#define IM_RGB32 9
#define IM_RGBA 10

#define IM_RGB_SHORT 11
#define IM_RGBA_SHORT 12
#define IM_GRAYA 13 


#define HOOPS_IM_FORMATS ((1<<IM_MAPPED8) | (1<<IM_MAPPED16) | (1<<IM_RGB24) ) | (1<<IM_RGBA) )

#define IM_NO_COMPRESSION 0
#define IM_RLE_COMPRESSION 1

#define IBYTE unsigned char
#define IWORD unsigned short
#define ISHORT short
#define ILONG unsigned long

/* alpha ops */
enum IMAGE_OP {
	IMOP_NOOP,
	IMOP_SETR,
	IMOP_SETG,
	IMOP_SETB,
	IMOP_SETA,
	IMOP_SETRGB,
	IMOP_SETRGBA,
	IMOP_SETA_IF_RGBEQ,
	IMOP_RGB_IF_AEQ,

	IMOP_COPY_DEST,
	IMOP_COPY_SRC,
	IMOP_COPY_SRC_CLAMP,
	IMOP_ADD,
	IMOP_SUB,
	IMOP_MUL,
	IMOP_DIV,
	IMOP_MIN,
	IMOP_MAX,
	IMOP_ADDC,
	IMOP_SUBC,
	IMOP_MULC,
	IMOP_DIVC
	};





/* for colormap entries */
typedef struct {
	IBYTE r,g,b,a;
	} irgb_color;

typedef struct {
	float r,g,b,a;
	} frgb_color;

typedef struct {
	ISHORT r,g,b,a;
	} irgb_short_color;

/* as pixel types */
typedef struct {
	IBYTE r,g,b;
	} irgb_pixel;

typedef struct {
	IBYTE r,g,b,a;
	} irgba_pixel;




typedef struct image_ image;

typedef struct image_methods_ {
	int (*query) (image *im,char *filename,char *options);
	int (*open)  (image *im);
	int (*close) (image *im);

	int (*putrows) (image *im,int y1,int nrows,IBYTE *data);
	int (*getrows) (image *im,int y1,int nrows,IBYTE *data);

	} image_methods;

struct image_ {
	IBYTE *image;	   /* image data */
	int width,height;  /* width and hight of image */

	int imageByRef;	  /* flag, its data by ref */ 
	
	char fmt;		  /* format layout of image */
	char compression;  /* compression 0 = uncompressed, 1= RLE */

	int ncolors;	   /* number of colors for color mapped images */
	irgb_color *cmap;  /* color map for mapped images */
	frgb_color *fcmap; /* float color map */

	int		   colorKey;     /* flag has color key */	
	irgb_pixel colorKeyRgb; /* color key in RGB */ 
	int		   colorKeyIndex; /* color key index */ 

	int		   backgroundIndex; // index of background color
	int		   isGray;     /* flag is or was this image a gray scale image ? */	


	int bytesperpixel;	  /* bytes per pixel */
	int bytesperscan;	  /* bytes per scanline */
	long bytesperimage;	  /* bytes for image */
	long compressed_bytesperimage;	  /* bytes for image if compressed */

	/* misc infos, dependend of routines */
	long supported_fmts;	  /* supported formats, 1 bit per FMT */

	int error;		  /* current error */

#if 0
	long key;		  /* HOOPS image key */
	long segkey;		  /* HOOPS segment key */
	char *filename;		  /* file name */
	char *name;		  /* image name */
	char *comment;		  /* image comment */
	FILE *file;		  /* file */
	void *data;
#endif
	image_methods m;	  /* methods */
	};

/* pointer adressing */


#define IROW(im,Y) ((im)->image+(Y)*(im)->bytesperscan)

#define IPIX(im,X,Y) ((im)->image+(Y)*(im)->bytesperscan+ (X) * (im)->bytesperpixel)


// prototypes 

extern const char * get_image_error_message(
	int im_err);

extern void image_zero(
	image * im);

extern int image_alloc(
	image * im,
	int fmt,
	int width,
	int height);

extern int image_set_fmt(
	image * im,
	int fmt,
	int width,
	int height);

extern int image_set_fmt_by_ref(
	image * im,
	int fmt,
	int width,
	int height,
	IBYTE *imageData	   /* image data */
	);


extern void image_free(
	image * im);

extern int image_set_subimage(
	image * im,
	int x,
	int y,
	int width,
	int height,
	image * subim);


extern int image_is_gray(
	image * im
	);

extern int image_alloc_fcmap(
	image * im,
	int ncolors);

extern void image_free_fcmap(
	image * im);

extern int image_set_fcmap(
	image * im,
	int i,
	float r,
	float g,
	float b);

extern int image_alloc_cmap(
	image * im,
	int ncolors);

extern void image_free_cmap(
	image * im);

extern int image_set_cmap(
	image * im,
	int i,
	int r,
	int g,
	int b);

extern int image_get_cmap(
	image * im,
	int i,
	int * r,
	int * g,
	int * b);

extern void image_free_all(
	image * im);

extern int image_itof_cmap(
	image * im);

extern int image_ftoi_cmap(
	image * im);

extern int image_copy_cmap(
	const struct image_ * imsrc,
	image * im);

extern int image_scale_copy(
	image * iin,
	image * iout);

extern int image_scale_copy_filtered(
	image *iin,
	image *iout);


extern int image_vflip(
	image * iin,
	image * iout);

extern int image_mapped8_to_rgb(
	image * iin,
	image * iout);

extern int image_mapped8_to_rgba(image *iin,
	IBYTE alphaColor, 
	IBYTE alpha1, IBYTE alpha2, 
	image *iout);


extern int image_rgb_to_rgba(
	image * iin,
	image * iout,
	unsigned char alpha);

extern int image_rgba_to_rgb(image *iin,image *iout);


extern int image_graya_to_rgba(
	image * iin,
	image * iout
	);

extern int image_graya_to_rgb(
	image * iin,
	image * iout
	);

extern int image_graya_to_gray(image *iin,image *iout);




extern void mapped4_to_mapped8(
	unsigned char * a,
	int cnt,
	unsigned char * b);

extern void mapped4_to_mapped8_rev(
	unsigned char * a,
	int cnt,
	unsigned char * b);

extern void mapped8_to_mapped4(
	unsigned char * a,
	int cnt,
	unsigned char * b);

extern void packed_to_mapped8(
	const unsigned char * a,
	int cnt,
	unsigned char * b);

extern void packed_to_mapped8_rev(
	const unsigned char * a,
	int cnt,
	unsigned char * b);

extern int image_packed_to_mapped8(
	image * iin,
	image * iout);

extern int pack_byte_run_length(
	int cols,
	const unsigned char * pixel,
	unsigned char * dest_start);

extern int unpack_byte_run_length(
	int cols,
	const unsigned char * src,
	unsigned char * dest_start);

extern int pack_rgb_run_length(
	int cols,
	irgb_pixel * pixel,
	unsigned char * dest_start);

extern int unpack_rgb_run_length(
	int cols,
	const unsigned char * src,
	irgb_pixel * dest_start);

extern int pack_word_run_length(
	int cols,
	const unsigned short * pixel,
	unsigned char * dest_start);

extern int unpack_word_run_length(
	int cols,
	const unsigned char * src,
	unsigned short * dest_start);

extern int pack_long_run_length(
	int cols,
	const unsigned long * pixel,
	unsigned char * dest_start);

extern int unpack_long_run_length(
	int cols,
	const unsigned char * src,
	unsigned long * dest_start);

extern int image_pack_rle(
	image * iin,
	image * iout);

extern int image_unpack_rle(
	image * iin,
	image * iout);

extern int image_get_rgb_color(
	image * im,
	int x,
	int y,
	irgb_color * p);

extern int image_alpha_op(
	image * im,
	int op,
	irgb_color * p);

extern int image_channel_map(image *im,IBYTE *map,IBYTE *channel);

extern int image_rgb_swap(image *im);

/*
  Copy image subrectangle

  iout[xdest:width,ydest:height] = iin[xstart:width,ystart:height]
  iout + iin should have same depth
  no error checking !

  color key support if MAPPED8 format
*/

extern int image_copy(	
	image *iin, // src image
	int xstart,int ystart, int width, int height,
	image *iout, // destination image
	int xdest,int ydest,
	
	int useCkey // don"t write transparent pixels of iin
	);


/*
 set subrectangle
 to constant color
*/


extern int image_set_8(
	image *iout,	// destination image
	int xdest,int ydest, // position
	int width, int height, // size
	IBYTE color	// color
	);


#ifdef __cplusplus
}
#endif

#endif


