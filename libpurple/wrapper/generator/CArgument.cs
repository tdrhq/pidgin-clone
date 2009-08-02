using System;
using System.Collections.Generic;

namespace Scripts
{
    class CArgument : CTyped
    {
        private bool isEllipsis = false;
        private bool isFunctionPointer = false;
        public List<CArgument> functionPointerArguments = new List<CArgument>();
        int argumentNumber = -1;

        public CArgument(CFile file)
            : base(file, "", "")
        {
        }

        public CArgument(CFile file, String type, String name, int argumentNumber)
            : base(file, type, name)
        {
            this.ArgumentNumber = argumentNumber;
        }

        public int ArgumentNumber
        {
            get { return argumentNumber; }
            set { argumentNumber = value; }
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

        public string GetArgumentSafeName()
        {
            if (this.SafeName == "")
                return "_PurpleWrapper_arg" + this.ArgumentNumber;
            else
                return this.SafeName;
        }

        public string GetCSharpPrivateFunction()
        {
            return GetCSharpFunction(CSharpFunctionType.Private, null);
        }

        public string GetCSharpPrivateFunction(String functionName)
        {
            return GetCSharpFunction(CSharpFunctionType.Private, functionName);
        }

        public string GetCSharpPublicFunction()
        {
            return GetCSharpFunction(CSharpFunctionType.Public, null);
        }

        public string GetCSharpPublicFunction(String functionName)
        {
            return GetCSharpFunction(CSharpFunctionType.Public, functionName);
        }

        private enum CSharpFunctionType { Public, Private };

        // NEXT: Function Name
        private string GetCSharpFunction(CSharpFunctionType functionType, String functionName)
        {
            if (this.IsEllipsis)
                throw new UnableToCreateWrapperException("The function argument contains the ellipsis argument and cannot be automatically wrapped.");
            else
            {
                String result = "";

                if (this.IsFunctionPointer)
                    result = "IntPtr";
                else
                {
                    switch (functionType)
                    {
                        case CSharpFunctionType.Private:
                            result = this.CSharpInternalAPIType;
                            break;

                        case CSharpFunctionType.Public:
                            result = this.CSharpExternalAPIType;
                            break;
                    }
                }

                result += " " + this.GetArgumentSafeName();
                return result;
            }

        }
    }
}
