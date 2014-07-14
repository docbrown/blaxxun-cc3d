// EventOutMFVec3f.java

// package
package vrml.external.field;

// imports

public class EventOutMFVec3f extends EventOutMField
{

	// constructor
  public EventOutMFVec3f(blaxxunvrml.EventOut glEventOutMFVec3f)
  {
    // save gl event out
    super (glEventOutMFVec3f);
  }

  // get value
  public float[][] getValue ()
  {
    float [][] v;
    int size;
    int i;

	blaxxunvrml.EventOutMFVec3f me;
	me = (blaxxunvrml.EventOutMFVec3f) m_bxEventOut;


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
    ((blaxxunvrml.EventOutMFVec3f) m_bxEventOut).get1Value(index,v);
	return v;
  }
}
