// EventOutSFVec3f.java

// package
package vrml.external.field;

public class EventOutSFVec3f extends EventOut
{
  // constructor
  public EventOutSFVec3f (blaxxunvrml.EventOut glEventOutSFVec3f)
  {
    // save gl event out
    super (glEventOutSFVec3f);
  }

  // get value
  public float [] getValue ()
  {
    float [] value = new float [3];

    // pass on to glEventOut
    ((blaxxunvrml.EventOutSFVec3f) m_bxEventOut).getValue (value);

    return value;
  }
}
