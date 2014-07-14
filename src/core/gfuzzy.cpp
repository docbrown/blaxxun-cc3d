
#include "stdafx.h"     //This may have to be removed 
                        //if not compiled for Windows. 
#include "gfuzzy.h" 
#include "math.h" 



#define STATIC_AREA 4.f
#define OK_AREA 40.f
#define FR_UPPER_BOUND 80.f
#define FR_LOWER_BOUND 10.f

//////////////////////////////////////////////////////////
//														//
//														//
//						GFuzzyLogic						//
//														//
//////////////////////////////////////////////////////////


GFuzzyLogic::GFuzzyLogic()
{
	pFuzzyRuleBase= NULL;
	Initialize(20.f);
}


float GFuzzyLogic::Initialize(float targetFR)
{

	//////////////////////////////////////////////////////////
	// the fuzzy rules:
	//
	// if framerate == low THEN less render budget
	// if framerate == ok  THEN keep render budget
	// if framerate == high THEN more rendering budget
	//
	// manchmal auch kein impact, wenn budget kleiner gemacht wird:
	// if deltabudget negativ und framerate immer noch konstant dann budget higher
	//////////////////////////////////////////////////////////
	
	if (targetFR < FR_LOWER_BOUND)
		targetFR = FR_LOWER_BOUND;

	if (targetFR > FR_UPPER_BOUND)
		targetFR = FR_UPPER_BOUND;
	
	if (pFuzzyRuleBase)	
		delete pFuzzyRuleBase;
	
	
	int NumberOfRules=3; 
	int NumberOfInputs=1; //framerate 
	//int * NumberOfInputMFs; 


	
	GFuzzyRule** rule = new GFuzzyRule*[NumberOfRules];

	
/*
//	for(i=0;i<NumberOfRules;i++) 
	{ 
		//Allocate memory for array of pointers to membership functions 
		pInputMFs[0] = new GFuzzySetElement *[NumberOfInputMFs[i]]; 
//		MFInputNumber[i]=new unsigned long[NumberOfInputMFs[i]]; 
	} 
  */	
	float okCentre = targetFR;
	float okLowerBound = okCentre - OK_AREA/2;
	float okUpperBound = okCentre + OK_AREA/2;

	float lowLowerBound = -20.f;
	float lowUpperBound = okCentre - STATIC_AREA/2;
	float lowCentre = (lowLowerBound + lowUpperBound)/2;

	float highLowerBound = okCentre + STATIC_AREA/2;
	float highUpperBound = 150.f ;
	float highCentre = (highLowerBound + highUpperBound)/2;


	TRACE("GFuzzyLogic::Initialize: low(%f %f %f) ok(%f %f %f) high(%f %f %f)\n", lowLowerBound, lowCentre, lowUpperBound,
		okLowerBound, okCentre, okUpperBound, highLowerBound, highCentre, highUpperBound);
	
	GFuzzySetLinearElement *low, *ok, *high;
	low= new GFuzzySetLinearElement("low", lowLowerBound, lowCentre, lowUpperBound, 1); 
	ok=  new GFuzzySetLinearElement("ok",  okLowerBound, okCentre, okUpperBound, 1);
	high=new GFuzzySetLinearElement("high",  highLowerBound, highCentre, highUpperBound, 1);
/*
	low= new GFuzzySetLinearElement("low", -20, 0, 18.0, 1); 
	ok=  new GFuzzySetLinearElement("ok", 0.0 , 20.0, 40.0, 1);
	high=new GFuzzySetLinearElement("high", 22.0 , 40.0, 150.0, 1);

*/
	GFuzzySetLinearElement *lessBudget, *keepBudget, *moreBudget;
	lessBudget=new GFuzzySetLinearElement("lessBudget", -1.5, -1, -.5, 1.0); 
	keepBudget=new GFuzzySetLinearElement("keepBudget", -.5, 0.f, .5, 1.0); 
	moreBudget=new GFuzzySetLinearElement("moreBudget", .5, 1.0, 1.5, 1.0);
	
	//
	// initialize the rules in the rule table
	//
	rule[0] = new GFuzzyRule(1, low, lessBudget);
	rule[1] = new GFuzzyRule(1, ok, keepBudget);
	rule[2] = new GFuzzyRule(1, high, moreBudget);


	//
	//Create the fuzzy rule base and pass it its structure 
	//
	 pFuzzyRuleBase = new GFuzzyRuleBase(NumberOfRules, rule);
	 


#ifdef _GFUZZY_LOG
	//
	// perform a test
	//
	 out.open("c:\\cc3d\\fuzzy.log");
	 testLogic();
#endif

	 return targetFR;
}



GFuzzyLogic::~GFuzzyLogic()
{
	delete pFuzzyRuleBase;
}


float GFuzzyLogic::applyLogic(float inputValue)
{	
	//Allocate memory to use rule base 
	//float *Input;
	//Input = new float[pFuzzyRuleBase->NumberOfInputs];
	float Output; 
	//Input[0]= (float) inputValue;
  
	//Give the rule base inputs, and perform fuzzy inference to compute output 
	Output = pFuzzyRuleBase->PerformInference(&inputValue); 

#ifdef _GFUZZY_LOG
	out << inputValue << ' ' << Output << endl;
#endif

	return (float) Output;
}


void GFuzzyLogic::testLogic(void)
{
#ifdef _WIN32
	ofstream testOut("c:\\cc3d\\fuzzyTest.log");
	
	//Allocate memory to use rule base 

	float Input, Output; 

	for (int i=0; i<100; i++)
	{
		Input = (float)i;
  
		//Give the rule base inputs, and perform fuzzy inference to compute output 
		Output= pFuzzyRuleBase->PerformInference(&Input); 
		testOut << i << ' ' << Output << endl;
	}
#endif
}



//////////////////////////////////////////////////////////
//														//
//														//
//						GFuzzyRule						//
//														//
//////////////////////////////////////////////////////////



GFuzzyRule::GFuzzyRule(const int numberOfInputs, GFuzzySetElement *pNewInputMF, GFuzzySetElement *pNewOutputMF)

{ 
  	ASSERT(numberOfInputs>=1); 
	ASSERT(pInputMF);
	ASSERT(pOutputMF);

	//Record new parameters 
	NumberOfInputs = numberOfInputs;
	pInputMF = pNewInputMF;
	pOutputMF = pNewOutputMF;
} 


GFuzzyRule::~GFuzzyRule() 
{ 
	delete pInputMF;
	delete pOutputMF;
} 





//////////////////////////////////////////////////////////
//														//
//														//
//						GFuzzyRuleBase					//
//														//
//////////////////////////////////////////////////////////



GFuzzyRuleBase::GFuzzyRuleBase(const int NewNumberOfRules, GFuzzyRule** NewRuleTable) 
{ 
  	ASSERT(!(NewNumberOfRules<1)); 
	ASSERT(NewRuleTable);

	//Record new parameters 
	NumberOfRules=NewNumberOfRules; 
	rule = NewRuleTable;
} 


GFuzzyRuleBase::~GFuzzyRuleBase() 
{ 
	int i;
	for(i=0;i<NumberOfRules;i++) 
	{ 
		delete rule[i];
	} 
	delete rule;
} 



float GFuzzyRuleBase::PerformInference(const float * const Inputs) 
{ 
	
	ASSERT(Inputs); 
	int i; 
  
 //Temporary storage for rule activations 
	//float RuleActivation[3]; 
	
	float Numerator=0.0,Denominator=0.0; 

 
	//
	//Calculate rule activations 
	//
	 for(i=0;i<NumberOfRules;i++) 
	 { 
//		TRACE("\nRule #%i\n", i, j, RuleActivation[i]);

		 //RuleActivation[i]=1.0; 

			rule[i]->pOutputMF->cut = rule[i]->pInputMF->GetMembership(Inputs[0]); 
			
			TRACE("GFuzzyRuleBase::PerformInference: rule #%i: %f fuzzyfied: %f \n", i, Inputs[0], rule[i]->pOutputMF->cut);
	#ifdef _DEBUG
			//afxDump << ((GFuzzySetLinearElement*)pInputMF[i][j])->name;
	#endif
	}

	
	 //
	 //defuzzify 
	 //
	 for(i=0;i<NumberOfRules;i++) 
	 { 
		Numerator += rule[i]->pOutputMF->GetCentreOfMass() * rule[i]->pOutputMF->A; 
		Denominator += rule[i]->pOutputMF->A; 
	 } 
	 if(Denominator==0.0) 
		return 0.0; 
	 TRACE ( "Ergebnis: %g\n", Numerator/Denominator);
	 return Numerator/Denominator; 
} 
  
  




//////////////////////////////////////////////////////////
//														//
//														//
//				class GFuzzySetElement					//
//														//
//////////////////////////////////////////////////////////


GFuzzySetElement::GFuzzySetElement() 
{ 
	A = 0.f;
} 

GFuzzySetElement::~GFuzzySetElement() 
{ 
} 

float GFuzzySetElement::GetMembership(const float x) 
{ 
 return 0.0; 
}

float GFuzzySetElement::GetArea(void) 
{ 
 return 0.0; 
} 

float GFuzzySetElement::GetCentreOfMass(void) 
{ 
 return 0.0; 
} 
  




//////////////////////////////////////////////////////////
//														//
//														//
//					GFuzzySetLinearElement				//
//														//
//////////////////////////////////////////////////////////


GFuzzySetLinearElement::GFuzzySetLinearElement(char *newName, const float NewLowerBound, 
                          const float NewCentre, 
                          const float NewUpperBound, 
                          const float NewHeight) 
{ 
	  //Check validity of parameters 
	 ASSERT(!(NewLowerBound>=NewCentre)); 
	 ASSERT(!(NewUpperBound<=NewCentre)); 
	 ASSERT(!(NewHeight<=0.0)); 

	  //Store parameters 
	 //name = newName;
	 LowerBound=NewLowerBound; 
	 Centre=NewCentre; 
	 UpperBound=NewUpperBound; 
	 Height=NewHeight; 
} 



GFuzzySetLinearElement::~GFuzzySetLinearElement() 
{ 
} 



float GFuzzySetLinearElement::GetMembership(const float x) 
{ 
	float Membership=0.0; 
	if(x>Centre && x<UpperBound) 
		Membership=Height*(UpperBound-x)/(UpperBound-Centre); 
	else 
		if(x<=Centre && x>LowerBound) 
			Membership=Height*(x-LowerBound)/(Centre-LowerBound); 
	return Membership; 
} 



float GFuzzySetLinearElement::GetArea(void) 
{ 
 float Area=0.0; 
 Area+=0.5*Height*(Centre-LowerBound); 
 Area+=0.5*Height*(UpperBound-Centre); 
 return Area; 
} 



float GFuzzySetLinearElement::GetCentreOfMass(void) 
{ 
/*
	float AreaA,CentreOfMassA,AreaB,CentreOfMassB,CentreOfMass; 
 AreaA=0.5*(Centre-LowerBound); 
 AreaB=0.5*(UpperBound-Centre); 
 CentreOfMassA=0.292893*LowerBound+0.707107*Centre; 
 CentreOfMassB=0.292893*UpperBound+0.707107*Centre; 
 CentreOfMass=(AreaA*CentreOfMassA+AreaB*CentreOfMassB)/(AreaA+AreaB); 
 return CentreOfMass;
 */

	//com = centre of mass
	
	float ALowerLeftTriangle, ABlock, ALowerRightTriangle;
	float comLLT, comB, comLRT, CentreOfMass;
	float x1, x2;
	x1 = cut * (Centre - LowerBound) + LowerBound;
	x2 = UpperBound - cut * (UpperBound-Centre);


	//flaechen

	ALowerLeftTriangle = .5* (x1-LowerBound)*cut;
	ABlock			   = (x2 - x1) * cut;
	ALowerRightTriangle = .5 * (UpperBound - x2) * cut;

	A = (ALowerLeftTriangle + ABlock + ALowerRightTriangle);

	comLLT = .666 *(x1-LowerBound) + LowerBound;
	comB = .5 * (x2 - x1) + x1;
	comLRT = .333 * (UpperBound - x2) + x2;

	if (A != 0.f)
		CentreOfMass = (comLLT*ALowerLeftTriangle + comB*ABlock + comLRT*ALowerRightTriangle)/A;
	else
		CentreOfMass = 0.f;

	TRACE("CentreOfMass %g\n", CentreOfMass);
	return CentreOfMass; 
} 
	
