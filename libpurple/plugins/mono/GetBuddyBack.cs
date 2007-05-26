using System;
using Purple;

public class GetBuddyBack : Plugin
{
	private static PluginInfo info = new PluginInfo("mono-buddyback", "C# Get Buddy Back", "0.1", "Prints when a Buddy returns", "Longer Description", "Eoin Coffey", "urled");

	public GetBuddyBack()
		: base (info)
	{
	}

	public void HandleSig(IntPtr p)
	{
		Buddy buddy = new Buddy(p);

		Debug.debug(Debug.INFO, "buddyback", "buddy " + buddy.Alias + " is back!\n");
	}
	
	public override void Load()
	{
		Debug.debug(Debug.INFO, "buddyback", "loading...\n");
		
		BuddyList.OnBuddyStatusChanged.connect(this.Handle, new Signal.Handler(HandleSig));
	}
	
	public override void Unload()
	{
	}
	
	public override void Destroy()
	{
	}
}
