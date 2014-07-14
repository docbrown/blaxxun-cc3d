// XMESAGL.h : header file
//
// XMESAGL class
//

#ifndef __XMESAGL__
#define __XMESAGL__

#include "grenderbase.h"

class GXMesaDevice : public GRenderDevice
{

public://functions
    //
	//constructor
	//
	GXMesaDevice();

	//
	//destructor
	//
    ~GXMesaDevice();

 	// Helpful functions
	void OutputGlError(char* label); 
	void MakeCurrent();

    void Destroy(){};
	
};					   


#endif // __XMESAGL__
