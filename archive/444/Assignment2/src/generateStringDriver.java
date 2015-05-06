/**
 * A simple example test driver.
 *
 * Compile it with : 'javac generateStringDriver.java'
 * Run it with     : 'java generateStringDriver '.
 */

import java.io.*;
import java.util.*;

public class generateStringDriver 
{
    /**
     * Test the generateString() method in the CalcMarks class.
     */
    public static void main(String argv[])
    {
        ///////////////////////////////////////////////////////////////////////
        // Read the valid from the command line arguments.
        ///////////////////////////////////////////////////////////////////////
        String output = ""; 
        
        long sNum = 0;
        String bMth = "";
        int bDate = 0;
        String fName = "";
        char sInit = ' ';
        int numMissed = 0;
        boolean slept = false;
        
        if (argv.length < 7)
        {
            System.err.println("Please enter valid arguments.");
            System.exit(0);
        }
        else
        {
            try
            {
                String sNS = (String) argv[0];
                sNum = Long.valueOf(argv[0]).longValue();
                
                bMth = (String) argv[1];

                String bDS = (String) argv[2];
                bDate = Integer.valueOf(bDS).intValue();
                
                fName = (String) argv[3];
               
                String sS = (String) argv[4];
                sInit = sS.charAt(0);
                
                String nMS = (String) argv[5];
                numMissed = Integer.valueOf(nMS).intValue();
                
                String sSt = (String) argv[6];
                slept = Boolean.valueOf(sSt).booleanValue();

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
            output = mark.generateString(sNum, bMth, bDate, fName,
                                         sInit, numMissed, slept);

            // Print out the result to standard out.
            System.out.println("Result: " + output);
        }
        catch (Exception e)
        {
            System.out.println("generateString() threw an exception");
            System.out.println(e);
        }
    }
}
