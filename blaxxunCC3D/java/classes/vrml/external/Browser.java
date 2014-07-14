// Browser.java
// Wrapper for blaxxunCC3D Control EAI Interface
// 26.09.97 hg 

// package
package vrml.external;

// imports
import vrml.external.field.EventOut;
import vrml.external.field.EventOutObserver;
import vrml.external.field.EventInMFNode;
import vrml.external.field.EventOutMFNode;
import vrml.external.exception.*;
import com.ms.com.*;


/** 
 * VRML External Interface class for blaxxunCC3D control</P>
 * For example: 
 * <pre> 
 *    Browser = Browser.getBrowser(handleOfControl);
 * </pre> 
 * 
 */ 

public class Browser implements IBrowser 
{
    private static final _Guid IID_IDispatch = new _Guid("{00020400-0000-0000-C000-000000000046}");
    private static final _Guid SID_SContainerDispatch = new _Guid("{b722be00-4e68-101b-a2bc-00aa00404770}");
    private    static int      tries = 0;

    
    // glview control
  private blaxxuncc3d.IblaxxunCC3D m_bxCtrl;

  // constructor
  public Browser (blaxxuncc3d.IblaxxunCC3D bxCtrl)
  {
    m_bxCtrl = bxCtrl;
  }


  /** 
  * Get the "name" of the VRML browser (browser-specific)
  * @return           name of the browser "blaxxunCC3D" for blaxxunCC3D
  */ 
  public String                getName()
  {
    // just pass on to gl
    return m_bxCtrl.getName ();
  }

  /** 
  * Get the  class version "name" of the VRML browser as static (browser-specific)
  * @return           name of the browser "blaxxunCC3D" for blaxxunCC3D
  */ 
  public static String                getClassName()
  {
    return new String("blaxxunCC3D");
  }

  /** 
  * Get the  "version" of the VRML browser (browser-specific)
  * @return           version of the browser
  */ 
  public String                getVersion()
  {
    // same here
    return m_bxCtrl.getVersion ();
  }


  /** 
  * Get the current velocity of the bound viewpoint in meters/sec, if available, or 0.0 if not
  * @return           current speed
  */ 

  public float         getCurrentSpeed()
  {
    // just pass on to gl
    return m_bxCtrl.getCurrentSpeed();
  }

  /** 
  * Get the current frame rate of the browser, or 0.0 if not available
  * @return           current frame rate
  */ 

  public float         getCurrentFrameRate()
  {
    // just pass on to gl
    return m_bxCtrl.getCurrentFrameRate();
  }

  /** 
  * Get the URL for the root of the current world, or an empty string if not available
  * @return           url 
  */ 

  public String                getWorldURL()
  {
    // just pass on to gl
    return m_bxCtrl.getWorldURL();
  }

  /** 
  * Replace the current world with the passed array of nodes
  * @param nodes      The array of nodes
  * @return           url 
  */ 

  public void          replaceWorld(Node[] nodes)
       throws IllegalArgumentException
  {
        Node[] sceneNodes = createVrmlFromString("Group {}");
 
        // this node is actually a scene -- its children field contains the nodes
        ((EventInMFNode) sceneNodes[0].getEventIn ("set_children")).setValue (nodes);

        replaceWorld(sceneNodes[0]);
  }          

  /** 
  * Load the given URL with the passed parameters (as described in the Anchor node)
  * @param url      The array of urls    (blaxxunCC3D uses only the element at index 0
  * @param parameter The array of params
  */ 

  public void          loadURL(String[] url, String[] parameter)
  {
        if (url.length == 0 ) return; // ignore it 

      // just pass onto glview -- it does not accept an array of urls, so just
       // pass the first one for now
       if (parameter.length >0) {
            m_bxCtrl.loadURL (url[0], parameter[0]);
       }
       else m_bxCtrl.loadURL (url[0], "");

  }    

  /** 
  * Set the description of the current world in a browser-specific manner. To clear the description, pass an empty string as argumentLoad the given URL with the passed parameters (as described in the Anchor node)
  * @param description the string, will be displayed in status line of browser
  */ 
  public void          setDescription(String description)
  {
    // just pass on to gl
    m_bxCtrl.setDescription(description);
  }


  /** 
  * Parse STRING into a VRML scene and return the list of root nodes for the resulting scene
  * @param vrmlSyntax string with a valid vrml syntax string (without #VRML header)
  * @return           an array of nodes, if the length is 0 this also means a syntax error or a bad string
  */ 
  public Node[]                createVrmlFromString(String vrmlSyntax)
       throws InvalidVrmlException
  {
    blaxxunvrml.Node bxNode;

    Node node;

    bxNode = (blaxxunvrml.Node) m_bxCtrl.createVrmlFromString(vrmlSyntax);

    if (bxNode == null)
    {
      throw new InvalidVrmlException("bad vrml  :"+vrmlSyntax);
    }


    // get the node
    node = new Node (bxNode);

    // this node is actually a scene -- its children field contains the nodes
    return ((EventOutMFNode) node.getEventOut ("children_changed")).getValue ();


  }          

  /** 
  * Tells the browser to load a VRML scene from the passed URL or URLs. 
  * After the scene is loaded, an event is sent to the MFNode  eventIn in node NODE named by the EVENT argument
  * @param url url to be loaded 
  * @param node node with MFNode event in 
  * @param eventName Event in of node e.g. "addChildren" or "set_children"
  */ 

  public void          createVrmlFromURL(String[] url,
                                         Node node,
                                         String event)
  {
    // just pass onto blaxxunCC3D -- it does not accept an array of urls, so just
    // pass the first one for now
    m_bxCtrl.createVrmlFromURL (url [0], node.getBxNode(), event);
  }



 /** 
  * Get a DEFed node by name. Nodes given names in the root scene
  * graph must be made available to this method. DEFed nodes in inlines,
  * as well as DEFed nodes returned from createVrmlFromString/URL, may
  * or may not be made available to this method, depending on the
  * browser's implementation
  * @param name node name 
  * @return        node if found or exception if node is not present
  */

  public Node          getNode(String name)
       throws InvalidNodeException
  {
    blaxxunvrml.Node bxNode;
    Node node;

    // create vrml node from glview node
    bxNode = (blaxxunvrml.Node) m_bxCtrl.getNode (name);
    if (bxNode == null)
    {
      node = null;
      throw new InvalidNodeException("no such node :"+name);
    }
    else
    {
      node = new Node (bxNode);
    }

    return node;
  }

  /** 
  * Add a route between the specified eventOut
  * and eventIn of the given nodes
  * @param fromNode     source node
  * @param fromEventOut event out name eg. "fraction_changed"    
  * @param toNode    destination node         
  * @param toEventIn    event int name eg. "set_fraction"    
  */

  public void          addRoute(Node fromNode, String fromEventOut,
                                Node toNode, String toEventIn)
       throws IllegalArgumentException
  {
      // get the native GLView nodes 
      blaxxunvrml.Node bxNodeA;
      blaxxunvrml.Node bxNodeB;
      bxNodeA = fromNode.getBxNode();
      bxNodeB = toNode.getBxNode();
      
      if (bxNodeA == null || bxNodeB == null) throw new IllegalArgumentException();

      m_bxCtrl.addRoute(bxNodeA,fromEventOut,bxNodeB,toEventIn);
  }
  /** 
  * Delete a route between the specified eventOut
  * and eventIn of the given nodes
  * @param fromNode     source node
  * @param fromEventOut event out name eg. "fraction_changed"    
  * @param toNode destination node     
  * @param toEventIn    event int name eg. "set_fraction"    
  */
  
  public void          deleteRoute(Node fromNode, String fromEventOut,
                                   Node toNode, String toEventIn)
       throws IllegalArgumentException
  {
      // get the native GLView nodes 
      blaxxunvrml.Node bxNodeA;
      blaxxunvrml.Node bxNodeB;
      bxNodeA = fromNode.getBxNode();
      bxNodeB = toNode.getBxNode();
      
      if (bxNodeA == null || bxNodeB == null) throw new IllegalArgumentException();

      m_bxCtrl.deleteRoute(bxNodeA,fromEventOut,bxNodeB,toEventIn);
  }

  /** 
  * return an instance of the IblaxxunCC3D<P>
  * IE : scans the context to find handle to CC3D control<P>
  */

   private static blaxxuncc3d.IblaxxunCC3D findCC3D(java.applet.Applet applet)
    {
        java.awt.Component object1 = applet; // no member parent ???

        //java.awt.Component object1;               //HG 
        
        try
        {
            // query parents 
            tries++;
            
            com.ms.activeX.ActiveXControlServices activeXControlServices = null;
            for (; object1 != null; object1 = object1.getParent())
            {
                if (object1 instanceof com.ms.activeX.ActiveXControlServices)
                {
                    activeXControlServices = (com.ms.activeX.ActiveXControlServices)object1;
                    break;
                }
            }
            if (activeXControlServices == null)
                return null;

            // found activeXControlServices
            try
            {
                com.ms.com.IServiceProvider iServiceProvider = (com.ms.com.IServiceProvider)activeXControlServices.getClientSite();
                Object aobject[] = new Object[1];
                // com.ms.com._Guid.
                iServiceProvider.QueryService( SID_SContainerDispatch, IID_IDispatch, aobject);
                com.ms.com.Variant variant3 = com.ms.com.Dispatch.get(aobject[0], "applets");
                int i = com.ms.com.Dispatch.get(variant3, "length").toInt();
                for (int j = 0; j < i; j++)
                {
                    com.ms.com.Variant variant1 = com.ms.com.Dispatch.call(variant3, "item", new Integer(j), null);
                    Object object2 = variant1.toObject();
                    if (object2 instanceof blaxxuncc3d.IblaxxunCC3D)
                        return (blaxxuncc3d.IblaxxunCC3D)object2;
                }
                variant3 = com.ms.com.Dispatch.get(aobject[0], "embeds");
                i = com.ms.com.Dispatch.get(variant3, "length").toInt();
                for (int k = 0; k < i; k++)
                {
                    com.ms.com.Variant variant2 = com.ms.com.Dispatch.call(variant3, "item", new Integer(k), null);
                    Object object3 = variant2.toObject();
                    if (object3 instanceof blaxxuncc3d.IblaxxunCC3D)
                        return (blaxxuncc3d.IblaxxunCC3D)object3;
                }
                return null;
            }
            catch (Exception e)
            {
                e.printStackTrace();
            }
        }
        catch (Exception e1)
        {
            if (tries ==1 )
                System.out.println("required Microsoft Java classes not found, please upgrade to IE 4.0\n");
        }
        return null;
    }





  /** 
  * return an instance of the Browser class<P>
  * Netscape : This returns the first embedded plugin in the current frame.<P>
  * IE : call setControl(handleOfVrmlControl) first or use getBrowser(handleOfVrmlControl)<P>
  */
  static public Browser getBrowser(java.applet.Applet pApplet)  
  {

    // get control first from hashtable
    blaxxuncc3d.IblaxxunCC3D bxCtrl = blaxxunCC3DControl.GetControl(pApplet);

    // now try IE 4 services 
    
    if (bxCtrl == null)
    {
        try {
            bxCtrl = findCC3D(pApplet);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
        
    }

    if (bxCtrl == null)
    {
      return null;
    }
    else
    {
      return new Browser (bxCtrl);
    }
  }    

  /** 
  * return an instance of the Browser class<P>
  * Netscape : If frameName is NULL, current frame is assumed.<P>
  * IE : call setControl(handleOfVrmlControl) <P>
  *    or blaxxunCC3DControl.SetControl(ctrlObject,applet) first <P>    
  * or use simply getBrowser(handleOfVrmlControl)<P>
  */

  static public Browser getBrowser(java.applet.Applet pApplet, String frameName, int index)
  {
    // get control first
    blaxxuncc3d.IblaxxunCC3D bxCtrl = blaxxunCC3DControl.GetControl(pApplet,frameName,index);
    if (bxCtrl == null)
    {
      return null;
    }
    else
    {
      return new Browser (bxCtrl);
    }

  }    

  //
  // blaxxunCC3D Control extended messages
  //

  /** 
  * return an instance of the Browser class, used for Internet Explorer : input is handle to blaxxunCC3D Active X Control<P>
  * Netscape : not supported<P>
  * @param bxCtrl     handle of blaxxunCC3D active X control passed via the VBScript onLoad function<P>
  */

  public static Browser getBrowser (Object bxCtrl)
  {
    return new Browser ((blaxxuncc3d.IblaxxunCC3D) bxCtrl);
  }


  // worldview compatibility 
  /** 
  * set the control object (blaxxunCC3D) used for Internet Explorer : input is handle to blaxxunCC3D Active X Control<P>
  * Netscape : not supported<P>
  * @param bxCtrl     handle of blaxxunCC3D active X control passed via the VBScript onLoad function<P>
  * @param pApplet     the applet bxCtrl is associated with
  */
  public static void SetControl(Object bxCtrl, java.applet.Applet pApplet)
  {
      blaxxunCC3DControl.SetControl(bxCtrl,pApplet);  
  }

  /** 
  * free the control object (blaxxunCC3D)<P>
  * Netscape : not supported<P>
  */
  public static void FreeControls()
  {
      blaxxunCC3DControl.FreeControls();
  }


  /** 
  * Get the name of current navigation mode (blaxxunCC3D)
  * @return           viewmode string 
  */ 

  public String                getViewerMode()
  {
        return m_bxCtrl.getnavigationMode();
  }

  /** 
  * set name of current navigation mode (walk, examine, fly ...) (blaxxunCC3D)
  * @param  navigationMode viewmode string 
  */ 
  public void              setViewerMode(String navigationMode)
  {
        m_bxCtrl.putnavigationMode(navigationMode);
  }

  /** 
  * get name of currently bound viewpoint (blaxxunCC3D)
  * @return           viewpoint name string 
  */ 

  public String                getViewpoint()
  {
        return m_bxCtrl.getviewpoint();
  }

  /** 
  * set new viewpoint by string (blaxxunCC3D)
  * @param  viewpointName viewpoint name string 
  */ 

  public void              setViewpoint(String viewpointName)
  {
        m_bxCtrl.putviewpoint(viewpointName);
  }

  /** 
  * save viewpoint  (blaxxunCC3D)
  * @param  viewpointName, if not node not present, new Viewpoint will be created 
  */ 

  public void              saveViewpoint(String viewpointName)
  {
        m_bxCtrl.saveViewpoint(viewpointName);
  }

 /** 
  * get state of viewers collisionDetection switch (blaxxunCC3D)
  */ 
  public boolean                getCollisionDetection()
  {
        return m_bxCtrl.getcollisionDetection();
  }

 /** 
  * set state of viewers collisionDetection switch (blaxxunCC3D)
  */ 
  public void              setCollisionDetection(boolean collisionDetection)
  {
      m_bxCtrl.putcollisionDetection(collisionDetection);
  }


 /** 
  * get state of viewers headlight switch (blaxxunCC3D)
  */ 
  public boolean                getHeadlight()
  {
        return m_bxCtrl.getheadlight()!=0;
  }

 /** 
  * set state of viewers headlight switch (blaxxunCC3D)
  */ 
  public void              setHeadlight(boolean headlight)
  {
      m_bxCtrl.putheadlight((short) (headlight ? 1 : 0));
  }



 /** 
  * get state of viewers animateAllViewpoints switch (blaxxunCC3D)
  */ 
  public boolean                getanimateAllViewpoints()
  {
        return m_bxCtrl.getanimateAllViewpoints();
  }

 /** 
  * set state of viewers animateAllViewpoints switch (blaxxunCC3D)
  */ 
  public void              setAnimateAllViewpoints(boolean animateAllViewpoints)
  {
      m_bxCtrl.putanimateAllViewpoints(animateAllViewpoints);
  }

 /** 
  * jump to next viewpoint (blaxxunCC3D)
  */ 
  public void                setNextViewpoint()
  {
        m_bxCtrl.OnNextViewpoint();
  }

 /** 
  * jump to prev viewpoint (blaxxunCC3D)
  */ 
  public void                setPrevViewpoint()
  {
        m_bxCtrl.OnPrevViewpoint();
  }

 /** 
  * notify browser of a batch of EAI operations requiring no temporay update (blaxxunCC3D) <P>
  * a call of beginUpdate must be followed by endUpdate, begin/end Updatecalls can be nested
  */ 
  public void                beginUpdate()
  {
        m_bxCtrl.beginUpdate();
  }
 /** 
  * end batch of EAI operations (blaxxunCC3D)
  */ 
  public void                endUpdate()
  {
        m_bxCtrl.endUpdate();
  }

 /** 
  * set browser timer animation cycle in milliSeconds (blaxxunCC3D) <P>
  * result is old timerInterval
  * usefull to reduce systemload if greater numbers  200 - 500
  * or get more  frequent updates of lower numbers 10 .. 100
  */ 
  public int                 setTimerInterval(int milliSeconds)
  {
        return m_bxCtrl.setTimerInterval(milliSeconds);
  }


 /** 
  * add node as new top-level scene node (blaxxunCC3D) 
  */ 
  public void          addNode(Node newNode)
       throws IllegalArgumentException
  {
      // get the native GLView node 
      blaxxunvrml.Node bxNode;
      bxNode = newNode.getBxNode();
      
      if (bxNode == null) throw new IllegalArgumentException();

      m_bxCtrl.addNode(bxNode);
  }

 /** 
  *  remove node from top-level scene node (blaxxunCC3D)<P> 
  * true: node    was a top level node, and has been removed
  */ 
  public boolean          removeNode(Node removeNode)
       throws IllegalArgumentException
  {
      // get the native GLView node 
      blaxxunvrml.Node bxNode;
      bxNode = removeNode.getBxNode();
      
      if (bxNode == null) throw new IllegalArgumentException();

      return m_bxCtrl.removeNode(bxNode);
  }

 /** 
  * create a node of the given classname or prototype name  (blaxxunCC3D) <P>
  * simpler alternative for createVrmlFromString(" name {} ")
  */ 
  public Node          createNode(String name)
       throws InvalidNodeException
  {
    blaxxunvrml.Node bxNode;
    Node node;

    bxNode = (blaxxunvrml.Node) m_bxCtrl.createNode (name);
    if (bxNode == null)
    {
      node = null;
      throw new InvalidNodeException("no such node class:"+name);
    }
    else
    {
      // create vrml node from glview node
      node = new Node (bxNode);
    }

    return node;
  }

 /** 
  * set name of node to newName (blaxxunCC3D) <P>
  * node will be availabe afterwards to getNode(newName)
  * application is responsible, that node is or will be part of the current scene
  *
  */ 
  public void          setNodeName(Node newNode, String newName)
       throws IllegalArgumentException
  {
      // get the native GLView node 
      blaxxunvrml.Node bxNode;
      bxNode = newNode.getBxNode();
      
      if (bxNode == null) throw new IllegalArgumentException();

      m_bxCtrl.setNodeName(bxNode,newName);
  }


 /** 
  * returns top level blaxxunCC3D Scene group node, holding the VRML world  (blaxxunCC3D)<P>
  * get the eventout "children" to acess all top level nodes
  */ 
  public Node          getWorld()
       throws InvalidNodeException
  {
    blaxxunvrml.Node bxNode;
    Node node;

    bxNode = (blaxxunvrml.Node) m_bxCtrl.getWorld();
    if (bxNode == null)
    {
      node = null;
      throw new InvalidNodeException("no world node ");
    }
    else
    {
      // create vrml node from glview node
      node = new Node (bxNode);
    }

    return node;
  }
  
 /** 
  *  Replace the current world with the passed node 
  * optimized case, where node is a "Scene" node (blaxxunCC3D)
  */ 
  public void          replaceWorld(Node node)
       throws IllegalArgumentException
  {
    blaxxunvrml.Node bxNode;

    bxNode = node.getBxNode();

    if (bxNode == null)
    {
      throw new IllegalArgumentException();
    }


    m_bxCtrl.replaceWorld(bxNode);
  }          


 /** 
  * set EventOutObserver for url changed event  (blaxxunCC3D) <P>
  * call back will be called whenever scene download or new scene will be loaded internally
  *  eventype is SFString, containing the URL 
  */ 
  public EventOut adviseOnUrlChanged(EventOutObserver observer, Object userData) {
        blaxxunvrml.EventOut glEventOut = (blaxxunvrml.EventOut) m_bxCtrl.getEventOut("worldUrl_changed");
        EventOut theEvent = EventOut.newEventOut (glEventOut);    
        theEvent.advise(observer,userData);
        return theEvent;
  }    

  /** 
  * save the world to a local file  (blaxxunCC3D)
  * @param  fileName file name of local file 
  */ 

  public boolean           saveWorld(String fileName)
  {
        return m_bxCtrl.saveWorld(fileName);
  }

  // called after the scene is loaded, before the first event is processed
  public void initialize()
  {
      // to do:
  }    
  
  // called just before the scene is unloaded
  public void shutdown()
  {
      // to do:
  }

    /**
    * set 3d window to fullscreen
    * @param boolean true if fullscreen
    * @return boolean previous mode
    */
    public boolean setFullscreen(boolean mode)
    {
        return m_bxCtrl.setFullscreen(mode);
    }

    /**
    * get the user interface mode, e.g. no menu
    * @return int current mode
    */
    public int getUiMode()
    {
        return m_bxCtrl.getUiMode();
    }

    /**
    * set the user interface mode, e.g. no menu
    * @param int new mode
    */
    public void setUiMode(int mode)
    {
        m_bxCtrl.setUiMode(mode);
    }

    /**
    * popup prefernces dialog
    * @param int flag
    */
    public void openPreferences(int flag)
    {
        m_bxCtrl.openPreferences(flag);
    }

    /**
    * play the specified sound file
    * @param String sound file
    */
    public void playSound(String soundFile)
    {
        m_bxCtrl.playSound(soundFile);
    }

    /**
    * print a tring to the vrml console
    * @param String message
    */
    public void print(String message)
    {
        m_bxCtrl.print(message);
    }

    /**
    * compute ray hit (see release notes for details on result node)
    * @param Node start node
    * @param float[3] start 
    * @param float[3] end 
    * @param int mode 
    * @result Node rayhit node
    */
    public Node computeRayHit(Node startNode, float[] start, float[] end, int mode)
    {
        blaxxunvrml.Node bxNode;
        Node node = null;

        if (startNode == null || start.length != 3 || end.length != 3 )
        {
            return node;
        }
        bxNode = startNode.getBxNode();
        if (bxNode == null)
        {
            return node;
        }
        bxNode = (blaxxunvrml.Node) m_bxCtrl.computeRayHit(bxNode, start[0], start[1], start[2], end[0], end[1], end[2], mode);
        if (bxNode != null)
        {
            // create vrml node from glview node
            node = new Node (bxNode);
        }

        return node;
    }

    /**
    * set the bound viewpoint to the specified value
    * @param float[3] position 
    * @param float[4] orientation 
    * @param int mode 1 => animate, 4 => relative modus: position = translation, orientation = rotation
    */
    public void setViewpointByValue(float[] position, float[] orientation, int mode)
    {
        if (position.length != 3 || orientation.length != 4 )
        {
            return;
        }
        m_bxCtrl.setViewpointByValue(position[0], position[1], position[2], orientation[0],
            orientation[1], orientation[2], orientation[3], mode);
    }

    /**
    * get the current viewpoint position
    * @param float[3] result position 
    * @param float[4] result orientation 
    * @param int mode, 0 => local viewpoint, 1 => global viewpoint, 2 => 3dperson view
    */
    public void getViewpointByValue(float[] position, float[] orientation, int mode)
    {
        if (position.length != 3 || orientation.length != 4 )
        {
            return;
        }
        float[] f1,f2,f3,f4,f5,f6,f7;
        f1 = new float[1];
        f2 = new float[1];
        f3 = new float[1];
        f4 = new float[1];
        f5 = new float[1];
        f6 = new float[1];
        f7 = new float[1];
        m_bxCtrl.getViewpointByValue(f1, f2, f3, f4, f5, f6, f7, mode);
        position[0] = f1[0];
        position[1] = f2[0];
        position[2] = f3[0];
        orientation[0] = f4[0];
        orientation[1] = f5[0];
        orientation[2] = f6[0];
        orientation[3] = f7[0];
    }

    /**
    * get the bounding box of a node
    * @param Node nod to check
    * @param int mode = 1 => don't use defined bboxes (only calculated bboxes)
    * @param float[3] result min point 
    * @param float[3] result max point 
    * @return boolean true if bounding box available 
    */
    public boolean getNodeBoundingBox(Node node, int mode, float[] minimum, float[] maximum)
       throws InvalidNodeException
    {
        blaxxunvrml.Node bxNode;

        if (node == null)
        {
            throw new InvalidNodeException();
        }
        bxNode = node.getBxNode();
        if (bxNode == null)
        {
            throw new InvalidNodeException();
        }
        if (minimum.length != 3 || maximum.length != 3 )
        {
            return false;
        }
        float[] f1,f2,f3,f4,f5,f6;
        f1 = new float[1];
        f2 = new float[1];
        f3 = new float[1];
        f4 = new float[1];
        f5 = new float[1];
        f6 = new float[1];
        if (m_bxCtrl.getNodeBoundingBox(bxNode, mode, f1, f2, f3, f4, f5, f6))
        {
            minimum[0] = f1[0];
            minimum[1] = f2[0];
            minimum[2] = f3[0];
            maximum[0] = f4[0];
            maximum[1] = f5[0];
            maximum[2] = f6[0];
        }
        return true;
    }

    /**
    * activate the 3rd person view
    * @param boolean true = on
    */
    public void setThirdPersonView(boolean mode)
    {
        m_bxCtrl.setThirdPersonView(mode);
    }

    /**
    * get the 3rd person view
    * @return boolean true = on
    */
    public boolean getThirdPersonView()
    {
        return m_bxCtrl.getThirdPersonView();
    }

    /**
    * set my avatar node (for 3rd person view)
    * @param Node node of avatar
    */
    public void setMyAvatarNode(Node node)
       throws InvalidNodeException
    {
        blaxxunvrml.Node bxNode;

        if (node == null)
        {
            throw new InvalidNodeException();
        }
        bxNode = node.getBxNode();
        if (bxNode == null)
        {
            throw new InvalidNodeException();
        }
        m_bxCtrl.setMyAvatarNode(bxNode);
    }

    /**
    * get my avatar node (for 3rd person view)
    * @return Node node of avatar
    */
    public Node getMyAvatarNode()
       throws InvalidNodeException
    {
        blaxxunvrml.Node bxNode;
        Node node = null;

        bxNode = (blaxxunvrml.Node) m_bxCtrl.getMyAvatarNode();
        if (bxNode != null)
        {
            // create vrml node from glview node
            node = new Node (bxNode);
        }

        return node;
    }

    /**
    * set my avatar URL (for 3rd person view)
    * @param String URL of avatar
    */
    public void setMyAvatarURL(String url)
    {
        m_bxCtrl.setMyAvatarURL(url);
    }

    /**
    * get my avatar URL (for 3rd person view)
    * @return String URL of avatar
    */
    public String getMyAvatarURL()
    {
        return m_bxCtrl.getMyAvatarURL();
    }

    /**
    * follow the spcified node in the specified distance
    * @param Node node to follow
    * @param float[] ref, reference point inside node
    * @param int mode unused
    * @return boolean true if successful
    */
    public boolean setViewpointFollow(Node node, float[] ref, int mode)
       throws InvalidNodeException
    {
        blaxxunvrml.Node bxNode;

        if (node == null)
        {
            throw new InvalidNodeException();
        }
        bxNode = node.getBxNode();
        if (bxNode == null)
        {
            throw new InvalidNodeException();
        }
        if (ref.length != 3)
        {
            return false;
        }
        return m_bxCtrl.setViewpointFollow(bxNode, ref[0], ref[1], ref[2], mode);
    }

    /**
    * set node eventin from string, usable from script
    * @param String name of node
    * @param String name of eventIn
    * @param String value for eventIn
    * @return boolean true if successful
    */
    public boolean setNodeEventIn(String node, String event, String value)
    {
        return m_bxCtrl.setNodeEventIn(node, event, value);
    }

    /**
    * get node eventout by string, usable from script
    * @param String name of node
    * @param String name of eventOut
    * @return String value of eventOut
    */
    public String getNodeEventOut(String node, String event)
    {
        String s = new String();
        s = m_bxCtrl.getNodeEventOut(node, event);
        return s;
    }

    /**
    * get a C++ direct X/RSX object, only reusable in C!!!
    * @param int objectId:<br>
    * DIRECTDRAW    = 0<br>
    * DIRECT3D    = 1<br>
    * DIRECT3DDEVICE    = 2<br>
    * DIRECT3DVIEWPORT    = 3<br>
    * DD_FRONTBUFFER    = 4<br>
    * DD_BACKBUFFER    = 5<br>
    * DD_ZBUFFER    = 6<br>
    * DIRECTSOUND    = 7<br>
    * DIRECTSOUND3DLISTENER    = 8<br>
    * RSX    = 9<br>
    * RSX_LISTENER    = 10<br>
    *
    * @return com object (com.ms.com.IUnknown)
    */
    public Object getComObject(int objectId)
    {
           return (Object)m_bxCtrl.getObject(objectId);
    }

}

