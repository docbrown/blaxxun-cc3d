// Node.java

// package
package vrml.node;

// imports
import vrml.*;
import vrml.field.*;


/** 
 * VRML Script Interface class for GLView Node</P>
 * <pre> 
 * </pre> 
 * 
 */ 



public class Node  extends BaseNode
{

  // constructor
  public Node (blaxxunvrml.Node bxNode)
  {
     super(bxNode);
  }


   // Get the Browser that this node is contained in.
   //public Browser getBrowser();  

   // get handle to an EventIn of this node
   public Field getEventIn (String name)
       throws InvalidEventInException
   {
    blaxxunvrml.EventIn glEventIn;

    // get gl event in, return vrml event in
    glEventIn = (blaxxunvrml.EventIn) m_bxNode.getEventIn (name);
	if (glEventIn == null) throw new InvalidEventInException("No such eventIn "+name);
    //return EventIn.newEventIn (glEventIn);
	return Field.newInstance(glEventIn);
   }

  // get handle to an EventOut of this node
  public ConstField  getEventOut (String name)
       throws InvalidEventOutException
  {
    blaxxunvrml.EventOut glEventOut;
	// get gl event out, return vrml event out
	glEventOut = m_bxNode.getEventOut(name);
	if (glEventOut == null) throw new InvalidEventOutException("No such eventOut "+name);
	return ConstField.newInstance(glEventOut);
  }

   // get handle to an EventIn of this node
   public Field  getExposedField (String name)
       throws InvalidExposedFieldException
   {
    blaxxunvrml.Field glField;
    // get gl field, return vrml field
    glField = (blaxxunvrml.Field) m_bxNode.getField(name);
	if (glField == null) throw new InvalidExposedFieldException("No such exposedField "+name);
	return Field.newInstance(glField);
   }
  
  // public String toString();   // This overrides a method in Object


}
  
