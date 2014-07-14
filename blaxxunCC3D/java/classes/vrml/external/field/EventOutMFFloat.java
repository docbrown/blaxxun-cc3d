// EventOutMFFloat.java

// package
package vrml.external.field;

// imports

public class EventOutMFFloat extends EventOutMField
{

	// constructor
  public EventOutMFFloat (blaxxunvrml.EventOut glEventOutMFFloat)
  {
    // save gl event out
    super (glEventOutMFFloat);
  }

  // get value
  public float[] getValue ()
  {
    float [] v;
    int size;
    int i;

	blaxxunvrml.EventOutMFFloat me;
	me = (blaxxunvrml.EventOutMFFloat) m_bxEventOut;


    // allocate array
    size = me.getSize();
    v = new float[size];

    // get gl values
    for (i = 0; i < size; i ++)
    {
      v[i] = me.get1Value(i);
    }

    return v;
  }

   public float  get1Value(int index)
   {
	   return ((blaxxunvrml.EventOutMFFloat) m_bxEventOut).get1Value(index);
   }
}
