// EventOutSFVec2f.java

// package
package vrml.external.field;

public class EventOutSFVec2f extends EventOut
{
  // constructor
  public EventOutSFVec2f (blaxxunvrml.EventOut glEventOutSFVec2f)
  {
    // save gl event out
    super (glEventOutSFVec2f);
  }

  // get value
  public float [] getValue ()
  {
    float [] value = new float [2];

    // pass on to glEventOut
    ((blaxxunvrml.EventOutSFVec2f) m_bxEventOut).getValue (value);

    return value;
  }
}
