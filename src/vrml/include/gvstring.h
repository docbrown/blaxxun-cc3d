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
#ifndef _GV_STRING_
#define _GV_STRING_

#include <gvbasic.h>
#include <string.h>

//
// GvString
//

// string class, storing a zero terminated C String

class GvString {
  public:

	// constructors
    GvString()		{ string = staticStorage;   string[0] = '\0';  }

    GvString( _TCHAR c)	{ string = staticStorage;  string[0]=c; string[1] = '\0'; }

    GvString(LPCTSTR str) { string = staticStorage;  *this = str; }
    GvString(LPCTSTR str1,LPCTSTR str2) { string = staticStorage;  *this = str1; *this += str2; }
    GvString(const GvString &str) { string = staticStorage;   *this = str.string; }

	// destructor
    ~GvString();
    
	// compute hash code
    u_long		hash()		{ return GvString::hash(string); }

	// get the string length
    int			getLength() const	{ return strlen(string); }
    
	// set to empty string 
    void		makeEmpty(GvBool freeOld = TRUE);

    // get a C -Style const char *
    LPCTSTR	getString() const	{ return string; }
	operator LPCTSTR() const { return getString(); }  

	// operations 
    GvString &		operator =(LPCTSTR str);
    GvString &		operator =(const GvString &str)	{ return (*this = str.string); }
    
    GvString &		operator +=(LPCTSTR str);
    GvString &		operator +=(_TCHAR c);
	
	friend GvString operator+(const GvString& string1, const GvString& string2);
	friend GvString operator+(const GvString& string, TCHAR ch);
	friend GvString operator+(TCHAR ch, const GvString& string);
	friend GvString operator+(const GvString& string, LPCTSTR lpsz);
	friend GvString operator+(LPCTSTR lpsz, const GvString& string);


    GvBool				operator !() const { return (string[0] == '\0'); }
    friend GvBool		operator ==(const GvString &str, const char *s);
   
    friend GvBool		operator ==(const char *s, const GvString &str)
								{ return (str == s); }
    
    friend GvBool		operator ==(const GvString &str1, const GvString &str2)	{ return (str1 == str2.string); }
    friend GvBool		operator !=(const GvString &str, const char *s);
    
    friend GvBool		operator !=(const char *s, const GvString &str)	{ return (str != s); }
    friend GvBool		operator !=(const GvString &str1, const GvString &str2) { return (str1 != str2.string); }
    
	static u_long	hash(const char *s);    	

	// similar to CString, allocate buffer
	// create a RW buffer of at least that size
	char *getBuffer(int size);

	// release the buffer
	void releaseBuffer( int nNewLength = -1 );

	// get quoted
	int getQuoted(GvString &str);

	// set quoted 
	int setQuoted(const GvString &str);

	// set character at index to char 
	void set1(int index,char c) { if (index>=0 && index<getLength()) string[index]=c; }

	// vrmlscript methods
	//Returns the index of the first occurrence of string in the object, 
	//starting the search from fromIndex. 
	//fromIndex must be between 0 and length -1, where length is the number of characters in the string. If fromIndex is not specified, 
	//the search will start with character 0. 

	int indexOf(const GvString &search,int fromIndex=0);

	//Returns the index of the first occurrence of string in the object, 
	//starting the search backwards from fromIndex. 
	//fromIndex must be between 0 and length -1, where length is the number of characters in the string. If fromIndex is not specified, 
	//the search will start with character length - 1. 

	int lastIndexOf(const GvString &search,int fromIndex=-1);

	GvString& setSubstring(const GvString &string, int index1, int index2);
	
	//sets string to the  desired substring. 
	//If index1 is less than index2, the substring returned starts 
	//with the character at index1 and ends with the character before index2. 
	//If index1 is greater than index2, the substring returned starts with the character at index2 and ends with the character before index1. If index1 equals index2, the empty string is returned. index1 and index2 are any integer-valued expressions where 0<=index1, index2<length, where length is the number of characters in the string 

	GvString& toLowerCase(); 
	//all alphabetic characters of the string converted to lower case. 

	GvString& toUpperCase(); 
	// all alphabetic characters of the string converted to upper case. 



#ifdef _COM
	// assign by BSTR
	GvString&  operator = (BSTR str);

	// get SysAlloced BSTR
	BSTR SysAllocString() const;

    GvString(BSTR str) { string = staticStorage;  *this = str; }

#endif



  protected:
    _TCHAR		*string; // allocated string buffer, only if size exceeds STATIC_STORAGE						
    int			storageSize;
												
    #define GV_STRING_STATIC_STORAGE_SIZE		16

    _TCHAR		staticStorage[GV_STRING_STATIC_STORAGE_SIZE];

	// expand the buffer
    void		expand(int bySize);


	friend class GvSFString;
	friend class GvMFString;
};


//
// GvNameEntry
//
// entry in name hash table 

class GvNameEntry {
 public:
    GvBool		isEmpty() const   { return (string[0] == '\0'); }
    GvBool		isEqual(const char *s) const
	            { return (string[0] == s[0] && ! strcmp(string, s)); }
 private:

    // static NameEntry hashTable
    static int		nameTableSize;
    static GvNameEntry	**nameTable;
    static struct GvNameChunk *chunk;		// allocate blocks for names

 public:
     
    // init hash table 
    static void	initClass();
    // free chunk table and all table entrys 
    static void termClass();
    
    // insert string into name-table
    static const GvNameEntry *	insert(const char *s);

	// nametable is a global resource, protect by critical section
	static CRITICAL_SECTION locker; // Problem using CCriticalSection in static section
	static BOOL lockerInit;

 private:
    
    const char	*string;
    u_long		hashValue;
    
    GvNameEntry		*next;	// next element with same hash table entry		
    
    GvNameEntry(const char *s, u_long h, GvNameEntry *n)
	    { string = s; hashValue = h; next = n; }

friend class GvName;
};

//
// GvName 
//

// a name holds a token string
// instead of storing the string directly
// identical strings are merged, they are reffering to the same nameEntry
//

class GvName {
  public:
    GvName();
    GvName(const char *s)		{ entry = GvNameEntry::insert(s); }
    GvName(const GvString &s)	{ entry = GvNameEntry::insert(s.getString()); }

    GvName(const GvName &n)			{ entry = n.entry; }
    ~GvName()					{}
    
    const char		*getString() const	{ return entry->string; }
	operator const char *() const	{ return entry->string; }

    int			getLength() const   { return strlen(entry->string); }
    int	Length() const   { return strlen(entry->string); }


    static GvBool 	isIdentStartChar(char c,GvBool vrml2);
    static GvBool	isIdentChar(char c,GvBool vrml2);
    static GvBool 	isNodeNameStartChar(char c,GvBool vrml2);
    static GvBool	isNodeNameChar(char c,GvBool vrml2);

    int			operator !() const   { return entry->isEmpty(); }
    
	friend int		operator ==(const GvName &n, const char *s)
	{ return n.entry->isEqual(s); }
    
    friend int		operator ==(const char *s, const GvName &n)
	{ return n.entry->isEqual(s); }
    
    friend int 		operator ==(const GvName &n1, const GvName &n2)
	{ return n1.entry == n2.entry; }
    friend int		operator !=(const GvName &n, const char *s)
	{ return ! n.entry->isEqual(s); }
    friend int		operator !=(const char *s, const GvName &n)
	{ return ! n.entry->isEqual(s); }
    
    friend int 		operator !=(const GvName &n1, const GvName &n2)
	{ return n1.entry != n2.entry; }
  
  private:

    const GvNameEntry	*entry; // hash table entry
};

#endif /* _GV_STRING_ */
