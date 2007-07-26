using System;
using Purple;

public class GetBuddyBack : Plugin
{
	private static PluginInfo info = new PluginInfo("mono-buddyback", "C# Get Buddy Back", "0.1", "Prints when a Buddy returns", "Longer Description", "Eoin Coffey", "urled");

	public GetBuddyBack()
		: base (info)
	{
	}

	public void HandleSig(Buddy buddy, Status oldstatus, Status newstatus)
	{
		//Debug.debug(Debug.INFO, "buddyback", "Caught signal!\n");
		Debug.debug(Debug.INFO, "buddyback", "buddy " + buddy.Alias + " went from " + oldstatus.Id + " to " + newstatus.Id + "\n");
		Debug.debug(Debug.INFO, "buddyback", "account: " + buddy.PAccount.Username + ", " + buddy.PAccount.ProtocolId + "\n");
	}
	
	public override void Load()
	{
		Debug.debug(Debug.INFO, "buddyback", "loading...\n");
		
		//BuddyList.OnBuddyStatusChanged.connect(this.Handle, new Signal.VOID__POINTER_POINTER_POINTER(HandleSig));
		BuddyList.OnBuddyStatusChanged.connect(this.Handle, new BuddyList.BuddyStatusChangedHandle(HandleSig));
	}
	
	public override void Unload()
	{
	}
	
	public override void Destroy()
	{
	}
}
