// file:        CalcPrime.java
package math.server.mathcontainer;

public class Prime {

  public static  execute() {

	int i;

	if (num == 1 || num == 2) {
		return new String( num + " is a prime number!\n" );
	}
	for (i=2 ; i<num ; i++) {
	  if (num % i == 0) {
		return new String( num + " is NOT a prime number :(" );
	  }
	}

    return new String( num + " is a prime number!\n" );
  }

}
