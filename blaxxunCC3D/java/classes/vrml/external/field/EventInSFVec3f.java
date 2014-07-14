// EventInSFVec3f.java

// package
package vrml.external.field;

public class EventInSFVec3f extends EventIn
{
  // constructor
  public EventInSFVec3f (blaxxunvrml.EventIn glEventInSFVec3f)
  {
    // set m_bxEventIn
    super (glEventInSFVec3f);
  }

  // set value
  public void setValue (float [] value)
  {
    // pass on to glEventIn
    ((blaxxunvrml.EventInSFVec3f) m_bxEventIn).setValue (value);
  }
}
