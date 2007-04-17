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
import java.util.Vector;

import javax.bluetooth.*;
import javax.microedition.io.*;
import javax.microedition.lcdui.*;


import javax.wireless.messaging.TextMessage.*;
import javax.wireless.messaging.*;

//for importing phonebook
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;
import javax.microedition.midlet.MIDlet;
import javax.microedition.pim.*;
import java.util.Enumeration;


public class Server implements Runnable, CommandListener 
{
	// major service class as TELEPHONY
  	private final static int TELEPHONY = 0x400000;
	
	private CellPhoneMIDlet midlet;
  	
  	private StreamConnectionNotifier notifier = null;
  	private StreamConnection streamConnection = null;
  	
  	private DataOutputStream out;
  	private DataInputStream in;

	private LocalDevice localDevice;
	private RemoteDevice remoteDevice;
 	
 	private Command QuitCommand;
 	
 	Form f;
 	Form f2;
 	
  /**
   * serviceURL, Service URL that is opened when this server starts to
   * provide service.
   */
  private String serviceURL = "btspp://localhost:"+CellPhoneMIDlet.UUIDString+";name=RoutingSMS";

  private boolean connectionOpen;
  private boolean clientExit = false;
  boolean applicationClosed;
  private Vector unsuccessfulNumbers;
  
  Thread thread ;

  /**
   * Server, Constructor that creates UI and sets discoverable mode to LIAC to
   * speed up device inquiry by the clients.
   *
   * @param midlet CellPhoneMIDlet
   */
  public Server (CellPhoneMIDlet midlet) 
  {
    this.midlet = midlet;
    connectionOpen = false;
	applicationClosed = false;
	unsuccessfulNumbers = new Vector ();
	
    try 
    {
      localDevice = LocalDevice.getLocalDevice();
      localDevice.setDiscoverable(DiscoveryAgent.GIAC);
      notifier = (StreamConnectionNotifier) Connector.open(serviceURL);
    }
    catch (IOException e) 
    {
    }
  }
  
  /**
   * Creates this thread and starts it.
   * start a thread to serve the server connection.
   * only one server thread is started
   * see run() for the task of this thread
   */
  public void start() 
  {
    thread = new Thread(this);
    thread.start();
  }

  /**
   * Run method that is called when this thread starts. opens connectin and
   * starts to wait for clients to conatct. After succesful connection by a
   * client starts to read data from the client and then sends response to the
   * client. 
   */
  public void run() 
  {
    f = new Form("Waiting...");
	f2 = new Form("Connected to client");
	
	QuitCommand = new Command("Quit", Command.EXIT, 1);
	
	f.addCommand(QuitCommand);
    f.setCommandListener(this);
    
    f2.addCommand(QuitCommand);
    f2.setCommandListener(this);		 
	
    Gauge gauge = new Gauge("Waiting for clients to contact", false,
                            Gauge.INDEFINITE, Gauge.CONTINUOUS_RUNNING);
    f.append(gauge);

	while (! applicationClosed) 
 	{
 		Display.getDisplay(midlet).setCurrent(f);
 		
 		clientExit = false;
 				
		waitAndOpenConnection ();
		
    	if (connectionOpen == true) 
    	{
    		try
			{	
				retrieveContacts ();
			}
			catch (IOException e)
			{
			//	Jout ("Exception in connection to client\n");
				
				continue; //waits for another client
			}
			catch (Exception e)
			{
			//	Jout ("exception in retrieve contacts\n");
				 				//Jout ("Unable to retrieve names and numbers "+"\n");
  				try 
				{
					Jout ("sending EXCEPTION"+"\n");
					out.writeUTF("EXCEPTION");  //notifies the client that an error occured while retrieving the contacts
					out.flush ();
				}
				catch (Exception ee)
				{
				}
			}
			
			while (true)		
			{
				if (connectionOpen == true)
				{
					try
					{
						sendSMS ();
						if (applicationClosed==true)
						{
							break;
						}
						if (clientExit==true)
						{
							break;
						}	
					}
					catch (Exception e)
					{
 				//		Jout ("Exception in sending SMS" + "\n");
 						//try{Thread.sleep (1000);}catch (Exception eee){}
 						break;
 					}
 				}
 				else
 				{
 			//		Jout ("connection with PC failed failed"+"\n");
 					break;
 				}
 			}
    	}
    	else
    	{
    		break;
    	}
    	
    	f2.deleteAll ();
    }
    
    closeConnection();
    midlet.showMenu();
    
  }

  public void waitAndOpenConnection ()
  {
  	try 
	{
		//this will block the current thread until a client connects
    	streamConnection = notifier.acceptAndOpen(); 
    	    
    	// Retrieve the service record template
    	ServiceRecord rec = localDevice.getRecord( notifier );
    	
    	// Setting the Major Service Classes flag in Bluetooth stack.
    	// which I choose to be Telephony Service
    	rec.setDeviceServiceClasses( TELEPHONY  );
    	
    	remoteDevice = RemoteDevice.getRemoteDevice( streamConnection );
        
    	//Jout ("remoteDevice.getBluetoothAddress (): " + remoteDevice.getBluetoothAddress ()+ "\n");
		Jout ("Connected to  " + remoteDevice.getFriendlyName (false)+"\n");
    	
    	// obtain an input stream to the remote service  
    	      
    	in = streamConnection.openDataInputStream();
	    out = streamConnection.openDataOutputStream();

   		connectionOpen = true;
   	}
    catch(Exception e)
    {
		connectionOpen = false;
        return;
    }

  }
	
  /**
   * Handles Server Command events.
   *
   * param cmd Command
   * param disp Displayable
   */
   
  public void commandAction(Command cmd, Displayable d) 
  {
    if (d == f2) 
    {
      if (cmd == QuitCommand) 
      {
      	applicationClosed = true;
      }
    }
   	else if ((d==f) && (cmd == QuitCommand))
   	{
   		try
   		{
        	notifier.close();
        }
        catch (Exception e)
        {
        }
		   		
   	}
  }	
  /**
   * Closes connection and streams.
   */
  public void closeConnection() 
  {
    if (connectionOpen == true) 
    {
      connectionOpen = false;

      try 
      {
        in.close();
        out.close();
        streamConnection.close();
        notifier.close();
      }
      catch (IOException e) 
      {
      }
    }

  }
  
  /*
   *Method responsible for getting the databases containing the contacts 
   */  
  public void retrieveContacts ()
  throws Exception
  {
  	String[] allContactLists = PIM.getInstance().listPIMLists (PIM.CONTACT_LIST); 
 
  	if (allContactLists.length > 0)
  	{
  		for (int i = 0; i < allContactLists.length;i++)
  		{
  			try
  			{
  				retrieveNamesAndNumbers (allContactLists[i]);
  			}
  			catch (Exception e)
  			{
  				return;
  			}
  		}
  	}
  	out.writeUTF ("EOF");   //indicates to the clients end of contacts 
	out.flush();
  }
  
  /*
   * Retrieves the names and the numbers of the contacts and passes them to the client 
   */
  public void retrieveNamesAndNumbers (String theContactList)
  throws Exception
  {
	ContactList clist = null;
	try
	{
		//get items in contacts and then get the list in these items nd then get the items in each list
		clist = (ContactList) PIM.getInstance().openPIMList(PIM.CONTACT_LIST, PIM.READ_ONLY, theContactList);	
		Jout ("Openning Contacts" +"\n");
	}
	catch (PIMException e) 
	{
		// failed opening the default contact list!
		Jout ("Error accessing database - aborting action" +"\n");
	} 
	catch (SecurityException e) 
	{
		// user rejected application's request for
		// read access to contact list
		// This is not an error condition and can be normal
		Jout("Okay, this application won't get contacts"+"\n");
	}
	// Retrieve contact values
	try
	{
		Enumeration contacts = clist.items();//This operation takes most of the time of importing
		
		//Jout ("before hasMoreElements \n" );
		
		while(contacts.hasMoreElements() )
		{
			Contact contact = (Contact) contacts.nextElement();
			
        	String contactNumber = "";  
        	String contactName = "";
        	
        	contactName = contact.getString(Contact.FORMATTED_NAME, 0);
        	
        	/* 
        	 * The countValues() method returns the number of data values currently
			 * set in a particular field.
        	 */
        	int numValues = contact.countValues(Contact.TEL);
			
			for (int i = 0; i < numValues; i++)
			{
				if (contact.getAttributes(Contact.TEL, i) == Contact.ATTR_WORK)
				{
					//work = contact.getString(Contact.TEL, i);
				}
				else if (contact.getAttributes(Contact.TEL, i) == Contact.ATTR_HOME)
				{
					//home = contact.getString(Contact.TEL, i);
				}
				
				// I only need the mobile number and the name
				else if (contact.getAttributes(Contact.TEL, i) == Contact.ATTR_MOBILE)
				{
					contactNumber = contact.getString( Contact.TEL, i );
					out.writeUTF( contactNumber+" "+contactName );
        			out.flush();
				}
				else if (contact.getAttributes(Contact.TEL, i) == Contact.ATTR_PAGER)
				{
					//	pager = contact.getString(Contact.TEL, i);
				}
			}
			out.flush() ;
		}
		//Jout ("After hasMoreElement\n");

	}
	catch( PIMException e )
	{
    	// an error occurred and raised a PIM access exception 
	}
	catch( SecurityException e )
	{
    	// can't read this list (security Exception)
	} 	
	
	clist.close ();
  }
  
  /*
   * Method responsible for sending the SMS to the desired numbers entered by the user on the pC
   */ 
   
  public void sendSMS ()
  throws Exception
  {

  	String message=null;
  	Jout ("Waiting for user message"+"\n");

	try
	{	
		//checks whether the buffer empty
		//waits for the same client to see if he likes to send another message
		while (in.available()==0)  
		{	
			if (applicationClosed==true)  //the application on the phone was closed by the user
				return;
			
			Thread.sleep(5000);
		
		}		
    	message = in.readUTF();   //reading the message from the user on PC
    	
    	if (message.equals ("CLIENT_EXIT"))  //the client on PC quit the application and so no messages to be sent
    	{
    		clientExit = true;
    		return;
    	}
    
    }
    catch (EOFException eof)
    {
    	//Jout ("EOF Exception" + "\n");
    	return;
    }
    catch (IOException io)
    {
    	clientExit = true;
    	//Jout ("IO Exception"+"\n");	
    	return;
    }
    catch (Exception e)
    {
    	return;
    }
    
    //reading number of recipients
    int countDesiredNumbers = in.read ();
    String cellPhoneNumber;
    
  	MessageConnection smsConnection = null;
    
    unsuccessfulNumbers.removeAllElements(); //clears the vector bec of batch sending
        
    for(int i=0; i<countDesiredNumbers; i++)
    {
        cellPhoneNumber = in.readUTF(); //reading phone number to send sms to
		//sending SMS
		String address = "sms://" + cellPhoneNumber;
		try 
        {
            /** Open the message connection. */
            
            smsConnection = (MessageConnection)Connector.open(address);
            TextMessage txtmessage = (TextMessage) smsConnection.newMessage(
            MessageConnection.TEXT_MESSAGE);
            txtmessage.setAddress(address);
            txtmessage.setPayloadText(message);
            
            smsConnection.send(txtmessage); //sending the message

        	Jout ("Your message to "+ cellPhoneNumber + " has been sent successfully"+ "\n");
        } 
        catch (Throwable t) 
        {
        	unsuccessfulNumbers.addElement (cellPhoneNumber);
        	Jout("Your message to "+cellPhoneNumber+" has failed"+"\n");
        }

  		if (smsConnection != null) 
        {
            try 
            {
                smsConnection.close();
            } 
            catch (IOException ioe) 
            {
            }                
        }
        
     }
     
     try
     {
     	success (); //reports to the client whether the server was able to send the sms to the desired number (s) or not
     }
     catch (Exception e)
     {
     }
 
  }
  
  /*
   * sends to the server a message indicating the number of the unsuccessful sms 's if any as well as the numbers 
   */
  public void success ()
  throws Exception
  {
  	out.write(unsuccessfulNumbers.size());
  	out.flush();
  	
  	for (int i = 0 ; i < unsuccessfulNumbers.size() ; i++)
  	{
  		out.writeUTF( (String)unsuccessfulNumbers.elementAt(i));
  		out.flush();
  	}
  }
  
  /*
   * utility for Displaying output 
   */
  public void Jout( String s )   //displays messages on the screen
  {
   f2.append (s);
   Display.getDisplay(midlet).setCurrent(f2);
  }
}
