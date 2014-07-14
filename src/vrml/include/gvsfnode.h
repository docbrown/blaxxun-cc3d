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
#ifndef  _GV_SF_NODE_
#define  _GV_SF_NODE_

#include <gvnode.h>

#include <gvsubfield.h>

class GvNode;

class GvSFNode : public GvSField 
#ifdef _COM
	,public CComDualImpl<EventInSFNode, &IID_EventInSFNode, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutSFNode, &IID_EventOutSFNode, &LIBID_GLViewVrmlLib>
#endif

{
  public:

    GvNodeHandle value;
    
    GV_SFIELD_HEADER(GvSFNode);

    GvSFNode(GvNode* defaultValue=NULL) : value(defaultValue) {}

	static GvFieldClass getClassFieldType() { return (SFNODE); }
	GvFieldClass	 getFieldType() const { return (SFNODE); }

 	
	//int ioValue(AStream &a); 
	
	operator GvNode*(){ return (GvNode *)value; }
	GvNode *  get() const  { return (GvNode *)value; }
	
	void set(GvNode * newValue);
    void get(GvNode* &Value) const { Value = (GvNode*) value; }
    void get(GvNodeHandle &Value) const { Value = value; }

    //eval field to the actual value node of node,
    GvNode* Eval(GTraversal &state) { 
		if (!value) return NULL;
		return ((GvNode*) value)->Eval(state);
	}
					
    //eval field to the actual value node of node,
    GvNode* Eval() { 
		if (!value) return NULL;
		return ((GvNode*) value)->Eval();
	}


	/* now inline 
	virtual int Traverse(GTraversal &state);
	virtual int Traverse(GPathTraversal &state);
	*/

	// Traverse the node 
	inline int Traverse(GTraversal &state) 
	{
   		if (value) {
   			return  value->Traverse(state);
		} 
		return(0) ; 
	}

	inline int Traverse(GPathTraversal &state) 
	{
   		if (value) {
			int ret;
			GSET_PATH_FIELD(state,*this);
   			ret = value->Traverse(state);
   			return(ret);
		} 
		return(0) ; 
	}



	virtual int Do(GioTraversal &state);
	virtual int Do(GDisplayTreeTraversal &state); // implemented in GDisplayTree.cpp

	int get(GvString &value);
	int set(const char* value);

    const GvSFNode& operator= (const GvSFNode &f) { set((GvNode*) f.value);  return(f); }
    GvBool operator==(const GvSFNode &f) const { return((GvNode*)value == (GvNode*)f.value); }

    // computes copyOnInstance flag from any child nodes flag set
    GvBool computeCopyOnInstance(GvCopyContext &context);

	// get bounding box of SFNode
	GvBool getBBox(BBox &bbox);

#ifdef _COM
	BEGIN_COM_MAP(GvSFNode)
		COM_INTERFACE_ENTRY(EventInSFNode)
		COM_INTERFACE_ENTRY(EventOutSFNode)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvSFNode)

	COM_DEFAULT_SFIELD_METHODS()

    /* EventIn methods */
    STDMETHOD(setValue)(Node* newValue);

	/* EventOut methods */ 
	STDMETHOD(getValue)(Node* *value);


#endif



};

#endif /* _GV_SF_NODE_ */
