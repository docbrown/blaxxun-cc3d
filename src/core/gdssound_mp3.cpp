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

@module GDSSound.cpp - Direct Sound sound device implementation |

Copyright (c) 1998	by  blaxxun interactive - Holger Grahn 
All rights reserved

Purpose:

Classes:
<c GDSSoundDevice>
<c GDSSoundEmitter>


Notes:

  Direct 3D Sound available beginning from Direct X 5
  need Direct X 5 SDK to compile

Changes:

$Revision: 1.33 $
$Log: gdssound.cpp,v $
Revision 1.33  1999/07/06 16:54:22  tom
*** empty log message ***

Revision 1.13  1998/07/14 14:01:29  kristof
removed wait cursor

Revision 1.12  1998/07/06 17:50:19  holger
empty message

Revision 1.9  1998/03/31 17:29:20  holger
empty message



Todo :
   
******************************************************************************/


#include "stdafx.h"



#include "gversion.h"

#include "gdssound.h"


#ifdef G_DSOUND

#include "gmidi.h"
#include "www.h" // for MIDI_FMT

#include "dsutil3d.h"




#if 0

extern "C" {

#include "media/mpg123/mpglib/mpg123.h"
#include "media/mpg123/mpglib/mpglib.h"

}


// decode MP3
int DecodeMp3(const char *fileName,	WAVEFORMATEX &wfx,
			  char **bufferOut,size_t *bufferSize) 
{
	int ret;

	char buf[16384];
	
	char *buffer = NULL;

	size_t bufferLen=0;
	
	struct mpstr mp;

	int size;
	char out[8192];
	int len;


    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 2;
    wfx.nSamplesPerSec =  44100;  // 22050; could change format

	
	FILE *f;
	f = fopen(fileName,"rb");
	if (!f) return -1;


	ret=InitMP3(&mp);

	while(1) {
		len = fread(buf,1,sizeof(buf),f);
		if(len <= 0)
			break;

		ret = decodeMP3(&mp,buf,len,out,sizeof(out),&size);
		if (ret == MP3_OK) {
			// header ready ??

		}

		while(ret == MP3_OK) {
			if (size>=0) {
				buffer = (char *) REALLOC(buffer,bufferLen+size);
				memcpy(buffer+bufferLen,out,size);
				bufferLen +=size;
			}

			ret = decodeMP3(&mp,NULL,0,out,8192,&size);
		}
	}

	ExitMP3(&mp);
	fclose(f);

	*bufferOut = buffer;
	*bufferSize = bufferLen;

	return ret;
}


#endif



/*
Direct Sound 3D like Direct 3D use left handed coordinate system
VRML right handed
this results in stereo left/right exchange

*/
#define GDSZFLIP -1.0f


RTIMP(GDSSoundDevice,0,GSoundDevice);


GDSSoundDevice::GDSSoundDevice()
{
	m_lpDS = NULL;
	lpDSBuff=NULL;
	m_lpDL = NULL;


	m_emitters = NULL;
	m_playEmitter = NULL;

	m_has3d = FALSE;

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

	m_numEnabledSounds = 0;
	m_passUrl = FALSE;
	m_activeSounds = defaultMaxSounds;


	m_speedOfSound=0; // for doppler effect 

	m_quality = GS_QUALITY_MAX;

	maxUpdateIntervall = 0; // test


};



//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

GDSSoundDevice::~GDSSoundDevice()
{
	RemoveEmitters();

	Release();
}



//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

// quality if sound we want 
void GDSSoundDevice::SetQuality(int quality)
{
	m_quality = quality;
}




//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

// set the number of active sounds 
int GDSSoundDevice::SetNumActiveSounds(int num)
{
	m_activeSounds = num;
	return m_activeSounds;
}




//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

// initialize DS and create listener

// try to get a 3D listener 
 
BOOL GDSSoundDevice::Initialize(HWND mainWnd)
{

	HRESULT result;

	BOOL ret = FALSE;

//	CWaitCursor				cursor; // HG 


	TRACE("GDSSoundDevice::Initialize .\n");


    m_time = 0;
        // Description for our primary buffer creation
        DSBUFFERDESC dsbd;
        
		// Make the primary 44.1 KHz so that it sounds better
        WAVEFORMATEX wfx;
        wfx.wFormatTag = WAVE_FORMAT_PCM;
        wfx.nChannels = 2;
        wfx.nSamplesPerSec =  44100;  // 22050; could change format

		if (m_quality < GS_QUALITY_MAX) {
            wfx.nSamplesPerSec =  22050; 
		}  

        wfx.nAvgBytesPerSec = wfx.nSamplesPerSec*wfx.nChannels*2;

        wfx.nBlockAlign = wfx.nChannels*2;
        wfx.wBitsPerSample = 16;
        wfx.cbSize = 0;


       // Create the directsound object
       if (!DS3D_SUCCEED(result=DirectSoundCreate(NULL, &m_lpDS, NULL))) {
			TRACE("DirectSoundCreate init failed \n");
			return FALSE;
	   }	

       // Set cooperative level
	   result=m_lpDS->SetCooperativeLevel(mainWnd, DSSCL_PRIORITY);
	   //result=m_lpDS->SetCooperativeLevel(mainWnd, DSSCL_NORMAL); does not help sharing device ??
       if (!DS3D_SUCCEED(result)) {
			result=m_lpDS->SetCooperativeLevel(mainWnd, DSSCL_NORMAL);
	   }	


       if (DS3D_SUCCEED(result)) {

               // Create a primary buffer so we can query for a 3D Listener interface
               memset(&dsbd, 0, sizeof(DSBUFFERDESC));
               dsbd.dwSize= 4*sizeof(DWORD) + sizeof(LPWAVEFORMATEX);
//               dsbd.dwSize = sizeof(DSBUFFERDESC);
		 	   if (m_quality <= GS_QUALITY_NO3D) 
				  dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
			   else 
				  dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;

			   result = m_lpDS->CreateSoundBuffer(&dsbd, &lpDSBuff, NULL);
			   
//			   if (result != DS_OK) { // hg try without 3d bit 
               if(!DS3D_SUCCEED(result))   {
		               dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
	 			       result = m_lpDS->CreateSoundBuffer(&dsbd, &lpDSBuff, NULL);
			   }	
			   
			   if (DS3D_SUCCEED(result)) {
                      
                       result=lpDSBuff->SetFormat(&wfx);
					   if (result != DS_OK) { // 
							TRACE("DSOUND can't change format\n");
							//result=lpDSBuff->GetFormat(&wfx);		// get current 					
					   }	

                       // Get the 3D listener information (error currently ignored)
                       if (DS3D_SUCCEED(lpDSBuff->QueryInterface(IID_IDirectSound3DListener, (void**) &m_lpDL))) {
							m_has3d = TRUE;
							m_lpDL->SetPosition(m_position.x,m_position.y,GDSZFLIP*m_position.z,DS3D_DEFERRED);
							m_lpDL->SetOrientation(m_direction.x,m_direction.y,GDSZFLIP*m_direction.z,m_up.x,m_up.y,GDSZFLIP*m_up.z,DS3D_DEFERRED);

                            //m_lpDL->SetDopplerFactor(D3DVAL(100.0), DS3D_IMMEDIATE);
							// no doppler 
                            m_lpDL->SetDopplerFactor(DS3D_MINDOPPLERFACTOR, DS3D_DEFERRED);
							//DS3D_DEFAULTROLLOFFFACTOR 1.0
                            // m_lpDL->SetRolloffFactor(5.0, DS3D_DEFERRED);

                       }
                       else {
/*
                               // Failed to get listener info
                               lpDSBuff->Release();
                               m_lpDS->Release();
                               m_lpDS = NULL;
*/
							m_has3d = FALSE;
                       }
               }
               else {
                       // Failed to create a primary buffer
                       m_lpDS->Release();
                       m_lpDS = NULL;
               }
       }
       else {
               // Failed to set cooperative level
               m_lpDS->Release();
               m_lpDS = NULL;
       }
				
       // Warn that we could create the DirectSound object
       if (!m_lpDS) {
			TRACE("Direct Sound init failed \n");
			return FALSE;
        }

	   return TRUE;

}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

void GDSSoundDevice::Term()
{
	RemoveEmitters();

	Release();
}




//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

// release all sound device resources 
void GDSSoundDevice::Release()
{


	ReleaseEmitters();

    if (m_lpDL != NULL) {
        m_lpDL->Release();
		m_lpDL = NULL;
    }

	if (lpDSBuff) {
		lpDSBuff->Release();
		lpDSBuff = NULL;
	}

    if (m_lpDS != NULL) {
        m_lpDS->Release();
		m_lpDS = NULL;
    }
   
}





//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

// set the lister position and orientation
void GDSSoundDevice::SetViewpoint(const Point &position,const Vector &direction,const Vector &up,DWORD time)
{
	BOOL setPos,setDir;
	

	if (m_lpDL && (time-m_time)< maxUpdateIntervall) 
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
		if (setPos)	m_lpDL->SetPosition(m_position.x,m_position.y,GDSZFLIP*m_position.z,DS3D_DEFERRED);
		if (setDir)	m_lpDL->SetOrientation(m_direction.x,m_direction.y,GDSZFLIP*m_direction.z,m_up.x,m_up.y,GDSZFLIP*m_up.z,DS3D_DEFERRED);
		
		if (setPos || setDir)
			NeedCommit();
	}

}
/*
 * CompareModes
 */


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

static int
G__cdecl CompareEmitters(const void* element1, const void* element2) {
    GDSSoundEmitter *e1, *e2;

    e1 = *((GDSSoundEmitter**)element1);
    e2 = *((GDSSoundEmitter**)element2);

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



//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

// enabled the n highest priority sounds only 
int GDSSoundDevice::PrioritizeSounds()
{
	//m_pDS->CommitDeferredSetting();
	
	/*
	// Examine each emitter and update its audible level
	*/
	GDSSoundEmitter *e =  m_emitters;
	Array<GDSSoundEmitter*> active;
	int cnt=0;

	while (e){
		if (e->IsEnabled() && e->m_pDSB) {
			
			e->m_pDSB->GetStatus(&e->status);
			
			Point d(m_position.x-e->m_position.x,m_position.y-e->m_position.y,m_position.z-e->m_position.z);

			e->m_distance2 = d.Len2();

			if ((e->m_intensity>0.0001f) && (e->m_distance2  <= (e->m_maxDistance2+0.001f))) {
				// e->distance
				if (e->status & DSBSTATUS_BUFFERLOST) {
					TRACE("Buffer lost \n");
				}	
				if (!e->is3d()) 
					e->ComputeDistanceAttentuation();

				active.Append(e);
				cnt++;
			} 
			else 
			{   // sound away to far, mute it 
				e->SetMuteState(1); // turn off 
			}
		}
		e = e->m_next;
	} /* while */

	m_numEnabledSounds = cnt;
	
	/*
		//  sort the list by audible level
	*/
	if (active.Length() >m_activeSounds)
		qsort((void *)&active[0], (size_t)active.Length(), sizeof(GDSSoundEmitter*),CompareEmitters);

	/*
	// Mute all but the  most audible
	*/

	for (int i=0; i<active.Length(); i++)
	{
		e = active[i];
		if (i<m_activeSounds) {
			e->SetMuteState(0); // turn on
		} else { 
			e->SetMuteState(1); // turn off 
		}
	} 

	if (m_lpDL) m_lpDL->CommitDeferredSettings();
	m_needCommit = FALSE;
	
	return (active.Length());

}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

// check the commit state 
BOOL GDSSoundDevice::CheckCommit()
{
	if (m_soundListChanged || !m_has3d) { // if manual attenuation we need to prioritize sound's anyway
		PrioritizeSounds();
		m_soundListChanged = FALSE;
	}
	else 
	if (m_needCommit) 
		if (m_lpDL) m_lpDL->CommitDeferredSettings();

	m_needCommit = FALSE;
	return FALSE;
}


// create new sound emitter 

GSoundEmitter* GDSSoundDevice::newSoundEmitter(int fileFmt)
{
/*
	if (fileFmt == MIDI_FMT)
		return newMidiEmitter();
*/
	return new GDSSoundEmitter();

	//return new GDSStreamingSoundEmitter();

}

// play a single sound 
BOOL GDSSoundDevice::PlaySound(const char *fileName)
{
	if (m_playEmitter) {
		m_playEmitter->Release();
		m_playEmitter->unref();
		m_playEmitter = NULL;
    }

	if ( (fileName == NULL) || (fileName[0] == 0)) return FALSE;

	m_playEmitter =  new GDSSoundEmitter();
	m_playEmitter->ref();

	// create a sound 
	if (m_playEmitter->Initialize(this,fileName,FALSE)) {
		m_playEmitter->Play(FALSE,0); 
		return TRUE;
	}
	else { 
		TRACE("Can't play sound file %s \n",fileName);
		return FALSE;
	}
}

// add new emitter 
void GDSSoundDevice::Add(GDSSoundEmitter *emitter)
{
	emitter->ref();
	emitter->m_next = m_emitters;
	m_emitters = emitter;
}

// to do:
// remove emitter 
BOOL GDSSoundDevice::Remove(GDSSoundEmitter *emitter)
{
	GDSSoundEmitter * e =  m_emitters;
	while (e) {
		if (emitter == e)
			emitter->Release();
		e= e->m_next;
	}

	return (FALSE);

}

// release all emitters 
void GDSSoundDevice::ReleaseEmitters()
{
	GDSSoundEmitter *emitter =  m_emitters;
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
void GDSSoundDevice::RemoveEmitters()
{
	GDSSoundEmitter *emitter =  m_emitters;
	while (emitter) {
		GDSSoundEmitter *next =  emitter->m_next;
		emitter->Release();
		emitter->m_next = NULL;
		emitter->unref();	// this may delete this guy 
		emitter= next;
	}
	m_emitters = NULL;
	m_numEnabledSounds=0;

	if (m_playEmitter) {
		m_playEmitter->Release();
		m_playEmitter->unref();
		m_playEmitter = NULL;
    }
}


// release all emitters not in used (not rendered)
void GDSSoundDevice::DisableUnusedEmitters()
{
	GDSSoundEmitter *emitter =  m_emitters;
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

void GDSSoundDevice::ResetEmittersUseFlag()
{
	GDSSoundEmitter *emitter =  m_emitters;
	while (emitter) {
		emitter->SetUnused();
		emitter= emitter->m_next;
	}
	if (m_playEmitter) {
		// m_playEmitter->SetUnused();
    }
}





GDSSoundEmitter::GDSSoundEmitter()
{
	m_parent = NULL;
	m_isMidi = FALSE;

	m_pDSB=NULL;
	m_lpCE=NULL;
	m_next = NULL;
	m_refCnt = 0;
	m_used = 0;
	m_pitch = 1.0;

	m_enabled = TRUE;
	m_muted = FALSE;
	m_is3d = FALSE;
	m_volume = DSBVOLUME_MAX;
	m_currentVolume = DSBVOLUME_MAX;
	
	m_intensity = 1.0f;
	m_priority = 1.0f;
    m_distance2 = 0.0f; // distance to viewer 

	m_minDistance = 5;
	m_maxDistance = 50;
	m_distMult = 1.0f / (m_maxDistance-m_minDistance);
	
	m_maxDistance2 = m_maxDistance * m_maxDistance;


    mNotYetWritten= TRUE;

    mBytesPerSec=    0;
    mSecondsPerByte= 0;

    mpLockedData1=   NULL;
    mpLockedData2=   NULL;
    mLockedLen1=     0;
    mLockedLen2=     0;

    mWriteCursor=    0;


	status = 0;

}

GDSSoundEmitter::~GDSSoundEmitter()
{
	Release();
}

// relese the sound 
void GDSSoundEmitter::Release()
{
	if (m_enabled) {
		Stop();
	}

	if(m_lpCE){
		m_lpCE->Release();
        m_lpCE = NULL;
	} /* if */

	if(m_pDSB) {
		m_pDSB->Release();
        m_pDSB = NULL;
	} /* if */
}



// create a sound attached to a certain device, setting the Emitter in e_SimpleWav Mode.
BOOL GDSSoundEmitter::Initialize(GSoundDevice *device,const char *fileName,BOOL spatialize)
{

    ASSERT(RTISA(device, GDSSoundDevice));
    GDSSoundDevice* DSdevice= (GDSSoundDevice*) device;

	// release sound if currently active 
	if (m_pDSB || m_lpCE)
		Release();

    m_Mode= e_SimpleWav;

    HRESULT res;


	TRACE("GDSSoundEmitter::Initialize(.) for static mode reading a file. %s .\n",fileName);

	m_parent = DSdevice;
	m_isMidi = FALSE;
	m_volume = DSBVOLUME_MAX;
	m_intensity = 1.0f;
	m_fileName = fileName;

	// to think :
	// CTRLFREQ (pitch) is overhead if not needed !
	// pitch not known at start, could be changed by ROUTES 

    if (!spatialize || !DSdevice->m_has3d ||(DSdevice->m_quality <= GS_QUALITY_NO3D)) // NO 3D sound 
		m_pDSB = DSLoadSoundBuffer(DSdevice->m_lpDS, fileName,&m_BufferSize);
    else { 
		m_pDSB = DSLoad3DSoundBuffer(DSdevice->m_lpDS, fileName,&m_BufferSize);
		// above function will fall back to non 3d buffer automatically 
	}


    if (!m_pDSB ) {

		// check for midi
		return FALSE; 
	}
	
	// get the waveformat structure
	DWORD size;
	
	m_pDSB->GetFormat(&m_wfx,sizeof(m_wfx),&size);
	m_bytesPerSec = m_wfx.nAvgBytesPerSec;
	
	if (m_wfx.wBitsPerSample == 0 || m_wfx.nSamplesPerSec ==0 || (m_wfx.nChannels==0 )) {
		m_duration = 1.0; // ???
	} else {
		long nSamples =  ((long) m_BufferSize*8) / (m_wfx.nChannels * m_wfx.wBitsPerSample);
		m_duration = (1000l*nSamples) / (long) m_wfx.nSamplesPerSec ;
	}

	//TRACE("Duration %d \n",m_duration);
     
    res = m_pDSB ->QueryInterface(IID_IDirectSound3DBuffer, (void**)&m_lpCE);
    if (!m_lpCE) {
		TRACE("No 3D Sound Buffer Interface. \n");
		m_is3d = FALSE;
	}
	else m_is3d = TRUE; // do software muting

	// turn it of at the beginning 
	m_enabled = FALSE;
	m_muted = FALSE;
    m_enabledTime = timeGetTime();
	
	ASSERT(m_next == NULL);

	// add entry to emitter list 
	DSdevice->Add(this);

	
	 if (m_lpCE) {
        // Set the minimum distance at which the sound's amplitude should decay.
        // m_lpCE->SetMinDistance((D3DVALUE)10.0, DS3D_IMMEDIATE);

		 m_lpCE->SetConeAngles(DS3D_MAXCONEANGLE,DS3D_MAXCONEANGLE,DS3D_DEFERRED);
		 if (!spatialize) m_lpCE->SetMode(DS3DMODE_DISABLE,DS3D_DEFERRED);
		 if (m_enabled) m_parent->NeedCommit();
	 }
	 return TRUE;

}



// create a sound attached to a stream object, setting the Emitter in e_Streaming Mode.
BOOL GDSSoundEmitter::Initialize(GSoundDevice *device, DWORD Frequency, DWORD Channels, DWORD BitsPerSample, double BufferLength, BOOL spatialize)
{

    ASSERT(RTISA(device, GDSSoundDevice));
    GDSSoundDevice* DSdevice= (GDSSoundDevice*) device;

	// release sound if currently active 
	if (m_pDSB || m_lpCE)
		Release();

    m_Mode= e_Streaming;

 	HRESULT res;


	TRACE("GDSSoundEmitter::Initialize(.) for streaming mode. Freq<%d> Channels<%d> BitsPerChan<%d> length<%5.3g> \n", Frequency, Channels, BitsPerSample, BufferLength);

	m_parent=    DSdevice;
	m_isMidi=    FALSE;
	m_volume=    DSBVOLUME_MAX;
	m_intensity= 1.0f;
	m_fileName=  "";

	// to think :
	// CTRLFREQ (pitch) is overhead if not needed !
	// pitch not known at start, could be changed by ROUTES 
    //spatialize && DSdevice->m_has3d && (DSdevice->m_quality > GS_QUALITY_NO3D)

    m_pDSB= DSSetupSoundBuffer(DSdevice->m_lpDS, Frequency, Channels, BitsPerSample, BufferLength, &m_BufferSize,
                                  spatialize && DSdevice->m_has3d && (DSdevice->m_quality > GS_QUALITY_NO3D)
                              );

    DS3D_SUCCEED(m_pDSB->GetCurrentPosition(NULL, &mWriteCursor));
    mNotYetWritten= TRUE;

    WAVEFORMATEX wfx;
    mSecondsPerByte= DS3D_SUCCEED(m_pDSB->GetFormat(&wfx, sizeof(wfx), NULL));
    if(wfx.nAvgBytesPerSec)
        mBytesPerSec= wfx.nAvgBytesPerSec;
    else
        mBytesPerSec= 11025*1*2;    // to guess something.

    mSecondsPerByte= 1/mBytesPerSec;


    if(!m_pDSB)
    {
        TRACE("GDSSoundEmitter::Initialize(streaming): Could not set up a sound buffer.\n");
return FALSE; 
	}

	
	// get the waveformat structure
	DWORD size;
	
	m_pDSB->GetFormat(&m_wfx,sizeof(m_wfx),&size);
	m_bytesPerSec = m_wfx.nAvgBytesPerSec;
	
    m_duration= 1000;  //TBD:

    // TBD:  Do this only if spatialisation is wanted:
    res= m_pDSB ->QueryInterface(IID_IDirectSound3DBuffer, (void**)&m_lpCE);
    if (!m_lpCE) {
		TRACE("GDSSoundEmitter::Initialize(streaming): could not get 3D sound buffer interface. \n");
		m_is3d= FALSE;
	}
	else m_is3d= TRUE; // do software muting

	//turn it of at the beginning 
	m_enabled=     FALSE;
	m_muted=       FALSE;
    m_enabledTime= timeGetTime();
	
	ASSERT(!m_next);

	// add entry to emitter list 
	DSdevice->Add(this);


    if(m_lpCE)
    {
        // Set the minimum distance at which the sound's amplitude should decay.
        // m_lpCE->SetMinDistance((D3DVALUE)10.0, DS3D_IMMEDIATE);
        
        m_lpCE->SetConeAngles(DS3D_MAXCONEANGLE,DS3D_MAXCONEANGLE,DS3D_DEFERRED);
        if (!spatialize) m_lpCE->SetMode(DS3DMODE_DISABLE,DS3D_DEFERRED);
        if (m_enabled) m_parent->NeedCommit();
    }

return TRUE;
}






// query state of sound 
BOOL GDSSoundEmitter::QueryInfo(BOOL &playing,float &secondsPlayed,float &totalSeconds, int &numLoops,float &level)
{

	playing = FALSE;

	if (!m_pDSB) return FALSE;
	
	if (!SUCCEEDED(m_pDSB->GetStatus(&status))) return FALSE;

	playing = (status & DSBSTATUS_PLAYING) != 0;

	
	// get position 
    DWORD pos = 0,wpos=0;
    m_pDSB->GetCurrentPosition(&pos,&wpos);

	secondsPlayed =  pos / (float) m_bytesPerSec;


	totalSeconds = m_duration / 1000.0f;

	numLoops = -1;
	level = 1;

/// #	TRACE("GDSSoundEmitter:Query %d %f %f %d %f %d\n",playing, secondsPlayed, totalSeconds, numLoops, level, status);
	return (TRUE);

}
/*

  Problems : looping out of sync, not exact over time 
time = TimeGetTime();
Play();
...fill buffer with data...service it...etc...
msbufferlen = buffersize/samplerate/(bStereo?2:1)/(b16Bit?2:1) * 1000;
currentlen = currentpos/samplerate/(bStereo?2:1)/(b16Bit?2:1) * 1000;
totaltimeplayed = ((TimeGetTime() - time) % msbufferlen) * msbufferlen +
currentlen;
*/


// get sort value 
float GDSSoundEmitter::getValue() 
{ return  /* qmi.fAudibleLevel+ */
		  -m_currentVolume +
		  m_priority*0.5f; 
}


// compute a volume for non 3D soundss

void GDSSoundEmitter::ComputeDistanceAttentuation() 
{
	long myMinVolume =  0.3* DSBVOLUME_MIN;  // DSBVOLUME_MIN to strong 

	float d = sqrt(m_distance2);

	long volume = DSBVOLUME_MAX;

	if (d <= m_minDistance) {
		volume = m_volume;
	}
	else if (d > m_maxDistance) {
		volume = DSBVOLUME_MIN;
	}
	else {

		float f= (d-m_minDistance) * m_distMult;
		
		if (myMinVolume > m_volume) myMinVolume = DSBVOLUME_MIN; // must be below intensity

		volume = (1.0-f) * m_volume + f * myMinVolume;
	}
	
	m_currentVolume = volume;

	if (m_pDSB) m_pDSB->SetVolume(volume);


}

// Sets/updates the emitter's model parameters. 
void GDSSoundEmitter::SetModel(
	float			fMinBack,
	float			fMinFront,
	float			fMaxBack,
	float			fMaxFront,
	float			fIntensity
	)
{

	
	// amplification  not supported
	// must have DSBCAPS_CTRLPAN
	// DS settings IS DB
	// TO DO: how map 0..1 to DB ????
	// VRML spec 0..1 -20 DB to max

	m_maxDistance2 = fMaxFront * fMaxFront; // maximum audible distance
	
	m_minDistance = fMinFront;
	m_maxDistance = fMaxFront;

	m_distMult = 1.0f / (m_maxDistance-m_minDistance);

	if (fIntensity != m_intensity) {
		m_volume = DSBVOLUME_MAX;

		//>   LONG lVolume=LONG(log(float(nPercent)/100.0f)/log(10.0f)*1000);
		// ( LONG )(( log( percent/100.0 )/log( 2 ))*1000 )
/*

	to do to do
		if (fIntensity <=0.0f)
		{
			m_volume = DSBVOLUME_MIN;
		}
		else
		{
			m_volume=LONG(log(fIntensity)/log(10.0f)*1000);
			if (m_volume < DSBVOLUME_MIN) lVolume = DSBVOLUME_MIN;
			else if (m_volume > DBVOLUME_MAX) lVolume = DSBVOLUME_MAX;
		}
*/
		if (fIntensity <= 0.0f) {
			m_volume = DSBVOLUME_MIN;
		}
		else 
		if (fIntensity < 1.0f) 
			m_volume = (1.0-fIntensity) *  0.2f* (float) (DSBVOLUME_MIN);

		m_currentVolume = m_volume;
		m_intensity = fIntensity;

		if (!is3d()) { // need to adjust by distance 
				ComputeDistanceAttentuation();
		
		} else { // Direct 3D Sound will handle 
			if (m_pDSB) m_pDSB->SetVolume(m_volume);
			if (m_enabled) m_parent->NeedCommit();
		}

		

	}
	else {
		if (!is3d()) { // need to adjust by distance 
				ComputeDistanceAttentuation();
		}
    }
	

	if (!m_pDSB) return;
	

	if (!m_lpCE) return;
	
	m_lpCE->SetMinDistance(fMinFront,DS3D_DEFERRED);
	m_lpCE->SetMaxDistance(fMaxFront,DS3D_DEFERRED);

	// minBack maxBack not supported ?????
	// compute cone ???

	// must commit settings, so that they take effect
	if (m_enabled) m_parent->NeedCommit();

}



// enable / disable sound 
void GDSSoundEmitter::Enable(BOOL flag)
{
	if (!m_pDSB) return;

	if (flag){
		m_pDSB->Play(0,0,m_loop ? DSBPLAY_LOOPING  : 0);
    } else  {
		m_pDSB->Stop();
	} /* if */


	m_enabled = flag; 

}

// enable / disable via mute 
void GDSSoundEmitter::SetMuteState(BOOL flag)
{
	if (!m_pDSB) return;
	
	if (flag == m_muted) return;


	if (!flag && m_enabled && !(status & DSBSTATUS_PLAYING)) {

		//find the right startTime
		DWORD t= timeGetTime();
		t -= m_enabledTime;
		if (m_loop) t = t % m_duration;
		
		if (t<m_duration) {
			DWORD pos = 0;
			pos = (DWORD) (t * m_bytesPerSec) / 1000;
			m_pDSB->SetCurrentPosition(pos); // 6.01.2000 hg 
			m_pDSB->Play(0,0,m_loop ? DSBPLAY_LOOPING  : 0);
		}
		else Stop();

    } else  {
		m_pDSB->Stop();
	} /* if */

	m_muted = flag; 

}


// play sound 
void GDSSoundEmitter::Play(BOOL loop,float initialStartTime)
{

    switch(m_Mode)
    {
    case e_SimpleWav:
        m_loop = loop;

        if (m_pDSB) {
			// use saem muting rule as in PrioritizeSounds
		   if ((m_intensity>0.0001f) && (m_distance2  <= (m_maxDistance2+0.001f))) {
				DWORD pos = 0;
				pos = (DWORD) (initialStartTime * m_bytesPerSec);
	   
				m_pDSB->SetCurrentPosition(pos);
				m_pDSB->Play(0,0,m_loop ? DSBPLAY_LOOPING  : 0);
				status |= DSBSTATUS_PLAYING;
				if (m_lpCE) m_parent->NeedCommit();

				if (!m_enabled) m_parent->SoundListChanged();
				m_muted = FALSE;
		   } else {
			 m_muted = TRUE;
		   }			 
           m_enabled = TRUE; 
           m_enabledTime = timeGetTime()-initialStartTime/1000.0f;  // loading delay
        }
        break;

    case e_Streaming:

        //TBD: ASSERT(!loop); // Looping a streaming sound doesn't make sense.  
        ASSERT(!initialStartTime);   // This doesn't make sense either.

        m_loop= FALSE; 

        HRESULT dsr;
        
        if(m_pDSB)
        {
            DS3D_SUCCEED(dsr=m_pDSB->Play(0, 0, DSBPLAY_LOOPING));
            if(dsr)  
                TRACE("ERROR: Couldn't Play(.) the DSound Buffer.");  
            else
                TRACE("NOTE: Streaming GDSSoundEmitter now playing.");

            break; // hg ?????

            if (m_lpCE) m_parent->NeedCommit();

            if (!m_enabled) m_parent->SoundListChanged();

            m_enabled = TRUE; 
            m_muted = FALSE;
            m_enabledTime = timeGetTime();
        }
        break;


    default:
        ASSERT(FALSE);  // should never be reached.
        break;
    }
}



// stop playing sound 
void GDSSoundEmitter::Stop()
{

    switch(m_Mode)
    {
    case e_SimpleWav:

        if (m_pDSB)	{
           if (m_enabled) m_parent->SoundListChanged();
           m_pDSB->Stop();
           m_enabled = FALSE;
        }
        break;





    case e_Streaming:

		{
        if (!DS3D_SUCCEED(m_pDSB->Stop()))
        {
            TRACE("ERROR: Could not stop the DSound Buffer.");  
			return;	
        }
        if (!DS3D_SUCCEED(m_pDSB->GetCurrentPosition(NULL, &mWriteCursor))) {
            TRACE("ERROR: Could not GetCurrentPosition the DSound Buffer.");  
		}				
        TRACE("NOTE: GDSSoundEmitter stopped.");
		m_enabled = FALSE;
		}
        break;



    default:
        ASSERT(FALSE); // should never be reached.
    }
}




 



// GetMarkPosition retrieves the begin and end playback mark. 
// begin = 0 from begin end = 0 to end of file 
void GDSSoundEmitter::GetMarkPosition(float &beginTime, float &endTime)
{

	if (m_pDSB) {
		beginTime = 0;
		endTime = 0;
	}
	else {
		beginTime = 0;
		endTime = 0;
	}
}

// Sets the begin and end playback mark. 
void GDSSoundEmitter::SetMarkPosition(float beginTime, float endTime)
{
}

// get the pitch factor 
void GDSSoundEmitter::GetPitch(float &pitchFactor)
{
	DWORD freq;
	if (m_pDSB)	{
	   m_pDSB->GetFrequency(&freq);
	   pitchFactor = (float) (freq) / (float) m_wfx.nSamplesPerSec;
	}
	else {
		pitchFactor = 1.0;
	}
}

void GDSSoundEmitter::SetPitch(float pitchFactor)
{
	//if (pitchFactor <=0.25) pitchFactor = 0.25;
	//else if (pitchFactor > 4.0) pitchFactor = 4.0;

	if (pitchFactor != m_pitch)
	
	if (m_pDSB)	{
		DWORD freq = pitchFactor * (float) m_wfx.nSamplesPerSec;
		
		if (pitchFactor == 1.0f) freq = DSBFREQUENCY_ORIGINAL;
		else if (freq < DSBFREQUENCY_MIN) freq = DSBFREQUENCY_MIN;
		else if (freq > DSBFREQUENCY_MAX) freq = DSBFREQUENCY_MAX;
	    
		HRESULT result = m_pDSB->SetFrequency(freq);

		if (!DS3D_SUCCEED(result)) {
			TRACE("Error setting pitch to %f n",pitchFactor);
		}
	    m_pitch = pitchFactor;
		if (!is3d()) { 
		
		} else { 
			if (m_enabled) m_parent->NeedCommit();
		}

	}
}


// set position of sound 
void GDSSoundEmitter::SetPosition(const Point &position)
{
	if ((m_position.x != position.x) ||
	    (m_position.y != position.y) ||
	    (m_position.z != position.z))
	{
		m_position.x = position.x;
		m_position.y = position.y;
		m_position.z = position.z;
		
		if (m_parent) { // compute distance 
			Point d(m_parent->m_position.x-m_position.x,m_parent->m_position.y-m_position.y,m_parent->m_position.z-m_position.z);
			m_distance2 = d.Len2();
		}

		if (m_lpCE)	{
			m_lpCE->SetPosition(position.x,position.y,GDSZFLIP*position.z,DS3D_DEFERRED);
			if (m_enabled) m_parent->NeedCommit();
		}
	} else {
		if (m_parent) { // compute distance 
			Point d(m_parent->m_position.x-m_position.x,m_parent->m_position.y-m_position.y,m_parent->m_position.z-m_position.z);
			m_distance2 = d.Len2();
		}

	}
}

//Specifies a 3D vector that points in the same direction as the emitter's orientation. 
void GDSSoundEmitter::SetOrientation (const Point &direction)
{
	if ((m_direction.x != direction.x) ||
		(m_direction.y != direction.y) ||
		(m_direction.z != direction.z))
	{
		m_direction.x = direction.x;
		m_direction.y = direction.y;
		m_direction.z = direction.z;


		if (m_lpCE) {
			m_lpCE->SetConeOrientation(direction.x,direction.y,GDSZFLIP*direction.z,DS3D_DEFERRED);
			if (m_enabled) m_parent->NeedCommit();
		}
	}

}













  ////////////////////////////////////////////////////////////////////
 // Buffer Handling functions, and functions needed for streaming: //
////////////////////////////////////////////////////////////////////



DWORD GDSSoundEmitter::LockBuffer(DWORD Len, char* *Data1, DWORD *Len1, char* *Data2, DWORD *Len2)
{
    mNotYetWritten= FALSE;

    DWORD dsr;
	if (!DS3D_SUCCEED(dsr=m_pDSB->Lock(mWriteCursor, Len, (void**)&mpLockedData1, &mLockedLen1, (void**)&mpLockedData2, &mLockedLen2, 0))){
		ASSERT(0);
	}
    *Data1= mpLockedData1;        *Len1= mLockedLen1;
    *Data2= mpLockedData2;        *Len2= mLockedLen2;
	return dsr;
}




DWORD GDSSoundEmitter::UnlockBuffer()
{
  DWORD dsr;

  DS3D_SUCCEED(dsr=m_pDSB->Unlock(mpLockedData1, mLockedLen1, mpLockedData2, mLockedLen2));

  if(!dsr)
  {
    mWriteCursor= (mWriteCursor + mLockedLen1 + mLockedLen2) % m_BufferSize;
  }
  return dsr;

}




DWORD GDSSoundEmitter::WriteData(char *Data, DWORD Len)
{

  mNotYetWritten= FALSE;


  char *Data1, *Data2;
  DWORD  Len1,   Len2;
  DWORD dsr;

  DS3D_SUCCEED(dsr=m_pDSB->Lock(mWriteCursor, Len, (void**)&Data1, &Len1, (void**)&Data2, &Len2, 0));
  if(dsr)
return dsr;

  if(Data1)
    CopyMemory(Data1, Data, Len1);
  else
return -1;

  if(Data2)
    CopyMemory(Data2, Data+Len1, Len2);

  DS3D_SUCCEED(dsr=m_pDSB->Unlock(Data1, Len1, Data2, Len2));
  if(dsr)
return dsr;

  mWriteCursor= (mWriteCursor + Len) % m_BufferSize;

return 0;

}




DWORD GDSSoundEmitter::ZeroWrite(DWORD Count)
{
  if(Count <= 0)
return DS_OK;

  mNotYetWritten= FALSE;

  void *Data1, *Data2;
  DWORD  Len1,   Len2;
  DWORD dsr;

  DS3D_SUCCEED(dsr=m_pDSB->Lock(mWriteCursor, Count, &Data1, &Len1, &Data2, &Len2, 0));
  if(dsr)
return dsr;

  if(Data1)
    ZeroMemory(Data1, Len1);
  else
return -1;

  if(Data2)
    ZeroMemory(Data2, Len2);


  DS3D_SUCCEED(dsr=m_pDSB->Unlock(Data1, Len1, Data2, Len2));
  if(dsr)
return dsr;

  mWriteCursor= (mWriteCursor + Count) % m_BufferSize;

return DS_OK;
                   
}


DWORD GDSSoundEmitter::getSaveCursor()
{
  DWORD PlayCursor, SaveCursor;
  DWORD dsr;

  DS3D_SUCCEED(dsr=m_pDSB->GetCurrentPosition(&PlayCursor, &SaveCursor));

   return (SaveCursor - PlayCursor + m_BufferSize) % m_BufferSize;
}


double GDSSoundEmitter::getSaveCursorSec()
{
return getSaveCursor() * mSecondsPerByte;
}


DWORD GDSSoundEmitter::getWriteCursor()
{
  if(mNotYetWritten)
	return getSaveCursor();

  DWORD PlayCursor;
  DS3D_SUCCEED(m_pDSB->GetCurrentPosition(&PlayCursor, NULL));
  return (mWriteCursor - PlayCursor + m_BufferSize) % m_BufferSize;
}


double GDSSoundEmitter::getWriteCursorSec()
{
return getWriteCursor() * mSecondsPerByte;
}


DWORD GDSSoundEmitter::getBufferLength()
{
return m_BufferSize;
}


double GDSSoundEmitter::getBufferLengthSec()
{
return m_BufferSize * mSecondsPerByte;
}


DWORD GDSSoundEmitter::getBytesPerSec()
{
return mBytesPerSec;
}    

void GDSSoundEmitter::DecreaseWriteCursor(int delta)
{
  mWriteCursor+= (-delta +m_BufferSize) % m_BufferSize;
}

#ifdef __stream


// test , not yet working

//-----------------------------------------------------------------------------
// File: PlayStreaming.cpp
//
// Desc: DirectSound support file for sample showing how to load a wave file 
//       and play it using a streaming DirectSound buffer.
//
// Copyright (c) 1999 Microsoft Corp. All rights reserved.
//-----------------------------------------------------------------------------
#define STRICT
//#include <objbase.h>
//#include <initguid.h>
//#include <commdlg.h>
//#include <mmreg.h>
//#include <dsound.h>
//#include <stdio.h>


//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

GDSStreamingSoundEmitter::GDSStreamingSoundEmitter()
{


	g_pDS            = NULL;
	//m_pDSB      = NULL;
	g_pDSNotify      = NULL;
	g_pWaveSoundRead = NULL;




	ZeroMemory( &g_aPosNotify, sizeof(DSBPOSITIONNOTIFY) * 
                               (NUM_PLAY_NOTIFICATIONS + 1) );

	ZeroMemory( &g_hNotificationEvents[0], sizeof(g_hNotificationEvents));


    g_dwBufferSize      = 0;
    g_dwNotifySize      = 0;
    g_dwNextWriteOffset = 0;
    g_dwProgress        = 0;
    g_dwLastPos         = 0;
    g_bFoundEnd         = FALSE;

}




//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------


GDSStreamingSoundEmitter::~GDSStreamingSoundEmitter()
{
	Release();
}



UINT G__cdecl MyThreadProc( LPVOID pParam )
	{
			GDSStreamingSoundEmitter* pObject = (GDSStreamingSoundEmitter*)pParam;
			if (pObject == NULL)// || !pObject->IsKindOf(RUNTIME_CLASS(GDSStreamingSoundEmitter)))
				return 1;   // if pObject is not valid
				
			// do something with 'pObject'
			pObject->HandleNotificationLoop();

			return 0;   // thread completed successfully
	}

//-----------------------------------------------------------------------------
// 
// create a sound attached to a certain device 
// call loadwavfile
//
//-----------------------------------------------------------------------------
BOOL GDSStreamingSoundEmitter::Initialize(GDSSoundDevice *device,const char *fileName,BOOL spatialize)
{
	HRESULT res;
	
	
	g_pDS	= device->m_lpDS;
	

	g_hNotificationEvents[0] = CreateEvent( NULL, FALSE, FALSE, NULL ); // Play position
    g_hNotificationEvents[1] = CreateEvent( NULL, FALSE, FALSE, NULL ); // Stop

		
	DWORD bufferSize = 0;

	TRACE("GDSSoundEmitter::Initialize %s \n",fileName);

	m_parent = device;
	m_volume = DSBVOLUME_MAX;
	m_intensity = 1.0f;
	m_fileName = fileName;

/*
    if (!spatialize || !device->m_has3d ||(device->m_quality <= GS_QUALITY_NO3D)) // NO 3D sound 
		m_pDSB = DSLoadSoundBuffer(device->m_lpDS, fileName,&bufferSize);
    else { 
		m_pDSB = DSLoad3DSoundBuffer(device->m_lpDS, fileName,&bufferSize);
		// above function will fall back to non 3d buffer automatically 

	}


    if (!m_pDSB ) {

		// check for midi
		return FALSE; 
	}
	
	// get the waveformat structure
	DWORD size;
	
	m_pDSB->GetFormat(&m_wfx,sizeof(m_wfx),&size);
	m_bytesPerSec = m_wfx.nAvgBytesPerSec;
	
	if (m_wfx.wBitsPerSample == 0 || m_wfx.nSamplesPerSec ==0 || (m_wfx.nChannels==0 )) {
		m_duration = 1.0; // ???
	} else {
		long nSamples =  ((long) bufferSize*8) / (m_wfx.nChannels * m_wfx.wBitsPerSample);
		m_duration = (1000l*nSamples) / (long) m_wfx.nSamplesPerSec ;
	}

	//TRACE("Duration %d \n",m_duration);
     
	// Query to get the 3D interface, destroy the sound buffer if it's not available...
    res = m_pDSB ->QueryInterface(IID_IDirectSound3DBuffer, (void**)&m_lpCE);
    if (!m_lpCE) {

		TRACE("No 3D Sound Buffer Interface \n");
		m_is3d = FALSE;
	}
	else m_is3d = TRUE; // do software muting
*/
	// turn it of at the beginning 
	m_enabled = FALSE;
	m_muted = FALSE;
    m_enabledTime = timeGetTime();
	
	ASSERT(m_next == NULL);

	if (!LoadWaveFile(fileName)) {
		return FALSE;
	}

	// add entry to emitter list 
	device->Add(this);

	
	 if (m_lpCE) {
        // Set the minimum distance at which the sound's amplitude should decay.
        // m_lpCE->SetMinDistance((D3DVALUE)10.0, DS3D_IMMEDIATE);

		 m_lpCE->SetConeAngles(DS3D_MAXCONEANGLE,DS3D_MAXCONEANGLE,DS3D_DEFERRED);
		 if (!spatialize) m_lpCE->SetMode(DS3DMODE_DISABLE,DS3D_DEFERRED);
		 if (m_enabled) m_parent->NeedCommit();
	 }



	 return TRUE;

}







//-----------------------------------------------------------------------------
// Name: LoadWaveFile()
// Desc: Loads a wave file into a secondary streaming DirectSound buffer
//		 build a CWaveSoundRead object, call CreateStreamingBuffer
//-----------------------------------------------------------------------------
BOOL GDSStreamingSoundEmitter::LoadWaveFile(const TCHAR* strFileName )
{
	//convert from const char* to char*
	char* _strFileName = new char[strlen(strFileName)+1];
	strcpy(_strFileName, strFileName);
	
	
	// Close and delete any existing open wave file
    if( NULL != g_pWaveSoundRead )
    {
        g_pWaveSoundRead->Close();
        SAFE_DELETE( g_pWaveSoundRead );
    }

    g_pWaveSoundRead = new CWaveSoundRead();

    // Load the wave file
    if( FAILED( g_pWaveSoundRead->Open( _strFileName ) ) )
    {
        TRACE("Bad wave file.");
		return FALSE;

    }
    else // The load call succeeded
    {
        // Start with a new sound buffer object
        SAFE_RELEASE( g_pDSNotify );
        SAFE_RELEASE( m_pDSB );
        ZeroMemory( &g_aPosNotify, sizeof(DSBPOSITIONNOTIFY) );

        
		//
		// Create the sound buffer object from the wave file data
        //
		if( FAILED( CreateStreamingBuffer() ) )
        {
            TRACE("Couldn't create sound buffer.\n");
			return FALSE;
        }
		return TRUE;
  
    }
}




//-----------------------------------------------------------------------------
// Name: CreateStreamingBuffer()
// Desc: Creates a streaming buffer, and the notification events to handle
//       filling it as sound is played
//-----------------------------------------------------------------------------
HRESULT GDSStreamingSoundEmitter::CreateStreamingBuffer()
{
			HRESULT hr; 

			// This samples works by dividing a 3 second streaming buffer into 
			// NUM_PLAY_NOTIFICATIONS (or 16) pieces.  it creates a notification for each
			// piece and when a notification arrives then it fills the circular streaming 
			// buffer with new wav data over the sound data which was just played

			// The size of wave data is in pWaveFileSound->m_ckIn
			DWORD nBlockAlign = (DWORD)g_pWaveSoundRead->m_pwfx->nBlockAlign;
			INT nSamplesPerSec = g_pWaveSoundRead->m_pwfx->nSamplesPerSec;

			// The g_dwNotifySize should be an integer multiple of nBlockAlign
			g_dwNotifySize = nSamplesPerSec * 3 * nBlockAlign / NUM_PLAY_NOTIFICATIONS;
			g_dwNotifySize -= g_dwNotifySize % nBlockAlign;   

			// The buffersize should approximately 3 seconds of wav data
			g_dwBufferSize  = g_dwNotifySize * NUM_PLAY_NOTIFICATIONS;
			g_bFoundEnd     = FALSE;
			g_dwProgress    = 0;
			g_dwLastPos     = 0;

			// Set up the direct sound buffer, and only request the flags needed
			// since each requires some overhead and limits if the buffer can 
			// be hardware accelerated
			DSBUFFERDESC dsbd;
			ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
			dsbd.dwSize        = sizeof(DSBUFFERDESC);
			dsbd.dwFlags       = DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2;
			dsbd.dwBufferBytes = g_dwBufferSize;
			dsbd.lpwfxFormat   = g_pWaveSoundRead->m_pwfx;

			dsbd.dwFlags |= DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY; //                         | DSBCAPS_LOCSOFTWARE
						
			if (m_is3d) {
				dsbd.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2  | DSBCAPS_CTRL3D
				| DSBCAPS_MUTE3DATMAXDISTANCE |  DSBCAPS_CTRLVOLUME 
				| DSBCAPS_CTRLFREQUENCY;	// new 08.10.98 hg 
			}

    // Create a DirectSound buffer
    if( FAILED( hr = g_pDS->CreateSoundBuffer( &dsbd, &m_pDSB, NULL ) ) )
        return hr;

	// DSOUND: Error: VxD hardware buffers don't support CTRLPOSITIONNOTIFY
	// DSOUND: Error: Format not PCM
    // Create a notification event, for when the sound stops playing
    if( FAILED( hr = m_pDSB->QueryInterface( IID_IDirectSoundNotify, 
                                                  (VOID**)&g_pDSNotify ) ) )
        return hr;



			// get the waveformat structure
			DWORD size;
			
			m_pDSB->GetFormat(&m_wfx,sizeof(m_wfx),&size);
			m_bytesPerSec = m_wfx.nAvgBytesPerSec;
			
			if (m_wfx.wBitsPerSample == 0 || m_wfx.nSamplesPerSec ==0 || (m_wfx.nChannels==0 )) {
				m_duration = 1.0; // ???
			} else {
				m_duration = 100.0; // ???

				//long nSamples =  ((long) bufferSize*8) / (m_wfx.nChannels * m_wfx.wBitsPerSample);
				//m_duration = (1000l*nSamples) / (long) m_wfx.nSamplesPerSec ;
			}


	// Query to get the 3D interface
    hr = m_pDSB ->QueryInterface(IID_IDirectSound3DBuffer, (void**)&m_lpCE);
		
			if (!m_lpCE) {
			
				TRACE("No 3D Sound Buffer Interface \n");
				m_is3d = FALSE;
			}
			else m_is3d = TRUE; // do software muting



	// Tell DirectSound when to notify us. the notification will come in the from 
	// of signaled events that are handled in our notification thread
			for( INT i = 0; i < NUM_PLAY_NOTIFICATIONS; i++ )
			{
				g_aPosNotify[i].dwOffset = (g_dwNotifySize * i) + g_dwNotifySize - 1;
				g_aPosNotify[i].hEventNotify = g_hNotificationEvents[0];             
			}
    
			g_aPosNotify[i].dwOffset     = DSBPN_OFFSETSTOP; // stop event
			g_aPosNotify[i].hEventNotify = g_hNotificationEvents[1];


    if( FAILED( hr = g_pDSNotify->SetNotificationPositions( NUM_PLAY_NOTIFICATIONS + 1, 
                                                            g_aPosNotify ) ) )
        return hr;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: PlayBuffer()
// Desc: Play the DirectSound buffer
//-----------------------------------------------------------------------------
void GDSStreamingSoundEmitter::Play(BOOL loop,float initialStartTime)
{
    HRESULT hr;
    VOID*   pbBuffer = NULL;
	m_loop = loop;

    if( NULL == m_pDSB )
        return ;
	if (m_enabled) { 
		TRACE("already playing ??????\n");
	}

	Lock(); 
    // Restore the buffers if they are lost
    if( FAILED( hr = RestoreBuffers( m_loop ) ) )
    { Unlock(); return ; }
    // need to think how this fit with prioritize 	
	DWORD pos = 0;
	pos = (DWORD) (initialStartTime * m_bytesPerSec);

    // Fill the entire buffer with wave data
    if( FAILED( hr = FillBuffer( m_loop ) ) )
  	{ Unlock(); return ; }

	   
    //m_pDSB->SetCurrentPosition(pos);



    // Always play with the LOOPING flag since the streaming buffer
    // wraps around before the entire WAV is played
    if( FAILED( hr = m_pDSB->Play( 0, 0, DSBPLAY_LOOPING ) ) )
	{ Unlock(); return ; }

	AfxBeginThread(MyThreadProc, this);

	   if (m_lpCE) m_parent->NeedCommit();

  	   if (!m_enabled) m_parent->SoundListChanged();

	   m_enabled = TRUE; 
	   m_muted = FALSE;
	   m_enabledTime = timeGetTime()-initialStartTime/1000.0f;  // loading delay
	Unlock();

    return;
}




//-----------------------------------------------------------------------------
// Name: FillBuffer()
// Desc: Fills the DirectSound buffer with wave data
//-----------------------------------------------------------------------------
HRESULT GDSStreamingSoundEmitter::FillBuffer( BOOL bLooped )
{
    HRESULT hr;
    VOID*   pbBuffer = NULL;
    DWORD   dwBufferLength;

    if( NULL == m_pDSB )
        return E_FAIL;

    g_bFoundEnd = FALSE;
    g_dwNextWriteOffset = 0; 
    g_dwProgress = 0;
    g_dwLastPos  = 0;

    // Reset the wav file to the beginning
    g_pWaveSoundRead->Reset();
    m_pDSB->SetCurrentPosition( 0 );

    // Lock the buffer down, 
    if( FAILED( hr = m_pDSB->Lock( 0, g_dwBufferSize, 
                                        &pbBuffer, &dwBufferLength, 
                                        NULL, NULL, 0L ) ) )
        return hr;

    // Fill the buffer with wav data 
    if( FAILED( hr = WriteToBuffer( bLooped, pbBuffer, dwBufferLength ) ) )
        return hr;

    // Now unlock the buffer
    m_pDSB->Unlock( pbBuffer, dwBufferLength, NULL, 0 );

    g_dwNextWriteOffset = dwBufferLength; 
    g_dwNextWriteOffset %= g_dwBufferSize; // Circular buffer

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: WriteToBuffer()
// Desc: Writes wave data to the streaming DirectSound buffer 
//-----------------------------------------------------------------------------
HRESULT GDSStreamingSoundEmitter::WriteToBuffer( BOOL bLooped, VOID* pbBuffer, DWORD dwBufferLength )
{
    HRESULT hr;
    UINT nActualBytesWritten;

    if( !g_bFoundEnd )
    {
        // Fill the DirectSound buffer with WAV data
        if( FAILED( hr = g_pWaveSoundRead->Read( dwBufferLength, 
                                                 (BYTE*) pbBuffer, 
                                                 &nActualBytesWritten ) ) )           
            return hr;
    }
    else
    {
        // Fill the DirectSound buffer with silence
        FillMemory( pbBuffer, dwBufferLength, 
                    (BYTE)( g_pWaveSoundRead->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
        nActualBytesWritten = dwBufferLength;
    }

    // If the number of bytes written is less than the 
    // amount we requested, we have a short file.
    if( nActualBytesWritten < dwBufferLength )
    {
        if( !bLooped ) 
        {
            g_bFoundEnd = TRUE;

            // Fill in silence for the rest of the buffer.
            FillMemory( (BYTE*) pbBuffer + nActualBytesWritten, 
                        dwBufferLength - nActualBytesWritten, 
                        (BYTE)(g_pWaveSoundRead->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
        }
        else
        {
            // We are looping.
            UINT nWritten = nActualBytesWritten;    // From previous call above.
            while( nWritten < dwBufferLength )
            {  
                // This will keep reading in until the buffer is full. For very short files.
                if( FAILED( hr = g_pWaveSoundRead->Reset() ) )
                    return hr;

                if( FAILED( hr = g_pWaveSoundRead->Read( (UINT)dwBufferLength - nWritten,
                                                          (BYTE*)pbBuffer + nWritten,
                                                          &nActualBytesWritten ) ) )
                    return hr;

                nWritten += nActualBytesWritten;
            } 
        } 
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: StopBuffer()
// Desc: Stop the DirectSound buffer
//-----------------------------------------------------------------------------
void GDSStreamingSoundEmitter::Stop() 
{

	if (m_pDSB)	{
		Lock();
  	    if (m_enabled) m_parent->SoundListChanged();
		m_pDSB->Stop();
	    m_enabled = FALSE;
		Unlock();

		SetEvent(g_hNotificationEvents[1]); // send thread stop signal 

	}

	// stop notification thread ???
}




//-----------------------------------------------------------------------------
// Name: HandleNotification()
// Desc: Handle the notification that tell us to put more wav data in the 
//       circular buffer
//-----------------------------------------------------------------------------
HRESULT GDSStreamingSoundEmitter::HandleNotification( BOOL bLooped ) 
{
    HRESULT hr;
    VOID* pbBuffer  = NULL;
    DWORD dwBufferLength;

	if( NULL == m_pDSB )
        return E_FAIL;

    UpdateProgress();

	// TRACE("%p GDSStreamingSoundEmitter::HandleNotification %d\n",this,g_dwProgress);

    // Lock the buffer down
    if( FAILED( hr = m_pDSB->Lock( g_dwNextWriteOffset, g_dwNotifySize, 
                                        &pbBuffer, &dwBufferLength, NULL, NULL, 0L ) ) )
        return hr;

    // Fill the buffer with wav data 
    if( FAILED( hr = WriteToBuffer( bLooped, pbBuffer, dwBufferLength ) ) )
        return hr;

    // Now unlock the buffer
    m_pDSB->Unlock( pbBuffer, dwBufferLength, NULL, 0 );
    pbBuffer = NULL;

    // If the end was found, detrimine if there's more data to play 
    // and if not, stop playing
    if( g_bFoundEnd )
    {
        // We don't want to cut off the sound before it's done playing.
        // if doneplaying is set, the next notification event will post a stop message.
        if( g_pWaveSoundRead->m_ckInRiff.cksize > g_dwNotifySize )
        {
            if( g_dwProgress >= g_pWaveSoundRead->m_ckInRiff.cksize - g_dwNotifySize )
            {
                Stop(); // m_pDSB->Stop();
            }
        }
        else // For short files.
        {
            if( g_dwProgress >= g_pWaveSoundRead->m_ckInRiff.cksize )
            {
                Stop(); // m_pDSB->Stop();
            }
        }
    }

    g_dwNextWriteOffset += dwBufferLength; 
    g_dwNextWriteOffset %= g_dwBufferSize; // Circular buffer

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: UpdateProgress()
// Desc: Update the progress variable to know when the entire buffer was played
//-----------------------------------------------------------------------------
HRESULT GDSStreamingSoundEmitter::UpdateProgress()
{
    HRESULT hr;
    DWORD   dwPlayPos;
    DWORD   dwWritePos;
    DWORD   dwPlayed;

    if( FAILED( hr = m_pDSB->GetCurrentPosition( &dwPlayPos, &dwWritePos ) ) )
        return hr;

    if( dwPlayPos < g_dwLastPos )
        dwPlayed = g_dwBufferSize - g_dwLastPos + dwPlayPos;
    else
        dwPlayed = dwPlayPos - g_dwLastPos;

    g_dwProgress += dwPlayed;
    g_dwLastPos = dwPlayPos;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RestoreBuffers()
// Desc: Restore lost buffers and fill them up with sound if possible
//-----------------------------------------------------------------------------
HRESULT GDSStreamingSoundEmitter::RestoreBuffers( BOOL bLooped )
{
    HRESULT hr;

    if( NULL == m_pDSB )
        return S_OK;

    DWORD dwStatus;
    if( FAILED( hr = m_pDSB->GetStatus( &dwStatus ) ) )
        return hr;

    if( dwStatus & DSBSTATUS_BUFFERLOST )
    {
        // Since the app could have just been activated, then
        // DirectSound may not be giving us control yet, so 
        // the restoring the buffer may fail.  
        // If it does, sleep until DirectSound gives us control.
        do 
        {
            hr = m_pDSB->Restore();
            if( hr == DSERR_BUFFERLOST )
                Sleep( 10 );
        }
        while( hr = m_pDSB->Restore() );

        if( FAILED( hr = FillBuffer( bLooped ) ) )
            return hr;
    }

    return S_OK;
}





void GDSStreamingSoundEmitter::HandleNotificationLoop(void)
{

    MSG     msg;
    DWORD   dwResult;
    BOOL    bDone = FALSE;
    
	ref();
	TRACE("%p GDSStreamingSoundEmitter::HandleNotificationLoop  { begin \n",this);

    while( !bDone ) 
    { 
        dwResult = MsgWaitForMultipleObjects( 2, g_hNotificationEvents, 
                                              FALSE, INFINITE, QS_ALLEVENTS );
        switch( dwResult )
        {
            case WAIT_OBJECT_0 + 0:
                // g_hNotificationEvents[0] is signaled

                // This means that DirectSound just finished playing 
                // a piece of the buffer, so we need to fill the circular 
                // buffer with new sound from the wav file
				Lock();
                if( FAILED( HandleNotification( m_loop ) ) )
                {
                    TRACE("Error handling DirectSound notifications. Sample will now exit. DirectSound Sample\n");
                    bDone = TRUE;
					SetEvent(g_hNotificationEvents[1]); // send thread stop signal 

                }
				Unlock();

                break;

            case WAIT_OBJECT_0 + 1:
                // g_hNotificationEvents[1] is signaled

                // This means that the stop event was signaled, so 
                // update the UI controls to show the sound as stopped
                //OnEnablePlayUI( hDlg, TRUE );

				bDone = TRUE;

                break;

            case WAIT_OBJECT_0 + 2:
                // Messages are available
                /*while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
                { 
                    if( !IsDialogMessage( hDlg, &msg ) )  
                    {
                        TranslateMessage( &msg ); 
                        DispatchMessage( &msg ); 
                    }

                    if( msg.message == WM_QUIT )
                        bDone = TRUE;
                }*/
                break;
        }
    }

    // Clean up everything
    //FreeDirectSound();

    //CloseHandle( g_hNotificationEvents[0] );
    //CloseHandle( g_hNotificationEvents[1] );
	TRACE("%p GDSStreamingSoundEmitter::HandleNotificationLoop exit }\n",this);
	unref();

}


	// restore if buffer lost 
	BOOL GDSStreamingSoundEmitter::Restore()
	{
		return TRUE;
	}


	// release the sound 
	void GDSStreamingSoundEmitter::Release()
	{ 
	   if (m_enabled)
		   Stop();
        // wait for thread exit event set  
	   WaitForSingleObject(g_hNotificationEvents[1], INFINITE);
	
	   Lock();

	   if( NULL != g_pWaveSoundRead )
	   {
         g_pWaveSoundRead->Close();
         SAFE_DELETE( g_pWaveSoundRead );
	   }

       // Release DirectSound interfaces
      SAFE_RELEASE( g_pDSNotify );


 
	  GDSSoundEmitter::Release();

	  CloseHandle( g_hNotificationEvents[0] );
	  CloseHandle( g_hNotificationEvents[1] );	  
	  
	  Unlock();

	}
	


#endif __stream

#endif // G_DS
