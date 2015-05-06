import java.net.*;
import java.io.*;
import math.protocol.*;

public class ServerConnection extends Thread {

	Socket client;
	
	ServerConnection(Socket client) throws SocketException {
		this.client = client;
		System.out.println("Service Requested");
		setPriority( NORM_PRIORITY - 1);
	}

	public void run() {
	  try {
		ObjectInputStream in = 
		  new ObjectInputStream( client.getInputStream() );
		ObjectOutputStream out = 
		  new ObjectOutputStream( client.getOutputStream() );

		while (true) {
		  out.writeObject( processRequest( (WorkRequest) in.readObject() ) );
		  out.flush();
		}
      } catch (EOFException e) {
      	try{
			client.close();
		} catch (IOException e2) { }
	  } catch (IOException e2) {
	  	System.out.println("I/O error: " + e2);
	  } catch (ClassNotFoundException e3) {
	  	System.out.println("Unknown request object error: " + e3);
	  }
	}

	private Object processRequest( WorkRequest request ) {

		  String functionName = request.getFunctionName();
		  float num = request.getNum();

		  if (functionName.equals("sqrt") ) {
			float answer = (float) Math.sqrt(num); 
          	return new 
			  WorkResponse("The square root of " + num + " is: " + answer); 
		  }
		  else {
			return null;
		  }
	}

}
