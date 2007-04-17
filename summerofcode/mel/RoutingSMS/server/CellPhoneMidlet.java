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
import javax.microedition.lcdui.*;
import javax.microedition.midlet.*;

public class CellPhoneMIDlet extends MIDlet implements CommandListener 
{

  /**
   * menulist, main menu List object.
   */
  public List menulist;
  private String title;

  /**
   * elements, String elements of the menu.
   */
  private String[] elements;

  /**
   * server, Server object is created if this MIDlet is launced as Server.
   */
  private Server server;

  /**
   * UUIDString, Service record UUID that is used in this example application.
   * Both Client and Server retrives this UUID string from this MIDlet class.
   */
  public static final String UUIDString = "11223344556677889900aabbccddeeff";

  /**
   * CellPhoneMIDlet
   * Constructor. Initialize main menu;
   */
  public CellPhoneMIDlet() 
  {

    title = "Main menu";
    elements = new String[3];
    elements[0] = "Start Application";
    elements[1] = "About";
    elements[2] = "Exit";
    
    menulist = new List(title, List.IMPLICIT, elements, null);

    menulist.setCommandListener(this);

  }

  /**
   * startApp Shows main menu
   *
   * @throws MIDletStateChangeException
   */
  protected void startApp() 
  throws MIDletStateChangeException 
  {
    showMenu();
  }

  protected void pauseApp() 
  {
  }

  /**
   * destroyApp. Ensures that connections have been closed.
   *
   * param p1 boolean
   */
  protected void destroyApp(boolean p1) 
  {
    if (server != null) 
    {
      server.applicationClosed = true;
      server.closeConnection();
    }
    
  }

  /**
   * commandAction. Handles main menu command events.
   *
   * param c Command
   * param d Displayable
   */
  public void commandAction(Command c, Displayable d) 
  {
    if (d == menulist) 
    {
      int index = menulist.getSelectedIndex();
      switch (index) 
      {
        case 0:
          startServer();
          break;
        case 1:
          showAbout();
          break;
        case 2:
          quitApp();
          break;

        default:
          break;
      }
    }
  }

  /*
   * quitApp. Called when application is closed.
   */
  public void quitApp() 
  {
   	destroyApp(false);
    notifyDestroyed();
  }

  /*
   * showMenu. Shows main menu list.
   */
  public void showMenu() 
  {
    Display.getDisplay(this).setCurrent(menulist);
  }

  /*
   * showAbout. Shows About dialog.
   */
  public void showAbout() 
  {
    AboutForm aboutForm = new AboutForm("About SMS Routing over BT Link", this);
    Display.getDisplay(this).setCurrent(aboutForm);
  }

  /**
   * startServer. Starts bluetooth server thread.
   */
  private void startServer() 
  {
    server = new Server(this);
    server.start();
  }
}
