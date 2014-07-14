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
#ifndef __GReadGV_H
#define __GReadGV_H

class GReporter;
class GWorld;
class GvNode;
class GvSFImage;
class GvMFNode;
class GvSFNode;

class GTraversal;

class GglRenderTraversal;

/* register all qv property nodes to state */
// in GvTraverse
//void InitializeGvAttributes(GTraversal &state,int all=1);

#define GV_READ_NOERROR 1

// Read vrml file  into scene
int ReadVrmlFile(const char *fileName,GReporter *reporter,GvScene *w,int flags=0);

// Read vrml file  into scene
int ReadVrmlFile(const char *fileName,GReporter *reporter,GvMFNode *w,int flags=0);


int ReadVrmlFile(const char *fileName,GReporter *reporter,GWorld *w);
int ReadVrmlFile(const char *fileName,GReporter *reporter,GvNode * & root);

// read a file a a GvNode object
int ReadFile(const char *FileName,GReporter *reporter,GvNode * & root);

// read a texture file into the current glState 
int ReadTextureFile(const char *fileName,int fileFmt, 
					GglRenderTraversal &state,const char *refType,int &hasAlpha);

// read an image file into the SFImage field
int ReadImageFile(const char *FileName,int fmt,
				  GTraversal &state,
				  const char *refType,
				  GvSFImage &imageField);



#endif
