// EventInSFString.java

// package
package vrml.external.field;

public class EventInSFString extends EventIn
{
  // constructor
  public EventInSFString (blaxxunvrml.EventIn glEventInSFString)
  {
    // set m_bxEventIn
    super (glEventInSFString);
  }

  // set value
  public void setValue (String value)
  {
    // pass on to glEventIn
    ((blaxxunvrml.EventInSFString) m_bxEventIn).setValue (value);
  }
}
