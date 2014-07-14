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

// Dual interface EventInMFTimeDefault
/** @com.interface(iid=00020400-0000-0000-C000-000000000046, thread=AUTO, type=DUAL) */
public interface EventInMFTimeDefault extends blaxxunvrml.EventInMFTime
{
  /** @com.method(vtoffset=4, dispid=1610743808, type=METHOD, name="getType")
      @com.parameters([type=I4] return) */
  public int getType();

  /** @com.method(vtoffset=5, dispid=1610743809, type=METHOD, name="setValueFromString")
      @com.parameters([in,type=STRING] value) */
  public void setValueFromString(String value);

  /** @com.method(vtoffset=6, dispid=1610809344, type=METHOD, name="setValue")
      @com.parameters([in,type=I4] cnt, [in,size=1,elementType=R8,type=ARRAY] value) */
  public void setValue(int cnt, double[] value);

  /** @com.method(vtoffset=7, dispid=1610809345, type=METHOD, name="set1Value")
      @com.parameters([in,type=I4] index, [in,type=R8] value) */
  public void set1Value(int index, double value);


  public static final com.ms.com._Guid iid = new com.ms.com._Guid((int)0x20400, (short)0x0, (short)0x0, (byte)0xc0, (byte)0x0, (byte)0x0, (byte)0x0, (byte)0x0, (byte)0x0, (byte)0x0, (byte)0x46);
}
