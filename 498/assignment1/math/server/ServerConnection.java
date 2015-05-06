// author: Mark Angrish (mangr)
// file: ServerConnection.java
// description: thread that the server executes to handle a client request.

import java.net.*;
import java.io.*;

public class ServerConnection extends Thread {

	Socket client;
	
	// Constructor: public ServerSocket
	// description: Takes in a client socket and reduces the client
	//              threads priority so the server socket listener
	//              can keep polling.
	public ServerConnection(Socket client) throws SocketException {
		this.client = client;
		setPriority( NORM_PRIORITY - 1);
	}

	// method:      public void run();
	// description: this is what the threads start method lets loose...
	//              services the client while they are still alive then
	//              dies upon explicit kill or a termination (ctrl C)
	public void run() {
	  try {

		// get the client sockets' input and output streams...
		InputStream sin    = client.getInputStream();
		OutputStream sout  = client.getOutputStream();

		// ... and then use a buffered reader and printwriter on top
		// of the streams to allow char to byte and byte to char for
		// network transmission.. in network byte order naturally ;)
		BufferedReader in  = new BufferedReader( new InputStreamReader(sin) );
		PrintWriter out    = new PrintWriter( new OutputStreamWriter(sout) );

		// .. a boolean to set up service lifecycle for this thread... we will
		// set it to true when a kill is requested from the client...
		boolean done = false;

		// ... while the client is still alive and wanting more jobs...
		while (!done) {

			// The protocol we are defining for this program will be that the
			// server is expecting to read two lines; a math function name and
			// a number string delimited by newlines as the methods suggest...
			String functionName  = in.readLine();
			String number        = in.readLine();

			// ... and we will echo it to the server terminal to allow the
			// logging of all server interaction, just coz we want to know 
			// whats happening ...
			System.out.println("Request: " +  functionName + " " + number);

			// close the service to the client if he requests a kill...
			if (functionName.trim().equals("kill") || 
				number.trim().equals("kill") ) {
			  done = true;
			  break;
			}

			// ok.. do the dirty work here... i think adam sandler is a pretty 
			// good actor..
			String answer = processRequest( functionName, number );

			// .. finally our protocol will define that the client program
			out.print( answer + "\n" );
			out.flush();
		}
		// .. and then close the output stream to complete the thread.
		out.close();

      } 
	  // catch and EOF exceptions, the gracefully close the port to the
	  // client..
	  catch (EOFException e) {
      	try{
			client.close();
		} catch (IOException e2) { }
	  } catch (IOException e3) {
	  	System.out.println("I/O error: " + e3);
	  }
	}



	// method:      private String processRequest( String , String );
	// description: trivial method that returns the appropriate
	//              answer to a given function given a double (precision
	//              issues).
	private String processRequest( String functionName, String number ) {

		  double answer = 0;
		  double num = Double.valueOf(number).doubleValue(); // we use a double
		                                                     // as most Math 
		                                                     //functions expect it

		  if (functionName.equals("sqrt") ) {
			answer = Math.sqrt( num );
		  }
		  else if (functionName.equals("sin") ) {
			answer = Math.sin( num );
		  }
		  else if (functionName.equals("cos") ) {
			answer = Math.cos( num );
		  }		  
		  else if (functionName.equals("tan") ) {
			answer = Math.tan( num );
		  } 
		  else if (functionName.equals("log") ) {
			answer = Math.log( num );
		  }
		  else if (functionName.equals("prime") ) {
			int i;
			if (num == 1 || num == 2) {
				answer = 1;	
			}
			for (i=2 ; i<num ; i++) {
	  			if (num % i == 0) {
					answer = 0;
					break;
	  			}
				answer = 1;
			}
		  }
		  else 
			return new String("ERROR"); // return an error if one of the above 
		                                // isnt hit.

		 return String.valueOf(answer).toString();
	}

}
