/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.4 $
   Date:        $Date: 1999/03/18 10:28:58 $
   Modification History:
   $Log: EventOutMFNode.java,v $
   Revision 1.4  1999/03/18 10:28:58  kristof
   *** empty log message ***

   Revision 1.3  1998/03/17 14:05:24  holger
   empty message

   Revision 1.2  1997/12/10 10:35:05  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:36  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

// package
package vrml.external.field;

// imports
import vrml.external.Node;


public class EventOutMFNode extends EventOutMField
{
    // constructor
    protected EventOutMFNode (int type, int cb, int ce)
    {
        super (type, cb, ce);
    }

    // get value
    public Node[] getValue ()
    {
        Node [] v;
        int size;

        // allocate array
        size = getSize();
        v = new Node[size];
        getNode(v);

        return v;
    }

    public Node get1Value(int index)
    {
        return get1Node(index);
    }
}
