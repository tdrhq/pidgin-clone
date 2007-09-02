using System;
using System.Runtime.CompilerServices;

namespace Purple {
	public class PluginInfo {
		private string id;
		private string name;
		private string version;
		private string summary;
		private string description;
		private string author;
		private string homepage;
		
		public PluginInfo(string id, string name, string version, string summary,
						  string description, string author, string homepage)
		{
			this.id = id;
			this.name = name;
			this.version = version;
			this.summary = summary;
			this.description = description;
			this.author = author;
			this.homepage = homepage;	
		}

		public string Id {
			get { return id; }
		}

		public string Name {
			get { return name; }
		}

		public string Version {
			get { return version; }
		}

		public string Summary {
			get { return summary; }
		}

		public string Description {
			get { return description; }
		}

		public string Author {
			get { return author; }
		}

		public string Homepage {
			get { return homepage; }
		}
	}
	
	abstract public class Plugin {
		private PluginInfo info;

		public Plugin(PluginInfo info) {
			this.info = info;
		}

		public abstract void Load();
		public abstract void Unload();
		public abstract void Destroy();

		public PluginInfo Info {
			get { return info; }
		}

		public IntPtr Handle {
			get {
				return _plugin_get_handle(this);
			}
		}

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		private static extern IntPtr _plugin_get_handle(object plugin);
	}
}
