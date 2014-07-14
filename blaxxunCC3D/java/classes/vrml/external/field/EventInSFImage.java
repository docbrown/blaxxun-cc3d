// EventInSFImage.java

// package
package vrml.external.field;

import vrml.external.exception.*;


public class EventInSFImage extends EventIn {

  // constructor
  public EventInSFImage (blaxxunvrml.EventIn glEventInSFImage)
  {
    // set m_bxEventIn
    super (glEventInSFImage);
  }

  // set value
  public void  setValue(int width, int height, int numComponents,
                                byte[] pixels)
       throws IllegalArgumentException
  {
		if (width <0 || height <0 || numComponents<1  || numComponents>4) 
		  throw new IllegalArgumentException();

		if ((width*height*numComponents) < pixels.length)
			throw new IllegalArgumentException("EventInSFImage : array to small");

	   
	    // pass on to glEventIn

		((blaxxunvrml.EventInSFImage) m_bxEventIn).setValue (width,height,numComponents,pixels.length,pixels);

  }
}
