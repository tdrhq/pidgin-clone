using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Scripts
{
    abstract class CNamed
    {
        private String name;
        private CFile file;

        public CNamed(CFile file, String name)
        {
            this.File = file;
            this.Name = name;
        }

        public String Name
        {
            get
            {
                return name;
            }
            set
            {
                if (value.Length > 0 && value[0] == '_')
                    name = value.Substring(1);
                else
                    name = value;
            }
        }

        public String SafeName
        {
            get
            {
                if (this.Name == "in" || this.Name == "event")
                    return this.Name + "_";
                else
                    return this.Name;
            }
        }

        public CFile File
        {
            get { return file; }
            set { file = value; }
        }

    }
}
