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
// Read vrml file using Gv library

#include "stdafx.h"
/*
	  15.04.97 added 	  CWaitCursor waitCursor; // display wait cursor ==> Problems in Netscape !
	  20.08.97 streaming unzipped , GReporter Argument

*/




#include <stdio.h>

#include <gvdb.h>
#include <gvinput.h>

#include <gvscene.h>

//#include <gvnode.h>
#include <gvnodes.h>

#include <gvnodeshell.h>

#include "gshell.h"
#include "gshelli.h"

#include "grender.h"

#include "gshellop.h"
#include "gshellio.h"

#include "gclass.h"
#include "gmodel.h"

#include "greporter.h"

#include "glutils.h"


#include "www.h"

#include <greadgv.h>

#include "gversion.h"

#include "gzreadstream.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


FILE * fopenZ(const char * fileName, const char *mode);


// Read vrml file  into scene
int ReadVrmlFile(const char *fileName,GReporter *reporter,GvScene *w,int flags)
{
//	CWaitCursor waitCursor; // display wait cursor

    FILE *inFile = NULL;
	int ret = 0;


	GvString src;

    GvInput	in;

	GZReadStream s;

	in.setReporter(reporter);
	in.setScene(w);

	if (flags & 1)
		in.reportErrors = FALSE;


	// open the stream (file or zipped file)
	if ((ret=s.Open(fileName)) <0) {
		return ret;
	}		

	in.setFilePointer(&s);

	src =  w->GetUrl();

	// set url / file name  for error reporting 
	if (src.getLength() >0) {

		src += " (";
		src += fileName;
		src += ")";

		in.setSrc(src);

	}
	else in.setSrc(fileName);


	// check header 
    if (!in.readHeader && ! in.checkHeader()) {
		ret = -3; 
	}
    else 
    if (w->read(&in)) {
		ret = 0;
    } else {
		ret = -2;
		// may be part ok
    }

	if (s.Error()) {	// check for stream errors 
		const char *msg=NULL;
		int err = 0;
		s.GetError(err,msg);
		ret = -3;
		if (reporter && !(flags & 1)) 
			reporter->Error("VRML File read error : %s (%d) in %s " ,msg,err, src.getString());
	}

	s.Close();

	// if (inFile) fclose(inFile);

    return ret;
}


#if 0
// Read vrml file into an MFNode field
int ReadVrmlFile(const char *fileName,GReporter *reporter,GvMFNode *w,int flags)
{
//	CWaitCursor waitCursor; // display wait cursor

    FILE *inFile = NULL;

	int ret = 0;


    GvInput	in;

	GZReadStream s;

	in.setReporter(reporter);
	if (flags & 1)
		in.reportErrors = FALSE;

	if ((ret=s.Open(fileName)) <0) {
		return ret;
	}		

	in.setFilePointer(&s);


	//inFile = fopenZ(fileName, "r");
	////inFile = fopen(fileName,"r");
	//if (!inFile) return(-1);
	// in.setFilePointer(inFile);
	in.setSrc(fileName);
    
    if (w->readValueList(&in,EOF)) {
		ret = 0;
		w->setDefault(FALSE);

    } else {
		ret = -2;
		// may be part ok
    }

	s.Close();

	// if (inFile) fclose(inFile);

    return ret;
}
#endif

#if 0
int ReadVrmlFile(const char *fileName,GReporter *reporter,GWorld *w)
{
   	CWaitCursor waitCursor; // display wait cursor
	
	FILE * inFile = NULL;

	int ret = 0;

    GvDB::init();

    GvInput	in;

    GvNode	*root=NULL;
	
	in.setReporter(reporter);



	inFile = fopenZ(fileName, "r");

	//inFile = fopen(fileName,"r");
	if (!inFile) return(-1);

	in.setFilePointer(inFile);
	in.setSrc(fileName);
    
    if (GvDB::read(&in, root) && root != NULL)
		ret = 0;
    else {
	    // if (inFile) fclose(inFile);
		ret = -2;
		// may be part ok
    }

	if (root) {
	w->Open("/model");
		  	w->Current()->Insert(root);
	w->Close();
	}

	
	if (inFile) fclose(inFile);

    return ret;
}
#endif


// Read single node vrml file 
int ReadVrmlFile(const char *fileName,GReporter *reporter,GvNode * &root)
{
//	CWaitCursor waitCursor; // display wait cursor

	int ret = 0;
    FILE * inFile = NULL;


    GvInput	in;
	
	GZReadStream s;

	in.setReporter(reporter);

	if ((ret=s.Open(fileName)) <0) {
		return ret;
	}		

	in.setFilePointer(&s);

	//	inFile = fopen(fileName,"r");
	//inFile = fopenZ(fileName, "r");
	//if (!inFile) return(-1);
	//in.setFilePointer(inFile);

	in.setSrc(fileName);

    
    if (GvDB::read(&in, root) && root != NULL)
		ret = 0;
    else {
		ret = -2;
    }
	
//	if (inFile) fclose(inFile);

	s.Close();


    return ret;
}



// read a simple polyhedron object (GShell)

int ReadShellFile(const char *FileName,int fmt,GvNode * & root)
{  
//   CWaitCursor waitCursor; // display wait cursor

   int ret= -100;

   GShell * newShell = NULL;;

   if (0) {} 
#ifdef _GEO
   else if (fmt == GEO_FMT)
       newShell = ReadFromGeoFile(FileName);
#endif

#ifdef _RAW
   else if (fmt == RAW_FMT)
       ret=ReadShellRaw(FileName,newShell);
#endif
   
#ifdef _OBJ
   else if (fmt == OBJ_FMT) {
	   GShellI *theShellI=NULL;  	
       ret=ReadShellWavefront(FileName,theShellI);
	   newShell = theShellI;
   }
#endif 

#ifdef _HMF
   else if (fmt == HMF_FMT) {
        newShell = ReadFromHmfFile(FileName);
   }
#endif
#ifdef _3DV
   else if (fmt == D3V_FMT)
       ret=ReadShell3DV(FileName,newShell);
#endif


   if (newShell) {
	 GvNodeShell *node;
	 node  = new GvNodeShell();
	 node->SetShell(newShell);
	 root = node;
	 ret = 0;

   }
   return(ret);
}



// read a file a a GvNode object
int ReadFile(const char *FileName,GReporter *reporter,GvNode * & root)
{
 int ret = -100;

 int isZipped = IsZippedFile(FileName);
 int fmt = GetFileType(FileName); 
 reporter->Status("Reading file '%s' ...",FileName);

 // see GModel :: ReadModel
   switch (fmt) { 
   case DIB_FMT:
   case RGB_FMT:
   case TGA_FMT:
   case JPEG_FMT:
   case GIF_FMT:
   case PPM_FMT:
   //case PNG_FMT:
   //case PCX_FMT:
	  ret = -100;
	  break;
   case GEO_FMT:
   case RAW_FMT:
   case D3V_FMT:
   case OBJ_FMT:
   case HMF_FMT:
	 ret = ReadShellFile(FileName,fmt,root);
	 break;
   case VRML_FMT:
	ret =  ReadVrmlFile(FileName,reporter,root);
	break;
   default : 
	  ret = -200;
	  break;
 }
 if (ret == - 100)
 {
 	reporter->Error("Read File : unknown file extension for file '%s'",FileName);
	ret = -1;
 }
 else reporter->Status("File '%s' Status = %d",FileName,ret);

 return(ret);
}


// read a texture file into the current glState 
int ReadTextureFile(const char *FileName,int fileFmt,
					GglRenderTraversal &state,const char *refType,int &hasAlpha)
{
	int ret = -100;
	BOOL isGrayscale;
	BOOL hasColorKey;
	unsigned char	components;
	short			sizeX,sizeY;
	short			textureFormat;



   int fmt = fileFmt;

   Reporter.Status("Reading texture file '%s' ...",FileName /*file.localFileUnzipped*/);
   switch (fmt) { 
   case DIB_FMT:
   case RGB_FMT:
   case TGA_FMT:
   case JPEG_FMT:
   case GIF_FMT:
   case PPM_FMT:
   case PNG_FMT:
   case PCX_FMT:
  	   ret = InitImageTexture(fmt,FileName /* file.localFileUnzipped */,0, state.glState->textureParameters.NeedMipmaps(),state.glState->textureParameters.max_size_x,state.glState->textureParameters.max_size_y,hasAlpha,hasColorKey,isGrayscale,components,sizeX,sizeY,textureFormat);
	   if (ret == 0) ret = -1;

	  break;
   default : 
	  ret = -100;
	  break;
 }
 if (ret == - 100)
 {
 	Reporter.Error("Read Texture File : unknown file extension for file '%s'",FileName);
	ret = -1;
 }
 else Reporter.Status("Texture File '%s' Status = %d",FileName,ret);

 return(ret);
}

// read an image file into the SFImage field
int ReadImageFile(const char *FileName,int fmt,
				  GTraversal &state,
				  const char *refType,
				  GvSFImage &imageField)
{
 int ret = -100;


	// URL processing alread done ???
	GFile file;

	CTime refreshTime(0);
	const char *refedFromUrl= state.GetUrl(refreshTime);	 


	// get home directorys
	if (state.view) {

		file.SetHome(state.GetHomeUrl());

	}
	file.Set(FileName,1);
	
	file.SetRefedFrom(refedFromUrl,refType);
	
	// url process loading / unzipping  / unknow extension dispatch
	ret = file.Process();
	
	if (ret <0) {
	  return(ret);
	}

	if (file.localFileUnzipped.GetLength() == 0) {
	  return(-1);
	}


   if (fmt<=0) fmt = GetFileType(file.localFileUnzipped); 
 
   Reporter.Status("Reading image '%s' ...",(LPCTSTR) file.localFileUnzipped);
   switch (fmt) { 
   case DIB_FMT:
   case RGB_FMT:
   case TGA_FMT:
   case JPEG_FMT:
   case GIF_FMT:
   case PPM_FMT:
   case PNG_FMT:
   case PCX_FMT:
       
    AUX_RGBImageRec *image;
       
    image = LoadImage(fmt, file.localFileUnzipped,0,0);
    if (image) {
        imageField.set(image->sizeX,image->sizeY,image->components,image->data);
        image->data = NULL;
        free(image);

    }

	  break;
   default : 
	  ret = -100;
	  break;
 }
 if (ret == - 100)
 {
 	Reporter.Error("Read Image File : unknown file extension for file '%s'",FileName);
	ret = -1;
 }
 else Reporter.Status("Image File '%s' Status = %d",FileName,ret);

 return(ret);
}



