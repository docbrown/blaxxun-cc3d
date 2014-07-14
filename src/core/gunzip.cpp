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

@module GUnzip.cpp - GLView ZLIB unzipping interface|

Copyright (c) 1997	by Holger Grahn
All rights reserved

Purpose:

Classes:


Notes:


Todo :
	think about unzip stream to unzip during file reading

******************************************************************************/



/* based on minigzip.c -- simulate gzip using the zlib compression library
 * Copyright (C) 1995-1996 Jean-loup Gailly.
 * For conditions of distribution and use, see copyright notice in zlib.h 
 */

/*
 * minigzip is a minimal implementation of the gzip utility. This is
 * only an example of using zlib and isn't meant to replace the
 * full-featured gzip. No attempt is made to deal with file systems
 * limiting names to 14 or 8+3 characters, etc... Error checking is
 * very limited. So use minigzip only for testing; use gzip for the
 * real thing. On MSDOS, use only on file names without extension
 * or in pipe mode.
 */

/* $Id: gunzip.cpp,v 1.2 1997/11/07 11:28:07 bernd Exp $ */

#include "stdafx.h"


#include <stdio.h>
#include "zlib/zlib.h"

#include "greporter.h"



#ifdef VMS
#  define unlink delete
#  define GZ_SUFFIX "-gz"
#endif
#ifdef RISCOS
#  define unlink remove
#  define GZ_SUFFIX "-gz"
#  define fileno(file) file->__file
#endif

#ifndef GZ_SUFFIX
#  define GZ_SUFFIX ".gz"
#endif
#define SUFFIX_LEN sizeof(GZ_SUFFIX)


//#define BUFLEN 4096
//#define MAX_NAME_LEN 1024

#define BUFLEN (4096*4)
#define MAX_NAME_LEN _MAX_PATH

#define local static

/* For MSDOS and other systems with limitation on stack size. For Unix,
    #define local
   works also.
 */



/* ===========================================================================
 * Display error message 
 */
static void error(const char * msg)
{
    CString tmpMsg = "";
    tmpMsg = msg;
    if ( tmpMsg.Find("failed") >= 0 && tmpMsg.Find("close") >= 0 )
        return;
    Reporter.Error("Unzip-Error: %s\n",  msg);
}



/* ===========================================================================
 * Uncompress input to output then close both files.
 */
int  gz_uncompress( gzFile in, FILE   *out,BOOL reportErrors)
{
    local char buf[BUFLEN];
    int len;
    int err;
	int ret = -1;

    for (;;) {

        len = gzread(in, buf, sizeof(buf));

        if (len < 0) { 
			if (reportErrors) error (gzerror(in, &err));
			ret = -1;
			break;
		}
        if (len == 0) {
			ret = 0;
			break;
		}

        if ((int)fwrite(buf, 1, (unsigned)len, out) != len) {
			ret = -1;
			if (reportErrors) error("failed fwrite");
			break;
		}
    }

    if (fclose(out)) {
		if (reportErrors) error("failed fclose");
		ret = -1;
		
	}

    if (gzclose(in) != Z_OK) {
		if (reportErrors) error("failed gzclose");
	}
	return ret;
}


/* ===========================================================================
 * Uncompress the given file and remove the original.
 * ret <0 error, Error Message reported if reportErrors 
 * ret >=0 ok,
 */
int gz_file_uncompress(char *file,BOOL reportErrors)
{
	int ret=0;

    local char buf[MAX_NAME_LEN];
    char *infile;
	char *outfile;
    FILE  *out;
    gzFile in;
    int len = strlen(file);

    strcpy(buf, file);

    if (len > SUFFIX_LEN && strcmp(file+len-SUFFIX_LEN, GZ_SUFFIX) == 0) {
        infile = file;
        outfile = buf;
        outfile[len-3] = '\0';
    } else {
        outfile = file;
        infile = buf;
        strcat(infile, GZ_SUFFIX);
    }
    in = gzopen(infile, "rb");
    if (in == NULL) {
        if (reportErrors) Reporter.Error("gzip uncompress: can't gzopen %s", infile);
        return -1;
    }
    out = fopen(outfile, "wb");

    if (out == NULL) {
		if (reportErrors) Reporter.Error("gzip OpenError for %s \n%s", outfile,strerror( errno ));
        return -1;
    }

    ret = gz_uncompress(in, out,reportErrors);

    unlink(infile);
	return ret;
}


/* ===========================================================================
 * Uncompress the given infile to outfile 
 * ret <0 error, Error Message reported if reportErrors 
 * ret >=0 ok,
 */
int gz_file_uncompress(const char *infile,const char *outfile,BOOL reportErrors)
{
	int ret=0;

    FILE  *out;
    gzFile in;

    in = gzopen(infile, "rb");

    if (in == NULL) {
        if (reportErrors) Reporter.Error("gzip uncompress: can't gzopen %s", infile);
        return -1;
    }

    out = fopen(outfile, "wb");

    if (out == NULL) {
		if (reportErrors) Reporter.Error("gzip OpenError for %s \n%s", outfile,strerror( errno ));
        return -3;
    }

    ret = gz_uncompress(in, out,reportErrors);

	return ret;
}


