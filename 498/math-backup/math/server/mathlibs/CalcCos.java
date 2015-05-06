// file:        CalcCos.java
// description: subclasses WorkRequest to create CalcCos, which has code to 
//              calculate a cosine function of a given number.

package math.service.mathlibs;

import math.service.*;

public class CalcCos extends WorkRequest {

  double num;

  public CalcCos( float num ) {
	this.num = (double) num;
  }

  public Object execute() {
	return new Double( Math.cos(num) );
  }

}
