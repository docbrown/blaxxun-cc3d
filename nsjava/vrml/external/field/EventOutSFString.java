/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.4 $
   Date:        $Date: 1999/03/18 10:29:13 $
   Modification History:
   $Log: EventOutSFString.java,v $
   Revision 1.4  1999/03/18 10:29:13  kristof
   *** empty log message ***

   Revision 1.3  1998/03/17 14:05:28  holger
   empty message

   Revision 1.2  1997/12/10 10:35:16  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:45  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */


// package
package vrml.external.field;

public class EventOutSFString extends EventOutMField
{
    // constructor
    protected EventOutSFString (int type, int cb, int ce)
    {
        super (type, cb, ce);
    }

    // get value
    public String getValue ()
    {
        return get1String (0);
    }
}
