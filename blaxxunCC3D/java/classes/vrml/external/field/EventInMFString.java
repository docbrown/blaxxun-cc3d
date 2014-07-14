// EventInMFString.java

// package
package vrml.external.field;

// imports
import vrml.external.Node;

public class EventInMFString extends EventIn
{
  // constructor
  public EventInMFString(blaxxunvrml.EventIn glEventInMFString)
  {
    // set m_bxEventIn
    super (glEventInMFString);
  }


  // set value
  public void setValue (String [] value)
  {
    ((blaxxunvrml.EventInMFString) m_bxEventIn).setValue (value.length,value);
  }

  // set value
  public void set1Value (int i, String value)
  {
    // pass on to gl setValue
    ((blaxxunvrml.EventInMFString) m_bxEventIn).set1Value (i, value);
  }

}
