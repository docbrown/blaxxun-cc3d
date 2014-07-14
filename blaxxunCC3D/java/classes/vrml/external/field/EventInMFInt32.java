// EventInMFInt32.java

// package
package vrml.external.field;

// imports
import vrml.external.Node;

public class EventInMFInt32 extends EventIn
{
  // constructor
  public EventInMFInt32 (blaxxunvrml.EventIn glEventInMFInt32)
  {
    // set m_bxEventIn
    super (glEventInMFInt32);
  }


  // set value
  public void setValue (int [] value)
  {
    ((blaxxunvrml.EventInMFInt32) m_bxEventIn).setValue (value.length,value);
  }

  // set value
  public void set1Value (int i, int value)
  {
    // pass on to gl setValue
    ((blaxxunvrml.EventInMFInt32) m_bxEventIn).set1Value (i, value);
  }

}
