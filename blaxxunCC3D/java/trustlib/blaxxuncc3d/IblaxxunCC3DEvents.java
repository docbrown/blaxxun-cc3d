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

// Dispatch-only interface IblaxxunCC3DEvents
/** @com.interface(iid=4B6E3012-6E45-11D0-9309-0020AFE05CC8, thread=AUTO, type=DISPATCH) */
public interface IblaxxunCC3DEvents extends IUnknown
{
  /** @com.method(dispid=4294966687, type=METHOD, name="ReadyStateChange", returntype=VOID)
      @com.parameters() */
  public void ReadyStateChange();

  /** @com.method(dispid=1, type=METHOD, name="OnSceneChanged", returntype=VOID)
      @com.parameters([in,type=STRING] url) */
  public void OnSceneChanged(String url);

  /** @com.method(dispid=2, type=METHOD, name="OnEvent", returntype=VOID)
      @com.parameters([in,type=STRING] name, [in,type=STRING] value, [in,type=R8] timeStamp) */
  public void OnEvent(String name, String value, double timeStamp);

  /** @com.method(dispid=1958, type=METHOD, name="OnProgress", returntype=VOID)
      @com.parameters([in,type=I4] amount) */
  public void OnProgress(int amount);


  public static final com.ms.com._Guid iid = new com.ms.com._Guid((int)0x4b6e3012, (short)0x6e45, (short)0x11d0, (byte)0x93, (byte)0x9, (byte)0x0, (byte)0x20, (byte)0xaf, (byte)0xe0, (byte)0x5c, (byte)0xc8);
}
