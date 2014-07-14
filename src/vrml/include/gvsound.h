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
#ifndef  _GV_Sound_
#define  _GV_Sound_

#include <gvsfstring.h>
#include <gvmfstring.h>
#include <gvsffloat.h>
#include <gvsftime.h>
#include <gvsfvec3f.h>
#include <gvmffloat.h>
#include <gvsfbool.h>
#include <gvsfnode.h>

#include <gvsubnode.h>

#include "www.h"

class GTraversal;

class GView;


class GSoundEmitter;


/*

AudioClip {
  exposedField   SFString description      ""
  exposedField   SFBool   loop             FALSE
  exposedField   SFFloat  pitch            1.0
  exposedField   SFTime   startTime        0
  exposedField   SFTime   stopTime         0
  exposedField   MFString url              []
  eventOut       SFTime   duration_changed
  eventOut       SFBool   isActive
}
*/

class GvAudioClip : public GvNode {

    GV_NODE_HEADER(GvAudioClip);

  public:
	// special EventIns 
	void set_startTime(GvSFTime *t);
	void set_stopTime(GvSFTime *t);

    // Fields

    GvSFString description;      // ""
    GvSFBool   loop;             // FALSE
    GvSFFloat  pitch;            // 1.0
    GvSFTime   startTime;        // 0
    GvSFTime   stopTime;         // 0
    GvMFString url;              // []

    // Event Out
    GvSFTime   duration; // _changed
    GvSFBool   isActive; 

    GvBool		isRegistered; //!< TRUE if node registered in Scene information


	// a field has been changed check, for redraw
	int OnFieldChanged(GvField *field);

	// notify all registered Sound node  owners, that this child has been loaded
	int NotifyOnAudioClipLoaded();
	int NotifyOnAudioClipUnload();
	int NotifyOnAudioClipPitchChanged();



	// set active state 
    void setActive(GvBool state) { if (isActive != state) isActive.set(state); }

	// time has been changed, check for active state
	int OnTimeChange(double t); 

	// next time we should check this sensor 
    double nextScheduleTime;
	double last_setStartTime; // data of last set_startTime
	double last_setStartTimeTime;

	// do we need the sound at this time ? 
	GvBool SoundNeededNow(double t); 


	GvString		urlToPlay; 	
	GFileRef        loader;			// url loader

	//! get the url to play for streaming mode
	const char *GetUrlToPlay(GTraversal &state);


	//! get absolute home url
	const char *GetHomeUrl() const;

	//! get the local file name 
	const char* GetLocalFile() const; 

	//! get the local file format 
	int GetLocalFileFmt() const;

	//! there was a problem with the local file 
	int OnLocalFileError(HRESULT result);


	// try to load url
	int Load(GTraversal &state);

	int Do(GLoadInlinesTraversal &state);
	int Do(GglRenderTraversal &state);


};


/*
Sound {
  exposedField SFVec3f  direction     0 0 1
  exposedField SFFloat  intensity     1
  exposedField SFVec3f  location      0 0 0
  exposedField SFFloat  maxBack       10
  exposedField SFFloat  maxFront      10
  exposedField SFFloat  minBack       1
  exposedField SFFloat  minFront      1
  exposedField SFFloat  priority      0
  exposedField SFNode   source        NULL
  field        SFBool   spatialize    TRUE
}
*/



class GvSound : public GvNode {

    GV_NODE_HEADER(GvSound);

  public:


    // Fields

    GvSFVec3f  direction;       // 0 0 1
    GvSFFloat  intensity;       // 1
    GvSFVec3f  location;        // 0 0 0
    GvSFFloat  maxBack;         // 10
    GvSFFloat  maxFront;        // 10
    GvSFFloat  minBack;         // 1
    GvSFFloat  minFront;         // 1
    GvSFFloat  priority;        // 0
    GvSFNode   source;          // NULL
    GvSFBool   spatialize;      // TRUE


	GSoundEmitter	*emitter;		// the sound resource 

	enum SoundState {
		NOT_INITIALIZED,	// not yet intialized
		CANT_INITIALIZE,	// no sound device, bad data
		INITIALIZED,		// sound initialized but not playing
		PLAYING,			// initialized & playning 
		RELOAD				// reload audioclip 

	};

	SoundState soundState;
	int ignoreChildNotify;  //   if >0 ignore childNotifiaction in OnChildChanged

    GvBool		isRegistered; //!< TRUE if node registered in Scene information

	//! need to update sound model params
	BOOL doUpdateModel;

	// return bbox
	GvBool getBBoxEx(BBox &bbox);

	
	// a field has been changed check, for redraw
	int OnFieldChanged(GvField *field);

	// a child node changed
	int OnChildChanged(GvNode *child);

	// notify from Audio Clip that clip is loaded 
	int OnAudioClipLoaded(GvAudioClip *child);
	
	// pitch changed
	int OnAudioClipPitchChanged(GvAudioClip *child);
	
	//! audio clip unloaded
	int OnAudioClipUnload(GvAudioClip *child);

	//! initialize sound from audioClip
	int Initialize(GGeometryTraversal &state,GvAudioClip *audioClip);

	//! update emitters position & location values 
	int UpdateEmitter(GGeometryTraversal &state,GvAudioClip *audioClip);

	
	//! release the sound 
	void Term();

	//! check play state for time t
	int OnTimeChange(double t); 


	int Do(GTraversal &state);
	int Do(GglRenderTraversal &state);


};


#endif /* _GV_Sound_ */

