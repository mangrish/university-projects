/**
 * A simple example test driver.
 *
 * Compile it with : 'javac generateReportDriver.java'
 * Run it with     : 'java generateReportDriver <infile> <outfile>'.
 */

import java.io.*;
import java.util.*;

public class generateReportDriver 
{
    /**
     * Test the generateReport() method in the CalcMarks class.
     */
    public static void main(String argv[])
    {
        ///////////////////////////////////////////////////////////////////////
        // Read the input and output filenames from the command line arguments.
        ///////////////////////////////////////////////////////////////////////
        String inF = "";
        String outF = ""; 
        if (argv.length < 2)
        {
            System.err.println("Please enter valid filenames.");
            System.exit(0);
        }
        else
        {
            try
            {
                inF = argv[0];
                outF = argv[1];
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

        // Create the CalcMarks object.
        MarkReporter mark = new MarkReporter();

        // Test generateReport().
        try
        {
            // This method may throw an exception, so we need to wrap it
            // in a try-catch block.
            mark.generateReport(inF,outF);

            // Print out the result to standard out.
            System.out.println("Result: Success");
        }
        catch (Exception e)
        {
            System.out.println("generateReport() threw an exception");
            System.out.println(e);
        }
    }
}
