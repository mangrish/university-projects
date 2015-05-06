package proj.server;

import java.net.*;
import java.io.*;


public class MathServer {

  public static void main( String argv[] ) throws IOException {

	if (argv.length != 1) {
		System.out.println("Usage: java MathServer <Port>");	
		System.exit(1);
	}
	  
    int portID = Integer.valueOf(argv[0]).intValue();
	ServerSocket serverSocket = new ServerSocket( portID );

	while (true) {
		Socket client = serversocket.accept();
		new ServerConnection( client.start() );
	}
}
