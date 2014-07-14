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
#include "stdafx.h"
#include <gvsensors.h>

#include <gvscene.h>
#include <gvelement.h>

// for proximity sensor
#include "quaternation.h"
#include "gcamera.h"
#include "gcamerautil.h"

// for view object 
#include "grender.h"
#include "gmodel.h"




//
// GvSensor 
//

GV_NODE_SOURCE_IMP(GvSensor,GvNode);

GvSensor::GvSensor(): enabled(TRUE),isActive(FALSE),isRegistered(FALSE)
{
    GV_NODE_CONSTRUCTOR(GvSensor);
    isBuiltIn = FALSE;
    GV_NODE_ADD_FIELD(enabled);
    // Event outs
    GV_NODE_ADD_EVENT_OUT(isActive);

}


GvSensor::~GvSensor()
{
}



//
// GvTimeSensor 
//
#if 0
//GvSFTime GvTimeSensor::theTime(0.0); // current simulation time
double GvTimeSensor::theTime = 0.0; // current simulation time


// get global simulation time
double GvGetTheTime() 
{
	return (GvTimeSensor::theTime);
}

// set global simulation time

void GvSetTheTime(double newTime) 
{
//	GvTimeSensor::theTime.set(newTime);
	GvTimeSensor::theTime = newTime;
}

#endif



GV_NODE_SOURCE_IMP(GvTimeSensor,GvSensor);

GvTimeSensor::GvTimeSensor() : loop(FALSE),
	cycleInterval(1.0),
	stopTime(0.0),
	startTime(0.0),
	cycleTime(-1.0),
	fraction(0.0),
	time(-1.0)
{
    GV_NODE_CONSTRUCTOR(GvTimeSensor);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(cycleInterval);
    GV_NODE_ADD_FIELD(enabled);
    GV_NODE_ADD_FIELD(loop);

    GV_NODE_ADD_EVENT_IN(set_startTime,GvSFTime);
    GV_NODE_ADD_EVENT_IN(set_stopTime,GvSFTime);
    
    GV_NODE_ADD_FIELD(stopTime);
    GV_NODE_ADD_FIELD(startTime);
    
    // Event outs
    GV_NODE_ADD_EVENT_OUT(cycleTime);
    GV_NODE_ADD_EVENT_OUT(isActive);
    GV_NODE_ADD_EVENT_OUT(fraction);
    GV_NODE_ADD_EVENT_OUT(time);


    oldTime = -1.1233333;
	oldTm = 0;
	nextScheduleTime = 0;
	last_setStartTime=0;
	last_setStartTimeTime=0;

}


GvTimeSensor::~GvTimeSensor()
{
/* HG removed 25.10.97, no proper global scene 
   GvScene *scene = getScene();
   if (scene) scene->removeTimeSensor(this);
*/
}


void GvTimeSensor::set_enabled(GvBool state)
{
   if (state == enabled) return;
   // OnTimeChange(event.t); // ????

   if (!state) {
		enabled.set(FALSE);
        setActive(FALSE);
   }
   else {
		enabled.set(TRUE);
        setActive(TRUE);
   }
}

void GvTimeSensor::set_startTime(GvSFTime *t)
{
/*
	CString x;
	x.Format("%p t = %g new start=%g cy %g %d \n",(void *) this,(double) *t,(double )GvGetTheTime()- *t,  
		(double) cycleInterval,(int)isActive);
	::OutputDebugString(x);
*/
	// TRACE("TimeSensor %p:: set_startTime t=%g active =%d \n",this,t->value,(int)isActive.value);	

	if (isActive) {
		// ignore
		last_setStartTime = t->value;
		last_setStartTimeTime = getTime();
	}
	else 
    {
		startTime.set(*t);
		nextScheduleTime = *t;
	}

	//TRACE("GvTimeSensor::set_startTime: Time delta between now & start time = %.16g \n", double(*t) - getTime());
}

void GvTimeSensor::set_stopTime(GvSFTime *t)
{
	// TRACE("TimeSensor %p::set_stopTime t=%g active =%d \n",this,t->value,(int)isActive.value);	
	if (isActive) {

		if (last_setStartTimeTime == getTime() && last_setStartTime == t->value) {
			startTime.set(*t);
			nextScheduleTime = *t;
			TRACE("TimeSensor %p:: should restart set_startTime t=%g active =%d \n",this,t->value,(int)isActive.value);	
		}
		if (*t < startTime) { // was <=
			// ignore
		}
		else {
			stopTime.set(*t);
			// really, make him inactive ???? 
			//???? 5.06.98 setActive(FALSE);
		}
	
	}
	else {
		stopTime.set(*t);
	}
}




int GvTimeSensor::OnInitTime(double t) 
{
	//oldTime=t;
	if (loop && (stopTime<=startTime)) {
            setActive(TRUE);
	}
	return (isActive);
}

// Timer sensors are triggered before rendering 

int GvTimeSensor::OnTimeChange(double t) 
{
	int ret=0;

		//if (t == oldTime) return(0);

		GvBool wasActive = isActive;

		if (wasActive) 
			ret = 1;

        if (!enabled) {
            setActive(FALSE);
			oldTime = t;
            return(ret);
        }
        
        if (t<startTime) { // nothing to do yet 
			nextScheduleTime = startTime.value;
            setActive(FALSE);
			oldTime = t;
            return(ret);
        }
        
        double tl= t-startTime; // the local time relative to startTimer

		// t >= startTime

        if (stopTime > startTime) {  // stopTime given 

		    double te = stopTime;	
		    if (!loop) te = min(startTime+cycleInterval,te);

			if (t>= (te+0.0000001))  // time expired stopTime 
			{
				// new 26.06.98
				GvBool shouldHaveBeenActive = !wasActive && (startTime > oldTime) && (oldTime>0);
				if (shouldHaveBeenActive) {
					setActive(TRUE);
					wasActive = TRUE;
				}
				//

				if (wasActive || ((startTime > oldTime && (startTime <t) && (oldTime>0) )) ) // create a event at stop Time 
				{
					//theTime = stopTime;

						//TRACE("TimeSensor disabled \n");

						
						double tm = fmod(stopTime-startTime,cycleInterval);
						
						if (tm < 1e-9  && stopTime>startTime) fraction.set(1.0);
						else fraction.set(tm/cycleInterval);

						if (tm <= 0.0001)   
							cycleTime.set(stopTime);

						// theTime=t;

						// time.set(stopTime); // ??  HG 26.06 could be long in the past ??
						time.set(t); // ?? 

						setActive(FALSE);
						
						// check if startTime has been changed inbetween  new 27.09.98 
						if (last_setStartTime >= t) startTime.set(last_setStartTime);


				}
				else nextScheduleTime = stopTime;
				
				oldTime = t;
				return(ret);
			}
		}
    
		double cycleInterval = this->cycleInterval.get();		
		double tm = fmod(tl,cycleInterval); // tm relativ to cycleInterval 
	
		nextScheduleTime = startTime +(floor(tl/cycleInterval)+1.0)*cycleInterval;
        
        if (loop) 
		{
				

				setActive(TRUE);
				// if (cycleTime.hasConnections())  no backpoint routes yet !!!!
				{

				GvBool cycleWrap=oldTm > tm; // interval wrap around 


				if (tm <= 0.0001  || !wasActive ) {  

					//long interval = (t / cycleInterval);
					//double theTime_ = (double)interval*cycleInterval;
					double theTime_ = startTime + floor(tl / cycleInterval)*cycleInterval;

					//TRACE("Cycle start %f \n",theTime_);
					cycleTime.set(theTime_);
					
					// theTime = t;
					// cycleTime.set(t);
				}
				else if (wasActive && cycleWrap ) {
					//long interval = (t / cycleInterval);
					//double theTime_ = (double)interval*cycleInterval;

					//double theTime_ = floor(t / cycleInterval)*cycleInterval;
					// 20.11.98	
					double theTime_ = startTime + floor(tl / cycleInterval)*cycleInterval;

					// TRACE("Cycle wrap %f \n",theTime_);

					cycleTime.set(theTime_);
					//theTime = t;
				}
				}
				
				oldTm = tm;

				// end of interval
				if (tm < 1e-9 && t>startTime) fraction.set(1.0);
                else fraction.set(tm/cycleInterval);

                time.set(t);
                //TRACE("Time sensor triggering t = %f frac = %f \n",time.value,fraction.value);

        } else { // not loop 
            if (tl >= cycleInterval) { 

				// new 26.06.98
				GvBool shouldHaveBeenActive = !wasActive && (startTime > oldTime) && (oldTime>0);
				if (shouldHaveBeenActive) {
					setActive(TRUE);
					wasActive = TRUE;
				}
				//

				if (wasActive || ((startTime > oldTime && (startTime <t) && (oldTime>0) ))) { // cycle expired  // do we missed the exact end ????

					fraction.set(1.0);

					//cycleTime.set(stopTime);

					//long interval = (t / cycleInterval);
					//double theTime_ = (double)interval*(double)cycleInterval;
					// double theTime_ = floor(t / cycleInterval)*cycleInterval;
					double theTime_ = startTime + floor(tl / cycleInterval)*cycleInterval;
					
					//TRACE("Cycle start %f \n",theTime_);
					cycleTime.set(theTime_);
					
					time.set(t); // hg new 

				
					setActive(FALSE);

					// check if startTime has been changed inbetween  new 27.09.98 
					if (last_setStartTime >= min(theTime_,t)) startTime.set(last_setStartTime);
				}
            } else {
				
				//TRACE("%p t = %g tl=%g cy %g frac = %g %d \n",(void *) this,t,tl,  (double) cycleInterval,tm/cycleInterval,wasActive);

                setActive(TRUE);
                // TRACE("Time sensor once triggering t = %f \n",time.value);

				BOOL tmSmall = tm <= 0.0001;
				
				if (tmSmall || !wasActive ) { 
					//long interval = (t / cycleInterval);
					//double theTime_ = (double)interval*(double)cycleInterval;

					// double theTime_ = floor(t / cycleInterval)*cycleInterval;					
					double theTime_ = startTime + floor(tl / cycleInterval)*cycleInterval;

					//TRACE("Cycle start %f \n",theTime_);
					
					cycleTime.set(theTime_);
					// theTime = t;
					
					// cycleTime.set(t);
				}
				
				//if (!wasActive  && tm<1e-9 && t>startTime) 
				//	fraction.set(1.0); // ??
                //else 
				if (wasActive && (tm<1e-9) && t>startTime) {
					fraction.set(1.0); 
				
				} 
				else fraction.set(tm/cycleInterval);

                time.set(t);

            }
			oldTm = tm;

        }    
		oldTime = t;
		return(ret||isActive.value);
}

int GvTimeSensor::Do(GTraversal &state)   
{ 
/* 	TimeSensors are triggered before rendering 
	temp

	GTimeStack *current,*prev;

	prev = (GTimeStack*) state.Get(GTimeStack::attrId);
	if (!prev) return(0);
	
	float t=prev->time.t;
    
    return OnTimeChange(t);
*/
	return(1);
}

// check for registration
int GvTimeSensor::Do(GglRenderTraversal &state)
{
	if (!isRegistered)
		state.getBrowser()->Register(this);
	return (1);

}


//
// GvGeometrySensor 
//

GV_NODE_SOURCE_IMP(GvGeometrySensor,GvSensor);

GvGeometrySensor::GvGeometrySensor() 
{
    GV_NODE_CONSTRUCTOR(GvGeometrySensor);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(enabled);
    // Event outs
    GV_NODE_ADD_EVENT_OUT(isActive);

}


GvGeometrySensor::~GvGeometrySensor()
{
}


int GvGeometrySensor::Do(GTraversal &state)
{
	if (!enabled) return(0);
	return(1);
}

int GvGeometrySensor::Do(GglRenderTraversal &state)
{
	return(0);
}


// put sensor in state for element GvWWWAnchorElement
// only  really needed for GRaySelectTraversal
int GvGeometrySensor::Apply(GTraversal &state,GvNode *parent)
{
	if (!enabled) return(0);

	GvWWWAnchorElement *current,*prev;
	prev = (GvWWWAnchorElement*) state.Get(GvWWWAnchorElement::attrId);
	if (!prev) {
	
	} else {
		current = (GvWWWAnchorElement *) prev->NewInstance();								
		current->data = this;
		current->parent = parent;
		state.Push(GvWWWAnchorElement::attrId,current);			
	}

	return(1);
}

GvBool GvGeometrySensor::NeedMouseDrag() 
{
  return TRUE; 
}


//
// GvCylinderSensor 
//

GV_NODE_SOURCE_IMP(GvCylinderSensor,GvGeometrySensor);

GvCylinderSensor::GvCylinderSensor()
{
    GV_NODE_CONSTRUCTOR(GvCylinderSensor);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(diskAngle);
    GV_NODE_ADD_FIELD(enabled);
    GV_NODE_ADD_FIELD(maxAngle);
    GV_NODE_ADD_FIELD(minAngle);
    GV_NODE_ADD_FIELD(offset);
    GV_NODE_ADD_FIELD(autoOffset);

    // Event outs
    GV_NODE_ADD_EVENT_OUT(isActive);
    GV_NODE_ADD_EVENT_OUT(rotation);
    GV_NODE_ADD_EVENT_OUT(trackPoint);

    diskAngle.value = 0.262;
    maxAngle.value = -1;
    minAngle.value = 0;
    offset.value = 0;
    autoOffset.value = TRUE;
}


GvCylinderSensor::~GvCylinderSensor()
{
}


//
// GvPlaneSensor 
//

GV_NODE_SOURCE_IMP(GvPlaneSensor,GvGeometrySensor);

GvPlaneSensor::GvPlaneSensor()
{
    GV_NODE_CONSTRUCTOR(GvPlaneSensor);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(enabled);
    GV_NODE_ADD_FIELD(minPosition);
    GV_NODE_ADD_FIELD(maxPosition);
    GV_NODE_ADD_FIELD(offset);
    GV_NODE_ADD_FIELD(autoOffset);

    // Event outs
    GV_NODE_ADD_EVENT_OUT(isActive);
    GV_NODE_ADD_EVENT_OUT(translation);
    GV_NODE_ADD_EVENT_OUT(trackPoint);

    maxPosition.value[0]=maxPosition.value[1]=-1;
    minPosition.value[0]=minPosition.value[1]=0;
    autoOffset.value = TRUE;


}


GvPlaneSensor::~GvPlaneSensor()
{
}

//
// GvProximitySensor 
//

GV_NODE_SOURCE_IMP(GvProximitySensor,GvSensor);

GvProximitySensor::GvProximitySensor() : 
enterTime(0),exitTime(0),lastPosition(-1e35f,0,0),lastRotation(-1e35f,0,0,0)
{
    GV_NODE_CONSTRUCTOR(GvProximitySensor);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(center);
    GV_NODE_ADD_FIELD(enabled);
    GV_NODE_ADD_FIELD(size);

    // Event outs
    GV_NODE_ADD_EVENT_OUT(isActive);
    GV_NODE_ADD_EVENT_OUT(position);
    GV_NODE_ADD_EVENT_OUT(orientation);
    GV_NODE_ADD_EVENT_OUT(enterTime);
    GV_NODE_ADD_EVENT_OUT(exitTime);

	lastTime=0;
	isLarge = -1;
}


GvProximitySensor::~GvProximitySensor()
{
}

int GvProximitySensor::OnFieldChanged(GvField *field)
{

	if ( (field == &center) || (field == &size) ) {
		// visibility status may changed 
		setRedraw();

		// force a recompute
		isLarge = -1;
		lastPosition.Set(-1e35f,0,0);

	}	
	return GvNode::OnFieldChanged(field); 
}


GvBool GvProximitySensor::getBBoxEx(BBox &bbox)
{
	bbox.SetCenterSize(center,size);
	return !bbox.IsEmpty();
}

// To Do: interpolated enter/exit time ??

int GvProximitySensor::Do(GglRenderTraversal &state)
{

	if (!state.doProximitySensors || !enabled) return(0);

	if (isLarge <0) { // compute a catchall sensor flag 
		if ( (size.value[0]>=1e30f) && 
			 (size.value[1]>=1e30f) && 
			 (size.value[2]>=1e30f)) 
		{
			isLarge = 1;
		}
		else isLarge = 0;

	}

		GvBool wasActive = isActive;

		Point eye = state.camera->position; // !!! we are using the logical avatars camera
		Point pos = center.value;
	
		eye *= state.GetMatrixInverse();

		// check if eye is in box 
	
		Point d;
		if (isLarge <= 0) {
			d = eye-pos;
		
			if (d.x<0) d.x=-d.x;
			if (d.y<0) d.y=-d.y;
			if (d.z<0) d.z=-d.z;
		}

		if ( isLarge ||  
			 (d.x<=(size.value[0]*0.5f) && 
			 d.y<=(size.value[1]*0.5f) && 
			 d.z<=(size.value[2]*0.5f)))
		{	// its in, trigger events 

			SetActive(TRUE);

			if (!(eye == lastPosition))
				position.set(eye);

			if (orientation.connections != NULL) // there may be ROUTES 
			{

				Vector axis;
				float angle;
				Point target = state.GetMatrixInverse() * state.camera->target ;
				Point up = state.camera->up;
				ConvertCameraToOrientation(eye, target,up,axis, angle);
				Rotation r(axis.x,axis.y,axis.z,angle);

				if (! (r == lastRotation)) {
					orientation.set(r);
					lastRotation=r;
				}
			}

			if (!wasActive)  {
				//intersectRayBox(lastPosition,eye-lastPosition,t);
				// the time = lastTime +  t *(curTim->lastTime);
				enterTime.set(state.view->theTime);
			}

		}
		else {
		   if (wasActive) {
				SetActive(FALSE);
				exitTime.set(state.view->theTime);
		   }	
		}
		lastPosition = eye;
		lastTime = state.view->theTime;


	return(1);
}


//
// GvSphereSensor 
//

GV_NODE_SOURCE_IMP(GvSphereSensor,GvGeometrySensor);

GvSphereSensor::GvSphereSensor()
{
    GV_NODE_CONSTRUCTOR(GvSphereSensor);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(enabled);
    GV_NODE_ADD_FIELD(offset);
    GV_NODE_ADD_FIELD(autoOffset);

    // Event outs
    GV_NODE_ADD_EVENT_OUT(isActive);
    GV_NODE_ADD_EVENT_OUT(rotation);
    GV_NODE_ADD_EVENT_OUT(trackPoint);

    offset.value.set(0,1,0,0);
    autoOffset.value = TRUE;

}


GvSphereSensor::~GvSphereSensor()
{
}


//
// GvTouchSensor 
//

GV_NODE_SOURCE_IMP(GvTouchSensor,GvGeometrySensor);

GvTouchSensor::GvTouchSensor() : isOver(FALSE),touchTime(0)
{
    GV_NODE_CONSTRUCTOR(GvTouchSensor);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(enabled);

    // Event outs
    GV_NODE_ADD_EVENT_OUT(hitNormal);
    GV_NODE_ADD_EVENT_OUT(hitPoint);
    GV_NODE_ADD_EVENT_OUT(hitTexCoord);
    GV_NODE_ADD_EVENT_OUT(isActive);
    GV_NODE_ADD_EVENT_OUT(isOver);
    GV_NODE_ADD_EVENT_OUT(touchTime);

}


GvTouchSensor::~GvTouchSensor()
{
}


GvBool GvTouchSensor::NeedMouseDrag() 
{

	if (hitNormal.hasConnections()
		|| hitPoint.hasConnections()
		|| hitTexCoord.hasConnections()
		|| isActive.hasConnections()
	)
		return TRUE; 
	else return FALSE;
}

//
// GvMouseSensor
//

GV_NODE_SOURCE_IMP(GvMouseSensor,GvTouchSensor);

GvMouseSensor::GvMouseSensor() 
{
    GV_NODE_CONSTRUCTOR(GvMouseSensor);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(enabled);

    // Event outs
    GV_NODE_ADD_EVENT_OUT(hitNormal);
    GV_NODE_ADD_EVENT_OUT(hitPoint);
    GV_NODE_ADD_EVENT_OUT(hitTexCoord);
    GV_NODE_ADD_EVENT_OUT(isActive);
    GV_NODE_ADD_EVENT_OUT(isOver);
    GV_NODE_ADD_EVENT_OUT(touchTime);
    GV_NODE_ADD_EVENT_OUT(event);

}


GvMouseSensor::~GvMouseSensor()
{
}

//
// GvVisibilitySensor 
//

GV_NODE_SOURCE_IMP(GvVisibilitySensor,GvSensor);

GvVisibilitySensor::GvVisibilitySensor() : enterTime(0),exitTime(0)
{
    GV_NODE_CONSTRUCTOR(GvVisibilitySensor);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(enabled);
    GV_NODE_ADD_FIELD(center);
    GV_NODE_ADD_FIELD(size);

    // Event outs
    GV_NODE_ADD_EVENT_OUT(isActive);
    GV_NODE_ADD_EVENT_OUT(enterTime);
    GV_NODE_ADD_EVENT_OUT(exitTime);


	wasRendered = FALSE;

}


GvVisibilitySensor::~GvVisibilitySensor()
{
}

GvBool GvVisibilitySensor::getBBoxEx(BBox &bbox)
{
	bbox.SetCenterSize(center,size);
	return !bbox.IsEmpty();
}

int GvVisibilitySensor::OnFieldChanged(GvField *field)
{

	if ( (field == &center) || (field == &size) ) {
		// visibility status may changed 
		setRedraw();
	}	
	return GvNode::OnFieldChanged(field); 
}



int GvVisibilitySensor::Do(GglRenderTraversal &state)
{
	if (!state.doVisibilitySensors || !enabled) 
		return(0);

	if (!state.CullCheck(center,size)) 
		return GV_NODE_CULLED;

	// if first rendered, enable
	// works with Bsp
	// TO DO: if Group viewCulling 

	GvBool wasActive = isActive;
	if (!wasActive) {
		isActive.set(TRUE);
		enterTime.set(state.view->theTime);
		state.view->activeVisibilitySensors.add(this);
	}
	wasRendered = TRUE; // that flag that is was traversed
	// this means visible for now 

	return(1);

}



//
// GvMenuSensor 
//

GV_NODE_SOURCE_IMP(GvMenuSensor,GvSensor);

GvMenuSensor::GvMenuSensor(): choices(0),descriptions(0),choice(0),position("BOTTOM")
{
    GV_NODE_CONSTRUCTOR(GvMenuSensor);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(enabled);
    GV_NODE_ADD_FIELD(title);
    GV_NODE_ADD_FIELD(choices);
    GV_NODE_ADD_FIELD(descriptions);
    GV_NODE_ADD_FIELD(position);

    // Event outs
    GV_NODE_ADD_EVENT_OUT(isActive);
    GV_NODE_ADD_EVENT_OUT(choice);

}


GvMenuSensor::~GvMenuSensor()
{
}

int GvMenuSensor::Do(GglRenderTraversal &state)
{
	if (!isRegistered) {
		state.getBrowser()->Register(this);
	}
	return(0);
}


//
// GvRayHitSensor 
//

GV_NODE_SOURCE_IMP(GvRayHitSensor,GvSensor);

GvRayHitSensor::GvRayHitSensor(): ray(0),children(0),touchTime(0)
{
    GV_NODE_CONSTRUCTOR(GvTouchSensor);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(enabled);
    GV_NODE_ADD_FIELD(ray);
    GV_NODE_ADD_FIELD(children);

    // Event outs
    GV_NODE_ADD_EVENT_OUT(hitNormal);
    GV_NODE_ADD_EVENT_OUT(hitPoint);
    GV_NODE_ADD_EVENT_OUT(hitTexCoord);
    GV_NODE_ADD_EVENT_OUT(isActive);
    GV_NODE_ADD_EVENT_OUT(touchTime);

}


GvRayHitSensor::~GvRayHitSensor()
{
}

int GvRayHitSensor::Do(GTraversal &state)
{

	if (!enabled) return(0);
	return(1);
}

// a field has been changed
int GvRayHitSensor::OnFieldChanged(GvField *field)
{
	if (field == &ray) {
		return OnTrigger();
	} else 	if (field == &enabled) {
		return OnTrigger();
	}
	else return GvSensor::OnFieldChanged(field);
}



#include "grayselect.h"


int GvRayHitSensor::OnTrigger()
{
   if (!enabled) { 
	   if (isActive) isActive.set(FALSE); 
	   return(0);
   }
   if (ray.getNum() <2) return(0);


   GHitInfo *hit = NULL;
   GRaySelectTraversal traversal;
   traversal.method = GRaySelectTraversal::BY_OBJECT; 
   traversal.storeFullHitInfo = 1;
   traversal.view = (GView *) getBrowser();
   // ??? InitializeGvAttributes(traversal);
   traversal.InitializeAttributes();

   Point objP,objP1;
   ray.get1(0, objP);
   ray.get1(1, objP1);

   traversal.ray.SetByPoints(objP,objP1);
   children.Traverse(traversal);

   hit = traversal.GetHit();

   if (!hit) return(0);

   if (hit ) {
		if (hit->vn.Length()>0) 
			hitNormal.set(hit->vn);

		hitTexCoord.set(hit->vp);
		hitPoint.set(hit->pw);	// to do : in what coordinate sys

		if (!isActive) isActive.set(TRUE); 
		
		touchTime.set(getTime());

		//hitOrientation = 
   } else {
	  if (isActive) isActive.set(FALSE); 
   }		   

   return(1);
}


//
// GvEvent 
//

GV_NODE_SOURCE_IMP(GvEvent,GvNode);

GvEvent::GvEvent()
{
    GV_NODE_CONSTRUCTOR(GvEvent);
    isBuiltIn = FALSE;

    GV_NODE_ADD_EVENT_OUT(type);

    GV_NODE_ADD_FIELD(cancelBubble);

	// Event outs
    GV_NODE_ADD_EVENT_OUT(altKey);
    GV_NODE_ADD_EVENT_OUT(ctrlKey);
    GV_NODE_ADD_EVENT_OUT(shiftKey);
    GV_NODE_ADD_EVENT_OUT(button);
    GV_NODE_ADD_EVENT_OUT(keyCode);

    GV_NODE_ADD_EVENT_OUT(client);
    GV_NODE_ADD_EVENT_OUT(screen);
    GV_NODE_ADD_EVENT_OUT(position);

    GV_NODE_ADD_EVENT_OUT(target);

    GV_NODE_ADD_FIELD(returnValue);


}
GvEvent::~GvEvent()
{
}



void GvEvent::setMouseStatus(float x,float y, unsigned int flags) 
{
	// win32 specific 
	//ctrlKey.set( (flags & MK_CONTROL) !=0); // seems not to be present !
	//shiftKey.set( (flags & MK_SHIFT) !=0); 
	int m=0;
	if (flags & MK_LBUTTON) m=1;   
	if (flags & MK_RBUTTON) m|=2;   
	if (flags & MK_MBUTTON) m|=4;
	button.set(m);   
	client.set(x,y);
}


int GvEvent::Do(GTraversal &state)
{
	return(0);
}

// a field has been changed
int GvEvent::OnFieldChanged(GvField *field)
{
	return(0);
}



//
// GvRayHitInfo 
//

GV_NODE_SOURCE_IMP(GvRayHitInfo,GvNode);

GvRayHitInfo::GvRayHitInfo(): hitPath(0),hitInfo(0)
{
    GV_NODE_CONSTRUCTOR(GvRayHitInfo);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(hitPath);

    GV_NODE_ADD_FIELD(hitNormal);
    GV_NODE_ADD_FIELD(hitPoint);
    GV_NODE_ADD_FIELD(hitTexCoord);

    GV_NODE_ADD_FIELD(hitNormalLocal);
    GV_NODE_ADD_FIELD(hitPointLocal);
    GV_NODE_ADD_FIELD(hitMatrix);
    GV_NODE_ADD_FIELD(hitInfo);
 
}


GvRayHitInfo::~GvRayHitInfo()
{
}

int GvRayHitInfo::Do(GTraversal &state)
{
	return(0);
}

// a field has been changed
int GvRayHitInfo::OnFieldChanged(GvField *field)
{
	return(0);
}





int GvRayHitInfo::OnTrigger(GView *browser,const Point &objP,const Point &objP1,GvNode *node)
{


   GHitInfo *hit = NULL;
   GRaySelectTraversal traversal;
   traversal.method = GRaySelectTraversal::BY_OBJECT; 
   traversal.storeFullHitInfo = 1;
   traversal.mode= GRaySelectTraversal::MODE_RAYHIT;
   traversal.storePath = 1;

   traversal.view = browser;
   //InitializeGvAttributes(traversal);
   traversal.InitializeAttributes();

   traversal.SetCamera(browser->globalCamera);
   traversal.SetPhysicalCamera(browser->GetPhysicalCamera());


   traversal.ray.SetByPoints(objP,objP1);
   
   if (node) node->Traverse(traversal);
   else browser->Traverse(traversal);

   hit = traversal.GetHit();

   if (!hit) return(0);

   if (hit ) {
		hitNormal.set(hit->vn);

		hitTexCoord.set(hit->vp);
		hitPoint.set(hit->pw);	
		hitPointLocal.set(hit->p);	
		hitNormalLocal.set(hit->vn);	
		hitMatrix.set(hit->m);

		hitInfo.makeRoom(4);

		hitInfo[0]=hit->findex;
		hitInfo[1]=hit->tvindex0;
		hitInfo[2]=hit->tvindex1;
		hitInfo[3]=hit->tvindex2;

		//if (!isActive) isActive.set(TRUE); 
		hitPath.makeRoom(hit->path.Length());
		for(int i=0; i<hit->path.Length();i++) {
			hitPath[i]=RTCAST(hit->path.path[i].node,GvNode);
		}	
		

		//hitOrientation = 
   } else {
	  //if (isActive) isActive.set(FALSE); 
   }		   

   return(1);
}

//
// GvCollisionInfo 
//

GV_NODE_SOURCE_IMP(GvCollisionInfo,GvNode);

GvCollisionInfo::GvCollisionInfo(): hitPath(0),hitInfo(0)
{
    GV_NODE_CONSTRUCTOR(GvRayHitInfo);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(hitPath);

    GV_NODE_ADD_FIELD(hitNormal);
    GV_NODE_ADD_FIELD(hitPoint);
    GV_NODE_ADD_FIELD(hitTexCoord);

    GV_NODE_ADD_FIELD(hitNormalLocal);
    GV_NODE_ADD_FIELD(hitPointLocal);
    GV_NODE_ADD_FIELD(hitMatrix);
    GV_NODE_ADD_FIELD(hitInfo);
 
}


GvCollisionInfo::~GvCollisionInfo()
{
}

int GvCollisionInfo::Do(GTraversal &state)
{
	return(0);
}

// a field has been changed
int GvCollisionInfo::OnFieldChanged(GvField *field)
{
	return(0);
}



#include "collision/rapid/rapid.h"
#include "gvnodeshell.h"
#include "gshell.h"
#include "gvisibility.h"


int GvCollisionInfo::OnTrigger(GView *browser, GvNode *a, Matrix *aMatrix, GvNode *b,Matrix *bMatrix,int options)
{
   GShell *speedBoxGlobal = NULL;

   GConvexVolume volume;


   // setup traversal structure
   GHitInfo *hit = NULL;

   GRaySelectTraversal traversal;
   traversal.method = GRaySelectTraversal::BY_OBJECT; 
   traversal.storeFullHitInfo = 1;
   //traversal.mode= GRaySelectTraversal::MODE_COLLISION;

   rapidCollide	myRapid;
   myRapid.RAPID_contact_flag = FIRST_CONTACT;

   traversal.mode= GRaySelectTraversal::MODE_RAPID;
   traversal.myRapid = &myRapid;

   traversal.colCyclesPerFrame = 0;
   if (aMatrix) traversal.speedBoxMat = *aMatrix;
   traversal.collision = FALSE;
		
   traversal.mode= GRaySelectTraversal::MODE_RAPID;

   traversal.storePath = 1;

   traversal.view = browser;
   //InitializeGvAttributes(traversal);
   traversal.InitializeAttributes();

   traversal.SetCamera(browser->globalCamera);
   traversal.SetPhysicalCamera(browser->GetPhysicalCamera());
 
   
   // get the Shell object from the geometry node

   if (!a) return 0;
	
   GvNodeShell *ns;
   ns = RTCAST(a,GvNodeShell);

	
   if (!ns) 
		return 0;

	GShell *s=ns->GetShell();
	if (!s) {
		ns->BuildShell(traversal);
		s=ns->GetShell();
	}

	if (!s) 
		return 0;
	
	traversal.speedBox = s;
	speedBoxGlobal = new GShell;
	
	// mm wrong for GShellI ??
	speedBoxGlobal->SetV(s->v);	
	speedBoxGlobal->SetFaceList(s->f);	
	traversal.speedBoxGlobal = speedBoxGlobal;
	traversal.speedBoxVolume =  &volume;
	// const Array<Plane> planes;

	//volume.bbox = s->
	//volume.Set(s->v,aMatrix,aMatrix.Inverse(),planes);
	if (aMatrix)
	volume.Set(s->GetBBox(),*aMatrix);
	else volume.Set(s->GetBBox());

   //traversal.ray.SetByPoints(objP,objP1);
   if (bMatrix) {
   }	
   
   if (b) b->Traverse(traversal);
   else browser->Traverse(traversal); // TraverseWithoutAvatar

   hit = traversal.GetHit();

   if (!hit) return(0);

   if (hit ) {
		hitNormal.set(hit->vn);

		hitTexCoord.set(hit->vp);
		hitPoint.set(hit->pw);	
		hitPointLocal.set(hit->p);	
		hitNormalLocal.set(hit->vn);	
		hitMatrix.set(hit->m);

		hitInfo.makeRoom(4);

		hitInfo[0]=hit->findex;
		hitInfo[1]=hit->tvindex0;
		hitInfo[2]=hit->tvindex1;
		hitInfo[3]=hit->tvindex2;

		//if (!isActive) isActive.set(TRUE); 
		hitPath.makeRoom(hit->path.Length());
		for(int i=0; i<hit->path.Length();i++) {
			hitPath[i]=RTCAST(hit->path.path[i].node,GvNode);
		}	
		

		//hitOrientation = 
   } else {
	  //if (isActive) isActive.set(FALSE); 
   }		   

   return(1);
}

