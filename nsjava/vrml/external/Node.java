/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.5 $
   Date:        $Date: 1998/05/05 16:24:30 $
   Modification History:
   $Log: Node.java,v $
   Revision 1.5  1998/05/05 16:24:30  kristof
   gettype added

   Revision 1.4  1998/03/17 14:05:15  holger
   empty message

   Revision 1.3  1997/12/15 09:47:29  kristof
   finalize added

   Revision 1.1  1997/12/02 12:10:13  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

// package
package vrml.external;

// imports
import vrml.external.field.EventIn;
import vrml.external.field.EventOut;
import vrml.external.exception.InvalidEventInException;
import vrml.external.exception.InvalidEventOutException;

public class Node
{
    // the native objects
    private int cBrowser;
    private int cNode;

    /**
    * constructor protected, use Browser methids to get node
    * @param c handle of browser object
    */
    protected Node (int cb, int cn)
    {
        cBrowser = cb;
        cNode    = cn;
    }
    /**
    * get type of a node
    * @return string type of the node
    */
    public native String getType ( );
    /**
    * get handle to an EventIn of this node
    * @param string name of the eventIn
    */
    public native EventIn getEventIn (String name)
       throws InvalidEventInException;
    /**
    * get handle to an EventOutIn of this node
    * @param string name of the eventOut
    */
    public native EventOut getEventOut (String name)
       throws InvalidEventOutException;

    public void finalize () throws Throwable
    {
        nFinalize();
        super.finalize();
    }
    private native void nFinalize();
}
