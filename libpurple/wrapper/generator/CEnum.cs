using System;
using System.Collections.Generic;
using System.IO;

namespace Scripts
{
    class CEnum : CNamed
    {
        private List<KeyValuePair<string, string>> elements = new List<KeyValuePair<string, string>>();
        
        public CEnum(CFile file, String name)
            : base(file, name)
        {
        }


        public void AddElement(string name, string value)
        {
            elements.Add(new KeyValuePair<string, string>(name, value));
        }

        public override string ToString()
        {
            String str = "enum " + this.Name + " { ";

            for (int i = 0; i < elements.Count; i++)
            {
                String name = elements[i].Key;
                String value = elements[i].Value;
                
                str += name;
                if (value != "")
                    str += " = " + value;

                if (i != elements.Count - 1)
                    str += ", ";
                else
                    str += " ";
            }

            str += "};";

            return str;
        }

        public void WriteForCSharp(StreamWriter writer, string initalPrefix)
        {
            writer.WriteLine(initalPrefix + "public enum " + this.Name);
            writer.WriteLine(initalPrefix + "{");

            for (int i = 0; i < elements.Count; i++)
            {
                String name = elements[i].Key;
                String value = elements[i].Value;

                String str = initalPrefix + "\t" + name;
                if (value != "")
                    str += " = " + value;

                if (i != elements.Count - 1)
                    str += ",";

                writer.WriteLine(str);
            }

            writer.WriteLine(initalPrefix + "};");
            writer.WriteLine();
        }


    }
}
