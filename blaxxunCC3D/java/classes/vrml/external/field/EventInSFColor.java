// EventInSFColor.java

// package
package vrml.external.field;

public class EventInSFColor extends EventIn
{
  // constructor
  public EventInSFColor (blaxxunvrml.EventIn glEventInSFColor)
  {
    // set m_bxEventIn
    super (glEventInSFColor);
  }

  // set value
  public void setValue (float [] value)
  {
    // pass on to glEventIn
    ((blaxxunvrml.EventInSFColor) m_bxEventIn).setValue (value);
  }
}
