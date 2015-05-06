import java.io.*;
import java.util.*;

/**
 * This class contains code to generate a report containing LWIG marks
 * from an input file containing student information.
 *
 * This class contains the main method for the LWIG Marking system.
 *
 * See the main() method description for details.
 */
public class MarkReporter
{
    /**
     * Main program. This will read an input file from the command line
     * argument, and use that to call generateReport(). A report will be
     * generated for the specified file. The report will have the same
     * name as the original file, but will have a ".output" extension
     * appended to it.
     * 
     * e.g. input file: "TestData.txt"
     *      output file: "TestData.txt.output"
     *
     * If there are any errors in the input file:
     *    - If the input file contains invalid data, the application
     *      will print out a warning, skip the faulty record, and
     *      continue.
     *
     * For any other kinds of errors (e.g. input file does not exist,
     * unable to write to output file, etc), the application will fail
     * gracefully, printing out an error message.
     */
    public static void main(String argv[])
    {
        if (argv.length > 0)
        {
            MarkReporter rep = new MarkReporter();
            try
            {
                rep.generateReport(argv[0], argv[0]+".output");
            }
            catch (Exception e)
            {
                System.err.println("Exception - " + e);
            }
        }
        else
        {
            System.err.println("Please specify an input file. "
                               + "e.g. 'java MarkReporter input.txt'");
        }
    }


    /**
     * Constructor for this class. Does not need to do anything.
     */
    public MarkReporter()
    {
        // Constructor does not need to do anything.
    }


    /**
     * Read in the specified input file "inFilePath", process the
     * items in the input file, and write out the results in the output
     * file "outFilePath".
     *
     * The input file will contain one record per line. Each record will
     * have the format:
     *  (Student number);(Birth month as 3 char string);(Birth date);
     *  (First name);(Initial of surname);(Number of missed lectures);
     *  (Whether the student slept during any lectures)
     *
     *  e.g. 199223495;Jun;21;Franklin;R;2;true
     *
     * The output file will contain one record per line. Each record
     * will have the format:
     *   (Assignment 1 mark);(Assignment 2 mark);(Assignment 3 mark);
     *   (Exam mark);(Overall grade for the subject);
     *   (Comment about the student)
     *
     * e.g. 5;25;10;60;H1;Excellent.
     *
     * If it encounters a faulty record in the input file, this method
     * will print out a warning and skip the faulty record.
     *
     * If it is unable to read/write a file, it will generate an
     * IOException.
     *
     * @param inFilePath   Full filepath name of the input file.
     * @param outFilePath  Full filepath name of the output file.
     */
    public void generateReport(String inFilePath, String outFilePath)
        throws IOException
    {
        File f = new File(inFilePath);
        if (f.exists())
        {
            // Read in the file.
            Vector inFileVector = new Vector();
            FileSupport.fileToVector(inFilePath, inFileVector);

            // Process the data from the file.
            Vector outFileVector = new Vector();
            for (int i=0; i<inFileVector.size(); i++)
            {
                String currInLine = (String) inFileVector.elementAt(i);

                // Parse the input line into a Vector.
                Vector currLineVector = new Vector();
                splitString(currInLine, currLineVector);

                // Examine the contents of the currLineVector.
                if (currLineVector.size() == 7)
                {
                  try
                  {
                    String studentNumStr 
                        = (String) currLineVector.elementAt(0);
                    long studentNum 
                        = Long.valueOf(studentNumStr).longValue();

                    String birthMonth 
                        = (String) currLineVector.elementAt(1);

                    String birthDateStr  
                        = (String) currLineVector.elementAt(2);
                    int birthDate 
                        = Integer.valueOf(birthDateStr).intValue();

                    String firstName 
                        = (String) currLineVector.elementAt(3);

                    String surnameStr 
                        = (String) currLineVector.elementAt(4);
		    //Added a guard fo the length of the suname char.
		    if (surnameStr.length() != 1) {
		      throw new Exception("Invalid surname length");
		    }
                    char surnameInitial 
                        = surnameStr.charAt(0);

                    String numMissedStr  
                        = (String) currLineVector.elementAt(5);
                    int numMissed 
                        = Integer.valueOf(numMissedStr).intValue();

                    String sleptStr 
                        = (String) currLineVector.elementAt(6);
		    //Added a guard to valid input allowed for this variable
		    System.out.println("word is=" + sleptStr );
		      
		    if ((!(sleptStr.equalsIgnoreCase("true"))) &&
			(!(sleptStr.equalsIgnoreCase("false")))) {
		      throw new Exception("Invalid boolean");
		    }
                    boolean slept 
                        = Boolean.valueOf(sleptStr).booleanValue();


                    // Generate the output String.
                    String output = "";
                    try
                    {
                        output = generateString(studentNum, birthMonth, 
                                                birthDate, firstName,
                                                surnameInitial, numMissed, 
                                                slept);
                    }
                    catch (Exception e)
                    {
                        // Generate string found an error with the input
                        // parameters.
                        System.err.println("Exception - " + e);
                    }

                    // Add the generated string to the output file.
                    outFileVector.addElement(output);
                  }
                  catch (Exception e)
                  {
                    System.err.println("WARNING: Invalid input. "
                                       + "Will skip it. -  " + currInLine);
                  }
                }
                else
                {
                    System.err.println("WARNING: Invalid input. "
                                       + "Will skip it. -  " + currInLine);
                }
            }

            // Write out the output file.
            FileSupport.vectorToFile(outFilePath, outFileVector);
        }
        else
        {
            throw new IOException("File does not exist.");
        }
    }


    /**
     * Splits the given string using the delimiter ";", and then storing
     * each item in the vector "v".
     * e.g. Splits the string "a;b;c;d" into a vector containing elements
     *      {"a", "b", "c", "d"}
     *
     * @param str  String to be split
     * @param v    Vector into which the split data is placed
     */
    public void splitString(String str, Vector v)
    {
        // Clear the vector.
        v.removeAllElements();

        // Parse the input line into a Vector.
        int start         = 0;
        int nextDelimiter = str.indexOf(";", start);
        while (nextDelimiter > 0)
        {
            String currString = str.substring(start, nextDelimiter);
            v.addElement(currString);

            start = nextDelimiter+1;
            nextDelimiter = str.indexOf(";", start);
        }
        String lastString = str.substring(start);
        v.addElement(lastString);
    }


    /**
     * Generate marks using the provided parameters, and then generate
     * an output string containing the generated results.
     *
     * If the input parameters contains invalid data, this method will
     * throw an Exception. See project spec for what the valid values of
     * these input parameters are.
     *
     * @param studentNum      The student number
     * @param birthMonth      The month of the student's birthday as a 3
     *                        character string. (e.g.  "Aug")
     * @param birthDate       The date of the student's birthday.
     * @param firstName       The student's first name.
     * @param surnameInitial  The first letter of the student's surname.
     * @param numMissed       The number of missed lectures.
     * @param slept           Whether the student slept in any of the lectures.
     *
     * @return The output string is of the form:
     *             (Assignment 1 mark);(Assignment 2 mark);(Assignment 3 mark);
     *             (Exam mark);(Overall grade for the subject);
     *             (Comment about the student)
     *         e.g. "5;25;10;60;H1;Excellent."
     */
    public String generateString(long studentNum,
                                 String birthMonth, int birthDate,
                                 String firstName, char surnameInitial,
                                 int numMissed, boolean slept)
				throws Exception
 //This Section below has been changed//

    {
        CalcMarks calc = new CalcMarks();

        int a1 = 0;
	int a2 = 0;
	int a3 = 0;
	int ex = 0;
	String grade = "";
	String comment = "";
	String output = "";
        try
        {
            a1 = calc.calcAssignment1Mark(studentNum);
	    a2 = calc.calcAssignment2Mark(birthMonth, birthDate);
            a3 = calc.calcAssignment3Mark(numMissed, slept);
            ex = calc.calcExamMark(firstName, surnameInitial);
            grade   = calc.calcGrade();
            comment = calc.getComment();
            output = studentNum + ";" + a1 + ";" + a2 + ";" + a3 + ";" 
                               + ex + ";" + grade + ";" + comment + ";";

        }
        catch (Exception e)
        {
            // Invalid student number.
            System.err.println("Exception - " + e);
        }

     //   int a2 = calc.calcAssignment2Mark(birthMonth, birthDate);
    //    int a3 = calc.calcAssignment3Mark(numMissed, slept);
    //    int ex = calc.calcExamMark(firstName, surnameInitial);
    //    String grade   = calc.calcGrade();
    //    String comment = calc.getComment();
    //    String output = studentNum + ";" + a1 + ";" + a2 + ";" + a3 + ";" 
    //                       + ex + ";" + grade + ";" + comment + ";";
        return output;
    }

}
