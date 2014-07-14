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
#ifndef _GEvent_h
#define _GEvent_h
/******************************************************************************
@doc

@module GEvent.h - GLView event handler classes|

Copyright (c) 1996	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GEventHandler>

Notes:

Changes:

Todo :

******************************************************************************/

#include "quaternation.h"


class GView;

class GvGroup;

//
// GEventHandler 
// base class for EventHandlers
//

class GEventHandler : public RTRoot {
public:
	RT(GEventHandler);

	// the current state of eventhandler 
	int refCnt;
	void ref() { refCnt ++; }
	void unref() { refCnt--; if (refCnt<=0) delete this; }


	int state; 

	// the view this eventhandler is attached to
	GView *view;

	GEventHandler(GView *View =NULL) : view(View) { refCnt = 0; state = 0; }
	virtual ~GEventHandler();

	// get reference to camera in effect for view 
	GCamera &GetCamera() { return *view->GetPhysicalCamera(); }

	// set state variable
	void SetState(int State) { state=State; }

	// get state variable
	int State() const { return(state); }

	Point firstPoint;
	UINT  firstFlags;

	// event handler action routines

	// nFlags are the OS status flags, point is in normalized screen space with dept undefined (0)
	virtual int OnLButtonDown(UINT nFlags, Point &point);
	virtual int OnLButtonDblClk(UINT nFlags, Point &point);
	virtual int OnMouseMove(UINT nFlags, Point &point);
	virtual int OnLButtonUp(UINT nFlags,  Point &point);
	virtual int OnTimer();

	enum EventStatus {
				EV_OK,		// event was handled, continue
				EV_CANCEL,	// event handler should terminate
				EV_DONE,	// event handler done, terminate
				EV_POP		// event handler done, pop from stack and unref 
			};


	// coorindate mapping mode
	enum MapMode {
				MAP_SCREEN,		// drag in screen plane
				MAP_WORLD,		// drag in world plane
				MAP_LOCAL		// drag in local object plane
			};

};


//
// GPathHandler 
// handler maintain a path to a scene node 
//


class GPathHandler : public GEventHandler {
public:
	RT(GEventHandler);

	GPathHandler(GView *View =NULL) : GEventHandler(View) {  }
	~GPathHandler();

	// transform from world to local 
	MatrixAndInverse m;


	// path to node affected by handler 
	GvPath path;

	// hit information from ComputeRayHit
	GHitInfo lastHit;

	void World2Local(const Point &world, Point &local) {  local = m.Inverse() * world; }
	void Local2World(const Point &local, Point &world) {  world = m.M() * local; }




	// Compute hit ray with scene and initialize path
	gbool ComputeRayHit(RayRange	 &ray);



};




//
// GSensorHandler: event handler for handling VRML 2.0 geometry sensors 
//


class GSensorHandler : public GPathHandler { //{GTransformPathHandler {
public:
	RT(GSensorHandler);

	GSensorHandler(GView *View =NULL) : GPathHandler(View) 
			{ sensorType = NO_SENSOR; 
			  SetMapMode(MAP_LOCAL);
		  	  mapPlane.SetZ();
			}
	~GSensorHandler();

	Point firstS; // first point screen
	Point firstO; // first point object 	


	Point lastS; // last point screen
	Point lastO; // last point object 	

	Point centerS; // trackball center screen
	float radiusS; // trackball radius screen
	Point centerL; // trackball center local
	float radiusL; // trackball radius local
	gbool diskMode; // diskMode for cylinder Sensor 

	Point V0, V0L;
	Vector VectorS;

	MapMode mapMode;
	Plane mapPlane;

	// the drag computation is based on the last valid mouse move on the sphere/sensor
	Vector mouseMove;
	float lastRotation, mouseMoveLength;
	Vector rotationAxis;

	// in order to limit the rotation we need to save the accumulated rotation
	// the rotation quat [0;3.14] !
	float totalRotation;


	void SetMapMode(int mode) { if (mode >= MAP_SCREEN && mode <= MAP_LOCAL) mapMode = (MapMode) mode; }


	// map coordinates 
	gbool MapPoint(const Point &screen, Point &world, Point &local);



	Quaternation rotation;	// accumulated rotation 


	typedef enum {
		NO_SENSOR,
		TOUCH_SENSOR,
		PLANE_SENSOR,
		SPHERE_SENSOR,
		CYLINDER_SENSOR
	} SensorType;
	
	SensorType sensorType;

	// set the sensor node handled by the event handler 
	void SetSensor(GvNode *sensor);
	GvNodeHandle sensor;

	// set path  and matrix based on lastHit + sensor 
	gbool InitPath();


	int OnLButtonDown(UINT nFlags, Point &point);
	int OnLButtonDblClk(UINT nFlags, Point &point);
	int OnMouseMove(UINT nFlags,  Point &point);
	int OnLButtonUp(UINT nFlags,  Point &point);

};



//
// GMultitSensorHandler: event handler handling a set of geometry sensors 
//

class GMultitSensorHandler : public GEventHandler 
{


	// the list of activate sensor handlers
	Array<GSensorHandler *> handlers;
	gbool hasTouchSensors;

	RT(GMultitSensorHandler);

	GMultitSensorHandler(GView *View =NULL) : GEventHandler(View) { hasTouchSensors = FALSE; }
	~GMultitSensorHandler();


	// add a new handler to list
	void add( GSensorHandler * newHandler);
	// remove a new handler from list
	void remove(int i);

	// propagate events to all handlers and check for EV_POP
	int OnLButtonDown(UINT nFlags, Point &point);
	int OnLButtonDblClk(UINT nFlags, Point &point);
	int OnMouseMove(UINT nFlags,  Point &point);
	int OnLButtonUp(UINT nFlags,  Point &point);


};




#endif




