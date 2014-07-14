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
#ifndef _GEdit_h
#define _GEdit_h
/******************************************************************************
@doc

@module GEdit.h - GLView edit event handler classes|

Copyright (c) 1996	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GTransformPathHandler>

Notes:

Changes:

Todo :

******************************************************************************/

typedef enum {
		PATH_TOP,
		PATH_TOP_DEF,
		PATH_BOTTOM_DEF,
		PATH_BOTTOM,
		PATH_BOTTOM_SHAPE,
		PATH_BOTTOM_SHAPE_SINGLE,

		// extended 
		PATH_MATERIAL,
		PATH_TEXTURE,
		PATH_APPEARANCE,
		PATH_GEOMETRY,
		PATH_FACE,
		PATH_EDGE,
		PATH_VERTEX

	} GPathMode;

const char *toString(GPathMode mode);
GPathMode fromString(const char *mode,GPathMode m);



class DTreeEditor;


class GEditController {

public:

	GEditController(GView *view_);
	~GEditController();


	GvSFNode defaultAppearance;
	GvSFNode defaultMaterial;
	GvSFNode defaultTexture;

	GvSFNode defaultObject;

	GvSFNode highlightApperance;

	GvMFNode highlight;


	void OnSelectionPath(GPath &path,int upto);
	void OnNodeTransformed(GvNode *node);

	void OnNodeChanged(GvNode *node);
	
	void OnNodeInserted(GvNode *node);
	void OnNodeDeleted(GvNode *node);



	void hightlightNode(GvPath &path,GvNode *node,GvBool add=FALSE);
	// tree editor

	// create a highlight geometry 
	GvNode *createHightlightGeometry(const Matrix &parentM, Gv2Transform *boxTransform,const BBox &bbox);

	// view 

	GView *view;

	DTreeEditor *treeEditor;

	void SetView(GView *v) { view = v;}

	void SetTreeEditor(DTreeEditor *d) { treeEditor = d; }

//	void SetMaterialEditor();


	// the copy buffer 

	GvMFNode  copyBuffer; 
	GvField		*copyBufferField;

	void setCopyBuffer(GvNode *node) { copyBuffer.set(node); }
	
	void setCopyBuffer(GvMFNode &nodes) { copyBuffer.set(nodes); }

	// with check for material / app etc.
	void setCopyBufferEx(GvNode *node);

	// is the copy buffer empty 
	gbool copyBufferEmpty() const { return copyBuffer.getNum() == 0; }

	// return the contents of the copy buffer as single node
	GvNode* getCopyBufferAsNode() {
			int l = copyBuffer.getNum();
			if (l== 0) return NULL;
			if (l == 1) return copyBuffer.get1(0);
			Gv2Group* g= new Gv2Group();
			g->children.set(copyBuffer);
			return g;
	}
	
	// return a copy of the copybuffer 
	GvNode* copyCopyBuffer() { // to do: deep copy 
				int l = copyBuffer.getNum();
				if (l== 0) return NULL;
				if (l == 1) { 
					GvNode* n= copyBuffer.get1(0);
					if (!n) return n;
					return n->copy(NULL);
				}
				Gv2Group* g= new Gv2Group();
				g->children.set(copyBuffer);
				return g;
	}

	void getCopyBuffer(GvMFNode &nodes) { nodes.set(copyBuffer); }
	void appendCopyBuffer(GvMFNode &nodes) { nodes.add(&copyBuffer); }
	void getCopyBuffer(GvSFNode &node) { node.set(getCopyBufferAsNode()); }


	// helpers
	// add a child node smartly to node
	GvBool addChild(GvNode *node,GvNode *child); 
	// remove a child node smartly to node
	GvBool removeChild(GvNode *node,GvNode *child); 


	// selection


    GPathMode defaultPathMode;


};



class	DTransformPosition;

class GTransformPathHandler : public GPathHandler {
public:
	RT(GTransformPathHandler);


	static GPathMode defaultPathMode;
	static GvBool defaultPathDeep;


	GTransformPathHandler(GView *View =NULL) : GPathHandler(View) 
			{ 
				mapMode = MAP_SCREEN; 
				transformMode = MODE_TRANSLATE;
				dialog = NULL; 
				transformField=NULL;
				scaleUniform = TRUE;
				rotateTrackball = TRUE;
				axes=1+2; // xy 
				mapPlane.SetZ();
				pathElement = 0;
				pathMode = defaultPathMode;
				pathDeep = defaultPathDeep;
			}

	~GTransformPathHandler();

	MapMode mapMode;
	Plane mapPlane;

	GPathMode pathMode; // selection heuristic 
	GvBool pathDeep;    // allow editing accross inline /protoinstance
	int pathElement; 

	// backpointer to dialog 
	DTransformPosition *dialog;


	enum TransformMode {
				MODE_TRANSLATE,
				MODE_CENTER,
				MODE_SCALE,
				//MODE_RADIUS,
				MODE_SCALE_ORIENTATION,
				MODE_ROTATE
				};

	// type of operation 
	TransformMode transformMode;

	// uniform scale flag
	BOOL scaleUniform;

	// trackball mode rotation 
	BOOL rotateTrackball;


	// dragging axis 
	int axes;


	void SetMapMode(int mode) { if (mode >= MAP_SCREEN && mode <= MAP_LOCAL) mapMode = (MapMode) mode; }
	void SetPathMode(int mode) { if (mode >= PATH_TOP && mode <= PATH_BOTTOM) pathMode = (GPathMode) mode; }
	void SetTransformMode(int mode) { if (mode >= MODE_TRANSLATE && mode <= MODE_ROTATE) transformMode = (TransformMode) mode; }
	void SetDialog(DTransformPosition *d) { dialog = d; }
	void SetAxes(int a) { axes = a;}

	// convert a delta value to a scale value
	void Delta2Scale (BOOL uniform,Point &p); 

	// filter coordinates of p, all coordinates with axis bit not set, will get the def value
	void AxisMap (int axis,Point &p,float def); 


	// map coordinates 
	BOOL MapPoint(const Point &screen, Point &world, Point &local);


	// points to VRML 2.0 transform node
	GvNodeHandle transform;
	GvField		*transformField;

	// get the transform Node 
	Gv2Transform * GetTransform() const { return (Gv2Transform*) (GvNode*) transform; }

	// set pointer to field in node to be transformed 
	// in VRML 1.0 search a transformable childNode up to index
	BOOL SetTransformField(TransformMode mode,GvGroup *group,int upToChild);

	BOOL SetTransformField(TransformMode mode);
	GvField * GetTransformField() { return(transformField); }

	// translate translation field of  node by delta 
	BOOL Translate(const Point &delta);

	// scale scale field of Transform node by delta 
	BOOL Scale(const Point &delta);

	// rotate rotate field of node by delta 
	BOOL Rotate(const Quaternation &delta);


	// update value in dialog 
	void UpdateDialogValue();

	// update value in dialog 
	void UpdateDialogValue(const char *value)
			#ifndef _GLVIEW_DLL		
				;
			#else
				{}
			#endif

	// update current field value with string value from dialog 
	BOOL SetValue(const char *valueString);



	// insert a new transform node, in path at position i
	BOOL InsertTransform(GvPath &p,int i);

	// initialize path to innermost Transform node of path 
	BOOL InitPath();

	// Search next higher Transform in path 
	BOOL InitPathUp();

	// Search next lower Transform in path 
	BOOL InitPathDown();

};


//
// GMoveTransformHandler : event handler for transforming VRML 2.0 Transform nodes 
//

class GMoveTransformHandler : public GTransformPathHandler {
public:
	RT(GTransformPathHandler);

	GMoveTransformHandler(GView *View =NULL) : GTransformPathHandler(View) {  }
	~GMoveTransformHandler();

	Point lastS; // last point screen
	Point lastO; // last point object 	

	Point centerS; // trackball center screen
	float radiusS; // trackball radius sceen


	int OnLButtonDown(UINT nFlags, Point &point);
	int OnLButtonDblClk(UINT nFlags, Point &point);
	int OnMouseMove(UINT nFlags,  Point &point);
	int OnLButtonUp(UINT nFlags,  Point &point);


};


//
// GNewObjectHandler: event handler for inserting new objects 
//


class GNewObjectHandler : public GTransformPathHandler {
public:
	RT(GNewObjectHandler);

	GNewObjectHandler(GView *View =NULL) : GTransformPathHandler(View) , initialScale(1,1,1) { 
		copyObject = FALSE; initialScale.setDefault(TRUE);}
	~GNewObjectHandler();


	// the group  / GvScene to add the new object
	GvNodeHandle group;

	// the object to insert on each mouse click
	GvNodeHandle object;

	BOOL SetObject(GvNode *node) { object = node; return(TRUE); }

	BOOL SetObjectType(const char *s);

	// flag to copy object on each new instance
	BOOL copyObject;

	GvSFVec3f initialScale;
	GvSFRotation initialRotation;


	// add the new object 
	BOOL NewObject(const Point &position);

	int OnLButtonDown(UINT nFlags, Point &point);
	int OnLButtonDblClk(UINT nFlags, Point &point);
	int OnMouseMove(UINT nFlags,  Point &point);
	int OnLButtonUp(UINT nFlags,  Point &point);

};


#endif


