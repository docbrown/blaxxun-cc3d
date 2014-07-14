// EventInMFNode.java

// package
package vrml.external.field;

// imports
import vrml.external.Node;

public class EventInMFNode extends EventIn
{
  // constructor
  public EventInMFNode (blaxxunvrml.EventIn glEventInMFNode)
  {
    // set m_bxEventIn
    super (glEventInMFNode);
  }

  // set value
  public void setValue (Node [] value)
  {
    int i;

    // get glnodes from vrml nodes
    for (i = 0; i < value.length; i ++)
    {
      ((blaxxunvrml.EventInMFNode) m_bxEventIn).set1Value (i, value [i].getBxNode ());
    }
  }

  // set value
  public void set1Value (int i, Node value)
  {
    // pass on to gl setValue
    ((blaxxunvrml.EventInMFNode) m_bxEventIn).set1Value (i, value.getBxNode ());
  }
}
