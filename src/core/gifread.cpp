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
/* +-------------------------------------------------------------------+ */
/* | Copyright 1990, David Koblas.									   | */
/* |   Permission to use, copy, modify, and distribute this software   | */
/* |   and its documentation for any purpose and without fee is hereby | */
/* |   granted, provided that the above copyright notice appear in all | */
/* |   copies and that both that copyright notice and this permission  | */
/* |   notice appear in supporting documentation.  This software is    | */
/* |   provided "as is" without express or implied warranty.           | */
/* +-------------------------------------------------------------------+ */

/* hg

  extended

  for info see gifspec.txt
*/

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "image.h"
#include "gifread.h"

class GImageMovie;



#define MAXCOLORMAPSIZE 	256

/*
#define TRUE	1
#define FALSE	0
*/

#define CM_RED		0
#define CM_GREEN	1
#define CM_BLUE 	2

#define MAX_LWZ_BITS		12

#define INTERLACE		0x40
#define LOCALCOLORMAP	0x80
#define BitSet(byte, bit)	(((byte) & (bit)) == (bit))

// #define ReadOK(file,buffer,len) (fread(buffer, len, 1, file) != 0)

#define LM_to_uint(a,b) 		(((b)<<8)|(a))

typedef struct {
	unsigned int	Width;
	unsigned int	Height;
	unsigned char	ColorMap[3][MAXCOLORMAPSIZE];
	int				GlobalColorTable;
	unsigned int	BitPixel;
	unsigned int	ColorResolution;
	unsigned int	Background;
	unsigned int	AspectRatio;
} GifScreen;

typedef struct {
	int	transparent;
	int	delayTime;
	int	inputFlag;
	int	disposal;
} Gif89;  



// a class encapsulating the state of the gifread 
class GifReaderState 
{
public :
	FILE *fd;


	int ReadOK(unsigned char buffer[],int len) { return (fread(buffer, len, 1, fd) != 0); }

	GifReaderState(FILE *theFile) {
		
		fd = theFile;

		gif89.transparent = -1;
		gif89.delayTime = -1;
		gif89.inputFlag = -1;
		gif89.disposal = -1;

		gif89Global.transparent = -1;
		gif89Global.delayTime = -1;
		gif89Global.inputFlag = -1;
		gif89Global.disposal = -1;

		verbose = FALSE;
		showComment = FALSE;
		ZeroDataBlock = FALSE;
		fresh = FALSE;

		curbit=0, lastbit=0, last_byte=0;

		done = FALSE;


	}
	
	int GetDataBlock (unsigned char	*buf );
	int	ZeroDataBlock;

	BOOL	verbose;
	BOOL	showComment;
	Gif89	gif89;
	
	Gif89	gif89Global;

	GifScreen gifScreen;

	unsigned char	buf[280]; // for getCode
	unsigned int		curbit, lastbit, last_byte;
	BOOL done;
	
	int GetCode(unsigned int code_size, int flag);

	int	fresh; //  = FALSE;
	unsigned int	code_size, set_code_size;
	int	max_code, max_code_size;
	int	firstcode, oldcode;
	int	clear_code, end_code;
	int	table[2][(1<< MAX_LWZ_BITS)];
	int	stack[(1<<(MAX_LWZ_BITS))*2], *sp;

	int LWZReadByte(int flag, unsigned int input_code_size);

	int ReadColorMap(int number,unsigned char	buffer[3][MAXCOLORMAPSIZE]);
	int DoExtension (int label );
	int ReadImage(image *im, int len, int height,int numColors, unsigned char cmap[3][MAXCOLORMAPSIZE], int interlace, int ignore);

	int ReadGIFImage(image *im, int imageNumber);
	// read a gif movie 
	int  ReadGifMovie(GImageMovie &movie);

};




/*
void pm_error(const char *message)
{
	TRACE("GifRead Error : %s \n",message);
	fprintf(stderr,"GifRead Error:%s \n",message);
}
#define pm_error TRACE printf
#define pm_message printf

*/

#define pm_error TRACE 
#define pm_message TRACE



int 
GifReaderState::ReadGIFImage(image *im, int imageNumber)
{

	int ret=0;

	unsigned char	buf[16];
	unsigned char	c;
	unsigned char	localColorMap[3][MAXCOLORMAPSIZE];

	int			useGlobalColormap;
	int			bitPixel;
	int			imageCount = 0;
	char		version[4];

	if (! ReadOK(buf,6)) {
		pm_error("error reading magic number" );
		return(IME_BADFILE);
	}

	if (strncmp((char *)buf,"GIF",3) != 0) {       /* hg (char *) cast */
		pm_error("not a GIF file" );
		return(IME_BADFILE);
	}

	strncpy(version, (char *) buf + 3, 3);
	version[3] = '\0';

	if ((strcmp(version, "87a") != 0) && (strcmp(version, "89a") != 0)) {
		pm_error("bad version number, not '87a' or '89a'" );
		return(IME_BADFILE);
	}

	if (! ReadOK(buf,7)) {
		pm_error("failed to read screen descriptor" );
		return(IME_FILEIO);
    }

	gifScreen.Width 	  = LM_to_uint(buf[0],buf[1]);
	gifScreen.Height	  = LM_to_uint(buf[2],buf[3]);
	gifScreen.BitPixel	  = 2<<(buf[4]&0x07);
	gifScreen.ColorResolution = (((buf[4]&0x70)>>3)+1);
	gifScreen.Background	  = buf[5];
	gifScreen.AspectRatio	  = buf[6];

	if (verbose) {
		pm_message("Width %d Height %d, Bit %d, ColorResolution : %d BackGround : %d  \n",	
			gifScreen.Width,gifScreen.Height,gifScreen.BitPixel,gifScreen.ColorResolution,gifScreen.Background); 
    }


	if (BitSet(buf[4], LOCALCOLORMAP)) {	/* Global Colormap */
		if (ReadColorMap(gifScreen.BitPixel,gifScreen.ColorMap)) {
			pm_error("error reading global colormap" );
			return(IME_FILEIO);
		}
	}

	if (gifScreen.AspectRatio != 0 && gifScreen.AspectRatio != 49) {
		float	r;
		r = ( (float) gifScreen.AspectRatio + 15.0f ) / 64.0f;
		pm_message("warning - non-square pixels; to fix do a 'pnmscale -%cscale %g'",
		    r < 1.0 ? 'x' : 'y',
		    r < 1.0 ? 1.0 / r : r );
	}

	for (;;) {
		if (! ReadOK(&c,1)) {
			pm_error("EOF / read error on image data" );
			return(IME_FILEIO);
		}


		if (c == ';') {         /* GIF terminator */
			if (imageCount < imageNumber)  {
				pm_message("only %d image%s found in file",
					 imageCount, imageCount>1?"s":"" );
				return(IME_FILEIO);
			}
			
			return(ret);

		}

		if (c == '!') {         /* Extension */
			if (! ReadOK(&c,1)) {
				pm_error("OF / read error on extention function code");
				return(IME_FILEIO);
			}
			DoExtension(c);
			continue;
		}

		if (c != ',') {         /* Not a valid start character */
			pm_message("bogus character 0x%02x, ignoring", (int) c );
			continue;
		}

		++imageCount;

		if (! ReadOK(buf,9)) {
			pm_error("couldn't read left/top/width/height");
			return(IME_FILEIO);
		}

		useGlobalColormap = ! BitSet(buf[8], LOCALCOLORMAP);

		bitPixel = 1<<((buf[8]&0x07)+1);

		if (! useGlobalColormap) {
			if (ReadColorMap(bitPixel, localColorMap)) {
				pm_error("error reading local colormap" );
				return(IME_FILEIO);
			}

			ret=ReadImage(im,LM_to_uint(buf[4],buf[5]),
				  LM_to_uint(buf[6],buf[7]), bitPixel, localColorMap,
				  BitSet(buf[8], INTERLACE), imageCount != imageNumber);
		} else {
			ret=ReadImage(im,LM_to_uint(buf[4],buf[5]),
				  LM_to_uint(buf[6],buf[7]), gifScreen.BitPixel, gifScreen.ColorMap,
				  BitSet(buf[8], INTERLACE), imageCount != imageNumber);
		}

	}
	return(ret);
}

int 
GifReaderState::ReadColorMap(int number,unsigned char	buffer[3][MAXCOLORMAPSIZE])

{
	int		i;
	unsigned char	rgb[3];

	for (i = 0; i < number; ++i) {
		if (! ReadOK( rgb, sizeof(rgb))) {
			pm_error("bad colormap" );
			return(TRUE);
		}

		buffer[CM_RED][i] = rgb[0] ;
		buffer[CM_GREEN][i] = rgb[1] ;
		buffer[CM_BLUE][i] = rgb[2] ;
	}
	return FALSE;
}

int 
GifReaderState::DoExtension(int label)
{

	unsigned	char	buf[256];
	char		*str;

	switch (label) {
	case 0x01:		/* Plain Text Extension */
		str = "Plain Text Extension";
#ifdef notdef
		if (GetDataBlock( buf) == 0)
			;

		lpos   = LM_to_uint(buf[0], buf[1]);
		tpos   = LM_to_uint(buf[2], buf[3]);
		width  = LM_to_uint(buf[4], buf[5]);
		height = LM_to_uint(buf[6], buf[7]);
		cellw  = buf[8];
		cellh  = buf[9];
		foreground = buf[10];
		background = buf[11];

		while (GetDataBlock(fd, (unsigned char*) buf) != 0) {
			PPM_ASSIGN(image[ypos][xpos],
					cmap[CM_RED][v],
					cmap[CM_GREEN][v],
					cmap[CM_BLUE][v]);
			++index;
		}

		return FALSE;
#else
		break;
#endif
	case 0xff:		/* Application Extension */
		str = "Application Extension";
		break;
	case 0xfe:		/* Comment Extension */
		str = "Comment Extension";
		while (GetDataBlock((unsigned char*) buf) != 0) {
			if (showComment)
				pm_message("gif comment: %s", buf );
		}
		return FALSE;
	case 0xf9:		/* Graphic Control Extension */
		str = "Graphic Control Extension";
		(void) GetDataBlock((unsigned char*) buf);
		gif89.disposal	  = (buf[0] >> 2) & 0x7;
		gif89.inputFlag   = (buf[0] >> 1) & 0x1;
		gif89.delayTime   = LM_to_uint(buf[1],buf[2]);
		if ((buf[0] & 0x1) != 0)
			gif89.transparent = buf[3];
		else 
			gif89.transparent = -1; // hg 

		//if (verbose)  printf("%s:  transparent %d index:%d %d \n",str,buf[0] & 0x1,buf[3],Gif89.delayTime);

		while (GetDataBlock( buf) != 0)
			;
		return FALSE;

	default:
		str = ( char *) buf;
		sprintf(( char *) buf, "UNKNOWN (0x%02x)", label);
		break;
	}

	pm_message("got a '%s' extension", str );

	TRACE("GifRead : got a '%s' extension \n", str );

	while (GetDataBlock(buf) != 0)
		;

	return FALSE;
}


int 
GifReaderState::GetDataBlock(unsigned char	* buf)
{
	unsigned char	count;

	if (!ReadOK(&count,1)) {
		pm_message("error in getting DataBlock size" );
		return -1;
	}

	ZeroDataBlock = count == 0;

	if ((count != 0) && (! ReadOK( buf, count))) {
		pm_message("error in reading DataBlock" );
		return -1;
	}

	return count;
}

int 
GifReaderState::GetCode(unsigned int code_size, int flag)
{
	//static unsigned char	buf[280];
	// static int		curbit, lastbit, done, last_byte;
	unsigned int		i, j, ret; // hg unsigned
	unsigned char		count;

	if (flag) {
		curbit = 0;
		lastbit = 0;
		done = FALSE;
		last_byte = 2; // hg have never seen this proper initialized
		return 0;
	}

	if ( (curbit+code_size) >= lastbit) {
		if (done) {
			if (curbit >= lastbit)
				pm_error("ran off the end of my bits" );
			return -1;
		}
		buf[0] = buf[last_byte-2];
		buf[1] = buf[last_byte-1];

		if ((count = GetDataBlock(&buf[2])) == 0)
			done = TRUE;

		last_byte = 2 + count;
		ASSERT((curbit+16) >= lastbit);
		curbit = ((curbit  + 16) - lastbit); // avoid underflow
		lastbit = (2+count)*8 ;
	}

	ret = 0;

	// costly loop, optimize ??

	for (i = curbit, j = 0; j < code_size; ++i, ++j)		
		ret |= ((buf[ i / 8 ] & (1 << (i % 8))) != 0) << j;

	// MSVC generates idiv / 8 here bad !!!!!!!!!!!!!!!! so changed vars to unsigned

	curbit += code_size;

	return ret;
}

int 
GifReaderState::LWZReadByte(int flag, unsigned int input_code_size)
{
	//static int	fresh = FALSE;
	int		code, incode;
	//static int	code_size, set_code_size;
	//static int	max_code, max_code_size;
	//static int	firstcode, oldcode;
	//static int	clear_code, end_code;
	//static int	table[2][(1<< MAX_LWZ_BITS)];
	//static int	stack[(1<<(MAX_LWZ_BITS))*2], *sp;
	register int	i;

	if (flag) {
		set_code_size = input_code_size;
		code_size = set_code_size+1;
		clear_code = 1 << set_code_size ;
		end_code = clear_code + 1;
		max_code_size = 2*clear_code;
		max_code = clear_code+2;

		GetCode(0, TRUE);

		fresh = TRUE;

		for (i = 0; i < clear_code; ++i) {
			table[0][i] = 0;
			table[1][i] = i;
		}
		for (; i < (1<<MAX_LWZ_BITS); ++i)
			table[0][i] = table[1][0] = 0;

		sp = stack;

		return 0;

	} else if (fresh) {
		fresh = FALSE;
		do {
			firstcode = oldcode = 	GetCode(code_size, FALSE);
		} while (firstcode == clear_code);
		return firstcode;
	}

	if (sp > stack)
		return *--sp;

	while ((code = GetCode(code_size, FALSE)) >= 0) {
		if (code == clear_code) {
			for (i = 0; i < clear_code; ++i) {
				table[0][i] = 0;
				table[1][i] = i;
			}
			for (; i < (1<<MAX_LWZ_BITS); ++i)
				table[0][i] = table[1][i] = 0;
			code_size = set_code_size+1;
			max_code_size = 2*clear_code;
			max_code = clear_code+2;
			sp = stack;
			firstcode = oldcode = GetCode(code_size, FALSE);
			return firstcode;
		} else if (code == end_code) {
			int		count;
			unsigned char	buf[260];

			if (ZeroDataBlock)
				return -2;

			while ((count = GetDataBlock(buf)) > 0)
				;

			if (count != 0)
				pm_message("missing EOD in data stream (common occurence)");
			return -2;
		}

		incode = code;

		if (code >= max_code) {
			*sp++ = firstcode;
			code = oldcode;
		}

		while (code >= clear_code) {
			*sp++ = table[1][code];
 
/* 			circular table entry BIG ERRORcircular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORThe thread 0xFFF7181B has exited with code 0 (0x0).
BLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular tab
e entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNC
3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: : circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entr
 BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: ci
cular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: : circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG E
RORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular 
able entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXX
NCC3D: circular table entry BIG ERRORBLAXXUNCC3D: circular table entry BIG ERRORBLAXX
*/

			if (code == table[0][code]) {
				pm_error("circular table entry BIG ERROR");
				return -1;
			}
			code = table[0][code];
		}

		*sp++ = firstcode = table[1][code];

		if ((code = max_code) < (1<<MAX_LWZ_BITS)) {
			table[0][code] = oldcode;
			table[1][code] = firstcode;
			++max_code;
			if ((max_code >= max_code_size) &&
				(max_code_size < (1<<MAX_LWZ_BITS))) {
				max_code_size *= 2;
				++code_size;
			}
		}


		oldcode = incode;

		if (sp > stack)
			return *--sp;
	}
	return code;
}

int 
GifReaderState::ReadImage(image *im, int len, int height,int numColors, unsigned char cmap[3][MAXCOLORMAPSIZE], int interlace, int ignore)
{
	unsigned char	c;
	int		v;
	int		xpos = 0, ypos = 0, pass = 0;
	IBYTE 	*image;
	int ret;

	/*
	**  Initialize the Compression routines
	*/
	if (! ReadOK(&c,1)) {
		pm_error("EOF / read error on image data" );
		return IME_FILEIO;
	}

	if (LWZReadByte(TRUE, c) < 0) {
		pm_error("error reading image" );
		return IME_FILEIO;
	}

	/*
	**  If this is an "uninteresting picture" ignore it.
	*/
	if (ignore) {
		if (verbose)
			pm_message("skipping image..." );

		while (LWZReadByte( FALSE, c) >= 0)
			;
		return(0);
	}

	ret=image_alloc(im,IM_MAPPED8,len,height);

	if (ret<0) return(ret);

	ret= image_alloc_cmap(im, numColors );
	if (ret<0) return(ret);

	for(int i= 0; i< numColors; i++) 
	 	image_set_cmap(im,i,cmap[CM_RED][i], cmap[CM_GREEN][i], cmap[CM_BLUE][i]);		

	im->backgroundIndex = gifScreen.Background; // gif89.transparent;

	if (gif89.transparent>=0) {
		im->colorKey=1;
		im->colorKeyIndex = gif89.transparent;

		if (im->cmap && gif89.transparent<numColors) {
			im->colorKeyRgb.r = im->cmap[gif89.transparent].r;
			im->colorKeyRgb.g = im->cmap[gif89.transparent].g;
			im->colorKeyRgb.b = im->cmap[gif89.transparent].b;
		}
	}





	if (verbose)
		pm_message("reading %d by %d%s GIF image \n",
			len, height, interlace ? " interlaced" : "" );

	image = IROW(im,ypos);

	while ((v = LWZReadByte(FALSE,c)) >= 0 ) {
/*
		pixel alpha = 0; //  to do 
		ASSIGN_RGB(image,ypos,xpos, len, cmap[CM_RED][v],
					cmap[CM_GREEN][v], cmap[CM_BLUE][v],alpha);
*/
		image[xpos]=v;

		++xpos;
		if (xpos == len) {
			xpos = 0;
			if (interlace) {
				switch (pass) {
				case 0:
				case 1:
					ypos += 8; break;
				case 2:
					ypos += 4; break;
				case 3:
					ypos += 2; break;
				}

				if (ypos >= height) {
					++pass;
					switch (pass) {
					case 1:
						ypos = 4; break;
					case 2:
						ypos = 2; break;
					case 3:
						ypos = 1; break;
					default:
						goto fini;
					}
				}
			} else {
				++ypos;
			}
	  		image = IROW(im,ypos);

		}
		if (ypos >= height)
			break;
	}

fini:
	if (LWZReadByte(FALSE,c)>=0)
		pm_message("too much input data, ignoring extra...");

//	if (verbose)
//		pm_message("writing output\n");

	return(0);
}



int ReadGif(image *im, const char *fileName)
{

	int	imageNumber;
	int	ret;

    /*
     * Find the image file and open it
     */
	FILE *fp = fopen(fileName,"rb");

    if (fp == NULL) {
        TRACE("Cannot find %s.\n", fileName);
        return IME_NOFILE;
    }

	setvbuf(fp,NULL,_IOFBF,16*1024);

	GifReaderState state(fp);
	
	imageNumber = 1;

	state.verbose = FALSE;
	state.showComment = FALSE;



	ret = state.ReadGIFImage(im, imageNumber);

	fclose(fp);
	return(ret);
}

//
//  GImageMovie
// 

#include "grender.h"
#include "glutils.h"


GImageMovie::GImageMovie()
{
	duration = 0.0;
	stop = FALSE;
	scale2power = FALSE;
	square = FALSE;
	hasAlpha = FALSE;
	supported_fmts = 0;
	sizeX=sizeY=0;
}

void GImageMovie::Release() 
{
	for (int i=0; i<images.Length(); i++) {
			image_free_all(images[i]);
			delete images[i];
			images[i]=0;
	}
	images.Flush();
	times.Flush();
	duration = 0;
}

// binary search 

int GImageMovie::GetFrameNr(float tf)
{

	int t = tf * 1000.0f;
	const int * k= times.Data();
	int n=times.Length();

    if (t<=k[0]) {
        return(0);
    }
    if (t >= k[n-1] || (n == 1) ) {
        return (n-1);
    }

	int l = 0; // left
	int r = n-1;  // right 

    int i; // mid element

	// binary search  terminate if interval below 1
	while ((l+1) < r) {

		i = (l+r) / 2;

		if (t < k[i]) {
			r = i;
			continue;
		}
		else if (t > k[i]) {
			l = i;
			continue;
		}
		else {
			return(i);
		}
	}

	return(l);
}

int GImageMovie::Scale2Power(int maxSizeX,int maxSizeY,gbool square) 
{	int ret=0;
	for (int i=0; i<images.Length(); i++) {
		ret = image_scale_2power(images[i],8,8 /* to do ? */,maxSizeX,maxSizeY,square);
	}
	return ret;
}

int GImageMovie::ConvertRgb(gbool alpha) 
{	int ret=0;
	for (int i=0; i<images.Length(); i++) {
	    if (alpha)
			ret = image_convert_to_rgba(images[i]);
		else 
			ret=image_convert_to_rgb(images[i]);
	}
	return ret;
}

/*
  iout[xdest:width,ydest:height] = iin[xstart:width,ystart:height]
  iout + iin should have same depth
  no error checking !
*/

int static image_copy_trans(image *iin,int xstart,int ystart, int width, int height,
	       image *iout,int xdest,int ydest,BOOL useCkey)
{ IBYTE *o,*i;
  int y,xsize;
  IBYTE key = iin->colorKeyIndex;

  gbool colorKey = useCkey &&(iin->colorKey>0 && iin->fmt == IM_MAPPED8);

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
			for (int j=0; j<xsize;j++) 
				if (i[j]!=key) o[j]=i[j];

		} else memcpy(o,i,xsize);
  }
  return(0);
}

int static image_set(image *iout,int xdest,int ydest, int width, int height,IBYTE color)
{ IBYTE *o;
  int y,xsize;
  IBYTE key = iout->colorKeyIndex;

  gbool colorKey = (iout->colorKey>0 && iout->fmt == IM_MAPPED8);
  colorKey = 0;


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




// read a gif movie 
int 
GifReaderState::ReadGifMovie(GImageMovie &movie)
{

	int ret=0;

	unsigned char	buf[16];
	unsigned char	c;
	unsigned char	localColorMap[3][MAXCOLORMAPSIZE];

	int			hasGlobalColormap=0;
	int			useGlobalColormap;
	int			bitPixel;
	int			imageCount = 0;
	char		version[4];

	image *currentImage=NULL;

	if (! ReadOK(buf,6)) {
		pm_error("error reading magic number" );
		return(IME_BADFILE);
	}

	if (strncmp((char *)buf,"GIF",3) != 0) {       /* hg (char *) cast */
		pm_error("not a GIF file" );
		return(IME_BADFILE);
	}

	strncpy(version, (char *) buf + 3, 3);
	version[3] = '\0';

	if ((strcmp(version, "87a") != 0) && (strcmp(version, "89a") != 0)) {
		pm_error("bad version number, not '87a' or '89a'" );
		return(IME_BADFILE);
	}

	if (! ReadOK(buf,7)) {
		pm_error("failed to read screen descriptor" );
		return(IME_FILEIO);
    }

	gifScreen.Width 	  = LM_to_uint(buf[0],buf[1]);
	gifScreen.Height	  = LM_to_uint(buf[2],buf[3]);
	//Global Color Table Flag       1 Bit
	//Color Resolution              3 Bits
	//Sort Flag                     1 Bit
	//Size of Global Color Table    3 Bits

	gifScreen.GlobalColorTable = BitSet(buf[4], LOCALCOLORMAP);
	gifScreen.ColorResolution = (((buf[4]&0x70)>>3)+1);
	gifScreen.BitPixel	  = 2<<(buf[4]&0x07);
	gifScreen.Background	  = buf[5];
	gifScreen.AspectRatio	  = buf[6];

	
	TRACE("GIF Header Color Res %d %d \n",gifScreen.ColorResolution,((buf[4]&0x70)>>3));

	if (verbose) {
		//pm_message("Width %d Height %d, Bit %d, ColorResolution : %d BackGround : %d  \n",	
		//	GifScreen.Width,GifScreen.Height,GifScreen.BitPixel,GifScreen.ColorResolution,GifScreen.Background); 
		TRACE("GIF: Width %d Height %d, Bit %d, ColorResolution : %d BackGround : %d  \n",	
			gifScreen.Width,gifScreen.Height,gifScreen.BitPixel,gifScreen.ColorResolution,gifScreen.Background); 
    }
	
	if (gifScreen.Width<=0 || gifScreen.Height<=0) {
		pm_error("bad file " );
		return(IME_BADFILE);
	}

	hasGlobalColormap = FALSE;

	if (BitSet(buf[4], LOCALCOLORMAP)) {	/* Global Colormap */
		if (ReadColorMap(gifScreen.BitPixel,gifScreen.ColorMap)) {
			pm_error("error reading global colormap" );
			return(IME_FILEIO);
		}
		hasGlobalColormap = TRUE;
	}

	if (gifScreen.AspectRatio != 0 && gifScreen.AspectRatio != 49) {
		float	r;
		r = ( (float) gifScreen.AspectRatio + 15.0f ) / 64.0f;
		pm_message("warning - non-square pixels; to fix do a 'pnmscale -%cscale %g'",
		    r < 1.0 ? 'x' : 'y',
		    r < 1.0 ? 1.0 / r : r );
	}

	currentImage = new image;

	if (currentImage == NULL ) {
			ret = IME_NOMEM;
			goto exit;
	}

			
	
	image_zero(currentImage);

	ret=image_alloc(currentImage,IM_MAPPED8,gifScreen.Width,gifScreen.Height);

	if (gifScreen.Background>=0) {
		// fill with background color
		memset(currentImage->image,gifScreen.Background,currentImage->bytesperimage);
		//if (BitSet(buf[4], LOCALCOLORMAP)) 
		currentImage->backgroundIndex = gifScreen.Background;
	}

#if 0	
	image_alloc_cmap(currentImage, gifScreen.BitPixel );

	for(i= 0; i< gifScreen.BitPixel; i++) 
	 	image_set_cmap(currentImage,i,gifScreen.ColorMap[CM_RED][i],gifScreen.ColorMap[CM_GREEN][i], gifScreen.ColorMap[CM_BLUE][i]);		
#endif


	for (;;) {

		if (movie.stop) {
			ret = 3;
			break;
		}

		if (! ReadOK(&c,1)) {
			pm_error("EOF / read error on image data" );
			ret = IME_FILEIO;
			break;
		}


		if (c == ';') {         /* GIF terminator */
/*
			if (imageCount < imageNumber)  {
				pm_message("only %d image%s found in file",
					 imageCount, imageCount>1?"s":"" );
				return(IME_FILEIO);
			}
*/			
			ret = IME_FILEIO;
			break;

		}

		if (c == '!') {         /* Extension */
			if (! ReadOK(&c,1)) {
				pm_error("EOF / read error on extension function code");
				ret = IME_FILEIO;
				break;
			}
			DoExtension(c);
			TRACE("\nExtension block disposal %d inputFlag %d delayTime %d transparent %d \n",
				gif89.disposal,gif89.inputFlag,gif89.delayTime,gif89.transparent);
			
			if (imageCount == 0) {
				gif89Global = gif89;
				// take transparent as background HG ??
				if (gif89.transparent>=0) {
					gifScreen.Background= gif89.transparent;
					// fill with background color
					memset(currentImage->image,gifScreen.Background,currentImage->bytesperimage);
				}
			}

			continue;
		}

		if (c != ',') {         /* Not a valid start character */
			pm_message("bogus character 0x%02x, ignoring", (int) c );
			continue;
		}

		++imageCount;

		if (! ReadOK(buf,9))
			pm_error("couldn't read left/top/width/height");

		// buf[8]
		// Local Color Table Flag        1 Bit
		// Interlace Flag                1 Bit
		// Sort Flag                     1 Bit
		// Reserved                      2 Bits
		// Size of Local Color Table     3 Bits


		useGlobalColormap = ! BitSet(buf[8], LOCALCOLORMAP);

		bitPixel = 1<<((buf[8]&0x07)+1);

		TRACE("Image %d globalColormap %d \n",imageCount,useGlobalColormap);

		image *im;
		im = new image;

		if (im == NULL ) {
			ret =  IME_NOMEM;
			break;
		}
		
		image_zero(im);
		int x1,y1,w,h;

		x1 = LM_to_uint(buf[0],buf[1]);
		y1 = LM_to_uint(buf[2],buf[3]);
		w = LM_to_uint(buf[4],buf[5]);
		h = LM_to_uint(buf[6],buf[7]);

		TRACE("Image #%d (%d %d ) %d  %d %d \n",imageCount,
			LM_to_uint(buf[0],buf[1]),LM_to_uint(buf[2],buf[3]),
			LM_to_uint(buf[4],buf[5]),LM_to_uint(buf[6],buf[7]),buf[8]);


       
		if (!useGlobalColormap) {
			if (ReadColorMap(bitPixel, localColorMap)) {
				pm_error("error reading local colormap" );
				ret = IME_FILEIO;
				break;
			}

			ret=ReadImage(im,LM_to_uint(buf[4],buf[5]),
				  LM_to_uint(buf[6],buf[7]), bitPixel, localColorMap,
				  BitSet(buf[8], INTERLACE), 0);
			// im->localColorMap = 1;
		} else {
			ret=ReadImage(im, LM_to_uint(buf[4],buf[5]),
				  LM_to_uint(buf[6],buf[7]), gifScreen.BitPixel, gifScreen.ColorMap,
				  BitSet(buf[8], INTERLACE), 0);
			// im->localColorMap = 1;

		}

		if (ret<0) {
			image_free_all(im);
			delete im;
			im = NULL;
			imageCount--;

			break;
		}
		


		if ((x1 >0) || (y1 >0) || (w <gifScreen.Width)|| (h <gifScreen.Height) || (1 || gif89.disposal>=2) )  // need to copy sub image into copy of prev image
		{
			image *newIm;
			newIm = new image;

			if (newIm == NULL ) {
				ret = IME_NOMEM;
				goto exit;
			}

			
			image *prevIm=currentImage;
		
			image_zero(newIm);

			ret=image_alloc(newIm,prevIm->fmt,prevIm->width,prevIm->height);

			if (ret<0) goto exit;


			//ret = image_scale_copy(prevIm,newIm); 
			//if (ret<0) return(ret);
			if ( prevIm->bytesperimage>0) {
				memcpy(newIm->image,prevIm->image,prevIm->bytesperimage);
			}

	
			ret=image_copy_trans(im,0,0, w,h,newIm,x1,y1,useGlobalColormap /*(gif89.disposal == 0)*/);

			image_free(im);

			im->image = newIm->image;

			image_set_fmt(im,newIm->fmt,newIm->width,newIm->height);
			newIm->image = NULL;
			image_free_all(newIm);
			delete newIm;
#if 0			
useGlobalColormap
			im->colorKey=prevIm->colorKey;
			im->colorKeyIndex = prevIm->colorKey;

			if (im->cmap && im->colorKey && im->colorKeyIndex<im->ncolors) {
				im->colorKeyRgb.r = im->cmap[im->colorKey].r;
				im->colorKeyRgb.g = im->cmap[im->colorKeyIndex].g;
				im->colorKeyRgb.b = im->cmap[im->colorKeyIndex].b;
			}
#endif


		}	
		if ( /*hasGlobalColormap &&  */ (currentImage->bytesperimage>0)  && (gif89.disposal == 2) && gifScreen.Background>=0) { // restore to background color
			// fill with background color
			//memset(currentImage->image,gifScreen.Background,currentImage->bytesperimage);

			image_set(currentImage,x1,y1,w,h,gifScreen.Background);

		}
		else 
		// set the current image 
		if (imageCount == 1 || ( !((gif89.disposal == 0) || (gif89.disposal == 3))))
			ret = image_scale_copy(im,currentImage); 

		if (hasGlobalColormap) { // use
	
			if (gif89Global.transparent>=0) {
				im->colorKey=1;
				im->colorKeyIndex = gif89Global.transparent;


				if (im->cmap && im->colorKey && im->colorKeyIndex<im->ncolors) {
					im->colorKeyRgb.r = im->cmap[im->colorKey].r;
					im->colorKeyRgb.g = im->cmap[im->colorKeyIndex].g;
					im->colorKeyRgb.b = im->cmap[im->colorKeyIndex].b;
				}
			} else im->colorKey=0;
		}	

		//if (ret>=0) 
		image_vflip(im,NULL); 

		TRACE("%d %d %d \n",im->colorKey, im->colorKeyIndex,im->backgroundIndex);


		if (movie.images.Length() == 0) { // first image 
			if (movie.scale2power) 
				ret = image_scale_2power(im,movie.minSizeX,movie.minSizeY,movie.maxSizeX,movie.maxSizeY,movie.square);
			movie.sizeX = im->width;
			movie.sizeY = im->height;
		} else  {
			image_scale(im,movie.sizeX,movie.sizeY);
		}



		movie.images.Append(im);
		movie.times.Append((int) (movie.duration*1000));
		// delayTime ix x/100 sec see gifspec.txt
		if (gif89.delayTime <=0) gif89.delayTime = 10;
		movie.duration += (float) gif89.delayTime / 100.0f;
		im = NULL;
	}

exit:

	if (currentImage) {
		image_free_all(currentImage);
		delete currentImage;
		currentImage = NULL;

	}
	if (imageCount>0 && ret == IME_FILEIO) ret = 2; // keep that what read 
	return(ret);
}


// read gif / animated gif into movie structure
int GImageMovie::ReadGif(const char *fileName)
{

	int	ret;

    /*
     * Find the image file and open it
     */
	FILE *fp = fopen(fileName,"rb");

    if (fp == NULL) {
        TRACE("Cannot find %s.\n", fileName);
        return IME_NOFILE;
    }

	setvbuf(fp,NULL,_IOFBF,16*1024);

	GifReaderState state(fp);
	

	state.verbose = TRUE;


	ret = state.ReadGifMovie(*this);

	fclose(fp);

	return(ret);
}

