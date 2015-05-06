import java.net.*;
import java.io.*;


public class ServerConnection extends Thread {

	Socket client;
	
	ServerConnection(Socket client) throws SocketException {
		this.client = client;
		setPriority( NORM_PRIORITY - 1);
	}

	public void run() {
	  try {
		ObjectInputStream in = 
		  new ObjectInputStream( client.getInputStream() );
		ObjectOutputStream out = 
		  new ObjectOutputStream( client.getOutputStream() );

		while (true) {
		  out.writeObject( processRequest( in.readObject() );
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

	private Object processRequest( Object request ) {
	//FIX HERE!!!
		if (request instanceof WorkRequest )
			return ((WorkRequest)request).excecute();
		else 
			return null;
	}

}
