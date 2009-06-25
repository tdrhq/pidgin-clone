using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Scripts
{
    class CFile
    {
        private String fileName;
        private List<CStruct> structs = new List<CStruct>();
        private List<CFunction> functions = new List<CFunction>();

        public CFile(String fileName)
        {
            this.fileName = fileName;
        }

        public String FileName
        {
            get { return fileName; }
            set { fileName = value; }
        }

        public String FileNameAsClassName
        {
            get
            {
                String name;
                if (this.FileName.Contains("."))
                    name = this.FileName.Substring(0, this.FileName.IndexOf("."));
                else
                    name = this.FileName;
                name = name.ToLower();


                String finalName = "";
                finalName += Char.ToUpper(name[0]);

                for (int i = 1; i < name.Length; i++)
                {
                    if ( (name[i] == '_' || name[i] == '-') && i != name.Length - 1)
                    {
                        finalName += Char.ToUpper(name[i + 1]);
                        i++;
                    }
                    else
                        finalName += name[i];
                }

                return finalName;
            }
        }

        public List<CStruct> Structs
        {
            get { return structs; }
        }

        public List<CFunction> Functions
        {
            get { return functions; }
        }

        public void addStruct(CStruct structToAdd)
        {
            structs.Add(structToAdd);
        }

        public void addFunction(CFunction function)
        {
            functions.Add(function);
        }
    }
}
