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

// Dual interface BrowserObserver
/** @com.interface(iid=B8B46AC1-185B-11D1-9447-0020AFE05CC8, thread=AUTO, type=DUAL) */
public interface BrowserObserver extends IUnknown
{
  /** @com.method(vtoffset=4, dispid=1610743808, type=METHOD, name="OnStatusMessage")
      @com.parameters([in,type=STRING] message, [in,type=I4] messageType) */
  public void OnStatusMessage(String message, int messageType);

  /** @com.method(vtoffset=5, dispid=1610743809, type=METHOD, name="OnUrlError")
      @com.parameters([in,type=STRING] url, [in,type=STRING] message, [in,type=I4] errorCode) */
  public void OnUrlError(String url, String message, int errorCode);

  /** @com.method(vtoffset=6, dispid=1610743810, type=METHOD, name="OnLoadUrl")
      @com.parameters([in,type=STRING] url, [in,type=STRING] targetFrame) */
  public void OnLoadUrl(String url, String targetFrame);

  /** @com.method(vtoffset=7, dispid=1610743811, type=METHOD, name="OnLoadAnchor")
      @com.parameters([in,type=STRING] description, [in,type=I4] numUrl, [in,size=1,elementType=STRING,type=ARRAY] url, [in,type=I4] numParameter, [in,size=1,elementType=STRING,type=ARRAY] parameter) */
  public void OnLoadAnchor(String description, int numUrl, String[] url, int numParameter, String[] parameter);

  /** @com.method(vtoffset=8, dispid=1610743812, type=METHOD, name="OnViewpointChanged")
      @com.parameters([in,size=1,elementType=R4,type=ARRAY] position, [in,size=1,elementType=R4,type=ARRAY] orientation) */
  public void OnViewpointChanged(float[] position, float[] orientation);

  /** @com.method(vtoffset=9, dispid=1610743813, type=METHOD, name="OnSceneChanged")
      @com.parameters([in,type=STRING] url) */
  public void OnSceneChanged(String url);

  /** @com.method(vtoffset=10, dispid=1610743814, type=METHOD, name="LoadUrlToFile")
      @com.parameters([in,type=STRING] url) */
  public void LoadUrlToFile(String url);


  public static final com.ms.com._Guid iid = new com.ms.com._Guid((int)0xb8b46ac1, (short)0x185b, (short)0x11d1, (byte)0x94, (byte)0x47, (byte)0x0, (byte)0x20, (byte)0xaf, (byte)0xe0, (byte)0x5c, (byte)0xc8);
}
