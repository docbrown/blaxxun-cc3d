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

// Dual interface ScriptDefault
/** @com.interface(iid=00020400-0000-0000-C000-000000000046, thread=AUTO, type=DUAL) */
public interface ScriptDefault extends blaxxunvrml.Script
{
  /** @com.method(vtoffset=4, dispid=1610743808, type=METHOD, name="setContainer")
      @com.parameters([in,iid=BAC50393-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] container) */
  public void setContainer(blaxxunvrml.Node container);

  /** @com.method(vtoffset=5, dispid=1610743809, type=METHOD, name="loadScriptObject")
      @com.parameters([in,type=STRING] urlData) */
  public void loadScriptObject(String urlData);

  /** @com.method(vtoffset=6, dispid=1610743810, type=METHOD, name="initialize")
      @com.parameters() */
  public void initialize();

  /** @com.method(vtoffset=7, dispid=1610743811, type=METHOD, name="shutdown")
      @com.parameters() */
  public void shutdown();

  /** @com.method(vtoffset=8, dispid=1610743812, type=METHOD, name="processEvent")
      @com.parameters([in,type=STRING] name, [in,type=I4] eventId, [in,iid=BAC503F7-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] value, [in,type=R8] timeStamp) */
  public void processEvent(String name, int eventId, blaxxunvrml.EventOut value, double timeStamp);

  /** @com.method(vtoffset=9, dispid=1610743813, type=METHOD, name="eventsProcessed")
      @com.parameters() */
  public void eventsProcessed();


  public static final com.ms.com._Guid iid = new com.ms.com._Guid((int)0x20400, (short)0x0, (short)0x0, (byte)0xc0, (byte)0x0, (byte)0x0, (byte)0x0, (byte)0x0, (byte)0x0, (byte)0x0, (byte)0x46);
}
