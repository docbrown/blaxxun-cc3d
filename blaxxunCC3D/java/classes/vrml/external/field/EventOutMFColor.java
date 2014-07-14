// EventOutMFColor.java

// package
package vrml.external.field;

// imports

public class EventOutMFColor extends EventOutMField
{

	// constructor
  public EventOutMFColor(blaxxunvrml.EventOut glEventOutMFColor)
  {
    // save gl event out
    super (glEventOutMFColor);
  }

  // get value
  public float[][] getValue ()
  {
    float [][] v;
    int size;
    int i;

	blaxxunvrml.EventOutMFColor me;
	me = (blaxxunvrml.EventOutMFColor) m_bxEventOut;


    // allocate array
    size = me.getSize();
    v = new float[size][3];

    // get gl values
    for (i = 0; i < size; i ++)
    {
       me.get1Value(i,v[i]);
    }

    return v;
  }

  public float[]  get1Value(int index)
  {
	float [] v;
	v = new float[3];
    ((blaxxunvrml.EventOutMFColor) m_bxEventOut).get1Value(index,v);
	return v;
  }
}
