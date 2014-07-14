

/*

GDisplaylist

*/

#include "stdafx.h"
#include "stack.h"
#include "gdisplaylist.h"



GDExecutionContext::GDExecutionContext(size_t size, GMemPtr base):
	 GMemMap(size,base)
{ 

	bboxEncoding=GDF_FLOAT;
	planeEncoding=GDF_FLOAT;
	coordEncoding=GDF_FLOAT;
	texCoordEncoding=GDF_FLOAT;
	normalEncoding=GDF_FLOAT;
	matrixEncoding=GDF_FLOAT;

}

GDExecutionContext::~GDExecutionContext()
{
}

// ! verify the current instruction pointer 
gbool GDExecutionContext::VerifyEx() 
{
	ASSERT( (current>=base) && (current <= (base + (size >>2)))); 
	return gtrue;
}


// example displaylist decoder 
gbool GDVerify(size_t	size,GMemPtr base) 
{
	GDExecutionContext context(size,base);
	gbool ok=gfalse;

	while (1) {
	
	if (!context.VerifyEx()) goto fail_;

	switch (context.getOp()) {
	case GDOP_NOP: continue;		// NO-OP
	case GDOP_EXIT: {
		goto return_;		// exit current block
					}
	case GDOP_RETURN: {
		if (!context.VerifyCallStack()) goto fail_;
		context.return_();	continue; // return from call 
		}
	
	case GDOP_JUMP:		//<relOffset> // jump relative
		{ GDRelOffset offset = context.getRelOffset();
	  	  
		  //if (!context.VerifyRelJump(offset)) goto fail_;

		  context.jumpRel(offset);
		  continue;
		}
	case GDOP_JUMP_ABS:	//<absOffset> // jump absolute
		{ GDAbsOffset offset = context.getAbsOffset();
		  context.jumpAbs(offset);
		  continue;
		}

	case GDOP_CALL:		//<relOffset> // call relative
		{ GDRelOffset offset = context.getRelOffset();
		  context.callRel(offset);
		  continue;
		}

	case GDOP_CALL_ABS:	//<absOffset> // call absolute
		{ GDAbsOffset offset = context.getAbsOffset();
		  context.jumpAbs(offset);
		  continue;
		}
	
	case GDOP_PUSH:		// push state stack
	case GDOP_POP:		// pop state stack

		continue;
	default :	// unhandled op
		ASSERT(0);
	}
	}

return_: ;
	return gtrue;
fail_:
	return gfalse;

}



