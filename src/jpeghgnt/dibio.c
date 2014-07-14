/*
  MS-WINDOW DIB BMP Files in/out

  Holger Grahn
  Changes:
     29.06.94  dib_write : rows on 4 byte boundary

     need to change RGB to BGR in DIB-FILE

*/

#include <windows.h>

#include <stdio.h>
#include <stdlib.h>

#include "image.h"
#include "dibio.h"

/* #define TEST, simple test shell  */


#if 0
/* from windef.h */

#define near
#define far
#define NEAR
#define FAR

#ifndef BASETYPE_ULONG
#define BASETYPE_ULONG
typedef unsigned long ULONG;
typedef ULONG *PULONG;
#endif

/* BASETYPES is defined in ntdef.h if these types are already defined */

#ifndef BASETYPES
#define BASETYPES
typedef unsigned short USHORT;
typedef USHORT *PUSHORT;
typedef unsigned char UCHAR;
typedef UCHAR *PUCHAR;
typedef char *PSZ;
#endif	/* !BASETYPES */

typedef long		    LONG;

typedef unsigned long	    DWORD;
typedef int		    BOOL;
typedef unsigned char	    BYTE;
typedef unsigned short	    WORD;
typedef float		    FLOAT;
typedef FLOAT		    *PFLOAT;
typedef BOOL near	    *PBOOL;
typedef BOOL far	    *LPBOOL;
typedef BYTE near	    *PBYTE;
typedef BYTE far	    *LPBYTE;
typedef int near	    *PINT;
typedef int far 	    *LPINT;
typedef WORD near	    *PWORD;
typedef WORD far	    *LPWORD;
typedef long far	    *LPLONG;
typedef DWORD near	    *PDWORD;
typedef DWORD far	    *LPDWORD;
typedef void far	    *LPVOID;

typedef int		    INT;
typedef unsigned int	    UINT;
typedef unsigned int	    *PUINT;


/* from wingdi.h */

/* Bitmap Header Definition */
typedef struct tagBITMAP
  {
    LONG	bmType;
    LONG	bmWidth;
    LONG	bmHeight;
    LONG	bmWidthBytes;
    WORD	bmPlanes;
    WORD	bmBitsPixel;
    LPVOID	bmBits;
  } BITMAP, *PBITMAP, NEAR *NPBITMAP, FAR *LPBITMAP;

typedef struct tagRGBTRIPLE {
	BYTE	rgbtBlue;
	BYTE	rgbtGreen;
	BYTE	rgbtRed;
} RGBTRIPLE;

typedef struct tagRGBQUAD {
	BYTE	rgbBlue;
	BYTE	rgbGreen;
	BYTE	rgbRed;
	BYTE	rgbReserved;
} RGBQUAD;

/* structures for defining DIBs */
typedef struct tagBITMAPCOREHEADER {
	DWORD	bcSize; 		/* used to get to color table */
	WORD	bcWidth;
	WORD	bcHeight;
	WORD	bcPlanes;
	WORD	bcBitCount;
} BITMAPCOREHEADER, FAR *LPBITMAPCOREHEADER, *PBITMAPCOREHEADER;


typedef struct tagBITMAPINFOHEADER{
	DWORD	   biSize;
	LONG	   biWidth;
	LONG	   biHeight;
	WORD	   biPlanes;
	WORD	   biBitCount;
	DWORD	   biCompression;
	DWORD	   biSizeImage;
	LONG	   biXPelsPerMeter;
	LONG	   biYPelsPerMeter;
	DWORD	   biClrUsed;
	DWORD	   biClrImportant;
} BITMAPINFOHEADER, FAR *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;



/* constants for the biCompression field */
#define BI_RGB	      0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_TOPDOWN    4L
    /*!!! This should be deleted, I only leave
      !!! it temp till Insignia changes there code !!! remove by 10/1/92 [patrickh] */

typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER	bmiHeader;
    RGBQUAD		bmiColors[1];
} BITMAPINFO, FAR *LPBITMAPINFO, *PBITMAPINFO;

typedef struct tagBITMAPCOREINFO {
    BITMAPCOREHEADER	bmciHeader;
    RGBTRIPLE		bmciColors[1];
} BITMAPCOREINFO, FAR *LPBITMAPCOREINFO, *PBITMAPCOREINFO;

#pragma pack(2)
typedef struct tagBITMAPFILEHEADER {
	WORD	bfType;
	DWORD	bfSize;
	WORD	bfReserved1;
	WORD	bfReserved2;
	DWORD	bfOffBits;
} BITMAPFILEHEADER, FAR *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;
#pragma pack()

#endif

int swap_bitmapinfoheader(BITMAPINFOHEADER *p)
{
    return;
    IFLIPL(p->biSize);
    IFLIPL(p->biWidth);
    IFLIPL(p->biHeight);
    IFLIPS(p->biPlanes);
    IFLIPS(p->biBitCount);
    IFLIPL(p->biCompression);
    IFLIPL(p->biSizeImage);
    IFLIPL(p->biXPelsPerMeter);
    IFLIPL(p->biYPelsPerMeter);
    IFLIPL(p->biClrUsed);
    IFLIPL(p->biClrImportant);
}


static void ErrorOut(const char *errstring)
{
DWORD Error=0;

 fprintf(stderr,"Error on %s = %d\n", errstring, Error);
}


/******************************Module*Header*******************************\
* Module Name: loadbmp.c
*
* Contains function that loads a bitmap file
*
*
\**************************************************************************/
/**************************************************************************\
*
*	   The DIB file format contains the BITMAPFILEHEADER then followed
* immediately by the BITMAPINFOHEADER.	Notice that the BITMAPFILEHEADER is
* not DWORD aligned.  Thus, the structure that follows it, the
* BITMAPINFOHEADER is not on a DWORD boundary.
*
*
\**************************************************************************/

/******************************Public*Routine******************************\
*
* LoadBitmapFile
*
* Effects:  Loads the bitmap from file and put into im
*
* Warnings: FileName contains the full path
*
\**************************************************************************/

int dib_read(image *im, const char *FileName)
{
    int Success;

    FILE  *hFile;

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bmh;

    LPBITMAPINFOHEADER pbmh;

    int 	debug;
    int		ncolorsleft;
    long		extrascanbytes;

    debug = 2;	 /* HG */
    debug = 0;
    Success = 0;

    if ((hFile = fopen(FileName,"rb")) == NULL) {
	ErrorOut("dib_read:Fail in file open");
	Success = IME_NOFILE;
	goto ErrExit1;
    }

    if (fread( (char *) &bfh,sizeof(bfh),1,hFile) != 1)
    {
	ErrorOut("dib_read:error reading file header");
	Success = IME_BADFILE;
	goto ErrExit3;
    }
    /*	First check that it is a bitmap file */

    if (bfh.bfType != 0x4d42) { 	     /* 'BM' */
	ErrorOut("dib_read:This is not a DIB bitmap file!");
	Success = IME_BADFILE;
	goto ErrExit3;
    }

    if (fread( (char *) &bmh,sizeof(bmh),1,hFile) != 1)
    {
	ErrorOut("dib_read:error reading bitmap info header");
	Success = IME_BADFILE;
	goto ErrExit3;
    }

    swap_bitmapinfoheader(&bmh);

    pbmh = &bmh;
    if (debug) {
	printf("BMP Type = %d,Size = %ld, OffBits=%ld \n",bfh.bfType,bfh.bfSize,bfh.bfOffBits);
	printf("Size = %ld,Width = %ld, Height = %ld, Planes =%d, BitCount = %d \n",
	       bmh.biSize,bmh.biWidth,bmh.biHeight,bmh.biPlanes,bmh.biBitCount);
	printf("Compression= %ld, Size Image = %ld, Xres = %ld, Yres=%ld, ClrUsed = %ld, ClrImportant = %ld \n",
	       bmh.biCompression,bmh.biSizeImage,bmh.biXPelsPerMeter,bmh.biYPelsPerMeter,bmh.biClrUsed,bmh.biClrImportant);
	printf("Sizeof bitmap header %d, core info %d, info = %d\n",  sizeof(BITMAPFILEHEADER),sizeof(BITMAPCOREHEADER),sizeof(BITMAPINFOHEADER));
	printf("Bytes left after header = %d\n",bfh.bfOffBits -sizeof(BITMAPFILEHEADER) -bmh.biSize);
    }

    im->width = bmh.biWidth;
    im->height = bmh.biHeight;

    ncolorsleft = (bfh.bfOffBits -sizeof(BITMAPFILEHEADER) -bmh.biSize) / sizeof(RGBQUAD);

    if (bmh.biPlanes != 1) {
	  Success = IME_NOTIMP;
	  goto ErrExit3;
    }

    if (bmh.biCompression != BI_RGB) {
	  Success = IME_NOTIMP;
	  goto ErrExit3;
    }

    switch (bmh.biBitCount) {
    case 1 : im->fmt = IM_BW; break;
    case 4 : im->fmt = IM_MAPPED4; break;
    case 8 : im->fmt = IM_MAPPED8; break;
    case 16: im->fmt = IM_RGB15; break;
    case 24: im->fmt = IM_RGB24; break;
    case 32: im->fmt = IM_RGB32; break;
    default :
	  Success = IME_NOTIMP;
	  goto ErrExit3;
    }

    if (pbmh->biSize == sizeof(BITMAPCOREHEADER))
    {
      printf("BITMAPCOREHEADER\n");
      Success = IME_NOTIMP;
      goto ErrExit3;
    }

    /* some bitmaps dosn't seem to store this field properly */

    if ((pbmh->biClrUsed ==0) && (ncolorsleft >0) && (pbmh->biBitCount <= 8)) {
	pbmh->biClrUsed = ncolorsleft;
    }

    /* read color map */

    if (pbmh->biClrUsed >0) {
       /* encountered this with scanjet images ??? with 24 bits per pixel format */
       RGBQUAD rgb;
       int i;

       image_alloc_cmap(im,pbmh->biClrUsed);
       for (i=0; i< pbmh->biClrUsed; i++) {
	  fread((char *) &rgb,1,sizeof(RGBQUAD),hFile);
	  image_set_cmap(im,i,rgb.rgbRed,rgb.rgbGreen,rgb.rgbBlue);
	  if (debug>1)
	    printf(" # %d (%d %d %d) \n",i,rgb.rgbRed,rgb.rgbGreen,rgb.rgbBlue);
       }
    }

    if ((im->fmt == IM_BW) && (!(im->supported_fmts & (1<<IM_BW)))) {
	Success=image_alloc(im,IM_MAPPED8,im->width,im->height);
	image_set_fmt(im,IM_BW,im->width,im->height);
    }
    else
    if ((im->fmt == IM_MAPPED4) && (!(im->supported_fmts & (1<<IM_MAPPED4)))) {
	Success=image_alloc(im,IM_MAPPED8,im->width,im->height);
	image_set_fmt(im,IM_MAPPED4,im->width,im->height);
    }
    else Success=image_alloc(im,im->fmt,im->width,im->height);

    if (Success<0) goto ErrExit3;

    if (bmh.biSizeImage ==0) bmh.biSizeImage =im->bytesperimage;

    extrascanbytes=bmh.biSizeImage-im->bytesperimage;
    if (extrascanbytes>0) {
if (debug) printf("Extrascanbytes : %d \n",extrascanbytes,extrascanbytes / bmh.biHeight);
	extrascanbytes /= bmh.biHeight;
    }

    fseek(hFile,bfh.bfOffBits,SEEK_SET);

    /* encountered to big biSizeImage Fields from  HP scanjet */
    /* fread( (char *) im->image,1,bmh.biSizeImage,hFile); */
    if (extrascanbytes >0) {
	int y;
	for (y=0; y < bmh.biHeight; y++) {
	  fread( (char *) IROW(im,y),1,im->bytesperscan,hFile);
	  fseek(hFile,extrascanbytes,SEEK_CUR); /* skip alignment bytes */
	}

    }
    else fread( (char *) im->image,1,im->bytesperimage,hFile);

    if ((im->fmt == IM_BW) && (!(im->supported_fmts & (1<<IM_BW)))) {
	if (debug)
	   printf("Expanding image from bw to mapped 8 \n");

	packed_to_mapped8_rev(im->image,im->bytesperimage,im->image);
	image_set_fmt(im,IM_MAPPED8,im->width,im->height);
	image_alloc_cmap(im,2);
	image_set_cmap(im,0, 0,0,0);
	image_set_cmap(im,1, 255,255,255);
    }
    else
    if ((im->fmt == IM_MAPPED4) && (!(im->supported_fmts & (1<<IM_MAPPED4)))) {
	if (debug)
	   printf("Expanding image from mapped 4 to mapped 8 \n");
	 mapped4_to_mapped8_rev(im->image,im->bytesperimage,im->image);
	image_set_fmt(im,IM_MAPPED8,im->width,im->height);
    }

ErrExit3:


ErrExit2:
    fclose(hFile);
ErrExit1:

    return (Success);
}


/******************************Module*Header*******************************\
* Module Name: savebmp.c
*
*
*
\**************************************************************************/

/******************************Public*Routine******************************\
* SaveBitmapFile
*
*
\**************************************************************************/

int dib_write(image *im,const char *FileName)
{
    FILE	*hFile;
    int		Success;
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bmh;

    int cmapcolors; /* colormap entrys to write */
    int debug = 0;

    long extrascanbytes = 0;
    Success = 0;

    bmh.biSize = sizeof(BITMAPINFOHEADER);

    bmh.biWidth = im->width;
    bmh.biHeight = im->height;
    bmh.biSizeImage = im->bytesperimage;
    bmh.biPlanes = 1;
    bmh.biBitCount =  im->bytesperpixel*8;
    bmh.biCompression = BI_RGB;
    bmh.biXPelsPerMeter =0;
    bmh.biYPelsPerMeter =0;
    bmh.biClrUsed =0;
    bmh.biClrImportant=0;

    /* rows should be aligned on long boundary */
    if ((im->bytesperscan % 4) > 0) {
       extrascanbytes = im->bytesperscan % 4;
       bmh.biSizeImage = (im->bytesperscan + extrascanbytes) *im->height;
       if (debug) printf("Extrascanbytes = %d \n",extrascanbytes);
    }

    switch (im->fmt) {
    case IM_BW : bmh.biBitCount = 1; break;
    case IM_MAPPED4 : bmh.biBitCount = 4; break;
    default: break;
    }

    cmapcolors = im->ncolors * (im->cmap != NULL);

    bmh.biClrUsed = cmapcolors;

    /*	fill in the info for the BitmapFileHeader */

    bfh.bfType = 0x4D42;			    /* 'BM' */
    bfh.bfSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
	+sizeof(RGBQUAD)*cmapcolors
	+bmh.biSizeImage;
    bfh.bfReserved1 =
    bfh.bfReserved2 = 0;
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
	sizeof(RGBQUAD)*cmapcolors;

    if ((hFile = fopen(FileName,"wb")) == NULL) {
	ErrorOut("dib_write:Fail in file open");
	Success = IME_NOFILE;
	goto ErrExit1;
    }

    /* Write out the file header now */
/*    SWAP_BITMAPFILEHEADER(bfh); */

    if (fwrite( (char *) &bfh,sizeof(bfh),1,hFile) != 1)
    {
	ErrorOut("dib_write:error writing file header");
	Success = IME_BADFILE;
	goto ErrExit3;
    }


    if (debug) {
	printf("BMP Type = %d,Size = %ld, OffBits=%ld \n",bfh.bfType,bfh.bfSize,bfh.bfOffBits);
	printf("Size = %ld,Width = %ld, Height = %ld, Planes =%d, BitCount = %d \n",
	       bmh.biSize,bmh.biWidth,bmh.biHeight,bmh.biPlanes,bmh.biBitCount);
	printf("Compression= %ld, Size Image = %ld, Xres = %ld, Yres=%ld, ClrUsed = %ld, ClrImportant = %ld \n",
	       bmh.biCompression,bmh.biSizeImage,bmh.biXPelsPerMeter,bmh.biYPelsPerMeter,bmh.biClrUsed,bmh.biClrImportant);
	printf("Sizeof bitmap header %d, core info %d, info = %d\n",  sizeof(BITMAPFILEHEADER),sizeof(BITMAPCOREHEADER),sizeof(BITMAPINFOHEADER));
	printf("Bytes left after header = %d\n",bfh.bfOffBits -sizeof(BITMAPFILEHEADER) -bmh.biSize);
    }


    /* Now write out the BitmapInfoHeader and color table, if any */
    swap_bitmapinfoheader(&bmh);

    if (fwrite( (char *) &bmh,sizeof(bmh),1,hFile) != 1)
    {
	ErrorOut("dib_write:error writing bitmap info header");
	Success = IME_FILEIO;
	goto ErrExit3;
    }

    swap_bitmapinfoheader(&bmh);

    /* write color map */

    if (bmh.biClrUsed >0) {
       RGBQUAD rgb;
       int i,r,g,b;

       rgb.rgbReserved = 0x0;

       for (i=0; i< bmh.biClrUsed; i++) {
	  image_get_cmap(im,i,&r,&g,&b);
	     rgb.rgbRed=r,rgb.rgbGreen=g,rgb.rgbBlue=b;

	  fwrite((char *) &rgb,1,sizeof(RGBQUAD),hFile);
	  if (debug>1)
	    printf(" # %d (%d %d %d) \n",i,rgb.rgbRed,rgb.rgbGreen,rgb.rgbBlue);
       }
    }




    /* write the bits also */
    if (extrascanbytes >0) {
	int y;
	IBYTE extra[4];
	int err;
	extra[0]=extra[1]=extra[2]=extra[3]=0;

	for (y=0; y < im->height; y++) {
	  err= (fwrite( (char *) IROW(im,y),1,im->bytesperscan,hFile) != im->bytesperscan);
	  if (!err) err = (fwrite( (char *) &extra[0],1,extrascanbytes,hFile)!=extrascanbytes);
	  if (err) {
	     ErrorOut("dib_write:error writing data");
	     Success = IME_FILEIO;
	     goto ErrExit3;
	  }

	}
    }
    else  {
    if (fwrite((char*) im->image,1,im->bytesperimage,hFile) != im->bytesperimage) {
	ErrorOut("dib_write:error writing data");
	Success = IME_FILEIO;
	goto ErrExit3;
    }
    }

ErrExit4:
ErrExit3:
    fclose(hFile);
ErrExit2:
ErrExit1:
    return Success;
}

#ifdef TEST

int main(int argc,char * argv[])
{ image im;
  int ret;
  image_zero(&im);
  if (argc>1)
   {

     ret=dib_read(&im,argv[1]);
     printf("Dib_read : Return = %d \n",ret);

   }
  return(0);
}

#endif
