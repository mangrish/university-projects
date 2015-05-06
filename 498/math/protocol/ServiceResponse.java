// file:	ServiceResponse.java

package math.protocol;

public class ServiceResponse extends Response {

	private String serviceNames;

	public ServiceResponse( String serviceNames ) {
	  this.serviceNames = serviceNames;
	}

	public String getServiceNames() {
	  return serviceNames;
	}

}
