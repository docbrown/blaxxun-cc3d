/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.4 $
   Date:        $Date: 1999/03/18 10:29:15 $
   Modification History:
   $Log: EventOutSFVec3f.java,v $
   Revision 1.4  1999/03/18 10:29:15  kristof
   *** empty log message ***

   Revision 1.3  1998/03/17 14:05:28  holger
   empty message

   Revision 1.2  1997/12/10 10:35:18  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:49  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */


// package
package vrml.external.field;

public class EventOutSFVec3f extends EventOutMField
{
    // constructor
    protected EventOutSFVec3f (int type, int cb, int ce)
    {
        super (type, cb, ce);
    }

    // get value
    public float [] getValue ()
    {
        float [] value = new float [3];

        getFloat (0, value);

        return value;
    }
}
