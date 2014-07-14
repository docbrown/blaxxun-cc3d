// EventInSFTime.java

// package
package vrml.external.field;

public class EventInSFTime extends EventIn
{
  // constructor
  public EventInSFTime (blaxxunvrml.EventIn glEventInSFTime)
  {
    // set m_bxEventIn
    super (glEventInSFTime);
  }

  // set value
  public void setValue (double value)
  {
    // pass on to glEventIn
    ((blaxxunvrml.EventInSFTime) m_bxEventIn).setValue (value);
  }
}
