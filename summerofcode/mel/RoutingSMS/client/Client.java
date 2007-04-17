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
import java.awt.*;
import javax.bluetooth.*;
import javax.microedition.io.*;   //for StreamConnection class
import java.util.*;
import java.lang.Object;

public class Client
{
	//application variables

	public static String number = "";	//to get the number entered by the user
	
	public static boolean importedPhonebook; //set to true after the client imports the phone book  
	public static boolean importingPhonebook; //set to true if the operation of importing the phonebook was successful otherwise set to false

 	public static Vector contactNumbers;   //holds the contacts number in phonebook
 	public static Vector contactNames;     //holds the contacts names in phonebook
	
	public static Vector unsuccessfulNumbers; // holds the numbers that were not successfuly sent
	public static boolean serverSendingResponse; //set to true when the server sends a message indicating the success of sending the sms
  	private StreamConnection streamConnection = null;
  	/*private*/ DataOutputStream out = null;
  	/*private*/ DataInputStream in = null;
  
  	private String connectionURL;
  	boolean connectionOpen;

  /**
   * Constructor initializes attributes.
   *
   */
  public Client() 
  {
  	//initializations

  	contactNumbers = new Vector();	
    contactNames = new Vector();
    unsuccessfulNumbers = new Vector ();  //unsuccessfully sent numbers
    
    importedPhonebook = false;
    /*
     *now it is capable of receiving address book, 
     *when it is set to false it indicates that an error occured 
     *while retrieving the phone book and so the client doesnt keep waiting for the server. 
     */
    importingPhonebook = true;    
    serverSendingResponse = false;
    connectionOpen = false;
  }
 
  /**
   * Starts the thread.
   */
   public void run_client (ServiceRecord r)
   {
 
    	String url  = r.getConnectionURL(ServiceRecord.AUTHENTICATE_NOENCRYPT, false );
    	
    	connectionURL = url;
    	
    	openConnection ();
    		
  		if (connectionOpen == true) 
  		{
  			//reading phonebook from the phone	
      		try
      		{
      			retrieveContacts ();
      		}
      		catch (Exception e)
      		{
      			importedPhonebook = false;   //still doesnt have the phone book
      			importingPhonebook = false;  //unable to import the phonebook
      			closeConnection ();
      		}
  		}
  		else
  		{
  			closeConnection ();
  		}
   }
  
  /*
   * Connects to the server and retrieves the address book 
   */
   public void retrieveContacts ()
   throws Exception
   {
   		String number, name;
		String number_name;
	
		number_name = in.readUTF();
	
      	while(!number_name.equals("EOF") ) //EOF indicates that the end of list was reached
      	{
      		if (number_name.equals ("EXCEPTION"))
      		{//this exception is raised from the phone side (for exmaple if the user refused to give permission to the application to access PIM library)
      			importedPhonebook = false;
      			importingPhonebook = false;
      			return;
      		}
      		
      		number = number_name.substring(0,number_name.indexOf(" "));
      		name = number_name.substring(number_name.indexOf(" ")+1,number_name.length());
      		
      		contactNumbers.addElement(number);
      		contactNames.addElement(name);
      		
  			Jout("read in data "+number_name+"");
      		
      		number_name = in.readUTF();
      	}
      	importedPhonebook = true;   //succeeded importing the phone book
	}
	/*
	 * connects to the server and sends the sms entered by the user 
	 * to the cell phone via bluetooth
	 */
	public void SMSroute (String message, int countRecipients, String[] desiredNumbers)
	throws Exception
	{
		unsuccessfulNumbers.removeAllElements(); //clears the vector bec of batch sending
		serverSendingResponse = false;

		//Thread.sleep(5000);
	
    	out.writeUTF (message);
    	out.flush();
  		out.write (countRecipients);   //the number of the recipients entered by the user
  		out.flush ();
    	
    	for (int i=0; i<countRecipients; i++)
    	{
    		out.writeUTF (desiredNumbers[i]);  //desired numbers entered by the user
    		out.flush ();
    	}
		try
		{
			success ();              //waits for server reply indicating its success or failure of sending the sms
		}
		catch (Exception e)
		{
			closeConnection ();
		}	
	}
	
	/*
	 * This method receives from the server a message indicates whether ir was successful 
	 * in sending the sms to the desired phone numbers
	 * if it failed to send the sms to any of the desired numbers,
	 * it notifies the client with those numbers to report to the user.
	 */
	public void success ()
	throws Exception 
	{
		String theUnsuccessful = null;
		int intSize;
		
		intSize = in.read ();
		
		if (intSize == 0)
		{
			serverSendingResponse = true;
			return;
		}
		else
		{
			for (int i = 0 ; i < intSize; i++)
			{
				theUnsuccessful = in.readUTF ();
				unsuccessfulNumbers.addElement(theUnsuccessful);  //vector of unsuccessful numbers
			}	
			serverSendingResponse = true;		
		}
	}
	
  
  /**
   * Initializes connection and streams between PC and Cellular phone.
   */
  public boolean openConnection() 
  {
    try 
    {
    	streamConnection = (StreamConnection) Connector.open(connectionURL);
      	out = streamConnection.openDataOutputStream();
      	in = streamConnection.openDataInputStream();  
    }
    catch (Exception e) 
    {
    	connectionOpen = false;
    	return false;
    }
    connectionOpen = true;
    return true;
  }

  
  /*
   *  Called when user disconnects connection or exception
   *  occurs during connection. Connection and streams between devices are closed.
   */
  
  public void closeConnection() 
  {
  	try 
    {
        in.close();
        out.close();
        streamConnection.close();
        connectionOpen = false;

     }
     catch (IOException ioe) 
     {
     }      
   }
  
  public void Jout (String s)
  {
  	System.out.println(s);
  }
  

}
