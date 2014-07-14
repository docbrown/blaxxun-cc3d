/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.3 $
   Date:        $Date: 1998/03/17 14:05:18 $
   Modification History:
   $Log: EventInMFFloat.java,v $
   Revision 1.3  1998/03/17 14:05:18  holger
   empty message

   Revision 1.2  1997/12/10 10:34:47  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:21  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

// package
package vrml.external.field;

// imports
import vrml.external.Node;

public class EventInMFFloat extends EventIn
{
    // constructor
    protected EventInMFFloat (int type, int cb, int ce)
    {
        super (type, cb, ce);
    }

    // set value
    public void setValue (float [] value)
    {
        setFloatArray (value.length, value);
    }

    // set value
    public void set1Value (int i, float value)
    {
        setFloat (i, value);
    }
}
