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

// Dual interface NodeDefault
/** @com.interface(iid=00020400-0000-0000-C000-000000000046, thread=AUTO, type=DUAL) */
public interface NodeDefault extends blaxxunvrml.Node
{
  /** @com.method(vtoffset=4, dispid=1610743808, type=METHOD, name="getType")
      @com.parameters([type=STRING] return) */
  public String getType();

  /** @com.method(vtoffset=5, dispid=1610743809, type=METHOD, name="getEventIn")
      @com.parameters([in,type=STRING] name, [iid=BAC5039D-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] return) */
  public blaxxunvrml.EventIn getEventIn(String name);

  /** @com.method(vtoffset=6, dispid=1610743810, type=METHOD, name="getEventOut")
      @com.parameters([in,type=STRING] name, [iid=BAC503F7-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] return) */
  public blaxxunvrml.EventOut getEventOut(String name);

  /** @com.method(vtoffset=7, dispid=1610743811, type=METHOD, name="getField")
      @com.parameters([in,type=STRING] name, [iid=BAC50398-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] return) */
  public blaxxunvrml.Field getField(String name);

  /** @com.method(vtoffset=8, dispid=1610743812, type=METHOD, name="getExposedField")
      @com.parameters([in,type=STRING] name, [iid=BAC50398-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] return) */
  public blaxxunvrml.Field getExposedField(String name);

  /** @com.method(vtoffset=9, dispid=1610743813, type=METHOD, name="getBrowser")
      @com.parameters([iid=BAC5046F-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] return) */
  public blaxxunvrml.Browser getBrowser();

  /** @com.method(vtoffset=10, dispid=1610743814, type=METHOD, name="getName")
      @com.parameters([type=STRING] return) */
  public String getName();

  /** @com.method(vtoffset=11, dispid=1610743815, type=METHOD, name="setName")
      @com.parameters([in,type=STRING] name) */
  public void setName(String name);

  /** @com.method(vtoffset=12, dispid=1610743816, type=METHOD, name="getNumInterfaceElements")
      @com.parameters([type=I4] return) */
  public int getNumInterfaceElements();

  /** @com.method(vtoffset=13, dispid=1610743817, type=METHOD, name="getInterfaceElement")
      @com.parameters([in,type=I4] index, [out,size=1,elementType=I4,type=ARRAY] elementType, [out,size=1,elementType=I4,type=ARRAY] fieldType, [out,size=1,elementType=STRING,type=ARRAY] name) */
  public void getInterfaceElement(int index, int[] elementType, int[] fieldType, String[] name);

  /** @com.method(vtoffset=14, dispid=1610743818, type=METHOD, name="getValueNode")
      @com.parameters([iid=BAC50393-75D9-11D0-9315-0020AFE05CC8,thread=AUTO,type=DISPATCH] return) */
  public blaxxunvrml.Node getValueNode();

  /** @com.method(vtoffset=15, dispid=1610743819, type=METHOD, name="getNative")
      @com.parameters([type=I4] return) */
  public int getNative();

  /** @com.method(vtoffset=16, dispid=1610743820, type=METHOD, name="getRefCnt")
      @com.parameters([type=I4] return) */
  public int getRefCnt();


  public static final com.ms.com._Guid iid = new com.ms.com._Guid((int)0x20400, (short)0x0, (short)0x0, (byte)0xc0, (byte)0x0, (byte)0x0, (byte)0x0, (byte)0x0, (byte)0x0, (byte)0x0, (byte)0x46);
}
