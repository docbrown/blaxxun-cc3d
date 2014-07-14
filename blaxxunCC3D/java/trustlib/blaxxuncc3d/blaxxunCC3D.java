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

/** @com.class(classid=4B6E3013-6E45-11D0-9309-0020AFE05CC8,DynamicCasts) */
public class blaxxunCC3D implements IUnknown,com.ms.com.NoAutoScripting,blaxxuncc3d.IblaxxunCC3D
{

  /** @com.method()
      @hidden */
  public native int getReadyState();


  /** @com.method()
      @hidden */
  public native String geturl();


  /** @com.method()
      @hidden */
  public native void puturl(String newValue);


  /** @com.method()
      @hidden */
  public native String getrenderMode();


  /** @com.method()
      @hidden */
  public native void putrenderMode(String newValue);


  /** @com.method()
      @hidden */
  public native String getnavigationMode();


  /** @com.method()
      @hidden */
  public native void putnavigationMode(String newValue);


  /** @com.method()
      @hidden */
  public native short getheadlight();


  /** @com.method()
      @hidden */
  public native void putheadlight(short newValue);


  /** @com.method()
      @hidden */
  public native float getwalkSpeed();


  /** @com.method()
      @hidden */
  public native void putwalkSpeed(float newValue);


  /** @com.method()
      @hidden */
  public native boolean getanimateAllViewpoints();


  /** @com.method()
      @hidden */
  public native void putanimateAllViewpoints(boolean newValue);


  /** @com.method()
      @hidden */
  public native String getviewpoint();


  /** @com.method()
      @hidden */
  public native void putviewpoint(String newValue);


  /** @com.method()
      @hidden */
  public native String getdescription();


  /** @com.method()
      @hidden */
  public native void putdescription(String newValue);


  /** @com.method()
      @hidden */
  public native String getworld();


  /** @com.method()
      @hidden */
  public native void putworld(String newValue);


  /** @com.method()
      @hidden */
  public native boolean getcollisionDetection();


  /** @com.method()
      @hidden */
  public native void putcollisionDetection(boolean newValue);


  /** @com.method()
      @hidden */
  public native float getavatarHeight();


  /** @com.method()
      @hidden */
  public native void putavatarHeight(float newValue);


  /** @com.method()
      @hidden */
  public native float getcollisionDistance();


  /** @com.method()
      @hidden */
  public native void putcollisionDistance(float newValue);


  /** @com.method()
      @hidden */
  public native float getvisibilityLimit();


  /** @com.method()
      @hidden */
  public native void putvisibilityLimit(float newValue);


  /** @com.method()
      @hidden */
  public native boolean getgravity();


  /** @com.method()
      @hidden */
  public native void putgravity(boolean newValue);

  /** @com.method()
      @hidden */
  public native void OnNextViewpoint();

  /** @com.method()
      @hidden */
  public native void OnPrevViewpoint();

  /** @com.method()
      @hidden */
  public native String getName();

  /** @com.method()
      @hidden */
  public native String getVersion();

  /** @com.method()
      @hidden */
  public native float getCurrentSpeed();

  /** @com.method()
      @hidden */
  public native float getCurrentFrameRate();

  /** @com.method()
      @hidden */
  public native String getWorldURL();

  /** @com.method()
      @hidden */
  public native void replaceWorld(IUnknown nodes);

  /** @com.method()
      @hidden */
  public native void loadURL(String url, String parameter);

  /** @com.method()
      @hidden */
  public native void setDescription(String description);

  /** @com.method()
      @hidden */
  public native IUnknown createVrmlFromString(String vrmlSyntax);

  /** @com.method()
      @hidden */
  public native void createVrmlFromURL(String url, Object node, String event);

  /** @com.method()
      @hidden */
  public native IUnknown getNode(String name);

  /** @com.method()
      @hidden */
  public native void addRoute(IUnknown fromNode, String fromEventOut, IUnknown toNode, String toEventIn);

  /** @com.method()
      @hidden */
  public native void deleteRoute(IUnknown fromNode, String fromEventOut, IUnknown toNode, String toEventIn);

  /** @com.method()
      @hidden */
  public native IUnknown createNode(String nodeclass);

  /** @com.method()
      @hidden */
  public native void addNode(IUnknown node);

  /** @com.method()
      @hidden */
  public native Object getWorld();

  /** @com.method()
      @hidden */
  public native void beginUpdate();

  /** @com.method()
      @hidden */
  public native void endUpdate();

  /** @com.method()
      @hidden */
  public native int setTimerInterval(int interval);

  /** @com.method()
      @hidden */
  public native boolean isSceneLoaded();

  /** @com.method()
      @hidden */
  public native void setNodeName(IUnknown node, String name);

  /** @com.method()
      @hidden */
  public native String getNodeName(IUnknown node);

  /** @com.method()
      @hidden */
  public native IUnknown getEventOut(String eventOutName);

  /** @com.method()
      @hidden */
  public native IUnknown getEventIn(String eventInName);

  /** @com.method()
      @hidden */
  public native String getControlPathname();

  /** @com.method()
      @hidden */
  public native boolean saveWorld(String fileName);

  /** @com.method()
      @hidden */
  public native void saveViewpoint(String viewpointName);

  /** @com.method()
      @hidden */
  public native boolean setFullscreen(boolean mode);

  /** @com.method()
      @hidden */
  public native boolean removeNode(IUnknown node);

  /** @com.method()
      @hidden */
  public native boolean setObserver(IUnknown observer, int flags);

  /** @com.method()
      @hidden */
  public native boolean loadURLfromFile(String url, String mimeType, String fileName);

  /** @com.method()
      @hidden */
  public native void onUrlNotify(String url, String mimeType, String fileName, int reason);

  /** @com.method()
      @hidden */
  public native boolean setObserverWnd(int hwndHandle);

  /** @com.method()
      @hidden */
  public native int getUiMode();

  /** @com.method()
      @hidden */
  public native void setUiMode(int newMode);

  /** @com.method()
      @hidden */
  public native void openPreferences(int flags);

  /** @com.method()
      @hidden */
  public native void playSound(String soundFilename);

  /** @com.method()
      @hidden */
  public native boolean loadURLfromFile2(String url, String mimeType, int lastModified, int size, String fileName);

  /** @com.method()
      @hidden */
  public native void onUrlNotify2(String url, String mimeType, int lastModified, int size, String fileName, int reason);

  /** @com.method()
      @hidden */
  public native void print(String message);

  /** @com.method()
      @hidden */
  public native void setViewpointByValue(float positionX, float positionY, float positionZ, float orientationX, float orientationY, float orientationZ, float orientationAngle, int mode);

  /** @com.method()
      @hidden */
  public native void getViewpointByValue(float[] positionX, float[] positionY, float[] positionZ, float[] orientationX, float[] orientationY, float[] orientationZ, float[] orientationAngle, int mode);

  /** @com.method()
      @hidden */
  public native boolean getNodeBoundingBox(IUnknown node, int mode, float[] xmin, float[] ymin, float[] zmin, float[] xmax, float[] ymax, float[] zmax);

  /** @com.method()
      @hidden */
  public native void setUrl2(String url, int lastModified, int mode);

  /** @com.method()
      @hidden */
  public native int getInterfaceVersion();

  /** @com.method()
      @hidden */
  public native void setThirdPersonView(boolean mode);

  /** @com.method()
      @hidden */
  public native boolean getThirdPersonView();

  /** @com.method()
      @hidden */
  public native IUnknown computeRayHit(IUnknown startNode, float startx, float starty, float startz, float endx, float endy, float endz, int mode);

  /** @com.method()
      @hidden */
  public native void setMyAvatarNode(IUnknown node);

  /** @com.method()
      @hidden */
  public native IUnknown getMyAvatarNode();

  /** @com.method()
      @hidden */
  public native void setMyAvatarURL(String url);

  /** @com.method()
      @hidden */
  public native String getMyAvatarURL();

  /** @com.method()
      @hidden */
  public native boolean setViewpointFollow(IUnknown node, float refX, float refY, float refZ, int mode);

  /** @com.method()
      @hidden */
  public native boolean setNodeEventIn(String nodeName, String eventInName, String value);

  /** @com.method()
      @hidden */
  public native String getNodeEventOut(String nodeName, String eventOutName);

  /** @com.method()
      @hidden */
  public native IUnknown getObject(int objectId);

  /** @com.method()
      @hidden */
  public native void setNavigationPanel(boolean mode);

  /** @com.method()
      @hidden */
  public native boolean getNavigationPanel();

  /** @com.method()
      @hidden */
  public native void AboutBox();


  public static final com.ms.com._Guid clsid = new com.ms.com._Guid((int)0x4b6e3013, (short)0x6e45, (short)0x11d0, (byte)0x93, (byte)0x9, (byte)0x0, (byte)0x20, (byte)0xaf, (byte)0xe0, (byte)0x5c, (byte)0xc8);
}
