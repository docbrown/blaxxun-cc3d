// EventOutSFInt32.java

// package
package vrml.external.field;

public class EventOutSFInt32 extends EventOut
{
  // constructor
  public EventOutSFInt32 (blaxxunvrml.EventOut glEventOutSFInt32)
  {
    // save gl event out
    super (glEventOutSFInt32);
  }

  // get value
  public int getValue ()
  {
    return ((blaxxunvrml.EventOutSFInt32) m_bxEventOut).getValue ();
  }
}
