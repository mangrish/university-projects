import java.net.*;
import java.io.*;
import math.protocol.*;
import math.server.mathcontainer.*;

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
		  out.writeObject( processRequest( (Request) in.readObject() ) );
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

	private Object processRequest( Request request ) {

		if ( request instanceof WorkRequest ) {
	  
		  String ansString;
		  WorkRequest wRequest = (WorkRequest) request;
		  
		  String functionName = wRequest.getFunctionName();
		  float num = wRequest.getNum();

		  if (functionName.equals("sqrt") ) {
			ansString = Sqrt(num);
          	return new WorkResponse( ansString ); 
		  }
		  elseif (functionName.equals("sin") ) {
			ansString = Sin(num);
			return new WorkResponse( ansString );
		  }
		  elseif (functionName.equals("cos") ) {
			ansString = Cos(num);
			return new WorkResponse( ansString );
		  }		  
		  elseif (functionName.equals("tan") ) {
			ansString = Tan(num);
			return new WorkResponse( ansString );
		  } 
		  elseif (functionName.equals("log") ) {
			ansString = Log(num);
			return new WorkResponse( ansString );
		  }
		  elseif (functionName.equals("prime") ) {
			ansString = Prime(num);
			return new WorkResponse( ansString );
		  } else {
			return null;
		  }
		}
		else if ( request instanceof ServiceRequest) {
			return new ServiceResponse("sin, cos, tan, log, sqrt, prime\n");
		}

		else {
			return null;
		}
	}

}
