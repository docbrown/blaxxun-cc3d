/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.3 $
   Date:        $Date: 1998/03/17 14:05:19 $
   Modification History:
   $Log: EventInMFNode.java,v $
   Revision 1.3  1998/03/17 14:05:19  holger
   empty message

   Revision 1.2  1997/12/10 10:34:49  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:23  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

// package
package vrml.external.field;

// imports
import vrml.external.Node;

public class EventInMFNode extends EventIn
{
    // constructor
    protected EventInMFNode (int type, int cb, int ce)
    {
        super (type, cb, ce);
    }

    // set value
    public void setValue (Node [] value)
    {
        setNode (value.length, value);
    }

    // set value
    public void set1Value (int i, Node value)
    {
        set1Node (i, value);
    }
}
