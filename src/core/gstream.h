
#ifndef _gstream_h
#define _gstream_h

#ifdef G_RMA_MOVIE
// Real G2 SDK problem , conflicting declarations in MSVC 6.0, see comment in pntypes.h !

#if (defined(_MSC_VER)) //  && (_MSC_VER >= 1200)) // VC6 
#include "pntypes.h"
#endif

#endif


// forward
class GMemMap;

typedef enum {
		GDF_FLOAT,		// 4 byte IEEE float
		GDF_DOUBLE,		// 8 byte IEEE double
		GDF_SHORT,		// 2 byte signed int	
		GDF_FIX_8_8,	// 8.8 fixed signed int	
		GDF_FIX_13_3	// 13.3 fixed signed int

} GDFloatEncoding;

// ! get floating point 
inline	float Fix8_8(USHORT s) 
{ 
		signed short a =s; 
		a = (a>>8);
		unsigned char b = (s & 0xFF);
		float f = a+ 1.0f / (float) b;
		return f;

}

inline	float Fix13_3(USHORT s) 
{ 
		signed short a =s; 
		s&=(1+2+4);
		s=(s>>3);
		unsigned char b = (s & (1+2+4));

		float f = a+ 1.0f / (float) b;
		return f;

}



typedef enum {
		GDF_UBYTE,		// 1 byte 
		GDF_USHORT,		// 2 byte 
		GDF_UINT,		// 4 byte 
} GSIntEncoding;




typedef UINT32 GStreamId;
typedef UINT32 GStreamType;

typedef enum {
  G_STYPE_EOF,
	  
  G_STYPE_STREAM_DESC,		// a stream describing streams (stream descriptors)		
  G_STYPE_TIME,				// a stream containing time stamps
  G_STYPE_RESOURCE,			// a stream containing resources (image materials, textures, vertexbuffers etc)
  G_STYPE_DISPLAYLIST,		// a stream containing a 3D displaylist
  G_STYPE_IMAGE,			// a stream containing image bits 
  G_STYPE_AUDIO,			// a stream containing audio bits 
  G_STYPE_DATA,				// a generic data stream 

} GStreamType_Enum;


class GStream {
public:

	//! the id number of the stream, used for indexing into stream table
	GStreamId  streamId; 

	//! purpose type of stream (GStreamType )
	GStreamType		streamType;

	//! name of stream for application purposes
	const char *name;
	
	//! mimeType of stream if applicable 
	const char *mimeType;

	//! 0, or number of bytes expected for complete stream
	size_t size;	

	//! priority of stream if low on resources 
	UINT32 priority;	

	//! user data
	UINT32 userData;

	GStream();
	virtual ~GStream();


	//! read descriptor fields from memory map by reference
	virtual gbool InitDescriptor(GStreamType streamType, GMemMap &p);

	//! read stream private initalizer data 
	virtual gbool InitStream(size_t size,GMemMap &p);

	//! initialize 
	gbool Init();

	//! add packet receive from net/file to stream,
	virtual gbool AddPacket(size_t size, void * data,gbool byRef);
	

	//!terminate
	virtual void Term();

};

typedef UCHAR * GMemPtr;

class GMemMap {
public:
	GMemPtr current;	//! the current read pointer

	GMemPtr base;		//! the current base pointer 
	size_t	size;		//! the current block size 

	GMemPtr Base() { return base; }
	GMemPtr End() { return base + size; }
	GMemPtr Current() { return current; }

	GMemMap(size_t size=0,void *data=NULL) {
		base = current = (GMemPtr) data;
		this->size = size;
	}		

	void Reset() { current = base; }
	gbool Init(size_t size,void *data) 
	{
		base = current = (GMemPtr) data;
		this->size = size;
		return (current != NULL);
	}

	//! skip size bytes
	void Skip(size_t size) 
	{
		current += size;
	}

	//!verify pointer 
	inline gbool Verify(GMemPtr p) {
		ASSERT( (p>=base) && (p < (base + (size)))); 
		return ((p>=base) && (p < (base + (size)))); 
	}

	inline gbool Verify() {
		return Verify(current);
	}

	//! get USHORT
	inline USHORT getUSHORT() { return *((USHORT *&)current)++; }
	inline USHORT getCnt() { return getUSHORT(); }
	inline USHORT getIndex() { return getUSHORT(); }
	inline UINT32 getUINT32() { return *((UINT32 *&)current)++; }

	//! get a string 
	const char * getString() {
		size_t cnt= getUSHORT();
		const char* ret = (char *) current;
		current +=cnt;
		align2();
		return ret;

	}

	void align2() { if (((UINT) current) & 1) current+=1; }
	void align4() { if (((UINT) current) & 2) current+=2; }
	void align8() { }
	void align16() { }
	void align32() { }

	//! decode n floats from buffer , and return pointer to buffer 
	const float * getFloats(GDFloatEncoding encoding, size_t cnt, float *buffer) 
	{

		switch (encoding) {
		case GDF_FLOAT : { size_t s=cnt*sizeof(float);
						   align4(); 
						   // memcpy(buffer,current,s); 
						   float * tmp = (float *) current;
						   ((UINT32 *&)current) +=s;
						   return tmp;  // avoid memcpy 

						 }
		case GDF_DOUBLE : { 
						   size_t s=cnt*sizeof(double);
						   align8(); 
						   const double *p= (const double *) current;
						   if (cnt>0) 
						   do {
							  *buffer++ = *p++;	
						   } while (cnt--);	   

						   ((UINT32 *&)current) +=s;
						   
						   return buffer; 
						 }
		case GDF_SHORT : { 
						   size_t s=cnt*sizeof(SHORT);
						   const SHORT *p= (const SHORT *) current;
						   if (cnt>0) 
						   do {
							  *buffer++ = *p++;	
						   } while (cnt--);	   

						   ((UINT32 *&)current) +=s;
						   
						   return buffer; 
						 }
		case GDF_FIX_8_8 : { 
						   size_t s=cnt*sizeof(SHORT);
						   const SHORT *p= (const SHORT *) current;
						   if (cnt>0) 
						   do {
							  *buffer++ = Fix8_8(*p++);	
						   } while (cnt--);	   

						   ((UINT32 *&)current) +=s;
						   
						   return buffer; 
						 }

		case GDF_FIX_13_3 : { 
						   size_t s=cnt*sizeof(SHORT);
						   const SHORT *p= (const SHORT *) current;
						   if (cnt>0) 
						   do {
							  *buffer++ = Fix13_3(*p++);	
						   } while (cnt--);	   

						   ((UINT32 *&)current) +=s;
						   
						   return buffer; 
						 }
		}
		return NULL;
	}


};

class GFileMap : public GMemMap {

protected:

    HANDLE hFile, hMapping;

public :
	
	GFileMap();

	~GFileMap();

	virtual gbool Init(LPCTSTR lpName);
	virtual void Term();

};

/*

  DescStream described the other streams occuring in the file

*/

class GStreamDescStream : public GStream
{
	Array<GStream *> streams;  // list of stream


public:

	GStreamDescStream();
	~GStreamDescStream();

	//! return number of streams
	inline int NumStreams() const {return streams.Length(); }

	//! get the stream consuming data with streamId, can be NULL for ignored stream
	inline GStream *GetStream(int streamNum) { return streams[streamNum]; }

	//! add a stream to table
	virtual void AddStream(UINT32  streamId,GStream* s); 
	
	//! delete & free a stream in table
	virtual void DeleteStream(UINT32  streamId);
	
	//! create a new stream instance for the given type 
	virtual GStream * NewStream(GStreamType streamType);


	//! get the stream consuming data with streamId
	GStream *GetStreamById(UINT32 streamId);

	//! add packet receive from net/file to stream,
	virtual gbool AddPacket(size_t size, void * data,gbool byRef);
	

	//!terminate, close & delete all streams
	virtual void Term();
};


// ! basic operations 
typedef enum 
{ G_OP_NOP,
  G_OP_NEW,
  G_OP_DELETE,
  G_OP_APPEND,
  G_OP_REPLACE,

} G_STREAM_OP;



//! elements in an resource stream 
typedef enum 
{
	G_RS_MATERIAL,	// <matId> <material>
	
	G_RS_TEXTURE,	// <id> <repeatS> <repeatT> <streamId>

	G_RS_TEXTURE_TRANSFORM,	// <matId>  <type>  <data>
	
	G_RS_APPEARANCE,	// <appId>  <matId> <texId> <texTransformId>

	G_RS_COLOR,		// <ColorId> <floatEncoding> <cnt> <float3data>
	G_RS_COORD,		// <CoordId> <floatEncoding> <cnt> <float3data>
	G_RS_NORMAL,	// <NormalId> <floatEncoding> <cnt> <float3data>
	G_RS_TEX_COORD,	// <TexCoordId> <floatEncoding> <cnt> <float2data>
	
	G_RS_VERTEX_BUFFER,	// <VbId> <indexEncoding> <cnt> ( <ColorId> <CoordId><NormalId><TexCoordid0..n>
	
	G_RS_AUDIO_CLIP,	// <AudioId> <streamId>
	G_RS_WHICH_CHOICE,	// <intEncoding> <cnt> <ints>

} G_RS_STREAM_SUBTYPE;


/*  Resource stream data :

	<Op> <RsStreamSubType> <subTypeData>

	op == replace append 

*/


#endif _gstream_h

