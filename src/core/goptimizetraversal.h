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
// GOptimizeTraversal.h
#ifndef _GOptimize_H
#define _GOptimize_H

#include <gvsfnode.h>
#include <gvmfnode.h>
#include <gvsffloat.h>
#include <gvmffloat.h>
#include <gvsfvec2f.h>
#include <gvmfvec2f.h>
#include <gvsfvec3f.h>
#include <gvmfvec3f.h>
#include <gvsfcolor.h>
#include <gvmfcolor.h>
#include <gvsfrotation.h>
#include <gvmfrotation.h>


class GvImageTexture;
class GvCoordinate;
class GvTextureCoordinate;
class GvNormal;
class GvColor;
class Gv2IndexedFaceSet;
class GvInterpolator;


// rounding support 
inline float GRound(float x,float scale,float xscale)
{ return ((int) (x*xscale))*scale; }

Point GRound(const Point &p,float scale,float xscale);

inline void GRoundEq(Point &p,float scale,float xscale)
{	p.x =GRound(p.x,scale,xscale),
	p.y=GRound(p.y,scale,xscale),
	p.z=GRound(p.z,scale,xscale); 
}


void GRound(PointArray &p,float scale,float xscale);
void GRound(GvMFVec3f &p,float scale,float xscale);

//
//	GOptimizeTraversal
//
// traverse graph to build shell for all geometry nodes
class GOptimizeTraversal : public GRenderTraversal 
{
public :
	RT(GOptimizeTraversal);

	GOptimizeTraversal();
	
	~GOptimizeTraversal();

	GvMFNode result;
	int resultMask;

	enum MASK {
		HAS_UNKNOWN_NODE=1,
		HAS_SHAPE=2,
		HAS_GROUP=4,
		HAS_ROUTES=8,
		KEEP_GROUP=8, // if group contains GEOMETRY-SENSOR
		KEEP_TRANSFORM=16, // keep if group contains non transformable geometry
		TRANSFORM_APPLIED=32, // TRANSFORM has been applied to object
		ANCHOR_APPLIED=64	  // ANCHOR // SENSORS has been applied to applied 

	};

	enum INTERPOLATOR {
			IP_KEY,

			IP_SCALAR,
			IP_POSITION,
			IP_ORIENTATION,
			IP_COLOR,
			IP_NORMAL,

			IP_NURBS_POSITION,
			IP_MAX
	};

	GvBool ignoreRoutes;
	GvBool removeNodeNames;
	GvBool doInlines;
	GvBool applyTransforms;
	
	GvBool applyTexTransform;

	GvBool expandAnchors;
	GvBool expandLods;

	GvBool updateBBox; // update BBoxes of converted / created Groups 
	GvBool buildBspTree;	// build bsp tree

	GvBool keepPrimitives; // keep non IFS VRML primitives

	GvBool decomposeIfs; // decompose IFS
	GvBool compressIfs; // compress IFS

	GvBool optimizeIfs; // optimize IFS
	
	GvBool expandFcIfs; // expand IFS with face colors
	GvBool unlitTextures; // make textured appearances unlit
	

	GvBool bx3d;		// convert to blaxxun3d
	GvBool vrml2;		// convert to VRML2

	// rounding values factor value e.g. 0.01 // for 3 digits 
	GvBool doCoordRound;
	double coordRound;

	GvBool doNormalRound;
	double normalRound;

	GvBool doTexCoordRound;
	double texCoordRound;

	GvBool doColorRound;
	double colorRound;

	GvBool doIpRound[IP_MAX];
	double ipRound[IP_MAX];
	
	GvBool doIpCompress;

	GvBool doLookupCoord; // share Coordinate TexCoord Color etc
	GvBool usePolygon; // use extra Polygon primitive

	// round to nearest value , ok for negative numbers ?
	double round(double value,double factor)  {	 return floor((value/factor)+0.5) * factor;}

	void RoundCoord(GvMFVec3f &v);
	void RoundCoord(GvSFFloat &v);
	
	void RoundTexCoord(GvMFVec2f &v);

	void RoundNormal(GvMFVec3f &v);
	void RoundNormal(GvSFRotation &v);
	void RoundNormal(GvMFRotation &v);

	void RoundColor(GvSFColor &v);
	void RoundColor(GvMFColor &v);

	void CompressInterpolator (GvInterpolator *ip);

	GvBool includeInline;		  // inline exapanded	
	GvBool includeExternProtos;	  // externProto ==> PROTO
	GvBool includeImageTextures; // ImageTexture ==> PixelTexture
	
	float smoothCreaseAngle; // 3.14
	float  decomposeVolumeLimit; // a volume limit 


	GvMFNode imageTextures;
	GvMFNode materials;
	GvMFNode appearances;
	GvMFNode coordinates;
	GvMFNode textureCoordinates;
	GvMFNode normals;
	GvMFNode colors;

	// set of nodes, with animation, kee
	GvMFNode animatedTransforms;


	// empty result 
	void FlushResult() { result.deleteAll(); resultMask = 0; }

	// append a resulting node to result list 
	// check for transforms and anchors 
	void AppendResult(GvNode *node,int mask=0);

	// return node 
	int ReturnNode(GvNode *node,int mask=0);

	// return node 
	int ReturnReplacedNode(GvNode *oldNode,GvNode *newNode,int mask=0);

	// set result 
	void SetResult(GvMFNode &node,int mask=0) { result.set(node); resultMask = mask; }

	// store result as single node, and return mask
	int GetResult(GvNodeHandle &ret);

	GvNode* BuildAnchorOrSensorGroup(GvNode *n,int &mask);



	// store result into SFNode, and return mask
	int GetResult(GvSFNode &ret) {
			GvNodeHandle n;
			int retMask =  GetResult(n);
			ret.set(n);
			if (n == (GvNode*) NULL) ret.setDefault(TRUE);
			return(retMask);
	}

	// append result to MFNode, and return mask
	// includes BSP tree building 

	int GetResult(GvMFNode &ret);

	int GetResultBsp(GvMFNode &ret);

	// true if node should go in bsp optimizer 
	GvBool UseNodeForBsp(GvNode *n);

	// do we need to keep the group e.g. if it contains
	// geometry sensors or directional lights 
	GvBool NeedGroup(GvMFNode &in);



	// do we have to create a transform node for result
	GvBool NeedTransform();
	// create transform node and store nodes at children 
	GvNode* MakeTransform(GvMFNode &nodes);

	GvNode* MakeGroup(GvMFNode &nodes);

	// do we have to create a anchor node for result
	GvBool NeedAnchor();
	GvNode* MakeAnchor(GvMFNode &nodes);


	GvBool checkForBx3dNode(GNode *n);

	// check if appearance already defined, and return looked up appearance
	GvBool Lookup(GvAppearance *old,GvAppearance **new_);
	GvBool Lookup(GvImageTexture *old,GvImageTexture **new_);
	GvBool Lookup(GvCoordinate *old,GvCoordinate **new_);
	GvBool Lookup(GvTextureCoordinate *old,GvTextureCoordinate **new_);
	GvBool Lookup(GvNormal *old,GvNormal **new_);
	GvBool Lookup(GvColor *old,GvColor **new_);

	GvBool LookupCoord(Gv2IndexedFaceSet *ifs);



	int Do(GNode *n) {	
		if (bx3d) if (!checkForBx3dNode(n)) return 0;
		return (n->Do(*this)); 
	}

   	void InitializeAttributes(); 

	// process MFNode field and append all results 
	int Do(GvMFNode &n);

	// process MFNode field and append all results to result
	int Do(GvMFNode &n,GvMFNode &result,GvBool buildBsp=FALSE);

	int Do(GvSFNode &n,GvSFNode &result);

	int Do(GvSFNode &n,GvMFNode &result);

};


// tools
class Gv2IndexedFaceSet;
class GShell;
class GShellI;


// create a IndexedFaceSet out of shell, wrap == TRUE, flip faces
// optional transformation and texture matrix 

Gv2IndexedFaceSet* CreateIndexedFaceSet(GShell *s, GvBool wrap = FALSE,  Matrix *m= NULL,Matrix * tm= NULL,GvBool isSolid= TRUE);
Gv2IndexedFaceSet* CreateIndexedFaceSet(GShellI *s, GvBool wrap = FALSE,  Matrix *m= NULL,Matrix * tm= NULL,GvBool isSolid= TRUE,GvBool bx3d=FALSE);

Gv2IndexedFaceSet* NewIfsFromFace(GTraversal &state,GShell *s, int fcnt, const int fp[], int faceNum, 
								  GvBool wrap = FALSE, GvBool solid = TRUE, Matrix *m= NULL,Matrix * tm= NULL);

Gv2IndexedFaceSet* NewIfsFromFace(GTraversal &state,GShellI *s, int fcnt, const int fp[], int faceNum, 
								  GvBool wrap = FALSE, GvBool solid = TRUE, Matrix *m= NULL,Matrix * tm= NULL);

// expand object into single faces IFS
GvBool ExpandIndexedFaceSet(GTraversal &state,GShell *s, GvBool wrap, GvBool solid,  Matrix *m,Matrix * tm, GvMFNode &result);

// create a new material node, and optionally copy settings from m
Gv2Material * NewMaterial(Gv2Material *m);

// check for VRML 1 shapes & wrap into Shape
void Shapify(GOptimizeTraversal &state,GvNode *result,int mask);


/*
  split shell with fc into separate sets 
  need root material

*/
gbool ExpandFaceColors(GShellI *s, 
					  // for appearance creation 
					  Gv2Material *mat,GvNode *texture,GvNode *textureTranform,
					  
					  gbool solid,gbool flip,
					  
					  gbool replaceEmmissive,
					  GvMFNode &result);




#endif

