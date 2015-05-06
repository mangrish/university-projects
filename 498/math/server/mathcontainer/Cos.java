// file:        Cos.java
package math.server.mathcontainer;

public class Cos {

  public String execute( float num) {
	Double answer = Math.cos( num ) ;
	return new String( "cos(" + num + ") = " + answer);
  }

}
