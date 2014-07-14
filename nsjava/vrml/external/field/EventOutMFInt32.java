/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.4 $
   Date:        $Date: 1999/03/18 10:28:57 $
   Modification History:
   $Log: EventOutMFInt32.java,v $
   Revision 1.4  1999/03/18 10:28:57  kristof
   *** empty log message ***

   Revision 1.3  1998/03/17 14:05:24  holger
   empty message

   Revision 1.2  1997/12/10 10:35:05  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:35  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

// package
package vrml.external.field;

// imports

public class EventOutMFInt32 extends EventOutMField
{
    // constructor
    protected EventOutMFInt32 (int type, int cb, int ce)
    {
        super (type, cb, ce);
    }

    // get value
    public int[] getValue ()
    {
        int [] v;
        int size;

        // allocate array
        size = getSize();
        v = new int[size];
        getInt(v);

        return v;
    }

    public int get1Value(int index)
    {
        return get1Int(index);
    }
}
