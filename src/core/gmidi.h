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
#ifndef _GMidi_h
#define _GMidi_h
/******************************************************************************
@doc

@module GMidi.h - Midi support  |

Copyright (c) 1998	by  blaxxun interactive - Holger Grahn 
All rights reserved

Purpose:

Classes:


Notes:


Changes:

$Revision: 1.2 $
$Log: gmidi.h,v $
Revision 1.2  1999/03/10 10:22:10  tom
*** empty log message ***



Todo :
   
******************************************************************************/



class GDSSoundDevice;
class GDSSoundEmitter;


#include "gsound.h"

class GMidiEmitter;


// create a MIDI Emitter
GDSSoundEmitter * newMidiEmitter();




#endif // _GMidi_h
