/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.3 $
   Date:        $Date: 1998/03/17 14:05:19 $
   Modification History:
   $Log: EventInMFRotation.java,v $
   Revision 1.3  1998/03/17 14:05:19  holger
   empty message

   Revision 1.2  1997/12/10 10:34:49  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:24  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

// package
package vrml.external.field;

public class EventInMFRotation extends EventIn
{
    // constructor
    protected EventInMFRotation (int type, int cb, int ce)
    {
        super (type, cb, ce);
    }

    // set value
    public void setValue (float[][] value)
    {
        int l = value.length;
        // make flat array 
        float v[] = new float[l*4];
        int i,j;
        j=0;
        for (i=0; i<l; i++) {
	        v[j++]=value[i][0];
	        v[j++]=value[i][1];
	        v[j++]=value[i][2];
	        v[j++]=value[i][3];
        }
        // and set value
        setFloatArray (l*4,v);
    }

    // set 1 value
    public void set1Value (int i, float value[])
    {
        set1Float (i, value);
    }
}
