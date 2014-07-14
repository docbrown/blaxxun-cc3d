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
#include "gclass.h"
#include "gshell.h"
#include "gshelli.h"

#include "gnurbs.h"


#include "gbuildshell.h"


// to do where to put ::

#include <gvnodes.h>

// Evaluate object as object of type "type"
// if return = this, don´t destroy result, otherwise destroy
GvNode *GvNode::EvalAs(TypeId type,int copyAttributes)
{
    if (type == Id()) return(this); // already indentical
	return(NULL);
}

GvNode *GvNodeShell::EvalAs(TypeId type,int copyAttributes)
{
	return(NULL);
}

#ifdef _G_VRML1

GvNode *GvGroup::EvalAs(TypeId type,int copyAttributes)
{
	return(NULL);
}

GvNode *GvSeparator::EvalAs(TypeId type,int copyAttributes)
{
	return(NULL);
}

#endif

GvNode *Gv2Group::EvalAs(TypeId type,int copyAttributes)
{
	if (RTISA(this,Gv2Group)) {
		Gv2Group *s = (Gv2Group *) this;
		if (type == Gv2Anchor::ClassId()) {
			Gv2Anchor *d= new Gv2Anchor();
			d->children.set(s->children);
			return(d);
		}
		if (type == Gv2Transform::ClassId()) {
			Gv2Transform *d= new Gv2Transform();
			d->children.set(s->children);
			return(d);
		}
	}
	return(GvNode::EvalAs(type,copyAttributes));
}



int GNode::Do(GBuildShellTraversal &state) // declared in GClass.h
{
	if (RTISA(this,GShell)) {
		GShell *s = (GShell *) this;
		// convert to this type 
		s = (GShell *) s->EvalAs(GShell::ClassId(),1);
		int ret= state.Append(s);
		
		if (s != this) { 
			delete s;
		}
		return (ret);
	}
	else return Do(*((GTraversal *) &state));		
}


//
// class GBuildShellTraversal
//
RTIMP(GBuildShellTraversal,0,GRenderTraversal);

void GBuildShellTraversal::InitializeAttributes()
{	
 	GRenderTraversal::InitializeAttributes();
}

GBuildShellTraversal::~GBuildShellTraversal()
{
  if (shell) 
//	  shell->unref(); 
     delete shell;
  shell = NULL;
}

int GBuildShellTraversal::Append(GShell * appendShell,const Point *fcolorp)

{
   if (!appendShell) return (0);

   if (!shell) {
       shell = new GShell();
	   //shell->ref();
   }
   
   Matrix *m = GetMatrix();
   
   Matrix *tm = GetTextureMatrix();

   	// get current
   const Point * fcolor = fcolorp;
   
   if (!fcolor) fcolor =  &((GFaceColorStack*) Get(GFaceColorStack::attrId))->color;

   int oldFcnt = shell->GetFaceCount();
   if (RTISA(appendShell,GShellI)) {
	 	GShellI *shellI = (GShellI *) appendShell;

		GShell *newS = NULL;
		//const PointArray &vn = shellI->GetVN();
		shellI->ComputeShell(newS,1);
	    shell->Append(*newS,doLookup,m);
		delete newS;

   } else {	
		if (RTISA(appendShell,GNurbsShell)) {
			GNurbsShell *s = (GNurbsShell *) appendShell;

			// use actual tessellation 
			if (!s->IsShellTessellationOk())
				s->ComputeShell();
			

		}
		shell->Append(*appendShell,doLookup,m);
	}	
   if (tm) {
   	
   }
   
   int newFcnt = shell->GetFaceCount();
   

   if (fcolor && (appendShell->fc.Length() == 0)) {	// set face color of new faces
	   	shell->fc.SetLength(newFcnt);
   		for (int f=oldFcnt; f<newFcnt;f++) {
	 		shell->fc[f] = *fcolor;
   		}
   }
   return(1);
}


#ifdef _G_VRML1

int GvMaterial::Do(GBuildShellTraversal &state) 
{
	// BuildShell::Append asks for face color, for newly appended faces
	Point c;
	GetColor(0,c);

	GFaceColorStack *current,*prev;
	prev = (GFaceColorStack*) state.Get(GFaceColorStack::attrId);
	current = (GFaceColorStack *) prev->NewInstance();								
	current->color = c;
	state.Push(current);
    // register attribute in state also
	Do(* ((GTraversal *) &state));
	return(1);
}

#endif _G_VRML1

int Gv2Material::Do(GBuildShellTraversal &state) 
{
	// BuildShell::Append asks for face color, for newly appended faces
	Point c;

	GetColor(c);

	GFaceColorStack *current,*prev;
	prev = (GFaceColorStack*) state.Get(GFaceColorStack::attrId);
	current = (GFaceColorStack *) prev->NewInstance();								
	current->color = c;
	state.Push(current);

	return(1);
}

