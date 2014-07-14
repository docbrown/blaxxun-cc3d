/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.4 $
   Date:        $Date: 1999/03/18 10:29:00 $
   Modification History:
   $Log: EventOutMFString.java,v $
   Revision 1.4  1999/03/18 10:29:00  kristof
   *** empty log message ***

   Revision 1.3  1998/03/17 14:05:25  holger
   empty message

   Revision 1.2  1997/12/10 10:35:07  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:37  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

// package
package vrml.external.field;

// imports

public class EventOutMFString extends EventOutMField
{
    // constructor
    protected EventOutMFString (int type, int cb, int ce)
    {
        super (type, cb, ce);
    }

    // get value
    public String[] getValue ()
    {
        String [] v;
        int size;

        // allocate array
        size = getSize();
        v = new String[size];
        getString(v);

        return v;
    }

    public String get1Value(int index)
    {
        return get1String(index);
    }
}
