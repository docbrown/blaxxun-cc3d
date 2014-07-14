// EventOutMField.java

// package
package vrml.external.field;

public class EventOutMField extends EventOut
{
	// constructor
	public EventOutMField(blaxxunvrml.EventOut glEventOut)
	{
		// save gl event out
		super (glEventOut);
	}

	// get the size / length of the MF Array
	public int getSize() 
	{
		return ((blaxxunvrml.EventOutMField) m_bxEventOut).getSize();

	}

}
