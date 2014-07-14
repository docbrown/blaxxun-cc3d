// EventInMFRotation.java

// package
package vrml.external.field;

// imports
import vrml.external.Node;

public class EventInMFRotation extends EventIn
{
  // constructor
  public EventInMFRotation (blaxxunvrml.EventIn glEventInMFRotation)
  {
    // set m_bxEventIn
    super (glEventInMFRotation);
  }


  // set value
  public void setValue (float[][] value)
  {
	int l = value.length;
	// make flat array 
	float v[] = new float[l*4];
	int i,j;
	j=0;
	for (i=0; i<l; i++) {
		v[j++]=value[i][0];
		v[j++]=value[i][1];
		v[j++]=value[i][2];
		v[j++]=value[i][3];
	}
	// and set value
    ((blaxxunvrml.EventInMFRotation) m_bxEventIn).setValue (l*4,v);
  }

  // set value
  public void set1Value (int i, float value[])
  {
    // pass on to gl setValue
    ((blaxxunvrml.EventInMFRotation) m_bxEventIn).set1Value (i, value);
  }

}
