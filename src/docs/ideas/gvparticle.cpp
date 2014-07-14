/******************************************************************************
@doc

@module gvnurbs.cpp 
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
#include "gvparticle.h"






void GvParticle::init() 
{

	clearParticles();
	createParticles(startCount);
}


void GvParticle::update(float dt) 
{

	updateParticles(dt);
 	createParticles(dt*rate);
	if (getNumParticles() > maxCount) 
	{
		deleteParticles(getNumParticles() - maxCount);
	}

}

void GvParticle::initialize(GParticle *parts) 
{
	int i;
	GParticle *p;
	int n= targetType.getNum();
	
	for	(i=0; i<n;i++) {
		
		GvString &type = targetType.values[i];
		GvNode *init=targetInitializer[i];


		if (type == "position") {
			GvBoxEmitter *e = RTCAST(init,GvBoxEmitter);
			if(e) { for (p=parts;p!=NULL; p= p->next) { p->initPosition(e); } continue; }
			continue;
		}
		// velocity
		if (type == "velocity") {
			GvConeEmitter *e = RTCAST(init,GvConeEmitter);
			if(e) { for (p=parts;p!=NULL; p= p->next) { p->initVelocity(e); } continue; }
			continue;
		}

		// color
		if (type == "color") {
			GvBoxEmitter *e = RTCAST(init,GvBoxEmitter);
			if(e) { for (p=parts;p!=NULL; p= p->next) { p->initColor(e); } continue; }
			continue;
		}

		// normal 
	}		

}


// create num new particeles and initialize the particles
void GvParticle::createParticles(int num)
{

	GParticle *p=NULL;

	if (num<=0) return;

	DWORD t=timeGetTime();
	
	// create list of num new particles
	for( ; num>0;num--) {
		GParticle *pnew=newParticle();
		if (pnew == NULL) break;
		pnew->next=p;
		pnew->initTime=t;
		p=pnew;
	}

	// initialize values
	initialize(p);

	// append new particle list
	append(p);

}


// delete num old particeles
void GvParticle::deleteParticles(int num)
{

	GParticle *p=NULL;
	int i;

	if (num<=0) return;

	// delete num first particles
	for( ; num>0;num--) {
		GParticle *pfree=firstPart;

		if (pfree == NULL) {
			lastPart=NULL;
			break;
		}
		firstPart = pfree->next;
		freePart(pfree);
		numParts--;
	}

	// re-alloc id's
	{ for (p=firstPart,i=0;p!=NULL; p= p->next;i++) { 
		p->id=i; 
	}

}
