 /*
 * The driver to test convertLetterToInt method in CalcMarks class
 *
 * Compile it with : 'javac convertLetterToIntDriver.java'
 * Run it with     : 'java convertLetterToIntDriver <letter> 
 */
public class convertLetterToIntDriver
{
    /**
     * Test the convertLetterToInt(char letter) method in the CalcMarks class.
     */
    public static void main(String argv[])
    {

	if(argv.length != 1)
	{
		System.err.println("Commnad line error.");
		System.exit(0);
	}

	char letter = argv[0].charAt(0);

        //////////////////////////////////////////////////////////////////
        // Test using the value.
        //////////////////////////////////////////////////////////////////

        // Create the CalcMarks object.
        CalcMarks calc = new CalcMarks();

        // Test convertMonthStrToInt() 

        int result = calc.convertLetterToInt(letter);

            // Print out the result to standard out.
            System.out.println("Result: " + result);
    }
}
