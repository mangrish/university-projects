import junit.framework.*;


/**
 * A sample test case, testing <code>java.util.Vector</code>.
 *
 */
public class A1Test extends TestCase {
	protected CalcMarks calc;

	public static void main (String[] args) {
		junit.swingui.TestRunner.run (A1Test.class);
	}

	public A1Test( String name) {
		super(name);
	}

	protected void setUp() {
		calc = new CalcMarks();
	}

	public void testCalcAssignment1Mark5numbers() {
		try
		{
			int expected = calc.calcAssignment1Mark(12345);
			assertEquals(3, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should equal 3, not an exeption");
		}
	}

	public void testCalcAssignment1Mark6numbers() {
		try
		{
			int expected = calc.calcAssignment1Mark(123456);
			assertEquals(3, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should equal 3, not an exeption");
		}
	}

	public void testCalcAssignment1Mark9numbers() {
		try
		{
			int expected = calc.calcAssignment1Mark(123456789);
			assertEquals(3, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should equal 3, not an exeption");
		}
	}

	public void testCalcAssignment1Mark4numbers() {
		try
		{
			int expected = calc.calcAssignment1Mark(1234);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exception");
	}

	public void testCalcAssignment1Mark7numbers() {
		try
		{
			int expected = calc.calcAssignment1Mark(1234567);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exception");
	}

	public void testCalcAssignment1Mark8numbers() {
		try
		{
			int expected = calc.calcAssignment1Mark(12345678);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exception");
	}

	public void testCalcAssignment1Mark10numbers() {
		try
		{
			int expected = calc.calcAssignment1Mark(1234567890);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exception");
	}
}