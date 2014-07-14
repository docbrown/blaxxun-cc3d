
// package
package vrml.external.field;

// Interface which all observer classes must implement.

public interface EventOutObserver {
  void callback(EventOut value, double timeStamp, Object userData);
}
