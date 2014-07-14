/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.3 $
   Date:        $Date: 1998/03/17 14:05:23 $
   Modification History:
   $Log: EventInSFTime.java,v $
   Revision 1.3  1998/03/17 14:05:23  holger
   empty message

   Revision 1.2  1997/12/10 10:34:59  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:31  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

// package
package vrml.external.field;

// imports
import vrml.external.Node;

public class EventInSFTime extends EventIn
{
    // constructor
    protected EventInSFTime (int type, int cb, int ce)
    {
        super (type, cb, ce);
    }

    // set value
    public void setValue (double value)
    {
        setTime (value);
    }
}
