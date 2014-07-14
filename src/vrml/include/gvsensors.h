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
#ifndef  _GVSensors_
#define  _GVSensors_
/******************************************************************************
@doc

@module GvSensors.h - VRML 2.0 Sensor Nodes|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvSensor>
	<c GvTimeSensor>
		<c GvGeometrySensor>
			<c GvCylinderSensor>
			<c GvPlaneSensor>
			<c GvSphereSensor>
			<c GvTouchSensor>
	<c GvProximitySensor>
	<c GvVisibilitySensor>
	<c GvMenuSensor>
	<c GvRayHitSensor>
<c GvRayHitInfo>

Notes:

Changes:
$Revision: 1.23 $
$Log: gvsensors.h,v $
Revision 1.23  1999/07/06 16:35:19  tom
*** empty log message ***

Revision 1.22  1999/05/31 13:15:05  tom
empty message



Todo :

******************************************************************************/


#include <gvsffloat.h>
#include <gvsftime.h>
#include <gvsfbool.h>
#include <gvsfint32.h>
#include <gvsfrotation.h>
#include <gvsfvec2f.h>
#include <gvsfvec3f.h>
#include <gvsfstring.h>
#include <gvmfstring.h>

#include <gvmfint32.h>
#include <gvmfvec3f.h>
#include <gvsfnode.h>
#include <gvmfnode.h>
#include <gvsfmatrix.h>

#include <gvsubnode.h>



class GvSensor : public GvNode {

    GV_NODE_HEADER(GvSensor);

public:

    // Fields
    GvSFBool		enabled; // Default : TRUE

	// Event out
    GvSFBool		isActive;	    // is the sensor currently active ?

	// send if isActive event, if a state change occurs 
    void setActive(GvBool state) { if (isActive != state) isActive.set(state); }

    GvBool			isRegistered; // TRUE if node registered in Scene information


};


//
// GvTimeSensor
//

class GvTimeSensor : public GvSensor {

    GV_NODE_HEADER(GvTimeSensor);

  public:

	// special EventIns 
	void set_startTime(GvSFTime *t);
	void set_stopTime(GvSFTime *t);

    // Fields
    GvSFTime		cycleInterval;	// 1

    // From GvSensor GvSFBool		enabled;	    // TRUE
    GvSFBool		loop;	        // FALSE
    GvSFTime		startTime;	    // 0
    GvSFTime		stopTime;	    // 0
    
    // Event outs
    GvSFTime		cycleTime; // new ??	    
    //GvSFBool		isActive; from GvSensor	    
    GvSFFloat		fraction;	    
    GvSFTime		time;	    



	// activate on beginning 
	int OnInitTime(double t); 

	// process time 
    int OnTimeChange(double t);

	void set_enabled(GvBool state);

	// old time this sensor has been evaluated
    double oldTime;
	
	// old cycle relative time (to detect wraps)
    double oldTm;

	// next time we should check this sensor 
    double nextScheduleTime;
	double last_setStartTime; // data of last set_startTime
	double last_setStartTimeTime;



   	int Do(GTraversal &state);

	// check for registration
	int Do(GglRenderTraversal &state);

	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp

	
	//static GvSFTime theTime; // current simulation time
//	static double theTime; // current simulation time


};

// get global simulation time
//double GvGetTheTime(); 

// set global simulation time
//void GvSetTheTime(double newTime);
 

class GvGeometrySensor : public GvSensor {

    GV_NODE_HEADER(GvGeometrySensor);

  public:


	// put sensor in state for element GvWWWAnchorElement
	// only  really needed for GRaySelectTraversal
	int Apply(GTraversal &state,GvNode *parent);

	// do nothing 
	int Do(GTraversal &state);
    int Do(GglRenderTraversal &state);

	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp

	// return TRUE if this sensors needs the Mouse in drag mode 
	virtual GvBool NeedMouseDrag();


};




class GvCylinderSensor : public GvGeometrySensor {

    GV_NODE_HEADER(GvCylinderSensor);

  public:

    // Fields
    GvSFFloat    diskAngle;     // Default : 0.262
    // from Sensor GvSFBool     enabled; // Default : TRUE
 
    GvSFFloat    maxAngle;  // Default :   -1
    GvSFFloat    minAngle;  // Default :   0
    
    GvSFFloat    offset;    // Default :     0 
    GvSFBool     autoOffset; // Default : TRUE
     
    // Event outs
    //GvSFBool     isActive;
    GvSFRotation rotation;
    GvSFVec3f    trackPoint;


};


class GvPlaneSensor : public GvGeometrySensor {

    GV_NODE_HEADER(GvPlaneSensor);

  public:

    // Fields
    // from Sensor GvSFBool     enabled; // Default : TRUE

    GvSFVec2f maxPosition;  // Default : -1 -1
    GvSFVec2f minPosition;  // Default : 0 0
    GvSFVec3f offset;       // Default :      0 0 0
    GvSFBool  autoOffset;   // Default :  TRUE
     
    // Event outs
    //GvSFBool  isActive;
    GvSFVec3f translation;
    GvSFVec3f trackPoint;


};


class GvProximitySensor : public GvSensor {

    GV_NODE_HEADER(GvProximitySensor);

  public:

    // Fields
    // from Sensor GvSFBool     enabled; // Default : TRUE
    GvSFVec3f    center;     // Default : 0 0 0
    GvSFVec3f    size;       // Default : 0 0 0

    // Event outs
    //GvSFBool     isActive;
    GvSFVec3f    position;
    GvSFRotation orientation;
    GvSFTime     enterTime;
    GvSFTime     exitTime;

	int isLarge;	// -1 don't know, 0 normal, 1 : it an coverAll sensor
	
	Point lastPosition;	   // used to minimize position and orientaion events 
	Rotation lastRotation;
	double lastTime;

	
	int OnFieldChanged(GvField *field);


	// return bbox of sensor box
	GvBool getBBoxEx(BBox &bbox);

    void SetActive(GvBool state) { if (isActive != state) isActive.set(state); }

	int Do(GglRenderTraversal &state);

};



class GvSphereSensor : public GvGeometrySensor {

    GV_NODE_HEADER(GvSphereSensor);

  public:

    // Fields
    // From sensor GvSFBool     enabled; // Default : TRUE
    GvSFRotation offset; // Default :  0 1 0 0
    GvSFBool     autoOffset; // Default :  TRUE

    // Event outs
    //GvSFBool     isActive;
    GvSFRotation rotation;
    GvSFVec3f    trackPoint;
};


// GvTouchSensor
// Sensor senses all childs in group, even if behind
//

class GvTouchSensor : public GvGeometrySensor {

    GV_NODE_HEADER(GvTouchSensor);

  public:

    // Fields
    // from Sensor GvSFBool     enabled; // Default : TRUE
    // Event outs

    GvSFVec3f hitNormal;
    GvSFVec3f hitPoint;
    GvSFVec2f hitTexCoord;
    //GvSFBool  isActive;
    GvSFBool  isOver;
    GvSFTime  touchTime;

	// return TRUE if this sensors needs, only if touchTime event 
	GvBool NeedMouseDrag();

};

//
// GvMouseSensor
// sends event, anolog to DHTML mouse handling

class GvMouseSensor : public GvTouchSensor 
{

    GV_NODE_HEADER(GvMouseSensor);

  public:

	GvSFNode	event;


};



class GvVisibilitySensor : public GvSensor {

    GV_NODE_HEADER(GvVisibilitySensor);

  public:

    // Fields
    GvSFVec3f    center;     // Default : 0 0 0

    // from Sensor GvSFBool     enabled; // Default : TRUE
    GvSFVec3f    size;       // Default : 0 0 0

    // Event outs
    //GvSFBool     isActive;	// from sensor

    GvSFTime     enterTime;
    GvSFTime     exitTime;

	int OnFieldChanged(GvField *field);

	// return bbox of sensor box
	GvBool getBBoxEx(BBox &bbox);


	int Do(GglRenderTraversal &state);

	GvBool wasRendered; // set to true each time it was reached by  Do(GglRenderTraversal &state);

};


//
// GvMenueSensor
//

class GvMenuSensor : public GvSensor {

    GV_NODE_HEADER(GvMenuSensor);

  public:
	// Fields
	GvSFString	title;		// the title of the menue, defaul ""
	GvMFInt32	choices;	// the choice ID for each choice, -1 separator
	GvMFString	descriptions; // the description of each choice
	GvSFString	position;	  // where to display the menu	


	// EventOut
	GvSFInt32	choice;	// the user selected choice 

	int Do(GglRenderTraversal &state);


};



// GvRayHitSensor
// check for hits of ray with geometry 
//
// hit detection is done if a set_enabled(TRUE) or set_ray() event occurs

class GvRayHitSensor : public GvSensor {

    GV_NODE_HEADER(GvRayHitSensor);

  public:

    // Fields
    // from Sensor GvSFBool     enabled; // Default : TRUE
    GvMFVec3f ray;				// the ray described by start & ending point

	GvMFNode children;			// nodes to test against 

	// Event outs

    GvSFVec3f hitNormal;
    GvSFVec3f hitPoint;			// point of intersection 
    
	GvSFVec2f hitTexCoord;
    
	GvSFRotation hitRotation;	// rotates positive z axis to ray starting point 

    //GvSFBool  isActive;			// TRUE sensor hits something, FALSE if it hits no longer
    //GvSFBool  isOver;
    GvSFTime  touchTime;		// triggered each time a hit occurs


	// a field has been changed
	int OnFieldChanged(GvField *field);

	// do nothing 
	int Do(GTraversal &state);
	
	int OnTrigger();


};
// Event Structure similar

class GvEvent : public GvNode {

    GV_NODE_HEADER(GvEvent);

  public:
	
	// mousedown mousemove mouseout mouseover mouseup
	// keydown keyup keypress
	// click dblclick
	GvSFString type; 

	GvSFBool  cancelBubble; // true cancel handlers

	// Event outs
	GvSFBool  altKey;
	GvSFBool  ctrlKey;
	GvSFBool  shiftKey;

	GvSFInt32 button; // 1 left 2 right 4 middle
	GvSFInt32 keyCode;	// unicode key 
    
	GvSFVec2f client; // coordinates relative to the client window size
    GvSFVec2f screen; // coordinates relative to the physical screen siye

    GvSFVec2f position; // x/z

	GvSFBool  returnValue; // false cancel default action

	GvSFNode target; // The target indicates the Node to which the event was originally dispatched

	void setMouseStatus(float x,float y, unsigned int flags); 

	// a field has been changed
	int OnFieldChanged(GvField *field);

	// do nothing 
	int Do(GTraversal &state);
	

};

//
// GvRayHitInfo
//
class GvRayHitInfo : public GvNode {

    GV_NODE_HEADER(GvRayHitInfo);

  public:


	// Event outs
    GvMFNode hitPath;

    GvSFVec3f hitNormal;
    GvSFVec3f hitPoint;			// point of intersection 
   
	GvSFVec2f hitTexCoord;

    GvSFMatrix hitMatrix;
    GvSFVec3f hitNormalLocal;
    GvSFVec3f hitPointLocal;	

	GvMFInt32 hitInfo;

	// a field has been changed
	int OnFieldChanged(GvField *field);

	// do nothing 
	int Do(GTraversal &state);
	
	int OnTrigger(GView *browser,const Point &objP,const Point &objP1,GvNode *start);


};

//
// GvCollisionInfo
//
class GvCollisionInfo : public GvNode {

    GV_NODE_HEADER(GvCollisionInfo);

  public:


	// Event outs
    GvMFNode hitPath;

    GvSFVec3f hitNormal;
    GvSFVec3f hitPoint;			// point of intersection 
   
	GvSFVec2f hitTexCoord;

    GvSFMatrix hitMatrix;
    GvSFVec3f hitNormalLocal;
    GvSFVec3f hitPointLocal;	

	GvMFInt32 hitInfo;

	// a field has been changed
	int OnFieldChanged(GvField *field);

	// do nothing 
	int Do(GTraversal &state);
	
	int OnTrigger(GView *browser, GvNode *a, Matrix *aMatrix, GvNode *b,Matrix *bMatrix,int options);


};

#endif /* _GVSensors_ */
