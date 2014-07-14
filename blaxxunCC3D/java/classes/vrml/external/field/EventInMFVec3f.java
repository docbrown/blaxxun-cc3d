// EventInMFVec3f.java

// package
package vrml.external.field;

// imports
import vrml.external.Node;

public class EventInMFVec3f extends EventIn
{
  // constructor
  public EventInMFVec3f (blaxxunvrml.EventIn glEventInMFVec3f)
  {
    // set m_bxEventIn
    super (glEventInMFVec3f);
  }


  // set value
  public void setValue (float[][] value)
  {
	int l = value.length;
	// make flat array 
	float v[] = new float[l*3];
	int i,j;
	j=0;
	for (i=0; i<l; i++) {
		v[j++]=value[i][0];
		v[j++]=value[i][1];
		v[j++]=value[i][2];
	}
	// and set value
    ((blaxxunvrml.EventInMFVec3f) m_bxEventIn).setValue (l*3,v);
  }

  // set value
  public void set1Value (int i, float value[])
  {
    // pass on to gl setValue
    ((blaxxunvrml.EventInMFVec3f) m_bxEventIn).set1Value (i, value);
  }

}
