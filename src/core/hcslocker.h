
  ////////////////////////////////////////
 // THERE IS NO ".cpp" FILE NECESSARY. //
////////////////////////////////////////

#ifndef _HCSLOCKER_H
#define _HCSLOCKER_H




//
// This class makes it very easy to lock and unlock a CRITICAL_SECTION
// object, and it protects the programmer from forgetting to properly
// call the LeaveCriticalSection(.) in all circumstances.
// 
// The class consists only of a constructor and a destructor.
// The constructor of the class calles EnterCriticalSection(.)
// and the destructor calles LeaveCriticalSection(.)
// Since the compiler automatically iserts the destructor's code 
// everywhere the block in which the class is instantiated is left,
// leaving the critical section can not be forgotton.
// And since this class is bound to the C++ block structure, it
// is clear to a human reader of code where a thread is inside
// a critical section. I. e. from the `{´ with the instantiation
// of thi
// till the closing bracket `}´ of the enclosing block.
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



class HCSlocker
{

public:

    HCSlocker(CRITICAL_SECTION *pCS)    {  mpCS= pCS;   EnterCriticalSection(mpCS);  };
   ~HCSlocker()                         {               LeaveCriticalSection(mpCS);  };


protected: 
    CRITICAL_SECTION* mpCS;
};




#endif

