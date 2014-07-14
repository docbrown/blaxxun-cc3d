/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.3 $
   Date:        $Date: 1998/03/17 14:05:22 $
   Modification History:
   $Log: EventInSFNode.java,v $
   Revision 1.3  1998/03/17 14:05:22  holger
   empty message

   Revision 1.2  1997/12/10 10:34:57  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:29  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

// package
package vrml.external.field;

// imports
import vrml.external.Node;

public class EventInSFNode extends EventIn
{
    // constructor
    protected EventInSFNode (int type, int cb, int ce)
    {
        super (type, cb, ce);
    }

    // set value
    public void setValue (Node value)
    {
        set1Node (0, value);
    }
}
