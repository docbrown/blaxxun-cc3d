#ifndef  _GvCompiledGroup_h_
#define  _GvCompiledGroup_h_

/******************************************************************************
@doc

@module GvCompiledGroup.h - blaxxun compile group|

Copyright (c) 1999 - 1999 blaxxun interactive, Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvCompiledGroup>
  
Notes:

Changes:

$Revision: $
$Log: $

Todo :

******************************************************************************/


#include <GvSFNode.h>
#include <GvMFNode.h>

#include <GvMFInt32.h>

#include <GvWWWInline.h>

// forward
class GFileMap;
class GStreamDescStream;
class GMyDisplayListPacket;

//#include "gdisplaylist.h"



class GvVertexBuffer : public GvNode
{

    GV_NODE_HEADER(GvVertexBuffer);

public:

    // Fields :
    GvSFNode	color;            // Default :NULL
    GvSFNode	specularColor;    // Default :NULL
    GvSFNode	coord;            // Default :NULL
    GvSFNode	normal;           // Default :NULL
    GvMFNode	texCoord;         // Default :NULL

	int Do(GBBoxTraversal &state);

};

//
// GvCompiledGroup  
//
class GvCompiledGroup : public Gv2Inline  
{

    GV_NODE_HEADER(GvCompiledGroup);

public:
	// fields from Group :
	// bboxSize bboxCenter
	// children
	//
	// Fields from Inline :
	// url
	// isLoaded

    // Fields :

	GvMFNode	material;		   // list of materials				
	GvMFNode	texture;
	GvMFNode	textureTransform;
	GvMFNode	appearance;

	GvMFNode	transform;

    GvMFNode	color;            // Default :NULL
    GvMFNode	coord;            // Default :NULL
    GvMFNode	normal;           // Default :NULL
    GvMFNode	texCoord;         // Default :NULL

	GvMFNode	vertexBuffer;

	GvMFNode	audioClip;

	GvMFInt32	whichChoice;

	// bool compile

	int Do(GTraversal &state);
	int Do(GglRenderTraversal &state);


	//! parse a memory map stream
	int ParseFile(LPCTSTR fileName);

private :


	//! file mapping object, mapping the data from disk
	GFileMap *fileMap;
	
	//! list of streams in file map
	GStreamDescStream *streams;
	
	//! render a single displaylist packet
	int Do(GMyDisplayListPacket &packet,GglRenderTraversal &state);


};



#endif  _GvCompiledGroup_h_ 


