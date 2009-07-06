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
            KnownArgument
        };


        public CTyped(CFile file, String type, String name)
            : base(file, name)
        {
            this.Type = type;
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

        public String CSharpPrivateType
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
                        str = "IntPtr";
                        break;

                    case TypeCategory.Void:
                        str = "void";
                        break;

                    case TypeCategory.DateTime:
                        str = "ulong";
                        break;

                    case TypeCategory.InnerStruct:
                        throw new UnableToCreateWrapperException("The struct contains an inner-struct or innter-union and cannot be automatically wrapped.");
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
                        throw new UnableToCreateWrapperException("The type could not be resolved (" + this.ToString() + ").");
                        break;
                }

                /*
                if (this.IsTypeNative)
                    str = this.NativeType;
                else if (this.IsTypePointer)
                    str = "IntPtr";
                else if (this.IsTypeVoid)
                    str = "void";
                else if (this.IsTypeTime)
                    str = "ulong";
                else if (this.Type == "GObject" || this.Type == "GObjectClass" || this.Type == "GType" || this.Type == "GCallback" || this.Type == "GSourceFunc")
                    str = "IntPtr";  // TODO: Find out if this is the correct typecast (or even if this works)...
                else if (this.Type == "struct" || this.Type == "union")
                    throw new UnableToCreateWrapperException("The struct contains an inner-struct or innter-union and cannot be automatically wrapped.");
                else
                {
                    CNamed resolvedType = CFile.LookUpName(this.Type);

                    if (resolvedType is CFunction)
                    {
                        if (((CFunction)resolvedType).IsFunctionPointer)
                            str = "IntPtr";
                        else
                            throw new UnableToCreateWrapperException("The struct contains an object that resolves to a function that is not a function pointer (" + this.ToString() + ").");
                    }
                    else if (resolvedType is CEnum)
                    {
                        CEnum resolvedEnum = (CEnum)resolvedType;
                        str = resolvedEnum.File.FileNameAsClassName + "." + resolvedEnum.Name;
                    }
                    else
                        throw new UnableToCreateWrapperException("The type could not be resolved (" + this.ToString() + ").");
                }
                */

                if (this.IsArray)
                    str += "[]";

                return str;
            }
        }

        public String CSharpPublicType
        {
            get
            {
                String str;

                if (this.IsTypeNative)
                    str = this.NativeType;
                else if (this.Type == "void *" || this.Type == "gpointer" || this.Type == "gconstpointer")
                    str = "IntPtr";
                else if (this.IsTypeTime)
                    str = "DateTime";
                else if (this.IsTypeVoid)
                    str = "void";
                else if (this.Type == "GObject" || this.Type == "GObjectClass" || this.Type == "GType" || this.Type == "GCallback" || this.Type == "GSourceFunc")
                    str = "/* libgobject */ IntPtr";  // TODO: Find out if this is the correct typecast (or even if this works)...
                else if (this.Type == "struct" || this.Type == "union")
                    throw new UnableToCreateWrapperException("The struct contains an inner-struct or innter-union and cannot be automatically wrapped.");
                else
                {
                    CNamed resolvedType = CFile.LookUpName(this.RawType);

                    if (resolvedType is CFunction)
                    {
                        if (((CFunction)resolvedType).IsFunctionPointer)
                            throw new UnableToCreateWrapperException("The struct contains an object that resolves to a function pointer (" + this.ToString() + ").");
                        else
                            throw new UnableToCreateWrapperException("The struct contains an object that resolves to a function that is not a function pointer (" + this.ToString() + ").");
                    }
                    else if (resolvedType is CEnum)
                    {
                        CEnum resolvedEnum = (CEnum)resolvedType;
                        str = resolvedEnum.File.FileNameAsClassName + "." + resolvedEnum.Name;
                    }
                    else if (resolvedType is CStruct)
                    {
                        if (this.IsTypePointer)
                            str = this.RawType;
                        else
                            throw new UnableToCreateWrapperException("The type resolved to a known struct but not a pointer to that struct (" + this.ToString() + ").");
                    }
                    else
                        throw new UnableToCreateWrapperException("The type could not be resolved (" + this.ToString() + ").");
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
