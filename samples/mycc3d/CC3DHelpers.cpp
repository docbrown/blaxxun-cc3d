#include "stdafx.h"

#include "CC3DHelpers.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




// callback implementation 
// simply call the (overrided) virtual function
// which can do some something usefull


STDMETHODIMP CMyEventOutObserver::callback(EventOut *valueEvent, double timeStamp,IDispatch *userData) 
{ 
	 return OnCallback( valueEvent, timeStamp, userData);
}

// override this callback in derived classes 
HRESULT CMyEventOutObserver::OnCallback(EventOut *valueEvent, double timeStamp,IDispatch *userData) 
{
		
		CString valueStr; // buffer to write field value 
		TCHAR delimiter = ' ';

		const TCHAR *prefix =" Value =";

		int type;
		float tmpPos[3]; // dummy buffers for retreiving values 
		float tmpRot[4];
		float tmpVec2f[2];
		float tmpFloat;
		double tmpDouble;
		int   tmpInt;
		CString tmpStr;

		TRACE("Got something  (%p) from the browser at %g ! \n",valueEvent,timeStamp);

		CMyObserverUserData* data = ( CMyObserverUserData*) userData;

		// low level code without using Cxxx classes 

		// real code would get member data from this or data
		// to decide on processing 

			   // get the eventType 	
               valueEvent->getType(&type);


                switch (type) // dpending on field type, convert to string 
                {  
			        case SFBOOL:
                        {
                            CComQIPtr<EventOutSFBool, &IID_EventOutSFBool> eBool(valueEvent);
                            VARIANT_BOOL v;
                            eBool->getValue(&v);

                            // (int)v is received bool value
                            if ( (int)v )
                            {
                                valueStr.Format( "%sTRUE", prefix );
                            }
                            else
                            {
                                valueStr.Format( "%sFALSE", prefix );
                            }
				            break;
                        }

				        break;
			        case SFCOLOR:
                        {
					        CComQIPtr<EventOutSFColor, &IID_EventOutSFColor> value(valueEvent);
					        ASSERT(value);
					        value->getValue(tmpPos);
                            valueStr.Format( "%s%g%c%g%c%g", prefix,
                                tmpPos[0], delimiter, tmpPos[1], delimiter, tmpPos[2] );
                        }
                        break;
			        case SFFLOAT:
                        {
					        CComQIPtr<EventOutSFFloat, &IID_EventOutSFFloat> value(valueEvent);
					        ASSERT(value);
					        value->getValue(&tmpFloat);
                            valueStr.Format( "%s%e", prefix,tmpFloat ); 
                        }
                        break;
			        case SFINT32:
                        {
					        CComQIPtr<EventOutSFInt32, &IID_EventOutSFInt32> value(valueEvent);
					        ASSERT(value);
					        value->getValue(&tmpInt);
                            valueStr.Format( "%s%d", prefix,
                                tmpInt ); 
                        }
				        
                        break;
			        case SFROTATION:
                        {
				            CComQIPtr<EventOutSFRotation, &IID_EventOutSFRotation> value(valueEvent);
				            ASSERT(value);
				            value->getValue(tmpRot);
                            valueStr.Format( "%s%g%c%g%c%g%c%g", prefix,
                                tmpRot[0], delimiter, tmpRot[1], delimiter, tmpRot[2], delimiter, tmpRot[3] ); 
                        }

                        break;
			        case SFSTRING:
				        {
                            CComQIPtr<EventOutSFString, &IID_EventOutSFString> value(valueEvent);
                            ASSERT(value);

                            BSTR v=NULL;

                            value->getValue(&v);
                            ASSERT(v);

                            tmpStr = v; // convert to standard string 
							{
								// BSTR allocated by browser  
								valueStr.Format( "%s%s", prefix, (const TCHAR *) tmpStr ); 
							}
                            ::SysFreeString(v);
                        }
                        break;
			        case SFTIME:
                        {
                            CComQIPtr<EventOutSFTime, &IID_EventOutSFTime> value(valueEvent);
                            ASSERT(value);
                            value->getValue(&tmpDouble);
                            valueStr.Format( "%s%g", prefix,  tmpDouble ); 
                        }
                        break;
			        case SFVEC2F:
				        
                        {
					        CComQIPtr<EventOutSFVec2f, &IID_EventOutSFVec2f> value(valueEvent);
					        ASSERT(value);
					        value->getValue(tmpVec2f);
                            valueStr.Format( "%s%g%c%g", prefix,
                                tmpVec2f[0], delimiter, tmpVec2f[1] );
                        }
                        break;
			        case SFVEC3F:
                        {
					        CComQIPtr<EventOutSFVec3f, &IID_EventOutSFVec3f> value(valueEvent);
					        ASSERT(value);
					        value->getValue(tmpPos);
                            valueStr.Format( "%s%g%c%g%c%g", prefix,
                                tmpPos[0], delimiter, tmpPos[1], delimiter, tmpPos[2] ); 
                        }
                        break;
					default :
						valueStr.Format("Event Type %d  not yet supported \n",type);
						// could use toString(BSTR) method 
						break;

				}
		
		TRACE("%s \n", (const TCHAR *) valueStr);
		return S_OK;
}




