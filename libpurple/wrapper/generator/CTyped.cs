using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Scripts
{
    class CTyped : CNamed
    {
        private string type;
        private bool isArray;
        private int arraySize;

        public enum TypeCategory
        {
            Void,
            DateTime,
            Native,
            PointerToKnownStruct,
            VoidPointer,
            GObjectObject,
            InnerStruct,
            PointerToUnknownStruct,
            Unknown,
            KnownEnum,
            KnownFunctionPointer,
            KnownFunction,
            KnownStruct,
            KnownArgument,
            PointerToGList,
            PointerToGHashTable,
            PointerToGSList
        };


        public CTyped(CFile file, String type, String name)
            : base(file, name)
        {
            this.Type = type;

            if (name.Contains("["))
            {
                this.ArraySize = int.Parse(name.Substring(name.IndexOf("[") + 1, name.IndexOf("]") - name.IndexOf("[") - 1));
                this.IsArray = true;
                this.Name = name.Substring(0, name.IndexOf("["));
            }

            if (name.EndsWith("[]"))
            {
                name = name.Substring(0, name.Length - 2);
                this.IsArray = true;
                this.ArraySize = -1;
            }
        }

        public int ArraySize
        {
            get { return arraySize; }
            set { arraySize = value; }
        }

        public String Type
        {
            get { return type; }
            set { type = value; }
        }

        public bool IsArray
        {
            get { return isArray; }
            set { isArray = value; }
        }

        public String RawType
        {
            get
            {
                String fullType = this.Type;

                if (fullType.StartsWith("const"))
                    fullType = fullType.Substring(("const").Length).Trim();
                
                if (fullType.StartsWith("unsigned"))
                    fullType = fullType.Substring(("unsigned").Length).Trim();

                if (fullType.Contains(" "))
                    return fullType.Substring(0, fullType.IndexOf(" "));
                else
                    return fullType;
            }
        }

        public bool IsTypePointer
        {
            get
            {
                if (this.Type != null && (this.Type.EndsWith("*") || this.Type == "gpointer" || this.Type == "gconstpointer"))
                    return true;
                else
                    return false;
            }
        }

        public bool IsTypeString
        {
            get
            {
                if (this.NativeType == "string")
                    return true;
                else
                    return false;
            }
        }

        public bool IsTypeNative
        {
            get
            {
                if (this.NativeType == null)
                    return false;
                else
                    return true;
            }
        }

        public bool IsTypeVoid
        {
            get
            {
                if (this.Type == "void")
                    return true;
                else
                    return false;
            }
        }

        public bool IsTypeTime
        {
            get
            {
                if (this.Type == "time_t")
                    return true;
                else
                    return false;
            }
        }

        public CNamed ResolvedType
        {
            get
            {
                return CFile.LookUpName(this.RawType);
            }
        }

        public TypeCategory Category
        {
            get
            {
                if (this.IsTypeNative)
                    return TypeCategory.Native;
                else if (this.Type == "void *" || this.Type == "gpointer" || this.Type == "gconstpointer")
                    return TypeCategory.VoidPointer;
                else if (this.IsTypeVoid)
                    return TypeCategory.Void;
                else if (this.IsTypeTime)
                    return TypeCategory.DateTime;
                else if (this.Type == "GObject" || this.Type == "GObjectClass" || this.Type == "GType" || this.Type == "GCallback" || this.Type == "GSourceFunc")
                    return TypeCategory.GObjectObject;
                else if (this.Type == "struct" || this.Type == "union")
                    return TypeCategory.InnerStruct;
                else if (this.Type == "GList *")
                    return TypeCategory.PointerToGList;
                else if (this.Type == "GHashTable *")
                    return TypeCategory.PointerToGHashTable;
                else if (this.Type == "GSList *")
                    return TypeCategory.PointerToGSList;
                else
                {
                    CNamed resolvedType = this.ResolvedType;

                    if (resolvedType != null)
                    {
                        if (resolvedType is CEnum)
                            return TypeCategory.KnownEnum;
                        else if (resolvedType is CArgument)
                            return TypeCategory.KnownArgument;
                        else if (resolvedType is CFunction)
                        {
                            if (((CFunction)resolvedType).IsFunctionPointer)
                                return TypeCategory.KnownFunctionPointer;
                            else
                                return TypeCategory.KnownFunction;
                        }
                        else if (resolvedType is CStruct)
                        {
                            if (this.IsTypePointer)
                                return TypeCategory.PointerToKnownStruct;
                            else
                                return TypeCategory.KnownStruct;
                        }
                        else
                            return TypeCategory.Unknown;
                    }
                    else
                    {
                        if (this.IsTypePointer)
                            return TypeCategory.PointerToUnknownStruct;
                        else
                            return TypeCategory.Unknown;
                    }
                }


            }
        }

        public String CSharpInternalAPIType
        {
            get
            {
                String str = "";

                switch (this.Category)
                {
                    case TypeCategory.Native:
                        str = this.NativeType;
                        break;

                    case TypeCategory.PointerToKnownStruct:
                    case TypeCategory.PointerToUnknownStruct:
                    case TypeCategory.VoidPointer:
                    case TypeCategory.GObjectObject:
                    case TypeCategory.KnownFunctionPointer:
                    case TypeCategory.PointerToGSList:
                    case TypeCategory.PointerToGList:
                    case TypeCategory.PointerToGHashTable:
                        str = "IntPtr";
                        break;

                    case TypeCategory.Void:
                        str = "void";
                        break;

                    case TypeCategory.DateTime:
                        str = "ulong";
                        break;

                    case TypeCategory.InnerStruct:
                        throw new UnableToCreateWrapperException("The struct contains an inner-struct or inner-union and cannot be automatically wrapped.");
                        break;

                    case TypeCategory.KnownStruct:
                        throw new UnableToCreateWrapperException("The struct contains an object that resolves to a function that is not a function pointer (" + this.ToString() + ").");
                        break;

                    case TypeCategory.KnownEnum:
                        {
                            CEnum resolvedEnum = (CEnum)this.ResolvedType;
                            str = resolvedEnum.File.FileNameAsClassName + "." + resolvedEnum.Name;
                        }
                        break;

                    default:
                        str = "/* Unresolved: [Category: " + this.Category.ToString() + "], [Name: " + this.ToString() + "] */ IntPtr";
                        //throw new UnableToCreateWrapperException("The type could not be resolved (" + this.ToString() + ").");
                        break;
                }

                if (this.IsArray)
                    str += "[]";

                return str;
            }
        }

        private enum APIType { External, Internal };

        private string GetCSharpAPIType(APIType apiType, string functionName, int argumentNumber)
        {
            if (this.Category == TypeCategory.PointerToGSList ||
                this.Category == TypeCategory.PointerToGList ||
                this.Category == TypeCategory.PointerToGHashTable)
            {
                string returnStructureInnerValueType = PurpleTranslations.GetListElementType(functionName, argumentNumber);

                // TODO: ManagedWrappers

                if (returnStructureInnerValueType == null)
                    throw new UnableToCreateWrapperException("Unknown inner-type of argument " + argumentNumber + " of " + functionName);

                if (this.Category == TypeCategory.PointerToGSList)
                {
                    if (apiType == APIType.External)
                        return "IList<" + returnStructureInnerValueType + ">";
                    else
                        return "GSList<" + returnStructureInnerValueType + ">";
                }
                else if (this.Category == TypeCategory.PointerToGList)
                {
                    if (apiType == APIType.External)
                        return "IList<" + returnStructureInnerValueType + ">";
                    else
                        return "GSList<" + returnStructureInnerValueType + ">";
                }
                else
                {
                    if (apiType == APIType.External)
                        return "IDictionary<string, " + returnStructureInnerValueType + ">";
                    else
                        return "GHashTable<" + returnStructureInnerValueType + ">";
                }
            }
            else
            {
                if (apiType == APIType.External)
                    return this.CSharpExternalAPIType;
                else
                    return this.CSharpInternalAPIType;
            }
        }

        public string GetCSharpExternalAPIType(string functionName, int argumentNumber)
        {
            return GetCSharpAPIType(APIType.External, functionName, argumentNumber);
        }


        public String CSharpExternalAPIType
        {
            get
            {
                String str;

                switch (this.Category)
                {
                    case TypeCategory.Native:
                        str = this.NativeType;
                        break;

                    case TypeCategory.VoidPointer:
                        str = "IntPtr";
                        break;

                    case TypeCategory.Void:
                        str = "void";
                        break;

                    case TypeCategory.DateTime:
                        str = "DateTime";
                        break;

                    case TypeCategory.GObjectObject:
                        str = "/* libobject */ IntPtr";
                        break;

                    case TypeCategory.InnerStruct:
                        throw new UnableToCreateWrapperException("The struct contains an inner-struct or inner-union and cannot be automatically wrapped.");
                        break;

                    case TypeCategory.KnownStruct:
                        throw new UnableToCreateWrapperException("The struct contains an object that resolves to a function that is not a function pointer (" + this.ToString() + ").");
                        break;

                    case TypeCategory.PointerToKnownStruct:
                        str = this.ResolvedType.SafeName;
                        break;

                    case TypeCategory.PointerToUnknownStruct:
                        str = "/* Resolved as PointerToUnknownStruct to " + this.RawType + " */ IntPtr";
                        break;

                    case TypeCategory.KnownEnum:
                        {
                            CEnum resolvedEnum = (CEnum)this.ResolvedType;
                            str = resolvedEnum.File.FileNameAsClassName + "." + resolvedEnum.SafeName;
                        }
                        break;

                    case TypeCategory.KnownFunctionPointer:
                        {
                            CFunction resolvedFunctionPointer = (CFunction)this.ResolvedType;
                            str = resolvedFunctionPointer.File.FileNameAsClassName + "." + resolvedFunctionPointer.SafeName;
                        }
                        break;

                    case TypeCategory.PointerToGSList:
                        throw new UnableToCreateWrapperException("Unable to provide a type for GSList as no function information was provided in code.");
                        break;

                    case TypeCategory.PointerToGList:
                        throw new UnableToCreateWrapperException("Unable to provide a type for GList as no function information was provided in code.");
                        break;

                    case TypeCategory.PointerToGHashTable:
                        throw new UnableToCreateWrapperException("Unable to provide a type for HashTable as no function information was provided in code.");
                        break;

                    default:
                        str = "/* Unresolved: [Category: " + this.Category.ToString() + "], [Name: " + this.ToString() + "] */ IntPtr";
                        //throw new UnableToCreateWrapperException("The type could not be resolved (" + this.ToString() + ").");
                        break;
                }

                if (this.IsArray)
                    str += "[]";

                return str;
            }
        }

        public String NativeType
        {
            get
            {
                if (this.Type == "char" && this.IsArray)
                    return "string";

                switch (this.Type)
                {
                    case "gboolean":
                        return "bool";

                    case "gchar":
                    case "char":
                    case "int8":
                        return "char";

                    case "uint8":
                    case "unsigned char":
                    case "guchar":
                        return "byte";

                    case "gchar *":
                    case "char *":
                        return "string";

                    case "gint":
                    case "int":
                    case "gint32":
                    case "PurplePluginPriority":   /* by typedef */
                        return "int";

                    case "guint":
                    case "unsigned int":
                    case "guint32":
                    case "PurpleCmdId":            /* by typedef */
                        return "uint";

                    case "gshort":
                    case "short":
                    case "gint16":
                        return "short";

                    case "gushort":
                    case "unsigned short":
                    case "guint16":
                        return "ushort";

                    case "glong":
                    case "long":
                    case "gint64":
                    case "gssize":
                    case "goffset":
                        return "long";

                    case "gulong":
                    case "unsigned long":
                    case "guint64":
                    case "gsize":
                        return "ulong";

                    case "gfloat":
                    case "float":
                        return "float";

                    case "gdouble":
                    case "double":
                        return "double";

                    case "size_t":
                        if (IntPtr.Size == 4) return "uint";
                        else return "ulong";
                }

                return null;
            }
        }

        public override string ToString()
        {
            return "Type: " + this.Type + ", Name: " + this.Name;
        }
    }
}
