import java.io.*;
import java.net.*;

public class EchoServer {
    public static void main(String argv[]) {

        try {
            ServerSocket s = new ServerSocket(8189);
            Socket incoming = s.accept();
            DataInputStream in = new DataInputStream (incoming.getInputStream());
		PrintStream out = new PrintStream(incoming.getOutputStream());
		out.println("Hello. Enter BYE to exit");

		boolean done = false;
		while (!done) {
			String str = in.readLine();
			if (str == null){
				done = true;
                System.out.println("returned null");
                            }
			else {
				out.println(str);
				if(str.trim().equals("BYE")) done = true;
			}
		}
		incoming.close();

		} catch (Exception e) {
			System.out.println(e);
		}
	}
}
