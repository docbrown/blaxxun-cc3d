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
// GRaySelect.h
#ifndef _GRaySelect_H
#define _GRaySelect_H
/******************************************************************************
@doc

@module GRaySelect.h - Ray intersection traversal action|

Copyright (c) 1995 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GRaySelectTraversal>

Notes:


Changes:
Started Rapid Collision detection

$Revision: 1.7 $
$Log:

Todo :

******************************************************************************/


#include <arraynew.h>

#include <gvpath.h>

class GShell;

class GConvexVolume;


//
//! stores information about ray intersection with object
//

class GHitInfo
{
public :
	GNode * node;		//!< node intersected
	GvPath path;		//!< path to node 

	float t;			//!< ray parameter
	Point p;			//!< intersect position
	Point pw;			//!< intersect position world
	
	BBox  bbox;			//!< bounding box of node
	
	int	vindex;			//!< intersected vertex index
	int	findex;			//!< intersect face index
    int tvindex0,tvindex1,tvindex2; //!< vertex indices for triangle
    
	float tu,tv;        //!< barycentric coordinates for triangle

	Point vn;			//!< vertex normal // only if tu,tv valid (in local space)
	Point vc;			//!< vertex color
	Point vp;			//!< vertex paramter

    //float ta,tb,tc;   //!< weight factors for triangles 

	GNode *wwwAnchor;	//!< wwwAnchor / touchSensor .., this node is a child of
	GNode *wwwInline;	//!< wwwInline, this node is a child of
	GNode *ownerNode;   // the node owning node
	GNode *ownerGroupNode; // the group nodes owning ownerNode
	GNode *relativeTo;  // 

	ArrayNew <GvNode*> sensors;
	ArrayNew <GvNode*> sensorParents;

	// search node in path, and retreive effective matrix of sensor
	GvBool GetSensorMatrix(GvNode *sensor,MatrixAndInverse &m);
	GvBool GetSensorMatrix(GvNode *parent,GvNode *sensor, MatrixAndInverse &mret);


	// for mouse sensor find first ProtoInstance or inline after group
	// containing sensor, otherwise use lowes level node
	
	GvBool GetTargetNode(GvNode *sensor,MatrixAndInverse &mret,GvNodeHandle &target);
	
	// if needed for layer support
	//GRectInt	viewport; 

	
	Matrix m;			// modelling matrix
	Matrix mInverse;    // the inverse modelling matrix is cached for rapid

	GHitInfo() { node = NULL; t=0.0; wwwAnchor= NULL; wwwInline = NULL; ownerNode = NULL; ownerGroupNode = NULL; relativeTo = NULL; findex = -1;}
	~GHitInfo();

};

class rapidCollide;
//
//	GRaySelectTraversal
//
// traverse graph to do selection for all geometry nodes
class GRaySelectTraversal : public GPathTraversal 
{
public :
	RT(GRaySelectTraversal);

	GRaySelectTraversal() : GPathTraversal() 
		{  
			method = BY_BBOX; 
			mode = MODE_SELECTION;
			selectWWWAnchorChildsOnly=0;
			selectNearestOnly=1;
			storeFullHitInfo = 0;
			storePath = 0;
			currentOwnerNode = NULL;
			currentGroupNode = NULL;
			needShapes = TRUE; // assume we need them 

		}
	
	~GRaySelectTraversal();
	
	//! mouse/ray screen position (in pixel)
	Point		pscreen;
	
	//! current viewport (in pixel)
	GRectInt	viewport; 

	void SetViewport(GRectInt &newVp) { viewport=newVp; }
	const GRectInt &GetViewport() { return viewport; }

	
	//! the ray for doing ray-intersection
	RayRange	ray; 

	BBox cameraBBox;

	gbool selectWWWAnchorChildsOnly;
	gbool selectNearestOnly;
	
	//! store all available hit information 
	gbool storeFullHitInfo;

	//! store the path to the hitted  node 
	gbool storePath;

	//! alls hits, hit[0] is nearest 
	ArrayNew<GHitInfo> hits;

	GNode *currentOwnerNode;   // the node owning node
	GNode *currentGroupNode; // the group nodes owning ownerNode


	//GHitInfo nearest;
	//float nearestT;

	// for collission detection

	// rapid
	// pointers are set & owned by GView 
	rapidCollide *myRapid;
	GShell* speedBox, *speedBox2;
	GShell* speedBoxGlobal;	// used for transforming speedbox
	Matrix speedBoxMat;

	gbool collision;	// if true, there was a collision 

	//used in collisionDetectRecursive

	//GShell* collideShell;
	//Matrix collideMatrix, collideMatrixInverse;

	int colCyclesPerFrame;
	Point collisionDistance;

    // a convex volume for doing bbox checks in RAPID mode		
	GConvexVolume *speedBoxVolume; // in global coordinates


	enum Method {
		BY_BBOX,		// use only the bounding box of objects for hit detection
		BY_BBOX_SCREEN, // not imp
		BY_OBJECT,		// fully intersect ray with object geometry 
		BY_OBJECT_SCREEN,	// not imp 

	};

	// type of algorithm

	Method method;

	enum Mode {
		MODE_SELECTION,		//!< normal mouse selection (pscreen is valid)	
		MODE_RAYHIT,		//!< some world ray 
		MODE_COLLISION,		//!< RAY collision detection 
		MODE_RAPID,			//!< Rapid collision detection  
		MODE_RAPID_RECURSIVE //!< Rapid recursive collision detection between 2 objects

	};

	//! mode of selection processing 
	Mode mode;				

	//! perform check of bounding box against transformed ray
	//! Result : true : continue processing, false : box not intersected
	//! bbox treated not empty if bboxSize.x >0 

	gbool CullCheck(GvSFVec3f &bboxCenter,GvSFVec3f &bboxSize); 
	gbool CullCheck(BBox &bbox); 

	
	int Do(GNode *n) {	return (n->Do(*this)); }
	
	//! node selection by BBox only
	int CheckForHitByBox(GNode *n, BBox &box);

    /*!
    Check bbox for hit by current ray,

    Return Code >0, hit detected
        hit info updated with hit data (m,anchor, bbox, inline ...
        tRay contains global ray transformed to local coordinates
    */

    int CheckForHitByBox(GNode *n, BBox &box, GHitInfo &hit,RayRange &tRay);


   	void InitializeAttributes(); 

	//! get nearest hit
	GHitInfo *GetHit();

	void FlushHits();

	//! fill hitInfo with required state information
	void FillHitInfo(GHitInfo &hit); 

	//! append/Replace hit, hit stored by value !!!!
	int AppendHit(GHitInfo &hit,gbool fillHit=FALSE);

};

// Tools
gbool IntersectRayCylinder(

	const	Point		&raybase,	/* Base of the intersection ray */
	const	Vector		&raycos,	/* Direction cosines of the ray */
	const	Point		&base,		/* Base of the cylinder		*/
	const	Vector		&axis,		/* Axis of the cylinder		*/
	float	radius,		/* Radius of the cylinder	*/
	float	&in,		/* Entering distance		*/
	float	&out);		/* Leaving distance		*/

/*  Graphic Gems I, Pg. 388 */
int IntersectRaySphere(
			/* Ray */
			const Point &ray_p, const Point &ray_d,
			/* sphere */
			const Point &sph_center, float sph_radius,
			/* intersection */
			float &isect_t);






#endif

