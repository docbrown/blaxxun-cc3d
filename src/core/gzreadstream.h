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

#ifndef GZReadStream_h
#define GZReadStream_h



class GZReadStream
{

	BOOL eof;		//!< is the stream at eof 

	BOOL isZipped;		//!< is this stream really zipped 

	BOOL error;			//!< there is an error with the stream
	int errorNumber;	//!< error code  (errno) zlib errno
	const char *errorMsg; //!< error string
	

	char *buf;		// buffer with uncompressed data
	size_t bufsize;	// allocated size

	char *bufReadPtr;	// readptr, points into buffer
	char *bufEndPtr;

	// gzFile
	void *in;		//!< gzip uncompress structure 

	FILE *inFile;


	//! get a char at end of buffer
	int getcBuf(); 

public :

	GZReadStream();
	~GZReadStream();

	//! opens the file, and prepares for streaming unzip 
	int Open(const char *infile);
	
	//! opens the file, and prepares for streaming unzip 
	int Open(FILE *infile,BOOL isZipped);
	

	//! closes the stream 
	int Close();

	//! at EOF ? 
	inline BOOL Eof() { return eof; }
	
	//! Error ? 
	BOOL Error() { return error; }
	
	//! get Error information 
	BOOL GetError(int &num,const char *&msg) 
	{ 
		num = errorNumber; 	
		msg = errorMsg;
		return error; 
	}


	//! get next char or EOF in case of EOF/error 
	inline int getc() 
	{
		if (bufReadPtr < bufEndPtr) {
			return (*bufReadPtr++);
		}
		else
			return getcBuf();

	}

	//! put back c if possible,  return  EOF in case of error 
	int ungetc(char c) {
		if (bufReadPtr > buf) {
			bufReadPtr--;
			*bufReadPtr = c;
			return c;
		}
		else return EOF;

	}



};


#endif // GZReadStream_h

