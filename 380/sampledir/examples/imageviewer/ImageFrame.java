/*
 *                 Sun Public License Notice
 * 
 * The contents of this file are subject to the Sun Public License
 * Version 1.0 (the "License"). You may not use this file except in
 * compliance with the License. A copy of the License is available at
 * http://www.sun.com/
 * 
 * The Original Code is NetBeans. The Initial Developer of the Original
 * Code is Sun Microsystems, Inc. Portions Copyright 1997-2000 Sun
 * Microsystems, Inc. All Rights Reserved.
 */

package examples.imageviewer;

/**
 *
 * @author
 * @version 
 */
public class ImageFrame extends javax.swing.JInternalFrame {

    /** Creates new form ImageFrame */
    public ImageFrame(String imageName ) {
        initComponents ();
        setTitle (imageName);
        imageLabel.setIcon (new javax.swing.ImageIcon (imageName) );
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the FormEditor.
     */
    private void initComponents () {//GEN-BEGIN:initComponents
        jScrollPane1 = new javax.swing.JScrollPane ();
        imageLabel = new javax.swing.JLabel ();
        getContentPane ().setLayout (new java.awt.BorderLayout ());
        setClosable (true);
        setResizable (true);
        setIconifiable (true);



        jScrollPane1.setViewportView (imageLabel);


        getContentPane ().add (jScrollPane1, java.awt.BorderLayout.CENTER);

    }//GEN-END:initComponents


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JLabel imageLabel;
    // End of variables declaration//GEN-END:variables

}