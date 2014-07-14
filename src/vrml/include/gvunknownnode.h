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
#ifndef  _GV_UNKNOWN_NODE_
#define  _GV_UNKNOWN_NODE_

#ifdef _G_VRML1

#include <gvgroup.h>

/* to do:
   isA
   field values ?

*/

class GvUnknownNode : public GvGroup {

    GV_NODE_HEADER_UNKNOWN_NODE(GvUnknownNode);

  public:
    // VRML class-name for this unknown node
    const char	*className;

	// get VRML Type name for node 
	const char * GetType();


    GvFieldData *instanceFieldData; // returned by getFielData

    // list of pointers to fields
    GvFieldList fields;

    // get field index of this instance
    GvField *getField(int index) const;


    // create fields according to fields specification
    void instanceFields();



	GvBool readInstance(GvInput *in);

	
	// problem: where to store enum values / strings ?

	int isAOk; // -1 not checked 0 could not instance 1 instanced
	int CheckForIsA();

	GvNode * isA;	// if non NULL, there was a isA field, and class was known

  
    void setClassName(const char *name);

	// get VRML class name
	const char *getClassName() const;


	int Do(GTraversal &state);   	
	int Do(GioTraversal &state);   	
	int Do(GDisplayTreeTraversal &state); // implemented in GDisplayTree.cpp

};

#endif /* _GV_UNKNOWN_NODE_ */
#endif _G_VRML1


