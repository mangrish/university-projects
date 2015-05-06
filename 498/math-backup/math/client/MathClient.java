import java.net.*;
import java.io.*;
import math.protocol.*;

public class MathClient {

  public static void main( String argv[] ) throws IOException {

	String serverName = "mungee.cs.mu.oz.au";

	if (argv.length != 1) {
		System.out.println("Usage: java MathClient <Port>");	
		System.exit(1);
	}
	  
	try {
      int portID = Integer.valueOf(argv[0]).intValue(); 
	  Socket socket = 
           new Socket( serverName, portID );
	  ObjectOutputStream out = 
           new ObjectOutputStream( socket.getOutputStream() );
	  ObjectInputStream in = 
           new ObjectInputStream( socket.getInputStream() );
     
	  //DUMMY DATA- USE OPTION DRIVEN APPROACH
	  out.writeObject( new WorkRequest("sqrt", 9) );
	  out.flush();
	  WorkResponse response = (WorkResponse) in.readObject();
	  System.out.println( response.getAnswer() );

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


