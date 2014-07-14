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

@module gvnurbsinventor.cpp 
Copyright (c) 1998-1999	by blaxxun interactive - Holger Grahn & Tom Volk
All rights reserved

Purpose:

Classes:

Notes:

Changes:

$Revision: $
$Log: $




Todo :

******************************************************************************/

#include "stdafx.h"

#include "greporter.h"

#include <gvtraverse.h>

#include <gvmaterial.h>
#include <gvtexture2.h>
#include <gvtexture2transform.h>
#include "gshell.h"
#include "gshelli.h"
#include "ggeom.h"
#include "gvcoordinate3.h"
#include "grender.h"
#include "gmodel.h"
#include "gvnurbs.h"
#include <grayselect.h>

#include "gvnurbsinventor.h"
#include "gv2geometry.h"
#include "gvelement.h"
#include "gvmaterialbinding.h"
#include "gvshapehints.h"
#include "gvisibility.h"

#include "goptimizetraversal.h"

// get the error reporting object 
static GReporter* getReporter(GTraversal &state) 
{
	GView * view = state.getBrowser(); 
	ASSERT(view != NULL);
	if (!view) return GReporter::current; // return NULL;
	return view->GetReporter();
}


#ifdef _G_INVENTOR


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//						GvCoordinate4
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
GV_NODE_SOURCE_IMP(GvCoordinate4,GvNode);


GvCoordinate4::GvCoordinate4()
{
    GV_NODE_CONSTRUCTOR(GvCoordinate4);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(point);

    point.values[0] = point.values[1] = point.values[2] = 0.0;
}

GvCoordinate4::~GvCoordinate4()
{
}

																		 
int GvCoordinate4::Do(GTraversal &state)			
{									      
//	TRACE("%s push %s \n",state.ClassName(),this->ClassName());
	GvCoordinate3Element *current,*prev;									
	/* get current */												
	prev = (GvCoordinate3Element*) state.Get(GvCoordinate3Element::attrId); 
	if (!prev) { /* ignore it, because not registerd (initialized)	*/				
		return(0);															
	}	
	if (prev->level == state.level) { // already pushed
	   prev->data = this;
	   return(1);
	}																	
																			
	current = (GvCoordinate3Element *) prev->NewInstance();								
	current->data = this;													

	state.Push(GvCoordinate3Element::attrId,current);			
	return(1);																
}	



//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//						GvProfileCoordinate2
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

GV_NODE_SOURCE_IMP(GvProfileCoordinate2,GvNode);


GvProfileCoordinate2::GvProfileCoordinate2()
{
    GV_NODE_CONSTRUCTOR(GvProfileCoordinate2);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(point);

    point.values[0] = point.values[1] = 0.0;
}

GvProfileCoordinate2::~GvProfileCoordinate2()
{
}

																			 
int GvProfileCoordinate2::Do(GTraversal &state)			
{									      
//	TRACE("%s push %s \n",state.ClassName(),this->ClassName());
	GvProfileCoordElement *current,*prev;	
	
	/* get current */												
	prev = (GvProfileCoordElement*) state.Get(GvProfileCoordElement::attrId); 
	if (!prev) { /* ignore it, because not registerd (initialized)	*/				
		return(0);															
	}	
	if (prev->level == state.level) { // already pushed
	   prev->data = this;
	   return(1);
	}																	
																			
	current = (GvProfileCoordElement *) prev->NewInstance();								
	current->data = this;													

	state.Push(GvProfileCoordElement::attrId,current);			
	return(1);																
}


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//						ProfileCoordinate3
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

GV_NODE_SOURCE_IMP(GvProfileCoordinate3,GvNode);


GvProfileCoordinate3::GvProfileCoordinate3()
{
    GV_NODE_CONSTRUCTOR(GvProfileCoordinate3);
    isBuiltIn = TRUE;

    GV_NODE_ADD_FIELD(point);

    point.values[0] = point.values[1] = point.values[2] = 0.0;
}

GvProfileCoordinate3::~GvProfileCoordinate3()
{
}


																	 
int GvProfileCoordinate3::Do(GTraversal &state)			
{									      
//	TRACE("%s push %s \n",state.ClassName(),this->ClassName());
	GvProfileCoordElement *current,*prev;									
	/* get current */												
	prev = (GvProfileCoordElement*) state.Get(GvProfileCoordElement::attrId); 
	if (!prev) { /* ignore it, because not registerd (initialized)	*/				
		return(0);															
	}	
	if (prev->level == state.level) { // already pushed
	   prev->data = this;
	   return(1);
	}																	
																			
	current = (GvProfileCoordElement *) prev->NewInstance();								
	current->data = this;													

	state.Push(GvProfileCoordElement::attrId,current);			
	return(1);																
}



//////////////////////////////////////////////////////////////	
//															//	
//															//	
//				T H E   N U R B S S U R F A C E				//
//															//
//////////////////////////////////////////////////////////////
 
GV_NODE_SOURCE_IMP(GvNurbsSurfaceInventor,GvNode);


GvNurbsSurfaceInventor::GvNurbsSurfaceInventor() :


		//the inventor fields
        numUControlPoints(0),
        numVControlPoints(0),
        uKnotVector(0),
        vKnotVector(0),
		child(0)

{
    GV_NODE_CONSTRUCTOR(GvNurbsSurfaceInventor);
    isBuiltIn = FALSE;

	GV_NODE_ADD_FIELD(numUControlPoints);
    GV_NODE_ADD_FIELD(numVControlPoints);
 
    GV_NODE_ADD_FIELD(uKnotVector);
    GV_NODE_ADD_FIELD(vKnotVector);

    GV_NODE_ADD_FIELD(child); // private ?


	m_bInitialized = gfalse;
	m_bBadValues = gfalse;
}



GvNurbsSurfaceInventor::~GvNurbsSurfaceInventor()
{

}




int GvNurbsSurfaceInventor::Do(GTraversal &state)
{ 
	if (!m_bInitialized) {
		m_bInitialized = TRUE;
		if (!BuildNode(state))
		{
			m_bBadValues = TRUE;
			return(0);
		}
	}
	return child.Traverse(state);
}

int GvNurbsSurfaceInventor::Do(GglRenderTraversal &state)
{ 
	if (!m_bInitialized) {
		m_bInitialized = TRUE;
		if (!BuildNode(state))
		{
			m_bBadValues = TRUE;
			return(0);
		}
	}

#ifdef _G_VRML1
	
	{ // VRML 1.0 
	G_GETSTATE(state,matB,GvMaterialBinding,	GvMaterialBindingElement)
	G_GETSTATE(state,shapeHints,GvShapeHints,	GvShapeHintsElement)
	G_GETSTATE(state,texture2,GvTexture2,	GvTexture2Element)
	
	GvMaterial *material=NULL; 
	GvMaterialElement *materialTop = (GvMaterialElement*) state.Get(GvMaterialElement::attrId); 
	if (materialTop) material = (GvMaterial *) materialTop->data;

	int matStatus = 0;
	int texStatus =  0;

	if (texture2) {  // may change color !!
		if ( (texStatus = texture2->glSet(state))>0) {
		   // force reload color, if texture switch off
	       state.glState->currentBaseColorNode = (GvNode *) -1;
		   if (state.glState->texturing && state.glState->textureParameters.set_diffuse_white) {
 		 	   matStatus |= MATERIAL_SET_DIFFUSE_WHITE;
		   }
		}
	 }
										
	if (material)			   // GvMaterial
		material->Apply(state,matStatus,texStatus);
	}
	// shapehints ?
#endif
#ifdef _D3D
	if (!state.glState->dx6) state.glState->FlushState(128);  // got assertion in G3RenderState.FlushState_() !!
#endif


	return child.Traverse(state);
}

int GvNurbsSurfaceInventor::Do(GOptimizeTraversal &state)
{ 
	if (!m_bInitialized) {
		m_bInitialized = TRUE;
		if (!BuildNode(state))
		{
			m_bBadValues = TRUE;
			return(0);
		}
	}
	return child.Traverse(state);
		
}


//--------------------------------------------------------------------------------
//int GvNurbsSurfaceInventor::BuildNode(GTraversal &state)
//--------------------------------------------------------------------------------
int			
GvNurbsSurfaceInventor::BuildNode(GTraversal &state)
{

	bool bIsTrimmed = FALSE;
		//
		//setup the nurbs surface
		//
	GvNurbsSurface* surface = BuildNurbsSurface(state);
	ASSERT(surface);

	if (!surface) 
		return 0;//surface not defined


	//
	// check for trimming curves in the state
	//
	GvMFNode profileCurveStack(0);

	//
	//get all profile elements from the stack and put them into a mfnode
	//
	
	//get the first elment
	GvElement *top=  (GvElement*) state.Get(GvProfileCurveElement::attrId); 
	if (top->data) 
	{
		profileCurveStack.set(top->data);
		bIsTrimmed = TRUE;
	}
	top = (GvElement*)top->prevThisAttr;
	
		while (top)
			{
				profileCurveStack.add(top->data);	
				top = (GvElement*)top->prevThisAttr;
			}



	
	if (bIsTrimmed) //surface is trimmed
	{
		int count;
		count = profileCurveStack.getNum();

		//setup a trimmed surface node
		GvTrimmedSurface *trSurf = new GvTrimmedSurface();
		GvContour2D* currentContour = NULL;
		trSurf->surface = surface;
		
		//
		//step through the profile segments in the order as they appear in the file
		//and build the corresponding vrml nodes
		//
		for(int i=count-1; i>=0; i--)
		{
				GvNode* _profileSegment;
				_profileSegment = profileCurveStack.get1(i);
				ASSERT(_profileSegment);


				//
				//check the type of the node we got from the state stack
				//
				if (RTISA(_profileSegment, GvNurbsProfile))//is a nurbs profile curve
				{
						GvNurbsProfile* mNurbsProfile = RTCAST(_profileSegment, GvNurbsProfile);
						

						if (mNurbsProfile->linkage == GvNurbsProfile::START_NEW ||
							mNurbsProfile->linkage == GvNurbsProfile::START_FIRST)
						{
								currentContour = new GvContour2D();
								trSurf->trimmingContour.append(currentContour);
						}

						BuildNurbsCurve2D(state, currentContour, mNurbsProfile);
						


				}
				else if (RTISA(_profileSegment, GvLinearProfile))// is a linear element
				{
						GvLinearProfile* mLinearProfile = RTCAST(_profileSegment, GvLinearProfile);
												
						//the output VMRL node
						GvPolyline2D* mPolyline2D = BuildPolyline2D(state, mLinearProfile);
						ASSERT(mPolyline2D);
					
						if (mLinearProfile->linkage == GvLinearProfile::START_NEW ||
							mLinearProfile->linkage == GvLinearProfile::START_FIRST)
						{
								currentContour = new GvContour2D();
								trSurf->trimmingContour.append(currentContour);
						}
					
						if (currentContour)
							currentContour->addChild(mPolyline2D);				
				}
				else
				{
					TRACE("GvNurbsSurfaceInventor::BuildNode: unknown profile segment on stack!\n");
					return 0;
				}
		}			
		//add the trimmedSurface node
		child = trSurf;

	//
	//if we are done, pop the stack?????????????????????????????

	}
	else //we have a untrimmed surface
	{
	    child = surface;
	}

	return 1;
}



//--------------------------------------------------------------------------------
//GvNurbsSurface*	GvNurbsSurfaceInventor::BuildNurbsSurface(GTraversal &state)
//--------------------------------------------------------------------------------
GvNurbsSurface*	GvNurbsSurfaceInventor::BuildNurbsSurface(GTraversal &state)
{
	G_GETSTATE(state, _mCoords, GvNode, GvCoordinate3Element)

	ASSERT(_mCoords);
	if (!_mCoords) return NULL;

	G_GETSTATE(state,shapeHints,GvShapeHints,	GvShapeHintsElement)


	GvNurbsSurface* surface = new GvNurbsSurface();

	if (surface)
	{

		if (shapeHints) { // convert shape hints 
			if (!shapeHints->CanCull()) surface->solid.set(FALSE);
			if (shapeHints->vertexOrdering == (int)GvShapeHints::CLOCKWISE) {
				 surface->ccw.set(FALSE);
			}
		}

				//knot
					surface->uKnot =	uKnotVector;
					surface->vKnot =	vKnotVector;
					
				//dimension controlpoints
					surface->uDimension.set(numUControlPoints);
					surface->vDimension.set(numVControlPoints);
					
				//order
					int num;
					num = surface->uKnot.getNum() - surface->uDimension.value;
					surface->uOrder.set(num);
					num = surface->vKnot.getNum() - surface->vDimension.value;
					surface->vOrder.set(num);
				
				//controlpoints and weights
					if (RTISA(_mCoords, GvCoordinate3))
					{	
						GvCoordinate3* mCoord3 = RTCAST(_mCoords, GvCoordinate3);

						surface->controlPoint = mCoord3->point;
						//default weights		
					}
					else if (RTISA(_mCoords, GvCoordinate4))
					{
						GvCoordinate4* mCoord4 = RTCAST(_mCoords, GvCoordinate4);
						
						surface->controlPoint.setLength(mCoord4->point.getMaxNum());
						surface->weight.setLength(mCoord4->point.getMaxNum());

						for(int i=0; i<numUControlPoints*numVControlPoints; i++)
						{
							Point point(mCoord4->point.values[i*4], mCoord4->point.values[i*4+1],mCoord4->point.values[i*4+2]);
							surface->controlPoint.set1(i, point);
							surface->weight.set1(i, mCoord4->point.values[i*4+3]);
						}
					}
					else
					{
						TRACE("GvNurbsSurfaceInventor::BuildNurbsSurface: unknown coord type on stack!\n");
						delete surface;
						return NULL;
					}

	}
	return surface;
}



//--------------------------------------------------------------------------------
//bool 
//GvNurbsSurfaceInventor::BuildNurbsCurve2D
//(GTraversal &state, GvContour2D* currentContour, GvNurbsProfile *mNurbsProfile)
//--------------------------------------------------------------------------------

bool GvNurbsSurfaceInventor::BuildNurbsCurve2D(GTraversal &state, GvContour2D* currentContour, GvNurbsProfile *mNurbsProfile)
{
	//input: nurbsProfile, profileCoord
	//output: trCurve (nurbsCurve2D)
	int i, order;
	
	int multiplicity = 1;
	int firstidx = 0;//the first idx in the current knot
	//int firstidxcv = 0;//the first idx in the current control point field	
	float point2d[2];

	G_GETSTATE(state, _profileCoord, GvNode, GvProfileCoordElement)

	GvProfileCoordinate2* mCoord2 = NULL;
	GvProfileCoordinate3* mCoord3 = NULL;

					
	//
	// check type: 2D or 3D coords
	//	
	if (RTISA(_profileCoord, GvProfileCoordinate2))
	{
		mCoord2 = RTCAST(_profileCoord, GvProfileCoordinate2); 
	}
	else if (RTISA(_profileCoord, GvProfileCoordinate3))
	{
		mCoord3 = RTCAST(_profileCoord, GvProfileCoordinate3); 
	}		
	else //error
	{
		TRACE("GvNurbsSurfaceInventor::BuildNode: unknown coord type on stack!\n");
		return false;
	}
			/*	
					GvNurbsCurve2D* trCurve = new GvNurbsCurve2D();
					
					// set the knot vector
					trCurve->knot = mNurbsProfile->knotVector;
					int knotLength = trCurve->knot.Length();
					
					GvProfileCoordinate3* mCoord3 = RTCAST(_profileCoord, GvProfileCoordinate3); 
					
					//fill the controlPoint field
					int dimension = mNurbsProfile->index.Length();
					trCurve->controlPoint.setLength(dimension);
					trCurve->weight.setLength(dimension);
					
					for (int i = 0; i<dimension; i++)
					{
						int idx = mNurbsProfile->index[i];
						const float* point = mCoord3->point.get1(idx);
						trCurve->controlPoint.set1(i, point);
						trCurve->weight.set1(i, point[2]);
					}
					
					//order
					trCurve->order.set(knotLength - dimension);
					
					
				}
	
				*/
				
				
				
						
	int dimension = mNurbsProfile->index.Length();
	int knotLength = mNurbsProfile->knotVector.Length();
	
	//order
	order = knotLength - dimension;
	TRACE("===GvNurbsSurfaceInventor::BuildNurbsCurve2D: check new ProfileCoord===\norder %i \n", order);
	//
	//check multiplicity within knot
	//if we have an invalid multiplicity with the knot vector
	//we split the knot vector
	//
	
	/*
	//   [0, 0, 0, 0, .1, .2, .3, .5, .5, .5, .5, .7, .1, .1, .1, .1]
	//
	//  firstidx								i
	//
	idx [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, ...15]
	*/

	float repeatedknot = mNurbsProfile->knotVector[0];
	for (i = 1; i<knotLength; i++)
	{
		float currentknot = mNurbsProfile->knotVector[i];
		
		if (repeatedknot != currentknot)
		{
			multiplicity = 1;
		}
		else
		{
			multiplicity++;
			int ii, toIdx;
			
			if (multiplicity == order && (i+firstidx) >= order)
			{//we have invalid multiplicity within the knot vector or reached the end
				
				int currentdimension = (i-firstidx+1)-order;//+1 because i and firstidx are indices
				
				if (0) //(order == 2) //TBD: connect single polylines to one polyline
				{
					//three knot spans are one polyline!!!!!!!!!!!
					ASSERT(currentdimension == 2);
					// linear segment ??  temp test hg 
					GvPolyline2D* mPolyline2D = new GvPolyline2D();
					
					mPolyline2D->point.setLength(2);
					
					int idx = mNurbsProfile->index[firstidx++];
					const float* point1 = mCoord2->point.get1(idx);
					mPolyline2D->point.set1(0, point1);
					idx = mNurbsProfile->index[firstidx];
					const float* point2 = mCoord2->point.get1(idx);
					mPolyline2D->point.set1(1, point2);
					TRACE("GvPolyline2D (%f %f) (%f %f)\n",
						point1[0], point1[1], point2[0], point2[1]); 
					firstidx = i - order + 1;
					
					//add it to the current contour
					ASSERT(currentContour);
					if (currentContour) 
						currentContour->addChild(mPolyline2D);
				}
				else
				{
					
					//setup a new curve
					GvNurbsCurve2D* trCurve = new GvNurbsCurve2D();
					TRACE("\n\nnew curve: reached multi = order at knot[i = %i]\n", i);
					
					//order
					trCurve->order.set(order);
					
					//fill the controlPoint and weight field
					toIdx = 0;
					TRACE("currentdimension %i\n", currentdimension);
					trCurve->controlPoint.setLength(currentdimension);
					if (mCoord3)
						trCurve->weight.setLength(currentdimension);
					
					TRACE("controlpoints:\n");
					for (ii = firstidx; ii<(firstidx+currentdimension); ii++)
					{
						int idx = mNurbsProfile->index[ii];

						if (mCoord2)
						{
							const float* point;
							point = mCoord2->point.get1(idx);
							trCurve->controlPoint.set1(toIdx++, point);
							TRACE("(%f, %f) \n", point[0], point[1]);
						}
						else
						{
							//split the 3d coord into 2d and weight
							const float *tmppoint = mCoord3->point.get1(idx);
							point2d[0] = tmppoint[0];
							point2d[1] = tmppoint[1];
							trCurve->controlPoint.set1(toIdx, point2d);
							trCurve->weight.set1(toIdx++, tmppoint[2]);
						}
					}
					//firstidxcv = firstidx+currentdimension -1;//the last cv is repeated in the next one
					
					//fill knot vector
					toIdx = 0;
					trCurve->knot.setLength(currentdimension+order);
					TRACE("\nknot with %i elems\n", currentdimension+order);
					
					for (ii = firstidx; ii<(firstidx+currentdimension+order); ii++)
					{
						float knot = mNurbsProfile->knotVector[ii];
						trCurve->knot.set1(toIdx++, knot);
						TRACE("%f, \n", knot);
					}
					
					//add it to the current contour
					ASSERT(currentContour);
					if (currentContour) 
						currentContour->addChild(trCurve);
					
				}
				
				//goto the next curve segment
				firstidx = i - order + 1;
				TRACE("\nfirstidx (%i)\n", firstidx);
				multiplicity = 1;
			}
		}
		repeatedknot = currentknot;
	}//end of loop over all knot members
				
	return true;
				
				
}



//--------------------------------------------------------------------------------
//GvPolyline2D*	
//GvNurbsSurfaceInventor::BuildPolyline2D(GTraversal &state, GvLinearProfile *mLinearProfile)
//--------------------------------------------------------------------------------

GvPolyline2D*	
GvNurbsSurfaceInventor::BuildPolyline2D(GTraversal &state, GvLinearProfile *mLinearProfile)
{
	//input: nurbsProfile, profileCoord
	//output: mPolyline2D 
			
	GvPolyline2D* mPolyline2D = new GvPolyline2D();
				
				
				G_GETSTATE(state, _profileCoord, GvNode, GvProfileCoordElement)
				
				//
				// 2D coords
				//	
				if (RTISA(_profileCoord, GvProfileCoordinate2))
				{
					GvProfileCoordinate2* mCoord2 = RTCAST(_profileCoord, GvProfileCoordinate2); 
					
					//fill the  field
					int dimension = mLinearProfile->index.Length();
					mPolyline2D->point.setLength(dimension);
					for (int i = 0; i<dimension; i++)
					{
						int idx = mLinearProfile->index[i];
						const float* point = mCoord2->point.get1(idx);
						mPolyline2D->point.set1(i, point);
					}

				}
	
				else //error
				{
					TRACE("GvNurbsSurfaceInventor::BuildPolyline2D: unknown coord type on stack!\n");
					return NULL;
				}
	return mPolyline2D;

				
}



//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//				T H E   N U R B S C U R V E											
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
 
  
GV_NODE_SOURCE_IMP(GvNurbsCurveInventor,GvNode);


GvNurbsCurveInventor::GvNurbsCurveInventor() :
		
		//new inventor fields
		numControlPoints(0),
		knotVector(0),
		child(0)
{
    GV_NODE_CONSTRUCTOR(GvNurbsCurveInventor);

    GV_NODE_ADD_FIELD(numControlPoints);
    GV_NODE_ADD_FIELD(knotVector);
    GV_NODE_ADD_FIELD(child);

	m_bInitialized = gfalse;
	m_bBadValues = gfalse;
    isBuiltIn = FALSE;

}


GvNurbsCurveInventor::~GvNurbsCurveInventor()
{}



int GvNurbsCurveInventor::Do(GTraversal &state)
{ 
	if (!m_bInitialized) {
		m_bInitialized = TRUE;
		if (!BuildNode(state))
		{
			m_bBadValues = TRUE;
			return(0);
		}
	}
	return child.Traverse(state);
}


int	GvNurbsCurveInventor::BuildNode(GTraversal &state)
{
	G_GETSTATE(state, _mCoords, GvNode, GvCoordinate3Element)
	ASSERT(_mCoords);


	GvNurbsCurve* curve = new GvNurbsCurve();

	if (curve)
	{

				//knot
					curve->knot =	knotVector;
					
				//dimension controlpoints
					//curve->dimension = numControlPoints;//obsolete
					
				//order
					int num;
					num = curve->knot.getNum() - numControlPoints;
					curve->order.set(num);
				
				//controlpoints and weights
					if (RTISA(_mCoords, GvCoordinate3))
					{	
						GvCoordinate3* mCoord3 = RTCAST(_mCoords, GvCoordinate3);

						curve->controlPoint = mCoord3->point;
						curve->controlPoint.setLength(numControlPoints);
						//default weights		
					}
					else if (RTISA(_mCoords, GvCoordinate4))
					{
						GvCoordinate4* mCoord4 = RTCAST(_mCoords, GvCoordinate4);
						
						curve->controlPoint.setLength(numControlPoints);
						curve->weight.setLength(numControlPoints);

						for(int i=0; i<numControlPoints; i++)
						{
							Point point(mCoord4->point.values[i*4], mCoord4->point.values[i*4+1],mCoord4->point.values[i*4+2]);
							curve->controlPoint.set1(i, point);
							curve->weight.set1(i, mCoord4->point.values[i*4+3]);
						}
					}
					else
					{
						TRACE("GvCurveInventor::BuildNurbsSurface: unknown coord type on stack!\n");
						return 0;
					}

	}	
	
	
	child = curve;

	return 1;
}



//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//				T H E   NurbsProfile	
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/


  
GV_NODE_SOURCE_IMP(GvNurbsProfile, GvNode);


GvNurbsProfile::GvNurbsProfile() :
			index(1),       
			linkage(START_FIRST), 
			knotVector(1),
			changed(FALSE)

{
    GV_NODE_CONSTRUCTOR(GvNurbsProfile);
	GV_NODE_ADD_FIELD(index);
    GV_NODE_ADD_FIELD(linkage);
    GV_NODE_ADD_FIELD(knotVector);

	GV_NODE_DEFINE_ENUM_VALUE(linkage_type, START_FIRST);
    GV_NODE_DEFINE_ENUM_VALUE(linkage_type, START_NEW);
    GV_NODE_DEFINE_ENUM_VALUE(linkage_type, ADD_TO_CURRENT);

    GV_NODE_SET_SF_ENUM_TYPE(linkage, linkage_type);
 
	m_bInitialized = gfalse;
	m_bBadValues = gfalse;
	index.values[0]=0;
	knotVector.values[0]=0;

}


GvNurbsProfile::~GvNurbsProfile()
{}




int GvNurbsProfile::Do(GTraversal &state)			
{									      
//	TRACE("%s push %s \n",state.ClassName(),this->ClassName());
	GvProfileCurveElement *current,*prev;									
	/* get current */												
	prev = (GvProfileCurveElement*) state.Get(GvProfileCurveElement::attrId); 
	if (!prev) { /* ignore it, because not registerd (initialized)	*/				
		return(0);															
	}	

																		
	current = (GvProfileCurveElement *) prev->NewInstance();								
	current->data = this;													

	state.Push(GvProfileCurveElement::attrId,current);			
	return(1);																
}	

	


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 
//				GvLinearProfile	
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/


  
GV_NODE_SOURCE_IMP(GvLinearProfile, GvNode);


GvLinearProfile::GvLinearProfile() :
			index(1),       
			linkage(START_FIRST), 
			changed(FALSE)

{
    GV_NODE_CONSTRUCTOR(GvLinearProfile);
	GV_NODE_ADD_FIELD(index);
    GV_NODE_ADD_FIELD(linkage);

	
	GV_NODE_DEFINE_ENUM_VALUE(linkage_type, START_FIRST);
    GV_NODE_DEFINE_ENUM_VALUE(linkage_type, START_NEW);
    GV_NODE_DEFINE_ENUM_VALUE(linkage_type, ADD_TO_CURRENT);

    GV_NODE_SET_SF_ENUM_TYPE(linkage, linkage_type);
	m_bInitialized = gfalse;
	m_bBadValues = gfalse;
	index.values[0]=0;


}


GvLinearProfile::~GvLinearProfile()
{}



int GvLinearProfile::Do(GTraversal &state)			
{									      
//	TRACE("%s push %s \n",state.ClassName(),this->ClassName());
	GvProfileCurveElement *current,*prev;									
	/* get current */												
	prev = (GvProfileCurveElement*) state.Get(GvProfileCurveElement::attrId); 
	if (!prev) { /* ignore it, because not registerd (initialized)	*/				
		return(0);															
	}	
																			
	current = (GvProfileCurveElement *) prev->NewInstance();								
	current->data = this;													

	state.Push(GvProfileCurveElement::attrId,current);			
	return(1);																
}	


#endif _G_INVENTOR


