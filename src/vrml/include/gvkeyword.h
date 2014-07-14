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

#ifndef  _GV_Keyword_
#define  _GV_Keyword_
/*

   definition for all VRML Keywords
   a name is constructed, for faster comparison with names

*/

#include <gvstring.h>


// Parsing keywords
#define OPEN_BRACE		'{'
#define CLOSE_BRACE		'}'
#define DEFINITION_KEYWORD	"DEF"
#define REFERENCE_KEYWORD	"USE"
#define NULL_KEYWORD		"NULL"

#define FIELDS_KEYWORD		"fields" // VRML 1.0

#define PROTO_KEYWORD		"PROTO"
#define EXTERN_PROTO_KEYWORD		"EXTERNPROTO"
#define ROUTE_KEYWORD		"ROUTE"
#define TO_KEYWORD		"TO"
#define IS_KEYWORD		"IS"
#define TRUE_KEYWORD		"TRUE"
#define FALSE_KEYWORD		"FALSE"

class GvKeyword {
public: 

    static GvName null;
    static GvName def;
    static GvName use;
    static GvName fields;
    static GvName proto;
    static GvName externProto;
    static GvName route;
    static GvName to;
    static GvName is;
    static GvName True;
    static GvName False;
};

#endif
