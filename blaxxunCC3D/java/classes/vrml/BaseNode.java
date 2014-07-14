// BaseNode.java

// package
package vrml;

// imports

/** 
 * VRML Script Interface class for GLView Node </P>
 * <pre> 
 * </pre> 
 * 
 */ 



public class BaseNode
{

  // glview node
  protected blaxxunvrml.Node m_bxNode;

  // constructor
  public BaseNode (blaxxunvrml.Node bxNode)
  {
		m_bxNode = bxNode;
  }

  // return the glview node
  public blaxxunvrml.Node getBxNode()
  {
		return m_bxNode;
  }
  

   // Returns the type of the node.  If the node is a prototype
   // it returns the name of the prototype.

   public String getType() {
		return m_bxNode.getType();
   }

   // Get the Browser that this node is contained in.
   //public Browser getBrowser();  

}
  
