// blaxxunCC3DControl.java
//

// package
package vrml.external;

// imports
//import blaxxuncc3d.*;
import java.applet.Applet;
import java.util.Hashtable;

/** 
 * VRML External Interface Helper class </P>
 * Static class for associating control handles with applets 
 * so that Browser.getBrowser(Applet) works
 * 
 */ 


public final class blaxxunCC3DControl
{
  static public void SetControl (Object control, Applet applet)
  {
    hashTable.put (applet, (blaxxuncc3d.IblaxxunCC3D) control);
  }
  
  static public void SetControl (Object control, Applet applet, String framename, int index)
  {
    // impliment later
  }
  
  static public blaxxuncc3d.IblaxxunCC3D GetControl (Applet applet)
  {						  
	
	Object x = hashTable.get (applet);

	// check if has been set
	
	if (x != null)
		return (blaxxuncc3d.IblaxxunCC3D) x;
	else return null;

  }
  
  static public blaxxuncc3d.IblaxxunCC3D GetControl (Applet applet, String framename, int index)
  {
    return GetControl(applet);
    // impliment fully later
  }
  
  static public void FreeControls ()
  {
    hashTable.clear ();
  }
  
  private static Hashtable hashTable = new Hashtable();

}
