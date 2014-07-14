/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.4 $
   Date:        $Date: 1999/03/18 10:29:08 $
   Modification History:
   $Log: EventOutSFColor.java,v $
   Revision 1.4  1999/03/18 10:29:08  kristof
   *** empty log message ***

   Revision 1.3  1998/03/17 14:05:26  holger
   empty message

   Revision 1.2  1997/12/10 10:35:11  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:41  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */


// package
package vrml.external.field;

public class EventOutSFColor extends EventOutMField
{
    // constructor
    protected EventOutSFColor (int type, int cb, int ce)
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
