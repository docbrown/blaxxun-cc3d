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
#ifndef _GDSSound_h
#define _GDSSound_h
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

$Revision: 1.12 $
$Log: gdssound.h,v $
Revision 1.12  1999/03/10 10:22:06  tom
*** empty log message ***

Revision 1.5  1998/03/31 17:29:20  holger
empty message



Todo :
   
******************************************************************************/



class GDSSoundDevice;
class GDSSoundEmitter;


#include "gsound.h"


#ifdef G_DSOUND

//#include <mmsystem.h>

#include "dsound.h"



class GDSSoundDevice : public GSoundDevice 
{

public :

	RT(GDSSoundDevice);

    /*
    // DS objects
    */
	// Pointer to the Directsound API
	LPDIRECTSOUND			m_lpDS;
	
	BOOL m_has3d;

	BOOL has3d() { return m_has3d; }

	// Listener information (connected to the camera)
	LPDIRECTSOUND3DLISTENER  m_lpDL; // only av. starting from DX5

	// Primary Buffer (we don't REALLY need a pointer to it but we keep it just in case)
	LPDIRECTSOUNDBUFFER lpDSBuff;

/*
	// position of listener 
	DSVECTOR3D				m_position,m_direction,m_up;
	

*/



	// set the quality level, set before initialize 
	void SetQuality(int quality);

	// set the number of active sounds 
	int SetNumActiveSounds(int num);


	// constructor 
	GDSSoundDevice();

	// destructor
	~GDSSoundDevice();


	// is SoundDevice properly setup ?
	BOOL Ok() const  { return (m_lpDS != NULL); }



	// initialize DS and create listener 
	BOOL Initialize(HWND mainWnd);

	// terminate DS 
	void Term();

	// release all sound device resources 
	void Release();


	// set the lister position and orientation
	void SetViewpoint(const Point &position,const Vector &direction,const Vector &up,DWORD time);

	// play a single sound 
	BOOL PlaySound(const char *fileName);

	// add a sound 
	BOOL AddSound(int id,const char *fileName);

	// create new sound emitter 
	virtual GSoundEmitter* newSoundEmitter(int fileFmt= 0);


	// emitters allocated 
	GDSSoundEmitter* m_emitters;


	// emitter for PlaySound
	GDSSoundEmitter* m_playEmitter;

	// add new emitter 
	void Add(GDSSoundEmitter *emitter);

	// remove emitter 
	BOOL Remove(GDSSoundEmitter *emitter);

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

	// check commit 
	BOOL CheckCommit();


};

// sound 


class GDSSoundEmitter : public GSoundEmitter 
{
// Threadsafity: GDSSoundEmitter is not designed to be called in a multithreaded environment.
//               But playing streaming audio makes some functions being called in some "back
//               ground thread". For this, see the comment on m_CsDSB.



public:

	GDSSoundDevice			*m_parent; // we need back pointer to the Device 

	CString					m_fileName;
	BOOL					m_isMidi;

    // Sound buffer interfaces (the standard one and the 3D interface)
    LPDIRECTSOUNDBUFFER		m_pDSB;  // !!!For access on m_pDSB read the comment on m_CsDSB.
    LPDIRECTSOUND3DBUFFER	m_lpCE;
    
    CRITICAL_SECTION        m_CsDSB;  
    // The main work of this class is done in one thread. But some functions run in a separate
    // thread. These are the streaming functions. They access m_pDSB while it is possible, that
    // the "main thread" releases m_pDSB. Therefore a critical section (m_CsDSB) is
    // necessary. Since many functions acessing m_pDSB are only called in the "main thread", the
    // same thread which releases m_pDSB, they don't need accessing m_CsDSB.
    //
    // The following rules must be followed accessing m_pDSB:
    // 
    // 1.: The pointer m_pDSB may only be changed in the "main thread". Otherwise the following
    //     rules are insufficient. (Release() is the functions concerned.)
    //
    // 2.: Functions which are only called in the "main thread" don't need to use the critical
    //     section m_CsDSB to access m_pDSB. 
    //     But when later in future one wants to access them in another thread, access to 
    //     m_CsDSB must be added to them.
    //
    // 3.: Functions which may be called not only in the "main thread" (the functions needed for
    //     streaming do this) must call EnterCriticalSection(&m_CsDSB) before and 
    //     LeaveCriticalSection(&m_CsDSB) after they access m_pDSB.
    // 
    // 4.: The functions mentioned in 1.: must also call EnterCriticalSection(&m_CsDSB) and
    //     LeaveCriticalSection(&m_CsDSB) though they run in the "main thread".
    //
    //
    // When 3D sound will be added to the streaming functions, this may not only apply to m_pDSB,
    // but also to m_lpCE. 




    // The GDSSoundEmitter::Release() function must only be called in the 





	WAVEFORMATEX m_wfx;

	DWORD m_duration; // duration in msec
	DWORD m_bytesPerSec; // buffer size for one secone
    DWORD m_BufferSize;  // whole buffer size.
	
	// position and direction of sound emitter 
	//DSVECTOR3D			m_position,m_direction;
	//float					m_pitch;

	GDSSoundEmitter * m_next; // linked list 

	BOOL m_is3d;
	BOOL is3d() { return m_is3d; }

	BOOL isMidi() { return m_isMidi; }

    virtual BOOL CanDoStreaming()    { return TRUE; };



	float m_distance2; // distance to viewer 
	float m_maxDistance2; // distance to viewer 

	long m_volume;		// max volume computed from intensity 
	long m_currentVolume;	// distance volume computed

	float m_minDistance;   // minFront maxFront VRML params
	float m_maxDistance;
	float m_distMult;


	void ComputeDistanceAttentuation();
	

	// get sort value 
	float getValue();


	GDSSoundEmitter();
	~GDSSoundEmitter();

	// is soundEmitter properly setup ?
	BOOL Ok() { return (m_pDSB != NULL); }

	// enable / disable sound 
	void Enable(BOOL flag);


	// we should loop this one 
	BOOL m_loop;

	// enable / disable via mute 
	void SetMuteState(BOOL flag);


	// Sets/updates the emitter's model parameters. 
	void SetModel(
		float			fMinBack,
		float			fMinFront,
		float			fMaxBack,
		float			fMaxFront,
		float			fIntensity
		);


	// create a sound 
	virtual BOOL Initialize(GSoundDevice *device, const char *fileName, BOOL spatialize= FALSE);
    virtual BOOL Initialize(GSoundDevice *device, DWORD Frequency, DWORD Channels, DWORD BitsPerSample, double BufferLength, BOOL spatialize= FALSE);  // For streaming sound.
    // BufferLength is the required Length of the buffer in seconds.




	// restore if buffer lost 
	BOOL Restore();


	DWORD status;

	// query state of sound 
	BOOL QueryInfo(BOOL &playing,float &secondsPlayed,float &totalSeconds, int &numLoops,float &level);


	//!< play sound 
	virtual void Play(BOOL loop=TRUE, float initialStartTime=0.0f);

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


    















///////// Buffer handling: ////////////////     (needed in streaming mode.) 

    // See GSoundEmitter for Docoumentation.

public:     
    virtual DWORD WriteData(char *Data, DWORD Len);
    virtual DWORD ZeroWrite(DWORD Count);
  

    virtual DWORD getSaveCursor();
    virtual double getSaveCursorSec();

    virtual DWORD getWriteCursor();
    virtual double getWriteCursorSec();

    virtual DWORD getBufferLength();
    virtual double getBufferLengthSec();

    virtual DWORD getBytesPerSec();


    virtual void DecreaseWriteCursor(int delta);

    


// Data Members for buffer handling:

protected:

    
    DWORD mWriteCursor;  // The Write Pointer, maintained by this class, not by DSound.


  // Implementation:

    char *mpLockedData1, *mpLockedData2;
    DWORD   mLockedLen1,    mLockedLen2;

    BOOL mNotYetWritten;  // Is TRUE, until the buffer is locked the first time.
                          // This is needed for the getWriteCursor() to return the save Cursor
                          // untill the first data has been written.

    double mBytesPerSec;                          
    double mSecondsPerByte;  // = 1/(Sampling Rate * NoChannels * BytesPerChannel);


};






// sound 

// #define __stream
#ifdef __stream


#include "WavRead.h"

#define NUM_PLAY_NOTIFICATIONS  16

class GDSStreamingSoundEmitter : public GDSSoundEmitter, public CWinThread  
{
public:


	GDSStreamingSoundEmitter();
	~GDSStreamingSoundEmitter();

	// exclusive scene access
	CCriticalSection locker;	

	int locked;

	BOOL Lock() {locked++;  return locker.Lock(); }
	BOOL Unlock() {locked-- ; return locker.Unlock(); }


	BOOL Initialize(GDSSoundDevice *device,const char *fileName,BOOL spatialize);
	// cast to specific specialized version 
	BOOL Initialize(GSoundDevice *device, const char *fileName,BOOL spatialize=FALSE) {
		return Initialize((GDSSoundDevice *)device,fileName,spatialize);
	}


	HRESULT CreateStreamingBuffer();
	//HRESULT PlayBuffer( BOOL bLooped );
	HRESULT FillBuffer( BOOL bLooped );

	HRESULT WriteToBuffer( BOOL bLooped, VOID* pbBuffer, DWORD dwBufferLength );


	HRESULT UpdateProgress();

	HRESULT RestoreBuffers( BOOL bLooped );
	BOOL LoadWaveFile(const TCHAR* strFileName );

	HRESULT HandleNotification( BOOL bLooped ) ;
	void HandleNotificationLoop(void);




	LPDIRECTSOUND				g_pDS;

	LPDIRECTSOUNDNOTIFY			g_pDSNotify;
	
	class CWaveSoundRead*		g_pWaveSoundRead;

	DSBPOSITIONNOTIFY   g_aPosNotify[ NUM_PLAY_NOTIFICATIONS + 1 ];  
	HANDLE              g_hNotificationEvents[2];

	DWORD               g_dwBufferSize;
	DWORD               g_dwNotifySize;
	DWORD               g_dwNextWriteOffset;
	DWORD               g_dwProgress;
	DWORD               g_dwLastPos;
	BOOL                g_bFoundEnd;



// restore if buffer lost 
	BOOL Restore();

	// query state of sound 
//	BOOL QueryInfo(BOOL &playing,float &secondsPlayed,float &totalSeconds, int &numLoops,float &level);


	// play sound 
	virtual void Play(BOOL loop=TRUE, float initialStartTime=0.0f);

	// stop playing sound 
	virtual void Stop();

/*

	// GetMarkPosition retrieves the begin and end playback mark. 
	// begin = 0 from begin end = 0 to end of file 
	void GetMarkPosition(float &beginTime, float &endTime);
	// Sets the begin and end playback mark. 
	void SetMarkPosition(float beginTime=0.0f, float endTime=0.0f);

*/
	// release the sound 
	void Release();
	

};
#endif __stream



#endif




#endif
