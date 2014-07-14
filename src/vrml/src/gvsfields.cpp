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
#include "stdafx.h"
/******************************************************************************
@doc

@module GvSFields.cpp - Implementation for all GvSFFields|

Copyright (c) 1996 - 1998	by Holger Grahn
All rights reserved

Purpose:

Classes:

Notes:
parts based on QvLib from SGI


Changes:
   26.02.96. SFEnum / SFBitmap no error if value not found && enumValues == NULL (Unknown Node)

$Revision: 1.32 $
$Log: gvsfields.cpp,v $
Revision 1.32  1999/07/06 16:46:13  tom
*** empty log message ***



Todo :
   what is the maxlength for writing double + float to a string

******************************************************************************/


#include <gvdebugerror.h>
#include <gvreaderror.h>
#include <gvsfbitmask.h>

#include <gvkeyword.h>

#include <quaternation.h>

// needed for parsing vrml string in SFNode

#include <gvscene.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Special characters when reading or writing value in ASCII
#define OPEN_PAREN	'('
#define CLOSE_PAREN	')'
#define BITWISE_OR	'|'

#define MAX_INT_STR 20
#define MAX_FLOAT_STR 80
#define MAX_DOUBLE_STR 80

// to do: for Mfields

GV_SFIELD_SOURCE(GvSFBitMask);

GvBool
GvSFBitMask::readValue(GvInput *in)
{
    char	c;
    GvName	n;
    int		v;

#ifdef DEBUG
    if (enumValues == NULL) {
	GvDebugError::post("GvSFBitMask::readValue",
			   "Enum values were never initialized");
	GvReadError::post(in, GV_RE_SYNTAX_ERROR, "Couldn't read SFBitMask values");
	return FALSE;
    }
#endif /* DEBUG */

    value = 0;

    // Read first character
    if (! in->read(c))
	return FALSE;

    // Check for parenthesized list of bitwise-or'ed flags
    if (c == OPEN_PAREN) {

	// Read names separated by BITWISE_OR
	while (TRUE) {
	    if (in->read(n, TRUE) && ! (! n) ) {

		if (findEnumValue(n, v))
		    value |= v;

		else {
		    if (enumValues == NULL) { // HG
				TRACE("Unknown SFBitMask mask value %s \n",n.getString());
				// to do: store value ????
			}
		    else  {
		       GvReadError::post(in, GV_RE_SYNTAX_ERROR, "Unknown SFBitMask bit "
				      "mask value \"%s\"", n.getString());
		    	// HG return FALSE;
			}
		}
	    }

	    if (! in->read(c)) {
		GvReadError::post(in, GV_RE_UNEXPECTED_EOF, "EOF reached before '%c' "
				  "in SFBitMask value", CLOSE_PAREN);
		return FALSE;
	    }

	    if (c == CLOSE_PAREN)
		break;

	    else if (c != BITWISE_OR) {
		GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Expected '%c' or '%c', got '%c' ",
				  "in SFBitMask value",
				  BITWISE_OR, CLOSE_PAREN, c);
		return FALSE;
	    }
	}
    }

    else {
	in->putBack(c);

	// Read mnemonic value as a character string identifier
	if (! in->read(n, TRUE))
	    return FALSE;

	if (! findEnumValue(n, value)) {
		    if (enumValues == NULL) { // HG
				TRACE("Unknown SFBitMask mask value %s \n",n.getString());
				// to do: store value ????
			}
		    else  {
		       GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Unknown SFBitMask bit "
				      "mask value \"%s\"", n.getString());
		    	// HG return FALSE;
			}
	}
    }

    return TRUE;
}

int
GvSFBitMask::ioValue(AStream &a)
{
	if (a.IsInput()) {
		// to do:
		return(0);
	}
	else 
	{ 

		// is there a enum equal to value ?
		const char *vals = Get();
		if (vals) return a.tok(vals);

		long mask = value;
		int cnt=0;

		//while(mask != 0) 
		{

    		int	i;

    		// Look through names table for one that matches
    		for (i = 0; i < numEnums; i++) {
				if ((value & enumValues[i])  == enumValues[i])  {
	    			 
	    			 if (cnt>0) a.Sep('|');

	    			 a.tok(enumNames[i]);
					 cnt ++;
					 mask &=  ~enumValues[i];		// zero out written bits
					 //break;
				 }
			}
		}
		if (mask !=0) {
		 	TRACE("Error writing enum values for %s %d \n",ClassName(),value);
			return(0);
		}
	    return(1);

    }
   
   return(0);
}

#if 0
int GvSFBitMask::get(GvString &s)
{
		// is there a enum equal to value ?
		const char *vals = Get();

		if (vals)  {
		    s = vals;
		} 
		else {


		long mask = value;
		int cnt=0;

		//while(mask != 0) 
		{

    		int	i;

    		// Look through names table for one that matches
    		for (i = 0; i < numEnums; i++) {
				if ((value & enumValues[i])  == enumValues[i])  {
	    			 if (cnt>0) s += "|";
	    			 s+=enumNames[i];
					 cnt ++;
					 mask &=  ~enumValues[i];		// zero out written bits
				 }
			}
		}
		if (mask !=0) {
		 	TRACE("Error writing enum values for %s %d \n",ClassName(),value);
			return(0);
		}
		}


  return(1);
}


int GvSFBitMask::set(const char* s)
{
// to do   int ret = Set(s);
  int ret=0;  
  TRACE("GvSFBitMask::set not implemented %s",s);
  setDefault(FALSE);
  OnChanged();
  return(ret);
}

#endif


//GvSFBool.cpp
#include <gvsfbool.h>

GV_SFIELD_SOURCE(GvSFBool);

GvBool
GvSFBool::readValue(GvInput *in)
{
    // accept 0 or 1
	int intValue;
    if (in->read(intValue)) {
		if (intValue != 0 && intValue != 1) {
			GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Illegal value for SFBool field: %d "
				      "(must be 0 or 1)", intValue);
			return FALSE;
		}
		value = (intValue !=0);
		return TRUE;
    }

    // read TRUE/FALSE keyword
    GvName n;
    if (! in->read(n, TRUE))
	return FALSE;
    
    if (n == TRUE_KEYWORD) {
		value = TRUE;
		return TRUE;
    }

    if (n == FALSE_KEYWORD) {
		value = FALSE;
		return TRUE;
    }

    GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Unknown value (\"%s\") for SFBool field ",
		      "(must be TRUE or FALSE)", n.getString());
    return FALSE;
}


int GvSFBool::ioValue(AStream &a)
{
	if (a.IsInput()) {
		// to do:
		return(0);
	}
	else 
	{ return a.tok(value ? "TRUE" : "FALSE");
	}
}

int GvSFBool::get(GvString &s)
{
  s = (value ? "TRUE" : "FALSE");
  return(1);
}


int GvSFBool::set(const char* s)
{

   int ret = 1;
   if (streq(s,"TRUE")) {
		set(TRUE);
   }
   else
   if (streq(s,"FALSE")) {
		set(FALSE);
   }
   else ret = 0;
    
  return(ret);

}




//GvSFColor.cpp
#include <gvsfcolor.h>

GV_SFIELD_SOURCE(GvSFColor);

GvBool
GvSFColor::readValue(GvInput *in)
{
    return (in->read(value[0]) &&
	    in->read(value[1]) &&
	    in->read(value[2]));
}


int
GvSFColor::ioValue(AStream &a)
{

    return (a.io(value[0]) &&
	    a.io(value[1]) &&
	    a.io(value[2]));
}

int GvSFColor::get(GvString &s)
{
  int ret=sprintf(s.getBuffer(3*MAX_FLOAT_STR+3),"%g %g %g",value[0],value[1],value[2]);
  s.releaseBuffer();
  return(ret > 0);
}


int GvSFColor::set(const char* s)
{
  int ret=sscanf(s,"%f %f %f",&value[0],&value[1],&value[2]);
  setDefault(FALSE);
  OnChanged();
  return(ret == 3);
}


int GvSFColor::interpolateFrom(GvField *a,GvField *b, float t) 
{ return(interpolateFrom( (GvSFColor *) a, (GvSFColor *) b,t));}

int GvSFColor::interpolateFrom(GvSFColor *a,GvSFColor *b, float t)
{
  Point x,y;
  a->getHsv(x);
  b->getHsv(y);
  Point z;
  ::Interpolate(x,1.0f-t,y,t,z);
  
  setHsv(z);

  return(1);
}

void GvSFColor::setHsv(const Point &val)
{
  float rgb[3];
  Hsv2Rgb(val.x,val.y,val.z,rgb);
  set(rgb);
}

void GvSFColor::getHsv(Point &val)
{
  Rgb2Hsv(value,val.x,val.y,val.z);
}


// convert color from hsv to rgb 
void Hsv2Rgb( float hue, float sat, float val, float vec[3])
{
    float f,q,t,p;
    int i;
    
    if (hue == 1.0f)
		hue = 0.0f;
    else
		hue *= 6.0f;
    i = (int)(floor(hue));
    f = hue-i;
    p = val*(1.0f-sat);
    q = val*(1.0f-(sat*f));
    t = val*(1.0f-(sat*(1.0f-f)));
    switch (i) {
	case 0: vec[0] = val; vec[1] = t; vec[2] = p; break;
	case 1: vec[0] = q; vec[1] = val; vec[2] = p; break;
	case 2: vec[0] = p; vec[1] = val; vec[2] = t; break;
	case 3: vec[0] = p; vec[1] = q; vec[2] = val; break;
	case 4: vec[0] = t; vec[1] = p; vec[2] = val; break;
	case 5: vec[0] = val; vec[1] = p; vec[2] = q; break;
    }
}

//
// convert rgb to HSV 
//
void Rgb2Hsv(const float vec[3], float &hue, float &sat, float &val)
{
    float max,min;
    
    max = (vec[0] > vec[1]) ? 
	((vec[0] > vec[2]) ? vec[0] : vec[2]) : 
	((vec[1] > vec[2]) ? vec[1] : vec[2]);
    min = (vec[0] < vec[1]) ? 
	((vec[0] < vec[2]) ? vec[0] : vec[2]) : 
	((vec[1] < vec[2]) ? vec[1] : vec[2]);
    
    // brightness
    val = max;
    
    // saturation
    if (max != 0.0f) 
	sat = (max-min)/max;
    else
	sat = 0.0f;
    
    // finally the hue
    if (sat  !=  0.0f) {
    	float h;
	
	if (vec[0]  ==  max) 
	    h = (vec[1] - vec[2]) / (max-min);
	else if (vec[1]  ==  max)
	    h = 2.0f + (vec[2] - vec[0]) / (max-min);
	else
	    h = 4.0f + (vec[0] - vec[1]) / (max-min);
	if (h < 0.0f)
	    h += 6.0f;
	hue = h/6.0f;
    }
    else
    	hue = 0.0f;
}



//GvSFEnum.cpp
#include <gvsfenum.h>

// Can't use macro, since we define a constructor.
RTIMP(GvSFEnum,0,GvSField);
GV_SFIELD_SOURCE_EQ(GvSFEnum);


GvSFEnum::GvSFEnum(int defaultValue) : value(defaultValue) 
{
    numEnums = 0;
    enumValues = NULL;
    enumNames  = NULL;
}

GvSFEnum::~GvSFEnum()
{
}

GvBool
GvSFEnum::findEnumValue(const GvName &name, int &val) const
{
    int	i;

    // Look through names table for one that matches
    for (i = 0; i < numEnums; i++) {
	if (name == enumNames[i]) {
	    val = enumValues[i];
	    return TRUE;
	}
    }

    return FALSE;
}

GvBool GvSFEnum::Set(const char* name)
{
    int	i;

    // Look through names table for one that matches
    for (i = 0; i < numEnums; i++) {
	if (name == enumNames[i]) {
	    set(enumValues[i]);
	    setDefault(FALSE); // to do: move into Set
  		OnChanged();


	    return TRUE;
	}
    }

    return FALSE;
}

const char *
GvSFEnum::Get() const
{
    int	i;

    // Look through names table for one that matches
    for (i = 0; i < numEnums; i++) {
		if (value == enumValues[i]) {
	    return enumNames[i];
		}
    }
    return NULL;
}


GvBool
GvSFEnum::readValue(GvInput *in)
{
    GvName n;

#ifdef DEBUG
    if (enumValues == NULL) {
	GvDebugError::post("GvSFEnum::readValue",
			   "Enum values were never initialized");
	GvReadError::post(in,GV_RE_SYNTAX_ERROR, "Couldn't read GvSFEnum value");
	return FALSE;
    }
#endif /* DEBUG */

    // Read mnemonic value as a character string identifier
    if (! in->read(n, TRUE))
	return FALSE;

    if (findEnumValue(n, value))
	return TRUE;

    // Not found? Too bad
    if (enumValues == NULL) { // HG
		TRACE("Unknown GvSFEnum value %s \n",n.getString());
		// to do: store value ????
	}
	else  {
    	GvReadError::post(in, GV_RE_SYNTAX_ERROR, "Unknown SFEnum enumeration value \"%s\"",
		      n.getString());
   }
// HG    return FALSE;
    return TRUE;
}


int GvSFEnum::ioValue(AStream &a)
{
	if (a.IsInput()) {
		// to do:
		return(0);
	}
	else 
	{ return a.tok(Get());
	}

}

int GvSFEnum::get(GvString &s)
{
  s = Get();
  return(1);
}


int GvSFEnum::set(const char* s)
{
  int ret = Set(s);
    
  setDefault(FALSE);
  OnChanged();
  return(ret);
}



//
//GvSFFloat.cpp
//
#include <gvsffloat.h>

GV_SFIELD_SOURCE(GvSFFloat);

GvBool
GvSFFloat::readValue(GvInput *in)
{
    return in->read(value);
}


int GvSFFloat::ioValue(AStream &a) 
{
   return a.io(value);
}

int GvSFFloat::get(GvString &s)
{
  int ret=sprintf(s.getBuffer(MAX_FLOAT_STR),"%g",value);
  s.releaseBuffer();

  return(ret > 0);

}


int GvSFFloat::set(const char* s)
{ float v;
  int ret=sscanf(s,"%f",&v);

  if (ret>0) set(v);
  return(ret == 1);
}

//
//GvSFTime.cpp
//
#include <gvsftime.h>

GV_SFIELD_SOURCE(GvSFTime);

GvBool
GvSFTime::readValue(GvInput *in)
{
    return in->read(value);
}


int GvSFTime::ioValue(AStream &a) 
{
   return a.io(value);
}

int GvSFTime::get(GvString &s)
{
  int ret=sprintf(s.getBuffer(MAX_DOUBLE_STR),"%.16g",value);
  s.releaseBuffer();
//  TRACE("%d \n",s.getLength());
  return(ret > 0);
}


int GvSFTime::set(const char* s)
{ double v;
  int ret=sscanf(s,"%lf",&v);
  if (ret>0) set(v);
  return(ret == 1);
}


//GvSFImage.cpp
#include <gvsfimage.h>

// Can't use macro, since we define a constructor.

RTIMP(GvSFImage,0,GvSField)

GV_SFIELD_SOURCE_EQ(GvSFImage)


GvSFImage::GvSFImage()
{
    size[0] = size[1] = 0;
    numComponents = 0;
    bytes = NULL;
}

GvSFImage::~GvSFImage()
{
    if (bytes != NULL)
    	free(bytes);
}

const GvSFImage& GvSFImage::operator= (const GvSFImage &f) 
{ 
    size[0] = f.size[0];
    size[1] = f.size[1];
    numComponents = f.numComponents;

    if (bytes != NULL)	free(bytes);
    bytes = (unsigned char *) malloc(sizeof(unsigned char) * size[0] * size[1] * numComponents);
    
    memcpy(&bytes[0],&f.bytes[0], size[0] * size[1] * numComponents);

    setDefault(FALSE);
    OnChanged();


    return(f);
}

GvBool GvSFImage::operator==(const GvSFImage &f) const 
{ 

    if ( size[0] == f.size[0] && 
         size[1] == f.size[1] && 
         numComponents == f.numComponents) 
    {
      int cnt = size[0] * size[1] * numComponents;
      while (cnt>0) {
          cnt --;
          if (bytes[cnt] != f.bytes[cnt]) return FALSE;
      }
      return(TRUE);

    } else return(FALSE);

}


GvBool
GvSFImage::readValue(GvInput *in)
{
    if (! in->read(size[0]) ||
		! in->read(size[1]) ||
		! in->read(numComponents))
		return FALSE;
    
    if (bytes != NULL)
		free(bytes);
    
    bytes = (unsigned char *) malloc(sizeof(unsigned char) * size[0] * size[1] * numComponents);
    if (!bytes) return(FALSE);


    int byte = 0;
    for (int i = 0; i < size[0] * size[1]; i++) {
		unsigned long l;
		if (! in->read(l))
	    	return FALSE;
		for (int j = 0; j < numComponents; j++)
	    	bytes[byte++] =
			(unsigned char) ((l >> (8*(numComponents-j-1))) & 0xFF);
    }
    setDefault(FALSE);
    return TRUE;
}


// set alpha channel of image, expand RGB to RGBA if needed
//
int GvSFImage::setAlpha(float alpha)
{
    if (!bytes) return(0);

    int a = alpha *255.0;
    a = max(0,min(255,a));

    int cnt = size[0] * size[1];
    unsigned char *p,*pend;


    switch (numComponents) {
    case 4:
    {
        p = bytes;
        pend = p + cnt * 4;
        p+=3;
        while (p<pend) {
            *p = a;
            p+=4;
        }
    } break;
    case 3: // expand image to 4 components
    {
        unsigned char *newBytes = (unsigned char *) malloc(sizeof(unsigned char) * size[0] * size[1] * 4);
        if (!newBytes) return(0);
        unsigned char *d=newBytes;
        p = bytes;
        pend = p + cnt * 3;
        while (p<pend) {
            *d++ = *p++;
            *d++ = *p++;
            *d++ = *p++;
            *d++ = a;
        }
        set(size[0],size[1],4,newBytes);
    }
    break;
    case 2:
    {
        p = bytes;
        pend = p + cnt * 2;
        p+=1;
        while (p<pend) {
            *p = a;
            p+=2;
        }
    } break;
    case 1: // expand image to 2 components
    {
        unsigned char *newBytes = (unsigned char *) malloc(sizeof(unsigned char) * size[0] * size[1] * 2);
        if (!newBytes) return(0);
        unsigned char *d=newBytes;
        p = bytes;
        pend = p + cnt * 1;
        while (p<pend) {
            *d++ = *p++;
            *d++ = a;
        }
        set(size[0],size[1],2,newBytes);
    }
    break;

    default :
         return(0);
    }
    setDefault(FALSE);
    OnChanged();
    return(1);
}

// set alpha channel of image to value, if pixel color matches rgb, 
// expand RGB to RGBA if needed
//

int GvSFImage::setAlpha(const float *rgb,float alpha)
{
    if (!bytes) return(0);

    int a = alpha *255.0; a = max(0,min(255,a));
    int r = rgb[0] *255.0; r = max(0,min(255,r));
    int g = rgb[0] *255.0; g = max(0,min(255,g));
    int b = rgb[0] *255.0; b = max(0,min(255,b));

    int cnt = size[0] * size[1];
    unsigned char *p,*pend;

    switch (numComponents) {
    case 4:
     {
        p = bytes;
        pend = p + cnt * 4;
        while (p<pend) {
            if (*p++ == r)
                if (*p++ == g)
                    if (*p++ == b)
                        *p++ = a;
                    else p++;
                else p+=2;
            else p+=3;
        }
    } break;
    case 3: // expand image to 4 components
    {
        unsigned char *newBytes = (unsigned char *) malloc(sizeof(unsigned char) * size[0] * size[1] * 4);
        if (!newBytes) return(0);

        unsigned char *d=newBytes;

        p = bytes;
        pend = p + cnt * 3;
        while (p<pend) {
            *d++ = *p++;
            *d++ = *p++;
            *d++ = *p++;
            if ((d[-3] == r) && (d[-2] == g) && (d[-1] == b))
                *d++ = a;
            else *d++= 255;
        }
        set(size[0],size[1],4,newBytes);
    }
    break;
    case 2:
     {
        p = bytes;
        pend = p + cnt * 2;
        while (p<pend) {
            if (*p++ == r)
                *p++ = a;
            else p+=1;
        }
    } break;
    case 1: // expand image to 2 components
    {
        unsigned char *newBytes = (unsigned char *) malloc(sizeof(unsigned char) * size[0] * size[1] * 2);
        if (!newBytes) return(0);

        unsigned char *d=newBytes;

        p = bytes;
        pend = p + cnt * 1;
        while (p<pend) {
            *d++ = *p++;
            if (d[-1] == r)
                *d++ = a;
            else *d++= 255;
        }
        set(size[0],size[1],2,newBytes);
    }
    break;

    default :
         return(0);
    }
    setDefault(FALSE);
    OnChanged();

    return(1);
}


// set image to new contents
// Warning data = malloc pointer is now owned by image,
// don´t delete your pointer afterwards

   
int GvSFImage::set(int width, int height, int comp,unsigned char *data)
{
    size[0]=width;
    size[1]=height;
    numComponents = comp;
    
	if (bytes != NULL)	free(bytes);
    bytes = data; // take over pointer
    //bytes = (unsigned char *) malloc(sizeof(unsigned char) * size[0] * size[1] * numComponents);

    setDefault(FALSE);
    OnChanged();

    return(bytes != NULL);

}

//  copy src data with optional stride, can be used to copy sub rectangles
int GvSFImage::set(int width, int height, int comp,int stride, const unsigned char *data)
{
	if (bytes != NULL)
		free(bytes);

    size[0]=width;
    size[1]=height;
    numComponents = comp;
 
    
    bytes = (unsigned char *) malloc(sizeof(unsigned char) * size[0] * size[1] * numComponents);

    if (!bytes) return(FALSE);

	const unsigned char *src=data;
	unsigned char *dest=bytes;

	int strideDest = width*numComponents;

	if (stride==0) {
		stride = strideDest;
	}

	if (strideDest == stride) {
		memcpy(dest,src,strideDest*height);
	} else 
		for (int i = 0; i < height; i++,dest+=strideDest,src+=stride) {
			memcpy(dest,src,strideDest);
		}

    setDefault(FALSE);
    OnChanged();

    return(bytes != NULL);

}


int GvSFImage::set(int width, int height, int comp,const unsigned long *data)
{
    if (bytes != NULL)
		free(bytes);

    size[0]=width;
    size[1]=height;
    numComponents = comp;
 
    
    bytes = (unsigned char *) malloc(sizeof(unsigned char) * size[0] * size[1] * numComponents);

    if (!bytes) return(FALSE);

    int byte = 0;
	if (data) 
    for (int i = 0; i < size[0] * size[1]; i++) {
		unsigned long l=*data++;
		for (int j = 0; j < numComponents; j++)
	    	bytes[byte++] =
			(unsigned char) ((l >> (8*(numComponents-j-1))) & 0xFF);
    }

    setDefault(FALSE);
    OnChanged();

    return(bytes != NULL);
}

int GvSFImage::set(int width, int height, int comp,const float *data)
{
    if (bytes != NULL)
		free(bytes);

    size[0]=width;
    size[1]=height;
    numComponents = comp;
	const float scale = 255.0f;
 
    
    bytes = (unsigned char *) malloc(sizeof(unsigned char) * size[0] * size[1] * numComponents);

    if (!bytes) return(FALSE);

    int byte = 0;
	if (data) 
    for (int i = 0; i < size[0] * size[1]; i++) {
		for (int j = 0; j < numComponents; j++)
	    	bytes[byte++] =  (unsigned char) (int) (*data++ * scale);
    }

    setDefault(FALSE);
    OnChanged();

    return(bytes != NULL);
}

int GvSFImage::getData(int cnt,unsigned long *data)
{
  const unsigned char *src=bytes;
  
  if (!bytes || !data) return 0;

  while (cnt >0) {
	  *data++ = unpack(src);
	  src+=numComponents;
	  cnt--;
  }	
  return(1);

}



// set image to a scaled version
// used fast integer scaling without interpolation
// rowbytes can be 0, and will be automatically set
// otherwise by modified data + width + rowbytes a subimage can be specified
int GvSFImage::setScaled(int newWidth,int newHeight, 
                        // Src Image
                        int width, int height, int comp,
                        const unsigned char *data,
                        int rowBytes)
{
    

    int flipx=0;
    if (newWidth<0) { newWidth = -newWidth; flipx=1; }

    int flipy=0;
    if (newHeight<0) { newHeight = -newHeight; flipy=1; }

    // check input 
    if (newWidth<=0) return(0);
    if (newHeight<=0) return(0);
    if (comp<=0) return(0);
    if (!data) return(0);


    size[0]=newWidth;
    size[1]=newHeight;
    numComponents = comp;

    unsigned char *toFree = NULL;
    
    if (rowBytes == 0) rowBytes = width * comp;

    if (data == bytes) toFree = bytes;
    else 
      if (bytes != NULL)	free(bytes);
    
    bytes = (unsigned char *) malloc(1+sizeof(unsigned char) * size[0] * size[1] * numComponents);
    
    if (!bytes) {
        if (toFree != NULL)	free(toFree);

        return(0);
    }
    
    // debug 
    //bytes[sizeof(unsigned char) * size[0] * size[1] * numComponents]=1+4+16;

    
    unsigned char *o=bytes; // output 
    int oRowBytes = newWidth * comp;    // ouput scanline offset 

    int prevsy=-100;
    int sy;

    for (int y=0;
	        y < newHeight;

	        y++,o+=oRowBytes, prevsy=sy

	    )
    {

    // mapy output y to nearest input y
    sy=  ((long) (y* height)) / newHeight;
    if (flipy) sy = height -1 - sy;

    if (prevsy == sy) {
	    /* replicate output row */
	    memcpy(o,o-oRowBytes,oRowBytes);
	    continue;
    }

    /* input row for sy */
    const unsigned char *i = data + sy * rowBytes;
    unsigned char *orow;    // pixel stepper 
    int x;

    for (x=0,orow=o; x < newWidth; x++) {
       // find nearest input pixel for output x
       int sx=  ((long) (x* width)) / newWidth;
       if (flipx) sx = width -1 -sx;

       // copy a pixel 
       memcpy(orow,i+sx*comp,comp);
       orow+=comp;
    }
  }
  
  //ASSERT(bytes[sizeof(unsigned char) * size[0] * size[1] * numComponents]== 1+4+16);

  if (toFree != NULL)	free(toFree);
  setDefault(FALSE);
  OnChanged();

  return(bytes != NULL);
}
/*
int setSubImage(int xoffset, int yoffset,
		const GvSFImage &src,
		int width, int height,	// optional
		int srcXoffset,int srcYoffset // optional 
		)
{

	if (width <= 0) width = src.size[0];
	else width = min (width,src.size[0]);
	if (height <= 0) height = src.size[1];
	else height = min (height,src.size[1]);

}
*/



int GvSFImage::ioValue(AStream &a) 
{
    if (a.io(size[0]) && a.io(size[1]) && a.io(numComponents)) {
		if (a.IsInput()) {
   			// to DO:

		} else {
            if (bytes) {
            int byte = 0;
    		for (int i = 0; i <  size[1]; i++) {
    		
    		for (int k = 0; k < size[0]; k++) {
				unsigned long l=0;
				for (int j = 0; j < numComponents; j++)
	    			l |= (( (unsigned long) bytes[byte++] << (8*(numComponents-1-j))) );

				char buf[60];
				sprintf(buf,"%#lx",l);
				a.w(buf);
    		}
			a.nl();
			}
            } // bytes
		}
	}
   return (1);
}


int GvSFImage::get(GvString &s)
{
  int cnt=0;
  char * p= s.getBuffer(MAX_INT_STR*4+10);

  int ret=sprintf(p,"%ld %ld %ld %n",size[0],size[1],numComponents,&cnt);
  s.releaseBuffer();

      if (bytes) {
            int byte = 0;
    		for (int i = 0; i <  size[1]; i++) {
			
			GvString buf1;	
			
			buf1=" ";
    		
    		for (int k = 0; k < size[0]; k++) {
				unsigned long l=0;
				for (int j = 0; j < numComponents; j++)
	    			l |= (unsigned long) bytes[byte++] << (8*(numComponents-1-j));

				char buf[60];
				sprintf(buf," %#lx",l);
				buf1+=buf;

    		}
			s+=buf1;
			}
      } // bytes


  return(ret > 0);
}


int GvSFImage::set(const char* s)
{

  if (bytes != NULL)	
	  free(bytes);

  bytes = NULL;

	TRACE("Parsing an SFImage from string \n");
	// create a parser 
	GvInput in;

    in.setString(s);
	in.version = 2.0;
	in.vrml2 = 1;
	
	// read 
	int ret = this->readValue(&in);
	setDefault(FALSE);
	OnChanged();

	return ret;

}


#ifdef _COM 

/* EventIn methods */
STDMETHODIMP GvSFImage::setValue( 
        /* [in] */ int width,
        /* [in] */ int height,
        /* [in] */ int numComponents,
		/* [in] */ int cnt,
        /* [size_is][in] */ unsigned small *pixels)
{
	int neededCnt = width * height * numComponents;
    unsigned char *newBytes = (unsigned char *) calloc(neededCnt,sizeof(unsigned char));

	if (!newBytes) return S_FALSE;
	if (cnt>neededCnt) cnt = neededCnt;

	if (pixels)
		memcpy(newBytes,pixels,cnt*1*sizeof( unsigned char)); 

	set(width,height,numComponents,newBytes); // because its by ref !!!

	return S_OK;  
}



/* EventOut methods */ 
STDMETHODIMP GvSFImage::getWidth( /* [retval][out] */ int *value)
{
	*value = size[0];
	return S_OK;
}
    
STDMETHODIMP GvSFImage::getHeight(/* [retval][out] */ int *value)
{
	*value = size[1];
	return S_OK;
}
    
STDMETHODIMP GvSFImage::getNumComponents(/* [retval][out] */ int  *value)
{
	*value = numComponents;
	return S_OK;
}
    
STDMETHODIMP GvSFImage::getPixels( 
        /* [in] */ int cnt,
        /* [size_is][out][in] */ unsigned small *value)
{

	if (value == NULL || bytes == NULL) return S_OK;

	memcpy(value,bytes,cnt*1*sizeof( unsigned small)); 
	return S_OK;  

}

#endif






//GvSFLong.cpp
#include <gvsflong.h>

GV_SFIELD_SOURCE(GvSFLong);

GvBool
GvSFLong::readValue(GvInput *in)
{
    return in->read(value);
}

int GvSFLong::ioValue(AStream &a) 
{
  return a.io(value);
}

int GvSFLong::get(GvString &s)
{
  int ret=sprintf(s.getBuffer(MAX_INT_STR),"%ld",value);
  s.releaseBuffer();
  return(ret > 0 );
}


int GvSFLong::set(const char* s)
{
  int ret=sscanf(s,"%ld",&value);
  setDefault(FALSE);
  OnChanged();
  return(ret == 1);
}

//GvSFInt32.h

#include <gvsfint32.h>
RTIMP(GvSFInt32,0,GvSFLong);



//GvSFMatrix.h
#include <gvsfmatrix.h>

GV_SFIELD_SOURCE(GvSFMatrix);

GvBool
GvSFMatrix::readValue(GvInput *in)
{
	value.contents = Matrix::HAS_ANYTHING; // we don't know, to do: analyze ?

    return  (in->read(value[0][0]) && in->read(value[0][1]) &&
	     in->read(value[0][2]) && in->read(value[0][3]) &&
	     in->read(value[1][0]) && in->read(value[1][1]) &&
	     in->read(value[1][2]) && in->read(value[1][3]) &&
	     in->read(value[2][0]) && in->read(value[2][1]) &&
	     in->read(value[2][2]) && in->read(value[2][3]) &&
	     in->read(value[3][0]) && in->read(value[3][1]) &&
	     in->read(value[3][2]) && in->read(value[3][3]));
}

int GvSFMatrix::ioValue(AStream &a) 
{
return  (a.io(value[0][0]) && a.io(value[0][1]) &&
	     a.io(value[0][2]) && a.io(value[0][3]) &&
	     a.io(value[1][0]) && a.io(value[1][1]) &&
	     a.io(value[1][2]) && a.io(value[1][3]) &&
	     a.io(value[2][0]) && a.io(value[2][1]) &&
	     a.io(value[2][2]) && a.io(value[2][3]) &&
	     a.io(value[3][0]) && a.io(value[3][1]) &&
	     a.io(value[3][2]) && a.io(value[3][3]));
}

int GvSFMatrix::get(GvString &s)
{
  int ret=sprintf(s.getBuffer(16*MAX_FLOAT_STR),"%g %g %g %g\t%g %g %g %g\t%g %g %g %g\t%g %g %g %g\t",
		  value[0][0],value[0][1]
	     ,value[0][2],value[0][3]
	     ,value[1][0],value[1][1]
	     ,value[1][2],value[1][3]
	     ,value[2][0],value[2][1]
	     ,value[2][2],value[2][3]
	     ,value[3][0],value[3][1]
	     ,value[3][2],value[3][3]
  );
  s.releaseBuffer();
  return(ret > 0);
}


int GvSFMatrix::set(const char* s)
{
	value.contents = Matrix::HAS_ANYTHING; // we don't know, to do: analyze ?
	int ret=sscanf(s,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
  		  &value[0][0],&value[0][1]
	     ,&value[0][2],&value[0][3]
	     ,&value[1][0],&value[1][1]
	     ,&value[1][2],&value[1][3]
	     ,&value[2][0],&value[2][1]
	     ,&value[2][2],&value[2][3]
	     ,&value[3][0],&value[3][1]
	     ,&value[3][2],&value[3][3]
  );
  setDefault(FALSE);
  OnChanged();
  return(ret == 16);
}


//GvSFNode.cpp
// See also GvMFNode
#include <gvsfnode.h>
#include <gvnode.h>

// HG
//GV_SFIELD_SOURCE(GvSFNode);

RTIMP(GvSFNode,0,GvSField)

GV_SFIELD_SOURCE_EQ(GvSFNode);

GvSFNode::~GvSFNode()							      
{									      
    if (value && container) { // 5.07.97 
		value->removeOwner(container);
	}
}                                                           


GvBool
GvSFNode::readValue(GvInput *in)
{	GvNode *newNode = NULL;

    GvBool ret = GvNode::read(in, newNode);

    set(newNode);
	return(ret);
}

void GvSFNode::set(GvNode * newValue)
{ 
    if (value && container) {
		value->removeOwner(container);
	}
	value = newValue; // NodeHandle handles referencing
    setDefault(FALSE); 
    if (value && container) {
		value->addOwner(container);
		if (newValue->m_browser == NULL) // tmp workaround for browser 
			newValue->m_browser = container->m_browser;

	}
	OnChanged();
}

// not implemented  fully 

int GvSFNode::get(GvString &s)
{
  if (!value)  {
     s = "NULL";
     return(1);
  } 
  else {
	  s = ((GvNode*)value)->GetType();
	  s += "{ }";
	  return(0);
  }
}


int GvSFNode::set(const char* s)
{

  if (strcmp(s,"NULL") == 0) {
  	set((GvNode*) NULL);
  	return(1);
  } 
  else  { 
	//  TRACE("Parsing an SFNode from :%s\n",s); // could be very long string 
	// create a parser 
	GvInput in;

	GvScene *scene = new GvScene();
	scene->ref();

		if (getContainer()) 
			scene->setBrowser(getContainer()->getBrowser());

		in.setString(s);
		in.version = 2.0;
		in.vrml2 = 1;
		in.setScene(scene);
		// read 
		int ret = this->readValue(&in);
	
	scene->unref();

	setDefault(FALSE);
	OnChanged();

	return ret;

  }
}


// computes copyOnInstance flag from child nodes flag set
GvBool 
GvSFNode::computeCopyOnInstance(GvCopyContext &context)
{
    GvNode *n = (GvNode*) value; 
    if (!n) return FALSE;
	if (n == container) { // simple self reference avoidance
		ASSERT(0);
		return n->getCopyOnInstance();
	 }	

    return n->computeCopyOnInstance(context); 
}


#ifdef _COM
/* EventIn methods */
STDMETHODIMP GvSFNode::setValue(Node * newValue)
{
	if (newValue == NULL)
		set((GvNode*) NULL);
	else {
		// get the pointer to native GLView node 
		GvNode *n = NULL;
		newValue->getNative((long *) &n);
		set(n);
	}
	COM_EXTERN_TOUCH();
	return S_OK;
}

/* EventOut methods */ 
STDMETHODIMP GvSFNode::getValue(Node* *v)
{
	GvNode *value = get();
	if (value == NULL) *v = NULL;
	else return value->QueryInterface(IID_Node, (void **)v);
	return S_OK;
}

#endif



/*
int GvSFLong::ioValue(AStream &a) 
{
  return a.io(value);
}

*/

//GvSFRotation.cpp

#include <gvsfrotation.h>

GV_SFIELD_SOURCE(GvSFRotation);


GvBool GvSFRotation::readValue(GvInput *in)  
{ return value.readValue(in); 
} 

#if 0
GvBool
GvSFRotation::readValue(GvInput *in)
{
    return (in->read(axis[0]) &&
	    in->read(axis[1]) &&
	    in->read(axis[2]) &&
	    in->read(angle));
}

int GvSFRotation::ioValue(AStream &a) 
{
    return (a.io(axis[0]) &&
	    a.io(axis[1]) &&
	    a.io(axis[2]) &&
	    a.io(angle));

}

int GvSFRotation::get(GvString &s)
{
  int ret=sprintf(s.getBuffer(4*MAX_FLOAT_STR+4),"%g %g %g %g",axis[0],axis[1],axis[2],angle);
  s.releaseBuffer();
  return(ret > 0);
}


int GvSFRotation::set(const char* s)
{
  int ret=sscanf(s,"%f %f %f %f",&axis[0],&axis[1],&axis[2],&angle);
  setDefault(FALSE);
  OnChanged();
  return(ret == 4);
}

#endif

// Rotation a single rotation value

// rotate given axis and angle 
void Rotation::get(Quaternation &q) const {
	    q.SetAxisRotation(axis[0],axis[1],axis[2], angle);
}

void Rotation::set(const Quaternation &q)
{
	    q.GetAxisRotation(axis[0],axis[1],axis[2],angle);
}


GvBool
Rotation::readValue(GvInput *in)
{
    return (in->read(axis[0]) &&
	    in->read(axis[1]) &&
	    in->read(axis[2]) &&
	    in->read(angle));
}

int Rotation::ioValue(AStream &a) 
{
    return (a.io(axis[0]) &&
	    a.io(axis[1]) &&
	    a.io(axis[2]) &&
	    a.io(angle));

}

int Rotation::get(GvString &s)
{
  int ret=sprintf(s.getBuffer(4*MAX_FLOAT_STR+3),"%g %g %g %g",axis[0],axis[1],axis[2],angle);
  s.releaseBuffer();
  return(ret > 0);
}


int Rotation::set(const char* s)
{
  int ret=sscanf(s,"%f %f %f %f",&axis[0],&axis[1],&axis[2],&angle);
  return(ret == 4);
}

/*
  unmatrix : Spencer W. Thomas, University of Michigan
  Graphic Gems II, Page 600 + 320
*/
#define XY x
#define XZ y
#define YZ z

int DecomposeMatrix(const Matrix &m, 
					Point &scale,Point &shear, 
					/*Point &rotate,*/ Rotation &rotation, 
					Point &translate)
{ register int i;
  
  Matrix locmat(m);
  Point row[3];
  Point cross12;

  /* assume
     det of upper 3*3 matrix non zero
     no perspective part
  */
  /* normalize */
  if (locmat.x[3][3] != 1.0f)
 	for ( i=0; i<4;i++ )
 		for ( int j=0; j<4; j++ )
 			locmat.x[i][j] /= locmat.x[3][3];


  /* get translation */ 

  translate.x = locmat.x[3][0]; locmat.x[3][0]= 0;
  translate.y = locmat.x[3][1]; locmat.x[3][1]= 0;
  translate.z = locmat.x[3][2]; locmat.x[3][2]= 0;

  /* now get scale and shear */
  for(i=0; i< 3; i++) { row[i].Set(locmat.x[i][0],locmat.x[i][1],locmat.x[i][2]); }

  /* Compute x scale factor and normalize first row */
  scale.x = row[0].Len();
  row[0].Normalize();

  shear.XY = DotProd(row[0],row[1]);
  
  row[1]+=row[0] * -shear.XY;

  /* Compute y scale factor and normalize second row */
  scale.y = row[1].Len();
  row[1].Normalize();

  if (scale.y != 0.0) shear.XY /= scale.y;

  /* compute xz and yz shears, orthogonalize 3rd row */
  shear.XZ = DotProd(row[0],row[2]);
  row[2]+=row[0]* -shear.XZ;

  shear.YZ = DotProd(row[1],row[2]);
  row[2]+=row[1]*-shear.YZ;

  scale.z = row[2].Len();
  row[2].Normalize();

  if (scale.z != 0.0) shear.XZ /= scale.z;
  if (scale.z != 0.0) shear.YZ /= scale.z;

  /* at this point the matrix in rows is orthonormal.
     Check for a coordinate system flip. if the determinat is -1
     then negate the matrix and the scaling factors.
  */
  cross12 = CrossProd(row[1],row[2]);

  if (DotProd(row[0],cross12) < 0)
     for(i=0; i <3;i++) {
        row[i]*= -1.0;
        scale[i] *= -1.0;
     }

  /* now get rotation  */
  for(i=0; i< 3; i++) 
  { locmat[i][0]=row[i].x,
	locmat[i][1]=row[i].y;
	locmat[i][2]=row[i].z;
  }

  Quaternation quat; 
  QuaternationFromMatrix(quat, locmat);
  rotation.set(quat);	

  //quat.GetAxisRotation(axis,angle);

#if 0
  /* rotations */
  rotate.y = asin(-row[0].z);

  if (cos(rotate.y) != 0) {
        rotate.x = atan2(row[1].z,row[2].z);
        rotate.z = atan2(row[0].y,row[0].x);
  } else {
        rotate.x = atan2(row[1].x,row[1].y);
        rotate.z = 0;
  }
//  rotate.x = RAD2DEG(rotate.x);
//  rotate.y = RAD2DEG(rotate.y);
//  rotate.z = RAD2DEG(rotate.z);

#if 0
  if (1) {
      printf("\nScale "); VECPRINT(*scale);
      printf("\nShear "); VECPRINT(*shear);
      printf("\nRotate "); VECPRINT(*rotate);
      printf("\nTrans  "); VECPRINT(*translate);
  }
#endif
#endif


  return(1);
}




//GVSFString.cpp

#include <gvsfstring.h>



GV_SFIELD_SOURCE(GvSFString);

GvBool
GvSFString::readValue(GvInput *in)
{
    return in->read(value);
}

int GvSFString::ioValue(AStream &a) 
{
   if (a.IsInput()) {
   		TRACE("To Do %s::ioValue \n",ClassName());
		return(0);

   } else 
   {
   		return(a.wstr(value.getString()));
   }
   return (0);
}

int GvSFString::get(GvString &s)
{
  s = value; // to do:quote ?
  return(1);
}


int GvSFString::set(const char* s)
{

  value = s;  // to do: unquote ?
  setDefault(FALSE);
  OnChanged();
  return(1);
}

void GvSFString::set_(const char* s)
{
  value = s; 
  setDefault(FALSE); 
  OnChanged();
}


#ifdef _COM
/* EventIn methods */
STDMETHODIMP GvSFString::setValue(BSTR newValue)
{
	const char *vs;
	USES_CONVERSION;
	vs = OLE2CT(newValue);
	set_(vs);
	COM_EXTERN_TOUCH();
	return S_OK;
}

/* EventOut methods */ 
STDMETHODIMP GvSFString::getValue(BSTR *v)
{
	USES_CONVERSION;
	const char *vs = value.getString();
	*v = SysAllocString(T2COLE(vs));
	return S_OK;
}
#endif





//GvSFVec2f.cpp
#include <gvsfvec2f.h>

GV_SFIELD_SOURCE(GvSFVec2f);

GvBool
GvSFVec2f::readValue(GvInput *in)
{
    return (in->read(value[0]) &&
	    in->read(value[1]));
}

int GvSFVec2f::ioValue(AStream &a) 
{
   return (a.io(value[0]) &&
	    a.io(value[1]));
}

int GvSFVec2f::get(GvString &s)
{
  int ret=sprintf(s.getBuffer(2*MAX_FLOAT_STR+1),"%g %g",value[0],value[1]);
  s.releaseBuffer();
  return(ret > 0);
}


int GvSFVec2f::set(const char* s)
{
  int ret=sscanf(s,"%f %f",&value[0],&value[1]);
  setDefault(FALSE);
  OnChanged();
  return(ret == 2);
}


//GvSFVec3f.cpp
#include <gvsfvec3f.h>

GV_SFIELD_SOURCE(GvSFVec3f);

GvBool
GvSFVec3f::readValue(GvInput *in)
{
    return (in->read(value[0]) &&
	    in->read(value[1]) &&
	    in->read(value[2]));
}

int GvSFVec3f::ioValue(AStream &a) 
{
   return (a.io(value[0]) &&
	    a.io(value[1]) &&
	    a.io(value[2]));
}

int GvSFVec3f::get(GvString &s)
{
  int ret=sprintf(s.getBuffer(3*MAX_FLOAT_STR+3),"%g %g %g",value[0],value[1],value[2]);
  s.releaseBuffer();
  return(ret > 0);
}


int GvSFVec3f::set(const char* s)
{
  int ret=sscanf(s,"%f %f %f",&value[0],&value[1],&value[2]);
  setDefault(FALSE);
  OnChanged();
  return(ret == 3);
}




//--------------------------------------------------------------
//
//							GvSFVec4f
//
//--------------------------------------------------------------


GV_SFIELD_SOURCE(GvSFVec4f);

GvBool
GvSFVec4f::readValue(GvInput *in)
{
    return (in->read(value[0]) &&
	    in->read(value[1]) &&
	    in->read(value[2]));
}

int GvSFVec4f::ioValue(AStream &a) 
{
   return (a.io(value[0]) &&
	    a.io(value[1]) &&
	    a.io(value[2]));
}

int GvSFVec4f::get(GvString &s)
{
  int ret=sprintf(s.getBuffer(3*MAX_FLOAT_STR+3),"%g %g %g",value[0],value[1],value[2]);
  s.releaseBuffer();
  return(ret > 0);
}


int GvSFVec4f::set(const char* s)
{
  int ret=sscanf(s,"%f %f %f",&value[0],&value[1],&value[2]);
  setDefault(FALSE);
  OnChanged();
  return(ret == 3);
}
