import java.io.*;
import java.net.*;

public class EchoClient {
    public static void main(String[] args) throws IOException {

        Socket echoSocket = null;
        DataOutputStream out = null;
        DataInputStream in = null;

        try {
            echoSocket = new Socket("mungee.cs.mu.oz.au", 8189);
            out = new DataOutputStream(echoSocket.getOutputStream());
            in = new DataInputStream(echoSocket.getInputStream());
        } catch (UnknownHostException e) {
            System.err.println("Don't know about host: disguise.dis.unimelb.edu.au");
            System.exit(1);
        } catch (IOException e) {
            System.err.println("Couldn't get I/O for the connection to: disguise.dis.unimelb.edu.au");
            System.exit(1);
        }
	DataInputStream stdIn = new DataInputStream(System.in);
	String userInput;

	while ((userInput = stdIn.readLine()) != null) {
	    out.writeBytes(userInput);
	    out.writeByte('\n');
	    System.out.println("echo: " + in.readLine());
	}
	out.close();
	in.close();
	stdIn.close();
	echoSocket.close();
    }
}
