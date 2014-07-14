// EventInSFRotation.java

// package
package vrml.external.field;

public class EventInSFRotation extends EventIn
{
  // constructor
  public EventInSFRotation (blaxxunvrml.EventIn glEventInSFRotation)
  {
    // set m_bxEventIn
    super (glEventInSFRotation);
  }

  // set value
  public void setValue (float [] value)
  {
    // pass on to glEventIn
    ((blaxxunvrml.EventInSFRotation) m_bxEventIn).setValue (value);
  }
}
