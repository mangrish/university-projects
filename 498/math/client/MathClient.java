import java.net.*;
import java.io.*;
import math.protocol.*;

public class MathClient {

  public static void main( String argv[] ) throws IOException {

	String serverNameSuffix = ".cs.mu.oz.au";

	if (argv.length != 2) {
		System.out.println("Usage: java MathClient <Port>");	
		System.exit(1);
	}
	  
	try {
      int portID = Integer.valueOf(argv[1]).intValue(); 
	  String serverName = new String(argv[0] + serverNameSuffix);
	  Socket socket = 
           new Socket( serverName, portID );
	  ObjectOutputStream out = 
           new ObjectOutputStream( socket.getOutputStream() );
	  ObjectInputStream in = 
           new ObjectInputStream( socket.getInputStream() );
     
	  //DUMMY DATA- USE OPTION DRIVEN APPROACH
	  out.writeObject( new ServiceRequest() );
	  out.flush();
	  ServiceResponse sResponse = (ServiceResponse) in.readObject();
	  System.out.println( sResponse.getServiceNames() + "\n");

	  out.writeObject( new WorkRequest("sqrt", 9) );
	  out.flush();
	  WorkResponse response = (WorkResponse) in.readObject();
	  System.out.println( response.getAnswer() );
// END OF DUMMY DATA
	  socket.close();

	} 
	catch (IOException e) {
	  System.out.println( "I/O error " + e );
	} 
	catch (ClassNotFoundException e2) {
	  System.out.println( e2 );
	}
  }

}


