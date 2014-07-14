// EventOutMFNode.java

// package
package vrml.external.field;

// imports
import vrml.external.Node;

public class EventOutMFNode extends EventOutMField
{
  // constructor
  public EventOutMFNode (blaxxunvrml.EventOut glEventOutMFNode)
  {
    // save gl event out
    super (glEventOutMFNode);
  }

  // get value
  public Node[] getValue ()
  {
    Node [] nodes;
    int size;
    int i;

	blaxxunvrml.EventOutMFNode me;
	me = (blaxxunvrml.EventOutMFNode) m_bxEventOut;

    // allocate array
    size = me.getSize ();
    nodes = new Node [size];

    // create vrml nodes from gl nodes
    for (i = 0; i < size; i ++)
    {
      nodes [i] = new Node(me.get1Value (i));
    }

    return nodes;
  }

  public Node  get1Value(int index)
  {
    return new Node(((blaxxunvrml.EventOutMFNode) m_bxEventOut).get1Value(index));
  }

}
