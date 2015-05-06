package enroller;

import utils.*;
import javax.swing.JInternalFrame;
import javax.swing.JDesktopPane;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JMenuBar;
import javax.swing.JFrame;
import javax.swing.JFileChooser;

import java.awt.event.*;
import java.awt.*;

public class Enroller extends JFrame {
  JDesktopPane desktop;
  String fileName;
  String userName;

  static final Integer FRAMELAYER = new Integer(6);
  static final Integer HELPLAYER = new Integer(7);
  static final String DATAFILEPATH = new String("DataFiles");
  static final String HTMLHELPPATH = new String("HelpFiles/toc.html");
	static final String HTMLJUSTPATH = new String("HelpFiles/justification.html");

  public Enroller(String userName) {
    super("Enroller");
    this.userName = userName;

    //Make the big window be indented 50 pixels from each edge 
    //of the screen.
    int inset = 50;
    Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
    setBounds(inset, inset, 
          980, //screenSize.width - inset*2, 
          680);//screenSize.height-inset*2);

    //Quit this app when the big window closes.
    addWindowListener(new WindowAdapter() {
      public void windowClosing(WindowEvent e) {
        System.exit(0);
      }
    });

    //Set up the GUI.
    desktop = new JDesktopPane(); //a specialized layered pane
    setContentPane(desktop);
    setJMenuBar(createMenuBar());

    //Make dragging faster:
    desktop.putClientProperty("JDesktopPane.dragMode", "outline");
  }

  protected JMenuBar createMenuBar() {
    JMenuBar menuBar = new JMenuBar();

    JMenu fileMenu = buildFileMenu();
    JMenu helpMenu = buildHelpMenu();
    menuBar.add(fileMenu);
    menuBar.add(helpMenu);

    return menuBar;
  }

  protected JMenu buildFileMenu() {
    JMenu menu = new JMenu("File");
    menu.setMnemonic(KeyEvent.VK_F);
    JMenuItem openMenuItem = new JMenuItem("Open");
    JMenuItem exitMenuItem = new JMenuItem("Exit");
    openMenuItem.setMnemonic(KeyEvent.VK_O);
    exitMenuItem.setMnemonic(KeyEvent.VK_X);
    openMenuItem.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        openFile();
      }
    });
    exitMenuItem.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        quit();
      }
    });
    menu.add(openMenuItem);
    menu.add(exitMenuItem);
    return menu;
  }

  protected JMenu buildHelpMenu() {
    JMenu menu = new JMenu("Help");
    menu.setMnemonic(KeyEvent.VK_H);
    JMenuItem helpMenuItem = new JMenuItem("Open Help Window");
		JMenuItem justificationMenuItem = new JMenuItem("Justification");
		helpMenuItem.setMnemonic(KeyEvent.VK_H);
		justificationMenuItem.setMnemonic(KeyEvent.VK_J);
    helpMenuItem.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        openHelpWindow();
      }
    });
		justificationMenuItem.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        openJustificationWindow();
      }
    });
    menu.add(helpMenuItem);
		menu.add(justificationMenuItem);
    return menu;
  }

  protected void openHelpWindow() {
    JInternalFrame help = new HelpFrame(HTMLHELPPATH);
    desktop.add(help, HELPLAYER);
    try {
      help.setVisible(true);
      help.setSelected(true);
    } catch (java.beans.PropertyVetoException e) {}
  }

  protected void openJustificationWindow() {
    JInternalFrame justification = new HelpFrame(HTMLJUSTPATH);
    desktop.add(justification, HELPLAYER);
    try {
      justification.setVisible(true);
      justification.setSelected(true);
    } catch (java.beans.PropertyVetoException e) {}
  }


  protected void createFrame() {
    EnrolmentFrame frame = new EnrolmentFrame(fileName, userName);
    frame.setVisible(true); //necessary as of kestrel
    desktop.add(frame, FRAMELAYER);
    try {
      frame.setSelected(true);
    } catch (java.beans.PropertyVetoException e) {}
  }

  public static void main(String[] args) {
    if(args.length != 1) {
      System.out.println("Syntax: java enroller/Enroller USERNAME\n"
        + "  Under Windows: make sure your USERNAME environment variable is set"
        + "  Under 'n'x: make sure your USER unvironment variable is set\n");
      return;
    }
    Enroller frame = new Enroller(args[0]);
    frame.setVisible(true);
  }

  protected void openFile() {
    JFileChooser chooser = new JFileChooser(DATAFILEPATH);
    ExampleFileFilter filter = new ExampleFileFilter();
    filter.addExtension("xml");
    chooser.setFileFilter(filter);
    int returnVal = chooser.showOpenDialog(this);
    if(returnVal == JFileChooser.APPROVE_OPTION) {
      fileName = DATAFILEPATH + java.io.File.separator + chooser.getSelectedFile().getName();
    }
    createFrame();
  }

  protected void quit() {
    System.exit(0);
  }
}


