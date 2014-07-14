// EventOutSFFloat.java

// package
package vrml.external.field;

public class EventOutSFFloat extends EventOut
{
  // constructor
  public EventOutSFFloat (blaxxunvrml.EventOut glEventOutSFFloat)
  {
    // save gl event out
    super (glEventOutSFFloat);
  }

  // get value
  public float getValue()
  {
    return ((blaxxunvrml.EventOutSFFloat) m_bxEventOut).getValue();
  }
}
