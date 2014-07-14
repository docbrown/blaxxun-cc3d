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

@module gnurbs.cpp 
Copyright (c) 1998/1999	by Thomas Volk / Holger Grahn - blaxxun interactive
All rights reserved

Purpose:

Lowlevel NURBS Surface rendering primitive implementation

Classes:

Notes:
--------------------------------------------
				Rendering
--------------------------------------------
main rendering routines

	Do(GglRenderTraversal &traversalState)
	
	  branches in
		1. D3dRenderTL(GglRenderTraversal &traversalState) if software lighting
			CVs are transformed
		2. GShell::Do(traversalState) if D3D lighting
			CVs are not transformed (ComputeShell)

--------------------------------------------
			       Tessellation
--------------------------------------------
if a tessellation value is greater 0, the number of tessellation points are tessellation+1; 
if a tessellation value is smaller then 0, the number of tessellation points are (-tessellation * dimension)+1 
if a tessellation value is  0, the number of tessellation points are  (2 * dimension)+1  

relevant methods:
	SetInitialTessellation:
		sets the initial values (needed for dynamic tessel) and calls setTessellation

	SetTessellation
		checks if values are valid

	ComputeTessellationValues(int mode)
		called before rendering
		changes the values according to the mode (dynamic etc.) and calls setTessellation

--------------------------------------------
			       LOD
--------------------------------------------
  
To diffrent kinds of LOD computation are used:
1. based on the distance of the viewer to the object (classical LOD)
2. based on the framerate/performance of the system

These two methods are used in combination, too.

related methods:
	computeTessellationValues
		scales the Tessellations according to mode
  		scale = m_fRenderQuality*sqrt((m_fBBSurface)/(float)m_iInitialUTess/(float)m_iInitialVTess);
		nUTessellations = scale * m_iInitialUTess;
		nVTessellations = scale * m_iInitialVTess;

---------------------------------------------  
1. classical LOD
---------------------------------------------  
A initial render quality is defined (based on values of the author etc). 
This quality is supposed to be sufficient in any case, so the quality
is only scaled down.
  
The visual quality of the object should be view independent/constant.
	
		quality = tris/bbsurface(d) = const

		tris = m_iInitialUTess * scale * m_iInitialVTess * scale *2


The resulting function

		scale(d) = sqrt( (quality * bbsurface(d))/(m_iInitialUTess * m_iInitialVTess * 2) )

		d: distance from the viewser to the object

Methods to define the quality: 

		1. parameters of the author

			quality = (m_iInitialUTess * m_iInitialVTess * 2)/2
			
			the value 2 is based on expierence, i.e. the author provides tessellation values
			so that the object looks good fully shown on screen. So find a modelmat where
			the object is fully shown and compute bbsurf or take a value based on expirience,
			because a fully shown object is after projection always about the same.

			OR:

			quality = (m_iInitialUTess * m_iInitialVTess * 2)/bbsurf(d0) (parameters of the author)

				scale(d) = sprt(bbsurface(d)/bbsurface(d0) )

				Problem: wo ist d0? d0 nicht fuer alle Obj gleich, da sonst
				grosses obj wegen grosser flaeche kleinere initial qual hat!

		2. quality(chord_length)



related methods:
	ComputeBBSurface
		computes something, not clear now
		surface is bad, because ~1/d^2
		pixel is bad, because diffrent screen sizes have low influence
		and make the curve steeper

---------------------------------------------  
2. LOD based on CPU-performance/fps
---------------------------------------------  

related methods:
	SetRenderQuality
		m_fRenderQuality = budget * m_fInitialRenderQuality;
		budget is computed in gview
	
	 computeTessellationValues  
		scale = m_fRenderQuality;

---------------------------------------------  
3. LOD with 1&2 in combination
---------------------------------------------  

related methods:
	SetRenderQuality
		m_fRenderQuality = budget * m_fInitialRenderQuality;
	
	 computeTessellationValues  
		same computation as in 1, but scale is not limited to 1


Changes:
$Revision: 1.12 $
$Log: gnurbs.cpp,v $
Revision 1.12  1999/07/06 16:54:39  tom
*** empty log message ***



Todo :

  deal with strange cases,
  in GNurbsSurface::init()
  z.b. 
  iknot = order + dimension
  falsche order fuehrt zum crash !
  wahre laenge des knot vec muss geprueft werden!


******************************************************************************/

#include "stdafx.h"

#include "gversion.h"


#include "gnurbs.h"
#include "gcamera.h"
#include "grayselect.h"

#include "gkatmai.h"

#define MAX_BBSURF 1.f

// compute knotvector
// page 370
// 
void ComputeKnotVectorParam(int N, int pdim, float u[], const float ud[]) 
{
	int m,k;

	m = N + pdim + 3; 

	for (k=0; k<=pdim; k++) 
	{ u[k] = 0.0f; 
	} 

	for (k=(m-pdim); k<=(m); k++) 
	{ u[k] = 1.0f;
	}
	int i,j;
	for (j=0; j<=(N-pdim+1); j++) 
	{	u[j+pdim+1] = 0.; 
		for (i=j; i<=(j+pdim-1); i++)
		{ u[j+pdim+1] = u[j+pdim+1] + ud[i]; 
		}
		u[j+pdim+1] = u[j+pdim+1]/pdim; 
	}
}
// Pg. 365 , 9.7
void ComputeKnotVector(int N, int pdim, float u[]) 
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


//
//	GNurbsShell
//

RTIMP(GNurbsShell,0,GShell);

//operator HPoint *(FloatArrayAligned &array) { return (HPoint *)array.Data(); }
#define GetHPoint(array)	((HPoint *)array.Data())


GNurbsShell::GNurbsShell() : GShell()
{
#ifdef GN_LOG
	bbsurf.open("c:\\cc3d\\bbsurf.log");
	tris.open("c:\\cc3d\\tris.log");
	scaleStream.open("c:\\cc3d\\scale.log");
#endif
	texCompute = FALSE;//TRUE;
	triStrip = FALSE; // TRUE; //FALSE;
	m_fBBSurface = 0.0;

	
	m_iUControlPoints = 0;
	m_iVControlPoints = 0;

	m_iUCoefficients = 0;
	m_iVCoefficients = 0;

	m_iUKnots = 0;
	m_iVKnots = 0;

	m_TessUKnot = NULL;
	m_TessVKnot = NULL;

	m_iVertexIndices[0] = NULL;
	m_iVertexIndices[1] = NULL;
	m_iVertices = 0;

	m_iUTessellations = 0;
	m_iVTessellations = 0;
	m_iMaxUTessellations = 0;
	m_iMaxVTessellations = 0;
	m_iMinUTessellations = 0;
	m_iMinVTessellations = 0;
	m_iLastUTess = 0;
	m_iLastVTess = 0; 
	m_iInitialUTess = 0;
	m_iInitialVTess = 0; 
	m_fBudget = 0;

	m_iUDegree = m_iVDegree = 0;
	m_iUOrder = m_iVOrder = 1;
	m_bInitialized = FALSE;

	shellVerticesOk = gfalse;
	shellFacelistOk = gfalse;
	tessellationChanged = gfalse;

}

GNurbsShell::~GNurbsShell() 
{
		SAFE_DELETE( m_iVertexIndices[0] );
		SAFE_DELETE( m_iVertexIndices[1] );

		SAFE_DELETE( m_TessUKnot );
		SAFE_DELETE( m_TessVKnot );
}


int GNurbsShell::ComputeShell()
{
	unsigned int numVerts, num;
	
	num = 0;

	if (tessellationChanged) {
		shellVerticesOk = gfalse;
		shellFacelistOk = gfalse;
	}
	numVerts =(m_iVTessellations + 1)*(m_iUTessellations + 1);

	// directly use shell vertex arrays for output
	// to avoid one data copy
	
	v.SetLength(numVerts);
	if (v.Data() == NULL) return -1; // out of memory
	vn.SetLength(numVerts);
	if (vn.Data() == NULL) return -1; // out of memory
	vp.SetLength(numVerts);
	if (vp.Data() == NULL) return -1; // out of memory


	Point *vertices=this->v.DataRW();

	// use stepping pointers to avoid imul instructions

	Point *verticesp=this->v.DataRW();

	Point *vpp=vp.DataRW(); 
	Point *vnp=vn.DataRW(); 

	
	unsigned int u, v; // see if unsigned makes faster code
	unsigned int k, l;
	int uKnot, vKnot;
	
	HPoint *UTemp = GetHPoint(m_UTemp), 
	*dUTemp = GetHPoint(m_dUTemp);

	HPoint Pw, TanU, TanV;
	Point Norm, texCoord;
	float rhw;
	float Len;
	
	int iVertices;

//	HPoint *pTxPoints = GetHPoint(m_TxControlPoints);
//  use untransformed coords instead...

	HPoint *pTxPoints =	GetHPoint(m_ControlPoints);
	unsigned int iTxOffset;
	float VBasis, dVBasis;
	int iSet = 0;
	unsigned int idx, uidx;


	if( !m_bInitialized || !m_iUTessellations || !m_iVTessellations )
		return 0;

	if( (m_iUTessellations != m_iLastUTess) ||
		(m_iVTessellations != m_iLastVTess) )
	{
		EvaluateBasisFunctions();
		m_iLastUTess = m_iUTessellations;
		m_iLastVTess = m_iVTessellations;
	}

	iVertices = 2 * (m_iVTessellations + 1);

	//m_pNormals.SetLength((m_iVTessellations + 1)*(m_iUTessellations + 1));
	//m_TexCoords.SetLength((m_iVTessellations + 1)*(m_iUTessellations + 1));

	float Fuinc = 1.0f/(float)m_iUTessellations;
	float Fvinc = 1.0f/(float)m_iVTessellations;
	float Fu = 0.0f;
	float Fv;

	num = 0;

	//
	// Step over the U and V coordinates and generate triangle strips to render
	//
	for( u=0; u<=m_iUTessellations; u++ )
	{
		uKnot = m_TessUKnot[u];

		uidx = u * m_iUOrder * VECTOR_SIZE;
		vKnot = -1;
		idx = iSet;
		Fv = 0.0f;
		for( v=0; v<=m_iVTessellations; v++, idx+=2 )
		{
			if( vKnot != m_TessVKnot[v] )
			{
				vKnot = m_TessVKnot[v];
				for( k=0; k<=m_iVDegree; k++)
				{
					iTxOffset = (uKnot - m_iUDegree) * m_iVControlPoints + (vKnot - m_iVDegree);
					UTemp[k].x = m_UBasis[uidx] * pTxPoints[ iTxOffset + k ].x;
					UTemp[k].y = m_UBasis[uidx] * pTxPoints[ iTxOffset + k ].y;
					UTemp[k].z = m_UBasis[uidx] * pTxPoints[ iTxOffset + k ].z;
					UTemp[k].w = m_UBasis[uidx] * pTxPoints[ iTxOffset + k ].w;

					dUTemp[k].x = m_dUBasis[uidx] * pTxPoints[ iTxOffset + k ].x;
					dUTemp[k].y = m_dUBasis[uidx] * pTxPoints[ iTxOffset + k ].y;
					dUTemp[k].z = m_dUBasis[uidx] * pTxPoints[ iTxOffset + k ].z;
					dUTemp[k].w = m_dUBasis[uidx] * pTxPoints[ iTxOffset + k ].w;

					for( l=1; l<=m_iUDegree; l++ )
					{
						iTxOffset += m_iVControlPoints;
						UTemp[k].x += m_UBasis[uidx+l * VECTOR_SIZE] * pTxPoints[ iTxOffset + k].x;
						UTemp[k].y += m_UBasis[uidx+l * VECTOR_SIZE] * pTxPoints[ iTxOffset + k].y;
						UTemp[k].z += m_UBasis[uidx+l * VECTOR_SIZE] * pTxPoints[ iTxOffset + k].z;
						UTemp[k].w += m_UBasis[uidx+l * VECTOR_SIZE] * pTxPoints[ iTxOffset + k].w;
						dUTemp[k].x += m_dUBasis[uidx+l * VECTOR_SIZE] * pTxPoints[ iTxOffset + k ].x;
						dUTemp[k].y += m_dUBasis[uidx+l * VECTOR_SIZE] * pTxPoints[ iTxOffset + k ].y;
						dUTemp[k].z += m_dUBasis[uidx+l * VECTOR_SIZE] * pTxPoints[ iTxOffset + k ].z;
						dUTemp[k].w += m_dUBasis[uidx+l * VECTOR_SIZE] * pTxPoints[ iTxOffset + k ].w;
					}
				}
			}

			VBasis = m_VBasis[ (v * m_iVOrder)*VECTOR_SIZE ];
			dVBasis = m_dVBasis[ (v * m_iVOrder)*VECTOR_SIZE ];
			Pw.x = VBasis * UTemp[0].x;
			Pw.y = VBasis * UTemp[0].y;
			Pw.z = VBasis * UTemp[0].z;
			Pw.w = VBasis * UTemp[0].w;
			TanU.x = VBasis * dUTemp[0].x;
			TanU.y = VBasis * dUTemp[0].y;
			TanU.z = VBasis * dUTemp[0].z;
			TanU.w = VBasis * dUTemp[0].w;
			TanV.x = dVBasis * UTemp[0].x;
			TanV.y = dVBasis * UTemp[0].y;
			TanV.z = dVBasis * UTemp[0].z;
			TanV.w = dVBasis * UTemp[0].w;

			for( k=1; k<=m_iVDegree; k++ )
			{
				VBasis = m_VBasis[ (v * m_iVOrder + k)*VECTOR_SIZE ];
				dVBasis = m_dVBasis[ (v * m_iVOrder + k)*VECTOR_SIZE ];
				Pw.x += VBasis * UTemp[k].x;
				Pw.y += VBasis * UTemp[k].y;
				Pw.z += VBasis * UTemp[k].z;
				Pw.w += VBasis * UTemp[k].w;
				TanU.x += VBasis * dUTemp[k].x;
				TanU.y += VBasis * dUTemp[k].y;
				TanU.z += VBasis * dUTemp[k].z;
				TanU.w += VBasis * dUTemp[k].w;
				TanV.x += dVBasis * UTemp[k].x;
				TanV.y += dVBasis * UTemp[k].y;
				TanV.z += dVBasis * UTemp[k].z;
				TanV.w += dVBasis * UTemp[k].w;
			}

			rhw = 1.0f / Pw.w;

			verticesp->x = Pw.x = Pw.x * rhw;
			verticesp->y = Pw.y = Pw.y * rhw;
			verticesp->z = Pw.z = Pw.z * rhw;
			verticesp++;


			// TRACE("vert[%i]: %f %f %f\n", num, Pw.x, Pw.y, Pw.z);

			//
			// Convert the Tangents from Homogenous to Euclidean space
			//
			TanU.x  = (TanU.x - TanU.w * Pw.x) * rhw;
			TanU.y  = (TanU.y - TanU.w * Pw.y) * rhw;
			TanU.z  = (TanU.z - TanU.w * Pw.z) * rhw;
			TanV.x  = (TanV.x - TanV.w * Pw.x) * rhw;
			TanV.y  = (TanV.y - TanV.w * Pw.y) * rhw;
			TanV.z  = (TanV.z - TanV.w * Pw.z) * rhw;
			//TRACE("TanU: %f %f %f ", TanU.x, TanU.y, TanU.z);
			//TRACE("TanV: %f %f %f ", TanV.x, TanV.y, TanV.z);

			//
			// Determine the normal from the cross product of the two tangents
			//
//			Norm.x = (TanV.y * TanU.z - TanU.y * TanV.z);
//			Norm.y = (TanU.x * TanV.z - TanV.x * TanU.z);
//			Norm.z = (TanV.x * TanU.y - TanU.x * TanV.y);

			Norm.x = (TanU.y * TanV.z - TanU.z * TanV.y);
			Norm.y = (TanU.z * TanV.x - TanU.x * TanV.z);
			Norm.z = (TanU.x * TanV.y - TanU.y * TanV.x);
	
		/*	TRACE("Norm: %f %f %f \n", Norm.x, Norm.y, Norm.z);
			if (Norm.x == 0)
				TRACE("error\n");
			if (Norm.y == 0)
				TRACE("error\n");			
			if (Norm.z == 0)
				TRACE("error\n");/
		*/			
        /*  later is normal check
			//
			// Normalize the normal
			//
			Len = (float)sqrt( Norm.x * Norm.x + Norm.y * Norm.y + Norm.z * Norm.z);
			if (Len == 0.0) { Norm.z = 1.0,Norm.x=Norm.y=0.0; }
 			else { 
 				Len /= Len;
 				Norm.x *= Len;	Norm.y *= Len; Norm.z *= Len;
 			}
         */
			vnp->x = Norm.x;
			vnp->y = Norm.y;
			vnp->z = Norm.z;
			vnp++;
			
			vpp->x = Fu;
			vpp->y = Fv;
			vpp->z = 0.f;
			vpp++;
			
			// m_TexCoords.Set1(num, texCoord);
			//num++;
			Fv += Fvinc;
		}
		Fu += Fuinc;
		g_CurrentUKnot = uKnot;
		iSet = 1 - iSet;
	}
	
	num=numVerts;	
	ASSERT(num == numVerts);
	ASSERT(num==this->v.Length());
	ASSERT(num==vn.Length());
	ASSERT(num==vp.Length());

	SetEditFlag(SHELL_VERTICES_CHANGED | SHELL_VERTEX_ATTRIBUTES_CHANGED);


	shellVerticesOk=gtrue; // vertices are up to date					

	// is the facelist of the shell Computed ??

	if (!shellFacelistOk) {

	this->f.SetLength(m_iVTessellations*m_iUTessellations*2*4);
	
	int *f = this->f.DataRW();

	if (f == NULL) return -1; // out of memory


	//TRACE("the facelist***********************# triangles = %d *******\n",m_iVTessellations*m_iUTessellations*2);
	num = 0;

	for( u=0; u<m_iUTessellations; u++)
	{

		for( v=0; v<m_iVTessellations; v++ )
		{
			*f++ = 3;							//TRACE("#%i: ", num*2);
			*f++ = num;							//TRACE("%i ", num);
			*f++ = num + m_iVTessellations + 1; //TRACE("%i ", num + m_iUTessellations +1);
			*f++ = num + 1;						//TRACE("%i \n", num + 1);

			*f++ = 3;							//TRACE("#%i: ", num*2+1);
			*f++ = num + 1;						//TRACE("%i ", num + 1);	
			*f++ = num + m_iVTessellations +1;	//TRACE("%i ", num + m_iUTessellations+1);

			*f++ = num + m_iVTessellations + 2; //TRACE("%i \n", num+ m_iUTessellations+2);	
			
			num++;//number of quads
		}
		//a new line
		num++;
		//TRACE("\n");

	}
	ASSERT((f - this->f.Data()) == this->f.Length());



	SetEditFlag(SHELL_FACELIST_CHANGED);
	nfaces = m_iVTessellations*m_iUTessellations*2;

	shellFacelistOk = gtrue;
	tessellationChanged = gfalse;
	}
	
	num = 0;
/*
    vnp=vn.DataRW(); 


	for( u=0; u<m_iUTessellations; u++)
	{

		for( v=0; v<m_iVTessellations; v++ )
		{
			//
			// Normalize the normal
			//
				
			//
			//while normal degenerated
			//
			Len = (float)sqrt( vnp->x * vnp->x + vnp->y * vnp->y + vnp->z * vnp->z );
			if (Len <  .01f)
			{
					vnp->x = vn[num + 1].x;
					vnp->y = vn[num + 1].y;
					vnp->z = vn[num + 1].z;
				
					Len = (float)sqrt( vnp->x * vnp->x + vnp->y * vnp->y + vnp->z * vnp->z );
					if (Len <  .00001f)
					{
						vnp->x = vn[num + m_iVTessellations + 1].x;
						vnp->y = vn[num + m_iVTessellations + 1].y;
						vnp->z = vn[num + m_iVTessellations + 1].z;
						Len = (float)sqrt( vnp->x * vnp->x + vnp->y * vnp->y + vnp->z * vnp->z );
					}		
			}				
			
			vnp->x /= Len;
			vnp->y /= Len;
			vnp->z /= Len;	
			vnp++;
			num++;
		}
		
		//now the last point in a line
		Len = (float)sqrt( vn[num].x * vn[num].x + vn[num].y * vn[num].y + vn[num].z * vn[num].z );
		if (Len <  .00001f)
		{
			vn[num].x = vn[num - 1].x;
			vn[num].y = vn[num - 1].y;
			vn[num].z = vn[num - 1].z;
				
			Len = (float)sqrt( vn[num].x * vn[num].x + vn[num].y * vn[num].y + vn[num].z * vn[num].z );
			if (Len <  .00001f)
			{
				vn[num].x = vn[num + m_iVTessellations + 1].x;
				vn[num].y = vn[num + m_iVTessellations + 1].y;
				vn[num].z = vn[num + m_iVTessellations + 1].z;
			}		
		}				
		
		Len = (float)sqrt( vn[num].x * vn[num].x + vn[num].y * vn[num].y + vn[num].z * vn[num].z );
			
		vn[num].x /= Len;
		vn[num].y /= Len;
		vn[num].z /= Len;				
		//a new line
		num++;
		TRACE("\n");
	}
*/
	while (num < (numVerts)) {
		Len = (float)sqrt( vn[num].x * vn[num].x + vn[num].y * vn[num].y + vn[num].z * vn[num].z );
		/*
		if (Len <  .00001f)
		{
			vn[num].x = vn[num - 1].x;
			vn[num].y = vn[num - 1].y;
			vn[num].z = vn[num - 1].z;
				
			Len = (float)sqrt( vn[num].x * vn[num].x + vn[num].y * vn[num].y + vn[num].z * vn[num].z );
			if (Len <  .00001f)
			{
				vn[num].x = vn[num - m_iVTessellations - 1].x;
				vn[num].y = vn[num - m_iVTessellations - 1].y;
				vn[num].z = vn[num - m_iVTessellations - 1].z;
			}	
			
			Len = (float)sqrt( vn[num].x * vn[num].x + vn[num].y * vn[num].y + vn[num].z * vn[num].z );
			if (Len <  .00001f)
			{
				vn[num].x = vn[num + 1].x;
				vn[num].y = vn[num + 1].y;
				vn[num].z = vn[num + 1].z;
			}
			Len = (float)sqrt( vn[num].x * vn[num].x + vn[num].y * vn[num].y + vn[num].z * vn[num].z );
		}				
		*/	
		vn[num].x /= Len;
		vn[num].y /= Len;
		vn[num].z /= Len;
		num++;
	}
/*
	Len = (float)sqrt( vn[num].x * vn[num].x + vn[num].y * vn[num].y + vn[num].z * vn[num].z );				
			
	vn[num].x /= Len;
	vn[num].y /= Len;
	vn[num].z /= Len;

*/	

	if (texCompute)
	{
	
	float chord_u, chord_v;
	Point diff;
	chord_u = chord_v  = 0.f;
	num = 0;

	for( u=0; u<=m_iUTessellations; u++)
	{
		chord_v = 0;
		
		if (u > 0)
			{
				diff = vertices[num]-vertices[num-m_iVTessellations-1];
				chord_u += diff.Length();
			}

		for( v=0; v<=m_iVTessellations; v++ )
		{

			if (v > 0)
			{
				diff = vertices[num]-vertices[num-1];
				chord_v += diff.Length();
			}

			vp[num].x = chord_u;			TRACE("#%i: chord_u %f ",num, chord_u);
			vp[num].y = chord_v;			TRACE("chord_v %f \n", chord_v);
			//vp[num].z = 0.f;
			//m_TexCoords.Set1(num, texCoord);
				
			num++;//number of quads
		}
		//a new line
		TRACE("\n");
	}

	//Point p;
	for (u=0; u<numVerts; u++)
	{
			//p = m_TexCoords[u];
			Point &p = vp[u];
			p.x /= chord_u;
			p.y /= chord_v;
			//m_TexCoords.Set1(u, p);
			TRACE("#%i: chord_u %f ",u, p.x);
			TRACE("chord_v %f \n", p.y);
	
	}

	//SetVP(m_TexCoords);

	}//end texcompute

	UpdateActions();

    SetFlag(SHELL_TRIANGULATED);
    UnSetAction(SHELL_COMPUTE_VNORMALS); // we already set vp 


	return 1;
}


Point& BilinearInterpolate 
	 (const Point& a,const Point& b,const Point& c,const Point& d,
	  float u,float v, Point &result)
{

  float u1 = 1.0-u;
  float v1 = 1.0-v;
  
  result = a*(u*v) + b*(u1*v) + c*(u*v1) + d*(u1*v1);

  return(result);
}


/*
	Evaluate point on surface using the current computed tesselation
*/

gbool GNurbsShell::EvaluateOnTessellation(float pu, float pv, Point &p, Point *vn, Point *vp)
{
		
		if ( tessellationChanged || !shellVerticesOk )
			ComputeShell();	
	
		float gu = (m_iUTessellations-1) * (pu - m_UKnots[m_iUDegree]) / (m_UKnots[m_iUKnots-m_iUOrder] - m_UKnots[m_iUDegree]);
		float gv = (m_iVTessellations-1) * (pv- m_VKnots[m_iVDegree]) / (m_VKnots[m_iVKnots-m_iVOrder] - m_VKnots[m_iVDegree]);
		
		float fu;
		float fv;

		// map to uniform space grid point 
		int iu = fu=floor(gu);
		if (iu<0) return gfalse;

		int iv = fv=floor(gv);
		if (iv<0) return gfalse;

/* 

		if (iszero(gu-fgu)) 
			p.gridStatus = G_ON_ULINE;
		else p.gridStatus =0;
		if (iszero(gv-fgv)) 
			p.gridStatus |= G_ON_VLINE;
*/

		fu=1.0-(gu-fu);
		fv=1.0-(gv-fv);
		
		const Point *v=this->v.Data();

		// get the 4 quad indices
		int pu1=iu*(m_iVTessellations+1)+iv;
		int pu2=(iu+1)*(m_iVTessellations+1)+iv;

		if (iu == (m_iUTessellations+1)) pu2=pu1;
		else if (iu > (m_iUTessellations+1)) return FALSE;

		int pv1= pu1+1;
		int pv2= pu2+1;
		
		if (iv == (m_iVTessellations+1)) pv2=pv1;
		else if (iv > (m_iVTessellations+1)) return FALSE;

		// the point 
		BilinearInterpolate(v[pu1],v[pu2],v[pv1],v[pv2],fu,fv,p);

		// the normal 
		if (vn) {
			v=this->vn.Data();
			if (v) BilinearInterpolate(v[pu1],v[pu2],v[pv1],v[pv2],fu,fv,*vn);
			vn->Normalize();

		}
		// the normal 
		if (vp) {
			v=this->vp.Data();
			if (v) BilinearInterpolate(v[pu1],v[pu2],v[pv1],v[pv2],fu,fv,*vp);
		}

		return TRUE;


}


// -----------------------------------------------------------------------
// void GNurbsShell::EvaluateBasisFunctions( void )
//
// -----------------------------------------------------------------------
void GNurbsShell::EvaluateBasisFunctions()
{
	unsigned int i, j, idx;
	float u, uinc;
	float v, vinc;

	//
	// First evaluate the U basis functions and derivitives at uniformly spaced u values
	//
	idx = 0;
	u = m_UKnots[idx+m_iUDegree];
	uinc = (m_UKnots[m_iUKnots-m_iUOrder] - m_UKnots[m_iUDegree])/(m_iUTessellations);

	for( i=0; i<=m_iUTessellations; i++ )
	{
		while( (idx < m_iUKnots - m_iUDegree*2 - 2) && (u >= m_UKnots[idx+m_iUDegree+1] ) )
			idx++;

		m_TessUKnot[i] = idx+m_iUDegree;

		//
		// Evaluate using Horner's method -- switch this to forward differences before
		// optimizing for Katmai
		//
		for( j=0; j<m_iUOrder; j++)
		{
			m_UBasis[(i*m_iUOrder+j) * VECTOR_SIZE] = m_UBasisCoefficients[ (idx * m_iUOrder + j) * m_iUOrder + m_iUDegree ];
			m_dUBasis[(i*m_iUOrder+j) * VECTOR_SIZE] = m_UBasis[(i*m_iUOrder+j) * VECTOR_SIZE] * m_iUDegree;
			for( int k=m_iUDegree-1; k>=0; k-- ) // if k unsigned, overflow 
			{
				m_UBasis[(i*m_iUOrder+j)*VECTOR_SIZE] = m_UBasis[ (i*m_iUOrder+j)*VECTOR_SIZE ] * u + 
					m_UBasisCoefficients[ (idx * m_iUOrder + j) * m_iUOrder + k ];
				if( k>0)
				{
					m_dUBasis[(i*m_iUOrder+j)*VECTOR_SIZE] = m_dUBasis[(i * m_iUOrder+j)*VECTOR_SIZE] * u + 
						m_UBasisCoefficients[ (idx * m_iUOrder + j) * m_iUOrder + k ] * k;
				}
			}
			m_UBasis[(i*m_iUOrder+j) * VECTOR_SIZE + 1 ] = 
			m_UBasis[(i*m_iUOrder+j) * VECTOR_SIZE + 2 ] = 
			m_UBasis[(i*m_iUOrder+j) * VECTOR_SIZE + 3 ] = m_UBasis[(i*m_iUOrder+j) * VECTOR_SIZE ];
			m_dUBasis[(i*m_iUOrder+j) * VECTOR_SIZE + 1 ] = 
			m_dUBasis[(i*m_iUOrder+j) * VECTOR_SIZE + 2 ] = 
			m_dUBasis[(i*m_iUOrder+j) * VECTOR_SIZE + 3 ] = m_dUBasis[(i*m_iUOrder+j) * VECTOR_SIZE ];
		}

		u += uinc;
	}

	//
	// Finally evaluate the V basis functions at uniformly spaced v values
	//
	idx = 0;
	v = m_VKnots[idx+m_iVDegree];
	vinc = (m_VKnots[m_iVKnots-m_iVOrder] - m_VKnots[m_iVDegree])/(m_iVTessellations);

	for( i=0; i<=m_iVTessellations; i++ )
	{
		while( (idx < m_iVKnots - m_iVDegree*2 - 2) && (v >= m_VKnots[idx+m_iVDegree+1] ) )
			idx++;

		m_TessVKnot[i] = idx+m_iVDegree;

		//
		// Evaluate using Horner's method -- switch this to forward differences before
		// optimizing for Katmai
		//
		for( j=0; j<m_iVOrder; j++)
		{
			m_VBasis[(i*m_iVOrder+j)*VECTOR_SIZE] = m_VBasisCoefficients[ (idx * m_iVOrder + j) * m_iVOrder + m_iVDegree ];
			m_dVBasis[(i*m_iVOrder+j)*VECTOR_SIZE] = m_VBasis[(i*m_iVOrder+j)*VECTOR_SIZE] * m_iVDegree;
			for(int k=m_iVDegree-1; k>=0; k-- )
			{
				m_VBasis[(i*m_iVOrder+j)*VECTOR_SIZE] = m_VBasis[ (i*m_iVOrder+j)*VECTOR_SIZE ] * v + 
					m_VBasisCoefficients[ (idx * m_iVOrder + j) * m_iVOrder + k ];
				if( k>0)
				{
					m_dVBasis[(i*m_iVOrder+j)*VECTOR_SIZE] = m_dVBasis[(i * m_iVOrder+j)*VECTOR_SIZE] * v + 
						m_VBasisCoefficients[ (idx * m_iVOrder + j) * m_iVOrder + k ] * k;
				}
			}
			m_VBasis[(i*m_iVOrder+j) * VECTOR_SIZE + 1 ] = 
			m_VBasis[(i*m_iVOrder+j) * VECTOR_SIZE + 2 ] = 
			m_VBasis[(i*m_iVOrder+j) * VECTOR_SIZE + 3 ] = m_VBasis[(i*m_iVOrder+j) * VECTOR_SIZE ];
			m_dVBasis[(i*m_iVOrder+j) * VECTOR_SIZE + 1 ] = 
			m_dVBasis[(i*m_iVOrder+j) * VECTOR_SIZE + 2 ] = 
			m_dVBasis[(i*m_iVOrder+j) * VECTOR_SIZE + 3 ] = m_dVBasis[(i*m_iVOrder+j) * VECTOR_SIZE ];
		}
		v += vinc;
	}
}



// -----------------------------------------------------------------------
// float GNurbsShell::ComputeCoefficient( float *fKnots, int iInterval, int i, int p, int k )
// void GNurbsShell::ComputeBasisCoefficients( void )
//
//
// Determines the polynomial coefficients from the knot vector
//
// Remember that the b-spline basis functions of degree p on knot interval
// i = (Ni,p) defined on a knot vector u = {U0, U1, ..., Um} are defined:
//
// Ni,0(u) = 1 if Ui <= u < Ui+1
//			 0 otherwise
//
//             u - Ui               Ui+p+1 - U
// Ni,p(u) = ---------- * Ni,p-1 + ------------- * Ni+1,p-1
//            Ui+p - Ui            Ui+p+1 - Ui+1
//
// 
// For some degree p on interval i, there exist p+1 polynomials of 
// degree p of the form:
//
//  Ci,p,0 + Ci,p,1 * u^1 + Ci,p,2 * u^2 + ... + Ci,p,p * u^p
//
// I derived a recursive formula for these constant coefficients as
//
//   Ci,0,0 = Ni,0(u)   (i.e. Ci,0,0 will be either 0 or 1)
//
// For p > 0
//              Ui+p+1 * Ci+1,p-1,0       UiCi,p-1,0
//    Ci,p,0 = ---------------------  -  ------------  
//                Ui+p+1 - Ui+1           Ui+p - Ui
//
//              Ci,p-1,p-1       Ci+1,p-1,p-1
//    Ci,p,p = ------------  -  ---------------  
//               Ui+p - Ui       Ui+p+1 - Ui+1
//
// For 0<k<p
//              Ci,p-1,k-1 u Ui * Ci,p-1,k       Ci+1,p-1,k-1 - Ui+p+1 * Ci+1,p-1,k
//    Ci,p,k = ----------------------------  -  ------------------------------------
//                      Ui+p - Ui                           Ui+p+1 - Ui+1
//    
//
// From this, for a pth degree b-spline, for each interval i, there are 
// p+1 b-spline basis functions that are non-zero and each one has p+1
// coefficients.  The next two functions compute those coefficients for the
// various intervals
// -----------------------------------------------------------------------
float GNurbsShell::ComputeCoefficient( float *fKnots, int iInterval, int i, int p, int k )
{
    float fResult = 0.0f;

    if( p == 0 )
    {
        if( i == iInterval )
            fResult = 1.0f;
    }
    else if( k == 0 )
    {
        if( fKnots[i+p] != fKnots[i])
            fResult -= fKnots[i] * ComputeCoefficient( fKnots, iInterval, i, p-1, 0 ) 
								/ (fKnots[i+p] - fKnots[i]);
        if( fKnots[i+p+1] != fKnots[i+1] )
            fResult += fKnots[i+p+1] * ComputeCoefficient( fKnots, iInterval, i+1, p-1, 0 ) 
			/ (fKnots[i+p+1] - fKnots[i+1]);
    }
    else if( k == p )
    {
        if( fKnots[i+p] != fKnots[i] )
            fResult += ComputeCoefficient( fKnots, iInterval, i, p-1, p-1 ) / (fKnots[i+p] - fKnots[i]);
        if( fKnots[i+p+1] != fKnots[i+1] )
            fResult -= ComputeCoefficient( fKnots, iInterval, i+1, p-1, p-1 ) / (fKnots[i+p+1] - fKnots[i+1]);
    }
    else if( k > p )
    {
        fResult = 0.0f;
    }
    else
    {
		float C1, C2;
        if( fKnots[i+p] != fKnots[i] )
        {
            C1 = ComputeCoefficient( fKnots, iInterval, i, p-1, k-1 );
            C2 = ComputeCoefficient( fKnots, iInterval, i, p-1, k );
            fResult += (C1 - fKnots[i] * C2 ) / (fKnots[i+p] - fKnots[i] );
        }
        if( fKnots[i+p+1] != fKnots[i+1] )
        {
            C1 = ComputeCoefficient( fKnots, iInterval, i+1, p-1, k-1 );
            C2 = ComputeCoefficient( fKnots, iInterval, i+1, p-1, k );
            fResult -= (C1 - fKnots[i+p+1] * C2 ) / (fKnots[i+p+1] - fKnots[i+1] );
        }

    }

    return fResult;
}


// -----------------------------------------------------------------------
// void GNurbsShell::ComputeBasisCoefficients( void )
//
// See the comment from the function above, ComputeCoefficient()
// -----------------------------------------------------------------------
void GNurbsShell::ComputeBasisCoefficients()
{
	unsigned int i, j, k;


	for( i=0; i<m_iUCoefficients; i++)
	{
		for( j=0; j<m_iUOrder; j++)
		{
			for( k=0; k<m_iUOrder; k++)
			{
				m_UBasisCoefficients[ (i * m_iUOrder + j) * m_iUOrder + k ] =
					ComputeCoefficient( m_UKnots, i + m_iUDegree, i + j, m_iUDegree, k );
			}
		}
	}

	// hg optimize indices ???

	for( i=0; i<m_iVCoefficients; i++)
	{
		for( j=0; j<m_iVOrder; j++)
		{
			for( k=0; k<m_iVOrder; k++)
			{
				m_VBasisCoefficients[ (i * m_iVOrder + j) * m_iVOrder + k ] =
					ComputeCoefficient( m_VKnots, i + m_iVDegree, i + j, m_iVDegree, k );
			}
		}
	}

}

// -----------------------------------------------------------------------
// void Init( )
//
// -----------------------------------------------------------------------
gbool GNurbsShell::Init(int uDimension, int vDimension, 
						int uOrder, int vOrder,
					   const float *controlPoint, 
					   int numWeight,const float* weight, 
					   const float* uKnot, const float* vKnot, 
					   int nUTessellations, int nVTessellations)
{
	unsigned int i;

	shellVerticesOk = gfalse;
	shellFacelistOk = gfalse;

	m_iUControlPoints = uDimension;
	m_iVControlPoints = vDimension;
	TRACE("Number of controlPoints (u,v): %i, %i\n", m_iUControlPoints, m_iVControlPoints);

	m_fRenderQuality = m_fInitialRenderQuality = 1.0f;
	m_fBudget = 1.0f;


	m_iUDegree = uOrder-1;
	m_iVDegree = vOrder-1;
	m_iUOrder = uOrder;
	m_iVOrder = vOrder;
	m_iUKnots = m_iUOrder + m_iUControlPoints;
	m_iVKnots = m_iVOrder + m_iVControlPoints;

	m_iUCoefficients = m_iUKnots - 2 * m_iUDegree;
	m_iVCoefficients = m_iVKnots - 2 * m_iVDegree;

	m_UBasisCoefficients.SetLength( m_iUOrder * m_iUOrder * m_iUCoefficients);
	m_VBasisCoefficients.SetLength( m_iVOrder * m_iVOrder * m_iVCoefficients);
	
	m_ControlPoints.SetLength( m_iUControlPoints * m_iVControlPoints * VECTOR_SIZE );
	m_TxControlPoints.SetLength( m_iUControlPoints * m_iVControlPoints * VECTOR_SIZE );

	m_UKnots.SetLength( m_iUKnots);
	m_VKnots.SetLength( m_iVKnots);

	m_UTemp.Set( m_iVOrder * VECTOR_SIZE,0.0f ); // HG v-order or u-order ???
	m_dUTemp.Set( m_iVOrder * VECTOR_SIZE,0.0f );

	//setting the controlpoints and the weights
	int imax=(m_iUControlPoints * m_iVControlPoints);

	gbool doWeight = weight!=NULL;
	float defaultWeight = 1.0;
	// default weight ??

	if (doWeight)
		if (numWeight == 1 ) {
			defaultWeight=weight[0];
			doWeight = gfalse;
		} else if (numWeight  < imax) {
			doWeight = gfalse;
		}

	
	HPoint *cp= (HPoint *) m_ControlPoints.Data();

	//we have u-polylines with v in successive order
	//we need internally v-polys
	int n = 0;
	for(i=0; i<m_iUControlPoints; i++)
	{
		for(int j=0; j<m_iVControlPoints; j++)
		{
			cp[n].x = controlPoint[(j*m_iUControlPoints +i)*3];
			cp[n].y = controlPoint[(j*m_iUControlPoints +i)*3+1];
			cp[n].z = controlPoint[(j*m_iUControlPoints +i)*3+2];
			cp[n].w = (doWeight ? weight[j*m_iUControlPoints +i]:defaultWeight);
			n++;

		//TRACE("#%i: %f %f %f %f\n",(j*m_iUControlPoints +i),cp[n].x,cp[n].y,cp[n].z,cp[n].w);
		}
	}


	if (uKnot != NULL) 
		for(i=0; i<m_iUKnots; i++){
			m_UKnots[i] = uKnot[i];
			//TRACE("uknot %i: //%f\n", i, 	m_UKnots[i]);
		}
	else {
		ComputeKnotVector(m_iUControlPoints-1,m_iUDegree,m_UKnots);
		for(i=0; i<m_iUKnots; i++) {
			TRACE("uknot %i: //%f\n", i, 	m_UKnots[i]);
		}

	}

	if (vKnot != NULL) 
		for(i=0; i<m_iVKnots; i++) {
			m_VKnots[i] = vKnot[i];
			//TRACE("vknot %i: %f\n", i, 	m_VKnots[i]);
		}
	else {
		ComputeKnotVector(m_iVControlPoints-1,m_iVDegree,m_VKnots);
		for(i=0; i<m_iVKnots; i++) {
			TRACE("vknot %i: //%f\n", i, 	m_VKnots[i]);
		}

	}


	ComputeBasisCoefficients();	
	SetAction(SHELL_COMPUTE_BBOX);
	shellVerticesOk = gfalse;


	InitTessellationValues(nUTessellations,nVTessellations);

	return gtrue;
  }



//---------------------------//---------------------------//---------------------------
//							set the initial tesselation values
//---------------------------//---------------------------//---------------------------
void GNurbsShell::InitTessellationValues(int nUTessellations, int nVTessellations)
{

	//check if values are valid
		if (nUTessellations == 0) nUTessellations = m_iUControlPoints *2;	
		if (nVTessellations == 0) nVTessellations = m_iVControlPoints *2;
		if (nUTessellations < 0) nUTessellations = m_iUControlPoints * - nUTessellations;	
		if (nVTessellations < 0) nVTessellations = m_iVControlPoints * - nVTessellations;
	

	//specify lower tessellation bounds
		float minUTess = LOWER_TESSELLATION_BOUND * (float) nUTessellations;
		float minVTess = LOWER_TESSELLATION_BOUND * (float) nVTessellations;

	SetTessellationLimits(  max(3, (int)minUTess ),
							max(3, (int)minVTess ),
							min(1023,max(nUTessellations,159)),
							min(1023,max(nVTessellations, 159)));
	
	
	//Set the new tessellationvalues and 
	SetTessellation(nUTessellations,nVTessellations);
	
	//take the checked values!
	m_iInitialUTess = m_iUTessellations;
	m_iInitialVTess = m_iVTessellations; 

	m_fBudget = 1.0f;
	
	
		m_fRenderQuality = m_fInitialRenderQuality =
			((float) m_iInitialUTess * (float)m_iInitialVTess *2.f)/MAX_BBSURF;//Erfahrungswert
		/*	m_fInitialRenderQuality = 2.f;
		Matrix initMat;
		initMat = model2view;
		initMat[3][0] = initMat[3][1] = 0.f; 
		initMat[3][2] = .4f; 
		m_fRenderQuality = m_fInitialRenderQuality =
			(m_iInitialUTess * m_iInitialVTess)/ComputeBBSurface(initMat, projection, w, h);
		*/
		  
		//TRACE("GNurbsShell::ComputeBBSurface: m_fInitialRenderQuality = %f \n", m_fInitialRenderQuality);
	
	  
}



// -----------------------------------------------------------------------
// void GNurbsShell::SetTessellationLimits( int iMinUTessellations, int iMinVTessellations, int iMaxUTessellations, int iMaxVTessellations )
//
// -----------------------------------------------------------------------
void GNurbsShell::SetTessellationLimits( int iMinUTessellations, int iMinVTessellations, int iMaxUTessellations, int iMaxVTessellations )
{
	int i;

	if( iMaxUTessellations < iMinUTessellations )
		iMaxUTessellations = iMinUTessellations;
	if( iMaxVTessellations < iMinVTessellations )
		iMaxVTessellations = iMinVTessellations;

	iMaxUTessellations = ((iMaxUTessellations+1+3) & ~3) -1; // align to multiple of 4
	iMaxVTessellations = ((iMaxVTessellations+1+3) & ~3) -1; // align to multiple of 4


	if( (iMaxUTessellations != m_iMaxUTessellations) ||
		(iMaxVTessellations != m_iMaxVTessellations) )
	{
		m_bInitialized = FALSE;

		m_iMaxUTessellations = iMaxUTessellations;
		m_iMaxVTessellations = iMaxVTessellations;
		m_iMinUTessellations = iMinUTessellations;
		m_iMinVTessellations = iMinVTessellations;

		m_UBasis.Set( m_iUOrder * VECTOR_SIZE * (m_iMaxUTessellations+1),0.0f);
		m_VBasis.Set( m_iVOrder * VECTOR_SIZE * (m_iMaxVTessellations+1),0.0f);
		m_dUBasis.Set( m_iUOrder * VECTOR_SIZE * (m_iMaxUTessellations+1),0.0f);
		m_dVBasis.Set( m_iVOrder * VECTOR_SIZE * (m_iMaxVTessellations+1),0.0f);

		SAFE_DELETE( m_iVertexIndices[0] );
		SAFE_DELETE( m_iVertexIndices[1] );

		SAFE_DELETE( m_TessUKnot );
		SAFE_DELETE( m_TessVKnot );

		m_TessUKnot = new int[ m_iMaxUTessellations+1 ];
		m_TessVKnot = new int[ m_iMaxVTessellations+1 ];

		m_pVertexPos.SetLength( (m_iMaxVTessellations+1) * VECTOR_SIZE );

		m_pNormals.SetLength((m_iMaxVTessellations + 1) /* *(m_iUTessellations + 1)*/ );

#ifdef _KATMAI
		kv.SetLength((m_iMaxVTessellations + 1));
		kvn.SetLength((m_iMaxVTessellations + 1));
		
		vtl.SetLength( (sizeof(D3DTLVERTEX) / sizeof(float)) * ((2*(m_iMaxVTessellations+1)))); // katmai uses at least multiple of 4 
#endif


		if (triStrip) {
		
			m_iVertices = 2 * (iMaxVTessellations + 1);
			m_iVertexIndices[0] = new WORD[ m_iVertices ];
			m_iVertexIndices[1] = new WORD[ m_iVertices ];

			//
			// Setup the tri-strip indices here since these will only change
			// if the number of tessellations changes
			//

			for( i=0; i<m_iVertices/2; i++ )
			{
				m_iVertexIndices[0][i*2] = i*2;
				m_iVertexIndices[0][i*2+1] = i*2+1;
				m_iVertexIndices[1][i*2] = i*2+1;
				m_iVertexIndices[1][i*2+1] = i*2; 
			}
		}
		else {
			m_iVertices = 3* 2 * (iMaxVTessellations );
			m_iVertexIndices[0] = new WORD[ m_iVertices ];
			m_iVertexIndices[1] = new WORD[ m_iVertices ];

			//
			// Setup the triangle indices here since these will only change
			// if the number of tessellations changes
			//

			for( i=0; i<iMaxVTessellations; i++ )
			{
				m_iVertexIndices[0][i*6+0] = i*2;
				m_iVertexIndices[0][i*6+1] = i*2+2;
				m_iVertexIndices[0][i*6+2] = i*2+1;

				m_iVertexIndices[0][i*6+3] = i*2+1;
				m_iVertexIndices[0][i*6+4] = i*2+2;
				m_iVertexIndices[0][i*6+5] = i*2+3;

				m_iVertexIndices[1][i*6+0] = i*2+1;
				m_iVertexIndices[1][i*6+1] = i*2+3;
				m_iVertexIndices[1][i*6+2] = i*2+0;

				m_iVertexIndices[1][i*6+3] = i*2+0;
				m_iVertexIndices[1][i*6+4] = i*2+3;
				m_iVertexIndices[1][i*6+5] = i*2+2;

			}

		}

		m_bInitialized = TRUE;
	}
}



//---------------------------//---------------------------//---------------------------
//							set the current tesselation limit
//---------------------------//---------------------------//---------------------------
void GNurbsShell::SetTessellation(int nUTessellations, int nVTessellations)
{

	//clamp to min-max
		if (nUTessellations > (int)m_iMaxUTessellations) nUTessellations = m_iMaxUTessellations;
		if (nVTessellations > (int)m_iMaxVTessellations) nVTessellations = m_iMaxVTessellations;
		
		if (nUTessellations < m_iMinUTessellations) nUTessellations = m_iMinUTessellations;
		if (nVTessellations < m_iMinVTessellations) nVTessellations = m_iMinVTessellations;
	
	// align to multiple of 4
		nUTessellations = ((nUTessellations+1+3) & ~3) -1; 
		nVTessellations = ((nVTessellations+1+3) & ~3) -1; 

	if ((m_iUTessellations != nUTessellations) || (m_iVTessellations != nVTessellations)) 
	{
		tessellationChanged = gtrue;		
		TRACE("************ New tessel (%p): %d %d \n",(void *) this,m_iUTessellations,m_iVTessellations);
	}

	m_iUTessellations = nUTessellations;
	m_iVTessellations = nVTessellations;
}




//---------------------------//---------------------------//---------------------------							//
//										LOD				
//---------------------------//---------------------------//---------------------------
int GNurbsShell::SetRenderQuality(float budget)
{
	m_fRenderQuality = budget * m_fInitialRenderQuality;
	return gtrue;
}

/*
pixel scheint kein gutes mass zu sein:
wenn in einem grossen Fenster ein obj eckig erscheint,
sieht es in einem kleinen Fenster genauso aus, obwohl 
die pixelflaeche um ein vielfaches kleiner ist

math:
macht die bbsurf-kurve steiler, d.h. die Abnahme der tris
ist staerker

*/

float GNurbsShell::ComputeBBSurface(Matrix &model2view, Matrix &projection, float w, float h)
{

		/*
	BBox bScreen = (model2view*state.projection) * GetBBox();

	Point diagonal;
	diagonal = bScreen.Size();
	diagonal.x*=w;
	diagonal.y*=h;

	m_fBBSurface = diagonal.Length();
*/

	BBox b = GetBBox();
	//b = (model2view*projection) * b;
	//Point center = b.Center();
	
	b *= model2view;
	b *= projection;

	Point rr=b.Size();
	
	float fBBSurface = rr.Length();//max(rr.x,rr.y);
	if (fBBSurface > MAX_BBSURF) 
		fBBSurface = MAX_BBSURF;
	//fBBSurface = max(fBBSurface,rr.z);
	//fBBSurface *= (w*h);
	//m_fBBSurface + sqr(
	// pixel number is greatly increasing, extra sqrt
//	fBBSurface = ((fBBSurface*2.0*w*fBBSurface*2.0*h)); // pixel 

//	TRACE("BBox Screen  %f durchmesser der bbox %f\n", b.mi.z, m_fBBSurface);

#ifdef GN_LOG
		bbsurf  << model2view.x[3][2] << ' ' << fBBSurface << endl;
		tris  << model2view.x[3][2] << ' ' << (m_iUTessellations*m_iVTessellations*2) << endl;
		scaleStream<< model2view.x[3][2] << ' ';
#endif

	return fBBSurface;
}


int GNurbsShell::ComputeTessellationValues(int mode)
{
	float scale;
	int nUTessellations;
	int nVTessellations;

	switch (mode) {
	case GNURBS_STATIC_TESSELATION:
	default :
		scale = m_fRenderQuality/m_fInitialRenderQuality;
		break;
	case GNURBS_FRAMERATE_TESSELATION:
		scale = m_fRenderQuality/m_fInitialRenderQuality;
		break;
	case GNURBS_DYNAMIC_TESSELATION:
		scale = sqrt((m_fInitialRenderQuality*m_fBBSurface)/(float)m_iInitialUTess/(float)m_iInitialVTess/2.f);
		break;
	case GNURBS_DYNAMIC_FRAMERATE_TESSELATION:
		//m_fRenderQuality is scaled by the budget
		scale = sqrt((m_fRenderQuality*m_fBBSurface)/(float)m_iInitialUTess/(float)m_iInitialVTess/2.f);
		break;

	} 

	//if (mode == GNURBS_DYNAMIC_TESSELATION && scale > 1) scale = 1;

	nUTessellations = scale * m_iInitialUTess;
	nVTessellations = scale * m_iInitialVTess;

	nUTessellations= max(3,nUTessellations);
	nVTessellations= max(3,nVTessellations);

#define lerp(a,b,f) ((a)*f + (b) * (1.0 -(f)))

	//nUTessellations=lerp(m_iUTessellations,nUTessellations,0.75);
	//nVTessellations=lerp(m_iVTessellations,nVTessellations,0.75);


	//TRACE("%p old (%d %d) new (%d %d) area %f qual %f \n",this,m_iUTessellations,m_iVTessellations
	//	,nUTessellations,nVTessellations,m_fBBSurface,m_fRenderQuality);
	
	// if below 10% change in quads ignore 
	if ( ((float) fabs(nUTessellations-m_iUTessellations) / (float) m_iInitialUTess) <.1f) 
	{
		#ifdef GN_LOG
			scaleStream  << (float)m_iUTessellations/(float)m_iInitialUTess << endl;
		#endif

		return gfalse;
	}


#ifdef GN_LOG
		scaleStream  << scale << endl;
#endif



	SetTessellation(nUTessellations,nVTessellations);

	return gtrue;
}


void GNurbsShell::setControlPoints(int numCtrlPts, const float *ctrlPts, 
								   int numWeights, const float *weights)
{
	//setting the controlpoints and the weights

	/*for(i=0; i<(m_iUControlPoints * m_iVControlPoints); i++)
	{
	 	m_ControlPoints[i].x= *controlPoint++;
		m_ControlPoints[i].y = *controlPoint++;
		m_ControlPoints[i].z = *controlPoint++;
		m_ControlPoints[i].w = weight[i];
		TRACE("%f %f %f %f\n",	m_ControlPoints[i].x,	m_ControlPoints[i].y,	m_ControlPoints[i].z,  	m_ControlPoints[i].w);
	}*/

	SetAction(SHELL_COMPUTE_BBOX);
	shellVerticesOk = gfalse;

}

void GNurbsShell::setControlPoints(int num, const float *ctrlPts)
{
	int imax=(m_iUControlPoints * m_iVControlPoints);
	
	HPoint *cp= (HPoint *) m_ControlPoints.Data();
	
	if (cp == NULL || ctrlPts == NULL)	return;
	
	if (num<imax) imax = num;

	//we have u-polylines with v in successive order
	//we need internally v-polys
	if (m_iUControlPoints==0 || m_iVControlPoints==0) {
		TRACE("GNurbsShell::setControlPoints: m_iU/VControlPoints is not initialized!\n");
		return;
	}

	if ((m_iUControlPoints*m_iVControlPoints) != num)
		TRACE("GNurbsShell::setControlPoints: bad number of controlpoints!\n");

	int n = 0;
	for(int i=0; i<m_iUControlPoints; i++)
	{
		for(int j=0; j<m_iVControlPoints; j++)
		{
			cp[n].x = ctrlPts[(j*m_iUControlPoints +i)*3];
			cp[n].y = ctrlPts[(j*m_iUControlPoints +i)*3+1];
			cp[n].z = ctrlPts[(j*m_iUControlPoints +i)*3+2];
			n++;
		}
	}


	SetAction(SHELL_COMPUTE_BBOX);
	shellVerticesOk = gfalse;

}

void GNurbsShell::setWeight(int num, const float * weights)
{
	int imax=(m_iUControlPoints * m_iVControlPoints);
	
	HPoint *cp= (HPoint *) m_ControlPoints.Data();
	
	if (cp == NULL)	return;

	// default weight ??

	float w = 1.0f;
	if (num == 1 ) {
		w=weights[0];
		num = 0;
	}

	if (num == 0 || weights == NULL) { // all have same weight
		for(int i=imax; i>0; i--,cp++)
		{
			cp->w = w;
		}

	} else {

	//we have u-polylines with v in successive order
	//we need internally v-polys

	if (m_iUControlPoints==0 || m_iVControlPoints==0) {
		TRACE("GNurbsShell::setControlPoints: m_iU/VControlPoints is not initialized!\n");
		return;
	}

	if ((m_iUControlPoints*m_iVControlPoints) != num)
		TRACE("GNurbsShell::setControlPoints: bad number of controlpoints!\n");

	int n = 0;
	for(int i=0; i<m_iUControlPoints; i++)
	{
		for(int j=0; j<m_iVControlPoints; j++)
		{
			cp[n].w = weights[j*m_iUControlPoints +i];
			n++;

		//TRACE("#%i: %f %f %f %f\n",(j*m_iUControlPoints +i),cp[n].x,cp[n].y,cp[n].z,cp[n].w);
		}
	}


	}
	SetAction(SHELL_COMPUTE_BBOX);
	shellVerticesOk = gfalse;

}

// -----------------------------------------------------------------------
// void GNurbsShell::ComputeBBox(BBox &box)
// compute bounding box, based on projected control points
// hg really projected or unprojected ?? 
// -----------------------------------------------------------------------

void GNurbsShell::ComputeBBox(BBox &bbox)
{
	float rhw;

	int imax=(m_iUControlPoints * m_iVControlPoints);
	
	const HPoint *cp= (HPoint *) m_ControlPoints.Data();
	
	if (cp == NULL) {
		bbox.SetEmpty();
		return;
	}


	rhw = 1.0f / cp->w;
	//rhw = 1.0f; // xxx
	bbox.mi.x = cp->x * rhw;
	bbox.ma.x = cp->x * rhw;
	bbox.mi.y = cp->y * rhw;
	bbox.ma.y = cp->y * rhw;
	bbox.mi.z = cp->z * rhw;
	bbox.ma.z = cp->z * rhw;
	cp++;

	for(int i=imax-1; i>0; i--,cp++)
	{
			rhw = 1.0f / cp->w;
			if( cp->x * rhw < bbox.mi.x )
				bbox.mi.x = cp->x * rhw;
			else if( cp->x * rhw > bbox.ma.x )
				bbox.ma.x = cp->x * rhw;
			if( cp->y * rhw < bbox.mi.y )
				bbox.mi.y = cp->y * rhw;
			else if( cp->y * rhw > bbox.ma.y )
				bbox.ma.y = cp->y * rhw;
			if( cp->z * rhw < bbox.mi.z )
				bbox.mi.z = cp->z * rhw;
			else if( cp->z * rhw > bbox.ma.z )
				bbox.ma.z = cp->z * rhw;
	}
	bboxMin = bbox.mi;
	bboxMax = bbox.ma;
}

// -----------------------------------------------------------------------
// void GNurbsShell::TransformControlPoints( void )
//
// -----------------------------------------------------------------------
void GNurbsShell::TransformControlPoints(const Matrix &m)
{
//	float zMin;
//	float rhw;

//	zMin = FLT_MAX;

	
	const HPoint *pControlPoints =	GetHPoint(m_ControlPoints);
	HPoint *pTxControlPoints = GetHPoint(m_TxControlPoints);
#if 0
	int u,v;
	for( u=0; u<m_iUControlPoints; u++ )
	{
		for( v=0; v<m_iVControlPoints; v++ )
		{
			pTxControlPoints[u*m_iVControlPoints+v].x = 
				pControlPoints[u*m_iVControlPoints+v].x * m.x[0][0] +
				pControlPoints[u*m_iVControlPoints+v].y * m.x[1][0] +
				pControlPoints[u*m_iVControlPoints+v].z * m.x[2][0] +
				pControlPoints[u*m_iVControlPoints+v].w * m.x[3][0];
			pTxControlPoints[u*m_iVControlPoints+v].y = 
				pControlPoints[u*m_iVControlPoints+v].x * m.x[0][1] +
				pControlPoints[u*m_iVControlPoints+v].y * m.x[1][1] +
				pControlPoints[u*m_iVControlPoints+v].z * m.x[2][1] +
				pControlPoints[u*m_iVControlPoints+v].w * m.x[3][1];
			pTxControlPoints[u*m_iVControlPoints+v].z = 
				pControlPoints[u*m_iVControlPoints+v].x * m.x[0][2] +
				pControlPoints[u*m_iVControlPoints+v].y * m.x[1][2] +
				pControlPoints[u*m_iVControlPoints+v].z * m.x[2][2] +
				pControlPoints[u*m_iVControlPoints+v].w * m.x[3][2];
			pTxControlPoints[u*m_iVControlPoints+v].w = 
				pControlPoints[u*m_iVControlPoints+v].x * m.x[0][3] +
				pControlPoints[u*m_iVControlPoints+v].y * m.x[1][3] +
				pControlPoints[u*m_iVControlPoints+v].z * m.x[2][3] +
				pControlPoints[u*m_iVControlPoints+v].w * m.x[3][3];

			//rhw = 1.0f / pTxControlPoints[u*m_iVControlPoints+v].w;

		}
	}
#else
	int imax=(m_iUControlPoints * m_iVControlPoints);

	for( int i=0; i < imax; i++ )
	{
			pTxControlPoints[i].x = 
				pControlPoints[i].x * m.x[0][0] +
				pControlPoints[i].y * m.x[1][0] +
				pControlPoints[i].z * m.x[2][0] +
				pControlPoints[i].w * m.x[3][0];
			pTxControlPoints[i].y = 
				pControlPoints[i].x * m.x[0][1] +
				pControlPoints[i].y * m.x[1][1] +
				pControlPoints[i].z * m.x[2][1] +
				pControlPoints[i].w * m.x[3][1];
			pTxControlPoints[i].z = 
				pControlPoints[i].x * m.x[0][2] +
				pControlPoints[i].y * m.x[1][2] +
				pControlPoints[i].z * m.x[2][2] +
				pControlPoints[i].w * m.x[3][2];
			pTxControlPoints[i].w = 
				pControlPoints[i].x * m.x[0][3] +
				pControlPoints[i].y * m.x[1][3] +
				pControlPoints[i].z * m.x[2][3] +
				pControlPoints[i].w * m.x[3][3];

			//rhw = 1.0f / pTxControlPoints[i].w;

	}
#endif

#if 0			
	Point TxbboxMin, TxbboxMax;
	
	TxbboxMin.x = 
				bboxMin.x * m.x[0][0] +
				bboxMin.y * m.x[1][0] +
				bboxMin.z * m.x[2][0] +
				m.x[3][0] ;
	TxbboxMin.y = 
				bboxMin.x * m.x[0][1] +
				bboxMin.y * m.x[1][1] +
				bboxMin.z * m.x[2][1] +
				m.x[3][1];
			TxbboxMin.z = 
				bboxMin.x * m.x[0][2] +
				bboxMin.y * m.x[1][2] +
				bboxMin.z * m.x[2][2] +
				m.x[3][2];
			rhw = 1/(
				bboxMin.x * m.x[0][3] +
				bboxMin.y * m.x[1][3] +
				bboxMin.z * m.x[2][3] +
				m.x[3][3]);	
	TxbboxMin.x *= rhw;
	TxbboxMin.y *= rhw;
	TxbboxMin.z *= rhw;

			
			TxbboxMax.x = 
				bboxMax.x * m.x[0][0] +
				bboxMax.y * m.x[1][0] +
				bboxMax.z * m.x[2][0] +
				m.x[3][0];
			TxbboxMax.y = 
				bboxMax.x * m.x[0][1] +
				bboxMax.y * m.x[1][1] +
				bboxMax.z * m.x[2][1] +
				m.x[3][1];
			TxbboxMax.z = 
				bboxMax.x * m.x[0][2] +
				bboxMax.y * m.x[1][2] +
				bboxMax.z * m.x[2][2] +
				m.x[3][2];
			rhw = 1/(
				bboxMax.x * m.x[0][3] +
				bboxMax.y * m.x[1][3] +
				bboxMax.z * m.x[2][3] +
				m.x[3][3]);
	TxbboxMax.x *= rhw;
	TxbboxMax.y *= rhw;
	TxbboxMax.z *= rhw;

		

			Point Length;
			Length = (TxbboxMax - TxbboxMin);
			m_fBBSurface = Length.Length();
			TRACE("TxbboxMin.z %f durchmesser der bbox %f\n", TxbboxMin.z, m_fBBSurface);
			m_fBBSurface  /= TxbboxMin.z;
			m_fBBSurface *= m_fBBSurface;
			TRACE("bbsurface %f\n", m_fBBSurface);
#endif
}


// do boundingbox computation on control vertices
int GNurbsShell::Do(GBBoxTraversal &state)       
{ 
	state.Union(GetBBox());
	return(1);
};


////////////////////////////////////////////////////////
//
// do rendering, check if shell need to be updated
//
/////////////////////////////////////////////////////////
int GNurbsShell::Do(GglRenderTraversal &traversalState)   
{ 

	RenderState &state = *traversalState.glState;

#ifdef _D3D


	// own lighting enabled ???
	if (state.doOwnLighting) {
		//Update(); ?? 
		if (state.faces.smooth)  // undo any FLAT changes
			state.SetLightModel(GL_SMOOTH);

	    // flush any pending state changes 
	    state.FlushState();
		return D3dRenderTL(traversalState);

	}

#endif


	if (traversalState.nurbsTessellationMode & GNURBS_DYNAMIC_TESSELATION) {

#ifdef _D3D
	GMatrixStack *mstack = traversalState.GetCurrentMatrix();

	Matrix model2view;
	Multiply(mstack->GetMatrix(),state.view,model2view);
		
	// to viewpoint scale
	float w= state.device->szClient.cx*0.5f;
	float h= state.device->szClient.cy*0.5f;

	m_fBBSurface = ComputeBBSurface(model2view, state.projection, w, h);
#else
	m_fBBSurface = 1.0f; // to do: for OpenGL 
#endif

	}

	// if (!(traversalState.nurbsTessellationMode & GNURBS_DYNAMIC_TESSELATION)) // we do not have the matrices here
	
	ComputeTessellationValues(traversalState.nurbsTessellationMode);

	// evaluate tessellation in object space , and treat as shell

	if ( tessellationChanged || !shellVerticesOk || !shellFacelistOk)
		ComputeShell();

	return GShell::Do(traversalState);

}

//@cmember GClass do ray selection 
int GNurbsShell::Do(GRaySelectTraversal &state)
{ 
	// do bbox check first 
	// BBox check first
	if (!state.CullCheck(GetBBox())) return 0;


	// no need to compute new shell if onlz tesselation changed
	if (!shellVerticesOk || !shellFacelistOk) {
		// don't compute to high tess
		if	((m_iUTessellations > (m_iInitialUTess*2) ||
		 	(m_iVTessellations > m_iInitialVTess*2) ))
			SetTessellation(m_iInitialUTess,m_iInitialVTess);

		ComputeShell();
	}
	return GShell::Do(state);
}

#ifdef _D3D



//Render using ownLighting/Transform 
int GNurbsShell::D3dRenderTL(GglRenderTraversal &traversalState)
{
	RenderState &state = *traversalState.glState;

	// TRACE(" GShell %d GNurbs shell %d %d \n",sizeof(GShell),sizeof(GNurbsShell),sizeof(Matrix));

	//return GShell::D3dRenderTL(traversalState);

	gbool culling = state.backfaceCulling; // backface culling enabled ?
	gbool singlsidelighting = culling;

	DWORD primMask;
	if (traversalState.doClipping)
		primMask =  D3DDP_DONOTUPDATEEXTENTS; 
	else  
		primMask = D3DDP_DONOTCLIP |  D3DDP_DONOTUPDATEEXTENTS;

	//if (state.enableD3DCulling) { // hw culling
		state.SetCullMode(culling  ? (state.frontFaceCW ? D3DCULL_CCW: D3DCULL_CW) : D3DCULL_NONE);
		culling = 0;
	//}

	// matrix 
	GMatrixStack *mstack = traversalState.GetCurrentMatrix();

	Matrix model2view;
	Multiply(mstack->GetMatrix(),state.view,model2view);

	//viewpoint = state.view * traversalState.physicalCamera->position;
	//// should be 0 0 0 in view space 

#ifdef _KATMAI
	if (useKatmai) {
		TransformControlPointsKatmai(model2view);
	} else 
#endif
		TransformControlPoints(model2view);
	
		// viewport translation & scale

	float cx,w;
	float cy,h;
	RECT & r=state.device->rcViewportRect;

	w=(r.right - r.left)*0.5;
	h=(r.bottom - r.top)*0.5;

	cy= r.bottom - h;
	cx= r.left + w;


	m_fBBSurface = ComputeBBSurface(model2view, state.projection, w, h);

	
	gbool updateLights= !state.lightsTransformedView; // need to transform lights
	
	updateLights = gtrue;


	// Warning update same code in GNurbsShell::Do(GglRenderTraversal)

	gbool doVp = state.textureDefined;
	gbool doVn =  !state.Unlit();

	G3Material &mat = state.currentMaterial->material;
	RGBAColor start(mat.emissive); // start with emissive color

	// do we need specular lighting ?
	gbool doSpecular;

	// cache lighting values 
	if (doVn ) {

		this->isUnlit = gfalse;
		// do we need specular lighting ?
		doSpecular = (mat.specular.r>0) || (mat.specular.g>0) || (mat.specular.b>0);


		// transform lights from world to local 
		if (updateLights) {
		G3LightHandle *l=state.lights;

		int numLights=0;

		Matrix &m = state.view;

		while ( l != NULL ) {	// for all lights 
			
			l->directionView = RotateOnly(m , *((Point *)&l->dvDirection));
			l->directionView.Normalize();
			l->positionView = m * (*((Point *)&l->dvPosition));
			
			// to do : range , attenuation 

			if (l->enabled) {

				if (numLights == 0) state.firstLight = l;

#ifdef _KATMAI
			if (useKatmai) {
			KSetNumLights(numLights+1);
			switch(l->dltType) { 					
			case  D3DLIGHT_DIRECTIONAL : 
			    KSetLightType(numLights,KDIFFUSE_LIGHT);
				KSetLightDirection(numLights, - l->directionView);
				break;

			case  D3DLIGHT_POINT : 
			case  D3DLIGHT_PARALLELPOINT : // actually not used
			    KSetLightType(numLights,KPOINT_LIGHT);
				KSetLightPosition(numLights, l->positionView);
			    KSetLightAttenuation(numLights,(l->flags & GL_ATTENUATION),l->dvAttenuation0,l->dvAttenuation1,l->dvAttenuation2);

				break;

			case  D3DLIGHT_SPOT : 
			    KSetLightType(numLights,KSPOT_LIGHT);
				KSetLightDirection(numLights, - l->directionView);
				KSetLightPosition(numLights, l->positionView);
			    KSetLightAttenuation(numLights,(l->flags & GL_ATTENUATION),l->dvAttenuation0,l->dvAttenuation1,l->dvAttenuation2);
				KSetLightSpotCone(numLights,l->cosPhi,l->cosTheta);
				KSetLightRadius(numLights,l->rangeView);
				break;
			}
			}
#endif
			numLights++;
			} // e->enabled 


			l = l->next;

		}
		state.lightsTransformedView = gtrue;
		state.numLights = numLights;
		state.materialChanged = gtrue; // already set
		if (state.fogEnabled) { // fog 
#ifdef _KATMAI
			if (useKatmai) {
				KSetLightType(numLights,KFOG_LINEAR);
				float s=state.fog.start;
				float e=state.fog.end;
				s=s*state.projection.x[2][2] + state.projection.x[3][2];
				e=e*state.projection.x[2][2] + state.projection.x[3][2];
			
				TRACE("Katmai Fog %g %g == > %g %g \n",state.fog.start,state.fog.end,s,e);
				KSetFogLinear(state.fog.color,s,e);
				KSetNumLights(numLights+1);
			}
#endif
		}


		} // updateLights

		if (state.materialChanged) { 	// only if material / lights changing

			state.materialChanged=gfalse;
			int numLights=0;
			G3LightHandle *l=state.lights;

		// setup light * material values
		while ( l != NULL ) {	// for all lights 

			l->kAmbient.r =  mat.ambient.r * l->ambientColor.r,	 l->kAmbient.g = mat.ambient.g * l->ambientColor.g, l->kAmbient.b = mat.ambient.b * l->ambientColor.b;

			l->kDiffuse.r =  mat.diffuse.r * l->dcvColor.r,
			l->kDiffuse.g =  mat.diffuse.g * l->dcvColor.g,
			l->kDiffuse.b =  mat.diffuse.b * l->dcvColor.b;

			l->kSpecular.r =  mat.specular.r * l->dcvColor.r,
			l->kSpecular.g =  mat.specular.g * l->dcvColor.g,
			l->kSpecular.b =  mat.specular.b * l->dcvColor.b;

			if (l->enabled) {

#ifdef _KATMAI
				if (useKatmai) {
					KSetLightColors(numLights, l->kAmbient,l->kDiffuse,l->kSpecular,doSpecular);
				}
#endif
				numLights++;
			}

			l = l->next;

		}
		
		state.numLights = numLights;

#ifdef _KATMAI
		if (useKatmai) {
			KSetBasicMaterial(mat.emissive,mat.diffuse.a,mat.power);
		}

#endif
		} // material changed 


	}

	HRESULT res;

	ComputeTessellationValues(traversalState.nurbsTessellationMode);

#ifdef _KATMAI
	if (useKatmai) {
		int l= (m_iVTessellations + 1);
		if (kv.Length() <l) kv.SetLength(l); // for savety check lengths of temp k arrays 
		if (kvn.Length() <l) kvn.SetLength(l);
		res = D3DRenderKatmai(state,state.projection, w,cx,h,cy,primMask);
	} else 	
#endif
	   res = D3DRenderX86(state,state.projection, w,cx,h,cy,primMask);
	
	state.trisDrawn += m_iVTessellations*m_iUTessellations*2; // nfaces
	state.primsDrawn++;
	//TRACE ("quality (tris/bbox-surface) %f\n", (float)nfaces/m_fBBSurface);

#ifdef _GSTATS
	// standalone statistics display 
	int tris=m_iVTessellations*m_iUTessellations*2;
	int nvert=(1+m_iVTessellations)*(1+m_iUTessellations);
	verticesProcessed += nvert;
	facesProcessed+= tris;
	trianglesDrawn += tris;
	if (tris > maxPrimTriCnt) maxPrimTriCnt= tris;
	if (nvert> maxPrimVertexCnt) maxPrimVertexCnt= nvert;

#endif


	return 0;
}

// -----------------------------------------------------------------------
// HRESULT GNurbsShell::Render( LPDIRECT3DDEVICE2 pd3dDevice, LPDIRECT3DMATERIAL2 lpd3dSurfMaterial )
//
// -----------------------------------------------------------------------
HRESULT GNurbsShell::D3DRenderX86( 
		RenderState &state,
		const Matrix &m_MxProjection, // projection & viewport information
		float sx,float tx, float sy,float ty,
		DWORD clipFlags	// for DrawPrimitive
		)

{
	unsigned int u, v;
	unsigned int k, l;
	int uKnot, vKnot;
	HPoint *UTemp = GetHPoint(m_UTemp), *dUTemp = GetHPoint(m_dUTemp);
	HPoint Pw, TanU, TanV, Norm;
	float rhw;
	float Len;
	int iVertices;
	const HPoint *pTxPoints = GetHPoint(m_TxControlPoints);
	HPoint *pVertexPos =  GetHPoint(m_pVertexPos);
	unsigned int iTxOffset;
	float VBasis, dVBasis;
	int iSet = 0;
	unsigned int idx, uidx;

	if( !m_bInitialized || !m_iUTessellations || !m_iVTessellations )
		return DD_OK;

	if( (m_iUTessellations != m_iLastUTess) ||
		(m_iVTessellations != m_iLastVTess) )
	{
		EvaluateBasisFunctions();
		m_iLastUTess = m_iUTessellations;
		m_iLastVTess = m_iVTessellations;
	}

	iVertices = 2 * (m_iVTessellations + 1);
	
	D3DTLVERTEX * m_pScreenVertices = GetScreenVertices(iVertices);
/*
	pd3dDevice->GetCurrentViewport( &m_pViewport );
	d3dView.dwSize = sizeof( D3DVIEWPORT2 );
	m_pViewport->GetViewport2( &d3dView );
	m_fHalfX = (float)d3dView.dwWidth / 2.0f;
	m_fHalfY = (float)d3dView.dwHeight / 2.0f;
*/

	//m_fHalfX = w;
	//m_fHalfY = h;


	float Proj00 = m_MxProjection.x[0][0] * sx;// m_fHalfX;
	float Proj11 = -m_MxProjection.x[1][1] * sy; //m_fHalfY;
	float Proj22 = m_MxProjection.x[2][2];
	float Proj23 = m_MxProjection.x[2][3];
	float Proj32 = m_MxProjection.x[3][2];
	//float fHalfX = m_fHalfX;
	//float fHalfY = m_fHalfY;

	// xxxxxxxx PrepLights( pd3dDevice );

	float Fuinc = 1.0f/(float)m_iUTessellations;
	float Fvinc = 1.0f/(float)m_iVTessellations;
	float Fu = 0.0f;
	float Fv;

	//
	// Step over the U and V coordinates and generate triangle strips to render
	//
	for( u=0; u<=m_iUTessellations; u++ )
	{
		uKnot = m_TessUKnot[u];

		uidx = u * m_iUOrder * VECTOR_SIZE;
		vKnot = -1;
		idx = iSet;
		Fv = 0.0f;
		for( v=0; v<=m_iVTessellations; v++, idx+=2 )
		{
			if( vKnot != m_TessVKnot[v] )
			{
				vKnot = m_TessVKnot[v];
				for( k=0; k<=m_iVDegree; k++)
				{
					iTxOffset = (uKnot - m_iUDegree) * m_iVControlPoints + (vKnot - m_iVDegree);
					UTemp[k].x = m_UBasis[uidx] * pTxPoints[ iTxOffset + k ].x;
					UTemp[k].y = m_UBasis[uidx] * pTxPoints[ iTxOffset + k ].y;
					UTemp[k].z = m_UBasis[uidx] * pTxPoints[ iTxOffset + k ].z;
					UTemp[k].w = m_UBasis[uidx] * pTxPoints[ iTxOffset + k ].w;
					dUTemp[k].x = m_dUBasis[uidx] * pTxPoints[ iTxOffset + k ].x;
					dUTemp[k].y = m_dUBasis[uidx] * pTxPoints[ iTxOffset + k ].y;
					dUTemp[k].z = m_dUBasis[uidx] * pTxPoints[ iTxOffset + k ].z;
					dUTemp[k].w = m_dUBasis[uidx] * pTxPoints[ iTxOffset + k ].w;

					for( l=1; l<=m_iUDegree; l++ )
					{
						iTxOffset += m_iVControlPoints;
						UTemp[k].x += m_UBasis[uidx+l * VECTOR_SIZE] * pTxPoints[ iTxOffset + k].x;
						UTemp[k].y += m_UBasis[uidx+l * VECTOR_SIZE] * pTxPoints[ iTxOffset + k].y;
						UTemp[k].z += m_UBasis[uidx+l * VECTOR_SIZE] * pTxPoints[ iTxOffset + k].z;
						UTemp[k].w += m_UBasis[uidx+l * VECTOR_SIZE] * pTxPoints[ iTxOffset + k].w;
						dUTemp[k].x += m_dUBasis[uidx+l * VECTOR_SIZE] * pTxPoints[ iTxOffset + k ].x;
						dUTemp[k].y += m_dUBasis[uidx+l * VECTOR_SIZE] * pTxPoints[ iTxOffset + k ].y;
						dUTemp[k].z += m_dUBasis[uidx+l * VECTOR_SIZE] * pTxPoints[ iTxOffset + k ].z;
						dUTemp[k].w += m_dUBasis[uidx+l * VECTOR_SIZE] * pTxPoints[ iTxOffset + k ].w;
					}
				}
			}

			VBasis = m_VBasis[ (v * m_iVOrder)*VECTOR_SIZE ];
			dVBasis = m_dVBasis[ (v * m_iVOrder)*VECTOR_SIZE ];
			Pw.x = VBasis * UTemp[0].x;
			Pw.y = VBasis * UTemp[0].y;
			Pw.z = VBasis * UTemp[0].z;
			Pw.w = VBasis * UTemp[0].w;
			TanU.x = VBasis * dUTemp[0].x;
			TanU.y = VBasis * dUTemp[0].y;
			TanU.z = VBasis * dUTemp[0].z;
			TanU.w = VBasis * dUTemp[0].w;
			TanV.x = dVBasis * UTemp[0].x;
			TanV.y = dVBasis * UTemp[0].y;
			TanV.z = dVBasis * UTemp[0].z;
			TanV.w = dVBasis * UTemp[0].w;
			for( k=1; k<=m_iVDegree; k++ )
			{
				VBasis = m_VBasis[ (v * m_iVOrder + k)*VECTOR_SIZE ];
				dVBasis = m_dVBasis[ (v * m_iVOrder + k)*VECTOR_SIZE ];
				Pw.x += VBasis * UTemp[k].x;
				Pw.y += VBasis * UTemp[k].y;
				Pw.z += VBasis * UTemp[k].z;
				Pw.w += VBasis * UTemp[k].w;
				TanU.x += VBasis * dUTemp[k].x;
				TanU.y += VBasis * dUTemp[k].y;
				TanU.z += VBasis * dUTemp[k].z;
				TanU.w += VBasis * dUTemp[k].w;
				TanV.x += dVBasis * UTemp[k].x;
				TanV.y += dVBasis * UTemp[k].y;
				TanV.z += dVBasis * UTemp[k].z;
				TanV.w += dVBasis * UTemp[k].w;
			}

			rhw = 1.0f / Pw.w;
			Pw.x = Pw.x * rhw;
			Pw.y = Pw.y * rhw;
			Pw.z = Pw.z * rhw;


			//
			// Convert the Tangents from Homogenous to Euclidean space
			//
			
			TanU.x  = (TanU.x - TanU.w * Pw.x) * rhw;
			TanU.y  = (TanU.y - TanU.w * Pw.y) * rhw;
			TanU.z  = (TanU.z - TanU.w * Pw.z) * rhw;
			TanV.x  = (TanV.x - TanV.w * Pw.x) * rhw;
			TanV.y  = (TanV.y - TanV.w * Pw.y) * rhw;
			TanV.z  = (TanV.z - TanV.w * Pw.z) * rhw;
			
			//
			// Determine the normal from the cross product of the two tangents
			//
			//Norm.x = (TanV.y * TanU.z - TanU.y * TanV.z);
			//Norm.y = (TanU.x * TanV.z - TanV.x * TanU.z);
			//Norm.z = (TanV.x * TanU.y - TanU.x * TanV.y);

			Norm.x = (TanU.y * TanV.z - TanU.z * TanV.y);
			Norm.y = (TanU.z * TanV.x - TanU.x * TanV.z);
			Norm.z = (TanU.x * TanV.y - TanU.y * TanV.x);
	

			//
			// Normalize the normal
			//
			Len = 1.0f / (float)sqrt( Norm.x * Norm.x + Norm.y * Norm.y + Norm.z * Norm.z );

			Norm.x *= Len;
			Norm.y *= Len;
			Norm.z *= Len;

			pVertexPos[v].x = Pw.x;
			pVertexPos[v].y = Pw.y;
			pVertexPos[v].z = Pw.z;

			rhw = 1.0f / (Pw.z * Proj23);
			Pw.x = Proj00 * Pw.x * rhw + tx;// fHalfX;
			Pw.y = Proj11 * Pw.y * rhw + ty;//fHalfY;
			Pw.z = (Pw.z * Proj22 + Proj32 ) * rhw;			

			m_pScreenVertices[ idx ].sx = Pw.x;
			m_pScreenVertices[ idx ].sy = Pw.y;
			m_pScreenVertices[ idx ].sz = Pw.z;
			m_pScreenVertices[ idx ].rhw = rhw;
			m_pScreenVertices[ idx ].tu = Fu;
			m_pScreenVertices[ idx ].tv = Fv;

			m_pNormals[ v ].x = Norm.x;
			m_pNormals[ v ].y = Norm.y;
			m_pNormals[ v ].z = Norm.z;

			Fv += Fvinc;
		}
		Fu += Fuinc;

		g_CurrentUKnot = uKnot;

		//xxxxxxxxxxxx LightVertices( pd3dDevice, pd3dSurfMaterial, iSet );
		LightVertices(state,m_iVTessellations+1,pVertexPos,m_pNormals, m_pScreenVertices+iSet,2);

		if( u > 0 )
		{
			if (triStrip)
			state.lpD3DDevice2->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, D3DVT_TLVERTEX,
							  m_pScreenVertices, iVertices, // number of vertices 
							  m_iVertexIndices[iSet], iVertices, // number of indices
							  clipFlags );
			else state.lpD3DDevice2->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DVT_TLVERTEX,
							  m_pScreenVertices, iVertices, // number of vertices 
							  m_iVertexIndices[iSet],3* 2 * (m_iVTessellations ),
							  // number of indices
							  clipFlags );
		}
		iSet = 1 - iSet;
	}

	return DD_OK;
}


// approximation for shininess
#define SPOW(dot,shin) (dot / (shin-shin*dot+dot))
// pow(dot,shin)


int GNurbsShell::LightVertices(
					  RenderState &state,
					  int nv, // number of vertices
					  const HPoint *v, const Point *vn,
					  D3DTLVERTEX * vtl,
					  int vtlstep // VTL BUFFER OFFSET
					  )
{
	
	int i;
	D3DTLVERTEX *vtlp= vtl;

	gbool culling = state.backfaceCulling; // backface culling enabled ?
	gbool singlsidelighting = culling;

	Point viewpoint(0,0,0);

	gbool doVp = state.textureDefined;
	gbool doVn =  !state.Unlit();
	gbool doVc = gfalse;

	if (doVp) { // transform texture coordinates

		// recreate texture coordinates	
		textureMatrixVersion = state.GetTextureMatrixVersion();

			
			if (!state.textureMatrixIdentity) {
				vtlp= vtl;

				Matrix &M = state.textureMatrix;
				for(i = 0;  i < nv; i++) {
					float x =vtlp->tu;
					float y =vtlp->tv;


					vtlp->tu = x * M.x[0][0] + y * M.x[1][0] /* + p.z * M.x[2][0] */ + M.x[3][0];
					vtlp->tv=  x * M.x[0][1] + y * M.x[1][1] /* + p.z * M.x[2][1] */+ M.x[3][1];
					vtlp += vtlstep;
				}
			} 

	}
	if (!doVn) { // no lighting

			vtlp=vtl;
			

/*
			float a=state.alpha;

			if (vc.Length()>0) { 
				for(i = 0;  i < nv; i++,vtlp++) {
					const Point &c = vc[i]; // we have vertex colors 
					vtlp->color = D3DRGBA_X(c.x,c.y,c.z,a);
					vtlp->specular = 0; // vtlp->color; 
				}
			} else  
*/
			{
				D3DCOLOR c= state.currentColorD3D;
				for(i = 0;  i < nv; i++) {
					vtlp->color = c; 
					vtlp->specular = 0; 
					vtlp += vtlstep;
				}
			}
			this->currentColorD3D = state.currentColorD3D;
			return 0;
	} // !doVn


		// vertex normal 
		// light vertices

	    gbool flipNormals =  state.frontFaceCW; 
		G3Material &mat = state.currentMaterial->material;
		RGBAColor start(mat.emissive); // start with emissive color

		// do we need specular lighting ?
		gbool doSpecular = (mat.specular.r>0) || (mat.specular.g>0) || (mat.specular.b>0);

		vtlp=vtl;

		for ( i=0; i<nv;i++, vtlp+=vtlstep) { // for all vertices, light
		
			// get vertex in view space
			Point p(v[i].x,v[i].y,v[i].z);

			// get normal in view space
			Point n= vn[i];
			
			if (flipNormals) { n = -n; }  // FLIP

			G3LightHandle *l=state.lights;	// first light pointer

			// emissive 
			
			RGBAColor c(start) ; // resulting color

			RGBAColor cspec(0,0,0,start.a); 

			// get material diffuse color, 3 floats
			const float* diffuse;

			if (doVc) diffuse = &vc[i].x; // we have vertex colors 
			else diffuse = &mat.diffuse.r;

			Point toViewer; // points from point to viewer 

			//doVertexCull = TRUE;
			// if (doSpecular) {
				toViewer = viewpoint - p; // vector to viewer in local space
				toViewer.Normalize();
			//}

			if (!singlsidelighting && (DotProd(toViewer,n) < 0.0f)) {
					n = -n; // flip normal for two sided lighting
			}

			// Light : http://www.vrml.org/Specifications/VRML97/part1/concepts.html			
			// if (DotProd(toViewer,n) >= 0)  // Idea : can cull lighting, if pointing away from viewer
			{ 

			// vcull[i]=1; // reuse info for vertex culling

			while ( l != NULL ) {	// for all lights 

			// diffuse part 	
			RGBAColor c1;
			float f;


			if (l->enabled)

			switch(l->dltType) { 					
			case  D3DLIGHT_DIRECTIONAL : 
				{


				f =	n.x * l->directionView.x + n.y * l->directionView.y + n.z * l->directionView.z ;

				f= -f; // enabled in standard version !

				if(f>0.0f)  { // add diffuse part
					c1.r=diffuse[0] * f, c1.g=diffuse[1] * f, c1.b=diffuse[2] * f;
				}
				else c1.r=c1.g=c1.b = 0;



				// specular part 
	
				if (doSpecular) {
					Point tmp(-l->directionView.x + toViewer.x,
							-l->directionView.y + toViewer.y,
							-l->directionView.z + toViewer.z);
					tmp.Normalize();

					f = n.x * tmp.x + n.y * tmp.y + n.z * tmp.z; // dotprod

					if (f > 0.0f)  { // there is a specular contribution

						f = SPOW ( f , mat.power);
				
						// if(f<0.0f) 	f=0.0f;	else 
						if (f>1.0) {
							f=1.0f;
						}
				
						cspec.r += l->kSpecular.r*f,cspec.g += l->kSpecular.g*f,cspec.b += l->kSpecular.b*f;

					}

				}

				// multiply by light color + intensity 
				c.r+=c1.r * l->dcvColor.r,c.g+=c1.g * l->dcvColor.g,c.b+=c1.b * l->dcvColor.b;
				if (l->flags & GL_AMBIENT) { // ambient (Extra to D3D, req. by VRML)
					c.r +=  l->kAmbient.r,	 c.g += l->kAmbient.g, c.b += l->kAmbient.b;
				}

			} 
			break; // case  D3DLIGHT_DIRECTIONAL :

			case  D3DLIGHT_POINT : 
			case  D3DLIGHT_SPOT : 
			case  D3DLIGHT_PARALLELPOINT : // actually not used
			{

				Point toLight(l->positionView.x - p.x,
							l->positionView.y - p.y,
							l->positionView.z - p.z);
					
				float dl=toLight.Length();
				
				if ((l->flags & GL_RANGE) && (dl >l->dvRange)) { // to do: range in  local  coordinates
					break;
				}

				toLight *= 1.0f /dl; // Normalize();

				float scale=1.0f; // attenuation and spotlight scale 

				if ((l->flags & GL_ATTENUATION)) {
				
					// attenuation
					scale = l->dvAttenuation0+dl*(l->dvAttenuation1 + l->dvAttenuation2*dl);

					if (scale>=1.0f) scale = 1.0f;
					else if (scale == 0.0f) break;
					else scale = 1.0/f;
				}



				// spot here
				if (l->dltType == D3DLIGHT_SPOT) { 
					float tmp =	-1.0f * (toLight.x * l->directionView.x + toLight.y * l->directionView.y + toLight.z * l->directionView.z);

#if 1
					float spota = tmp;	
					if ((spota<=0.0f) || (spota < l->cosPhi)) {
						scale=0.0f; // outside
						break;
					} else
					if (spota >= l->cosTheta) {
						// scale*=1.0f;
					}
					else  // check this 
						scale *= ((spota - l->cosPhi ) / (l->cosTheta - l->cosPhi)); // * l->thetaScale

#else
					if (tmp <=0.0f) tmp = 0.0f;
					float spota = acos(tmp);	
					if (spota >= l->dvPhi)
						scale =0.0f;
					else
					if (spota <= l->dvTheta) {
						// f*=1.0f;
					}
					else 
						scale *= ((spota - l->dvPhi ) / (l->dvTheta - l->dvPhi));
#endif

				}

				if (scale >0.0f) {

				// diffuse 

				f =	n.x * toLight.x + n.y * toLight.y + n.z * toLight.z ;

				if(f>0.0f)  {
					f*=scale;
					c1.r=diffuse[0] * f, c1.g=diffuse[1] * f, c1.b=diffuse[2] * f;
				}
				else c1.r=c1.g=c1.b = 0;


				// specular part 
				if (doSpecular) {
					Point tmp(toLight.x + toViewer.x,
							toLight.y + toViewer.y,
							toLight.z + toViewer.z);
					tmp.Normalize();

					f = n.x * tmp.x + n.y * tmp.y + n.z * tmp.z; // dotprod

					if (f > 0.0f)  {

						f = SPOW ( f , mat.power);
				
						if(f<0.0f) 
							f=0.0f;
						else if (f>1.0) {
							f=1.0f;
						}
						f *= scale;
						cspec.r += l->kSpecular.r*f,cspec.g += l->kSpecular.g*f,cspec.b += l->kSpecular.b*f;

					}

				}


				// multiply by light color + intensity 
				c.r+=c1.r * l->dcvColor.r,c.g+=c1.g * l->dcvColor.g,c.b+=c1.b * l->dcvColor.b;

				if (l->flags & GL_AMBIENT) { // ambient (Extra to D3D, req. by VRML)
					c.r +=  scale * l->kAmbient.r,	 c.g += scale * l->kAmbient.g, c.b += scale * l->kAmbient.b;
				}
				
				} // scale > 0

				break;
			}

			} // switch(l->dltType)


			l = l->next;
			}
			
			}
			// clamp or scale by max 
			if (c.r>1.0f) c.r=1.0f;
			if (c.g>1.0f) c.g=1.0f;
			if (c.b>1.0f) c.b=1.0f;

			// fog
			/* to do: Vertex Fog 
				float dV = dist to viewer

				f = 0.0;
				// lin fog	
				if (dV < fogVisibility)
					f= (fogVisibility-dV) / fogVisibility
				
				 // exp fog
				 f = exp(-dV / (fogVisibility-dV ) )
				
                c*=f;
				c += fogCOlor * (1.0-f);

			*/

			// D3DRGBA is verz expensive because of float to int conversion
			vtlp->color = D3DRGBA_X(c.r,c.g,c.b,c.a); // this involves a * 255 mul ==> Katami
		
			if (doSpecular) {
				// clamp or scale by max 
				if (cspec.r>1.0f) cspec.r=1.0f;
				if (cspec.g>1.0f) cspec.g=1.0f;
				if (cspec.b>1.0f) cspec.b=1.0f;

				vtlp->specular = D3DRGBA_X(cspec.r,cspec.g,cspec.b,cspec.a);	

			} else vtlp->specular = 0;
		} // for all vertices

		return 1;
}



#endif _D3D

#ifdef gems



#endif gems


//##########################################################################################
//##########################################################################################
//
//										GTrimmedNurbsShell
//
//##########################################################################################
//##########################################################################################


RTIMP(GTrimmedNurbsShell,0,GShell);

#ifdef _LOG_TRIMMED_

static ofstream logx; 

#endif


GTrimmedNurbsShell::GTrimmedNurbsShell() : 
	surface(NULL)
{
	m_ScaleFactor = 1.0f;
  m_TessTol = 0.0025f;
  m_DomainTessTol = 0.0001f;
  m_MaxSubDivisions = 7;

#ifdef _LOG_TRIMMED_
	logx.open("c:\\cc3d\\trimnurb.log");	
//	streambuf *old_buf = cout.rdbuf();
#endif

		changed = gtrue;
}

GTrimmedNurbsShell::~GTrimmedNurbsShell() 
{
	SetSurface(NULL);
	DeleteTrimmingCurves();
}

// ! set the base surface
gbool GTrimmedNurbsShell::SetSurface(GNurbsShell *newSurface)
{
	if (surface) surface->unref();
	surface = newSurface;
	if (surface) surface->ref();
	SetChanged();
	return surface != NULL;

}



/* ----------------------------------------------------------------------
  functions for adding trimming contours to surface
----------------------------------------------------------------------- */
gbool GTrimmedNurbsShell::BeginTrim(void)
{
#if defined(G_T_NURBS)
	contour = new ListNode< trim_region >;
#endif

	return gtrue;
}

/* ----------------------------------------------------------------------
----------------------------------------------------------------------- */
//! add a trimming curve
gbool GTrimmedNurbsShell::AddNurbsTrimmingCurve2D(
					   int order, 
					   int numControlPoint,const float *controlPoint, 
					   int numWeight,const float* weight, 
					   int numKnot, const float* knot, 
					   int tessellations )

{
#if defined(G_T_NURBS)

	int i;

	  elnode = new ListNode<trim_element>;

	  elnode->el.curv.p = order-1;
	  elnode->el.curv.knt.m = numKnot-1;
	  elnode->el.curv.knt.U.reserve_pool(numKnot);
	  for( i = 0; i < numKnot; i++ ) elnode->el.curv.knt.U[i] = *knot++;

	  elnode->el.curv.cpt.n = numControlPoint-1;
	  elnode->el.curv.cpt.Pw.reserve_pool(numControlPoint);
	  for( i = 0; i < numControlPoint; i++ ) {
			elnode->el.curv.cpt.Pw[i].x = *controlPoint++;
			elnode->el.curv.cpt.Pw[i].y = *controlPoint++;
			elnode->el.curv.cpt.Pw[i].w = *weight++;
	  }

	contour->el.AppendRight(elnode); // append it to the end of the list !!!
#endif

	return gtrue;
}

/* ----------------------------------------------------------------------
----------------------------------------------------------------------- */
gbool GTrimmedNurbsShell::AddPolyline2D(int numOfPoints, const float* points)
{

#if defined(G_T_NURBS)

	elnode = new ListNode<trim_element>;
//	elnode->el.poly.nElems	= numOfPoints -1;
	elnode->el.poly.reserve_pool(numOfPoints);
	for (int i=0;i<numOfPoints;i++)
	{
		elnode->el.poly[i].x = *points++;
		elnode->el.poly[i].y = *points++;
	}

	contour->el.AppendRight(elnode); // append it to the end of the list !!!
#endif

	return gtrue;
}


/* ----------------------------------------------------------------------
----------------------------------------------------------------------- */
gbool GTrimmedNurbsShell::EndTrim(void)
{


	SetTrimmingChanged();

#if defined(G_T_NURBS)	
	//add the current segment to the contour
	mContours2D.Append(contour);
#endif

	return gtrue;
}


/* ----------------------------------------------------------------------
----------------------------------------------------------------------- */
// ! delete all trimming curves
gbool GTrimmedNurbsShell::DeleteTrimmingCurves()
{
#if defined(G_T_NURBS)	

	ListNode<trim_region> *cont, *cont_next;

	cont = mContours2D.head;// lists doesn't automatically delete their elements
                          // in their destructor, so some manual work is 
                          // necessary when a list of lists shall be destroyed
	while( cont != 0 )
	{
		cont->el.DeleteElems(); // delete trim elements of this region

		cont_next = cont->next;
		delete cont;            // delete list node
		cont = cont_next;
	}
	mContours2D.ReleaseElems();// this just sets head = nElems = 0, which is ok,
                             // since its elements already were destroyed above

	SetTrimmingChanged();
#endif

	return gtrue;
}


/* ----------------------------------------------------------------------
----------------------------------------------------------------------- */
void GTrimmedNurbsShell::ComputeBBox(BBox &bbox)
{
	if (!surface) return;

	// delegate for now
	surface->ComputeBBox(bbox);

}

/* ----------------------------------------------------------------------
----------------------------------------------------------------------- */
int GTrimmedNurbsShell::Do(GBBoxTraversal &state)       
{ 
	if (!surface) return 0;

	// delegate for now
	return surface->Do(state);
}


/* ----------------------------------------------------------------------
----------------------------------------------------------------------- */
int GTrimmedNurbsShell::Do(GglRenderTraversal &state)       
{ 
	if (!surface) return 0;

//tbd:	ComputeTessellationValues();

	// update ?
  if (changed || tessellationChanged)
  {
    GMatrixStack *mstack = state.GetCurrentMatrix();
    const Matrix &m = mstack->GetMatrix();
    m_ScaleFactor = m.ScaleFactor();
	
    ComputeShell();
  }
	// render !
	return GShell::Do(state);

}


/* ----------------------------------------------------------------------
----------------------------------------------------------------------- */
int GTrimmedNurbsShell::Do(GRaySelectTraversal &state)       
{ 
	if (!surface) return 0;

	// delegate for now
	// return surface->Do(state);

	return GShell::Do(state);
}

/* ----------------------------------------------------------------------
----------------------------------------------------------------------- */
int GTrimmedNurbsShell::SetRenderQuality(float budget)
{
	if (m_fRenderQuality != budget)
	{
		tessellationChanged = gtrue;
		m_fRenderQuality = budget;

	}



	ASSERT(m_fRenderQuality >= 0.f);
	return gtrue;
}



/*--------------------------------------------------------------------------------


								GUL


-----------------------------------------------------------------------------------*/

#if defined(G_T_NURBS)

#include <gul/common/gul_types.h>
#ifdef  _LOG_TRIMMED_
	#include <gul/common/gul_io.h>
#endif
#include <gul/common/gul_vector.h>
#include <gul/common/guar_exact.h>
#include <gul/common/gugr_basics.h>
#include <gul/common/gugr_io.h>
#include <gul/common/gugr_split.h>
#include <gul/common/guge_normalize.h>
#include <gul/common/gunu_basics.h>
#include <gul/common/gunu_divide.h>
#include <gul/common/guge_linear.h>
#include <gul/common/gunu_intersect.h>
#include <gul/common/gunu_linearize.h>
#include <gul/common/gunu_tesselate.h>

using gul::Ptr;
using gul::hpoint;
using gul::hpoint2;
using gul::point;
using gul::point2;
using gunu::TessInfo;
using gunu::LinearizeSurface;
using gugr::GraphInfo;


/* -----------------------------------------------------------
  callback function for undivided quads
-------------------------------------------------------------*/

struct quad_info   // for temporarily storing the data for a quad
{
  point<float> p[4];
  point<float> n[4];
  float u[2];
  float v[2];
};

struct tri_info   // for temporarily storing the data for a triangle
{
  point<float>  p[3];
  point<float>  n[3];
  point2<float> d[3];
};


struct cb_data  // data structure passed to callbacks
{
  List< ListNode<quad_info> > *QL;
  List< ListNode<tri_info> >  *TL;
};




/* ----------------------------------------------------------------------
----------------------------------------------------------------------- */
void  quadfunc( void *data, 
             gul::point<float> *p11, gul::point<float> *p12, 
             gul::point<float> *p21, gul::point<float> *p22,
             gul::point<float> *n11, gul::point<float> *n12, 
             gul::point<float> *n21, gul::point<float> *n22,
             float *u1, float *u2, float *v1, float *v2)
{
  cb_data *cbdata = (cb_data *)data;
  List< ListNode<quad_info> > *L = cbdata->QL;

  ListNode<quad_info> *node = new ListNode<quad_info>;
  node->el.p[0] = *p11;
  node->el.p[1] = *p12;
  node->el.p[2] = *p21;
  node->el.p[3] = *p22;
  node->el.n[0] = *n11;
  node->el.n[1] = *n12;
  node->el.n[2] = *n21;
  node->el.n[3] = *n22;
  node->el.u[0] = *u1;
  node->el.u[1] = *u2;
  node->el.v[0] = *v1;
  node->el.v[1] = *v2;

  L->Append(node);

 
#ifdef _LOG_TRIMMED_
 
  // dump for debugging
  cout << "cb: quad(" << *p11 << "), (" <<  *p12 << "), (" << *p21 << "), (" << *p22 << ")\n";
#endif
}

/* ----------------------------------------------------------------------
  callback function for triangles (called when quads were triangulated) 
----------------------------------------------------------------------- */
void  trifunc( void *data, 
             gul::point<float> *p1, gul::point<float> *p2, 
             gul::point<float> *p3,
             gul::point<float> *n1, gul::point<float> *n2, 
             gul::point<float> *n3,
             gul::point2<float> *d1, gul::point2<float> *d2, 
             gul::point2<float> *d3 )
{
  cb_data *cbdata = (cb_data *)data;
  List< ListNode<tri_info> > *L = cbdata->TL;

  ListNode<tri_info> *node = new ListNode<tri_info>;
  node->el.p[0] = *p1;
  node->el.p[1] = *p2;
  node->el.p[2] = *p3;
  node->el.n[0] = *n1;
  node->el.n[1] = *n2;
  node->el.n[2] = *n3;
  node->el.d[0] = *d1;
  node->el.d[1] = *d2;
  node->el.d[2] = *d3;

  L->Append(node);


#ifdef _LOG_TRIMMED_

  // dump for debugging
  cout << "cb: tri(" << *p1 << "), (" <<  *p2 << "), (" << *p3 << ")\n";
#endif
}




/* ----------------------------------------------------------------------
----------------------------------------------------------------------- */
int GTrimmedNurbsShell::ComputeTessellationValues(int mode)
{
/*	switch (mode) {
	case GNURBS_STATIC_TESSELATION:
	default :
		scale = m_fRenderQuality/m_fInitialRenderQuality;
		break;
	case GNURBS_FRAMERATE_TESSELATION:
		scale = m_fRenderQuality/m_fInitialRenderQuality;
		break;
	case GNURBS_DYNAMIC_TESSELATION:
		scale = sqrt((m_fInitialRenderQuality*m_fBBSurface)/(float)m_iInitialUTess/(float)m_iInitialVTess/2.f);
		break;
	case GNURBS_DYNAMIC_FRAMERATE_TESSELATION:
		//m_fRenderQuality is scaled by the budget
		scale = sqrt((m_fRenderQuality*m_fBBSurface)/(float)m_iInitialUTess/(float)m_iInitialVTess/2.f);
		break;

	} 
*/

return 1;



}




/* ----------------------------------------------------------------------
----------------------------------------------------------------------- */
int GTrimmedNurbsShell::ComputeShell()
{
	try{
	int S_nu,S_nv,S_pu,S_pv,i,j,nRegions;
	Ptr< float > SU,SV;
	Ptr< Ptr< hpoint<float> > > SPw;
	float u, v, minu, maxu, minv, maxv;
	
	gul::rtl<float> dom_zerotol(1.0e-7,1.0e-7,1.0e-7,1.0e-7); 
	
  float dom_tol = m_DomainTessTol / m_ScaleFactor / m_fRenderQuality;
  if( dom_tol > 0.5f ) dom_tol = 0.5f;
  else if( dom_tol < 1.0e-6 ) dom_tol = 1.0e-6;
  TRACE("TrimmedNurbsShell::ComputeShell(): dom_tol = %f\n", dom_tol);


	List< ListNode<quad_info> > QL;
	ListNode<quad_info> *qnode,*qnode_next;
	List< ListNode<tri_info> > TL;
	ListNode<tri_info> *tnode,*tnode_next;
	cb_data cbdata;
	cbdata.QL = &QL; cbdata.TL = &TL;
	
	int numberOfContours, nConts, k, count;
	ListNode<trim_region> *contour2D;
	ListNode<trim_element> *seg; 
	Ptr< Ptr< point2<float> > > cl, contour;
	
	bool normalizedU, normalizedV;
	float offsetU, offsetV, scaleU, scaleV, x,y,w; 
	Ptr< hpoint2<float> > domPw;
	
	bool bresult, is_clampedU, is_normalizedU, is_clampedV, is_normalizedV;
	bool is_clamped, is_normalized;
	
	int surf_nu, surf_nv, curv_n, dummy;
	Ptr< float > surfU, surfV, curvU;
	Ptr< Ptr< hpoint<float> > > surfPw;
	Ptr< hpoint2<float> > curvPw;
	float dummyf;
	
	
#ifdef _LOG_TRIMMED_
	
	streambuf *old_buf = cout.rdbuf();
	cout = logx.rdbuf();     // redirect cout to log file 
#endif

	// get NURBS Surface data
	S_pv =  surface->m_iVDegree;
	S_pu =  surface->m_iUDegree;
	S_nv =  surface->m_iVControlPoints - 1;
	S_nu =  surface->m_iUControlPoints - 1;
	
	// share pointers
	SV.use_pointer( surface->m_VKnots.Data(), S_nv+S_pv+2 );
	SU.use_pointer( surface->m_UKnots.Data(), S_nu+S_pu+2 );
	
	hpoint<float> *CtrlPoints = (hpoint<float> *)GetHPoint(surface->m_ControlPoints);
	
	SPw.reserve_pool( S_nv+1 );
	for( i = 0; i <= S_nv; i++ )
		SPw[i].reserve_pool( S_nu+1 );
	
	for( i = 0; i <= S_nv; i++ )
		for( j = 0; j <= S_nu; j++ )
			SPw[i][j] = CtrlPoints[j*(S_nv+1)+i];    
  
#ifdef _LOG_TRIMMED_
		// ------------------ dump ----------------------------------
    dump_to_file();		

		cout << "input NURBS SURFACE\n";
		cout << "(\n";
		
		cout << "/* Knot Vector U (degree =" << S_pu << ") */\n";
		cout << "(" << SU[0];
		for( i = 1; i < S_nu+S_pu+2; i++ )
			cout << ", " << SU[i];
		cout << "),\n";
		
		cout << "/* Knot Vector V (degree =" << S_pv << ") */\n";
		cout << "(" << SV[0];
		for( i = 1; i < S_nv+S_pv+2; i++ )
			cout << ", " << SV[i];
		cout << "),\n";
		
		cout << "/* Control Points */\n";
		cout << "(\n";
		for( i = 0; i < S_nv+1; i++ )
		{
			cout << "( ";
			
			cout << "(" << SPw[i][0].x << ", " << SPw[i][0].y << ", " 
				<< SPw[i][0].z << ", " << SPw[i][0].w << ")";
			for( j = 1; j < S_nu+1; j++ )
			{
				cout << ", (" << SPw[i][j].x << ", " << SPw[i][j].y << ", " 
					<< SPw[i][j].z << ", " << SPw[i][j].w << ")";
			} 
			
			cout << " ),\n";
		}
		cout << ")\n";
		
		cout << ");\n";
#endif

		//
		// clamp and normalize knot vectors of surface
		//
		
		bresult = gunu::ValidateKnotVec( 
			S_nu, S_pu, SU, is_clampedU, is_normalizedU );
		if( !bresult ) 
			{
				#ifdef _LOG_TRIMMED_
					cout << "\nERROR: invalid uknot\n";
				#endif 
				TRACE("gul: invalid uknot");
				return 0;   // or -1 ???
			}

		bresult = gunu::ValidateKnotVec( 
			S_nv, S_pv, SV, is_clampedV, is_normalizedV ); 
		if( !bresult )
			{
				#ifdef _LOG_TRIMMED_
					cout << "\nERROR: invalid vknot\n";
				#endif 				
				TRACE("gul: invalid vknot");
				return 0;   // or -1 ???
			}

		if( !is_clampedU || !is_clampedV )
		{
			if( (SU[S_pu] != 0.0f) || (SU[S_nu+1] != 1.0f) )
			{
				offsetU = SU[S_pu];
				scaleU = SU[S_nu+1] - SU[S_pu];
				normalizedU = true;
			}
			else
				normalizedU = false;
			
			if( (SV[S_pv] != 0.0f) || (SV[S_nv+1] != 1.0f) )
			{
				offsetV = SV[S_pv];
				scaleV = SV[S_nv+1] - SV[S_pv];
				normalizedV = true;
			}
			else
				normalizedV = false;
			
			gunu::ExtractSurface<float,hpoint<float> >( 
				SU[S_pu], SU[S_nu+1], SV[S_pv], SV[S_nv+1], 
				S_nu, S_pu, SU, S_nv, S_pv, SV, SPw, 
				&surf_nu, &surfU, &surf_nv, &surfV, &surfPw );
		}
		else if( !is_normalizedU || !is_normalizedV )
		{
			surf_nu = S_nu;
			surf_nv = S_nv;
			surfPw = SPw; 
			
			if( !is_normalizedU )
			{
				normalizedU = true;      
				
				surfU.reserve_pool( SU.nElems() );
				gunu::NormalizeKnotVector( S_nu, S_pu, SU, offsetU, scaleU, surfU );
			}
			else
			{
				surfU = SU;
				normalizedU = false;
			}
			
			if( !is_normalizedV )
			{
				normalizedV = true;
				
				surfV.reserve_pool( SV.nElems() );
				gunu::NormalizeKnotVector( S_nv, S_pv, SV, offsetV, scaleV, surfV );
			}
			else
			{
				surfV = SV;
				normalizedV = false;
			}
		}
		else
		{
			surf_nu = S_nu;
			surfU = SU;
			surf_nv = S_nv;
			surfV = SV;
			surfPw = SPw; 
			normalizedU = normalizedV = false;
		}
		

	//
	// tesselate trim-contours
	//
		
  numberOfContours = mContours2D.nElems; // number of regions
  if( numberOfContours == 0 ) 
  {
    #ifdef _LOG_TRIMMED_
      cout << "\nERROR: no contours specified\n";
    #endif 
    TRACE("no contours specified\n");
    return 0; // must have at least one region
  }
		
  contour2D = mContours2D.head;      // first region
		
  contour.reserve_pool(numberOfContours);  // create one closed polyline for each region

  nRegions = 0;   // initialize counter for non-vanishing regions 
		
  //
  // tesselate all contours
  //
  for( i = 0; i < numberOfContours; i++ )    
  {
    nConts = contour2D->el.nElems;  /* number of contours of the current region*/
    seg = contour2D->el.head;       /* first trim element */
			
    cl.reserve_pool(nConts);        /* temporary array for linearized contours */
    ASSERT(nConts);

    //
    //tessellate all segments
    //
    for( j = 0, count = 0; j < nConts; j++ )
    {				
      //
      // segment is a polyline2D
      //	
      if( seg->el.poly.nElems() > 0 )  
      {
        //
        // readjust domain points, if surface knot vectors were normalized
        //
        if( !normalizedU && !normalizedV )
        {
          cl[j] = seg->el.poly;   // just use it as it is
        }
        else  
        {
          cl[j].reserve_pool( seg->el.poly.nElems() );
						
          if( !normalizedU ) // was only normalized in V direction
          {
            for( k = 0; k < seg->el.poly.nElems(); k++ )
            {
              cl[j][k].x = seg->el.poly[k].x;
              cl[j][k].y = (seg->el.poly[k].y - offsetV) / scaleV;
            }
          } else if( !normalizedV ) // was only normalized in U direction
          {
            for( k = 0; k < seg->el.poly.nElems(); k++ )
            {
              cl[j][k].x = (seg->el.poly[k].x - offsetU) / scaleU;
              cl[j][k].y = seg->el.poly[k].y;
            }
          } else                     // was normalized in both directions
          {
            for( k = 0; k < seg->el.poly.nElems(); k++ )
            {
              cl[j][k].x = (seg->el.poly[k].x - offsetU) / scaleU;
              cl[j][k].y = (seg->el.poly[k].y - offsetV) / scaleV;
            }
          }
        }
      }
				
      //
      // segment is a curve
      //
      else
      {

#ifdef _LOG_TRIMMED_
        cout << "\nTRIM CURVE\n";
        cout << "p = " << seg->el.curv.p << ", ";
        cout << "m = " << seg->el.curv.knt.m << ", ";
        cout << "n = " << seg->el.curv.cpt.n << "\nKNOT VECTOR\n";
        for( k = 0; k < seg->el.curv.knt.U.nElems(); k++ )
          cout << seg->el.curv.knt.U[k] << " ";
        cout << "\nCONTROL POINTS\n";
        for( k = 0; k < seg->el.curv.cpt.Pw.nElems(); k++ )
          cout << "(" << seg->el.curv.cpt.Pw[k] << "), ";
        cout << "\n";					
        cout.flush();
#endif
					
        // more convenient notation					

        int C_n = seg->el.curv.cpt.n;
        int C_p = seg->el.curv.p;
        Ptr< float >& CU = seg->el.curv.knt.U;
        Ptr< hpoint2<float> >& CPw = seg->el.curv.cpt.Pw;
					
        //
        // readjust domain control points, if surface knot
        // vectors were normalized
        //					
        if( !normalizedU && !normalizedV )  
        {
          domPw = CPw;
        }
        else               
        {                  
          int dom_nPw = C_n + 1;
						
          domPw.reserve_pool( dom_nPw );
						
          if( !normalizedV )//only u knots has been normalized
          {
            for( k = 0; k < dom_nPw; k++ )
            {
              w = CPw[k].w;
              x = CPw[k].x;
              domPw[k].x = (x - w*offsetU)/scaleU;
              domPw[k].y = CPw[k].y;
              domPw[k].w = w; 
            }
          }
          else if( !normalizedU )
          {
            for( k = 0; k < dom_nPw; k++ )
            {
              w = CPw[k].w;
              y = CPw[k].y;
              domPw[k].x = CPw[k].x;
              domPw[k].y = (y - w*offsetV)/scaleV;
              domPw[k].w = w; 
            }
          }                  
          else
          {
            for( k = 0; k < dom_nPw; k++ )
            {
              w = CPw[k].w;
              x = CPw[k].x;
              y = CPw[k].y;
              domPw[k].x = (x - w*offsetU)/scaleU;
              domPw[k].y = (y - w*offsetV)/scaleV;             
              domPw[k].w = w; 
            }
          }
        }	

        //
        // clamp and normalize knot vector of domain curve
        //						
        bresult = gunu::ValidateKnotVec(
                          C_n, C_p, CU, is_clamped, is_normalized );
        if( !bresult )
        {
          TRACE("gul: invalid knot");
          #ifdef _LOG_TRIMMED_
            cout << "\nERROR: invalid knotvector of curve\n";
          #endif 
          return 0;   // or -1 ???
        }
						
        if( !is_clamped )  // clamping necessary
        {
          gunu::ExtractCurve<float,hpoint2<float> >( 
            CU[C_p], CU[C_n+1], 
            C_n, C_p, CU, domPw, 
            &curv_n, &curvU, &curvPw );
        }
        else
        {
          curv_n = C_n;
          curvPw = domPw;
							
          if( !is_normalized ) // need only to normalize
          {
            curvU.reserve_pool( CU.nElems() );
            gunu::NormalizeKnotVector<float>( 
                    C_n, C_p, CU, dummyf, dummyf, curvU );
          }
          else
            curvU = CU;
        }
					
#ifdef _LOG_TRIMMED_					
        cout << "\nCONTROL POINTS BEFORE LINEARIZATION\n";
        for( k = 0; k < curvPw.nElems(); k++ )
          cout << "(" << curvPw[k] << "), ";
        cout << "\n";
        cout.flush();
#endif
					
        //
        // linearize domain curve
        //
        gunu::LinearizeCurve( 10000, curv_n, C_p, curvU, curvPw, 
                dom_tol, dom_zerotol, &dummy, &cl[j] );
					
        //
        // for linear curves check that subdivision has not created more
        // points than control points
        //
        if( (C_p == 1) && (cl[j].nElems() > curv_n+1) ) 
        {
          cl[j].reserve_pool( curv_n+1 );
          for( k = 0; k <= curv_n; k++ )
          {
            cl[j][k].x = curvPw[k].x / curvPw[k].w;
            cl[j][k].y = curvPw[k].y / curvPw[k].w;
          }
        }
      }
	  
      count += cl[j].nElems()-1;
	  
      seg = seg->next;  // next trim element of current region
    }
		
    //
    // create one continuous polyline for the whole region
    // and check if its bounding box has a minimum size
    //
    if( count > 2 )
    {
      contour[nRegions].reserve_pool(count+1);

      minu = maxu = cl[0][0].x;
      minv = maxv = cl[0][0].y;

      for( j = 0, count = 0; j < nConts; j++ )
      {
        for( k = 0; k < cl[j].nElems()-1; k++ )
        {
          u = cl[j][k].x;
          v = cl[j][k].y;

          if( u < minu ) minu = u;
          else if( u > maxu ) maxu = u;
          if( v < minv ) minv = v;
          else if( v > maxv ) maxv = v;

          contour[nRegions][count+k].x = u;
          contour[nRegions][count+k].y = v;
        }
        count += cl[j].nElems()-1;
      }
      contour[nRegions][count] = cl[nConts-1][k];

      //
      // only use the region, when its diameter is >= domtol 		
      //
      u = maxu - minu;
      v = maxv - minv;

      if( sqrtf(u*u + v*v) >= dom_tol )
        nRegions++;
    }

    contour2D = contour2D->next;    // next trim region
  }
	
	//
	//  tesselate surface
	//
	
#ifdef _LOG_TRIMMED_
	
	/* --- dump input data (for debugging) --- */
	cout << "\nSURFACE\n";
	
	cout << "Knot Vector V\n";
	for( i = 0; i < surf_nv+S_pv+2; i++ )
		cout << surfV[i] << " ";
	cout << "\n";
	
	cout << "Knot Vector U\n";
	for( i = 0; i < surf_nu+S_pu+2; i++ )
		cout << surfU[i] << " ";
	cout << "\n";
	
	cout << "Control Points:\n";
	for( i = 0; i < surf_nv+1; i++ )
	{
		for( j = 0; j < surf_nu+1; j++ )
			cout << "(" << surfPw[i][j] << ") ";
		cout << "\n";
	}
	
	for( i = 0; i < contour.nElems(); i++ )
	{
		cout << "Contour " << i << ":\n";
		cout << "====================\n";
		for( j = 0; j < contour[i].nElems(); j++ )
			cout << "(" << contour[i][j].x << ", " << contour[i][j].y << ")\n";
	} 
	cout.flush();
#endif
	
  // 
  // only use the contours which are big enough
  //
  if( nRegions <= 0 ) return 0;  // surface too small 

	Ptr< Ptr< point2<float> > > novanish_contour;

  if( nRegions != numberOfContours )
  {
    novanish_contour.reserve_pool(nRegions);
    for( i = 0; i < nRegions; i++ )
      novanish_contour[i] = contour[i];
  }
  else
    novanish_contour = contour;

  //
  // calculate tesselation tolerance
  //
  point<float> minP,maxP;

  guge::CalcBoundingBoxVerts( surfPw[0], minP, maxP );
  for( i = 1; i <= surf_nv; i++ )
    guge::UpdateBoundingBoxVerts( surfPw[i], minP, maxP );
  float diam = gul::distance( minP, maxP );
 
 // float tesstol = m_TessTol;
  float tesstol;
  if( tesstol > 0.5f ) tesstol = 0.5f;
  else if( tesstol < 1.0e-6 ) tesstol = 1.0e-6;
 
 // tesstol = tesstol * diam / m_ScaleFactor / m_fRenderQuality;
  tesstol = m_TessTol * diam / m_ScaleFactor / m_fRenderQuality;
  if( tesstol <= 0.0f ) return(0);

  TRACE("Diameter BBox = %f, ScaleFactor = %f => tesstol = %f\n", 
          (double)diam, (double)m_ScaleFactor, (double)tesstol );

  gul::rtl<float> zerotol(1.0e-7*diam/m_ScaleFactor,1.0e-7*diam/m_ScaleFactor,
                          1.0e-7,1.0e-7); 

  //
  //  tesselate the surface
	//
	gunu::LinearizeSurfaceX<float,hpoint<float>,point<float>,point2<float> >( 
		m_MaxSubDivisions, surf_nu, S_pu, surfU, surf_nv, S_pv, surfV, surfPw,
		tesstol, zerotol, novanish_contour, true, quadfunc, trifunc, (void*)&cbdata );
	

	//
	// fill gshell arrays with quads and triangles
	//	
	int nQel = QL.nElems;
	int nTel = TL.nElems;
	int numVerts = 4*nQel + 3*nTel;
	nfaces = 2*nQel + nTel;
	
#ifdef _LOG_TRIMMED_
	cout << "Start to fill gshell: number of vertices " << numVerts << " number of faces " <<  nfaces << "\n";
#endif
	
	this->v.SetLength(numVerts);
	if (this->v.Data() == NULL) return -1; // out of memory
	vn.SetLength(numVerts);
	if (vn.Data() == NULL) return -1; // out of memory
	vp.SetLength(numVerts);
	if (vp.Data() == NULL) return -1; // out of memory
	this->f.SetLength(nfaces*4);
	int *f = this->f.DataRW();
	if (f == NULL) return -1; // out of memory
	
	Point *vptr= this->v.DataRW();
	Point *vpptr = vp.DataRW(); 
	Point *vnptr = vn.DataRW(); 
	
	qnode = QL.head;
	for( i = 0; i < nQel; i++ )  // fill gshell arraya for quads
	{
		for( j = 0; j < 4; j++ )
		{
			vptr->x = qnode->el.p[j].x;
			vptr->y = qnode->el.p[j].y;
			vptr->z = qnode->el.p[j].z;
#ifdef _LOG_TRIMMED_
			cout << "quad (" << j << " (" << vptr->x << ", " <<  vptr->y << ", " << vptr->z << ") ";
			//TRACE("GTrimmedNurbsShell::ComputeShell() (%f %f %f)\n", vptr->x, vptr->y, vptr->z);
#endif
			vptr++;
			
			vnptr->x = qnode->el.n[j].x;
			vnptr->y = qnode->el.n[j].y;
			vnptr->z = qnode->el.n[j].z;
#ifdef _LOG_TRIMMED_
			cout << "normal(" << vnptr->x << ", " <<  vnptr->y << ", " << vnptr->z << ") ";
#endif
			vnptr++;
			
			vpptr->x = qnode->el.u[j&1];
			vpptr->y = qnode->el.v[j&2];
			vpptr->z = 0.f;
#ifdef _LOG_TRIMMED_
			cout << "texture(" << vpptr->x << ", " <<  vpptr->y << ", " << vpptr->z << ")\n";
#endif
			vpptr++;
		}
		*f++ = 3;      // add new faces (triangles)
		*f++ = i*4;
		*f++ = i*4+3;
		*f++ = i*4+2;
		
		*f++ = 3;
		*f++ = i*4;
		*f++ = i*4+1;
		*f++ = i*4+3;
		
		qnode_next = qnode->next;
		delete qnode;
		qnode = qnode_next;
	}
	int fqend = i*4; // index of first face behind quads
	
	tnode = TL.head;
	for( i = 0; i < nTel; i++ )  // fill gshell arraya for triangles
	{
		for( j = 0; j < 3; j++ )
		{
			vptr->x = tnode->el.p[j].x;
			vptr->y = tnode->el.p[j].y;
			vptr->z = tnode->el.p[j].z;
#ifdef _LOG_TRIMMED_
			cout << "tri(" << j << ") ("<< vptr->x << ", " <<  vptr->y << ", " << vptr->z << ")";
#endif
			vptr++;
			
			vnptr->x = tnode->el.n[j].x;
			vnptr->y = tnode->el.n[j].y;
			vnptr->z = tnode->el.n[j].z;
#ifdef _LOG_TRIMMED_
			cout << "normal(" << vnptr->x << ", " <<  vnptr->y << ", " << vnptr->z << ")";
#endif
			vnptr++;
			
			vpptr->x = tnode->el.d[j].y;
			vpptr->y = tnode->el.d[j].x;
			vpptr->z = 0.f;
#ifdef _LOG_TRIMMED_
			cout << "texture(" << vpptr->x << ", " <<  vpptr->y << ", " << vpptr->z << ")\n";
#endif
			vpptr++;
		}
		*f++ = 3;      // add new faces (triangle)
		*f++ = fqend + i*3;
		*f++ = fqend + i*3+1;
		*f++ = fqend + i*3+2;
		
		tnode_next = tnode->next;
		delete tnode;
		tnode = tnode_next;
	}
	
	SetEditFlag(SHELL_VERTICES_CHANGED | SHELL_VERTEX_ATTRIBUTES_CHANGED | SHELL_FACELIST_CHANGED | SHELL_VERTS_LENGTH_CHANGED);
	//	shellVerticesOk=gtrue; // vertices are up to date					
	//	shellFacelistOk = gtrue;
	tessellationChanged = gfalse;
	
	UpdateActions();
	
	SetFlag(SHELL_TRIANGULATED);
	UnSetAction(SHELL_COMPUTE_VNORMALS); // we already set vp 
	//SetAction(SHELL_COMPUTE_VNORMALS); // we already set vp 
	
	changed = gfalse;
	
#ifdef _LOG_TRIMMED_
	cout = old_buf;   // restore original cout streambuf
#endif
}//end try

catch (gul::AllocError* error)
{
	TRACE("error\n");
}
catch (gul::InternalError* error)
{
	TRACE("error\n");
}
catch (gul::IntervalDivZeroError* error)
{
	TRACE("error\n");
}


	return 1;
}


#if defined(_LOG_TRIMMED_) 

#define _DUMP_TRIMMED_

#if !defined(_DUMP_TRIMMED_)

/*****************************************************************

  DUMP THE DATA OF A TRIMMED NURBS SURFACE FOR DEBUGGING

*****************************************************************/

//void GTrimmedNurbsShell::dump_to_file()
//{
//}

#else

static int nsurf_total = 0; 

class Logger : public ofstream
{
  public:
    Logger() { precision(8); }
    ~Logger();
};

Logger::~Logger()
{
  //
  // Create Root of Scene Tree
  //
  (*this) << "/* Scene Tree */\n";
  (*this) << "SCENEROOT = SCENETREE(\n";
  (*this) << "  (\n";
  (*this) << "    node0"; 
  for( int i = 1; i < nsurf_total; i++ )
  {
    (*this) << ",\n    node" << i; 
  }
  (*this) << "\n  )\n";
  (*this) << ");\n";
}

static Logger nlog;


void GTrimmedNurbsShell::dump_to_file()
{
  if( !nlog.is_open() )
    nlog.open( "c:\\cc3d\\nurb_run.log" );

	int S_nu,S_nv,S_pu,S_pv,i,j,nConts;
	Ptr< float > SU,SV;
	Ptr< Ptr< hpoint<float> > > SPw;
	ListNode<trim_region> *contour2D;
	ListNode<trim_element> *seg; 


	// get NURBS Surface data
	S_pv =  surface->m_iVDegree;
	S_pu =  surface->m_iUDegree;
	S_nv =  surface->m_iVControlPoints - 1;
	S_nu =  surface->m_iUControlPoints - 1;
	
	// share pointers
	SV.use_pointer( surface->m_VKnots.Data(), S_nv+S_pv+2 );
	SU.use_pointer( surface->m_UKnots.Data(), S_nu+S_pu+2 );
	
	hpoint<float> *CtrlPoints = (hpoint<float> *)GetHPoint(surface->m_ControlPoints);
	
	SPw.reserve_pool( S_nv+1 );
	for( i = 0; i <= S_nv; i++ )
		SPw[i].reserve_pool( S_nu+1 );
	
	for( i = 0; i <= S_nv; i++ )
		for( j = 0; j <= S_nu; j++ )
			SPw[i][j] = CtrlPoints[j*(S_nv+1)+i];    

/* ----------------------------------------------------
   dump the whole data for debugging
-----------------------------------------------------*/
  //
  // Standard Definitions (only the first time)
  //
  if( nsurf_total == 0 )
  {
    nlog << "\n\n\n";
	  nlog << "/****************************************************************\n";
    nlog << "  STANDARD DEFINITIONS\n";
    nlog << "*****************************************************************/\n\n";

    nlog << "\n\ndefT = TRNSFMATf(\n";
    nlog << "  1., 0., 0., 0.,\n";
    nlog << "  0., 1., 0., 0.,\n";
    nlog << "  0., 0., 1., 0.,\n";
    nlog << "  0., 0., 0., 1. );\n\n";

    nlog << "defMFr = FRONTMATf(\n";
    nlog << "  (0.05, 0.15, 0.075, 1.0),        /* diffuse color    */\n";
    nlog << "  (0.0125, 0.0375, 0.019, 1.0),    /* ambient color    */\n";
    nlog << "  (0.0, 0.0, 0.0, 1.0),            /* emissive color   */\n";
    nlog << "  (0.2, 0.6, 0.3, 1.0),            /* specular color   */\n";
    nlog << "  0.5 );                           /* shininess        */\n\n";

    nlog << "defMBk = BACKMATf(\n";
    nlog << "  (0.3, 0.15, 0.12, 1.0),          /* diffuse color    */\n";
    nlog << "  (0.075, 0.0375, 0.03, 1.0),      /* ambient color    */\n";
    nlog << "  (0.0, 0.0, 0.0, 1.0),            /* emissive color   */\n";
    nlog << "  (0.8, 0.4, 0.3, 1.0),            /* specular color   */\n";
    nlog << "  0.5 );                           /* shininess        */\n\n";
  }

  nlog << "\n\n\n";
	nlog << "/****************************************************************\n";
  nlog << "  NURBS SURFACE\n";
  nlog << "*****************************************************************/\n\n";
    
  int nregions = mContours2D.nElems; // number of regions
  if( nregions <= 0 ) return;        // must have at least one region
		
	contour2D = mContours2D.head;  // first region
   	
  for( i = 0; i < nregions; i++ )    
  {
    nConts = contour2D->el.nElems;  /* number of contours of the current region*/
    if( nConts <= 0 ) return;

    seg = contour2D->el.head;       /* first trim element */
    int telem_total = 0;

    do
    {
      if( seg->el.poly.nElems() > 0 )  
      {
        nlog << "/* Trim Polyline */\n";
        nlog << "cnt_" << nsurf_total << "_" << i << "_" << telem_total << " = ";
        telem_total++;

        // open Polyline
        nlog << "POLYLINE2f(\n";
        nlog << "  (" << seg->el.poly[0].x << ", " << seg->el.poly[0].y << ")";

        for( j = 1; j < seg->el.poly.nElems(); j++ )
        {
          nlog << ",\n  (" << seg->el.poly[j].x << ", " << seg->el.poly[j].y << ")";
        }
        // close Polyline
        nlog << "\n);\n";
      }
      else
      {
        nlog << "/* Trim Curve */\n";
        nlog << "cnt_" << nsurf_total << "_" << i << "_" << telem_total << " = ";
        telem_total++;

        // open Nurbscurve
        nlog << "NURBCURV2f(\n\n";
        // knot vector
        nlog << "  /* Knot Vector */\n";
        nlog << "  (\n";
        nlog << "    " << seg->el.curv.knt.U[0];
        for( j = 1; j < seg->el.curv.knt.U.nElems(); j++ )
				  nlog << ",\n    " << seg->el.curv.knt.U[j];
        nlog << "\n  ),\n";

        // open control points
        nlog << "  /* Control Points */\n";
        nlog << "  (\n";
        // first control point
        nlog << "    (" << seg->el.curv.cpt.Pw[0].x << ", "
               << seg->el.curv.cpt.Pw[0].y << ", "
               << seg->el.curv.cpt.Pw[0].w << ")";				
        // remaining control points
        for( j = 1; j < seg->el.curv.cpt.Pw.nElems(); j++ )
          nlog << ",\n    (" << seg->el.curv.cpt.Pw[j].x << ", "
               << seg->el.curv.cpt.Pw[j].y << ", "
               << seg->el.curv.cpt.Pw[j].w << ")";
        // close control points
        nlog << "\n  )\n";
        // close Nurbscurve
        nlog << ");\n\n";
      }

      seg = seg->next;
    }
    while( seg != 0 );

    nlog << "\n";
    nlog << "reg_" << nsurf_total << "_" << i << " = REGION2f(";
    nlog << "cnt_" << nsurf_total << "_" << i << "_0";
    for( j = 1; j < nConts; j++ )
      nlog << ", cnt_" << nsurf_total << "_" << i << "_" << j;
    nlog << ");\n\n";

    contour2D = contour2D->next;
  }

  nlog << "\n";
  nlog << "part" << nsurf_total << " = PARTITION2f(";
  nlog << "reg_" << nsurf_total << "_" << "0";
  for( i = 1; i < nregions; i++ )
    nlog << ", reg_" << nsurf_total << "_" << i;
  nlog << ");\n\n\n";

  //
  // open Nurbssurface
  //

  nlog << "nsurf" << nsurf_total << " = NURBSURF3f" << "(\n\n";
  nlog << "  part" << nsurf_total << ",\n\n";

	nlog << "  /* Knot Vector U (degree = " << S_pu << ") */\n";
	nlog << "  (" << SU[0];
  for( i = 1; i < S_nu+S_pu+2; i++ )
    nlog << ", " << SU[i];
  nlog << "),\n\n";
		
  nlog << "  /* Knot Vector V (degree = " << S_pv << ") */\n";
  nlog << "  (" << SV[0];
  for( i = 1; i < S_nv+S_pv+2; i++ )
    nlog << ", " << SV[i];
  nlog << "),\n\n";
		
  nlog << "  /* Control Points */\n";
  nlog << "  (\n";

  // first row of control points
  nlog << "    (\n";
  nlog << "      (" << SPw[0][0].x << ", " << SPw[0][0].y << ", " 
       << SPw[0][0].z << ", " << SPw[0][0].w << ")";
  for( j = 1; j < S_nu+1; j++ )
  {
    nlog << ",\n      (" << SPw[0][j].x << ", " << SPw[0][j].y << ", " 
         << SPw[0][j].z << ", " << SPw[0][j].w << ")";
  } 
  nlog << "\n    )";

  // remaining rows of control points
  for( i = 1; i < S_nv+1; i++ )
  {
    nlog << ",\n    (\n";
			
    nlog << "      (" << SPw[i][0].x << ", " << SPw[i][0].y << ", "
         << SPw[i][0].z << ", " << SPw[i][0].w << ")";
    for( j = 1; j < S_nu+1; j++ )
    {
      nlog << ",\n      (" << SPw[i][j].x << ", " << SPw[i][j].y << ", "
           << SPw[i][j].z << ", " << SPw[i][j].w << ")";
    } 
    nlog << "\n    )";
  }

  // close control point array
  nlog << "\n  )\n";
		
  // close nurbs surface
  nlog << ");\n\n\n";

  //
  // Create a Scene Tree Node
  //
  nlog << "/* Scene Tree Node */\n";
  nlog << "node" << nsurf_total << " = SCENENODE ( (), (defT), (defMFr,defMBk), "; 
  nlog << "(nsurf" << nsurf_total << ") );\n\n";

  // SCENEROOT = SCENETREE( (pth1) );

  nsurf_total++;		
}

#endif   // _DUMP_TRIMMED_

#endif   // _LOG_TRIMMED_








#else G_T_NURBS

int GTrimmedNurbsShell::ComputeShell()
{ 
	return 0; 
}



#endif G_T_NURBS

