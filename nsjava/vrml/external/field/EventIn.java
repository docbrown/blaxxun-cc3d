/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.4 $
   Date:        $Date: 1998/03/17 14:05:18 $
   Modification History:
   $Log: EventIn.java,v $
   Revision 1.4  1998/03/17 14:05:18  holger
   empty message

   Revision 1.3  1997/12/15 09:47:01  kristof
   finalize added

   Revision 1.1  1997/12/02 12:10:20  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

// package
package vrml.external.field;

import vrml.external.field.FieldTypes;
import vrml.external.Node;

public class EventIn
{
    // the native objects
    private int cBrowser;
    private int cEvent;
    // type
    private int type;

    // constructor called only from C
    protected EventIn (int t, int cb, int ce)
    {
        type = t;
        cBrowser = cb;
        cEvent   = ce;
    }

    // return type of event in
    public int getType()
    {
        return type;
    }

    // call all native functions here
    protected native void setString (int len, String[] value);
    protected native void set1String (int pos, String value);
    protected native void setNode (int len, Node[] value);
    protected native void set1Node (int pos, Node value);
    protected native void setInt (int len, int[] value);
    protected native void set1Int (int pos, int value);
    protected native void setFloat (int pos, float value);
    protected native void setFloatArray (int len, float[] value);
    protected native void set1Float (int pos, float[] value);
    protected native void setTime (double value);
    protected native void setBoolean (boolean value);
    protected native void setImage (int width, int height, int numComponents,
                                    int pixelsLen, byte[] pixels);

    // create a new vrml event in given a c event in
    protected static EventIn newEventIn (int type, int cb, int ce)
    {
        EventIn eventIn;

        // create vrml event based on type
        switch (type) // sort by alphabet 
        {

	    // MF Fields 
        case FieldTypes.MFCOLOR:
            eventIn = new EventInMFColor(type, cb, ce);
            break;
        case FieldTypes.MFFLOAT:
            eventIn = new EventInMFFloat (type, cb, ce);
            break;
        case FieldTypes.MFINT32:
            eventIn = new EventInMFInt32 (type, cb, ce);
            break;
        case FieldTypes.MFNODE:
            eventIn = new EventInMFNode (type, cb, ce);
            break;

        case FieldTypes.MFROTATION:
            eventIn = new EventInMFRotation (type, cb, ce);
            break;

        case FieldTypes.MFSTRING:
            eventIn = new EventInMFString (type, cb, ce);
            break;

        case FieldTypes.MFVEC2F:
            eventIn = new EventInMFVec2f (type, cb, ce);
            break;

        case FieldTypes.MFVEC3F:
            eventIn = new EventInMFVec3f (type, cb, ce);
            break;

       // SF Fields 

        case FieldTypes.SFBOOL:
            eventIn = new EventInSFBool (type, cb, ce);
            break;

        case FieldTypes.SFCOLOR:
            eventIn = new EventInSFColor (type, cb, ce);
            break;

        case FieldTypes.SFFLOAT:
            eventIn = new EventInSFFloat (type, cb, ce);
            break;

        case FieldTypes.SFIMAGE:
            eventIn = new EventInSFImage (type, cb, ce);
            break;

        case FieldTypes.SFINT32:
            eventIn = new EventInSFInt32 (type, cb, ce);
            break;

        case FieldTypes.SFNODE:
            eventIn = new EventInSFNode (type, cb, ce);
            break;

        case FieldTypes.SFROTATION:
            eventIn = new EventInSFRotation (type, cb, ce);
            break;

        case FieldTypes.SFSTRING:
            eventIn = new EventInSFString (type, cb, ce);
            break;

        case FieldTypes.SFTIME:
            eventIn = new EventInSFTime (type, cb, ce);
            break;

        case FieldTypes.SFVEC2F:
            eventIn = new EventInSFVec2f (type, cb, ce);
            break;

        case FieldTypes.SFVEC3F:
            eventIn = new EventInSFVec3f (type, cb, ce);
            break;

        default:
            System.out.println ("EventIn::newEventIn found no event in! type = " + type );
            eventIn = null;
            break;
        }

        return eventIn;
    }

    public void finalize () throws Throwable
    {
        nFinalize();
        super.finalize();
    }
    private native void nFinalize();
}
