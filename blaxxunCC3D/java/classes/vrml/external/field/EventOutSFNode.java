// EventOutSFNode.java

// package
package vrml.external.field;

// imports
import vrml.external.Node;


public class EventOutSFNode extends EventOut
{
  // constructor
  public EventOutSFNode (blaxxunvrml.EventOut glEventOutSFNode)
  {
    // save gl event out
    super (glEventOutSFNode);
  }

  // get value
  public Node getValue ()
  {
	blaxxunvrml.Node v;

	v = ((blaxxunvrml.EventOutSFNode) m_bxEventOut).getValue();
	
	if (v == null)
		return null;
    else 
		return new Node (v);

  }
}
