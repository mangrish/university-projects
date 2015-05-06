 /*
 * The driver to test convertMonthStrToInt method in CalcMarks class
 *
 * Compile it with : 'javac convertMonthStrToIntDriver.java'
 * Run it with     : 'java convertMonthStrToInt <month> 
 */
public class convertMonthStrToIntDriver
{
    /**
     * Test the convertMonthStrToInt(String Month) method in the CalcMarks class.
     */
    public static void main(String argv[])
    {

	if(argv.length != 1)
	{
		System.err.println("Commnad line error.");
		System.exit(0);
	}

	String month = argv[0];

        //////////////////////////////////////////////////////////////////
        // Test using the value.
        //////////////////////////////////////////////////////////////////

        // Create the CalcMarks object.
        CalcMarks calc = new CalcMarks();

        // Test convertMonthStrToInt() 

        int result = calc.convertMonthStrToInt(month);

            // Print out the result to standard out.
            System.out.println("Result: " + result);
    }
}
