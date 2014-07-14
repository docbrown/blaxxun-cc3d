/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.2 $
   Date:        $Date: 1997/12/10 10:35:19 $
   Modification History:
   $Log: FieldTypes.java,v $
   Revision 1.2  1997/12/10 10:35:19  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:50  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

// package
package vrml.external.field;



// Wrapper class specifying the types of all VRML eventIns/eventOuts.

public final class FieldTypes {
  public final static int UnknownType  = 0;
  public final static int SFBOOL       = 1;
  public final static int SFIMAGE      = 2;
  public final static int SFTIME       = 3;
  public final static int SFCOLOR      = 4;
  public final static int MFCOLOR      = 5;
  public final static int SFFLOAT      = 6;
  public final static int MFFLOAT      = 7;
  public final static int SFINT32      = 8;
  public final static int MFINT32      = 9;
  public final static int SFNODE       = 10;
  public final static int MFNODE       = 11;
  public final static int SFROTATION   = 12;
  public final static int MFROTATION   = 13;
  public final static int SFSTRING     = 14;
  public final static int MFSTRING     = 15;
  public final static int SFVEC2F      = 16;
  public final static int MFVEC2F      = 17;
  public final static int SFVEC3F      = 18;
  public final static int MFVEC3F      = 19;

  // This class should never need to be instantiated
  private FieldTypes() {}
}
