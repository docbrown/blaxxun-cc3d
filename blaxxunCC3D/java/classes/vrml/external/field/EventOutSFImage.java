// EventOutSFImage.java

// package
package vrml.external.field;

public class EventOutSFImage extends EventOut
{
  // constructor
  public EventOutSFImage (blaxxunvrml.EventOut glEventOutSFImage)
  {
    // save gl event out
    super (glEventOutSFImage);
  }

  public int           getWidth()
  {
    return ((blaxxunvrml.EventOutSFImage) m_bxEventOut).getWidth();
  }
  
  public int           getHeight()
  {
    return ((blaxxunvrml.EventOutSFImage) m_bxEventOut).getHeight();
  }
  
  public int           getNumComponents()
  {
    return ((blaxxunvrml.EventOutSFImage) m_bxEventOut).getNumComponents();
  }

  public byte[]         getPixels()
  {
	blaxxunvrml.EventOutSFImage e = (blaxxunvrml.EventOutSFImage) m_bxEventOut;	
	int width = e.getWidth();
	int height = e.getHeight();
	int numComponents = e.getNumComponents();
	// allocate return array 
	int cnt = width * height * numComponents;
	byte value[] = new byte[cnt];
    e.getPixels(cnt,value);
	return(value);
  }

}
