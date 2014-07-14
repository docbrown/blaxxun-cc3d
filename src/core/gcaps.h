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


// capability query
#ifndef _gcaps_h
#define _gcaps_h

// capability definition for Browser.getCap(x) call


enum GCaps {

	GC_OPENGL=0,				// 1 if using OpenGL Renderer
	GC_DIRECT3D=1,			//!< 1 if using Direct3D Renderer

	GC_IS_HARDWARE=2,		//!< 1 if hardware rendering is used

	GC_TOTAL_VIDEO_MEMORY,	//!< amount of total video memory
	GC_FREE_VIDEO_MEMORY,	//!< amout of free video memory

	GC_RGB_LIGHTING,		//!< 1 if true RGB ligthing support, 0 for ramp mode
	GC_ALPHA_BLENDING,		//!< 1 if alpha blending supported
	
	GC_MIPMAPPING,			//!< device can do mip-mapping 

	GC_MAX_TEXTURESIZE_X,	//!< maximum texture size x
	GC_MAX_TEXTURESIZE_Y,
	GC_TEXTURE_SQUARE,		//!< 1 if only square textures supported

	GC_MAX_LIGHTS,			//!< max number of lights supported

	// 4.3
	GC_FREE_TEXTURE_MEMORY,	//!< amout of free texture memory
	GC_TOTAL_TEXTURE_MEMORY,//!< amout of total texture memory

	GC_FREE_PHYSICAL_MEMORY,	//!< amout of free memory in KB
	GC_TOTAL_PHYSICAL_MEMORY,	//!< amout of total memory in KB
	GC_USED_PHYSICAL_MEMORY,	//!< amout of used memory in %


	GC_USING_TEXTURES,			//!< currently using textures
	GC_USING_MOVIES,			//!< currently using textures
	GC_USING_SOUND,				//!< currently using sounds
	GC_HAS_SOUND,				//!< currently has sound device 
	GC_USING_BACKGROUND,		//!< currently using background

	GC_CURRENT_MAX_TEXTURESIZE_X,	//!< maximum texture size x
	GC_CURRENT_MAX_TEXTURESIZE_Y,

};

// return value might be -1 if value currently unknown

#endif
