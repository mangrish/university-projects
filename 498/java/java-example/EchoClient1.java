import java.io.*;
import java.net.*;

public class EchoClient1 {
    public static void main(String[] args) throws IOException {

        Socket echoSocket = null;
        DataOutputStream out = null;
        DataInputStream in = null;

        if (args.length != 1) {
        	System.out.println("usage: EchoClient1 <host>");
        	System.exit(1);
		}

        try {
            echoSocket = new Socket(args[0], 8189);
            out = new DataOutputStream(echoSocket.getOutputStream());
            in = new DataInputStream(echoSocket.getInputStream());
        } catch (UnknownHostException e) {
            System.err.println("Don't know about host: " + args[0]);
            System.exit(1);
        } catch (IOException e) {
            System.err.println("Couldn't get I/O for the connection to: " + args[0]);
            System.exit(1);
        }
	DataInputStream stdIn = new DataInputStream(System.in);
	String userInput;
	System.out.println("" + in.readLine());

	while ((userInput = stdIn.readLine()) != null) {
	    out.writeBytes(userInput);
	    out.writeByte('\n');
	    if (userInput.equals("BYE"))
	    	System.exit(0);
	    System.out.println("echo: " + in.readLine());
	}
	out.close();
	in.close();
	stdIn.close();
	echoSocket.close();
    }
}
