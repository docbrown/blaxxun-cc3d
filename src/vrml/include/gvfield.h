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
#ifndef  _GV_FIELD_
#define  _GV_FIELD_
/******************************************************************************
@doc

@module GvField.h - root class for VRML fields|

Copyright (c) 1996 - 1998 by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GvField>
	<c GvSField>
	<c GvMField>

Notes:

Changes:
$Revision: 1.19 $
$Log: gvfield.h,v $
Revision 1.19  1999/07/06 16:34:41  tom
*** empty log message ***

Revision 1.18  1999/05/10 13:53:46  tom
*** empty log message ***



Todo :

******************************************************************************/



#include <gvstring.h>

class AStream;
class GioTraversal;
class GDisplayTreeTraversal;

class GvInput;
class GvNode;
class GvField;
class GvConnection;

#include <gvfielddata.h>
#include <gvconnection.h>


//
// field are storing a value
// 
#ifdef _COM


//#include "glviewvrml.h"
//typedef FieldType GvFieldClass;

// default COM methods for FIELD 
#define COM_DEFAULT_SFIELD_METHODS() \
    /* IUnknown methods */ \
	STDMETHOD_(ULONG, AddRef)() { return InternalAddRef();  } \
	STDMETHOD_(ULONG, Release)() { return InternalRelease();  } \
	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject) {return _InternalQueryInterface(iid, ppvObject);} \
	\
    /* EventIn methods */ \
    STDMETHOD(getType)(int *type) { return GvField::getType(type); } \
	STDMETHOD(setValueFromString)( /* [in] */ BSTR value) { return GvField::setValueFromString(value); } \
    \
	/* EventOut methods */  \
    STDMETHOD(advise)(EventOutObserver *observer, IDispatch* userData) { return GvField::advise(observer,userData); } \
    STDMETHOD(unAdvise)(EventOutObserver *observer) { return GvField::unAdvise(observer); } \
	STDMETHOD(toString) (/* [retval][out] */ BSTR  *value) { return GvField::toString(value); } \


#define COM_DEFAULT_MFIELD_METHODS() COM_DEFAULT_SFIELD_METHODS()

// notify browser of an extern touch of field ==> redraw 
#define COM_EXTERN_TOUCH() 	/* GvConnection::connectionsTriggeredCnt++ */ 


// temp hack definitions for Vec4f
#define SFVEC4F  SFVEC3F
#define MFVEC4F  MFVEC3F

#endif


//
// GvField : Root for all field classes
//

/*! \class GvField
 *  \brief ROOT class for all VRML field classes
 *   
 *  Supports basic mechanism for field information, parsing, routing.

Anatomy of a Field :
A Field has value.
A field is usually contained in a class: GvNode. container
The member owner points back to the Node containing the field.

changes on the Field Value ::set(xxx) is triggering all connections and is informing 
the owner node via OnFieldChanged(*this) that the field has been changed.

Problem:
	each Interface adds 4 Bytes for the VTBL ptr, gr
	to think: find a way using a static vtable & virtual function (QI)
	checkout TearOff objects in ATL
	//,public Field,EventIn
	//,public CComTearOffObjectBase<EventOut>

	ATL_NO_VTABLE 

	also IDispatchImpl has a tih and call adrref/release in each addref / release


*/



class GvField : 
	public RTRoot

#ifdef _COM
	,public CComDualImpl<Field, &IID_Field, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventIn, &IID_EventIn, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOut, &IID_EventOut, &LIBID_GLViewVrmlLib>
#endif

{

public:	


#ifdef _COM


	BEGIN_COM_MAP(GvField)
		//COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(Field)
		COM_INTERFACE_ENTRY(EventIn)
		COM_INTERFACE_ENTRY(EventOut)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvField)


	// propagate to node containing field
	ULONG InternalAddRef();
	ULONG InternalRelease();

	// ATL 2.0 
	void Lock() {}
	void Unlock() {}


    /* IUnknown methods */
	STDMETHOD_(ULONG, AddRef)(); 
	STDMETHOD_(ULONG, Release)();
	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
	{return _InternalQueryInterface(iid, ppvObject);}

	// needed static Methods from CComObjectRoot 
	static HRESULT WINAPI InternalQueryInterface(void* pThis,
		const _ATL_INTMAP_ENTRY* entries, REFIID iid, void** ppvObject);
	static HRESULT WINAPI _Chain(void* pv, REFIID iid, void** ppvObject, DWORD dw);


    /* Field methods */
    STDMETHOD(getType)(int * type);
	STDMETHOD(setValueFromString)( /* [in] */ BSTR value);

	/* EventOut methods */ 
    STDMETHOD(advise)(EventOutObserver *observer, IDispatch* userData);
    STDMETHOD(unAdvise)(EventOutObserver *observer);
	STDMETHOD(toString) (/* [retval][out] */ BSTR  *value);


#endif 

  public:
	RT(RTRoot);

    virtual ~GvField();

	//! Inventor ignored flag - obsolete 
    void		setIgnored(GvBool ig)	{ flags.ignored = ig;   }
    GvBool		isIgnored() const	{ return flags.ignored; }

    //! has the field the initial default value for the node ?
    inline GvBool		isDefault() const	{ return flags.hasDefault; }

    //! set default flag 
    inline void		setDefault(GvBool def)	{ flags.hasDefault = def; }

	// modified flag
    //! has the field been modied ?
    inline GvBool		isModified() const	{ return flags.modified; }

    //! set modied flag 
    inline void		setModified(GvBool def)	{ flags.modified = def; }


	//! set pointer to node containing this field
    void		setContainer(GvNode *cont);

    //! get pointer to node, containing this field
    inline GvNode *	getContainer() const	{ return container; }

    //! read field from in
    GvBool		read(GvInput *in, const GvName &name);

    //! constructor
    GvField()		{ 
        flags.hasDefault = TRUE; 
        flags.ignored = FALSE; 
        flags.modified = FALSE; 
        flags.isInput = TRUE; 
        flags.isOutput = TRUE; 
        flags.connectionsEnabled = TRUE; 
        container = NULL;
        connections = NULL;
    }


	//! get VRML Type name for field
	virtual const char * GetType();

	//! get field type as predefine enum type 
	virtual GvFieldClass getFieldType() const = 0;

    //! write field to AStrea,
	virtual int io(AStream &a, const GvName &name);
    
    //! set value to interpolation from two same fields 
	virtual int interpolateFrom(GvField *a,GvField *b, float t);



  public:

  public:
	//! field flags
	struct {
	unsigned int hasDefault		: 1;	//!< Field is set to default value
	unsigned int ignored		: 1;	//!< Field value is to be ignored - Inventor only
	unsigned int modified		: 1;	//!< Field has been modified
	unsigned int isInput		: 1;	//!< true if field is a input event
	unsigned int isOutput		: 1;	//!< true if field is output event
	unsigned int connectionsEnabled	: 1;  //!< true if field is output event

    }	flags;


    //! the node containing this field
    GvNode		*container;

    //! create a new field, given the classname for field e.g. SFFloat
    static GvField *createInstanceFromName(const GvName &className);

    //! read the field value from input
    virtual GvBool	readValue(GvInput *in) = 0;
	
    //! output value to AStream
	virtual int ioValue(AStream &a);

	//! do output traversal
    virtual int Do(GioTraversal &state);

	//! get value of field as string
    virtual int get(GvString &value);

    //! set value of field by string
	virtual int set(const char* value);

    //!a ssignment operator, assuming same field type 
    //! do basic assignment, types must be the same !!!!!!!

    virtual const GvField& operator =(const GvField &f) = 0;

    //! set value of field to value using = and call OnChange and setDefault(FALSE)
    virtual int set(const GvField *value);

    //! create a acopy of this field, copy will have same class as this 
	virtual GvField* copy();

    //! a list of connections implemting ROUTES, IS etc.
    GvConnection* connections;
    
    //! true if field has any outgoing connections
    GvBool hasConnections() { return(connections != NULL); }

    //! return pointer to first connection
    GvConnection*  getConnections() { return(connections); }
    
    //! delete all connections
    int deleteConnections();
    
    //! add a connection, and optionally trigger the the connection 
    GvBool addConnection(GvConnection *connection,GvBool trigger=FALSE);
    
    //! remove a connection
    GvBool removeConnection(GvConnection *connection);

	//! remove a specific connection filtered by predicate function
	GvBool removeConnection(GvBool (*Pred)(GvConnection *c, void *data),void *data);

	//! find a specific connection filtered by predicate function
	GvConnection* findConnection(GvBool (*Pred)(GvConnection *c, void *data),void *data);


    // removes connections to a node
    // int removeConnections(GvNode *node);

    // removes connections to a field
    // int removeConnections(GvField *field);

    //! trigger all outgoing connections 
    int OnTrigger();

	//! field has been changed, trigger all connections if they are enabled
    int touch()	{ 
            int ret = 0;
            flags.modified = TRUE; 

            if (hasConnections() && connectionsEnabled()) { 
                 enableConnections(0);  // prevent recursive looping
                       ret=OnTrigger(); 
                 enableConnections(1);
            }
            return(ret);
    }
    //!reset modified flag
    void untouch()	{ flags.modified = FALSE; }
    
    //! are connections enabled ?
    int connectionsEnabled() const { return(flags.connectionsEnabled); }
    
    //! set enable connection flag, and return old flag
    int enableConnections(int flag)	{ 
                int old = (flags.connectionsEnabled); 
                flags.connectionsEnabled=flag; 
                return (old);
            }
	

	//! Notif owner node on change 
	int OnChanged();

	friend class GvFieldData;
};


#ifdef _COM_XXX

// a static helper to get the vtable ?

class GvFieldTearOff : GvField
	,public CComDualImpl<Field, &IID_Field, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventIn, &IID_EventIn, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOut, &IID_EventOut, &LIBID_GLViewVrmlLib>
{

};

static GvFieldTearOff __fieldTearOff;


#endif


/*! \class GvSField
 *  \brief abstract root for single valued VRML fields
 *   
 */

class GvSField : public GvField {

  public:
	RT(GvField);

    virtual ~GvSField();
	
	int Do(GioTraversal &state);

  protected:
    GvSField();

  private:
  public:	

    //! read the field value from input
    virtual GvBool	readValue(GvInput *in) = 0;

};

/*! \class GvMField
 *  \brief abstract root for multi valued VRML fields
 *   
 */

class GvMField : public GvField {

  public:
	RT(GvField);
    int			num;		//!< Number of values
    int			maxNum;		//!< Number of values allocated

    //! Destructor
    virtual ~GvMField();

  protected:

    GvMField();
  
  public:      
	//! get field type of corresponding SF Field 
	virtual GvFieldClass getSFFieldType() const = 0;

	//! set new length
    virtual void	makeRoom(int newNum);

  private:
    virtual void	allocValues(int num) = 0;

	//! read1 MF Element value, must be implemented by derived classes 
    virtual GvBool	read1Value(GvInput *in, int index) = 0;

  public:  

    //! read the field value from input
	virtual GvBool	readValue(GvInput *in);

    //! read a list of value until endChar found
    //! endChar is not read !
    GvBool readValueList(GvInput *in,int endChar);


  public:

	//!return number of elements
	inline int getNum() const { return(num); }

	//!return Number of values allocated
	inline int getMaxNum() const { return(maxNum); }

	//!return number of elements
	inline int Length() const { return(num); }
	//!return Number of values allocated
	inline int MaxLength() const  { return(maxNum); }

	//!set Number of values allocated
	void setLength(int newLength) { makeRoom(newLength); }
	
	virtual int ioValue(AStream &a);  // not needed
    virtual int	io1Value(AStream &a, int index);

	// string conversion 
	virtual int get(GvString &value);
	virtual int set(const char* value);
	
	virtual int get1(GvString &value, int index);
	virtual int set1(const char* value, int index);

	//! allocates a new SFField with value at index 
	virtual GvField* get1Field(int index) = 0;

	virtual int Do(GioTraversal &state);
	virtual int Do(GDisplayTreeTraversal &state); // implemented in GDisplayTree.cpp

    // each instance will normally have an valueType *values; entry

    // macros are adding functions like

    // int set(int newNum,const valueType *newValues);
    // const className & operator= (const className &f) { set(f.num,f.values);return(f); }

};

#endif /* _GV_FIELD_ */

