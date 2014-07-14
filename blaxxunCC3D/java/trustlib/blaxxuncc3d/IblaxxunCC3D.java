//
// Auto-generated using JActiveX.EXE 5.00.2924
//   (d:\sdkj31\bin\jactivex /javatlb /d d:\java blaxxuncc3d.tlb)
//
// WARNING: Do not remove the comments that include "@com" directives.
// This source file must be compiled by a @com-aware compiler.
// If you are using the Microsoft Visual J++ compiler, you must use
// version 1.02.3920 or later. Previous versions will not issue an error
// but will not generate COM-enabled class files.
//

package blaxxuncc3d;

import com.ms.com.*;
import com.ms.com.IUnknown;
import com.ms.com.Variant;

// Dispatch-only interface IblaxxunCC3D
/** @com.interface(iid=4B6E3011-6E45-11D0-9309-0020AFE05CC8, thread=AUTO, type=DISPATCH) */
public interface IblaxxunCC3D extends IUnknown
{

  /** @com.method(dispid=4294966771, name="ReadyState", name2="getReadyState", type=PROPGET)
      @com.parameters([type=I4] return) */
  public int getReadyState();


  /** @com.method(dispid=1, name="url", name2="geturl", type=PROPGET)
      @com.parameters([type=STRING] return) */
  public String geturl();


  /** @com.method(dispid=1, name="url", name2="puturl", type=PROPPUT)
      @com.parameters([in,type=STRING] newValue) */
  public void puturl(String newValue);


  /** @com.method(dispid=2, name="renderMode", name2="getrenderMode", type=PROPGET)
      @com.parameters([type=STRING] return) */
  public String getrenderMode();


  /** @com.method(dispid=2, name="renderMode", name2="putrenderMode", type=PROPPUT)
      @com.parameters([in,type=STRING] newValue) */
  public void putrenderMode(String newValue);


  /** @com.method(dispid=3, name="navigationMode", name2="getnavigationMode", type=PROPGET)
      @com.parameters([type=STRING] return) */
  public String getnavigationMode();


  /** @com.method(dispid=3, name="navigationMode", name2="putnavigationMode", type=PROPPUT)
      @com.parameters([in,type=STRING] newValue) */
  public void putnavigationMode(String newValue);


  /** @com.method(dispid=4, name="headlight", name2="getheadlight", type=PROPGET)
      @com.parameters([type=I2] return) */
  public short getheadlight();


  /** @com.method(dispid=4, name="headlight", name2="putheadlight", type=PROPPUT)
      @com.parameters([in,type=I2] newValue) */
  public void putheadlight(short newValue);


  /** @com.method(dispid=5, name="walkSpeed", name2="getwalkSpeed", type=PROPGET)
      @com.parameters([type=R4] return) */
  public float getwalkSpeed();


  /** @com.method(dispid=5, name="walkSpeed", name2="putwalkSpeed", type=PROPPUT)
      @com.parameters([in,type=R4] newValue) */
  public void putwalkSpeed(float newValue);


  /** @com.method(dispid=6, name="animateAllViewpoints", name2="getanimateAllViewpoints", type=PROPGET)
      @com.parameters([type=BOOLEAN] return) */
  public boolean getanimateAllViewpoints();


  /** @com.method(dispid=6, name="animateAllViewpoints", name2="putanimateAllViewpoints", type=PROPPUT)
      @com.parameters([in,type=BOOLEAN] newValue) */
  public void putanimateAllViewpoints(boolean newValue);


  /** @com.method(dispid=7, name="viewpoint", name2="getviewpoint", type=PROPGET)
      @com.parameters([type=STRING] return) */
  public String getviewpoint();


  /** @com.method(dispid=7, name="viewpoint", name2="putviewpoint", type=PROPPUT)
      @com.parameters([in,type=STRING] newValue) */
  public void putviewpoint(String newValue);


  /** @com.method(dispid=8, name="description", name2="getdescription", type=PROPGET)
      @com.parameters([type=STRING] return) */
  public String getdescription();


  /** @com.method(dispid=8, name="description", name2="putdescription", type=PROPPUT)
      @com.parameters([in,type=STRING] newValue) */
  public void putdescription(String newValue);


  /** @com.method(dispid=9, name="world", name2="getworld", type=PROPGET)
      @com.parameters([type=STRING] return) */
  public String getworld();


  /** @com.method(dispid=9, name="world", name2="putworld", type=PROPPUT)
      @com.parameters([in,type=STRING] newValue) */
  public void putworld(String newValue);


  /** @com.method(dispid=10, name="collisionDetection", name2="getcollisionDetection", type=PROPGET)
      @com.parameters([type=BOOLEAN] return) */
  public boolean getcollisionDetection();


  /** @com.method(dispid=10, name="collisionDetection", name2="putcollisionDetection", type=PROPPUT)
      @com.parameters([in,type=BOOLEAN] newValue) */
  public void putcollisionDetection(boolean newValue);


  /** @com.method(dispid=11, name="avatarHeight", name2="getavatarHeight", type=PROPGET)
      @com.parameters([type=R4] return) */
  public float getavatarHeight();


  /** @com.method(dispid=11, name="avatarHeight", name2="putavatarHeight", type=PROPPUT)
      @com.parameters([in,type=R4] newValue) */
  public void putavatarHeight(float newValue);


  /** @com.method(dispid=12, name="collisionDistance", name2="getcollisionDistance", type=PROPGET)
      @com.parameters([type=R4] return) */
  public float getcollisionDistance();


  /** @com.method(dispid=12, name="collisionDistance", name2="putcollisionDistance", type=PROPPUT)
      @com.parameters([in,type=R4] newValue) */
  public void putcollisionDistance(float newValue);


  /** @com.method(dispid=13, name="visibilityLimit", name2="getvisibilityLimit", type=PROPGET)
      @com.parameters([type=R4] return) */
  public float getvisibilityLimit();


  /** @com.method(dispid=13, name="visibilityLimit", name2="putvisibilityLimit", type=PROPPUT)
      @com.parameters([in,type=R4] newValue) */
  public void putvisibilityLimit(float newValue);


  /** @com.method(dispid=14, name="gravity", name2="getgravity", type=PROPGET)
      @com.parameters([type=BOOLEAN] return) */
  public boolean getgravity();


  /** @com.method(dispid=14, name="gravity", name2="putgravity", type=PROPPUT)
      @com.parameters([in,type=BOOLEAN] newValue) */
  public void putgravity(boolean newValue);

  /** @com.method(dispid=15, type=METHOD, name="OnNextViewpoint", returntype=VOID)
      @com.parameters() */
  public void OnNextViewpoint();

  /** @com.method(dispid=16, type=METHOD, name="OnPrevViewpoint", returntype=VOID)
      @com.parameters() */
  public void OnPrevViewpoint();

  /** @com.method(dispid=17, type=METHOD, name="getName", returntype=VOID)
      @com.parameters([type=STRING] return) */
  public String getName();

  /** @com.method(dispid=18, type=METHOD, name="getVersion", returntype=VOID)
      @com.parameters([type=STRING] return) */
  public String getVersion();

  /** @com.method(dispid=19, type=METHOD, name="getCurrentSpeed", returntype=VOID)
      @com.parameters([type=R4] return) */
  public float getCurrentSpeed();

  /** @com.method(dispid=20, type=METHOD, name="getCurrentFrameRate", returntype=VOID)
      @com.parameters([type=R4] return) */
  public float getCurrentFrameRate();

  /** @com.method(dispid=21, type=METHOD, name="getWorldURL", returntype=VOID)
      @com.parameters([type=STRING] return) */
  public String getWorldURL();

  /** @com.method(dispid=22, type=METHOD, name="replaceWorld", returntype=VOID)
      @com.parameters([in,type=OBJECT] nodes) */
  public void replaceWorld(IUnknown nodes);

  /** @com.method(dispid=23, type=METHOD, name="loadURL", returntype=VOID)
      @com.parameters([in,type=STRING] url, [in,type=STRING] parameter) */
  public void loadURL(String url, String parameter);

  /** @com.method(dispid=24, type=METHOD, name="setDescription", returntype=VOID)
      @com.parameters([in,type=STRING] description) */
  public void setDescription(String description);

  /** @com.method(dispid=25, type=METHOD, name="createVrmlFromString", returntype=VOID)
      @com.parameters([in,type=STRING] vrmlSyntax, [type=OBJECT] return) */
  public IUnknown createVrmlFromString(String vrmlSyntax);

  /** @com.method(dispid=26, type=METHOD, name="createVrmlFromURL", returntype=VOID)
      @com.parameters([in,type=STRING] url, [in,type=DISPATCH] node, [in,type=STRING] event) */
  public void createVrmlFromURL(String url, Object node, String event);

  /** @com.method(dispid=27, type=METHOD, name="getNode", returntype=VOID)
      @com.parameters([in,type=STRING] name, [type=OBJECT] return) */
  public IUnknown getNode(String name);

  /** @com.method(dispid=28, type=METHOD, name="addRoute", returntype=VOID)
      @com.parameters([in,type=OBJECT] fromNode, [in,type=STRING] fromEventOut, [in,type=OBJECT] toNode, [in,type=STRING] toEventIn) */
  public void addRoute(IUnknown fromNode, String fromEventOut, IUnknown toNode, String toEventIn);

  /** @com.method(dispid=29, type=METHOD, name="deleteRoute", returntype=VOID)
      @com.parameters([in,type=OBJECT] fromNode, [in,type=STRING] fromEventOut, [in,type=OBJECT] toNode, [in,type=STRING] toEventIn) */
  public void deleteRoute(IUnknown fromNode, String fromEventOut, IUnknown toNode, String toEventIn);

  /** @com.method(dispid=30, type=METHOD, name="createNode", returntype=VOID)
      @com.parameters([in,type=STRING] nodeclass, [type=OBJECT] return) */
  public IUnknown createNode(String nodeclass);

  /** @com.method(dispid=31, type=METHOD, name="addNode", returntype=VOID)
      @com.parameters([in,type=OBJECT] node) */
  public void addNode(IUnknown node);

  /** @com.method(dispid=32, type=METHOD, name="getWorld", returntype=VOID)
      @com.parameters([type=DISPATCH] return) */
  public Object getWorld();

  /** @com.method(dispid=33, type=METHOD, name="beginUpdate", returntype=VOID)
      @com.parameters() */
  public void beginUpdate();

  /** @com.method(dispid=34, type=METHOD, name="endUpdate", returntype=VOID)
      @com.parameters() */
  public void endUpdate();

  /** @com.method(dispid=35, type=METHOD, name="setTimerInterval", returntype=VOID)
      @com.parameters([in,type=I4] interval, [type=I4] return) */
  public int setTimerInterval(int interval);

  /** @com.method(dispid=36, type=METHOD, name="isSceneLoaded", returntype=VOID)
      @com.parameters([type=BOOLEAN] return) */
  public boolean isSceneLoaded();

  /** @com.method(dispid=37, type=METHOD, name="setNodeName", returntype=VOID)
      @com.parameters([in,type=OBJECT] node, [in,type=STRING] name) */
  public void setNodeName(IUnknown node, String name);

  /** @com.method(dispid=38, type=METHOD, name="getNodeName", returntype=VOID)
      @com.parameters([in,type=OBJECT] node, [type=STRING] return) */
  public String getNodeName(IUnknown node);

  /** @com.method(dispid=39, type=METHOD, name="getEventOut", returntype=VOID)
      @com.parameters([in,type=STRING] eventOutName, [type=OBJECT] return) */
  public IUnknown getEventOut(String eventOutName);

  /** @com.method(dispid=40, type=METHOD, name="getEventIn", returntype=VOID)
      @com.parameters([in,type=STRING] eventInName, [type=OBJECT] return) */
  public IUnknown getEventIn(String eventInName);

  /** @com.method(dispid=41, type=METHOD, name="getControlPathname", returntype=VOID)
      @com.parameters([type=STRING] return) */
  public String getControlPathname();

  /** @com.method(dispid=42, type=METHOD, name="saveWorld", returntype=VOID)
      @com.parameters([in,type=STRING] fileName, [type=BOOLEAN] return) */
  public boolean saveWorld(String fileName);

  /** @com.method(dispid=43, type=METHOD, name="saveViewpoint", returntype=VOID)
      @com.parameters([in,type=STRING] viewpointName) */
  public void saveViewpoint(String viewpointName);

  /** @com.method(dispid=44, type=METHOD, name="setFullscreen", returntype=VOID)
      @com.parameters([in,type=BOOLEAN] mode, [type=BOOLEAN] return) */
  public boolean setFullscreen(boolean mode);

  /** @com.method(dispid=45, type=METHOD, name="removeNode", returntype=VOID)
      @com.parameters([in,type=OBJECT] node, [type=BOOLEAN] return) */
  public boolean removeNode(IUnknown node);

  /** @com.method(dispid=46, type=METHOD, name="setObserver", returntype=VOID)
      @com.parameters([in,type=OBJECT] observer, [in,type=I4] flags, [type=BOOLEAN] return) */
  public boolean setObserver(IUnknown observer, int flags);

  /** @com.method(dispid=47, type=METHOD, name="loadURLfromFile", returntype=VOID)
      @com.parameters([in,type=STRING] url, [in,type=STRING] mimeType, [in,type=STRING] fileName, [type=BOOLEAN] return) */
  public boolean loadURLfromFile(String url, String mimeType, String fileName);

  /** @com.method(dispid=48, type=METHOD, name="onUrlNotify", returntype=VOID)
      @com.parameters([in,type=STRING] url, [in,type=STRING] mimeType, [in,type=STRING] fileName, [in,type=I4] reason) */
  public void onUrlNotify(String url, String mimeType, String fileName, int reason);

  /** @com.method(dispid=49, type=METHOD, name="setObserverWnd", returntype=VOID)
      @com.parameters([in,type=I4] hwndHandle, [type=BOOLEAN] return) */
  public boolean setObserverWnd(int hwndHandle);

  /** @com.method(dispid=50, type=METHOD, name="getUiMode", returntype=VOID)
      @com.parameters([type=I4] return) */
  public int getUiMode();

  /** @com.method(dispid=51, type=METHOD, name="setUiMode", returntype=VOID)
      @com.parameters([in,type=I4] newMode) */
  public void setUiMode(int newMode);

  /** @com.method(dispid=52, type=METHOD, name="openPreferences", returntype=VOID)
      @com.parameters([in,type=I4] flags) */
  public void openPreferences(int flags);

  /** @com.method(dispid=53, type=METHOD, name="playSound", returntype=VOID)
      @com.parameters([in,type=STRING] soundFilename) */
  public void playSound(String soundFilename);

  /** @com.method(dispid=54, type=METHOD, name="loadURLfromFile2", returntype=VOID)
      @com.parameters([in,type=STRING] url, [in,type=STRING] mimeType, [in,type=I4] lastModified, [in,type=I4] size, [in,type=STRING] fileName, [type=BOOLEAN] return) */
  public boolean loadURLfromFile2(String url, String mimeType, int lastModified, int size, String fileName);

  /** @com.method(dispid=55, type=METHOD, name="onUrlNotify2", returntype=VOID)
      @com.parameters([in,type=STRING] url, [in,type=STRING] mimeType, [in,type=I4] lastModified, [in,type=I4] size, [in,type=STRING] fileName, [in,type=I4] reason) */
  public void onUrlNotify2(String url, String mimeType, int lastModified, int size, String fileName, int reason);

  /** @com.method(dispid=56, type=METHOD, name="print", returntype=VOID)
      @com.parameters([in,type=STRING] message) */
  public void print(String message);

  /** @com.method(dispid=57, type=METHOD, name="setViewpointByValue", returntype=VOID)
      @com.parameters([in,type=R4] positionX, [in,type=R4] positionY, [in,type=R4] positionZ, [in,type=R4] orientationX, [in,type=R4] orientationY, [in,type=R4] orientationZ, [in,type=R4] orientationAngle, [in,type=I4] mode) */
  public void setViewpointByValue(float positionX, float positionY, float positionZ, float orientationX, float orientationY, float orientationZ, float orientationAngle, int mode);

  /** @com.method(dispid=58, type=METHOD, name="getViewpointByValue", returntype=VOID)
      @com.parameters([in,elementType=R4,type=ARRAY] positionX, [in,elementType=R4,type=ARRAY] positionY, [in,elementType=R4,type=ARRAY] positionZ, [in,elementType=R4,type=ARRAY] orientationX, [in,elementType=R4,type=ARRAY] orientationY, [in,elementType=R4,type=ARRAY] orientationZ, [in,elementType=R4,type=ARRAY] orientationAngle, [in,type=I4] mode) */
  public void getViewpointByValue(float[] positionX, float[] positionY, float[] positionZ, float[] orientationX, float[] orientationY, float[] orientationZ, float[] orientationAngle, int mode);

  /** @com.method(dispid=59, type=METHOD, name="getNodeBoundingBox", returntype=VOID)
      @com.parameters([in,type=OBJECT] node, [in,type=I4] mode, [in,elementType=R4,type=ARRAY] xmin, [in,elementType=R4,type=ARRAY] ymin, [in,elementType=R4,type=ARRAY] zmin, [in,elementType=R4,type=ARRAY] xmax, [in,elementType=R4,type=ARRAY] ymax, [in,elementType=R4,type=ARRAY] zmax, [type=BOOLEAN] return) */
  public boolean getNodeBoundingBox(IUnknown node, int mode, float[] xmin, float[] ymin, float[] zmin, float[] xmax, float[] ymax, float[] zmax);

  /** @com.method(dispid=60, type=METHOD, name="setUrl2", returntype=VOID)
      @com.parameters([in,type=STRING] url, [in,type=I4] lastModified, [in,type=I4] mode) */
  public void setUrl2(String url, int lastModified, int mode);

  /** @com.method(dispid=61, type=METHOD, name="getInterfaceVersion", returntype=VOID)
      @com.parameters([type=I4] return) */
  public int getInterfaceVersion();

  /** @com.method(dispid=62, type=METHOD, name="setThirdPersonView", returntype=VOID)
      @com.parameters([in,type=BOOLEAN] mode) */
  public void setThirdPersonView(boolean mode);

  /** @com.method(dispid=63, type=METHOD, name="getThirdPersonView", returntype=VOID)
      @com.parameters([type=BOOLEAN] return) */
  public boolean getThirdPersonView();

  /** @com.method(dispid=64, type=METHOD, name="computeRayHit", returntype=VOID)
      @com.parameters([in,type=OBJECT] startNode, [in,type=R4] startx, [in,type=R4] starty, [in,type=R4] startz, [in,type=R4] endx, [in,type=R4] endy, [in,type=R4] endz, [in,type=I4] mode, [type=OBJECT] return) */
  public IUnknown computeRayHit(IUnknown startNode, float startx, float starty, float startz, float endx, float endy, float endz, int mode);

  /** @com.method(dispid=65, type=METHOD, name="setMyAvatarNode", returntype=VOID)
      @com.parameters([in,type=OBJECT] node) */
  public void setMyAvatarNode(IUnknown node);

  /** @com.method(dispid=66, type=METHOD, name="getMyAvatarNode", returntype=VOID)
      @com.parameters([type=OBJECT] return) */
  public IUnknown getMyAvatarNode();

  /** @com.method(dispid=67, type=METHOD, name="setMyAvatarURL", returntype=VOID)
      @com.parameters([in,type=STRING] url) */
  public void setMyAvatarURL(String url);

  /** @com.method(dispid=68, type=METHOD, name="getMyAvatarURL", returntype=VOID)
      @com.parameters([type=STRING] return) */
  public String getMyAvatarURL();

  /** @com.method(dispid=69, type=METHOD, name="setViewpointFollow", returntype=VOID)
      @com.parameters([in,type=OBJECT] node, [in,type=R4] refX, [in,type=R4] refY, [in,type=R4] refZ, [in,type=I4] mode, [type=BOOLEAN] return) */
  public boolean setViewpointFollow(IUnknown node, float refX, float refY, float refZ, int mode);

  /** @com.method(dispid=70, type=METHOD, name="setNodeEventIn", returntype=VOID)
      @com.parameters([in,type=STRING] nodeName, [in,type=STRING] eventInName, [in,type=STRING] value, [type=BOOLEAN] return) */
  public boolean setNodeEventIn(String nodeName, String eventInName, String value);

  /** @com.method(dispid=71, type=METHOD, name="getNodeEventOut", returntype=VOID)
      @com.parameters([in,type=STRING] nodeName, [in,type=STRING] eventOutName, [type=STRING] return) */
  public String getNodeEventOut(String nodeName, String eventOutName);

  /** @com.method(dispid=72, type=METHOD, name="getObject", returntype=VOID)
      @com.parameters([in,type=I4] objectId, [type=OBJECT] return) */
  public IUnknown getObject(int objectId);

  /** @com.method(dispid=73, type=METHOD, name="setNavigationPanel", returntype=VOID)
      @com.parameters([in,type=BOOLEAN] mode) */
  public void setNavigationPanel(boolean mode);

  /** @com.method(dispid=74, type=METHOD, name="getNavigationPanel", returntype=VOID)
      @com.parameters([type=BOOLEAN] return) */
  public boolean getNavigationPanel();

  /** @com.method(dispid=4294966744, type=METHOD, name="AboutBox", returntype=VOID)
      @com.parameters() */
  public void AboutBox();


  public static final com.ms.com._Guid iid = new com.ms.com._Guid((int)0x4b6e3011, (short)0x6e45, (short)0x11d0, (byte)0x93, (byte)0x9, (byte)0x0, (byte)0x20, (byte)0xaf, (byte)0xe0, (byte)0x5c, (byte)0xc8);
}
