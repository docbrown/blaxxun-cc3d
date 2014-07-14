/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.4 $
   Date:        $Date: 1999/03/18 10:29:10 $
   Modification History:
   $Log: EventOutSFImage.java,v $
   Revision 1.4  1999/03/18 10:29:10  kristof
   *** empty log message ***

   Revision 1.3  1998/03/17 14:05:27  holger
   empty message

   Revision 1.2  1997/12/10 10:35:13  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:42  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */


// package
package vrml.external.field;

public class EventOutSFImage extends EventOutMField
{
    // constructor
    protected EventOutSFImage (int type, int cb, int ce)
    {
        super (type, cb, ce);
    }
    public int getWidth()
    {
        return get1Int(0);
    }
    public int getHeight()
    {
        return get1Int(1);
    }
    public int getNumComponents()
    {
        return get1Int(2);
    }
    public byte[] getPixels()
    {
        byte value[] = new byte[get1Int(3)]; // 3 is size of image
        getByte(value);
        return value;
    }

}
