
#ifndef _gdisplaylist_h
#define _gdisplaylist_h

/*

GDisplaylist

*/

#include "stack.h"

#include "gstream.h"


typedef SHORT GDRelOffset;
typedef USHORT GDAbsOffset;

typedef INT32 GDRelOffset32;
typedef UINT32 GDAbsOffset32;


typedef USHORT GDOpCode;

// points to an instruction block
#define GDOP_ALIGN 2

typedef GDOpCode *GDOpPtr;

typedef USHORT GDId;

typedef USHORT GDMatId;
typedef USHORT GDTexId;
typedef USHORT GDAppId;
typedef USHORT GDVbId;

typedef USHORT GDCnt;
typedef USHORT GDIndex;
typedef USHORT GDNumIndex;


typedef enum {
	GDOP_NOP,		// NO-OP
	GDOP_EXIT,		// exit current block
	GDOP_RETURN,	// return from call 
	
	GDOP_JUMP,		//<relOffset>	// jump relative
	GDOP_JUMPL,		//<relOffset32> // jump relative
	GDOP_JUMP_ABS,	//<absOffset>	// jump absolute
	GDOP_JUMPL_ABS,	//<absOffset32> // jump absolute

	GDOP_CALL,		//<relOffset> // call relative
	GDOP_CALLL,		//<relOffset32> // call relative
	GDOP_CALL_ABS,	//<absOffset> // call absolute
	GDOP_CALLL_ABS,	//<absOffset32> // call absolute

	GDOP_CALL_PACKET,	// <PacketId> <absOffset>	// call packet at offset if present
	GDOP_CALLL_PACKET,	// <PacketId32> <absOffset32>

	GDOP_JUMP_IF_VB_NOT_VISIBLE, //<relOffset> // jump relative if current VB vertices not visible
	GDOP_JUMP_IF_BBOX_NOT_VISIBLE, //<relOffset> <bbox> // jump relative if bbox not visible

	
	GDOP_PUSH,		// push state stack
	GDOP_POP,		// pop state stack

	GDOP_TRANSFORM_ID, 	// <TransformId>  // multiply with transform Id
	
	GDOP_TRANSFORM,	// <matrix>	// muliply with modelling matrix immidiate
	GDOP_TRANSLATE,	// <float3>	// muliply with translation matrix immidiate
	GDOP_ROTATE,	// <float3>	// muliply with rotationxyz matrix immidiate
	GDOP_ROTATE_AXIS,	// <float4>	// muliply with quaternation immidiate
	GDOP_SCALE,	// <float3>	// muliply with scale matrix immidiate
	
	// set cullding & handed ness 
	GDOP_SET_CCW_SOLID,			// set ccw TRUE, solid TRUE
	GDOP_SET_CCW_NOT_SOLID,		// set ccw TRUE, solid FALSE
	
	GDOP_SET_NOT_CCW_SOLID,		// set ccw FALSE, solid TRUE
	GDOP_SET_NOT_CCW_NOT_SOLID,	// set ccw FALSE, solid FALSE

	GDOP_SET_MATERIAL,		// <MatId> // set material 
	GDOP_SET_TEXTURE,		// <TexId> // set texture
	GDOP_SET_APPEARANCE,	// <AppId> // set appearance

	GDOP_SET_VB, 			// <VbId>  // set current vertex buffer,
	
	GDOP_SET_VB_IM, 		// <vertexFormat> <bbox> <cnt> <bytesPerVertex> <align4> <vertexData>

    GDOP_DRAW_PRIM, // <primType> <vertexOffset> <vertexCount> <Option> // Drawprimitive
    GDOP_DRAW_POLY, // <plane> <vertexOffset> <vertexCount>  // Drawprimitive

    GDOP_DRAW_INDEXED_PRIM, // <primType> <vertexOffset> <vertexCount> <Option> <numIndex> <indices> // Drawprimitive
	
	GDOP_SET_RENDER_OP,		// <OpID> <data> // set render operation
	GDOP_SET_TEXTURE_OP,	// <OpID> <data> // set texture operation
	GDOP_SET_TRANSFORM_OP,  // <OpID> <data> // set transform operation
	
	GDOP_SET_SELECTION_CD_ON,		// set coll detection on
	GDOP_SET_SELECTION_CD_OFF,		// set coll detection off

	GDOP_JUMP_IF_SELECTION,    //<relOffset> // jump relative if currently in selection processing mode
	
	GDOP_SET_ANCHOR,	//  <AnchorInfo>
	GDOP_BSP_PLANE,		// <plane> left<relOffset> on<relOffset> right<relOffset>	// BspTree node
	GDOP_BSP_PLANE_B2F, // <plane> left<relOffset> on<relOffset> right<relOffset>// BspTree back 2 front 

	GDOP_RANGE_LOD,		// <cnt> <levels> <relOffsets>		// LOD node
	GDOP_SWITCH,		// <ChoiceId> <cnt> <relOffsets>	// Switch  node
	
	GDOP_BILLBOARD,		// axisOfRotion			// BillBoard
	GDOP_BILLBOARD_SCREEN,	// 	// BillBoard screen aligned


} GDOpCode_Enum;


// exactly maps  D3DPRIMITIVETYPE
typedef enum {
    GDPT_POINTLIST     = 1,
    GDPT_LINELIST       = 2,
    GDPT_LINESTRIP     = 3,
    GDPT_TRIANGLELIST  = 4,
    GDPT_TRIANGLESTRIP = 5,
    GDPT_TRIANGLEFAN   = 6,
} GDPrimType;

// Flexible vertex format bits
// maps D3D level 7 bits
#define GDFVF_RESERVED0        0x001
#define GDFVF_POSITION_MASK    0x00E
#define GDFVF_XYZ              0x002
#define GDFVF_XYZRHW           0x004
#define GDFVF_XYZB1            0x006
#define GDFVF_XYZB2            0x008
#define GDFVF_XYZB3            0x00a
#define GDFVF_XYZB4            0x00c
#define GDFVF_XYZB5            0x00e
#define GDFVF_NORMAL           0x010
#define GDFVF_RESERVED1        0x020
#define GDFVF_DIFFUSE          0x040
#define GDFVF_SPECULAR         0x080

#define GDFVF_TEXCOUNT_MASK    0xf00
#define GDFVF_TEXCOUNT_SHIFT   8
#define GDFVF_TEX0             0x000
#define GDFVF_TEX1             0x100
#define GDFVF_TEX2             0x200
#define GDFVF_TEX3             0x300
#define GDFVF_TEX4             0x400
#define GDFVF_TEX5             0x500
#define GDFVF_TEX6             0x600
#define GDFVF_TEX7             0x700
#define GDFVF_TEX8             0x800

#define GDFVF_RESERVED2        0xf000  // 4 reserved bits


class GBufferStream 
{
public :

};


class GDExecutionContext : public GMemMap {
public :

	Stack <GMemPtr> callStack; //! the call stack

	GDExecutionContext(size_t	size, GMemPtr base);
	~GDExecutionContext();

	gbool VerifyEx();
	gbool VerifyCallStack() { return callStack.Length()>0; }


	GDFloatEncoding bboxEncoding;
	GDFloatEncoding planeEncoding;
	GDFloatEncoding coordEncoding;
	GDFloatEncoding texCoordEncoding;
	GDFloatEncoding normalEncoding;
	GDFloatEncoding matrixEncoding;

	//! get opcode of next instruction
	inline GDOpCode_Enum getOp() { return (GDOpCode_Enum) *((GDOpCode *&)current)++;; }

	//! jump relative to current instruction
	inline void jumpRel(GDRelOffset offset) { current = current + (((INT32) offset) * GDOP_ALIGN); }
	
	//! jump absolute to instruction base 
	inline void jumpAbs(GDAbsOffset offset) { current = base + (((UINT32) offset) * GDOP_ALIGN); }

	//! call relative to current instruction
	inline void callRel(GDRelOffset offset) { push(); current = current + (((INT32) offset) * GDOP_ALIGN); }
	
	//! jump absolute to instruction base 
	inline void callAbs(GDAbsOffset offset) { push(); current = base + (((INT32) offset) * GDOP_ALIGN); }

	// ! push instruction pointer
	inline void push() { callStack.Push(current); }
	
	// ! pop instruction pointer
	inline void pop() { current = callStack.Pop(); }
	
	//! execute return instruction 
	inline void return_() { pop(); }
	
	// ! get an relative instruction offset
	inline GDRelOffset getRelOffset() { return *((GDRelOffset *&)current)++; }
	inline GDRelOffset32 getRelOffset32() { return *((GDRelOffset32 *&)current)++; }
	
	// ! get an absolute instruction offset TO DO Alignment ?
	inline GDAbsOffset getAbsOffset() { return *((GDAbsOffset *&)current)++; }
	inline GDAbsOffset32 getAbsOffset32() { return *((GDAbsOffset32 *&)current)++; }

	inline GDId getId() { return *((GDId *&)current)++; }

	void align4() { // UINT32 *&p = ((UINT32 *)current);
					if (((UINT32) current) & 2) current+=1;
	}
	void align8() { }
	void align16() { }
	void align32() { }

	inline void getFloats(GDFloatEncoding encoding, size_t cnt, float *buffer) 
	{

		switch (encoding) {
		case GDF_FLOAT : { size_t s=cnt*sizeof(float);
						   align4(); 
						   memcpy(buffer,current,s); 
						   ((UINT32 *&)current) +=s;
						   return; 
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
						   
						   return; 
						 }
		case GDF_SHORT : { 
						   size_t s=cnt*sizeof(SHORT);
						   const SHORT *p= (const SHORT *) current;
						   if (cnt>0) 
						   do {
							  *buffer++ = *p++;	
						   } while (cnt--);	   

						   ((UINT32 *&)current) +=s;
						   
						   return; 
						 }
		case GDF_FIX_8_8 : { 
						   size_t s=cnt*sizeof(SHORT);
						   const SHORT *p= (const SHORT *) current;
						   if (cnt>0) 
						   do {
							  *buffer++ = Fix8_8(*p++);	
						   } while (cnt--);	   

						   ((UINT32 *&)current) +=s;
						   
						   return; 
						 }

		case GDF_FIX_13_3 : { 
						   size_t s=cnt*sizeof(SHORT);
						   const SHORT *p= (const SHORT *) current;
						   if (cnt>0) 
						   do {
							  *buffer++ = Fix13_3(*p++);	
						   } while (cnt--);	   

						   ((UINT32 *&)current) +=s;
						   
						   return; 
						 }
		}
	}

	void getPlane (Plane &p) {
		getFloats(planeEncoding,4,&p.n.x);
	}

	void getPoint (Point &p) {
		getFloats(matrixEncoding,3,&p.x);
	}
	void getAngle(float &a) {
		getFloats(matrixEncoding,1,&a);
	}

	void getAxisAngle(Point &p,float &a) {
		getFloats(matrixEncoding,3,&p.x);
		getAngle(a);
	}
	
	void getBBox (BBox &p) {
		getFloats(bboxEncoding,6,&p.mi.x);
	}

	void getMatrix (Matrix &m) {
		getFloats(matrixEncoding,16,&m.x[0][0]);
		// contents is anything by default 
	}

	// return a string pointer 
	void getString(const char * &string) {

	   UINT32 s=getCnt(); 
	   string = (const char *) current;
	   if (s&1) s+=1; // 2 byte aligned 
 	   ((UINT32 *&)current) +=s;
	}


};



/*
	chunks
	
	MaterialTable
	TextureTable 
	SoundTable
	


*/

#endif _gdisplaylist_h

