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
#ifndef _dsutil3d_h
#define _dsutil3d_h

/******************************************************************************
@doc

@module DSUtil3d.h - Direct Sound helper functions|

Copyright (c) 1998	by  blaxxun interactive - Holger Grahn 
All rights reserved

Purpose:

  reads WAV files into DirectSound buffers

Classes:


Notes:

  based on Direct X 5 SDK sample ds3dview / dsutil3d.c

Changes:

$Revision: 1.10 $
$Log: dsutil3d.h,v $
Revision 1.10  1999/03/10 10:22:03  tom
*** empty log message ***

Revision 1.3  1998/10/22 16:32:47  holger
empty message



Todo :

   
******************************************************************************/

#include <mmsystem.h>
#include <dsound.h>

// WAAAAAAAAARNING> DX7 makes datastructures bigger,
// sound then fails on older DX systems 

//TBD: There are problems with the checking dwSize in DX 6.0 when the headers define
//     DIRECTSOUND_VERSION as 0x0700 which creates an extra field in the structure. 
//     Maybe it is a better solution to define DIRECTSOUND_VERSION before including
//     dsound.h if DX 700 is not wanted.....

typedef struct _DSBUFFERDESC5
{
    DWORD           dwSize;
    DWORD           dwFlags;
    DWORD           dwBufferBytes;
    DWORD           dwReserved;
    LPWAVEFORMATEX  lpwfxFormat;
#if DIRECTSOUND_VERSION >= 0x0700
//    GUID            guid3DAlgorithm;
#endif
} DSBUFFERDESC5, *LPDSBUFFERDESC5;




/*==========================================================================
 *
 *  Copyright (C) 1995-1997 Microsoft Corporation. All Rights Reserved.
 *
 *  File:       dsutil.cpp
 *  Content:    Routines for dealing with sounds from resources
 *
 *
 ***************************************************************************/










///////////////////////////////////////////////////////////////////////////////
//
// Two functions which are usefull for error handling.
// 
//  TranslateDSError   translates direct sound error numbers to verbose text
//                     strings.
//                      
//  DS3D_SUCCEED       traces translated error messages, and returns a bool
//                     which tells if there was an error.
//
//////////////////////////////////////////////////////////////////////////////



LPCSTR TranslateDSError( HRESULT hr);
BOOL DS3D_SUCCEED(HRESULT result);


///////////////////////////////////////////////////////////////////////////////
//
// These Load functions will first look in the Win32 resources, and then they
//   will attempt to find the given name as a file on disk.
//
// DSLoadSoundBuffer    Loads an IDirectSoundBuffer from a Win32 resource in
//                      the current application, or alternately from disk.
//
// DSLoad3DSoundBuffer  Loads an IDirectSoundBuffer from a Win32 resource in
//                      the current application, and asks for DSBCAPS_CTRL3D.
//
// Params:
//  pDS         -- Pointer to an IDirectSound that will be used to create
//                 the buffer.
//
//  lpName      -- Name of WAV resource to load the data from.  Can be a
//                 resource id specified using the MAKEINTRESOURCE macro.
//
// pBufferSize  -- if this is not NULL, the size of the buffer in bytes
//                 is written to that location.
//
// Returns an IDirectSoundBuffer containing the wave data or NULL on error.
//
// example:
//  in the application's resource script (.RC file)
//      Turtle WAV turtle.wav
//
//  some code in the application:
//      IDirectSoundBuffer *pDSB = DSLoadSoundBuffer(pDS, "Turtle");
//
//      if (pDSB)
//      {
//          IDirectSoundBuffer_Play(pDSB, 0, 0, DSBPLAY_TOEND);
//          /* ... */
//
//  or you can substitute the first line above as:
//     IDirectSoundBuffer *pDSB = DSLoadSoundBuffer(pDS, "turtle.wav");
//
///////////////////////////////////////////////////////////////////////////////
IDirectSoundBuffer *DSLoadSoundBuffer(IDirectSound *pDS, LPCTSTR lpName,DWORD *pBufferSize,HRESULT &result);
IDirectSoundBuffer *DSLoad3DSoundBuffer(IDirectSound *pDS, LPCTSTR lpName,DWORD *pBufferSize,HRESULT &result);



///////////////////////////////////////////////////////////////////////////////
//
// These Setuup functions will set up a direct sound buffer with the given
// sound format parameters, but will noht initialize the data in the buffer.
//
// DSSetupSoundBuffer    creates an IDirectSoundBuffer and optionally asks
//                       for DSBCAPS_CTRL3D.
//                       falls back for non 3D if 3D is not available.
//                       WARNING: DSBCAPS_CTRL3D is not implemented now.
//
// Params:         
//  pDS           -- Pointer to an IDirectSound that will be used to create
//                   the buffer.
//
//  Frequency,    -- PCM parameters to be used.
//  Channels,  
//  BitsPerSample,
//  
//  BufferLength, -- length of the buffer to create in seconds. (It's
//                   a floating point parameter.)
//
//  pBufferSize   -- if this is not NULL, the size of the buffer in bytes
//                   is written to that location.
//
//  AskFor3D      -- if true, asks for DSBCAPS_CTRL3D.
//                   WARNING: is not implemented now.
//
// Returns an IDirectSoundBuffer containing the wave data or NULL on error.
//
// example:
//    DSSetupSoundBuffer(pDS, 44100, 2, 16, 5, &BufferLength);
//
//    this will set up a buffer for five seconds of CD quality sound.
//
///////////////////////////////////////////////////////////////////////////////
IDirectSoundBuffer *DSSetupSoundBuffer( IDirectSound *pDS,
                                        DWORD Frequency, DWORD Channels, DWORD BitsPerSample, double BufferLength,
                                        DWORD *pBufferSize= NULL,
                                        BOOL  AskFor3D= FALSE
                                      );


///////////////////////////////////////////////////////////////////////////////
//
// DSReloadSoundBuffer  Reloads an IDirectSoundBuffer from a Win32 resource in
//                      the current application. normally used to handle
//                      a DSERR_BUFFERLOST error. Will also attempt to find
//                      the given name as a file, if there is no resource.
// Params:
//  pDSB        -- Pointer to an IDirectSoundBuffer to be reloaded.
//
//  lpName      -- Name of WAV resource to load the data from.  Can be a
//                 resource id specified using the MAKEINTRESOURCE macro.
//
// Returns a BOOL indicating whether the buffer was successfully reloaded.
//
// example:
//  in the application's resource script (.RC file)
//      Turtle WAV turtle.wav
//
//  some code in the application:
//  TryAgain:
//      HRESULT hres = IDirectSoundBuffer_Play(pDSB, 0, 0, DSBPLAY_TOEND);
//
//      if (FAILED(hres))
//      {
//          if ((hres == DSERR_BUFFERLOST) &&
//              DSReloadSoundBuffer(pDSB, "Turtle"))
//          {
//              goto TryAgain;
//          }
//          /* deal with other errors... */
//      }
//
///////////////////////////////////////////////////////////////////////////////
BOOL DSReloadSoundBuffer(IDirectSoundBuffer *pDSB, LPCTSTR lpName,HRESULT &result);

///////////////////////////////////////////////////////////////////////////////
//
// DSGetWaveResource    Finds a WAV resource in a Win32 module.
//
// Params:
//  hModule     -- Win32 module handle of module containing WAV resource.
//                 Pass NULL to indicate current application.
//
//  lpName      -- Name of WAV resource to load the data from.  Can be a
//                 resource id specified using the MAKEINTRESOURCE macro.
//
//  ppWaveHeader-- Optional pointer to WAVEFORMATEX * to receive a pointer to
//                 the WAVEFORMATEX header in the specified WAV resource.
//                 Pass NULL if not required.
//
//  ppbWaveData -- Optional pointer to BYTE * to receive a pointer to the
//                 waveform data in the specified WAV resource.  Pass NULL if
//                 not required.
//
//  pdwWaveSize -- Optional pointer to DWORD to receive the size of the
//                 waveform data in the specified WAV resource.  Pass NULL if
//                 not required.
//
// Returns a BOOL indicating whether a valid WAV resource was found.
//
///////////////////////////////////////////////////////////////////////////////
BOOL DSGetWaveResource(HMODULE hModule, LPCTSTR lpName,
    WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pdwWaveSize);

///////////////////////////////////////////////////////////////////////////////
//
// DSGetWaveFile    Finds a WAV file on disk and uses a memory mapped file to
//                    get a memory image of the file.
//
// Params:
//  hModule     -- Win32 module handle of module containing WAV resource.
//                 Pass NULL to indicate current application.
//
//  lpName      -- Name of WAV resource to load the data from.  Can be a
//                 resource id specified using the MAKEINTRESOURCE macro.
//
//  ppWaveHeader-- Optional pointer to WAVEFORMATEX * to receive a pointer to
//                 the WAVEFORMATEX header in the specified WAV resource.
//                 Pass NULL if not required.
//
//  ppbWaveData -- Optional pointer to BYTE * to receive a pointer to the
//                 waveform data in the specified WAV resource.  Pass NULL if
//                 not required.
//
//  pdwWaveSize -- Optional pointer to DWORD to receive the size of the
//                 waveform data in the specified WAV resource.  Pass NULL if
//                 not required.
//
//  ppvBase     -- Required out-parameter which receives the base-address of
//                 the file mapping.  This is needed for the caller to Unmap
//                 the file.
//  result		-- result code

// Returns a BOOL indicating whether a valid WAV file was found and mapped.
// The caller is responsible for calling UnmapViewOfFile(*ppvBase) when they
// are done with the "memory" for the file.
//
///////////////////////////////////////////////////////////////////////////////
BOOL DSGetWaveFile(HMODULE hModule, LPCTSTR lpName, WAVEFORMATEX **ppWaveHeader,
     BYTE **ppbWaveData, DWORD *pdwWaveSize, void **ppvBase,HRESULT &result);

///////////////////////////////////////////////////////////////////////////////
//
// HSNDOBJ             Handle to a SNDOBJ object.
//
//  SNDOBJs are implemented in dsutil as an example layer built on top
//      of DirectSound.
//
//      A SNDOBJ is generally used to manage individual
//      sounds which need to be played multiple times concurrently.  A
//      SNDOBJ represents a queue of IDirectSoundBuffer objects which
//      all refer to the same buffer memory.
//
//      A SNDOBJ also automatically reloads the sound resource when
//      DirectSound returns a DSERR_BUFFERLOST
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _HSNDOBJ_DEFINED

#define _HSNDOBJ_DEFINED
typedef struct
{
    BYTE *pbWaveData;               // pointer into wave resource (for restore)
    DWORD cbWaveSize;               // size of wave data (for restore)
    int iAlloc;                     // number of buffers.
    int iCurrent;                   // current buffer
    IDirectSoundBuffer* Buffers[1]; // list of buffers

} SNDOBJ, *HSNDOBJ;


//DECLARE_HANDLE32(HSNDOBJ);
#endif

///////////////////////////////////////////////////////////////////////////////
//
// SndObjCreate     Loads a SNDOBJ from a Win32 resource in
//		    the current application.
//
// Params:
//  pDS         -- Pointer to an IDirectSound that will be used to create
//                 the SNDOBJ.
//
//  lpName      -- Name of WAV resource to load the data from.  Can be a
//                 resource id specified using the MAKEINTRESOURCE macro.
//
//  iConcurrent -- Integer representing the number of concurrent playbacks of
//                 to plan for.  Attempts to play more than this number will
//                 succeed but will restart the least recently played buffer
//                 even if it is not finished playing yet.
//
// Returns an HSNDOBJ or NULL on error.
//
// NOTES:
//      SNDOBJs automatically restore and reload themselves as required.
//
///////////////////////////////////////////////////////////////////////////////
HSNDOBJ SndObjCreate(IDirectSound *pDS, LPCTSTR lpName, int iConcurrent);

///////////////////////////////////////////////////////////////////////////////
//
// SndObjDestroy  Frees a SNDOBJ and releases all of its buffers.
//
// Params:
//  hSO         -- Handle to a SNDOBJ to free.
//
///////////////////////////////////////////////////////////////////////////////
void SndObjDestroy(HSNDOBJ hSO);

///////////////////////////////////////////////////////////////////////////////
//
// SndObjPlay	Plays a buffer in a SNDOBJ.
//
// Params:
//  hSO         -- Handle to a SNDOBJ to play a buffer from.
//
//  dwPlayFlags -- Flags to pass to IDirectSoundBuffer::Play.  It is not
//                 legal to play an SndObj which has more than one buffer
//                 with the DSBPLAY_LOOPING flag.  Pass 0 to stop playback.
//
///////////////////////////////////////////////////////////////////////////////
BOOL SndObjPlay(HSNDOBJ hSO, DWORD dwPlayFlags);

///////////////////////////////////////////////////////////////////////////////
//
// SndObjStop	Stops one or more buffers in a SNDOBJ.
//
// Params:
//  hSO         -- Handle to a SNDOBJ to play a buffer from.
//
///////////////////////////////////////////////////////////////////////////////
BOOL SndObjStop(HSNDOBJ hSO);

///////////////////////////////////////////////////////////////////////////////
//
// SndObjGetFreeBuffer	    returns one of the cloned buffers that is
//			    not currently playing
//
// Params:
//  hSO 	-- Handle to a SNDOBJ
//
// NOTES:
//  This function is provided so that callers can set things like pan etc
//  before playing the buffer.
//
// EXAMPLE:
//  ...
//
///////////////////////////////////////////////////////////////////////////////
IDirectSoundBuffer *SndObjGetFreeBuffer(HSNDOBJ hSO);

///////////////////////////////////////////////////////////////////////////////
//
// helper routines
//
///////////////////////////////////////////////////////////////////////////////

BOOL DSFillSoundBuffer(IDirectSoundBuffer *pDSB, BYTE *pbWaveData, DWORD dwWaveSize);

BOOL DSParseWaveResource(void *pvRes, WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData,DWORD *pcbWaveSize,HRESULT &result);


#endif
