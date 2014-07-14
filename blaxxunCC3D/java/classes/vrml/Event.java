

package vrml;

import vrml.field.ConstField;


public class Event implements Cloneable 
{

   ConstField value;
   String name;
   double timeStamp;

   public Event(ConstField value, String name,double timeStamp)
   { this.value = value; this.name = name; this.timeStamp = timeStamp;}

   public String getName() 	{ return name; }
   
   public double getTimeStamp() { return timeStamp; }
   
   public ConstField getValue() { return (value); }

   public Object clone() { return new Event(value,name,timeStamp); }
   // do we need to clone value ???

   
   // This overrides a method in Object
   public String toString() { return "Event( ,)" + name+ " "+String.valueOf(timeStamp); }; 
}
