/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.2 $
   Date:        $Date: 1997/12/10 10:35:09 $
   Modification History:
   $Log: EventOutObserver.java,v $
   Revision 1.2  1997/12/10 10:35:09  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:39  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

// package
package vrml.external.field;

// Interface which all observer classes must implement.

public interface EventOutObserver {
  void callback(EventOut value, double timeStamp, Object userData);
}
