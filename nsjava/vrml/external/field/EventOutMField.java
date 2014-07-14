/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.1 $
   Date:        $Date: 1999/03/18 10:29:07 $
   Modification History:
   $Log: EventOutMField.java,v $
   Revision 1.1  1999/03/18 10:29:07  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */


// package
package vrml.external.field;

public class EventOutMField extends EventOut
{
    // constructor
    protected EventOutMField (int type, int cb, int ce)
    {
        super (type, cb, ce);
    }

    // get size
    public int getSize ()
    {
        return super.getSize ();
    }
}
