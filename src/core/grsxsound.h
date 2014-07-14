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
#ifndef _GRSXSound_h
#define _GRSXSound_h

/******************************************************************************
@doc

@module GRSXSound.h - Intel RSX 3D sound device|

Copyright (c) 1997- 1998	by  blaxxun interactive - Holger Grahn 
All rights reserved

Purpose:

Classes:
<c GRSXSoundDevice>
<c GRSXSoundEmitter>


Notes:

  need Intel RSX SDK to compile www.intel.com/ial/rsx
  we need ole32.lib for CoCreateInstance

Changes:

$Revision: 1.8 $
$Log: grsxsound.h,v $
Revision 1.8  1999/07/06 16:54:52  tom
*** empty log message ***

Revision 1.7  1999/03/10 10:22:14  tom
*** empty log message ***



Todo :
   
******************************************************************************/




#include "gversion.h"



#include "gsound.h"


#ifdef G_RSX

#include "rsx.h"

class GRSXSoundDevice;
class GRSXSoundEmitter;



//
// GRSXSoundDevice 
//



class GRSXSoundDevice : public GSoundDevice 
{

public :

	RT(GRSXSoundDevice);
    /*
    // RSX objects
    */
   	IRSX*					m_lpRSX;
	IRSXDirectListener*		m_lpDL;

	// position of listener 
//	RSXVECTOR3D				m_position,m_direction,m_up;


	// set the quality level, set before initialize 
	void SetQuality(int quality);

	enum RSX_CPU_Budget		m_CPUBudget;



	// set get the CPU Budget 0..2
	void setCPUBudget(int budget);
	int  getCPUBudget();

	// set the number of active sounds 
	int SetNumActiveSounds(int num);

	// constructor 
	GRSXSoundDevice();

	// destructor
	~GRSXSoundDevice();


	// is SoundDevice properly setup ?
	BOOL Ok() const  { return (m_lpDL != NULL); }



	// initialize RSX and create listener 
	BOOL Initialize(HWND mainWnd);

	// terminate RSX 
	void Term();

	// release all sound device resources 
	void Release();


	// set the lister position and orientation
	void SetViewpoint(const Point &position,const Vector &direction,const Vector &up,DWORD time);

	// play a single sound 
	virtual BOOL PlaySound(const char *fileName);

	// add a sound 
	BOOL AddSound(int id,const char *fileName);

	// create new sound emitter 
	virtual GSoundEmitter* newSoundEmitter(int fileFmt= 0);


	// emitters allocated 
	GRSXSoundEmitter* m_emitters;


	// emitter for PlaySound
	GRSXSoundEmitter* m_playEmitter;

	// add new emitter 
	void Add(GRSXSoundEmitter *emitter);

	// remove emitter 
	BOOL Remove(GRSXSoundEmitter *emitter);

	// remove (delete) all emitters 
	void RemoveEmitters();


	// release all emitters 
	void ReleaseEmitters();

	// release all emitters not in used (not rendered)
	void DisableUnusedEmitters();

	// set 
	void ResetEmittersUseFlag();

	// enable the n highest priority sounds only 
	int PrioritizeSounds();

};

//
// GRSXSoundEmitter 
//

class GRSXSoundEmitter : public GSoundEmitter {
public:
	IRSXCachedEmitter*		m_lpCE;

	
	// position and direction of sound emitter 
	//RSXVECTOR3D				m_position,m_direction;
	//float					m_pitch;

	GRSXSoundEmitter * m_next; // linked list 

	// enable / disable via mute 
	void SetMuteState(BOOL flag);


	RSXQUERYMEDIAINFO qmi;

	// get sort value 
	float getValue() { return  qmi.fAudibleLevel+m_priority*0.1f; }


	GRSXSoundEmitter();
	~GRSXSoundEmitter();

	// is soundEmitter properly setup ?
	BOOL Ok() { return (m_lpCE != NULL); }

	// enable / disable sound 
	void Enable(BOOL flag);

	// Sets/updates the emitter's model parameters. 
	void SetModel(
		float			fMinBack,
		float			fMinFront,
		float			fMaxBack,
		float			fMaxFront,
		float			fIntensity
		);


	// create a sound 
	BOOL Initialize(GRSXSoundDevice *device, const char *fileName,BOOL spatialize=FALSE);

	// cast to specific specialized version 
	BOOL Initialize(GSoundDevice *device, const char *fileName,BOOL spatialize=FALSE) {
		return Initialize((GRSXSoundDevice *)device,fileName,spatialize);
	}

	// query state of sound 
	BOOL QueryInfo(BOOL &playing,float &secondsPlayed,float &totalSeconds, int &numLoops,float &level);


	// play sound 
	void Play(BOOL loop=TRUE, float initialStartTime=0.0f);

	// stop playing sound 
	void Stop();

	// GetMarkPosition retrieves the begin and end playback mark. 
	// begin = 0 from begin end = 0 to end of file 
	void GetMarkPosition(float &beginTime, float &endTime);
	// Sets the begin and end playback mark. 
	void SetMarkPosition(float beginTime=0.0f, float endTime=0.0f);

	// get the pitch factor 
	void GetPitch(float &pitchFactor);
	void SetPitch(float pitchFactor=1.0f);



	// set position of sound 
	void SetPosition(const Point &position);

	//Specifies a 3D vector that points in the same direction as the emitter's orientation. 
	void SetOrientation (const Point &direction);

	// release the sound 
	void Release();

};



#endif




#endif
