// EventOutSFString.java

// package
package vrml.external.field;

public class EventOutSFString extends EventOut
{
  // constructor
  public EventOutSFString (blaxxunvrml.EventOut glEventOutSFString)
  {
    // save gl event out
    super (glEventOutSFString);
  }

  // get value
  public String getValue ()
  {
    return ((blaxxunvrml.EventOutSFString) m_bxEventOut).getValue ();
  }
}
