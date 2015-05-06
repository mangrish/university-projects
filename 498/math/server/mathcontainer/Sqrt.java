// file:        CalcSqrt.java
// description: subclasses WorkRequest to create CalcSqrt, which has code to 
//              calculate the square root of a given number.

package math.service.mathlibs;

import math.service.*;

public class CalcSqrt extends WorkRequest {

  double num;

  public CalcSqrt( float num ) {
	this.num = (double) num;
  }

  public Object execute() {
	return new Double( Math.sqrt(num) );
  }

}
