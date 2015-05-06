 /*
 * The driver to test getComment method in CalcMarks class
 *
 * Compile it with : 'javac getCommentDriver.java'
 * Run it with     : 'java getCommentDriver <final grade, passedExam, 
			passedAssignment>
 */
public class getCommentDriver
{
    /**
     * Test the getComment() method in the CalcMarks class.
     */
    public static void main(String argv[])
    {

        //////////////////////////////////////////////////////////////////
        // Test using the value.
        //////////////////////////////////////////////////////////////////

        // Create the CalcMarks object.
        CalcMarks calc = new CalcMarks();

	if(argv.length == 1)
	{
		calc.finalGrade = argv[0];
	}
	else if(argv.length == 3)
	{
		calc.finalGrade = argv[0];
		if(argv[1].equals("true"))
		{
			calc.passedE = true;
		}
		else
		{
			calc.passedE = false;
		}
		if(argv[2].equals("true")) 
		{
			calc.passedA = true;
		}
		else
		{
			calc.passedA = false;
		}

	}
        // Test getComment() 

        // This method may throw an exception, so we need to wrap it
        // in a try-catch block.
        String result = calc.getComment();

            // Print out the result to standard out.
            System.out.println("Result: " + result);
    }
}
