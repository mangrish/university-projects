// author: Mark Angrish (mangr)
// file: MathClient.java
// description: client sends (multiple) request/s to the server given various
//              input functions.

import java.net.*;
import java.io.*;

public class MathClient {

  // method: static main();
  public static void main( String argv[] ) throws IOException {

	// To shorten command line entry of server names, the
	// local domain suffix gets added inside the client :)
	String serverNameSuffix = ".cs.mu.oz.au";

	// Make sure the correct command line arguments are
	// given.
	if (argv.length != 2) {
		System.out.println("Usage: java MathClient <server-name (no suffix)>" +
			"<Port>");	
		System.exit(1);
	}
	  
	try {

	  // Assign the port ID to the command line number given...
      int portID = Integer.valueOf(argv[1]).intValue(); 

	  // ...then create a new socket to connect to the server with...
	  String serverName = new String(argv[0] + serverNameSuffix);
	  Socket socket = new Socket( serverName, portID );

	  // Create a new input and output stream...
	  InputStream sin = socket.getInputStream();
	  OutputStream sout = socket.getOutputStream();

	  // ... and create a new buffered reader and printwriter to support
	  // network transmission of chars to bytes(out) and bytes to chars(int).
	  // It is assumed that Network Byte Order network support is running...
	  BufferedReader in = new BufferedReader( new InputStreamReader(sin)); 
	  PrintWriter out = new PrintWriter(new OutputStreamWriter(sout));
     
	  //  Now we will set up a client interface for multiple service requests
	  //  by creating a stream reader from std input (console)...
	  BufferedReader stdIn = 
		new BufferedReader( new InputStreamReader(System.in));

	  // .. and now we will read in the input from the user for a function 
	  // name and a number
	  String userInput;

	  System.out.println("Welcome to a crappy client server implementation!\n");
	  System.out.println("Services available are:\n" + " -> sin(x)\n" + 
		  " -> cos(x)\n" + " -> tan(x)\n" + " -> sqrt(x)\n" + " -> log(x)\n" +
		  " -> prime(x)\n");
	  System.out.println("You may quit at anytime by typing \"kill\" (not in " +
		  "quotation marks) at the prompt for " + "function name!");
	  System.out.println("Example Usage- \n" + "funtion name:  sin\n" + 
		  "number: 9\n\n");
	  System.out.print("function name: ");

	  while ((userInput = stdIn.readLine()) != null ) { // keep going until 
		                                                // its not null man! 
	  	String functionName = userInput; 
	  	System.out.print("number: "); 
	  	userInput = stdIn.readLine();
	  	String numString  = userInput;
      
		// sends the output to the server.. remember to flush the stream!
	  	out.print( functionName + "\n");
	  	out.flush();
	  	out.print( numString + "\n");
	  	out.flush();

		// get the input.. if its null, then a kill was requested or the users 
		// some pathetic c!@t who deliberately
		// wants to break some pretty shitty code as it is anyway!
	  	String answer = in.readLine(); 
		if (answer == null) {
		  break;
		}
		if (functionName.equals("prime")) {
		  if ( answer.equals("1.0" ))
		  	System.out.println( numString + " is a prime\n");
		  else
		  	System.out.println( numString + " is not a prime\n");
		}
		else 
	  		System.out.println(functionName + "(" + numString + ") = " + 
				answer + "\n");
		System.out.print("function name: ");
	  } 

	  // .. and close all our streams, and then finally our socket.
	  out.close();
	  in.close();
	  stdIn.close();
	  socket.close();
	} 
	catch (IOException e) {
	  System.out.println( "I/O error " + e );
	} 

  }

}
