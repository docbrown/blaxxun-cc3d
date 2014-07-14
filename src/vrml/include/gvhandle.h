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
#ifndef  _GV_HANDLE_
#define  _GV_HANDLE_

#include <gvsfstring.h>
#include <gvmfstring.h>
#include <gvsffloat.h>
#include <gvsftime.h>
#include <gvsfcolor.h>
#include <gvsfvec3f.h>
#include <gvmffloat.h>
#include <gvsfbool.h>
#include <gvmfcolor.h>
#include <gvsfnode.h>


#include <gvsubnode.h>

#include <gv2group.h>


class GTraversal;
class GView;



class GvHandle : public Gv2Group {

    GV_NODE_HEADER(GvHandle);

  public:
    // Fields
    GvSFNode 		theNode;


};


class GvBoxHandle : public GvHandle 
{

    GV_NODE_HEADER(GvBoxHandle);

  public:
	  GvBool createProxy(const BBox &bbox);

};


enum TextureOp {
	OP_IMAGE_TEXTURE,
	OP_MOVIE_TEXTURE,
	OP_PIXEL_TEXTURE,
	OP_TEXTURED_RECT,
	OP_MOVIE_RECT,
	OP_BACKGROUND,
	OP_ELEVATION_GRID,
	OP_ELEVATION_GRID_TEXTURED,
	OP_ELEVATION_GRID_COLORED,
};

/*

  create a node for image

*/

GvNode *TextureToNode(int fileFormat,const char *fileName,int op);



#endif /* _GV_HANDLE_ */
