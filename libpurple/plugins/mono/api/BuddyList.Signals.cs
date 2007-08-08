
namespace Purple {
	public partial class BuddyList {

		public static Event OnUpdateIdle = new Event(GetHandle(), "update-idle");
		public delegate void UpdateIdleHandle();

		public static Event OnBlistNodeAliased = new Event(GetHandle(), "blist-node-aliased");
		public delegate void BlistNodeAliasedHandle(BlistNode arg1, string arg2);

		public static Event OnBuddyGotLoginTime = new Event(GetHandle(), "buddy-got-login-time");
		public delegate void BuddyGotLoginTimeHandle(Buddy arg1);

		public static Event OnBuddyAdded = new Event(GetHandle(), "buddy-added");
		public delegate void BuddyAddedHandle(Buddy arg1);

		public static Event OnBuddySignedOff = new Event(GetHandle(), "buddy-signed-off");
		public delegate void BuddySignedOffHandle(Buddy arg1);

		public static Event OnBuddySignedOn = new Event(GetHandle(), "buddy-signed-on");
		public delegate void BuddySignedOnHandle(Buddy arg1);

		public static Event OnBuddyRemoved = new Event(GetHandle(), "buddy-removed");
		public delegate void BuddyRemovedHandle(Buddy arg1);

		public static Event OnBuddyStatusChanged = new Event(GetHandle(), "buddy-status-changed");
		public delegate void BuddyStatusChangedHandle(Buddy arg1, Status arg2, Status arg3);

		public static Event OnBuddyPrivacyChanged = new Event(GetHandle(), "buddy-privacy-changed");
		public delegate void BuddyPrivacyChangedHandle(Buddy arg1);

		public static Event OnBuddyIdleChanged = new Event(GetHandle(), "buddy-idle-changed");
		public delegate void BuddyIdleChangedHandle(Buddy arg1, int arg2, int arg3);

		public static Event OnBuddyIconChanged = new Event(GetHandle(), "buddy-icon-changed");
		public delegate void BuddyIconChangedHandle(Buddy arg1);

	}
}
