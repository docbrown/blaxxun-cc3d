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
#ifndef  _GV_NODES_
#define  _GV_NODES_

/******************************************************************************
@doc

@module GvNodes.h - Headerfile including all common VRML nodes|

Copyright (c) 1995 - 1999 by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:


Changes:

$Revision: 1.5 $
$Log: gvnodes.h,v $
Revision 1.5  1999/07/06 16:35:11  tom
*** empty log message ***



Todo :

******************************************************************************/


#include <gvcone.h>
#include <gvcoordinate3.h>
#include <gvcube.h>
#include <gvcylinder.h>
#include <gvdirectionallight.h>
#include <gvgroup.h>
#include <gvindexedfaceset.h>
#include <gvindexedlineset.h>
#include <gvinfo.h>
#include <gvlevelofdetail.h>
#include <gvmaterial.h>
#include <gvmaterialbinding.h>
#include <gvmatrixtransform.h>
#include <gvnormal.h>
#include <gvnormalbinding.h>
//HG
#include <gvnodeshell.h>
#include <gvorthographiccamera.h>
#include <gvperspectivecamera.h>
//HG
#include <gvpendulum.h>
#include <gvpointlight.h>
#include <gvpointset.h>
#include <gvrotation.h>
#include <gvrotor.h>
#include <gvscale.h>
#include <gvseparator.h>
#include <gvshapehints.h>
#include <gvsphere.h>
#include <gvspotlight.h>
#include <gvswitch.h>
#include <gvtexture2.h>
#include <gvtexture2transform.h>
#include <gvtexturecoordinate2.h>
//HG
#include <gvtexturerotor.h>

#include <gvtransform.h>
#include <gvtransformseparator.h>
#include <gvtranslation.h>
#include <gvwwwanchor.h>
#include <gvwwwinline.h>
//HG
#include <gvasciitext.h>
#include <gvbasecolor.h>
#include <gvcomplexity.h>
#include <gvcallback.h>
#include <gvshuttle.h>
#include <gvmorph.h>
#include <gvlod.h>
#include <gvlabel.h>
#include <gvindexedtrianglestripset.h>
#include <gvfontstyle.h>
#include <gvtexturecoordinatebinding.h>
#include <gvsweep.h>
#include <gvblinker.h>

#include <gvtimetransform.h>
#ifdef _G_VRML1
#include <gvtextureshuttle.h>

#endif

#include <gvsensors.h>

#include <gvinterpolators.h>

#include <gvsound.h>
#include <gv2group.h>
#include <gv2geometry.h>

#include <gvavatar.h>

#endif /* _GV_NODES_ */
