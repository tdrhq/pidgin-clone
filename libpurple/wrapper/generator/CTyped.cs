using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Scripts
{
    class CTyped
    {
        private string type;

        public String Type
        {
            get { return type; }
            set { type = value; }
        }

        public String RawType
        {
            get
            {
                if (this.Type.Contains(" "))
                    return this.Type.Substring(0, type.IndexOf(" "));
                else
                    return this.Type;
            }
        }

        public bool IsTypePointer
        {
            get
            {
                if (this.Type != null && (this.Type.EndsWith("*") || this.Type == "gpointer"))
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

        public String CSharpPrivateType
        {
            get
            {
                String str = "";

                if (this.IsTypeNative)
                    str += this.NativeType;
                else if (this.IsTypePointer)
                    str += "IntPtr";
                else if (this.IsTypeVoid)
                    str += "void";
                else
                    str += "UNKNOWN";

                return str;
            }
        }

        public String CSharpPublicType
        {
            get
            {
                String str = "";

                if (this.IsTypeNative)
                    str += this.NativeType;
                else if (this.Type == "void *" || this.Type == "gpointer")
                    str += "IntPtr";
                else
                    str += this.RawType;

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
                        return "byte";

                    case "gchar *":
                    case "char *":
                        return "string";

                    case "gint":
                    case "int":
                    case "gint32":
                        return "int";

                    case "guint":
                    case "unsigned int":
                    case "guint32":
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
                        return "long";

                    case "gulong":
                    case "unsigned long":
                    case "guint64":
                        return "ulong";

                    case "gfloat":
                    case "float":
                        return "float";

                    case "gdouble":
                    case "double":
                        return "double";
                }

                return null;
            }
        }

    }
}
