/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.4 $
   Date:        $Date: 1998/03/17 14:05:24 $
   Modification History:
   $Log: EventOut.java,v $
   Revision 1.4  1998/03/17 14:05:24  holger
   empty message

   Revision 1.3  1997/12/15 09:44:26  kristof
   finalize added

   Revision 1.1  1997/12/02 12:10:33  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

// package
package vrml.external.field;

// imports
import vrml.external.Node;

public class EventOut
{
    // the native objects
    private int cBrowser;
    private int cEvent;
    // type
    private int type;
    // user data
    private Object userData = null;
    // saved observer
    private EventOutObserver observer = null;

    // constructor called only from C
    protected EventOut (int t, int cb, int ce)
    {
        type = t;
        cBrowser = cb;
        cEvent   = ce;
    }

    // return type of event out
    public int getType ()
    {
        return type;
    }

    // set observer for this event out
    public void advise(EventOutObserver obs, Object data)
    {
        // save observer and user data
        observer = obs;
        userData = data;
        advise();
    }

    private native void advise();

    // observer callback
    protected void callback (double timestamp)
    {
        // now call initial observer
        if (observer != null)
            observer.callback (this, timestamp, userData);
    }

    // call all native functions here
    protected native void getString (String[] value);
    protected native String get1String (int pos);
    protected native void getNode (Node[] value);
    protected native Node get1Node (int pos);
    protected native void getInt (int[] value);
    protected native int get1Int (int pos);
    protected native void getFloat (int pos, float[] value);
    protected native float get1Float (int pos);
    protected native int  getSize ();
    protected native double getTime ();
    protected native boolean getBoolean ();
    protected native void getByte ( byte[] pixels );

    // create a new vrml event in given a c event in
    protected static EventOut newEventOut (int type, int cb, int ce)
    {
        EventOut eventOut;

        // create vrml event based on type
        switch (type) // sort by alphabet 
        {

	    // MF Fields 
        case FieldTypes.MFCOLOR:
            eventOut = new EventOutMFColor(type, cb, ce);
            break;
        case FieldTypes.MFFLOAT:
            eventOut = new EventOutMFFloat (type, cb, ce);
            break;
        case FieldTypes.MFINT32:
            eventOut = new EventOutMFInt32 (type, cb, ce);
            break;
        case FieldTypes.MFNODE:
            eventOut = new EventOutMFNode (type, cb, ce);
            break;

        case FieldTypes.MFROTATION:
            eventOut = new EventOutMFRotation (type, cb, ce);
            break;

        case FieldTypes.MFSTRING:
            eventOut = new EventOutMFString (type, cb, ce);
            break;

        case FieldTypes.MFVEC2F:
            eventOut = new EventOutMFVec2f (type, cb, ce);
            break;

        case FieldTypes.MFVEC3F:
            eventOut = new EventOutMFVec3f (type, cb, ce);
            break;

       // SF Fields 

        case FieldTypes.SFBOOL:
            eventOut = new EventOutSFBool (type, cb, ce);
            break;

        case FieldTypes.SFCOLOR:
            eventOut = new EventOutSFColor (type, cb, ce);
            break;

        case FieldTypes.SFFLOAT:
            eventOut = new EventOutSFFloat (type, cb, ce);
            break;

        case FieldTypes.SFIMAGE:
            eventOut = new EventOutSFImage (type, cb, ce);
            break;

        case FieldTypes.SFINT32:
            eventOut = new EventOutSFInt32 (type, cb, ce);
            break;

        case FieldTypes.SFNODE:
            eventOut = new EventOutSFNode (type, cb, ce);
            break;

        case FieldTypes.SFROTATION:
            eventOut = new EventOutSFRotation (type, cb, ce);
            break;

        case FieldTypes.SFSTRING:
            eventOut = new EventOutSFString (type, cb, ce);
            break;

        case FieldTypes.SFTIME:
            eventOut = new EventOutSFTime (type, cb, ce);
            break;

        case FieldTypes.SFVEC2F:
            eventOut = new EventOutSFVec2f (type, cb, ce);
            break;

        case FieldTypes.SFVEC3F:
            eventOut = new EventOutSFVec3f (type, cb, ce);
            break;

        default:
            System.out.println ("EventOut::newEventOut found no event Out! type = " + type);
            eventOut = null;
            break;
        }

        return eventOut;
    }
    public void finalize () throws Throwable
    {
        nFinalize();
        super.finalize();
    }
    private native void nFinalize();

}
