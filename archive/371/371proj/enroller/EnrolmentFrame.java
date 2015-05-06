package enroller;

import javax.swing.*;
import javax.accessibility.*;
import java.awt.event.*;
import java.awt.*;
import javax.swing.border.Border;

/** A frame to display the Enrolment timetable GUI
 * 
 * @author Mark Angrish 
 * @version 1.0 10/2000
 */
public class EnrolmentFrame extends JInternalFrame implements ActionListener {
  static int openFrameCount = 0;
  static final int xOffset = 15, yOffset = 15;

	// global private variables
  private Enrolment enrolment;
  private Enrolment.Day day;
  private Enrolment.DayList days ;
  private Enrolment.Day.Time time;
  private Enrolment.Day.TimeList times;
  private Enrolment.Day.Time.EnrolledList enrolledList;

	private Enrolment.DayList days1;
  private Enrolment.Day day1;
  private Enrolment.Day.Time time1;
  private Enrolment.Day.TimeList times1;
  private Enrolment.Day.Time.EnrolledList enrolledList1;

  private Enrolment.DayList days2;
  private Enrolment.Day day2;
  private Enrolment.Day.Time time2;
  private Enrolment.Day.TimeList times2;
  private Enrolment.Day.Time.EnrolledList enrolledList2;

  private String user;

	// the main panel for each screen in the wizard
  private JPanel introPanel = new JPanel();
  private JPanel mainPanel = new JPanel(); 
  private JPanel confirmPanel = new JPanel();
  private JPanel clashPanel = new JPanel();

	// components required for the intro screen
  private JButton introNextButton = new JButton("Next >");
  private JButton introQuitButton = new JButton("Quit");
  private JButton introCancelButton = new JButton("Cancel");
  private JComboBox introSelectCombo;
  private JSeparator introSepTop = new JSeparator();
  private JSeparator introSepBottom = new JSeparator();	

	// components for the main tute enrol screen
  private JPanel mainNorthPanel = new JPanel();
  private JPanel mainCenterPanel = new JPanel();
  private JPanel mainSouthPanel = new JPanel();
  private JPanel leftCenterPane = new JPanel();//split pane left
  private JPanel rightCenterPane = new JPanel();//split pane right
  private JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, leftCenterPane, rightCenterPane);//the split pane
  private JPanel gridPane1 = new JPanel();//the grids for the gridlayout of left pane
  private JPanel gridPane2 = new JPanel();
  private JPanel gridPane3 = new JPanel();
  private JPanel gridPane4 = new JPanel();
  private JPanel gridPane5 = new JPanel();
  private JPanel gridPane6 = new JPanel();
  private JButton mainBackButton = new JButton("< Back");
  private JButton mainNextButton = new JButton("Next >");
  private JButton mainQuitButton = new JButton("Quit");
  private JButton mainCancelButton = new JButton("Cancel");
  private JButton enrolButton = new JButton("Enroll");
  private JButton unenrolButton = new JButton("Unenroll");
  private JButton clashButton = new JButton("Clash resolution form");
  private JLabel mainDayLabel = new JLabel("Day:");
  private JLabel mainStartLabel = new JLabel("Time:");
  private JLabel mainDurationLabel = new JLabel("Duration:");
  private JLabel mainLocationLabel = new JLabel("Location:");
  private JLabel mainCapacityLabel = new JLabel("Capacity:");
  private JLabel mainAvailLabel = new JLabel("Available:");
  private JLabel mainEnrolledLabel = new JLabel("Enrolled:");
  private JTextArea mainDayArea = new JTextArea();
  private JTextArea mainStartArea = new JTextArea();
  private JTextArea mainDurationArea = new JTextArea();
  private JTextArea mainLocationArea = new JTextArea();	
  private JTextArea mainCapacityArea = new JTextArea();	
  private JTextArea mainAvailArea = new JTextArea();	
  private JSeparator mainSepTop= new JSeparator();
  private JSeparator mainSepBottom= new JSeparator();
  private JSeparator sepTopRite = new JSeparator();
  private JSeparator sepBottomRite = new JSeparator();
  private JLabel status1 = new JLabel();// the enrolled/unenrolled label
  private	JTextArea textArea = new JTextArea(45,55 );//the area of enrolled students
  private JScrollPane scrollPane = new JScrollPane(textArea,ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS, ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER); //scrool pane for the text area

	// components for the final confirmation panel
  private JButton confirmBackButton = new JButton("< Back");
  private JButton confirmCancelButton = new JButton("Cancel");	
  private JButton saveButton = new JButton("Save and Exit");
  private JButton confirmQuitButton = new JButton("Quit");
  private JLabel confirmLoginLabel = new JLabel("Login:");
  private JLabel confirmDayLabel = new JLabel("Day:");
  private JLabel confirmStartLabel = new JLabel("Time:");
  private JLabel confirmDurationLabel = new JLabel("Duration:");
  private JLabel confirmLocationLabel = new JLabel("Location:");
  private JTextArea confirmLoginArea = new JTextArea();
  private JTextArea confirmDayArea = new JTextArea();
  private JTextArea confirmStartArea = new JTextArea();
  private JTextArea confirmDurationArea = new JTextArea();
  private JTextArea confirmLocationArea = new JTextArea();
  private JSeparator confirmSepTop= new JSeparator();
  private JSeparator confirmSepBottom= new JSeparator();

	// days in week for comparing and displaying in left split pane
  private String monday = "Monday";
  private String tuesday = "Tuesday";	
  private String wednesday = "Wednesday";
  private String thursday = "Thursday";
  private String friday = "Friday";

	// global counters
  private int tuteNum = 0; //counter for tutes in a particular day
  private int dayNum = 0;//int representation of days of the week (1:monday-5:friday)
  private int totTute = 1;//counter for total number of tutes



  public EnrolmentFrame(String fileName, String userName) {
    super("Tute Enrol" + (++openFrameCount), 
        false, //not resizable
        false, //not closable
        false, //not maximizable
        true);//iconifiable

    initGUI(fileName,userName);
		if (openFrameCount == 1) 
			setLocation(280, 120);
		else
     setLocation(280 + xOffset*openFrameCount, 120 + yOffset*openFrameCount);
  }

 /** create the enrolment frame to display details from the
   * given file and to enrol the given user
   *
   * @param fileName name of xml file holding enrolment data
   * @param userName the name of the person to add to the enrolment
   * list
  */

  public void initGUI(String fileName, String userName){
	try {
  		enrolment = new Enrolment(fileName);
    	days = enrolment.getDayList();
		user = userName;

		introPane();// create selection pane
    	setContentPane(introPanel);//add it to the content pane
		setSize(440,340);//set the size
		setVisible(true);//and visibility

		mainPane();//create the tute enrol screen

		confirmPane();//create the final confirmation screen

    } catch(java.io.IOException e) {
	  //catch any exceptions and print the errors
			e.printStackTrace();
	  }
  }

	/** create the selection screen to display the subjects
	  * available 
	 */
  public void introPane() {
	//create new components/layout managers
	JLabel title1 = new JLabel("CSSE Tute Enrol Semester 2, 2000");
	JLabel selectSubject = new JLabel("Select a subject:");
	GridBagLayout gbl1  = new GridBagLayout();
	GridBagConstraints gbc1 = new GridBagConstraints();
	JComboBox subjectSelect = new JComboBox();
	JPanel quitPanel = new JPanel();

	introPanel.setLayout(gbl1); // layout for this screen

	// create and set the quit button
	quitButton(introQuitButton);
	introQuitButton.setToolTipText("Quit out of Tute Enrol");
	introQuitButton.setMnemonic(KeyEvent.VK_Q);

	// create, add the listener and set the cancel button
	introCancelButton.setActionCommand("cancel");
	introCancelButton.setToolTipText("Quit to menu");
	introCancelButton.addActionListener(this);
	introCancelButton.setMnemonic(KeyEvent.VK_C);

	// set the font for the title
	title1.setFont(new Font("Times-Roman", Font.BOLD + Font.ITALIC, 16));

	// create the next button, add the listener 
	introNextButton.setToolTipText("To the next screen");
	introNextButton.setMnemonic(KeyEvent.VK_N);
	introNextButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
			setVisible(false);
			setContentPane(mainPanel);
			setSize(900, 540);
			setLocation(35, 30);
			setVisible(true);
		}
	});

	//there really should be a chooser type of combo box here, but for the sake of this program
	//there is only one subject in the combo box
	subjectSelect.addItem(enrolment.getSubjectName());
	subjectSelect.setToolTipText("Select a subject from the following list");

	//create the layout of the screen using gridbaglayout
	gbc1.anchor = GridBagConstraints.NORTHWEST;
	gbc1.gridwidth = GridBagConstraints.REMAINDER;
	introPanel.add(title1, gbc1);

	gbc1.anchor = GridBagConstraints.NORTH;
	gbc1.fill = GridBagConstraints.HORIZONTAL;
	gbc1.insets = new Insets(0, 0 , 10, 0);
	introPanel.add(introSepTop, gbc1);

	gbc1.anchor = GridBagConstraints.WEST;
	gbc1.gridwidth = 1;
	introPanel.add(selectSubject, gbc1);
	gbc1.insets = new Insets(80, 0, 80, 0);

	introPanel.add(Box.createHorizontalStrut(10));
	gbc1.gridwidth = GridBagConstraints.NONE;
	gbc1.fill = GridBagConstraints.NONE;
	introPanel.add(subjectSelect, gbc1);

	JPanel rightPanel = new JPanel();
	rightPanel.setLayout(new FlowLayout());
	rightPanel.add(introCancelButton);
	rightPanel.add(introNextButton);

	JPanel leftPanel = new JPanel();
	leftPanel.setLayout(new FlowLayout());
	leftPanel.add(introQuitButton);

	//create the button panel for the buttons on this screen
	ButtonPanel1 buttonPanel1 = new ButtonPanel1();
	buttonPanel1.add(leftPanel, BorderLayout.WEST);
	buttonPanel1.add(rightPanel, BorderLayout.EAST);
	introPanel.add(buttonPanel1, gbc1);

	gbc1.anchor = GridBagConstraints.SOUTH;
	gbc1.insets = new Insets(15, 0, 0, 0);
	gbc1.fill  = GridBagConstraints.HORIZONTAL;
	gbc1.gridwidth = 7;
	introPanel.add(buttonPanel1, gbc1);
  }

  /** create the main tute enrol screen
	*
	*
   */
  public void mainPane() {
	// create new internal components
	JPanel filler1 = new JPanel();
	JPanel filler2 = new JPanel();
	JPanel filler3 = new JPanel();
	JPanel filler4 = new JPanel();
	JPanel filler5 = new JPanel();
	JPanel filler6 = new JPanel();
	Border etched = BorderFactory.createEtchedBorder();
    JLabel title2 = new JLabel("CSSE Tute Enrol Semester 2, 2000:" + enrolment.getSubjectName() );
	JLabel statusLabel = new JLabel("STATUS: You are currently");
	TitlePanel2 titlePanel2 = new TitlePanel2();

	// create a new quit button and set its characteristics
	quitButton(mainQuitButton);
	mainQuitButton.setToolTipText("Quit out of Tute Enrol");
	mainQuitButton.setMnemonic(KeyEvent.VK_Q);

	mainLocationArea.setBackground(Color.lightGray);

	// set the text areas
	mainStartArea.setEditable(false);
	mainStartArea.setToolTipText("Start time of tutorial");
	mainDayArea.setEditable(false);
	mainDayArea.setToolTipText("Day of tutorial");
	mainLocationArea.setEditable(false);
	mainLocationArea.setToolTipText("Location of tutorial");
	mainDurationArea.setEditable(false);
	mainDurationArea.setToolTipText("Length of tutorial");
	mainCapacityArea.setEditable(false);
	mainCapacityArea.setToolTipText("Maximum number of students for tutorial");
	mainAvailArea.setEditable(false);
	mainAvailArea.setToolTipText("Number of places left in tutorial");

	//make the split pane
	splitPane.setDividerLocation(560);
	splitPane.setOneTouchExpandable(false);
	leftCenterPane.setMinimumSize(new Dimension(555,410));
	leftCenterPane.setBorder(BorderFactory.createTitledBorder(etched, "Select a tutorial to view:"));
	rightCenterPane.setMinimumSize(new Dimension(235,410));
	splitPane.setPreferredSize(new Dimension(830, 410));

	// set font for the title of this screen
	title2.setFont(new Font("Times-Roman", Font.BOLD + Font.ITALIC, 16));
	status1.setFont(new Font("Times-Roman", Font.BOLD + Font.ITALIC, 14));

	mainPanel.setLayout(new BorderLayout());// the layout manager for the center panel

	//create the cancel button and its listeners andits characteristics
	mainCancelButton.setActionCommand("cancel");
	mainCancelButton.addActionListener(this);
	mainCancelButton.setToolTipText("Quit to menu");
	mainCancelButton.setMnemonic(KeyEvent.VK_C);
	
	//create the back button and its listeners andits characteristics
	mainBackButton = new JButton("< Back");
	mainBackButton.setToolTipText("Back to the last screen");
	mainBackButton.setMnemonic(KeyEvent.VK_B);
	mainBackButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
			setVisible(false);
			setContentPane(introPanel);
			setSize(440, 340);
			setLocation(280, 120);
			setVisible(true);
		 }
	});

	//create the next button and its listeners andits characteristics
	mainNextButton = new JButton("Next >");
	mainNextButton.setToolTipText("To the next screen");
	mainNextButton.setMnemonic(KeyEvent.VK_N);
	if (isEnrolled()==false) {
		mainNextButton.setEnabled(false);
	}
	mainNextButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
			setVisible(false);
			setContentPane(confirmPanel);
			setSize(480, 280);
			setLocation(250,160);
			setVisible(true);
	 	}
	});

	//when the screen opens, if the user is enrolled, set up correct details, just like in the real
	//tute enrol program
	if (isEnrolled()==true) {
		enrolButton.setEnabled(false);//disable enrol button
		unenrolButton.setEnabled(true);//enable unenrol button
		status1.setForeground(Color.green);
		int flag = 0;
		days1 = enrolment.getDayList();
		//find where the user is enrolled...
	  	for (int i = 0; i < days.size(); i++){
 		 	day = days.item(i);
			day1 = days.item(i);
    		times = day.getTimeList();
			times1 = day1.getTimeList();

  			for (int j = 0; j < times.size(); j++){
  				time = times.item(j);
				time1 = times1.item(j);
    			enrolledList = time.getEnrolledList();
				enrolledList1 = time1.getEnrolledList();
    			for (int k = 0; k < enrolledList.size(); k++){
      				if (user.equalsIgnoreCase(enrolledList.item(k))) {
					  	//....then set up correct details of the users tute
						mainDayArea.setText("  " + day.getName());
						mainStartArea.setText("  " + convertTime(time.getStartTime()));
						mainDurationArea.setText("  " + time.getDuration()+ "hr/s");
						mainCapacityArea.setText("  " + time.getEnrolmentLimit()+ " ");
						int avail = time.getEnrolmentLimit() - enrolledList.size(); 
						mainAvailArea.setText("  " + avail);
						textArea.setText("");
      					for(int l = 0; l<enrolledList.size(); l++) {
           					String enrolled = enrolledList.item(l);
            				textArea.append("  " + enrolled + "\n");
          				}
						confirmLoginArea.setText("  " + user);
						confirmDayArea.setText("  " + day.getName());
						confirmStartArea.setText("  " + convertTime(time.getStartTime()));
						confirmDurationArea.setText("  " + time.getDuration()+ "hr/s");
						flag = 1;
						break;
      				}
				if (flag ==1) {break;}
    			}
			if (flag ==1) {break;}
  			}
		if (flag ==1) {break;}
 		}
	}

	else {
		unenrolButton.setEnabled(false);
		enrolButton.setEnabled(false);
		status1.setForeground(Color.red);
	}

	//set up enrol  and unenrol buttons with listeners
	enrolButton.setActionCommand("enrol");
	enrolButton.addActionListener(this);
	enrolButton.setToolTipText("Enrol in the selected time slot");
	enrolButton.setMnemonic(KeyEvent.VK_E);
	unenrolButton.setActionCommand("unenrol");
	unenrolButton.addActionListener(this);
	unenrolButton.setToolTipText("Unenrol from current tute");
	unenrolButton.setMnemonic(KeyEvent.VK_U);
		
	// set up the clash button
	clashButton.setActionCommand("clash");
	clashButton.addActionListener(this);
	clashButton.setToolTipText("Request a clash resolution form");
	clashButton.setMnemonic(KeyEvent.VK_L);

	//append the title to the main panel
	titlePanel2.add(title2, BorderLayout.CENTER);
	mainNorthPanel.add(titlePanel2);
	mainPanel.add(mainNorthPanel, BorderLayout.NORTH);

	// create the gridlayout for the left pane of the center
	leftCenterPane.setLayout( new GridLayout(0,6,2,2));
	gridPane1.setLayout(new GridLayout(10,0,2,2));//grid for the times
	gridPane2.setLayout(new GridLayout(10,0,2,2));//grid for each day of the week
	gridPane3.setLayout(new GridLayout(10,0,2,2));
	gridPane4.setLayout(new GridLayout(10,0,2,2));
	gridPane5.setLayout(new GridLayout(10,0,2,2));
	gridPane6.setLayout(new GridLayout(10,0,2,2));

	// list the times down the left hand side of the grid
	int ptime=9;
	gridPane1.add(new JPanel());
	for (int i=0; i<9; i++) {
		JLabel timeLabel = new JLabel(convertTime(ptime + ":00"), SwingConstants.CENTER);
		timeLabel.setBackground(Color.lightGray);
		gridPane1.add(timeLabel);
		ptime++;
	}

	//then the days of the week for the 1st of each day grid
	gridPane2.add(new JLabel(monday), SwingConstants.CENTER);
	gridPane3.add(new JLabel(tuesday), SwingConstants.CENTER);
	gridPane4.add(new JLabel(wednesday), SwingConstants.CENTER);
	gridPane5.add(new JLabel(thursday), SwingConstants.CENTER);
	gridPane6.add(new JLabel(friday), SwingConstants.CENTER);

	// create the available tutorials
	setGrid(monday, gridPane2);
	setGrid(tuesday, gridPane3);
	setGrid(wednesday, gridPane4);
	setGrid(thursday, gridPane5);
	setGrid(friday, gridPane6);

	// add the grid to the left pane
	leftCenterPane.add(gridPane1);
	leftCenterPane.add(gridPane2);	
	leftCenterPane.add(gridPane3);
	leftCenterPane.add(gridPane4);
	leftCenterPane.add(gridPane5);
	leftCenterPane.add(gridPane6);

	//set the layout for the right pane
	GridBagLayout gbl2  = new GridBagLayout();
	GridBagConstraints gbc2 = new GridBagConstraints();
	rightCenterPane.setLayout(gbl2);

	//set the title for the right Pane
	statusLabel.setFont(new Font("Times-Roman", Font.BOLD + Font.ITALIC, 14));

	//add each component to the rightPane
	gbc2.anchor = GridBagConstraints.NORTHWEST;
	gbc2.gridwidth = GridBagConstraints.REMAINDER;
	rightCenterPane.add(statusLabel, gbc2);
	if (isEnrolled()==true) {
		status1.setText("ENROLLED");
		status1.setForeground(Color.green);
		rightCenterPane.add(status1, gbc2);
	}
	else {
		status1.setText("UNENROLLED");
		status1.setForeground(Color.red);
		rightCenterPane.add(status1,gbc2);
	}
	gbc2.anchor = GridBagConstraints.NORTH;
	gbc2.fill	 = GridBagConstraints.HORIZONTAL;
	gbc2.insets = new Insets(0, 0 , 10, 0);
	rightCenterPane.add(sepTopRite, gbc2);

	gbc2.anchor = GridBagConstraints.WEST;
	gbc2.gridwidth = 1;
	gbc2.insets = new Insets(5, 0 ,5, 0);
	rightCenterPane.add(mainDayLabel, gbc2);

	rightCenterPane.add(Box.createHorizontalStrut(10));
	gbc2.gridwidth = GridBagConstraints.REMAINDER;
	rightCenterPane.add(mainDayArea, gbc2);

	gbc2.gridwidth = 1;
	rightCenterPane.add(mainStartLabel, gbc2);

	rightCenterPane.add(Box.createHorizontalStrut(10));
	gbc2.gridwidth = GridBagConstraints.REMAINDER;
	rightCenterPane.add(mainStartArea, gbc2);

	gbc2.gridwidth = 1;
	rightCenterPane.add(mainLocationLabel, gbc2);

	rightCenterPane.add(Box.createHorizontalStrut(10));
	gbc2.gridwidth = GridBagConstraints.REMAINDER;
	rightCenterPane.add(mainLocationArea, gbc2);
	
	gbc2.gridwidth = 1;
	rightCenterPane.add(mainDurationLabel, gbc2);

	rightCenterPane.add(Box.createHorizontalStrut(10));
	gbc2.gridwidth = GridBagConstraints.REMAINDER;
	rightCenterPane.add(mainDurationArea, gbc2);
		
	gbc2.gridwidth = 1;
	rightCenterPane.add(mainCapacityLabel, gbc2);

	rightCenterPane.add(Box.createHorizontalStrut(10));
	gbc2.gridwidth = GridBagConstraints.REMAINDER;
	rightCenterPane.add(mainCapacityArea, gbc2);

	gbc2.gridwidth = 1;
	rightCenterPane.add(mainAvailLabel, gbc2);

	rightCenterPane.add(Box.createHorizontalStrut(10));
	gbc2.gridwidth = GridBagConstraints.REMAINDER;
	rightCenterPane.add(mainAvailArea, gbc2);

	gbc2.gridwidth = 1;
	rightCenterPane.add(mainEnrolledLabel, gbc2);

	// set up the text area or enrolled students logins
	textArea.setEditable(false);
	textArea.setToolTipText("Students currently enrolled in tutorial");
	scrollPane.setPreferredSize(new Dimension(50, 60));

	rightCenterPane.add(Box.createHorizontalStrut(10));
	gbc2.gridwidth = GridBagConstraints.REMAINDER;
	rightCenterPane.add(scrollPane, gbc2);

	gbc2.gridwidth = 1;
	gbc2.insets = new Insets(15,0 ,0, 0);

	//set the button panel for the enrol/unenrol/clash buttons
	JPanel buttons2 = new JPanel();
	buttons2.setLayout(new GridLayout(3,0,0,4));
	buttons2.add(enrolButton);
	buttons2.add(unenrolButton);
	buttons2.add(clashButton);

	JPanel buttonPanel4 = new JPanel();

	buttonPanel4.setLayout( new BorderLayout());
	buttonPanel4.add(mainSepBottom, "North");
	buttonPanel4.add(buttons2, "Center");
	rightCenterPane.add(buttonPanel4, gbc2);

	gbc2.anchor = GridBagConstraints.SOUTH;
	gbc2.insets = new Insets(15,0 ,0, 0);
	gbc2.fill	 = GridBagConstraints.HORIZONTAL;
	gbc2.gridwidth = 7;
	rightCenterPane.add(buttonPanel4, gbc2);

	//append the right pane to the center panel then the
	// center panel to the main panel
	mainCenterPanel.add(splitPane, BorderLayout.CENTER);
	mainPanel.add(mainCenterPanel, BorderLayout.CENTER);

	//set up the buttons for the bottom of the screen
	JPanel buttons = new JPanel();
	buttons.setLayout(new GridLayout(0, 10));
	buttons.add(mainQuitButton);
	buttons.add(filler1);
	buttons.add(filler2);
	buttons.add(filler3);
	buttons.add(filler4);
	buttons.add(filler5);
	buttons.add(filler6);
	buttons.add(mainBackButton);
	buttons.add(mainCancelButton);
	buttons.add(mainNextButton);

	JPanel buttonPanel2 = new JPanel();

	buttonPanel2.setLayout( new BorderLayout());
	buttonPanel2.add(sepBottomRite, "North");
	buttonPanel2.add(buttons, "Center");

	//append the south panel to the main panel
	mainSouthPanel.add(buttonPanel2);
	mainPanel.add(mainSouthPanel, BorderLayout.SOUTH);

  }

  /** create the confirmation screen
	*
	*
   */
  public void confirmPane() {
	//create new components needed for the panel
	GridBagLayout gbl3  = new GridBagLayout();
	GridBagConstraints gbc3 = new GridBagConstraints();
	confirmPanel.setLayout(gbl3);
	JLabel title3 =new JLabel("CSSE Tute Enrol Semester 2, 2000: CONFIRMATION");
	confirmLocationArea.setBackground(Color.lightGray);

	// create the back button and its listeners and characteristics
	confirmBackButton = new JButton("< Back");
	confirmBackButton.setMnemonic(KeyEvent.VK_B);
	confirmBackButton.setToolTipText("Back to the last screen");
	confirmBackButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
			setVisible(false);
			setContentPane(mainPanel);
			setSize(900, 540);
			setLocation(35,30);
			setVisible(true);
		 }
	});
	// create the quit button and its listeners and characteristics
	quitButton(confirmQuitButton);
	confirmQuitButton.setToolTipText("Quit out of Tute Enrol");
	confirmQuitButton.setMnemonic(KeyEvent.VK_Q);

	// create the save and quit button and its listeners and characteristics
	saveButton.setActionCommand("save");
	saveButton.setToolTipText("Save enrolment and quit");
	saveButton.addActionListener(this);
	saveButton.setMnemonic(KeyEvent.VK_S);

	// create the cancel button and its listeners and characteristics
	confirmCancelButton.setActionCommand("cancel");
	confirmCancelButton.addActionListener(this);
	confirmCancelButton.setToolTipText("Quit to menu");
	confirmCancelButton.setMnemonic(KeyEvent.VK_C);

	//set up the text areas for this pane
	confirmLoginArea.setEditable(false);
	confirmLoginArea.setToolTipText("Your login name");
	confirmStartArea.setEditable(false);
	confirmStartArea.setToolTipText("Selected start time of tutorial");
	confirmDayArea.setEditable(false);
	confirmDayArea.setToolTipText("Selected day of tutorial");
	confirmLocationArea.setEditable(false);
	confirmLocationArea.setToolTipText("Selected Location of tutorial");

	//add the components to the panel using gridbaglayout
	title3.setFont(new Font("Times-Roman", Font.BOLD + Font.ITALIC, 16));
	gbc3.anchor = GridBagConstraints.NORTHWEST;
	gbc3.gridwidth = GridBagConstraints.REMAINDER;
	confirmPanel.add(title3, gbc3);

	gbc3.anchor = GridBagConstraints.NORTH;
	gbc3.fill	 = GridBagConstraints.HORIZONTAL;
	gbc3.insets = new Insets(0, 0 , 10, 0);
	confirmPanel.add(confirmSepTop, gbc3);

	gbc3.anchor = GridBagConstraints.WEST;
	gbc3.gridwidth = 1;
	gbc3.insets = new Insets(5, 0 ,5, 0);
	confirmPanel.add(confirmLoginLabel, gbc3);

	confirmPanel.add(Box.createHorizontalStrut(10));
	gbc3.gridwidth = GridBagConstraints.REMAINDER;
	confirmPanel.add(confirmLoginArea, gbc3);

	gbc3.gridwidth = 1;
	confirmPanel.add(confirmDayLabel, gbc3);

	confirmPanel.add(Box.createHorizontalStrut(10));
	gbc3.gridwidth = GridBagConstraints.REMAINDER;
	confirmPanel.add(confirmDayArea, gbc3);

	gbc3.gridwidth = 1;
	confirmPanel.add(confirmStartLabel, gbc3);

	confirmPanel.add(Box.createHorizontalStrut(10));
	gbc3.gridwidth = GridBagConstraints.REMAINDER;
	confirmPanel.add(confirmStartArea, gbc3);

	gbc3.gridwidth = 1;
	confirmPanel.add(confirmDurationLabel, gbc3);

	confirmPanel.add(Box.createHorizontalStrut(10));
	gbc3.gridwidth = GridBagConstraints.REMAINDER;
	confirmPanel.add(confirmDurationArea, gbc3);

	gbc3.gridwidth = 1;
	confirmPanel.add(confirmLocationLabel, gbc3);

	confirmPanel.add(Box.createHorizontalStrut(10));
	gbc3.gridwidth = GridBagConstraints.REMAINDER;
	confirmPanel.add(confirmLocationArea, gbc3);

	// this creates the buttons on the right of the pane
	JPanel cRightPanel = new JPanel();
	cRightPanel.setLayout(new FlowLayout());
	cRightPanel.add(confirmBackButton);
	cRightPanel.add(confirmCancelButton);
	cRightPanel.add(saveButton);

	// this creates the buttons on the left of the pane
	JPanel cLeftPanel = new JPanel();
	cLeftPanel.setLayout(new FlowLayout());
	cLeftPanel.add(confirmQuitButton);

	//then adds them to the main pane
	JPanel buttons3 = new JPanel();
	buttons3.setLayout(new BorderLayout());
	buttons3.add(cLeftPanel, BorderLayout.WEST);
	buttons3.add(cRightPanel, BorderLayout.EAST);

	JPanel buttonPanel3 = new JPanel();
	buttonPanel3.setLayout( new BorderLayout());
	buttonPanel3.add(confirmSepBottom, "North");
	buttonPanel3.add(buttons3, "Center");
	confirmPanel.add(buttonPanel3, gbc3);

	gbc3.anchor = GridBagConstraints.SOUTH;
	gbc3.insets = new Insets(15, 0, 0, 0);
	gbc3.fill	 = GridBagConstraints.HORIZONTAL;
	gbc3.gridwidth = 7;
	confirmPanel.add(buttonPanel3, gbc3);

  }
/** This is the is the actionlister for the 
 *  actions performed by this programs buttons etc. 
 *
 * @param e e is teh ActionEvent required
*/

  public void actionPerformed(ActionEvent e){
	//if the enrol button is hit..
	if (e.getActionCommand().equals("enrol")) {
	  	// set up areas as is required
		time1.addEnrolled(user);//add user to the timeslot
		enrolButton.setEnabled(false);
		mainNextButton.setEnabled(true);
		unenrolButton.setEnabled(true);
		status1.setText("ENROLLED(Unconfirmed)");
		status1.setForeground(Color.red);
		confirmLoginArea.setText("  " + user);
		confirmDayArea.setText("  " + day1.getName());
		confirmStartArea.setText("  " + convertTime(time1.getStartTime()));
		confirmDurationArea.setText("  " + time1.getDuration()+ "hr/s");
		//find the tute (in case user opens window and is enrolled and then 
		//unenrols and wants 2 enrol again
		for (int i = 0; i < days1.size(); i++){
  			day2 = days1.item(i);
    		times2 = day1.getTimeList();

  			for (int j = 0; j < times1.size(); j++){
  				time2 = times1.item(j);
    			enrolledList2 = time2.getEnrolledList();
				//decrement the availability
				int avail2 = time2.getEnrolmentLimit() - enrolledList2.size();
				mainAvailArea.setText("  " +avail2);
				textArea.setText("");
				// show the enrolled students
    			for (int k = 0; k < enrolledList2.size(); k++){
					String enrolled = enrolledList2.item(k);
					textArea.append("  " + enrolled + "\n");
      			}
    		}
  		}
	}

	//if the unenrol button is clicked
	else if (e.getActionCommand().equals("unenrol")) {
	  	//do the required setting of components
		unenrolButton.setEnabled(false);
		mainNextButton.setEnabled(false);
		enrolButton.setEnabled(true);
		status1.setText("UNENROLLED");
		status1.setForeground(Color.red);
		int flag = 0;
		for (int i = 0; i < days.size(); i++){
  			day = days.item(i);
    		times = day.getTimeList();

  			for (int j = 0; j < times.size(); j++){
  				time = times.item(j);
    			enrolledList = time.getEnrolledList();
    			for (int k = 0; k < enrolledList.size(); k++){
					if (user.equalsIgnoreCase(enrolledList.item(k))) {
						time.removeEnrolled(user);
						enrolment.save();
						flag = 1;
						break;
					}
				}
				if (flag ==1) {break;}
			}
			if (flag ==1) {break;}
		}
		mainAvailArea.setText("  " + (time.getEnrolmentLimit() - enrolledList.size()+1) );
		textArea.setText("");
		for (int k = 0; k < enrolledList.size(); k++){
			if (user.equalsIgnoreCase(enrolledList.item(k))); 
			else {
				String enrolled = enrolledList.item(k);
        		textArea.append("  " + enrolled + "\n");
			}
		}
	}

	//if the clash button is clicked
	else if (e.getActionCommand().equals("clash")) {
	  	// create a diolog box
		String dialogTitle = "Send Clash Resolution Form?";
		String fill[] = {"By clicking yes a clash resolution form for ",
						enrolment.getSubjectName(), " ",
						"will be sent to ",user+"@students.cs.mu.oz.au", " ", }; 
		int result = JOptionPane.showConfirmDialog(clashButton,fill, dialogTitle,
								 JOptionPane.YES_NO_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
		switch(result) {
			case JOptionPane.YES_OPTION:
				break;
			case JOptionPane.NO_OPTION:
				break;
			case JOptionPane.CANCEL_OPTION:
				break;
		}
	}

	// if the save button is hit then save and exit
	else if (e.getActionCommand().equals("save")) {
		enrolment.save();
		System.exit(0);
	}

	//if a combo box is hit ( i dont know how 2 handle events any other way)
	else if (e.getActionCommand().substring(0,4).equals("Tute")) {
		int j = Integer.parseInt(e.getActionCommand().substring(4,5));//get the day
		int k = Integer.parseInt(e.getActionCommand().substring(5,6));//get the tute number of that day
		days1 = enrolment.getDayList();
		day1 = days1.item(j);
		times1 =day1.getTimeList();
		time1 = times1.item(k);
		enrolledList1 = time1.getEnrolledList();
		//set the buttons accordingly
		if (isEnrolled()==true) {
			enrolButton.setEnabled(false);
			unenrolButton.setEnabled(true);
		}
		else {
			enrolButton.setEnabled(true);
			unenrolButton.setEnabled(false);
		}
		//if the class is full then display there are no more places
		if (time1.getEnrolmentLimit() - enrolledList1.size() == 0) {
			enrolButton.setEnabled(false);
			unenrolButton.setEnabled(false);
			mainAvailArea.setForeground(Color.red);
			mainAvailArea.setText("TUTORIAL FULL!");
		}
		else {
			mainAvailArea.setForeground(Color.black);
		}
		//then set the text areas for the tute selected
		mainDayArea.setText("  " +day1.getName());
		mainStartArea.setText("  " + convertTime(time1.getStartTime()));
		mainDurationArea.setText("  " +time1.getDuration()+ "hr/s");
		mainCapacityArea.setText("  " +time1.getEnrolmentLimit());
		int avail = time1.getEnrolmentLimit() - enrolledList1.size();
		if (avail != 0) {
			mainAvailArea.setText("  " +avail);
		}
		textArea.setText("");
      	for(int l = 0; l<enrolledList1.size(); l++) {
            String enrolled = enrolledList1.item(l);
            textArea.append("  " + enrolled + "\n");
          }
		}

	//if the cancel button is selected close the Internal frame
	else if (e.getActionCommand().equals("cancel")) {
		dispose();
	}

  };

  /** creates a new Button panel for the selection screen
	*
	*
   */
  class ButtonPanel1 extends JPanel {
	JPanel buttonPanel1 = new JPanel();
	JSeparator sep1 = new JSeparator();

	public ButtonPanel1() {
		buttonPanel1.setLayout( new BorderLayout());
		setLayout(new BorderLayout(0,5));
		add(sep1, "North");
		add(buttonPanel1, "Center");
	}
	public void add(JButton button) {
		buttonPanel1.add(button);
	}
  }

  /** creates a title for the main tute enrol screen
	*
   */

  class TitlePanel2 extends JPanel {
	JPanel titlePanel2 = new JPanel();
	JSeparator sep2 = new JSeparator();

	public TitlePanel2() {
		titlePanel2.setLayout( new BorderLayout());
		setLayout(new BorderLayout());
		add(sep2, "South");
		add(titlePanel2, "Center");
	}
	public void add(JLabel label) {
		titlePanel2.add(label);
	}
  }

  /** creates a quit button with its own action listener
	*
	*
   */

  public void quitButton(final JButton quitbutton) {
  	final String dialogTitle = "You are currently UNENROLLED";
	final String noSaveMsg[] = {"You have not enrolled in " , enrolment.getSubjectName(),
								" ", "Are you sure you want to quit?", " ", };

	quitbutton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent d) {
			if (isEnrolled()==false) {
			  int result = JOptionPane.showConfirmDialog( quitbutton, noSaveMsg, dialogTitle,
						   JOptionPane.YES_NO_CANCEL_OPTION, JOptionPane.WARNING_MESSAGE);
				switch(result) {
					case JOptionPane.YES_OPTION:
						System.exit(0);
						break;
					case JOptionPane.NO_OPTION:
						break;
					case JOptionPane.CANCEL_OPTION:
						break;
				}
			}
			else
				System.exit(0);
		}
	});
  }

  /** sets the grid for one day
	*
	* @param aDay a String representing the day 
	* @param gridPane the grid representing the day itself
   */
  public void setGrid(String aDay,JPanel gridPane ) {
	final String finaltime = "18:00"; //last possible timeslot
	int flag = 0;//set if there is a tute on that day
	int intTime = 9;// start time int
	String currtime = intTime+ ":00";//the actual time

	//if there is a day in the list given that matches the given param day
	for (int i=0; i<days.size(); i++) {
		day = days.item(i);
		times = day.getTimeList();
		if (aDay.equals(day.getName())) {
			flag = 1;//there is a tute on this day
			break;
		}
	}
	
	if (flag ==1) {
		tuteNum =0;//total num of tues on this day
		int k = 1; //for multiple tutes at 1 time 
		for (int j = 0; j<times.size();j++) {
		int flagger = 0;// for multiple tutes on 1 day
		time = times.item(j);
		if ( k >= times.size()-1) 
			flagger = 1;
		else
			time2 = times.item(k);

		//skip over leading time slots until a time slot is reached and fill in 
		//with JPanels
		while (!currtime.equals(time.getStartTime())) {
			JPanel panel = new JPanel();
			panel.setBackground(Color.lightGray);
			gridPane.add(panel);
			intTime++;
			currtime = intTime + ":00";
		}
		//create a new combo box
		JComboBox comboBox = new JComboBox();
		comboBox.setToolTipText("Click to select a tute..");
		comboBox.addItem("<Show>");//to make user click on box
		comboBox.addItem("Tute"+ totTute);//the tutre and number of tute
		comboBox.setActionCommand("Tute"+ dayNum + ""+ tuteNum);//set the action command
		comboBox.addActionListener(this);//and the listener
		tuteNum++;
		totTute++;
		// if there is more than 1 tute on that time
		if (flagger == 0) {
		  //add all the tutes that are at that time
			while (time.getStartTime().equals(time2.getStartTime())) {
				comboBox.addItem("Tute"+ totTute);
				comboBox.setActionCommand("Tute"+ dayNum + ""+ tuteNum);
				comboBox.addActionListener(this);
				tuteNum++;
				totTute++;
				j++;
				k++;
				time = times.item(j);
				time2 = times.item(k);
			}
		}
		//then add teh combo box to the grid
		gridPane.add(comboBox);
		intTime++;
		currtime = intTime + ":00";
		k++;
	}
	dayNum++;
			
	//skip over any timeslots that are left until the final slot
	while (!currtime.equals(finaltime)) {
		JPanel panel = new JPanel();
		panel.setBackground(Color.lightGray);
		gridPane.add(panel);
		intTime++;
		currtime = intTime + ":00";
	}
	}
	//else there is no tutes for this day, so fill the grid with Jpanels
	else {
		for (int j=0; j<9; j++) {
			JPanel panel = new JPanel();
			panel.setBackground(Color.lightGray);
			gridPane.add(panel);
		}
	}
  }
  /** checks to see if the user is enrolled
	*
	* @return true if the user is enrolled
	* @return false if the user isnt enrolled
   */

  private boolean isEnrolled() {
	days = enrolment.getDayList();
	boolean enrolled = false;

	for (int i = 0; i < days.size(); i++){
		day = days.item(i);
    	times = day.getTimeList();

  		for (int j = 0; j < times.size(); j++){
  			time = times.item(j);
    		enrolledList = time.getEnrolledList();
    		for (int k = 0; k < enrolledList.size(); k++){
      			if (user.equalsIgnoreCase(enrolledList.item(k))) {
        		enrolled = true;
      			}
    		}
  		}
 	}
	return enrolled;
 }

  /** converts the time in the xml file to an 12 hour clock with am/pm
	*
	* @param time the time given
   */
   private String convertTime(String time) {
      String returnTime;

      if (time.equals("9:00") || time.equals("10:00") || time.equals("11:00"))
          return time+"am";
      else if (time.equals("12:00"))
          return time+"pm";
      else if (time.equals("13:00"))
          return "1:00pm";
      else if (time.equals("14:00"))
          return "2:00pm";
      else if (time.equals("15:00"))
          return "3:00pm";
      else if (time.equals("16:00"))
          return "4:00pm";
      else 
          return "5:00pm";
  }


}

