/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.11 $
   Date:        $Date: 2000/01/10 17:46:24 $
   Modification History:
   $Log: Browser.java,v $
   Revision 1.11  2000/01/10 17:46:24  kristof
   catch exception in getBrowser

   Revision 1.10  1998/10/22 16:04:45  kristof
   *** empty log message ***

   Revision 1.9  1998/09/16 10:22:00  kristof
   *** empty log message ***

   Revision 1.8  1998/07/15 13:58:52  kristof
   *** empty log message ***

   Revision 1.6  1998/03/10 18:22:23  kristof
   *** empty log message ***

   Revision 1.5  1998/03/10 18:08:51  kristof
   *** empty log message ***

   Revision 1.4  1998/03/10 17:53:07  bernd
   replaceWorld analog IE implementation

   Revision 1.3  1997/12/17 12:09:29  kristof
   coonnect chaged

   Revision 1.1  1997/12/02 12:10:12  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

// package
package vrml.external;

// imports
import vrml.external.field.EventOut;
import vrml.external.field.EventIn;
import vrml.external.field.EventInMFNode;
import vrml.external.field.EventOutObserver;
import vrml.external.exception.*;
import netscape.javascript.JSObject;

public class Browser extends netscape.plugin.Plugin
{
    // opcodes for native functions to reduce number
    private  final static int OP_NAME        = 1;
    private  final static int OP_VERSION     = 2;
    private  final static int OP_WORLDURL    = 3;
    private  final static int OP_VIEWERMODE  = 4;
    private  final static int OP_VIEWPOINT   = 5;
    private  final static int OP_COLLISION   = 6;
    private  final static int OP_HEADLIGHT   = 7;
    private  final static int OP_ANIMATE     = 8;
    private  final static int OP_DESCRIPTION = 9;
    private  final static int OP_NEXTVIEW    = 11;
    private  final static int OP_PREVVIEW    = 12;
    private  final static int OP_BEGINUPDATE = 13;
    private  final static int OP_ENDUPDATE   = 14;
    private  final static int OP_AVATARURL   = 15;
    private  final static int OP_3RDPERSON   = 16;
    private  final static int OP_PLAYSOUND   = 17;
    private  final static int OP_PRINT       = 18;
    private  final static int OP_SAVEVIEW    = 20;
    private  final static int OP_ISLOADED    = 21;

    // to keep handles to observer eventouts in Java
    private java.util.Vector obsEvents = null;

    public static Browser theBrowser = null;

    // constructor, doesn't make much sense if not connected to plugin
    public Browser ()
    {
    }

    public void setBrowser()
    {
        theBrowser = this;
    }

    public void init()
    {
//System.out.println("browser initialized "+this);
        theBrowser = this;
    }

    public void destroy()
    {
//System.out.println("browser destroyed "+this);
        theBrowser = null;
    }

    /**
    * get the CC3D browser specified by IUnkonwon interface, used for IE compatibility
    * @param Object object
    */
    static public Browser getBrowser(Object object)
    {
//System.out.println("getBrowser with object not implemented in Netscape ");
        return null;
    }

    /**
    * get the CC3D browser running in the same frame as the applet
    * @param applet
    */
    static public Browser getBrowser(java.applet.Applet applet)
    {
//System.out.println("applet = "+applet);
      JSObject js;
      Object obj;
      try 
      {
        js = JSObject.getWindow(applet);
        if (js != null)
        {
            obj = js.getMember("document");
            if (!(obj instanceof JSObject))
            {
                //System.out.println("No document object , "+obj);
                return theBrowser;
            }
            js = (JSObject)obj;
            if (js != null)
            {
                obj = js.getMember("embeds");
                if (!(obj instanceof JSObject))
                {
                    //System.out.println("No embeds object , "+obj);
                    return theBrowser;
                }
                js = (JSObject)obj;
                if (js != null)
                {
                    Object browser = (Object)js.getSlot(0);
                    if (browser instanceof Browser)
                        return (Browser)browser;
                    else
                    {
                        //System.out.println("No browser at embeds0 , "+browser);
                        return theBrowser;
                    }
                }
            }
        }
      }
      catch (Exception e)
      {
        //System.out.println("No browser object found, "+e);
      }
      return theBrowser;
    }

    /**
    * get the CC3D browser with the specified frame name and embed index<br>
    * the CC3D frame MUST be part of the frameset of the parent of the applet
    * @param applet
    * @param frame name
    * @param embed number
    */
    static public Browser getBrowser(java.applet.Applet applet, String frameName, int index)
    {
//System.out.println("frame = "+frameName+ "index ="+index);
      try 
      {
        JSObject js = JSObject.getWindow(applet);
        if (js == null)
            return null;
        if (frameName == null || frameName.length() == 0)
        {
            js = (JSObject)js.getMember("document");
            if (js == null)
                return theBrowser;
            js = (JSObject)js.getMember("embeds");
//System.out.println("js = "+js);
        }
        else
        {
            js = (JSObject)js.getMember("parent");
            if (js == null)
                return theBrowser;
            js = (JSObject)js.getMember("frames");
            if (js == null)
                return theBrowser;
            js = (JSObject)js.getMember(frameName);
            if (js == null)
                return theBrowser;
            js = (JSObject)js.getMember("document");
            if (js == null)
                return theBrowser;
            js = (JSObject)js.getMember("embeds");
//System.out.println("js = "+js);
        }
        if (js == null)
            return theBrowser;
        Object browser = (Object)js.getSlot(index);
//System.out.println("browser = "+browser);
        if (browser instanceof Browser)
            return (Browser)browser;
        else
            return theBrowser;
      }
      catch (Exception e)
      {
        //System.out.println("No browser object not found, "+e);
        return theBrowser;
      }
    }

    // helper functions to reduce number of native functions & netscape crashes
    private native String getString(int opcode);
    private native boolean getBoolean(int opcode);
    private native void setString(int opcode, String s);
    private native void setBoolean(int opcode, boolean b);
    // add event out to list just to keep reference in Java
    // will be removed automatically when Browser is deleted
    private void addObserver(EventOut event)
    {
        if (obsEvents == null)
            obsEvents = new java.util.Vector();
        obsEvents.addElement(event);
    }

    /**
    * get the name of the browser
    * @return name 
    */
    public String getName()
    {
        return getString(OP_NAME);
    }
    /**
    * get current version
    * @return version
    */
    public String getVersion()
    {
        return getString(OP_VERSION);
    }
    /**
    * get URL of current world
    * @return string with URL
    */
    public String getWorldURL()
    {
        return getString(OP_WORLDURL);
    }
    /**
    * get the current viewer mode
    * @return string with mode
    */
    public String getViewerMode()
    {
        return getString(OP_VIEWERMODE);
    }
    /**
    * get the current viewerpoint
    * @return string with name of viewpoint
    */
    public String getViewpoint()
    {
        return getString(OP_VIEWPOINT);
    }
    /**
    * Get the current velocity of the bound viewpoint in meters/sec,
    * if available, or 0.0 if not
    * @return float with speed
    */
    public native float getCurrentSpeed();
    /**
    * Get the current frame rate of the browser, or 0.0 if not available
    * @return float with framerate
    */
    public native float getCurrentFrameRate();
    /**
    * get collision detection
    * @return true if on
    */
    public boolean getCollisionDetection()
    {
        return getBoolean(OP_COLLISION);
    }
    /**
    * get headlight
    * @return true if on
    */
    public boolean                getHeadlight()
    {
        return getBoolean(OP_HEADLIGHT);
    }
    /**
    * get state of viewers animateAllViewpoints switch
    * @return true if switched on
    */
    public boolean                getanimateAllViewpoints()
    {
        return getBoolean(OP_ANIMATE);
    }
    /**
    * returns top level GLView Scene group node, holding the VRML world
    * @return node of top level scene
    * @exception InvalidNodeException if no scene node
    */
    public native Node getWorld()
       throws InvalidNodeException;
    /**
    * Get a DEFed node by name. Nodes given names in the root scene
    * graph must be made available to this method. DEFed nodes in inlines,
    * as well as DEFed nodes returned from createVrmlFromString/URL, may
    * or may not be made available to this method, depending on the
    * browser's implementation
    * @param name of node
    * @return node handle for that node
    * @exception InvalidNodeException if node doesn't exist
    */
    public native Node getNode(String name)
       throws InvalidNodeException;
    /**
    * Load the given URL with the passed parameters (as described
    * in the Anchor node)
    * this will reload the frame 
    * @param string array of URLs
    * @param string array of parameters
    * @exception IllegalArgumentException
    */
    public void loadURL(String[] url, String[] parameter)
    {
        if (url.length == 0)
            return;
        String par = "";
        if (parameter.length > 0)
            par = parameter[0];
        nLoadURL(url[0], par);
    }
    private native void nLoadURL(String url, String parameter);

    /**
    * Parse STRING into a VRML scene and return the list of root
    * nodes for the resulting scene
    * @param string with VRML
    * @return array of root nodes in that string
    * @exception IllegalVrmlException
    */
    public native Node[] createVrmlFromString(String vrmlSyntax)
       throws InvalidVrmlException;

    /** 
    * Tells the browser to load a VRML scene from the passed URL or URLs. 
    * After the scene is loaded, an event is sent to the MFNode  eventIn in node NODE named by the EVENT argument
    * @param url url to be loaded 
    * @param node node with MFNode event in 
    * @param eventName Event in of node e.g. "addChildren" or "set_children"
    */ 
    public void createVrmlFromURL(String[] url, Node node, String event)
    {
        if (url.length == 0)
            return;
        nCreateVrmlFromURL(url[0], node, event);
    }
    private native void nCreateVrmlFromURL(String url, Node node, String event);

    /**
    * create a node of the given classname or prototype name 
    * simpler alternative for createVrmlFromString(" name {} ")
    * @param string with VRML
    * @return root node
    * @exception IllegalVrmlException
    */
    public native Node createNode(String name)
       throws InvalidVrmlException;
    /**
    * Tells the browser to load a VRML scene from the passed URL or
    * URLs. After the scene is loaded, an event is sent to the MFNode
    * eventIn in node NODE named by the EVENT argument
    * @param string array of URLs
    * @param node handle that gets the result
    * @param string with field name
    */
    /**
    * Add a route between the specified eventOut
    * and eventIn of the given nodes
    * @param from node handle
    * @param string with field name of eventOut
    * @param to node handle
    * @param string with field name of eventIn
    * @exception IllegalArgumentException
    */
    public native void addRoute(Node fromNode, String fromEventOut,
                                Node toNode, String toEventIn)
       throws IllegalArgumentException;
    /**
    * delete a route between the specified eventOut
    * and eventIn of the given nodes
    * @param from node handle
    * @param string with field name of eventOut
    * @param to node handle
    * @param string with field name of eventIn
    * @exception IllegalArgumentException
    */
    public native void deleteRoute(Node fromNode, String fromEventOut,
                                   Node toNode, String toEventIn)
       throws IllegalArgumentException;
    /**
    * Replace the current world with the passed node 
    * optimized case, where node is a "Scene" node  
    * @param node
    * @exception IllegalArgumentException
    */
    public native void replaceWorld(Node node)
       throws IllegalArgumentException;

    /**
    * Replace the current world with the passed array of nodes
    * @param nodes array
    * @exception IllegalArgumentException
    */
    public void replaceWorld(Node[] nodes)
       throws IllegalArgumentException
    {
        Node[] sceneNodes = createVrmlFromString("Group {}");
 
        // this node is actually a scene -- its children field contains the nodes
        ((EventInMFNode) sceneNodes[0].getEventIn ("set_children")).setValue (nodes);

        replaceWorld(sceneNodes[0]);
    }

    /**
    * add node as new top-level scene node 
    * @param node
    * @exception IllegalArgumentException
    */
    public native void addNode(Node newNode)
       throws IllegalArgumentException;
    /**
    * remove node from top-level scene node 
    * @param node
    * @return true: node	was a top level node, and has been removed
    */
    public native boolean removeNode(Node removeNode)
       throws IllegalArgumentException;
    /**
    * set name of node to newName 
    * node will be availabe afterwards to getNode(newName)
    * application is responsible, that node is or will be part of the current scene
    * @param node
    * @param string new name
    * @exception IllegalArgumentException
    */
    public native void setNodeName(Node newNode, String newName)
       throws IllegalArgumentException;
    /**
    * Set the description of the current world in a browser-specific
    * manner. To clear the description, pass an empty string as argument
    * @param string new description
    */
    public void setDescription(String description)
    {
        setString(OP_DESCRIPTION, description);
    }
    /**
    * set name of current navigation mode (walk, examine, fly ...)
    * @param string new mode
    */
    public void setViewerMode(String navigationMode)
    {
        setString(OP_VIEWERMODE, navigationMode);
    }
    /**
    * set new viewpoint
    * @param string new viewpoint
    */
    public void setViewpoint(String viewpointName)
    {
        setString(OP_VIEWPOINT, viewpointName);
    }
    /**
    * set state of viewers collisionDetection switch
    * @param boolean collision on/off
    */
    public void setCollisionDetection(boolean collisionDetection)
    {
        setBoolean(OP_COLLISION, collisionDetection);
    }
    /**
    * switch headlight on/off
    * @param boolean headlight on/off
    */
    public void setHeadlight(boolean headlight)
    {
        setBoolean(OP_HEADLIGHT, headlight);
    }
    /**
    * set state of viewers animateAllViewpoints switch
    * @param boolean 
    */
    public void setAnimateAllViewpoints(boolean animateAllViewpoints)
    {
        setBoolean(OP_ANIMATE, animateAllViewpoints);
    }
    /**
    * jump to next viewpoint
    */
    public void setNextViewpoint()
    {
        setBoolean(OP_NEXTVIEW, true);
    }
    /**
    * jump back to previous viewpoint
    */
    public void setPrevViewpoint()
    {
        setBoolean(OP_PREVVIEW, true);
    }
    /**
    * notify browser of a batch of EAI operations requiring no temporay update 
    * a call of beginUpdate must be followed by endUpdate, begin/end Updatecalls can be nested
    */
    public void beginUpdate()
    {
        setBoolean(OP_BEGINUPDATE, true);
    }
    /**
    * end batch of EAI operations 
    */
    public void endUpdate()
    {
        setBoolean(OP_ENDUPDATE, true);
    }
    /**
    * set browser timer animation cylcle to milliSeconds 
    * use full to reduce systemload if greater numbers  200 - 500
    * or get more  frequent updates of lower numbers 10 .. 100
    * @param time interval in milli seconds
    * @return old timerinterval
    */
    public native int setTimerInterval(int milliSeconds);
    /**
    * set EventOutObserver for url changed event
    * call back will be called whenever scene download or new scene will be loaded internally
    * eventype is SFString, containing the URL 
    * @param EventOutObserver observer
    * @param Object userData
    * @return eventout that will be called
    */
    public native EventOut adviseOnUrlChanged(EventOutObserver observer, Object userData);

    /**
    * activate next viewpoint
    */
    public void OnNextViewpoint()
    {
        setBoolean(OP_NEXTVIEW, true);
    }
    /**
    * activate previous viewpoint
    */
    public void OnPrevViewpoint()
    {
        setBoolean(OP_PREVVIEW, true);
    }
    /**
    * true if the whole scene is loaded
    */
    public boolean isSceneLoaded()
    {
        return getBoolean(OP_ISLOADED);
    }
    /**
    * get the name of a node
    */
    public native String getNodeName(Node node)
       throws InvalidNodeException;
    /**
    * get an eventOut of the browser object
    * @param String name of the eventOut
    * @return the EventOut
    */
    public native EventOut getEventOut(String name);
    /**
    * get an eventIn of the browser object
    * @param String name of the eventIn
    * @return the EventIn
    */
    public native EventIn getEventIn(String name);
    /**
    * save the current status of the world to a vrml file
    * @param String filename
    * @return true if successful
    */
    public native boolean saveWorld(String filename);
    /**
    * save the current position as viewpoint
    * @param String viewpoint name
    */
    public void saveViewpoint(String name)
    {
        setString(OP_SAVEVIEW, name);
    }
    /**
    * set 3d window to fullscreen
    * @param boolean true if fullscreen
    * @return boolean previous mode
    */
    public native boolean setFullscreen(boolean mode);
    /**
    * get the user interface mode, e.g. no menu
    * @return int current mode
    */
    public native int getUiMode();
    /**
    * set the user interface mode, e.g. no menu
    * @param int new mode
    */
    public native void setUiMode(int mode);
    /**
    * open the CC3D prefernces dialog
    * @param int flag
    */
    public native void openPreferences(int flag);
    /**
    * play the specified sound file
    * @param String sound file
    */
    public void playSound(String soundFile)
    {
        setString(OP_PLAYSOUND, soundFile);
    }
    /**
    * print a tring to the vrml console
    * @param String message
    */
    public void print(String message)
    {
        setString(OP_PRINT, message);
    }
    /**
    * compute ray hit (see release notes for details on result node)
    * @param Node start node
    * @param float[3] start 
    * @param float[3] end 
    * @param int mode 
    * @result Node rayhit node
    */
    public native Node computeRayHit(Node startNode, float[] start, float[] end, int mode);
    /**
    * set the bound viewpoint to the specified value
    * @param float[3] position 
    * @param float[4] orientation 
    * @param int mode 1 => animate, 4 => relative modus: position = translation, orientation = rotation
    */
    public native void setViewpointByValue(float[] position, float[] orientation, int mode);
    /**
    * get the current viewpoint position
    * @param float[3] result position 
    * @param float[4] result orientation 
    * @param int mode, 0 => local viewpoint, 1 => global viewpoint, 2 => 3dperson view
    */
    public native void getViewpointByValue(float[] position, float[] orientation, int mode);
    /**
    * get the bounding box of a node
    * @param Node node to check
    * @param int mode = 1 => don't use defined bboxes (only calculated bboxes)
    * @param float[3] result min point 
    * @param float[3] result max point 
    * @return boolean true if bounding box available 
    */
    public native boolean getNodeBoundingBox(Node node, int mode, float[] minimum, float[] maximum)
       throws InvalidNodeException;
    /**
    * activate the 3rd person view
    * @param boolean true = on
    */
    public void setThirdPersonView(boolean mode)
    {
        setBoolean(OP_3RDPERSON, mode);
    }
    /**
    * get the 3rd person view
    * @return boolean true = on
    */
    public boolean getThirdPersonView()
    {
        return getBoolean(OP_3RDPERSON);
    }
    /**
    * set my avatar node (for 3rd person view)
    * @param Node node of avatar
    */
    public native void setMyAvatarNode(Node node)
       throws InvalidNodeException;
    /**
    * get my avatar node (for 3rd person view)
    * @return Node node of avatar
    */
    public native Node getMyAvatarNode()
       throws InvalidNodeException;
    /**
    * set my avatar URL (for 3rd person view)
    * @param String URL of avatar
    */
    public void setMyAvatarURL(String url)
    {
        setString(OP_AVATARURL, url);
    }
    /**
    * get my avatar URL (for 3rd person view)
    * @return String URL of avatar
    */
    public String getMyAvatarURL()
    {
        return getString(OP_AVATARURL);
    }
    /**
    * follow the spcified node in the specified distance
    * @param Node node to follow
    * @param float[] ref, reference point inside node
    * @param int mode unused
    * @return boolean true if successful
    */
    public native boolean setViewpointFollow(Node node, float[] ref, int mode)
       throws InvalidNodeException;
    /**
    * set node eventin from string, usable from script
    * @param String name of node
    * @param String name of eventIn
    * @param String value for eventIn
    * @return boolean true if successful
    */
    public native boolean setNodeEventIn(String node, String event, String value);
    /**
    * get node eventout by string, usable from script
    * @param String name of node
    * @param String name of eventOut
    * @return String value of eventOut
    */
    public native String getNodeEventOut(String node, String event);

    /**
    * get a C++ direct X/RSX object, only reusable in C!!!
    * @param int objectId:<br>
    * DIRECTDRAW	= 0<br>
    * DIRECT3D	= 1<br>
    * DIRECT3DDEVICE	= 2<br>
    * DIRECT3DVIEWPORT	= 3<br>
    * DD_FRONTBUFFER	= 4<br>
    * DD_BACKBUFFER	= 5<br>
    * DD_ZBUFFER	= 6<br>
    * DIRECTSOUND	= 7<br>
    * DIRECTSOUND3DLISTENER	= 8<br>
    * RSX	= 9<br>
    * RSX_LISTENER	= 10<br>
    *
    * @return int C handle of object (LPUNKNOWN)
    */
    public native int getObject(int objectId);

    /**
    * get a C++ direct X/RSX object, only reusable in C!!!
    * @param int objectId:<br>
    * DIRECTDRAW	= 0<br>
    * DIRECT3D	= 1<br>
    * DIRECT3DDEVICE	= 2<br>
    * DIRECT3DVIEWPORT	= 3<br>
    * DD_FRONTBUFFER	= 4<br>
    * DD_BACKBUFFER	= 5<br>
    * DD_ZBUFFER	= 6<br>
    * DIRECTSOUND	= 7<br>
    * DIRECTSOUND3DLISTENER	= 8<br>
    * RSX	= 9<br>
    * RSX_LISTENER	= 10<br>
    *
    * @return Integer object with C handle of object (LPUNKNOWN)
    */
    public Object getComObject(int objectId)
    {
        int obj = getObject(objectId);
        if (obj != 0)
	    return new Integer(obj);
        else
	    return null;
    }
}
