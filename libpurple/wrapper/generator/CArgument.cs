using System;
using System.Collections.Generic;

namespace Scripts
{
    class CArgument : CTyped
    {
        private bool isEllipsis = false;
        private bool isFunctionPointer = false;
        public List<CArgument> functionPointerArguments = new List<CArgument>();

        public CArgument(CFile file)
            : base(file, "", "")
        {
        }

        public CArgument(CFile file, String type, String name)
            : base(file, type, name)
        {
        }

        public bool IsEllipsis
        {
            get { return isEllipsis; }
            set { isEllipsis = value; }
        }

        public bool IsFunctionPointer
        {
            get { return isFunctionPointer; }
            set { isFunctionPointer = value; }
        }

        public List<CArgument> FunctionPointerArguments
        {
            get { return functionPointerArguments; }
        }

        public void AddFunctionPointerArgument(CArgument argument)
        {
            functionPointerArguments.Add(argument);
        }

        public override string ToString()
        {
            if (this.IsEllipsis)
                return "...";
            else if (this.IsFunctionPointer)
            {
                String str = this.Type + " (*" + this.Name + ")(";

                for (int i = 0; i < this.FunctionPointerArguments.Count; i++)
                {
                    if (i != 0)
                        str += ", ";

                    str += this.FunctionPointerArguments[i].ToString();
                }
                str += ")";

                return str;
            }
            else
                return this.Type + " " + this.Name;
        }

        public string GetCSharpPrivateFunction()
        {
            if (this.IsEllipsis)
                throw new UnableToCreateWrapperException("The function argument contains the ellipsis argument and cannot be automatically wrapped.");
            else
                return this.CSharpPrivateType + " " + this.SafeName;
        }

        public string GetCSharpPublicFunction()
        {
            if (this.IsEllipsis)
                throw new UnableToCreateWrapperException("The function argument contains the ellipsis argument and cannot be automatically wrapped.");
            else
            {
                return this.CSharpPublicType + " " + this.SafeName;
            }
        }
    }
}
