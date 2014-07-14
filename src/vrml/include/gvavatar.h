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
#ifndef  _GvAvatar_h_
#define  _GvAvatar_h_

/******************************************************************************
@doc

@module GvAvatar.h - blaxxun VRML Avatar node|

Copyright (c) 1998 - 1998 blaxxun interactive, Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvAvatar>
  
Notes:

Changes:

$Revision: 1.11 $
$Log: gvavatar.h,v $
Revision 1.11  1999/07/06 16:34:31  tom
*** empty log message ***

Revision 1.10  1999/05/10 13:53:41  tom
*** empty log message ***


Todo :

******************************************************************************/


#include <gvwwwinline.h>
#include <gvmffloat.h>


//
// GvAvatar  Pseudo node 
//
class GvAvatar : public Gv2Inline  // Gv2CreateVrmlFromUrl
{

    GV_NODE_HEADER(GvAvatar);

public:
	// fields from Group :
	// bboxSize bboxCenter
	// children
	//
	// Fields from Inline :
	// url
	// isLoaded

    // Fields :

	// Avatar Interface
    GvSFVec3f		position;		// Translation vector default : 0 0 0
    GvSFRotation	rotation;	    // Rotation default : 0 0 1  0
    
	GvSFVec3f		scale;	        // Scale factors default : 1 1 1
	GvMFFloat		avatarSize;		// [ 0.5 1.75 0.5 ]

	GvSFInt32		whichChoice;		// 0 

	// EventOut
	GvSFBool		isOver;			// FALSE 
	GvSFTime		touchTime;		//  0 

	// EventIn 
	GvSFString		nickname;		// ""	
	GvMFString		chatGesture;	// [] 


	// standard interface
	// EventIn
	GvSFTime		gesture1;				// 0 
	GvSFTime		gesture2;				// 0 
	GvSFTime		gesture3;				// 0 
	GvSFTime		gesture4;				// 0 
	GvSFTime		gesture5;				// 0 
	GvSFTime		gesture6;				// 0 
	GvSFTime		gesture7;				// 0 
	GvSFTime		gesture8;				// 0 
	GvSFBool		isAvatar;				// TRUE 

	// 

	GvSFInt32		gesture; 			// eventIn to trigger, eventOut to transmitt
	GvSFBool		isPilot;			// FALSE , TRUE for 3rd person mode

	GvSFNode		sharedZone;			// pointer to shared zone 
	GvSFString		avatarURL;			// current url in effect for avatar

	GvMFString		properties;			// set by parser 

	/*
		parameters out :
			fileSize = fileSize in bytes
			isGzipped = file is gzipped

		parameters in: ??

			?? defaultGestureURL = "http://www.blaxxun.com/vrml/protos/gestures.wrl#DefaultGesture" & number
			maxFileSize = max allowed filesize in bytes


	*/

/*
	// CreateVrmlFromString stuff needed ?

	GvSFNode	node;	// node with addChildren event
	GvSFInt32   nodeEvent; // event of node to be called 

	// check the Loading state for a completed file task
	// return TRUE if done 
	GvBool CheckForLoad(GFile *file);
	
	// send the event out 
	GvBool SendEvent();
*/

	// add a route from/to this node to/from avatar implementation node
	GvBool addRoute (GvNode *from, int fromIndex, GvNode *to,int toIndex) 
	{
		 return GvScene::instanceRoute(from,fromIndex,to,toIndex);
	}

	// check if avatar node has eventIn with name eventName
	// if yes ROUTE this.myField TO avatar.eventName

	GvBool RouteEventIn(GvField *myField,GvNode *avatar,const char *eventName)
	{
		GvFieldData *fields=avatar->getFieldData();
		int nindex=fields->getEventInIndex(avatar,eventName,myField->Id());
		if (nindex < 0) return gfalse;
		int index=this->getFieldIndex(myField);
		if (index < 0) return gfalse;
		return addRoute(this,index,avatar,nindex);
	}

	/*

	addTransform = TRUE;
	if (RouteEventIn(&position,avatar,"set_position")) 
	if (RouteEventIn(&rotation,avatar,"set_rotation")) 
		addTransform = FALSE;

	etc ..

	*/



};



#endif /* _GvAvatar_h_ */
