// file:        CalcLog.java
// description: subclasses WorkRequest to create CalcLog, which has code to 
//              calculate the natural logarithm function of a given number.

package math.service.mathlibs;

import math.service.*;

public class CalcLog extends WorkRequest {

  double num;

  public CalcLog( float num ) {
	this.num = (double) num;
  }

  public Object execute() {
	return new Double( Math.log(num) );
  }

}
