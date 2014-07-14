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
// dtreeeditor.cpp : implementation file
//

/*

2.06.96 HG PasteInto : DoubleClick = FieldEdit, Color Edit
28.05.97 Fix OnLBUttonDown if actually no drag, vrml 2.0 save fix 

  To Do:
        Cut / Copy / Paste & drag on SFNode MFNode fields

        edit as filename

  more ops 

  paste into during drag (e.g. with shift)
  mfield editing



*/


#include "stdafx.h"


#include "glviewhdr.h"

#include "gversion.h"

#include "gclass.h"
#include "gshell.h"

#include "gshelli.h"

#include "ghull.h"

#include <gvfields.h>

#include <gvnamespace.h>
#include <gvnodes.h>

#include <gvproto.h>
#include <gvscript.h>

#include "gdisplaytree.h"
#include "giotraversal.h"
#include "grayselect.h"
#include "gbuildshell.h"


#include "dtreeeditor.h"
#include "DTransform.h"

#include "greporter.h"

#include "DString.h"

#include "greadgv.h"

#include "grender.h"
#include "gmodel.h"

#include "gvtraverse.h"

#include "gevent.h"
#include "gedit.h"

// undo // redo 

#include <gvedit.h>

// new 
#include "goptimizetraversal.h"
#include "gvbsp.h"
#include "gnurbsutil.h"
#include "gvnurbs.h"




#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*	
use first planar object
use largest planar object
split along largest dimension
*/

static CString bspHeuristic("separation planes"); // "use largest planar object";

static gbool buildBspTree=FALSE;
static gbool applyTransforms=TRUE;

static gbool decomposeIfs=FALSE;
static gbool doInlines=TRUE;
static gbool splitFaces = FALSE;
static gbool compressIfs = TRUE;
static gbool removeNames = FALSE;
static gbool expandFaceColors = FALSE;

static gbool bx3dOpt = FALSE;


static gbool writeVrmlVerbose=TRUE;
static float knotReductionFactor=1.0f;


static gbool ignoreRoutes = FALSE;
static gbool keepLods = TRUE;
static int   maxBspLevel = 26;
static int   bspSearchLimit = 77;

static  int		bspInlineLevel=10;
static	BOOL	bspInline=FALSE;
static	BOOL	keepPrimitives=FALSE;
static	BOOL	ifsRoundCoord=FALSE;
static	float	ifsRoundCoordScale= 0.001f;
static	BOOL	ifsLookupCoord=FALSE;


void GetOptimizerSettings(	GOptimizeTraversal &traversal)
{

		traversal.buildBspTree = buildBspTree;
		traversal.decomposeIfs = decomposeIfs;
		traversal.doInlines = doInlines;
		traversal.applyTransforms= applyTransforms;
		traversal.ignoreRoutes = ignoreRoutes;
		traversal.expandLods = !keepLods;
		traversal.compressIfs = compressIfs;
		traversal.removeNodeNames = removeNames;
		traversal.expandFcIfs = expandFaceColors;
		traversal.keepPrimitives =keepPrimitives;
		
		traversal.doCoordRound = ifsRoundCoord;
		traversal.coordRound = ifsRoundCoordScale;
		traversal.doLookupCoord = ifsLookupCoord;

		if (bx3dOpt) {
			traversal.bx3d = TRUE;
			traversal.buildBspTree = FALSE;
			traversal.keepPrimitives = FALSE;
			traversal.optimizeIfs = TRUE;
			traversal.expandFcIfs = TRUE;
		}



		// bsp builder settings 
		GvBspTree::bspSplit = splitFaces;
		GvBspTree::maxBspSplit = maxBspLevel;
		GvBspTree::bspSearchLimit = bspSearchLimit;
		GvBspTree::ClearStats();

		if (bspHeuristic == "use first planar object")
			GvBspTree::bspHeuristic = USE_FIRST_PLANAR_OBJECT;
		else if (bspHeuristic == "use largest planar object")
			GvBspTree::bspHeuristic = USE_LARGEST_PLANAR_OBJECT;
		else if (bspHeuristic == "split along largest dimension")
			GvBspTree::bspHeuristic = SPLIT_ALONG_BBOX;
		else if (bspHeuristic == "separation planes")
			GvBspTree::bspHeuristic = COMPUTE_SEPARATION_PLANE;

}


/////////////////////////////////////////////////////////////////////////////
// DTreeEditor dialog


int DTreeEditor::writeInlines=0; // write inlines recursively
int DTreeEditor::updateInlineBbox=1;	// update the bbox of the inlines
int DTreeEditor::maxMFieldItems=100;		// max number of MFIELD items to expand




DTreeEditor::DTreeEditor(CWnd* pParent /*=NULL*/,CMyglView *pView)
	: CDialog(DTreeEditor::IDD, pParent), m_pView(pView), selection(m_tree),selectionOwner(m_tree)
{

//static enum GDisplayTreeTraversal::Mode 
	
	refDict = NULL;

	viewMode = GDisplayTreeTraversal::MODE_TREE;
	viewCompact = FALSE;

  	// images for type
	for(int i=0; i< DGvTreeItem::MAX_ITEM; i++) 
	   image[i] = selectedImage[i] =0;

	image[DGvTreeItem::GV_GROUP_NODE] = 4;
	selectedImage[DGvTreeItem::GV_GROUP_NODE] =5;
	image[DGvTreeItem::GV_NODE] = 3;
	selectedImage[DGvTreeItem::GV_NODE] =1;

	image[DGvTreeItem::FIELD_ITEM] = 6;
	selectedImage[DGvTreeItem::FIELD_ITEM] =7;
	// 8 9 == USE

	m_Source = NULL;
	m_Target = NULL;
	m_dragImage = NULL;
	changes=0;
	autoRedraw = 1;
	m_indelete = FALSE;
	m_draging = FALSE;
	m_dirty = FALSE;

	//{{AFX_DATA_INIT(DTreeEditor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

DTreeEditor::~DTreeEditor()
{
   if (refDict) delete refDict;
   refDict = NULL;
   // copyBuffer auto
}

void DTreeEditor::Redraw()
{
	if (m_pView) {
		m_pView->Redraw();
	}
}

int DTreeEditor::OnChanged()
{
 	changes++;
	if (autoRedraw) Redraw();
	return(1);
}



void DTreeEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DTreeEditor)
	DDX_Control(pDX, IDC_TREE, m_tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DTreeEditor, CDialog)
	//{{AFX_MSG_MAP(DTreeEditor)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE, OnBegindragTree)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TREE, OnBeginlabeleditTree)
	ON_NOTIFY(TVN_DELETEITEM, IDC_TREE, OnDeleteitemTree)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE, OnEndlabeleditTree)
	ON_NOTIFY(TVN_GETDISPINFO, IDC_TREE, OnGetdispinfoTree)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE, OnItemexpandedTree)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE, OnItemexpandingTree)
	ON_NOTIFY(TVN_KEYDOWN, IDC_TREE, OnKeydownTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE, OnSelchangingTree)
	ON_NOTIFY(TVN_SETDISPINFO, IDC_TREE, OnSetdispinfoTree)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnDblclkTree)
	ON_NOTIFY(NM_OUTOFMEMORY, IDC_TREE, OnOutofmemoryTree)
	ON_NOTIFY(NM_RCLICK, IDC_TREE, OnRclickTree)
	ON_NOTIFY(NM_RDBLCLK, IDC_TREE, OnRdblclkTree)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXIT, OnUpdateFileExit)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEAS, OnUpdateFileSaveas)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_NODE_INSERTFILE, OnNodeInsertFile)
	ON_UPDATE_COMMAND_UI(ID_NODE_INSERTFILE, OnUpdateNodeInsertFile)
	ON_COMMAND(ID_NODE_NEW, OnNodeNew)
	ON_UPDATE_COMMAND_UI(ID_NODE_NEW, OnUpdateNodeNew)
	ON_COMMAND(ID_NODE_RENAME, OnNodeRename)
	ON_UPDATE_COMMAND_UI(ID_NODE_RENAME, OnUpdateNodeRename)
	ON_UPDATE_COMMAND_UI(ID_NODE_SAVEAS, OnUpdateNodeSaveAs)
	ON_COMMAND(ID_NODE_SAVEAS, OnNodeSaveAs)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REFRESH, OnUpdateViewRefresh)
	ON_COMMAND(ID_FIELD_EDIT, OnFieldEdit)
	ON_UPDATE_COMMAND_UI(ID_FIELD_EDIT, OnUpdateFieldEdit)
	ON_COMMAND(ID_NODE_EXPANDFIELDS, OnNodeExpandfields)
	ON_UPDATE_COMMAND_UI(ID_NODE_EXPANDFIELDS, OnUpdateNodeExpandfields)
	ON_COMMAND(ID_VIEW_DEFED_OBJECTS, OnViewDefedObjects)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DEFED_OBJECTS, OnUpdateViewDefedObjects)
	ON_COMMAND(ID_VIEW_TREE, OnViewTree)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TREE, OnUpdateViewTree)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_REFERENCES, OnViewReferences)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REFERENCES, OnUpdateViewReferences)
	ON_COMMAND(ID_NODE_CONVERTTO_INDEXEDFACESET, OnNodeConvertToIndexedFaceset)
	ON_UPDATE_COMMAND_UI(ID_NODE_CONVERTTO_INDEXEDFACESET, OnUpdateNodeConvertToIndexedFaceset)
	ON_COMMAND(ID_NODE_CONVERTTO_SEPARATOR, OnNodeConvertToSeparator)
	ON_UPDATE_COMMAND_UI(ID_NODE_CONVERTTO_SEPARATOR, OnUpdateNodeConvertToSeparator)
	ON_COMMAND(ID_FIELD_EDIT_AS_FILENAME, OnFieldEditAsFilename)
	ON_UPDATE_COMMAND_UI(ID_FIELD_EDIT_AS_FILENAME, OnUpdateFieldEditAsFilename)
	ON_COMMAND(ID_FIELD_EDIT_AS_COLOR, OnFieldEditAsColor)
	ON_UPDATE_COMMAND_UI(ID_FIELD_EDIT_AS_COLOR, OnUpdateFieldEditAsColor)
	ON_COMMAND(ID_FIELD_DRAG, OnFieldDrag)
	ON_UPDATE_COMMAND_UI(ID_FIELD_DRAG, OnUpdateFieldDrag)
	ON_COMMAND(ID_NODE_EXPANDINLINES, OnNodeExpandInlines)
	ON_UPDATE_COMMAND_UI(ID_NODE_EXPANDINLINES, OnUpdateNodeExpandInlines)
	ON_COMMAND(ID_OPTIONS_UPDATEINLINEBOUNDINGBOXES, OnOptionsUpdateinlineboundingboxes)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_UPDATEINLINEBOUNDINGBOXES, OnUpdateOptionsUpdateinlineboundingboxes)
	ON_COMMAND(ID_OPTIONS_WRITEINLINES, OnOptionsWriteinlines)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_WRITEINLINES, OnUpdateOptionsWriteinlines)
	ON_COMMAND(ID_EDIT_PASTE_into, OnEditPasteInto)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_into, OnUpdateEditPasteInto)
	ON_COMMAND(ID_FIELD_EDIT_AS_STRING, OnFieldEditAsString)
	ON_UPDATE_COMMAND_UI(ID_FIELD_EDIT_AS_STRING, OnUpdateFieldEditAsString)
	ON_COMMAND(ID_EDIT_PASTE_COPY, OnEditPasteCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_COPY, OnUpdateEditPasteCopy)
	ON_COMMAND(ID_NODE_CONVERTTO_ANCHOR, OnNodeConverttoAnchor)
	ON_COMMAND(ID_NODE_CONVERTTO_TRANSFORM, OnNodeConvertToTransform)
	ON_COMMAND(ID_NODE_UPDATE_BBOX, OnNodeUpdateBbox)
	ON_UPDATE_COMMAND_UI(ID_NODE_UPDATE_BBOX, OnUpdateNodeUpdateBbox)
	ON_COMMAND(ID_NODE_COMBINE, OnNodeCombine)
	ON_UPDATE_COMMAND_UI(ID_NODE_COMBINE, OnUpdateNodeCombine)
	ON_COMMAND(ID_NODE_EXPANDBBOX, OnNodeExpandbbox)
	ON_UPDATE_COMMAND_UI(ID_NODE_EXPANDBBOX, OnUpdateNodeExpandbbox)
	ON_COMMAND(ID_NODE_TRANSFORM, OnNodeTransform)
	ON_UPDATE_COMMAND_UI(ID_NODE_TRANSFORM, OnUpdateNodeTransform)
	ON_COMMAND(ID_NODE_NORMALIZE, OnNodeNormalize)
	ON_UPDATE_COMMAND_UI(ID_NODE_NORMALIZE, OnUpdateNodeNormalize)
	ON_COMMAND(ID_NODE_OPTIMIZE, OnNodeOptimize)
	ON_UPDATE_COMMAND_UI(ID_NODE_OPTIMIZE, OnUpdateNodeOptimize)
	ON_COMMAND(ID_SETTINGS_OPTIMIZER, OnSettingsOptimizer)
	ON_COMMAND(ID_NODE_CONVEXHULL, OnNodeConvexHull)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_NODE_CHECK, OnNodeCheck)
	ON_UPDATE_COMMAND_UI(ID_NODE_CHECK, OnUpdateNodeCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




int DTreeEditor::ExpandRoot() 
{
	
  m_tree.Expand( GetRoot(), TVE_EXPAND );
  return(0);
}

int DTreeEditor::SetSelection(GvPath &path, int pathIndex)
{
	int i=0;

	HTREEITEM hParent = GetRoot();
 	DGvTreeItem current(m_tree);

	DGvTreeItem first(m_tree);

	i=0; // assume root is ok 

	while (i<=pathIndex &&  hParent != NULL ) {

		 current.Set(hParent);

		 m_tree.Expand( hParent, TVE_EXPAND );
		 

		 int hasFields = m_tree.ItemHasChildren(hParent);

		 if (!hasFields && current.type == DGvTreeItem::GV_NODE &&  current.node != NULL) {
		     InsertNodeFields(current.item,current.node);  // fieldsWithDefaults,int fieldTypes)
			 m_tree.Expand( hParent, TVE_EXPAND );
		 }


		 {

			// find current field & node in children  
			HTREEITEM newItem;
	   		
			first.Set(newItem = m_tree.GetChildItem(hParent));

			while (newItem != NULL) {
				switch (first.type) {
				case  DGvTreeItem::FIELD_ITEM :
					if ((i==0) && path.path[i].field == (GvField*) first.node) {
						hParent = newItem;
						i++;
						goto break_loop;
					}
					// item is the root field  of current node to be searched
					if ((i>0) && path.path[i-1].field == (GvField*) first.node) {
						hParent = newItem;
						goto break_loop;
						break;
					}
					break;

				case  DGvTreeItem::GV_NODE :
				case  DGvTreeItem::GV_GROUP_NODE :

					if (path.path[i].node == first.node) { // found the node 
						hParent = newItem;

						i++;
						if (i >= pathIndex) { // at end of path, set selection 
							m_tree.SelectItem(hParent);
						}

						goto break_loop;
					}
					break;
				default : break;
				}

				newItem = m_tree.GetNextSiblingItem(first.item);
				first.Set(newItem);
			}
			TRACE("Not found \n");
			break;
break_loop:	  ;

		 }
	} // while 

	return(0);
}

// refresh node , by reinserting fields
int DTreeEditor::UpdateNode(GvPath &path, int pathIndex)
{
	int i=0;

	HTREEITEM hParent = GetRoot();
 	DGvTreeItem current(m_tree);

	DGvTreeItem first(m_tree);


	i=0; // assume root is ok 

	while (i<=pathIndex &&  hParent != NULL ) {

		 current.Set(hParent);
		 int hasFields = m_tree.ItemHasChildren(hParent);

		 if (!hasFields) break; // nothing to update, no yet expanded 

		 {

			// find current field & node in children  
			HTREEITEM newItem;
	   		
			first.Set(newItem = m_tree.GetChildItem(hParent));

			while (newItem != NULL) {
				switch (first.type) {
				case  DGvTreeItem::FIELD_ITEM :
					if ((i==0) && path.path[i].field == (GvField*) first.node) {
						hParent = newItem;
						i++;
						goto break_loop;
					}
					// item is the root field  of current node to be searched
					if ((i>0) && path.path[i-1].field == (GvField*) first.node) {
						hParent = newItem;
						goto break_loop;
						break;
					}
					break;

				case  DGvTreeItem::GV_NODE :
				case  DGvTreeItem::GV_GROUP_NODE :

					if (path.path[i].node == first.node) { // found the node 
						hParent = newItem;

						i++;
						if (i >= pathIndex) { // at end of path, set selection 
			 		        InsertNodeFields(first.item,first.node);  // fieldsWithDefaults,int fieldTypes)
						}

						goto break_loop;
					}
					break;
				default : break;
				}

				newItem = m_tree.GetNextSiblingItem(first.item);
				first.Set(newItem);
			}
			TRACE("Not found \n");
			break;
break_loop:	  ;

		 }
	} // while 

	return(0);

}


HTREEITEM DTreeEditor::AddItem(HTREEITEM hParent,const char *text,
			void * param,HTREEITEM hInsertAfter,int expand,int type)
{	
	TV_INSERTSTRUCT TreeCtrlItem;

   	TreeCtrlItem.hParent = hParent;
	TreeCtrlItem.hInsertAfter = hInsertAfter;

	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TreeCtrlItem.item.pszText = (char *)text; // hope this is save
	
	TreeCtrlItem.item.lParam = (long)param;

	TreeCtrlItem.item.iImage = image[type];
	TreeCtrlItem.item.iSelectedImage = selectedImage[type];

    HTREEITEM hTreeRoot = m_tree.InsertItem(&TreeCtrlItem);
    
    //HTREEITEM hTreeRoot = Add(key,TreeCtrlItem);

	if (expand) m_tree.Expand( hTreeRoot, TVE_EXPAND );

	return(hTreeRoot);
}

BOOL DTreeEditor::DeleteItem(HTREEITEM item) 
{
	return(m_tree.DeleteItem(item));
}

// Expand and display node & children
int DTreeEditor::InsertNode(HTREEITEM hParent,HTREEITEM hLocation,GvNode *node,HTREEITEM &hReturn)
{

    hReturn = NULL;
	if (node == NULL) return(0);

    int doRef = 0;
	
	// prevent unnecessary name generation 
//	if (node == copyBuffer)
//		if (node->refCnt >1) { node->unref(); doRef = 1; }

	// Update tree data 
	GDisplayTreeTraversal traversal;

	traversal.top = hParent;
	traversal.topInsertAfter = hLocation;
	traversal.treeEditor = this;
//	int displayFields;
//	int displayChilds;
//	int expandInlines;

		
	traversal.InitializeAttributes();
	
	int ret = node->Traverse(traversal);

	hReturn = traversal.returnItem;

	if (doRef) node->ref();	// refback for copybuffer

    return(ret);
}

// Expand and display node & children
int DTreeEditor::UpdateNode(HTREEITEM hParent,HTREEITEM hLocation,GvNode *node,HTREEITEM &hReturn)
{
  return(0);
}


// delete direct childs of type field 
int DTreeEditor::DeleteFieldItems(HTREEITEM hParent)
{
	   int hasFields = m_tree.ItemHasChildren(hParent);
	   if (hasFields) {
	   		DGvTreeItem first(m_tree);
	   		first.Set(m_tree.GetChildItem(hParent));

			while (first.item != NULL) { // first.type == DGvTreeItem::FIELD_ITEM) {
			  	HTREEITEM newItem = m_tree.GetNextSiblingItem(first.item);
			  	m_tree.DeleteItem(first.item);
			  	first.Set(newItem);

			}
	   }
	   return(0);
}


// Insert an MFNode childs below hParent
int DTreeEditor::Insert(HTREEITEM hParent,GvMFNode *fn)
{
	GDisplayTreeTraversal traversal(refDict);
    traversal.treeEditor = this; // target for inserting items
	traversal.top = hParent;
	// mode ....
		
	traversal.InitializeAttributes();

    int ret = fn->Do(traversal);
    return(ret);
}


int DTreeEditor::Insert(HTREEITEM hParent,GvGroup *fn)
{
	GDisplayTreeTraversal traversal(refDict);
    traversal.treeEditor = this; // target for inserting items
	traversal.top = hParent;
	// mode ....
		
	traversal.InitializeAttributes();

    int ret = fn->TraverseChildren(traversal);
    return(ret);
}


// Expand and display fields of node
int DTreeEditor::InsertNodeFields(HTREEITEM hParent,GvNode *node,int fieldsWithDefaults,int fieldTypes)
{	int cnt=0;

	GvBool isGroup = viewCompact && RTISA(node,Gv2Group);

	DeleteFieldItems(hParent);

    GvFieldData *fields = node->getFieldData();

	if (fields == NULL) return(0);
    if (!fieldsWithDefaults) {
        if (RTISA(node,GvScript)) fieldsWithDefaults = 1;
        else if (RTISA(node,GvProto)) fieldsWithDefaults = 1;
    }
	
	int allEvents = fieldsWithDefaults; // display all events 


	HTREEITEM lastItem = TVI_FIRST;

    for (int i = 0; i < fields->getNumFields(); i++) {

	

//	    GvField *f = fields->getField(node, i);
	    
        GvField *f = node->getField(i);
        GvFieldEntry *fieldEntry = fields->getFieldEntry(i);

		CString buf;

		const GvName &fni = fieldEntry->getName();
        const char *type = fieldEntry->getType();
        const char *fieldType = fieldEntry->getFieldType();

		if (!allEvents) {
			// ignore certain entrys 
			switch (fieldEntry->type) {
			case GV_NODE_EVENT_IN :
					continue;
					break;
			case GV_FIELD_EVENT_IN:
			case GV_FIELD_EVENT_OUT:
					
					if (f && !f->hasConnections())
						continue;

					break;
			default :
					break;
			}
		}


        if (f == NULL || fieldEntry->type ==GV_NODE_EVENT_IN) {

            // Proto interface description
           	buf += fieldType;
            buf += " ";
			buf += type;
            buf += " ";
			buf += fni;

            GvField *fieldDefault = fieldEntry->getDefault();

            // write default value

            if (fieldDefault) {

                // write field value
			    if (RTISA(fieldDefault,GvSField)) {
				    GvString val;
				    fieldDefault->get(val);
				    buf += " ";
				    buf += val;
			    }
			    else if (RTISA(fieldDefault,GvMField)) {
				    GvMField *mf= (GvMField *) fieldDefault;
				    if (mf->Length() <= 4) {
					    GvString val;
					    fieldDefault->get(val);
					    buf += " ";
					    buf += val;
				    }
				    else buf += " [ ....]";
			    }
            }


			HTREEITEM item = AddItem(hParent,buf,f,lastItem,0,DGvTreeItem::FIELD_ITEM);

            if (fieldDefault) {
			if (RTISA(fieldDefault,GvMFNode)) {
			    Insert(item,(GvMFNode *) fieldDefault); 
			} else 
			if (RTISA(fieldDefault,GvSFNode)) {
			    GvSFNode *fn = (GvSFNode *) fieldDefault; 
            
                HTREEITEM newItem;                
                int ret = InsertNode(item,TVI_LAST,fn->value,newItem);
			}
            }
			lastItem=item;
			cnt ++;


        } // f == NULL
		else {
		if (fieldsWithDefaults || !f->isDefault()) {
			
            // write field name
            buf += fni;
	    
			// write field value
			if (RTISA(f,GvSField)) {
				GvString val;
				f->get(val);
				buf += " ";
				buf += val;
			}
			else if (RTISA(f,GvMField)) {
				GvMField *mf= (GvMField *) f;

				if (RTISA(mf,GvMFNode)) {

					if (viewCompact && isGroup) continue; // add as direct childs 

				} else 
				if (mf->Length() <= 4) {
					GvString val;
					f->get(val);
					buf += " ";
					buf += val;
				}
				else buf += "[ ....]";


			}

			buf += " # ";
			buf += fieldType;
            buf += " ";

			buf += type;

	    	//f->Do(state);
			HTREEITEM item = AddItem(hParent,buf,f,lastItem,0,DGvTreeItem::FIELD_ITEM);

			if (RTISA(f,GvMFNode)) {
			    Insert(item,(GvMFNode *) f); 
			} else 
			if (RTISA(f,GvSFNode)) {
			    GvSFNode *fn = (GvSFNode *) f; 
	            
                HTREEITEM newItem;                
                int ret = InsertNode(item,TVI_LAST,fn->value,newItem);
			}

			lastItem=item;
			cnt ++;
		}
        } // else f = NULL
	}

	if (viewCompact && isGroup && RTISA(node,Gv2Group)) {
	    Gv2Group * n = (Gv2Group *) node;
	    Insert(hParent, &n->children); 

	} else
	if (RTISA(node,GvProtoInstance)) {
	    GvProtoInstance * n = (GvProtoInstance *) node;
        GvField *f;
		CString buf;
	    // insert hidden fields 
		f = &n->children;
		buf = "instance";
		HTREEITEM item = AddItem(hParent,buf,f,lastItem,0,DGvTreeItem::FIELD_ITEM);
	    Insert(item,&n->children); 

		lastItem = item; cnt ++;
		f = NULL;

		buf = "PROTO ";
		item = AddItem(hParent,buf,f,lastItem,0,DGvTreeItem::FIELD_ITEM);

        HTREEITEM newItem;                
		InsertNode(item,TVI_LAST,(GvNode*) (n->proto),newItem);

		lastItem = item; cnt ++;
 
	}
	else if (RTISA(node,GvExternProto)) { // URL field of proto
	    GvExternProto * n = (GvExternProto *) node;
        GvField *f;
		CString buf;
	    // insert hidden fields 
		f = &n->url;
		buf = "URL";
		{
			GvString val;
			f->get(val);
			buf += " ";
			buf += val;
		}

		HTREEITEM item = AddItem(hParent,buf,f,lastItem,0,DGvTreeItem::FIELD_ITEM);
	    // Insert(item,&n->children); 

		lastItem = item; cnt ++;
		f = NULL;

		buf = "DEFINING PROTO";
		item = AddItem(hParent,buf,f,lastItem,0,DGvTreeItem::FIELD_ITEM);

        HTREEITEM newItem;                
		InsertNode(item,TVI_LAST,(GvNode*) (n->proto),newItem);

		lastItem = item; cnt ++;


	}
	else if (RTISA(node,GvGroup)) { // group children
	    GvGroup * n = (GvGroup *) node;
		Insert(hParent,n);
	}


	return(cnt);
}

//int DTreeEditor::UpdateField(HTREEITEM hParent,HTREEITEM hLocation,GvField *field,HTREEITEM &hReturn)
//{
//}



/////////////////////////////////////////////////////////////////////////////
// DTreeEditor message handlers
#if 0
void DTreeEditor::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void DTreeEditor::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
#endif

BOOL DTreeEditor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_imagelist.Create(IDB_BITMAP_TREEVIEW, 16, 1, RGB(255,255,255));
	m_tree.SetImageList(&m_imagelist,TVSIL_NORMAL); 

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Tree control messages

void DTreeEditor::OnBegindragTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	HTREEITEM hti = pNMTreeView->itemNew.hItem; //Handle Tree Item

	m_dragImage = m_tree.CreateDragImage(hti);
	
	m_tree.SelectItem(hti);	

	m_Source = hti;
	m_Target = NULL;
	
	// Compute hot-spot
	CRect rItem;
	m_tree.GetItemRect( hti, &rItem, TRUE );

	// Begin drag operation
	CPoint pt(pNMTreeView->ptDrag);
	CSize tmp(rItem.TopLeft());
	pt -= tmp; 
	if (m_dragImage) {
	   m_dragImage->BeginDrag( 0, pt );
	   m_dragImage->DragEnter( GetDlgItem(IDC_TREE), pt );
	}

	SetCapture();
	//::ShowCursor(FALSE); 
	
	m_draging = TRUE;

	
	*pResult = 0;
}

void DTreeEditor::OnBeginlabeleditTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void DTreeEditor::OnDeleteitemTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void DTreeEditor::OnEndlabeleditTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
#if 0
	// TODO: Add your control notification handler code here
	HTREEITEM hti = pNMTreeView->item.hItem; //Handle Tree Item
	long param = pNMTreeView->item.lParam;
	
	if (pNMTreeView->item.pszText == NULL)  return; // edit canceled 
#endif
	
	*pResult = 0;
}

void DTreeEditor::OnGetdispinfoTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void DTreeEditor::OnItemexpandedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void DTreeEditor::OnItemexpandingTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

#if 0
// test  dosn´t work
	int ok=0;

	if  (pNMTreeView->action == TVE_TOGGLE /*EXPAND*/) {
		DGvTreeItem item(m_tree);
		item.Set(pNMTreeView->itemNew.hItem);
	
		if (item.type >= DGvTreeItem::GV_NODE) {
	   		int hasFields = m_tree.ItemHasChildren(item.item);
	   		if (hasFields) {
	   			DGvTreeItem first(m_tree);
	   			first.Set(m_tree.GetChildItem(item.item));
				if (first.type != DGvTreeItem::FIELD_ITEM) hasFields = 0;
	   	}
	   
	   	if (!hasFields) {
	     	InsertNodeFields(item.item,item.node);  // fieldsWithDefaults,int fieldTypes)
	     //m_tree.Expand(item.item,TVE_EXPAND); 
	   	}
	}
	
	}
#endif
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void DTreeEditor::OnKeydownTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	TV_KEYDOWN* pNMTreeView = (TV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void DTreeEditor::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	// is this needed ? 
	selection.Set(GetSelection()); // ?? ok
	
	*pResult = 0;
}

void DTreeEditor::OnSelchangingTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void DTreeEditor::OnSetdispinfoTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void DTreeEditor::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

#if 1
	int ok=0;
	DGvTreeItem item(m_tree);
	item.Set(GetSelection());
	DGvTreeItem owner(m_tree);
	owner.SetToOwner(item);


	if (item.type >= DGvTreeItem::GV_NODE) {
       // insert any fields  
	   int hasFields = m_tree.ItemHasChildren(item.item);
	   if (hasFields) {
	   		DGvTreeItem first(m_tree);
	   		first.Set(m_tree.GetChildItem(item.item));
			if (first.type != DGvTreeItem::FIELD_ITEM) hasFields = 0;
	   }
	   
	   if (!hasFields) {
	     InsertNodeFields(item.item,item.node);  // fieldsWithDefaults,int fieldTypes)
	     //m_tree.Expand(item.item,TVE_EXPAND); 
	   }
	} else 
	if (item.type == DGvTreeItem::FIELD_ITEM)
	{
		GvField *f = (GvField *) item.node;

		if (RTISA(f,GvMField)) {  // expand MField elements
				if (!RTISA(f,GvMFNode)) { // MFNode already expanded 
	
				GvMField *mf = (GvMField *) f;

                if (mf->Length() == 1) {
//                    OnFieldEdit();
                    PostMessage(WM_COMMAND,ID_FIELD_EDIT);
                } else {
			
				// Delete all items
				DGvTreeItem first(m_tree);
	   			first.Set(m_tree.GetChildItem(item.item));

				while (first.type == DGvTreeItem::MFIELD_ELEMENT) {
			  		HTREEITEM newItem = m_tree.GetNextSiblingItem(first.item);
			  		m_tree.DeleteItem(first.item);
			  		first.Set(newItem);
			    }

				HTREEITEM lastItem = TVI_FIRST;

				for(int i=0; i< mf->num; i++) {
				   GvString val;
				   CString buf;
				   mf->get1(val,i);
				   
				   buf.Format("%s #%d",(const char *)val,i+1);
				   // to do:: set MFIELD_ELEMENT_INDEX as value
				   HTREEITEM newItem = AddItem(item.item,buf,(void *) i,lastItem,0,DGvTreeItem::MFIELD_ELEMENT);
	   			   lastItem=newItem;

				   if (i>1000) break; // limit

				}
                }
				}
		} //if (RTISA(f,GvMField))
        else  {
            if (!RTISA(f,GvSFNode)) 
                PostMessage(WM_COMMAND,ID_FIELD_EDIT);
            
//            OnFieldEdit();
        }
	
	} // item.type == DGvTreeItem::FIELD_ITEM
	else if (item.type == DGvTreeItem::MFIELD_ELEMENT)
	{
             PostMessage(WM_COMMAND,ID_FIELD_EDIT);
	}

#endif

*pResult = 0;
}

void DTreeEditor::OnOutofmemoryTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void DTreeEditor::OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	DGvTreeItem item(m_tree);
	item.Set(GetSelection());
	CPoint point(::GetMessagePos());

		//ClientToScreen(&point);
		CMenu menu;

		if (menu.LoadMenu(IDR_TREE_EDITOR_POPUP))
		{
			CMenu* pPopup=NULL;


			CMenu menu1;
			CMenu* subMenu;

			if (menu1.LoadMenu(IDR_MENU_TREE_EDITOR)) {

			if (item.type >= DGvTreeItem::GV_NODE) {
				pPopup = menu.GetSubMenu(0);
				CMenu* subMenu = menu1.GetSubMenu(2);
				pPopup->AppendMenu(MF_POPUP,(UINT) subMenu->GetSafeHmenu(), "Edit");
				subMenu = menu1.GetSubMenu(3);
				if (subMenu) pPopup->AppendMenu(MF_POPUP,(UINT) subMenu->GetSafeHmenu(), "Node");
			}
			else if (item.type == DGvTreeItem::FIELD_ITEM
				) {
				pPopup = menu.GetSubMenu(1);

				subMenu = menu1.GetSubMenu(4);
				if (subMenu) pPopup->AppendMenu(MF_POPUP,(UINT) subMenu->GetSafeHmenu(), "Node");
			}

			if (pPopup)
			   pPopup->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN,
								   point.x, point.y,
								   this); // route commands through dialog 
			}

		}


	
	*pResult = 0;
}
#if 0
const MSG* PASCAL CWnd::GetCurrentMessage()
{
	// fill in time and position when asked for
	_AFX_THREAD_STATE* pThreadState = _afxThreadState.GetData();
	pThreadState->m_lastSentMsg.time = ::GetMessageTime();
	pThreadState->m_lastSentMsg.pt = CPoint(::GetMessagePos());
	return &pThreadState->m_lastSentMsg;
}

LRESULT CWnd::Default()
#endif


void DTreeEditor::OnRdblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int ok=0;
	DGvTreeItem item(m_tree);
	item.Set(GetSelection());
	if (item.type >= DGvTreeItem::GV_NODE) {
	   int hasFields = m_tree.ItemHasChildren(item.item);
	   if (hasFields) {
	   		DGvTreeItem first(m_tree);
	   		first.Set(m_tree.GetChildItem(item.item));
			if (first.type != DGvTreeItem::FIELD_ITEM) hasFields = 0;
	   }
	   
	   if (!hasFields) {
	     InsertNodeFields(item.item,item.node);  // fieldsWithDefaults,int fieldTypes)
	     m_tree.Expand(item.item,TVE_EXPAND); 
	   }
	}

	
	*pResult = 0;
}

void DTreeEditor::OnEditDelete() 
{
	// TODO: Add your command handler code here
	OnEditCut();
	
}

void DTreeEditor::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

GView *DTreeEditor::GetView() { return m_pView->view; }
GvScene *DTreeEditor::GetScene() { return m_pView->view->GetScene(); }

int DTreeEditor::DoCut(DGvTreeItem &owner,DGvTreeItem &item)
{
	int ok=0;
	if (item.type >= DGvTreeItem::GV_NODE) { // item is a node 

        m_controller->setCopyBufferEx(item.node);
		
		if (RTISA(item.node,GvRoute)) {
			GvScene *scene = GetScene();
			if (scene) scene->deleteRoute((GvRoute*) item.node);
		}
		else if (RTISA(item.node,GvViewpoint)) {
			GView *view = GetView();
			view->doUpdateSceneInfo++;
		}
		
		if  (owner.type >= DGvTreeItem::GV_GROUP_NODE) {
			GvGroup *group= (GvGroup*) owner.node;
			if (item.nodeChildIndex>=0) {
				//copyBuffer = group->getChild(item.nodeChildIndex);
			    group->deleteChild(item.nodeChildIndex);
				m_tree.SelectItem(m_tree.GetParentItem(item.item));
				DeleteItem(item.item); item.item = NULL;
				ok=1;
			}
		}
		else 
		if  (owner.type == DGvTreeItem::FIELD_ITEM
			 || (owner.type >= DGvTreeItem::GV_NODE && owner.field != NULL)			
			) {
			
			GvField *f = (GvField *) owner.field;

			if (RTISA(f,GvSFNode)) {
				GvSFNode *n = (GvSFNode *) f;
          		//copyBuffer = n->value;
				n->set((GvNode*)NULL);
				DeleteItem(item.item); item.item = NULL;
				ok=1;
			}
			else 
			if (RTISA(f,GvMFNode)) {
				GvMFNode *field = RTCAST(f,GvMFNode);
				if (field && item.nodeChildIndex>=0) {
					//copyBuffer = field->get1(item.nodeChildIndex);
					field->deleteElement(item.nodeChildIndex);
					field->setDefault(FALSE);
					field->OnChanged();

					m_tree.SelectItem(m_tree.GetParentItem(item.item));

					DeleteItem(item.item); item.item = NULL;
					ok=1;
				}
			}
		} // field item 

	}
   	else if (item.type == DGvTreeItem::FIELD_ITEM)
	{
		GvField *f = (GvField *) item.node;
        if (RTISA(f,GvSFNode)) {
            GvSFNode *n = (GvSFNode *) f;
          	m_controller->setCopyBufferEx(n->value);
            n->set((GvNode*)NULL);
            ok=1;
		   	// update field
            if (owner.type >= DGvTreeItem::GV_NODE) {
	     	    InsertNodeFields(owner.item,owner.node,0);  // fieldsWithDefaults,int fieldTypes)
	     		    m_tree.Expand(owner.item,TVE_EXPAND); 
			}
        }            
        else if (RTISA(f,GvMFNode)) {
            GvMFNode *n = (GvMFNode *) f;
			m_controller->setCopyBuffer(*n);
			n->makeRoom(0);
		   	// update field
            if (owner.type >= DGvTreeItem::GV_NODE) {
	     	    InsertNodeFields(owner.item,owner.node,0);  // fieldsWithDefaults,int fieldTypes)
	     		    m_tree.Expand(owner.item,TVE_EXPAND); 
			}
            ok=1;
        }            
        else if (RTISA(f,GvMField)) {
            GvMField *n = (GvMField *) f;
			//m_controller->setCopyBuffer(n);
			n->makeRoom(0);
		   	// update field
            if (owner.type >= DGvTreeItem::GV_NODE) {
	     	    InsertNodeFields(owner.item,owner.node,0);  // fieldsWithDefaults,int fieldTypes)
	     		    m_tree.Expand(owner.item,TVE_EXPAND); 
			}
            ok=1;
        }            
    }
	return(ok);

}

void DTreeEditor::OnEditCut() 
{
	int ok=0;
	
	DGvTreeItem item(m_tree);
	item.Set(GetSelection());
	DGvTreeItem owner(m_tree);
	// initialize by the owner of child
	owner.SetToOwner(item);

	ok = DoCut(owner,item);


	if (!ok) Reporter.Warning("Can´t do cut operation on this object");
	else OnChanged();
}


void DTreeEditor::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here

	
}

// do low level paste 
int DTreeEditor::DoPaste(DGvTreeItem &owner,DGvTreeItem &item,GvNode *__copyBuffer,int replace)
{   int ok=0;

	int incRef = 0;

	// temporary decrement of refcnt, no new node name needed
	GvNode *buffer=NULL;

	if (__copyBuffer != NULL) buffer = __copyBuffer;
	else buffer = m_controller->getCopyBufferAsNode();

	
	if (buffer && buffer->refCnt >1) { buffer->unref(); incRef = 1 ; }


	if (item.type >= DGvTreeItem::GV_NODE)	{

		if  (owner.type >= DGvTreeItem::GV_GROUP_NODE) {
			GvGroup *group= (GvGroup*) owner.node;
			if (item.nodeChildIndex>=0) {
			    
				int where = item.nodeChildIndex+1;				

				group->insertChild(where,buffer);

				// Expand and display node & children
				HTREEITEM newItem;
				int ret = InsertNode(owner.item,item.item,buffer,newItem);
				m_tree.Select(newItem,TVGN_CARET );
				if (replace) {
				    DeleteItem(item.item); item.item = NULL;
				}
	

				ok=1;
			}

		} // owner is VRML 1 group node
		else 
		if  (owner.type == DGvTreeItem::FIELD_ITEM) {
			GvField *f = (GvField *) owner.node;

			if (RTISA(f,GvSFNode)) {
				GvSFNode *n = (GvSFNode *) f;
          	
				n->set(buffer);
				HTREEITEM childItem = m_tree.GetChildItem(owner.item);
				if (childItem) DeleteItem(childItem);

				HTREEITEM newItem;
				int ret = InsertNode(owner.item,TVI_FIRST,buffer,newItem);
				m_tree.Select(newItem,TVGN_CARET );
				item.Set(newItem);
				ok=1;
			}
			else 
			if (RTISA(f,GvMFNode)) {
				
				if (buffer && RTISA(buffer,GvNodeShell)) {
					// & owner is group node 
					if (incRef) { buffer->ref(); incRef = 0; }

					// wrap into shape node 
					GvShape *s = new GvShape();
					s->geometry.set(buffer);
					s->appearance.set(m_controller->defaultAppearance);
					m_controller->setCopyBufferEx(s);

					buffer = s;
				}

				GvMFNode *field = RTCAST(f,GvMFNode);
				if (field && item.nodeChildIndex>=0) {
					if (replace) 
						field->set1(item.nodeChildIndex,buffer);
					else { 	
						field->insert(item.nodeChildIndex+1,buffer);
						field->setDefault(FALSE); // not handled by insert 
						field->OnChanged();
					}

					// Expand and display node & children
					HTREEITEM newItem;
					int ret = InsertNode(owner.item,item.item,buffer,newItem);
					m_tree.Select(newItem,TVGN_CARET );
					if (replace) {
						DeleteItem(item.item); item.item = NULL;
					}

					ok=1;
				}
			}
		} // owner is field item 
		else 
		if (item.type >= DGvTreeItem::GV_GROUP_NODE) {   // add to group
			    GvGroup *group= (GvGroup*) item.node;
				
				group->addChild(buffer);
			    // Expand and display node & children
				HTREEITEM newItem;
				int ret = InsertNode(item.item,TVI_LAST,buffer,newItem);
				m_tree.Select(newItem,TVGN_CARET );

				ok=1;
		}
		else if (item.type == DGvTreeItem::GV_NODE && RTISA(item.node,Gv2Group)) {   // add to group
			    Gv2Group *group= (Gv2Group*) item.node;
				//tbd : check 
				group->children.add(buffer);
			    // Expand and display node & children
				HTREEITEM newItem;
				int ret = InsertNode(item.item,TVI_LAST,buffer,newItem);
				m_tree.Select(newItem,TVGN_CARET );

				ok=1;
		}
	 } // item is node
	 else 	
     if (item.type == DGvTreeItem::FIELD_ITEM) {
            GvField *f = (GvField *) item.node;

		    if (RTISA(f,GvSFNode)) {	// Paste into SFNode field  
                GvSFNode *nf = (GvSFNode *) f;

                nf->set(buffer);
				
				HTREEITEM childItem = m_tree.GetChildItem(item.item);
				if (childItem) DeleteItem(childItem);

				HTREEITEM newItem;
				int ret = InsertNode(item.item,TVI_FIRST,buffer,newItem);
				m_tree.Select(newItem,TVGN_CARET );

 		  	    ok = 1;
            }
			else 
			if (RTISA(f,GvMFNode)) { // Paste into MFNode field  
				GvMFNode *field = RTCAST(f,GvMFNode);

				if (field) {
					field->insert(0,buffer);
					field->setDefault(FALSE); // not handled by insert 
					field->OnChanged();

				    // Expand and display node & children
					HTREEITEM newItem;
					int ret = InsertNode(item.item,TVI_FIRST,buffer,newItem);
					m_tree.Select(newItem,TVGN_CARET );
					ok=1;
				}
			}
	 } // field 


  if (incRef) { buffer->ref(); }


  return(ok);
}



void DTreeEditor::OnEditPaste() 
{
	int ok=0;
	
	DGvTreeItem item(m_tree);
	item.Set(GetSelection());

	if (m_controller->copyBufferEmpty())  { 
	    Reporter.Error("Nothing to paste");
		return;
	}

	DGvTreeItem owner(m_tree);
	owner.SetToOwner(item);


	ok = DoPaste(owner,item,NULL);

	if (!ok) Reporter.Warning("Can´t do paste operation on this object");
	else OnChanged();
	
}

void DTreeEditor::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_controller->copyBufferEmpty());
}

void DTreeEditor::OnEditPasteCopy() 
{
	int ok=0;
	
	DGvTreeItem item(m_tree);

	item.Set(GetSelection());

	if (m_controller->copyBufferEmpty()) { 
	    Reporter.Error("Nothing to paste");
		return;
	}

	DGvTreeItem owner(m_tree);
	owner.SetToOwner(item);


	GvNodeHandle theCopy = m_controller->copyCopyBuffer();

	ok = (theCopy != NULL);

	if (ok) ok = DoPaste(owner,item,theCopy);

	if (!ok) Reporter.Warning("Can´t do paste operation on this object");
	else OnChanged();
	
}

void DTreeEditor::OnUpdateEditPasteCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_controller->copyBufferEmpty());
}


void DTreeEditor::OnEditPasteInto() 
{
	int ok=0;
	
	DGvTreeItem item(m_tree);

	item.Set(GetSelection());

	if (m_controller->copyBufferEmpty())  { 
	    Reporter.Error("Nothing to paste");
		return;
	}

    DGvTreeItem owner(m_tree);

	GvNodeHandle copyBuffer = m_controller->getCopyBufferAsNode();



	if (item.type >= DGvTreeItem::GV_NODE) {
		owner.SetToOwner(item);
		
		if (item.type >= DGvTreeItem::GV_GROUP_NODE) {   // add to group
			    GvGroup *group= (GvGroup*) item.node;
				group->addChild(copyBuffer);
			    // Expand and display node & children
				HTREEITEM newItem;
				int ret = InsertNode(item.item,TVI_LAST,copyBuffer,newItem);
				m_tree.Select(newItem,TVGN_CARET );
				ok=1;
		} 
		else {
			// could find first MFNode etc
			ok = m_controller->addChild(item.node,copyBuffer);
			OnNodeExpandfields(); // ??
			// update 
		}
    } else 	
    if (item.type == DGvTreeItem::FIELD_ITEM) {
            GvField *f = (GvField *) item.node;

		    if (RTISA(f,GvSFNode)) {
                GvSFNode *nf = (GvSFNode *) f;
                nf->set(copyBuffer);
				
				HTREEITEM childItem = m_tree.GetChildItem(item.item);
				if (childItem) DeleteItem(childItem);

				HTREEITEM newItem;
				int ret = InsertNode(item.item,TVI_FIRST,copyBuffer,newItem);
				m_tree.Select(newItem,TVGN_CARET );
 		  	    ok = 1;
            }
			else 
			if (RTISA(f,GvMFNode)) {
				GvMFNode *field = RTCAST(f,GvMFNode);
				if (field) {
					//copyBuffer = field->get1(item.nodeChildIndex);
					field->append(copyBuffer);
					field->OnChanged();
					// Expand and display node & children
					HTREEITEM newItem;
					int ret = InsertNode(item.item,TVI_LAST,copyBuffer,newItem);
					m_tree.Select(newItem,TVGN_CARET );
					ok=1;
				}
			}


        if (ok) {
	            // update all fields of owner node, to do: optimize
		   	   if (owner.type >= DGvTreeItem::GV_NODE) {
                   //m_tree.Select(owner.item,TVGN_CARET); // child is gone
	     		   //InsertNodeFields(owner.item,owner.node,0);  // fieldsWithDefaults,int fieldTypes)
	     		   //m_tree.Expand(owner.item,TVE_EXPAND);  // got a crash here 
                  // PostMessage(WM_COMMAND,ID_VIEW_REFRESH);
			   }
	    }
	 }
	 
     if (!ok) Reporter.Warning("Can´t do paste into operation on this object");
     else OnChanged();

}

void DTreeEditor::OnUpdateEditPasteInto(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_controller->copyBufferEmpty());
}

int DTreeEditor::DoCopy(DGvTreeItem &owner,DGvTreeItem &item)
{
	int ok=0;
	if (item.type >= DGvTreeItem::GV_NODE) {
		
		m_controller->setCopyBufferEx(item.node);

        ok = 1; // if we have a node pointer, it´s ok 

	}
   	else if (item.type == DGvTreeItem::FIELD_ITEM)
	{
		GvField *f = (GvField *) item.node;
        if (RTISA(f,GvSFNode)) {
            GvSFNode *n = (GvSFNode *) f;
          	m_controller->setCopyBufferEx(n->value);
            ok=1;

        }            
        else if (RTISA(f,GvMFNode)) {
            GvMFNode *n = (GvMFNode *) f;
			m_controller->setCopyBuffer(*n);
            ok=1;
        }            
		// else to do:

    }
	return(ok);
}
	



void DTreeEditor::OnEditCopy() 
{
	int ok=0;
	
	DGvTreeItem item(m_tree);

	item.Set(GetSelection());

	DGvTreeItem owner(m_tree);
	// initialize by the owner of child
	owner.SetToOwner(item);
	ok = DoCopy(owner,item);

	if (!ok) Reporter.Warning("Can´t do copy operation on this object");
}


void DTreeEditor::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void DTreeEditor::OnFileExit() 
{
	// TODO: Add your command handler code here
	OnOK();
}

void DTreeEditor::OnUpdateFileExit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void DTreeEditor::OnUpdateFileSaveas(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void DTreeEditor::OnFileSaveas() 
{
	// TODO: Add your command handler code here
	Reporter.Error("Operation not supported in this version");
	
}


void DTreeEditor::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	HTREEITEM hTarget;
	UINT flags;

	if(m_draging)
	{
							 		
		CPoint pos(point);

		MapWindowPoints(GetDlgItem(IDC_TREE), &pos, 1);

		m_dragImage->DragMove(pos);	

		flags = TVHT_ONITEM;

		if((hTarget = m_tree.HitTest(pos, &flags)) != NULL)
		{			
			m_dragImage->DragLeave(GetDlgItem(IDC_TREE));
			m_tree.SelectDropTarget(hTarget);
		 	m_dragImage->DragEnter(GetDlgItem(IDC_TREE), pos );

			m_Target = hTarget;
		}
		else
			m_Target = NULL;
	}
	
	
	CDialog::OnMouseMove(nFlags, point);
}

int DGvTreeItem::Set(HTREEITEM newItem)
{
  item=newItem;
  type = BASIC_NODE;
  nodeChildIndex = -1;

  if (item == NULL) {
      ownerItem = NULL;
	  node = NULL;
	  field = NULL;
	  return(0);
  }

  		char text[1025];
		TV_ITEM TreeCtrlItem;
		TreeCtrlItem.mask = TVIF_PARAM | TVIF_TEXT;
		TreeCtrlItem.hItem = item;
		TreeCtrlItem.pszText=text;
		TreeCtrlItem.cchTextMax=1024;

		m_tree.GetItem(&TreeCtrlItem);

	    unsigned long param = TreeCtrlItem.lParam;
		if (param >1024) {
		   node = (GvNode *) param;
		   if (RTISA(node,GvField)) { 
			   type = FIELD_ITEM;
			   field = (GvField * )node;
		   }	
		   else if (RTISA(node,GvSeparator)) type = GV_GROUP_NODE;
		   else if (RTISA(node,GvGroup)) type = GV_GROUP_NODE;
		   else if (RTISA(node,GvNode)) type = GV_NODE;
		   else {
			   TRACE("Unknown param type !!!!! \n");
			
			   node = NULL;
				}
		} else {
			node = NULL;
		}
		ownerItem = m_tree.GetParentItem(item);
   return(1);
}

// initialize by the owner of child
int DGvTreeItem::SetToOwner(DGvTreeItem &child)
{
	Set(child.ownerItem);
	if  (type >= DGvTreeItem::GV_GROUP_NODE || (type == DGvTreeItem::GV_NODE)) {
		if (RTISA(node,GvGroup)) {
			GvGroup *group= (GvGroup*) node;
			child.nodeChildIndex = group->findChild(child.node);
			return(child.nodeChildIndex >= 0);
		}
		else if (RTISA(node,Gv2Group)) {
			Gv2Group *group= (Gv2Group*) node;
			field = &group->children;
			child.nodeChildIndex = group->children.find(child.node);
			return(child.nodeChildIndex >= 0);
		}  // GvExternProto
		else if (RTISA(node,GvProto)) {
			GvProto *group= (GvProto*) node;
			field = &group->children;
			child.nodeChildIndex = group->children.find(child.node);
			return(child.nodeChildIndex >= 0);
		}  // GvProto
		else if (RTISA(node,Gv2Switch)) {
			Gv2Switch *group= (Gv2Switch*) node;
			field = &group->choice;
			child.nodeChildIndex = group->choice.find(child.node);
			return(child.nodeChildIndex >= 0);
		}
		else if (RTISA(node,Gv2LOD)) {
			Gv2LOD *group= (Gv2LOD*) node;
			field = &group->level;
			child.nodeChildIndex = group->level.find(child.node);
			return(child.nodeChildIndex >= 0);
		}
	} else 
	if(type == DGvTreeItem::FIELD_ITEM) {
		GvField *f = (GvField *) node;

		if (RTISA(f,GvMField)) {  
			if (RTISA(f,GvMFNode)) {  
				GvMFNode *mf = (GvMFNode *) f;
				child.nodeChildIndex = mf->find(child.node);
				child.field = f;
				return(child.nodeChildIndex >= 0);
			} else {
				GvMField *mf = (GvMField *) f;
				child.type = DGvTreeItem::MFIELD_ELEMENT;
				child.field = f;
				// to do:  find element index
			    child.nodeChildIndex = m_tree.GetItemData(child.item);
			}
		}
		
	}
	return(0);
}



void DTreeEditor::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_draging)
	{
		m_dragImage->DragLeave(GetDlgItem(IDC_TREE));
		m_dragImage->EndDrag();
		
		ReleaseCapture();	
		ShowCursor(TRUE);
		m_draging = FALSE;

		int copy = (nFlags & MK_CONTROL) !=0;
		//int include = (nFlags & MK_SHIFT) !=0;

		int ok=0;
		//
		//
		// we have a m_Target and a m_Source so do something about it here...
		//
		//
		//
		if (m_Target != NULL && m_Target != m_Source) {  // & ~ IsAParent(

		DGvTreeItem target(m_tree);
		target.Set(m_Target);

		DGvTreeItem source(m_tree);
		source.Set(m_Source);


		char text[1024];
		TV_ITEM TreeCtrlItem;
		TreeCtrlItem.mask = TVIF_PARAM | TVIF_TEXT;
		TreeCtrlItem.hItem = m_Target;
		TreeCtrlItem.pszText=text;
		TreeCtrlItem.cchTextMax=1024;

		m_tree.GetItem(&TreeCtrlItem);


		TreeCtrlItem.hItem = m_Source;
		m_tree.GetItem(&TreeCtrlItem);

		ok = (source.type >= DGvTreeItem::GV_NODE) && 
			(target.type >= DGvTreeItem::GV_NODE);


		DGvTreeItem targetOwner(m_tree);
		ok = ok && targetOwner.SetToOwner(target);

		DGvTreeItem sourceOwner(m_tree);
		ok= ok && sourceOwner.SetToOwner(source);


//		Reporter.Warning("%s %s to %s %s:%d",copy ? "copy":"move",source.node->ClassName(),target.node->ClassName(),text,ok);
		
		if (copy) { 

			ok = DoCopy(sourceOwner,source);
			if (ok) {
				 ok = DoPaste(targetOwner,target,NULL);
			}	


		}

		else {  // Move 


			ok = DoCut(sourceOwner,source);

			if (ok) {
	 			 targetOwner.SetToOwner(target); // childNode index might changed 
				 ok = DoPaste(targetOwner,target,NULL);
			}	
		}

		m_tree.SelectDropTarget(NULL); 

	    if (!ok) Reporter.Warning("Can´t do move operation on this object");
		else OnChanged(); // new 

		} // target != source 
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}


void DTreeEditor::OnNodeInsertFile() 
{
	int ok=0;
	
	DGvTreeItem item(m_tree);
	DGvTreeItem owner(m_tree);

	item.Set(GetSelection());
	owner.SetToOwner(item);

	if (item.type >= DGvTreeItem::GV_NODE) {
		if  (owner.type >= DGvTreeItem::GV_GROUP_NODE) {
			if (item.nodeChildIndex>=0) {
				ok=1;
			}
		}
		else 
		if  (owner.type == DGvTreeItem::FIELD_ITEM) {
			GvField *f = (GvField *) owner.node;

			if (RTISA(f,GvSFNode)) {
				GvSFNode *n = (GvSFNode *) f;
        		ok=1;
			}
			else 
			if (RTISA(f,GvMFNode)) {
				GvMFNode *field = RTCAST(f,GvMFNode);
				if (field && item.nodeChildIndex>=0) {
					ok=1;
				}
			}
		} // owner is field item 
	 } // item is node
	 else 	
     if (item.type == DGvTreeItem::FIELD_ITEM) {
            GvField *f = (GvField *) item.node;

		    if (RTISA(f,GvSFNode)) {	// Paste into SFNode field  
 		  	    ok = 1;
            }
			else 
			if (RTISA(f,GvMFNode)) { // Paste into MFNode field  
				GvMFNode *field = RTCAST(f,GvMFNode);
				ok=1;
			}
	} // field 
	

	if (!ok)  { 
	   Reporter.Warning("Can´t insert file on this object");
	   return;
	}

	CString fileName,fileExt;

	CFileDialog dlgFile(TRUE,".wrl",fileName,
							OFN_FILEMUSTEXIST,
							"VRML-Files (*.wrl)|*.wrl|"
							"Image-Files (" IMAGE_EXTENSIONS ")|" IMAGE_EXTENSIONS "|"
							"All Files (*.*)|*.*|"
							 "|"
					
							,NULL);

	dlgFile.m_ofn.lpstrTitle = "Insert VRML node from File";

	BOOL bRet = dlgFile.DoModal() == IDOK ? TRUE : FALSE;

	// TO DO : textures & sounds ? 

	if (bRet== TRUE) {
				fileExt = dlgFile.GetFileExt();
				fileExt.MakeLower();
				//GvNode * root = NULL;
				GvScene * parent = GetScene();

				GvScene * root = NULL;

				fileName=dlgFile.GetPathName();

				BeginWaitCursor();
				int fmt = GetFileType(fileName);

				switch (fmt) {
				case WAV_FMT : 
				case AU_FMT : 
				case MIDI_FMT : 
					{
						GvAudioClip *clip = new GvAudioClip();
						GvSound *sound = new GvSound();
						clip->url.set_(fileName);
						clip->loop.set(TRUE);

						sound->source.set(clip);
						sound->spatialize.set(FALSE);
						m_controller->setCopyBufferEx(sound);

						ok = DoPaste(owner,item,NULL);
						if (ok) OnChanged();

					}	
					break;
				case DIB_FMT:
				case RGB_FMT:
				case JPEG_FMT:
				case GIF_FMT:
				case TGA_FMT:
				case PPM_FMT:
				case PCX_FMT:
				case PNG_FMT:
					{
						GvBool asMovie = FALSE;
						if (fmt == GIF_FMT)
								asMovie = AfxMessageBox(CString("Insert ")+fileName + " as MovieTexture ?",MB_YESNO) == IDYES;
						else if (fmt == AVI_FMT) 
								asMovie = TRUE;

						if (asMovie) {
							GvMovieTexture *n = new GvMovieTexture();
							n->url.set_(fileName);
							m_controller->setCopyBufferEx(n);

						ok = DoPaste(owner,item,NULL);
						
						if (ok) OnChanged();
						} else {

						GvImageTexture *n = new GvImageTexture();
						n->url.set_(fileName);
						m_controller->setCopyBufferEx(n);

						ok = DoPaste(owner,item,NULL);
						
						if (ok) OnChanged();
						}

						break;

					}	

				case VRML_FMT:
					{
					GvBool asInline =
						AfxMessageBox(CString("Insert ")+fileName + " as Inline node ?",MB_YESNO) == IDYES;
					
					if (asInline) {
						Gv2Inline *n = new Gv2Inline();

						n->url.set_(fileName);
						m_controller->setCopyBufferEx(n);

						ok = DoPaste(owner,item,NULL);
						
						if (ok) OnChanged();

					} else {		
					// read a file as a GvNode object
					//int ret= ReadFile(fileName,root);
					root = new GvScene();
					root->setBrowser(GetView());

					// TO DO: set base url 
					int ret = ReadVrmlFile(fileName,GetView()->GetReporter(),root);
					
					m_controller->setCopyBuffer(root->children);


					// add protos / routes

					if (ret >= 0) {
						if (parent) {
							parent->protos.add(&root->protos);
							parent->routes.add(&root->routes);
						}
						ok = DoPaste(owner,item,NULL);
						if (ok) OnChanged();
					}
					else {
						if (root) delete root;
					}
					}
					break;
					}
				default :
					{
						Gv2Anchor *n = new Gv2Anchor();

						n->url.set_(fileName);
						m_controller->setCopyBufferEx(n);

						ok = DoPaste(owner,item,NULL);
						
						if (ok) OnChanged();

					break;
					}
				}

				EndWaitCursor();
				
	}
	return;
}

void DTreeEditor::OnUpdateNodeInsertFile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void DTreeEditor::OnNodeNew() 
{
    DNodeNew d;

    if (d.DoModal() == IDOK) {
        GvNode *n= NULL;
        GvName name(d.m_type);
        n = GvNode::createInstanceFromName(name,1); // to do : get VRML version 
        if (!n)
            Reporter.Error("Node type %s unknown",d.m_type);
        else  {
            m_controller->setCopyBuffer(n);
            OnEditPaste();
			OnNodeExpandfields();

        }
    }

	

	
}

void DTreeEditor::OnUpdateNodeNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void DTreeEditor::OnNodeRename() 
{
	int ok=0;
	
	DGvTreeItem item(m_tree);
	DGvTreeItem owner(m_tree);

	item.Set(GetSelection());

	if (item.type >= DGvTreeItem::GV_NODE) {
		// initialize by the owner of child
		owner.SetToOwner(item);
		ok = 1;
	}

	if (!ok)  { 
	   Reporter.Warning("Can´t rename this object");
	   return;
	}



	CString v = item.node->getName();;
	DString vedit("New Node Name",v);
	if (vedit.DoModal() == IDOK) 
	{
		item.node->setName(GvName(v));
		HTREEITEM newItem;
		int ret = InsertNode(owner.item,item.item,item.node,newItem);
		DeleteItem(item.item);
		m_tree.Select(newItem,TVGN_CARET );
		ok=1;

		//Refresh(item.item);
	}

}

void DTreeEditor::OnUpdateNodeRename(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void DTreeEditor::OnUpdateNodeSaveAs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void DTreeEditor::OnNodeSaveAs() 
{

	// TODO: Add your command handler code here
	int ok=0;
	
	DGvTreeItem item(m_tree);
	DGvTreeItem owner(m_tree);

	item.Set(GetSelection());


	if (item.type >= DGvTreeItem::GV_NODE) { 
		owner.SetToOwner(item);
	}
	else {
		Reporter.Error("Can only save a selected node");
		return;
	}


	CString    fileExt;
	CString    FullPath;
	CString    Name;
	CString    Path;

//#ifdef _EVALUATION
#if 0
	 {
		AfxMessageBox("Save File function not supported in this unregistered Evaluation Version" ORDER_INFO);
		return;
	}
#endif


	CFileDialog dlgFile(FALSE,"*.wrl",NULL,OFN_HIDEREADONLY,
#ifdef _VRML
	"VRML-Files (*.wrl;*.vrml)|*.wrl|"
#endif
	"|"
	,NULL);

	dlgFile.m_ofn.lpstrTitle = "Write node to 3D file";

	BOOL bRet = dlgFile.DoModal() == IDOK ? TRUE : FALSE;

	if (bRet== TRUE) {
		int res=0;
		fileExt = dlgFile.GetFileExt();
		fileExt.MakeLower();
		FullPath=dlgFile.GetPathName();

		Reporter.Status("Writing file "+FullPath + "...");
		
		CWaitCursor waitCursor;

				// write
#ifdef _VRML
			if ((fileExt == "wrl")|| (fileExt == "vrml")) {

				  	GioTraversal traversal(FullPath,"w");
					traversal.vrml2 = GetView()->IsVrml2();
					traversal.format = VRML_FMT;
					traversal.writeInlines = 0;

					traversal.DoFileHeader();
					traversal.astream->comment("written by " _PROGRAM _PROGRAM_TYPE " " _VERSION " " URL_HOME);
					traversal.astream->nl();
					
					
					InitializeGvAttributes(traversal);
					traversal.InitializeAttributes();

					item.node->Traverse(traversal);
					GvBool asInline = AfxMessageBox(CString("Replace node by Inline to  ")+FullPath + " ?",MB_YESNO) == IDYES;

					if (asInline) {
						OnEditCopy();
						Gv2Inline *anchor=new Gv2Inline();
						anchor->url.set_(FullPath);
						m_controller->getCopyBuffer(anchor->children);
						m_controller->setCopyBuffer(anchor);
						OnEditPaste();
						OnNodeExpandfields();

					}
				  
			} else 
#endif
			{
				Reporter.Error("Operation not supported in this version");
			}
	}
}

//static enum GDisplayTreeTraversal::Mode viewMode = GDisplayTreeTraversal::MODE_TREE;

void DTreeEditor::OnViewRefresh() 
{
	
	if (refDict) delete refDict;
	refDict = new GvDict;

	// Update tree data 
	GDisplayTreeTraversal traversal(refDict);
	
	traversal.mode = (enum GDisplayTreeTraversal::Mode)viewMode;
		
	traversal.InitializeAttributes();
		
	traversal.treeEditor = this;
	m_tree.DeleteAllItems();
	BeginWaitCursor();
	
	if (m_pView)
	  int ret = m_pView->view->Traverse(traversal);
	
	ExpandRoot();
	
	EndWaitCursor();
}

void DTreeEditor::OnUpdateViewRefresh(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}


void DTreeEditor::OnViewTree() 
{
	if (viewMode != GDisplayTreeTraversal::MODE_TREE) {
		viewMode = GDisplayTreeTraversal::MODE_TREE;
		OnViewRefresh();
	}

}

void DTreeEditor::OnUpdateViewTree(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(viewMode == GDisplayTreeTraversal::MODE_TREE);
}


void DTreeEditor::OnViewReferences() 
{
	if (viewMode != GDisplayTreeTraversal::MODE_EXTERN_REF) {
		viewMode = GDisplayTreeTraversal::MODE_EXTERN_REF;
		OnViewRefresh();
	}
}

void DTreeEditor::OnUpdateViewReferences(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(viewMode == GDisplayTreeTraversal::MODE_EXTERN_REF);
}

void DTreeEditor::OnViewDefedObjects() 
{
	if (viewMode != GDisplayTreeTraversal::MODE_DEF) {
		viewMode = GDisplayTreeTraversal::MODE_DEF;
		OnViewRefresh();
	}
}

void DTreeEditor::OnUpdateViewDefedObjects(CCmdUI* pCmdUI) 
{
   pCmdUI->SetCheck(viewMode == GDisplayTreeTraversal::MODE_DEF);
}


/* **********************************************************************************+ Field functions */

// edit field value as string
void DTreeEditor::OnFieldEdit() 
{
    OnFieldEditType();
}


int  DTreeEditor::OnFieldEditType(int editType) 
{


	int ok=0;

    GvField *f = NULL;      // Field single value
    GvMField *mf = NULL;    // MultiValue filed
	int isMfield = 0;
	int mfieldIndex = 0;

  	GvString val;  // value of field 
	
	DGvTreeItem item(m_tree);
	DGvTreeItem owner(m_tree);

	item.Set(GetSelection());

	// initialize by the owner of child
	owner.SetToOwner(item);

	if (item.type == DGvTreeItem::FIELD_ITEM) {
        f = (GvField *) item.node;
		ok = 1;

		// get field value as string
		    if (RTISA(f,GvSField)) {
				if (f->get(val) <= 0) ok=0;

                if (editType == EDIT_AUTO) 
                    if (RTISA(f,GvSFColor)) editType = EDIT_COLOR;

			}
			else if (RTISA(f,GvMField)) {
				mf= (GvMField *) f;
                mfieldIndex = 0;
				
                if (mf->Length() == 1) {
    				if (mf->get1(val,mfieldIndex)<=0) ok=0;
                }
                else 
				if (mf->Length() <= 4) {
					f->get(val);
				}
				else { val = "[ ....]"; ok=0; }

                if (editType == EDIT_AUTO) 
                    if (RTISA(f,GvMFColor)) editType = EDIT_COLOR;

			}

//		    const char *type = fields->getFieldType(node, i);
//			buf += " # ";
//			buf += type;

	}
	else if (item.type == DGvTreeItem::MFIELD_ELEMENT) {
		isMfield = 1;
		ok = 1;
    	f = (GvField *) owner.node;
    	mf = (GvMField *) owner.node;
		mfieldIndex = item.nodeChildIndex;
		if (mfieldIndex<0 || mfieldIndex>=mf->getNum() ) ok=0;
    	else if (mf->get1(val,mfieldIndex) <= 0) ok=0;
        
		if (editType == EDIT_AUTO) 
            if (RTISA(mf,GvMFColor)) editType = EDIT_COLOR;

	}

	if (!ok)  { 
	   Reporter.Warning("Can´t edit this object");
	   return(0);
	}
	    
// write field name
//	const GvName &fni = fields->getFieldName(i);
//	buf += fni;

	CString v(val);
    CString prompt("New Field value");

	//if (f && f->owner) prompt = f->owner
    
    if (f) prompt.Format("New %s Field value", f->GetType());
	
    switch (editType) {
    case EDIT_COLOR:
        {
        Point color;

        if (f && RTISA(f,GvSFColor)) ((GvSFColor *) f)->get(color);
        else sscanf(v,"%f %f %f",&color.x,&color.y,&color.z);
        
        ok = m_pView->EditColor(prompt,color)>0;
        
        if (ok) v.Format("%f %f %f",color.x,color.y,color.z);
        }

        break;

    default :
        DString vedit(prompt,v);
	    ok = (vedit.DoModal() == IDOK);
    }
    

    if (ok)
	{
		int  ret;

		
		if (isMfield) {  // set MField element + update label
			ret = mf->set1(v,mfieldIndex);
			mf->OnChanged();

			CString buf;
			mf->get1(val,mfieldIndex);

			buf.Format("%s #%d",(const char *)val,mfieldIndex+1);
			m_tree.SetItemText(item.item,buf);


		} else {
            if (mf) {
			   
			   if (mfieldIndex >=mf->getNum())
				   mf->makeRoom(mfieldIndex+1);
				
			   if (mfieldIndex <mf->getNum()) {
					ret = mf->set1(v,mfieldIndex);
					mf->setDefault(FALSE);
					mf->OnChanged();
				}

            }
            else 
			  ret = f->set(v); // set update, set field to value 
		}
		
		if (ret<=0) {
            Reporter.Error("Can´t set field to new value '%s'",v);
            ok = 0;

        } else {
		   //Update item
		   //HTREEITEM hReturn;
		   
		   //ret = UpdateField(owner.item,item.item,f,hReturn);

 			// update all fields of owner node, to do: optimize
		   	if (owner.type >= DGvTreeItem::GV_NODE) {
	     		InsertNodeFields(owner.item,owner.node,0);  // fieldsWithDefaults,int fieldTypes)
	     		m_tree.Expand(owner.item,TVE_EXPAND); 
			}

		    OnChanged();
            ok = 1;
		}
	}
    return(ok);
}

void DTreeEditor::OnUpdateFieldEdit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void DTreeEditor::OnNodeExpandfields() 
{
	int ok=0;
	DGvTreeItem item(m_tree);
	item.Set(GetSelection());
	if (item.type >= DGvTreeItem::GV_NODE) {
	     InsertNodeFields(item.item,item.node,1);  // fieldsWithDefaults,int fieldTypes)
	     m_tree.Expand(item.item,TVE_EXPAND); 

#if 0
	   int hasFields = m_tree.ItemHasChildren(item.item);
	   if (hasFields) {
	   		DGvTreeItem first(m_tree);
	   		first.Set(m_tree.GetChildItem(item.item));
			if (first.type != DGvTreeItem::FIELD_ITEM) hasFields = 0;
	   }
	   
	   if (!hasFields) {
	     InsertNodeFields(item.item,item.node);  // fieldsWithDefaults,int fieldTypes)
	     m_tree.Expand(item.item,TVE_EXPAND); 
	   }
#endif
	}

	
}

void DTreeEditor::OnUpdateNodeExpandfields(CCmdUI* pCmdUI) 
{
//	selection.Set(GetSelection());
//	pCmdUI->Enable(selection.type >= DGvTreeItem::GV_NODE);
}


void DTreeEditor::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void DTreeEditor::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if ( m_tree.m_hWnd !=NULL && nType != SIZE_MINIMIZED) {
		RECT rect;
		GetClientRect( &rect );
		m_tree.MoveWindow( &rect);

	} 
	TRACE("Size %d %d %d \n",nType,cx,cy);
	// TODO: Add your message handler code here

	
}


void DTreeEditor::OnNodeConvertToIndexedFaceset() 
{
	// TODO: Add your command handler code here
	Reporter.Error("Operation not supported in this version");
	
}

void DTreeEditor::OnUpdateNodeConvertToIndexedFaceset(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((selection.type >= DGvTreeItem::GV_NODE) && RTISA(selection.node,GvNodeShell));
}

void DTreeEditor::OnNodeConvertToSeparator() 
{	int ok=0;

	DGvTreeItem item(m_tree);
	item.Set(GetSelection());

	if (item.type >= DGvTreeItem::GV_NODE) {
		DGvTreeItem owner(m_tree);
		owner.SetToOwner(item);
		GvNode *newNode;
		
		if  (owner.type >= DGvTreeItem::GV_GROUP_NODE) {
			GvGroup *group= (GvGroup*) owner.node;
			if (item.nodeChildIndex>=0) {
				newNode = item.node->EvalAs(GvSeparator::ClassId(),0);
				if (newNode && newNode != item.node) {
			    	group->deleteChild(item.nodeChildIndex);
			    	group->insertChild(item.nodeChildIndex,newNode);
					// Expand and display node & children
					HTREEITEM newItem;
					int ret = InsertNode(owner.item,item.item,newNode,newItem);
					DeleteItem(item.item);
					m_tree.Select(newItem,TVGN_CARET );
					OnChanged();
					ok=1;
				}
			}
		}
	 }
	 if (!ok) Reporter.Warning("Can´t convert this object to a Separator");
}

void DTreeEditor::OnUpdateNodeConvertToSeparator(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(selection.type >= DGvTreeItem::GV_NODE);
}

void DTreeEditor::OnFieldEditAsFilename() 
{
	// TODO: Add your command handler code here
	Reporter.Error("Operation not supported in this version");

	
}

void DTreeEditor::OnUpdateFieldEditAsFilename(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->Enable((selection.type == DGvTreeItem::GV_FIELD) && RTISA(selection.node,GvNodeShell));
	
}

void DTreeEditor::OnFieldEditAsString() 
{
    OnFieldEditType(EDIT_ASCII);
	
}

void DTreeEditor::OnUpdateFieldEditAsString(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here

}


void DTreeEditor::OnFieldEditAsColor() 
{
    OnFieldEditType(EDIT_COLOR);
}

void DTreeEditor::OnUpdateFieldEditAsColor(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
}

void DTreeEditor::OnFieldDrag() 
{
	// TODO: Add your command handler code here
	Reporter.Error("Operation not supported in this version");
	
}

void DTreeEditor::OnUpdateFieldDrag(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void DTreeEditor::OnNodeExpandInlines() 
{
	// TODO: Add your command handler code here
	Reporter.Error("Operation not supported in this version");
	
}

void DTreeEditor::OnUpdateNodeExpandInlines(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void DTreeEditor::OnOptionsUpdateinlineboundingboxes() 
{
	// TODO: Add your command handler code here
	Reporter.Error("Operation not supported in this version");
	
}

void DTreeEditor::OnUpdateOptionsUpdateinlineboundingboxes(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void DTreeEditor::OnOptionsWriteinlines() 
{
	Reporter.Error("Operation not supported in this version");
	// TODO: Add your command handler code here
	
}

void DTreeEditor::OnUpdateOptionsWriteinlines(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
/////////////////////////////////////////////////////////////////////////////
// DNodeNew dialog


DNodeNew::DNodeNew(CWnd* pParent /*=NULL*/)
	: CDialog(DNodeNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(DNodeNew)
	m_type = _T("");
	//}}AFX_DATA_INIT
}


void DNodeNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DNodeNew)
	DDX_Control(pDX, IDC_NODE_TYPE, m_typeList);
	DDX_CBString(pDX, IDC_NODE_TYPE, m_type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DNodeNew, CDialog)
	//{{AFX_MSG_MAP(DNodeNew)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DNodeNew message handlers



void DTreeEditor::OnNodeConverttoAnchor() 
{
	OnEditCopy();
	Gv2Anchor *anchor=new Gv2Anchor();
	m_controller->getCopyBuffer(anchor->children);
	m_controller->setCopyBuffer(anchor);

	DGvTreeItem item(m_tree);
	item.Set(GetSelection());
	DGvTreeItem owner(m_tree);
	owner.SetToOwner(item);
	int ok = DoPaste(owner,item,NULL,1);

	if (!ok) Reporter.Warning("Can´t do convert operation on this object");
	else OnChanged();

}

#if 0
void DTreeEditor::OnNodeConverttoInline() 
{
	int ok=0;

	OnEditCopy();

	Gv2Inline *anchor=new Gv2Inline();
	m_controller->getCopyBuffer(anchor->children);
	m_controller->setCopyBuffer(anchor);

	DGvTreeItem item(m_tree);
	item.Set(GetSelection());
	DGvTreeItem owner(m_tree);
	owner.SetToOwner(item);
	int ok = DoPaste(owner,item,NULL,1);

	if (!ok) Reporter.Warning("Can´t do convert operation on this object");
	else OnChanged();

}
#endif



void DTreeEditor::OnNodeConvertToTransform() 
{
	OnEditCopy();
	Gv2Transform *anchor=new Gv2Transform();
	m_controller->getCopyBuffer(anchor->children);
	m_controller->setCopyBuffer(anchor);

	DGvTreeItem item(m_tree);
	item.Set(GetSelection());
	DGvTreeItem owner(m_tree);
	owner.SetToOwner(item);
	int ok = DoPaste(owner,item,NULL,1);

	if (!ok) Reporter.Warning("Can´t do convert operation on this object");
	else OnChanged();
	
}


// compute bbox from children and update nodes fields 
BOOL UpdateBBox(GvNode *node,BOOL enlarge = FALSE) 
{
	BBox bbox;
	BOOL ret = TRUE;
	if (RTISA(node,Gv2Group)) {
		Gv2Group *n = (Gv2Group *) node;
		n->children.getBBox(bbox);
		if (enlarge) {
			BBox old;
			n->getBBox(old);
			if (!old.IsEmpty() && !bbox.IsEmpty()) {
				bbox.Union(old);
			}
		}
		n->setBBox(bbox);
	}
	else ret = FALSE;
	return(ret);
}

// normalize a Transform transformation to show the children in  unit box 
BOOL NormalizeToBBox(GvNode *node,BOOL enlarge = FALSE) 
{
	BBox bbox;
	BOOL ret = TRUE;
	if (RTISA(node,Gv2Transform)) {
		
		Gv2Transform *n = (Gv2Transform *) node;
		n->children.getBBox(bbox);

		if (enlarge) {
			BBox old;
			n->getBBox(old);
			if (!old.IsEmpty() && !bbox.IsEmpty()) {
				bbox.Union(old);
			}
		}
		n->setBBox(bbox);
		if (bbox.IsEmpty())
				ret = FALSE;
		else {
			Point translation  = -bbox.Center(); 
			Point size = bbox.Size();
			float factor = max(size.x,max(size.y,size.z));
			factor = 2.0f/factor;
			n->scale.set(factor,factor,factor);
			translation *=factor;
			n->translation.set(translation);

		}

	}
	else ret = FALSE;
	return(ret);
}


// recompute the bounding box from the children 
void DTreeEditor::OnNodeUpdateBbox() 
{
	int ok=0;

	DGvTreeItem item(m_tree);
	item.Set(GetSelection());

	if (item.type >= DGvTreeItem::GV_NODE) {
		DGvTreeItem owner(m_tree);
		owner.SetToOwner(item);
		ok = UpdateBBox(item.node);
		if (ok) OnNodeExpandfields();

	 }
	 if (!ok) Reporter.Warning("Can´t do operation on this object ");
}

void DTreeEditor::OnUpdateNodeUpdateBbox(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}


BOOL BuildIfs(GShell *s,GvNodeHandle &node,GvBool isSolid)
{
 
	if (RTISA(s,GShellI))
		node =  CreateIndexedFaceSet((GShellI *) s, FALSE,  NULL,NULL,isSolid);
	else node =  CreateIndexedFaceSet(s, FALSE,  NULL,NULL,isSolid);
 
	return TRUE;
 
 //GvNodeShell *newNode = new GvNodeShell();
 //	newNode->SetShell((GShell *) (GvNode *) shell);

}


// get this node as one shell
BOOL DTreeEditor::GetShell(GvNode *node,GvNodeHandle &shell,GvBool &isSolid)
{

	if (node == NULL) return FALSE;

	if (RTISA(node,GvNodeShell)) {
		GvNodeShell *nshell = (GvNodeShell *) node;

		isSolid = nshell->isSolid();
		// if (nshell->theShell == NULL) 	nshell->BuildShell(state);

		shell = nshell->theShell;
		if (shell != (GvNode *) NULL) 
			return TRUE;
	}
	
	isSolid = TRUE;
	// buildShell Traversal 
	GBuildShellTraversal t;
	
	t.doLookup = ifsRoundCoord;

	if (ifsRoundCoord) 
		GShell::maxLookupDistance2= ifsRoundCoordScale*ifsRoundCoordScale;

 	t.InitializeAttributes();
  	t.view = m_pView->view;

	InitializeGvAttributes(t);

	node->Traverse(t);

	if (t.shell) {
		// TRACE("O
		t.shell->f.Optimize(FALSE);
	}
	
	shell = t.GetShell();

	t.shell = NULL; // currently stored 

	if (shell == (GvNode *) NULL) 
		return FALSE;


	return TRUE;
}


void DTreeEditor::OnNodeConvexHull() 
{
	int ok=0;

	DGvTreeItem item(m_tree);
	item.Set(GetSelection());

	if (item.type >= DGvTreeItem::GV_NODE) {
		DGvTreeItem owner(m_tree);
		owner.SetToOwner(item);

		GvNode *node = item.node;
		GvNodeHandle shell;
		GvBool isSolid;

		if (GetShell(node,shell,isSolid)) {

			GvNodeHandle newNode;

			GShell *s = (GShell *) (GvNode *) shell;
	    
			int numTriangles;
			Triangle *tri = NULL;

			if (ConvexHull3D (s->v.Length(), s->v.Data(), 
				numTriangles, tri) >0) {
				GFaceList f;
			
				for(int i=0; i<numTriangles; i++)
					f.AppendTri(tri[i][0],tri[i][1],tri[i][2]);

				delete [] tri;
				s->SetFaceList(f);
				s->SetFlag(SHELL_CONVEX | SHELL_CLOSED);

			}

			BuildIfs(s,newNode,TRUE);
			((GvNode *) newNode)->setName("_ConvexHull");
			m_controller->setCopyBuffer(newNode);

			ok = DoPaste(owner,item,NULL,1);
		}
		ok = 0;

	 }
	 if (!ok) Reporter.Warning("Can´t do operation on this object ");
	
}


void DTreeEditor::OnNodeCombine() 
{
	int ok=0;

	DGvTreeItem item(m_tree);
	item.Set(GetSelection());

	if (item.type >= DGvTreeItem::GV_NODE) {
		DGvTreeItem owner(m_tree);
		owner.SetToOwner(item);

		GvNode *node = item.node;
		GvNodeHandle shell;
		GvBool isSolid;

		if (GetShell(node,shell,isSolid)) {
			GShell *theShell = (GShell *) (GvNode *) shell;

			if (compressIfs && theShell->fc.Length() == 0) {
				theShell->f.Optimize();
			}
			if (ifsRoundCoord) {
				GRound(theShell->v,ifsRoundCoordScale,1.0f/ifsRoundCoordScale);	
			}

			GvNodeHandle newNode;
			
			if (RTISA(theShell,GShellI)) {
				GShellI *shellI = (GShellI*) theShell;


			if (compressIfs) {
				GShellI *newS = NULL;
				TRACE("Compressing shell : facelist length = %d, verts %d v %d n %d p%d c%d \n",
				shellI->f.Length(),shellI->verts.Length(),shellI->v.Length(),shellI->vn.Length(),shellI->vp.Length(),shellI->vc.Length());
				newS = CompressShell(shellI,NULL,1,ifsRoundCoord);
				
				if (ifsRoundCoord) 
					newS->f.Optimize();
				shellI = newS;

				TRACE("Compressed Shell : facelist length = %d, verts %d v %d n %d p%d c%d \n",
				shellI->f.Length(),shellI->verts.Length(),shellI->v.Length(),shellI->vn.Length(),shellI->vp.Length(),shellI->vc.Length());
				shell = shellI;
			}
			}	



			BuildIfs((GShell *) (GvNode *) shell,newNode,isSolid);

			m_controller->setCopyBuffer(newNode);

			ok = DoPaste(owner,item,NULL,1);
		}
		ok = 0;

	 }
	 if (!ok) Reporter.Warning("Can´t do operation on this object ");
}


void DTreeEditor::OnUpdateNodeCombine(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void DTreeEditor::OnNodeExpandbbox() 
{
	int ok=0;

	DGvTreeItem item(m_tree);
	item.Set(GetSelection());

	if (item.type >= DGvTreeItem::GV_NODE) {
		DGvTreeItem owner(m_tree);
		owner.SetToOwner(item);
		ok = UpdateBBox(item.node,TRUE);
		if (ok) OnNodeExpandfields();

	 }
	 if (!ok) Reporter.Warning("Can´t do operation on this object ");
}

void DTreeEditor::OnUpdateNodeExpandbbox(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void DTreeEditor::OnNodeTransform() 
{

   if (m_pView->dialogs.m_dTransform == NULL) {
    	m_pView->dialogs.m_dTransform = new DTransformSheet(_T("Transform"),m_pView);
		m_pView->dialogs.m_dTransform->AddPage(new DTransformPosition(m_pView));
		m_pView->dialogs.m_dTransform->AddPage(new DTransformScale(m_pView));
		m_pView->dialogs.m_dTransform->AddPage(new DTransformRotation(m_pView));
		m_pView->dialogs.m_dTransform->Create(m_pView);
	}
    else {
		if (m_pView->dialogs.m_dTransform->m_hWnd == NULL) m_pView->dialogs.m_dTransform->Create(m_pView);
        else m_pView->dialogs.m_dTransform->UpdateData(FALSE);	// View state ==> to dialog
		m_pView->dialogs.m_dTransform->ShowWindow(SW_SHOWNORMAL);
    }

	
}

void DTreeEditor::OnUpdateNodeTransform(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void DTreeEditor::OnNodeNormalize() 
{
	int ok=0;

	DGvTreeItem item(m_tree);
	item.Set(GetSelection());

	if (item.type >= DGvTreeItem::GV_NODE) {
		DGvTreeItem owner(m_tree);
		owner.SetToOwner(item);
		ok = NormalizeToBBox(item.node);
		if (ok) OnNodeExpandfields();

	 }
	 if (!ok) Reporter.Warning("Can´t do operation on this object, need a transform node ");
	
}

void DTreeEditor::OnUpdateNodeNormalize(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
}


void DTreeEditor::OnNodeOptimize() 
{
	int ok=0;

	DGvTreeItem item(m_tree);
	item.Set(GetSelection());
	DGvTreeItem owner(m_tree);
	owner.SetToOwner(item);

	OnEditCopy();

	CWaitCursor wait;
	GOptimizeTraversal t;
  	t.InitializeAttributes();

	t.view = m_pView->view;

	InitializeGvAttributes(t);
	GetOptimizerSettings(t);
	GvBspTree::ClearStats();

	Reporter.Status("Optimizing Tree ...");



	if (item.type >= DGvTreeItem::GV_NODE) {

		GvNode *node = item.node;

		node->Traverse(t);


		GvNodeHandle result;
		t.GetResult(result);
		m_controller->setCopyBuffer(result);

		ok = DoPaste(owner,item,NULL,1);
	 }
	 else {
		m_controller->copyBuffer.Traverse(t);
		OnEditCut();
		GvMFNode result(0);
		t.GetResultBsp(result);
		m_controller->setCopyBuffer(result);
		ok = DoPaste(owner,item,NULL,1);

	 }
	 Reporter.Status("Optimizing Tree done");

	if (buildBspTree)
		Reporter.Trace("Optimizing : Geom Nodes for BSP %d , Num BspTree Nodes %d Max Level %d Mean #overlap %f",
		GvBspTree::bspNumGeomNodes,GvBspTree::bspNumNodes,GvBspTree::bspMaxLevel,(float) GvBspTree::bspOverlapMean / (float) GvBspTree::bspOverlapMeanCnt);
		
	 if (!ok) Reporter.Warning("Can´t do operation on this object ");
	
}

void DTreeEditor::OnUpdateNodeOptimize(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
/////////////////////////////////////////////////////////////////////////////
// DOptimizeSettings dialog


DOptimizeSettings::DOptimizeSettings(CWnd* pParent /*=NULL*/)
	: CDialog(DOptimizeSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(DOptimizeSettings)
	m_bspHeuristic = _T("");
	m_buildBsp = FALSE;
	m_expandIfs = FALSE;
	m_keepInlines = FALSE;
	m_splitFaces = FALSE;
	m_ignoreRoutes = FALSE;
	m_keepLods = FALSE;
	m_maxBspLevel = 0;
	m_bspSearchLimit = 0;
	m_compressIfs = TRUE;
	m_removeNames = FALSE;
	m_expandFaceColors = TRUE;
	m_bspInlineLevel = 0;
	m_bspInline = FALSE;
	m_keepPrimitives = TRUE;
	m_ifsRoundCoord = FALSE;
	m_ifsRoundCoordScale = 0.001f;
	m_bx3d = FALSE;
	m_applyTransforms = FALSE;
	m_lookupCoord = FALSE;
	//}}AFX_DATA_INIT
}


void DOptimizeSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DOptimizeSettings)
	DDX_CBString(pDX, IDC_BSP_HEURISTIC, m_bspHeuristic);
	DDX_Check(pDX, IDC_BUILD_BSP, m_buildBsp);
	DDX_Check(pDX, IDC_EXPAND_IFS, m_expandIfs);
	DDX_Check(pDX, IDC_KEEP_INLINES, m_keepInlines);
	DDX_Check(pDX, IDC_SPLIT, m_splitFaces);
	DDX_Check(pDX, IDC_IGNORE_ROUTES, m_ignoreRoutes);
	DDX_Check(pDX, IDC_KEEP_LODS, m_keepLods);
	DDX_Text(pDX, IDC_MAX_BSP_LEVEL, m_maxBspLevel);
	DDX_Text(pDX, IDC_BSP_SEARCH_LIMT, m_bspSearchLimit);
	DDX_Check(pDX, IDC_COMPRESS, m_compressIfs);
	DDX_Check(pDX, IDC_REMOVE_NAMES, m_removeNames);
	DDX_Check(pDX, IDC_EXPAND_FACE_COLOR, m_expandFaceColors);
	DDX_Text(pDX, IDC_BSP_INLINE_LEVEL, m_bspInlineLevel);
	DDX_Check(pDX, IDC_BSP_INLINES, m_bspInline);
	DDX_Check(pDX, IDC_KEEP_PRIMITIVES, m_keepPrimitives);
	DDX_Check(pDX, IDC_IFS_ROUND_COORD, m_ifsRoundCoord);
	DDX_Text(pDX, IDC_IFS_ROUND_COORD_SCALE, m_ifsRoundCoordScale);
	DDX_Check(pDX, IDC_BX3D, m_bx3d);
	DDX_Check(pDX, IDC_APPLY_TRANSFORMS, m_applyTransforms);
	DDX_Check(pDX, IDC_SHARE_COORD, m_lookupCoord);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DOptimizeSettings, CDialog)
	//{{AFX_MSG_MAP(DOptimizeSettings)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DOptimizeSettings message handlers

void DTreeEditor::OnSettingsOptimizer() 
{
	// TODO: Add your command handler code here
	DOptimizeSettings d;
	d.m_buildBsp = buildBspTree;
	d.m_expandIfs = decomposeIfs;
	d.m_keepInlines = doInlines;
	d.m_applyTransforms = applyTransforms;
	d.m_bspHeuristic = bspHeuristic;
	d.m_splitFaces = splitFaces;
	d.m_ignoreRoutes = ignoreRoutes;
	d.m_keepLods = keepLods;
	d.m_maxBspLevel = maxBspLevel;
	d.m_bspSearchLimit = bspSearchLimit;
	d.m_compressIfs = compressIfs;
	d.m_removeNames = removeNames;
	d.m_expandFaceColors = expandFaceColors;
								
	d.m_bspInlineLevel		= bspInlineLevel;
	d.m_bspInline			= bspInline;
	d.m_keepPrimitives		= keepPrimitives;
	d.m_ifsRoundCoord		= ifsRoundCoord;
	d.m_ifsRoundCoordScale	= ifsRoundCoordScale;
	d.m_lookupCoord		= ifsLookupCoord;

	d.m_bx3d=bx3dOpt;


	if (d.DoModal() == IDOK) {
		 buildBspTree = d.m_buildBsp ;
		 decomposeIfs =d.m_expandIfs;
		 doInlines = d.m_keepInlines;
	 	applyTransforms = d.m_applyTransforms;

		 bspHeuristic = d.m_bspHeuristic;
		 splitFaces = d.m_splitFaces;
		 ignoreRoutes = d.m_ignoreRoutes;
		 keepLods = d.m_keepLods;
		 maxBspLevel = d.m_maxBspLevel;
		 bspSearchLimit= d.m_bspSearchLimit;
		 compressIfs = d.m_compressIfs;
		 removeNames = d.m_removeNames;
		 expandFaceColors = d.m_expandFaceColors;

		bspInlineLevel = d.m_bspInlineLevel;
		bspInline	 =	d.m_bspInline;
		keepPrimitives = d.m_keepPrimitives;
		ifsRoundCoord	 = d.m_ifsRoundCoord;
		ifsRoundCoordScale =	d.m_ifsRoundCoordScale;
		ifsLookupCoord = d.m_lookupCoord ;

		bx3dOpt=d.m_bx3d;

	}
	
}


void DTreeEditor::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar) {
	
	case VK_DELETE: OnEditDelete(); break;

	default : break;
	}
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

// Pg. 365 , 9.7
static void ComputeKnotVector(int N, int pdim, float u[]) 
{
	int m,k;

	// N = numCtrlPts -1
	// m = numCtrlPts+pdim == N+1+pdim
	// nknots = m+1

	m = N + pdim+1; 

	for (k=0; k<=pdim; k++) 
	{ u[k] = 0.0f; 
	} 

	for (k=(m-pdim); k<=(m); k++) 
	{ u[k] = 1.0f;
	}
	
	if ((N-pdim+1) <=0) return;

	float div = 1.0f / (float) (N-pdim+1);

	for (int j=1; j<=(N-pdim); j++) 
	{	u[j+pdim] = (float )j  * div; 
	}
}


void DTreeEditor::OnNodeCheck() 
{
    int ok=0;
	// TODO: Add your command handler code here
	OnEditCopy();
	GvNode * n = m_controller->getCopyBufferAsNode();

	DGvTreeItem item(m_tree);
	item.Set(GetSelection());
	DGvTreeItem owner(m_tree);
	owner.SetToOwner(item);

	if (RTISA(n,GvNurbsPositionInterpolator)) {
		{
		GvNurbsPositionInterpolator *s = (GvNurbsPositionInterpolator *) n;
		int dimension = s->keyValue.getNum();
		if (s->knot.getNum() != (dimension + s->order))  {
			int m=(dimension + s->order);
			s->knot.setLength(m);
			ComputeKnotVector(dimension-1, s->order-1,s->knot.values);
			s->knot.setDefault(FALSE);  s->knot.OnChanged();	
			Reporter.Trace("PositionInterpolator:Updated knot vector ");
			if (!s->fractionAbsolute) {
				s->fractionAbsolute.set(TRUE);	s->fractionAbsolute.setDefault(TRUE);
			}
		}
		if (s->keyWeight.getNum() != (dimension) )
		{
			int m=dimension;
			if (s->keyWeight.getNum() >0) {
				s->keyWeight.setLength(m);
				s->keyWeight.setDefault(FALSE);  s->keyWeight.OnChanged();	
				Reporter.Trace("PositionInterpolator:Updated length of keyWeight ");
			}
		}

		}

		ok=1;
		if (ok) OnNodeExpandfields();
	} // shell 
	else


	if (RTISA(n,GvInterpolator)) {
			int numKeys = 0;

			GvInterpolator *ip = (GvPositionInterpolator *) n;
			if (RTISA(n,GvPositionInterpolator)) { numKeys = ((GvPositionInterpolator *) n)->keyValue.getNum();	}
			else if (RTISA(n,GvScalarInterpolator)) { numKeys = ((GvScalarInterpolator *) n)->keyValue.getNum();	}
			else if (RTISA(n,GvColorInterpolator)) { numKeys = ((GvColorInterpolator *) n)->keyValue.getNum();	}
			else if (RTISA(n,GvOrientationInterpolator)) { numKeys = ((GvOrientationInterpolator *) n)->keyValue.getNum();	}
			else if (RTISA(n,GvCoordinateInterpolator)) { numKeys = ((GvCoordinateInterpolator *) n)->keyValue.getNum();	}
			else if (RTISA(n,GvNormalInterpolator)) { numKeys = ((GvNormalInterpolator *) n)->keyValue.getNum();	}
			
			if (numKeys != ip->key.getNum()) {
				ip->key.makeRoom(numKeys);
				for(int i=0;i<numKeys;i++) {
					float f= (float) i / (float) (numKeys-1);
					ip->key.set1(i,f);
				}
				ip->key.set1(numKeys-1,1.0f);
			}	
			if (RTISA(n,GvPositionInterpolator)) {

				if (AfxMessageBox("Convert PositionInterpolator to NurbsPositionInterpolator ? ",MB_YESNO) == IDYES) 
				{


				GvPositionInterpolator *ip=(GvPositionInterpolator *) n;
				GvNurbsPositionInterpolator *nip =new GvNurbsPositionInterpolator();
				float scaleFactor = 1.0;
				int dimension, order;
				FloatArray knot;
				PointArray pts;

				ComputeNurbsCurve (
					   ip->keyValue.getNum(),
					   ip->key,ip->keyValue,
					   scaleFactor,
					   dimension,order,
					   knot,
					   pts
					   );
				nip->knot.set(knot);
				nip->keyValue.set(pts);

				//nip->dimension.set(dimension);
				nip->order.set(order);
				m_controller->setCopyBuffer(nip);
				OnEditPaste();
				}
				else if (AfxMessageBox("Convert PositionInterpolator to NurbsCurve ? ",MB_YESNO) == IDYES) 
				{


				GvPositionInterpolator *ip=(GvPositionInterpolator *) n;
				GvNurbsCurve *nip =new GvNurbsCurve();
				float scaleFactor = 1.0;
				int dimension, order;
				FloatArray knot;
				PointArray pts;


				ComputeNurbsCurve (
					   ip->keyValue.getNum(),
					   ip->key,ip->keyValue,
					   scaleFactor,
					   dimension,order,
					   knot,
					   pts
					   );

				//nip->dimension.set(dimension);
				nip->knot.set(knot);
				nip->controlPoint.set(pts);

				nip->order.set(order);
				GvShape *s = new GvShape();
				s->geometry.set(nip);
				m_controller->setCopyBuffer(s);
				OnEditPaste();
				}

				else if (AfxMessageBox("Convert PositionInterpolator to IndexedLineSet ? ",MB_YESNO) == IDYES) 
				{


				GvPositionInterpolator *ip=(GvPositionInterpolator *) n;
				Gv2IndexedLineSet *nip =new Gv2IndexedLineSet();
			    GvCoordinate *n = new  GvCoordinate();
				int num=ip->keyValue.getNum();
				n->point.set(ip->keyValue);
				nip->coord.set(n);
				nip->coordIndex.setLength(num+1);
				for (int i=0; i<num;i++) nip->coordIndex.values[i]=i;
				nip->coordIndex.values[num]=-1;
				nip->coordIndex.setDefault(FALSE);
				


				float scaleFactor = 1.0;
				GvShape *s = new GvShape();
				s->geometry.set(nip);
				m_controller->setCopyBuffer(s);
				OnEditPaste();
				}



			}
			ok=1;
			if (ok) OnNodeExpandfields();
	}
	else

	if (RTISA(n,GvNurbsSurface)) {
		{
		GvNurbsSurface *s = (GvNurbsSurface *) n;
		if (s->uKnot.getNum() != (s->uDimension + s->uOrder))  {
			int m=s->uDimension + s->uOrder;
			s->uKnot.setLength(m);
			ComputeKnotVector(s->uDimension-1, s->uOrder-1,s->uKnot.values);
			s->uKnot.setDefault(FALSE);  s->uKnot.OnChanged();	
			Reporter.Trace("NurbsSurface:Updated uKnot vector ");
		}
		if (s->vKnot.getNum() != (s->vDimension + s->vOrder))  {
			int m=s->vDimension + s->vOrder;
			s->vKnot.setLength(m);
			ComputeKnotVector(s->vDimension-1, s->vOrder-1,s->vKnot.values);
			s->vKnot.setDefault(FALSE);  s->vKnot.OnChanged();	
			Reporter.Trace("NurbsSurface:Updated vKnot vector ");
		}
		if (s->controlPoint.getNum() != (s->uDimension * s->vDimension) )
		{
			int m=s->uDimension + s->vDimension;
			s->controlPoint.setLength(m);
			s->controlPoint.setDefault(FALSE);  s->controlPoint.OnChanged();	
			Reporter.Trace("NurbsSurface:Updated length of control points");

		}
		if (s->weight.getNum() != (s->uDimension * s->vDimension) )
		{
			int m=s->uDimension + s->vDimension;
			if (s->weight.getNum() >0) {
				s->weight.setLength(m);
				s->weight.setDefault(FALSE);  s->weight.OnChanged();	
				Reporter.Trace("NurbsSurface:Updated length of weights ");
			}
		}

		}

		
		GShell *s = ((GvNodeShell *) n)->theShell;
		if (s != (GvNode *) NULL) {
		Reporter.Trace("Geometry Node info %s #coords %d, vertexColors %d, vertexTextureCoords %d, flat shaded : %d "
			,s->ClassName()
			,s->v.Length()
			,(s->vc.Length() > 0)
			,(s->vp.Length() > 0)
			,s->Flat()
		);
		Reporter.Trace("Convex Faces : %d, Number Faces %d, Number Triangle %d, FaceColors %d, TriangleStrips computed %d "
			,(s->flags & SHELL_HAS_CONVEX_FACES) != 0
			,s->GetFaceCount()
			,s->f.GetTriangleCount()
			,(s->fc.Length() > 0)
			,(s->prims.Length()> 0)
			);
		/* 	
		BBox box;
		s->ComputeBBox(box);
		CstringFromPoint(d.m_bbox_min,box.mi);
		CstringFromPoint(d.m_bbox_max,box.ma);
		*/


		}

		ok=1;
		if (ok) OnNodeExpandfields();
	} // shell 
	else if (RTISA(n,GvNurbsCurve)) {
		{
		GvNurbsCurve *s = (GvNurbsCurve *) n;
		int dimension = s->controlPoint.getNum();
		if (s->knot.getNum() != (dimension + s->order))  {
			int m=(dimension + s->order);
			s->knot.setLength(m);
			ComputeKnotVector(dimension-1, s->order-1,s->knot.values);
			s->knot.setDefault(FALSE);  s->knot.OnChanged();	
			Reporter.Trace("NurbsCurve:Updated knot vector ");
		}
		if (s->weight.getNum() != (dimension) )
		{
			int m=dimension;
			if (s->weight.getNum() >0) {
				s->weight.setLength(m);
				s->weight.setDefault(FALSE);  s->weight.OnChanged();	
				Reporter.Trace("NurbsCurve:Updated length of weights ");
			}
		}

		}

		ok=1;
		if (ok) OnNodeExpandfields();
	} // shell 
	else if (RTISA(n,GvNurbsCurve2D)) {
		{
		GvNurbsCurve2D *s = (GvNurbsCurve2D *) n;
		int dimension = s->controlPoint.getNum();
		if (s->knot.getNum() != (dimension + s->order))  {
			int m=(dimension + s->order);
			s->knot.setLength(m);
			ComputeKnotVector(dimension-1, s->order-1,s->knot.values);
			s->knot.setDefault(FALSE);  s->knot.OnChanged();	
			Reporter.Trace("NurbsCurve:Updated knot vector ");
		}
		if (s->weight.getNum() != (dimension) )
		{
			int m=dimension;
			if (s->weight.getNum() >0) {
				s->weight.setLength(m);
				s->weight.setDefault(FALSE);  s->weight.OnChanged();	
				Reporter.Trace("NurbsCurve:Updated length of weights ");
			}
		}

		}

		ok=1;
		if (ok) OnNodeExpandfields();
	} // shell 
	else
	if (RTISA(n,GvNodeShell)) {
		GShell *s = ((GvNodeShell *) n)->theShell;
		if (s != (GvNode *) NULL) {
		Reporter.Trace("Geometry Node info %s #coords %d, vertexColors %d, vertexTextureCoords %d, flat shaded : %d "
			,s->ClassName()
			,s->v.Length()
			,(s->vc.Length() > 0)
			,(s->vp.Length() > 0)
			,s->Flat()
		);
		Reporter.Trace("Convex Faces : %d, Number Faces %d, Number Triangle %d, FaceColors %d, TriangleStrips computed %d "
			,(s->flags & SHELL_HAS_CONVEX_FACES) != 0
			,s->GetFaceCount()
			,s->f.GetTriangleCount()
			,(s->fc.Length() > 0)
			,(s->prims.Length()> 0)
			);
		/* 	
		BBox box;
		s->ComputeBBox(box);
		CstringFromPoint(d.m_bbox_min,box.mi);
		CstringFromPoint(d.m_bbox_max,box.ma);
		*/


		}

		ok=1;
	} // shell 
	else 
	if (RTISA(n,GvTexture)) {
			GvTexture *t = (GvTexture *) n;
			Reporter.Trace("Texture info  : Defined : %d, alpha %d, color key %d gray scale %d ", 
				t->defined,t->texture.HasAlpha(),t->texture.HasColorKey(),t->texture.IsGrayscale());
		ok = 1;
	}


	if (!ok) Reporter.Warning("Can´t do convert operation on this object");
	else OnChanged();

	
}

void DTreeEditor::OnUpdateNodeCheck(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
