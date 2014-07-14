// EventOutSFRotation.java

// package
package vrml.external.field;

public class EventOutSFRotation extends EventOut
{
  // constructor
  public EventOutSFRotation (blaxxunvrml.EventOut glEventOutSFRotation)
  {
    // set m_bxEventOut
    super (glEventOutSFRotation);
  }

  // get value
  public float [] getValue ()
  {
    float [] value = new float [4];

    // pass on to glEventOut
    ((blaxxunvrml.EventOutSFRotation) m_bxEventOut).getValue (value);

    return value;
  }
}
