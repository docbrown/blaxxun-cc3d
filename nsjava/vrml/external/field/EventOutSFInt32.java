/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.4 $
   Date:        $Date: 1999/03/18 10:29:11 $
   Modification History:
   $Log: EventOutSFInt32.java,v $
   Revision 1.4  1999/03/18 10:29:11  kristof
   *** empty log message ***

   Revision 1.3  1998/03/17 14:05:27  holger
   empty message

   Revision 1.2  1997/12/10 10:35:14  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:43  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */


// package
package vrml.external.field;

public class EventOutSFInt32 extends EventOutMField
{
    // constructor
    protected EventOutSFInt32 (int type, int cb, int ce)
    {
        super (type, cb, ce);
    }

    // get value
    public int getValue ()
    {
        return get1Int (0);
    }
}
