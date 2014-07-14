// EventOutMFInt32.java

// package
package vrml.external.field;

// imports

public class EventOutMFInt32 extends EventOutMField
{

	// constructor
  public EventOutMFInt32(blaxxunvrml.EventOut glEventOutMFInt32)
  {
    // save gl event out
    super (glEventOutMFInt32);
  }

  // get value
  public int[] getValue ()
  {
    int [] v;
    int size;
    int i;

	blaxxunvrml.EventOutMFInt32 me;
	me = (blaxxunvrml.EventOutMFInt32) m_bxEventOut;


    // allocate array
    size = me.getSize();
    v = new int[size];

    // get gl values
    for (i = 0; i < size; i ++)
    {
      v[i] = me.get1Value(i);
    }

    return v;
  }

   public int  get1Value(int index)
   {
	   return ((blaxxunvrml.EventOutMFInt32) m_bxEventOut).get1Value(index);
   }
}
