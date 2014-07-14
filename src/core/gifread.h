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

#ifndef _GifRead_h
#define _GifRead_h


// read single gif image
int ReadGif(image *im, const char *fileName);

#include "array.h"

typedef image *imagePtr;

//
//  GImageMovie
// 
class GImageMovie {
public:
	Array<imagePtr> images;		// array of images structes
	
	IntArray		times;		// corresponding times in milli-seconds
	float			duration;	// totatl duration in seconds

	// size of the images 
	int     sizeX,sizeY;

	//
	gbool	stop;

	// conversion flags 
	gbool	hasAlpha;
	gbool	scale2power;
	gbool	square;
	int     minSizeX,minSizeY;
	int     maxSizeX,maxSizeY;
	int		supported_fmts;

	// get number of frames
	int NumFrames() { return images.Length(); }

	// get duration in seconds
	float GetDuration() { return duration;} 

	// get frame number for local time	
	int GetFrameNr(float t); 

	// free all images & reset variables
	void Release();
	
	GImageMovie();

	~GImageMovie() { Release(); }

	// limit all images to maxSize, and scale to power of 2
	int Scale2Power(int maxSizeX=256,int maxSizeY=256,gbool square=FALSE); 

	// convert to RGB / RGBA
	int ConvertRgb(gbool alpha= FALSE);  

	// read gif / animated gif into movie structure
	int ReadGif(FILE *fd);
	int ReadGif(const char *fileName);

};




#endif


