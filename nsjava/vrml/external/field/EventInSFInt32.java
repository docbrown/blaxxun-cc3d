/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.3 $
   Date:        $Date: 1998/03/17 14:05:21 $
   Modification History:
   $Log: EventInSFInt32.java,v $
   Revision 1.3  1998/03/17 14:05:21  holger
   empty message

   Revision 1.2  1997/12/10 10:34:56  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:29  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

// package
package vrml.external.field;

public class EventInSFInt32 extends EventIn
{
    // constructor
    protected EventInSFInt32 (int type, int cb, int ce)
    {
        super (type, cb, ce);
    }

    // set value
    public void setValue (int value)
    {
        set1Int (0, value);
    }
}
