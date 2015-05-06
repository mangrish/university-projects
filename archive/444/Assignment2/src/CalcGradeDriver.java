 /*
 * The driver to test CalcGrade method in CalcMarks class
 *
 * Compile it with : 'javac CalcGradeDriver.java'
 * Run it with     : 'java CalcGradeDriver <ass1 mark, ass2 mark, ass3 mark
 *			exam mark, slept, lectures missed>
 */
public class CalcGradeDriver
{
    /**
     * Test the calcGrade() method in the CalcMarks class.
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
	
        // Test calcExamMark() 
		try {
	  		calc.calcAssignment1Mark(studentNum);
	    	calc.calcAssignment2Mark(birthMonth, birthDate);
        	calc.calcAssignment3Mark(lectsMissed, didSleep);
			calc.calcExamMark(firstName, surnameInitial);
 			String result = calc.calcGrade();

			System.out.println("Result: " + result);
		} catch (Exception e) {
	    	System.out.println("calcAssignment3Mark() threw an exception");
        	System.out.println(e);
    	}

    }
}
