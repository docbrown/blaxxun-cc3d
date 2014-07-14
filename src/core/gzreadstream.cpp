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

@module GZReadStream.cpp - streaming unzip 	|

Copyright (c) 1997	by Holger Grahn
All rights reserved

Purpose:

Classes:


Notes:
	current implementation based on zlib 

Changes:

Todo :



******************************************************************************/

#include "stdafx.h"
#include <assert.h>


#include "zlib/zlib.h"
#include "gunzip.h"

#include "gzreadstream.h"


#define UNGET_SIZE 16 

GZReadStream::GZReadStream()
{
  eof = TRUE;
  error = FALSE;
  errorNumber = 0;
  errorMsg = NULL;

  bufsize = 16*1024;
  buf = NULL;
  in = NULL;
  bufReadPtr = bufEndPtr = buf;

}

GZReadStream::~GZReadStream()
{
	if (buf) free(buf);
	eof = TRUE;
}

//gzopenFromFile  (FILE *file,const char *mode);


// opens the file, and prepares for streaming unzip 
int GZReadStream::Open(const char *infile) 
{


	FILE * f= fopen(infile,"rb");
	if (!f) return -1;

    // check if its really zipped 
	int c1,c2;
	c1 = fgetc(f);
	c2 = fgetc(f);
	fseek(f,0,SEEK_SET);
	if (c1==0x1F && c2==0x8B) {
		isZipped = TRUE;
	}
	else 
	  isZipped = FALSE;

	return Open(f,isZipped);

/*
    in = gzopen(infile, "rb");


    if (in == NULL) {
        // if (reportErrors) Reporter.Error("gzip uncompress: can't gzopen %s", infile);
		eof = TRUE;
        return -1;
    }
		this->isZipped = TRUE;

	if (buf == NULL) {
		buf = (char *) calloc(sizeof(char) ,bufsize);
	}

	eof = FALSE;
	bufReadPtr = bufEndPtr = buf;
	return 0;
*/
}

// opens the file, and prepares for streaming unzip 
int GZReadStream::Open(FILE *infile,BOOL isZipped) 
{

	if (!isZipped) {
		this->isZipped = FALSE;
	} else {

		in = gzopenFromFile(infile, "rb");


		if (in == NULL) {
			// if (reportErrors) Reporter.Error("gzip uncompress: can't gzopen %s", infile);
			eof = TRUE;
			return -1;
		}
		this->isZipped = TRUE;
	}

	if (buf == NULL) {
		buf = (char *) calloc(sizeof(char) ,bufsize+UNGET_SIZE);
	}
	
	this->inFile = infile;

	bufReadPtr = bufEndPtr = buf;
	
	if (buf == NULL)
		return -1;

	eof = FALSE;
	return 0;
}


int GZReadStream::Close() {
	int ret=0;

	if (!isZipped) {
		// we need to close the infile 
		if ( inFile != NULL) {
			fclose(inFile);
			inFile = NULL;
		}
	}

    if (in && gzclose(in) != Z_OK) { // gzclose will close infile for now
		// if (reportErrors) error("failed gzclose");
	}
	in = NULL;
	return 0;
}

// get a char at end of buffer
int GZReadStream::getcBuf() 
{
	if (eof) return EOF;
	
	int len;

	if (!isZipped) {
	   len = fread(buf+UNGET_SIZE, sizeof(char) ,bufsize,inFile);
	}
	else 
		len = gzread(in, buf+UNGET_SIZE, bufsize);


	if (len < 0) { 
		//if (reportErrors) error (gzerror(in, &err));
		if (isZipped) {
			int err;

			const char *msg=gzerror(in, &err);
			if (err < Z_OK) {
				if (err == Z_ERRNO) {
					err = errno;
					msg = strerror(err);
				}
				error = TRUE; errorMsg = msg; errorNumber = err;
				TRACE("gzip file:Error %s (%d) \n",msg,err);
				
			}
		}
		eof = TRUE;
		return EOF;
	}
    if (len == 0) {
		if (isZipped) {
			int err;
			const char *msg=gzerror(in, &err);
			if (err < Z_OK) {


				if (err == Z_ERRNO) {
					err = errno;
					msg = strerror(err);
				}
				error = TRUE; errorMsg = msg; errorNumber = err;
				TRACE("gzip file:Error %s (%d) \n",msg,err);
			
			}
		}

		eof = TRUE;
		return EOF;
	}
	else {
		bufReadPtr = buf+UNGET_SIZE;
		bufEndPtr = bufReadPtr+len;
		return *bufReadPtr++;
	}

}
