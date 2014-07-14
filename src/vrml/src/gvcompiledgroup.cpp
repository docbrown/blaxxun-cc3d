
/******************************************************************************
@doc

@module GvCompiledGroup.cpp - CompiledGroup extensions |

Copyright (c) 1998 - 1999 blaxxun interactive, Holger Grahn
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


#include "stdafx.h"

#include "gvcompiledgroup.h"
#include "gvmaterial.h"
#include "gvtexture2.h"
#include "gv2geometry.h"

#include "gstream.h"
#include "gdisplaylist.h"




//
// GvVertexBuffer
//


GV_NODE_SOURCE_IMP(GvVertexBuffer,GvNode);


GvVertexBuffer::GvVertexBuffer() :texCoord(0)
{
    GV_NODE_CONSTRUCTOR(GvVertexBuffer);
    isBuiltIn = FALSE;

    GV_NODE_ADD_FIELD(color);
    GV_NODE_ADD_FIELD(coord);
    GV_NODE_ADD_FIELD(normal);
    GV_NODE_ADD_FIELD(texCoord);
        
}

GvVertexBuffer::~GvVertexBuffer()
{
}

int GvVertexBuffer::Do(GBBoxTraversal &state)
{
	// compute bbox from coordinates
	return (coord.Traverse(state));
}



class GMyResourceStream : public GStream
{
public:
	// parent to store/update resources
	GvCompiledGroup *parent;

	GDFloatEncoding matEncoding;

	GMyResourceStream() 
	{ 
		matEncoding = GDF_FLOAT;
	};

	~GMyResourceStream() {};

	//! add packet receive from net/file to stream,
	gbool AddPacket(size_t size, void * data,gbool byRef);

};

// store the packt in list
gbool GMyResourceStream::AddPacket(size_t size, void * data,gbool byRef)
{
	GMemMap p;
	
	if (!p.Init(size,data)) 
		return gfalse;

	while (p.Current() < p.End())	{

		USHORT resourcType = p.getUSHORT();
		USHORT op = p.getUSHORT();
		USHORT id = p.getUSHORT();
		
		switch (resourcType) {
		case G_RS_MATERIAL:	
			{
			Gv2Material *mat=NULL;
			GvMFNode &field = parent->material;
			float buffer[3];

			int num = field.getNum();
			if (id<num) mat = (Gv2Material *) field.get1(num);

			switch (op)	{
			case G_OP_DELETE: {
				if (id<num) field.set1(id,(GvNode *) NULL);
				break;
			}
			case G_OP_NEW :
			case G_OP_APPEND:
			case G_OP_REPLACE: {
				if (mat == NULL) {
					mat = new Gv2Material();
					if (id>=num) field.makeRoom(id+1),
					field.set1(id,(GvNode *) NULL);
				}
				mat->ambientIntensity.set(p.getFloats(matEncoding,1,buffer) [0]);
				mat->diffuseColor.set(p.getFloats(matEncoding,3,buffer));
				mat->specularColor.set(p.getFloats(matEncoding,3,buffer));
				mat->emissiveColor.set(p.getFloats(matEncoding,3,buffer));
				mat->shininess.set(p.getFloats(matEncoding,1,buffer) [0]);
				mat->transparency.set(p.getFloats(matEncoding,1,buffer) [0]);

				break;
				}

			default : return gfalse;
			}
			break;
			}

		default : return gfalse;
		}
	}
	return gtrue;

	return gtrue;
}



class GMyDisplayListPacket : public GDExecutionContext
{
public :

	//! store the pointer to execution block
	
	GMyDisplayListPacket(size_t size, void * data,gbool byRef) : 
	   GDExecutionContext(size,(GMemPtr)data)
	{
		ASSERT(byRef);
	}
	~GMyDisplayListPacket() {}


};

class GMyDisplayListStream : public GStream
{
public:
	
	//!list of displaylist packet
	Array<GMyDisplayListPacket*> packets;

	GMyDisplayListStream() {};
	~GMyDisplayListStream() {};

	//! add packet receive from net/file to stream,
	gbool AddPacket(size_t size, void * data,gbool byRef);


};

// store the packt in list
gbool GMyDisplayListStream::AddPacket(size_t size, void * data,gbool byRef)
{
	GMyDisplayListPacket *p = new GMyDisplayListPacket(size,data,byRef);
	if (!p) return gfalse;
	packets.Append(p);
	return gtrue;
}



class GMyStreamDescStream : public GStreamDescStream
{
public:

	GMyStreamDescStream() {};
	~GMyStreamDescStream() {};

	//! create a new stream instance 
	GStream * NewStream(GStreamType streamType)
	{
		switch (streamType) {
		case G_STYPE_RESOURCE:
				return new GMyResourceStream();
		case G_STYPE_DISPLAYLIST:
				return new GMyDisplayListStream();

		default : return GStreamDescStream::NewStream(streamType);
		}
	}

};





//
// GvCompiledGroup  
//


GV_NODE_SOURCE_IMP(GvCompiledGroup,Gv2Inline);


GvCompiledGroup::GvCompiledGroup() : streams(NULL),fileMap(NULL)
{
    GV_NODE_CONSTRUCTOR(GvCompiledGroup);
    isBuiltIn = FALSE;

	// from Gv2group
	GV_NODE_ADD_FIELD(bboxCenter);
	GV_NODE_ADD_FIELD(bboxSize);

	GV_NODE_ADD_FIELD(url);

    // normally hidden
    //from Gv2group
	GV_NODE_ADD_FIELD(children);
    GV_NODE_ADD_EVENT_IN(addChildren,GvMFNode);
    GV_NODE_ADD_EVENT_IN(removeChildren,GvMFNode);

	// extra
    GV_NODE_ADD_EVENT_IN(set_unload,GvSFBool);
    GV_NODE_ADD_EVENT_OUT(isLoaded);

	// from GvCompiledGroup

    GV_NODE_ADD_FIELD(material);
    GV_NODE_ADD_FIELD(texture);
    GV_NODE_ADD_FIELD(textureTransform);
    GV_NODE_ADD_FIELD(appearance);
    GV_NODE_ADD_FIELD(transform);

    GV_NODE_ADD_FIELD(color);
    GV_NODE_ADD_FIELD(coord);
    GV_NODE_ADD_FIELD(normal);
    GV_NODE_ADD_FIELD(texCoord);
    
	GV_NODE_ADD_FIELD(vertexBuffer);
	GV_NODE_ADD_FIELD(audioClip);
	GV_NODE_ADD_FIELD(whichChoice);



};

GvCompiledGroup::~GvCompiledGroup() 
{
	if (streams) delete streams;

	if (fileMap) delete fileMap;

}

int GvCompiledGroup::Do(GTraversal &state)
{
  if (!loader.Ok()) {
	if ((state.inlineLevel <= state.maxInlineLevel)
  		&& state.loadInlines >= GTraversal::LOAD_IF_NEEDED)
		Load(state);
  }	

  if (loader.Ok()) { 
		// pass to TraverseChildren for proper stack handling
  		return(TraverseChildren(state));
  }

  return(0);
}

//! parse a memory map stream
int GvCompiledGroup::ParseFile(LPCTSTR fileName)

{
	int ret=0;

	if (!fileMap)
		fileMap = new GFileMap();

	if (!fileMap->Init(fileName)) 
		return -1;

	if (!streams)
		streams = new GMyStreamDescStream();

	streams->Init();

	// parse the header
	UINT32 magic1 = fileMap->getUINT32();
	UINT32 magic2 = fileMap->getUINT32();
	
	UINT32 version = fileMap->getUINT32();
	
	UINT32 flags = fileMap->getUINT32();
	
	UINT32 vendor = fileMap->getUINT32();

	UINT32 sizeHigh = fileMap->getUINT32();
	UINT32 sizeLow = fileMap->getUINT32();

	UINT32 maxStreams = fileMap->getUINT32();
	UINT32 maxPackets = fileMap->getUINT32();
	UINT32 maxPacketSize = fileMap->getUINT32();


	const char * title = fileMap->getString();
	const char * info = fileMap->getString();
	const char * copyright = fileMap->getString();

	fileMap->align4();



	UINT32 streamId=0;
	UINT32 chunkSize=0;


	while (1) {

		if (!fileMap->Verify()) {
			ret=-1;
			break;
		}

		streamId = fileMap->getUINT32();
		chunkSize = fileMap->getUINT32();
	
		GStream *s = streams->GetStreamById(streamId);

		if (!s) {
			// simply skip this stream
		} else { 
			// stream should consume packet
			if (!s->AddPacket(chunkSize,fileMap->current,gtrue)) {
				ret = -1; // something wrent wrong
				break;
			}
		}
		
		fileMap->current+=chunkSize;
	}

	return ret;

}







// render a single displaylist packet
int GvCompiledGroup::Do(GMyDisplayListPacket &context,GglRenderTraversal &state)
{

	Matrix m;
	Point pt;
	Plane pl;

	// the current vertexbuffer
	DWORD  dwVertexTypeDesc=0;
	BBox  vertexBBox;
	GDCnt  vertexCount=0;
	GDCnt  bytesPerVertex = 0;
	LPVOID lpvVertices = NULL;


	while (1) {


#ifdef _DEBUG
	if (!context.Verify()) {
		TRACE("GvCompiledGroup:Execution pointer out of range \n");
		ASSERT(0);
		break;
	}
#endif

	GDOpCode_Enum op = context.getOp();
	switch (op) {

	case GDOP_NOP:	continue;		// NO-OP
	case GDOP_EXIT:	goto return_;	// exit current block
	case GDOP_RETURN:				// return from call 
		context.return_();	continue; 
	case GDOP_JUMP:		//<relOffset>	// jump relative
		{ GDRelOffset offset = context.getRelOffset();
		  context.jumpRel(offset);
		  continue;
		}
	case GDOP_JUMPL:		//<relOffset32> // jump relative
		{ GDRelOffset32 offset = context.getRelOffset32();
		  context.jumpRel(offset);
		  continue;
		}
	case GDOP_JUMP_ABS:	//<absOffset>	// jump absolute
		{ GDAbsOffset offset = context.getAbsOffset();
		  context.jumpAbs(offset);
		  continue;
		}

	case GDOP_JUMPL_ABS:	//<absOffset32> // jump absolute
		{ GDAbsOffset32 offset = context.getAbsOffset32();
		  context.jumpAbs(offset);
		  continue;
		}

	case GDOP_CALL:			//<relOffset> // call relative
		{ GDRelOffset offset = context.getRelOffset();
		  context.callRel(offset);
		  continue;
		}

	case GDOP_CALLL:		//<relOffset32> // call relative
		{ GDRelOffset32 offset = context.getRelOffset32();
		  context.callRel(offset);
		  continue;
		}
	case GDOP_CALL_ABS:	//<absOffset> // call absolute
		{ GDAbsOffset offset = context.getAbsOffset();
		  context.callAbs(offset);
		  continue;
		}
	case GDOP_CALLL_ABS:	//<absOffset32> // call absolute
		{ GDAbsOffset32 offset = context.getAbsOffset32();
		  context.callAbs(offset);
		  continue;
		}

#if 0
	case GDOP_CALL_PACKET:	// <PacketId> <absOffset>	// call packet at offset if present
	case GDOP_CALLL_PACKET:	// <PacketId32> <absOffset32>

	case GDOP_JUMP_IF_VB_NOT_VISIBLE: //<relOffset> // jump relative if current VB vertices not visible
	case GDOP_JUMP_IF_BBOX_NOT_VISIBLE: //<relOffset> <bbox> // jump relative if bbox not visible
		{ GDRelOffset offset = context.getRelOffset();
		  context.getBBoxl(bbox);
		  if (!state.Cullcheck(bbox)) context.jumpRel(offset);
		  continue;
		}

#endif
		
	case GDOP_PUSH:		// push state stack
			state.Push(); 
			continue;

	case GDOP_POP:		// pop state stack
			state.Pop(); 
			continue;

	case GDOP_TRANSFORM_ID: 	// <TransformId>  // multiply with transform Id
		{

			GDId id = context.getId();
			GvNode *n= (this->transform.get1(id));
			if (!n) continue;
			if (n->getMatrix(m))
				state.AppendMatrix(m);
			continue;
		}
		

	case GDOP_TRANSFORM:	// <matrix>	// muliply with modelling matrix immidiate
		{
			context.getMatrix(m);
			state.AppendMatrix(m);
			continue;
		}
	case GDOP_TRANSLATE:	// <float3>	// muliply with translation matrix immidiate
		{
			context.getPoint(pt);
			m.TranslationMatrix(pt);
			state.AppendMatrix(m);
			continue;
		}
	case GDOP_ROTATE:	// <float3>	// muliply with rotationxyz matrix immidiate
		{
			context.getPoint(pt);
			m =RotationMatrix(pt);
			state.AppendMatrix(m);
			continue;
		}
	case GDOP_ROTATE_AXIS:	// <float4>	// muliply with quaternation immidiate
		{	float angle;
			context.getAxisAngle(pt,angle);
			m =RotationAxisMatrix(pt,angle);
			state.AppendMatrix(m);
			continue;
		}
	case GDOP_SCALE:	// <float3>	// muliply with scale matrix immidiate
		{
			context.getPoint(pt);
			m=ScaleMatrix(pt);
			state.AppendMatrix(m);
			continue;
		}

	
	// set cullding & handed ness 
	case GDOP_SET_CCW_SOLID:			// set ccw TRUE, solid TRUE
			// apply shape hints state.glState->SetCulling(Solid(),!ccw.value);
		    state.glState->SetCulling(gtrue,!gtrue);
			continue;


	case GDOP_SET_CCW_NOT_SOLID:		// set ccw TRUE, solid FALSE
		    state.glState->SetCulling(gfalse,!gtrue);
			continue;
	
	case GDOP_SET_NOT_CCW_SOLID:		// set ccw FALSE, solid TRUE
		    state.glState->SetCulling(gtrue,gtrue);
			continue;
	case GDOP_SET_NOT_CCW_NOT_SOLID:	// set ccw FALSE, solid FALSE
		    state.glState->SetCulling(gfalse,gtrue);
			continue;

	case GDOP_SET_MATERIAL:		// <MatId> // set material 
		{ 
			GDId id = context.getId();
			Gv2Material *n= (Gv2Material*)(this->material.get1(id));
			if (!n) n = state.defaultMaterial;
			state.material = n;
			state.material->Apply(state,0,0);
			continue;
		}
	case GDOP_SET_TEXTURE:		// <TexId> // set texture
		{ 
			GDId id = context.getId();
			GvTexture *n;
			n=(GvTexture*)(this->texture.get1(id));
			
			//if (!n) n = state.defaultTexture;
			if (n) {
				state.texture = n;
				//state.texture->Apply(state);
			}
			else { // NULL texture 
			}

			continue;
		}

	case GDOP_SET_APPEARANCE:	// <AppId> // set appearance
		{ 
			GDId id = context.getId();
			GvAppearance *app=(GvAppearance*) (this->apperance.get1(id));
			if (!app) app = state.defaultAppearance;
			state.appearance = app;
			state.appearance->Apply(state);
			continue;
		}

	case GDOP_SET_VB: 			// <VbId>  // set current vertex buffer,
		{	
			GDId id = context.getId();
			GvVertexBuffer *n=(GvVertexBuffer*) (this->vertexBuffer.get1(id));
			if (n) {

			}
			continue;
		}

	case GDOP_SET_VB_IM: 		// <vertexFormat> <bbox> <cnt> <bytesPerVertex> <align4> <vertexData>
		{
			dwVertexTypeDesc=context.getUINT32();
			context.getBBox(vertexBBox);
			vertexCount = context.getCnt();
			bytesPerVertex = context.getCnt();
			context.align4();
			lpvVertices = context.Current();

			context.Skip((size_t) vertexCount * (size_t) bytesPerVertex);
			continue;

		}	
#ifdef _D3D

    case GDOP_DRAW_PRIM: // <primType> <vertexOffset> <vertexCount> <Option> // Drawprimitive
		{
			GDId primType = context.getId();
			GDIndex  vertexOffset = context.getIndex();
			GDCnt	 primVertexCount = context.getCnt();
			GDId	 option = context.getId();

			RenderState &rstate = *state.glState;
			D3DPRIMITIVETYPE dptPrimitiveType = D3DPT_POINTLIST;
			
			DWORD  dwFlags=0;

			// clamp
			if (primVertexCount >vertexCount) primVertexCount=vertexCount;
			
			if (primVertexCount >0 ) { // skip if no vertices (yet)
			if (primType <=	D3DPT_TRIANGLEFAN) dptPrimitiveType  = (D3DPRIMITIVETYPE) primType;
			
			if (option & 1) dwFlags |= D3DDP_DONOTLIGHT; 

			if (rstate.dx6) {
				rstate.GLastError =	rstate.lpD3DDevice3->DrawPrimitive(
					dptPrimitiveType,  dwVertexTypeDesc, lpvVertices,     
					primVertexCount,dwFlags);
 			}
			else if (rstate.dx5) {
				D3DVERTEXTYPE vType=D3DVT_FORCE_DWORD;
				if (dwVertexTypeDesc == D3DFVF_LVERTEX) vType = D3DVT_LVERTEX;
				else if (dwVertexTypeDesc == D3DFVF_TLVERTEX ) vType = D3DVT_TLVERTEX;
				if (dwVertexTypeDesc == D3DFVF_VERTEX) vType = D3DVT_VERTEX;
				if (vType!=D3DVT_FORCE_DWORD)
					rstate.GLastError =	rstate.lpD3DDevice2->DrawPrimitive(
						dptPrimitiveType, vType, lpvVertices, primVertexCount,dwFlags);
 			}
			if (rstate.GLastError != D3D_OK) { D3DError("Error DrawPrimitive",rstate.GLastError); }

			}

			continue;
		}

    case GDOP_DRAW_POLY: // <plane> <vertexOffset> <vertexCount>  // Drawprimitive
		{
			context.getPlane(plane);
			GDIndex  vertexOffset = context.getIndex();
			GDCnt	 primVertexCount = context.getCnt();
			//if (!BackFaceTest(plane)) continue;

			// draw it 
			continue;
		}

    case GDOP_DRAW_INDEXED_PRIM: // <primType> <vertexOffset> <vertexCount> <Option> <numIndex> <Align2> <indices> // Drawprimitive
		{
			GDId primType = context.getId();
			GDIndex  vertexOffset = context.getIndex();
			GDCnt	 primVertexCount = context.getCnt();
			GDId	 option = context.getId();

			GDCnt	 primIndexCount = context.getCnt();
			context.align2();
			LPWORD lpwIndices = (WORD *) context.Current();

			context.Skip((size_t) primIndexCount * sizeof(WORD));


			RenderState &rstate = *state.glState;
			D3DPRIMITIVETYPE dptPrimitiveType = D3DPT_POINTLIST;
			
			DWORD  dwFlags=0;

			// clamp
			if (primVertexCount >vertexCount) primVertexCount=vertexCount;
			
			if (primVertexCount >0 ) { // skip if no vertices (yet)
			if (primType <=	D3DPT_TRIANGLEFAN) dptPrimitiveType  = (D3DPRIMITIVETYPE) primType;
			
			if (option & 1) dwFlags |= D3DDP_DONOTLIGHT; 

			if (rstate.dx6) {
				rstate.GLastError =	rstate.lpD3DDevice3->DrawIndexedPrimitive(
					dptPrimitiveType,  dwVertexTypeDesc, lpvVertices,     
					primVertexCount,lpwIndices,primIndexCount,dwFlags);
 			}
			else if (rstate.dx5) {
				D3DVERTEXTYPE vType=D3DVT_FORCE_DWORD;
				if (dwVertexTypeDesc == D3DFVF_LVERTEX) vType = D3DVT_LVERTEX;
				else if (dwVertexTypeDesc == D3DFVF_TLVERTEX ) vType = D3DVT_TLVERTEX;
				if (dwVertexTypeDesc == D3DFVF_VERTEX) vType = D3DVT_VERTEX;
				if (vType!=D3DVT_FORCE_DWORD)
					rstate.GLastError =	rstate.lpD3DDevice2->DrawIndexedPrimitive(
						dptPrimitiveType, vType, lpvVertices, primVertexCount,lpwIndices,primIndexCount,dwFlags);
 			}
			if (rstate.GLastError != D3D_OK) { D3DError("Error DrawIndexedPrimitive",rstate.GLastError); }

			}

			continue;
		}

#endif

	
#if 0

	case GDOP_SET_RENDER_OP,		// <OpID> <data> // set render operation
	case GDOP_SET_TEXTURE_OP,	// <OpID> <data> // set texture operation
	case GDOP_SET_TRANSFORM_OP,  // <OpID> <data> // set transform operation
	
	case GDOP_SET_SELECTION_CD_ON,		// set coll detection on
	case GDOP_SET_SELECTION_CD_OFF,		// set coll detection off

	case GDOP_JUMP_IF_SELECTION,    //<relOffset> // jump relative if currently in selection processing mode
	
	case GDOP_SET_ANCHOR,	//  <AnchorInfo>
	case GDOP_BSP_PLANE,		// <plane> left<relOffset> on<relOffset> right<relOffset>	// BspTree node
	case GDOP_BSP_PLANE_B2F, // <plane> left<relOffset> on<relOffset> right<relOffset>// BspTree back 2 front 

	case GDOP_RANGE_LOD,		// <cnt> <levels> <relOffsets>		// LOD node
	case GDOP_SWITCH,		// <ChoiceId> <cnt> <relOffsets>	// Switch  node
	
	case GDOP_BILLBOARD,		// axisOfRotion			// BillBoard
	case GDOP_BILLBOARD_SCREEN,	// 	// BillBoard screen aligned
#endif
	default :	// unhandled op
		ASSERT(0);
		goto fail_;
	} // switch

	} // while

return_: ;
	return 1;
fail_:
	return -1;
	
}



int GvCompiledGroup::Do(GglRenderTraversal &state)
{
  if (!loader.Ok()) {
	if ((state.inlineLevel <= state.maxInlineLevel)
  		&& state.loadInlines >= GTraversal::LOAD_IF_NEEDED)
		Load(state);
  }	

  if (!loader.Ok()) return 0;


  if (streams) {   // for all streams
  for (int i=0; i<streams->NumStreams(); i++) {
	  GStream *s=streams->GetStream(i);

	  if (!s) continue;
	  // if its a display list
	  if (s->streamType == G_STYPE_DISPLAYLIST) {
		  GMyDisplayListStream *sd= (GMyDisplayListStream *) s;
		  // for all packets in the displaylist
		  for(int j=0;j<sd->packets.Length();j++)
			  Do(*sd->packets[i],state); // render packet
	  }		
  }	
  
  }

  return(1);
}

