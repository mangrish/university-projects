// file:        WorkRequest.java

package math.protocol;

public class WorkRequest extends Request {

  private String functionName;
  private float num;

  public WorkRequest( String functionName, float num ) {
	this.functionName = functionName;
	this.num = num;
  }

  public String getFunctionName() {
	return functionName;
  }

  public float getNum() {
	return num;
  }

}
