/* tgaio.c - write and read rgb images to/form a TrueVision Targa file
   extracted from pbmplus Toolkit 10 dec 91
   Holger Grahn, Tech Soft GmbH

   Warning : don't know if it's save to cast (unsigned char *) to  *pixel struct
   on all systems. sizeof(pixel) must be 3.

   To do:
      optimize ?
      support for color mapped images / gray images
      errors are written to stderr

*/

/* ppmtotga.c - read a portable pixmap and produce a TrueVision Targa file
**
** Copyright (C) 1989, 1991 by Mark Shand and Jef Poskanzer
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose and without fee is hereby granted, provided
** that the above copyright notice appear in all copies and that both that
** copyright notice and this permission notice appear in supporting
** documentation.  This software is provided "as is" without express or
** implied warranty.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"

#define ARGS(X) X

/* tga.h - header file for Targa files
*/

/* Header definition. */
struct ImageHeader {
    unsigned char IDLength;		/* length of Identifier String */
    unsigned char CoMapType;		/* 0 = no map */
    unsigned char ImgType;		/* image type (see below for values) */
    unsigned char Index_lo, Index_hi;	/* index of first color map entry */
    unsigned char Length_lo, Length_hi;	/* number of entries in color map */
    unsigned char CoSize;		/* size of color map entry (15,16,24,32) */
    unsigned char X_org_lo, X_org_hi;	/* x origin of image */
    unsigned char Y_org_lo, Y_org_hi;	/* y origin of image */
    unsigned char Width_lo, Width_hi;	/* width of image */
    unsigned char Height_lo, Height_hi;	/* height of image */
    unsigned char PixelSize;		/* pixel size (8,16,24,32) */
    unsigned char AttBits;		/* 4 bits, number of attribute bits per pixel */
    unsigned char Rsrvd;		/* 1 bit, reserved */
    unsigned char OrgBit;		/* 1 bit, origin: 0=lower left, 1=upper left */
    unsigned char IntrLve;		/* 2 bits, interleaving flag */
    };

typedef char TGA_ImageIDField[256];

/* Definitions for image types. */
#define TGA_Null 0
#define TGA_Map 1
#define TGA_RGB 2
#define TGA_Mono 3
#define TGA_RLEMap 9
#define TGA_RLERGB 10
#define TGA_RLEMono 11
#define TGA_CompMap 32
#define TGA_CompMap4 33

/* Definitions for interleave flag. */
#define TGA_IL_None 0
#define TGA_IL_Two 1
#define TGA_IL_Four 2

/* #include "tga.h"*/

/* from ppm.h */

typedef struct
    {
    IBYTE r, g, b;
    } pixel;

#define PPM_GETR(p) ((p).r)
#define PPM_GETG(p) ((p).g)
#define PPM_GETB(p) ((p).b)
#define PPM_ASSIGN(p,red,grn,blu) do { (p).r = (red); (p).g = (grn); (p).b = (blu); } while ( 0 )
#define PPM_EQUAL(p,q) ( (p).r == (q).r && (p).g == (q).g && (p).b == (q).b )


#define PPM_MAXMAXVAL 255

/* Color scaling macro -- to make writing ppmtowhatever easier. */

#define PPM_DEPTH(newp,p,oldmaxval,newmaxval) \
    PPM_ASSIGN( (newp), \
	( (int) PPM_GETR(p) * (newmaxval) + (oldmaxval) / 2 ) / (oldmaxval), \
	( (int) PPM_GETG(p) * (newmaxval) + (oldmaxval) / 2 ) / (oldmaxval), \
	( (int) PPM_GETB(p) * (newmaxval) + (oldmaxval) / 2 ) / (oldmaxval) )


/* Luminance macro. */

#define PPM_LUMIN(p) ( 0.299 * PPM_GETR(p) + 0.587 * PPM_GETG(p) + 0.114 * PPM_GETB(p) )

static int pm_error_flag;

static void pm_error(char *msg)
{
  fprintf( stderr, "%s: ", msg );
  pm_error_flag = 1;
}

/* #include "ppmcmap.h" */

/* Max number of colors allowed for colormapped output. */
#define MAXCOLORS 256

/* Forward routines. */

static void writetga_header ARGS(( struct ImageHeader* tgaP, char* id, FILE *ofp ));
static void put_map_entry ARGS(( pixel* valueP, int size, IBYTE maxval, FILE *ofp ));
static void compute_runlengths ARGS(( int cols, pixel* pixelrow, int* runlength));
static void put_pixel ARGS(( pixel* pP, int imgtype, IBYTE maxval, FILE *ofp ));
static void put_mono ARGS(( pixel* pP, IBYTE maxval, FILE *ofp));
static void put_map ARGS(( pixel* pP,  FILE *ofp));
static void put_rgb ARGS(( pixel* pP, IBYTE maxval, FILE *ofp ));


static void
writetga_header( tgaP, id , ofp )
    struct ImageHeader* tgaP;
    char* id;
    FILE *ofp;
    {
    unsigned char flags;

    fputc( tgaP->IDLength , ofp);
    fputc( tgaP->CoMapType , ofp);
    fputc( tgaP->ImgType , ofp);
    fputc( tgaP->Index_lo , ofp);
    fputc( tgaP->Index_hi , ofp);
    fputc( tgaP->Length_lo , ofp);
    fputc( tgaP->Length_hi , ofp);
    fputc( tgaP->CoSize , ofp);
    fputc( tgaP->X_org_lo , ofp);
    fputc( tgaP->X_org_hi , ofp);
    fputc( tgaP->Y_org_lo , ofp);
    fputc( tgaP->Y_org_hi , ofp);
    fputc( tgaP->Width_lo , ofp);
    fputc( tgaP->Width_hi , ofp);
    fputc( tgaP->Height_lo , ofp);
    fputc( tgaP->Height_hi , ofp);
    fputc( tgaP->PixelSize , ofp);
    flags = ( tgaP->AttBits & 0xf ) | ( ( tgaP->Rsrvd & 0x1 ) << 4 ) |
	    ( ( tgaP->OrgBit & 0x1 ) << 5 ) | ( ( tgaP->OrgBit & 0x3 ) << 6 );
    fputc( flags, ofp );
    if ( tgaP->IDLength )
	fwrite( id, 1, (int) tgaP->IDLength, ofp );
    }

static void
put_map_entry( pixel* valueP, int size, IBYTE maxval, FILE *ofp )
{
    int j;
    pixel p;

    switch ( size )
	{
	case 8: 			/* Grey scale. */
	put_mono( valueP, maxval, ofp );
	break;

	case 16:			/* 5 bits each of red green and blue. */
	case 15:			/* Watch for byte order. */
	PPM_DEPTH( p, *valueP, maxval, 31 );
	j = (int) PPM_GETB( p ) | ( (int) PPM_GETG( p ) << 5 ) |
	    ( (int) PPM_GETR( p ) << 10 );
	fputc( j % 256, ofp );
	fputc( j / 256, ofp );
	break;

	case 32:
	case 24:			/* 8 bits each of blue green and red. */
	put_rgb( valueP, maxval, ofp );
	break;

	default:
	pm_error( "unknown colormap pixel size" );
	}
    }

static void
compute_runlengths( cols, pixelrow, runlength )
    int cols;
    pixel* pixelrow;
    int* runlength;
    {
    int col, start;

    /* Initialize all run lengths to 0.  (This is just an error check.) */
    for ( col = 0; col < cols; ++col )
	runlength[col] = 0;

    /* Find runs of identical pixels. */
    for ( col = 0; col < cols; )
	{
	start = col;
	do {
	    ++col;
	    }
	while ( col < cols &&
		col - start < 128 &&
		PPM_EQUAL( pixelrow[col], pixelrow[start] ) );
	runlength[start] = col - start;
	}

    /* Now look for runs of length-1 runs, and turn them into negative runs. */
    for ( col = 0; col < cols; )
	{
	if ( runlength[col] == 1 )
	    {
	    start = col;
	    while ( col < cols &&
		    col - start < 128 &&
		    runlength[col] == 1 )
		{
		runlength[col] = 0;
		++col;
		}
	    runlength[start] = - ( col - start );
	    }
	else
	    col += runlength[col];
	}
    }

static void
put_pixel( pixel* pP, int imgtype, IBYTE maxval,  FILE *ofp)
    {
    switch ( imgtype )
	{
	case TGA_Mono:
	case TGA_RLEMono:
	put_mono( pP, maxval, ofp );
	break;
	case TGA_Map:
	case TGA_RLEMap:
	put_map( pP, ofp );
	break;
	case TGA_RGB:
	case TGA_RLERGB:
	put_rgb( pP, maxval, ofp );
	break;
	default:
	pm_error( "can't happen" );
	}
    }

static void
put_mono( pixel* pP, IBYTE maxval, FILE *ofp )
    {
    PPM_DEPTH( *pP, *pP, maxval, (IBYTE) 255 );
    fputc( PPM_GETR( *pP ), ofp );
    }

static void
put_map( pP, ofp )
    pixel* pP;
    FILE *ofp;
    {
/* to do :
    fputc( ppm_lookupcolor( cht, pP ), ofp ); */
    }

static void
put_map_index( pP, ofp )
    pixel* pP;
    FILE *ofp;
    {
    fputc(pP->r , ofp );
    }

static void
put_rgb( pixel* pP, IBYTE maxval, FILE *ofp )
{
/*    PPM_DEPTH( *pP, *pP, maxval, (IBYTE) 255 ); don't need */
    fputc( PPM_GETB( *pP ), ofp);
    fputc( PPM_GETG( *pP ), ofp);
    fputc( PPM_GETR( *pP ), ofp);
}


/*
  write RGB image pixels_[cols*rows*3] to targa file "out_file"
  "tgaformat" must be RGA_RGB
  rle_flag : 1 use run-length encoding (not all programs can read this !)
	     0 unpacked
*/

int targa_write(image *im, char *tga_out_file, int tgaformat, int rle_flag)
{
    FILE* ofp;
    register pixel* pP;
    pixel* pixels=(pixel *) im->image;
    pixel p;
    int argn,  ncolors, row, col, i, format, realrow;
    int rowstep;
    IBYTE maxval=255;
    char out_name[180];
    char* cp;
    int* runlength=NULL;
    struct ImageHeader tgaHeader;

    strcpy(out_name,tga_out_file);

    if (tgaformat == 0) {
	switch (im->fmt) {
	case IM_RGB24	: tgaformat = TGA_RGB; break;
	case IM_MAPPED8 : tgaformat = TGA_Map; break;
	}
    }
    tgaHeader.ImgType = tgaformat;
    if (tgaformat != TGA_RGB) return(-2);

/*
    while ( argn < argc && argv[argn][0] == '-' && argv[argn][1] != '\0' )
	{
	if ( pm_keymatch( argv[argn], "-name", 2 ) )
	    {
	    ++argn;
	    if ( argn == argc )
		pm_usage( usage );
	    (void) strcpy( out_name, argv[argn] );
	    }
	else if ( pm_keymatch( argv[argn], "-cmap", 2 ) )
	    tgaHeader.ImgType = TGA_Map;
	else if ( pm_keymatch( argv[argn], "-mono", 2 ) )
	    tgaHeader.ImgType = TGA_Mono;
	else if ( pm_keymatch( argv[argn], "-rgb", 2 ) )
	    tgaHeader.ImgType = TGA_RGB;
	else if ( pm_keymatch( argv[argn], "-norle", 2 ) )
	    rle_flag = 0;
	else
	    pm_usage( usage );
	++argn;
	}
*/

    if ( out_name[0] != '\0' )
	    ofp = fopen( out_name, "wb");
    else ofp = stdout;

    if (! ofp) {
       pm_error( "can't open outputfile " );
       return(-1);
    }

    tgaHeader.ImgType = TGA_RGB;

    if (im->fmt == IM_MAPPED8) {
	tgaHeader.ImgType = TGA_Map;
	ncolors=im->ncolors;

	if (im->cmap == NULL)
	   image_ftoi_cmap(im);
    }

#if 0

    /* Figure out the colormap. */
    switch ( PPM_FORMAT_TYPE( format ) )
	{
	case PPM_TYPE:
	if ( tgaHeader.ImgType == TGA_Mono )
	    pm_error( "input is not a graymap, filter through ppmtopgm first" );
	if ( tgaHeader.ImgType == TGA_Null || tgaHeader.ImgType == TGA_Map )
	    {
	    pm_message( "computing colormap..." );
	    chv = ppm_computecolorhist(
		pixels, im->width, im->height, MAXCOLORS, &ncolors );
	    if ( chv == (colorhist_vector) 0 )
		{
		if ( tgaHeader.ImgType == TGA_Map )
		    pm_error(
			"too many colors - try doing a 'ppmquant %d'",
			MAXCOLORS );
		else
		    tgaHeader.ImgType = TGA_RGB;
		}
	    else
		{
		pm_message( "%d colors found", ncolors );
		if ( tgaHeader.ImgType == TGA_Null )
		    tgaHeader.ImgType = TGA_Map;
		}
	    }
	break;

	case PGM_TYPE:
	case PBM_TYPE:
	if ( tgaHeader.ImgType == TGA_Null )
	    tgaHeader.ImgType = TGA_Mono;
	else if ( tgaHeader.ImgType == TGA_Map )
	    {
	    pm_message( "computing colormap..." );
	    chv = ppm_computecolorhist(
		pixels, im->width, im->height, MAXCOLORS, &ncolors );
	    if ( chv == (colorhist_vector) 0 )
		pm_error( "can't happen" );
	    pm_message( "%d colors found", ncolors );
	    }
	break;

	default:
	pm_error( "can't happen" );
	}
#endif

    if ( rle_flag )
	{
	switch ( tgaHeader.ImgType )
	    {
	    case TGA_Mono:
	    tgaHeader.ImgType = TGA_RLEMono;
	    break;
	    case TGA_Map:
	    tgaHeader.ImgType = TGA_RLEMap;
	    break;
	    case TGA_RGB:
	    tgaHeader.ImgType = TGA_RLERGB;
	    break;
	    default:
	    pm_error( "can't happen" );
	    }
	runlength = (int*) calloc( im->width, sizeof(int) );
	}

    tgaHeader.IDLength = 0;
    tgaHeader.Index_lo = 0;
    tgaHeader.Index_hi = 0;

    if ( tgaHeader.ImgType == TGA_Map || tgaHeader.ImgType == TGA_RLEMap )
	{
	/* Make a hash table for fast color lookup. */
/*	cht = ppm_colorhisttocolorhash( chv, ncolors ); */

	tgaHeader.CoMapType = 1;
	tgaHeader.Length_lo = ncolors % 256;
	tgaHeader.Length_hi = ncolors / 256;
	tgaHeader.CoSize = 24;
	}
    else
	{
	tgaHeader.CoMapType = 0;
	tgaHeader.Length_lo = 0;
	tgaHeader.Length_hi = 0;
	tgaHeader.CoSize = 0;
	}
    if ( tgaHeader.ImgType == TGA_RGB || tgaHeader.ImgType == TGA_RLERGB )
	tgaHeader.PixelSize = 24;
    else
	tgaHeader.PixelSize = 8;
    tgaHeader.X_org_lo = tgaHeader.X_org_hi = 0;
    tgaHeader.Y_org_lo = tgaHeader.Y_org_hi = 0;
    tgaHeader.Width_lo = im->width % 256;
    tgaHeader.Width_hi = im->width / 256;
    tgaHeader.Height_lo = im->height % 256;
    tgaHeader.Height_hi = im->height / 256;
    tgaHeader.AttBits = 0;
    tgaHeader.Rsrvd = 0;
    tgaHeader.IntrLve = 0;
    tgaHeader.OrgBit = 0;

    /* Write out the Targa header. */
    writetga_header( &tgaHeader, (char*) 0, ofp );

    if ( tgaHeader.ImgType == TGA_Map || tgaHeader.ImgType == TGA_RLEMap )
	{
	/* Write out the Targa colormap. */
	for ( i = 0; i < im->ncolors; ++i ) {
	    pixel c;
	    c.r = im->cmap[i].r;
	    c.g = im->cmap[i].g;
	    c.b = im->cmap[i].b;
	    put_map_entry( &c, tgaHeader.CoSize, maxval, ofp );
/*	    put_map_entry( &chv[i].color, tgaHeader.CoSize, maxval, ofp ); */
	}
	}


    rowstep=im->width;

    /* Write out the pixels */
    for ( row = 0; row < im->height; ++row )
	{

	realrow = row;
	if ( tgaHeader.OrgBit == 0 )
	    realrow = im->height - realrow - 1;

/*	printf("%d ",realrow); */

	if ( rle_flag )
	    {
	    compute_runlengths( im->width, &pixels[realrow*rowstep], runlength );
	    for ( col = 0; col < im->width; )
		{
		if ( runlength[col] > 0 )
		    {
		    fputc( 0x80 + runlength[col] - 1,ofp );
		    put_pixel(
			&(pixels[realrow*rowstep+col]),
			tgaHeader.ImgType, maxval, ofp );
		    col += runlength[col];
		    }
		else if ( runlength[col] < 0 )
		    {
		    fputc( -runlength[col] - 1,ofp );
		    for ( i = 0; i < -runlength[col]; ++i )
			put_pixel(
			    &(pixels[realrow*rowstep+(col+i)]),
			    tgaHeader.ImgType, maxval, ofp );
		    col += -runlength[col];
		    }
		else
		    pm_error( "can't happen" );
		}
	    }
	else
	    {
	    for ( col = 0, pP = &pixels[realrow*rowstep]; col < im->width; ++col, pP+=1 )
		put_pixel( pP, tgaHeader.ImgType, maxval, ofp );
	    }
	}
    if (runlength) free(runlength);

    if ( ofp != stdout)
       fclose(ofp);

    return( 0 );
}
/******************************************************************************/

/* tgatoppm.c - read a TrueVision Targa file and write a portable pixmap
**
** Partially based on tga2rast, version 1.0, by Ian MacPhedran.
**
** Copyright (C) 1989 by Jef Poskanzer.
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose and without fee is hereby granted, provided
** that the above copyright notice appear in all copies and that both that
** copyright notice and this permission notice appear in supporting
** documentation.  This software is provided "as is" without express or
** implied warranty.
*/

/*
#define MAXCOLORS 16384
use 255 from read
*/
static int mapped, rlencoded;

static pixel ColorMap[MAXCOLORS];
static int RLE_count = 0, RLE_flag = 0;

static void get_map_entry ARGS(( FILE* ifp, pixel* Value, int Size ));
static void get_pixel ARGS(( FILE* ifp, pixel* dest, int Size ));

static unsigned char
getbyte( FILE *ifp )
    {
    unsigned char c;

    if ( fread( (char*) &c, 1, 1, ifp ) != 1 )
	pm_error( "EOF / read error" );

    return c;
    }



static void
read_tga_header( ifp, tgaP, junk )
    FILE* ifp;
    struct ImageHeader* tgaP;
    TGA_ImageIDField junk;
    {
    unsigned char flags;

    tgaP->IDLength = getbyte( ifp );
    tgaP->CoMapType = getbyte( ifp );
    tgaP->ImgType = getbyte( ifp );
    tgaP->Index_lo = getbyte( ifp );
    tgaP->Index_hi = getbyte( ifp );
    tgaP->Length_lo = getbyte( ifp );
    tgaP->Length_hi = getbyte( ifp );
    tgaP->CoSize = getbyte( ifp );
    tgaP->X_org_lo = getbyte( ifp );
    tgaP->X_org_hi = getbyte( ifp );
    tgaP->Y_org_lo = getbyte( ifp );
    tgaP->Y_org_hi = getbyte( ifp );
    tgaP->Width_lo = getbyte( ifp );
    tgaP->Width_hi = getbyte( ifp );
    tgaP->Height_lo = getbyte( ifp );
    tgaP->Height_hi = getbyte( ifp );
    tgaP->PixelSize = getbyte( ifp );
    flags = getbyte( ifp );
    tgaP->AttBits = flags & 0xf;
    tgaP->Rsrvd = ( flags & 0x10 ) >> 4;
    tgaP->OrgBit = ( flags & 0x20 ) >> 5;
    tgaP->IntrLve = ( flags & 0xc0 ) >> 6;

    if ( tgaP->IDLength != 0 )
	fread( junk, 1, (int) tgaP->IDLength, ifp );
    junk[tgaP->IDLength] = 0;
    }

static void
get_map_entry( ifp, Value, Size )
    FILE* ifp;
    pixel* Value;
    int Size;
    {
    unsigned char j, k, r, g, b;

    /* Read appropriate number of bytes, break into rgb & put in map. */
    switch ( Size )
	{
	case 8:				/* Grey scale, read and triplicate. */
	r = g = b = getbyte( ifp );
	break;

	case 16:			/* 5 bits each of red green and blue. */
	case 15:			/* Watch for byte order. */
	j = getbyte( ifp );
	k = getbyte( ifp );
	r = ( k & 0x7C ) >> 2;
	g = ( ( k & 0x03 ) << 3 ) + ( ( j & 0xE0 ) >> 5 );
	b = j & 0x1F;
	break;

	case 32:
	case 24:			/* 8 bits each of blue green and red. */
	b = getbyte( ifp );
	g = getbyte( ifp );
	r = getbyte( ifp );
	if ( Size == 32 )
	    (void) getbyte( ifp );	/* Read alpha byte & throw away. */
	break;

	default:
	 fprintf(stderr, "unknown colormap pixel size (#2) - %d\n", Size );
	}
    PPM_ASSIGN( *Value, r, g, b );
    }

static void
get_pixel( ifp, dest, Size )
    FILE* ifp;
    pixel* dest;
    int Size;
    {
    static IBYTE Red, Grn, Blu;
    unsigned char j, k;
    static unsigned int l;

    /* Check if run length encoded. */
    if ( rlencoded )
	{
	if ( RLE_count == 0 )
	    { /* Have to restart run. */
	    unsigned char i;
	    i = getbyte( ifp );
	    RLE_flag = ( i & 0x80 );
	    if ( RLE_flag == 0 )
		/* Stream of unencoded pixels. */
		RLE_count = i + 1;
	    else
		/* Single pixel replicated. */
		RLE_count = i - 127;
	    /* Decrement count & get pixel. */
	    --RLE_count;
	    }
	else
	    { /* Have already read count & (at least) first pixel. */
	    --RLE_count;
	    if ( RLE_flag != 0 )
		/* Replicated pixels. */
		goto PixEncode;
	    }
	}
    /* Read appropriate number of bytes, break into RGB. */
    switch ( Size )
	{
	case 8:				/* Grey scale, read and triplicate. */
	Red = Grn = Blu = l = getbyte( ifp );
	break;

	case 16:			/* 5 bits each of red green and blue. */
	case 15:			/* Watch byte order. */
	j = getbyte( ifp );
	k = getbyte( ifp );
	l = ( (unsigned int) k << 8 ) + j;
	Red = ( k & 0x7C ) >> 2;
	Grn = ( ( k & 0x03 ) << 3 ) + ( ( j & 0xE0 ) >> 5 );
	Blu = j & 0x1F;
	break;

	case 32:
	case 24:			/* 8 bits each of blue green and red. */
	Blu = getbyte( ifp );
	Grn = getbyte( ifp );
	Red = getbyte( ifp );
	if ( Size == 32 )
	    (void) getbyte( ifp );	/* Read alpha byte & throw away. */
	l = 0;
	break;

	default:
	 fprintf(stderr, "unknown pixel size (#2) - %d", Size );
	}

PixEncode:
    if ( mapped )
	*dest = ColorMap[l];
    else
	PPM_ASSIGN( *dest, Red, Grn, Blu );
    }

/*
  read targa file "tga_in_file" in 24 bit true color format
  im->image points to allocated image

  im->width, im->height the dimensions of the image

  Result :
    <0 Error

*/

int targa_read(image *im,char * tga_in_file)
    {
    struct ImageHeader tga_head;
    int i;
    unsigned int temp1, temp2;
    FILE* ifp;
    int debug, rows, cols, row, col, realrow, truerow, baserow;
    int maxval;
    pixel* pixels;

    TGA_ImageIDField junk;

    debug = 0;


    /* open file */
    ifp = fopen(tga_in_file,"rb");

    if (ifp == NULL)
       return(-1);


    /* Read the Targa file header. */
    read_tga_header( ifp, &tga_head, junk );

    if ( debug )
	{
	printf( "IDLength = %d\n", (int) tga_head.IDLength );
	printf( "CoMapType = %d\n", (int) tga_head.CoMapType );
	printf( "ImgType = %d\n", (int) tga_head.ImgType );
	printf( "Index_lo = %d\n", (int) tga_head.Index_lo );
	printf( "Index_hi = %d\n", (int) tga_head.Index_hi );
	printf( "Length_lo = %d\n", (int) tga_head.Length_lo );
	printf( "Length_hi = %d\n", (int) tga_head.Length_hi );
	printf( "CoSize = %d\n", (int) tga_head.CoSize );
	printf( "X_org_lo = %d\n", (int) tga_head.X_org_lo );
	printf( "X_org_hi = %d\n", (int) tga_head.X_org_hi );
	printf( "Y_org_lo = %d\n", (int) tga_head.Y_org_lo );
	printf( "Y_org_hi = %d\n", (int) tga_head.Y_org_hi );
	printf( "Width_lo = %d\n", (int) tga_head.Width_lo );
	printf( "Width_hi = %d\n", (int) tga_head.Width_hi );
	printf( "Height_lo = %d\n", (int) tga_head.Height_lo );
	printf( "Height_hi = %d\n", (int) tga_head.Height_hi );
	printf( "PixelSize = %d\n", (int) tga_head.PixelSize );
	printf( "AttBits = %d\n", (int) tga_head.AttBits );
	printf( "Rsrvd = %d\n", (int) tga_head.Rsrvd );
	printf( "OrgBit = %d\n", (int) tga_head.OrgBit );
	printf( "IntrLve = %d\n", (int) tga_head.IntrLve );
	printf( "Image Id = %s\n", junk );
	}

    im->height=rows = ( (int) tga_head.Height_lo ) + ( (int) tga_head.Height_hi ) * 256;
    im->width=cols = ( (int) tga_head.Width_lo ) + ( (int) tga_head.Width_hi ) * 256;
    im->fmt = IM_RGB24;

    switch ( tga_head.ImgType )
	{
	case TGA_Map:
	case TGA_RGB:
	case TGA_Mono:
	case TGA_RLEMap:
	case TGA_RLERGB:
	case TGA_RLEMono:
	break;

	default:
	  fprintf(stderr,"unknown Targa image type %d", tga_head.ImgType );
	  fclose(ifp);
	  return(-1);
	}

    if ( tga_head.ImgType == TGA_Map ||
	 tga_head.ImgType == TGA_RLEMap ||
	 tga_head.ImgType == TGA_CompMap ||
	 tga_head.ImgType == TGA_CompMap4 )
	{ /* Color-mapped image */
	if ( tga_head.CoMapType != 1 ) {
	    fprintf(stderr,
		"mapped image (type %d) with color map type != 1",
		tga_head.ImgType );
	    fclose(ifp);
	    return(-1);
	}
	mapped = 1;
	/* Figure maxval from CoSize. */
	switch ( tga_head.CoSize )
	    {
	    case 8:
	    case 24:
	    case 32:
	    maxval = 255;
	    break;

	    case 15:
	    case 16:
	    maxval = 31;
	    break;

	    default:
	      fprintf(stderr,
		"unknown colormap pixel size - %d\n", tga_head.CoSize );
	      fclose(ifp);
	      return(-1);

	    }
	if ( maxval > PPM_MAXMAXVAL ) {
	      fprintf(stderr,"CoSize is too large \n" );
	      fclose(ifp);
	      return(-1);
	      }

	}
    else
	{ /* Not colormap, so figure maxval from PixelSize. */
	mapped = 0;
	switch ( tga_head.PixelSize )
	    {
	    case 8:
	    case 24:
	    case 32:
	    maxval = 255;
	    break;

	    case 15:
	    case 16:
	    maxval = 31;
	    break;

	    default:
	       fprintf(stderr, "unknown pixel size - %d", tga_head.PixelSize );
	       fclose(ifp);
	       return(-1);
	    }
	if ( maxval > PPM_MAXMAXVAL ) {
	    fprintf(stderr, "PixelSize is too large " );
	    fclose(ifp);
	    return(-1);
	    }

	}

    /* If required, read the color map information. */
    if ( tga_head.CoMapType != 0 )
	{
	temp1 = tga_head.Index_lo + tga_head.Index_hi * 256;
	temp2 = tga_head.Length_lo + tga_head.Length_hi * 256;
	if ( ( temp1 + temp2 + 1 ) >= MAXCOLORS ) {
	    fprintf(stderr, "too many colors - %d\n", ( temp1 + temp2 + 1 ) );
	    fclose(ifp);
	    return(-1);
	}

	image_alloc_cmap(im,temp1+temp2);

	for ( i = temp1; i < ( temp1 + temp2 ); ++i ) {
	    get_map_entry( ifp, &ColorMap[i], (int) tga_head.CoSize );
	    image_set_cmap(im,i,ColorMap[i].r,ColorMap[i].g,ColorMap[i].b);
	    }
	}

    /* Check run-length encoding. */
    if ( tga_head.ImgType == TGA_RLEMap ||
	 tga_head.ImgType == TGA_RLERGB ||
	 tga_head.ImgType == TGA_RLEMono )
	rlencoded = 1;
    else
	rlencoded = 0;

    /* Read the Targa file body and convert to portable format. */
    image_alloc(im,im->fmt,im->width,im->height);

    pixels = (pixel *) im->image;

    /* return pointer to caller */

    if (pixels == NULL) {
	fclose(ifp);
	return(-1);
    }

    if (0) {
    if (( tga_head.OrgBit == 0 )&& ( tga_head.IntrLve == TGA_IL_None )
	&& !rlencoded && ( tga_head.PixelSize == 24 ) && (!mapped)) {
	printf("can Optimize\n ");
	if (fread( (char*) pixels, sizeof(pixel), rows*cols, ifp ) != (rows*cols) ) {
	}
    }
    }
    truerow = 0;
    baserow = 0;
    for ( row = 0; row < rows; ++row )
	{
	realrow = truerow;
	if ( tga_head.OrgBit == 0 )
	    realrow = rows - realrow - 1;

	for ( col = 0; col < cols; ++col )
	    get_pixel( ifp, &(pixels[realrow*cols+col]), (int) tga_head.PixelSize );
	if ( tga_head.IntrLve == TGA_IL_Four )
	    truerow += 4;
	else if ( tga_head.IntrLve == TGA_IL_Two )
	    truerow += 2;
	else
	    ++truerow;
	if ( truerow >= rows )
	    truerow = ++baserow;
	}
    fclose( ifp );

    return( 0 );
    }
