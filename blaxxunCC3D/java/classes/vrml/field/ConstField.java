// ConstField.java

// package
package vrml.field;


// imports

public abstract class ConstField extends vrml.Field
{
	
	// constructor
	public ConstField (blaxxunvrml.Field glField)
	{
		super(glField);
	}

	public static ConstField newInstance(blaxxunvrml.Field glField) 
	{
	  // to do:
	  return null;
	}

	public static ConstField newInstance(blaxxunvrml.EventOut glField) 
	{
	  // EventOut's are always field and readable, so we can cast
	  return newInstance((blaxxunvrml.Field) glField);
	}



}
