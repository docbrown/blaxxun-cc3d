// package
package vrml.external;

import vrml.external.exception.*;

/** 
* Specification of the External Interface for a VRML browser.
*
*/ 


public interface IBrowser {

  /** 
  * Get the "name" of the VRML browser (browser-specific)
  * @return           name of the browser "blaxxunCC3D" for blaxxunCC3D
  */ 
  public String                getName();

  /** 
  * Get the  "version" of the VRML browser (browser-specific)
  * @return           version of the browser
  */
  public String       getVersion();

  /** 
  * Get the current velocity of the bound viewpoint in meters/sec, if available, or 0.0 if not
  * @return           current speed
  */ 
  
  public float         getCurrentSpeed();
  /** 
  * Get the current frame rate of the browser, or 0.0 if not available
  * @return           current frame rate
  */ 

  public float		  getCurrentFrameRate();
  /** 
  * Get the URL for the root of the current world, or an empty string if not available
  * @return           url 
  */ 

  public String		  getWorldURL();
  /** 
  * Replace the current world with the passed array of nodes
  * @param nodes      The array of nodes
  * @return           url 
  */ 
  public void		  replaceWorld(Node[] nodes)
	  throws IllegalArgumentException;

  /** 
  * Load the given URL with the passed parameters (as described in the Anchor node)
  * @param url      The array of urls	(blaxxunCC3D uses only the element at index 0
  * @param parameter The array of params
  */ 
  public void		  loadURL(String[] url, String[] parameter);

  /** 
  * Set the description of the current world in a browser-specific manner. To clear the description, pass an empty string as argumentLoad the given URL with the passed parameters (as described in the Anchor node)
  * @param description the string, will be displayed in status line of browser
  */ 
  public void		  setDescription(String description);

  /** 
  * Parse STRING into a VRML scene and return the list of root nodes for the resulting scene
  * @param vrmlSyntax string with a valid vrml syntax string (without #VRML header)
  * @return           an array of nodes, if the length is 0 this also means a syntax error or a bad string
  */ 
  public Node[]       createVrmlFromString(String vrmlSyntax)
	  throws InvalidVrmlException;

  /** 
  * Tells the browser to load a VRML scene from the passed URL or URLs. 
  * After the scene is loaded, an event is sent to the MFNode  eventIn in node NODE named by the EVENT argument
  * @param url url to be loaded 
  * @param node node with MFNode event in 
  * @param eventName Event in of node e.g. "addChildren" or "set_children"
  */ 
  public void         createVrmlFromURL(String[] url,Node node,String event);

 /** 
  * Get a DEFed node by name. Nodes given names in the root scene
  * graph must be made available to this method. DEFed nodes in inlines,
  * as well as DEFed nodes returned from createVrmlFromString/URL, may
  * or may not be made available to this method, depending on the
  * browser's implementation
  * @param name node name 
  * @return		node if found or exception if node is not present
  */
  public Node         getNode(String name) throws InvalidNodeException;

  /** 
  * Add a route between the specified eventOut
  * and eventIn of the given nodes
  * @param fromNode	 source node
  * @param fromEventOut event out name eg. "fraction_changed"	
  * @param toNode	destination node 		
  * @param toEventIn	event int name eg. "set_fraction"	
  */
  public void		   addRoute(Node fromNode, String fromEventOut,
								Node toNode, String toEventIn)
								throws IllegalArgumentException;

  /** 
  * Delete a route between the specified eventOut
  * and eventIn of the given nodes
  * @param fromNode	 source node
  * @param fromEventOut event out name eg. "fraction_changed"	
  * @param toNode destination node 	
  * @param toEventIn	event int name eg. "set_fraction"	
  */
  public void			deleteRoute(Node fromNode, String fromEventOut,
								   Node toNode, String toEventIn)
								   throws IllegalArgumentException;

 /** 
  * notify browser of a batch of EAI operations requiring no temporay update<P>
  * a call of beginUpdate must be followed by endUpdate, begin/end Updatecalls can be nested
  */ 
  public void		   beginUpdate();

  /** 
  * end batch of EAI operations
  */ 
  public void          endUpdate();
  
  /** 
  * called after the scene is loaded, before the first event is processed
  */ 

  public void initialize();
  
  /** 
  * called just before the scene is unloaded
  */ 
  public void shutdown();

}
