// file:        CalcTan.java
// description: subclasses WorkRequest to create CalcTan, which has code to 
//              calculate a tangent function of a given number.

package math.service.mathlibs;

import math.service.*;

public class CalcTan extends WorkRequest {

  double num;

  public CalcTan( float num ) {
	this.num = (double) num;
  }

  public Object execute() {
	return new Double( Math.tan(num) );
  }

}
