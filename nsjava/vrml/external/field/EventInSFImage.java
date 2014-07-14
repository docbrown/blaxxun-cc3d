/* ----   file information  -------------------------------------------------
   Revison:     $Revision: 1.3 $
   Date:        $Date: 1998/03/17 14:05:21 $
   Modification History:
   $Log: EventInSFImage.java,v $
   Revision 1.3  1998/03/17 14:05:21  holger
   empty message

   Revision 1.2  1997/12/10 10:34:55  kristof
   *** empty log message ***

   Revision 1.1  1997/12/02 12:10:28  kristof
   Initial revision

   ----   file information  ---------------------------------------------- */

// package
package vrml.external.field;

public class EventInSFImage extends EventIn 
{

    // constructor
    protected EventInSFImage (int type, int cb, int ce)
    {
        super (type, cb, ce);
    }

    // set value
    public void  setValue(int width, int height, int numComponents, byte[] pixels)
        throws IllegalArgumentException
    {
		if (width <0 || height <0 || numComponents<1  || numComponents>4) 
		  throw new IllegalArgumentException();

		if ((width*height*numComponents) < pixels.length)
			throw new IllegalArgumentException("EventInSFImage : array to small");
		
        setImage (width,height,numComponents,pixels.length,pixels);
    }
}
