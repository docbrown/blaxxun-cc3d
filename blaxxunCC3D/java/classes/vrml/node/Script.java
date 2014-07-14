// Script.java 


// package
package vrml.node;

// imports
import vrml.*;
import vrml.field.*;



public class Script extends BaseNode implements blaxxunvrml.Script 
{ 

   //BaseNode m_bxNode is  the GLView script node hosting this script 
	
	
  // constructor
   public Script(blaxxunvrml.Node bxNode)
   {
     super(bxNode);
   }
	

   // blaxxunvrml/Script interface methods 
/*
    public abstract void setContainer(blaxxunvrml.Node);
    public abstract void loadScriptObject(java.lang.String);
    public abstract void initialize();
    public abstract void processEvent(java.lang.String, int, blaxxunvrml.EventOut, double);
    public abstract void eventsProcessed();
    public abstract void shutdown();
*/


   public void setContainer(blaxxunvrml.Node bxNode)
   {
      m_bxNode = bxNode;
   }

   public void processEvent(java.lang.String name, int id, blaxxunvrml.EventOut event, double timeStamp)
   {
	   ConstField f=null;
	   
	   System.out.println("Script.processEvent("+name +","+id+String.valueOf(timeStamp)+"...)");

	  // f = ConstField.newInstance(event);
	   Event e =  new Event(f,name,timeStamp);
	   processEvent(e);
   }

    public void loadScriptObject(java.lang.String urlString ) {
	}
 

   // overrides 

   public String getType() {
	return m_bxNode.getType(); // will be Script
   }

   // Get the Browser that this node is contained in.
   //public Browser getBrowser();  

   	
	
   // This method is called before any event is generated
   public void initialize() {
	   System.out.println("Script.initialize()");
   } 


   // Get a Field by name.
   //   Throws an InvalidFieldException if fieldName isn't a valid
   //   field name for a node of this type.
   protected  Field  getField (String name)
       throws InvalidFieldException
   {
		blaxxunvrml.Field glField;

		// get GLView field , return vrml Field 
		glField = (blaxxunvrml.Field) m_bxNode.getField(name);

		if (glField == null) throw new InvalidFieldException("No such Field "+name);
		return Field.newInstance(glField);
   }

   


   // Get an EventOut by name.
   //   Throws an InvalidEventOutException if eventOutName isn't a valid
   //   eventOut name for a node of this type.
   protected final Field getEventOut(String name)
       throws InvalidEventOutException
   {
		blaxxunvrml.Field glField;

		// get GLView field , return vrml Field 
		glField = (blaxxunvrml.Field) m_bxNode.getEventOut(name);

		if (glField == null) throw new InvalidEventOutException("No such EventOut "+name);
		return Field.newInstance(glField);
   }

   // Get an EventIn by name.
   //   Throws an InvalidEventInException if eventInName isn't a valid
   //   eventIn name for a node of this type.
   protected final Field getEventIn(String name)
       throws InvalidEventInException
   {
		blaxxunvrml.Field glField;

		// get GLView field , return vrml Field 
		glField = (blaxxunvrml.Field) m_bxNode.getEventOut(name);

		if (glField == null) throw new InvalidEventInException("No such EventIn "+name);
		return Field.newInstance(glField);
   }

   // processEvents() is called automatically when the script receives 
   //   some set of events. It shall not be called directly except by its subclass.
   //   count indicates the number of events delivered.
   public void processEvents(int count, Event events[]) {
	   for (int i=0; i<events.length; i++)
		   processEvent(events[i]);
	   eventsProcessed();
   }

   // processEvent() is called automatically when the script receives 
   // an event. 
   public void processEvent(Event event) 
   {
		System.out.println("Script.processEvent " + event.toString());
   }

   // eventsProcessed() is called after every invocation of processEvents().
   public void eventsProcessed() 
   {
	   System.out.println("Script.eventsProcessed()");

   }

   // shutdown() is called when this Script node is deleted.
   public void shutdown()
   {	   
	   System.out.println("Script.shutdown()");
   }

   //public String toString() {return null;}   // This overrides a method in Object

}
