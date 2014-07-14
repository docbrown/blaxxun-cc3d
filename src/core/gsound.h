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
#ifndef _GSound_h
#define _GSound_h

/******************************************************************************
@doc

@module GSound.h - abstract sound device|

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

$Revision: 1.17 $
$Log: gsound.h,v $
Revision 1.17  1999/07/06 16:55:04  tom
*** empty log message ***



Todo :

   remove RSX datatypes
   
******************************************************************************/



// #include "rsx.h"

#ifdef WIN32
#include "mmsystem.h"
#endif


class GSoundDevice;
class GSoundEmitter;


#define GS_QUALITY_NO3D 0
#define GS_QUALITY_MIN 1
#define GS_QUALITY_MID 2
#define GS_QUALITY_MAX 3


enum GSErrorCodes {
	GS_NOT_PCM_FORMAT=100, // WAV is not PCM format
	GS_FILE_NOT_FOUND,	// file error 
	GS_FILE_ERROR,		// file system error 
	GS_FILE_TRUNCATED,	// WAV is truncated 
	GS_FILE_BAD,		// BAD data 

};


//!
// GSoundDevice
// all datastructures for a sound playing device
//

/** 
	abstract root class for sound devices
*/


class GSoundDevice : public RTRoot {

public :

	RT(GSoundDevice);

	//! should 3D sound be used ?
	BOOL					m_use3d;	
	
	//! need to commit changes ? (to allow sound system 3D recomputation)
	BOOL					m_needCommit; 
	
	//! list of running sounds changed 
	BOOL					m_soundListChanged; 

	//! position of 3D listener 
	Point 					m_position,m_direction,m_up;
	
	//! last time listener was updated  in millisec
	DWORD					m_time; 
	
	//! the number of sound active at a time
	int						m_activeSounds; 

	//! quality if sound we want 
	int m_quality; 

	//! set the quality level, set before initialize 
	virtual void SetQuality(int quality)=0;

	// RSX specials 
	//! speed of sound (RSX)
	float					m_speedOfSound;

	//! set the number of active sounds 
	virtual int SetNumActiveSounds(int num)=0;

	//! max number of sounds active at a time
	int						m_numEnabledSounds;
	
	//! pass url for download to soundEmitter (RSX could do streaming download)
	BOOL					m_passUrl;


	//! constructor 
	GSoundDevice();

	//! destructor
	virtual ~GSoundDevice();


	//! is SoundDevice properly setup and working ?
	virtual BOOL Ok() const=0;

	//! signal that we need a commit 
	void NeedCommit() { m_needCommit = TRUE; }

	//! signal that soundlist is changed 
	void SoundListChanged() { m_soundListChanged = TRUE; }

	//! check the commit state 
	virtual BOOL CheckCommit();


	//! initialize Sound device and create listener 
	virtual BOOL Initialize(HWND mainWnd) = 0;

	//! terminate the sound device 
	virtual void Term()=0;

	//! release all sound device resources 
	virtual void Release();

	//! set the listener position and orientation
	virtual void SetViewpoint(const Point &position,const Vector &direction,const Vector &up,DWORD time)=0;

	//! play a single background type sound 
	virtual BOOL PlaySound(const char *fileName)=0;

	/*! create new sound emitter. 
	    Will be overwritten to return specialized soundEmitters
    */	 
	virtual GSoundEmitter* newSoundEmitter(int fileFmt= 0)=0;


/*
TO be done by implementers of a GSoundDevice 

	// emitters allocated 
	GSoundEmitter* m_emitters;


	// emitter for PlaySound
	GSoundEmitter* m_playEmitter;

	// add new emitter 
	virtual void Add(GSoundEmitter *emitter)=0;

	// remove emitter 
	virtual BOOL Remove(GSoundEmitter *emitter)=0;
*/

	//! remove (delete) all emitters 
	virtual void RemoveEmitters()=0;


	//! release all emitters 
	virtual void ReleaseEmitters()=0;

	//! release all emitters not in use (not rendered)
	virtual void DisableUnusedEmitters()=0;

	//! reset the emitter use flag 
	virtual void ResetEmittersUseFlag()=0;

	//! enable the n highest priority sounds only 
	// disable (mute) all others 
	virtual int PrioritizeSounds()=0;

	//! how often can SetViewpoint be called ?
	static DWORD maxUpdateIntervall;  
	
	//! default number of max sounds played
	static int defaultMaxSounds; 
	
	// the default CPU-Budget (RSX)
	static int defaultCpuBudget; 
};




//
// GSoundEmitter 
// the class representing a sound to be played 
//

class GSoundEmitter {

public:

    // This class has two modes of operation. 

    typedef enum {
        e_SimpleWav= 1,
        e_Streaming
    } mt_Mode;

    mt_Mode m_Mode;

    virtual BOOL CanDoStreaming()    { return FALSE; };
                                    // If a derived class implements all
                                    // functions, which are needed for streaming,
                                    // this must return TRUE. 

    
    //! position and direction of sound emitter 
	Point 					m_position,m_direction;
	float					m_pitch;

	//GSoundEmitter * m_next; // linked list 

	//! reference counting, device has counter on all sounds
	LONG m_refCnt;
    int ref() { 
        TRACE("SDHM: \n");
        TRACE("SDHM: >>> GSoundEmitter<%08x>::ref() \n", this);
        LONG ret= InterlockedIncrement(&m_refCnt); 
        TRACE("SDHM: m_refCnt<%d> (after increment) \n", m_refCnt);
        TRACE("SDHM: <<< GSoundEmitter<%08x>::ref() \n", this);
        TRACE("SDHM: \n");
    return ret;
    }

 void unref() {
        TRACE("SDHM: \n");
        TRACE("SDHM: >>> GSoundEmitter<%08x>::unref() \n", this);
        TRACE("SDHM: m_refCnt<%d> (before Decrementing) \n", m_refCnt);
        if (InterlockedDecrement(&m_refCnt) <= 0) 
        {
            TRACE("SDHM: >>> deleting this (GSoundEmitter)\n");
            delete this; 
        }
        TRACE("SDHM: <<< GSoundEmitter<%08x>::unref() \n", this);
        TRACE("SDHM: \n");
    }


	HRESULT m_result; //!< last  error or diagnostics code

	//! the sound should play ?
	BOOL m_enabled; 
	
	//! last time listener was enabled  (i.e. startTime)
	DWORD m_enabledTime; 


	//! is the sound enabled for playing
	BOOL IsEnabled() const { return m_enabled; }

    BOOL HasStreamingMode() const { return m_Mode == e_Streaming; };

	//! the sound is muted, due to priority computation
	BOOL m_muted;   


	//! enable / disable via mute 
	virtual void SetMuteState(BOOL flag)=0;


	//! the priority value from the VRML Sound node 
	float m_priority; 
	
	//! the intensity value from the VRML Sound node 
	float m_intensity; 

	//! get the priority sort value 
	virtual float getValue()=0; 


	//! flag set by traversal, if true this sound is beeing used by the VRML scene graph
	unsigned m_used;

    //! set the used flag
	void SetUsed() { m_used ++; }
	
	//! reset the use flag
	void SetUnused() { m_used =0; }

	//! has this sound be used recently  ?
	BOOL IsUsed() const { return (m_used>0); }


	GSoundEmitter();
	
	virtual ~GSoundEmitter();

	//! is soundEmitter properly setup ?
	virtual BOOL Ok()=0;

	//! enable / disable sound 
	virtual void Enable(BOOL flag)=0;

	/*! Sets/updates the emitter's model parameters 
		from the VRML Sound node
	*/		
	virtual void SetModel(
		float			fMinBack,
		float			fMinFront,
		float			fMaxBack,
		float			fMaxFront,
		float			fIntensity
		)=0;


	//! initialize a sound emitter given a sound filename
	virtual BOOL Initialize(GSoundDevice *device, const char *fileName, BOOL spatialize=FALSE)=0; // For simple playing a wav file.
    virtual BOOL Initialize(GSoundDevice *device, DWORD Frequency, DWORD Channels, DWORD BitsPerSample, double BufferLength, BOOL spatialize= FALSE);  // For streaming sound.      // BufferLength is the required Length of the buffer in seconds.
    // for implementors: the first Initialize(.) function must set m_Mode to e_SimpleWav, the second one to e_Streaming.


	//! query state of sound 
	virtual BOOL QueryInfo(BOOL &playing,float &secondsPlayed,float &totalSeconds, int &numLoops,float &level)=0;

	//! play sound 
	virtual void Play(BOOL loop=TRUE, float initialStartTime=0.0f)=0;
    // In e_Streaming mode, initialStartTime has no meaning.

	//! stop playing sound 
	virtual void Stop()=0;

	/* GetMarkPosition retrieves the begin and end playback mark. 
	   begin = 0 from begin end = 0 to end of file 
	   rsx only ?
	*/
	virtual void GetMarkPosition(float &beginTime, float &endTime)=0;
	
	//! Sets the begin and end playback mark. 
	//! rsx only ?
	virtual void SetMarkPosition(float beginTime=0.0f, float endTime=0.0f)=0;

	//! get the pitch factor 
	virtual void GetPitch(float &pitchFactor)=0;

	//! set the pitch factor 
	virtual void SetPitch(float pitchFactor=1.0f)=0;


	//! set 3D position of sound 
	virtual void SetPosition(const Point &position)=0;

	//!Specifies a 3D vector that points in the same direction as the emitter's orientation. 
	virtual void SetOrientation (const Point &direction)=0;

	//! release the sound 
	virtual void Release();
















///////// Buffer handling: ////////////////     (needed in streaming mode.) 

    // See the implementation, why these are no pure virtual functions:

public:     
  virtual DWORD WriteData(char *Data, DWORD Len); // Locks the Buffer, writes the data
                                                  // and unlocks it again.

  virtual DWORD ZeroWrite(DWORD Count); // Locks the Buffer, writes Count bytes of zero
                                        // and unlocks it again.
                           
  
  
  virtual DWORD getSaveCursor();  // returns the byte offset to the play
                                  //  cursor, from where it is save to write.
  virtual double getSaveCursorSec(); // the same as getSaveCursor(), but in seconds.


  virtual DWORD getWriteCursor();  // returns the byte offset to the play cursor,
                                   // where the next data will be written. This
                                   // is also a measurement how full the buffer is.
                                   // Since the Play Cursor constantly moves towards
                                   // the Write Cursor, this value may decrease
                                   // between subsequent calls.
  virtual double getWriteCursorSec(); // the same as getWriteCursor(), but in seconds.
                                      // This tells how long it will last, untill
                                      // Data which would have now been written to
                                      // the buffer will be actually played. (It doesn't
                                      // contain the delay in the primary DSound buffer
                                      // - if there is any, I don't know.)

  virtual DWORD getBufferLength(); // returns the size of the buffer in bytes.


  virtual double getBufferLengthSec(); // return the size of the buffer in Seconds.
                                       // This is what was passed to the constructor.

  virtual DWORD getBytesPerSec();

  virtual void DecreaseWriteCursor(int delta); // This decreases the WriteCursor by
                                               // the given amount of Bytes. This
                                               // could be used if some Data should
                                               // be rewritten to the buffer.
                                               // it is also possible to specify a
                                               // negative value for delta, which
                                               // will skip some data. But it could
                                               // be better to write Zeroes, or something
                                               // else. Another usage for negative delta
                                               // could be to jump back after rewriting
                                               // some earlier data. But this in not
                                               // necessary if you write as much data
                                               // as you jumped back.
                                               //
                                               // This only works for values smaller
                                               // than the size of the buffer.
  
                                               //Note: Lock() doesn't increase the WriteCursor, but UnLock() does.

                                   

// Data Members for buffer handling:

protected:
    // none in this base class.



};



#endif
