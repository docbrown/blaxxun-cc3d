/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.4 $
   Date:        $Date: 1999/03/18 10:29:07 $
   Modification History:
   $Log: EventOutSFBool.java,v $
   Revision 1.4  1999/03/18 10:29:07  kristof
   *** empty log message ***

   Revision 1.3  1998/03/17 14:05:26  holger
   empty message

   Revision 1.2  1997/12/10 10:35:10  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:40  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */


// package
package vrml.external.field;

public class EventOutSFBool extends EventOutMField
{
    // constructor
    protected EventOutSFBool (int type, int cb, int ce)
    {
        super (type, cb, ce);
    }

    // get value
    public boolean getValue ()
    {
        return getBoolean ();
    }
}
