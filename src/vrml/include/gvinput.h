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
#ifndef  _GV_INPUT_
#define  _GV_INPUT_
/******************************************************************************
@doc

@module GvInput.cpp - VRML token Parser|

Copyright (c) 1995 - 1999 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvInput>


Notes:
Originally based on QvLib


Changes:

$Revision: 1.7 $
$Log: gvinput.h,v $
Revision 1.7  1999/07/06 16:34:49  tom
*** empty log message ***

Revision 1.6  1999/05/10 13:53:50  tom
*** empty log message ***



Todo :

******************************************************************************/

#include <gvdict.h>
#include <gvstring.h>

class GvNode;
class GvDB;
class GvProto;
class GvScene;


class GvNameSpace;
class GReporter;

class GZReadStream;

//
// GvInput 
//


class GvInput {
 public:

    GvInput();
    ~GvInput();

    static float isASCIIHeader(const char *string);
    
    // set input from FILE *
    // void		setFilePointer(FILE *newFP);
    //FILE *		getCurFile() const { return fp; }

	//! read directly from a GZip decompressor
    void		setFilePointer(GZReadStream *newFP);
    
    //! set input from string
    void setString(const char *string,GvBool needHeader = FALSE);

    float		getVersion();

	//! get a character 
    GvBool		get(char &c);
	
	// read basic entities
    GvBool		read(char	    &c);
    GvBool		read(GvString   &s);
    GvBool		read(GvName	    &n, GvBool validIdent = FALSE);

	//! read a nodename for DEF USE ROUTE etc
	GvBool		readNodeName(GvName &n);

    GvBool		read(int	    &i);
    GvBool		read(unsigned int   &i);
    GvBool		read(short	    &s);
    GvBool		read(unsigned short &s);
    GvBool		read(long	    &l);
    GvBool		read(unsigned long  &l);
    GvBool		read(float	    &f);
    GvBool		read(double	    &d);
    GvBool		eof() const;
    void		getLocationString(GvString &string) const;

	//! push back lookahead token
    void		putBack(char c);
    void		putBack(const char *string);


	const char *src;		// file name / title / url for error messages ...
	void setSrc(const char * newSrc) { src = newSrc; }

    GvBool vrml2;      // 1 VRML Moving worlds 0 vrml 1.0
    
    GvBool isVrml2() const { return(vrml2); }

//  private:
    
	GZReadStream *fp;		// File pointer

	GvBool		reportErrors;	//!< report errors
    int			lineNum;		//!<  number of line currently reading
    int			lastErrorLineNum;//!< number of last line with error
	int			errorCode;		// !< parse error code 

	//! error reporter object 
	GReporter*	reporter;
	GReporter*	getReporter() { return reporter; };
	
	//! set error reporter object 
	void setReporter(GReporter* newReporter) { reporter = newReporter; };


    float		version;	//!< Version number of file
    GvBool		readHeader;	//!< TRUE if header was checked for A/B
    GvBool		headerOk;	//!< TRUE if header was read ok

    
    GvString	backBuf;	//!< buffer for unget 
    
	int			backBufIndex;		

	//! check the file header
    GvBool		checkHeader();

	//! is c whiteSpace ? note vrml2 includes , as white space !
	GvBool      isWhiteSpace(char c);

    GvBool		skipWhiteSpace();

	// read basic data types 
    GvBool		readInteger(long &l);
    GvBool		readUnsignedInteger(unsigned long &l);
    GvBool		readReal(double &d);

	//! read interger digits into string up to maxCnt (including hex support)
    GvBool		readUnsignedIntegerString(char *str,int maxCnt);
	
	//! read digits into string up to maxCnt
    int			readDigits(char *string,int maxCnt);

	//! read hex digits into string up to maxCnt
    int			readHexDigits(char *string,int maxCnt);

	//! read a single char if it matches charToRead
    int			readChar(char *string, char charToRead);

	friend class GvNode;
	friend class GvProto;
	friend class GvDB;



    //!  name space for keeping node and proto dictionarys
    GvNameSpace *nameSpace;

    //! scene pointer to store top-level PROTO's 
	GvScene *scene;

    GvScene *getScene() {return scene; }
    void setScene(GvScene *scene) { this->scene = scene; }

    //! set top level name space (pointer not owned // need pop)
    void pushNameSpace(GvNameSpace *newNameSpace);
    
    //! return prev pushedNameSpace and pop
    GvNameSpace* popNameSpace();

    //! get current namespace
    GvNameSpace * getNameSpace() const { return(nameSpace);};   
   
    //! set current namespace
    void setNameSpace(GvNameSpace *newNameSpace)  { nameSpace = newNameSpace;};


    //! add node to current name space, and set node name
    GvBool		addReference(const GvName &name, GvNode *node);

    //! remove node from current name space
    GvBool		removeReference(const GvName &name, GvNode *node);
    
    //! lookup node in current namspace, null if not found
    GvNode *	findReference(const GvName &name) const;

    //! add proto to current name space
    GvBool		addProto(const GvName &name, GvProto *proto);
    
    //! lookup proto in all current namspaces, null if none found
    GvProto *	findProto(const GvName &name) const;

    GvProto *currentProto; //!< the current proto beeing parsed
						  


};



#endif /* _GV_INPUT_ */
