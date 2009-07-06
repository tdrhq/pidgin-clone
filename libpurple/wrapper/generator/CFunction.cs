using System;
using System.Collections.Generic;

namespace Scripts
{
    class CFunction : CTyped
    {
        private List<CArgument> arguments = new List<CArgument>();
        private bool isFunctionPointer = false;

        public CFunction(CFile file, String returnType, String name)
            : base(file, returnType, name)
        {
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

        public bool IsFunctionPointer
        {
            get { return isFunctionPointer; }
            set { isFunctionPointer = true; }
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
            str += this.CSharpPrivateType + " " + this.SafeName + "(";

            for (int i = 0; i < this.Arguments.Count; i++)
            {
                if (i != 0)
                    str += ", ";

                str += this.Arguments[i].GetCSharpPrivateFunction();
            }

            str += ")";
            return str;
        }

        public string GetCSharpPublicFunction()
        {
            String str = "";

            String modifiedName = this.SafeName.ToLower();
            if (modifiedName.StartsWith("purple_"))
                modifiedName = modifiedName.Substring(7);
            if (modifiedName.StartsWith(this.File.FileNameAsClassName.ToLower() + "_"))
                modifiedName = modifiedName.Substring(this.File.FileNameAsClassName.Length + 1);

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
