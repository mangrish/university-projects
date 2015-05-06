// file:        CalcPrime.java
// description: subclasses WorkRequest to create CalcPrime, which has code to 
//              calculate if a given number is a prime number.

package math.service.mathlibs;

import math.service.*;

public class CalcPrime extends WorkRequest {

  float num;

  public CalcPrime( float num ) {
	this.num = num;
  }

  public Object execute() {

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
