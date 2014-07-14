/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.3 $
   Date:        $Date: 1998/03/17 14:05:18 $
   Modification History:
   $Log: EventInMFInt32.java,v $
   Revision 1.3  1998/03/17 14:05:18  holger
   empty message

   Revision 1.2  1997/12/10 10:34:48  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:22  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

// package
package vrml.external.field;

// imports
import vrml.external.Node;

public class EventInMFInt32 extends EventIn
{
    // constructor
    protected EventInMFInt32 (int type, int cb, int ce)
    {
        super (type, cb, ce);
    }
    // set value
    public void setValue (int [] value)
    {
        setInt (value.length,value);
    }

    // set 1 value
    public void set1Value (int i, int value)
    {
        set1Int (i, value);
    }

}
