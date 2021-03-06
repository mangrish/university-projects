/* Enrolment.java
 *
 * Created 23 August 2000
 */

package enroller;

import java.io.*;
import org.w3c.dom.*;
import org.xml.sax.*;
import com.sun.xml.tree.*;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;
import java.util.Vector;

/** Class for loading and accessing xml enrolment data for the
 * Enroller.
 *
 * @author Tim Dwyer
 * @version 1.0
 */
public class Enrolment
{
  private XmlDocument doc;
  private String fileName;
  /** Create and load the Enrolment data structures from the data held
   * in the specified file
   * @param fileName the name of the xml file containing the enrolment
   * data (file must use the Enrolment.dtd template)
   */ 
  public Enrolment (String fileName)
  throws IOException
  {
    this.fileName = fileName;
    try {
      //
      // turn the filename into a fully qualified URL
      //
      String uri = "file:" + new File (fileName).getAbsolutePath ();
      System.out.println(uri);

      // (the following comment comes from an example distributed
      // with the JAXP parser)
      // turn it into an in-memory object ... we couple the
      // parser and builder bidirectionally to ensure that 
      // the parser enforces namespace syntax rules (which
      // add constraints that XML 1.0 doesn't have).
      //
      doc = XmlDocument.createXmlDocument(uri,true);

    } catch (SAXException e) {
      Exception	x = e;
      if (e.getException () != null)
       	x = e.getException ();
      x.printStackTrace ();

    } catch (Throwable t) {
      t.printStackTrace ();
    }
  }

  /** wrapper class to access enrolment details for a whole day from
   * the xml data in memory.
   */
  public class Day {
    private Element dayElement;
    protected Day(Element dayElement) {
      this.dayElement = dayElement;
    }
    /** wrapper class to access enrolment details for one timeslot
     */
    public class Time {
      private Element timeElement;
      protected Time(Element timeElement) {
        this.timeElement = timeElement;
      }
      /** returns the start time for the timeslot
       * @return the start time as a String format HH24:MI
       */
      public String getStartTime() {
        return timeElement.getAttribute("StartTime");
      }
      /** get the duration of the class or meeting
       * @return an integer number of hours duration
       */
      public int getDuration() {
        return Integer.parseInt(timeElement.getAttribute("Duration"));
      }
      /** get the maximum number of people allowed to enrol in this
       * timeslot
       * @return the maximum allowed as an integer
       */
      public int getEnrolmentLimit() {
        return Integer.parseInt(timeElement.getAttribute("EnrolmentLimit"));
      }
      /** A typesafe vector to get a list of names enrolled in a
       * given timeslot
       */
      public class EnrolledList extends Vector {
        protected boolean add(String enrolled) {
          return super.add(enrolled);
        }
        /** retrieve the name of the person enrolled at a given
         * location in the list.
         * @param i index of name in the list to retrieve (starts
         * from 0)
         * @return the name as a String
         */
        public String item(int i) {
          return (String)super.elementAt(i);
        }
      }
      /** 
       * @return the list of all those enrolled in this timeslot
       */
      public EnrolledList getEnrolledList() {
        EnrolledList enrolledList = new EnrolledList();
        NodeList enrolledNodes = timeElement.getElementsByTagName("Enrolled");
        for(int i=0; i < enrolledNodes.getLength(); i++) {
          Element e = (Element)enrolledNodes.item(i);
          enrolledList.add(e.getFirstChild().getNodeValue().trim());
        }
        return enrolledList;
      }
     
      /** add a name to this timeslot
       * @param enrolledName the name of the person to add to the
       * list
       */
      public void addEnrolled(
        String enrolledName
      ) {
        Element newEnrolled = doc.createElement("Enrolled");
        newEnrolled.appendChild(doc.createTextNode(enrolledName));
        timeElement.appendChild(newEnrolled);
      }
      
      /** remove a name from this timeslot
       * @param enrolledName the name of the person to remove from the
       * list
       */
      public void removeEnrolled(
        String enrolledName
      ) {
        NodeList enrolledNodes = timeElement.getElementsByTagName("Enrolled");
        for(int i=0; i < enrolledNodes.getLength(); i++) {
          Element e = (Element)enrolledNodes.item(i);
	  if(enrolledName.equals(e.getFirstChild().getNodeValue().trim())) {
	    timeElement.removeChild(e);
	  }
        }
      }
    }
    /** A typesafe vector to get a list of all the timeslots
     * available on this day
     */
    public class TimeList extends Vector {
      protected boolean add(Time time) {
        return super.add(time);
      }
      /** retrieve the timeslot at a given location in the list
       * @param i index in the list of the time to retrieve (starts
       * from 0).
       * @return the time at that index
       */
      public Time item(int i) {
        return (Time)super.elementAt(i);
      }
    }
    /** Get a list of all the times available on this day
     * @return the list of all the times available on this day
     */
    public TimeList getTimeList() {
      TimeList times = new TimeList();
      NodeList timeNodes = dayElement.getElementsByTagName("Time");
      for(int i=0; i < timeNodes.getLength(); i++) {
        times.add(new Time((Element)timeNodes.item(i)));
      }
      return times;
    }
    /** Get a text description name for the day (eg "Wednesday")
     * @return a string representing the day
     */
    public String getName() {
      return dayElement.getAttribute("Name");
    }
  }

  /** A typesafe vector to get a list of all the days with times
   * allocated in this enrolment timetable.
   */
  public class DayList extends Vector {
    protected DayList() {
      super();
    }
    protected boolean add(Day day) {
      return super.add(day);
    }
    /** retrieve the day at a given location in the list
     * @param i index in the list of the day to retrieve (starts from
     * 0).
     * @return the day at that index
     */
    public Day item(int i) {
      return (Day)super.elementAt(i);
    }
  }

  /** get the list of all days with times allocated in this
   * enrolment timetable
   * @return DayList of days 
   */
  public DayList getDayList() {
    DayList days = new DayList();
    NodeList dayNodes =
      doc.getDocumentElement().getElementsByTagName("Day");
    for(int i=0; i < dayNodes.getLength(); i++) {
      days.add(new Day((Element)dayNodes.item(i)));
    }
    return days;
  }

  /** get the name of the subject in this enrolment file
   * @return String the name of the subject
   */
  public String getSubjectName() {
    Element e = doc.getDocumentElement();
    return e.getAttribute("SubjectName");
  }

  /** write out the updated data
   */
  public void save() {
    try {
      FileWriter out = new FileWriter(fileName);
      doc.write(out);
    } catch(Exception e) {
      e.printStackTrace();
    }
  }

}
