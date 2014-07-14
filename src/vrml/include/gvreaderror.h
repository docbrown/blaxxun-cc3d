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
#ifndef  _GV_READ_ERROR
#define  _GV_READ_ERROR

class GvInput;

// specific VRML read error codes

typedef enum
{
	GV_RE_NONE,
	GV_RE_ERROR,			//!< unspecified error
	GV_RE_OUT_OF_MEMORY,	//!< out of memory
	GV_RE_INVLAID_HEADER,	//!< invalid header 
	GV_RE_SYNTAX_ERROR,		//!< syntax error
	GV_RE_TYPE_ERROR,		//!< type mismatch error
	GV_RE_UNEXPECTED_EOF,	//!< unexpected EOF
	GV_RE_FILE_ERROR,		//!< problem with file 
	GV_RE_FILE_CORRUPT_ERROR,	//!< problem with file 
	GV_RE_UNKNOWN_FIELD,	//!< unknown field
	GV_RE_UNKNOWN_NODE_REF,	//!< unknown node reference 
	GV_RE_UNKNOWN_NODE,		//!< unknown node 
	GV_RE_BAD_ROUTE,		//!< bad ROUTE


} GvReadErrorCode;


class GvReadError {
  public:
    static void	 G__cdecl post(GvInput *in,GvReadErrorCode errorCode, const char *formatString, ...);
};

#endif /* _GV_READ_ERROR */
