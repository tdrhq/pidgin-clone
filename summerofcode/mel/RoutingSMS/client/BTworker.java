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
import java.util.*;
import javax.bluetooth.*;

import javax.bluetooth.LocalDevice;

public class BTworker
{
  
  public static Vector devices = new Vector();  //to hold the remote devices discovered
  public static Vector deviceClasses = new Vector();
  
  public static Vector services = new Vector(); //to hold services discovered
  
  public static int selectedDevice = -1;      //the index of the remote device, selected by the user,in the vector

  //since device discovery and service discovery methods I used are non blocking 
  // these 2 variables will be needed for synchronization
  
  public static boolean deviceWaiting = true;  
  public static boolean serviceWaiting = true;

  public static BTworker instance;	

  public UUID[] serviceUUIDs = null;   // list of UUID to match during service discovery

  private LocalDevice device;   //PC 
  private DiscoveryAgent agent;
  
  
  public BTworker()
  {
  	instance = this;
  	serviceUUIDs = new UUID[1];
    serviceUUIDs[0] = new UUID(PCapplication.UUIDString, false);
  }

  /**
   * Function to write message on the console.
   * 
   */
  
  public static void Jout(String s)
  {
    System.out.println(s);
  }
  
  /**
   *initialize the JABWT stack
   *
   */ 
  public void initializeJABWTStack ()
  {
	  try
	  { 
	  	// obtain reference to singleton	  	     
      	device = LocalDevice.getLocalDevice();
      
      	//in order to allow bluetooth devices to find your device
	  	//GIAC to allow all other bt devices discover me.	
      	device.setDiscoverable(DiscoveryAgent.GIAC); 
      
      	// obtain reference to singleton   
      	agent = device.getDiscoveryAgent(); 
      }
      catch (BluetoothStateException e)
      {
      	Jout ("Exception in initaializing stack");
      }

  }
  
  /**
   * Get the first discovered service from selected remote device,
   * since the UUID that matches the start inquiry was placed at the begining
   * of the array. This will return the first service that matches my
   * UUIDs in startInquiry().
   * connects to the RemoteDevice where this ServiceRecord originated
   * return ServiceRecord null if no service discovered
   */
  
  public ServiceRecord getOneDiscoveredService()
  {
    if ( services.size() > 0 )
      return (ServiceRecord) services.elementAt(0);
    else
      return null;
  }

  /** 
   * Return user selected remote device that is used for service discovery.
   * return RemoteDevice null if user didn't select anything
   */
  public RemoteDevice getSelectedDevice()
  {
    if ( selectedDevice != -1 )
      return (RemoteDevice) devices.elementAt(selectedDevice);
    else
      return null;
  }

  /**
   * Start device discovery. My application calls this method to do device discovery
   */
  public void startDeviceInquiry()
  {
    try
    {
      deviceWaiting = true;
      devices.removeAllElements();        //clears previous values first
      deviceClasses.removeAllElements(); //clears previous values first

	  initializeJABWTStack ();

      boolean result = agent.startInquiry( DiscoveryAgent.GIAC, new Listener() );

      Jout ("please wait " + "\n");

    } 
    catch ( BluetoothStateException e )
    {
      e.printStackTrace();
    }

  }
/*
 ** Start service discovery. My application calls this method to do service discovery
 **/
  public void startServiceInquiry ()
  { 	
    serviceWaiting = true;
    services.removeAllElements(); //clears previous values first

    try
    {
    	//Searches for services on a remote Bluetooth device that have all the UUIDs specified in uuidSet
       	agent.searchServices(null,serviceUUIDs,this.getSelectedDevice(),new Listener() );
    } 
    catch (BluetoothStateException ex)
    {
      ex.printStackTrace();
    }

  }
  /*
   * Method that checks whether the server is still available and the client is connected to or not.
   * it returns true if the connection is still present and false if the connection was lost 
   * The aim of this method is to ensure that the application terminates norally without raising
   * any exceptions 
   */
  public boolean checkCellularPhoneConnectivity()
  {
  	try
    {
    	startServiceInquiry ();
    	while (serviceWaiting == true)
    		Thread.sleep (1000);
    	if (services.size ()== 0 )
    	{
        	return false;  
    	}
    }
    catch (Exception e)
    {
    	return false;
    }
    
    return true;
  } 
 
  /**
   * Bluetooth listener object.
   * Register this listener object to DiscoveryAgent in device inqury and service discovery.
   */
  class Listener implements DiscoveryListener
  {
  	/*
	 * called  each time a device is discovered
	 */
  	public void deviceDiscovered(RemoteDevice remoteDevice, DeviceClass deviceClass)
    {
	   	Jout("A remote Bluetooth device is discovered:");
	
		devices.addElement( remoteDevice );
		deviceClasses.addElement( deviceClass );
	}
	
	/*
	 * called when device discovery is finished
	 */
	public void inquiryCompleted(int complete)
	{
		if ( devices.size() == 0 )
		{
			Jout ("No Bluetooth device found");
		} 
	    else
	    {
	    	System.out.println("finished searching");
		    
		}
		deviceWaiting = false;
	}
	/*
	 * called  each time a service is discovered
	 */
	public void servicesDiscovered(int transId, ServiceRecord[] records)
	{
		for ( int i=0; i< records.length;  i ++ )
		{
			ServiceRecord record = records[i];
		    services.addElement( record );
		}
	}
	/*
	 * called when service discovery is finished
	 */	
	public void serviceSearchCompleted(int transId, int complete)
	{
	   Jout(""+services.size()+" services are discovered");
	   serviceWaiting = false;
    }

  } // Listener

}
