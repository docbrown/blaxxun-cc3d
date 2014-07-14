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
/******************************************************************************
GVersion.h		- blaxxunCC3D / GLView hardcoded version numbers + Info URL's

$Revision: 1.26 $
$Log: gversion.h,v $
Revision 1.26  1998/08/13 14:14:20  kristof
308

Revision 1.25  1998/07/02 17:46:31  holger
empty message

Revision 1.24  1998/07/01 16:48:42  holger
empty message




******************************************************************************/


// for registry keys 
#ifdef _GLVIEW_IE
// OCX 
#define _COMPANY "blaxxun interactive"
#define _PROGRAM "blaxxunCC3D"
#define _VERSION "4.300"
#define _VERSION_INT 4300
#define _VERSION_BUILD _VERSION_INT

#else

#define _COMPANY "hg"
#define _PROGRAM "GLView"
#define _VERSION "4.3"
#define _VERSION_INT 4300
#define _VERSION_BUILD _VERSION_INT

// collect statistic information
#define _GSTATS

#endif


#ifdef _EVALUATION
#define _PROGRAM_TYPE " Demo "
#else
#define _PROGRAM_TYPE ""
#endif

#define _PROGRAM_DATE _DATE_

#define _WRITTEN_BY_STRING  _PROGRAM  _PROGRAM_TYPE  " " _VERSION

#ifdef _D3D
#define SEC_OPTIONS "Direct3D"
#endif

#ifdef _OGL
#define SEC_OPTIONS "OpenGL"
#endif

#define IMAGE_EXTENSIONS "*.bmp;*.dib;*.rgb;*.jpg;*.jpeg;*.gif;*.tga;*.ppm;*.png"


// urn definitions for builtin nodes
#define URL_BLAXXUN "http://www.blaxxun.com"

#ifdef _GLVIEW_IE

#define URN_NODE "urn:inet:blaxxun.com:node:"
#define URN_NODE1 "urn:inet:glview.com:node:"
#define URN_NODE_VRML1 "urn:inet:blaxxun.com:node:vrml1"

#define URL_HOME "http://www.blaxxun.com"
#define URL_PROTOS URL_HOME "/vrml/protos/nodes.wrl#"

#else
#define URN_NODE "urn:inet:blaxxun.com:node:"
#define URN_NODE1 "urn:inet:glview.com:node:"
#define URN_NODE_VRML1 "urn:inet:blaxxun.com:node:vrml1"

//#define URN_NODE "urn:inet:glview.com:node:"
//#define URN_NODE1 "urn:inet:blaxxun.com:node:"
//#define URN_NODE_VRML1 "urn:inet:glview.com:node:vrml1"

//#define URL_HOME "http://www.glview.com"
#define URL_HOME "http://www.snafu.de/~hg"

#define URL_PROTOS URL_BLAXXUN "/vrml/protos/nodes.wrl#"

//#define URL_HOME "http://www.blaxxun.com"

#endif

#define URL_PRODUCT URL_HOME "/products/" _PROGRAM 



