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
/******************************************************************************
@doc

@module GEdit.cpp - GLView edit event handler classes implementation|

Copyright (c) 1996	by Holger Grahn
All rights reserved

Purpose:

Classes:
<c GEventHandler>
<c GView>

Notes:

Changes:

Todo :

******************************************************************************/
#include "stdafx.h"


#include "gcamera.h"
#include "quaternation.h"

#include "grender.h"
#include "greporter.h"

#include "gmodel.h"

#ifdef _GV
// VRML 
#include <gvscene.h>
#include <gvnodes.h>
#include <gvsensors.h>
#include <gvproto.h>

#include <gvtraverse.h>

// VRML 1.0 
#include <gvtransform.h>
#include <gvscale.h>

#endif

#include "grayselect.h"
#include "gfindnode.h"


#include "gevent.h"
#include "gedit.h"

#if !defined(_GLVIEW_DLL)
#include "glviewhdr.h"


class DTreeEditor;

#include "DTreeEditor.h"

#endif

class GvHighlightInfo {
public:

};

class GvNodeHighlightInfo : public GvHighlightInfo {

};

GEditController::GEditController(GView *view_) : view(view_),treeEditor(NULL),copyBuffer(0) 
{
} 

GEditController::~GEditController()
{
} 

void GEditController::setCopyBufferEx(GvNode *node) { 
	copyBuffer.set(node); 
	if (node == NULL) return;
	if (RTISA(node,GvAppearance)) {
		defaultAppearance.set(node);
	}
	if (RTISA(node,Gv2Material)) {
		defaultMaterial.set(node);
	}
	if (RTISA(node,GvTexture)) {
		defaultTexture.set(node);
	}
}

// add a child node smartly to node
GvBool GEditController::addChild(GvNode *node,GvNode *child) 
{ 
	int ret = FALSE;
	if (node == child) return FALSE;
	if (child == NULL) return FALSE;

		if (RTISA(node,GvGroup)) {   // add to group
			    GvGroup *group= (GvGroup*) node;
				group->addChild(child);
				ret = TRUE;
		} 
		else if (RTISA(node,Gv2Group)) {
			    Gv2Group *group= (Gv2Group*) node;
				group->addChild(child);
				ret = TRUE;
		}
		else if (RTISA(node,GvAppearance)) {
			    GvAppearance *group= (GvAppearance*) node;
				ret=group->addChild(child);

		}
		else if (RTISA(node,GvShape)) {
			    GvShape *group= (GvShape*) node;
				ret=group->addChild(child);

		}
		else if (RTISA(node,Gv2IndexedLineSet)) {
			    Gv2IndexedLineSet *group= (Gv2IndexedLineSet*) node;
				ret=group->addChild(child);

		}
		else if (RTISA(node,Gv2IndexedFaceSet)) {
			    Gv2IndexedFaceSet *group= (Gv2IndexedFaceSet*) node;
				ret=group->addChild(child);

		}
		else if (RTISA(node,Gv2PointSet)) {
			    Gv2PointSet *group= (Gv2PointSet*) node;
				ret=group->addChild(child);

		}
		else if (RTISA(node,GvElevationGrid)) {
			    GvElevationGrid *group= (GvElevationGrid*) node;
				ret=group->addChild(child);

		}

		return(ret!=0);
}

// remove a child node smartly to node
GvBool GEditController::removeChild(GvNode *node,GvNode *child) 
{ 
	int ret = FALSE;
	if (node == child) return FALSE;
	if (child == NULL) return FALSE;
/*
		if (RTISA(node,GvGroup)) {   // add to group
			    GvGroup *group= (GvGroup*) node;
				int index = group->findChild(child)
				group->removeChild(child);
				ret = TRUE;
		} 
		else 
*/ 
		if (RTISA(node,Gv2Group)) {
			    Gv2Group *group= (Gv2Group*) node;
				ret = group->children.remove(child);
				if (ret) group->children.OnChanged();

		}
		return(ret!=0);
}


// create a highlight geometry 
GvNode *GEditController::createHightlightGeometry(const Matrix &parentM, Gv2Transform *boxTransform,const BBox &bbox)
{
	GvBox *box = new GvBox();
	box->size.set(bbox.Size());
	GvShape *boxShape = new GvShape();
	Gv2Transform *boxCenterTransform = new Gv2Transform();
	Gv2MatrixTransform *rootTransform = new Gv2MatrixTransform();
	
	rootTransform->matrix.set(parentM);

	boxShape->geometry.set(box);
	boxShape->appearance.set((GvNode*) highlightApperance);

	boxCenterTransform->children.add(boxShape);
	boxCenterTransform->translation.set(-bbox.Center());
	boxTransform->children.add(boxCenterTransform);
	rootTransform->children.add(boxTransform);
	return rootTransform;
}





// GTransformPathHandler 
//
RTIMP(GTransformPathHandler,0,GPathHandler);


GPathMode GTransformPathHandler::defaultPathMode=PATH_TOP;
GvBool GTransformPathHandler::defaultPathDeep=FALSE;


GTransformPathHandler::~GTransformPathHandler()
{
}

GvBool hasName(GvNode *n) {
	//return n->getName().getLength()>0;
	const GvName &name =  n->getName();

	return (name.getLength()>0 && name.getString()[0]!='_');
}


// insert a new transform node, in path at position i
BOOL GTransformPathHandler::InsertTransform(GvPath &p,int i)
{
	if (i<=0) return FALSE;

	if (p.path[i-1].field !=NULL) {
		// insert new Transform node and move shape into children 
		Gv2Transform *t= new Gv2Transform();
		t->children.add(p.path[i].node);

		if (RTISA(p.path[i-1].field,GvMFNode)) {
			GvMFNode *f= (GvMFNode *) p.path[i-1].field;
			f->set1(p.path[i-1].index,t);
		}
		else if (RTISA(p.path[i-1].field,GvSFNode)) {
			GvSFNode *f= (GvSFNode *) p.path[i-1].field;
			f->set(t);
		}

		// to do move path up 
		p.path[i].node = t;
		p.path[i].field = &t->children;
		p.path[i].index = 0;

	    if (view->pView->dialogs.m_dTreeEditor != NULL)
			view->pView->dialogs.m_dTreeEditor->UpdateNode(p,i-1);

		return TRUE;

	}
	else return FALSE;
}

#if 1

BOOL GTransformPathHandler::InitPath()
{
  path.Flush();

  GvPath &p = lastHit.path;
  int l = p.path.Length();

  BOOL isSep = FALSE;

  int theElement = pathElement; // means take the (nth -1 object)
  
  int i;
  int maxPath=-1; // index of last ok element in path
  int bestElement = -1;

  // limit path up to first Inline or protoInstance 
  if (!pathDeep) {
	  i=1;

	  while (i<l) {
		if (RTISA(p.path[i].node,Gv2Inline)) {
			maxPath = i;
			break;
		}
		else if (RTISA(p.path[i].node,GvProtoInstance)) {
			maxPath = i;
			break;
		}
		i++;
	  }
  }

  if (pathMode>= PATH_BOTTOM_DEF) { 	// Find inner most Transform in path
	theElement = - theElement;
	i=l-1;
	if (maxPath >0 ) i = maxPath;
	
	while (i>=0) {		
	  if (RTISA(p.path[i].node,Gv2Transform)) {
		  bestElement = i;
		  theElement--;
		  if (pathMode == PATH_BOTTOM_DEF) {
			  if (hasName(p.path[i].node)) break;
		  } else if (theElement <=0) break;
	  }	
	  else if (RTISA(p.path[i].node,GvSeparator)) {
		  bestElement = i;
		  theElement--;
		  if (pathMode == PATH_BOTTOM_DEF) {
			  if (hasName(p.path[i].node)) break;
		  } 
		  else if (theElement <=0) break;
	  }	
	  else if (pathMode >= PATH_BOTTOM_SHAPE && (RTISA(p.path[i].node,GvShape) || i==maxPath)) {
			if (i>0) {
				GvBool newTransform= FALSE;

				if (pathMode== PATH_BOTTOM_SHAPE_SINGLE) {
					// we only want to have transform with single shape 
					if (RTISA(p.path[i-1].field,GvMFNode)) {
						GvMFNode *f= (GvMFNode *) p.path[i-1].field;
						newTransform = (f->getNum()>1);
					}
				}

				if (!newTransform && RTISA(p.path[i-1].node,Gv2Transform)) {
					i--; break;
				}
				else 
				if (p.path[i-1].field !=NULL) {

					InsertTransform(p,i);

					break;
				}

			}
	  } // BOTTOM_SHAPE
	  i--;
	}		
	if (i<0) {
		if (bestElement >=0) i=bestElement;
		else return FALSE;
	}
  } else  {  // Find outer most Transform in path
	i=0;
	
	if (maxPath>0) l= maxPath+l;

	while (i<l) {		
	  if (RTISA(p.path[i].node,Gv2Transform)) {
		  bestElement = i;
		  theElement--;
		  if (pathMode == PATH_TOP_DEF) {
			  if (hasName(p.path[i].node)) break;
		  } 
		  else if (theElement <=0) break;
	  }	
	  else if (RTISA(p.path[i].node,GvSeparator)) {
		  bestElement = i;
		  theElement--;
		  if (pathMode == PATH_TOP_DEF) {
			  if (hasName(p.path[i].node)) break;
		  } 
		  else  if (theElement <=0) break;
	  }	
	  i++;
	}		

	if (i>=l){
		if (bestElement >=0) i=bestElement;
		else {
			if (l >1) { // insert a Transfrom here 
				i=1;
				InsertTransform(p,i);
			}
			else 
			  return FALSE;
		}
	}
  }	

  isSep = (RTISA(p.path[i].node,GvSeparator));

  // create path from root to Transform (not included)
  path.path.SetMaxLength(i);

  for(l=0; l<i; l++) {
	  //TRACE("%s \n",p.path[i].node->ClassName());
	  path.Append(p.path[l]);

  }	

  if (isSep) {
	  path.Append(p.path[i]);

	  SetTransformField(transformMode,(GvGroup *) (GvNode*)p.path[i].node,p.path[i].index);
	  // get matrix of path 
	  Matrix mm;
	  if (path.getMatrix(mm))
		  m.Set(mm);
	  else m.SetIdentity();
  } else {	
  
	transform = p.path[i].node;
  // get matrix of path 
	Matrix mm;
	if (path.getMatrix(mm))
		  m.Set(mm);
	else m.SetIdentity();
	SetTransformField(transformMode);
  }

  if (view->pView->dialogs.m_dTreeEditor != NULL)
	view->pView->dialogs.m_dTreeEditor->SetSelection(p,i);


  return(TRUE);
}

#else
BOOL GTransformPathHandler::InitPath()
{
  path.Flush();

  GvPath &p = lastHit.path;
  int l = p.path.Length();

  BOOL isSep = FALSE;

  int theElement = pathElement;
  
  int i;

  if (theElement <0) { 	// Find inner most Transform in path
	theElement = - theElement;
	i=l-1;
	while (i>=0) {		
	  if (RTISA(p.path[i].node,Gv2Transform)) {
		  theElement--;
		  if (theElement <=0) break;
	  }	
	  else if (RTISA(p.path[i].node,GvSeparator)) {
		  isSep = TRUE;
		  theElement--;
		  if (theElement <=0) break;
	  }	
	  i--;
	}		
	if (i<0) return FALSE;
  } else  {  // Find outer most Transform in path
	i=0;
	while (i<l) {		
	  if (RTISA(p.path[i].node,Gv2Transform)) {
		  theElement--;
		  if (theElement <=0) break;
	  }	
	  else if (RTISA(p.path[i].node,GvSeparator)) {
		  theElement--;
		  isSep = TRUE;
   	      if (theElement <=0) break;
	  }	
	  i++;
	}		
	if (i>=l) return FALSE;
  }	



  // create path from root to Transform (not included)
  path.path.SetMaxLength(i);

  for(l=0; l<i; l++) {
	  //TRACE("%s \n",p.path[i].node->ClassName());
	  path.Append(p.path[l]);

  }	

  if (isSep) {
	  path.Append(p.path[i]);

	  SetTransformField(transformMode,(GvGroup *) (GvNode*)p.path[i].node,p.path[i].index);
	  // get matrix of path 
	  Matrix mm;
	  if (path.getMatrix(mm))
		  m.Set(mm);
	  else m.SetIdentity();
  } else {	
  
	transform = p.path[i].node;
  // get matrix of path 
	Matrix mm;
	if (path.getMatrix(mm))
		  m.Set(mm);
	else m.SetIdentity();
	SetTransformField(transformMode);
  }


  return(TRUE);
}

#endif

// set pointer to field in node to be transformed 
BOOL GTransformPathHandler::SetTransformField(TransformMode mode)
{

	GvNode *node = GetTransform();

	transformField = NULL;
		
	if (!node) return FALSE;
	// VRML 2.0
	if (RTISA(node,Gv2Transform)) {
		Gv2Transform *t = (Gv2Transform*) node;

		switch(transformMode) {
		case MODE_TRANSLATE :
				transformField = &t->translation;
				break;
		case MODE_CENTER :
				transformField = &t->center;
				break;
		case MODE_SCALE :
				transformField = &t->scale;
				break;
		case MODE_ROTATE :
				transformField = &t->rotation;
				break;
		case MODE_SCALE_ORIENTATION :
				transformField = &t->scaleOrientation;
				break;
		}
	} else
// VRML 1.0 support
	if (RTISA(node,GvTransform)) {
		GvTransform *t = (GvTransform*) node;

		switch(transformMode) {
		case MODE_TRANSLATE :
				transformField = &t->translation;
				break;
		case MODE_CENTER :
				transformField = &t->center;
				break;
		case MODE_SCALE :
				transformField = &t->scaleFactor;
				break;
		case MODE_ROTATE :
				transformField = &t->rotation;
				break;
		case MODE_SCALE_ORIENTATION :
				transformField = &t->scaleOrientation;
				break;
		}
	} else
	if (RTISA(node,GvScale)) {
		GvScale *t = (GvScale*) node;

		switch(transformMode) {
		case MODE_SCALE :
				transformField = &t->scaleFactor;
				break;
		}
	} else
	if (RTISA(node,GvTranslation)) {
		GvTranslation *t = (GvTranslation*) node;

		switch(transformMode) {
		case MODE_TRANSLATE :
				transformField = &t->translation;
				break;
		}
	} else
	if (RTISA(node,GvRotation)) {
		GvRotation *t = (GvRotation*) node;

		switch(transformMode) {
		case MODE_ROTATE :
				transformField = &t->rotation;
				break;
		}
	} else
	// MatrixTransform ???
// end vrml 1.0
	;


	return(transformField != NULL);

}


// set pointer to field in node to be transformed 
// in VRML 1.0 search a transformable childNode up to index
BOOL GTransformPathHandler::SetTransformField(TransformMode mode,GvGroup *group,int upToChild)
{


	transformField = NULL;
		
	if (!group) return FALSE;

	int maxi = group->getNumChildren();

	if (upToChild<maxi && upToChild>=0) maxi = upToChild;

	GvNode *theNode = NULL;
	int i=0;

	while (theNode != NULL && i<maxi) {
		GvNode *node;

		node = group->getChild(i);

		if (RTISA(node,GvTransform)) {
			theNode =  node;
		} else
		if ( mode == MODE_SCALE && RTISA(node,GvScale)) {
			theNode = node;
		} else
		if ( mode == MODE_TRANSLATE && RTISA(node,GvTranslation)) {
			theNode = node;
		} else
		if ( mode == MODE_ROTATE && RTISA(node,GvRotation)) {
			theNode = node;
		}
		else if (RTISA(node,GvNodeShell) || RTISA(node,GvGroup) || RTISA(node,GvLight)) {
			break;
		}			

	}

	if (theNode) {
		transform = theNode;
		return(SetTransformField(mode));
	} else {
		// create a new transform
		theNode = new GvTransform();
		group->insertChild(0,theNode);
		transform = theNode;
		return(SetTransformField(mode));
	}

	return(transformField != NULL);

}


// Search next higher Transform in path 
BOOL GTransformPathHandler::InitPathUp()
{

  GvPath &p = lastHit.path;
  int l = path.Length(); // start search at current path lenght

  // Find inner most Transform in path
  int i;
  i=l-1;
  while (i>=0) {		
	  if (RTISA(p.path[i].node,Gv2Transform)) {
		  break;
	  }	
	  i--;
  }		
  if (i<0) return FALSE;


  path.Flush();

  // create path from root to Transform (not included)
  path.path.SetMaxLength(i);

  for(l=0; l<i; l++) {
	  TRACE("%s \n",p.path[i].node->ClassName());
	  path.Append(p.path[l]);

  }	
  
  transform = p.path[i].node;

  // get matrix of path 
  Matrix mm;
  if (path.getMatrix(mm))
	  m.Set(mm);
  else m.SetIdentity();

  return(TRUE);
}

// Search next lower Transform in path 
BOOL GTransformPathHandler::InitPathDown()
{

  GvPath &p = lastHit.path;
  int l = path.Length()+1; // start search at current path lenght

  // Find inner most Transform in path
  int i;
  i=l-1;
  while (i<p.Length()) {		
	  if (RTISA(p.path[i].node,Gv2Transform)) {
		  break;
	  }	
	  i++;
  }		
  if (i>=p.Length()) return FALSE;


  path.Flush();

  // create path from root to Transform (not included)
  path.path.SetMaxLength(i);

  for(l=0; l<i; l++) {
	  TRACE("%s \n",p.path[i].node->ClassName());
	  path.Append(p.path[l]);
  }	
  
  transform = p.path[i].node;

  // get matrix of path 
  Matrix mm;
  if (path.getMatrix(mm))
	  m.Set(mm);
  else m.SetIdentity();

  return(TRUE);
}




BOOL GTransformPathHandler::Translate(const Point &delta)
{

		if (!transformField) return FALSE;

		ASSERT(RTISA(transformField,GvSFVec3f));
		
		GvSFVec3f *t = (GvSFVec3f *) transformField;
		
		Point current;
		t->get(current);
		current += delta;
		t->set(current);

		return(TRUE);

}

BOOL GTransformPathHandler::Scale(const Point &delta)
{

		if (!transformField) return FALSE;

		ASSERT(RTISA(transformField,GvSFVec3f));
		
		GvSFVec3f *t = (GvSFVec3f *) transformField;
		
		Point current;
		t->get(current);
		current *= delta;
		t->set(current);

		return(TRUE);
}


BOOL GTransformPathHandler::Rotate(const Quaternation &delta)
{

		if (!transformField) return FALSE;

		ASSERT(RTISA(transformField,GvSFRotation));
		
		GvSFRotation *t = (GvSFRotation *) transformField;
		
		Quaternation current;
		t->get(current);
		current = current * delta;
		t->set(current);

		return(TRUE);

}





//
// GMoveTransformHandler 
//
RTIMP(GMoveTransformHandler,0,GTransformPathHandler);
GMoveTransformHandler::~GMoveTransformHandler()
{
}


int GMoveTransformHandler::OnLButtonDown(UINT nFlags, Point &point)
{
	
	TRACE("%s:OnLButtonDown (%f %f %f) %x \n",this->ClassName(),point.x,point.y,point.z,nFlags);
	firstPoint = point;
	firstFlags = nFlags;
	RayRange ray;
	view->ComputeWorldRay(point,ray);
	if (ComputeRayHit(ray)) {
		if (InitPath()) { 
			SetState(1);

			// get starting point in view and local object coordinate 
			view->viewInfo.World2View(lastHit.pw,lastS);
			World2Local(lastHit.pw,lastO);
			// compute center for trackball
			{ Point p = lastHit.bbox.Center();
				p= lastHit.m  * p;
				view->World2Screen(p,centerS);
			}
			UpdateDialogValue();

			return(EV_OK);
		} else {
			Reporter.Error("Can´t edit selected object, because not contained in Transform group. "); 
			return(EV_CANCEL);
		}
	}
	else return(EV_CANCEL);
}

// map input ps point to world pw and local coordinates
// do plane projection if appropriate mapping mode 
BOOL GTransformPathHandler::MapPoint(const Point &ps /* screen */ ,Point &pw /* world */,Point &pl /* local */) 
{

    view->viewInfo.View2World(ps,pw);
	
	if (mapMode == MAP_WORLD) {
		RayRange ray;
		view->ComputeWorldRay(ps,ray);
		float t;
		if (mapPlane.Intersect(ray.p,ray.d,t))
		   pw = ray.Eval(t);

	} else
	if (mapMode == MAP_LOCAL) {
		RayRange ray;
		view->ComputeWorldRay(ps,ray);
		// transform ray to local space 
		ray *= m.Inverse();

		float t;
		if (mapPlane.Intersect(ray.p,ray.d,t)) {
		   pl = ray.Eval(t);
		   // pw = m.m * pl;
		   Local2World(pl,pw);
		}

	}

	World2Local(pw,pl);
	return(TRUE);

}

int GMoveTransformHandler::OnLButtonDblClk(UINT nFlags, Point &point)
{
	TRACE("%s:OnLButtonDblClk (%f %f %f) %x \n",this->ClassName(),point.x,point.y,point.z,nFlags);
	return(EV_DONE);
}

void GTransformPathHandler::Delta2Scale (BOOL uniform,Point &p) 
{
	
	if (uniform) {
		if (fabs(p.x) > fabs(p.y)) { // x>y
			if (fabs(p.x) > fabs(p.z)) 
				p.y=p.z= p.x;
			else 
				p.x=p.y= p.z;
		}
		else { // y > x
			if (fabs(p.y) > fabs(p.z)) 
				p.x=p.z= p.y;
			else 
				p.x=p.y= p.z;
		}
	}
	p *= 0.5;
	if (p.x<0)  p.x = 1.0 - -p.x; else p.x=1.0+p.x;
	if (p.y<0)  p.y = 1.0 - -p.y; else p.y=1.0+p.y;
	if (p.z<0)  p.z = 1.0 - -p.z; else p.z=1.0+p.z;

}

void GTransformPathHandler::AxisMap (int axis,Point &p,float def) 
{
	if (!(axis&1)) p.x = def;
	if (!(axis&2)) p.y = def;
	if (!(axis&4)) p.z = def;
}


int GMoveTransformHandler::OnMouseMove(UINT nFlags, Point &point)
{
	if (State()==1) { // mouse pressed 
		// map new point to same depth plane 
		point.z = lastS.z;
		Point pw,pl;

		MapPoint(point,pw,pl);
/*
        view->viewInfo.View2World(point,pw);
		World2Local(pw,pl);
*/
		
		Point delta = pl  - lastO;
		switch(transformMode) {
		case MODE_TRANSLATE :
			AxisMap (axes,delta,0.0f); 
			Translate(delta);
			break;

		case MODE_SCALE :
			Delta2Scale(scaleUniform,delta);
			AxisMap (axes,delta,1.0f); 
			Scale(delta);
			break;
		case MODE_ROTATE: 
			{
				Quaternation q;

				if (rotateTrackball) { 
//						q.SetTrackball(lastS.x-centerS.x,lastS.y-centerS.y,point.x-centerS.x,point.y-centerS.y);
						q.SetTrackball(point.x-centerS.x,point.y-centerS.y,lastS.x-centerS.x,lastS.y-centerS.y,0.4);
				} else {
					delta = point - lastS; // in screen units -1 .. 1 

					Point a(1,1,1);
					AxisMap (axes,a,0.0f); 
					double phi = 0.2 * delta.x * TWOPI;

					q.SetAxisRotation(a.x,a.y, a.z,phi);
				}
				Rotate(q);
			}
			break;

		default : 
			break;
		}
		
		UpdateDialogValue();

		view->Redraw();

		//TRACE("%s:OnMouseMove (%f %f %f) %x \n",this->ClassName(),delta.x,delta.y,delta.z,nFlags);

		lastS = point;
		lastO = pl;

	}
	return(EV_OK);
}

int GMoveTransformHandler::OnLButtonUp(UINT nFlags,  Point &point)
{
	TRACE("%s:OnLButtonUp (%f %f %f) %x \n",this->ClassName(),point.x,point.y,point.z,nFlags);
	SetState(2);
	return(EV_DONE);
}


// update value in dialog 
void GTransformPathHandler::UpdateDialogValue()
{
	if (!dialog) return;
	Gv2Transform *t = GetTransform();
		
	if (!t) return;
	
	GvField *field =  transformField;
	if (!field) return;

	GvString s;
	field->get(s);
	UpdateDialogValue(s.getString());
}


// update current field value with string value from dialog 
BOOL GTransformPathHandler::SetValue(const char *valueString)
{
	GvField *field =  transformField;
	if (!field) return FALSE;

	return field->set(valueString)>0;
	// UpdateDialogValue(s.getString());
}





//
// GNewObjectHandler 
//
RTIMP(GNewObjectHandler,0,GTransformPathHandler);

GNewObjectHandler::~GNewObjectHandler()
{
}

	// add the new object 
BOOL GNewObjectHandler::NewObject(const Point &position)
{
	Gv2Transform *t;
	GvNode *o=object;
	GvNode *g=group;

	if (o == (GvNode*) NULL) return FALSE;
	if (g == (GvNode*) NULL) return FALSE;


	// get the object
//	if (copyObject) 
	//	o = o->copy();
	if (o == (GvNode*) NULL) return FALSE;

	// create transform with object as child
	t = new Gv2Transform();
	t->translation.set(position);
	t->addChild(o);


	// add to container
	((Gv2Group*) g)->addChild(t);

	transform = t;
	transformField = &t->translation;

//		UpdateDialogValue();

	view->Redraw();

	return(TRUE);

}


int GNewObjectHandler::OnLButtonDown(UINT nFlags, Point &point)
{
	Point pw,pl;
	Point p ;
	view->World2Screen(GetCamera().target,p);
	point.z = p.z; // which depth plane ?? 
	MapPoint(point,pw,pl);

	if (nFlags & MK_SHIFT) { // find a hit on a point 
		RayRange ray;
		view->ComputeWorldRay(point,ray);
		if (ComputeRayHit(ray)) {
			pw = lastHit.pw;
			World2Local(pw,pl);
		}
	}
	if (NewObject(pl))
		return(EV_OK);
	else return(EV_CANCEL);
}

int GNewObjectHandler::OnLButtonDblClk(UINT nFlags, Point &point)
{
	TRACE("%s:OnLButtonDblClk (%f %f %f) %x \n",this->ClassName(),point.x,point.y,point.z,nFlags);
	return(EV_DONE);
}

int GNewObjectHandler::OnLButtonUp(UINT nFlags,  Point &point)
{
	//TRACE("%s:OnLButtonUp (%f %f %f) %x \n",this->ClassName(),point.x,point.y,point.z,nFlags);
	return(EV_DONE);

}
int GNewObjectHandler::OnMouseMove(UINT nFlags, Point &point)
{
	Point pw,pl;
	Point p ;
	view->World2Screen(GetCamera().target,p);
	point.z = p.z; // which depth plane ?? 
	MapPoint(point,pw,pl);
	CString s;
	s.Format("%g %g %g",pl.x,pl.y,pl.z);
	UpdateDialogValue(s);

//	TRACE("%s:OnMouseMove (%f %f %f) %x \n",this->ClassName(),point.x,point.y,point.z,nFlags);
	return(EV_OK);
}							


BOOL GNewObjectHandler::SetObjectType(const char *object)
{
  GvNode *g=NULL;
  GvShape *s=NULL;
  s = new GvShape();

  //g = view->GetLibraryObject(object);

  if (g) {
	  if (RTISA(g,GvNodeShell)) {
	  	  s->geometry.set(g);
	  } else {
		 delete s; s = NULL;
	  }	
  } else 	
  if (streq(object,"Box")) {
	  s->geometry.set(new GvBox());
  } 	
  else if (streq(object,"Cone")) {
	  s->geometry.set(new Gv2Cone());
  } 	
  else if (streq(object,"Cylinder")) {
	  s->geometry.set(new Gv2Cylinder());
  } 	
  else if (streq(object,"Sphere")) {
	  s->geometry.set(new Gv2Sphere());
  } 	
  if(g) SetObject(g);
  else SetObject(s);
  return(TRUE);
}
