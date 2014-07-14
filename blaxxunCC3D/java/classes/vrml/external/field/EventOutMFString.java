// EventOutMFString.java

// package
package vrml.external.field;

public class EventOutMFString extends EventOutMField
{
  // constructor
  public EventOutMFString (blaxxunvrml.EventOut glEventOutMFString)
  {
    // save gl event out
    super (glEventOutMFString);
  }

  // get value
  public String [] getValue ()
  {
    String [] strings;
    int size;
    int i;

	blaxxunvrml.EventOutMFString me;
	me = (blaxxunvrml.EventOutMFString) m_bxEventOut;


    // allocate array
    size = me.getSize ();
    strings = new String [size];

    // get gl strings
    for (i = 0; i < size; i ++)
    {
      strings [i] = me.get1Value (i);
    }

    return strings;
  }

  public String  get1Value(int index)
  {
    return ((blaxxunvrml.EventOutMFString) m_bxEventOut).get1Value(index);
  }


}
