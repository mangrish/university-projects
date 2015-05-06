// file:        CalcSin.java
// description: subclasses WorkRequest to create CalcSin, which has code to 
//              calculate a sine function of a given number.

package math.service.mathlibs;

import math.service.*;

public class CalcSin extends WorkRequest {

  double num;

  public CalcSin( float num ) {
	this.num = (double) num;
  }

  public Object execute() {
	return new Double( Math.sin(num) );
  }

}
