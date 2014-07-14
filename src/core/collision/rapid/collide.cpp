 #include "stdafx.h"
/*************************************************************************\

  Copyright 1995 The University of North Carolina at Chapel Hill.
  All Rights Reserved.

  Permission to use, copy, modify and distribute this software and its
  documentation for educational, research and non-profit purposes, without
   fee, and without a written agreement is hereby granted, provided that the
  above copyright notice and the following three paragraphs appear in all
  copies.

  IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL BE
  LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
  CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE
  USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY
  OF NORTH CAROLINA HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGES.

  THE UNIVERSITY OF NORTH CAROLINA SPECIFICALLY DISCLAIM ANY
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE
  PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
  NORTH CAROLINA HAS NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT,
  UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

  The authors may be contacted via:

  US Mail:             S. Gottschalk
                       Department of Computer Science
                       Sitterson Hall, CB #3175
                       University of N. Carolina
                       Chapel Hill, NC 27599-3175

  Phone:               (919)962-1749

  EMail:              {gottscha}@cs.unc.edu


\**************************************************************************/


#include "rapid_version.h"

static char rapidtag_data[] = "RAPIDTAG  file: "__FILE__"    date: "__DATE__"    time: "__TIME__;

// to silence the compiler's complaints about unreferenced identifiers.
static void r1(char *f){  r1(f);  r1(rapidtag_data);  r1(rapid_version);}

extern int RAPID_initialized;
void RAPID_initialize();

#include "rapid.h"
#include "matvec.h"
#include "overlap.h"
#include "obb.h"



int add_collision(int id1, int id2);


void Mult(const Matrix& M, const float *v, float *ret)
{//point * matrix
    ret[0] = v[0] * M[0][0] + v[1] * M[1][0] + v[2] * M[2][0] + M[3][0];
    ret[1] = v[0] * M[0][1] + v[1] * M[1][1] + v[2] * M[2][1] + M[3][1];
    ret[2] = v[0] * M[0][2] + v[1] * M[1][2] + v[2] * M[2][2] + M[3][2];
}


int rapidCollide::tri_contact(box *b1, box *b2)
{
  // assume just one triangle in each box.

  // the vertices of the tri in b2 is in model1 C.S.  The vertices of
  // the other triangle is in model2 CS.  Use RAPID_mR, RAPID_mT, and
  // RAPID_ms to transform into model2 CS.

  float i1[3];
  float i2[3];
  float i3[3];
  
//  float i1a[3];
//  float i2a[3];
// float i3a[3];
  int rc;  // return code

  // Mult(b1Tob2, b1->trp->p1, i1);
  // Mult(b1Tob2, b1->trp->p2, i2);
   //Mult(b1Tob2, b1->trp->p3, i3);
  
   
  sMxVpV(i1, RAPID_ms, RAPID_mR, b1->trp->p1, RAPID_mT);
  sMxVpV(i2, RAPID_ms, RAPID_mR, b1->trp->p2, RAPID_mT);
  sMxVpV(i3, RAPID_ms, RAPID_mR, b1->trp->p3, RAPID_mT);

//  if (i1a[0] != i1[0])
//	  TRACE("error\n");



  RAPID_num_tri_tests++;

  int f = tri_contact(i1, i2, i3, b2->trp->p1,b2->trp->p2, b2->trp->p3);

  if (f) 
    {
      // add_collision may be unable to allocate enough memory,
      // so be prepared to pass along an OUT_OF_MEMORY return code.
      if ((rc = add_collision(b1->trp->id, b2->trp->id)) != RAPID_OK)
	return rc;
    }
  
  return RAPID_OK;
}



int rapidCollide::collide_recursive(box *b1, box *b2, float R[3][3], float T[3], float s)
{
  float d[3]; // temp storage for scaled dimensions of box b2.
  int rc;      // return codes
  
  if (1)
    {
#if TRACE1
      printf("Next collision: b1, b2, R, T, s\n");
      printf("b1=%x, b2=%x\n", b1, b2);
      Mprint(R);
      Vprint(T);
      printf("%lf\n", s);
#endif
      
      if (RAPID_first_contact && (RAPID_num_contacts > 0)) return RAPID_OK;

      // test top level

      RAPID_num_box_tests++;
  
      int f1;
  
      d[0] = s * b2->d[0];
      d[1] = s * b2->d[1];
      d[2] = s * b2->d[2];
      f1 = obb_disjoint(R, T, b1->d, d);

#if TRACE1
      if (f1 != 0)
	{
	  printf("BOX TEST %d DISJOINT! (code %d)\n", RAPID_num_box_tests, f1);
	}
      else
	{
	  printf("BOX TEST %d OVERLAP! (code %d)\n", RAPID_num_box_tests, f1);
	}
      
#endif

      if (f1 != 0) 
	{
	  return RAPID_OK;  // stop processing this test, go to top of loop
	}

      // contact between boxes
      if (b1->leaf() && b2->leaf()) 
	{
	  // it is a leaf pair - compare the polygons therein
          // tri_contact uses the model-to-model transforms stored in
	  // RAPID_mR, RAPID_mT, RAPID_ms.

	  // this will pass along any OUT_OF_MEMORY return codes which
	  // may be generated.
	  return tri_contact(b1, b2);
	}

      float U[3];

      float cR[3][3], cT[3], cs;
      
      // Currently, the transform from model 2 to model 1 space is
      // given by [B T s], where y = [B T s].x = s.B.x + T.

      if (b2->leaf() || (!b1->leaf() && (b1->size() > b2->size())))
	{
	  // here we descend to children of b1.  The transform from
	  // a child of b1 to b1 is stored in [b1->N->pR,b1->N->pT],
	  // but we will denote it [B1 T1 1] for short.  Notice that
	  // children boxes always have same scaling as parent, so s=1
	  // for such nested transforms.

	  // Here, we compute [B1 T1 1]'[B T s] = [B1'B B1'(T-T1) s]
	  // for each child, and store the transform into the collision
	  // test queue.

	  MTxM(cR, b1->N->pR, R); 
	  VmV(U, T, b1->N->pT); MTxV(cT, b1->N->pR, U);
	  cs = s;

	  if ((rc = collide_recursive(b1->N, b2, cR, cT, cs)) != RAPID_OK)
	    return rc;
	  
	  MTxM(cR, b1->P->pR, R); 
	  VmV(U, T, b1->P->pT); MTxV(cT, b1->P->pR, U);
	  cs = s;

	  if ((rc = collide_recursive(b1->P, b2, cR, cT, cs)) != RAPID_OK)
	    return rc;
	  
	  return RAPID_OK;
	}
      else 
	{
	  // here we descend to the children of b2.  See comments for
	  // other 'if' clause for explanation.

	  MxM(cR, R, b2->N->pR); sMxVpV(cT, s, R, b2->N->pT, T);
	  cs = s;
	  
	  if ((rc = collide_recursive(b1, b2->N, cR, cT, cs)) != RAPID_OK)
	    return rc;
	  
	  MxM(cR, R, b2->P->pR); sMxVpV(cT, s, R, b2->P->pT, T);
	  cs = s;

	  if ((rc = collide_recursive(b1, b2->P, cR, cT, cs)) != RAPID_OK)
	    return rc;
	  
	  return RAPID_OK; 
	}
  
    }
  
  return RAPID_OK;
} 


  //without scale
int rapidCollide::Collide(float R1[3][3], float T1[3], RAPID_model *RAPID_model1,
	float R2[3][3], float T2[3], RAPID_model *RAPID_model2,
	int flag)
{
  return Collide(R1, T1, 1.0, RAPID_model1, R2, T2, 1.0, RAPID_model2, flag);
}






int rapidCollide::Collide(const Matrix &m1, RAPID_model *RAPID_model1,
	const Matrix &m2, RAPID_model *RAPID_model2,
	int flag)
{	

#ifdef _DEBUG_RAPID		
	Point scale,shear,rotate,translate;
	m1.Decompose(scale,shear,rotate,translate);
	if (scale.x != 1 || scale.y != 1 || scale.z != 1)
		TRACE("rapidCollide::Collide: scale! invalid parameters!\n");

	
	m2.Decompose(scale,shear,rotate,translate);
	if (scale.x != 1 || scale.y != 1 || scale.z != 1)
		TRACE("rapidCollide::Collide: scale! invalid parameters!\n");
	Matrix tmpMat;
	Point einheit;
	einheit.One();
	tmpMat.Compose(rotate, einheit, einheit, einheit);
	
#endif _DEBUG_RAPID	
	
	float R1[3][3];
	float T1[3];
	float R2[3][3];
	float T2[3];
	
	R1[0][0] = m1[0][0];
	R1[0][1] = m1[1][0];
	R1[0][2] = m1[2][0];
	R1[1][0] = m1[0][1];
	R1[1][1] = m1[1][1];
	R1[1][2] = m1[2][1];
	R1[2][0] = m1[0][2];
	R1[2][1] = m1[1][2];
	R1[2][2] = m1[2][2];
	
	T1[0] = m1[3][0];
	T1[1] = m1[3][1];
	T1[2] = m1[3][2];

	R2[0][0] = m2[0][0];
	R2[0][1] = m2[1][0];
	R2[0][2] = m2[2][0];
	R2[1][0] = m2[0][1];
	R2[1][1] = m2[1][1];
	R2[1][2] = m2[2][1];
	R2[2][0] = m2[0][2];
	R2[2][1] = m2[1][2];
	R2[2][2] = m2[2][2];
	
	T2[0] = m2[3][0];
	T2[1] = m2[3][1];
	T2[2] = m2[3][2];
	
	//call the RAPID collision detection routine.
#ifdef _DEBUG_RAPID
	TRACE("rapidCollide::Collide\n");
#endif _DEBUG_RAPID
  return Collide(R1, T1, 1, RAPID_model1, R2, T2, 1, RAPID_model2, flag);
}



int rapidCollide::CollideWithScale(const Matrix &m1, RAPID_model *RAPID_model1,
	const Matrix &m2, RAPID_model *RAPID_model2,
	int flag)
{
	

	
	Point scale,shear,rotate,translate;
	float s1, s2;

	m1.Decompose(scale,shear,rotate,translate);
	s1 = scale.x;
	//s1 = max(scale.x,max(scale.y,scale.z));

#ifdef _DEBUG_RAPID
	if (scale.x != scale.y || scale.y != scale.z)
		TRACE("rapidCollide::CollideWithScale: nonuniform scale in mat1! invalid parameters! scale=%f %f %f\n", scale.x, scale.y, scale.z);
	//Probe
	Matrix tmpMat;
	Point einheit;
	einheit.One();
	tmpMat.Compose(scale, einheit, rotate ,translate);
#endif _DEBUG_RAPID	
	
	Matrix rot1 = RotationMatrix(rotate);
		
	m2.Decompose(scale,shear,rotate,translate);
	s2 = scale.x;
	//s2 = max(scale.x,max(scale.y,scale.z));
#ifdef _DEBUG_RAPID
	if (scale.x != scale.y || scale.y != scale.z)
		TRACE("rapidCollide::CollideWithScale: nonuniform scale in mat2! invalid parameters! scale=%f %f %f\n", scale.x, scale.y, scale.z);
	TRACE("rapidCollide::CollideWithScale scale1=%f scale2=%f\n", s1, s2);
	
//	tmpMat.Compose(scale, einheit, rotate ,translate);

//	AStream out("matrix.log", "w");
//	tmpMat.io(out);
//	TRACE("matrix \n");
#endif _DEBUG_RAPID	

	
	Matrix rot2 = RotationMatrix(rotate);

	float R1[3][3];
	float T1[3];
	float R2[3][3];
	float T2[3];
	
	R1[0][0] = rot1[0][0];
	R1[0][1] = rot1[1][0];
	R1[0][2] = rot1[2][0];
	R1[1][0] = rot1[0][1];
	R1[1][1] = rot1[1][1];
	R1[1][2] = rot1[2][1];
	R1[2][0] = rot1[0][2];
	R1[2][1] = rot1[1][2];
	R1[2][2] = rot1[2][2];
	
	T1[0] = m1[3][0];
	T1[1] = m1[3][1];
	T1[2] = m1[3][2];

	R2[0][0] = rot2[0][0];
	R2[0][1] = rot2[1][0];
	R2[0][2] = rot2[2][0];
	R2[1][0] = rot2[0][1];
	R2[1][1] = rot2[1][1];
	R2[1][2] = rot2[2][1];
	R2[2][0] = rot2[0][2];
	R2[2][1] = rot2[1][2];
	R2[2][2] = rot2[2][2];
	
	T2[0] = m2[3][0];
	T2[1] = m2[3][1];
	T2[2] = m2[3][2];
	
	//call the RAPID collision detection routine.
	//::Collide(R1, T1, vc_objects[i]->b, R2, T2, vc_objects[curr_ovrlp->id]->b, FIRST_CONTACT);
	
  return Collide(R1, T1, s1, RAPID_model1, R2, T2, s2, RAPID_model2, flag);
}




int rapidCollide::Collide(float R1[3][3], float T1[3], float s1, RAPID_model *RAPID_model1,
	float R2[3][3], float T2[3], float s2, RAPID_model *RAPID_model2,
	int flag)
{
  if (!RAPID_initialized) RAPID_initialize();

  if (RAPID_model1->build_state != RAPID_BUILD_STATE_PROCESSED)
    return RAPID_ERR_UNPROCESSED_MODEL;

  if (RAPID_model2->build_state != RAPID_BUILD_STATE_PROCESSED)
    return RAPID_ERR_UNPROCESSED_MODEL;

  box *b1 = RAPID_model1->b;
  box *b2 = RAPID_model2->b;
  
  RAPID_first_contact = 0; 
  if (flag == FIRST_CONTACT) RAPID_first_contact = 1;
  
  float tR1[3][3], tR2[3][3], R[3][3];
  float tT1[3], tT2[3], T[3], U[3];
  float s;
  
  // [R1,T1,s1] and [R2,T2,s2] are how the two triangle sets
  // (i.e. models) are positioned in world space.  [tR1,tT1,s1] and
  // [tR2,tT2,s2] are how the top level boxes are positioned in world
  // space
  
  MxM(tR1, R1, b1->pR);                  // tR1 = R1 * b1->pR;
  sMxVpV(tT1, s1, R1, b1->pT, T1);       // tT1 = s1 * R1 * b1->pT + T1;
  MxM(tR2, R2, b2->pR);                  // tR2 = R2 * b2->pR;
  sMxVpV(tT2, s2, R2, b2->pT, T2);       // tT2 = s2 * R2 * b2->pT + T2;
  
  // (R,T,s) is the placement of b2's top level box within
  // the coordinate system of b1's top level box.

  MTxM(R, tR1, tR2);                            // R = tR1.T()*tR2;
  VmV(U, tT2, tT1);  sMTxV(T, 1.0/s1, tR1, U);  // T = tR1.T()*(tT2-tT1)/s1;
  
  s = s2/s1;

  // To transform tri's from model1's CS to model2's CS use this:
  //    x2 = ms . mR . x1 + mT

  {
    MTxM(RAPID_mR, R2, R1);
    VmV(U, T1, T2);  sMTxV(RAPID_mT, 1.0/s2, R2, U);
    RAPID_ms = s1/s2;
  }
  

  // reset the report fields
  RAPID_num_box_tests = 0;
  RAPID_num_tri_tests = 0;
  RAPID_num_contacts = 0;

  // make the call
  return collide_recursive(b1, b2, R, T, s);
}


int rapidCollide::add_collision(int id1, int id2)
{
  if (!RAPID_contact)
    {
      RAPID_contact = new collision_pair[10];
      if (!RAPID_contact) 
	{
	  return RAPID_ERR_COLLIDE_OUT_OF_MEMORY;
	}
      RAPID_num_cols_alloced = 10;
      RAPID_num_contacts = 0;
    }
  
  if (RAPID_num_contacts == RAPID_num_cols_alloced)
    {
      collision_pair *t = new collision_pair[RAPID_num_cols_alloced*2];
      if (!t)
	{
	  return RAPID_ERR_COLLIDE_OUT_OF_MEMORY;
	}
      RAPID_num_cols_alloced *= 2;
      
      for(int i=0; i<RAPID_num_contacts; i++) t[i] = RAPID_contact[i];
      delete [] RAPID_contact;
      RAPID_contact = t;
    }
  
  RAPID_contact[RAPID_num_contacts].id1 = id1;
  RAPID_contact[RAPID_num_contacts].id2 = id2;
  RAPID_num_contacts++;

  return RAPID_OK;
}

/*
int 
Collide_old(const Matrix &m1, RAPID_model *RAPID_model1,
	const Matrix &m2, RAPID_model *RAPID_model2,
	int flag)
{
	Collide_old(m1, RAPID_model1,m2, RAPID_model2,flag);

	Point scale,shear,rotate,translate;

	// convert coordinates from space of model2 to model 1

	Matrix btoa, box1Mat, box2Mat, mat;

  box *b1 = RAPID_model1->b;
  box *b2 = RAPID_model2->b;
	mat[0][0] = b1->pR[0][0];
	mat[0][1] = b1->pR[1][0];
	mat[0][2] = b1->pR[2][0];
	mat[1][0] = b1->pR[0][1];
	mat[1][1] = b1->pR[1][1];
	mat[1][2] = b1->pR[2][1];
	mat[2][0] = b1->pR[0][2];
	mat[2][1] = b1->pR[1][2];
	mat[2][2] = b1->pR[2][2];
	
	mat[3][0] = b1->pT[0];
	mat[3][1] = b1->pT[1];
	mat[3][2] = b1->pT[2];


  box1Mat = mat * m1;

	mat[0][0] = b2->pR[0][0];
	mat[0][1] = b2->pR[1][0];
	mat[0][2] = b2->pR[2][0];
	mat[1][0] = b2->pR[0][1];
	mat[1][1] = b2->pR[1][1];
	mat[1][2] = b2->pR[2][1];
	mat[2][0] = b2->pR[0][2];
	mat[2][1] = b2->pR[1][2];
	mat[2][2] = b2->pR[2][2];
	
	mat[3][0] = b2->pT[0];
	mat[3][1] = b2->pT[1];
	mat[3][2] = b2->pT[2];

	box2Mat =  mat * m1;

	btoa = box2Mat * Invert(box1Mat);
	btoa.Decompose(scale,shear,rotate,translate);

	Matrix rot = RotationMatrix(rotate);
	rot *=TranslationMatrix(translate);

	float S;
	
	RAPID_mR[0][0] = rot[0][0];
	RAPID_mR[0][1] = rot[1][0];
	RAPID_mR[0][2] = rot[2][0];
	RAPID_mR[1][0] = rot[0][1];
	RAPID_mR[1][1] = rot[1][1];
	RAPID_mR[1][2] = rot[2][1];
	RAPID_mR[2][0] = rot[0][2];
	RAPID_mR[2][1] = rot[1][2];
	RAPID_mR[2][2] = rot[2][2];

/*	
	RAPID_mT[0] = translate.x;
	RAPID_mT[1] = translate.y;
	RAPID_mT[2] = translate.z;
*/
/*
	RAPID_mT[0] = rot[3][0];
	RAPID_mT[1] = rot[3][1];
	RAPID_mT[2] = rot[3][2];

	S = max(scale.x,max(scale.y,scale.z));
	if (S>1.0) {
		TRACE("S>1 \n");
	}

	//call the RAPID collision detection routine.
	//::Collide(R1, T1, vc_objects[i]->b, R2, T2, vc_objects[curr_ovrlp->id]->b, FIRST_CONTACT);
	
//xxxx   return Collide(R1, T1, 1.0, RAPID_model1, R2, T2, 1.0, RAPID_model2, flag);

	if (!RAPID_initialized) RAPID_initialize();

	if (RAPID_model1->build_state != RAPID_BUILD_STATE_PROCESSED)
		return RAPID_ERR_UNPROCESSED_MODEL;

	if (RAPID_model2->build_state != RAPID_BUILD_STATE_PROCESSED)
		return RAPID_ERR_UNPROCESSED_MODEL;

//	box *b1 = RAPID_model1->b;
//	box *b2 = RAPID_model2->b;
  
	RAPID_first_contact = 0; 
	if (flag == FIRST_CONTACT) RAPID_first_contact = 1;


#if 0
  
  float tR1[3][3], tR2[3][3], R[3][3];
  float tT1[3], tT2[3], T[3], U[3];
  float s;
  
  // [R1,T1,s1] and [R2,T2,s2] are how the two triangle sets
  // (i.e. models) are positioned in world space.  [tR1,tT1,s1] and
  // [tR2,tT2,s2] are how the top level boxes are positioned in world
  // space
  
  MxM(tR1, R1, b1->pR);                  // tR1 = R1 * b1->pR;
  sMxVpV(tT1, s1, R1, b1->pT, T1);       // tT1 = s1 * R1 * b1->pT + T1;
  MxM(tR2, R2, b2->pR);                  // tR2 = R2 * b2->pR;
  sMxVpV(tT2, s2, R2, b2->pT, T2);       // tT2 = s2 * R2 * b2->pT + T2;
  
  // (R,T,s) is the placement of b2's top level box within
  // the coordinate system of b1's top level box.

  MTxM(R, tR1, tR2);                            // R = tR1.T()*tR2;
  VmV(U, tT2, tT1);  sMTxV(T, 1.0/s1, tR1, U);  // T = tR1.T()*(tT2-tT1)/s1;
  
  s = s2/s1;

  // To transform tri's from model1's CS to model2's CS use this:
  //    x2 = ms . mR . x1 + mT

  {
    MTxM(RAPID_mR, R2, R1);
    VmV(U, T1, T2);  sMTxV(RAPID_mT, 1.0/s2, R2, U);
    RAPID_ms = s1/s2;
  }
#endif
  
  RAPID_ms = S;
  

  // reset the report fields
  RAPID_num_box_tests = 0;
  RAPID_num_tri_tests = 0;
  RAPID_num_contacts = 0;

  // make the call
  return collide_recursive(b1, b2, RAPID_mR, RAPID_mT, S);

}

*/
