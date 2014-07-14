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
#ifndef  _GV_MFNode_
#define  _GV_MFNode_

#include <gvsubfield.h>

class GvNode;
class GTraversal;
class GPathTraversal;
class GDisplayTreeTraversal;
class GvSFNode;


class GvMFNode : public GvMField
#ifdef _COM
	,public CComDualImpl<EventInMFNode, &IID_EventInMFNode, &LIBID_GLViewVrmlLib>
	,public CComDualImpl<EventOutMFNode, &IID_EventOutMFNode, &LIBID_GLViewVrmlLib>
#endif

 {
  public:
	// array of nodehandles 
	GvNodeHandle *values;
    
    GV_MFIELD_HEADER(GvMFNode,GvNodeHandle, 1);

	operator GvNodeHandle *() { return(values); }

	static GvFieldClass getClassFieldType() { return (MFNODE); }
	GvFieldClass getFieldType() const { return (MFNODE); }
	GvFieldClass getSFFieldType() const { return (SFNODE); }
 
    // get the class id of the corresponding SF field class
    TypeId getSFType();

	const GvNodeHandle *get() const { return values; }

	// set to single node 
	void set(GvNode *node);

	// set and parse from string 
	int set(const char* s);

	
	// append a node
	//int Append(GvNode * newValue); 
	
	// delete a node
	//int Delete(int index); 
	// delete(unref) all nodes
	//void Delete(); 

	// get node at index i
	GvNode* get1(int i);

    // get SFNode copy from element at index i
	GvField* copy1(int i);

    // GvGroup functions
	
    // append a new child
	void append(GvNode *node);

    // add a new child, if not already in
	GvBool add(GvNode *node);

    // replace child at index i
	GvBool set1(int index, GvNode *node);

    // replace child at index i
	GvBool set1(int index, GvSFNode *node);

	// allocate a new SFField for element at index 
	GvField* get1Field(int index);

    // add new childs, if not already in list
    void add(GvMFNode *nodes);

    // remove  child, if in list, TRUE node removed
    GvBool remove(GvNode *node);

    // remove new childs, if  in list
    void remove(GvMFNode *nodes);


	// delete child at index
	GvBool deleteElement(int index);

	// delete child at index with replacement swap (value[index]=value[num-1]
	GvBool deleteElementSwap(int index);

	// delete all children
	void deleteAll();
	void flush() { deleteAll(); }

	// remove all nodes from list with a refCnt <= refCnt 
	// return number of removed nodes 
	int removeUnused(int refcnt=1);

	
	// insert & ref child at index
	int insert(int index,GvNode *node);
	
	// move(index, srcGroup[index])
	int moveChild(int index,GvMFNode *srcField, int srcIndex);
	
	// find a child by index
	GvNode *find(int index);

	// find a child by ptr, return index, -1  if not found
	int  find(GvNode * node);

	// find a child by name
	GvNode *find(const GvName &name);
	
    // find a child by type
	GvNode *find(TypeId type);

    // computes copyOnInstance flag from any child nodes flag set
    GvBool computeCopyOnInstance(GvCopyContext &context);

	// get bounding box of MFNode
	GvBool getBBox(BBox &bbox);

	// check for light sources in list and move them into front 
	GvBool CheckLights();
	

	// traverse all nodes
	/* now inline 
	virtual int Traverse(GTraversal &state);
	virtual int Traverse(GPathTraversal &state);
	*/
	inline int Traverse(GTraversal &state) 
	{
		int ret=0;
		int curIndex=0;
		while (curIndex < Length()) {
			GvNode *n = values[curIndex]; // for savety ignore NULL nodes 
			if (n) ret=n->Traverse(state);
			curIndex ++;
		}
		return ret;
	}

	// need to push & pop Path information
	inline int Traverse(GPathTraversal &state) 
	{
		int ret=0;
		GSET_PATH_FIELD(state,*this);
		int curIndex=0;
		while (curIndex < Length()) {
			GvNode *n = values[curIndex]; // for savety ignore NULL nodes 
			
			GSET_PATH_INDEX(state,curIndex);

			if (n) ret=n->Traverse(state);
			curIndex ++;
		}
		return ret;
	}


	// traverse all nodes, and a nl after each element
	virtual int TraverseIo(GioTraversal &state);

	virtual int Do(GioTraversal &state);

	virtual int Do(GDisplayTreeTraversal &state); // implemented in GDisplayTree.cpp

#ifdef _COM
	BEGIN_COM_MAP(GvMFNode)
		COM_INTERFACE_ENTRY(EventInMFNode)
		COM_INTERFACE_ENTRY(EventOutMFNode)
		COM_INTERFACE_ENTRY_CHAIN(GvField)
	END_COM_MAP()

	DECLARE_NOT_AGGREGATABLE(GvMFNode)

	COM_DEFAULT_SFIELD_METHODS()

    /* EventIn methods */
    STDMETHOD(setValue)(int cnt,IDispatch* *newValue); 
    STDMETHOD(set1Value)(int index,Node* newValue); 
   //STDMETHOD(setValueByField) (/* [in] */ EventOutMFNode *value);


	/* EventOut methods */ 
    STDMETHOD(getSize)(int *cnt);
	STDMETHOD(getValue)(int cnt,IDispatch* *v); 
	STDMETHOD(get1Value)(int index,Node**v); 


#endif



};

#endif /* _GV_MFNode_ */
