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
#ifndef  _GV2Geometry_
#define  _GV2Geometry_

/******************************************************************************
@doc

@module Gv2Geometry.h - VRML 2.0 Geometry nodes|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c Gv2Color>
...

  
Notes:

Changes:

$Revision: 1.19 $
$Log: gv2geometry.h,v $
Revision 1.19  1999/07/06 16:34:28  tom
*** empty log message ***

Revision 1.18  1999/05/10 13:53:39  tom
*** empty log message ***


Todo :

******************************************************************************/


#include <gvsubnode.h>
#include <gvsfvec3f.h>
#include <gvmfvec3f.h>
#include <gvsfrotation.h>
#include <gvsfbool.h>

#include <gvsffloat.h>
#include <gvmffloat.h>

#include <gvsfnode.h>
#include <gvmfnode.h>

#include <gvsfint32.h>
#include <gvmfint32.h>
#include <gvmfstring.h>
#include <gvmfvec2f.h>
#include <gvmfcolor.h>

#include <gvnodeshell.h>

// needed for D3D Material Handle
#include "grender.h"


// would the creaseAngle result in facetted changed
////#define GvCreaseAngleIsFlat(VALUE) ((VALUE) >= 3.14)
#define GvCreaseAngleIsFlat(VALUE) ((VALUE) < 0.5 )
#define GvAlwaysSmoothCreaseAngle (3.1)

//
// geometric properties
//
class GvColor : public GvNode {

    GV_NODE_HEADER(GvColor);

  public:
    // Fields
    GvMFColor		color;		// RGB color values


};


class GvCoordinate : public GvNode {

    GV_NODE_HEADER(GvCoordinate);

  public:
    // Fields
    GvMFVec3f		point;		// Coordinate point(s)

	// VRML draf #2 points

	int Do(GBBoxTraversal &state);
};


class Gv2Normal : public GvNode {

    GV_NODE_HEADER(Gv2Normal);

  public:
    // Fields						
    GvMFVec3f		vector;		// Normal vector(s)
	// VRML draf #2 vectors

};


class GvTextureCoordinate : public GvNode {

    GV_NODE_HEADER(GvTextureCoordinate);

  public:
    // Fields
    GvMFVec2f		point;		// TextureCoordinate point(s)
	// VRML draf #2 points


};




//
// GvAppearance
//


class GvAppearance : public GvNode {

    GV_NODE_HEADER(GvAppearance);

public:
    // Fields
   GvSFNode material; // Default NULL
   GvSFNode texture;  // Default NULL 
   GvSFNode textureTransform; // Default NULL 

   signed char changed;    // set to 1 if any field has changed 


public:
   int checkForDefaultValues(int mode);

   // put pointer to appearance in state
   int Do(GTraversal &state);   	
   int Do(GPathTraversal &state);

   int Do(GglRenderTraversal &state);
   int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp

   int Apply(GglRenderTraversal &state);

	// add child node , for easier editing
	// result 0 : bad node, not changed , 1 : node added / replaced
	int addChild(GvNode *child);

	// some child node has been changed, set redraw bit 
	int OnChildChanged(GvNode *child);

	// a field has been changed
	int OnFieldChanged(GvField *field);

	// something changed 
	int OnChanged();

	int OnMaterialChanged() { changed = 1; return(1); }
	int OnTextureChanged() { changed = 1; return(1); }
	int OnTextureTransformChanged() { changed = 1; return(1); }


#ifdef _D3D
     G3MaterialRef handle;  // need a material handle if material == NULL
	 // get the D3D Material handle strucutre 
	 G3MaterialRef* GetMaterialHandle();

#endif


};

//
// GvShape
//

class GvShape : public GvNode {

    GV_NODE_HEADER(GvShape);

public:
    // Fields
   GvSFNode appearance; // Default NULL
   GvSFNode geometry;  // Default NULL 

public:

	// set redraw bit if a field changed 
	int OnFieldChanged(GvField *field);

	// some child node has been changed, set redraw bit 
	int OnChildChanged(GvNode *child);

	// get bounding box from geometry
	GvBool getBBox(BBox &bbox);

	GvNode *getGeometry() { return geometry; }
	GvNode *getAppearance() { return appearance; }
	
	int checkForDefaultValues(int mode);




   int Do(GTraversal &state);   	
   int Do(GglRenderTraversal &state);
   int Do(GRaySelectTraversal &state);  
   int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp


   int Do(GBBoxTraversal &state);

	// add child node , for easier editing
	// result 0 : bad node, not changed , 1 : node added / replaced
	int addChild(GvNode *child);

	int OnAppearanceChanged() { return(1); }
	int OnGeometryChanged() { return(1); }

};


// Geometry Nodes 



class GvBox : public GvNodeShell
{

    GV_NODE_HEADER(GvBox);

  public:
    // Fields
    GvSFVec3f size;             // Default : 2 2 2 

	// is this a solid shape ?
	GvBool Solid() const { return (TRUE); }

	// is p outside the shape 
	GvBool isOutside(const Point &p) const;
	GvBool isSolid() const { return Solid(); }
	GvBool isConvex() const { return(TRUE); }

	GvBool getBBox(BBox &bbox);


	int BuildShell(GTraversal &state);
	int Do(GBBoxTraversal &state);
	int Do(GglRenderTraversal &state);
	int Do(GGeometryTraversal &state);
	int Do(GRaySelectTraversal &state);   // in GRaySelect.cpp

};

class Gv2Cone : public GvNodeShell 
{

    GV_NODE_HEADER(Gv2Cone);

  public:

    // Fields
    GvSFFloat		bottomRadius;	//Default : 1 Radius of bottom circular face
    GvSFFloat		height;		// Default : 2,Size in y dimension

    GvSFBool		side;		// Default : true,Visible parts of cone
    GvSFBool		bottom;		// Default : true, Visible parts of cone

	// is this a solid shape ?
	GvBool Solid() const { return (side.value && bottom.value); }
	//GvBool isOutside(const Point &p) const;
	GvBool isSolid() const { return Solid(); }
	GvBool isConvex() const { return(TRUE); }

	GvBool getBBox(BBox &bbox);
	
    int BuildShell(GTraversal &state);
	int Do(GBBoxTraversal &state);
	int Do(GglRenderTraversal &state);
	int Do(GGeometryTraversal &state);
	int Do(GRaySelectTraversal &state);

};

class Gv2Cylinder : public GvNodeShell 
{

    GV_NODE_HEADER(Gv2Cylinder);

  public:

    // Fields
    GvSFBool		bottom;		// Default : true, Visible parts of cone
    GvSFFloat		height;		// Default : 2,Size in y dimension
    GvSFFloat		radius;	    // Default : 1 Radius of bottom circular face
    GvSFBool		side;		// Default : true,Visible parts of cone
    GvSFBool		top;		// Default : true, Visible parts of cone

	// is this a solid shape ?
	GvBool Solid()  const { return (side.value && bottom.value && top.value); }
	GvBool isOutside(const Point &p) const;
	GvBool isSolid() const { return Solid(); }
	GvBool isConvex() const { return(TRUE); }

	GvBool getBBox(BBox &bbox);

	int BuildShell(GTraversal &state);
	int Do(GBBoxTraversal &state);
	int Do(GglRenderTraversal &state);
	int Do(GGeometryTraversal &state);
	int Do(GRaySelectTraversal &state); 

};

class Gv2IndexedFaceSet : public GvNodeShell 
{

    GV_NODE_HEADER(Gv2IndexedFaceSet);

  public:

    // Fields
    // EventsIn
    //void set_colorIndex(GvMFInt32 *value);
    //void set_coordIndex(GvMFInt32 *value);
    //void set_normalIndex(GvMFInt32 *value);
    //void set_texCoordIndex(GvMFInt32 *value);

    //exposed
    GvSFNode  color;            // Default :NULL
    GvSFNode  coord;            // Default :NULL
    GvSFNode  normal;           // Default :NULL
    GvSFNode  texCoord;         // Default :NULL
    // not exposed 
    GvSFBool  ccw;              // Default :TRUE
    GvMFInt32 colorIndex;        // Default :[]
    GvSFBool  colorPerVertex;   // Default :TRUE
    GvSFBool  convex;           // Default :TRUE
    GvMFInt32 coordIndex;       // Default :[]
    GvSFFloat creaseAngle;      // Default :0
    GvMFInt32 normalIndex;      // Default :[]
    GvSFBool  normalPerVertex;  // Default :TRUE
    GvSFBool  solid;            // Default :TRUE
    GvMFInt32 texCoordIndex;    // Default :[]


	// is this a solid shape ?
	GvBool Solid() const { return (solid.value); }
	GvBool isSolid() const { return Solid(); }

  	// build shell 
  	int BuildShell(GTraversal &state);

	// if no shell build, use coord 
	int Do(GBBoxTraversal &state);
	int Do(GglRenderTraversal &state);
	int Do(GRaySelectTraversal &state);
	int Do(GGeometryTraversal &state);
    int Do(GOptimizeTraversal &state);		// implemented in GOptimizeTraversal.cpp

	// add child node , for easier editing
	// result 0 : bad node, not changed , 1 : node added / replaced
	int addChild(GvNode *child);

	// internal
	// try to update internal shell
	int OnColorChanged();
	int OnCoordChanged();
	int OnNormalChanged();
	int OnTexCoordChanged();

	// a child node changed
	int OnChildNodeChanged(GvNode *child);
	int OnChildChanged(GvNode *child);

	int OnFieldChanged(GvField *field);


};


class Gv2IndexedLineSet : public GvNodeShell 
{

    GV_NODE_HEADER(Gv2IndexedLineSet);

  public:

    // Fields
    // EventsIn
    
	//void set_colorIndex(GvMFInt32 *value);
    //void set_coordIndex(GvMFInt32 *value);

    //exposed
    GvSFNode  color;             // Default :NULL
    GvSFNode  coord;             // Default :NULL

    // not exposed 
    GvMFInt32 colorIndex;      // Default :[]
    GvSFBool  colorPerVertex;    // Default :TRUE
    GvMFInt32 coordIndex;      // Default :[]

	GvBool Solid() const { return (FALSE); }

  	// build shell 
  	int BuildShell(GTraversal &state);

	// if no shell build, use coord 
	int Do(GBBoxTraversal &state);
	int Do(GglRenderTraversal &state);


	int Do(GRaySelectTraversal &state);   // in GRaySelect.cpp

	// add child node , for easier editing
	// result 0 : bad node, not changed , 1 : node added / replaced
	int addChild(GvNode *child);

	// internal
	// try to update internal shell
	int OnColorChanged();
	int OnCoordChanged();

	// a child node changed
	int OnChildNodeChanged(GvNode *child);
	int OnChildChanged(GvNode *child);

	int OnFieldChanged(GvField *field);


};

class Gv2PointSet : public GvNodeShell 
{

    GV_NODE_HEADER(Gv2PointSet);

  public:

    // Fields
    //exposed
    GvSFNode  color;             // Default :NULL
    GvSFNode  coord;             // Default :NULL

	GvBool Solid() const { return (FALSE); }


  	// build shell 
  	int BuildShell(GTraversal &state);

	// if no shell build, use coord 
	int Do(GBBoxTraversal &state);
	int Do(GglRenderTraversal &state);


	int Do(GRaySelectTraversal &state);   // in GRaySelect.cpp

	// add child node , for easier editing
	// result 0 : bad node, not changed , 1 : node added / replaced
	int addChild(GvNode *child);

	// internal
	// try to update internal shell
	int OnColorChanged();
	int OnCoordChanged();

	// a child node changed
	int OnChildNodeChanged(GvNode *child);
	int OnChildChanged(GvNode *child);

};


class Gv2Sphere : public GvNodeShell 
{

    GV_NODE_HEADER(Gv2Sphere);

  public:

    // Fields
    GvSFFloat		radius;	    //Default : 1 Radius of bottom circular face

	// is this a solid shape ?
	GvBool Solid() const { return (TRUE); }
	GvBool isOutside(const Point &p) const;
	GvBool isSolid() const { return Solid(); }
	GvBool isConvex() const { return(TRUE); }


	int BuildShell(GTraversal &state);
	GvBool getBBox(BBox &bbox);

	int Do(GBBoxTraversal &state);
	int Do(GglRenderTraversal &state);
	int Do(GGeometryTraversal &state);
	int Do(GRaySelectTraversal &state);   // in GRaySelect.c

};

class Gv2Text : public GvNodeShell 
{

    GV_NODE_HEADER(Gv2Text);

  public:

    // Fields
    GvMFString string;     //Default :[]
    GvSFNode   fontStyle;  //Default :NULL
    GvMFFloat  length;     //Default :[]
    GvSFFloat  maxExtent;  //Default :0.0

	// GvBool Solid() const { return (TRUE); }
	// is two sided 
	GvBool	solid;

	GvBool Solid() const { return (solid); }
	GvBool isSolid() const { return Solid(); }

	// a child node changed
	int OnChildNodeChanged(GvNode *child);


	int BuildShell(GTraversal &state);
	int Do(GglRenderTraversal &state);
	int Do(GGeometryTraversal &state);
	int Do(GRaySelectTraversal &state);  

};

// 1.1 proposed NODE
//The verticesPerRow and verticesPerColumn fields define the number of grid
//points in the Z and X directions, respectively, defining a surface that contains
//(verticesPerRow-1) x (verticesPerColumn-1) rectangles. 


class GvElevationGrid : public GvNodeShell {

    GV_NODE_HEADER(GvElevationGrid);

  public:
    // Fields


    GvSFInt32		xDimension;	// 0 x direction
    GvSFFloat		xSpacing;	// 0.0 x direction
    GvSFInt32		zDimension;	// 0 z direction
    GvSFFloat		zSpacing;	// 0.0 z direction

    GvMFFloat		height;
    
    GvSFNode        color;		// exposed
    GvSFBool        colorPerVertex;

    GvSFNode        normal;		// exposed
    GvSFBool        normalPerVertex;

    GvSFNode        texCoord;	// exposed

    GvSFBool        ccw;
    GvSFBool        solid;
    GvSFFloat       creaseAngle;

	// EventIN set_height(GvMFFloat *heights)


	// is this a solid shape ?
	GvBool Solid() const { return (solid.value); }
	GvBool isSolid() const { return Solid(); }


  	// build shell for current state 
  	int BuildShell(GTraversal &state);  

    int Do(GBBoxTraversal &state);
    int Do(GglRenderTraversal &state);
	int Do(GGeometryTraversal &state);
	int Do(GRaySelectTraversal &state);  


    // compute vertex colors for current grid size
    virtual int ComputeVertexColors(GTraversal &state,PointArray &colors);

#if 0	
	Point& Get(int row, int col,Point &p) {
		p.Set(gridStep[0] * col, height[row*verticesPerRow+col], gridStep[1] * row); 
		//where 0<=row<verticesPerColumn, 0<=j<verticesPerRow. 
		return(p);
	};
#endif
	Point& Get(int j /*row */, int i /* col */,Point &p) {
		p.Set(xSpacing * i, height[i + j * xDimension], zSpacing * j); 
		//where 0<=i<xDimension, 0<=j<zDimension. 
		return(p);
	};

	int Set(int j, int i,float h) {
		height[i + j * xDimension] = h;
		//where 0<=i<xDimension, 0<=j<zDimension. 
		return(1);
	};


	Point Get(int row,int col) {
		Point p;
		Get(row,col,p);
		return(p);
	};

	// add child node , for easier editing
	// result 0 : bad node, not changed , 1 : node added / replaced
	int addChild(GvNode *child);

	// internal
	// try to update internal shell
	int OnHeightChanged();
	int OnColorChanged();
	int OnNormalChanged();
	int OnTexCoordChanged();

	int OnFieldChanged(GvField *field);

	// a child node changed
	int OnChildNodeChanged(GvNode *child);
	int OnChildChanged(GvNode *child);

};

/*
The imageElevationGrid, creates an ElevationGrid from Imagedata


*/

class GvImageElevationGrid : public GvElevationGrid {

    GV_NODE_HEADER(GvImageElevationGrid);

  public:
      // Fields
      GvSFNode heightImage;   // a Image or ScaledImage Node, default : NULL   
      GvSFFloat heightScale; // additional heightScale factor for images values, default: 1.0
      GvSFNode colorImage;   // a Image or ScaledImage Node for per vertex colors, default : NULL   
      GvSFBool colorPerHeight;


    // compute vertex colors for current grid size
    virtual int ComputeVertexColors(GTraversal &state,PointArray &colors);

  	// build shell for current state 
  	int BuildShell(GTraversal &state);  

    // overwritten to load images 
    int Do(GglRenderTraversal &state);					     



};



class GvPolygon : public GvNodeShell 
{

    GV_NODE_HEADER(GvPolygon);

  public:

    // Fields
    // EventsIn
    //void set_coordIndex(GvMFInt32 *value);

    //exposed
    GvSFNode  color;            // Default :NULL
    GvSFNode  coord;            // Default :NULL
    GvSFNode  normal;           // Default :NULL
    GvMFNode  texCoord;         // Default :NULL

    // not exposed 
    GvSFBool  ccw;              // Default :TRUE
    GvMFInt32 coordIndex;       // Default :[]
    GvSFBool  solid;            // Default :TRUE

	// is this a solid shape ?
	GvBool Solid() const { return (solid.value); }
	GvBool isSolid() const { return Solid(); }


	//	flags;
	BEGIN_PACKED_STRUCT //    struct {
		unsigned int modified       : 1;  //!< node has been modified 
		unsigned int bboxOk			: 1;  //!< bbox is computed 
		unsigned int planeOk		: 1;  //!< plane is computed 
    END_PACKED_STRUCT // }; 

	BBox	bbox;	// plane 
	Plane	plane;	// bbox 

	// update cached information
	void Update(gbool theBox,gbool thePlane);


  	// build shell 
  	int BuildShell(GTraversal &state);

	// if no shell build, use coord 
	int Do(GBBoxTraversal &state);
	int Do(GglRenderTraversal &state);
	int Do(GRaySelectTraversal &state);
	int Do(GGeometryTraversal &state);
    int Do(GOptimizeTraversal &state);		// implemented in GOptimizeTraversal.cpp

	// add child node , for easier editing
	// result 0 : bad node, not changed , 1 : node added / replaced
	int addChild(GvNode *child);

	// internal
	// try to update internal shell
	int OnColorChanged();
	int OnCoordChanged();
	int OnNormalChanged();
	int OnTexCoordChanged();

	// a child node changed
	int OnChildNodeChanged(GvNode *child);
	int OnChildChanged(GvNode *child);

	int OnFieldChanged(GvField *field);


};



#endif /* _GV2Geometry_ */
