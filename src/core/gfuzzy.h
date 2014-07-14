

/******************************************************************************
@doc

@module gfuzzy.h 

Copyright (c) 1999 by blaxxun interactive
All rights reserved

Purpose:

Classes:

  
Notes:

		References:
			fuzzy demo programm
					Author:        John Manslow 
					E-mail:        jfm96r@ecs.soton.ac.uk 
					Web-address:   http://www.isis.soton.ac.uk/~jfm96r 
								   http://www.isis.ecs.soton.ac.uk/~jfm96r/FuzzyLogicIndex.html
Changes:



Todo :

******************************************************************************/



#ifndef _FuzzyRuleBase_ 
#define _FuzzyRuleBase_ 

#include <fstream.h>

#ifdef _DEBUG
	//#define _GFUZZY_LOG
#endif





//////////////////////////////////////////////////////////
//														//
//														//
//				class GFuzzySetElement					//
//														//
//////////////////////////////////////////////////////////

//base class for all membership functions
class GFuzzySetElement
{ 
public: 
	GFuzzySetElement(); 
	~GFuzzySetElement(); 
	
	//Required for fuzzification 
	virtual float GetMembership(const float); 
	
	//Required for defuzzification 
	virtual float GetArea(void); 
	virtual float GetCentreOfMass(void); 
	float cut, A;
}; 






//////////////////////////////////////////////////////////
//														//
//														//
//					GFuzzySetLinearElement				//
//														//
//////////////////////////////////////////////////////////

class GFuzzySetLinearElement: public GFuzzySetElement 
{ 
public: 
	GFuzzySetLinearElement(char *newName, const float,const float,const float,const float); 
	~GFuzzySetLinearElement(); 
	float GetMembership(const float); 
	float GetArea(void); 
	float GetCentreOfMass(void); 
	//CString name;
private: 
	float LowerBound,Centre,UpperBound,Height; 
}; 







//////////////////////////////////////////////////////////
//														//
//														//
//						CFuzzyRule						//
//														//
//////////////////////////////////////////////////////////

class GFuzzyRule 
{ 
public: 
	GFuzzyRule(const int numberOfInputs, GFuzzySetElement *pNewInputMF, GFuzzySetElement *pNewOutputMF);
	~GFuzzyRule();
	//float PerformInference(const float * const); 


//private: 
	int NumberOfInputs; 
	//int *NumberOfInputMFs; 
	//falls variable anzahl von inputs GFuzzySetElement **pInputMF; 
	GFuzzySetElement *pInputMF; 
	GFuzzySetElement *pOutputMF; 

	float * RuleActivation; 
}; 





//////////////////////////////////////////////////////////
//														//
//														//
//						GFuzzyRuleBase					//
//														//
//////////////////////////////////////////////////////////

class GFuzzyRuleBase 
{ 
public: 
	GFuzzyRuleBase(const int NewNumberOfRules, GFuzzyRule** NewRuleTable);
	~GFuzzyRuleBase(); 
	float PerformInference(const float * const); 


private: 
	int NumberOfRules; 
	GFuzzyRule **rule;
	//int NumberOfInputs; 
	//int *NumberOfInputMFs; 
	//GFuzzySetElement ***pInputMF; 
	//GFuzzySetElement **pOutputMF; 

	//float * RuleActivation; 
}; 







//////////////////////////////////////////////////////////
//														//
//														//
//						GFuzzyLogic						//
//														//
//////////////////////////////////////////////////////////


class GFuzzyLogic
{
public:
	GFuzzyLogic();
	~GFuzzyLogic();

	float Initialize(float targetFR);
	float applyLogic(float inputValue);
	void testLogic(void);

private:	
	GFuzzyRuleBase *pFuzzyRuleBase;

#ifdef _GFUZZY_LOG
	ofstream out;
#endif
};

#endif 
