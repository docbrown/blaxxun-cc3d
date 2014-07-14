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

@module GRSXSound.cpp - Intel RSX 3D sound device implementation |

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

$Revision: 1.14 $
$Log: grsxsound.cpp,v $
Revision 1.14  1999/07/06 16:54:50  tom
*** empty log message ***

Revision 1.13  1999/03/10 10:21:45  tom
*** empty log message ***

Revision 1.7  1998/07/14 14:01:34  kristof
removed wait cursor

Revision 1.6  1998/07/06 17:50:34  holger
empty message



Todo :
   
******************************************************************************/

#include "stdafx.h"



#include "gversion.h"
#include "grsxsound.h"




#ifdef G_RSX


RTIMP(GRSXSoundDevice,0,GSoundDevice);

GRSXSoundDevice::GRSXSoundDevice()
{
	m_lpRSX = NULL;
	m_lpDL = NULL;

	m_emitters = NULL;
	m_playEmitter = NULL;

	m_time = 0;
	m_position.x = 0;
	m_position.y = 0;
	m_position.z = 2;

	m_direction.x = 0;
	m_direction.y = 0;
	m_direction.z = -1;

	m_up.x = 0;
	m_up.y = 1;
	m_up.z = 0;

	m_activeSounds = defaultMaxSounds;


	m_speedOfSound=0; // for doppler effect 

	m_quality = GS_QUALITY_MID;


	//Specifies the processing budget for audio localization quality. 
	//The higher the budget, the better the audio localization quality. 
	//Valid choices are RSX_NO_LOCALIZE, RSX_LOW_LOCALIZE, and RSX_MODERATE_LOCALIZE. The default is RSX_LOW_LOCALIZE. 
	m_CPUBudget = RSX_MIN_BUDGET; // RSX_LOW_LOCALIZE;
	setCPUBudget(defaultCpuBudget);

};


GRSXSoundDevice::~GRSXSoundDevice()
{
	RemoveEmitters();
	Release();
}



// set get the CPU Budget 0..2
void GRSXSoundDevice::setCPUBudget(int budget)
{
	if (budget >=RSX_MIN_BUDGET && budget <= RSX_MODERATE_BUDGET)
		m_CPUBudget = (enum RSX_CPU_Budget)budget;

	if( m_lpRSX){
		/*
		// Default Environment
		*/
		RSXENVIRONMENT rsxEnv;

		rsxEnv.cbSize = sizeof(RSXENVIRONMENT);
		rsxEnv.dwFlags = RSXENVIRONMENT_SPEEDOFSOUND | RSXENVIRONMENT_CPUBUDGET;
		rsxEnv.fSpeedOfSound = m_speedOfSound;
		rsxEnv.CPUBudget = m_CPUBudget;
		m_lpRSX->SetEnvironment(&rsxEnv);
	}

}


int  GRSXSoundDevice::getCPUBudget() { return (int)m_CPUBudget; } 

// quality if sound we want 
void GRSXSoundDevice::SetQuality(int quality)
{
	m_quality = quality;
	if (quality == GS_QUALITY_MIN)
		setCPUBudget(RSX_MIN_BUDGET);
	else if (quality == GS_QUALITY_MID)
		setCPUBudget(RSX_LOW_BUDGET);
	else if (quality == GS_QUALITY_MAX)
		setCPUBudget(RSX_MODERATE_BUDGET);

}



// set the number of active sounds 
int GRSXSoundDevice::SetNumActiveSounds(int num)
{
	m_activeSounds = num;
	return m_activeSounds;
}


// initialize RSX and create listener 
BOOL GRSXSoundDevice::Initialize(HWND mainWnd)
{

	BOOL ret = FALSE;

	HRESULT rval;

//	CWaitCursor				cursor; // HG 


	TRACE("GRSXSoundDevice::Initialize .\n");

	ASSERT((sizeof(RSXVECTOR3D) == sizeof(Point)));

    m_time = 0;

	if (m_lpRSX == NULL) 
		rval = CoCreateInstance(
			CLSID_RSX20,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IRSX20,
			(void ** ) &m_lpRSX);
  
	/*
	// Set the Audio Environment
	*/
	if( m_lpRSX){
        
		/*
		// Default Environment
		*/
		RSXENVIRONMENT rsxEnv;

		rsxEnv.cbSize = sizeof(RSXENVIRONMENT);
		rsxEnv.dwFlags = /* RSXENVIRONMENT_SPEEDOFSOUND | */ RSXENVIRONMENT_CPUBUDGET;
		
		rsxEnv.fSpeedOfSound = m_speedOfSound;
		rsxEnv.CPUBudget = m_CPUBudget;

		m_lpRSX->SetEnvironment(&rsxEnv);

		/*
		// Create a listener and save the IRSXDirectListener interface
		*/
   		RSXDIRECTLISTENERDESC rsxDL;            // listener description

		rsxDL.cbSize = sizeof(RSXDIRECTLISTENERDESC);
		rsxDL.hMainWnd = mainWnd; // AfxGetMainWnd()->GetSafeHwnd();
		rsxDL.dwUser = 0;
		rsxDL.lpwf = NULL;

		if( SUCCEEDED(m_lpRSX->CreateDirectListener(&rsxDL, &m_lpDL, NULL)) ) 
		{

			m_lpDL->SetPosition((RSXVECTOR3D *)&m_position.x);
			m_lpDL->SetOrientation((RSXVECTOR3D *)&m_direction.x, (RSXVECTOR3D *)&m_up.x);

			ret = TRUE;
		}
	}
	return ret;
}

void GRSXSoundDevice::Term()
{
	RemoveEmitters();

	Release();
}



// release all sound device resources 
void GRSXSoundDevice::Release()
{

    /*
    // Release Emitters
    */

	ReleaseEmitters();
    
    /*
	// Release the listener
	*/
	if(m_lpDL){
		m_lpDL->Release();
        m_lpDL = NULL;
	} /* if */

	/*
	// Release RSX
	*/
	if(m_lpRSX){
		m_lpRSX->Release();
        m_lpRSX = NULL;
	} /* if */
}




// set the lister position and orientation
void GRSXSoundDevice::SetViewpoint(const Point &position,const Vector &direction,const Vector &up,DWORD time)
{
	BOOL setPos,setDir;
	
	if ( (time-m_time)< maxUpdateIntervall) 
		return; // don't update to often 
	
	m_time = time;

	setPos = (m_position.x != position.x) ||
			 (m_position.y != position.y) ||
		     (m_position.z != position.z);

	m_position.x = position.x;
	m_position.y = position.y;
	m_position.z = position.z;

	setDir = (m_direction.x != direction.x) ||
			 (m_direction.y != direction.y) ||
			 (m_direction.z != direction.z) ||
			 (m_up.x != up.x) ||
			 (m_up.y != up.y) ||
			 (m_up.z != up.z);

	if (setDir) { 
		m_direction.x = direction.x;
		m_direction.y = direction.y;
		m_direction.z = direction.z;

		m_up.x = up.x;
		m_up.y = up.y;
		m_up.z = up.z;
    }

	if (m_lpDL) {
		if (setPos) m_lpDL->SetPosition((RSXVECTOR3D *)&m_position.x);
		if (setDir) m_lpDL->SetOrientation((RSXVECTOR3D *)&m_direction.x,(RSXVECTOR3D *) &m_up.x);
	}

}
/*
 * CompareModes
 */

static int
G__cdecl CompareEmitters(const void* element1, const void* element2) {
    GRSXSoundEmitter *e1, *e2;

    e1 = *((GRSXSoundEmitter**)element1);
    e2 = *((GRSXSoundEmitter**)element2);

	float l1 = e1->m_priority;
	float l2 = e2->m_priority;

	if (l1<l2) return 1;
	if (l1>l2) return -1;

	if (e1->m_priority > 0.5) {
		l1 = e1->m_enabledTime;
		l2 = e2->m_enabledTime;

		if (l1<l2) return 1;
		if (l1>l2) return -1;
	}


	l1 = e1->getValue();
	l2 = e2->getValue();
	
	if (l1<l2) return 1;
	if (l1>l2) return -1;

	return 0;

}


// enabled the n highest priority sounds only 
int GRSXSoundDevice::PrioritizeSounds()
{
	/*
	// Examine each emitter and update its audible level
	*/
	GRSXSoundEmitter *e =  m_emitters;
	Array<GRSXSoundEmitter*> active;
	int cnt=0;

	while(e){
		if (e->IsEnabled() && e->m_lpCE) {
			if (e->m_intensity>0.0001f) { 
				e->m_lpCE->QueryMediaState(&e->qmi);
				active.Append(e);
				cnt++;
			} else e->m_lpCE->SetMuteState(0);
	
		}
		e = e->m_next;
	} /* while */

	m_numEnabledSounds = cnt;
	
	/*
		//  sort the list by audible level
	*/
	if (active.Length() >m_activeSounds)
		qsort((void *)&active[0], (size_t)active.Length(), sizeof(GRSXSoundEmitter*),CompareEmitters);

	/*
	// Mute all but the  most audible
	*/

	for (int i=0; i<active.Length(); i++)
	{
		e = active[i];
		if (i<m_activeSounds)
			e->m_lpCE->SetMuteState(0);
		else e->m_lpCE->SetMuteState(1);
	} 
	return (active.Length());

}
// create new sound emitter 

GSoundEmitter* GRSXSoundDevice::newSoundEmitter(int fileFmt)
{
	return new GRSXSoundEmitter();
}

// play a single sound 
BOOL GRSXSoundDevice::PlaySound(const char *fileName)
{
	if (m_playEmitter) {
		m_playEmitter->Release();
		m_playEmitter->unref();
		m_playEmitter = NULL;
    }

	if ( (fileName == NULL) || (fileName[0] == 0)) return FALSE;

	m_playEmitter =  new GRSXSoundEmitter();
	m_playEmitter->ref();

	// create a sound 
	if (m_playEmitter->Initialize(this,fileName,FALSE)) {
		m_playEmitter->Play(FALSE,0);
		return TRUE;
	}
	else return FALSE;
}

// add new emitter 
void GRSXSoundDevice::Add(GRSXSoundEmitter *emitter)
{
	emitter->ref();
	emitter->m_next = m_emitters;
	m_emitters = emitter;
}

// to do:
// remove emitter 
BOOL GRSXSoundDevice::Remove(GRSXSoundEmitter *emitter)
{
	GRSXSoundEmitter * e =  m_emitters;
	while (e) {
		if (emitter == e)
			emitter->Release();
		e= e->m_next;
	}

	return (FALSE);

}

// release all emitters 
void GRSXSoundDevice::ReleaseEmitters()
{
	GRSXSoundEmitter *emitter =  m_emitters;
	while (emitter) {
		emitter->Release();
		emitter= emitter->m_next;
	}
	m_numEnabledSounds=0;
	if (m_playEmitter) {
		m_playEmitter->Release();
    }
}

// remove (delete) all emitters 
void GRSXSoundDevice::RemoveEmitters()
{
	GRSXSoundEmitter *emitter =  m_emitters;
	while (emitter) {
		GRSXSoundEmitter *next =  emitter->m_next;
		emitter->Release();
		emitter->m_next = NULL;
		emitter->unref();	// this may delete this guy 
		emitter= next;
	}
	m_emitters = NULL;

	if (m_playEmitter) {
		m_playEmitter->Release();
		m_playEmitter->unref();
		m_playEmitter = NULL;
    }
}


// release all emitters not in used (not rendered)
void GRSXSoundDevice::DisableUnusedEmitters()
{
	GRSXSoundEmitter *emitter =  m_emitters;
	while (emitter) {
		if (!emitter->IsUsed())
			emitter->Release();
		emitter= emitter->m_next;
	}
	if (m_playEmitter) {
		if (!m_playEmitter->IsUsed())
			m_playEmitter->Release();
    }
}

void GRSXSoundDevice::ResetEmittersUseFlag()
{
	GRSXSoundEmitter *emitter =  m_emitters;
	while (emitter) {
		emitter->SetUnused();
		emitter= emitter->m_next;
	}
	if (m_playEmitter) {
		// m_playEmitter->SetUnused();
    }
}





GRSXSoundEmitter::GRSXSoundEmitter()
{
	m_lpCE=NULL;
	m_next = NULL;
	m_refCnt = 0;
	m_used = 0;
	m_pitch = 1.0;

	m_enabled = TRUE;
	m_muted = FALSE;

	m_priority = 1.0;
	m_intensity = 1.0f;

	ZeroMemory(&qmi, sizeof(RSXQUERYMEDIAINFO));
    qmi.cbSize = sizeof(RSXQUERYMEDIAINFO);
}

GRSXSoundEmitter::~GRSXSoundEmitter()
{
	Release();
}

// relese the sound 
void GRSXSoundEmitter::Release()
{
	if(m_lpCE){ // release the cached emitter object
		
		if (m_enabled) Stop();

		m_lpCE->Release();
        m_lpCE = NULL;

	} /* if */
}


// create a sound attached to a certain device 
BOOL GRSXSoundEmitter::Initialize(GRSXSoundDevice *device,const char *fileName,BOOL spatialize)
{

	RSXCACHEDEMITTERDESC	rsxCE; 	
	RSXEMITTERMODEL			rsxModel;

//	CWaitCursor				cursor; // HG 

	// release sound if currently active 
	if (m_lpCE)
		Release();


	TRACE("GRSXSoundEmitter::Initialize %s \n",fileName);

	ZeroMemory(&rsxCE, sizeof(RSXCACHEDEMITTERDESC));
	rsxCE.cbSize = sizeof(RSXCACHEDEMITTERDESC);

	// for background sound 
	//rsxCE.dwFlags = RSXEMITTERDESC_NOSPATIALIZE | RSXEMITTERDESC_NOATTENUATE | RSXEMITTERDESC_NODOPPLER | RSXEMITTERDESC_NOREVERB;
	rsxCE.dwFlags = RSXEMITTERDESC_NODOPPLER | RSXEMITTERDESC_NOREVERB;
	rsxCE.dwUser = 0;

	if (!spatialize ||(device->m_quality <= GS_QUALITY_NO3D)) 
		 rsxCE.dwFlags |= RSXEMITTERDESC_NOSPATIALIZE;
	// seems to consume much memory on NT 15.11 : rsxCE.dwFlags |=  RSXEMITTERDESC_PREPROCESS |  RSXEMITTERDESC_INMEMORY; // new


	strcpy(rsxCE.szFilename,fileName);

	rsxModel.cbSize = sizeof(RSXEMITTERMODEL);
	rsxModel.fIntensity = 1.0f;		

    rsxModel.fMinBack = 0.5f;
    rsxModel.fMinFront = 0.5f;
    rsxModel.fMaxBack = 100.0f;
    rsxModel.fMaxFront = 500.0f;
	m_intensity = rsxModel.fIntensity;


	if( SUCCEEDED(device->m_lpRSX->CreateCachedEmitter(
		&rsxCE, &m_lpCE, NULL)) && (m_lpCE) ) {

		// turn it of at the beginning 
		m_enabled = FALSE;

		m_lpCE->SetMuteState(1);

		m_lpCE->SetModel(&rsxModel);
		
		ASSERT(m_next == NULL);

		device->Add(this);

		return(TRUE);

    } else {
		m_lpCE = NULL;
		return(FALSE);
	}/* if */
}


// query state of sound 
BOOL GRSXSoundEmitter::QueryInfo(BOOL &playing,float &secondsPlayed,float &totalSeconds, int &numLoops,float &level)
{
	
    
	playing = FALSE;

	if (!m_lpCE) return FALSE;

    if (!SUCCEEDED(m_lpCE->QueryMediaState(&qmi))) return FALSE;

    // What state is the emitter in??
	//qmi.dwControl

	playing = (qmi.dwControl == RSX_PLAY);
	
	secondsPlayed = qmi.fSecondsPlayed;
	totalSeconds = qmi.fTotalSeconds;

	numLoops = 	qmi.dwNumLoops;

	level = qmi.fAudibleLevel;

	//TRACE("GRSXSoundEmitter:Query %d %f %f %d %f \n", 	qmi.dwControl, secondsPlayed,	 totalSeconds, numLoops, level);
	return (TRUE);


}

// Sets/updates the emitter's model parameters. 
void GRSXSoundEmitter::SetModel(
	float			fMinBack,
	float			fMinFront,
	float			fMaxBack,
	float			fMaxFront,
	float			fIntensity
	)
{

	if (fIntensity< 0.0) fIntensity = 0.0;
	//else if (fIntensity > 1.0) fIntensity = 1.0;

	RSXEMITTERMODEL			rsxModel;

	rsxModel.cbSize = sizeof(RSXEMITTERMODEL);
	rsxModel.fIntensity = fIntensity;		
    rsxModel.fMinBack = fMinBack;
    rsxModel.fMinFront = fMinFront;
    rsxModel.fMaxBack = fMaxBack;
    rsxModel.fMaxFront = fMaxFront;
	m_intensity = rsxModel.fIntensity;


	if(m_lpCE) {
 		m_lpCE->SetModel(&rsxModel);
	}

}



// enable / disable sound 
void GRSXSoundEmitter::Enable(BOOL flag)
{
	int state;

	if (flag){
	    state = RSX_RESUME;
    } else  {
        state = RSX_PAUSE;
	} /* if */


	if(m_lpCE){
		m_lpCE->ControlMedia(state,0,0.0f);
		m_enabled = flag; 
		m_lpCE->SetMuteState(!flag);
	} /* if */
}

// enable / disable via mute 
void GRSXSoundEmitter::SetMuteState(BOOL flag)
{
	
	if (flag == m_muted) return;

    if (m_lpCE) m_lpCE->SetMuteState(flag);

	m_muted = flag; 

}



// play sound 
void GRSXSoundEmitter::Play(BOOL loop,float initialStartTime)
{
	if (m_lpCE) {
	   m_lpCE->ControlMedia(RSX_PLAY, (loop ? 0 : 1), initialStartTime);
	   m_enabled = TRUE; 
	   m_lpCE->SetMuteState(0);
	   m_enabledTime = timeGetTime(); 
	   m_muted = FALSE;

	}
}

// stop playing sound 
void GRSXSoundEmitter::Stop()
{
	if (m_lpCE)	{
	   m_lpCE->ControlMedia(RSX_STOP, 0.0,0.0f);
	   m_enabled = FALSE;
	}
}
 



// GetMarkPosition retrieves the begin and end playback mark. 
// begin = 0 from begin end = 0 to end of file 
void GRSXSoundEmitter::GetMarkPosition(float &beginTime, float &endTime)
{
	if (m_lpCE) {
	   m_lpCE->GetMarkPosition(&beginTime,&endTime);
	}
	else {
		beginTime = 0;
		endTime = 0;
	}
}

// Sets the begin and end playback mark. 
void GRSXSoundEmitter::SetMarkPosition(float beginTime, float endTime)
{
	if (m_lpCE)	{
	   m_lpCE->SetMarkPosition(beginTime,endTime);
	}
}

// get the pitch factor 
void GRSXSoundEmitter::GetPitch(float &pitchFactor)
{
	if (m_lpCE)	{
	   m_lpCE->GetPitch(&pitchFactor);
	}
	else {
		pitchFactor = 1.0;
	}
}

void GRSXSoundEmitter::SetPitch(float pitchFactor)
{
	if (pitchFactor <=0.25) pitchFactor = 0.25;
	else if (pitchFactor > 4.0) pitchFactor = 4.0;

	if (pitchFactor != m_pitch)
	if (m_lpCE)	{
	   m_lpCE->SetPitch(pitchFactor);
	   m_pitch = pitchFactor;
	}
}


// set position of sound 
void GRSXSoundEmitter::SetPosition(const Point &position)
{
	if ((m_position.x != position.x) ||
	    (m_position.y != position.y) ||
	    (m_position.z != position.z))
	{
		m_position.x = position.x;
		m_position.y = position.y;
		m_position.z = position.z;

		if (m_lpCE)	{
			m_lpCE->SetPosition((RSXVECTOR3D *)&m_position.x);
		}
	}
}

//Specifies a 3D vector that points in the same direction as the emitter's orientation. 
void GRSXSoundEmitter::SetOrientation (const Point &direction)
{
	if ((m_direction.x != direction.x) ||
		(m_direction.y != direction.y) ||
		(m_direction.z != direction.z))
	{
		m_direction.x = direction.x;
		m_direction.y = direction.y;
		m_direction.z = direction.z;


		if (m_lpCE) {
			m_lpCE->SetOrientation ((RSXVECTOR3D *)&m_direction.x);
		}
	}

}

#endif // G_RSX
