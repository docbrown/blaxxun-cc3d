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
#ifndef  _GV_TEXTURE_2_
#define  _GV_TEXTURE_2_

/******************************************************************************
@doc

@module GvTexture2.h - all VRML *Texture nodes|

Copyright (c) 1996 - 1999 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvTexture>
	<c GvImageTexture>
	<c GvPixelTexture>
	<c GvMovieTexture>

Notes:

Changes:
$Revision: 1.15 $
$Log: gvtexture2.h,v $
Revision 1.15  1999/07/06 16:35:38  tom
*** empty log message ***



Todo :

******************************************************************************/


#include <gvsfenum.h>
#include <gvsfbool.h>
#include <gvsffloat.h>
#include <gvsftime.h>
#include <gvsfcolor.h>
#include <gvsfimage.h>
#include <gvsfstring.h>
#include <gvmfstring.h>
#include <gvsfvec2f.h>
#include <gvsfnode.h>
#include <gvmfint32.h>

#include <gvsubnode.h>

#include "grender.h"

class glCache;
class GFile;

#include "www.h"



//class image;

// the root for all texture nodes 
class GvTexture : public GvNode {

    GV_NODE_HEADER(GvTexture);

public:


	//! low level rendering device texture handle
	GTextureRef texture; 
	//! is the low level texture handle ready 
	GvBool Ok() { return(texture.Ok());}

	BEGIN_PACKED_STRUCT //struct {
		GvBool defined:1;	//!< object has been defined
							//!< e.g. cache valid or empty image / url ...
		GvBool hasAlpha:1;	//!< has this texture an alpha channel
		GvBool hasColorKey:1; //!< has this texture an color key (cheaper then alpha)
		GvBool isGrayscale:1; //!< is this a grayscale texture 

	END_PACKED_STRUCT //};
	
	//! has this texture been defined ? e.g. loaded from file or defined from SFImage
	GvBool Defined() { return(defined); }
	
	//! set defined stated
	void SetDefined(GvBool state) { defined = state; }
	

	//! is this a transparent texture ?
	GvBool HasTransparency() { return (hasAlpha); }
	GvBool HasColorKey() { return (hasColorKey); }
	GvBool IsGrayscale() { return (isGrayscale); }

	//! define texture by image data 
	virtual int Define(GglRenderTraversal &state,int glFormat,int width,int height,int components,unsigned char * data);

	//! define texture by image, image might be format converted on return
	virtual int Define(GglRenderTraversal &state,image *im,GvBool checkScale=TRUE);

	//! define texture by SFImage  
	virtual int Define(GglRenderTraversal &state,GvSFImage *image);
	
	//! define texture by reading texture file + create display list , reload if needed
	virtual int Define(GglRenderTraversal &state, LPCTSTR fileName, LPCTSTR fileUrl,int fileFmt,GvBool reload=gfalse);

	//! try to find texture by url in RenderState cache
	virtual int TryDefineByUrl(GglRenderTraversal &state,
				  LPCTSTR fileUrl,
				  GvBool reload=gfalse);

	//! generic define, abstract for GvTexture 
	virtual int Define(GglRenderTraversal &state); 

	int glFlushCache();	//!< flush the cache if any

	// 
	virtual int glSet(GglRenderTraversal &state);
    
	//! low level glSet
	int glSet(GglRenderTraversal &state,GvBool repeatS,GvBool repeatT,GvBool alpha,GvBool matHasAlpha);

	//! call Define(state) if not yet defined			
    virtual int Do(GglRenderTraversal &state);


};

// VRML 2.0
class GvImageTexture : public GvTexture {

    GV_NODE_HEADER(GvImageTexture);

  public:
    // Fields.
    GvMFString url;		// file to read texture from, Default []
	GvSFBool   repeatS;	// Default : TRUE
	GvSFBool   repeatT;	// Default : TRUE

	//GvSFBool     set_unload
    void set_unload(GvSFBool *mode);
	// EventOut
	GvSFBool		isLoaded;		// FALSE,
	// TRUE, url is loaded, FALSE no url could be loaded 


	// try to load if not loaded 
	int Define(GglRenderTraversal &state);
	int glSet(GglRenderTraversal &state);

	GFileRef        loader;			// url loader

	// try to load url
	int Load(GglRenderTraversal &state);
	int Load(GTraversal &state);

	// unload the node
	int Unload();


	int Do(GLoadInlinesTraversal &state);
    int Do(GglRenderTraversal &state);

	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp

	// a field has been changed
	int OnFieldChanged(GvField *field);

};

class GvPixelTexture : public GvTexture {

    GV_NODE_HEADER(GvPixelTexture);

  public:
    // Fields.
    GvSFImage  image;	// The texture if declared inline
	GvSFBool   repeatS;	// Default : TRUE
	GvSFBool   repeatT;	// Default : TRUE

	int Define(GglRenderTraversal &state);

	// call Define(state) if not yet defined
	// or check for lost texture
    int Do(GglRenderTraversal &state);
	
	int glSet(GglRenderTraversal &state);

    GvSFImage* getImage(GTraversal &state);

	// a field has been changed
	int OnFieldChanged(GvField *field);

};
#if 0
class GvTextureFromImage : public GvTexture {

    GV_NODE_HEADER(GvTextureFromImage);

  public:
    // Fields.
    GvSFImage  image;	// The texture if declared inline
	GvSFBool   repeatS;	// Default : TRUE
	GvSFBool   repeatT;	// Default : TRUE

	int Define(GglRenderTraversal &state);
	virtual int glSet(GglRenderTraversal &state);

    GvSFImage* getImage(GTraversal &state);

	// a field has been changed
	virtual int OnFieldChanged(GvField *field);

};
#endif


// forwards
class GImageMovie;
class GDSStream;
class GRMAPlayer;

//


enum GMovieTextureType {
		GMT_NONE,
		GMT_IMAGE,			// single image
		GMT_IMAGE_MOVIE,	// animated gif
		GMT_AMOVIE,			// a  ctive moview / direct show
		GMT_RMA_MOVIE		// real media archictecutre G2
};



class GvMovieTexture : public GvTexture {

    GV_NODE_HEADER(GvMovieTexture);

  public:
    // Fields.
	GvSFBool   loop;		// Default : FALSE
	GvSFFloat  speed;		// Default : 1
	GvSFTime   startTime;	// Default : 0
	GvSFTime   stopTime;	// Default : 0
	GvMFString url;			// Default : []
	GvSFBool   repeatS;		// Default : TRUE
	GvSFBool   repeatT;		// Default : TRUE

	// special EventIns 
	void set_startTime(GvSFTime *t);
	void set_stopTime(GvSFTime *t);
	
	void set_pause(GvSFBool *t); // extra

	// extra non standard, report local media time 
	GvSFTime   mediaTime;	// Default : 0


	// EventOuts
	GvSFTime   duration;
	GvSFBool   isActive;

	// send if isActive event, if a state change occurs 
    void setActive(GvBool state) { if (isActive != state) isActive.set(state); }
    GvBool		isRegistered; // TRUE if node registered in Scene information

	// next time we should check this sensor 
    double nextScheduleTime;
	double last_setStartTime; // data of last set_startTime
	double last_setStartTimeTime;

	// process time 
    int OnTimeChange(double t);
	
	int curFrame;  // the current displayed frame
	int numFrames;	// number of frames in video
	double framesPerSec;	// how much frames per sec

	double cycleInterval; // == numFrame  / fps

	int ComputeFrameNr( double t);

	// compute the current requested frame number
	int SetCurrentFrame(double t);

	// release MovieTexture resources
	virtual void Term();

	// release surface because device eg has been changed 
	void ReleaseSurface();

	// Define texture by reading texture file + create display list 
	virtual int Define(GglRenderTraversal &state, LPCTSTR fileName, LPCTSTR fileUrl, int fileFmt);

	// try to load if not loaded 
	int Define(GglRenderTraversal &state);
	int glSet(GglRenderTraversal &state);

	// file loader
	GFileRef        loader;			// url loader

	GvBool isRmaUrl(const char *url);


	// try to load url
	int Load(GglRenderTraversal &state);
	int Load(GTraversal &state);

	int Do(GLoadInlinesTraversal &state);
    int Do(GglRenderTraversal &state);

	// a field has been changed
	int OnFieldChanged(GvField *field);

	// the type of movie
	int movieType;

	// Animated GIF
	GImageMovie * imageMovie;
	int displayedFrame;
	
	// active movie 
	GDSStream	*dsMovie;

	BOOL updateVideo;	// flag that a new frame is pending 
	GvBool updateAudio;

	// RMA
	GRMAPlayer	*rmaPlayer;
	
	GvBool waitingForFirstFrame; // show splash screen before initial buffering


};


#ifdef _G_VRML1


// VRML 1.0
class GvTexture2 : public GvTexture {

    GV_NODE_HEADER(GvTexture2);

  public:
    enum Wrap {				// Texture wrap type
	    REPEAT,
	    CLAMP,
    };

    // Fields.
    GvSFString		filename;	// file to read texture from
    GvSFImage		image;		// The texture if declared inline
    GvSFEnum		wrapS;
    GvSFEnum		wrapT;


  	// push attribute on stack
    int Do(GTraversal &state);					     
    int Do(GglRenderTraversal &state);					     

	int Do(GLoadInlinesTraversal &state);
	int Do(GioTraversal &state);

	int Do(GDisplayTreeTraversal &state); // implemented in GDisplayTree.cpp


 	GFileRef        loader;			// url loader

	// try to load url / file 
	int Load(GglRenderTraversal &state); 

	int Define(GglRenderTraversal &state);

	int glSet(GglRenderTraversal &state);

	// a field has been changed
	int OnFieldChanged(GvField *field);

    // something changed 
	int OnChanged();

};
#endif _G_VRML1


class GvImage : public GvImageTexture {

    GV_NODE_HEADER(GvImage);

  public:
    // Fields.
    //GvMFString url;		// file to read texture from, Default []
	//GvSFBool   repeatS;	// Default : TRUE
	//GvSFBool   repeatT;	// Default : TRUE

   
    GvSFFloat       alpha;  // default 1.0 alpha Value of image if not RGBA
    GvSFColor       alphaColor; // default black, alpha comparison color 

    GvSFImage		image;		// The texture if declared inline



	int Load(GTraversal &state);

    // get pointer to image,
    // load it if needed
    // null if image not loaded or loader still busy
    virtual GvSFImage* getImage(GglRenderTraversal &state);
    virtual GvSFImage* getImage(GTraversal &state);


	// a field has been changed
	virtual int OnFieldChanged(GvField *field);
	// something changed 
	virtual int OnChanged();

    // Apply Alpha rule onto image 
    virtual int ApplyAlpha();

    // Read image file into SF Image 
	virtual int Define(GglRenderTraversal &state, LPCTSTR fileName, LPCTSTR fileUrl, int fileFmt,GvBool reload=gfalse);
	virtual int Define(GTraversal &state, LPCTSTR fileName, LPCTSTR fileUrl, int fileFmt,GvBool reload=gfalse);
	virtual int Define(GglRenderTraversal &state,GvSFImage *theImage);
    
    virtual int Do(GglRenderTraversal &state);					     
};


class GvScaledImage : public GvImage {

    GV_NODE_HEADER(GvScaledImage);

  public:
    // Fields.
    //GvSFString		filename;	// file to read texture from
    //GvSFImage		image;		// The texture
    
    //GvSFFloat       alpha;  // default 1.0 alpha Value of image if not RGBA
    //GvSFColor       alphaColor; // default black, alpha comparison color 

    enum ScaleMode {
            NONE = 0,
            TO_WINDOW=1,
            TO_SIZE=2,
            BY_FACTOR=3,
            //TILE_SIZE=4
    };

    GvSFNode         srcImage;
    
    GvSFEnum         scale;
    GvSFVec2f        scaleFactor;
	GvSFBool		 filter; // Filtered scale


#if 0

  	// push attribute on stack
    int Do(GTraversal &state);					     
    int Do(GglRenderTraversal &state);					     

	int Do(GLoadInlinesTraversal &state);
	int Do(GioTraversal &state);
	int Do(GDisplayTreeTraversal &state); // implemented in GDisplayTree.cpp

	// try to load url / file 
	int Load(GglRenderTraversal &state); // in GvTraverse

#endif

    // Apply Scale rule onto image 
    virtual int ApplyScale(GTraversal &state);


    // Read image file into SF Image 
    virtual int Define(GglRenderTraversal &state, LPCTSTR fileName, LPCTSTR fileUrl, int fileFmt,GvBool reload=gfalse);	
	virtual int Define(GTraversal &state, LPCTSTR fileName, LPCTSTR fileUrl, int fileFmt,GvBool reload=gfalse);

	virtual int Define(GglRenderTraversal &state,GvSFImage *theImage);
    
    // Read image file into SF Image 
//    virtual int CreateTextureFromFile(GglRenderTraversal &state,const char *fileName);
//    virtual int CreateTextureFromImage(GglRenderTraversal &state);


	// a field has been changed
	int OnFieldChanged(GvField *field);
	// something changed 
	int OnChanged();

    int Do(GglRenderTraversal &state);					     

};

// experiment
class GvMultiTexture : public GvTexture {

    GV_NODE_HEADER(GvMultiTexture);

  public:
    // Fields.
    GvMFString mode;	//!< the mode of each textures

    GvMFNode texture;	//!< list of textures
    GvMFNode textureTransform;	//!< list of textures transform

    GvMFInt32 textureOp;		//!< low level op codes 

	//! set texture i as the main texture
	int glSet(GglRenderTraversal &state,int index);
	
	int glSet(GglRenderTraversal &state);

    int Do(GglRenderTraversal &state);					     

};



#endif /* _GV_TEXTURE_2_ */
