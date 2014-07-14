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

// Dual interface EventOutObserver
/** @com.interface(iid=BAC50406-75D9-11D0-9315-0020AFE05CC8, thread=AUTO, type=DUAL) */
public interface EventOutObserver extends IUnknown
{
  /** @com.method(vtoffset=4, dispid=1610743808, type=METHOD, name="callback")
      @com.parameters([in,iid=BAC503F7-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] value, [in,type=R8] timeStamp, [in,iid=00020400-0000-0000-C000-000000000046,thread=AUTO,type=DISPATCH] userData) */
  public void callback(blaxxunvrml.EventOut value, double timeStamp, Object userData);


  public static final com.ms.com._Guid iid = new com.ms.com._Guid((int)0xbac50406, (short)0x75d9, (short)0x11d0, (byte)0x93, (byte)0x15, (byte)0x0, (byte)0x20, (byte)0xaf, (byte)0xe0, (byte)0x5c, (byte)0xc8);
}
