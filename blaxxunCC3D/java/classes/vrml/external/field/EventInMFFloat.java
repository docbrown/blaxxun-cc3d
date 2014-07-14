// EventInMFFloat.java

// package
package vrml.external.field;

// imports
import vrml.external.Node;

public class EventInMFFloat extends EventIn
{
  // constructor
  public EventInMFFloat (blaxxunvrml.EventIn glEventInMFFloat)
  {
    // set m_bxEventIn
    super (glEventInMFFloat);
  }


  // set value
  public void setValue (float [] value)
  {
    ((blaxxunvrml.EventInMFFloat) m_bxEventIn).setValue (value.length,value);
  }

  // set value
  public void set1Value (int i, float value)
  {
    // pass on to gl setValue
    ((blaxxunvrml.EventInMFFloat) m_bxEventIn).set1Value (i, value);
  }

}
