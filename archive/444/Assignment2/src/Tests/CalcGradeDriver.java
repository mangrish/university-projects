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
    public static void main(String argv[])
    {

        //////////////////////////////////////////////////////////////////
        // Test using the value.
        //////////////////////////////////////////////////////////////////

        // Create the CalcMarks object.
        CalcMarks calc = new CalcMarks();
	
	if(argv.length == 6)
	{
		//set ass1, 2, 3, and exam marks to the command line input
		calc.a1Mark = java.lang.Integer.parseInt(argv[0]);
		calc.a2Mark = java.lang.Integer.parseInt(argv[1]);
		calc.a3Mark = java.lang.Integer.parseInt(argv[2]);
		calc.examMark = java.lang.Integer.parseInt(argv[3]);
		if(argv[4].equalsIgnoreCase("true") )
		{
			calc.sleptDuringLectures = true;
		}
		else
		{
			calc.sleptDuringLectures = false;
		}
		calc.numMissedLectures = java.lang.Integer.parseInt(argv[5]);

	}
        // Test calcExamMark() 
 	String result = calc.calcGrade();
	
      	  System.out.println("Result: " + result);
    }
}
