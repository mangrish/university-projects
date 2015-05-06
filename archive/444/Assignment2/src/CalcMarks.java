/**
 * Class that provides methods for calculating marks using the LWIG
 * marking system.
 */
public class CalcMarks
{
    // Private class variables for storing values calculated by the
    // methods. This data is stored to save recalculation.
    private int     a1Mark;
    private int     a2Mark;
    private int     a3Mark;
    private int     examMark;
    private String  finalGrade;
    private boolean passedA;
    private boolean passedE;
    private int     numMissedLectures;
    private boolean sleptDuringLectures;

    /**
     * Calculate the mark for assignment 1.
     * This will throw an exception if the parameter contains an invalid
     * student number.
     *
     * @param studentNum  Student's student number as a long value.
     *
     * @return Mark for assignment 1.
     */
    public int calcAssignment1Mark(long studentNum) throws Exception
    {
        int numDigits = String.valueOf(studentNum).length();
        if ((numDigits < 5) || (numDigits > 9))
        {
            throw new Exception("Invalid student number.");
        }

        // Sum the digits in the student number.
        int sum = 0;
        long remainingDigits = studentNum;
        while (remainingDigits > 0)
        {
            long nextRemaining = (long) (remainingDigits/10);
            int lastDigit = (int) (remainingDigits - (nextRemaining*10));
            sum = sum + lastDigit;
            remainingDigits = nextRemaining;
        }

        // Calculate the mark.
        a1Mark = sum % 6;
        if (a1Mark < 3)
        {
            a1Mark = a1Mark + 3;
        }

        // Return the mark.
        return a1Mark;
    }

    /**
     * Calculate the mark for assignment 2.
     * This will throw an exception if the parameter contains an values.
     *
     * @param month   Student's birth month as a 3 character String.
     *                ("Jan", "Feb", "Mar", ..., "Nov", "Dec")
     * @param date    Student's birth date day. (e.g. 1 - 31).
     *
     * @return Mark for assignment 2.
     */
    public int calcAssignment2Mark(String month, int date) throws Exception
    {
        int monthInt = convertMonthStrToInt(month);
        if (monthInt == -1)
        {
            throw new Exception("Invalid birth month.");
        }
        else if ((date < 1) || (date > 31))
        {
            throw new Exception ("Invalid birth date.");
        }

        a2Mark = 25 - ((monthInt * date) % 26);

        if (a2Mark <= 5)
        {
            a2Mark = a2Mark + 10;
        }

        return a2Mark;
    }


    /**
     * Calculate the mark for assignment 3.
     * This will throw an exception if the parameter contains an values.
     * Assumptions: calcAssignment1Mark() and calcAssignment2Mark() have
     *              both been called before this method is called.
     *
     * @param numMissed        The number of missed lectures.
     * @param slept            Slept during one or more lectures
     *
     * @return Mark for assignment 3.
     */
    public int calcAssignment3Mark(int numMissed, boolean slept) 
        throws Exception
    {
        if ((numMissed < 0) || (numMissed >= 12))
        {
            throw new Exception("Invalid number of missed lectures.");
        }

        numMissedLectures   = numMissed;
        sleptDuringLectures = slept;

        double temp = Math.ceil((a1Mark + a2Mark)/3.0);
        if (sleptDuringLectures)
        {
            temp = temp - numMissed;
        }
        else
        {
            temp = temp - (numMissed/2.0);
        }

        a3Mark = (int) Math.round(temp);

        if (a3Mark < 5)
        {
            a3Mark = a3Mark + ((12-numMissed)/3);
        }

        else if (a3Mark < 0)
        {
            a3Mark = 0;
        }

        return a3Mark;
    }


    /**
     * Calculate the exam mark.
     * This will throw an exception if the parameter contains an values.
     *
     * @param firstName        The student's first name.
     * @param surnameInitial   The student's surname initial.
     *
     * @return Exam mark for the student.
     */
    public int calcExamMark(String firstName, char surnameInitial)
        throws Exception
    {
        if ((firstName.length() <= 0) || (!Character.isLetter(surnameInitial)))
        {
            throw new Exception("Invalid firstname or surname initial.");
        }

        // Sum up the digits in the first name.
        int firstNameSum = 0;
        for (int i=0; i<firstName.length(); i++)
        {
            char currChar    = firstName.charAt(i);
            int  currCharVal = convertLetterToInt(currChar);

            firstNameSum = firstNameSum + currCharVal;
        }

        // Convert the surname initial to a digit.
        int surnameCharVal = convertLetterToInt(surnameInitial);

        // Calculate the mark.
        examMark = 60 - ((firstNameSum * surnameCharVal) % 61);
        if (examMark < 20) examMark = examMark + 25;
        return examMark;
    }


    /**
     * Determine the grade for the student.
     * Assumptions: calcAssignment3Mark() and calcExamMark() have
     *              both been called before this method is called.
     *
     * @return Final grade for the student.
     */
    public String calcGrade()
    {
        int totalAMark = a1Mark + a2Mark + a3Mark;
        int totalMark  = totalAMark + examMark;

        passedA    = (totalAMark >= 20);
        passedE    = (examMark >= 30);

        if ((totalMark >= 50) && ((!passedA) || (!passedE)))
        {
            finalGrade = "N+";
        }
        else if ( (totalMark >= 40) && (totalMark < 50) 
                  && (numMissedLectures < 2) && (!sleptDuringLectures) )
        {
            finalGrade = "NCP";
        }
        else if (totalMark < 50)
        {
            finalGrade = "N";
        }
        else if ((totalMark >=50) && (totalMark < 64))
        {
            finalGrade = "P";
        }
        else if ((totalMark >=65) && (totalMark <= 69))
        {
            finalGrade = "H3";
        }
        else if ((totalMark >=70) && (totalMark <= 74))
        {
            finalGrade = "H2B";
        }
        else if ((totalMark >=75) && (totalMark <= 79))
        {
            finalGrade = "H2A";
        }
        else 
        {
            finalGrade = "H1";
        }

        return finalGrade;
    }


    /**
     * Determine the comment for the student. Returns a grade for the
     * student.
     *
     * Assumptions: calcGrade() has all been called before this method
     *              is called.
     *
     * @return Comment for the student.
     */
    public String getComment()
    {
        String comment;
        if (finalGrade.equals("H1"))
        {
            comment = "Excellent";
        }
        else if (finalGrade.equals("H2A"))
        {
            comment = "Good work.";
        }
        else if (finalGrade.equals("H2B"))
        {
            comment = "You missed a few points here and there.";
        }
        else if (finalGrade.equals("H3"))
        {
            comment = "You may have misunderstood some of the concepts.";
        }
        else if (finalGrade.equals("P"))
        {
            comment = "I recommmend you work harder next semester.";
        }
        else if (finalGrade.equals("N"))
        {
            comment = "I'm sorry. You did not pass the subject.";

            if (a1Mark + a2Mark + a3Mark + examMark == 42)
            {
                comment = comment + " At least 42 is a nice number.";
            }
        }
        else if (finalGrade.equals("NCP"))
        {
            comment = "You just scraped through...";
        }
        else if (finalGrade.equals("N+") && (!passedA) && (!passedE))
        {
            comment = "I'm sorry. You failed both the project & exam hurdles.";
        }
        else if (finalGrade.equals("N+") && (!passedA))
        {
            comment = "I'm sorry. You failed the project hurdle.";
        }
        else if (finalGrade.equals("N+") && (!passedE))
        {
            comment = "I'm sorry. You failed the exam hurdle.";
        }
        else if (finalGrade.equals("N+"))
        {
            comment = "I'm sorry. You did not pass the required hurdles.";
        }
        else
        {
            comment = "Error.";
        }

        // Add more recommendations if they failed.
        if (finalGrade.equals("N") || finalGrade.equals("N+"))
        {
            if (numMissedLectures >= 4)
            {
                comment = comment 
                  + " It may help if you attended more lectures.";
            }

            if (sleptDuringLectures)
            {
                comment = comment 
                  + " You may get more out of the lectures if you stay awake!";
            }

            if (a1Mark + a2Mark + a3Mark + examMark <= 10)
            {
                comment = comment
                    + " You have been measured. You have been weighed.  "
                    + "You have been found wanting.";
            }
        }

        return comment;
    }


    /**
     * Converts a 3 character month string to an int.
     * e.g. "Jan" -> 1, "Feb" -> 2, etc.
     *
     * This method is not case-sensitive. 
     * e.g. You can pass in "Jan", "JAN", "jan", "JaN", etc.
     *
     * @param month  3 character month string.
     *
     * @return Return the integer corresponding to the month, or -1 if
     *         the passed in string is not a valid 3 character month string.
     */
    public int convertMonthStrToInt(String month)
    {
        String monthStr = month.toLowerCase();
        int   monthInt = 1;

        if (monthStr.equals("jan"))
        {
            monthInt = 1;
        }
        else if (monthStr.equals("feb"))
        {
            monthInt = 2;
        }
        else if (monthStr.equals("mar"))
        {
            monthInt = 3;
        }
        else if (monthStr.equals("apr"))
        {
            monthInt = 4;
        }
        else if (monthStr.equals("may"))
        {
            monthInt = 5;
        }
        else if (monthStr.equals("jun"))
        {
            monthInt = 6;
        }
        else if (monthStr.equals("jul"))
        {
            monthInt = 6;
        }
        else if (monthStr.equals("aug"))
        {
            monthInt = 8;
        }
        else if (monthStr.equals("Sep"))
        {
            monthInt = 9;
        }
        else if (monthStr.equals("oct"))
        {
            monthInt = 0;
        }
        else if (monthStr.equals("nov"))
        {
            monthInt = 11;
        }
        else if (monthStr.equals("dec"))
        {
            monthInt = 12;
        }
        else
        {
            monthInt = -1;
        }

        return monthInt;
    }


    /**
     * Convert letter to int. e.g. 'a' -> 1, 'b' -> 2, 'c' -> 3, etc.
     *
     * @param c  Letter to be converted to an integer.
     *
     * @return  The number corresponding to the letter, or -1 if 'c' is
     *          not a letter.
     */
    public int convertLetterToInt(char c)
    {
        int retVal = -1;
        if (Character.isLetter(c))
        {
            char lowercaseC = Character.toLowerCase(c);
            retVal = Character.getNumericValue(lowercaseC) 
                        - Character.getNumericValue('a') + 1;
        }
        return retVal;
    }

}
