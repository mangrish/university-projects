import java.util.*;
import java.io.*;

/**
 * Class for reading in and writing out text file. It provides the
 * fileToVector() method that reads in a file and stores the information
 * in a Vector (storing each line as a single vector element). It also
 * provides the vectorToFile() method that writes the contents of a
 * vector back into a text file.
 *
 * This class may behave unexpected if used with binary files.
 */
public class FileSupport
{
    public static final String NEWLINE = System.getProperty("line.separator");


    /**
     * Reads the contents of the file "srcFile" and stores it in the 
     * "lines" vector.
     *
     * @param srcFile  Path and filename of the file to read in.
     * @param lines    Vector of Strings. Each element is one line of
     *                 the file. This method will fill this Vector.
     */
    public static void fileToVector(String srcFile, Vector lines)
        throws IOException
    {
        // Clear the lines vector.
        lines.removeAllElements();

        // Read the file if it exists, or throw an exception if it
        // doesn't.
        File f = new File(srcFile);
        if (f.exists())
        {
            // Create a FileReader for reading the file.
            FileReader fileReader = new FileReader(f);
            
            // Read the file into "fileData".
            StringBuffer fileData = new StringBuffer();
            char[] buffer  = new char[1024];
            int    numRead = fileReader.read(buffer, 0, buffer.length);
            while (numRead != -1)
            {
                String buffStr = new String(buffer);
                buffStr = buffStr.substring(0, numRead);
                fileData.append(buffStr);

                numRead = fileReader.read(buffer, 0, buffer.length);
            }

            // Split the string into the "lines" vector.
            int startSearchIndex = 0;
            String fileStr = fileData.toString();
            while (startSearchIndex < fileStr.length())
            {
                int newlineIndex 
                    = fileStr.indexOf(NEWLINE, startSearchIndex);
                String currLine 
                    = fileStr.substring(startSearchIndex, newlineIndex);

                lines.addElement(currLine);

                startSearchIndex = newlineIndex+NEWLINE.length();
            }
        }
        else
        {
            throw new IOException("File does not exist.");
        }
    }


    /**
     * Writes the contents of the "lines" vector out to the specified
     * file "outFile".
     *
     * @param outFile  Path and filename of output file.
     * @param lines    Vector of Strings. Each element is one line of
     *                 the file.
     */
    public static void vectorToFile(String outFile, Vector lines)
        throws IOException
    {
        FileWriter fileWriter = new FileWriter(outFile);
        for (int i=0; i<lines.size(); i++)
        {
            String currLine = ((String) lines.elementAt(i)) + NEWLINE;
            int    len      = currLine.length();

            fileWriter.write(currLine, 0, len);
        }
        fileWriter.close();
    }


    /**
     * Test program for copying a specified text file.
     * argv[0] is the filepath of the file to copy.
     */
    public static void main(String[] argv)
    {
        if (argv.length > 0)
        {
            String filePath = argv[0];
            File   f        = new File(filePath);
            if (f.exists())
            {
                try
                {
                    // Vector into which we read the file.
                    Vector v = new Vector();

                    // Read the file into the vector.
                    FileSupport.fileToVector(filePath, v);

                    // Write the contents of the vector out to a new
                    // file.
                    FileSupport.vectorToFile(filePath + ".test", v);
                }
                catch (Exception e)
                {
                    System.err.println("Exception - " + e);
                }
            }
            else
            {
                System.err.println("Specified file does not exist.");
            }
        }
    }

}

