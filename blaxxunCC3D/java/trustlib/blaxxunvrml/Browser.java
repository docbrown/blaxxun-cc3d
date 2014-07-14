//
// Auto-generated using JActiveX.EXE 5.00.2924
//   (d:\sdkj31\bin\jactivex /javatlb /d d:\java blaxxunvrml.tlb)
//
// WARNING: Do not remove the comments that include "@com" directives.
// This source file must be compiled by a @com-aware compiler.
// If you are using the Microsoft Visual J++ compiler, you must use
// version 1.02.3920 or later. Previous versions will not issue an error
// but will not generate COM-enabled class files.
//

package blaxxunvrml;

import com.ms.com.*;
import com.ms.com.IUnknown;
import com.ms.com.Variant;

// Dual interface Browser
/** @com.interface(iid=BAC5046F-75D9-11D0-9315-0020AFE05CC8, thread=AUTO, type=DUAL) */
public interface Browser extends IUnknown
{
  /** @com.method(vtoffset=4, dispid=1610743808, type=METHOD, name="getName")
      @com.parameters([type=STRING] return) */
  public String getName();

  /** @com.method(vtoffset=5, dispid=1610743809, type=METHOD, name="getVersion")
      @com.parameters([type=STRING] return) */
  public String getVersion();

  /** @com.method(vtoffset=6, dispid=1610743810, type=METHOD, name="getCurrentSpeed")
      @com.parameters([type=R4] return) */
  public float getCurrentSpeed();

  /** @com.method(vtoffset=7, dispid=1610743811, type=METHOD, name="getCurrentFrameRate")
      @com.parameters([type=R4] return) */
  public float getCurrentFrameRate();

  /** @com.method(vtoffset=8, dispid=1610743812, type=METHOD, name="getWorldURL")
      @com.parameters([type=STRING] return) */
  public String getWorldURL();

  /** @com.method(vtoffset=9, dispid=1610743813, type=METHOD, name="replaceWorld")
      @com.parameters([in,iid=BAC50393-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] value) */
  public void replaceWorld(blaxxunvrml.Node value);

  /** @com.method(vtoffset=10, dispid=1610743814, type=METHOD, name="loadURL")
      @com.parameters([in,type=I4] cntUrl, [in,size=1,elementType=STRING,type=ARRAY] url, [in,type=I4] cntParameter, [in,size=1,elementType=STRING,type=ARRAY] parameter) */
  public void loadURL(int cntUrl, String[] url, int cntParameter, String[] parameter);

  /** @com.method(vtoffset=11, dispid=1610743815, type=METHOD, name="setDescription")
      @com.parameters([in,type=STRING] description) */
  public void setDescription(String description);

  /** @com.method(vtoffset=12, dispid=1610743816, type=METHOD, name="createVrmlFromString")
      @com.parameters([in,type=STRING] vrmlSyntax, [iid=BAC50393-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] return) */
  public blaxxunvrml.Node createVrmlFromString(String vrmlSyntax);

  /** @com.method(vtoffset=13, dispid=1610743817, type=METHOD, name="createVrmlFromURL")
      @com.parameters([in,type=STRING] url, [in,iid=BAC50393-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] Node, [in,type=STRING] event) */
  public void createVrmlFromURL(String url, blaxxunvrml.Node Node, String event);

  /** @com.method(vtoffset=14, dispid=1610743818, type=METHOD, name="getNode")
      @com.parameters([in,type=STRING] name, [iid=BAC50393-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] return) */
  public blaxxunvrml.Node getNode(String name);

  /** @com.method(vtoffset=15, dispid=1610743819, type=METHOD, name="addRoute")
      @com.parameters([in,iid=BAC50393-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] fromNode, [in,type=STRING] fromEventOut, [in,iid=BAC50393-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] toNode, [in,type=STRING] toEventIn) */
  public void addRoute(blaxxunvrml.Node fromNode, String fromEventOut, blaxxunvrml.Node toNode, String toEventIn);

  /** @com.method(vtoffset=16, dispid=1610743820, type=METHOD, name="deleteRoute")
      @com.parameters([in,iid=BAC50393-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] fromNode, [in,type=STRING] fromEventOut, [in,iid=BAC50393-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] toNode, [in,type=STRING] toEventIn) */
  public void deleteRoute(blaxxunvrml.Node fromNode, String fromEventOut, blaxxunvrml.Node toNode, String toEventIn);

  /** @com.method(vtoffset=17, dispid=1610743821, type=METHOD, name="createNode")
      @com.parameters([in,type=STRING] nodeclass, [iid=BAC50393-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] return) */
  public blaxxunvrml.Node createNode(String nodeclass);

  /** @com.method(vtoffset=18, dispid=1610743822, type=METHOD, name="getWorld")
      @com.parameters([iid=BAC50393-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] return) */
  public blaxxunvrml.Node getWorld();

  /** @com.method(vtoffset=19, dispid=1610743823, type=METHOD, name="beginUpdate")
      @com.parameters() */
  public void beginUpdate();

  /** @com.method(vtoffset=20, dispid=1610743824, type=METHOD, name="endUpdate")
      @com.parameters() */
  public void endUpdate();

  /** @com.method(vtoffset=21, dispid=1610743825, type=METHOD, name="addNode")
      @com.parameters([in,iid=BAC50393-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] Node) */
  public void addNode(blaxxunvrml.Node Node);

  /** @com.method(vtoffset=22, dispid=1610743826, type=METHOD, name="removeNode")
      @com.parameters([in,iid=BAC50393-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] Node, [type=BOOLEAN] return) */
  public boolean removeNode(blaxxunvrml.Node Node);

  /** @com.method(vtoffset=23, dispid=1610743827, type=METHOD, name="setNodeName")
      @com.parameters([in,iid=BAC50393-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] Node, [in,type=STRING] name) */
  public void setNodeName(blaxxunvrml.Node Node, String name);

  /** @com.method(vtoffset=24, dispid=1610743828, type=METHOD, name="getNodeName")
      @com.parameters([in,iid=BAC50393-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] Node, [type=STRING] return) */
  public String getNodeName(blaxxunvrml.Node Node);

  /** @com.method(vtoffset=25, dispid=1610743829, type=METHOD, name="getEventOut")
      @com.parameters([in,type=STRING] eventOutName, [iid=BAC503F7-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] return) */
  public blaxxunvrml.EventOut getEventOut(String eventOutName);

  /** @com.method(vtoffset=26, dispid=1610743830, type=METHOD, name="getEventIn")
      @com.parameters([in,type=STRING] eventInName, [iid=BAC5039D-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] return) */
  public blaxxunvrml.EventIn getEventIn(String eventInName);

  /** @com.method(vtoffset=27, dispid=1610743831, type=METHOD, name="getControlPathname")
      @com.parameters([type=STRING] return) */
  public String getControlPathname();

  /** @com.method(vtoffset=28, dispid=1610743832, type=METHOD, name="saveWorld")
      @com.parameters([in,type=STRING] fileName, [type=BOOLEAN] return) */
  public boolean saveWorld(String fileName);

  /** @com.method(vtoffset=29, dispid=1610743833, type=METHOD, name="saveViewpoint")
      @com.parameters([in,type=STRING] viewpointName) */
  public void saveViewpoint(String viewpointName);

  /** @com.method(vtoffset=30, dispid=1610743834, type=METHOD, name="setObserver")
      @com.parameters([in,iid=00000000-0000-0000-C000-000000000046,thread=AUTO,type=OBJECT] observer, [in,type=I4] flags, [type=BOOLEAN] return) */
  public boolean setObserver(IUnknown observer, int flags);

  /** @com.method(vtoffset=31, dispid=1610743835, type=METHOD, name="setViewpointByValue")
      @com.parameters([in,size=1,elementType=R4,type=ARRAY] position, [in,size=1,elementType=R4,type=ARRAY] orientation, [in,type=R4] fov, [in,type=BOOLEAN] animate) */
  public void setViewpointByValue(float[] position, float[] orientation, float fov, boolean animate);

  /** @com.method(vtoffset=32, dispid=1610743836, type=METHOD, name="setFullscreen")
      @com.parameters([in,type=BOOLEAN] mode, [type=BOOLEAN] return) */
  public boolean setFullscreen(boolean mode);

  /** @com.method(vtoffset=33, dispid=1610743837, type=METHOD, name="setCollisionDetection")
      @com.parameters([in,type=BOOLEAN] mode) */
  public void setCollisionDetection(boolean mode);

  /** @com.method(vtoffset=34, dispid=1610743838, type=METHOD, name="setRenderMode")
      @com.parameters([in,type=STRING] mode) */
  public void setRenderMode(String mode);

  /** @com.method(vtoffset=35, dispid=1610743839, type=METHOD, name="setNavigationMode")
      @com.parameters([in,type=STRING] mode) */
  public void setNavigationMode(String mode);

  /** @com.method(vtoffset=36, dispid=1610743840, type=METHOD, name="setViewpoint")
      @com.parameters([in,type=STRING] viewpointName) */
  public void setViewpoint(String viewpointName);

  /** @com.method(vtoffset=37, dispid=1610743841, type=METHOD, name="setHeadlight")
      @com.parameters([in,type=BOOLEAN] mode) */
  public void setHeadlight(boolean mode);

  /** @com.method(vtoffset=38, dispid=1610743842, type=METHOD, name="setAvatarHeight")
      @com.parameters([in,type=R4] value) */
  public void setAvatarHeight(float value);

  /** @com.method(vtoffset=39, dispid=1610743843, type=METHOD, name="setCollisionDistance")
      @com.parameters([in,type=R4] value) */
  public void setCollisionDistance(float value);

  /** @com.method(vtoffset=40, dispid=1610743844, type=METHOD, name="setVisibilityLimit")
      @com.parameters([in,type=R4] value) */
  public void setVisibilityLimit(float value);

  /** @com.method(vtoffset=41, dispid=1610743845, type=METHOD, name="setWalkSpeed")
      @com.parameters([in,type=R4] value) */
  public void setWalkSpeed(float value);

  /** @com.method(vtoffset=42, dispid=1610743846, type=METHOD, name="setNavigationPanel")
      @com.parameters([in,type=BOOLEAN] mode) */
  public void setNavigationPanel(boolean mode);

  /** @com.method(vtoffset=43, dispid=1610743847, type=METHOD, name="getNavigationPanel")
      @com.parameters([type=BOOLEAN] return) */
  public boolean getNavigationPanel();


  public static final com.ms.com._Guid iid = new com.ms.com._Guid((int)0xbac5046f, (short)0x75d9, (short)0x11d0, (byte)0x93, (byte)0x15, (byte)0x0, (byte)0x20, (byte)0xaf, (byte)0xe0, (byte)0x5c, (byte)0xc8);
}
