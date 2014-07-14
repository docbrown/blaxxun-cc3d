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
@doc

@module GVSound.cpp - VRML 2.0 sound nodes |

Copyright (c) 1995-1998	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:

Changes:

$Revision: 1.41 $
$Log: gvsound.cpp,v $
Revision 1.41  1999/07/06 16:46:14  tom
*** empty log message ***

Revision 1.40  1999/05/10 13:55:10  tom
*** empty log message ***



Todo :

******************************************************************************/


#include "stdafx.h"


#include "gversion.h"
#include "gsound.h"

#include <gvsound.h>
#include <gvsensors.h>

#include "grender.h"
#include "gmodel.h"

#include <gvelement.h>
#include "gvtraverse.h"

#include "greporter.h"


#ifdef G_DSOUND

#include "gdssound.h"

#include "dsutil3d.h"

#endif 

// read a sound file 
// into vrml compatible field types 

BOOL ReadSoundFile(LPCTSTR lpName,GvMFInt32 &waveFormat,GvMFInt32 &waveData)
{
 
#ifdef G_DSOUND
	
	LPWAVEFORMATEX  lpwfxFormat=NULL;

    BYTE *pbWaveData;
	DWORD dwBufferBytes;
    void *pvBase;
	int i;
	HRESULT result=0;


	if (DSGetWaveFile(NULL, lpName, &lpwfxFormat, &pbWaveData,
                        &dwBufferBytes, &pvBase,result))
	{
 	        
		if (lpwfxFormat->wFormatTag == WAVE_FORMAT_PCM) 
        {
			i=0;
			waveFormat.makeRoom(6);
			waveFormat[i++]= lpwfxFormat->wFormatTag;
			waveFormat[i++]= lpwfxFormat->nChannels;
			waveFormat[i++]= lpwfxFormat->nSamplesPerSec;
			waveFormat[i++]= lpwfxFormat->nAvgBytesPerSec;
			waveFormat[i++]= lpwfxFormat->nBlockAlign;
			waveFormat[i++]= lpwfxFormat->wBitsPerSample;
			
			//waveFormat[1]= lpwfxFormat->cbSize;

			waveData.makeRoom( (dwBufferBytes +sizeof(int)-1 ) / sizeof(int));
			// could be not even size but so what

			memcpy(waveData.values,pvBase,dwBufferBytes);

			UnmapViewOfFile (pvBase);
			return TRUE;
        }
        else
        {

        }
		UnmapViewOfFile (pvBase);
		return FALSE;
	}
#endif

	return FALSE;

}

//
// GvAudioClip
//

GV_NODE_SOURCE_IMP(GvAudioClip,GvNode);

GvAudioClip::GvAudioClip() : 
      loop(FALSE),pitch(1.0),startTime(0),stopTime(0),url(0),
		  isActive(FALSE),duration(0.0),isRegistered(FALSE)
{
    GV_NODE_CONSTRUCTOR(GvAudioClip);
    isBuiltIn = TRUE;

    GV_NODE_ADD_EVENT_IN(set_startTime,GvSFTime);
    GV_NODE_ADD_EVENT_IN(set_stopTime,GvSFTime);


    GV_NODE_ADD_FIELD(description);
    GV_NODE_ADD_FIELD(loop);
    GV_NODE_ADD_FIELD(pitch);
    
    GV_NODE_ADD_FIELD(startTime);

    GV_NODE_ADD_FIELD(stopTime);
    GV_NODE_ADD_FIELD(url);

    GV_NODE_ADD_EVENT_OUT(duration);
    GV_NODE_ADD_EVENT_OUT(isActive);

	nextScheduleTime = 0;
	last_setStartTime=0;
	last_setStartTimeTime=0;

}


GvAudioClip::~GvAudioClip()
{
}


void GvAudioClip::set_startTime(GvSFTime *t)
{
	if (isActive) {
		// ignore
		last_setStartTime = t->value;
		last_setStartTimeTime = getTime();
	}
	else 
    {
		if (loader.GetState() == S_NOT_LOADED)  // url was changed, or not yet loaded 
			NotifyOnAudioClipUnload(); 

		startTime.set(*t);
		nextScheduleTime = *t;
	}
}

void GvAudioClip::set_stopTime(GvSFTime *t)
{

	if (isActive) {
		if (last_setStartTimeTime == getTime() && last_setStartTime == t->value) {
			startTime.set(*t);
			nextScheduleTime = *t;
			TRACE("AudioClip %p:: should restart set_startTime t=%g active =%d \n",this,t->value,(int)isActive.value);	
		}

		if (*t <= startTime) {
			// ignore
		}
		else {
			stopTime.set(*t);
			// really, make him inactive ???? 
			setActive(FALSE);
		}
	
	}
	else {
		stopTime.set(*t);
	}
}



// a field has been changed
int GvAudioClip::OnFieldChanged(GvField *field)
{
//	if (field == &isActive) {
		//Reporter.Error("Is Active changed %d \n",isActive.value);
//		return(0);
//	}
	if (field == &url) {
		TRACE("GvAudioClip::OnFieldChanged(&url)\n");
		loader.SetNotLoaded();

		if (url.getNum() == 0) // new 6.10.98
		   NotifyOnAudioClipUnload(); // to unload the sound 

		setRedraw();
		// notify owners  ???? 
		return(GvNode::OnFieldChanged(field));
	}
	if (field == &pitch) {
		return NotifyOnAudioClipPitchChanged();
	}
	else {
		setRedraw();

		return(GvNode::OnFieldChanged(field));
	}
}


// get absolute home url
const char *GvAudioClip::GetHomeUrl() const
{
	// has GetHome just been setup ?????
	return(loader.GetHomeUrl());
}

// get the local file name 
const char *GvAudioClip::GetLocalFile() const
{
	return(loader.GetLocalFile());
}

// get the local file format 
int GvAudioClip::GetLocalFileFmt() const
{
	return(loader.GetLocalFileFmt());
}
	//! there was a problem with the local file 
int GvAudioClip::OnLocalFileError(HRESULT result)
{

	if ((result == GS_FILE_TRUNCATED) || (result == GS_FILE_BAD)) {
		loader.DeleteCachedLocalFile();
	} 

	if (result >= GS_NOT_PCM_FORMAT) {
		loader.SetBadData();
		//isLoaded.set(FALSE);
	}


	return 0;
}


// notify all registered Sound owners, that this child has been loaded
int GvAudioClip::NotifyOnAudioClipLoaded()
{
	int ret = 0;
	this->ref();

	for(int i =owners.getLength()-1; i>=0; i--) {
		GvNode * owner = owners[i];
		ASSERT(owner != NULL);
		GvSound * parent = RTCAST( (GvNode *)owner,GvSound);
		if (parent)
			ret=parent->OnAudioClipLoaded(this);
	}

/*
	GvConnection *c = getConnections();

	while (c!= NULL) {
		GvOwnerConnection *connection = RTCAST(c,GvOwnerConnection);
		if ( connection) {
			GvNodeHandle to = connection->to;
			GvSound * parent = RTCAST( (GvNode *)to,GvSound);
			if (parent)
				ret=parent->OnAudioClipLoaded(this);
			
		}
		c = c->next;
	}
*/
	this->unref();
	return ret;

}

int GvAudioClip::NotifyOnAudioClipUnload()
{
	int ret = 0;
	this->ref();

	for(int i=owners.getLength()-1; i>=0; i--) {
		GvNode * owner = owners[i];
		ASSERT(owner != NULL);
		GvSound * parent = RTCAST( (GvNode *)owner,GvSound);
		if (parent)
			ret=parent->OnAudioClipUnload(this);
	}

/*
	GvConnection *c = getConnections();

	while (c!= NULL) {
		GvOwnerConnection *connection = RTCAST(c,GvOwnerConnection);
		if ( connection) {
			GvNodeHandle to = connection->to;
			GvSound * parent = RTCAST( (GvNode *)to,GvSound);
			if (parent)
				ret=parent->OnAudioClipUnload(this);
			
		}
		c = c->next;
	}
*/
	this->unref();
	return ret;

}

int GvAudioClip::NotifyOnAudioClipPitchChanged()
{
	int ret = 0;
	this->ref();

	for(int i=owners.getLength()-1; i>=0; i--) {
		GvNode * owner = owners[i];
		ASSERT(owner != NULL);
		GvSound * parent = RTCAST( (GvNode *)owner,GvSound);
		if (parent)
			ret=parent->OnAudioClipPitchChanged(this);
	}

/*
	GvConnection *c = getConnections();

	while (c!= NULL) {
		GvOwnerConnection *connection = RTCAST(c,GvOwnerConnection);
		if ( connection) {
			GvNodeHandle to = connection->to;
			GvSound * parent = RTCAST( (GvNode *)to,GvSound);
			if (parent)
				ret=parent->OnAudioClipPitchChanged(this);
			
		}
		c = c->next;
	}
*/

	this->unref();
	return ret;

}




int 			
GvAudioClip::Load(GTraversal &state)
{
	// whats the current status
	GFileState status = loader.GetState();

Redo:
	switch(status) {
	case S_BAD_DATA:
	case S_BAD_URL:
		if (loader.urlI< (url.Length() -1) ) {
			loader.urlI ++;
			// fall through and load next url 
		} else {
			status = loader.state = S_ALL_BAD; // can´t load any URL 
			NotifyOnAudioClipUnload(); 
		    break;
		}
	case S_NOT_LOADED: // starting state : NOT loaded 
	     if (url.Length() == 0) { 
			 loader.SetNoUrl();		
			 NotifyOnAudioClipUnload(); 
			 return(0);
		 }
		 if (state.view) {
			GSoundDevice *device = state.view->GetSoundDevice();
			if (device == NULL) {	// no sound device, ignore dowload 
				loader.SetNoUrl();		
				return(0);
			}
			// if device.m_passUrl ...

		 }

		 if (!loader.HasBaseUrl())
			loader.SetBaseUrl(state.GetHomeUrl());

		 
		 // start processing 
		 {
		 CTime refreshTime(0);
		 const char *refedFromUrl= state.GetUrl(refreshTime);	 


		 status = loader.Load(loader.GetBaseUrl(),url[loader.urlI].getString(),
			 refedFromUrl,"AudioClip",state.inlineWwwFlags,state.view->hWnd,loader.urlI,refreshTime,state.view->GetReporter(),state.view->file);
		 }
 		 // set url parameters
		 //worldUrl.set(loader.GetUrl());
		 //baseUrl.set(loader.GetHomeUrl());

		 if (status != S_NOT_LOADED) goto Redo;

		 break;

	case S_LOADING:
	case S_PROCESSING:
				state.pendingLoads ++;
				break;

	case S_LOADED: {
			int ret=-1;

		 	if (state.view) {
				state.view->lastFrameLoadCnt++;
	 		    state.view->lastFrameSoundLoadCnt++;
			 }

			 // ok its now loaded, Owner Sound node must detect it ! 
			 ret = 1;



			if (ret>=0) {  // fine, we could read it 
					loader.SetProcessed();
					NotifyOnAudioClipLoaded(); 	// notify all registered Sound owners, that this child has been loaded

			}
			else { 
					// try to delete 
					loader.DeleteCachedLocalFile();

					loader.SetBadData(); // bad, try next
					if (state.view) state.view->lastFrameLoadErrorCnt++;
			}
				
			
			return(1);
			}
	case S_PROCESSED:
				return(1);
	default :
		break;

	}
	return(0);
}


// time has been changed, check active state 

int GvAudioClip::OnTimeChange(double t) 
{

	GvBool wasActive = isActive;

	//TRACE(" AC t %g start %g stop %g \n",t,startTime.get(),stopTime.get());

    if (t<startTime) {
		//nextScheduleTime = startTime.value;
        setActive(FALSE);
        return(isActive);
    }
        

	// t >= startTime

	 // if stopTime valid  ?? 
     if (stopTime > startTime )  // we have an stop time
     {
		   double te = stopTime;	
		   if (!loop) te = min(startTime+duration,te);

		   if ( t>= te)  {
			   setActive(FALSE);
		   }
		   else {
			   setActive(TRUE);
		   }

	 }
	 else {
		 // no stopTime 
		 if (loop)	  {
			setActive(TRUE);
		 }
		 else {
			 // check if single cycle expired
			 if (!wasActive /* && duration >0*/  && ((startTime + duration) < t))
				 setActive(FALSE);
				 // if already active sound system decides on stop 
			 else
				 setActive(TRUE);
		 }
	 }

	 return(isActive);
}

GvBool GvAudioClip::SoundNeededNow(double t) 
{
	float TEPS = 5.0;        

    if (t< (startTime-TEPS)) {
		return FALSE;
	}

	 // if stopTime valid and expired ?? 
     if (stopTime > startTime && t>= (stopTime))  
     {
		 return FALSE;
	 }

	 return(TRUE);
}


int GvAudioClip::Do(GLoadInlinesTraversal &state)
{
  if (state.resolveRelativeUrls) {
	 loader.SetBaseUrl(state.GetHomeUrl());
  }


  if (!loader.Ok() && state.loadSounds >= GTraversal::LOAD_IF_NEEDED) {
		  Load(state);
  }	

  return(0);
}



int 			
GvAudioClip::Do(GglRenderTraversal &state)
{
	if (!isRegistered)
		state.getBrowser()->Register(this);

	if (!loader.Ok()) { // we need to check the Loader state 

		if (state.loadSounds >= GTraversal::LOAD_IF_NEEDED)
			Load(state);
	}	

	if (loader.Ok()) { 
	    return(1);
	}
	return(0);
}



//
// GvSound
//

GV_NODE_SOURCE_IMP(GvSound,GvNode);

GvSound::GvSound() : 
    intensity(1),maxBack(10),maxFront(10),
     minBack(1), minFront(1), priority(0),spatialize(TRUE),
	 isRegistered(FALSE)
{
    GV_NODE_CONSTRUCTOR(GvSound);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(direction);
    GV_NODE_ADD_FIELD(intensity);
    GV_NODE_ADD_FIELD(location);
    GV_NODE_ADD_FIELD(maxBack);
    GV_NODE_ADD_FIELD(maxFront);
    GV_NODE_ADD_FIELD(minBack);
    GV_NODE_ADD_FIELD(minFront);
    GV_NODE_ADD_FIELD(priority);
    GV_NODE_ADD_FIELD(source);
    GV_NODE_ADD_FIELD(spatialize);

    direction.value[0] = direction.value[1] = 0.0;
    direction.value[2] = 1.0; 
    location.value[0] = location.value[1] = location.value[2] = 0.0; 

	emitter = NULL;
	soundState = NOT_INITIALIZED; 
	ignoreChildNotify=0;
	doUpdateModel = TRUE;

}


GvSound::~GvSound()
{
	Term();
}

GvBool GvSound::getBBoxEx(BBox &bbox)
{
	float radius = maxFront * 2;
	bbox.SetCenterSize(location,Point(radius,radius,radius)); // bit to large, so what
	return (!bbox.IsEmpty());
}



// a field has been changed
int GvSound::OnFieldChanged(GvField *field)
{
	setRedraw();
	if (field == &source || field == &spatialize) {
		Term(); // must reinit whole stuff 
	}
	else if ((field == &maxBack) || (field == &maxFront) ||
		(field == &minBack) || (field == &minFront) || (field == &intensity)){
			// we need to update the model 
			doUpdateModel = TRUE;
	}
	return GvNode::OnFieldChanged(field);
}


// release the sound 
void GvSound::Term()
{
	if (emitter) { // free the emitter
		if (soundState == PLAYING) 	emitter->Stop();
		emitter->Release();
		emitter->unref();
		emitter = NULL;
	} 
	soundState = NOT_INITIALIZED; 
}


// initialize sound from audioClip
int 			
GvSound::Initialize(GGeometryTraversal &state,GvAudioClip *audioClip)
{
	GSoundDevice *device = state.view->GetSoundDevice();

	if (!device) {
		soundState = CANT_INITIALIZE;
		return 0;
	}

	// create emitter 
	if (!emitter) {
		emitter = device->newSoundEmitter(audioClip->GetLocalFileFmt());
		emitter->ref();
	} else {
		emitter->Release();
	}

	if (!emitter) {
		soundState = CANT_INITIALIZE;
		return FALSE;
	}

	if (emitter->Initialize(device,audioClip->GetLocalFile(),spatialize)) {

		if (emitter->m_result>=GS_NOT_PCM_FORMAT) {
			audioClip->OnLocalFileError(emitter->m_result);
		}
		emitter->SetUsed(); // mark it beeing traversed
		doUpdateModel = TRUE; // update all model parameters


		UpdateEmitter(state,audioClip);



		// query state of sound 
		BOOL playing; float secondsPlayed; float totalSeconds; int numLoops; float level;
		//t = (now - startTime) modulo (duration / pitch)							

		// get duration 
		if (emitter->QueryInfo(playing,secondsPlayed,totalSeconds, numLoops,level)) {
			audioClip->duration.set(totalSeconds);
		}

		soundState = INITIALIZED;

		// lets update all deffered settings now 
		device->NeedCommit();
		device->CheckCommit();

		OnTimeChange(state.view->theTime);

		if (emitter->QueryInfo(playing,secondsPlayed,totalSeconds, numLoops,level)) {
		}

	} else {
		GReporter *reporter = state.view->GetReporter();
		reporter->Warning("Cant't load AudioClip %s \n",(const char *) audioClip->GetLocalFile());
		audioClip->OnLocalFileError(emitter->m_result);
	}
	return (soundState >= INITIALIZED);
}

// update emitters position & location values 
int 			
GvSound::UpdateEmitter(GGeometryTraversal &state,GvAudioClip *audioClip)
{
	
		Point pos,dir;
		location.get(pos);
		direction.get(dir);

		// transform all values 

		const Matrix &m = state.GetMatrixRef();

		float s = m.ScaleFactor();

		pos *= m;
		dir = RotateOnly(m,dir);
		dir.Normalize();

		emitter->SetPosition(pos);
		emitter->SetOrientation(dir);
		emitter->SetPitch(audioClip->pitch);
		emitter->m_priority = (float) priority;

		if (doUpdateModel) {
			float			fMinBack = minBack;
			float			fMinFront = minFront;
			float			fMaxBack = maxBack;
			float			fMaxFront = maxFront;
			float			fIntensity = intensity;

			emitter->SetModel(fMinBack*s,fMinFront*s,fMaxBack*s,fMaxFront*s,fIntensity);
			doUpdateModel = FALSE;
		}

	
		
		return(1);


}

// check playstart for time t
int GvSound::OnTimeChange(double t) 
{
	GvNode *n=source.Eval();

	if (n == NULL) {
		soundState = CANT_INITIALIZE;
		return (0);
	}
	
	GvAudioClip *audioClip = NULL;
			
	if (RTISA(n,GvAudioClip)) { // we found a AudioClip Source 
		audioClip  = (GvAudioClip *) n;
	}


Redo:
	switch(soundState) {
	case CANT_INITIALIZE: 
		return(0);

	case NOT_INITIALIZED:
		setRedraw(); 
		return(0); // we can do it only during render cycle 
		break;	

	case INITIALIZED:
			if (audioClip) {
				if (!emitter->Ok()) { // reload ??
					Term();
					goto Redo;

				}

				ignoreChildNotify ++;

				if (audioClip->OnTimeChange(t)) { // we should play it

					// get duration
					float beginTime=0;
					float endTime=0;

					beginTime = fmod(t - audioClip->startTime,audioClip->duration /* /audioClip->pitch */ );

					if (audioClip->stopTime > audioClip->startTime) {
						endTime = audioClip->stopTime - audioClip->startTime;
					}						
					emitter->SetMarkPosition(0.0,endTime);
					soundState = PLAYING;

					//UpdateEmitter(state, audioClip);
					emitter->Play(audioClip->loop,beginTime); // 15.11
//					emitter->Play(audioClip->loop,0.0);

					TRACE("OnTimeChange : Starting sound (%f %f) :%s  \n",beginTime,endTime, (const char *) audioClip->GetLocalFile());

				} else {
					emitter->Stop();
					soundState = INITIALIZED;
				}
			}
			ignoreChildNotify--;
			break;


	case PLAYING :
			if (audioClip) {
				if (!audioClip->isActive)  { // unexpected shut off 
					emitter->Stop();
					soundState = INITIALIZED;
					goto Redo;
				}
				if (!emitter->Ok()) { // reload ??
					Term();
					goto Redo;

				}

				ignoreChildNotify ++;

				if (audioClip->OnTimeChange(t)) {
					
					// query state of sound 
					BOOL playing; float secondsPlayed; float totalSeconds; int numLoops; float level;

					if (emitter->QueryInfo(playing,secondsPlayed,totalSeconds, numLoops,level)) {
						//audioClip->duration.set(totalSeconds);
						if (!playing) { 
							soundState = INITIALIZED;
							audioClip->setActive(playing!=0);
							TRACE("Sound terminated, no long playing  t = %g \n",t);
						}
					}
		
					//UpdateEmitter(state, audioClip);
					// soundState = PLAYING;
					// emitter->Play(audioClip->loop);

				} else {
					emitter->Stop();
					TRACE("Stopping sound :%s \n",(const char *) audioClip->GetLocalFile());

					soundState = INITIALIZED;
				}
				ignoreChildNotify --;
			}

			break;

	}
	return (soundState == PLAYING);

}

// notify from Audio Clip that clip is loaded 
int GvSound::OnAudioClipLoaded(GvAudioClip *child)
{
	switch(soundState) {
	case PLAYING :
			emitter->Stop();

	case CANT_INITIALIZE: 
	case NOT_INITIALIZED:
	case INITIALIZED:
	default :
			Term();
			setRedraw(); 
			break;

			// soundState will be NOT_INITIALIZED;
	}
	
	return(1);

}
	
// pitch changed
int GvSound::OnAudioClipPitchChanged(GvAudioClip *child)
{
	if (emitter) 
		emitter->SetPitch(child->pitch);
	
	return(1);

}

	
// audio clip unloaded
int GvSound::OnAudioClipUnload(GvAudioClip *child)
{
	switch(soundState) {
	case PLAYING :
			emitter->Stop();

	case CANT_INITIALIZE: 
	case NOT_INITIALIZED:
	case INITIALIZED:
	default :
			Term();
			setRedraw(); 
			break;

			// soundState will be NOT_INITIALIZED;
	}
	return(1);
}



int GvSound::Do(GTraversal &state)
{	int ret = 0;
  
	ret = source.Traverse(state);
	return(ret);
}


int 			
GvSound::Do(GglRenderTraversal &state)
{   int ret = 0;
	
	if (!isRegistered)
		state.getBrowser()->Register(this);

	GvNode *n=source.Eval(state);

	if (n == NULL) {
		soundState = CANT_INITIALIZE;
		return (0);
	}
	
	GvAudioClip *audioClip = NULL;

	GvBool audioClipLoaded = FALSE;

	if (RTISA(n,GvAudioClip)) { // we found a AudioClip Source 
		audioClip  = (GvAudioClip *) n;

		if (audioClip->Do(state) <=0) { // let do any processing 
			audioClipLoaded	= FALSE;	
		}	
		else audioClipLoaded	= TRUE;	
		// audio clip might have called callback OnAudioClipLoaded() 

	}

	double t= state.view->theTime;

Redo:
	switch(soundState) {
	case CANT_INITIALIZE: 
		return(0);

	case RELOAD :

	case NOT_INITIALIZED:
		{
			
			if (audioClip) { // we found a AudioCLip Source 

				if (!audioClipLoaded) {  // clip not loaded yet ???
					return(0); // remain in not_initialized state 
				}		
				ignoreChildNotify ++;
				// try to initialize, will update state 
				if (Initialize(state,audioClip)) {
					ignoreChildNotify --;
					return (1);
				}
				ignoreChildNotify --;
			}
			// to do: else if Mpeg 
			// else bad node 
			soundState = CANT_INITIALIZE;
			return (0);

		}

		break;	
	

	case INITIALIZED:
			if (audioClip) {
				if (!emitter->Ok()) { // reload ??
					Term();
					goto Redo;

				}
			
				ignoreChildNotify++;
				
				emitter->SetUsed(); // mark it beeing traversed

				if (audioClip->OnTimeChange(t)) { // we should play it

					// get duration
					float beginTime=0;
					float endTime=0;

					beginTime = fmod(t - audioClip->startTime,audioClip->duration /* /audioClip->pitch */ );

					if (audioClip->stopTime > audioClip->startTime) {
						endTime = audioClip->stopTime - audioClip->startTime;
					}						
					
					emitter->SetMarkPosition(0.0,endTime);
					soundState = PLAYING;

					UpdateEmitter(state, audioClip);
					emitter->Play(audioClip->loop,beginTime); // 15.11
//					emitter->Play(audioClip->loop,0.0);

					TRACE("Starting sound t = %g (%f %f) :%s  \n",t,beginTime,endTime, (const char *) audioClip->GetLocalFile());
				} else {
					emitter->Stop();
					soundState = INITIALIZED;
				}
				ignoreChildNotify--;

			}
			break;


	case PLAYING :
			if (audioClip) {

				emitter->SetUsed(); // mark it beeing traversed

				if (!emitter->Ok()) { // reload ??
					Term();
					goto Redo;
				}

				if (!audioClip->isActive)  { // unexpected shut off 
					emitter->Stop();
					soundState =INITIALIZED;
					goto Redo;
				}

				ignoreChildNotify++;
				if (audioClip->OnTimeChange(t)) {
					
					// query state of sound 
					BOOL playing; float secondsPlayed; float totalSeconds; int numLoops; float level;

					if (emitter->QueryInfo(playing,secondsPlayed,totalSeconds, numLoops,level)) {
						//audioClip->duration.set(totalSeconds);
		
						if (!playing) { 
							soundState = INITIALIZED;
							audioClip->setActive(playing!=0);
							TRACE("Sound terminated t = %g \n",t);
						}
					}
		
					UpdateEmitter(state, audioClip);
					// soundState = PLAYING;
					// emitter->Play(audioClip->loop);

				} else {
					emitter->Stop();
					TRACE("Stopping sound  t = %g :%s \n",t,(const char *) audioClip->GetLocalFile());

					soundState = INITIALIZED;
				}
				ignoreChildNotify--;
			}

			ret = 1;
			break;

	}
	return (ret);
}


// a child node changed
int GvSound::OnChildChanged(GvNode *child)
{ 

	// we changed audioClip 
	if (ignoreChildNotify >0) 
		return 0;

	GvNode *n=source.Eval();

	if (n == NULL) {
		return (0);
	}
	
	GvAudioClip *audioClip = NULL;
			
	if (RTISA(n,GvAudioClip)) { // we found a AudioClip Source 
		audioClip  = (GvAudioClip *) n;
/*
		// sound turned off by setStopTimer ??? 
		if (!audioClip->isActive && soundState == PLAYING) {
				emitter->Stop();
				TRACE("ChildChanged :: Stopping sound :%s \n",(const char *) audioClip->GetLocalFile());
				soundState = INITIALIZED;
		}
*/
		// check if sound should be played ?, because startTime triggered
		ignoreChildNotify++;
			OnTimeChange(getTime());
		ignoreChildNotify--;

		//emitter->SetPitch(audioClip->pitch);


	}
	return GvNode::OnChildChanged(child);
}



