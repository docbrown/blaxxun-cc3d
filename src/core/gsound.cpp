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
/******************************************************************************
@doc

@module GSound.cpp - abstract sound device implementation|

Copyright (c) 1997- 1998	by  blaxxun interactive - Holger Grahn 
All rights reserved

Purpose:
Abstract root classes for a sound device

actual sound implementation devices are GRSXSound.cpp and GDSSound.cpp

Classes:
<c GSoundDevice>
<c GSoundEmitter>


Notes:


  Think about extension for streaming
	  SoundEmmitter::AddSound(newSoundData)
  
  Think about extension for sound capture
	  SoundDevice::Capture ...


Changes:

$Revision: 1.15 $
$Log: gsound.cpp,v $
Revision 1.15  1999/03/10 10:21:50  tom
*** empty log message ***

Revision 1.11  1998/07/14 14:01:38  kristof
removed wait cursor

Revision 1.10  1998/07/06 17:50:41  holger
empty message



Todo :

   remove RSX datatypes
   
******************************************************************************/



#include "gversion.h"
#include "gsound.h"


RTIMP_ABSTRACT(GSoundDevice,0,RTRoot);


DWORD GSoundDevice::maxUpdateIntervall=150;  // how often can SetViewpoint be called
int GSoundDevice::defaultMaxSounds=8; // default number of max sounds played

#ifdef G_RSX

#include "rsx.h"

int GSoundDevice::defaultCpuBudget= RSX_MIN_BUDGET; // RSX_LOW_BUDGET; // the default CPU-Budget

#else

int GSoundDevice::defaultCpuBudget= 0;

#endif


GSoundDevice::GSoundDevice()
{

	m_use3d = TRUE;

	m_needCommit = FALSE;
	m_soundListChanged = FALSE;

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

	m_quality = 0;
};

// check the commit state 
BOOL GSoundDevice::CheckCommit()
{
	m_needCommit = FALSE;
	return FALSE;
}


// release all sound device resources 
void GSoundDevice::Release()
{
}

GSoundDevice::~GSoundDevice()
{
	Release();
}



GSoundEmitter::GSoundEmitter()
{
	//m_next = NULL;
	m_refCnt = 0;
	m_used = 0;
	m_pitch = 1.0;

	m_enabled = TRUE;
	m_enabledTime= 0;
	m_muted = FALSE;
	m_priority = 1.0;
	m_intensity = 1.0;
	
	
	m_result=0; 


}

GSoundEmitter::~GSoundEmitter()
{
    TRACE("SDHM: >>># GSoundEmitter<%08x>::~GSoundEmitter() \n", this);
    Release();
    TRACE("SDHM: <<<# GSoundEmitter<%08x>::~GSoundEmitter() \n", this);
}

// relese the sound 
void GSoundEmitter::Release()
{
    TRACE("SDHM: >>># GSoundEmitter<%08x>::~GSoundEmitter() \n", this);
    TRACE("SDHM: GSoundEmitter (base class) doing nothing.\n");
    TRACE("SDHM: <<<# G!!!SoundEmitter<%08x>::~GSoundEmitter() \n", this);
}



///////////// Functions needed for Streaming Mode: ////////////////////////

// They should be pure virtual functions, but if a derived class should not
// support streaming mode, they should not need to be (dummy)implemented.
// Threfore they get theire dummy implementation here.


BOOL   GSoundEmitter::Initialize(GSoundDevice *device, DWORD Frequency, DWORD Channels, DWORD BitsPerSample, double BufferLength, BOOL spatialize)
                                             {   ASSERT(FALSE);    return 0;   }
DWORD  GSoundEmitter::WriteData(char *Data, DWORD Len)
                                             {   ASSERT(FALSE);    return 0;   }
DWORD  GSoundEmitter::ZeroWrite(DWORD Count) {   ASSERT(FALSE);    return 0;   }
DWORD  GSoundEmitter::getSaveCursor()        {   ASSERT(FALSE);    return 0;   }
double GSoundEmitter::getSaveCursorSec()     {   ASSERT(FALSE);    return 0;   }
DWORD  GSoundEmitter::getWriteCursor()       {   ASSERT(FALSE);    return 0;   }
double GSoundEmitter::getWriteCursorSec()    {   ASSERT(FALSE);    return 0;   }
DWORD  GSoundEmitter::getBufferLength()      {   ASSERT(FALSE);    return 0;   }
double GSoundEmitter::getBufferLengthSec()   {   ASSERT(FALSE);    return 0;   }
DWORD  GSoundEmitter::getBytesPerSec()       {   ASSERT(FALSE);    return 0;   }
void GSoundEmitter::DecreaseWriteCursor(int delta)        {   ASSERT(FALSE);   }


    














#if 0


// set get the CPU Budget 0..2
void GSoundDevice::setCPUBudget(int budget)
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


int  GSoundDevice::getCPUBudget() { return (int)m_CPUBudget; } 


// set the number of active sounds 
int GSoundDevice::SetNumActiveSounds(int num)
{
	m_activeSounds = num;
	return m_activeSounds;
}


// initialize RSX and create listener 
BOOL GSoundDevice::Initialize(HWND mainWnd)
{

	BOOL ret = FALSE;

	HRESULT rval;

//	CWaitCursor				cursor; // HG 


	TRACE("GSoundDevice::Initialize .\n");


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

			m_lpDL->SetPosition(&m_position);
			m_lpDL->SetOrientation(&m_direction, &m_up);

			ret = TRUE;
		}
	}
	return ret;
}

void GSoundDevice::Term()
{
	RemoveEmitters();

	Release();
}



// release all sound device resources 
void GSoundDevice::Release()
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
void GSoundDevice::SetViewpoint(const Point &position,const Vector &direction,const Vector &up,DWORD time)
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
		if (setPos) m_lpDL->SetPosition(&m_position);
		if (setDir) m_lpDL->SetOrientation(&m_direction, &m_up);
	}

}
/*
 * CompareModes
 * Compare two display modes during sorting.  Modes are sorted by depth and
 * then resolution.
 */
static int
G__cdecl CompareEmitters(const void* element1, const void* element2) {
    GSoundEmitter *e1, *e2;

    e1 = *((GSoundEmitter**)element1);
    e2 = *((GSoundEmitter**)element2);

	float l1 = e1->getValue();
	float l2 = e2->getValue();
	if (l1<l2) return 1;
	if (l1>l2) return -1;
	return 0;

}


// enabled the n highest priority sounds only 
int GSoundDevice::PrioritizeSounds()
{
	/*
	// Examine each emitter and update its audible level
	*/
	GSoundEmitter *e =  m_emitters;
	Array<GSoundEmitter*> active;
	int cnt=0;

	while(e){
		if (e->IsEnabled() && e->m_lpCE) {
			e->m_lpCE->QueryMediaState(&e->qmi);
			active.Append(e);
			cnt++;
		}
		e = e->m_next;
	} /* while */

	m_numEnabledSounds = cnt;
	
	/*
		//  sort the list by audible level
	*/
	if (active.Length() >m_activeSounds)
		qsort((void *)&active[0], (size_t)active.Length(), sizeof(GSoundEmitter*),CompareEmitters);

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

GSoundEmitter* GSoundDevice::newSoundEmitter(int fileFmt)
{
	return new GSoundEmitter();
}

// play a single sound 
BOOL GSoundDevice::PlaySound(const char *fileName)
{
	if (m_playEmitter) {
		m_playEmitter->Release();
		m_playEmitter->unref();
		m_playEmitter = NULL;
    }

	if ( (fileName == NULL) || (fileName[0] == 0)) return FALSE;

	m_playEmitter =  new GSoundEmitter();
	m_playEmitter->ref();

	// create a sound 
	if (m_playEmitter->Initialize(this,fileName,FALSE)) {
		m_playEmitter->Play(FALSE,0);
		return TRUE;
	}
	else return FALSE;
}

// add new emitter 
void GSoundDevice::Add(GSoundEmitter *emitter)
{
	emitter->ref();
	emitter->m_next = m_emitters;
	m_emitters = emitter;
}

// to do:
// remove emitter 
BOOL GSoundDevice::Remove(GSoundEmitter *emitter)
{
	GSoundEmitter * e =  m_emitters;
	while (e) {
		if (emitter == e)
			emitter->Release();
		e= e->m_next;
	}

	return (FALSE);

}

// release all emitters 
void GSoundDevice::ReleaseEmitters()
{
	GSoundEmitter *emitter =  m_emitters;
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
void GSoundDevice::RemoveEmitters()
{
	GSoundEmitter *emitter =  m_emitters;
	while (emitter) {
		GSoundEmitter *next =  emitter->m_next;
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


	// release all emitters 
//	void DisabledUnusedEmitters();

	// set 
//	void ResetEmittersUseFlag();



#if 0

// add a sound 
BOOL GSoundDevice::AddSound(int id,const char *fileName)
{

	RSXCACHEDEMITTERDESC	rsxCE; 	
	RSXEMITTERMODEL			rsxModel;
    RSXVECTOR3D				p;	    

    p.x = 0.0f;
    p.y = 0.0f;
    p.z = 0.0f;



	strcpy(rsxCE.szFilename,fileName);

	rsxModel.fIntensity = 0.5f;		
    rsxModel.fMinBack = 2.0f;
    rsxModel.fMinFront = 2.0f;
    rsxModel.fMaxBack = 150.0f;
    rsxModel.fMaxFront = 500.0f;

	rsxCE.dwFlags |= RSXEMITTERDESC_NODOPPLER;

	if( SUCCEEDED(m_lpRSX->CreateCachedEmitter(
		&rsxCE, &m_lpCE[id], NULL)) && (m_lpCE) ) {

        m_lpCE[id]->SetPosition(&p);

        p.x = 0.0f;
        p.y = 0.0f;
        p.z = 1.0f;

        m_lpCE[id]->SetOrientation(&p);
		
		m_lpCE[id]->SetModel(&rsxModel);
		return(TRUE);

    } else {
		m_lpCE[id] = NULL;
		return(FALSE);
	}/* if */
}

#endif




GSoundEmitter::GSoundEmitter()
{
	m_lpCE=NULL;
	m_next = NULL;
	m_refCnt = 0;
	m_used = 0;
	m_pitch = 1.0;

	enabled = TRUE;
	muted = FALSE;
	priority = 1.0;

	ZeroMemory(&qmi, sizeof(RSXQUERYMEDIAINFO));
    qmi.cbSize = sizeof(RSXQUERYMEDIAINFO);
}


// create a sound attached to a certain device 
BOOL GSoundEmitter::Initialize(GSoundDevice *device,const char *fileName,BOOL spatialize)
{

	return(FALSE);
}


// query state of sound 
BOOL GSoundEmitter::QueryInfo(BOOL &playing,float &secondsPlayed,float &totalSeconds, int &numLoops,float &level)
{
	
    
	playing = FALSE;

	//TRACE("GSoundEmitter:Query %d %f %f %d %f \n", 	qmi.dwControl, secondsPlayed,	 totalSeconds, numLoops, level);
	return (TRUE);
}

// Sets/updates the emitter's model parameters. 
void GSoundEmitter::SetModel(
	float			fMinBack,
	float			fMinFront,
	float			fMaxBack,
	float			fMaxFront,
	float			fIntensity
	)
{

	if (fIntensity< 0.0) fIntensity = 0.0;
	//else if (fIntensity > 1.0) fIntensity = 1.0;


}



// enable / disable sound 
void GSoundEmitter::Enable(BOOL flag)
{
	int state;

	if (flag){
	    state = RSX_RESUME;
    } else  {
        state = RSX_PAUSE;
	} /* if */


	if(m_lpCE){
		m_lpCE->ControlMedia(state,0,0.0f);
		enabled = flag; 
		m_lpCE->SetMuteState(!flag);
	} /* if */
}

// play sound 
void GSoundEmitter::Play(BOOL loop,float initialStartTime)
{
	   enabled = TRUE; 
}

// stop playing sound 
void GSoundEmitter::Stop()
{
	   enabled = FALSE;
}
 



// GetMarkPosition retrieves the begin and end playback mark. 
// begin = 0 from begin end = 0 to end of file 
void GSoundEmitter::GetMarkPosition(float &beginTime, float &endTime)
{
	{
		beginTime = 0;
		endTime = 0;
	}
}

// Sets the begin and end playback mark. 
void GSoundEmitter::SetMarkPosition(float beginTime, float endTime)
{
}

// get the pitch factor 
void GSoundEmitter::GetPitch(float &pitchFactor)
{
}

void GSoundEmitter::SetPitch(float pitchFactor)
{
   m_pitch = pitchFactor;
}


// set position of sound 
void GSoundEmitter::SetPosition(const Point &position)
{
	if ((m_position.x != position.x) ||
	    (m_position.y != position.y) ||
	    (m_position.z != position.z))
	{
		m_position.x = position.x;
		m_position.y = position.y;
		m_position.z = position.z;

	}
}

//Specifies a 3D vector that points in the same direction as the emitter's orientation. 
void GSoundEmitter::SetOrientation (const Point &direction)
{
	if ((m_direction.x != direction.x) ||
		(m_direction.y != direction.y) ||
		(m_direction.z != direction.z))
	{
		m_direction.x = direction.x;
		m_direction.y = direction.y;
		m_direction.z = direction.z;


	}

}

#endif

