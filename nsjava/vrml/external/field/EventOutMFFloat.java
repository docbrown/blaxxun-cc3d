/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.4 $
   Date:        $Date: 1999/03/18 10:28:54 $
   Modification History:
   $Log: EventOutMFFloat.java,v $
   Revision 1.4  1999/03/18 10:28:54  kristof
   *** empty log message ***

   Revision 1.3  1998/03/17 14:05:24  holger
   empty message

   Revision 1.2  1997/12/10 10:35:04  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:34  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

// package
package vrml.external.field;

// imports

public class EventOutMFFloat extends EventOutMField
{
    // constructor
    protected EventOutMFFloat (int type, int cb, int ce)
    {
        super (type, cb, ce);
    }

    // get value
    public float[] getValue ()
    {
        float [] v;
        int size;
        int i;

        // allocate array
        size = getSize();
        v = new float[size];
        getFloat(0,v);

        return v;
    }

    public float get1Value(int index)
    {
        return get1Float(index);
    }
}
