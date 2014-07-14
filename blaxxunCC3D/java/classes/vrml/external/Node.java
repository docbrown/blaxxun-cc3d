// Node.java

// package
package vrml.external;

// imports
import vrml.external.field.EventIn;
import vrml.external.field.EventOut;
import vrml.external.exception.InvalidEventInException;
import vrml.external.exception.InvalidEventOutException;
import vrml.external.exception.*;


/** 
 * VRML External Java Interface to a VRML node</P>
 * For example: 
 * <pre> 
 *    vrml.external.node n = browser.getNode("ROOT");
 * </pre> 
 * 
 */ 

public class Node
{
  // handle to blaxxunVRML node (Internal) 
  private blaxxunvrml.Node m_bxNode;

  // constructor (Internal) 
  public Node (blaxxunvrml.Node bxNode)
  {
    m_bxNode = bxNode;
  }

  // return the blaxxunVRML node (internal) 
  public blaxxunvrml.Node getBxNode ()
  {
    return m_bxNode;
  }

  /** 
  * Get a string specifying the type of this node. May return the
  * name of a PROTO, or the class name
  * @return           class name e.g. "Transform"
  */ 

  public String getType()
  {
	  return m_bxNode.getType();
  }	  

  /**
  * get handle to an EventIn of this node
  * @param  name	name of the requested EventIn e.g. "set_translation" for a Transform node
  * @return           handle of eventIn, can be casted to specific EventIn type
  **/
  public EventIn getEventIn (String name)
       throws InvalidEventInException
  {
    blaxxunvrml.EventIn glEventIn;

    // get gl event in, return vrml event in
    glEventIn = (blaxxunvrml.EventIn) m_bxNode.getEventIn (name);
	if (glEventIn == null) throw new InvalidEventInException("No such EventIn "+name);
    return EventIn.newEventIn (glEventIn);
  }

  /**
  * get handle to an EventOut of this node
  * @param  name	name of the requested EventOut e.g. "translation_changed" for a Transform node
  * @return           handle of eventOut, can be casted to specific EventOut type
  **/
  public EventOut getEventOut (String name)
       throws InvalidEventOutException
  {
    blaxxunvrml.EventOut glEventOut;
	// get gl event out, return vrml event out
	glEventOut = m_bxNode.getEventOut (name);
	if (glEventOut == null) throw new InvalidEventOutException("No such EventOut "+name);
    return EventOut.newEventOut (glEventOut);
  }


  /**
  * get node name of this node (blaxxunCC3d)
  * @return name eg "My-Node" or "" if no name set 
  **/

  public String getName()
  {
	  return m_bxNode.getName(); // fixed 26.10.99 HG
  }	  

  /**
  * set node name of this node  (blaxxunCC3d)
  * @param  newName	 new name for the node 
  * browser.getNode(newName) may or may not return this node
  * use browser.setNodeName(node,name) instead to enshure this 
  **/
  public void setName(String newName)
  {
	  if (newName == null)
		  m_bxNode.setName("");
	  else m_bxNode.setName(newName);
  }	  

  /**
  * get number of interface elements (eventIn/Out/exposedFields/fields of this node (blaxxunCC3d)
  * @return number of interface elements
  **/

  public int getNumInterfaceElements()
  {
	  return m_bxNode.getNumInterfaceElements();
  }	  

  /**
  * query event type information of a specific interface element (blaxxunCC3d)
  * @param  index  index of element (0 <= index < getNumInterfaceElements)
  * @param  eventType  the eventType 
  * @param  fieldType  the field type 
  * @param  name  the name of the event / field
  **/

  public void getInterfaceElement(int index, int eventType[], int fieldType[], java.lang.String name[])
  {
	  m_bxNode.getInterfaceElement(index,eventType ,fieldType,name);
  }		  

  /**
  * query the node this node evalutes to, (blaxxunCC3d)
  * for Nodes created as a instance of a PROTO/EXTERNPROTO it would return the top-level node of the PROTO instance 
  * for non-Proto nodes it evalutes to this
  * @return the value node 
  */
 
  public Node getValueNode() 
  {
		blaxxunvrml.Node bxNode = m_bxNode.getValueNode();
		if (bxNode == null) return null;
		if (bxNode == m_bxNode) return this;
		return new Node(bxNode);
  }

}
  
