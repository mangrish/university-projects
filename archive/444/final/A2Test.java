import junit.framework.*;


/**
 * A sample test case, testing <code>java.util.Vector</code>.
 *
 */
public class A2Test extends TestCase {
	protected CalcMarks calc;

	public static void main (String[] args) {
		junit.swingui.TestRunner.run (A2Test.class);
	}

	public A2Test( String name) {
		super(name);
	}

	protected void setUp() {
		calc = new CalcMarks();
	}


//////////////////////////////////////////////////////////
///// 2 letter  TESTS
//////////////////////////////////////////////////////////
	public void testCalcAssignment1MarkInvalid2LetterString() {
		try
		{
			int expected = calc.calcAssignment2Mark("ju", 1);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exeption");
	}


//////////////////////////////////////////////////////////
///// 4 letter  TESTS
//////////////////////////////////////////////////////////
	public void testCalcAssignment1MarkInvalid4LetterString() {
		try
		{
			int expected = calc.calcAssignment2Mark("june", 1);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exeption");
	}

//////////////////////////////////////////////////////////
///// Empty string TESTS
//////////////////////////////////////////////////////////
	public void testCalcAssignment1MarkInvalidEmptyString() {
		try
		{
			int expected = calc.calcAssignment2Mark("", 0);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exeption");
	}





//////////////////////////////////////////////////////////
///// JAN TESTS
//////////////////////////////////////////////////////////


	public void testCalcAssignment2MarkValidJan1() {
		try
		{
			int expected = calc.calcAssignment2Mark("jan", 1);
			assertEquals(24, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkValidJan31() {
		try
		{
			int expected = calc.calcAssignment2Mark("jan", 31);
			assertEquals(20, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkInvalidJan0() {
		try
		{
			int expected = calc.calcAssignment2Mark("jan", 0);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exeption");
	}

	public void testCalcAssignment1MarkInvalidJan32() {
		try
		{
			int expected = calc.calcAssignment2Mark("jan", 32);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exception");
	}

//////////////////////////////////////////////////////////
///// FEB TESTS
//////////////////////////////////////////////////////////


	public void testCalcAssignment2MarkValidFeb1() {
		try
		{
			int expected = calc.calcAssignment2Mark("feb", 1);
			assertEquals(23, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkValidFeb29() {
		try
		{
			int expected = calc.calcAssignment2Mark("feb", 29);
			assertEquals(19, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkInvalidFeb0() {
		try
		{
			int expected = calc.calcAssignment2Mark("feb", 0);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exeption");
	}

	public void testCalcAssignment1MarkInvalidFeb30() {
		try
		{
			int expected = calc.calcAssignment2Mark("feb", 30);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exception");
	}

//////////////////////////////////////////////////////////
///// MAR TESTS
//////////////////////////////////////////////////////////


	public void testCalcAssignment2MarkValidMar1() {
		try
		{
			int expected = calc.calcAssignment2Mark("mar", 1);
			assertEquals(22, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkValidMar31() {
		try
		{
			int expected = calc.calcAssignment2Mark("mar", 31);
			assertEquals(10, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkInvalidMar0() {
		try
		{
			int expected = calc.calcAssignment2Mark("mar", 0);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exeption");
	}

	public void testCalcAssignment1MarkInvalidMar32() {
		try
		{
			int expected = calc.calcAssignment2Mark("mar", 32);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exception");
	}

//////////////////////////////////////////////////////////
///// APR TESTS
//////////////////////////////////////////////////////////


	public void testCalcAssignment2MarkValidApr1() {
		try
		{
			int expected = calc.calcAssignment2Mark("Apr", 1);
			assertEquals(21, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkValidApr30() {
		try
		{
			int expected = calc.calcAssignment2Mark("Apr", 30);
			assertEquals(9, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkInvalidApr0() {
		try
		{
			int expected = calc.calcAssignment2Mark("Apr", 0);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exeption");
	}

	public void testCalcAssignment1MarkInvalidApr31() {
		try
		{
			int expected = calc.calcAssignment2Mark("Apr", 31);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exception");
	}

//////////////////////////////////////////////////////////
///// MAY TESTS
//////////////////////////////////////////////////////////


	public void testCalcAssignment2MarkValidMay1() {
		try
		{
			int expected = calc.calcAssignment2Mark("May", 1);
			assertEquals(20, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkValidMay31() {
		try
		{
			int expected = calc.calcAssignment2Mark("May", 31);
			assertEquals(10, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkInvalidMay0() {
		try
		{
			int expected = calc.calcAssignment2Mark("May", 0);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exeption");
	}

	public void testCalcAssignment1MarkInvalidMay32() {
		try
		{
			int expected = calc.calcAssignment2Mark("May", 32);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exception");
	}

//////////////////////////////////////////////////////////
///// JUN TESTS
//////////////////////////////////////////////////////////


	public void testCalcAssignment2MarkValidJun1() {
		try
		{
			int expected = calc.calcAssignment2Mark("Jun", 1);
			assertEquals(19, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkValidJun30() {
		try
		{
			int expected = calc.calcAssignment2Mark("Jun", 30);
			assertEquals(11, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkInvalidJun0() {
		try
		{
			int expected = calc.calcAssignment2Mark("Jun", 0);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exeption");
	}

	public void testCalcAssignment1MarkInvalidJun31() {
		try
		{
			int expected = calc.calcAssignment2Mark("Jun", 31);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exception");
	}

//////////////////////////////////////////////////////////
///// JUL TESTS
//////////////////////////////////////////////////////////


	public void testCalcAssignment2MarkValidJul1() {
		try
		{
			int expected = calc.calcAssignment2Mark("Jul", 1);
			assertEquals(18, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkValidJul31() {
		try
		{
			int expected = calc.calcAssignment2Mark("Jul", 31);
			assertEquals(16, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkInvalidJul0() {
		try
		{
			int expected = calc.calcAssignment2Mark("Jul", 0);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exeption");
	}

	public void testCalcAssignment1MarkInvalidJul32() {
		try
		{
			int expected = calc.calcAssignment2Mark("Jul", 32);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exception");
	}

//////////////////////////////////////////////////////////
///// AUG TESTS
//////////////////////////////////////////////////////////


	public void testCalcAssignment2MarkValidAug1() {
		try
		{
			int expected = calc.calcAssignment2Mark("Aug", 1);
			assertEquals(17, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkValidAug31() {
		try
		{
			int expected = calc.calcAssignment2Mark("Aug", 31);
			assertEquals(11, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkInvalidAug0() {
		try
		{
			int expected = calc.calcAssignment2Mark("Aug", 0);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exeption");
	}

	public void testCalcAssignment1MarkInvalidAug32() {
		try
		{
			int expected = calc.calcAssignment2Mark("Aug", 32);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exception");
	}

//////////////////////////////////////////////////////////
///// SEP TESTS
//////////////////////////////////////////////////////////


	public void testCalcAssignment2MarkValidSep1() {
		try
		{
			int expected = calc.calcAssignment2Mark("Sep", 1);
			assertEquals(16, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkValidSep30() {
		try
		{
			int expected = calc.calcAssignment2Mark("Sep", 30);
			assertEquals(15, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkInvalidSep0() {
		try
		{
			int expected = calc.calcAssignment2Mark("Sep", 0);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exeption");
	}

	public void testCalcAssignment1MarkInvalidSep31() {
		try
		{
			int expected = calc.calcAssignment2Mark("Sep", 31);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exception");
	}

//////////////////////////////////////////////////////////
///// OCT TESTS
//////////////////////////////////////////////////////////


	public void testCalcAssignment2MarkValidOct1() {
		try
		{
			int expected = calc.calcAssignment2Mark("Oct", 1);
			assertEquals(15, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkValidOct31() {
		try
		{
			int expected = calc.calcAssignment2Mark("Oct", 31);
			assertEquals(11, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkInvalidOct0() {
		try
		{
			int expected = calc.calcAssignment2Mark("Oct", 0);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exeption");
	}

	public void testCalcAssignment1MarkInvalidOct32() {
		try
		{
			int expected = calc.calcAssignment2Mark("Oct", 32);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exception");
	}

//////////////////////////////////////////////////////////
///// NOV TESTS
//////////////////////////////////////////////////////////


	public void testCalcAssignment2MarkValidNov1() {
		try
		{
			int expected = calc.calcAssignment2Mark("Nov", 1);
			assertEquals(14, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkValidNov30() {
		try
		{
			int expected = calc.calcAssignment2Mark("Nov", 30);
			assertEquals(7, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkInvalidNov0() {
		try
		{
			int expected = calc.calcAssignment2Mark("Nov", 0);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exeption");
	}

	public void testCalcAssignment1MarkInvalidNov31() {
		try
		{
			int expected = calc.calcAssignment2Mark("Nov", 31);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exception");
	}

//////////////////////////////////////////////////////////
///// DEC TESTS
//////////////////////////////////////////////////////////


	public void testCalcAssignment2MarkValidDec1() {
		try
		{
			int expected = calc.calcAssignment2Mark("Dec", 1);
			assertEquals(13, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkValidDec31() {
		try
		{
			int expected = calc.calcAssignment2Mark("Dec", 31);
			assertEquals(17, expected);
			return;
		}
		catch (Exception e)
		{
			fail("Should not be an exeption");
		}
	}

	public void testCalcAssignment1MarkInvalidDec0() {
		try
		{
			int expected = calc.calcAssignment2Mark("Dec", 0);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exeption");
	}

	public void testCalcAssignment1MarkInvalidDec32() {
		try
		{
			int expected = calc.calcAssignment2Mark("Dec", 32);
			
		}
		catch (Exception e)
		{
			return;
		}
		fail("Should be an exception");
	}


}