using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Scripts
{
    class CFile
    {
        private static List<CFile> fileColleciton = new List<CFile>();

        public static List<CFile> FileCollection
        {
            get { return fileColleciton; }
        }

        public static CNamed LookUpName(String name)
        {
            foreach (CFile file in CFile.FileCollection)
            {
                foreach (CStruct structure in file.Structs)
                {
                    if (structure.Name == name)
                    {
                        //Console.WriteLine("Type \"{0}\" is a struct.", name);
                        return structure;
                    }
                }

                foreach (CFunction function in file.Functions)
                {
                    if (function.Name == name)
                    {
                        //Console.WriteLine("Type \"{0}\" is a function.", name);
                        return function;
                    }
                }

                foreach (CEnum enumeration in file.Enums)
                {
                    if (enumeration.Name == name)
                    {
                        //Console.WriteLine("Type \"{0}\" is an enum.", name);
                        return enumeration;
                    }
                }

                foreach (CFunction function in file.FunctionPointers)
                {
                    if (function.Name == name)
                    {
                        //Console.WriteLine("Type \"{0}\" is a function pointer.", name);
                        return function;
                    }
                }
            }

            //Console.WriteLine("Type \"{0}\" is really UNKNOWN.", name);
            //Console.ReadLine();
            return null;
        }


        private String fileName;
        private List<CStruct> structs = new List<CStruct>();
        private List<CFunction> functions = new List<CFunction>();
        private List<CEnum> enums = new List<CEnum>();
        private List<CFunction> functionPointers = new List<CFunction>();

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

                /*
                 * This fixes the same file name for the xmlnode struct and the xmlnode.h file.
                 */ 
                if (finalName == "Xmlnode")
                    return "XMLNodeClass";

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

        public List<CFunction> FunctionPointers
        {
            get { return functionPointers; }
        }

        public List<CEnum> Enums
        {
            get { return enums; }
        }

        public void addStruct(CStruct structToAdd)
        {
            structs.Add(structToAdd);
            Logger.Log("Added structure [" + structToAdd.ToString() + "] to file [" + this.ToString() + "].");
        }

        public void addFunction(CFunction function)
        {
            function.File = this;
            functions.Add(function);
            Logger.Log("Added function [" + function.ToString() + "] to file [" + this.ToString() + "].");
        }

        public void addFunctionPointer(CFunction function)
        {
            functionPointers.Add(function);
            Logger.Log("Added function pointer [" + function.ToString() + "] to file [" + this.ToString() + "].");
        }

        public void AddEnum(CEnum enumeration)
        {
            enums.Add(enumeration);
            Logger.Log("Added enum [" + enumeration.ToString() + "] to file [" + this.ToString() + "].");
        }

        public override string ToString()
        {
            return this.FileName;
        }

    }
}
