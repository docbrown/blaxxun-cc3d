
// package
package  vrml;

public class InvalidVRMLSyntaxException extends Exception
{
    public InvalidVRMLSyntaxException() {
        super();
    }

    public InvalidVRMLSyntaxException(String s) {
        super(s);
    }
	
	// public String getMessage();  // This overrides a method in Exception
}
