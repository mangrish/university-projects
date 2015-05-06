/**
 * A simple example test driver.
 *
 * Compile it with : 'javac SplitStringDriver.java'
 * Run it with     : 'java SplitStringDriver <String>'.
 */

import java.io.*;
import java.util.*;

public class SplitStringDriver
{
    /**
     * Test the splitString() method in the CalcMarks class.
     */
    public static void main(String argv[])
    {
        //////////////////////////////////////////////////////////////////
        // Read the String from the command line arguments.
        //////////////////////////////////////////////////////////////////
        String str = "";
        if (argv.length < 1)
        {
            System.err.println("Please enter a string.");
            System.exit(0);
        }
        else
        {
            try
            {
                str = argv[0];
            }
            catch (Exception e)
            {
                System.err.println("Exception - " + e);
                System.exit(0);
            }
        }

        //////////////////////////////////////////////////////////////////
        // Test using the value.
        //////////////////////////////////////////////////////////////////

        // Create the MarkReporter object.
        MarkReporter mark = new MarkReporter();
        Vector vect = new Vector();

        // Test splitString().
        try
        {
            // This method may throw an exception, so we need to wrap it
            // in a try-catch block.
            mark.splitString(str,vect);

            // Print out the result to standard out.
            for(int i = 0; i < vect.size(); i ++)
            {
                System.out.println("Result: " + vect.elementAt(i));
            }
        }
        catch (Exception e)
        {
            System.out.println("splitString() threw an exception");
            System.out.println(e);
        }
    }
}
