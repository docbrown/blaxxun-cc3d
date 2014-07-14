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

@module GVTexture2.cpp - VRML 1.0 + 2.0 texture nodes |

Copyright (c) 1995-1999	by Holger Grahn blaxxun interactive
All rights reserved

Purpose:

Classes:

Notes:

Changes:

$Revision: 1.34 $
$Log: gvtexture2.cpp,v $
Revision 1.34  1998/10/07 13:23:08  holger
empty message

Revision 1.30  1998/07/09 15:50:07  kristof
*** empty log message ***

Revision 1.29  1998/07/02 17:50:20  holger
empty message

Revision 1.28  1998/07/01 16:52:03  holger
empty message



Todo :

******************************************************************************/


#include "stdafx.h"
#include "grender.h"
#include "glutils.h"
#include "gversion.h"

#include "www.h"


#include <gvsensors.h>

// animated gifs
#include "gifread.h"


#include <gvtexture2.h>
#include "greadgv.h"
#include "gmodel.h"

#include "greporter.h"

// #include <gvmaterial.h>
#include <gvelement.h>
#include "gvtraverse.h"

#ifdef G_RMA_MOVIE

#include "media/rma/grmaplay.h"

#define G_RMA_URN "urn:inet:blaxxun.com:rma:"
#define G_RMA_URN_CACHED "urn:inet:blaxxun.com:rma_cached:"

// is given filename / url RMA format 
gbool IsRmaFmt(const char *fileName,int fileFmt)
{
	if (fileFmt>=RMA_FMT && fileFmt <= RMA_FMT) return gtrue;
	if (strprefix(fileName,G_RMA_PROTOCOL)) return gtrue;
	if (strprefix(fileName,G_RMA_URN)) return gtrue;
	if (strprefix(fileName,"pnm://")) return gtrue;
	return gfalse;
}
// is given url RMA protocol ?
gbool IsRmaProtocol(const char *fileName)
{
	if (strprefix(fileName,G_RMA_PROTOCOL)) return gtrue;
	if (strprefix(fileName,G_RMA_URN_CACHED)) return gtrue;
	if (strprefix(fileName,G_RMA_URN)) return gtrue;
	if (strprefix(fileName,"pnm://")) return gtrue;
	return gfalse;
}


void makeFileUrl(const char *szURL, char *szStringSpace)
{ 

	if (strprefix(szURL,G_RMA_URN)) {
		szURL += strlen(G_RMA_URN);
	}
	else if (strprefix(szURL,G_RMA_URN_CACHED)) {
		szURL += strlen(G_RMA_URN_CACHED);
	}
	
	
	// If this is not a URL, create a URL from the local filespec.
/// This code is DOS specific and Netscape specific!
    const char *del = strchr(szURL,':');

	if (del &&  (del - szURL) <=2) del = NULL; /* a: b:  mailto: */


    if (del || (strprefix(szURL, "file:///"))
     	    || (strprefix(szURL, "http://"))
     	    || (strprefix(szURL, G_RMA_PROTOCOL))
     	    || (strprefix(szURL, "pnm://"))
			)
    {
        lstrcpy(szStringSpace, szURL) ;
	}
    // Must be a local file!
    else
    {
        char    szDrive[8] ; //_MAX_DRIVE
        char    szDir[_MAX_PATH] ;
        char    szFile[_MAX_PATH] ;
        char    szExt[_MAX_PATH] ;
        int     i = 0 ;

        _splitpath(szURL, szDrive, szDir, szFile, szExt) ;

        lstrcpy(szStringSpace, "file:///") ;
		if (szDrive[1] == ':') {
			szDrive[1] = '|' ;
			//szDrive[2] = 0 ;
		}
		lstrcat(szStringSpace, szDrive) ;

        while(szDir[i] != '\0')
        {
            if (szDir[i] == '\\')
                szDir[i] = '/' ;
            i++ ;
        }

        lstrcat(szStringSpace, szDir) ;
        lstrcat(szStringSpace, szFile) ;
        lstrcat(szStringSpace, szExt) ;

    }
}

#endif

// Active Movie support 
#ifdef G_AMOVIE
#include "mmsystem.h"
#include "ddraw.h"
#include "dsound.h"


#include "mmstream.h"
#include "amstream.h"
#include "ddstream.h"

#include <strmif.h>
#include <control.h>
#include <evcode.h>
#include <uuids.h>


#ifdef _OGL
#define RELEASE(x) if (x) { (x)->Release(); x = NULL; }
#endif

#define CHECK_ERROR(x)     \
   if (FAILED(hr = (x))) { \
       Reporter.Error(#x "  failed with HRESULT(0x%8.8X)\n", hr); \
       goto Exit;          \
   }

gbool IsAudioFormat(int fmt) 
{
	return (fmt>=WAV_FMT && fmt <= MPS_FMT);
}

gbool IsVideoFormat(int fmt) 
{
	return (fmt>=AVI_FMT && fmt <= QT_FMT);
}

const LONGLONG MILLISECONDS = (1000);            // 10 ^ 3
const LONGLONG NANOSECONDS = (1000000000);       // 10 ^ 9
const LONGLONG UNITS = (NANOSECONDS / 100);      // 10 ^ 7

/*  Unfortunately an inline function here generates a call to __allmul
    - even for constants!
*/
#define MILLISECONDS_TO_100NS_UNITS(lMs) \
    Int32x32To64((lMs), (UNITS / MILLISECONDS))

inline LONG Millisecs(REFERENCE_TIME m_time)
{
       return (LONG)(m_time / (UNITS / MILLISECONDS));
};


//C:\DXMedia\help\ds\dssd0053.htm
// C:\DXMedia\help\ds\dssd0396.htm
// C:\DXMedia\help\ds\dssd0054.htm
class GDSStream {
public :

	IDirectDraw *pDD;

	IMoniker *pMoniker;
	IBindCtx  *pBindCtx;	

	IGraphBuilder *pGraphBuilder;
	
	IAMMultiMediaStream *pMMStream;

	DWORD streamFlags;
	STREAM_TYPE streamType;
	STREAM_TIME duration;
	HANDLE	hEOS;

	IDirectDrawSurface *pTargetSurface;

    HRESULT hr;
	gbool video;

    IMediaStream *pPrimaryVidStream;
    IDirectDrawMediaStream *pDDStream;
    IDirectDrawSurface *pSurface;
    IDirectDrawStreamSample *pSample;   // C:\DXMedia\help\ds\dssd0382.htm

	HANDLE sampleEvent;
	gbool waitingForSample;

	IDirectDrawPalette *pDirectDrawPalette;
	// CMonikerFile
	DDSURFACEDESC ddsd;
    RECT rect;
	int width2,height2; // width height 2 powered 
	
	gbool audio;
	gbool defaultAudio;
	// Audio 
    WAVEFORMATEX wfx;    
	DWORD audioBufferSize; 
    PBYTE pBuffer;
    IMediaStream *pPrimaryAudioStream;    
	IAudioStreamSample *pAudioSample;
	HANDLE audioSampleEvent;
    IAudioMediaStream *pAudioStream;    
	IAudioData 	*pAudioData;


	GDSStream();
	~GDSStream();


	HRESULT OpenMMStream(int fileFormat,const char * pszFileName,IAMMultiMediaStream **ppMMStream);

	BOOL CreateFilterGraph(int fileFormat,const char * pszFileName,IAMMultiMediaStream **ppMMStream);


	HRESULT Open(int fileFormat,const char * pszFileName,RenderState &state);
	
	void Close();
	BOOL CheckFilterGraph();

	// update the sample 
	gbool UpdateSample(BOOL &updateVideo);


	STREAM_STATE state;

	// set the stream state 
	gbool SetState(STREAM_STATE newState) { 
			state = newState;
			hr = pMMStream->SetState(newState);
			return (hr == S_OK);

	}
	gbool Seek(STREAM_TIME seekTime) 
	{
			hr = pMMStream->Seek(seekTime);
			return (hr == S_OK);

	}
	gbool GetTime(STREAM_TIME &time) 
	{
			hr = pMMStream->GetTime(&time);
			return (hr == S_OK);

	}

};

BOOL GDSStream::CreateFilterGraph(int fileFormat,const char * pszFileName,IAMMultiMediaStream **ppMMStream)
{
    ASSERT(pGraphBuilder == NULL);

    hr = CoCreateInstance(CLSID_FilterGraph, 		// get this documents graph object
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_IGraphBuilder,
                          (void **) &pGraphBuilder);
    if (FAILED(hr)){
    	pGraphBuilder = NULL;
        return FALSE;
    }

    // get hold of the event notification handle so we can wait for
    // completion
    IMediaEvent *pME;
    hr = pGraphBuilder->QueryInterface(IID_IMediaEvent, (void **) &pME);
    if (FAILED(hr)) {
        return FALSE;
    }

    //hr = pME->GetEventHandle((OAEVENT*) &m_hGraphNotifyEvent);

    pME->Release();

    if (FAILED(hr)) {
		return FALSE;
    }

    WCHAR       wPath[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, pszFileName, -1, wPath, sizeof(wPath)/sizeof(wPath[0]));

    if (FAILED( hr=pGraphBuilder->RenderFile(wPath, NULL) )) {
		CString tmp;
		// hr = AMGetErrorText(hr,tmp.GetBuffer(256),256);
		TRACE("Can't render file  HRESULT(0x%8.8X)\n", hr);
        return FALSE;
    }

	IBaseFilter  *pFilter=NULL;  // EnumFilters vars
	IEnumFilters *pEnum=NULL;
	ULONG        pcFetched;
	FILTER_INFO  info;

	hr = pGraphBuilder->EnumFilters(&pEnum);

	if (FAILED(hr))
		return TRUE;

	while ((hr = pEnum->Next(1, &pFilter, &pcFetched)) == S_OK)
	{
		hr = pFilter->QueryFilterInfo(&info);
		//hr = pFilter->QueryVendorInfo(LPWSTR * );
		CString tmp(info.achName);
		TRACE("Filter %s \n",(const char *) tmp);
		// Microsoft® Netshow(TM) with Real  
		//hr = pFilter->QueryInterface(IID_IIndeoDecode, &m_pIndeo);
		pFilter->Release();

	}
	// This interface needs to be released, even though
	// it was not obtained in the usual way with QueryInterface.
	pEnum->Release(); 

    return TRUE;
}


BOOL GDSStream::CheckFilterGraph()
{
	IBaseFilter  *pFilter=NULL;  // EnumFilters vars
	IEnumFilters *pEnum=NULL;
	ULONG        pcFetched;
	FILTER_INFO  info;
    
	if (pGraphBuilder == NULL) return FALSE;

	hr = pGraphBuilder->EnumFilters(&pEnum);

	if (FAILED(hr))
		return FALSE;

	while ((hr = pEnum->Next(1, &pFilter, &pcFetched)) == S_OK)
	{
		hr = pFilter->QueryFilterInfo(&info);
		//hr = pFilter->QueryVendorInfo(LPWSTR * );
		CString tmp(info.achName);
		TRACE("Filter %s \n",(const char *) tmp);
		// Microsoft® Netshow(TM) with Real  
		//hr = pFilter->QueryInterface(IID_IIndeoDecode, &m_pIndeo);
		pFilter->Release();

	}
	// This interface needs to be released, even though
	// it was not obtained in the usual way with QueryInterface.
	pEnum->Release(); 

    return TRUE;
}


HRESULT GDSStream::OpenMMStream(int fileFormat,const char * pszFileName,IAMMultiMediaStream **ppMMStream)
{
    *ppMMStream = NULL;
    IAMMultiMediaStream *pAMStream;
	BOOL opened = FALSE;

    CHECK_ERROR(CoCreateInstance(CLSID_AMMultiMediaStream, NULL, CLSCTX_INPROC_SERVER,
				 IID_IAMMultiMediaStream, (void **)&pAMStream));

	if ( FALSE &&  CreateFilterGraph(fileFormat,pszFileName,ppMMStream)) {
		//works but adds "Video Renderer" + "Default Direct Sound Device"
	
	    CHECK_ERROR(pAMStream->Initialize(STREAMTYPE_READ, 0, pGraphBuilder));
		opened = TRUE;
	}
	else 
		CHECK_ERROR(pAMStream->Initialize(STREAMTYPE_READ, 0, NULL));
   	// pAMStream->Initialize(STREAMTYPE_READ, AMMSF_NOGRAPHTHREAD, NULL);

#if 1
//	if (IsAudioFormat(fileFormat))
//		video = FALSE;
	if (1) {
    if (video) {
		CHECK_ERROR(pAMStream->AddMediaStream(pDD, &MSPID_PrimaryVideo, 0, NULL));
    }
	if (audio) {
		CHECK_ERROR(pAMStream->AddMediaStream(NULL, &MSPID_PrimaryAudio, 0, NULL));

	} else  if (defaultAudio) 
	{ 
		// let amovie play it 
		//CHECK_ERROR();
        hr = pAMStream->AddMediaStream(NULL, &MSPID_PrimaryAudio, AMMSF_ADDDEFAULTRENDERER, NULL);
        if (FAILED(hr)) {
            Reporter.Warning("Adding default AUDIO renderer failed with HRESULT(0x%8.8X)", hr); 
		    defaultAudio = FALSE;
        }
	}
	}
#endif

//	pszFileName = "http://www.mediadome.com/Webisodes/Titanic/Webcast/Movies/cameron.avi";	
//	pszFileName = "http://www.mediadome.com/Webisodes/Titanic/Clip/Asx/deepdive56.asx";

	if (!opened) {

    WCHAR       wPath[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, pszFileName, -1, wPath, sizeof(wPath)/sizeof(wPath[0]));
    CHECK_ERROR(CreateBindCtx(0, &pBindCtx));
	hr = ::MkParseDisplayNameEx(pBindCtx, wPath, 0, &pMoniker);
	if (hr == S_OK) {
		CHECK_ERROR(pAMStream->OpenMoniker(pBindCtx,pMoniker, 0));
	}
    else hr = pAMStream->OpenFile(wPath, AMMSF_RENDERALLSTREAMS );
	if (hr != S_OK) {
		TRACE("Open file failed with HRESULT(0x%8.8X)\n", hr);
		// .ra file Open file failed with HRESULT(0x80040217)
		// but ra DLL's loaded ???
		goto Exit;
	}
	}
    *ppMMStream = pAMStream;
    pAMStream->AddRef();

	RELEASE(pGraphBuilder);

	hr = pAMStream->GetFilterGraph(&pGraphBuilder);
	CheckFilterGraph();

	RELEASE(pGraphBuilder);

	hr = pAMStream->GetFilterGraph(&pGraphBuilder);
	CheckFilterGraph();

	//RELEASE(pGraphBuilder);


Exit:
    if (pAMStream == NULL) {
		Reporter.Error("Could not create MultiMediaStream object\n"
	       "Please install Microsoft Windows Media Player from \nhttp://www.microsoft.com/windows/mediaplayer/\n");
    }
    RELEASE(pAMStream);
    return hr;
}

GDSStream::GDSStream() : 
pBindCtx(NULL),pMoniker(NULL),
pGraphBuilder(NULL),
pDD(NULL),pMMStream(NULL),pTargetSurface(NULL),
hr(S_OK),
video(TRUE),
pPrimaryVidStream(NULL),pDDStream(NULL),pSurface(NULL),pSample(NULL),/* pSampleEvent(0),*/
waitingForSample(gfalse),
pDirectDrawPalette(NULL),
audio(FALSE),defaultAudio(TRUE),width2(0),height2(0),

pBuffer(NULL),pPrimaryAudioStream(NULL),pAudioSample(NULL),pAudioStream(NULL),pAudioData(NULL),
state(STREAMSTATE_STOP)
{
	audioBufferSize = 10 * 1024;
}


GDSStream::~GDSStream()
{
	Close();
}
/* 

         hr = ::CreateURLMoniker(0,url,ppmkr); 
       // hr = ::MkParseDisplayNameEx(0, url, 0, ppmkr);

	
	  vidclip
    CComPtr <IMoniker> pDeviceMoniker;
    CComPtr <IBindCtx> pBindCtx;
    CComPtr <IBaseFilter> pCodecFilter;
    CComPtr <IGraphBuilder> pFilterGraph;
    if (pszCodec) {
        unsigned long ccEaten;
        CHECK_ERROR(CreateBindCtx(0, &pBindCtx), IDS_NOCOMPRESSOR);
        CHECK_ERROR(MkParseDisplayName(pBindCtx, pszCodec, &ccEaten, &pDeviceMoniker), IDS_NOCOMPRESSOR);
        CHECK_ERROR(pDeviceMoniker->BindToObject(pBindCtx, NULL, IID_IBaseFilter, (void **)&pCodecFilter), IDS_NOCOMPRESSOR);
        CHECK_ERROR(pBuilder->GetFiltergraph(&pFilterGraph), IDS_INTERNALERROR);
        CHECK_ERROR(pFilterGraph->AddFilter(pCodecFilter, pszFilterName), IDS_INTERNALERROR);
        hr = pBuilder->RenderStream(NULL, pStream, pCodecFilter, pMuxFilter);
        if (FAILED(hr)) {
            ErrorMessage(IDS_CANTCOMPRESS);

*/
HRESULT GDSStream::Open(int fileFormat,const char * pszFileName,RenderState &state)
{	
	DDSURFACEDESC ddsdDesired;
    HRESULT hr = OpenMMStream(fileFormat,pszFileName, &pMMStream);
    if (SUCCEEDED(hr)) {
		// get all info from stream
	    hr = pMMStream->GetInformation(&streamFlags,&streamType);
		hr = pMMStream->GetDuration(&duration);
		hr = pMMStream->GetEndOfStreamEventHandle(&hEOS);
		IMediaStream *s=NULL;
		int i=0;
		while( (hr = pMMStream->EnumMediaStreams(i,&s)) == S_OK) {
			TRACE("Stream %d \n",i);
			MSPID purposeId;
			STREAM_TYPE stype;
			hr = s->GetInformation(  &purposeId,&stype);
			RELEASE(s);
			i++;
		}
		// get video 
		if (video) {
			hr =pMMStream->GetMediaStream(MSPID_PrimaryVideo, &pPrimaryVidStream);
			if (FAILED(hr) || pPrimaryVidStream == NULL) 
				video = FALSE;
		}

		if (video) {

			CHECK_ERROR(pPrimaryVidStream->QueryInterface(IID_IDirectDrawMediaStream, (void **)&pDDStream));

			ZeroMemory(&ddsd, sizeof(ddsd));		
			ddsd.dwSize = sizeof(ddsd); 
			
			ZeroMemory(&ddsdDesired, sizeof(ddsdDesired));		
			ddsdDesired.dwSize = sizeof(ddsdDesired); 	

			pDDStream->GetFormat(&ddsd, NULL, &ddsdDesired, NULL);
			TRACE("Video Format %d * %d PixelF flags %d bits %d (%x %x %x) \n",ddsd.dwWidth,ddsd.dwHeight,ddsd.ddpfPixelFormat.dwFlags,ddsd.ddpfPixelFormat.dwRGBBitCount,ddsd.ddpfPixelFormat.dwRBitMask,ddsd.ddpfPixelFormat.dwGBitMask,ddsd.ddpfPixelFormat.dwBBitMask);
			TRACE("Video Format Desired %d * %d PixelF flags %d bits %d (%x %x %x) \n",ddsdDesired.dwWidth,ddsd.dwHeight,ddsdDesired.ddpfPixelFormat.dwFlags,ddsdDesired.ddpfPixelFormat.dwRGBBitCount,ddsdDesired.ddpfPixelFormat.dwRBitMask,ddsdDesired.ddpfPixelFormat.dwGBitMask,ddsdDesired.ddpfPixelFormat.dwBBitMask);
			
			DDPIXELFORMAT texFormat;
			int texFormatIndex=-1;	

			texFormat = ddsdDesired.ddpfPixelFormat;
#ifdef _D3D
			texFormatIndex = state.FindTextureFormat(ddsdDesired.ddpfPixelFormat,texFormat);
			TRACE("matching tex Format  PixelF flags %d bits %d (%x %x %x) \n",texFormat.dwFlags,texFormat.dwRGBBitCount,texFormat.dwRBitMask,texFormat.dwGBitMask,texFormat.dwBBitMask);
#else
			if ( !(texFormat.dwFlags & DDPF_RGB) || texFormat.dwRGBBitCount<24)
				texFormat.dwFlags=DDPF_RGB,texFormat.dwRGBBitCount=24,texFormat.dwRBitMask=0xff0000,texFormat.dwGBitMask=0xff00,texFormat.dwBBitMask=0xff;
			// test 
			//texFormat.dwFlags=DDPF_RGB,texFormat.dwRGBBitCount=24,texFormat.dwBBitMask=0x0ff0000,texFormat.dwGBitMask=0x0ff00,texFormat.dwRBitMask=0x0ff;

			TRACE("tex Format  PixelF flags %d bits %d (%x %x %x) \n",texFormat.dwFlags,texFormat.dwRGBBitCount,texFormat.dwRBitMask,texFormat.dwGBitMask,texFormat.dwBBitMask);

#endif



			// try to change format 
			width2 = ddsd.dwWidth;
			height2 = ddsd.dwHeight;
			// map to nearest 2 power 
			state.GetTextureTargetSize(width2,height2,256);
			//get_image_2power(width2,height2,state.textureParameters.current_max_size_x,state.textureParameters.current_max_size_y,state.textureParameters.square_size);


			// SetDirectDraw(  IDirectDraw* pDirectDraw)
/*			scaling doesn't work ?			
			ddsd.dwWidth = width;
			ddsd.dwHeight = height;
*/
			ddsd.dwFlags |= DDSD_CAPS |  DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
			// ddsd.ddsCaps.dwCaps |=  /* DDSCAPS_TEXTURE | */  DDSCAPS_SYSTEMMEMORY;
			ddsd.ddsCaps.dwCaps |= DDSCAPS_OFFSCREENPLAIN; // test hg
			// DDSCAPS_LIVEVIDEO
			// DDSCAPS_HWCODEC
			if (texFormatIndex >=0) {
				ddsd.ddpfPixelFormat = texFormat; // test 			
			}
			hr = pDDStream->SetFormat(&ddsd, NULL);
			if (hr != S_OK) {
				TRACE("Changing of video format failed %x \n",hr);
			}


			CHECK_ERROR(pDDStream->CreateSample(NULL, NULL, 0, &pSample));
    
			CHECK_ERROR(pSample->GetSurface(&pSurface, &rect));

			pDDStream->GetFormat(&ddsd,&pDirectDrawPalette, NULL, NULL);
			TRACE("Format %d * %d PixelF flags %d bits %d (%x %x %x) \n",ddsd.dwWidth,ddsd.dwHeight,ddsd.ddpfPixelFormat.dwFlags,ddsd.ddpfPixelFormat.dwRGBBitCount,ddsd.ddpfPixelFormat.dwRBitMask,ddsd.ddpfPixelFormat.dwGBitMask,ddsd.ddpfPixelFormat.dwBBitMask);
		    // CHECK_ERROR(pDDStream->SetFormat(&ddsd, NULL));

		    sampleEvent = CreateEvent(FALSE, NULL, NULL, FALSE);
		}
		// pAMStream->AddMediaStream(NULL, &MSPID_PrimaryAudio, 0, NULL);

		if (audio) {
		    hr = pMMStream->GetMediaStream(MSPID_PrimaryAudio, &pPrimaryAudioStream);
			if (FAILED(hr) || pPrimaryAudioStream == NULL) 
				audio = FALSE;
			if (audio) {
				CHECK_ERROR(pPrimaryAudioStream->QueryInterface(IID_IAudioMediaStream, (void **)&pAudioStream));
				CHECK_ERROR(pAudioStream->GetFormat(&wfx));
				CHECK_ERROR(CoCreateInstance(CLSID_AMAudioData, NULL, CLSCTX_INPROC_SERVER,IID_IAudioData, (void **)&pAudioData));
				pBuffer = (PBYTE)LocalAlloc(LMEM_FIXED, audioBufferSize);
				CHECK_ERROR(pAudioData->SetBuffer(audioBufferSize, pBuffer, 0));
				CHECK_ERROR(pAudioData->SetFormat(&wfx));
				CHECK_ERROR(pAudioStream->CreateSample(pAudioData, 0, &pAudioSample));
			}
		}
    }

	if (!(video || audio || defaultAudio)) 
		hr = S_FALSE;
	return hr;

Exit:
    RELEASE(pPrimaryVidStream);
    RELEASE(pDDStream);
    RELEASE(pSample);
    RELEASE(pSurface);
    return hr;
}


void GDSStream::Close()
{
    if (state == STREAMSTATE_RUN)
		SetState(STREAMSTATE_STOP);

    RELEASE(pAudioData);
    RELEASE(pAudioStream);
    RELEASE(pAudioSample);
    RELEASE(pPrimaryAudioStream);

	if (pBuffer) LocalFree((HLOCAL)pBuffer); pBuffer = NULL;

    RELEASE(pSample);
	CloseHandle(sampleEvent);
    RELEASE(pSurface);
    RELEASE(pDirectDrawPalette);

    RELEASE(pPrimaryVidStream);
    RELEASE(pDDStream);
	RELEASE(pGraphBuilder);
	RELEASE(pMMStream);

	RELEASE(pTargetSurface);
	RELEASE(pBindCtx);
	RELEASE(pMoniker);

}

// update the sample 
gbool GDSStream::UpdateSample(BOOL &updateVideo)
{
	if (video) {
		//if ((hr=pSample->Update( /*SSUPDATE_ASYNC */   SSUPDATE_CONTINUOUS, NULL, NULL, 0)) != S_OK) {
		if (!waitingForSample) {
		if ((hr=pSample->Update(0, sampleEvent, NULL, 0)) != S_OK) {
			
			if (hr == MS_E_BUSY ) TRACE("Busy \n");
			else if (hr == MS_S_PENDING ) { 
				TRACE("MS_S_PENDING  \n");
				waitingForSample = TRUE;
			}
			else if (hr == MS_S_ENDOFSTREAM  ) {
				TRACE("MS_S_ENDOFSTREAM   \n");
			}
			else TRACE("UpdateSample video failed with HRESULT(0x%8.8X)\n", hr);

			//pSample->CompletionStatus(0,0);
			return FALSE;
		}
		updateVideo = TRUE;
		}


	}
/*	if (audio) {
		    HANDLE hEvent = CreateEvent(FALSE, NULL, NULL, FALSE);
            HRESULT hr = pAudioSample->Update(0, hEvent, NULL, 0);
            if (FAILED(hr) || MS_S_ENDOFSTREAM == hr) {                
				// break;
            }            
			WaitForSingleObject(hEvent, INFINITE);
            WORD dwLength;
            pAudioData->GetInfo(NULL, NULL, &dwLength);
            // WaveOut.Write(pBuffer, dwLength
	}
*/	
	return TRUE;
}



#if 0
// so works the showstream sample :

HRESULT RenderStreamToSurface(IDirectDraw *pDD, IDirectDrawSurface *pPrimary,
			      IMultiMediaStream *pMMStream)
{

    HRESULT hr;
    IMediaStream *pPrimaryVidStream = NULL;
    IDirectDrawMediaStream *pDDStream = NULL;
    IDirectDrawSurface *pSurface = NULL;
    IDirectDrawStreamSample *pSample = NULL;

    RECT rect;

    CHECK_ERROR(pMMStream->GetMediaStream(MSPID_PrimaryVideo, &pPrimaryVidStream));
    CHECK_ERROR(pPrimaryVidStream->QueryInterface(IID_IDirectDrawMediaStream, (void **)&pDDStream));

    CHECK_ERROR(pDDStream->CreateSample(NULL, NULL, 0, &pSample));
    
	CHECK_ERROR(pSample->GetSurface(&pSurface, &rect));
    
    CHECK_ERROR(pMMStream->SetState(STREAMSTATE_RUN));

    while (true) {
	if (pSample->Update(0, NULL, NULL, 0) != S_OK) {
	    break;
	}
	pPrimary->Blt(&rect, pSurface, &rect, DDBLT_WAIT, NULL);
    }

Exit:
    RELEASE(pPrimaryVidStream);
    RELEASE(pDDStream);
    RELEASE(pSample);
    RELEASE(pSurface);

    return hr;
}

#endif

#ifdef _OGL
#define     D3DError(MSG,RESULT) 0

/* 
   decode direct Draw surface
   and allocate and copy pixels to image im
*/

gbool ImageSet(image &im, 
			   LPDIRECTDRAWSURFACE lpDDS,
			   IDirectDrawPalette *lpDDP,
			   DDSURFACEDESC &format, 
			   RECT &srcRect)
{
    HRESULT ddrval;
    int i,j;

	gbool ret = FALSE;
	int dwWidth,dwHeight;
	DDSURFACEDESC ddsd;

    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
	ddsd.dwSize = sizeof(DDSURFACEDESC);
    
	ddrval = lpDDS->Lock(NULL, &ddsd, 0, NULL);
    
	if (ddrval != DD_OK) {

        D3DError("Lock failed while locking surface .",(ddrval));
        return FALSE;
    }
	dwWidth = ddsd.dwWidth;
	dwHeight = ddsd.dwHeight;

    if (ddsd.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8 ) {
				IBYTE *dest;

				ret = image_alloc(&im,IM_MAPPED8,dwWidth,dwHeight)>=0;
				if (!ret) goto Exit;
				dest = IROW(&im,0);
				if (lpDDP) {
					PALETTEENTRY ppe[256];
					memset(ppe, 0, sizeof(PALETTEENTRY) * 256);
					int numColors = 256;
					ddrval = lpDDP->GetEntries(0,0,numColors,ppe);
					if (ddrval != DD_OK) {
						D3DError("Can't get palette",ddrval);
					}
					ret= image_alloc_cmap(&im, numColors)>=0;
					for (int i = 0; i < numColors; i++) {
						image_set_cmap(&im,i,ppe[i].peRed,ppe[i].peGreen,ppe[i].peBlue);
					}
				}

                for (j = 0; j < dwHeight; j++) {
                    /*
                     * Point to next row in texture surface
                     */
                    unsigned char* lpSP = (unsigned char*)(((char*)ddsd.lpSurface) +  ddsd.lPitch * j);
					dest = IROW(&im,dwHeight-1-j);
#if 1
					memcpy(dest,lpSP,dwWidth);
#else
					i = dwWidth;                    
	                while (i-- > 0) {

						unsigned char p= *lpSP++;
						*dest++ = p;
                    }
#endif
                }
				ret = TRUE;

	}
	else 


    if (ddsd.ddpfPixelFormat.dwFlags & DDPF_RGB ) {
		IBYTE *dest;
        int red_shift, red_scale;
        int green_shift, green_scale;
        int blue_shift, blue_scale;
        int alpha_shift=0, alpha_scale=1;
		int s,m;
        /*
         * Determine the red, green and blue masks' shift and scale.
         */
        for (s = 0, m = ddsd.ddpfPixelFormat.dwRBitMask; !(m & 1);  s++, m >>= 1);
        red_shift = s;
        red_scale = 255 / (ddsd.ddpfPixelFormat.dwRBitMask >> s);
        for (s = 0, m = ddsd.ddpfPixelFormat.dwGBitMask; !(m & 1);
                                                               s++, m >>= 1);
        green_shift = s;
        green_scale = 255 / (ddsd.ddpfPixelFormat.dwGBitMask >> s);
        for (s = 0, m = ddsd.ddpfPixelFormat.dwBBitMask; !(m & 1);
                                                               s++, m >>= 1);
        blue_shift = s;
        blue_scale = 255 / (ddsd.ddpfPixelFormat.dwBBitMask >> s);

			if (ddsd.ddpfPixelFormat.dwRGBAlphaBitMask !=0) {
				for (s = 0, m = ddsd.ddpfPixelFormat.dwRGBAlphaBitMask; !(m & 1);  s++, m >>= 1);
				alpha_shift = s;
				alpha_scale = 255 / (ddsd.ddpfPixelFormat.dwRGBAlphaBitMask >> s);
			}

        /*
         * Each RGB bit count requires different pointers
         */
        switch (ddsd.ddpfPixelFormat.dwRGBBitCount) {
            case 24 :
				ret = image_alloc(&im,IM_RGB,dwWidth,dwHeight)>=0;
				if (!ret) goto Exit;
				dest = IROW(&im,0);

                for (j = 0; j < (int)dwHeight; j++) {
                    /*
                     * Point to next row in texture surface
                     */
                    IBYTE *lpLP = (((IBYTE*)ddsd.lpSurface) +  ddsd.lPitch * j);
					dest = IROW(&im,dwHeight-1-j); // FLIP 

#if 1
					memcpy(dest,lpLP,dwWidth*3);
#else					
					i = dwWidth;
                    while (i-- > 0) {
						*dest++ = *lpLP++;
						*dest++ = *lpLP++;
						*dest++ = *lpLP++;
                    }
#endif
                }
				ret = TRUE;
                break;
            case 32 :
				ret = image_alloc(&im,IM_RGB,dwWidth,dwHeight)>=0;
				if (!ret) goto Exit;
				dest = IROW(&im,0);

                for (j = 0; j < dwHeight; j++) {
                    /*
                     * Point to next row in texture surface
                     */
                    unsigned long* lpSP = (unsigned long*)(((char*)ddsd.lpSurface) +  ddsd.lPitch * j);
					dest = IROW(&im,dwHeight-1-j);
                    for (i = 0; i < dwWidth; i++) {
                        IBYTE r, g, b, a;
						unsigned long p= *lpSP++;
						r =  ((p & ddsd.ddpfPixelFormat.dwRBitMask) >> red_shift)*red_scale;
						g =  ((p & ddsd.ddpfPixelFormat.dwGBitMask) >> green_shift)*green_scale;
						b =  ((p & ddsd.ddpfPixelFormat.dwBBitMask) >> blue_shift)*blue_scale;

						*dest++ = r;
						*dest++ = g;
						*dest++ = b;
                    }
                }
				ret = TRUE;
                break;
            case 16 :
				ret = image_alloc(&im,IM_RGB,dwWidth,dwHeight)>=0;
				if (!ret) goto Exit;
				dest = IROW(&im,0);

                for (j = 0; j < dwHeight; j++) {
                    /*
                     * Point to next row in texture surface
                     */
                    unsigned short* lpSP = (unsigned short*)(((char*)ddsd.lpSurface) +  ddsd.lPitch * j);
					dest = IROW(&im,dwHeight-1-j);
                    for (i = 0; i < dwWidth; i++) {
                        IBYTE r, g, b, a;
						unsigned short p= *lpSP++;
						r =  ((p & ddsd.ddpfPixelFormat.dwRBitMask) >> red_shift)*red_scale;
						g =  ((p & ddsd.ddpfPixelFormat.dwGBitMask) >> green_shift)*green_scale;
						b =  ((p & ddsd.ddpfPixelFormat.dwBBitMask) >> blue_shift)*blue_scale;

						*dest++ = r;
						*dest++ = g;
						*dest++ = b;
                    }
                }
				ret = TRUE;
                break;
            case 8 :
				ret = image_alloc(&im,IM_RGB,dwWidth,dwHeight)>=0;
				if (!ret) goto Exit;
				dest = IROW(&im,0);

                for (j = 0; j < dwHeight; j++) {
                    /*
                     * Point to next row in texture surface
                     */
                    unsigned char* lpSP = (unsigned char*)(((char*)ddsd.lpSurface) +  ddsd.lPitch * j);
					dest = IROW(&im,dwHeight-1-j);
                    for (i = 0; i < dwWidth; i++) {
                        IBYTE r, g, b, a;
						unsigned char p= *lpSP++;
						r =  ((p & ddsd.ddpfPixelFormat.dwRBitMask) >> red_shift)*red_scale;
						g =  ((p & ddsd.ddpfPixelFormat.dwGBitMask) >> green_shift)*green_scale;
						b =  ((p & ddsd.ddpfPixelFormat.dwBBitMask) >> blue_shift)*blue_scale;

						*dest++ = r;
						*dest++ = g;
						*dest++ = b;
                    }
                }
				ret = TRUE;
                break;
            default:
                /*
                 * This wasn't a format I recognize
                 */
                lpDDS->Unlock(NULL);
                D3DError("Unknown pixel format (loadtex).",1);
                return FALSE;
        }

	}
Exit:
    lpDDS->Unlock(NULL);
	return ret;

}

#endif

#endif


//
// GvTexture
// 

GV_NODE_SOURCE_IMP(GvTexture,GvNode);

GvTexture::GvTexture()
{
    GV_NODE_CONSTRUCTOR(GvTexture);
    isBuiltIn = FALSE;
	defined = 0;
	hasAlpha = FALSE;
	hasColorKey = FALSE;
	isGrayscale = FALSE;
}

GvTexture::~GvTexture()
{
}


int GvTexture::glFlushCache()
{

	defined = 0;
	texture.Release();
	return(0);
}

int 			
GvTexture::glSet(GglRenderTraversal &state,GvBool repeatS,GvBool repeatT,GvBool alpha,GvBool matHasAlpha)
{
	int ret = 0;

#ifdef _OGL

    if (texture.Ok()) { // texture loaded 

	RenderState &glState = *state.glState;

	 
	
	// Enable texturing 
	state.glState->SetAnyTexturing(1);

#if 0 

	// temp: should be handled by GvAppearance::Apply also need check material alpha
	if (state.glState->textureParameters.set_diffuse_white) {
		// change diffuse color to white 

		Point rgb(1,1,1);
		glColor(rgb);    //temp hack  for white diffuse color 
	 
		GLenum face = GL_FRONT_AND_BACK;
		RGBAColor rgba;
	
		rgba.White();
		glMaterialfv(face,GL_DIFFUSE,rgba);

	 	// force reload color, if texture switch off
		state.glState->currentBaseColorNode = (GvNode *) -1;
	}
#endif
	
	if (alpha) {
		
	    // enable alphafunc
        gbool _alpha = glState.alphaTest;
        gbool _blend = glState.blend;
        if (!_alpha || !_blend) {
					GLint _alphaFunc=glState.alphaFunc;
                    GLfloat _alphaRef=glState.alphaRef;
					if (hasColorKey && !matHasAlpha) { // no blending done, enable alpha func skipping pixels with alpha = 0.0    
						// change state
						glState.SetAlphaFunc(GL_GREATER,0.75); // skip if more then 3 neighbours transparent
						glState.SetAlphaTestFromTexture(TRUE);
/*						if (glState.needBlendForCKey)
							glState.SetBlendFromMaterial(TRUE); */
					} else {
						glState.SetAlphaFunc(GL_GREATER,0.0); // could alpha test anyway 
						glState.SetAlphaTestFromTexture(TRUE);
						glState.SetBlendFromMaterial(TRUE); // also in material apply 
					}
		}	
		if (hasColorKey) {
			ret |= TEXTURE_HAS_COLORKEY;
		}
		else 
			if (hasAlpha) ret |= TEXTURE_HAS_ALPHA;


    }
	else glState.SetAlphaTestFromTexture(FALSE); 


    // call displaylist to set texture image
	if (!state.glState->SetTexture(texture)) {
		TRACE("setting of texture failed !!!??");
	}

	glState.SetTextureWrap(repeatS,repeatT);

	if (isGrayscale) 
		ret |= TEXTURE_MODULATE;
	ret |= TEXTURE_DEFINED;
	state.glState->textureDefined = 1;
	return(ret);
  
  } else {

	  state.glState->SetAnyTexturing(0); // will disable alpha from texturing
      
      //glDisable(GL_TEXTURE_2D);
	  
	  state.glState->textureDefined = 0;

#if 0      
      GvMaterial *material=NULL; 
	  GvMaterialElement *materialTop = (GvMaterialElement*) state.Get(GvMaterialElement::attrId); 
	  
	  // reset material color
	  if (materialTop) {
   	  	Point c;
   	  	materialTop->GetDiffuse(0,c);
	  	glColor(c);
	  }
#endif

	  return(0);
  }
#endif
  // simliar to GvTexture2:glSet ...

#ifdef _D3D
	if (!state.glState->SetTexture(texture)) {
			if (!state.glState->ReloadTexture(texture)) {
			   state.glState->SetTexturing(0); 
			   state.glState->textureDefined = 0;
			   return(0);

			} else  {
				state.glState->textureDefined
				= state.glState->texturing = state.glState->hTex != 0;
			}
	}
	else {
	    state.glState->textureDefined = 
		state.glState->texturing = state.glState->hTex != 0;

	}
	if (state.glState->texturing) {
 	  
	    state.glState->SetTextureWrap(repeatS, repeatT);

		ret |= TEXTURE_DEFINED;
		
		if (alpha) {
				RenderState &glState = *state.glState;

				// enable alphafunc
                gbool _alpha = glState.alphaTest;
                gbool _blend = glState.blend;
                if (!_alpha || !_blend) 
				{
                    // no blending done, enable alpha func skipping pixels with alpha = 0.0    
                    GLint _alphaFunc=glState.alphaFunc;
                    GLfloat _alphaRef=glState.alphaRef;

                    // change state
					if (hasColorKey) { // no blending done, enable alpha func skipping pixels with alpha = 0.0    
						
						if (glState.canDoAlphaTest) {

							//0.75 // skip if more then 3 neighbours transparent but looing detail
							glState.SetAlphaFunc(GL_GREATER,0.6); // skip if more then x neighbours transparent
							glState.SetAlphaTestFromTexture(TRUE);
							if (glState.needBlendForCKey) {
								glState.SetBlendFromMaterial(TRUE);
								ret |= TEXTURE_HAS_ALPHA;
							}

						} else 
							glState.SetBlendFromMaterial(TRUE); // also in material apply 

					} else {
						if (glState.canDoAlphaTest) {
							glState.SetAlphaFunc(GL_GREATER,0.0); // could alpha test anyway 
							glState.SetAlphaTestFromTexture(TRUE);
						}
						glState.SetBlendFromMaterial(TRUE); // also in material apply 
					}

				}	
				
				if (hasColorKey) {
					ret |= TEXTURE_HAS_COLORKEY;
					if (glState.needBlendForCKey) {
						ret |= TEXTURE_HAS_ALPHA;
					}
				}
				// should be do sorted blend with color keys, problems with some billboards and sorting ??
				else if (hasAlpha) // ** to check DX3 ** 
					ret |= TEXTURE_HAS_ALPHA;

		}
		else state.glState->SetAlphaTestFromTexture(FALSE); 
		if (isGrayscale) 
			ret |= TEXTURE_MODULATE;
	}

	return(ret);
#endif


  return(0);

}


int 			
GvTexture::glSet(GglRenderTraversal &state)
{
   // abstract
   return(0);
}

#ifdef _G_VRML1

int 			
GvTexture2::glSet(GglRenderTraversal &state)
{
	return GvTexture::glSet(state,wrapS==(int)GvTexture2::REPEAT, wrapT==(int)GvTexture2::REPEAT,
		HasTransparency(),state.materialStatus & MATERIAL_HAS_ALPHA);
}

#endif _G_VRML1



int 			
GvTexture::Do(GglRenderTraversal &state)
{
  // if no texture mapping ignore node
  if (state.useTextures) 
	  return(0);
  int ret;
  if (!Defined()) {
	  ret = Define(state);
  }
  return(ret);
}


int GvTexture::Define(GglRenderTraversal &state) 
{
	TRACE("Error:Abstract %s ::Define(GglRenderTraversal) called \n",this->ClassName());
	defined = TRUE;
	return(0);
}


int GvTexture::Define(GglRenderTraversal &state,int glFormat,int width,int height,int numComponents,unsigned char *	data)
{	
	int ret;

	ret = state.glState->SetTexture(texture,glFormat,width,height,numComponents,data); 
	hasAlpha = texture.HasAlpha();
	hasColorKey = texture.HasColorKey();
	isGrayscale = texture.IsGrayscale();
	defined = 1;

	return(ret);
}

int GvTexture::Define(GglRenderTraversal &state,image *im,GvBool checkScale)
{	
	int ret;
	
	ret = state.glState->SetTexture(texture,im,checkScale); 
	hasAlpha = texture.HasAlpha();
	hasColorKey = texture.HasColorKey();
	isGrayscale = texture.IsGrayscale();
	defined = 1;

	return(ret);
}


int GvTexture::Define(GglRenderTraversal &state,GvSFImage *image)
{   int ret=-1;

	if (image && image->size[0]>0 && image->size[1]>0 
		&& image->numComponents >0
		) {
			int format ;
			switch(image->numComponents) {
			default :
			case 1 : format = GL_LUMINANCE; break;
			case 2 : format = GL_LUMINANCE_ALPHA; break;
			case 3 : format = GL_RGB;	break;
			case 4 : format = GL_RGBA; break;
			}

		    ret = Define(state,format,image->size[0],image->size[1],
		   				image->numComponents, 
		   				image->bytes
						);
	} else {
		glFlushCache();
		defined = TRUE;
		hasAlpha = FALSE;
		hasColorKey = FALSE;

		ret = 1; // was 0 
	}
	//if (ret <=0) glFlushCache();
	return(ret);
}

// Define texture by reading texture file + create display list 
int 			
GvTexture::Define(GglRenderTraversal &state,
				  LPCTSTR fileName,
				  LPCTSTR fileUrl,
				  int fileFmt,GvBool reload)
{
    int ret=0;

	glFlushCache();

	ret = state.glState->SetTexture(texture,fileName,fileUrl,fileFmt,reload); 
	hasAlpha = texture.HasAlpha();
	hasColorKey = texture.HasColorKey();
	isGrayscale = texture.IsGrayscale();

	defined = TRUE;
    return(ret);
}


int 			
GvTexture::TryDefineByUrl(GglRenderTraversal &state,
				  LPCTSTR fileUrl,
				  GvBool reload)
{
    int ret=0;

	//glFlushCache();

	if (state.glState->FindTextureByUrl(texture,fileUrl)) {
		// yup we have it, shared now & no download
		hasAlpha = texture.HasAlpha();
		hasColorKey = texture.HasColorKey();
		isGrayscale = texture.IsGrayscale();
		defined = TRUE;
		ret=1;
	}
    return(ret);
}



//
// GvImageTexture
// 

GV_NODE_SOURCE_IMP(GvImageTexture,GvTexture);

GvImageTexture::GvImageTexture() : 
		url(0), repeatS(TRUE), repeatT(TRUE)
{
    GV_NODE_CONSTRUCTOR(GvImageTexture);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(url);
    GV_NODE_ADD_FIELD(repeatS);
    GV_NODE_ADD_FIELD(repeatT);

	// extra
    GV_NODE_ADD_EVENT_IN(set_unload,GvSFBool);
    GV_NODE_ADD_EVENT_OUT(isLoaded);

}

GvImageTexture::~GvImageTexture()
{
 	glFlushCache();
}

int 			
GvImageTexture::glSet(GglRenderTraversal &state)
{

#ifdef _OGL
	// TEEEEEEEEEEEEESTTT 

		// we need to redefine it 
	if (!texture.Ok()) {
		TRACE("GvImageTexture::glSet %d...\n",texture.Ok());
		int ret = Define(state);
		ret = GvTexture::glSet(state,repeatS.value,repeatT.value,HasTransparency(),state.materialStatus & MATERIAL_HAS_ALPHA);
		ret |= 	TEXTURE_CHANGED;
		return ret;
    }
#endif
	
	return(GvTexture::glSet(state,repeatS.value,repeatT.value,HasTransparency(),state.materialStatus & MATERIAL_HAS_ALPHA));
}

int GvImageTexture::Unload()
{	int ret=0;

	SetDefined(FALSE);
	glFlushCache();
	loader.SetNotLoaded();
	isLoaded.set(FALSE);
	return ret;

}

int GvImageTexture::Define(GglRenderTraversal &state)
{
	// empty
	if (url.Length()==0) {
		//GvTexture::Define(state,(GvSFImage *) NULL);
		return ((GvTexture *)this)->Define(state,(GvSFImage *) NULL);
    }
	return Load(state);
}

// a field has been changed
int GvImageTexture::OnFieldChanged(GvField *field)
{
	setRedraw();

    if (field == &url) {
		TRACE("GvImageTexture::OnFieldChanged(&name)\n");
		SetDefined(FALSE);
		//glFlushCache();
		loader.SetNotLoaded();
	}
	else 	if (field == &isLoaded) {
		return(0);
    } 

	return GvNode::OnFieldChanged(field);
}

void GvImageTexture::set_unload(GvSFBool *mode)
{
    TRACE("%s::set_unload \n",ClassName());

	if (! mode->get()) return;

	Unload();
	setRedraw();
	
}

int 			
GvImageTexture::Load(GglRenderTraversal &state)
{
	// whats the current status
	GFileState status = loader.GetState();

Redo:
	switch(status) {
	case S_BAD_DATA:
	case S_BAD_URL:
		if (loader.urlI< (url.Length() -1) ) {
			loader.urlI ++;
			// fall through and load  next 
		} else {
			status = loader.state = S_ALL_BAD;
			isLoaded.set(FALSE);
		    break;
		}
	case S_NOT_LOADED: {
	     if (url.Length() == 0) { 
			 loader.SetNoUrl();		
			 isLoaded.set(FALSE);
			 return(0);
		 }
		 if (!loader.HasBaseUrl())
			loader.SetBaseUrl(state.GetHomeUrl());

		/// xxxxxxxx try to see if URL texture already loaded !!!!!!!!!
		GvBool reload=gfalse;

		CString extraArg;
	 	CString theUrl = url[loader.urlI].getString();
		{
			CTime refreshTime(0);
			const char *refedFromUrl= state.GetUrl(refreshTime);
		 
			status = loader.SetupUrl(loader.GetBaseUrl(),theUrl,refedFromUrl,"ImageTexture",state.inlineWwwFlags,state.view->hWnd,loader.urlI,state.view->GetReporter(),state.view->file,extraArg);
		}
		if (( status <= S_LOADED) && (status != S_NOT_LOADED)) goto Redo; // download request already in progress
/*
		if (status == S_LOADING) { // pending load 
			state.pendingLoads ++;
			break;
		}
*/

		const char * s=loader.GetUrl();
		if (s)  {
			reload= (strstr(s,"/no_cache/") || strstr(s,"/cgi-bin/"));
			if (reload) {
				TRACE("Need to refresh texture url = %s \n",s);
			}
			else {
				int ret = ((GvTexture *) this)->TryDefineByUrl(state,s,reload);
				if (ret>0) {
					loader.SetProcessed();
					isLoaded.set(TRUE);
					return(1);
				}	
			}
		}
		loader.Release();

		{
		 CTime refreshTime(0);
		 const char *refedFromUrl= state.GetUrl(refreshTime);
		 status = loader.Load(loader.GetBaseUrl(),url[loader.urlI].getString(),refedFromUrl,"ImageTexture",state.inlineWwwFlags,state.view->hWnd,loader.urlI,refreshTime,state.view->GetReporter(),state.view->file);
		 }

		 if (status != S_NOT_LOADED) goto Redo;

		 }
		 break;

	case S_LOADING:
	case S_PROCESSING:
				state.pendingLoads ++;
				break;

	case S_LOADED: {
				int ret;
		
				if (state.view && (state.view->lastFrameTextureLoadCnt >= state.view->maxFrameTextureLoadCnt))  // make a small pause
				{
					return 0; // delay
				}


				//glFlushCache(); // need to release handle to possibly shared texture

				GvBool reload;
				const char * s=loader.GetUrl();
				reload= (strstr(s,"/no_cache/") || strstr(s,"/cgi-bin/"));
				if (reload)
					TRACE("Need to refresh texture url = %s \n",s);

				ret = ((GvTexture *) this)->Define(state,loader.GetLocalFile(),loader.GetUrl(),loader.GetLocalFileFmt(),reload);
				if (ret>0) {
					loader.SetProcessed();
					isLoaded.set(TRUE);
				} else { // bad image format 
					GReporter* reporter =state.view->GetReporter();

					if (reporter && reporter->verbose)
						reporter->Warning("Missing or bad image file \"%s\" (%d)",(const char *)loader.GetUrl(),ret);
					// try to delete 
					loader.DeleteCachedLocalFile();

					loader.SetBadData();
					isLoaded.set(FALSE);
				}
				if (state.view) {
					state.view->lastFrameLoadCnt++;
					state.view->lastFrameTextureLoadCnt++;
					if (ret<=0) state.view->lastFrameLoadErrorCnt++;
				}
				return(1);
			}
	case S_PROCESSED:
				return(1);
	default :
		break;

	}
	return(0);
}

// just load any URL's
int 			
GvImageTexture::Load(GTraversal &state)
{
	// whats the current status
	GFileState status = loader.GetState();

Redo:
	switch(status) {
	case S_BAD_DATA:
	case S_BAD_URL:
		if (loader.urlI< (url.Length() -1) ) {
			loader.urlI ++;
			// fall through and load  next 
		} else {
			status = loader.state = S_ALL_BAD;
			isLoaded.set(FALSE);
		    break;
		}
	case S_LOADING:
	case S_PROCESSING:
				state.pendingLoads ++;
				break;
	case S_NOT_LOADED:
	     if (url.Length() == 0) { 
			 loader.SetNoUrl();		
			 isLoaded.set(FALSE);
			 return(0);
		 }
		 if (!loader.HasBaseUrl())
			loader.SetBaseUrl(state.GetHomeUrl());
		 {
		 CTime refreshTime(0);
		 const char *refedFromUrl= state.GetUrl(refreshTime);

		 status = loader.Load(loader.GetBaseUrl(),url[loader.urlI].getString(),refedFromUrl,"ImageTexture",state.inlineWwwFlags,state.view->hWnd,loader.urlI,refreshTime,state.view->GetReporter(),state.view->file);

		 }
		 if (status != S_NOT_LOADED) goto Redo;
		 break;
	case S_LOADED: {
				// Don't go to PROCESSED state 
				return(1);
			}
	case S_PROCESSED:
				return(1);
	default :
		break;

	}
	return(0);
}

int GvImageTexture::Do(GLoadInlinesTraversal &state)
{

   if (state.resolveRelativeUrls && !loader.HasBaseUrl()) {
		loader.SetBaseUrl(state.GetHomeUrl());
   }


	if (!loader.Ok() && state.loadTextures >= GTraversal::LOAD_IF_NEEDED) {
		  Load(state);
	}	

	if (loader.Ok()) { 
	  return(1);
	}


	return(0);
}

int GvImageTexture::Do(GglRenderTraversal &state)
{

	if (!loader.Ok() && state.loadTextures >= GTraversal::LOAD_IF_NEEDED) {
		  Load(state);
	}	

	return(0);
}



//
// GvPixelTexture
// 

GV_NODE_SOURCE_IMP(GvPixelTexture,GvTexture);

GvPixelTexture::GvPixelTexture() : 
		 repeatS(TRUE), repeatT(TRUE)
{
    GV_NODE_CONSTRUCTOR(GvPixelTexture);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(image);
    GV_NODE_ADD_FIELD(repeatS);
    GV_NODE_ADD_FIELD(repeatT);
    image.size[0] = image.size[1] = 0.0;
    image.numComponents = 0;
    image.bytes = NULL;
}

GvPixelTexture::~GvPixelTexture()
{
}

int 			
GvPixelTexture::glSet(GglRenderTraversal &state)
{
	// we need to redefine it 
	if (!texture.Ok()) {
		TRACE("GvPixelTexture::glSet %d...\n",texture.Ok());
		int ret = Define(state);
		ret = GvTexture::glSet(state,repeatS.value,repeatT.value,HasTransparency(),state.materialStatus & MATERIAL_HAS_ALPHA);
		ret |= 	TEXTURE_CHANGED;
		return ret;
    }		

	return(GvTexture::glSet(state,repeatS.value,repeatT.value,HasTransparency(),state.materialStatus & MATERIAL_HAS_ALPHA));
}


int 			
GvPixelTexture::Do(GglRenderTraversal &state)
{
  // if no texture mapping ignore node
  if (state.useTextures) 
	  return(0);
  int ret;
  if (!Defined() || (Defined() && !texture.Ok())) {
	  ret = Define(state);
  }
  return(ret);
}


int GvPixelTexture::Define(GglRenderTraversal &state)
{
	return ((GvTexture *) this)->Define(state,&image);
}

GvSFImage* GvPixelTexture::getImage(GTraversal &state)
{
   return( &image);
}

// a field has been changed
int GvPixelTexture::OnFieldChanged(GvField *field)
{
	setRedraw(); // ?? done in Appearance 

	if (field == &image) {
		TRACE("GvPixelTexture::OnFieldChanged(&name)\n");
		SetDefined(0);
		glFlushCache();
	}
	return GvNode::OnFieldChanged(field);
}


//
// GvMovieTexture
// 

GV_NODE_SOURCE_IMP(GvMovieTexture,GvTexture);

GvMovieTexture::GvMovieTexture() : 
		 loop(FALSE),
		 speed(1),
		 url(0), repeatS(TRUE), repeatT(TRUE),
		 duration(0),isActive(FALSE),
		 cycleInterval(1.0f) // assume for now 
{
    GV_NODE_CONSTRUCTOR(GvMovieTexture);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(loop);
    GV_NODE_ADD_FIELD(speed);
    GV_NODE_ADD_EVENT_IN(set_startTime,GvSFTime);
    GV_NODE_ADD_EVENT_IN(set_stopTime,GvSFTime);
    
	GV_NODE_ADD_EVENT_IN(set_pause,GvSFBool);
    
	GV_NODE_ADD_FIELD(startTime);
    GV_NODE_ADD_FIELD(stopTime);

    GV_NODE_ADD_FIELD(url);
    GV_NODE_ADD_FIELD(repeatS);
    GV_NODE_ADD_FIELD(repeatT);

    GV_NODE_ADD_EVENT_OUT(duration);
    GV_NODE_ADD_EVENT_OUT(isActive);

    GV_NODE_ADD_EVENT_OUT(mediaTime);


	
	isRegistered = FALSE;

	nextScheduleTime = 0;
	last_setStartTime=0;
	last_setStartTimeTime=0;
	
	curFrame = 0;
	framesPerSec = 1;
	numFrames = 1;
	movieType = GMT_NONE;
	imageMovie = NULL;
	displayedFrame = -1;

	dsMovie = NULL;
	rmaPlayer = NULL;

	updateAudio = updateVideo = FALSE;

}

GvMovieTexture::~GvMovieTexture()
{
	TRACE("GvMovieTexture::~GvMovieTexture() \n");
	Term();

}

// release MovieTexture resources
void GvMovieTexture::Term()
{
	TRACE("GvMovieTexture::Term() \n");

 	glFlushCache();
	if (imageMovie) {
		delete imageMovie;
		imageMovie = NULL;
	}

#ifdef G_AMOVIE
	if (dsMovie) {
		dsMovie->Close();
		delete dsMovie;
		dsMovie = NULL;
	}
#endif

#ifdef G_RMA_MOVIE
	if (rmaPlayer) {
		rmaPlayer->Term();
		delete rmaPlayer;
		rmaPlayer = NULL;
	}
#endif
	movieType = GMT_NONE;

}

// release surface because device eg has been changed 
void GvMovieTexture::ReleaseSurface()
{
	TRACE("GvMovieTexture::ReleaseSurface() \n");

 	glFlushCache();
#if 0
	if (imageMovie) {
		delete imageMovie;
		imageMovie = NULL;
		movieType = GMT_NONE;
	}

#ifdef G_AMOVIE
	if (dsMovie) {
		dsMovie->Close();
		delete dsMovie;
		dsMovie = NULL;
		movieType = GMT_NONE;
	}
#endif

#endif


#ifdef G_RMA_MOVIE
	if (rmaPlayer) {
		rmaPlayer->ReleaseSurface();
	}
#endif

}


void GvMovieTexture::set_startTime(GvSFTime *t)
{
	// TRACE("GvMovieTexture %p:: set_startTime t=%g active =%d \n",this,t->value,(int)isActive.value);	

	if (isActive) {
		// ignore
		last_setStartTime = t->value;
		last_setStartTimeTime = getTime();
	}
	else 
    {
		startTime.set(*t);
		nextScheduleTime = *t;
	}
}

void GvMovieTexture::set_stopTime(GvSFTime *t)
{
	// TRACE("GvMovieTexture %p::set_stopTime t=%g active =%d \n",this,t->value,(int)isActive.value);	
	if (isActive) {

		if (last_setStartTimeTime == getTime() && last_setStartTime == t->value) {
			startTime.set(*t);
			nextScheduleTime = *t;
			TRACE("TimeSensor %p:: should restart set_startTime t=%g active =%d \n",this,t->value,(int)isActive.value);	
		}
		if (*t < startTime) { // was <=
			// ignore
		}
		else {
			stopTime.set(*t);
			// really, make him inactive ???? 
			setActive(FALSE);
		}
	
	}
	else {
		stopTime.set(*t);
	}
}

void GvMovieTexture::set_pause(GvSFBool *t)
{
	GvBool paused=t->get();

	TRACE("GvMovieTexture %p::set_pause t=%g active =%d \n",this,(int) paused,(int)isActive.value);	

	if (isActive) {
	switch (movieType) {
	case GMT_IMAGE_MOVIE: 
		{
		}
		break;
#ifdef G_AMOVIE

	case GMT_AMOVIE : 
		{
		}
		break;	
#endif

#ifdef G_RMA_MOVIE

	case GMT_RMA_MOVIE : 
		{
			if (paused) rmaPlayer->Pause();
			else rmaPlayer->Play();

		}
		break;	
#endif

	default:
		break;
	}
	}
}



int GvMovieTexture::ComputeFrameNr( double t) 
{
		if (stopTime > startTime) {
			if (t>=stopTime)
				t = stopTime;
		}
		t -= startTime;
		t = fmod(t,cycleInterval);

		long frame = t * framesPerSec * speed;
		if (frame <0) return 0;
		if (frame >= numFrames)
			if (!loop) frame = numFrames - 1;
			else frame = frame % numFrames;
		return frame;
	}

// compute the current requested frame number
int GvMovieTexture::SetCurrentFrame(double t) 
{
	int ret = 0;
	int newFrame;

	switch (movieType) {
	case GMT_IMAGE_MOVIE: 
		{
			t -= startTime;
			t *= speed;
			t = fmod(t,cycleInterval);
			newFrame = imageMovie->GetFrameNr(t); 
			if (newFrame != curFrame) {
				ret = Defined(); // need redraw
				curFrame = newFrame;
				updateVideo = TRUE;
				int pt=imageMovie->times[curFrame];

				if (isActive) mediaTime.set((double) pt /1000.0);

			}
			//ComputeFrameNr(t);
		}
		break;
#ifdef G_AMOVIE

	case GMT_AMOVIE : 
		{
			gbool eos= FALSE;
			TRACE("Amovie set current frame isActive %d\n",(int) isActive.get());

			if (!isActive) { // isActive == FALSE
				updateVideo = FALSE;
				if (dsMovie->state == STREAMSTATE_RUN) {
					TRACE("stopping stream \n");
					dsMovie->SetState(STREAMSTATE_STOP);
					if (dsMovie->Seek(0)) { // rewind 
					}
				}
			}
			else { // isActive == TRUE

				if (!dsMovie->state == STREAMSTATE_RUN) {
					TRACE("Starting stream ..........\n");
					dsMovie->SetState(STREAMSTATE_RUN);
				}

#if 1

				if (!dsMovie->UpdateSample(updateVideo)) {
					if (dsMovie->hr == MS_S_ENDOFSTREAM) {
					TRACE("MS_S_ENDOFSTREAM  stream ..........\n");
					eos = TRUE;

/*
					dsMovie->SetState(STREAMSTATE_STOP);
					if (loop) {
						if (dsMovie->Seek(0)) {
							TRACE("Seeking to start \n"); 							
							dsMovie->SetState(STREAMSTATE_RUN);
							dsMovie->UpdateSample();
						}
						else { TRACE("Can't seek back \n"); }
					} else {
					}
*/
					}
					else {
						TRACE("Update sample failed  %d \n",dsMovie->hr);
					}
				// updateVideo = FALSE;
				}
#endif

			STREAM_TIME time;
			double pt=0.0;
			if (dsMovie->GetTime(time)) {
					pt = Millisecs(time);
					pt *= 0.001;

			}

			if (dsMovie->state == STREAMSTATE_RUN) {
/*
			if (WaitForSingleObject(dsMovie->hEOS, 0) == WAIT_OBJECT_0) 
			{
				eos = TRUE;
				ResetEvent(dsMovie->hEOS);	// new 
			}
*/

				if (eos) {
					TRACE("End of stream \n");
					//dsMovie->SetState(STREAMSTATE_STOP);
					if (loop) {
						if (dsMovie->Seek(0)) {
							TRACE("Seeking to start \n"); 							
							//dsMovie->SetState(STREAMSTATE_RUN);
							dsMovie->UpdateSample(updateVideo);
							if (dsMovie->GetTime(time)) {
								pt = Millisecs(time);
								pt *= 0.001;
							}

						}
						else { TRACE("Can't seek back \n"); }
					}
					else {
						dsMovie->SetState(STREAMSTATE_STOP);
						// setActive(FALSE);
						isActive.set(FALSE);
					}

				}
			}
			
			if (isActive) mediaTime.set(pt);
			if (updateVideo)
				setRedraw();

			} // isActive
		}
		break;	
#endif

#ifdef G_RMA_MOVIE

	case GMT_RMA_MOVIE : 
		{
			gbool eos= FALSE;
			TRACE("RMA set current frame isActive %d\n",(int) isActive.get());

			if (!isActive) { // isActive == FALSE
				updateVideo = FALSE;
				if (rmaPlayer->IsPlaying()) {
					TRACE("stopping stream \n");
					rmaPlayer->Stop();
/*					if (rmaPlayer->Seek(0)) { // rewind 
					}
*/
				}
			}
			else { // isActive == TRUE

				if (!rmaPlayer->IsPlaying()) {
					TRACE("Starting stream ..........\n");
					rmaPlayer->Play();
				}
#if 1
				if (!rmaPlayer->UpdateSample(updateVideo)) {
					if (rmaPlayer->IsDone()) {
						TRACE("EOF  stream ..........\n");

					}
					else {
						TRACE("Update sample failed   \n");
					}
				// updateVideo = FALSE;
				}
#endif
				ULONG32 pt= rmaPlayer->GetCurrentTime();

				eos = rmaPlayer->IsDone();
				if (eos) {
					TRACE("End of stream \n");
					rmaPlayer->Stop();
					if (loop) {
						if (rmaPlayer->Seek(0)) {
							TRACE("Seeking to start \n"); 							
							rmaPlayer->Play();
							rmaPlayer->UpdateSample(updateVideo);
							pt= rmaPlayer->GetCurrentTime();

						}
						else { TRACE("Can't seek back \n"); }
					}
					else {
						rmaPlayer->Stop();
						// setActive(FALSE);
						isActive.set(FALSE);
					}
				}

				if (isActive) mediaTime.set((double) pt /1000.0);

			} // isActive
		}
		break;	
#endif

	default:
		newFrame = 0;
		if (newFrame != curFrame) {
			ret = Defined(); // need redraw
			curFrame = newFrame;
		}
		break;
	}

	return ret;
}

int GvMovieTexture::OnTimeChange(double t) 
{
	int ret=0;

		GvBool wasActive = isActive;

		if (wasActive) ret = 1;
/*
        if (!enabled) {
            setActive(FALSE);
            return(SetCurrentFrame(t));
        }
 */       
        if (t<startTime) {
			nextScheduleTime = startTime.value;
            setActive(FALSE);
            return(SetCurrentFrame(t));
        }
        
        double tl= t-startTime;


        if (stopTime > startTime) {  // stopTime given 
			if (t> (stopTime+0.0000001))  
			{
				if (wasActive) // create a event at stop Time 
				{
					setActive(FALSE);
				}
				else nextScheduleTime = stopTime;
				return(SetCurrentFrame(t));
			}
		}
        
		double tm = fmod(tl,cycleInterval);

        
        if (loop) 
		{
			setActive(TRUE);
        } 
		else 
		{
			// not loop 
            if (tl > cycleInterval) { 
			
				if (wasActive) { // cycle expired  // do we missed the exact end ????
					setActive(FALSE);
				}
            } else {
                setActive(TRUE);
            }

        }    
		return(SetCurrentFrame(t));
}


int 			
GvMovieTexture::glSet(GglRenderTraversal &state)
{
	switch (movieType) {
	case GMT_IMAGE_MOVIE: 
		{
			if (updateVideo || !texture.Ok()) {
				// TRACE("MT:changing to frame %d \n",curFrame);
				image *im = imageMovie->images[curFrame];
				GvTexture::Define(state,im,FALSE);
				displayedFrame = curFrame;
				updateVideo = FALSE;
			    return (GvTexture::glSet(state,repeatS.value,repeatT.value,HasTransparency(),state.materialStatus & MATERIAL_HAS_ALPHA)) | TEXTURE_CHANGED;
			}
			else return(GvTexture::glSet(state,repeatS.value,repeatT.value,HasTransparency(),state.materialStatus & MATERIAL_HAS_ALPHA));
		}
		break;

#ifdef G_RMA_MOVIE

	case GMT_RMA_MOVIE : {
				int ret;

				if (rmaPlayer->m_sampleReady) 
				{
					TRACE("RMA:video sample ready during render \n");
					updateVideo = true;
					rmaPlayer->m_sampleReady = false;
				}
	
					// HRESULT res = state.glState->device->lpBackBuffer->Blt(&dsMovie->rect, dsMovie->pSurface, &dsMovie->rect, DDBLT_WAIT, NULL);
					if (isActive)
					if (updateVideo || !texture.Ok()) {

						GDSurfaceInfo *surf = rmaPlayer->GetSurfaceInfo();
						if (surf && surf->m_pSurface != NULL ) {

							if (waitingForFirstFrame) { // splash screen 
								texture.Release(); 
								waitingForFirstFrame = gfalse;
							}

							if (!texture.Ok() || (surf->m_targetRect.right==0)) {
								int width = surf->m_rect.right; // -surf->m_rect.left;
								int height= surf->m_rect.bottom; // -surf->m_rect.bottom;
								TRACE("RMA:New video config : %d x %d  ",width,height);
								// map to nearest 2 power 
								state.glState->GetTextureTargetSize(width,height,256);
								surf->m_targetRect.left =0;
								surf->m_targetRect.top =0;
								surf->m_targetRect.right =width;
								surf->m_targetRect.bottom = height;
								TRACE("scaled to  %d x %d  \n",width,height);

								// to think : update texture format in case of device change
								#if 0 // def _D3D // test code 
								// set pointer to direct draw context
								rmaPlayer->SetDDraw(state.glState->lpDD);

								// find some good rgb texture format
								DDPIXELFORMAT texFormat;
								texFormat.dwFlags=DDPF_RGB,texFormat.dwRGBBitCount=24,texFormat.dwRBitMask=0xff0000,texFormat.dwGBitMask=0xff00,texFormat.dwBBitMask=0xff;


								int format = state.glState->FindTextureFormat(texFormat, rmaPlayer->m_ddsd.ddpfPixelFormat);

								texFormat = rmaPlayer->m_ddsd.ddpfPixelFormat;
								TRACE("tex Format  PixelF flags %d bits %d (%x %x %x) \n",texFormat.dwFlags,texFormat.dwRGBBitCount,texFormat.dwRBitMask,texFormat.dwGBitMask,texFormat.dwBBitMask);
								#endif

							}

#ifdef _D3D
							ret = state.glState->SetTexture(texture,surf->m_pSurface,surf->m_pPalette,
								surf->m_desc,surf->m_rect,TRUE,&surf->m_targetRect); 
#endif
#ifdef _OGL
							// get image from DD surface
							image im;
							image_zero(&im);
			/* to do: optimize
							surface lock , set_image_byRef or use D3D scaling 
							int image_set_fmt_by_ref(&im,IM_RGB,widht,height,data);
			*/
							if (ImageSet(im, surf->m_pSurface,surf->m_pPalette,surf->m_desc,surf->m_rect)) {
								int width2=surf->m_targetRect.right;
								int height2=surf->m_targetRect.bottom;
								image_scale(&im,width2,height2,FALSE); 
								// to do: scale using DDRAW, image_scale_copy div & memcpy takes much time !!!!!!
								GvTexture::Define(state,&im,FALSE);
							}
							image_free_all(&im);


#endif

							hasAlpha = texture.HasAlpha();
							hasColorKey = texture.HasColorKey();
							isGrayscale = texture.IsGrayscale();
							defined = 1;
							updateVideo = FALSE;
						}
						else {
							 TRACE("RMA: no surfaces yet, failed with HRESULT(0x%8.8X)\n", rmaPlayer->m_hr);
	
						}
						rmaPlayer->UpdateSample(updateVideo);
					}

					return (GvTexture::glSet(state,repeatS.value,repeatT.value,HasTransparency(),state.materialStatus & MATERIAL_HAS_ALPHA)) | TEXTURE_CHANGED;


				break;
				};
#endif

#ifdef G_AMOVIE
	case GMT_AMOVIE : {
				// define Texture by surface 
				{
					int ret=0;
					//HRESULT res = dsMovie->pTargetSurface->Blt(&dsMovie->rect, dsMovie->pSurface, &dsMovie->rect, DDBLT_WAIT, NULL);
					// RECT destRect;
					if (dsMovie->waitingForSample && WaitForSingleObject(dsMovie->sampleEvent, 0) == WAIT_OBJECT_0) 
					{
						TRACE("video sample ready \n");
						dsMovie->hr = S_OK;
						updateVideo = true;
						dsMovie->waitingForSample = false;
					}
					// else updateVideo = false;

#ifdef _D3D

					// HRESULT res = state.glState->device->lpBackBuffer->Blt(&dsMovie->rect, dsMovie->pSurface, &dsMovie->rect, DDBLT_WAIT, NULL);
					if (isActive)
					if (updateVideo || !texture.Ok()) {
//XX						dsMovie->hr = dsMovie->pSample->CompletionStatus(0,0); // COMPSTAT_WAIT ,2);


						if (dsMovie->hr == S_OK) {
							ret = state.glState->SetTexture(texture,dsMovie->pSurface,dsMovie->pDirectDrawPalette,dsMovie->ddsd,dsMovie->rect,TRUE); 

							hasAlpha = texture.HasAlpha();
							hasColorKey = texture.HasColorKey();
							isGrayscale = texture.IsGrayscale();
							defined = 1;
							updateVideo = FALSE;
						}
						else {
							 TRACE("CompletionStatus video failed with HRESULT(0x%8.8X)\n", dsMovie->hr);
	
						}
						dsMovie->UpdateSample(updateVideo);
					}

#endif
#ifdef _OGL

					if (updateVideo) {

						if (dsMovie->hr == S_OK) {
							// get image from DD surface
							image im;
							image_zero(&im);
/* to do: optimize
							surface lock , set_image_byRef or use D3D scaling 
							int image_set_fmt_by_ref(&im,IM_RGB,widht,height,data);
*/
							if (ImageSet(im, dsMovie->pSurface,dsMovie->pDirectDrawPalette,dsMovie->ddsd,dsMovie->rect)) {
								image_scale(&im,dsMovie->width2,dsMovie->height2,FALSE); 
								// to do: scale using DDRAW, image_scale_copy div & memcpy takes much time !!!!!!
								GvTexture::Define(state,&im,FALSE);
							}
							image_free_all(&im);

/*

							hasAlpha = texture.HasAlpha();
							hasColorKey = texture.HasColorKey();
							isGrayscale = texture.IsGrayscale();
							defined = 1;
*/
							updateVideo = FALSE;
						}
						else {
							 TRACE("CompletionStatus video failed with HRESULT(0x%8.8X)\n", dsMovie->hr);
	
						}
						dsMovie->UpdateSample(updateVideo);

					}

#endif

					return (GvTexture::glSet(state,repeatS.value,repeatT.value,HasTransparency(),state.materialStatus & MATERIAL_HAS_ALPHA)) | TEXTURE_CHANGED;

				}

				break;
				};
#endif
	case GMT_IMAGE:
	default :
		return(GvTexture::glSet(state,repeatS.value,repeatT.value,HasTransparency(),state.materialStatus & MATERIAL_HAS_ALPHA));
	}
	return(0);
}


int GvMovieTexture::Define(GglRenderTraversal &state)
{
	// empty
	if (url.Length()==0) {
		GvTexture::Define(state,(GvSFImage *) NULL);
		return(1);
    }
	return Load(state);
}

GvMovieTexture::Define(GglRenderTraversal &state, LPCTSTR fileName, LPCTSTR fileUrl, int fileFmt)
{   int ret = -1;

	framesPerSec = 3;
	numFrames = 1;
	displayedFrame = -1;
	
	Term();

	movieType = GMT_NONE;

	TRACE("Defining Movie Texture %s \n",fileName);

#ifdef G_RMA_MOVIE
	if (IsRmaFmt(fileName,fileFmt)) {
		rmaPlayer = new GRMAPlayer();
		// pointer to Direct  DRAW
		rmaPlayer->SetDDraw(state.view->GetDirectDraw());
		rmaPlayer->SetReporter(state.view->GetReporter());

#ifdef _D3D
		// set pointer to direct draw context
		//rmaPlayer->SetDDraw(state.glState->lpDD);

		// find some good rgb texture format
		DDPIXELFORMAT texFormat;
		texFormat.dwFlags=DDPF_RGB,texFormat.dwRGBBitCount=24,texFormat.dwRBitMask=0xff0000,texFormat.dwGBitMask=0xff00,texFormat.dwBBitMask=0xff;


		int format = state.glState->FindTextureFormat(texFormat, rmaPlayer->m_ddsd.ddpfPixelFormat);

		texFormat = rmaPlayer->m_ddsd.ddpfPixelFormat;
		TRACE("tex Format  PixelF flags %d bits %d (%x %x %x) \n",texFormat.dwFlags,texFormat.dwRGBBitCount,texFormat.dwRBitMask,texFormat.dwGBitMask,texFormat.dwBBitMask);
#else
		DDPIXELFORMAT texFormat;
		texFormat.dwFlags=DDPF_RGB,texFormat.dwRGBBitCount=24,texFormat.dwRBitMask=0xff0000,texFormat.dwGBitMask=0xff00,texFormat.dwBBitMask=0xff;
		rmaPlayer->m_ddsd.ddpfPixelFormat = texFormat;
#endif


		ret = -1;
		if (!rmaPlayer->Init()) {
			TRACE("Can't load Real Player \n");
			static bool grmaInstallError = gfalse;
			if (!grmaInstallError) { // report only once 
				state.view->GetReporter()->Error("Could not access REAL Player G2 for playing media\nPlease download and install from http://www.real.com");
				grmaInstallError = TRUE;
			}

		} else {
			char fileUrl[_MAX_PATH+1];
			makeFileUrl(fileName, fileUrl);
			TRACE("RMA OpenURL %s \n",fileUrl);

			if (rmaPlayer->OpenURL(fileUrl)) {
				double dur=1000.0;
				cycleInterval = dur;
				duration.set(dur);

				CString splashScreen = state.view->GetInstallDirectory();
				splashScreen += "firstrun.gif";
				if (ExistsFile(splashScreen))
					GvTexture::Define(state,splashScreen,NULL,GetFileType(splashScreen)); //GIF_FMT
				waitingForFirstFrame = gtrue;


#if 0 // defined(_WIN32) 
			// get location of rmacore from windows registry 
			DWORD bufSize;
			HKEY hKey; 
			PN_RESULT hRes;
    
			szDllName[0] = '\0'; 
			bufSize = sizeof(szDllName) - 1;

			if(ERROR_SUCCESS == RegOpenKey(HKEY_CLASSES_ROOT,
				"Software\\RealNetworks\\Preferences\\DT_Common", &hKey)) 
			{ 
				// get the path to pnen 
				hRes = RegQueryValue(hKey, "", szDllName, (long *)&bufSize); 
				RegCloseKey(hKey); 
			}

			strcat(szDllName, "pnen3260.dll");
#endif

				// duration changed, compute isActive state

				OnTimeChange(state.view->theTime); // GvNode::getTime());

				TRACE("Loaded video %s dur %g isActive %d \n", fileName,dur,isActive.get());

				if (isActive) 
					rmaPlayer->Play();

				movieType = GMT_RMA_MOVIE;
				ret = 0;

			}
		}

	}
	else

#endif

#ifdef G_AMOVIE


	if ( (fileFmt >= AVI_FMT  && fileFmt <= QT_FMT) || fileFmt == MPS_FMT
		 || fileFmt == WAV_FMT || fileFmt == AU_FMT) {
		ret = -1;
		
		dsMovie = new GDSStream();

		// pointer to Direct  DRAW
		dsMovie->pDD = state.view->GetDirectDraw();
		if (dsMovie->pDD) dsMovie->pDD->AddRef();

#ifdef _D3D
//		dsMovie->pTargetSurface = state.glState->device->lpBackBuffer;
//		dsMovie->pTargetSurface->AddRef();
#endif

		if (dsMovie->Open(fileFmt,fileName,*state.glState) == S_OK) {
			double dur = Millisecs(dsMovie->duration);
			dur *= 0.001;

			cycleInterval = dur;
			duration.set(dur);
			
			// duration changed, compute isActive state
			OnTimeChange(state.view->theTime); 

			TRACE("Loaded DS video %s dur %g isActive %d \n", fileName,dur,isActive.get());

			if (isActive)  {
				dsMovie->SetState(STREAMSTATE_RUN);
				dsMovie->UpdateSample(updateVideo);
			}

			movieType = GMT_AMOVIE;
			ret = 0;
		}
		
		if (ret <0) {
			movieType = GMT_NONE;
			delete dsMovie;
			dsMovie = NULL;
			curFrame = 0;
			displayedFrame = -1;
		}

	} else
#endif
	if (fileFmt == GIF_FMT) {
       // assume animated gif
		imageMovie = new GImageMovie();

		// set reader options 
		int formats =  state.glState->GetSupportedImageFormats();
		set_supported_image_formats(formats);

		imageMovie->supported_fmts = formats;
	
		imageMovie->scale2power = TRUE;
		imageMovie->minSizeX = state.glState->textureParameters.min_size_x;
		imageMovie->minSizeY = state.glState->textureParameters.min_size_y;
		imageMovie->maxSizeX = state.glState->textureParameters.current_max_size_x;
		imageMovie->maxSizeY = state.glState->textureParameters.current_max_size_y;
		imageMovie->square = state.glState->textureParameters.square_size;
		displayedFrame = -1;

		ret = imageMovie->ReadGif(fileName);

		if (ret >= 0 )
		{


			numFrames = imageMovie->NumFrames();

			//imageMovie->Scale2Power(state.glState->textureParameters.current_max_size_x,state.glState->textureParameters.current_max_size_y,state.glState->textureParameters.square_size);

			movieType = GMT_IMAGE_MOVIE;
			cycleInterval = imageMovie->GetDuration();
			duration.set(imageMovie->GetDuration());

			if (numFrames>0) {
				image *im = imageMovie->images[0];
				hasColorKey = im->colorKeyIndex >=0;


				//GvTexture::Define(state,im,FALSE);
				//displayedFrame = curFrame;
				//updateVideo = FALSE;

			}
			
		}
		else {
			movieType = GMT_NONE;
			delete imageMovie;
			imageMovie = NULL;
			curFrame = 0;
			displayedFrame = -1;
		}

	}
	else  { // single frame texture 
		curFrame = 0;
		ret = GvTexture::Define(state,fileName,fileUrl,fileFmt);
		movieType = GMT_IMAGE;
		cycleInterval = numFrames / framesPerSec;
		duration.set(numFrames / framesPerSec);
	}
	if (!isRegistered)
		state.getBrowser()->Register(this);

	return ret;
}

// a field has been changed
int GvMovieTexture::OnFieldChanged(GvField *field)
{
    if (field == &url) {
		TRACE("GvMovieTexture::OnFieldChanged(&name)\n");
		SetDefined(0);
		Term();
		loader.SetNotLoaded();
		setRedraw();
	}
	return GvNode::OnFieldChanged(field);

}

int 			
GvMovieTexture::Load(GglRenderTraversal &state)
{
	// whats the current status
	GFileState status = loader.GetState();

Redo:
	switch(status) {
	case S_BAD_DATA:
	case S_BAD_URL:
		if (loader.urlI< (url.Length() -1) ) {
			loader.urlI ++;
			// fall through and load  next 
		} else {
			status = loader.state = S_ALL_BAD;
		    break;
		}
	case S_LOADING:
	case S_PROCESSING:
				state.pendingLoads ++;
				break;
	case S_NOT_LOADED:
		{

		if (url.Length() == 0) { 
			 loader.SetNoUrl();		
			 return(0);
		 }
 		 if (!loader.HasBaseUrl())
			loader.SetBaseUrl(state.GetHomeUrl());
		
		 CString extraArg;
	 	 CString theUrl = url[loader.urlI].getString();

		 {
		 CTime refreshTime(0);
		 const char *refedFromUrl= state.GetUrl(refreshTime);	 
		 status = loader.SetupUrl(loader.GetBaseUrl(),theUrl,refedFromUrl,"MovieTexture",state.inlineWwwFlags,state.view->hWnd,loader.urlI,state.view->GetReporter(),state.view->file,extraArg);
		 }

		if (status == S_LOADING) { // pending load 
			state.pendingLoads ++;
			break;
		}

#ifdef G_RMA_MOVIE
	
		 // check for RMA && pass url, instead of downloading file
		 gbool isRma = gfalse;
		 gbool isRma_cached = gfalse;

		 const char *urlTmp = loader.GetUrl();
		 if (urlTmp) {

			isRma=IsRmaFmt(urlTmp,loader.GetLocalFileFmt());
			
			// download stuff to harddisk first ??
			if (strprefix(urlTmp,G_RMA_URN_CACHED)) {
				CTime refreshTime(0);
				const char *refedFromUrl= state.GetUrl(refreshTime);

				isRma_cached=gtrue;
				theUrl = (urlTmp + strlen(G_RMA_URN_CACHED));
				status = loader.SetupUrl(loader.GetBaseUrl(),theUrl,refedFromUrl,"MovieTexture",state.inlineWwwFlags,state.view->hWnd,loader.urlI,state.view->GetReporter(),state.view->file,extraArg);
			}
				

		 } else							 	 
			isRma=IsRmaFmt(loader.GetLocalFile(),loader.GetLocalFileFmt());

		 if (isRma && !isRma_cached) {
				// pass url directly to RMA
				int ret = this->Define(state,loader.GetUrl(),loader.GetUrl(),loader.GetLocalFileFmt());
				if (ret>=0) loader.SetProcessed();
				else { 
					GReporter* reporter =state.view->GetReporter();

					if (reporter && reporter->verbose)
						reporter->Warning("Missing or bad Real G2 file \"%s\" (%d)",(const char *)loader.GetUrl(),ret);

					loader.SetBadData();
					status = loader.GetState();

					goto Redo;
				}
				status = loader.GetState();
		 } 	
#endif

		 
		 if (( status <= S_LOADED) && (status != S_NOT_LOADED)) goto Redo; // download request already in progress
		 if (status == S_NOT_LOADED) {
			loader.Release();
			CTime refreshTime(0);
			const char *refedFromUrl= state.GetUrl(refreshTime);

			status = loader.Load(loader.GetBaseUrl(),theUrl,refedFromUrl,"MovieTexture",state.inlineWwwFlags,state.view->hWnd,loader.urlI,refreshTime,state.view->GetReporter(),state.view->file);
		 }
		 if (status != S_NOT_LOADED) goto Redo;
		}	
		break;

	case S_LOADED: {
				int ret;
				int fileFmt = loader.GetLocalFileFmt();
				// const char *urlTmp = loader.GetUrl();
				
				CString theUrl = url[loader.urlI].getString();

#ifdef G_RMA_MOVIE
				if (1) {
					// detect ovveride using _CACHED real media files
					// gbool isRma=IsRmaFmt(urlTmp,loader.GetLocalFileFmt());
					gbool isRma=IsRmaProtocol(theUrl);
					if (isRma) 
						fileFmt = RMA_FMT;
				}
#endif


				ret = this->Define(state,loader.GetLocalFile(),loader.GetUrl(),fileFmt);
				if (ret>=0)loader.SetProcessed();
				else { 
					GReporter* reporter =state.view->GetReporter();

					if (reporter && reporter->verbose)
						reporter->Warning("Missing or bad MovieTexture file \"%s\" (%d)",(const char *)loader.GetUrl(),ret);

					loader.SetBadData(); goto Redo; 
				}
				if (state.view) {
					state.view->lastFrameLoadCnt++;
					state.view->lastFrameTextureLoadCnt++;
					if (ret<0) state.view->lastFrameLoadErrorCnt++;
				}
				return(1);
			}
	case S_PROCESSED:
			return(1);
	default :
		break;

	}
	return(0);
}

// just load any URL's
int 			
GvMovieTexture::Load(GTraversal &state)
{
	// whats the current status
	GFileState status = loader.GetState();

Redo:
	switch(status) {
	case S_BAD_DATA:
	case S_BAD_URL:
		if (loader.urlI< (url.Length() -1) ) {
			loader.urlI ++;
			// fall through and load  next 
		} else {
			status = loader.state = S_ALL_BAD;
		    break;
		}
	case S_LOADING:
	case S_PROCESSING:
				state.pendingLoads ++;
				break;
	case S_NOT_LOADED:
		{

		if (url.Length() == 0) { 
			 loader.SetNoUrl();		
			 return(0);
		 }
 		 if (!loader.HasBaseUrl())
			loader.SetBaseUrl(state.GetHomeUrl());

		 if (loader.GetLocalFileFmt() == RMA_FMT) 
			 return 1; // do not preload
		
		 CString extraArg;
	 	 CString theUrl = url[loader.urlI].getString();

		 CTime refreshTime(0);
		 const char *refedFromUrl= state.GetUrl(refreshTime);

		 
		 status = loader.SetupUrl(loader.GetBaseUrl(),theUrl,refedFromUrl,"MovieTexture",state.inlineWwwFlags,state.view->hWnd,loader.urlI,state.view->GetReporter(),state.view->file,extraArg);

#ifdef G_RMA_MOVIE
		 
		 gbool isRma = gfalse;
		 gbool isRma_cached = gfalse;

		 const char *urlTmp = loader.GetUrl();
		 if (urlTmp) {
			CTime refreshTime(0);
			const char *refedFromUrl= state.GetUrl(refreshTime);

		 
			isRma=IsRmaFmt(urlTmp,loader.GetLocalFileFmt());
			
			// download stuff to harddisk first ??
			if (strprefix(urlTmp,G_RMA_URN_CACHED)) {
				isRma_cached=gtrue;
				theUrl = (urlTmp + strlen(G_RMA_URN_CACHED));
				status = loader.SetupUrl(loader.GetBaseUrl(),theUrl,refedFromUrl,"MovieTexture",state.inlineWwwFlags,state.view->hWnd,loader.urlI,state.view->GetReporter(),state.view->file,extraArg);
			}
				

		 } else							 	 
			isRma=IsRmaFmt(loader.GetLocalFile(),loader.GetLocalFileFmt());

		 if (isRma && !isRma_cached) {
			 loader.localFileFmt = RMA_FMT;
			 loader.Release();
			 return 1; // do not preload !!!!!!!!!!1
		 } 	
#endif
		 
		 if (( status <= S_LOADED) && (status != S_NOT_LOADED)) goto Redo; // download request already in progress
		 if (status == S_NOT_LOADED) {
			loader.Release();
			CTime refreshTime(0);
			const char *refedFromUrl= state.GetUrl(refreshTime);
			status = loader.Load(loader.GetBaseUrl(),theUrl,refedFromUrl,"MovieTexture",state.inlineWwwFlags,state.view->hWnd,loader.urlI,refreshTime,state.view->GetReporter(),state.view->file);
		 }
		 if (status != S_NOT_LOADED) goto Redo;
		}	
		break;
/*
	case S_NOT_LOADED:
	     if (url.Length() == 0) { 
			 loader.SetNoUrl();		
			 return(0);
		 }
		 if (!loader.HasBaseUrl())
			loader.SetBaseUrl(state.GetHomeUrl());

		 status = loader.Load(loader.GetBaseUrl(),url[loader.urlI].getString(),state.GetUrl(),"ImageTexture",state.inlineWwwFlags,state.view->hWnd,loader.urlI,state.view->GetReporter(),state.view->file);
		 if (status != S_NOT_LOADED) goto Redo;
		 break;
*/
	case S_LOADED: {
				// Don't go to PROCESSED state 
				return(1);
			}
	case S_PROCESSED:
				return(1);
	default :
		break;

	}
	return(0);
}


// trick to preload stuff 
int GvMovieTexture::Do(GglRenderTraversal &state)
{
	if (!loader.Ok() && state.loadMovies >= GTraversal::LOAD_IF_NEEDED) {
		  Load(state);
	}	
	if (loader.Ok()) { 
	  return(1);
	}

	return(0);
}


int GvMovieTexture::Do(GLoadInlinesTraversal &state)
{
  if (state.resolveRelativeUrls && !loader.HasBaseUrl()) {
	 loader.SetBaseUrl(state.GetHomeUrl());
  }

	if (!loader.Ok() && state.loadMovies >= GTraversal::LOAD_IF_NEEDED) {
		  Load(state);
	}	

	return(0);
}



#ifdef _G_VRML1

//
// GvTexture2
// 

GV_NODE_SOURCE_IMP(GvTexture2,GvTexture);

GvTexture2::GvTexture2()
{
    GV_NODE_CONSTRUCTOR(GvTexture2);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(filename);
    GV_NODE_ADD_FIELD(image);
    GV_NODE_ADD_FIELD(wrapS);
    GV_NODE_ADD_FIELD(wrapT);

    filename.value = "";
    image.size[0] = image.size[1] = 0.0;
    image.numComponents = 0;
    image.bytes = NULL;
    wrapS.value = REPEAT;
    wrapT.value = REPEAT;

//	loaded = 0;
//	cannotLoad = 0;
//	loader = NULL;


    GV_NODE_DEFINE_ENUM_VALUE(Wrap, REPEAT);
    GV_NODE_DEFINE_ENUM_VALUE(Wrap, CLAMP);
    
    GV_NODE_SET_SF_ENUM_TYPE(wrapS, Wrap);
    GV_NODE_SET_SF_ENUM_TYPE(wrapT, Wrap);
}

GvTexture2::~GvTexture2()
{
 	glFlushCache();
}



// a field has been changed
int GvTexture2::OnFieldChanged(GvField *field)
{
    if (field == &filename || field == &image) {
		TRACE("GvTexture2::OnFieldChanged(&name) to %s \n",(const char *)filename);
		SetDefined(0);
		glFlushCache();
		loader.SetNotLoaded();
	}
	return GvNode::OnFieldChanged(field);
}

int GvTexture2::OnChanged()
{
	glFlushCache();
	return GvNode::OnChanged();
}


int GvTexture2::Define(GglRenderTraversal &state)
{
	// empty
  	if (filename.isDefault() || (filename.value.getLength()== 0)) {
		GvTexture::Define(state,(GvSFImage *) NULL);
		return(1);
    }
	return Load(state);
}



int 			
GvTexture2::Load(GglRenderTraversal &state)
{
	// what is the current status ?
	GFileState status = loader.GetState();

Redo:
	switch(status) {
	case S_BAD_DATA:
	case S_BAD_URL:
			loader.SetBadData();
			status = loader.state = S_ALL_BAD;
		    break;
	case S_LOADING:
	case S_PROCESSING:
				state.pendingLoads ++;
				break;
	case S_NOT_LOADED:
	  	 if (filename.isDefault() || (filename.value.getLength()== 0)) {
			 loader.SetNoUrl();		
			 return(0);
		 }
		 if (!loader.HasBaseUrl())
			loader.SetBaseUrl(state.GetHomeUrl());
		 {
		 CTime refreshTime(0);
		 const char *refedFromUrl= state.GetUrl(refreshTime);
		 status = loader.Load(loader.GetBaseUrl(),filename.value.getString(),refedFromUrl,"Texture2",state.inlineWwwFlags,state.view->hWnd,0,refreshTime,state.view->GetReporter(),state.view->file);
		 }
		 if (status != S_NOT_LOADED) goto Redo;
		 break;
	case S_LOADED: {
				int ret;
				ret = GvTexture::Define(state,loader.GetLocalFile(),loader.GetUrl(),loader.GetLocalFileFmt());
				if (ret>=0) loader.SetProcessed();
				else loader.SetBadData();
				if (state.view) {
					state.view->lastFrameLoadCnt++;
					state.view->lastFrameTextureLoadCnt++;
					if (ret<0) state.view->lastFrameLoadErrorCnt++;
				}

				return(1);
			}
	case S_PROCESSED:
				return(1);
	default :
		break;

	}
	return(0);
}
#endif _G_VRML1


int 			
GvImage::Load(GTraversal &state)
{
	// whats the current status
	GFileState status = loader.GetState();

Redo:
	switch(status) {
	case S_BAD_DATA:
	case S_BAD_URL:
		if (loader.urlI< (url.Length() -1) ) {
			loader.urlI ++;
			// fall through and load  next 
		} else {
			status = loader.state = S_ALL_BAD;
		    break;
		}
	case S_NOT_LOADED:
	     if (url.Length() == 0) { 
			 loader.SetNoUrl();		
			 return(0);
		 }
		 if (!loader.HasBaseUrl())
			loader.SetBaseUrl(state.GetHomeUrl());

		 {
		 CTime refreshTime(0);
		 const char *refedFromUrl= state.GetUrl(refreshTime);
		 status = loader.Load(loader.GetBaseUrl(),url[loader.urlI].getString(),refedFromUrl,"ImageTexture",state.inlineWwwFlags,state.view->hWnd,loader.urlI,refreshTime,state.view->GetReporter(),state.view->file);
		 }

		 if (status != S_NOT_LOADED) goto Redo;
		 break;

	case S_LOADING:
	case S_PROCESSING:
				state.pendingLoads ++;
				break;

	case S_LOADED: {
				int ret;

				glFlushCache(); // need to release handle to possibly shared texture

				GvBool reload;
				const char * s=loader.GetUrl();
				reload= (strstr(s,"/no_cache/") || strstr(s,"/cgi-bin/"));
				if (reload)
					TRACE("Need to refresh texture url = %s \n",s);

				ret = /* ((GvTexture *) this)->*/ Define(state,loader.GetLocalFile(),loader.GetUrl(),loader.GetLocalFileFmt(),reload);
				if (ret>0)
					loader.SetProcessed();
				else { // bad image format 
					loader.SetBadData();
				}
				if (state.view) {
					state.view->lastFrameLoadCnt++;
					state.view->lastFrameTextureLoadCnt++;
					if (ret<=0) state.view->lastFrameLoadErrorCnt++;
				}
				return(1);
			}
	case S_PROCESSED:
				return(1);
	default :
		break;

	}
	return(0);

}


#ifdef _G_VRML1

int GvTexture2::Do(GLoadInlinesTraversal &state)
{
#if 0
	if (state.loadTextures >= GTraversal::LOAD_IF_NEEDED)
		Load(state);



  if (!loaded && !cannotLoad && !state.stop 
  		&& state.loadTextures >= GTraversal::LOAD_IF_NEEDED) { 
  			// to do :::::::::::::::::::::::Load(state); type problem
			if (state.stopOnFirst) state.stop ++;
  }		
  if (loaded) { 
  	return(1);
  }
#endif

  return(0);
}

#endif _G_VRML1



//
// GvImage
//

GV_NODE_SOURCE_IMP(GvImage,GvImageTexture);

GvImage::GvImage()
{
    GV_NODE_CONSTRUCTOR(GvImage);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(url);
    GV_NODE_ADD_FIELD(repeatS);
    GV_NODE_ADD_FIELD(repeatT);


    //GV_NODE_ADD_FIELD(filename);
    GV_NODE_ADD_FIELD(image);
    //GV_NODE_ADD_FIELD(wrapS);
    //GV_NODE_ADD_FIELD(wrapT);

    GV_NODE_ADD_FIELD(alpha);
    
    GV_NODE_ADD_FIELD(alphaColor);

    //filename.value = "";
    image.size[0] = image.size[1] = 0.0;
    image.numComponents = 0;
    image.bytes = NULL;
    //wrapS.value = REPEAT;
    //wrapT.value = REPEAT;

    alpha.value = 1.0;
    alphaColor.value[0] = alphaColor.value[1]= alphaColor.value[2]= 0.0;

//	loaded = 0;
//	cannotLoad = 0;
//	loader = NULL;
#ifdef _OGL_OLD
	cache = NULL;
#endif


    //GV_NODE_DEFINE_ENUM_VALUE(Wrap, REPEAT);
    //GV_NODE_DEFINE_ENUM_VALUE(Wrap, CLAMP);
    
    //GV_NODE_SET_SF_ENUM_TYPE(wrapS, Wrap);
    //GV_NODE_SET_SF_ENUM_TYPE(wrapT, Wrap);
}

GvImage::~GvImage()
{
    // use GvTexture2 destructor

}

// a field has been changed
int GvImage::OnFieldChanged(GvField *field)
{
    if (Defined() && (field == &alpha || field == &alphaColor)) { // loaded 
        if (!alpha.isDefault())
        {
            if (alphaColor.isDefault()) 
            {   image.setAlpha(alpha.value);
            }
            else 
              image.setAlpha(alphaColor.value, alpha.value);
            
            image.touch();
        }
		return(GvImageTexture::OnChanged());
	}
    else return GvImageTexture::OnFieldChanged(field);
}

int GvImage::OnChanged()
{
    return(GvImageTexture::OnChanged());
}

//
// GvScaledImage
//

GV_NODE_SOURCE_IMP(GvScaledImage,GvImage);

GvScaledImage::GvScaledImage()
{
    GV_NODE_CONSTRUCTOR(GvScaledImage);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(url);
    GV_NODE_ADD_FIELD(repeatS);
    GV_NODE_ADD_FIELD(repeatT);

    GV_NODE_ADD_FIELD(image);
   
    GV_NODE_ADD_FIELD(alpha);
    GV_NODE_ADD_FIELD(alphaColor);

    GV_NODE_ADD_FIELD(alpha);
    GV_NODE_ADD_FIELD(alphaColor);

    GV_NODE_ADD_FIELD(srcImage);
    GV_NODE_ADD_FIELD(scale);
    GV_NODE_ADD_FIELD(scaleFactor);
    GV_NODE_ADD_FIELD(filter);



    image.size[0] = image.size[1] = 0.0;
    image.numComponents = 0;
    image.bytes = NULL;


    alpha.value = 1.0;
    alphaColor.value[0] = alphaColor.value[1]= alphaColor.value[2]= 0.0;
    
    scale.value = NONE;
    scaleFactor.value[0] = scaleFactor.value[1] =  1.0;

	filter.value = FALSE;


    GV_NODE_DEFINE_ENUM_VALUE(Scale, NONE);
    GV_NODE_DEFINE_ENUM_VALUE(Scale, TO_WINDOW);
    GV_NODE_DEFINE_ENUM_VALUE(Scale, TO_SIZE);
    GV_NODE_DEFINE_ENUM_VALUE(Scale, BY_FACTOR);
    
    GV_NODE_SET_SF_ENUM_TYPE(scale, Scale);

}

GvScaledImage::~GvScaledImage()
{
}


// a field has been changed
int GvScaledImage::OnFieldChanged(GvField *field)
{
    
    if (Defined() && (field == &scale || field == &scaleFactor || field == &srcImage || field == &filter)) {
        SetDefined(0);
		return GvImage::OnFieldChanged(field);
        //return(1);
	}
    else return GvImage::OnFieldChanged(field);
}

int GvScaledImage::OnChanged()
{
    return(GvImage::OnChanged());
}


//
// GvImageTexture
// 

GV_NODE_SOURCE_IMP(GvMultiTexture,GvTexture);

GvMultiTexture::GvMultiTexture() :
	mode(0),texture(0),textureTransform(0),textureOp(0) 
		
{
    GV_NODE_CONSTRUCTOR(GvMultiTexture);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(mode);
    GV_NODE_ADD_FIELD(texture);
    GV_NODE_ADD_FIELD(textureTransform);
    GV_NODE_ADD_FIELD(textureOp);

}

GvMultiTexture::~GvMultiTexture()
{
}


int 			
GvMultiTexture::Do(GglRenderTraversal &state)
{
  // if no texture mapping ignore node
  if (state.useTextures) 
	  return(0);


  int ret;
  
  ret = texture.Traverse(state);

  if (!Defined()) {
	  ret = Define(state);
  }
  return(ret);
}


int 			
GvMultiTexture::glSet(GglRenderTraversal &state,int i)
{  int ret=0;

	int numTextures = texture.getNum();
	if (i<numTextures) {
		GvNode *n = texture.get1(i);

		if (n) n= n->Eval(state);
		const char *mode=NULL;
		
		if (i<this->mode.getNum()) {
			mode = this->mode.get1(i);
		}

		if (n && RTISA(n,GvTexture)) {
			GvTexture *t=(GvTexture *) n;
			
			ret |= t->glSet(state);
#ifdef _OGL
			if (!mode) 
				glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );			
			else if (streq(mode,"decal")) 
				glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
			else if (streq(mode,"blend")) 
				glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );
			//else if (streq(mode,"add") && glState.use_texture_env_add_EXT) 
			//	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD ); // is this right ?
			else 
				glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
#endif
		}
		

	}		
	return ret;
}


int 			
GvMultiTexture::glSet(GglRenderTraversal &state)
{  int ret=0;

	int numTextures = texture.getNum();
	int i;	
	
#ifdef _OGL
	RenderState &glState = *state.glState;
	gbool mulTex = glState.useARB_multitextureEXT;

	if (mulTex) {	// we have multi texture
	int maxUnit = 0;
	
	if (numTextures> glState.maxTexUnit) {	// limit to number of texture units
		// multipath
		numTextures = glState.maxTexUnit;
	}

	for (i=0;i<numTextures;i++) {
		GvNode *n = texture.get1(i);

		if (n) n= n->Eval(state);
		glState.glActiveTextureARB( GL_TEXTURE0_ARB + i ); // switch to unit i
		const char *mode=NULL;
		
		if (i<this->mode.getNum()) {
			mode = this->mode.get1(i);
		}

		if (n && RTISA(n,GvTexture)) {
			GvTexture *t=(GvTexture *) n;
			maxUnit = i;
			glEnable(GL_TEXTURE_2D);
			
			ret |= t->glSet(state);

			if (!mode) 
				glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );			
			else if (streq(mode,"decal")) 
				glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
			else if (streq(mode,"blend")) 
				glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );
			else if (streq(mode,"add") && glState.use_texture_env_add_EXT) 
				glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD ); // is this right ?
			else 
				glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

		}
		else glDisable(GL_TEXTURE_2D);

	}		
	
	// disable uneeded units
	for (i=maxUnit+1; i<glState.maxEnabledTexUnit;i++) { // turn off unsed texture units
		glState.glActiveTextureARB( GL_TEXTURE0_ARB + i );
		glDisable(GL_TEXTURE_2D);
	}
	glState.maxEnabledTexUnit = maxUnit+1;

	glState.glActiveTextureARB( GL_TEXTURE0_ARB); // set unit 0 as default
	}
	else {
		glSet(state,0); // set texture 0
	}

	ret |= 	TEXTURE_CHANGED;
#else
	glSet(state,0); // set texture 0
#endif

	return ret;
}





