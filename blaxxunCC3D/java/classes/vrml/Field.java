// Field.java

// package
package vrml;


public abstract class Field 
					//implements Cloneable
{
  // GLView field 
  protected blaxxunvrml.Field m_bxField;

  // constructor
  public Field (blaxxunvrml.Field glField)
  {
    m_bxField = glField;
  }

  public static Field newInstance(blaxxunvrml.Field glField) 
  {
	  // to do:
	  return null;
  }

  public static Field newInstance(blaxxunvrml.EventIn glField) 
  {
	  // problem, some fields are write only, you can't get
	  //the last value e.g. Group.addChildren
	  // to do:
	  return null;
  }



/*

  // create a new vrml field from  given glview field 
  public static Field newField(blaxxunvrml.Field glEventOut)
  {
    EventOut eventOut;

    // create vrml event based on type
    switch (glEventOut.getType ())
    {

	// MF Fields 
    case FieldTypes.MFCOLOR:
      eventOut = new EventOutMFColor(glEventOut);
      break;

    case FieldTypes.MFFLOAT:
      eventOut = new EventOutMFFloat (glEventOut);
      break;
    case FieldTypes.MFINT32:
      eventOut = new EventOutMFInt32 (glEventOut);
      break;
    case FieldTypes.MFNODE:
      eventOut = new EventOutMFNode (glEventOut);
      break;

    case FieldTypes.MFROTATION:
      eventOut = new EventOutMFRotation (glEventOut);
      break;

    case FieldTypes.MFSTRING:
      eventOut = new EventOutMFString (glEventOut);
      break;

    case FieldTypes.MFVEC2F:
      eventOut = new EventOutMFVec2f (glEventOut);
      break;

    case FieldTypes.MFVEC3F:
      eventOut = new EventOutMFVec3f (glEventOut);
      break;

   // SF Fields 

    case FieldTypes.SFBOOL:
      eventOut = new EventOutSFBool(glEventOut);
      break;

    case FieldTypes.SFCOLOR:
      eventOut = new EventOutSFColor(glEventOut);
      break;

    case FieldTypes.SFFLOAT:
      eventOut = new EventOutSFFloat(glEventOut);
      break;

    case FieldTypes.SFIMAGE:
      eventOut = new EventOutSFImage(glEventOut);
      break;

    case FieldTypes.SFINT32:
      eventOut = new EventOutSFInt32(glEventOut);
      break;

    case FieldTypes.SFNODE:
      eventOut = new EventOutSFNode(glEventOut);
      break;

    case FieldTypes.SFROTATION:
      eventOut = new EventOutSFRotation (glEventOut);
      break;


    case FieldTypes.SFSTRING:
      eventOut = new EventOutSFString(glEventOut);
      break;

    case FieldTypes.SFTIME:
      eventOut = new EventOutSFTime(glEventOut);
      break;

    case FieldTypes.SFVEC2F:
      eventOut = new EventOutSFVec2f(glEventOut);
      break;

    case FieldTypes.SFVEC3F:
      eventOut = new EventOutSFVec3f (glEventOut);
      break;


    default:
	  System.out.println ("EventOut: found no class for type = " + glEventOut.getType ());
      eventOut = null;
    }

    return eventOut;
  }
*/

  // return type of field 
  public int getType()
  {
    return m_bxField.getType();
  }

}
