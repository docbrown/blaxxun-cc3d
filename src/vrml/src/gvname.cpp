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
#include <gvstring.h>
#include <ctype.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// GvNameEntry
//

#define CHUNK_SIZE	4000

struct GvNameChunk {
    char		mem[CHUNK_SIZE];
    char		*curByte;
    int			bytesLeft;
    struct GvNameChunk	*next;
};

// static's

int		GvNameEntry::nameTableSize;
GvNameEntry **	GvNameEntry::nameTable;
struct GvNameChunk *GvNameEntry::chunk;	    	
CRITICAL_SECTION GvNameEntry::locker;
BOOL GvNameEntry::lockerInit = FALSE;


void
GvNameEntry::initClass()
{
    int i;
  
    nameTableSize = 2999;
    nameTable	  = new GvNameEntry *[nameTableSize];

    for (i = 0; i < nameTableSize; i++)
	    nameTable[i] = NULL;

    chunk = NULL;
}

// hg 
void
GvNameEntry::termClass()
{
    int i;



    if (nameTable) {
    for (i = 0; i < nameTableSize; i++) {

        GvNameEntry		*entry,*prev;

        entry = nameTable[i];

        // delete name entrys
        while (entry != NULL) {
	        prev = entry;
            entry = entry->next;
            //
            if (strlen(prev->string) >= CHUNK_SIZE) {
                //delete prev->string; problem, is const !
                //prev->string = NULL;
                TRACE("GvNameEntry::termClass: to do must free long string");
            }
            delete prev;
        }
        nameTable[i]=NULL;
    }
    // delete table
    delete []nameTable;
    nameTable = NULL;
    nameTableSize = 0;
    }

    // delete chunks
	struct GvNameChunk	*cur,*prev;
    for(cur=chunk; cur; cur = prev) {
        prev=cur->next;
        delete cur;
    }
    chunk = NULL;
}


const GvNameEntry *
GvNameEntry::insert(const char *s)
{
    u_long		h = GvString::hash(s);
    u_long		i;
    GvNameEntry		*entry;
    GvNameEntry		*head;

	// Initialize the critical section.
	if (!lockerInit) { 
		InitializeCriticalSection(&locker); 
		lockerInit = TRUE;
	}
	//TRACE("Enter section\n");

    EnterCriticalSection(&locker); 

    if (nameTableSize == 0)
	    initClass();

    i = h % nameTableSize;
    entry = head = nameTable[i];

    while (entry != NULL) {
	if (entry->hashValue == h && entry->isEqual(s))
	    break;
	entry = entry->next;
    }

    if (entry == NULL) {

	int len = strlen(s) + 1;

	if (len >= CHUNK_SIZE)
	    s = strdup(s);

	else 
	{

	    if (chunk == NULL || chunk->bytesLeft < len) {
		struct GvNameChunk	*newChunk = new GvNameChunk;

		newChunk->curByte   = newChunk->mem;
		newChunk->bytesLeft = CHUNK_SIZE;
		newChunk->next      = chunk;

		chunk = newChunk;
	    }

	    strcpy(chunk->curByte, s);
	    s = chunk->curByte;

	    chunk->curByte   += len;
	    chunk->bytesLeft -= len;
	}

	entry = new GvNameEntry(s, h, head);
	nameTable[i] = entry;
    }

	LeaveCriticalSection(&locker);

    return entry;
}

/* vrml2
The carriage return (0x0d), newline (0x0a), space (0x20), tab (0x09), and comma (0x2c) characters are whitespace characters
wherever they appear outside of quoted SFString or MFString fields. Any number of whitespace characters and comments may be
used to separate the syntactic entities of a VRML file. 

: 
     IdFirstChar 
     IdFirstChar IdRestChars 

IdFirstChar: 
     Any ISO-10646 character encoded using UTF-8 except: 0x30-0x39, 0x0-0x20, 0x22, 0x23, 0x27, 0x2c, 0x2e, 0x5b, 0x5c,
     0x5d, 0x7b, 0x7d. 
IdRestChars: 
     Any number of ISO-10646 characters except: 0x0-0x20, 0x22, 0x23, 0x27, 0x2c, 0x2e, 0x5b, 0x5c, 0x5d, 0x7b, 0x7d. 


	 Legal characters to start an identifier 

idStartChar ([^\x30-\x39\x00-\x20\x22\x23\x27\x2c\x2e\x5b-\x5d\x7b\x7d])

	Legal other characters in an identifier *

idRestChar	([^\x00-\x20\x22\x23\x27\x2c\x2e\x5b-\x5d\x7b\x7d])


*/

GvName::GvName()
{
    entry = GvNameEntry::insert("");
}


static const char
//badCharacters[] = "+\'\"\\{}";
badCharacters[] = "\'\"\\{},[]|()=";

GvBool
GvName::isIdentStartChar(char c,GvBool vrml2)
{
    if (isdigit(c)) return FALSE;
    return isIdentChar(c,vrml2);
}

GvBool
GvName::isIdentChar(char c,GvBool vrml2)
{
	 // u is -4 !!!!!!!!!!, char is signed 
	 if (((unsigned char)c) <= ((unsigned char)'\x20')) 
		 return FALSE;
	 if (vrml2) 
	 switch (c) {

	 case '\x22' :
	 case '\x23' :
	 case '\x27' :
	 case '\x2c' :
	 case '\x2e' :
	 case '\x5b' :
	 case '\x5c' :
	 case '\x5d' :
	 case '\x7b' :
	 case '\x7d' :
		 return FALSE;
	 // hg 
	 case '+' :
	 case '-' :
	 case '_' :
	 case '%' :

	 default :
		 return(TRUE);
	 }

	 // VRML 1.0
     if (isalnum(c) || c == '_') return TRUE;

     if ((strchr(badCharacters, c) != NULL) || isspace(c)) 
		return FALSE;

     return TRUE;


}

GvBool
GvName::isNodeNameStartChar(char c,GvBool vrml2)
{
	if (vrml2) return(isIdentStartChar(c,vrml2));

	// VRML 1.0
    // ////// if (isdigit(c)) return FALSE;

    return isIdentChar(c,vrml2);
}


GvBool
GvName::isNodeNameChar(char c,GvBool vrml2)
{
	if (vrml2) return(isIdentChar(c,vrml2));

	// VRML 1.0

    if (isalnum(c)) return TRUE;


// (0x00 .. 0x1F or 0x7F) iscntl(c) removed 7.10.96 
    if ((strchr(badCharacters, c) != NULL) || isspace(c)) 
		return FALSE;

    return TRUE;
}
