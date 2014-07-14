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
#ifndef  _GV_TEXTURE_2_TRANSFORM_
#define  _GV_TEXTURE_2_TRANSFORM_

#include <gvsffloat.h>
#include <gvsfvec2f.h>
#include <gvsubnode.h>

#ifdef _G_VRML1


class GvTexture2Transform : public GvNode {

    GV_NODE_HEADER(GvTexture2Transform);

  public:
    // Fields
    GvSFVec2f		translation;	// Translation vector
    GvSFFloat		rotation;		// Rotation
    GvSFVec2f		scaleFactor;	// Scale factors
    GvSFVec2f		center;	        // Center point for scale and rotate

    signed char changed;    // set to 1 if any field has changed 

	int Do(GTraversal &state);
	int Do(GglRenderTraversal &state);

	// something changed 
	int OnChanged();

	
	int isDefault() { return(0); }

	// get current transformation in matrix form
	void Get(Matrix &m);

	// apply setting to renderstate
	int Apply(GglRenderTraversal &state);

	unsigned long mVersion;	  // D3D: need to give each Matrix a distinctive version
							  // because matrix need to applied directly to objects 	



};
#endif


class Gv2TextureTransform : public GvNode {

    GV_NODE_HEADER(Gv2TextureTransform);

  public:
    // Fields
	GvSFVec2f center;	// Default : 0 0
	GvSFFloat rotation;	// Default : 0
	GvSFVec2f scale;	// Default : 1 1
	GvSFVec2f translation;	// Default : 0 0


    signed char changed;    // set to 1 if any field has changed 

  private:	
	
	Matrix 	transformCache;         // computed matrix for above fields
	GvBool transformCached;         // has matrix been computed ?   
  
  public:

	// push on stack
	int Do(GTraversal &state);

    int Do(GOptimizeTraversal &state);		// implemented in GOptimizeTraversal.cpp

	// something changed 
	int OnChanged();

	
	int checkForDefaultValues(int mode);

	// get the texture matrix 
	void get(Matrix &m);


    // get matrix, recompute if necessary
    inline Matrix& Transform() { 
            if (!transformCached) { get(transformCache); transformCached = 1; } 
            return(transformCache);        
    }


	// apply setting to renderstate
	int Apply(GglRenderTraversal &state);


  private :

	unsigned long mVersion;	  // D3D: need to give each Matrix a distinctive version
							  // because matrix need to applied directly to objects 	

};


#endif /* _GV_TEXTURE_2_TRANSFORM_ */
