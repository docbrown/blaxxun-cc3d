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
#ifndef _GioTraversal_h_
#define _GioTraversal_h_

class GvName;
class GvString;
class GvDict;
class GvProto;




class GioTraversal : public GTraversal {
public :

	RT(GioTraversal);
	
	GioTraversal(const char *FileName=NULL,const char *mode="w");
    ~GioTraversal();

	int format;				// file format
	float version;			// Version number of file
	
	GvBool vrml2;			// shorthand flag for VRML 2.0
	GvBool minimizeSize;	// minimum filesize 	
	
	GvDict	*refDict;	// Node reference dictionary
	GvDict	*protoDict;	// Written Protos 
	
	GvProto *currentProto; // current proto beeing written

	int writeInlines; // write inlines recursively
	int updateInlineBbox;	// update the bbox of the inlines
	
	int writeAllFields;	// write all fields with values (usefull for debugging)

	GvString inlineRoot; // root directroy for inlines

	// the stream 
	AStream *astream;

//    void InitializeAttributes(); 

    // referenced object information

	// write file header 
	int DoFileHeader();

	// write out field / object name
	// write a token
	int Do(const GvString &name);
	int Do(const char * name);
	
	// write out node class of node
	int DoNodeName(GvNode * node);

	// write object name
	int Do(const GvName &name);
	
	// write quoted string
	int DoString(const GvString &name);

	int Do(GNode *n) {	return (n->Do(*this)); }
	//int Do(GAttribute *a)	 { return (a->Do(*this)); }

	// generate a proto for node
	int DoProtoForNode(GvNode *n);


};

class GPovIoTraversal : public GioTraversal {
public :

	RT(GPovIoTraversal);
	
	GPovIoTraversal(const char *FileName=NULL,const char *mode="w");
    ~GPovIoTraversal();
};


#endif


