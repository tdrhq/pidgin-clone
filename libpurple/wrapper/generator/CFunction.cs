using System;
using System.Collections.Generic;

namespace Scripts
{
    class CFunction : CTyped
    {
        private String name;
        private List<CArgument> arguments = new List<CArgument>();

        public CFunction(String returnType, String name)
        {
            this.name = name;
            this.Type = returnType;
        }

        public String Name
        {
            get { return name; }
            set { name = value; }
        }




        public void AddArgument(CArgument argument)
        {
            arguments.Add(argument);
        }

        public List<CArgument> Arguments
        {
            get
            {
                return arguments;
            }
        }

        public override string ToString()
        {
            String str = this.Type + " " + this.Name + "(";

            for (int i = 0; i < this.Arguments.Count; i++)
            {
                if (i != 0)
                    str += ", ";
                
                str += this.Arguments[i].ToString();
            }

            str += ")";
            return str;
        }

        public string GetCSharpPrivateFunction()
        {
            String str = "";
            str += this.CSharpPrivateType + " " + this.Name + "(";

            for (int i = 0; i < this.Arguments.Count; i++)
            {
                if (i != 0)
                    str += ", ";

                str += this.Arguments[i].GetCSharpPrivateFunction();
            }

            str += ")";
            return str;
        }

        public string GetCSharpPublicFunction(String className)
        {
            String str = "";

            String modifiedName = this.Name.ToLower();
            if (modifiedName.StartsWith("purple_"))
                modifiedName = modifiedName.Substring(7);
            if (modifiedName.StartsWith(className.ToLower() + "_"))
                modifiedName = modifiedName.Substring(className.Length + 1);

            String finalName = "";
            finalName += Char.ToUpper(modifiedName[0]);
            for (int i = 1; i < modifiedName.Length; i++)
            {
                if (modifiedName[i] == '_' && i != modifiedName.Length - 1)
                {
                    finalName += Char.ToUpper(modifiedName[i + 1]);
                    i++;
                }
                else
                {
                    finalName += modifiedName[i];
                }
            }



            str += this.CSharpPublicType + " " + finalName + "(";

            for (int i = 0; i < this.Arguments.Count; i++)
            {
                if (i != 0)
                    str += ", ";

                str += this.Arguments[i].GetCSharpPublicFunction();
            }

            str += ")";
            return str;
        }
    }
}
