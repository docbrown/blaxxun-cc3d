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

// Dual interface EventInSFImage
/** @com.interface(iid=BAC503DE-75D9-11D0-9315-0020AFE05CC8, thread=AUTO, type=DUAL) */
public interface EventInSFImage extends blaxxunvrml.EventIn
{
  /** @com.method(vtoffset=4, dispid=1610743808, type=METHOD, name="getType")
      @com.parameters([type=I4] return) */
  public int getType();

  /** @com.method(vtoffset=5, dispid=1610743809, type=METHOD, name="setValueFromString")
      @com.parameters([in,type=STRING] value) */
  public void setValueFromString(String value);

  /** @com.method(vtoffset=6, dispid=1610809344, type=METHOD, name="setValue")
      @com.parameters([in,type=I4] width, [in,type=I4] height, [in,type=I4] numComponents, [in,type=I4] cnt, [in,size=1,elementType=U1,type=ARRAY] pixels) */
  public void setValue(int width, int height, int numComponents, int cnt, byte[] pixels);


  public static final com.ms.com._Guid iid = new com.ms.com._Guid((int)0xbac503de, (short)0x75d9, (short)0x11d0, (byte)0x93, (byte)0x15, (byte)0x0, (byte)0x20, (byte)0xaf, (byte)0xe0, (byte)0x5c, (byte)0xc8);
}
