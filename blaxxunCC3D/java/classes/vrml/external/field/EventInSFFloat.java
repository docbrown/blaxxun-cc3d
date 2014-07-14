// EventInSFFloat.java

// package
package vrml.external.field;

public class EventInSFFloat extends EventIn
{
  // constructor
  public EventInSFFloat (blaxxunvrml.EventIn glEventInSFFloat)
  {
    // set m_bxEventIn
    super (glEventInSFFloat);
  }

  // set value
  public void setValue (float value)
  {
    // pass on to glEventIn
    ((blaxxunvrml.EventInSFFloat) m_bxEventIn).setValue (value);
  }
}
