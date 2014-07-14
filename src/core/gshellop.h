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
#ifndef _GSHELLOP_H
#define _GSHELLOP_H
/******************************************************************************
@doc

@module GShellOP.h - GLView shell operations	|

Copyright (c) 1995	by Holger Grahn
All rights reserved

Purpose:
Implements HMF / 3DV / RAW formats
Classes:

Notes:

Changes:

Todo :

******************************************************************************/

extern GShell* ReadFromMetafile(AStream &a,GShell *the_shell=NULL);
extern GShell* ReadFromHmfFile(const char *FileName);

extern GShell* ReadFromGeoFile(const char *FileName);

extern int WriteShell3DV(GShell *s,const char *FileName);
extern int ReadShell3DV(const char *FileName,GShell *&s);

extern int ReadShellRaw(const char *FileName,GShell *&s);

// Read shell from basice lowlevel file formats 
extern int ReadShell(const char *FileName,int fmt,GShell * &newShell);


#endif





