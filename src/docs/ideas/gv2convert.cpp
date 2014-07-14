#include "stdafx.h"

#include <gvnodes.h>

class GVrml2ConvertTraversal : public GGeometryTraversal 
{
public: 

	GVrml2ConvertTraversal() { }

	GvNode *GetNodeTranslation(GvNode *node);

	void SetNodeTranslation(GvNode *node, GvNode * translatedNode);

	GvAppearance* GetAppearance();
	GvShape* BuildShape(GvNode * geometry);

	GvNode * Convert(GvNode *node);
	
	int Do(GNode *n) {	state.result = Convert(n); return(state.result != NULL); }

};


void GVrml2ConvertTraversal::SetNodeTranslation(GvNode *node, GvNode * translatedNode);

// build appearance node for current state
GvAppearance* GVrml2ConvertTraversal::GetAppearance()
{
	GvAppearance *n = new GvApperance();
	
	// Material :
	GvMaterial *material=NULL; 
	GvMaterialElement *materialTop = (GvMaterialElement*) state.Get(GvMaterialElement::attrId); 
	if (materialTop) material = (GvMaterial *) materialTop->data;
	G_GETSTATE(state,materialB,GvMaterialBinding,	GvMaterialBindingElement)
	if (material) n->material.Set(Convert(material));

	// texture 2
	G_GETSTATE(state,texture2,GvTexture2,	GvTexture2Element)
	if (texture2) n->texture.Set(Convert(texture2));

	// texture2Transform
	GTextureMatrixStack *current;
	current = (GTextureMatrixStack*) state.Get(GTextureMatrixStack::attrId);
	if (current) {
		Gv2TextureMatrix *m = new Gv2TextureMatrix;
		m->Set(current->m);
		n->textureTransform.Set(m);
    }
	return(n);
}

// build shape node for geometry with current appearance
GvShape* GVrml2ConvertTraversal::BuildShape(GvNode * geometry)
{
	GvShape *n = new GvShape();
	n->geometry.Set(geometry);
	n->appearance.Set(GetAppearance());
	return(n);
}



GvNode * GVrml2ConvertTraversal::Convert(GvNode *node) 
{
	GvNode *t=NULL;

	// node already translated ???
	t = GetNodeTranslation(node);
	if (t) return(t);
	
	// already a vrml2 node ? 
	//if (node->IsVrml2Node()) return(node);
//	else 
	{
		this.result = NULL;
		int ret = node->Do(state);
		t = this.result;
		if (ret>0) {
		  SetNodeTranslation(node,t);
		}
	}
	return(t);
}


// Gv2Geometry
// put attributes in stack
// convert if needed by a faceset

int GvCoordinate3::Do(GVrml2ConvertTraversal &state)
{	GvCoordinate *n = new GvCoordinate();
	n->points  = point;
	state.result = n;
	return(n!=NULL);
}

int GvNormal:: Do(GVrml2ConvertTraversal &state)
{	Gv2Normal *n = new Gv2Normal();
	n->vectors = vector;
	state.result = n;
	return(n!=NULL);
}

int GvTextureCoordinate2:: Do(GVrml2ConvertTraversal &state)
{	GvTextureCoordinate *n = new GvTextureCoordinate();
	n->points = point;
	state.result = n;
	return(n!=NULL);
}
// GvMaterial
int GvMaterial::Do(GVrml2ConvertTraversal &state)
{	Gv2Material *n = new Gv2Material();
	state.result = n;
	return(n!=NULL);
}
// Texture 
int GvTexture2Transform::Do(GVrml2ConvertTraversal &state)
{	Gv2TextureTransform *n = new Gv2TextureTransform();
	state.result = n;
	return(n!=NULL);
}

int GvTexture2::Do(GVrml2ConvertTraversal &state)
{	Gv2Texture *n = NULL;
	
	
	state.result = n;
	return(n!=NULL);
}




// simple shapes
int GvCube::Do(GVrml2ConvertTraversal &state)
{	GvBox *n = new GvBox();
	state.result = n;
	return(n!=NULL);
}

int GvCone::Do(GVrml2ConvertTraversal &state)
{	Gv2Cone *n = new Gv2Cone();
	state.result = n;
	return(n!=NULL);
}

int GvCylinder::Do(GVrml2ConvertTraversal &state)
{	Gv2Cylinder *n = new Gv2Cylinder();
	state.result = n;
	return(n!=NULL);
}

int GvSphere::Do(GVrml2ConvertTraversal &state)
{	Gv2Sphere *n = new Gv2Sphere();
	state.result = n;
	return(n!=NULL);
}

// 

int GvIndexedFaceSet:: Do(GVrml2ConvertTraversal &state)
{	Gv2IndexedFaceSet *n = new Gv2IndexedFaceSet();
	
	G_GETSTATE(state,coord,GvCoordinate3,		GvCoordinate3Element)
	G_GETSTATE(state,materialB,GvMaterialBinding,	GvMaterialBindingElement)
	G_GETSTATE(state,normal,GvNormal,			GvNormalElement)
	G_GETSTATE(state,normalB,GvNormalBinding,	GvNormalBindingElement)
	G_GETSTATE(state,shapeHints,GvShapeHints,	GvShapeHintsElement)
	G_GETSTATE(state,textureCoord,GvTextureCoordinate2, GvTextureCoordinate2Element)


	GvMaterial *material=NULL; 
	GvMaterialElement *materialTop = (GvMaterialElement*) state.Get(GvMaterialElement::attrId); 
	if (materialTop) material = (GvMaterial *) materialTop->data;

	int normalBinding = GvNormalBinding::PER_VERTEX_INDEXED;
	if (normalB) normalBinding = normalB->value.value;
	if (normalBinding == GvNormalBinding::DEFAULT) normalBinding = GvNormalBinding::PER_VERTEX_INDEXED; // 10.95

	int materialBinding = GvMaterialBinding::OVERALL;
	if (materialB) materialBinding = materialB->value.value;

	// new property nodes
	GvCoordinate *ncoord = NULL;
	GvColor *ncolor =  NULL;
	Gv2Normal *nnormal = NULL;
	GvTextureCoordinate *ntexCoord =  NULL;

 	if (!coordIndex.isDefault()) {
		n->coordIndices = coordIndex;
	}
	if (coord) {
		ncoord = state.Convert(coord);
/*
		ncoord = new GvCoordinate();
		ncoord->points  = coord->point;
		n->coord.set(ncoord);
*/
    }

	switch (normalBinding) { // Normal binding value
	case GvNormalBinding::PER_VERTEX_INDEXED:

 	if (!normalIndex.isDefault()) {
		n->normalIndices = normalIndex;
	}
	if (normal) {
		nnormal = state.Convert(normal);
/*
		nnormal = new Gv2Normal();
		nnormal->vectors = normal->vector;
		n->normal.set(nnormal);
*/
    }
	break;

	case GvNormalBinding::PER_FACE_INDEXED:
    n->normalPerVertex.set(FALSE);
 	if (!normalIndex.isDefault()) {
		n->normalIndices = normalIndex;
	}
	if (normal) {
		nnormal = state.Convert(normal);
/*
		nnormal = new Gv2Normal();
		nnormal->vectors = normal->vector;
		n->normal.set(nnormal);
*/  
	}
	break;
	}

	if (!textureCoordIndex.isDefault()) {
		n->texCoordIndices = textureCoordIndex;
	}
	if (textureCoord) {
		ntexCoord = state.Convert(textureCoord);
/*
		ntexCoord = new GvTextureCoordinate();
		ntexCoord->points  = textureCoord->point;
		n->texCoord.set(ntexCoord);
*/
  }
 if (shapeHints) {
	if (shapeHints->faceType == GvShapeHints::UNKNOWN_FACE_TYPE)) {
		convex.set(FALSE);
	}
	// to do ccw:
	// to do: solid
 }

	state.result = n;
	return(n);
}

int GvGroup:EvalMatrix(Matrix &m,int nextChild)
{
}

int GvGroup:: Do(GVrml2ConvertTraversal &state)
{	Matrix m;
	
	m.Identity();

	int i=0;
	int hasMatrix = 0;
	Gv2Group *root = NULL;
	Gv2Group *owner = NULL;

	Gv2Group *group = NULL;
	Gv2Transform *tgroup=NULL;
	
	
	while (i<getNumChildren()) {
		GvNode *child = getChild(i);
		GvNode *newChild = NULL;

		// action put in stack 
		if (	RTISA(child,GvMaterial)
			||  RTISA(child,GvNormal)
			||  RTISA(child,GvTextureCoordinate2)
			||  RTISA(child,GvShapeHints)
			||  RTISA(child,GvNormalBinding)
			||  RTISA(child,GvMaterialBinding)
			||  RTISA(child,GvTexture2Transform)
		   ) {
			newChild = NULL;
			child->Do( * ((GGeometryTraversal *) &state));
		} else 
		// action new Transform &&  put in stack 
		if (	RTISA(child,GvTranslation)
			||  RTISA(child,GvScale)
			||  RTISA(child,GvRotation)
			||  RTISA(child,GvRotationXYZ)
			||  RTISA(child,GvMatrixTransform)
	   ) {
			if (group != NULL) {
				owner = group;
				group = NULL;
				hasTransform = 1;
				m = IdentiyMatrix();
			}
			Matrix newM;
			child->Get(newM);
			m = m * newM; 
			newChild = NULL;
			// put in state
			child->Do( * ((GGeometryTraversal *) &state));
		} else 
		// action ignore
	
		// action geometry 
		if (RTISA(child,GvNodeShell))
		{
			newChild = state.Convert(child);
			newChild = BuildShape(newChild);
		}

		// action keep
		else 
			newChild = state.Convert(child);

		// add the new child ....
		if (newChild) {
			if (group == NULL) 
				if (hasMatrix)  { 
					group = tgroup = new Gv2Transform();
					tgroup->SetMatrix(m);

				}
				else 
					group = new Gv2Group();
				
				if (root == NULL) root = owner = group;
				else owner->addChild(group);

			}
			group->addChild(newChild);

	}
	state.result = root;
	return(root != NULL);
}



