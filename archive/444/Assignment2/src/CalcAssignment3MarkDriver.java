 /*
 * The driver to test CalcAssignment3Mark method in CalcMarks class
 *
 * Compile it with : 'javac CalcAssignment3MarkDriver.java'
 * Run it with     : 'java CalcAssignment3MarkDriver <student number>'.
 */
public class CalcAssignment3MarkDriver
{
    /**
     * Test the calcAssignment3Mark() method in the CalcMarks class.
     */
    public static void main(String argv[])
    {
	int lectsMissed = -1;
	boolean didSleep = true;
	long studentNum = 12345;
	String birthMonth = "Jun";
	int birthDate = 17;

        if (argv.length < 2)
        {
            System.err.println("Incorrect command line.");
            System.exit(0);
        }
        else
        {
		lectsMissed = java.lang.Integer.parseInt(argv[0]);
		
		if(argv[1].equalsIgnoreCase("true") )
		{
			didSleep = true;
		}
		else if(argv[1].equalsIgnoreCase("false"))
		{
			didSleep = false;
		}
		else
		{
			System.err.println("Error in command line.");
			System.exit(0);
		}

        }

        //////////////////////////////////////////////////////////////////
        // Test using the value.
        //////////////////////////////////////////////////////////////////

        // Create the CalcMarks object.
        CalcMarks calc = new CalcMarks();

	//Initialise the assignment 1 and 2 marks so that the assignment 3
	//mark can be tested. These marks can change depending on the test

        // Test calcAssignment3Mark() with student number 199224950.
        try
        {
            // This method may throw an exception, so we need to wrap it
            // in a try-catch block.
	    calc.calcAssignment1Mark(studentNum);
	    calc.calcAssignment2Mark(birthMonth, birthDate);
            int result = calc.calcAssignment3Mark(lectsMissed, didSleep);

            // Print out the result to standard out.
            System.out.println("Result: " + result);
        }
        catch (Exception e)
        {
            System.out.println("calcAssignment3Mark() threw an exception");
            System.out.println(e);
        }
    }
}
