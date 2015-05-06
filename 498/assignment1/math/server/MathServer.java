// author:      Mark Angrish (mangr)	
// file:        MathServer.java
// description: the Math Server listens for new incoming connections
//              and if a connection is made, creates a new thread to
//              handle the client request while the listener thread
//              listens for any other requests. 

import java.net.*;
import java.io.*;

public class MathServer {

  public static void main( String argv[] ) throws IOException {

	// Make sure the correct usage is invoked..
	if (argv.length != 1) {
		System.out.println("Usage: java MathServer <Port>");	
		System.exit(1);
	}
	
	// assign the port ID to the command line option given, then
	// create a new server socket to listen for incoming client
	// requests.
    int portID = Integer.valueOf(argv[0]).intValue();
	ServerSocket serverSocket = new ServerSocket( portID );

	// while we keep the server alive and dont press Ctrl-C,
	// if a client request occurs, accept the connection and
	// start a new ServerConnection thread.
	while (true) {
	  new ServerConnection ( serverSocket.accept() ).start();
	}
  }

}
