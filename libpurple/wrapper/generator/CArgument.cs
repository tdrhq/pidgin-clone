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
            return GetCSharpFunction(CSharpFunctionType.Private, 0, false);
        }

        public string GetCSharpPrivateFunction(int argumentNumber)
        {
            return GetCSharpFunction(CSharpFunctionType.Private, argumentNumber, true);
        }

        public string GetCSharpPublicFunction()
        {
            return GetCSharpFunction(CSharpFunctionType.Public, 0, false);
        }

        public string GetCSharpPublicFunction(int argumentNumber)
        {
            return GetCSharpFunction(CSharpFunctionType.Public, argumentNumber, true);
        }

        private enum CSharpFunctionType { Public, Private };

        private string GetCSharpFunction(CSharpFunctionType functionType, int argumentNumber, bool argumentNumberSupplied)
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
                            result = this.CSharpPrivateType;
                            break;

                        case CSharpFunctionType.Public:
                            result = this.CSharpPublicType;
                            break;
                    }
                }

                if (this.SafeName == "")
                {
                    if (argumentNumberSupplied)
                        result += " _PurpleWrapper_arg" + argumentNumber;
                }
                else
                    result += " " + this.SafeName;

                return result;
            }

        }
    }
}
