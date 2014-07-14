// EventOut.java

// package
package vrml.external.field;

public class EventOut implements blaxxunvrml.EventOutObserver
{
  // gl event in
  protected blaxxunvrml.EventOut m_bxEventOut;

  // saved observer
  protected EventOutObserver m_observer;

  // constructor
  public EventOut (blaxxunvrml.EventOut glEventOut)
  {
    m_bxEventOut = glEventOut;
  }

  // create a new vrml event in given a glview event in
  public static EventOut newEventOut (blaxxunvrml.EventOut glEventOut)
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

  // return type of event out
  public int getType ()
  {
    return m_bxEventOut.getType();
  }

  // set observer for this event out
  public void advise(EventOutObserver observer, Object userData)
  {
    //System.out.println ("EventOut advise " + this);

    // save observer
    m_observer = observer;

    // pass to gl
    m_bxEventOut.advise (this, userData);
  }

  public void finalize ()
  {
    //System.out.println ("EventOut finalize " + this);
    if (m_observer != null)
    {
      m_bxEventOut.unAdvise (this);
      m_observer = null;
    }
  }

  // observer callback
  public void callback (blaxxunvrml.EventOut value, double timestamp, Object userData)
  {
    // now call initial observer
    m_observer.callback (this, timestamp, userData);
  }
}
