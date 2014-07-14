// EventInSFInt32.java

// package
package vrml.external.field;

public class EventInSFInt32 extends EventIn
{
  // constructor
  public EventInSFInt32 (blaxxunvrml.EventIn glEventInSFInt32)
  {
    // set m_bxEventIn
    super (glEventInSFInt32);
  }

  // set value
  public void setValue (int value)
  {
    // pass on to glEventIn
    ((blaxxunvrml.EventInSFInt32) m_bxEventIn).setValue (value);
  }
}
