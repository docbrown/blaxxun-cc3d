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
#ifndef _GSHELLIO_H
#define _GSHELLIO_H
/******************************************************************************
@doc

@module GShellIO.h - GLView additional GShell IO functions |

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

Todo :

******************************************************************************/


class GShell;

//GShellIO.h
#ifdef _HMF
//@func write shell s to file FileName in HMF format
int WriteShellHmf(GShell *s,const char *FileName);
#endif

//@func write shell s to file FileName in GEO format, if outputNormals set, VN will be output
int WriteShellGeo(GShell *s,const char *FileName,int outputNormals=0);

//@func write shell s to file FileName in STL format
int WriteShellStl(GShell *s,const char *FileName);

//@func write shell s to file FileName in POV format
int WriteShellPov(GShell *s,const char *FileName);

//@func write shell s to file FileName in RAW format
int WriteShellRaw(GShell *s,const char *FileName);

#ifdef _VRML
//@func write shell s to file FileName in VRML format
int WriteShellVrml(GShell *s,const char *FileName,gbool vrml2);
#endif


#endif
