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

@module DSUtil3d.cpp - Direct Sound helper functions|

Copyright (c) 1998	by  blaxxun interactive - Holger Grahn 
All rights reserved

Purpose:

  reads WAV files into DirectSound buffers

Classes:


Notes:

  based on Direct X 5 SDK sample ds3dview / dsutil3d.c

Changes:

$Revision: 1.24 $
$Log: dsutil3d.cpp,v $
Revision 1.24  1999/03/10 10:21:30  tom
*** empty log message ***

Revision 1.17  1998/10/22 16:32:47  holger
empty message



Todo :

  support for compressed wav files

   
******************************************************************************/


/*==========================================================================
 *
 *  Copyright (C) 1995-1997 Microsoft Corporation. All Rights Reserved.
 *
 *  File:       dsutil3d.c
 *  Content:    Routines for dealing with sounds from resources -- this
 *                version has been modified and extended somewhat from the
 *                version found in the SAMPLES\MISC directory.
 *
 *
 ***************************************************************************/

//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
//#include <windowsx.h>

#include "greporter.h"

#include <mmsystem.h>
#include <dsound.h>

#include "dsutil3d.h"

#include "gsound.h"




static const char c_szWAV[] = "WAV";




// Silly little function gives meaningful error messages from HRESULT's
LPCSTR TranslateDSError( HRESULT hr )
{
    switch( hr )
	{
	case DSERR_ALLOCATED:
	    return "DSERR_ALLOCATED (The request failed because resources, such as a priority level, were already in use by another caller.)";

    case DSERR_CONTROLUNAVAIL:
	    return "DSERR_CONTROLUNAVAIL (The control (volume, pan, and so forth) requested by the caller is not available. )";

	case DSERR_INVALIDPARAM:
	    return "DSERR_INVALIDPARAM (An invalid parameter was passed to the returning function.)";

	case DSERR_INVALIDCALL:
	    return "DSERR_INVALIDCALL (This function is not valid for the current state of this object.)";

	case DSERR_GENERIC:
	    return "DSERR_GENERIC (An undetermined error occurred inside the DirectSound subsystem.)";

	case DSERR_PRIOLEVELNEEDED:
	    return "DSERR_PRIOLEVELNEEDED (The caller does not have the priority level required for the function to succeed.)";

	case DSERR_OUTOFMEMORY:
	    return "DSERR_OUTOFMEMORY (The DirectSound subsystem could not allocate sufficient memory to complete the caller's request.)";

	case DSERR_BADFORMAT:
	    return "DSERR_BADFORMAT (The specified wave format is not supported.)";

	case DSERR_UNSUPPORTED:
	    return "DSERR_UNSUPPORTED (The function called is not supported at this time.)";

	case DSERR_NODRIVER:
	    return "DSERR_NODRIVER (No sound driver is available for use.)";

	case DSERR_ALREADYINITIALIZED:
	    return "DSERR_ALREADYINITIALIZED (The object is already initialized.)";

	case DSERR_NOAGGREGATION:
	    return "DSERR_NOAGGREGATION (The object does not support aggregation.)";

	case DSERR_BUFFERLOST:
	    return "DSERR_BUFFERLOST (The buffer memory has been lost and must be restored.)";

	case DSERR_OTHERAPPHASPRIO:
	    return "DSERR_OTHERAPPHASPRIO (This value is obsolete and is not used.)";

	case DSERR_UNINITIALIZED:
	    return "DSERR_UNINITIALIZED (The IDirectSound::Initialize method has not been called or has not been called successfully before other methods were called.)";

	default:
	    return "Unknown HRESULT (Look into the Documentation)";
	}
}




BOOL DS3D_SUCCEED(HRESULT result)
{
        if (SUCCEEDED(result))
return TRUE;
        // Could be 0 for no message, 1 for non-critical (simply needs to report it), or 2 for critical (notify and quit)

        TRACE("DS3D_SUCCEED: ERROR %08x: %s \n", result, TranslateDSError(result));

return FALSE;
}











///////////////////////////////////////////////////////////////////////////////
//
// DSLoadSoundBuffer - Loads a .WAV into a sound buffer and returns the sound buffer
//
///////////////////////////////////////////////////////////////////////////////

BOOL DSVeryifySoundFormat(DSBUFFERDESC *dsBD,LPCTSTR lpName,HRESULT &result) 
{

	///* Write the PCMWAVEFORMAT structure to the 'fmt ' chunk if its that type. */
    //if (pwfxDest->wFormatTag == WAVE_FORMAT_PCM)
	
    	TRACE("Format %d  Bits %d  Channels %d Samps Per sec %d Bytes =%d \n ", 
		dsBD->lpwfxFormat->wFormatTag,dsBD->lpwfxFormat->wBitsPerSample,
		dsBD->lpwfxFormat->nChannels,dsBD->lpwfxFormat->nSamplesPerSec,		
		dsBD->dwBufferBytes);

	//	if (m_wfx.wBitsPerSample == 0 || m_wfx.nSamplesPerSec ==0 || (m_wfx.nChannels==0 )) {


	if (dsBD->lpwfxFormat->wFormatTag != WAVE_FORMAT_PCM) {

		TRACE("Wave file must be PCM format for Direct Sound %s\n",lpName);
		//Reporter.Warning("Wave file must be PCM format for Direct Sound (%s)",lpName);
		result = GS_NOT_PCM_FORMAT;

		return FALSE;
	}

	return TRUE;
}

	
IDirectSoundBuffer *DSLoadSoundBuffer(IDirectSound *pDS, LPCTSTR lpName,DWORD *pBufferSize,HRESULT &result)
{
    IDirectSoundBuffer *pDSB = NULL;
    DSBUFFERDESC dsBD = {0};
    BYTE *pbWaveData;
    void *pvBase;
	HRESULT res=0;

    dsBD.dwSize = sizeof(DSBUFFERDESC5);
    dsBD.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY
                        | DSBCAPS_LOCSOFTWARE
						| DSBCAPS_GETCURRENTPOSITION2;

    if (DSGetWaveResource(NULL, lpName, &dsBD.lpwfxFormat, &pbWaveData, &dsBD.dwBufferBytes))
    {
        if ( DSVeryifySoundFormat(&dsBD,lpName,result) && 
			SUCCEEDED(pDS->CreateSoundBuffer(&dsBD, &pDSB, NULL)))
        {
            if (!DSFillSoundBuffer(pDSB, pbWaveData, dsBD.dwBufferBytes))
            {
                pDSB->Release();
                pDSB = NULL;
            }
        }
        else
        {
            pDSB = NULL;
        }
    } else if (DSGetWaveFile(NULL, lpName, &dsBD.lpwfxFormat, &pbWaveData,
                        &dsBD.dwBufferBytes, &pvBase,result))
    {
 	        
		if (DSVeryifySoundFormat(&dsBD,lpName,result) && SUCCEEDED(res=pDS->CreateSoundBuffer(&dsBD, &pDSB, NULL)))
        {
            if (!DSFillSoundBuffer(pDSB, pbWaveData, dsBD.dwBufferBytes))
            {
                pDSB->Release();
                pDSB = NULL;
            }
        }
        else
        {
            pDSB = NULL;
			result = res;
        }
		UnmapViewOfFile (pvBase);
	}

	if(pBufferSize) 
        *pBufferSize = dsBD.dwBufferBytes;
    return pDSB;
}



///////////////////////////////////////////////////////////////////////////////
//
// DSLoad3DSoundBuffer - Loads a .WAV into a 3D sound buffer and returns the sound buffer
//
///////////////////////////////////////////////////////////////////////////////

IDirectSoundBuffer *DSLoad3DSoundBuffer(IDirectSound *pDS, LPCTSTR lpName,DWORD *pBufferSize,HRESULT &result)
{
    
	HRESULT res;
	IDirectSoundBuffer *pDSB = NULL;
    DSBUFFERDESC dsBD = {0};
    BYTE *pbWaveData;
    void *pvBase;

    dsBD.dwSize = sizeof(DSBUFFERDESC5);
/*
    dsBD.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLVOLUME
                        | DSBCAPS_CTRLFREQUENCY | DSBCAPS_LOCSOFTWARE
                        | DSBCAPS_STICKYFOCUS;

    
	dsBD.dwFlags |=  DSBCAPS_CTRL3D | DSBCAPS_MUTE3DATMAXDISTANCE;
*/
	dsBD.dwFlags = DSBCAPS_STATIC     | DSBCAPS_GETCURRENTPOSITION2  | DSBCAPS_CTRL3D
		| DSBCAPS_MUTE3DATMAXDISTANCE |  DSBCAPS_CTRLVOLUME 
		| DSBCAPS_CTRLFREQUENCY;	// new 08.10.98 hg 

    if (DSGetWaveResource(NULL, lpName, &dsBD.lpwfxFormat, &pbWaveData, &dsBD.dwBufferBytes))
    {
        if (DSVeryifySoundFormat(&dsBD,lpName,result) && SUCCEEDED(result=pDS->CreateSoundBuffer(&dsBD, &pDSB, NULL)))
        {
            if (!DSFillSoundBuffer(pDSB, pbWaveData, dsBD.dwBufferBytes))
            {
                TRACE("Filling of sound buffer failed \n");

				pDSB->Release();
                pDSB = NULL;
            }
        }
        else
        {
            pDSB = NULL;
        }
    } else if (DSGetWaveFile(NULL, lpName, &dsBD.lpwfxFormat, &pbWaveData,
                        &dsBD.dwBufferBytes, &pvBase,result))
    {
		
		if (DSVeryifySoundFormat(&dsBD,lpName,result)) {

			res = pDS->CreateSoundBuffer(&dsBD, &pDSB, NULL);

			if (!SUCCEEDED(res)) { 

				// on NT go 0x57 INVALID_ARG
						TRACE("DSUTIL3D: 3D Sound buffer init failed %x  %s \n",res,lpName);

				// retry without 3D 
				dsBD.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY
	                    | DSBCAPS_LOCSOFTWARE
						| DSBCAPS_GETCURRENTPOSITION2;
			
				res = pDS->CreateSoundBuffer(&dsBD, &pDSB, NULL);

			}
			result = res;
		} else res = S_FALSE; // S_FALSE dosnot yield (!SUCCEEDED(res))


        if (pDSB && SUCCEEDED(res))
        {
            if (!DSFillSoundBuffer(pDSB, pbWaveData, dsBD.dwBufferBytes))
            {
                TRACE("Filling of sound buffer failed \n");
			    TRACE("DSUTIL3D: Filling of 3D sound buffer failed  %s \n",lpName);
                pDSB->Release();
                pDSB = NULL;
            }
        }
        else
        {
            pDSB = NULL;
        }
	UnmapViewOfFile (pvBase);
    }

	if(pBufferSize) 
        *pBufferSize = dsBD.dwBufferBytes;

    return pDSB;
}




///////////////////////////////////////////////////////////////////////////////
//
// DSSetupSoundBuffer
//
///////////////////////////////////////////////////////////////////////////////

IDirectSoundBuffer *DSSetupSoundBuffer( IDirectSound *pDS,
                                        DWORD Frequency, DWORD Channels, DWORD BitsPerSample, double BufferLength,
                                        DWORD *pBufferSize,
                                        BOOL  AskFor3D
                                      )
{
    IDirectSoundBuffer *pDSB = NULL;

    if(AskFor3D)
    {
        TRACE("WARNING: Fallback if 3D not available is not implementd in DSSetupSoundBuffer(.) .\n");
        //TBD: Implement this.
    }

    WAVEFORMATEX wfx= {0};
    
    wfx.wFormatTag=      WAVE_FORMAT_PCM;
    wfx.nChannels=       Channels;
    wfx.wBitsPerSample=  BitsPerSample;
    wfx.nSamplesPerSec=  Frequency;
    wfx.nBlockAlign=     wfx.nChannels * wfx.wBitsPerSample / 8;
    wfx.nAvgBytesPerSec= wfx.nSamplesPerSec * wfx.nBlockAlign;
    wfx.cbSize=          0;
    
    
    DSBUFFERDESC dsbd= {0};

    dsbd.dwSize=      sizeof(DSBUFFERDESC5);
    dsbd.lpwfxFormat= &wfx;
    dsbd.dwFlags=     DSBCAPS_CTRLVOLUME                  
	                | DSBCAPS_GETCURRENTPOSITION2
                    | DSBCAPS_GLOBALFOCUS
                    | (AskFor3D? DSBCAPS_CTRL3D : 0);    // TBD: This Flag may not be all necessary for 3D.
    dsbd.dwBufferBytes= (DWORD)(BufferLength * wfx.nAvgBytesPerSec + 0.5);


    HRESULT dsr;

    if(!DS3D_SUCCEED(dsr= pDS->CreateSoundBuffer(&dsbd, &pDSB, NULL)))
    {
        TRACE("DSSetupSoundBuffer: Could not create a direct sound buffer. \n");
return NULL;
    }

	if(pBufferSize) 
        *pBufferSize = dsbd.dwBufferBytes;

return pDSB;
}











///////////////////////////////////////////////////////////////////////////////
//
// DSReloadSoundBuffer
//
///////////////////////////////////////////////////////////////////////////////

BOOL DSReloadSoundBuffer(IDirectSoundBuffer *pDSB, LPCTSTR lpName, HRESULT &result)
{
    BOOL ret=FALSE;
    BYTE *pbWaveData;
    DWORD cbWaveSize;
    void *pvBase;

	if (!pDSB) return FALSE;

    if (DSGetWaveResource(NULL, lpName, NULL, &pbWaveData, &cbWaveSize))
    {
        if (SUCCEEDED(result=pDSB->Restore()) &&
            DSFillSoundBuffer(pDSB, pbWaveData, cbWaveSize))
		{
			ret = TRUE;
		}
    } 
	else if( DSGetWaveFile(NULL, lpName, NULL, &pbWaveData, &cbWaveSize, &pvBase,result))
    {
        if (SUCCEEDED(result=pDSB->Restore()) &&
            DSFillSoundBuffer(pDSB, pbWaveData, cbWaveSize))
		{
			ret = TRUE;
        }
        UnmapViewOfFile (pvBase);
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
//
// DSGetWaveResource
//
///////////////////////////////////////////////////////////////////////////////
BOOL DSGetWaveResource(HMODULE hModule, LPCTSTR lpName,
    WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pcbWaveSize)
{
    HRSRC hResInfo;
    HGLOBAL hResData;
    void * pvRes;
	HRESULT result;
    if (((hResInfo = FindResource(hModule, lpName, c_szWAV)) != NULL) &&
        ((hResData = LoadResource(hModule, hResInfo)) != NULL) &&
        ((pvRes = LockResource(hResData)) != NULL) &&
        DSParseWaveResource(pvRes, ppWaveHeader, ppbWaveData, pcbWaveSize,result))
    {
        return TRUE;
    }

    return FALSE;

}


///////////////////////////////////////////////////////////////////////////////
//
// DSGetWaveFile
//
///////////////////////////////////////////////////////////////////////////////
BOOL DSGetWaveFile(HMODULE hModule, LPCTSTR lpName,
    WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pcbWaveSize,
	void **ppvBase,HRESULT &result)
{
    void *pvRes;
    HANDLE hFile, hMapping;
	
	DWORD dwSizeLow,dwSizeHigh;
	DWORD dwError;

    *ppvBase = NULL;

    hFile = CreateFile (lpName, GENERIC_READ, FILE_SHARE_READ, 
	                    NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
		result = GS_FILE_ERROR;
		return FALSE;
	}
 
	dwSizeLow = GetFileSize (hFile, &dwSizeHigh) ; 
 
	// If we failed ... 
	if (dwSizeLow == 0xFFFFFFFF  && 
    (dwError = GetLastError()) != NO_ERROR ) { 
		result = GS_FILE_ERROR;
 
    } // End of error handler. 
 

    hMapping = CreateFileMapping (hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (hMapping == INVALID_HANDLE_VALUE)
    {
		result = GS_FILE_ERROR;
        CloseHandle (hFile); 
        return FALSE;
    }

    CloseHandle (hFile);

    pvRes = MapViewOfFile (hMapping, FILE_MAP_READ, 0, 0, 0);
    
	if (pvRes == NULL)
    {
		result = GS_FILE_ERROR;
        CloseHandle (hMapping);
        return FALSE;
    }

    CloseHandle (hMapping);

    if (DSParseWaveResource(pvRes, ppWaveHeader, ppbWaveData, pcbWaveSize,result) == FALSE)
    {
        UnmapViewOfFile (pvRes);
        return FALSE;
    }

	// hg do a size check and truncate buffer
	if ( ( (*ppbWaveData) + *pcbWaveSize) > (((BYTE *)pvRes)+dwSizeLow)) {

			TRACE ("Wave File %s to small %d %d \n",lpName, dwSizeLow,*pcbWaveSize);
			*pcbWaveSize = (((BYTE *)pvRes)+dwSizeLow) - *ppbWaveData;
			// to do align ?

			//Reporter.Warning("Wave is truncated (%s)",lpName);
			result = GS_FILE_TRUNCATED;

	}

    *ppvBase = pvRes;
    return TRUE;
}







///////////////////////////////////////////////////////////////////////////////
// SndObj fns
///////////////////////////////////////////////////////////////////////////////

SNDOBJ *SndObjCreate(IDirectSound *pDS, LPCTSTR lpName, int iConcurrent)
{
    SNDOBJ *pSO = NULL;
    LPWAVEFORMATEX pWaveHeader;
    BOOL fFound = FALSE;
    BYTE *pbData;
    DWORD cbData;
    void * pvBase;
	HRESULT result;

    if (DSGetWaveResource(NULL, lpName, &pWaveHeader, &pbData, &cbData))
        fFound = TRUE;

    if (DSGetWaveFile(NULL, lpName, &pWaveHeader, &pbData, &cbData, &pvBase,result))
    {
        fFound = TRUE;
        UnmapViewOfFile( pvBase );
    }
    if( fFound )
    {
        if (iConcurrent < 1)
            iConcurrent = 1;

        if ((pSO = (SNDOBJ *)LocalAlloc(LPTR, sizeof(SNDOBJ) +
	    (iConcurrent-1) * sizeof(IDirectSoundBuffer *))) != NULL)
        {
            int i;
			DWORD bufferSize;

            pSO->iAlloc = iConcurrent;
            pSO->pbWaveData = pbData;
            pSO->cbWaveSize = cbData;
            pSO->Buffers[0] = DSLoadSoundBuffer(pDS, lpName,&bufferSize,result);

	    for (i=1; i<pSO->iAlloc; i++)
	    {
		if (FAILED(pDS->DuplicateSoundBuffer(pSO->Buffers[0], &pSO->Buffers[i])))
                {
			DWORD bufferSize;
		    pSO->Buffers[i] = DSLoadSoundBuffer(pDS, lpName,&bufferSize,result);
		    if (!pSO->Buffers[i]) {
			SndObjDestroy(pSO);
			pSO = NULL;
			break;
		    }
		}
            }
        }
    }

    return pSO;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void SndObjDestroy(SNDOBJ *pSO)
{
    if (pSO)
    {
        int i;

        for (i=0; i<pSO->iAlloc; i++)
        {
            if (pSO->Buffers[i])
            {
                pSO->Buffers[i]->Release();
                pSO->Buffers[i] = NULL;
            }
        }
        LocalFree((HANDLE)pSO);
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

IDirectSoundBuffer *SndObjGetFreeBuffer(SNDOBJ *pSO)
{
    IDirectSoundBuffer *pDSB;

    if (pSO == NULL)
        return NULL;

    if (pDSB = pSO->Buffers[pSO->iCurrent])
    {
        HRESULT hres;
        DWORD dwStatus;

        hres = pDSB->GetStatus(&dwStatus);

        if (FAILED(hres))
            dwStatus = 0;

        if ((dwStatus & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING)
        {
            if (pSO->iAlloc > 1)
            {
                if (++pSO->iCurrent >= pSO->iAlloc)
                    pSO->iCurrent = 0;

                pDSB = pSO->Buffers[pSO->iCurrent];
                hres = pDSB->GetStatus(&dwStatus);

                if (SUCCEEDED(hres) && (dwStatus & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING)
                {
                    pDSB->Stop();
                    pDSB->SetCurrentPosition(0);
                }
            }
            else
            {
                pDSB = NULL;
            }
        }

        if (pDSB && (dwStatus & DSBSTATUS_BUFFERLOST))
        {
            if (FAILED(pDSB->Restore()) ||
                !DSFillSoundBuffer(pDSB, pSO->pbWaveData, pSO->cbWaveSize))
            {
                pDSB = NULL;
            }
        }
    }

    return pDSB;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BOOL SndObjPlay(SNDOBJ *pSO, DWORD dwPlayFlags)
{
    BOOL result = FALSE;

    if (pSO == NULL)
        return FALSE;

    if ((!(dwPlayFlags & DSBPLAY_LOOPING) || (pSO->iAlloc == 1)))
    {
	IDirectSoundBuffer *pDSB = SndObjGetFreeBuffer(pSO);
	if (pDSB != NULL) {
	    result = SUCCEEDED(pDSB->Play(0, 0, dwPlayFlags));
	}
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BOOL SndObjStop(SNDOBJ *pSO)
{
    int i;

    if (pSO == NULL)
        return FALSE;

    for (i=0; i<pSO->iAlloc; i++)
    {
        pSO->Buffers[i]->Stop();
        pSO->Buffers[i]->SetCurrentPosition(0);
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BOOL DSFillSoundBuffer(IDirectSoundBuffer *pDSB, BYTE *pbWaveData, DWORD cbWaveSize)
{
    if (pDSB && pbWaveData && (cbWaveSize>0))
    {
        LPVOID pMem1, pMem2;
        DWORD dwSize1, dwSize2;

        if (SUCCEEDED(pDSB->Lock(0, cbWaveSize,
            &pMem1, &dwSize1, &pMem2, &dwSize2, 0)))
        {
            if (pMem1 && dwSize1>0 )
				CopyMemory(pMem1, pbWaveData, dwSize1);

            if ( pMem2 && dwSize2>0 )
                CopyMemory(pMem2, pbWaveData+dwSize1, dwSize2);

            pDSB->Unlock(pMem1, dwSize1, pMem2, dwSize2);
            return TRUE;
        }
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BOOL DSParseWaveResource(void *pvRes, WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData,DWORD *pcbWaveSize,HRESULT &result)
{
    DWORD *pdw;
    DWORD *pdwEnd;
    DWORD dwRiff;
    DWORD dwType;
    DWORD dwLength;

    if (ppWaveHeader)
        *ppWaveHeader = NULL;

    if (ppbWaveData)
        *ppbWaveData = NULL;

    if (pcbWaveSize)
        *pcbWaveSize = 0;

    pdw = (DWORD *)pvRes;
    dwRiff = *pdw++;
    dwLength = *pdw++;
    dwType = *pdw++;

	TRACE("Parsing Wave riff %d  length %d type %d \n",dwRiff,dwLength,dwType);


    if (dwRiff != mmioFOURCC('R', 'I', 'F', 'F')) {
		result = GS_FILE_BAD;
        goto exit;      // not even RIFF
	}
    if (dwType != mmioFOURCC('W', 'A', 'V', 'E')) {
		result = GS_FILE_BAD;
        goto exit;      // not a WAV
	}

    pdwEnd = (DWORD *)((BYTE *)pdw + dwLength-4);

    while (pdw < pdwEnd)
    {
        dwType = *pdw++;
        dwLength = *pdw++;

        switch (dwType)
        {
        case mmioFOURCC('f', 'm', 't', ' '):
            if (ppWaveHeader && !*ppWaveHeader)
            {
                if (dwLength < sizeof(WAVEFORMAT))
                    goto exit;      // not a WAV

                *ppWaveHeader = (WAVEFORMATEX *)pdw;
				
				//*ppWaveHeader->cbSize = 0; // hg ??  
                
				if ((!ppbWaveData || *ppbWaveData) &&
                    (!pcbWaveSize || *pcbWaveSize))
                {
                    return TRUE;
                }
            }
            break;

        case mmioFOURCC('d', 'a', 't', 'a'):
            if ((ppbWaveData && !*ppbWaveData) ||
                (pcbWaveSize && !*pcbWaveSize))
            {
                if (ppbWaveData)
                    *ppbWaveData = (LPBYTE)pdw;

                if (pcbWaveSize)
                    *pcbWaveSize = dwLength;

                if (!ppWaveHeader || *ppWaveHeader)
                    return TRUE;
            }
            break;
        }

        pdw = (DWORD *)((BYTE *)pdw + ((dwLength+1)&~1));
    }

exit:
    return FALSE;
}
