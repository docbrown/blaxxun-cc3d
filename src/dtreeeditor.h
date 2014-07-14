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
// dtreeeditor.h : header file
//

#ifndef _DTreeEditor_h_
#define _DTreeEditor_h_

class CMyglView;
class GvDict;
class GvNameSpace;

class GvEditor;

class GOptimizeTraversal;
class GEditController;

#include <gvpath.h>



class DGvTreeItem {
public:

enum Types {
	NULL_ITEM,
	BASIC_NODE,
	USE_NODE,
	FIELD_ITEM,
	
	MFIELD_ELEMENT,
//	MINDEX_FACE_ELEMENT,
	
	GV_NODE,
	GV_GROUP_NODE,
	MAX_ITEM

};

	
	CTreeCtrl	&m_tree;	// reference to tree
	
	DGvTreeItem(CTreeCtrl	&tree) : m_tree(tree),node(NULL),field(NULL) {}

	HTREEITEM item;	// the tree item
	Types type;		// type of item as computed by set
	
	GvNode *node;	// the param value of item
	
	GvField *field;	// the field if computed
	int nodeChildIndex;	// the childIndex in parent group if computed

	HTREEITEM ownerItem;

	int Set(HTREEITEM newItem);

	// initialize by the owner of child
	int SetToOwner(DGvTreeItem &child);


	
	HTREEITEM Owner();
		
};

/////////////////////////////////////////////////////////////////////////////
// DTreeEditor dialog



class DTreeEditor : public CDialog
{
// Construction
public:
	DTreeEditor(CWnd* pParent = NULL,CMyglView *pView =NULL);   // standard constructor
	~DTreeEditor();




	CMyglView *m_pView;
	
	GEditController	*m_controller;


	GvDict *refDict;

	GView *GetView();
	GvScene *GetScene();
    

    
    //GvNameSpace *nameSpace; // name space for tree

	
	int viewMode;
	gbool viewCompact; // TRUE, view children directly below node 

	static int writeInlines; // write inlines recursively
	static int updateInlineBbox;	// update the bbox of the inlines
	static int maxMFieldItems;		// max number of MFIELD items to expand

	// Node Buffer for cut, copy, paste
	// GvNodeHandle  copyBuffer; // to be moved in editor
    
    // GvEditor *editor;
 
	DGvTreeItem selection,selectionOwner;

	// images for type
	int image[DGvTreeItem::MAX_ITEM];
	int selectedImage[DGvTreeItem::MAX_ITEM];



    enum EditType {
            EDIT_AUTO,
            EDIT_ASCII,
            EDIT_COLOR
    };


	// for drag and drop operations
	CImageList  m_imagelist;
	CImageList* m_dragImage;
	
	HTREEITEM m_Source;					// source of drag
	HTREEITEM m_Target;					// target of drag
						
	CBitmap bm;

	BOOL m_indelete;
	BOOL m_draging;						// flag to indicate user is dragging
	BOOL m_dirty;						// flag to indicate whether user has edited any term



	
	int UpdateSelection(HTREEITEM item);

	int SetSelection(GvPath &path, int pathIndex);

	int SetSelection(HTREEITEM item);
	int SetSelection(GvNode *node);
	int SetSelection(GvName &name);


	void Redraw();
	
	int changes;
	
	int autoRedraw;


	int OnChanged();


	HTREEITEM AddItem(HTREEITEM hParent,const char *text,void * param,HTREEITEM hInsertAfter = TVI_LAST,int expand=0,int type=0);
	BOOL DeleteItem(HTREEITEM item); 
	
	// delete direct childs of type field 
	int DeleteFieldItems(HTREEITEM hParent);

	// Expand and display node & children
	int InsertNode(HTREEITEM hParent,HTREEITEM hLocation,GvNode *node,HTREEITEM &hReturn);
    
    // Insert an MFNode childs below hParent
    int Insert(HTREEITEM hParent,GvMFNode *fn);

    // Insert MField elements below hParent
    int Insert(HTREEITEM hParent,GvMField *fn);
    
	// Insert group children elements below hParent
	int Insert(HTREEITEM hParent,GvGroup *fn);


	// Expand and display node & children
	int InsertField(HTREEITEM hParent,HTREEITEM hLocation,GvNode *node,const GvName &fieldName,GvField *field,int expand,HTREEITEM &hReturn);
	
	// Expand and display fields of node
	int InsertNodeFields(HTREEITEM hParent,GvNode *node,int fieldsWithDefaults=0,int fieldTypes=0);


	int UpdateNode(HTREEITEM hParent,HTREEITEM hLocation,GvNode *node,HTREEITEM &hReturn);
	int UpdateField(HTREEITEM hParent,HTREEITEM hLocation,GvField *field,HTREEITEM &hReturn);
	
	int UpdateNode(DGvTreeItem &parent,DGvTreeItem &item);
	int UpdateField(DGvTreeItem &parent,DGvTreeItem &item);
	int UpdateFieldItem(DGvTreeItem &parent,DGvTreeItem &item);
	
	// refresh node , by reinserting fields
	int UpdateNode(GvPath &path, int pathIndex);



	// Do Node editing functions

	int DoPaste(DGvTreeItem &owner,DGvTreeItem &item,GvNode *value,int replace=0);

	int DoPasteInto(DGvTreeItem &owner,DGvTreeItem &item,GvNode *value);
	int DoCut(DGvTreeItem &owner,DGvTreeItem &item);
	int DoCopy(DGvTreeItem &owner,DGvTreeItem &item);

	// get this node as one shell
	BOOL GetShell(GvNode *node,GvNodeHandle &shell,GvBool &isSolid);


/*
	int DoDelete(HTREEITEM item); 
	int DoCut(HTREEITEM item); 
	int DoCopy(HTREEITEM item); 
	int DoCopyLink(HTREEITEM item); 
	int DoPaste(HTREEITEM item);
*/


    //  edit field 
	int OnFieldEditType(int editType = EDIT_AUTO);


	
	HTREEITEM GetRoot() { return (m_tree.GetRootItem()); }
	HTREEITEM GetSelection() { return (m_tree.GetSelectedItem()); }

	int ExpandRoot();

// Dialog Data
	//{{AFX_DATA(DTreeEditor)
	enum { IDD = IDD_TREE_EDITOR };				
	CTreeCtrl	m_tree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DTreeEditor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DTreeEditor)
	afx_msg void OnBegindragTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteitemTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandingTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetdispinfoTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOutofmemoryTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnFileExit();
	afx_msg void OnUpdateFileExit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveas(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveas();
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNodeInsertFile();
	afx_msg void OnUpdateNodeInsertFile(CCmdUI* pCmdUI);
	afx_msg void OnNodeNew();
	afx_msg void OnUpdateNodeNew(CCmdUI* pCmdUI);
	afx_msg void OnNodeRename();
	afx_msg void OnUpdateNodeRename(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNodeSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnNodeSaveAs();
	afx_msg void OnViewRefresh();
	afx_msg void OnUpdateViewRefresh(CCmdUI* pCmdUI);
	afx_msg void OnFieldEdit();
	afx_msg void OnUpdateFieldEdit(CCmdUI* pCmdUI);
	afx_msg void OnNodeExpandfields();
	afx_msg void OnUpdateNodeExpandfields(CCmdUI* pCmdUI);
	afx_msg void OnViewDefedObjects();
	afx_msg void OnUpdateViewDefedObjects(CCmdUI* pCmdUI);
	afx_msg void OnViewTree();
	afx_msg void OnUpdateViewTree(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnViewReferences();
	afx_msg void OnUpdateViewReferences(CCmdUI* pCmdUI);
	afx_msg void OnNodeConvertToIndexedFaceset();
	afx_msg void OnUpdateNodeConvertToIndexedFaceset(CCmdUI* pCmdUI);
	afx_msg void OnNodeConvertToSeparator();
	afx_msg void OnUpdateNodeConvertToSeparator(CCmdUI* pCmdUI);
	afx_msg void OnFieldEditAsFilename();
	afx_msg void OnUpdateFieldEditAsFilename(CCmdUI* pCmdUI);
	afx_msg void OnFieldEditAsColor();
	afx_msg void OnUpdateFieldEditAsColor(CCmdUI* pCmdUI);
	afx_msg void OnFieldDrag();
	afx_msg void OnUpdateFieldDrag(CCmdUI* pCmdUI);
	afx_msg void OnNodeExpandInlines();
	afx_msg void OnUpdateNodeExpandInlines(CCmdUI* pCmdUI);
	afx_msg void OnOptionsUpdateinlineboundingboxes();
	afx_msg void OnUpdateOptionsUpdateinlineboundingboxes(CCmdUI* pCmdUI);
	afx_msg void OnOptionsWriteinlines();
	afx_msg void OnUpdateOptionsWriteinlines(CCmdUI* pCmdUI);
	afx_msg void OnEditPasteInto();
	afx_msg void OnUpdateEditPasteInto(CCmdUI* pCmdUI);
	afx_msg void OnFieldEditAsString();
	afx_msg void OnUpdateFieldEditAsString(CCmdUI* pCmdUI);
	afx_msg void OnEditPasteCopy();
	afx_msg void OnUpdateEditPasteCopy(CCmdUI* pCmdUI);
	afx_msg void OnNodeConverttoAnchor();
	afx_msg void OnNodeConvertToTransform();
	afx_msg void OnNodeUpdateBbox();
	afx_msg void OnUpdateNodeUpdateBbox(CCmdUI* pCmdUI);
	afx_msg void OnNodeCombine();
	afx_msg void OnUpdateNodeCombine(CCmdUI* pCmdUI);
	afx_msg void OnNodeExpandbbox();
	afx_msg void OnUpdateNodeExpandbbox(CCmdUI* pCmdUI);
	afx_msg void OnNodeTransform();
	afx_msg void OnUpdateNodeTransform(CCmdUI* pCmdUI);
	afx_msg void OnNodeNormalize();
	afx_msg void OnUpdateNodeNormalize(CCmdUI* pCmdUI);
	afx_msg void OnNodeOptimize();
	afx_msg void OnUpdateNodeOptimize(CCmdUI* pCmdUI);
	afx_msg void OnSettingsOptimizer();
	afx_msg void OnNodeConvexHull();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNodeCheck();
	afx_msg void OnUpdateNodeCheck(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
/////////////////////////////////////////////////////////////////////////////
// DNodeNew dialog

class DNodeNew : public CDialog
{
// Construction
public:
	DNodeNew(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DNodeNew)
	enum { IDD = IDD_NEW_NODE };
	CComboBox	m_typeList;
	CString	m_type;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DNodeNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DNodeNew)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


void GetOptimizerSettings(GOptimizeTraversal &traversal);


/////////////////////////////////////////////////////////////////////////////
// DOptimizeSettings dialog

class DOptimizeSettings : public CDialog
{
// Construction
public:
	DOptimizeSettings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DOptimizeSettings)
	enum { IDD = IDD_OPTIMIZE_SETTINGS };
	CString	m_bspHeuristic;
	BOOL	m_buildBsp;
	BOOL	m_expandIfs;
	BOOL	m_keepInlines;
	BOOL	m_splitFaces;
	BOOL	m_ignoreRoutes;
	BOOL	m_keepLods;
	int		m_maxBspLevel;
	int		m_bspSearchLimit;
	BOOL	m_compressIfs;
	BOOL	m_removeNames;
	BOOL	m_expandFaceColors;
	int		m_bspInlineLevel;
	BOOL	m_bspInline;
	BOOL	m_keepPrimitives;
	BOOL	m_ifsRoundCoord;
	float	m_ifsRoundCoordScale;
	BOOL	m_bx3d;
	BOOL	m_applyTransforms;
	BOOL	m_lookupCoord;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DOptimizeSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DOptimizeSettings)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern gbool writeVrmlVerbose;
extern float knotReductionFactor;


