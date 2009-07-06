using System;
using System.Collections.Generic;

namespace Scripts
{
    class CStruct : CNamed
    {
        private List<CArgument> fields = new List<CArgument>();

        public CStruct(CFile file, String name) :
            base(file, name)
        {
            
        }

        public List<CArgument> Fields
        {
            get { return fields; }
        }

        public void AddField(CArgument argument)
        {
            fields.Add(argument);
        }

        public override string ToString()
        {
            String str = "struct " + this.Name + " { ";

            foreach (CArgument argument in this.Fields)
                str += argument.ToString() + "; ";

            str += "};";

            return str;
        }
    }
}
