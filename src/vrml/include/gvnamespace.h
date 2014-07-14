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
#ifndef  _GvNameSpace_
#define  _GvNameSpace_

/******************************************************************************
@doc

@module GvNameSpace.h - VRML 2.0 NameSpaces|

Copyright (c) 1996 - 1999 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvNameSpace>

Notes:

Changes:

$Revision: 1.4 $
$Log: gvnamespace.h,v $
Revision 1.4  1999/07/06 16:35:09  tom
*** empty log message ***

Revision 1.3  1999/05/10 13:53:57  tom
*** empty log message ***



Todo :

******************************************************************************/


#include <gvnode.h>
#include <gvdict.h>

#include <gvmfnode.h>

class GvNode;
class GvProto;

//
// GvNameSpace
//

// a name space provide
// dictonarys for node and proto names
// its a temporary push/pop like structure used during parsing

class GvNameSpace {
public:

    GvNameSpace();
    ~GvNameSpace();


    GvDict nodes;
    GvDict protos;
    //GvDict routes;

	// add/replace proto for key "name"
    GvBool addProto(const GvName &name,GvProto *proto);

	// add/replace node for key "name"
    GvBool addNode(const GvName &name,GvNode *node);

    // remove node with name "name"
	GvBool removeNode(const GvName &name);
    // remove node with name "name" only if current value for name matches node
	GvBool removeNode(const GvName &name, GvNode *node);

    
    GvProto* findProto(const GvName &name,GvBool recursive=TRUE) const;
    GvNode*  findNode(const GvName &name) const;

	GvMFNode parsedProtos; // a list to keep parsed PROTOS alive

    // RETURNS true, and in name a new unique node name, currently not in use
    GvBool generateNodeName(const char *prefix,GvName &name); 
    int nodeNameGenCounter;

	// delete all contents of scene
	void deleteAll();

    GvNameSpace *next; // linker for push/pop for input 

};

// RETURNS true, and in name a new unique node name, currently not in use
GvBool generateNodeName(GvDict &dict,const char *prefix,GvName &name);



#endif /* _GVNameSpace_ */

