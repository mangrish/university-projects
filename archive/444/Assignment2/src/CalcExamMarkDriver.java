 /*
 * The driver to test CalcExamMark method in CalcMarks class
 *
 * Compile it with : 'javac CalcExamMarkDriver.java'
 * Run it with     : 'java CalcExamMarkDriver <firstname, surnameInitial>'.
 */
public class CalcExamMarkDriver
{
    /**
     * Test the calcExamMark() method in the CalcMarks class.
     */
    public static void main(String argv[])
    {
	String firstName  = null;
	char surnameInitial = 'a';

        if (argv.length < 2)
        {
            System.err.println("Incorrect command line.");
            System.exit(0);
        }
        else
        {
		firstName = argv[0];
		surnameInitial = argv[1].charAt(0);

        }

        //////////////////////////////////////////////////////////////////
        // Test using the value.
        //////////////////////////////////////////////////////////////////

        // Create the CalcMarks object.
        CalcMarks calc = new CalcMarks();

        // Test calcExamMark() 
        try
        {
            // This method may throw an exception, so we need to wrap it
            // in a try-catch block.
            int result = calc.calcExamMark(firstName, surnameInitial);

            // Print out the result to standard out.
            System.out.println("Result: " + result);
        }
        catch (Exception e)
        {
            System.out.println("calcAssignment1Mark() threw an exception");
            System.out.println(e);
        }
    }
}
