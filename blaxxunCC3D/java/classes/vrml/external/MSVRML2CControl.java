// MSVRML2CControl.java
//

// package
package vrml.external;

// imports
//import blaxxuncc3d.*;
import java.applet.Applet;

/** 
 * VRML External Interface Helper class </P>
 * Static class for associating control handles with applets 
 * so that Browser.getBrowser(Applet) works
 * this class is for compatibility with existing IE-EAI MSMSVRML2CControl applications
 * and forwards all calls to blaxxunCC3DControl
 * 
 */ 
	 

public final class MSVRML2CControl
{
  static public void SetControl (Object control, Applet applet)
  {
    blaxxunCC3DControl.SetControl(control,applet);
  }
  
  static public void SetControl (Object control, Applet applet, String framename, int index)
  {
    blaxxunCC3DControl.SetControl(control,applet,framename,index);
  }
  
  static public blaxxuncc3d.IblaxxunCC3D GetControl (Applet applet)
  {						  
	
	return blaxxunCC3DControl.GetControl(applet);

  }
  
  static public blaxxuncc3d.IblaxxunCC3D GetControl (Applet applet, String framename, int index)
  {
	return blaxxunCC3DControl.GetControl(applet,framename,index);
  }
  
  static public void FreeControls ()
  {
	blaxxunCC3DControl.FreeControls();
  }
 

}
