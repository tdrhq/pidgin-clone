using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Penguin
{
    public partial class WelcomeForm : Form
    {
        public WelcomeForm()
        {
            /*  */
            InitializeComponent();

            
            /* libpurple init */
            PurpleWrapper.Debug.SetEnabled(true);
            PurpleWrapper.Plugin.PluginsAddSearchPath(System.IO.Directory.GetCurrentDirectory());
            labelPurpleVersion.Text = PurpleWrapper.Core.GetVersion();
        }
    }
}
