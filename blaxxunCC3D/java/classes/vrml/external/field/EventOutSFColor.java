// EventOutSFColor.java

// package
package vrml.external.field;

public class EventOutSFColor extends EventOut
{
  // constructor
  public EventOutSFColor (blaxxunvrml.EventOut glEventOutSFColor)
  {
    // save gl event out
    super (glEventOutSFColor);
  }

  // get value
  public float [] getValue ()
  {
    float [] value = new float [3];

    // pass on to glEventOut
    ((blaxxunvrml.EventOutSFColor) m_bxEventOut).getValue (value);

    return value;
  }
}
