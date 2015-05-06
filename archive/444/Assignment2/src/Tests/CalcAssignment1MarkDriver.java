 /*
 * The driver to test CalcAssignment1Mark method in CalcMarks class
 *
 * Compile it with : 'javac CalcAssignment1MarkDriver.java'
 * Run it with     : 'java CalcAssignment1MarkDriver <student number>'.
 */
public class CalcAssignment1MarkDriver
{
    /**
     * Test the calcAssignment1Mark() method in the CalcMarks class.
     */
    public static void main(String argv[])
    {
        //////////////////////////////////////////////////////////////////
        // Read student number from the command line arguments.
        //////////////////////////////////////////////////////////////////
        long studentNumber = 0;
        if (argv.length < 1)
        {
            System.err.println("Please specify a student number.");
            System.exit(0);
        }
        else
        {
            try
            {
                studentNumber = Long.valueOf(argv[0]).longValue();
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
        CalcMarks calc = new CalcMarks();

        // Test calcAssignment1Mark() with student number 199224950.
        try
        {
            // This method may throw an exception, so we need to wrap it
            // in a try-catch block.
            int result = calc.calcAssignment1Mark(studentNumber);

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
