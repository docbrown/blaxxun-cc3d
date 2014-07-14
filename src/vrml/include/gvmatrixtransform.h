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
#ifndef  _GV_MATRIX_TRANSFORM_
#define  _GV_MATRIX_TRANSFORM_

#include <gvsfmatrix.h>
#include <gvsubnode.h>

class GvMatrixTransform : public GvNode {

    GV_NODE_HEADER(GvMatrixTransform);

  public:
    // Fields
    GvSFMatrix		matrix;		// Transformation matrix

	void get(Matrix &m) {
		matrix.get(m);
	}
	
	int isDefault() { return(matrix.isDefault()); }

	//! get matrix of node , FALSE if node has no matrix 
	GvBool getMatrix(Matrix &m);

	void setMatrix(const Matrix &m) 
	{
			matrix.set(m);
	}
	
	int Do(GTraversal &state);   
	int Do(GOptimizeTraversal &state);	// implemented in GOptimizeTraversal.cpp



};

#endif /* _GV_MATRIX_TRANSFORM_ */
