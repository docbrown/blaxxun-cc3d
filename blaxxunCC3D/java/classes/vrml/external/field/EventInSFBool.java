// EventInSFBool.java

// package
package vrml.external.field;

public class EventInSFBool extends EventIn
{
  // constructor
  public EventInSFBool (blaxxunvrml.EventIn glEventInSFBool)
  {
    // set m_bxEventIn
    super (glEventInSFBool);
  }

  // set value
  public void setValue (boolean value)
  {
    // pass on to glEventIn
    ((blaxxunvrml.EventInSFBool) m_bxEventIn).setValue (value);
  }
}
