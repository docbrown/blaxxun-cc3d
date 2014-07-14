#include "stdafx.h"
#include "gstream.h"


//
// GStream
//

GStream::GStream()
{
}

GStream::~GStream() 
{
	Term();
}

//! initialize 
gbool GStream::Init()
{
	return gtrue;
}

//! add packet receive from net/file to stream,
gbool GStream::AddPacket(size_t size, void * data,gbool byRef)
{ 
	return gtrue;
}
	

//!terminate
void GStream::Term()
{}

//! read descriptor fields from memory map by reference
gbool GStream::InitDescriptor(GStreamType streamType,GMemMap &p)
{

	this->streamType = streamType;
	name  = p.getString();
	mimeType  = p.getString();
	size =	p.getUINT32();
	priority =	p.getUINT32();
	userData =	p.getUINT32();
		
	return gtrue;
}


//! read stream private initalizer data 
gbool GStream::InitStream(size_t size,GMemMap &p)
{
	// skip data block
	p.current += size;
	return gtrue;
}




//
// GFileMap
//

GFileMap::GFileMap() : GMemMap(0,NULL),hFile(NULL),hMapping(NULL)
{

}

GFileMap::~GFileMap() 
{
  Term();
}



gbool GFileMap::Init(LPCTSTR lpName) 
{
  
	HANDLE hFile;
	
	DWORD dwSizeLow,dwSizeHigh;
	DWORD dwError;

    base = NULL;

    hFile = CreateFile (lpName, GENERIC_READ, FILE_SHARE_READ, 
	                    NULL, OPEN_EXISTING, 0, NULL);

    if (hFile == INVALID_HANDLE_VALUE) return gfalse;
 
	dwSizeLow = GetFileSize (hFile, &dwSizeHigh) ; 
 
	// If we failed ... 
	if (dwSizeLow == 0xFFFFFFFF  && (dwError = GetLastError()) != NO_ERROR ) { 
 
    } // End of error handler. 
 

	hMapping = CreateFileMapping (hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (hMapping == INVALID_HANDLE_VALUE)
    {
        CloseHandle (hFile); 
        return gfalse;
    }

    CloseHandle (hFile);

    LPVOID p = (UCHAR *) MapViewOfFile (hMapping, FILE_MAP_READ, 0, 0, 0);
    
	if (p == NULL)
    {
        CloseHandle (hMapping);
        return gfalse;
    }
	
	return GMemMap::Init(dwSizeLow,p);

};

void GFileMap::Term() 
{
  if (hMapping) 
	  CloseHandle (hMapping);
   hMapping = NULL;
}
//
// GStreamDescStream
//

GStreamDescStream::GStreamDescStream()
{
}
	
GStreamDescStream::~GStreamDescStream()
{
}


//!terminate, free all substreams
void GStreamDescStream::Term()
{
	for (int i=0; i<streams.Length(); i++) {
		GStream *s = streams[i];
		if (s) {
			streams[i]=NULL;
			s->Term();
			delete s;
		}
	}
	
	streams.SetLength(0);

	GStream::Term();
}


//! add a stream to table
void GStreamDescStream::AddStream(UINT32  streamId,GStream* s)
{
	s->streamId = streamId;
	if (streamId>=streams.Length()) 
		streams.SetLength(streamId+1);
	
	GStream* sold = streams[(int)streamId];

	// if s!= null delete old 
	if (sold != NULL ) {
		ASSERT(sold->streamId == streamId);
		streams[(int)streamId] = NULL;
		delete sold;
	}

	streams[(int)streamId] = s;

}

//! delete & free a stream in table
void GStreamDescStream::DeleteStream(UINT32  streamId)
{
	GStream* s;

	if (streamId<streams.Length()) {
		s = streams[(int)streamId];

		// if s!= null delete old 
		if (s != NULL ) {
			ASSERT(s->streamId == streamId);
			streams[(int)streamId] = NULL;
			delete s;
		}
	}
}

	
// create a new stream instance 
// override this function for usefull stream implementers
GStream *GStreamDescStream::NewStream(GStreamType streamType)
{
	switch (streamType) {

	default : return NULL;
	}

}


// get the stream consuming data with streamId
GStream *GStreamDescStream::GetStreamById(UINT32 streamId)
{
	// is it the special stream descriptor stream ??
	if (streamId  == G_STYPE_STREAM_DESC) {
		return this;
	}
	if (streamId >= (UINT32) streams.Length()) return NULL;
	return streams[(int) streamId];
}


//! add packet receive from net/file to stream,
gbool GStreamDescStream::AddPacket(size_t size, void * data,gbool byRef)
{ 
	ASSERT(byRef);
	GMemMap p;
	
	if (!p.Init(size,data)) 
		return gfalse;

	while (p.Current() < p.End())	{

		UINT32  streamId = p.getUINT32();	//<streamId>
		USHORT op = p.getUSHORT();			//<op>
		
		switch (op) {
		case G_OP_NOP : break;

		case G_OP_REPLACE :
				DeleteStream(streamId);
				// fall through to new 

		case G_OP_NEW:	// add a new stream
		case G_OP_APPEND:
			{
				UINT32 streamType = p.getUINT32(); //<streamType>

				GStream *s = NewStream(streamType);
				
				if (!s) return gfalse;

				if (s->InitDescriptor(streamType,p)) {
					delete s;
					return gfalse;
				}
				UINT32 streamInitBlockSize = p.getUINT32();

				if (!s->InitStream(streamInitBlockSize,p)) {
					return gfalse;
				}

				AddStream(streamId,s);
				break;
			}
		case G_OP_DELETE:	// delete the stream
			{
				DeleteStream(streamId);
				break;
			}

		default : return gfalse;
		}
	}
	return gtrue;
}



