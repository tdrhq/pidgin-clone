/*
Copyright (c) <2005>, <Mel Dooki>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
* Neither the name of the <ORGANIZATION> nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

import java.io.*;
import javax.bluetooth.*;
import java.util.*;
import javax.swing.*;
import java.awt.*;
import java.awt.image.*;
import java.awt.event.*;
import java.awt.image.ImageObserver;
import java.awt.image.BufferedImage;
import java.net.URL;

public class PCapplication extends JFrame implements ImageObserver
{
	int countRecipients; //the number of recipients entered by user to send message to
	String desiredNumbers [];  //the desired contacts to send message to
    String msg;  //the message entered by user
	
	int messageCharacters;  //the number of characters entered by the user in the message area
	int messageNumber;      //the number of messages that will be sent according to the number of characters
	
	int searchDevicesThreadCounter = 0;  //controls the SearchForDevices Button
	int addToIndex = 0;    //controls the action of displaying contacts beginning with a certain character.
	
	public static final String UUIDString = "11223344556677889900aabbccddeeff"; //Both Client and Server retrives this UUID string from this MIDlet class.
	
	//Containers
	
	BufferedImage bi;   //background image
	
	JPanel p1,   //p1 provides interface for searching for nearby bluetooth devices and getting the selected device of the user   
		   p2, 	 //p2 provides the interface for importing the phonebook from the default database of the cellular phone and getting the mesage and the desired numbers from the user 	
		   p3; 	 //p3 provides a report about the successfulness of routing the message to the phne and sendin it 	

	private javax.swing.JFrame SMSFrame; //frame containing p2
	private javax.swing.JFrame ReportFrame; //frame containing p3

	//Home page Frame components: (those in p1)
	
	private javax.swing.JButton SearchForDevicesButton;
	private javax.swing.JButton QuitButton;
	private javax.swing.JTextPane DisplayText1;  //text for displaying messages to the user	

	private javax.swing.JButton SelectDevice;
	private javax.swing.JList DevicesList;
	
	Vector theDevices; //vector which holds the BT devices discovered	
	
	//SMS Frame components (those in p2)
	
	private javax.swing.JButton BackButton2;    //allows the use to return back to the home page 
	private javax.swing.JTextField RecipientNo; //text for the user to enter desired number (s), separated by commas
	private javax.swing.JLabel RecipientNumLabel;
	private javax.swing.JTextPane DisplayText2; //text to display messages to the user
	
	private javax.swing.JButton ClearMsgButton; //clears the message entered by the user
	private javax.swing.JButton SendButton;    //routs the message to cell phone to be sent
	private javax.swing.JTextArea Message;     //area for message entered by the user
	private javax.swing.JLabel MessageLabel;   
	private javax.swing.JLabel MessageInfoLabel;   
	
	private javax.swing.JList PhoneBook;      //list which holds the contacts imported from the phone
	private javax.swing.JButton RetrieveContacts; //triggers the action of displaying the address book
	private javax.swing.JButton SelectContactNumber; 
	private javax.swing.JTextField StartsWith;  //displays all te contacts that starts with the entered string

  	//Report page
  	private javax.swing.JButton SendNewMessageButton; //returns to SMS frame to allow user to send new message hence allowing batch sending
  	private javax.swing.JButton QuitButton3;     //returns to home page
  	private javax.swing.JTextPane DisplayText3; //displays messages to the user
  	
  	public static PCapplication instance;  // used singleton object
  	
  	private Client client;   //client instance
  	private BTworker blue ;  //BTworker instance which does device and service discovery actions

  	public PCapplication() 
  	{
    	initComponents();  //initializes components
  	}
  	
  	private void initComponents() 
  	{
  		
  		try
		{
			//this file is for the background & should be placed with the directory containing class files
			java.net.URL u = PCapplication.class.getResource ("babyblue.jpg");
			bi = javax.imageio.ImageIO.read (u);
		}  
		catch (Exception e)
		{ 
		}
		
		p1 = new Background();
  		p1.setOpaque (false);  //so that the component does not paint some or all of its pixels, allowing the underlying pixels to show through
  		
  		p2 = new Background();
  		p2.setOpaque (false);
  		
  		p3 = new Background ();
		p3.setOpaque (false);  		
		
		p1.setSize(510,510);
		p2.setSize(510,510);
		p3.setSize(510,510);
        
        p1.setLayout(null);
        p2.setLayout(null);
		p3.setLayout(null);
    	
    	//RecipientNumLabel
        RecipientNumLabel = new javax.swing.JLabel();
        RecipientNumLabel.setText("Recipient(s):");
        p2.add(RecipientNumLabel);
        RecipientNumLabel.setBounds( 10 ,65 , 80, 15);
       
        // RecipientNo
        RecipientNo = new javax.swing.JTextField();
        p2.add(RecipientNo);
        RecipientNo.setBounds(10 , 85 ,270 , 20);
        RecipientNo.setText("");
        
        // StartsWith
        StartsWith = new javax.swing.JTextField();
        p2.add(StartsWith);
        StartsWith.setBounds(335 ,415 ,155 , 20);
        StartsWith.setText("");
     	
     	// Display search icon
     	try 
     	{
     		//this file is for start with icon & should be placed with the directory containing class files
     		java.net.URL url = PCapplication.class.getResource ("contacts.gif");
			ImageIcon icon = new ImageIcon(url);
			JLabel SearchIcon = new JLabel(icon);
			SearchIcon.setBounds (270 ,415 ,100 , 40);
			p2.add(SearchIcon);
			SearchIcon.setOpaque (false);
			SearchIcon.setVisible (true);
  		}
  		catch (Exception e){Jout ("exception here");}
		
		// MessageLabel     
        MessageLabel = new javax.swing.JLabel();
        MessageLabel.setText("Message:");
        p2.add(MessageLabel);
        MessageLabel.setBounds( 10, 135 ,80 , 15);
		
		// MessageInfoLabel
		MessageInfoLabel = new javax.swing.JLabel ();   
		MessageInfoLabel.setText("");
        p2.add(MessageInfoLabel);
        MessageInfoLabel.setBounds( 90, 135 ,60 , 15);
		
		// Message
        Message = new javax.swing.JTextArea(10,10);
        p2.add(Message);
        Message.setBounds( 10, 155 ,270 , 130);
        Message.setLineWrap (true);
        Message.setWrapStyleWord (true);
                
        //scroll pane for the message area display
        javax.swing.JScrollPane MessageScrollPane = new javax.swing.JScrollPane(Message);
        p2.add(MessageScrollPane);
        MessageScrollPane.setBounds( 10, 155 ,270 , 130);
        
        // ClearMsgButton
        ClearMsgButton = new javax.swing.JButton();
        ClearMsgButton.setText("Clear");
        p2.add(ClearMsgButton);
        ClearMsgButton.setBounds(60 , 295 , 65, 25);
        ClearMsgButton.setBackground(Color.LIGHT_GRAY);
        
        // SendButton
        SendButton = new javax.swing.JButton();
        SendButton.setText("Send");
        p2.add(SendButton);
        SendButton.setBounds( 170, 295 , 65, 25);
        SendButton.setBackground(Color.LIGHT_GRAY);
       	
       	// BackButton2
        BackButton2 = new javax.swing.JButton();
        BackButton2.setText("Back");
        p2.add(BackButton2);
        BackButton2.setBounds(115, 350 ,65 , 25);
        BackButton2.setBackground(Color.LIGHT_GRAY);
        
        // QuitButton3
        QuitButton3 = new javax.swing.JButton();
        QuitButton3.setText("Home Page");
        p3.add(QuitButton3);
        QuitButton3.setBounds(250, 260, 140, 25);
		QuitButton3.setBackground(Color.LIGHT_GRAY);
		
        // SendNewMessageButton
        SendNewMessageButton = new javax.swing.JButton();
        SendNewMessageButton.setText("Send new message");
        p3.add(SendNewMessageButton);
        SendNewMessageButton.setBounds(95, 260, 150, 25);        
        SendNewMessageButton.setBackground(Color.LIGHT_GRAY);
        
        // SearchForDevicesButton
        SearchForDevicesButton = new javax.swing.JButton();
        SearchForDevicesButton.setText("Search for Devices");
        SearchForDevicesButton.setVisible(true);
        p1.add(SearchForDevicesButton);
        SearchForDevicesButton.setBounds(183, 135, 142, 25);
        SearchForDevicesButton.setBackground(Color.LIGHT_GRAY);
        
        // QuitButton
        QuitButton = new javax.swing.JButton();
        QuitButton.setText("Exit");
        p1.add(QuitButton);
        QuitButton.setBounds(370, 390, 65, 25);
        QuitButton.setBackground(Color.LIGHT_GRAY);
        
        // PhoneBook
        PhoneBook = new javax.swing.JList();
        PhoneBook.setBounds( 335 , 85 ,155 ,320);
        
        // scroll pane for the phonebook list
        javax.swing.JScrollPane scrollPane = new javax.swing.JScrollPane(PhoneBook);
        p2.add(scrollPane);
        scrollPane.setBounds( 335 , 85 ,155 ,320);
        
        // RetrieveContacts
        RetrieveContacts = new javax.swing.JButton();
        RetrieveContacts.setText("Retrieve Contacts");
        p2.add(RetrieveContacts);
        RetrieveContacts.setBounds( 335, 50 , 155 , 25);
        RetrieveContacts.setBackground(Color.LIGHT_GRAY);

        // selectContactNumber
        SelectContactNumber = new javax.swing.JButton();
        SelectContactNumber.setText("<<");
        SelectContactNumber.setEnabled(false);
        p2.add(SelectContactNumber);
        SelectContactNumber.setBounds( 283, 85, 50 ,20 );
        SelectContactNumber.setBackground(Color.LIGHT_GRAY);
        
        // SelectDevice
        SelectDevice = new javax.swing.JButton();
        SelectDevice.setText("Select Device");
        p1.add(SelectDevice);
        SelectDevice.setBounds(185, 360, 140, 25);
        SelectDevice.setBackground(Color.LIGHT_GRAY);
        
        // DeviceList
        DevicesList = new javax.swing.JList();
        DevicesList.setBorder(new javax.swing.border.MatteBorder(null));
        p1.add(DevicesList);
        DevicesList.setBounds(165, 170, 180, 180);
        
        // DisplayText1
        DisplayText1 = new javax.swing.JTextPane ();
        DisplayText1.setOpaque( false );
        DisplayText1.setFont(new java.awt.Font("MS Reference Sans Serif", 1, 18));
        DisplayText1.setForeground(java.awt.Color.BLUE);
        p1.add(DisplayText1);
        DisplayText1.setBounds(90, 40, 170, 70);
        
        // DisplayText2
        DisplayText2 = new javax.swing.JTextPane ();
        DisplayText2.setOpaque( false );
        DisplayText2.setFont(new java.awt.Font("MS Reference Sans Serif", 1, 18));
        DisplayText2.setForeground(java.awt.Color.BLUE);
        p2.add(DisplayText2);
        DisplayText2.setBounds(10 ,8 ,490 , 40);

        // DisplayText3
        DisplayText3 = new javax.swing.JTextPane ();
		DisplayText3.setOpaque( false );
        DisplayText3.setEnabled (false);
        DisplayText3.setFont(new java.awt.Font("MS Reference Sans Serif", 1, 18));
        DisplayText3.setForeground(java.awt.Color.BLUE);
        p3.add(DisplayText3);
        DisplayText3.setBounds(30, 30 , 440, 220);
       
        addWindowListener(new java.awt.event.WindowAdapter() 
        {
            public void windowClosing(java.awt.event.WindowEvent evt) 
            {
                exitForm1(evt);
            }
            public void windowClosed(java.awt.event.WindowEvent evt) 
            {
                exitForm1(evt);
            }
        });        

       	SMSFrame = new javax.swing.JFrame();

        SMSFrame.addWindowListener(new java.awt.event.WindowAdapter() 
        {
            public void windowClosing(java.awt.event.WindowEvent evt) 
            {
                exitForm2(evt);
            }
            public void windowClosed(java.awt.event.WindowEvent evt) 
            {
                exitForm2(evt);
            }
        });
   		
   		ReportFrame = new javax.swing.JFrame();     
        
        ReportFrame.addWindowListener(new java.awt.event.WindowAdapter() 
        {
            public void windowClosing(java.awt.event.WindowEvent evt) 
            {
                exitForm2(evt);
            }
            public void windowClosed(java.awt.event.WindowEvent evt) 
            {
                exitForm2(evt);
            }
        });          
        
        SelectDevice.addActionListener(new java.awt.event.ActionListener() 
        {
            public void actionPerformed(java.awt.event.ActionEvent evt) 
            {
                SelectDeviceActionPerformed(evt);
            }
        });
        
  
        BackButton2.addActionListener(new java.awt.event.ActionListener() 
        {
            public void actionPerformed(java.awt.event.ActionEvent evt) 
            {
                BackButton2ActionPerformed(evt);
            }
        });
        
        
        QuitButton3.addActionListener(new java.awt.event.ActionListener() 
        {
            public void actionPerformed(java.awt.event.ActionEvent evt) 
            {
                QuitButton3ActionPerformed(evt);
            }
        });
        
        
        SendNewMessageButton.addActionListener(new java.awt.event.ActionListener() 
        {
            public void actionPerformed(java.awt.event.ActionEvent evt) 
            {
                SendNewMessageButtonActionPerformed(evt);
            }
        });
        
        SearchForDevicesButton.addActionListener(new java.awt.event.ActionListener() 
        {
            public void actionPerformed(java.awt.event.ActionEvent evt) 
            {
            	searchDevicesThreadCounter++;
            	//this variable is set to zero searching for devices is completed
            	if (searchDevicesThreadCounter == 1)  //to ignore multiple clicks of the user
                	SearchForDevicesButtonActionPerformed(evt);
            }
        });
        
        ClearMsgButton.addActionListener(new java.awt.event.ActionListener() 
        {
        	public void actionPerformed(java.awt.event.ActionEvent evt) 
        	{
                Message.setText("");
            }
        });
        
        /*
         * implements the key listener so that whenever the user releases a key
         * all the contact names starting with that key are shown in the list of imported contacts
    	 */
    	StartsWith.addKeyListener(new java.awt.event.KeyListener ()
    	{
    		public void keyTyped (java.awt.event.KeyEvent kv)
    		{
    		}
    		public void keyReleased (java.awt.event.KeyEvent kv)
    		{
    			StartsWithKeyTyped ();
    		}
    		public void keyPressed (java.awt.event.KeyEvent kv)
    		{
    		}
    	});
    	/*
    	 * implements the key listener so that as the user releases a key
    	 * a number beside the message label will indicate to him the number 
    	 * of characters displayed as well as how many SMS will be sent for this message
    	 * One SMS contains a maximum of 160 characters
    	 */
    	Message.addKeyListener(new java.awt.event.KeyListener ()
    	{
    		public void keyTyped (java.awt.event.KeyEvent kv)
    		{
    		}
    		public void keyReleased (java.awt.event.KeyEvent kv)
    		{
				MessageKeyTyped ();    			    				
    		}
    		public void keyPressed (java.awt.event.KeyEvent kv)
    		{	
    		}
    	});
        
        
        SendButton.addActionListener(new java.awt.event.ActionListener() 
        {
        	public void actionPerformed(java.awt.event.ActionEvent evt) 
        	{
                SendButtonActionPerformed(evt);
            }
        });

        RetrieveContacts.addActionListener(new java.awt.event.ActionListener() 
        {
            public void actionPerformed(java.awt.event.ActionEvent evt) 
            {
                RetrieveContactsActionPerformed(evt);
            }
        });

        SelectContactNumber.addActionListener(new java.awt.event.ActionListener() 
        {
        	public void actionPerformed(java.awt.event.ActionEvent evt) 
        	{
        		SelectContactNumberActionPerformed(evt);
            }
        });
                
        QuitButton.addActionListener(new java.awt.event.ActionListener() 
        {
            public void actionPerformed(java.awt.event.ActionEvent evt) 
            {
                QuitButtonActionPerformed(evt);
            }
        });

		//setting the positions of the frames
		
        java.awt.Dimension screenSize = java.awt.Toolkit.getDefaultToolkit().getScreenSize();
        setBounds((screenSize.width-450)/2, (screenSize.height-450)/2, 300, 300);
        pack(); 
        
        SMSFrame.setBounds((screenSize.width-450)/2, (screenSize.height-450)/2, 300, 300);
       
        ReportFrame.setBounds((screenSize.width-450)/2, (screenSize.height-450)/2, 300, 300);
  	}
  	
  
  	/*
  	 * Gets the selected bluetooth device by the user and call the service discovery method over it
  	 */
    private void SelectDeviceActionPerformed(java.awt.event.ActionEvent evt) 
    {
       int selectedDevice = DevicesList.getSelectedIndex();
       
       if (selectedDevice != -1)
       {
       		blue.selectedDevice = selectedDevice;  //the blue instance discover services on that device
       		
       		/* 
       		 * a new thread to do service discovery 
       		 * so as not to freeze the current thread
       		 */
       		Thread t = new Thread ()
       		{
       			public void run ()
       			{
       				doServiceSearch ();
       			}  			
       		};
       		t.start ();

       	}
    }

    /*
     *Starts searching for nearby bluetooth devices as the user clicks on Search for devices button
     */
    private void SearchForDevicesButtonActionPerformed(java.awt.event.ActionEvent evt) 
    {
    	DevicesList.setListData(new Vector());
		/* a new thread to do device discovery 
		 * so as not to freeze the current thread
		 */
		Thread t =new Thread ()
		{
			public void run ()
			{
				doDeviceSearch ();

        	}
        
        } ; 
    	t.start ();
    }
    
    /*
     * Returns to the frame which allows the user to enter the SMS to be routed to the phone
     */
    private void SendNewMessageButtonActionPerformed(java.awt.event.ActionEvent evt) 
    {
    	RecipientNo.setText("");
        Message.setText("");
   		StartsWith.setText("");
   		DisplayText2.setVisible (false);
   		SMSFrame.setVisible(true);
    	ReportFrame.setVisible(false);
    	messageCharacters = 160;
  		messageNumber = 1;
    }
    
    /*
     * when the user chooses to quit the application
     * It avoids any abnormal termination of the client and 
     * notifies the server of the user action so that the server doesnt keep waiting for the same client. 
     * so this function sends a message to the server via bluetooth
     * informing it that the client will exit
     */    
     
    private void QuitButton3ActionPerformed(java.awt.event.ActionEvent evt) 
    { 		
  		try	
  		{
  			//ensures that the server is still connected before sending a message to it  
  			if (blue.checkCellularPhoneConnectivity())     
   			{
   				client.out.writeUTF("CLIENT_EXIT");
   			}
           	client.closeConnection();
        }
        catch (Exception e)
        {
        }
    	client.contactNames.removeAllElements ();
    	this.setVisible (true);
    	ReportFrame.setVisible (false);
    	showFirstPage();    			
    }
    /*
     *Displays the contacts retrieved from the phone in a list
     *
     */
    private void RetrieveContactsActionPerformed(java.awt.event.ActionEvent evt) 
    {
    	//thread prompts the user to wait till the contacts are retrieved from the phone
    	Thread t = new Thread ()
    	{
    		public void run ()
    		{
    			if (client.connectionOpen == true)
    			{
    				DisplayText2.setVisible (true);
 					DisplayText2.setText ("Retrieving contacts..");	 		
    				while (client.importedPhonebook==false)  //the client instance hasnt imported the phone book yet
    				{
    					try
    					{
            				Thread.sleep(1000);
           					
           					DisplayText2.setText(DisplayText2.getText() + "." );
        					
        					if (DisplayText2.getText().equals ("Retrieving contacts...."))
        						DisplayText2.setText("Retrieving contacts");
            				
            				if (client.importingPhonebook == false)  //the client instance finished importing phone book with error
            				{
            					DisplayText2.setVisible (false);
            					showMessageBox  ("Unable to retrieve contacts from the cell phone.", "SMS Routing",javax.swing.JOptionPane.DEFAULT_OPTION,javax.swing.JOptionPane.ERROR_MESSAGE, "HomePage");
    							return;
            				}
            		
       					}
        				catch (Exception e)
        				{
        				}
 
		    		}
		    		
		    		if (client.importedPhonebook==false)
		    		{
						showMessageBox  ("Unable to retrieve contacts from the cell phone. Please try again.", "SMS Routing",javax.swing.JOptionPane.DEFAULT_OPTION,javax.swing.JOptionPane.ERROR_MESSAGE, "");
		    		}
    		
       				PhoneBook.setListData(client.contactNames);
       				SelectContactNumber.setEnabled(true);   
    			}
    			else
    			{
    				showMessageBox  ("Unable to connect to the phone. Please try again.", "Error",javax.swing.JOptionPane.DEFAULT_OPTION,javax.swing.JOptionPane.ERROR_MESSAGE, "HomePage");
    			}
    			
    			DisplayText2.setVisible (false);
    		}
    	};
    	t.start ();
	}
    /*
     * It copies the number from the contact list to the the recipient number
     */
    private void SelectContactNumberActionPerformed(java.awt.event.ActionEvent evt) 
  	{ 
  		if (StartsWith.getText().equals (""))
 		{
 			addToIndex = 0;
  		}
  		int selectedIndexName = PhoneBook.getSelectedIndex();
        if (selectedIndexName == -1)
        	return;
        String alreadyEntered;
        if(RecipientNo.getText().equalsIgnoreCase(""))
        {
        	RecipientNo.setText((String)client.contactNumbers.elementAt(selectedIndexName + addToIndex));
        }
        else
        {
        	alreadyEntered = RecipientNo.getText();   //append to the numbers already entered
        	RecipientNo.setText(alreadyEntered+","+(String)client.contactNumbers.elementAt(selectedIndexName + addToIndex));
        }
    }
    
    /*
     * Ensures that the client has entered at least one number and a message
     * it also ensures that the server is still connected to the client before sending 
     * 
     */
 	private void SendButtonActionPerformed(java.awt.event.ActionEvent evt) 
    {

    	int begin = 0;
		int maxRecipientNo = 10;
		//int countRecipients = 0;
		//String desiredNumbers [];
        //String msg = Message.getText();
		countRecipients = 0;
        msg = Message.getText();
        String number;
        String allEntered = RecipientNo.getText()+',';
       
        if (msg.equals("") && allEntered.equals (","))
        {
        	showMessageBox  ("Please enter a message and desired number(s).", "SMS Routing",javax.swing.JOptionPane.DEFAULT_OPTION,javax.swing.JOptionPane.INFORMATION_MESSAGE, "");
        	return;
        }
        else if (msg.equals(""))
        {
           	showMessageBox  ("Please enter a message.", "SMS Routing",javax.swing.JOptionPane.DEFAULT_OPTION,javax.swing.JOptionPane.INFORMATION_MESSAGE, "");
           	return;
        }
        else if (allEntered.equals(","))
        {
        	showMessageBox  ("Please enter the desired number(s).", "SMS Routing",javax.swing.JOptionPane.DEFAULT_OPTION,javax.swing.JOptionPane.INFORMATION_MESSAGE, "");
        	return;
        }
        
        
    	//if the client is still trying to retrieve the contacts from the server.
    	//the user should wait  until the import finishes
    	while (client.importedPhonebook ==false)
    	{
    		try 
    		{
    			Thread.sleep (1000);
    		}
    		catch (Exception e)
    		{
    		}
    		if (client.importingPhonebook == false) //finished to retrieve contacts but with error 
    		{
    			if (client.connectionOpen == false)
				{	
					showMessageBox  ("Unable to connect to the cell phone", "SMS Routing",javax.swing.JOptionPane.DEFAULT_OPTION,javax.swing.JOptionPane.INFORMATION_MESSAGE, "Home Page");    			
					return;
				}
				break;
    		}
    	}
    	
    	try	
  		{
  			if (!blue.checkCellularPhoneConnectivity())
   			{
   				showMessageBox  ("Unable to connect to the cell phone. Please try again.", "SMS Routing",javax.swing.JOptionPane.DEFAULT_OPTION,javax.swing.JOptionPane.ERROR_MESSAGE, "HomePage");
				client.closeConnection();
	    		this.setVisible(true);
    			SMSFrame.setVisible (false);
        		showFirstPage ();  
   				return;
   			}
        }
        catch (Exception e)
        {
        }        
        
        
        	
        desiredNumbers = new String [maxRecipientNo];	
        
        for(int i = 0; i < allEntered.length(); i++)
        {
        	if(allEntered.charAt(i) == ',')
        	{
        		number = allEntered.substring(begin,i);
        		if(countRecipients < maxRecipientNo)
        		{
        			desiredNumbers[countRecipients] = number;
        			countRecipients++;
        		}
        		else
        		{
   			       	showMessageBox  ("You can only send a maximum of ten numbers at a time.", "SMS Routing",javax.swing.JOptionPane.DEFAULT_OPTION,javax.swing.JOptionPane.INFORMATION_MESSAGE, "");
        			return;
        		}
        		begin = i + 1;
        	}
        }
        
		if (client.connectionOpen == false)
		{
			showMessageBox  ("Unable to connect to the cell phone. Please try again.", "SMS Routing",javax.swing.JOptionPane.DEFAULT_OPTION,javax.swing.JOptionPane.ERROR_MESSAGE, "HomePage");
		}
		//Two threads in order not to freeze the current thread
		//Thread for routing the SMS to the cellular phone
   		Thread t = new Thread ()
   		{
   			public void run ()
   			{
   				try
   				{
   					client.SMSroute(msg,countRecipients,desiredNumbers);
   				}
   				catch (Exception e)
   				{
   					showMessageBox  ("Unable to route the message to the phone.", "SMS Routing",javax.swing.JOptionPane.DEFAULT_OPTION,javax.swing.JOptionPane.INFORMATION_MESSAGE, "HomePage");
   				}
   			}
   		};
   		t.start();
   	
   		// Thread to wait for the server response indicating the success of sending the message and display "Sending.." message to the user		
   		Thread t2 = new Thread ()
   		{
   			public void run ()
   			{
   				doWaitForSendingResponse ();
   			}
   		};
   		t2.start ();
	}
	/*
	 * The user desires to quit before a connection with the server established
	 */
	private void QuitButtonActionPerformed(java.awt.event.ActionEvent evt) 
    {
        System.exit(0);  
    }
 
    private void StartsWithKeyTyped ()
    {
    	/* initializes the variable addToIndex to zero each time the user releases a key
    	 * There are two vectors one containing the numbers and another containing the names
    	 * the list of contacts displayed to the user contains only the names
    	 * this variable is used because the list of devices contains only the contacts that starts with a certain string entered by the user
    	 * hence, the index of the contact in the list will change causing wrong numbers to be retrieved from the vector 
    	 * so I used that variable to make track of the index of the selected device 
    	 */
    	if ((client.importingPhonebook==false) || (client.importedPhonebook==false))
    		return;
    	addToIndex = 0;      
    	
    	boolean visited = false;
    	while (client.importedPhonebook==false)
    	{
    		try
    		{
           		Thread.sleep(1000);
       		}
        	catch (Exception e)
        	{
        	}
 
    	}
    	
    	Vector startsWith = new Vector (); //containing the contacts starting with a certain string entered by the user
    		
    	for (int i = 0 ; i < client.contactNames.size(); i++)
    	{
    		if (client.contactNames.elementAt(i).toString().toLowerCase().startsWith(StartsWith.getText().toLowerCase()))
    		{
    			startsWith.addElement(client.contactNames.elementAt(i));
    			if (!visited)
    			{
    				visited = true;
    				addToIndex = i;  //this index will be added to the index of the selected contact name to retrieve the right number from the numbers' vector
    			}
    		}
    	}
       		
       	PhoneBook.setListData(startsWith);
       	SelectContactNumber.setEnabled(true);   
   
   		visited = false;
    }
	/*
	 * messageNumber: variable that contains the number of SMS that will be sent from the phone 
	 * 				  carrying the message entered by the user
	 * messageCharacters: variable that contains the number of characters left in this message to be 
	 *					  to be sent within the same SMS     
	 */    
    private void MessageKeyTyped ()
    {
    	messageNumber = (Message.getText ().length () / 160) + 1;
 
    	messageCharacters = (messageNumber * 160) - Message.getText ().length ();

    	if (messageCharacters == 1)
    	{
    		messageCharacters = 160;
    	}
  		MessageInfoLabel.setText (messageCharacters + "["+messageNumber+"]");
    }
    
    /*
     * Returns back to the home page
     * the method notifies the server that the client will quit to release the erver from the task of waiting for serving this specific client 
     */    
    
    private void BackButton2ActionPerformed(java.awt.event.ActionEvent evt) 
    {
  		try	
  		{
  			if (blue.checkCellularPhoneConnectivity())
   			{
   				client.out.writeUTF("CLIENT_EXIT");
   			}
           	client.closeConnection();
	    	this.setVisible(true);
    		SMSFrame.setVisible (false);
        	showFirstPage ();  
        }
        catch (Exception e)
        {
        }
    }
    
    /*
     * The client closes the application from the window
     */
    private void exitForm1(java.awt.event.WindowEvent evt) 
    {
        System.exit(0);
    }
   	/*
   	 *There is already a connection with the server so the client sends a message to the server when it comes to quit
   	 */
   	private void exitForm2(java.awt.event.WindowEvent evt) 
    {    
      	try	
  		{
  			//ensures that the server is still connected before sending a message to it  
  			if (blue.checkCellularPhoneConnectivity())     
   			{
   				client.out.writeUTF("CLIENT_EXIT");
   			}
           	client.closeConnection();
        }
        catch (Exception e)
        {
        }
        System.exit(0);
    }

  /*
   * startApp Shows home page
   */
  protected void startApp() 
  {
  	blue = new BTworker ();
    showFirstPage();
  }

  /*
   *Displays the home page components
   */
  public void showFirstPage() 
  {
  	//any interface added here
  
  	DisplayText1.setVisible(false);
  	SelectDevice.setVisible(true);
  	SelectDevice.setEnabled (false);
  	DevicesList.setListData(new Vector());
  	DevicesList.setVisible (true);
	
	PhoneBook.setListData (new Vector ());
	
	this.getContentPane ().add (p1);

	QuitButton.setVisible(true);
	SearchForDevicesButton.setVisible(true);      
  }
  
  /*
   *Displays the sms frame components
   */  
  public void showSecondPage ()
  {
  	messageCharacters = 160;
  	messageNumber = 1;
  	MessageInfoLabel.setText (messageCharacters + "["+messageNumber+"]");
    SMSFrame.setSize(510,510);
  	SMSFrame.getContentPane().add(p2);
    SMSFrame.setVisible(true);
  
    DisplayText2.setVisible(false);
  	ReportFrame.setVisible(false);
  	this.setVisible(false);
  	SelectContactNumber.setEnabled(false);   
  }
  
  /*
   *diplays error messages and informative messages to the use
   *parameters: message to be contained 
   *      		title of the box
   *			optiontype: OK, Cancel,etc..
   *			messageType: error message, info message, etc..
   */
  public void showMessageBox (String message, String title, int optionType, int messageType, String display)
  {
  	javax.swing.JOptionPane msgBox = new javax.swing.JOptionPane ();
  	msgBox.showConfirmDialog(this,message,title, optionType , messageType);
  	if (display.equals ("HomePage"))
  	{
  		showFirstPage ();
  	}
  }
  
  public void Jout (String s)
  {
  	System.out.println (s);
  }
  
  /*
   *Displays the bluetooth devies discovered
   */
  public void showBTDevices ()
  {
  	DevicesList.setListData(theDevices);

    SelectDevice.setEnabled(true);   
  }
  /*
   * the blue instance starts the search for the devices
   */
  public void doDeviceSearch ()
  {
  	DisplayText1.setText ("Searching.");
  	theDevices = new Vector();
	DevicesList.removeAll();	
	
	blue.startDeviceInquiry();
	
	//waits until the blue instance complete its search for devices
	while (BTworker.deviceWaiting == true)
    { 
    	try
        {
        	DisplayText1.setVisible(true);
        	DisplayText1.setText(DisplayText1.getText() + "." );
        	if (DisplayText1.getText().equals ("Searching...."))
        		DisplayText1.setText("Searching");
        	Thread.sleep(1000);
         }
         catch(java.lang.InterruptedException e)
         {
         }
     }	
        	
     if (BTworker.devices.size() > 0)  
     {
     	//some devices where discovered and placed in the vector for devices
     	
      	DisplayText1.setVisible(false);
      	Jout ("[Found "+BTworker.devices.size()+" devices, selected one of the following:");
      	for (int i = 0; i < BTworker.devices.size(); i++)
      	{
        	try
        	{
        		RemoteDevice device = (RemoteDevice) BTworker.devices.elementAt(i);
        		String name = device.getFriendlyName(false);
        		theDevices.addElement("["+i+"] "+name);
       		} 
       		catch (Exception e)
        	{
        		e.printStackTrace();
        	}
      	}
      	showBTDevices ();
      }
      else
      {
      	//no devices were discovered
      	
      	DisplayText1.setVisible(true);
      	showMessageBox  ("No devices found. Try again later.", "SMS Routing",javax.swing.JOptionPane.DEFAULT_OPTION,javax.swing.JOptionPane.INFORMATION_MESSAGE, "HomePage");
       	Jout ("no devices found." +"\n");
      }
      
      searchDevicesThreadCounter = 0; 
  }
  
  /*
   * the blue instance does service discovery on the selected device by the user 
   */
  public void doServiceSearch ()
  {
    ReportFrame.setSize(510,510);
  	ReportFrame.getContentPane().add(p3);    
  		
    blue.startServiceInquiry();
   
    DisplayText1.setText ("Connecting.");   
    //waits tll the blue instance finishes discovering the services
    while (blue.serviceWaiting==true)
    {
    	try 
       	{
     		DisplayText1.setVisible(true);
        	DisplayText1.setText(DisplayText1.getText() + "." );
       		if (DisplayText1.getText().equals ("Please wait...."))
        		DisplayText1.setText("Please wait");
       		Thread.sleep(1000);
       	}
       	catch (Exception e)
     	{
	   	}
    }
    
    /*the blue instance finished service discovery on the device
     *if the blue instance succeeded in finding services (the vector of services is not empty), 
     *the application start running the client that will perform the routing over bluetooth
     */
    if (blue.services.size() > 0)
    {
   		Thread t = new Thread ()
    	{
    		public void run ()
    		{
    			startClient ();
    		}
    	};
    	t.start();
    
    	showSecondPage ();
    }
    else
    {
    	DisplayText1.setVisible(false);
    	showMessageBox  ("Unable to connect to the selected device. Please try again.", "Error",javax.swing.JOptionPane.DEFAULT_OPTION,javax.swing.JOptionPane.ERROR_MESSAGE, "HomePage");
    }
  }
  
  /*
   * Method responsible for waiting for server response after sending the SMS to it via bluetooth 
   * and display the result in a frame to the user
   */
  public void doWaitForSendingResponse ()
  {
  	DisplayText2.setText ("Sending SMS..");
  	DisplayText2.setVisible(true);   
  	
  	//waiting for the server response whether it succeeded in sending the message or not
    
    while (client.serverSendingResponse == false)
    {
		try 
       	{	
       		if (client.connectionOpen == false)
       			break; 	
        	
        	DisplayText2.setText(DisplayText2.getText() + "." );
        	if (DisplayText2.getText().equals ("Sending SMS...."))
        		DisplayText2.setText("Sending SMS");
       		Thread.sleep(1000);
       	}
       	catch (Exception e)
     	{
	   	}
     }  	   
     
     Thread t = new Thread ()
     {
    	public void run ()
    	{    		
        	ReportFrame.setVisible(true); 	   
 	    	SMSFrame.setVisible(false);
  			SendNewMessageButton.setVisible(true);
  			QuitButton3.setVisible(true);
    		DisplayText3.setVisible (true);
 
     		if ((client.connectionOpen==false) && (client.serverSendingResponse == false))
     		{
     			showMessageBox  ("Connection to the phone was lost. Please try again.", "Error",javax.swing.JOptionPane.DEFAULT_OPTION,javax.swing.JOptionPane.ERROR_MESSAGE, "HomePage");
     		}
     		if (client.unsuccessfulNumbers.size()==0)
     		{
     			//the cellular phone succeeded in sending the SMS to the desired number (s)
     			DisplayText3.setText("Your messages was Routed Successfully.");
     		}
     		else
     		{
     			//the cellular phone failed to send SMS to all or some of the numbers
     			DisplayText3.setText("Your message to the following Recipient(s) failed\n " + "\n");
     			
     			for(int i = 0 ; i < client.unsuccessfulNumbers.size() ; i++)
     			{
     				DisplayText3.setText(DisplayText3.getText() + client.unsuccessfulNumbers.elementAt(i) + "\n");				
     			}
     		}
        		
        }
    };
    t.start ();    
    
  }

  /*
   * startClient. Starts bluetooth client thread.
   */
  private void startClient() 
  {
      	client = new Client ();
       	try
       	{
       		client.run_client(blue.getOneDiscoveredService());
       	}
       	catch (Exception e)
       	{

       	}
  }
  
  public static void main(String args[])
  {
	try
    {
    	//the default look and feel
        javax.swing.UIManager.setLookAndFeel("com.sun.java.swing.plaf.motif.Motif.LookAndFeel");
    }
    catch (Exception e)
    {
    }
    
    PCapplication application = new PCapplication();
    application.setSize(510,510);
    application.startApp();
    	
 	application.show();
	
	//does not spawn other clients until this current client is completely served
   	
   	Object obj = new Object();
    
   	synchronized( obj )
   	{
   		try 
   		{
       		obj.wait();
   		}
   		catch (InterruptedException ex) 
   		{
   		}
   	}
  	
  }
  
  /*
   *Class responsible for displaying the background
   */
  class Background extends JPanel
  {	
  	public Background(){}
 	
	public void paint(Graphics g)
	{
		g.drawImage(bi , 0, 0, null);
		super.paint(g);

	}
			
  }
}
