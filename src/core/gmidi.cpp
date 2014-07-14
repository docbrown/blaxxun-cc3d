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

#include "gversion.h"


#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <assert.h>

#include "midstuff.h"

#include "gsound.h"
#include "gdssound.h"

#include "gmidi.h"


/*****************************************************************************/
/* Controls for buffer size, etc. */

#define TRACK_BUFFER_SIZE		1024
#define OUT_BUFFER_SIZE         1024 // Max stream buffer size in bytes
#define BUFFER_TIME_LENGTH		60   // Amount to fill in milliseconds
#define NUM_STREAM_BUFFERS              2

#define DEBUG_CALLBACK_TIMEOUT		2000	// Wait 2 seconds for callback

/*****************************************************************************/
#define TEMPO_TB_CX		150
#define TEMPO_TB_CY             32
#define VOL_TB_CX		150
#define VOL_TB_CY               32

#define TEMPO_TEXT_CX		TEMPO_TB_CX
#define VOL_TEXT_CX		VOL_TB_CX

#define VOL_TB_MIN		0
#define VOL_TB_MAX		1000
#define VOL_PAGESIZE		10
#define VOL_MIN			0
#define VOL_MAX			127
#define VOL_CACHE_INIT		100
#define TEMPO_MIN		1
#define TEMPO_MAX		500
#define TEMPO_PAGESIZE		10




#define WM_MSTREAM_PROGRESS	(WM_USER + 100)
#define WM_MSTREAM_UPDATEVOLUME	(WM_USER + 101)

/*****************************************************************************/
/* All sorts of bit flags and error codes for */
/* communicating between various subsystems.  */

#define MSTREAM_STOPF_NOREOPEN		0x0001

#define STREAMF_BUFFER_WARNING		0x00000002

#define CONVERTF_RESET			0x00000001

#define CONVERTF_STATUS_DONE		0x00000001
#define CONVERTF_STATUS_STUCK		0x00000002
#define CONVERTF_STATUS_GOTEVENT	0x00000004

#define CONVERTERR_NOERROR	0	// No error occured
#define CONVERTERR_CORRUPT	-101	// The input file is corrupt
// The converter has already encountered a corrupt file and cannot convert any
// more of this file -- you must reset the converter
#define CONVERTERR_STUCK	-102
#define CONVERTERR_DONE		-103	// Converter is done
#define CONVERTERR_BUFFERFULL	-104	// The buffer is full
#define CONVERTERR_METASKIP	-105	// Skipping unknown meta event

#define STATUS_KILLCALLBACK	100	// Signals that the callback should die
#define STATUS_CALLBACKDEAD	200	// Signals callback is done processing
#define STATUS_WAITINGFOREND	300    // Callback's waiting for buffers to play

#define VOLUME_BUFFER		3567	// Random value which acts as a buffer id


/*
 *   This structure is used to pass information to the ConvertToBuffer()
 * system and then internally by that function to send information about the
 * target stream buffer and current state of the conversion process to lower
 * level conversion routines internal to the MSTRCONV module.  See that source
 * file for specific details.
 */
typedef struct _ConvertInfo
{
    MIDIHDR	mhBuffer;	    // Standard Windows stream buffer header
    DWORD	dwStartOffset;	    // Start offset from mhStreamBuffer.lpStart
    DWORD	dwMaxLength;	    // Max length to convert on this pass

    DWORD	dwBytesRecorded;    // Used internally by the MSTRCONV module
    DWORD	tkStart;	    // Used internally by the MSTRCONV module
    BOOL	bTimesUp;	    // Used internally by the MSTRCONV module
} CONVERTINFO, *LPCONVERTINFO;


// Messages
//
static char szInitErrMem[]      = "Out of memory.\n";
static char szInitErrInFile[]   = "Read error on input file or file is corrupt.\n";
static char szNoTrackBuffMem[]  = "Insufficient memory for track buffer allocation\n";

static char szAppTitle[]      = "Midi player";

#ifdef DEBUG
static char gteBadRunStat[]     = "Reference to missing running status.";
static char gteRunStatMsgTrunc[]= "Running status message truncated";
static char gteChanMsgTrunc[]   = "Channel message truncated";
static char gteSysExLenTrunc[]  = "SysEx event truncated (length)";
static char gteSysExTrunc[]     = "SysEx event truncated";
static char gteMetaNoClass[]    = "Meta event truncated (no class byte)";
static char gteMetaLenTrunc[]   = "Meta event truncated (length)";
static char gteMetaTrunc[]      = "Meta event truncated";
static char gteNoMem[]          = "Out of memory during malloc call";
#endif



/*****************************************************************************/
/* Function declarations */

// void CALLBACK MidiProc( HMIDIIN, UINT, DWORD, DWORD, DWORD );



class GMidiEmitter : public  GDSSoundEmitter 
{
public:

    MMRESULT    mmrRetVal;

	char szFileBuffer[MAX_PATH];

	HMIDISTRM   hStream;
	BOOL		bFileOpen, bPlaying, bPaused, bInsertTempo;
	BOOL		bBuffersPrepared;	

	int			nCurrentBuffer, nEmptyBuffers;
	DWORD		dwBufferTickLength, dwTempoMultiplier, dwCurrentTempo;
	DWORD		dwProgressBytes, dwVolumePercent, dwVolCache[NUM_CHANNELS];
	UINT		uMIDIDeviceID, uCallbackStatus;
	CONVERTINFO ciStreamBuffers[NUM_STREAM_BUFFERS];

    HANDLE   hBufferReturnEvent;

	GMidiEmitter () {
		m_loop = FALSE;
		bFileOpen=FALSE;
		bPlaying=FALSE; 
		bPaused=FALSE;
		bInsertTempo = FALSE;
		bBuffersPrepared = FALSE;

		uMIDIDeviceID = MIDI_MAPPER;
		uCallbackStatus=0;
		hInFile = INVALID_HANDLE_VALUE;
		dwMallocBlocks = 0;
		dwVolumePercent = 1000;

	}

	// is soundEmitter properly setup ?

	BOOL Ok() { return (hStream != NULL) ; }

	// enable / disable sound 
	void Enable(BOOL flag) 
		{

			if (!hStream) return;

			if( flag ) {
				midiStreamRestart( hStream );
				bPaused = FALSE;
			}	
			else {
				midiStreamPause( hStream );
				bPaused = TRUE;
			}
			
			m_enabled = flag; 

		}

	// Sets/updates the emitter's model parameters. 
	// from the VRML Sound node
	void SetModel(
		float			fMinBack,
		float			fMinFront,
		float			fMaxBack,
		float			fMaxFront,
		float			fIntensity
		) 
	{

	m_maxDistance2 = fMaxFront * fMaxFront; // maximum audible distance
	
	m_minDistance = fMinFront;
	m_maxDistance = fMaxFront;

	m_distMult = 1.0f / (m_maxDistance-m_minDistance);

	if (fIntensity != m_intensity) {
		m_volume = DSBVOLUME_MAX;

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
			// if (m_enabled) m_parent->NeedCommit();
		}

		

	}
	else {
		if (!is3d()) { // need to adjust by distance 
				ComputeDistanceAttentuation();
		}
    }
	

	}


	// create a sound 
	BOOL Initialize(GDSSoundDevice *device, const char *fileName,BOOL spatialize)
	{
	m_parent = device;
	m_isMidi = TRUE;
	m_volume = DSBVOLUME_MAX;
	m_intensity = 1.0f;
	m_fileName = fileName;
	m_is3d = FALSE;

		if (!Init()) 
			return FALSE;
		strcpy( szFileBuffer,fileName);
		if (StreamBufferSetup()) 
			return FALSE;
		bFileOpen = TRUE;

	// turn it off at the beginning 
	m_enabled = FALSE;
	m_muted = FALSE;
    m_enabledTime = timeGetTime();
	
	ASSERT(m_next == NULL);

	// add entry to emitter list 
	device->Add(this);



	return TRUE;

	}

	// query state of sound 
	BOOL QueryInfo(BOOL &playing,float &secondsPlayed,float &totalSeconds, int &numLoops,float &level) 
	{
		playing = bPlaying;

		MMTIME mmt;
		mmt.wType = TIME_MS;
		
		if ((mmrRetVal = midiStreamPosition(hStream,&mmt,sizeof(mmt))) != MMSYSERR_NOERROR ) 
		{
			MidiErrorMessageBox( mmrRetVal );
		}
		switch(mmt.wType) {
		case TIME_MS  : secondsPlayed = (float) mmt.u.ms / 1000.0f; break;
		default :
			break;
		}
		return TRUE;

	}


	// play sound 
	//void Play(BOOL loop=TRUE, float initialStartTime);

	// stop playing sound 
	void Stop() { 
		Stop(MSTREAM_STOPF_NOREOPEN); 
	}

	// enable / disable via mute 
	void SetMuteState(BOOL flag)
	{
	
	if (flag == m_muted) return;


	if (!flag && m_enabled && !(bPlaying)) {

		//find the right startTime
		DWORD t= timeGetTime();
		t -= m_enabledTime;

		if (m_loop) t = t % m_duration;
		
		if (t<m_duration) {
			DWORD pos = 0;
			Play(m_loop,t);
		}
		else Stop();

    } else  {
		Stop();
	} /* if */

	m_muted = flag; 

	}


	// GetMarkPosition retrieves the begin and end playback mark. 
	// begin = 0 from begin end = 0 to end of file 
	// rsx only ?
	void GetMarkPosition(float &beginTime, float &endTime);
	
	// Sets the begin and end playback mark. 
	// rsx only ?
	void SetMarkPosition(float beginTime=0.0f, float endTime=0.0f);

	// get the pitch factor 
	void GetPitch(float &pitchFactor);

	// set the pitch factor 
	void SetPitch(float pitchFactor=1.0f);


	// set 3D position of sound 
	void SetPosition(const Point &position);

	//Specifies a 3D vector that points in the same direction as the emitter's orientation. 
	void SetOrientation (const Point &direction);


	// release the sound 
	void Release()
	{
		Term();
	}



	void SetVolume() {
		if( hStream )
			SetAllChannelVolumes( dwVolumePercent );
	}

	void SetPause() {
	    if( bPaused )
			midiStreamRestart( hStream );
		 else
			midiStreamPause( hStream );
	}


	void Play(BOOL loop,float initialStartTime) 
	{
			m_loop = loop;
			m_loop = loop;


		    if( bFileOpen )
			{
				// Clear the status of our callback so it will handle
				// MOM_DONE callbacks once more
				uCallbackStatus = 0;
				if(( mmrRetVal = midiStreamRestart( hStream ))
							!= MMSYSERR_NOERROR )
			    {
					MidiErrorMessageBox( mmrRetVal );
					return;
			    }
				bPlaying = TRUE;

		 	    m_enabled = TRUE; 
				m_muted = FALSE;
				m_enabledTime = timeGetTime()-initialStartTime/1000.0f;  // loading delay


			}
		    else
			{
				bPlaying = FALSE;
			}

	}

	void Stop(DWORD wParam)
	{
		    if( bFileOpen || bPlaying 
				|| ( uCallbackStatus != STATUS_CALLBACKDEAD ))
			{
			bPlaying = bPaused = FALSE;
	        m_enabled = FALSE;

			if( uCallbackStatus != STATUS_CALLBACKDEAD && uCallbackStatus != STATUS_WAITINGFOREND )
			    uCallbackStatus = STATUS_KILLCALLBACK;

			if (( mmrRetVal = midiStreamStop( hStream ))
							!= MMSYSERR_NOERROR )
			{
			    MidiErrorMessageBox( mmrRetVal );
			    return;
			}
			if (( mmrRetVal = midiOutReset( (HMIDIOUT)hStream ))
							!= MMSYSERR_NOERROR )
			    {
			    MidiErrorMessageBox( mmrRetVal );
			    return;
			    }

			// Wait for the callback thread to release this thread, which it will do by
			// calling SetEvent() once all buffers are returned to it
			if( WaitForSingleObject( hBufferReturnEvent,
						DEBUG_CALLBACK_TIMEOUT )
							    == WAIT_TIMEOUT )
			    {
// Note, this is a risky move because the callback may be genuinely busy, but
// when we're debugging, it's safer and faster than freezing the application,
// which leaves the MIDI device locked up and forces a system reset...
			    TRACE( "Timed out waiting for MIDI callback" );
			    uCallbackStatus = STATUS_CALLBACKDEAD;
			    }
			}

		    if( uCallbackStatus == STATUS_CALLBACKDEAD )
			{
			uCallbackStatus = 0;
			if( bFileOpen )
			    {
			    ConverterCleanup();
			    FreeBuffers();
			    if( hStream )
				{
				if(( mmrRetVal = midiStreamClose( hStream ))
							!= MMSYSERR_NOERROR )
				    {
				    MidiErrorMessageBox( mmrRetVal );
				    }
				hStream = NULL;
				}

			    bFileOpen = FALSE;
			    }
			
				if(!( wParam & MSTREAM_STOPF_NOREOPEN ))
			    {
			    if( StreamBufferSetup())
				{
				// Error setting up for MIDI file
				// Notification is already taken care of...
				return;
				}
			    else
				{
				bFileOpen = TRUE;
				}
			    }
			}

	}

	BOOL Init() {
		if(( mmrRetVal = midiStreamOpen( &hStream,
				    &uMIDIDeviceID,
				    (DWORD)1, (DWORD)MidiProc,
				    (DWORD)0,
				    CALLBACK_FUNCTION )) != MMSYSERR_NOERROR )
		{
			MidiErrorMessageBox( mmrRetVal );
			return( FALSE );
		}

		return( TRUE );
	}

	void Term() {
		if( hStream )
			Stop(MSTREAM_STOPF_NOREOPEN);

	    FreeBuffers();

	    if( hStream )
		{
			if(( mmrRetVal = midiStreamClose( hStream ))
							!= MMSYSERR_NOERROR )
		    {
				MidiErrorMessageBox( mmrRetVal );
		    }
			hStream = NULL;
		}
	}

	/****************************************************************************/
/* SetAllChannelVolumes()                                                   */
/*                                                                          */
/*   Given a percent in tenths of a percent, sets volume on all channels to */
/* reflect the new value.                                                   */
/****************************************************************************/
void SetAllChannelVolumes( DWORD dwVolumePercent )
    {
    DWORD       dwEvent, dwStatus, dwVol, idx;
    MMRESULT    mmrRetVal;

    if( !bPlaying )
	return;


    for( idx = 0, dwStatus = MIDI_CTRLCHANGE; idx < NUM_CHANNELS; idx++,
								dwStatus++ )
	{
	dwVol = ( dwVolCache[idx] * dwVolumePercent ) / 1000;
	dwEvent = dwStatus | ((DWORD)MIDICTRL_VOLUME << 8)
			| ((DWORD)dwVol << 16);
	if(( mmrRetVal = midiOutShortMsg( (HMIDIOUT)hStream, dwEvent ))
							!= MMSYSERR_NOERROR )
	    {
	    MidiErrorMessageBox( mmrRetVal );
	    return;
	    }
	}
    }


/****************************************************************************/
/* SetChannelVolume()                                                       */
/*                                                                          */
/*   Given a percent in tenths of a percent, sets volume on a specified     */
/* channel to reflect the new value.                                        */
/****************************************************************************/
void SetChannelVolume( DWORD dwChannel, DWORD dwVolumePercent )
    {
    DWORD       dwEvent, dwVol;
    MMRESULT    mmrRetVal;

    if( !bPlaying )
	return;

    dwVol = ( dwVolCache[dwChannel] * dwVolumePercent ) / 1000;
    dwEvent = MIDI_CTRLCHANGE | dwChannel | ((DWORD)MIDICTRL_VOLUME << 8)
							| ((DWORD)dwVol << 16);
    if(( mmrRetVal = midiOutShortMsg( (HMIDIOUT)hStream, dwEvent ))
							!= MMSYSERR_NOERROR )
	{
	MidiErrorMessageBox( mmrRetVal );
	return;
	}
    }



/*****************************************************************************/
/* FreeBuffers()                                                             */
/*                                                                           */
/*   This function unprepares and frees all our buffers -- something we must */
/* do to work around a bug in MMYSYSTEM that prevents a device from playing  */
/* back properly unless it is closed and reopened after each stop.           */
/*****************************************************************************/
void FreeBuffers()
{
    DWORD       idx;
    MMRESULT    mmrRetVal;

    if( bBuffersPrepared )
	{
	for( idx = 0; idx < NUM_STREAM_BUFFERS; idx++ )
	    if(( mmrRetVal = midiOutUnprepareHeader( (HMIDIOUT)hStream,
				&ciStreamBuffers[idx].mhBuffer,
				sizeof(MIDIHDR)))
						!= MMSYSERR_NOERROR )
		{
		MidiErrorMessageBox( mmrRetVal );
		}
	    bBuffersPrepared = FALSE;
	    }
	// Free our stream buffers...
	for( idx = 0; idx < NUM_STREAM_BUFFERS; idx++ )
	    if( ciStreamBuffers[idx].mhBuffer.lpData )
		{
		GlobalFreePtr( ciStreamBuffers[idx].mhBuffer.lpData );
		ciStreamBuffers[idx].mhBuffer.lpData = NULL;
		}
    }


/*****************************************************************************/
/* StreamBufferSetup()                                                       */
/*                                                                           */
/*    This function uses the filename stored in the global character array to*/
/* open a MIDI file. Then it goes about converting at least the first part of*/
/* that file into a midiStream buffer for playback.                          */
/*****************************************************************************/
BOOL StreamBufferSetup()
    {
    int     nChkErr;
    BOOL    bFoundEnd = FALSE;
    DWORD   dwConvertFlag, idx;

    MMRESULT            mmrRetVal;
    MIDIPROPTIMEDIV     mptd;

    if( !hStream )
	if(( mmrRetVal = midiStreamOpen( &hStream,
				    &uMIDIDeviceID,
				    (DWORD)1, (DWORD)MidiProc,
				    (DWORD)this,
				    CALLBACK_FUNCTION )) != MMSYSERR_NOERROR )
	    {
	    MidiErrorMessageBox( mmrRetVal );
	    return( TRUE );
	    }

    for( idx = 0; idx < NUM_STREAM_BUFFERS; idx++ )
	{
	ciStreamBuffers[idx].mhBuffer.dwBufferLength = OUT_BUFFER_SIZE;
	if(( ciStreamBuffers[idx].mhBuffer.lpData
		= (LPSTR) GlobalAllocPtr( GHND, OUT_BUFFER_SIZE )) == NULL )
	    {
	    // Buffers we already allocated will be killed by WM_DESTROY
	    // after we fail on the create by returning with -1
	    return( -1 );
	    }
	}
    if( ConverterInit( szFileBuffer ))
	return( TRUE );

    // Initialize the volume cache array to some pre-defined value
    for( idx = 0; idx < NUM_CHANNELS; idx++ )
	dwVolCache[idx] = VOL_CACHE_INIT;

    mptd.cbStruct = sizeof(mptd);
    mptd.dwTimeDiv = ifs.dwTimeDivision;
    if(( mmrRetVal = midiStreamProperty( hStream, (LPBYTE)&mptd,
					    MIDIPROP_SET | MIDIPROP_TIMEDIV ))
							!= MMSYSERR_NOERROR )
	{
	MidiErrorMessageBox( mmrRetVal );
	ConverterCleanup();
	return( TRUE );
	}

    nEmptyBuffers = 0;
    dwConvertFlag = CONVERTF_RESET;

    for( nCurrentBuffer = 0; nCurrentBuffer < NUM_STREAM_BUFFERS;
							    nCurrentBuffer++ )
	{
    // Tell the converter to convert up to one entire buffer's length of output
    // data. Also, set a flag so it knows to reset any saved state variables it
    // may keep from call to call.
	ciStreamBuffers[nCurrentBuffer].dwStartOffset = 0;
	ciStreamBuffers[nCurrentBuffer].dwMaxLength = OUT_BUFFER_SIZE;
	ciStreamBuffers[nCurrentBuffer].tkStart = 0;
	ciStreamBuffers[nCurrentBuffer].bTimesUp = FALSE;

	if(( nChkErr = ConvertToBuffer( dwConvertFlag,
					&ciStreamBuffers[nCurrentBuffer] ))
							!= CONVERTERR_NOERROR )
	    {
	    if( nChkErr == CONVERTERR_DONE )
		{
		bFoundEnd = TRUE;
		}
	    else
		{
		TRACE( "Initial conversion pass failed" );
		ConverterCleanup();
		return( TRUE );
		}
	    }
	ciStreamBuffers[nCurrentBuffer].mhBuffer.dwBytesRecorded
			    = ciStreamBuffers[nCurrentBuffer].dwBytesRecorded;

	if( !bBuffersPrepared )
	    if(( mmrRetVal = midiOutPrepareHeader( (HMIDIOUT)hStream,
				    &ciStreamBuffers[nCurrentBuffer].mhBuffer,
				    sizeof(MIDIHDR))) != MMSYSERR_NOERROR )
		{
		MidiErrorMessageBox( mmrRetVal );
		ConverterCleanup();
		return( TRUE );
		}
	if(( mmrRetVal = midiStreamOut( hStream,
				    &ciStreamBuffers[nCurrentBuffer].mhBuffer,
				    sizeof(MIDIHDR))) != MMSYSERR_NOERROR )
	    {
	    MidiErrorMessageBox( mmrRetVal );
	    break;
	    }
	dwConvertFlag = 0;

	if( bFoundEnd )
	    break;
	}

    bBuffersPrepared = TRUE;
    nCurrentBuffer = 0;
    // UpdateFromControls();
    return( FALSE );
    }


/*****************************************************************************/
/* MidiProc()                                                                */
/*                                                                           */
/*   This is the callback handler which continually refills MIDI data buffers*/
/* as they're returned to us from the audio subsystem.                       */
/*****************************************************************************/
static void CALLBACK MidiProc( HMIDIIN hMidi, UINT uMsg, DWORD dwInstance,
			DWORD dwParam1, DWORD dwParam2 )
{
		GMidiEmitter *me = (GMidiEmitter *)dwInstance;
		me->MidiCallback(hMidi,uMsg,dwInstance,dwParam1,dwParam2);
}

void MidiCallback( HMIDIIN hMidi, UINT uMsg, DWORD dwInstance,
			DWORD dwParam1, DWORD dwParam2 )
    {
    // static int  nWaitingBuffers = 0;
    MIDIEVENT   *pme;
    MIDIHDR     *pmh;

    MMRESULT    mmrRetVal;
    int         nChkErr;


    switch( uMsg )
	{
	case MOM_DONE:
	    if( uCallbackStatus == STATUS_CALLBACKDEAD )
		return;

	    nEmptyBuffers++;

	    if( uCallbackStatus == STATUS_WAITINGFOREND )
		{
		if( nEmptyBuffers < NUM_STREAM_BUFFERS )
		    {
		    return;
		    }
		else
		    {
		    uCallbackStatus = STATUS_CALLBACKDEAD;
		    // xxxxxx PostMessage( hWndMain, WM_COMMAND,MAKEWPARAM( IDC_STOP, 0 ), 0L );
		    SetEvent( hBufferReturnEvent );
		    return;
		    }
		}

	// This flag is set whenever the callback is waiting for all buffers to
	// come back.
	    if( uCallbackStatus == STATUS_KILLCALLBACK )
		{
		// Count NUM_STREAM_BUFFERS-1 being returned for the last time
		if( nEmptyBuffers < NUM_STREAM_BUFFERS )
		    {
		    return;
		    }
		// Then send a stop message when we get the last buffer back...
		else
		    {
		    // Change the status to callback dead
		    uCallbackStatus = STATUS_CALLBACKDEAD;
		    SetEvent( hBufferReturnEvent );
		    return;
		    }
		}

	    dwProgressBytes
		+= ciStreamBuffers[nCurrentBuffer].mhBuffer.dwBytesRecorded;
	    //xxxxx  PostMessage( hWndMain, WM_MSTREAM_PROGRESS, 0L, 0L );

///////////////////////////////////////////////////////////////////////////////
// Fill an available buffer with audio data again...

	    if( bPlaying && nEmptyBuffers )
		{
		ciStreamBuffers[nCurrentBuffer].dwStartOffset = 0;
		ciStreamBuffers[nCurrentBuffer].dwMaxLength = OUT_BUFFER_SIZE;
		ciStreamBuffers[nCurrentBuffer].tkStart = 0;
		ciStreamBuffers[nCurrentBuffer].dwBytesRecorded = 0;
		ciStreamBuffers[nCurrentBuffer].bTimesUp = FALSE;

		if(( nChkErr = ConvertToBuffer( 0,
					    &ciStreamBuffers[nCurrentBuffer] ))
							!= CONVERTERR_NOERROR )
		    {
		    if( nChkErr == CONVERTERR_DONE )
			{
			// Don't include this one in the count
			// nWaitingBuffers = NUM_STREAM_BUFFERS - 1;
			uCallbackStatus = STATUS_WAITINGFOREND;
			return;
			}
		    else
			{
			TRACE( "MidiProc() conversion pass failed!" );
			ConverterCleanup();
			return;
			}
		    }

		ciStreamBuffers[nCurrentBuffer].mhBuffer.dwBytesRecorded
			    = ciStreamBuffers[nCurrentBuffer].dwBytesRecorded;

		if(( mmrRetVal = midiStreamOut( hStream,
				    &ciStreamBuffers[nCurrentBuffer].mhBuffer,
				    sizeof(MIDIHDR))) != MMSYSERR_NOERROR )
		    {
		    MidiErrorMessageBox( mmrRetVal );
		    ConverterCleanup();
		    return;
		    }
		nCurrentBuffer = ( nCurrentBuffer + 1 ) % NUM_STREAM_BUFFERS;
		nEmptyBuffers--;
		}

	    break;

	case MOM_POSITIONCB:
	    pmh = (MIDIHDR *)dwParam1;
	    pme = (MIDIEVENT *)(pmh->lpData + pmh->dwOffset);
	    if( MIDIEVENT_TYPE( pme->dwEvent ) == MIDI_CTRLCHANGE )
		{
		if( MIDIEVENT_DATA1( pme->dwEvent ) == MIDICTRL_VOLUME_LSB )
		    {
		    TRACE( "Got an LSB volume event" );
		    break;
		    }

		if( MIDIEVENT_DATA1( pme->dwEvent ) != MIDICTRL_VOLUME )
		    break;

		// Mask off the channel number and cache the volume data byte
		dwVolCache[ MIDIEVENT_CHANNEL( pme->dwEvent )]
					= MIDIEVENT_VOLUME( pme->dwEvent );
		// Post a message so that the main program knows to counteract
		// the effects of the volume event in the stream with its own
		// generated event which reflects the proper trackbar position.
		// xxx PostMessage( hWndMain, WM_MSTREAM_UPDATEVOLUME, MIDIEVENT_CHANNEL( pme->dwEvent ), 0L );
		}
	    break;

	default:
	    break;
	}

    return;
    }


	void MidiErrorMessageBox( MMRESULT mmr )
    {
	char szTemp[512];
    midiOutGetErrorText( mmr, szTemp, sizeof(szTemp));
    MessageBox( GetActiveWindow(), szTemp, "Midi player",
			MB_OK | MB_ICONSTOP );
	#ifdef _DEBUG
		char szDebug[512];
		wsprintf( szDebug, "Midi subsystem error: %s", szTemp );
		TRACE( szDebug );
	#endif
    }

// converter

// ConverterInit
// 
// Open the input file
// Allocate and read the entire input file into memory
// Validate the input file structure
// Allocate the input track structures and initialize them
// Initialize the output track structures
//
// Return TRUE on success
// Prints its own error message if something goes wrong
//
BOOL ConverterInit( LPSTR szInFile )
    {
    BOOL                fRet = TRUE;
    DWORD               cbRead, dwTag, cbHeader, dwToRead;
    MIDIFILEHDR         Header;
    PINTRACKSTATE       ptsTrack;
    UINT                idx;

	char szTemp[256];

    tkCurrentTime = 0;

    // Initialize things we'll try to free later if we fail
    //
    memset( &ifs, 0, sizeof(INFILESTATE));
    ifs.cbFileLength = 0;
    ifs.pitsTracks = NULL;

    // Attempt to open the input and output files
    //
    hInFile = CreateFile( szInFile, GENERIC_READ,
			FILE_SHARE_READ, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);
    if( hInFile == INVALID_HANDLE_VALUE )
	{
	wsprintf( szTemp, "Could not open \"%s\" for read.\n", szInFile );
	MessageBox( GetActiveWindow(), szTemp,
			szAppTitle, MB_OK | MB_ICONEXCLAMATION );
	goto Init_Cleanup;
	}

// Figure out how big the input file is.
    if((( ifs.cbFileLength = GetFileSize( hInFile, NULL )) == (UINT)-1 ))
	{
	MessageBox( GetActiveWindow(), "File system error on input file.\n",
			szAppTitle, MB_OK | MB_ICONEXCLAMATION );
	goto Init_Cleanup;
	}

// Set up to read from the memory buffer. Read and validate
// - MThd header
// - size of file header chunk
// - file header itself
//      
    if( GetInFileData( &dwTag, sizeof(DWORD))
	    || ( dwTag != MThd )
	    || GetInFileData( &cbHeader, sizeof(DWORD))
	    || (( cbHeader = DWORDSWAP( cbHeader )) < sizeof(MIDIFILEHDR))
	    || GetInFileData( &Header, cbHeader ) )
	{
	MessageBox( GetActiveWindow(), szInitErrInFile,
			szAppTitle, MB_OK | MB_ICONEXCLAMATION );
	goto Init_Cleanup;
	}

// File header is stored in hi-lo order. Swap this into Intel order and save
// parameters in our native int size (32 bits)
//
    ifs.dwFormat = (DWORD)WORDSWAP( Header.wFormat );
    ifs.dwTrackCount = (DWORD)WORDSWAP( Header.wTrackCount );
    ifs.dwTimeDivision = (DWORD)WORDSWAP( Header.wTimeDivision );

// We know how many tracks there are; allocate the structures for them and parse
// them. The parse merely looks at the MTrk signature and track chunk length
// in order to skip to the next track header.
//
    ifs.pitsTracks = (PINTRACKSTATE)GlobalAllocPtr( GPTR,
				    ifs.dwTrackCount * sizeof(INTRACKSTATE));
    if( ifs.pitsTracks == NULL )
	{
	MessageBox( GetActiveWindow(), szInitErrMem,
			szAppTitle, MB_OK | MB_ICONEXCLAMATION );
	goto Init_Cleanup;
	}

    for( idx = 0, ptsTrack = ifs.pitsTracks; idx < ifs.dwTrackCount;
							    ++idx, ++ptsTrack )
	{
	if(( ptsTrack->pTrackStart
			= (LPBYTE) GlobalAllocPtr( GHND, TRACK_BUFFER_SIZE )) == NULL )
	    {
	    MessageBox( GetActiveWindow(), szNoTrackBuffMem,
				    szAppTitle, MB_OK | MB_ICONEXCLAMATION );
	    goto Init_Cleanup;
	    }

	if( GetInFileData( &dwTag, sizeof(dwTag)) || ( dwTag != MTrk )
			|| GetInFileData( &cbHeader, sizeof(cbHeader)))
	    {
	    MessageBox( GetActiveWindow(), szInitErrInFile,
				    szAppTitle, MB_OK | MB_ICONEXCLAMATION );
	    goto Init_Cleanup;
	    }

	cbHeader = DWORDSWAP( cbHeader );
	ptsTrack->dwTrackLength = cbHeader;     // Total track length
///////////////////////////////////////////////////////////////////////////////
// Here we need to determine if all track data will fit into a single one of
// our track buffers.  If not, we need to read in a buffer full and come back
// for more later, saving the file offset to continue from and the amount left
// to read in the track structure.

	// Save the file offset of the beginning of this track
	ptsTrack->foTrackStart = SetFilePointer( hInFile, 0, NULL,
						FILE_CURRENT );

	if( ptsTrack->dwTrackLength > TRACK_BUFFER_SIZE )
	    dwToRead = TRACK_BUFFER_SIZE;
	else
	    dwToRead = ptsTrack->dwTrackLength;
	if( !ReadFile( hInFile, ptsTrack->pTrackStart, dwToRead, &cbRead, NULL )
		|| ( cbRead != dwToRead ))
	    {
	    MessageBox( GetActiveWindow(), szInitErrInFile,
				szAppTitle, MB_OK | MB_ICONEXCLAMATION );
	    goto Init_Cleanup;
	    }
	// Save the number of bytes that didn't make it into the buffer
	ptsTrack->dwLeftOnDisk = ptsTrack->dwTrackLength - cbRead;
	ptsTrack->dwLeftInBuffer = cbRead;
	// Save the current file offset so we can seek to it later
	ptsTrack->foNextReadStart = SetFilePointer( hInFile, 0,
							NULL, FILE_CURRENT );

	// Setup pointer to the current position in the track
	ptsTrack->pTrackCurrent = ptsTrack->pTrackStart;
	ptsTrack->fdwTrack = 0;
	ptsTrack->byRunningStatus = 0;
	ptsTrack->tkNextEventDue = 0;

	// Handle bozo MIDI files which contain empty track chunks
	//
	if( !ptsTrack->dwLeftInBuffer && !ptsTrack->dwLeftOnDisk )
	    {
	    ptsTrack->fdwTrack |= ITS_F_ENDOFTRK;
	    continue;
	    }

	// We always preread the time from each track so the mixer code can
	// determine which track has the next event with a minimum of work
	//
	if( GetTrackVDWord( ptsTrack, &ptsTrack->tkNextEventDue ))
	    {
	    MessageBox( GetActiveWindow(), szInitErrInFile,
			szAppTitle, MB_OK | MB_ICONEXCLAMATION );
	    goto Init_Cleanup;
	    }
	// Step over any unread data, advancing to the beginning of the next
	// track's data
	SetFilePointer( hInFile, ptsTrack->foTrackStart + ptsTrack->dwTrackLength,
			NULL, FILE_BEGIN );
	}       // End of track initialization code

    fRet = FALSE;
	
    Init_Cleanup:
    if( fRet )
	ConverterCleanup();

    return( fRet );
    }

//
// GetInFileData
//
// Gets the requested number of bytes of data from the input file and returns
// a pointer to them.
// 
// Returns a pointer to the data or NULL if we'd read more than is
// there.
//
	BOOL GetInFileData( LPVOID lpDest, DWORD cbToGet )
    {
    DWORD       cbRead;

    if( !ReadFile( hInFile, lpDest, cbToGet, &cbRead, NULL )
		|| ( cbRead != cbToGet ))
	{
	return( TRUE );
	}

    return( FALSE );
    }


//
// ConverterCleanup
//
// Free anything we ever allocated
//

	//xx BOOL    bInsertTempo;

	//
	HANDLE   hInFile;
	INFILESTATE     ifs;
	DWORD    tkCurrentTime;

// Tracks how many malloc blocks exist. If there are any and we decide to shut
// down, we must scan for them and free them.  Malloc blocks are only created as
// temporary storgae blocks for extra parameter data associated with MIDI_SYSEX,
// MIDI_SYSEXEND, and MIDI_META events.
	DWORD    dwMallocBlocks;

	//DWORD    dwBufferTickLength, dwTempoMultiplier, dwCurrentTempo;
	//DWORD    dwProgressBytes, dwVolumePercent;
	BOOL     m_loop;


// Prototypes
//
/* 
static int      AddEventToStreamBuffer( PTEMPEVENT pteTemp, LPCONVERTINFO );
static BOOL     GetInFileData( LPVOID lpDest, DWORD cbToGet );
static BOOL     GetTrackByte( PINTRACKSTATE ptsTrack, LPBYTE lpbyByte );
static BOOL     GetTrackEvent( PINTRACKSTATE ptsTrack, PTEMPEVENT pteTemp );
static BOOL     GetTrackVDWord( PINTRACKSTATE ptsTrack, LPDWORD lpdw );
static BOOL     RefillTrackBuffer( PINTRACKSTATE ptsTrack );
static BOOL     RewindConverter( void );
*/ 

void ConverterCleanup()
    {
    DWORD   idx;

    if( hInFile != INVALID_HANDLE_VALUE )
	{
	CloseHandle( hInFile );
	hInFile = INVALID_HANDLE_VALUE;
	}

    if( ifs.pitsTracks )
	{
	// De-allocate all our track buffers
	for( idx = 0; idx < ifs.dwTrackCount; idx++ )
	    if( ifs.pitsTracks[idx].pTrackStart )
		GlobalFreePtr( ifs.pitsTracks[idx].pTrackStart );

	GlobalFreePtr( ifs.pitsTracks );
	ifs.pitsTracks = NULL;
	}
    }


/*****************************************************************************/
/* RewindConverter()                                                         */
/*                                                                           */
/*   This little function is an adaptation of the ConverterInit() code which */
/* resets the tracks without closing and opening the file, thus reducing the */
/* time it takes to loop back to the beginning when looping.                 */
/*****************************************************************************/
BOOL RewindConverter( void )
    {
    DWORD   dwToRead, cbRead, idx;
    BOOL    fRet;

    PINTRACKSTATE       ptsTrack;

    tkCurrentTime = 0;

    for( idx = 0, ptsTrack = ifs.pitsTracks; idx < ifs.dwTrackCount;
							    ++idx, ++ptsTrack )
	{
///////////////////////////////////////////////////////////////////////////////
// Here we need to determine if all track data will fit into a single one of
// our track buffers.  If not, we need to read in a buffer full and come back
// for more later, saving the file offset to continue from and the amount left
// to read in the track structure.

	SetFilePointer( hInFile, ptsTrack->foTrackStart, NULL, FILE_BEGIN );

	if( ptsTrack->dwTrackLength > TRACK_BUFFER_SIZE )
	    dwToRead = TRACK_BUFFER_SIZE;
	else
	    dwToRead = ptsTrack->dwTrackLength;
	if( !ReadFile( hInFile, ptsTrack->pTrackStart, dwToRead, &cbRead, NULL )
		|| ( cbRead != dwToRead ))
	    {
	    MessageBox( GetActiveWindow(), szInitErrInFile,
				szAppTitle, MB_OK | MB_ICONEXCLAMATION );
	    goto Rewind_Cleanup;
	    }
	// Save the number of bytes that didn't make it into the buffer
	ptsTrack->dwLeftOnDisk = ptsTrack->dwTrackLength - cbRead;
	ptsTrack->dwLeftInBuffer = cbRead;
	// Save the current file offset so we can seek to it later
	ptsTrack->foNextReadStart = SetFilePointer( hInFile, 0,
							NULL, FILE_CURRENT );

	// Setup pointer to the current position in the track
	ptsTrack->pTrackCurrent = ptsTrack->pTrackStart;
	ptsTrack->fdwTrack = 0;
	ptsTrack->byRunningStatus = 0;
	ptsTrack->tkNextEventDue = 0;


	// Handle bozo MIDI files which contain empty track chunks
	//
	if( !ptsTrack->dwLeftInBuffer && !ptsTrack->dwLeftOnDisk )
	    {
	    ptsTrack->fdwTrack |= ITS_F_ENDOFTRK;
	    continue;
	    }

	// We always preread the time from each track so the mixer code can
	// determine which track has the next event with a minimum of work
	//
	if( GetTrackVDWord( ptsTrack, &ptsTrack->tkNextEventDue ))
	    {
	    MessageBox( GetActiveWindow(), szInitErrInFile,
			szAppTitle, MB_OK | MB_ICONEXCLAMATION );
	    goto Rewind_Cleanup;
	    }
	// Step over any unread data, advancing to the beginning of the next
	// track's data
	SetFilePointer( hInFile, ptsTrack->foTrackStart + ptsTrack->dwTrackLength,
			NULL, FILE_BEGIN );
	}       // End of track initialization code

    fRet = FALSE;

    Rewind_Cleanup:

    if( fRet )
	return( TRUE );

    return( FALSE );
    }


/*****************************************************************************/
/* ConvertToBuffer()                                                         */
/*                                                                           */
/*    This function converts MIDI data from the track buffers setup by a     */
/* previous call to ConverterInit().  It will convert data until an error is */
/* encountered or the output buffer has been filled with as much event data  */
/* as possible, not to exceed dwMaxLength. This function can take a couple   */
/* bit flags, passed through dwFlags. Information about the success/failure  */
/* of this operation and the number of output bytes actually converted will  */
/* be returned in the CONVERTINFO structure pointed at by lpciInfo.          */
/*                                                                           */
/*****************************************************************************/
	INTRACKSTATE *ptsTrack, *ptsFound;
    DWORD        dwStatus;
    DWORD        tkNext;
    TEMPEVENT    teTemp;

int ConvertToBuffer( DWORD dwFlags, LPCONVERTINFO lpciInfo )
    {
/*
    static INTRACKSTATE *ptsTrack, *ptsFound;
    static DWORD        dwStatus;
    static DWORD        tkNext;
    static TEMPEVENT    teTemp;
*/

    int     nChkErr;
    DWORD   idx;

    lpciInfo->dwBytesRecorded = 0;

    if( dwFlags & CONVERTF_RESET )
	{
	dwProgressBytes = 0;
	dwStatus = 0;
	memset( &teTemp, 0, sizeof(TEMPEVENT));
	ptsTrack = ptsFound = NULL;
	}
    // If we were already done, then return with a warning...
    if( dwStatus & CONVERTF_STATUS_DONE )
	{
	if( m_loop )
	    {
	    RewindConverter();
	    dwProgressBytes = 0;
	    dwStatus = 0;
	    }
	else
	    return( CONVERTERR_DONE );
	}
    // The caller is asking us to continue, but we're already hosed because we
    // previously identified something as corrupt, so complain louder this time.
    else if( dwStatus & CONVERTF_STATUS_STUCK )
	{
	return( CONVERTERR_STUCK );
	}
    else if( dwStatus & CONVERTF_STATUS_GOTEVENT )
	{
	// Turn off this bit flag
	dwStatus ^= CONVERTF_STATUS_GOTEVENT;

	/*
	 *  The following code for this case is duplicated from below, and is
	 * designed to handle a "straggler" event, should we have one left over
	 * from previous processing the last time this function was called.
	 */

	// Don't add end of track event 'til we're done
	//
	if( teTemp.byShortData[0] == MIDI_META
				    && teTemp.byShortData[1] == MIDI_META_EOT )
	    {
	    if( dwMallocBlocks )
		{
			free( teTemp.pLongData );
			dwMallocBlocks--;
		}
	    }

	else if(( nChkErr = AddEventToStreamBuffer( &teTemp, lpciInfo ))
							!= CONVERTERR_NOERROR )
	    {
	    if( nChkErr == CONVERTERR_BUFFERFULL )
		{
		// Do some processing and tell caller that this buffer's full
		dwStatus |= CONVERTF_STATUS_GOTEVENT;
		return( CONVERTERR_NOERROR );
		}
	    else if( nChkErr == CONVERTERR_METASKIP )
		{
		// We skip by all meta events that aren't tempo changes...
		}
	    else
		{
		TRACE( "Unable to add event to stream buffer." );
		if( dwMallocBlocks )
		    {
		    free( teTemp.pLongData );
		    dwMallocBlocks--;
		    }
		return( TRUE );
		}
	    }
	}

    for( ; ; )
	{
	ptsFound = NULL;
	tkNext = 0xFFFFFFFFL;
	// Find nearest event due
	//
	for( idx = 0, ptsTrack = ifs.pitsTracks; idx < ifs.dwTrackCount;
							    ++idx, ++ptsTrack )
	    {
	    if(( !( ptsTrack->fdwTrack & ITS_F_ENDOFTRK ))
			&& ( ptsTrack->tkNextEventDue < tkNext ))
		{
		tkNext = ptsTrack->tkNextEventDue;
		ptsFound = ptsTrack;
		}
	    }

	// None found?  We must be done, so return to the caller with a smile.
	//
	if( !ptsFound )
	    {
	    dwStatus |= CONVERTF_STATUS_DONE;
	    // Need to set return buffer members properly
	    return( CONVERTERR_NOERROR );
	    }

	// Ok, get the event header from that track
	//
	if( GetTrackEvent( ptsFound, &teTemp ))
	    {
	    // Warn future calls that this converter is stuck at a corrupt spot
	    // and can't continue
	    dwStatus |= CONVERTF_STATUS_STUCK;
	    return( CONVERTERR_CORRUPT );
	    }

	// Don't add end of track event 'til we're done
	//
	if( teTemp.byShortData[0] == MIDI_META
				    && teTemp.byShortData[1] == MIDI_META_EOT )
	    {
	    if( dwMallocBlocks )
		{
		free( teTemp.pLongData );
		dwMallocBlocks--;
		}
	    continue;
	    }

	if(( nChkErr = AddEventToStreamBuffer( &teTemp, lpciInfo ))
							!= CONVERTERR_NOERROR )
	    {
	    if( nChkErr == CONVERTERR_BUFFERFULL )
		{
		// Do some processing and tell somebody this buffer is full...
		dwStatus |= CONVERTF_STATUS_GOTEVENT;
		return( CONVERTERR_NOERROR );
		}
	    else if( nChkErr == CONVERTERR_METASKIP )
		{
		// We skip by all meta events that aren't tempo changes...
		}
	    else
		{
		TRACE( "Unable to add event to stream buffer." );
		if( dwMallocBlocks )
		    {
		    free( teTemp.pLongData );
		    dwMallocBlocks--;
		    }
		return( TRUE );
		}
	    }
	}       

    return( CONVERTERR_NOERROR );
    }


//
// GetTrackVDWord
//
// Attempts to parse a variable length DWORD from the given track. A VDWord
// in a MIDI file
//  (a) is in lo-hi format 
//  (b) has the high bit set on every byte except the last
//
// Returns the DWORD in *lpdw and TRUE on success; else
// FALSE if we hit end of track first. Sets ITS_F_ENDOFTRK
// if we hit end of track.
//
BOOL GetTrackVDWord( PINTRACKSTATE ptsTrack, LPDWORD lpdw )
    {
    BYTE    byByte;
    DWORD   dw = 0;

    if( ptsTrack->fdwTrack & ITS_F_ENDOFTRK )
	return( TRUE );

    do
	{
	if( !ptsTrack->dwLeftInBuffer && !ptsTrack->dwLeftOnDisk )
	    {
	    ptsTrack->fdwTrack |= ITS_F_ENDOFTRK;
	    return( TRUE );
	    }

	if( GetTrackByte( ptsTrack, &byByte ))
	    return( TRUE );

	dw = ( dw << 7 ) | ( byByte & 0x7F );
	} while( byByte & 0x80 );

    *lpdw = dw;

    return( FALSE );
    }


//
// GetTrackEvent
//
// Fills in the event struct with the next event from the track
//
// pteTemp->tkEvent will contain the absolute tick time of the event
// pteTemp->byShortData[0] will contain
//  MIDI_META if the event is a meta event;
//   in this case pteTemp->byShortData[1] will contain the meta class
//  MIDI_SYSEX or MIDI_SYSEXEND if the event is a SysEx event
//  Otherwise, the event is a channel message and pteTemp->byShortData[1]
//   and pteTemp->byShortData[2] will contain the rest of the event.
//
// pteTemp->dwEventLength will contain
//  The total length of the channel message in pteTemp->byShortData if
//   the event is a channel message
//  The total length of the paramter data pointed to by
//   pteTemp->pLongData otherwise
//
// pteTemp->pLongData will point at any additional paramters if the 
//  event is a SysEx or meta event with non-zero length; else
//  it will contain NULL
//
// Returns FALSE on success or TRUE on any kind of parse error
// Prints its own error message ONLY in the debug version
//
// Maintains the state of the input track (i.e. ptsTrack->dwLeftInBuffer,
// ptsTrack->pTrackPointers, and ptsTrack->byRunningStatus).
//
BOOL GetTrackEvent( INTRACKSTATE *ptsTrack, PTEMPEVENT pteTemp )
    {
    DWORD   idx;
    BYTE    byByte;
    UINT    dwEventLength;

    // Clear out the temporary event structure to get rid of old data...
    memset( pteTemp, 0, sizeof(TEMPEVENT));

    // Already at end of track? There's nothing to read.
    //
    if(( ptsTrack->fdwTrack & ITS_F_ENDOFTRK )
		|| ( !ptsTrack->dwLeftInBuffer && !ptsTrack->dwLeftOnDisk ))
	return( TRUE );

    // Get the first byte, which determines the type of event.
    //
    if( GetTrackByte( ptsTrack, &byByte ))
	return( TRUE );

    // If the high bit is not set, then this is a channel message
    // which uses the status byte from the last channel message
    // we saw. NOTE: We do not clear running status across SysEx or
    // meta events even though the spec says to because there are
    // actually files out there which contain that sequence of data.
    //
    if( !( byByte & 0x80 ))
	{
	// No previous status byte? We're hosed.
	if( !ptsTrack->byRunningStatus )
	    {
	    TRACKERR(ptsTrack, gteBadRunStat);
	    return( TRUE );
	    }

	pteTemp->byShortData[0] = ptsTrack->byRunningStatus;
	pteTemp->byShortData[1] = byByte;

	byByte = pteTemp->byShortData[0] & 0xF0;
	pteTemp->dwEventLength = 2;

	// Only program change and channel pressure events are 2 bytes long;
	// the rest are 3 and need another byte
	//
	if(( byByte != MIDI_PRGMCHANGE ) && ( byByte != MIDI_CHANPRESS ))
	    {
	    if( !ptsTrack->dwLeftInBuffer && !ptsTrack->dwLeftOnDisk )
		{
		TRACKERR( ptsTrack, gteRunStatMsgTrunc );
		ptsTrack->fdwTrack |= ITS_F_ENDOFTRK;
		return( TRUE );
		}

	    if( GetTrackByte( ptsTrack, &pteTemp->byShortData[2] ))
		return( TRUE );
	    ++pteTemp->dwEventLength;
	    }
	}
    else if(( byByte & 0xF0 ) != MIDI_SYSEX )
	{
	// Not running status, not in SysEx range - must be
	// normal channel message (0x80-0xEF)
	//
	pteTemp->byShortData[0] = byByte;
	ptsTrack->byRunningStatus = byByte;
		
	// Strip off channel and just keep message type
	//
	byByte &= 0xF0;

	dwEventLength = ( byByte == MIDI_PRGMCHANGE || byByte == MIDI_CHANPRESS ) ? 1 : 2;
	pteTemp->dwEventLength = dwEventLength + 1;

	if(( ptsTrack->dwLeftInBuffer + ptsTrack->dwLeftOnDisk ) < dwEventLength )
	    {
	    TRACKERR( ptsTrack, gteChanMsgTrunc );
	    ptsTrack->fdwTrack |= ITS_F_ENDOFTRK;
	    return( TRUE );
	    }

	if( GetTrackByte( ptsTrack, &pteTemp->byShortData[1] ))
	    return( TRUE );
	if( dwEventLength == 2 )
	    if( GetTrackByte( ptsTrack, &pteTemp->byShortData[2] ))
		return( TRUE );
	} 
    else if(( byByte == MIDI_SYSEX ) || ( byByte == MIDI_SYSEXEND ))
	{
	// One of the SysEx types. (They are the same as far as we're concerned;
	// there is only a semantic difference in how the data would actually
	// get sent when the file is played. We must take care to put the proper
	// event type back on the output track, however.)
	//
	// Parse the general format of:
	//  BYTE        bEvent (MIDI_SYSEX or MIDI_SYSEXEND)
	//  VDWORD      cbParms
	//  BYTE        abParms[cbParms]
	//
	pteTemp->byShortData[0] = byByte;
	if( GetTrackVDWord( ptsTrack, &pteTemp->dwEventLength ))
	    {
	    TRACKERR( ptsTrack, gteSysExLenTrunc );
	    return( TRUE );
	    }

	if(( ptsTrack->dwLeftInBuffer + ptsTrack->dwLeftOnDisk )
						    < pteTemp->dwEventLength )
	    {
	    TRACKERR( ptsTrack, gteSysExTrunc );
	    ptsTrack->fdwTrack |= ITS_F_ENDOFTRK;
	    return( TRUE );
	    }

	// Malloc a temporary memory block to hold the parameter data
	if(( pteTemp->pLongData = (LPBYTE) malloc( pteTemp->dwEventLength )) == NULL )
	    {
	    TRACKERR( ptsTrack, gteNoMem );
	    return( TRUE );
	    }
	// Copy from the input buffer to the parameter data buffer
	for( idx = 0; idx < pteTemp->dwEventLength; idx++ )
	    if( GetTrackByte( ptsTrack, pteTemp->pLongData + idx ))
		{
		TRACKERR( ptsTrack, gteSysExTrunc );
		return( TRUE );
		}
	// Increment our counter, which tells the program to look around for
	// a malloc block to free, should it need to exit or reset before the
	// block would normally be freed
	dwMallocBlocks++;
	} 
    else if( byByte == MIDI_META )
	{
	// It's a meta event. Parse the general form:
	//  BYTE        bEvent  (MIDI_META)
	//  BYTE        bClass
	//  VDWORD      cbParms
	//  BYTE        abParms[cbParms]
	//
	pteTemp->byShortData[0] = byByte;

	if( !ptsTrack->dwLeftInBuffer && !ptsTrack->dwLeftOnDisk )
	    {
	    TRACKERR(ptsTrack, gteMetaNoClass );
	    ptsTrack->fdwTrack |= ITS_F_ENDOFTRK;
	    return( TRUE );
	    }

	if( GetTrackByte( ptsTrack, &pteTemp->byShortData[1] ))
	    return( TRUE );

	if( GetTrackVDWord( ptsTrack, &pteTemp->dwEventLength ))
	    {   
	    TRACKERR( ptsTrack, gteMetaLenTrunc );
	    return( TRUE );
	    }

	// NOTE: It's perfectly valid to have a meta with no data
	// In this case, dwEventLength == 0 and pLongData == NULL
	//
	if( pteTemp->dwEventLength )
	    {           
	    if(( ptsTrack->dwLeftInBuffer + ptsTrack->dwLeftOnDisk )
							< pteTemp->dwEventLength )
		{
		TRACKERR( ptsTrack, gteMetaTrunc );
		ptsTrack->fdwTrack |= ITS_F_ENDOFTRK;
		return( TRUE );
		}

	// Malloc a temporary memory block to hold the parameter data
	    if(( pteTemp->pLongData = (LPBYTE) malloc( pteTemp->dwEventLength ))
								    == NULL )
		{
		TRACKERR( ptsTrack, gteNoMem );
		return( TRUE );
		}
	// Copy from the input buffer to the parameter data buffer
	    for( idx = 0; idx < pteTemp->dwEventLength; idx++ )
		 if( GetTrackByte( ptsTrack, pteTemp->pLongData + idx ))
		     {
		     TRACKERR( ptsTrack, gteMetaTrunc );
		     return( TRUE );
		     }
	// Increment our counter, which tells the program to look around for
	// a malloc block to free, should it need to exit or reset before the
	// block would normally be freed
	    dwMallocBlocks++;
	    }

	if( pteTemp->byShortData[1] == MIDI_META_EOT )
	    ptsTrack->fdwTrack |= ITS_F_ENDOFTRK;
	}
    else
	{
	// Messages in this range are system messages and aren't supposed to
	// be in a normal MIDI file. If they are, we've either misparsed or the
	// authoring software is stupid.
	//
	return( TRUE );
	}

    // Event time was already stored as the current track time
    //
    pteTemp->tkEvent = ptsTrack->tkNextEventDue;

    // Now update to the next event time. The code above MUST properly
    // maintain the end of track flag in case the end of track meta is
    // missing.  NOTE: This code is a continuation of the track event
    // time pre-read which is done at the end of track initialization.
    //
    if( !( ptsTrack->fdwTrack & ITS_F_ENDOFTRK ))
	{
	DWORD   tkDelta;

	if( GetTrackVDWord( ptsTrack, &tkDelta ))
	    return( TRUE );

	ptsTrack->tkNextEventDue += tkDelta;
	}

    return( FALSE );
    }


//
// GetTrackByte
//
// Retrieve the next byte from the track buffer, refilling the buffer from
// disk if necessary.
//
BOOL GetTrackByte( PINTRACKSTATE ptsTrack, LPBYTE lpbyByte )
    {
    if( !ptsTrack->dwLeftInBuffer )
	{
	if( RefillTrackBuffer( ptsTrack ))
	    return( TRUE );
	}

    *lpbyByte = *ptsTrack->pTrackCurrent++;
    ptsTrack->dwLeftInBuffer--;
    return( FALSE );
    }


//
// RefillTrackBuffer()
//
// This function attempts to read in a buffer-full of data for a MIDI track.
//
BOOL RefillTrackBuffer( PINTRACKSTATE ptsTrack )
    {
    DWORD       dwBytesRead, dwResult;
    BOOL        bResult;

    if( ptsTrack->dwLeftOnDisk )
	{
	ptsTrack->pTrackCurrent = ptsTrack->pTrackStart;

	// Seek to the proper place in the file, indicated by
	// ptsTrack->foNextReadStart and read in the remaining data,
	// up to a maximum of the buffer size.

	if(( dwResult = SetFilePointer( hInFile,
				    (long)ptsTrack->foNextReadStart,
				    0L, FILE_BEGIN )) == 0xFFFFFFFF )
	    {
	    MessageBox( GetActiveWindow(),
			"Unable to seek to track buffer location in RefillTrackBuffer()!!",
			szAppTitle, MB_OK | MB_ICONEXCLAMATION );
	    return( TRUE );
	    }
	if( ptsTrack->dwLeftOnDisk > TRACK_BUFFER_SIZE )
	    ptsTrack->dwLeftInBuffer = TRACK_BUFFER_SIZE;
	else
	    ptsTrack->dwLeftInBuffer = ptsTrack->dwLeftOnDisk;
	bResult = ReadFile( hInFile, ptsTrack->pTrackStart,
				ptsTrack->dwLeftInBuffer,
				&dwBytesRead, NULL );

	ptsTrack->dwLeftOnDisk -= dwBytesRead;
	ptsTrack->foNextReadStart = dwResult + dwBytesRead;
	ptsTrack->dwLeftInBuffer = dwBytesRead;

	if( !bResult || ( bResult && !dwBytesRead )
		    || ( bResult && dwBytesRead != ptsTrack->dwLeftInBuffer ))
	    {
	    MessageBox( GetActiveWindow(),
			"Read operation failed prematurely!!",
			szAppTitle, MB_OK | MB_ICONEXCLAMATION );
	    ptsTrack->dwLeftInBuffer = dwBytesRead;
	    return( TRUE );
	    }
	else
	    return( FALSE );
	}

    return( TRUE );
    }


//
// AddEventToStreamBuffer
//
// Put the given event into the given stream buffer at the given location
// pteTemp must point to an event filled out in accordance with the
// description given in GetTrackEvent
//
// Returns FALSE on sucess or TRUE on an error condition
// Handles its own error notification by displaying to the appropriate
// output device (either our debugging window, or the screen).
//
int AddEventToStreamBuffer( PTEMPEVENT pteTemp, CONVERTINFO *lpciInfo )
    {
    DWORD       tkNow, tkDelta;
    MIDIEVENT   *pmeEvent;

    pmeEvent = (MIDIEVENT *)( lpciInfo->mhBuffer.lpData
						+ lpciInfo->dwStartOffset
						+ lpciInfo->dwBytesRecorded );

    // When we see a new, empty buffer, set the start time on it...
    if( !lpciInfo->dwBytesRecorded )
	lpciInfo->tkStart = tkCurrentTime;

    // Use the above set start time to figure out how much longer we should fill
    // this buffer before officially declaring it as "full"
    if( tkCurrentTime - lpciInfo->tkStart > dwBufferTickLength )
	if( lpciInfo->bTimesUp )
	    {
	    lpciInfo->bTimesUp = FALSE;
	    return( CONVERTERR_BUFFERFULL );
	    }
	else
	    lpciInfo->bTimesUp = TRUE;

    tkNow = tkCurrentTime;

    // Delta time is absolute event time minus absolute time
    // already gone by on this track
    tkDelta = pteTemp->tkEvent - tkCurrentTime;

    // Event time is now current time on this track
    tkCurrentTime = pteTemp->tkEvent;

    if( bInsertTempo )
	{
	bInsertTempo = FALSE;

	if( lpciInfo->dwMaxLength-lpciInfo->dwBytesRecorded < 3*sizeof(DWORD))
	    {
	    // Cleanup from our write operation
	    return( CONVERTERR_BUFFERFULL );
	    }
	if( dwCurrentTempo )
	    {
	    pmeEvent->dwDeltaTime = 0;
	    pmeEvent->dwStreamID = 0;
	    pmeEvent->dwEvent = ( dwCurrentTempo * 100 ) / dwTempoMultiplier;
	    pmeEvent->dwEvent |= (((DWORD)MEVT_TEMPO ) << 24 ) | MEVT_F_SHORT;
	
	    lpciInfo->dwBytesRecorded += 3 * sizeof(DWORD);
	    pmeEvent += 3 * sizeof(DWORD);
	    }
	}

    if( pteTemp->byShortData[0] < MIDI_SYSEX )
	{
	// Channel message. We know how long it is, just copy it.
	// Need 3 DWORD's: delta-t, stream-ID, event
	if( lpciInfo->dwMaxLength-lpciInfo->dwBytesRecorded < 3*sizeof(DWORD))
	    {
	    // Cleanup from our write operation
	    return( CONVERTERR_BUFFERFULL );
	    }

	pmeEvent->dwDeltaTime = tkDelta;
	pmeEvent->dwStreamID = 0;
	pmeEvent->dwEvent = ( pteTemp->byShortData[0] )
				| (((DWORD)pteTemp->byShortData[1] ) << 8 )
				| (((DWORD)pteTemp->byShortData[2] ) << 16 )
				| MEVT_F_SHORT;

	if((( pteTemp->byShortData[0] & 0xF0) == MIDI_CTRLCHANGE )
		&& ( pteTemp->byShortData[1] == MIDICTRL_VOLUME ))
	    {
	    // If this is a volume change, generate a callback so we can grab
	    // the new volume for our cache
	    pmeEvent->dwEvent |= MEVT_F_CALLBACK;
	    }
	lpciInfo->dwBytesRecorded += 3 *sizeof(DWORD);
	}
    else if(( pteTemp->byShortData[0] == MIDI_SYSEX )
		|| ( pteTemp->byShortData[0] == MIDI_SYSEXEND ))
	{
	TRACE( "AddEventToStreamBuffer: Ignoring SysEx event." );
	if( dwMallocBlocks )
	    {
	    free( pteTemp->pLongData );
	    dwMallocBlocks--;
	    }
	}
    else
	{
	// Better be a meta event.
	//  BYTE        byEvent
	//  BYTE        byEventType
	//  VDWORD      dwEventLength
	//  BYTE        pLongEventData[dwEventLength]
	//
	assert( pteTemp->byShortData[0] == MIDI_META );

	// The only meta-event we care about is change tempo
	//
	if( pteTemp->byShortData[1] != MIDI_META_TEMPO )
	    {
	    if( dwMallocBlocks )
		{
		free( pteTemp->pLongData );
		dwMallocBlocks--;
		}
	    return( CONVERTERR_METASKIP );
	    }

	// We should have three bytes of parameter data...
	assert( pteTemp->dwEventLength == 3 );

	// Need 3 DWORD's: delta-t, stream-ID, event data
	if( lpciInfo->dwMaxLength - lpciInfo->dwBytesRecorded < 3 *sizeof(DWORD))
	    {
	    // Cleanup the temporary event if necessary and return
	    if( dwMallocBlocks )
		{
			free( pteTemp->pLongData );
			dwMallocBlocks--;
		}
	    return( CONVERTERR_BUFFERFULL );
	    }

	pmeEvent->dwDeltaTime = tkDelta;
	pmeEvent->dwStreamID = 0;
	// Note: this is backwards from above because we're converting a single
	//       data value from hi-lo to lo-hi format...
	pmeEvent->dwEvent = ( pteTemp->pLongData[2] )
				| (((DWORD)pteTemp->pLongData[1] ) << 8 )
				| (((DWORD)pteTemp->pLongData[0] ) << 16 );

	/* This next step has absolutely nothing to do with the conversion of a
	 * MIDI file to a stream, it's simply put here to add the functionality
	 * of the tempo slider. If you don't need this, be sure to remove the
	 * next two lines.
	 */
	dwCurrentTempo = pmeEvent->dwEvent;
	pmeEvent->dwEvent = (pmeEvent->dwEvent * 100 ) / dwTempoMultiplier;

	pmeEvent->dwEvent |= (((DWORD)MEVT_TEMPO ) << 24 ) | MEVT_F_SHORT;

	dwBufferTickLength = ( ifs.dwTimeDivision * 1000 * BUFFER_TIME_LENGTH ) / dwCurrentTempo;
	//wsprintf( szTemp, "dwBufferTickLength = %lu", dwBufferTickLength );
	//TRACE( szTemp );

	if( dwMallocBlocks )
	    {
	    free( pteTemp->pLongData );
	    dwMallocBlocks--;
	    }
	lpciInfo->dwBytesRecorded += 3 *sizeof(DWORD);
	}

    return( FALSE );
    }


#ifdef DEBUG
void ShowTrackError( PINTRACKSTATE ptsTrack, LPSTR lpszErr )
    {
    TRACE("Track buffer offset %lu",
			(DWORD)(ptsTrack->pTrackCurrent - ptsTrack->pTrackStart));
    TRACE("Track total %lu  Track left %lu",
			ptsTrack->dwTrackLength, ptsTrack->dwLeftInBuffer );
    }
#endif

}; // class


// GetMarkPosition retrieves the begin and end playback mark. 
// begin = 0 from begin end = 0 to end of file 
void GMidiEmitter::GetMarkPosition(float &beginTime, float &endTime)
{

	beginTime = 0;
	endTime = 0;
}

// Sets the begin and end playback mark. 
void GMidiEmitter::SetMarkPosition(float beginTime, float endTime)
{
}

// get the pitch factor 
void GMidiEmitter::GetPitch(float &pitchFactor)
{
	   pitchFactor = m_pitch;
	
}

void GMidiEmitter::SetPitch(float pitchFactor)
{
	//if (pitchFactor <=0.25) pitchFactor = 0.25;
	//else if (pitchFactor > 4.0) pitchFactor = 4.0;

	if (pitchFactor != m_pitch) {
	
	    m_pitch = pitchFactor;
	
	}
}


// set position of sound 
void GMidiEmitter::SetPosition(const Point &position)
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

		// if (m_enabled) m_parent->NeedCommit();
	}
}

//Specifies a 3D vector that points in the same direction as the emitter's orientation. 
void GMidiEmitter::SetOrientation (const Point &direction)
{
	if ((m_direction.x != direction.x) ||
		(m_direction.y != direction.y) ||
		(m_direction.z != direction.z))
	{
		m_direction.x = direction.x;
		m_direction.y = direction.y;
		m_direction.z = direction.z;

		// if (m_enabled) m_parent->NeedCommit();
	}

}


// create a MIDI Emitter
GDSSoundEmitter* newMidiEmitter()
{
	return new GMidiEmitter();

}


