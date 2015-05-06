// file:	WorkResponse.java

package math.protocol;

public class WorkResponse extends Response {

	private String answer;

	public WorkResponse( String answer ) {
	  this.answer = answer;
	}

	public String getAnswer() {
	  return answer;
	}

}
