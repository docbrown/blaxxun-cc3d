// EventInSFVec2f.java

// package
package vrml.external.field;

public class EventInSFVec2f extends EventIn
{
  // constructor
  public EventInSFVec2f (blaxxunvrml.EventIn glEventInSFVec2f)
  {
    // set m_bxEventIn
    super (glEventInSFVec2f);
  }

  // set value
  public void setValue (float [] value)
  {
    // pass on to glEventIn
    ((blaxxunvrml.EventInSFVec2f) m_bxEventIn).setValue (value);
  }
}
