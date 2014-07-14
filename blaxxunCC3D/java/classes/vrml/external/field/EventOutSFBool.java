// EventOutSFBool.java

// package
package vrml.external.field;

public class EventOutSFBool extends EventOut
{
  // constructor
  public EventOutSFBool (blaxxunvrml.EventOut glEventOutSFBool)
  {
    // save gl event out
    super (glEventOutSFBool);
  }

  // get value
  public boolean getValue ()
  {
    return ((blaxxunvrml.EventOutSFBool) m_bxEventOut).getValue ();
  }
}
