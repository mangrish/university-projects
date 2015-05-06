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

	static int lectsMissed = 1;
	static boolean didSleep = true;
	static long studentNum = 12345;
	static String birthMonth = "Jun";
	static int birthDate = 17;
	static String firstName = "John";
	static char surnameInitial = 'D';

    public static void main(String argv[])
    {

        //////////////////////////////////////////////////////////////////
        // Test using the value.
        //////////////////////////////////////////////////////////////////

        // Create the CalcMarks object.
        CalcMarks calc = new CalcMarks();

        // Test getComment() 

        // This method may throw an exception, so we need to wrap it
        // in a try-catch block.
	try {
	  	calc.calcAssignment1Mark(studentNum);
	    	calc.calcAssignment2Mark(birthMonth, birthDate);
        	calc.calcAssignment3Mark(lectsMissed, didSleep);
		calc.calcExamMark(firstName, surnameInitial);
 		calc.calcGrade();
        String result = calc.getComment();

            // Print out the result to standard out.
            System.out.println("Result: " + result);

		} catch (Exception e) {
	    	System.out.println("calcAssignment3Mark() threw an exception");
        	System.out.println(e);
    	}

    }
}
