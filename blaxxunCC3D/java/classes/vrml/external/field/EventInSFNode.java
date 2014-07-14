// EventInSFNode.java

// package
package vrml.external.field;

// imports
import vrml.external.Node;


public class EventInSFNode extends EventIn
{
  // constructor
  public EventInSFNode (blaxxunvrml.EventIn glEventInSFNode)
  {
    // set m_bxEventIn
    super (glEventInSFNode);
  }

  // set value
  public void setValue (Node value)
  {
    // pass on to glEventIn
    ((blaxxunvrml.EventInSFNode) m_bxEventIn).setValue(value.getBxNode());
  }
}
