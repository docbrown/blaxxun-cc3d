// EventOutMFRotation.java

// package
package vrml.external.field;

// imports

public class EventOutMFRotation extends EventOutMField
{

	// constructor
  public EventOutMFRotation(blaxxunvrml.EventOut glEventOutMFRotation)
  {
    // save gl event out
    super (glEventOutMFRotation);
  }

  // get value
  public float[][] getValue ()
  {
    float [][] v;
    int size;
    int i;

	blaxxunvrml.EventOutMFRotation me;
	me = (blaxxunvrml.EventOutMFRotation) m_bxEventOut;


    // allocate array
    size = me.getSize();
    v = new float[size][4];

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
	v = new float[4];
    ((blaxxunvrml.EventOutMFRotation) m_bxEventOut).get1Value(index,v);
	return v;
  }
}
