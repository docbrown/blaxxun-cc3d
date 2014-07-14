// EventInMFColor.java

// package
package vrml.external.field;

// imports
import vrml.external.Node;

public class EventInMFColor extends EventIn
{
  // constructor
  public EventInMFColor (blaxxunvrml.EventIn glEventInMFColor)
  {
    // set m_bxEventIn
    super (glEventInMFColor);
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
    ((blaxxunvrml.EventInMFColor) m_bxEventIn).setValue (l*3,v);
  }

  // set value
  public void set1Value (int i, float value[])
  {
    // pass on to gl setValue
    ((blaxxunvrml.EventInMFColor) m_bxEventIn).set1Value (i, value);
  }

}
