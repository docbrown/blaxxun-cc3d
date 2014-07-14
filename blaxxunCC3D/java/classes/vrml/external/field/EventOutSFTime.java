// EventOutSFTime.java

// package
package vrml.external.field;

public class EventOutSFTime extends EventOut
{
  // constructor
  public EventOutSFTime (blaxxunvrml.EventOut glEventOutSFTime)
  {
    // save gl event out
    super (glEventOutSFTime);
  }

  // get value
  public double getValue ()
  {
    return ((blaxxunvrml.EventOutSFTime) m_bxEventOut).getValue ();
  }
}
