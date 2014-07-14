
  ////////////////////////////////////////////
 // THERE IS NO "easy.cpp" FILE NECESSARY. //
////////////////////////////////////////////

#ifndef _EASYCS_H
#define _EASYCS_H




//
// This class makes it very easy to access a critical section
// and it protects the programmer from forgetting to properly
// leave a critical secion.
// 
// The constructor of the class calles EnterCriticalSection(.)
// and the destructor calles LeaveCriticalSection(.)
// Since the compiler automatically iserts the destructor's code 
// everywhere he block in which the class is instantiated is left,
// leaving the critical section can not be forgotton.
// And since this class is bound to the C++ block structure, it
// is clear to the reader of a code where the critical section
// is.
//
// How to use CEasyCS:
// This is an example. Note, that the return statement doesn't need
// an explicitely statement to leave the critical section.
// 
//
//
//
//CDoDo gSomeResource;
//CRITICAL_SECTION gCS;
//...
//InitializeCriticalSection(&gCS);
//...
//
//
//void dings()
//{
//    int i= 5+3;
//    
//    do_something(&i);
//
//    {   CEasyCS CS(&gCS);  // This is the beginning of the critical section.
//      
//        gSomeResource.DoSomething;
//
//        if(gSomeResource.CountMe() < 10)
//            return;        // This leaves the critical section.
//
//        gSomeResource.ProcessItems(10);
//
//    }                      // This is the end of the critical section.
//
//
//    do_something_more(i);
//
//}
//
//
//



class CEasyCS
{

public:

    CEasyCS(CRITICAL_SECTION *pCS)    {  mpCS= pCS;   EnterCriticalSection(mpCS);  };
    ~CEasyCS()                        {               LeaveCriticalSection(mpCS);  };


protected: 
    CRITICAL_SECTION* mpCS;
};


#endif

