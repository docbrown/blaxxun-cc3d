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

// Dispatch-only interface IblaxxunCC3DEventsDefault
/** @com.interface(iid=00020400-0000-0000-C000-000000000046, thread=AUTO, type=DISPATCH) */
public interface IblaxxunCC3DEventsDefault extends blaxxuncc3d.IblaxxunCC3DEvents
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


  public static final com.ms.com._Guid iid = new com.ms.com._Guid((int)0x20400, (short)0x0, (short)0x0, (byte)0xc0, (byte)0x0, (byte)0x0, (byte)0x0, (byte)0x0, (byte)0x0, (byte)0x0, (byte)0x46);
}
