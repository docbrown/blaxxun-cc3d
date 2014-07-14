// EventIn.java

// package
package vrml.external.field;

import vrml.external.field.FieldTypes;


public class EventIn
{
  // gl event in
  protected blaxxunvrml.EventIn m_bxEventIn;

  // constructor
  public EventIn (blaxxunvrml.EventIn glEventIn)
  {
    m_bxEventIn = glEventIn;
  }

  // create a new vrml event in given a glview event in
  public static EventIn newEventIn (blaxxunvrml.EventIn glEventIn)
  {
    EventIn eventIn;

    // create vrml event based on type
    switch (glEventIn.getType ()) // sort by alphabet 
    {

	// MF Fields 
    case FieldTypes.MFCOLOR:
      eventIn = new EventInMFColor(glEventIn);
      break;

    case FieldTypes.MFFLOAT:
      eventIn = new EventInMFFloat (glEventIn);
      break;
    case FieldTypes.MFINT32:
      eventIn = new EventInMFInt32 (glEventIn);
      break;
    case FieldTypes.MFNODE:
      eventIn = new EventInMFNode (glEventIn);
      break;

    case FieldTypes.MFROTATION:
      eventIn = new EventInMFRotation (glEventIn);
      break;

    case FieldTypes.MFSTRING:
      eventIn = new EventInMFString (glEventIn);
      break;

    case FieldTypes.MFVEC2F:
      eventIn = new EventInMFVec2f (glEventIn);
      break;

    case FieldTypes.MFVEC3F:
      eventIn = new EventInMFVec3f (glEventIn);
      break;

   // SF Fields 

    case FieldTypes.SFBOOL:
      eventIn = new EventInSFBool (glEventIn);
      break;

    case FieldTypes.SFCOLOR:
      eventIn = new EventInSFColor (glEventIn);
      break;

    case FieldTypes.SFFLOAT:
      eventIn = new EventInSFFloat (glEventIn);
      break;

    case FieldTypes.SFIMAGE:
      eventIn = new EventInSFImage (glEventIn);
      break;

    case FieldTypes.SFINT32:
      eventIn = new EventInSFInt32 (glEventIn);
      break;

    case FieldTypes.SFNODE:
      eventIn = new EventInSFNode (glEventIn);
      break;

    case FieldTypes.SFROTATION:
      eventIn = new EventInSFRotation (glEventIn);
      break;

    case FieldTypes.SFSTRING:
      eventIn = new EventInSFString (glEventIn);
      break;

    case FieldTypes.SFTIME:
      eventIn = new EventInSFTime (glEventIn);
      break;

    case FieldTypes.SFVEC2F:
      eventIn = new EventInSFVec2f (glEventIn);
      break;

    case FieldTypes.SFVEC3F:
      eventIn = new EventInSFVec3f (glEventIn);
      break;

    default:
	  System.out.println ("EventIn::newEventIn found no event in! type = " + glEventIn.getType ());
      eventIn = null;
	  break;
    }

    return eventIn;
  }

  // return type of event in
  public int getType()
  {
    return m_bxEventIn.getType ();
  }

}
