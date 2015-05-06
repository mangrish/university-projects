 /*
 * The driver to test CalcAssignment2Mark method in CalcMarks class
 *
 * Compile it with : 'javac CalcAssignment2MarkDriver.java'
 * Run it with     : 'java CalcAssignment2MarkDriver <student number>'.
 */
public class CalcAssignment2MarkDriver
{
    /**
     * Test the calcAssignment2Mark() method in the CalcMarks class.
     */
    public static void main(String argv[])
    {
	String month=null;
	int date=0;	

        if (argv.length < 2)
        {
            System.err.println("Incorrect command line argument.");
            System.exit(0);
        }
        else
        {
		month = argv[0];
		date = java.lang.Integer.parseInt(argv[1]);
        }

        //////////////////////////////////////////////////////////////////
        // Test using the value.
        //////////////////////////////////////////////////////////////////

        // Create the CalcMarks object.
        CalcMarks calc = new CalcMarks();

        try
        {
            // This method may throw an exception, so we need to wrap it
            // in a try-catch block.
            int result = calc.calcAssignment2Mark(month, date);

            // Print out the result to standard out.
            System.out.println("Result: " + result);
        }
        catch (Exception e)
        {
            System.out.println("calcAssignment2Mark() threw an exception");
            System.out.println(e);
        }
    }
}
