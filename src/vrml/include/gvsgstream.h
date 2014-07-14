#ifndef  _GvSGStream_h_
#define  _GvSGStream_h_

/******************************************************************************
@doc

@module GvSGStream.h - blaxxun binary VRML stream|

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

// encoding types for all builtin field types

//....






class GvSGFieldInfo {

public:
	int internalFieldId: // the index into the GvFieldInfo of the node



	TypeId		eventTypeId;	// the event/field type GvSFString ...
	GvFieldType type;			// type of event/field
	GvFieldClass fieldClass;	// we need this for VRML script 
	
	GvField		*defaultValue; // the default value 
	
	//! how field values are encoded, type specific
	int encoding;		

	// encoder specific data here :


};


typedef enum {
	GVSGFID_BYTE7,	// 7 bits, one bit left for encoding of SFBool
	GVSGFID_BYTE,
	GVSGFID_WORD15,	// 15 bits, one bit left for encoding of SFBool
	
	// GVSFFID_BITS,	// use fieldIdBits bits


} GvSGFieldIdEncoding;

typedef enum {
	GVSGNID_BYTE,	// one unsigned byte
	GVSGNID_WORD,	// one unsigned word 
	GVSGNID_INT,	// one unsigned int
} GvSGNodeIdEncoding;

// node type/value encoding

typedef enum {
	GVSGNT_BYTE6,	// 6 bits, upper 2 bits: 0: normal, 1, NULL, 2:DEF ,3 USE
	GVSGNT_WORD14,	// 14 bits, upper 2 bits: 0: normal, 1, NULL, 2:DEF ,3 USE
	GVSGNT_INT30,	// 14 bits, upper 2 bits: 0: normal, 1, NULL, 2:DEF ,3 USE
	
}	GvSGNodeTypeEncoding;


class GvSGNodeInfo 
{

	int fieldIdBits;	// number of bits
	
	GvSGFieldIdEncoding fieldIdEncoding; // how field ids are encoded

	//! interface data of PROTO or builtin node 
	GvFieldData *fieldData;

	Array<GvSGFieldInfo*> fieldInfo; // encoding info for fields, directly indexed by fieldId


	// is this a proto/externproto node
	gvBool isProto;
	GvProto *proto;	//< the proto definition

	// the type id, has the new() member is used to crate a node instance
	TypeId nodeClassId;


	GvSGNodeInfo() : fieldIdBits(8),fieldIdEncoding(GVSGFID_BYTE7),fieldData(NULL),isProto(gfalse),proto(NULL) {}
	~GvSGNodeInfo() {};

	// create an instance for the node
	GvNode *createInstance()
	{
		if (isProto) return proto->NewInstance()
		GvNode *n = (GvNode *) nodeClassID.New();
		
		if (fieldData == NULL) // setup field data
			fieldData = n->getFieldData();
		return n;

	}

	// set node info as beeing instances of given proto argument
	void setProto (GvProto *p) {
		proto = p;
		isProto = (proto != NULL);
		fieldData=p->getFieldData();
	}

	// encode data
	int maxNumFields; // max number of used fields in file


};


class GvSGStreamDecoder 
{

public:



	int readField(GvField *field,GvSGFieldInfo*fieldInfo);

	//! read the fields of a node
	int readNodeInstance(GvNode *node,GvSGNodeInfo *nodeInfo);
	/*
		<numFields>

		(<fieldId> <fieldValue>)*
	*/

	int readNode(GvNode **node);

	/*
		<nodeType&info>

		if (use) <readNodeId>
		else {
			if (def) <readNodeId>
			// create node
		
			<readNodeInstace>
		}

	*/

	// basic types
	//! read num floats into allocated buffer
	int read(int encoding, int num,float *buffer);
	
	//! read num double into allocated buffer
	int read(int encoding, int num,double *buffer);
	
	//! read num floats into allocated buffer
	int read(int encoding, int num,int *buffer);

	//! read num strings into allocated buffer
	int read(int encoding, int num,GvString *buffer);


	//! read a list of nodes
	int readNodes(GvMFNode &nodes);

	//! read a list of PROTO definitions 
	int readProtos(GvMFNode &protos);
	
	//! read a list of EXTERNPROTO definitions 
	int readExternProtos(GvMFNode &protos);

	//! read a list of ROUTE definitions 
	int readRoutes(GvMFNode &routes);


	// meta block
	int readNodeInfoTable();


	int nodedIdBits;	//! number of bits for encoding node id's
	GvSGNodeIdEncoding nodeIdEncoding; //! how node ids are encoded

	int nodedTypeBits;	//! number of bits for encoding node types
	GvSGNodeTypeEncoding nodeTypeEncoding; // how node types are encoded


	Array<GvSGNode*> nodeInfo; // encoding info for fields, directly indexed by fieldId

	// node dictonary index by typeId


};


/*

file :

	<encoding info for stream> 

	// chunks updating nodeInfo of stream
	<nodeInfoTable>
	<protoBlock>
	<externprotoBlock>

	<nameBlock>
	<nodeIdNameTable> 

	<topLevelSceneNodes>

	<routes>
	
	<scenegraphupdatecommands>


*/



class GvSGStreamEncoder
{

public:
	// statistics
	int numNodes;

	int numNodeTypes;
	int maxFieldsPerNode;

	// correspoding structures from decoder


	int nodedIdBits;	//! number of bits for encoding node id's
	GvSGNodeIdEncoding nodeIdEncoding; //! how node ids are encoded

	int nodedTypeBits;	//! number of bits for encoding node types
	GvSGNodeTypeEncoding nodeTypeEncoding; // how node types are encoded


	Array<GvSGNode*> nodeInfo; // encoding info for fields, directly indexed by fieldId

	// node dictonary index by typeId

	// preprocess path
	
	// do we already know this nodetype ??
	gbool isNodeTypeDefined(TypeId id) 
	{
		for (int i=0; i<nodeInfo.Length(); i++) {
			GvSGNode &n = *(nodeInfo[i]);
			if (!n.isProto && n.nodeClassId == id) return gtrue;
		}
		return gfalse;
	}
	
	// do we already know this nodetype proto ??
	gbool isNodeTypeDefined(GvProto *proto) 
	{
		for (int i=0; i<nodeInfo.Length(); i++) {
			GvSGNode &n = *(nodeInfo[i]);
			if (n.isProto && n.proto == proto) return gtrue;
		}
		return gfalse;
	}

	//
	int preprocessNode(GvNode *n);
	int preprocessScene(GvScene *n);


	void write()
	{
		writeEncoderInfo();
		writeNodeInfoTable();
		writeSceneGraphs();
	}

	void writeNodeInfoTable()
	{
	}
	
	void writeProtos(GvMFNode &protos)
	{
	}

	void writeScene(GvScene *scene)
	{
		writeProtos(scene->protos);
		writeNodes(scene->children);
		writeRoutes(scene->routes);
	}

};








#endif  _GvSGStream_h 


