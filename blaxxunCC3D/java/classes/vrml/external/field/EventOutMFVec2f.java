// EventOutMFVec2f.java

// package
package vrml.external.field;

// imports

public class EventOutMFVec2f extends EventOutMField
{

	// constructor
  public EventOutMFVec2f(blaxxunvrml.EventOut glEventOutMFVec2f)
  {
    // save gl event out
    super (glEventOutMFVec2f);
  }

  // get value
  public float[][] getValue ()
  {
    float [][] v;
    int size;
    int i;

	blaxxunvrml.EventOutMFVec2f me;
	me = (blaxxunvrml.EventOutMFVec2f) m_bxEventOut;


    // allocate array
    size = me.getSize();
    v = new float[size][2];

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
	v = new float[2];
    ((blaxxunvrml.EventOutMFVec2f) m_bxEventOut).get1Value(index,v);
	return v;
  }
}
