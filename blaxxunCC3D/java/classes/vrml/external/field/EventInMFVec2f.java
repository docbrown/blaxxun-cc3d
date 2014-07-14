// EventInMFVec2f.java

// package
package vrml.external.field;

// imports
import vrml.external.Node;

public class EventInMFVec2f extends EventIn
{
  // constructor
  public EventInMFVec2f (blaxxunvrml.EventIn glEventInMFVec2f)
  {
    // set m_bxEventIn
    super (glEventInMFVec2f);
  }


  // set value
  public void setValue (float[][] value)
  {
	int l = value.length;
	// make flat array 
	float v[] = new float[l*2];
	int i,j;
	j=0;
	for (i=0; i<l; i++) {
		v[j++]=value[i][0];
		v[j++]=value[i][1];
	}
	// and set value
    ((blaxxunvrml.EventInMFVec2f) m_bxEventIn).setValue (l*2,v);
  }

  // set value
  public void set1Value (int i, float value[])
  {
    // pass on to gl setValue
    ((blaxxunvrml.EventInMFVec2f) m_bxEventIn).set1Value (i, value);
  }

}
