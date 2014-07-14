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
#ifndef  _GV_SF_IMAGE_
#define  _GV_SF_IMAGE_
/******************************************************************************
@doc

@module GvSFImage.h - VRML GvSFImage field|

Copyright (c) 1996 - 1998	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvSFImage>

Notes:

Changes:
$Revision: 1.17 $
$Log: gvsfimage.h,v $
Revision 1.17  1999/07/06 16:35:23  tom
*** empty log message ***

Revision 1.16  1999/05/10 13:54:16  tom
*** empty log message ***


Todo :

******************************************************************************/


#include <gvsubfield.h>
//
// GvSFImage
// 

class GvSFImage : public GvSField 

#ifdef _COM
	,public CComDualImpl<EventInSFImage, &IID_EventInSFImage, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutSFImage, &IID_EventOutSFImage, &LIBID_GLViewVrmlLib>
#endif


{
  public:
    short		size[2];		//!< Width and height of image
    int			numComponents;	//!< Number of components per pixel
    unsigned char *	bytes;		//!< Array of pixels

    GV_SFIELD_HEADER(GvSFImage);
    GvSFImage();

	static GvFieldClass getClassFieldType() { return (SFIMAGE);}
	GvFieldClass getFieldType() const { return (SFIMAGE); }

	//! get length of the flat int array (getData)
	int getDataLength() const { return size[0]*size[1]*numComponents; };

	int getDataLength(int w,int h,int c) const { return w*h*c; };

	void FlushData() { if (bytes) free(bytes); bytes = NULL; }

	//! get value of pixel packed in long
	inline unsigned long unpack(const unsigned char *bytes) const {
				unsigned long l=0;
				for (int j = 0; j < numComponents; j++)
	    			l |= (((unsigned long) *bytes++ << (8*(numComponents-1-j))));
				return l;
	}
	
	//! set value, unpack from long 
	inline void pack(unsigned char *bytes,unsigned long l) {
		for (int j = 0; j < numComponents; j++)
	    	*bytes++ =
			(unsigned char) ((l >> (8*(numComponents-j-1))) & 0xFF);
	}

	
	//! get 1 pixel 
	unsigned long get1(int o) const {
				return unpack(bytes + o*numComponents);
	}
	
	unsigned long getxy(int x,int y) const {
				int o = y * size[0]+x;
				return unpack(bytes + o*numComponents);
	}

	//! set 1 pixel 
	void set1(int o,unsigned long l) {
				pack(bytes + o*numComponents,l);
	}

	void set1(int o,int l) {
		 pack(bytes + o*numComponents,(unsigned long) l);
	}

	// float support  
	void unpack(const unsigned char *bytes,float *value,float scale=1.0f/255.0f) const {
				for (int j = 0; j < numComponents; j++)
	    			*value++ = scale * (float) *bytes++;
	}
	
	// set value
	void pack(unsigned char *bytes,const float *value,float scale=255.0f) {
		for (int j = 0; j < numComponents; j++)
	    	*bytes++ = (unsigned char) (int) (value[j] * scale);
	}


	int ioValue(AStream &a); 
	
	int get(GvString &value);
	
	int set(const char* value);
    
    // warning data gets owned
    int set(int width, int height, int comp,unsigned char *data);
	
	// copy, 
	//! set with data copy
	int set(int width, int height, int comp,const unsigned long *data);

	//! set with data copy
	int set(int width, int height, int comp,const float *data);

	//! set with data copy and stride = (offset between rows)
    int set(int width, int height, int comp,int stride, const unsigned char *data);

	//! get the data to buffer
	int getData(int cnt,unsigned long *data);

    //! change to RGBA + set alpha
    int setAlpha(float alpha = 1.0);
    
    //! change to RGBA + set alpha if color matches rgb
    int setAlpha(const float *rgb,float alpha = 1.0);

    // set image to a scaled version
    // used fast integer scaling without interpolation
    // rowbytes can be 0, and will be automatically set
    // otherwise by modified data + width + rowbytes a subimage can be specified
    int setScaled(int newWidth,int newHeight, 
                        // Src Image
                        int width, int height, int comp,
                        const unsigned char *data,
                        int rowBytes=0);
	
	//! set subimage 
	int setSubImage(int xoffset, int yoffset,
		const GvSFImage &src,
		int width, int height,	// optional
		int srcXoffset,int srcYoffset // optional 
		);


    const GvSFImage& operator= (const GvSFImage &f); 
    GvBool operator==(const GvSFImage &f) const;

#ifdef _COM
	BEGIN_COM_MAP(GvSFImage)
		COM_INTERFACE_ENTRY(EventInSFImage)
		COM_INTERFACE_ENTRY(EventOutSFImage)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvSFImage)

	COM_DEFAULT_SFIELD_METHODS()

    /* EventIn methods */
    STDMETHOD (setValue)( 
            /* [in] */ int width,
            /* [in] */ int height,
            /* [in] */ int numComponents,
            /* [in] */ int cnt,
            /* [size_is][in] */ unsigned small *pixels);


	/* EventOut methods */ 
    STDMETHOD(getWidth)( /* [retval][out] */ int *value);
        
    STDMETHOD(getHeight)(/* [retval][out] */ int *value);
        
    STDMETHOD(getNumComponents)(/* [retval][out] */ int  *value);
        
    STDMETHOD(getPixels)( 
            /* [in] */ int cnt,
            /* [size_is][out][in] */ unsigned small *value);
#endif



};

#endif /* _GV_SF_IMAGE_ */
