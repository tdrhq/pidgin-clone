using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;

namespace Scripts
{
    class WrapperGenerator
    {
        private List<CFile> files = new List<CFile>();
        private String path = @"C:\Users\Wade\Desktop\gen-test\";

        public void ParseHFilesInDirectory(String path)
        {
            DirectoryInfo directionryInfo = new DirectoryInfo(path);
            FileInfo[] fileInfos = directionryInfo.GetFiles("*.h");

            
            Regex structRegex = new Regex(@"struct \s+ (?<StructName>\w+?) \s* \{ (?<StructInternal>.*?) \} \s* ;", RegexOptions.Singleline | RegexOptions.IgnorePatternWhitespace);
            Regex structInternalRegex = new Regex(@"(?<VariableType> ( \w+ \s+ [\*]+ ) | ( \w+ \s+) )" +
                                                  @"(  ( \( \* (?<Name> \w+ ) \) \s* \( (?<FunctionPointerArguments> .*?) \) )" +
                                                  @"    |  (?<Name> .*? ) ) ;", RegexOptions.Singleline | RegexOptions.IgnorePatternWhitespace);

            Regex functionRegEx = new Regex(@"(?<ReturnType> ( \w+ \s+ [\*]+ ) | ( \w+ \s+)  ) (?<Name>\w+?) \s* \( (?<ParameterList> [^\)]*) \)", RegexOptions.Singleline | RegexOptions.IgnorePatternWhitespace);
            Regex functionArgumentsRegEx = new Regex(@"(?<VariableType> ( \w+ \s+ [\*]+ ) | ( \w+ \s+)  ) (?<VariableName>\w+)", RegexOptions.Singleline | RegexOptions.IgnorePatternWhitespace);

            Regex commentRegex = new Regex(@"(?<Comment> /\* .*? \*/)", RegexOptions.Singleline | RegexOptions.IgnorePatternWhitespace);

            foreach (FileInfo fileInfo in fileInfos)
            {
                if (fileInfo.Name == "internal.h" || fileInfo.Name == "dbus.h" || fileInfo.Name == "dbus-bindings.h" ||
                    fileInfo.Name == "dbus-maybe.h" || fileInfo.Name == "dbus-define-api.h" || fileInfo.Name == "dbus-purple.h" ||
                    fileInfo.Name == "dbus-server.h" || fileInfo.Name == "dbus-useful.h" || fileInfo.Name == "gaim-compat.h" ||
                    fileInfo.Name == "media-gst.h")
                    continue;


                StreamReader reader = new StreamReader(fileInfo.FullName);
                String fullFile = reader.ReadToEnd();
                reader.Close();


                CFile currentFile = new CFile(fileInfo.Name);

                foreach (Match commentMatch in commentRegex.Matches(fullFile))
                {
                    fullFile = fullFile.Remove(
                        fullFile.IndexOf(commentMatch.Groups["Comment"].Value),
                        commentMatch.Groups["Comment"].Length);
                }

                foreach (Match match in structRegex.Matches(fullFile))
                {
                    CStruct structObject = new CStruct(match.Groups["StructName"].Value);
                    if (structObject.Name.Length > 0 && structObject.Name[0] == '_')
                        structObject.Name = structObject.Name.Substring(1);

                    String internalStructData = match.Groups["StructInternal"].Value;

                    if (internalStructData != null)
                    {
                        foreach (Match internalStructMatch in structInternalRegex.Matches(internalStructData))
                        {
                            string elementType = internalStructMatch.Groups["VariableType"].Value.Trim();
                            string name = internalStructMatch.Groups["Name"].Value;

                            CArgument argument = new CArgument(elementType, name);

                            string functionPointerArguments = internalStructMatch.Groups["FunctionPointerArguments"].Value;
                            if (functionPointerArguments != "")
                            {
                                argument.IsFunctionPointer = true;

                                String[] functionPointerArgumentsList = functionPointerArguments.Split(',');
                                foreach (String functionPointerArgument in functionPointerArgumentsList)
                                {
                                    String currentArgument = functionPointerArgument.Replace("const ", "").Trim();

                                    CArgument functionPointerArgumentObject = new CArgument();
                                    if (currentArgument == "void")
                                        continue;
                                    else
                                    {
                                        Match parameterMatch = functionArgumentsRegEx.Match(currentArgument);
                                        functionPointerArgumentObject.Type = parameterMatch.Groups["VariableType"].Value.Trim();
                                        functionPointerArgumentObject.Name = parameterMatch.Groups["VariableName"].Value;
                                    }

                                    argument.AddFunctionPointerArgument(functionPointerArgumentObject);
                                }
                            }

                            structObject.AddField(argument);
                        }
                    }

                    //Console.WriteLine(structObject.ToString());
                    currentFile.addStruct(structObject);
                }

                foreach (Match match in functionRegEx.Matches(fullFile))
                {
                    String functionReturnType = match.Groups["ReturnType"].Value.Trim();
                    String functionName = match.Groups["Name"].Value;
                    String parameterList = match.Groups["ParameterList"].Value;

                    if (functionReturnType == "typedef" || functionReturnType == "define")
                        continue;


                    CFunction function = new CFunction(functionReturnType, functionName);

                    String[] functionParameters = parameterList.Split(',');
                    foreach (String functionParameterInList in functionParameters)
                    {
                        String functionParameter = functionParameterInList.Replace("const ", "").Trim();

                        CArgument argument = new CArgument();
                        if (functionParameter == "...")
                        {
                            argument.IsEllipsis = true;
                        }
                        else if (functionParameter == "void")
                            continue;
                        else
                        {
                            Match parameterMatch = functionArgumentsRegEx.Match(functionParameter);
                            argument.Type = parameterMatch.Groups["VariableType"].Value.Trim();
                            argument.Name = parameterMatch.Groups["VariableName"].Value;
                        }

                        function.AddArgument(argument);
                    }

                    currentFile.addFunction(function);
                    //Console.WriteLine(function.ToString());
                    //Console.WriteLine(function.GetCSharpPrivateFunction());
                    //Console.WriteLine(function.GetCSharpPublicFunction());
                    //Console.WriteLine();
                }

                files.Add(currentFile);
            }
        }

        private void WriteCommentHeader(StreamWriter writer)
        {
            writer.WriteLine("/* purple");
            writer.WriteLine(" *");
            writer.WriteLine(" * Purple is the legal property of its developers, whose names are too numerous");
            writer.WriteLine(" * to list here.  Please refer to the COPYRIGHT file distributed with this");
            writer.WriteLine(" * source distribution.");
            writer.WriteLine(" *");
            writer.WriteLine(" * This program is free software; you can redistribute it and/or modify");
            writer.WriteLine(" * it under the terms of the GNU General Public License as published by");
            writer.WriteLine(" * the Free Software Foundation; either version 2 of the License, or");
            writer.WriteLine(" * (at your option) any later version.");
            writer.WriteLine(" *");
            writer.WriteLine(" * This program is distributed in the hope that it will be useful,");
            writer.WriteLine(" * but WITHOUT ANY WARRANTY; without even the implied warranty of");
            writer.WriteLine(" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the");
            writer.WriteLine(" * GNU General Public License for more details.");
            writer.WriteLine(" *");
            writer.WriteLine(" * You should have received a copy of the GNU General Public License");
            writer.WriteLine(" * along with this program; if not, write to the Free Software");
            writer.WriteLine(" * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA");
            writer.WriteLine(" */");
            writer.WriteLine();
            writer.WriteLine("/*");
            writer.WriteLine(" * This file was auto-generated from the libpurple header files to provide a");
            writer.WriteLine(" * clean interface between .NET/CLR and the unmanaged C library, libpurple.");
            writer.WriteLine(" *");
            writer.WriteLine(" * This code isn't complete, but completely a work in progress. :)");
            writer.WriteLine(" * Three major things left:");
            writer.WriteLine(" *  - Resolve the remaining UNKNOWN types.");
            writer.WriteLine(" *  - Handle translation between delegate and function pointers.");
            writer.WriteLine(" *  - Fill in the translation between public .NET class calls and private DllImport[] calls.");
            writer.WriteLine(" */");
            writer.WriteLine();



        }

        public void GenerateStructFiles()
        {
            StreamWriter writer;
            foreach (CFile file in files)
            {
                foreach (CStruct structure in file.Structs)
                {
                    writer = new StreamWriter(path + structure.Name + ".cs");
                    WriteCommentHeader(writer);

                    writer.WriteLine("using System;");
                    writer.WriteLine("using System.Collections.Generic;");
                    writer.WriteLine("using System.Runtime.InteropServices;");
                    writer.WriteLine();
                    writer.WriteLine("namespace PurpleWrapper");
                    writer.WriteLine("{");
                    writer.WriteLine("\tpublic class " + structure.Name + " : UnmanagedWrapper<_" + structure.Name + ">");
                    writer.WriteLine("\t{");

                    writer.WriteLine("\t\tpublic " + structure.Name + "()");
                    writer.WriteLine("\t\t{");
                    writer.WriteLine("\t\t}");
                    writer.WriteLine();

                    writer.WriteLine("\t\tpublic " + structure.Name + "(IntPtr refernece)");
                    writer.WriteLine("\t\t{");
                    writer.WriteLine("\t\t\tthis.Reference = reference;");
                    writer.WriteLine("\t\t\tthis.Data = (_" + structure.Name + ")Marshal.PtrToStructure(this.Reference, typeof(_" + structure.Name + "));");
                    writer.WriteLine("\t\t}");
                    writer.WriteLine();



                    foreach (CArgument argument in structure.Fields)
                    {
                        writer.WriteLine("\t\tpublic " + argument.GetCSharpPublicFunction());
                        writer.WriteLine("\t\t{");


                        /* get */
                        writer.WriteLine("\t\t\tget");
                        writer.WriteLine("\t\t\t{");
                        if (argument.IsTypeNative)
                            writer.WriteLine("\t\t\t\treturn this.Data." + argument.Name + ";");
                        else
                            writer.WriteLine("\t\t\t\tthrow new NotImplementedException(); /* Non-native type. */");
                        writer.WriteLine("\t\t\t}");


                        /* set */
                        writer.WriteLine("\t\t\tset");
                        writer.WriteLine("\t\t\t{");
                        writer.WriteLine("\t\t\t\tif (this.Reference != IntPtr.Zero)");
                        writer.WriteLine("\t\t\t\t\tthis.Reference = IntPtr.Zero;");
                        writer.WriteLine();

                        if (argument.IsTypeNative)
                            writer.WriteLine("\t\t\t\tthis.Data." + argument.Name + " = value;");
                        else
                            writer.WriteLine("\t\t\t\tthrow new NotImplementedException(); /* Non-native type. */");

                        writer.WriteLine("\t\t\t}");
                        writer.WriteLine("\t\t}");
                        writer.WriteLine();
                    }

                    writer.WriteLine("\t}");
                    writer.WriteLine();
                    writer.WriteLine();


                    writer.WriteLine("\t[StructLayout(LayoutKind.Sequential)]");
                    writer.WriteLine("\tclass _" + structure.Name);
                    writer.WriteLine("\t{");

                    foreach (CArgument argument in structure.Fields)
                    {
                        writer.WriteLine("\t\t/*");
                        writer.WriteLine("\t\t * " + argument.ToString());
                        writer.WriteLine("\t\t */");
                        writer.WriteLine("\t\t" + argument.GetCSharpPrivateFunction() + ";");
                        writer.WriteLine();
                    }

                    writer.WriteLine("\t}");


                    writer.WriteLine("}");
                    writer.WriteLine();

                    writer.Close();


                }

            }

        }

        public void GenerateFunctionWrappers()
        {
            foreach (CFile file in files)
            {
                StreamWriter writer = new StreamWriter(path + file.FileNameAsClassName + ".cs");
                WriteCommentHeader(writer);

                writer.WriteLine("using System;");
                writer.WriteLine("using System.Collections.Generic;");
                writer.WriteLine("using System.Runtime.InteropServices;");
                writer.WriteLine();
                writer.WriteLine("namespace PurpleWrapper");
                writer.WriteLine("{");

                writer.WriteLine("\tpublic class " + file.FileNameAsClassName);
                writer.WriteLine("\t{");

                foreach (CFunction function in file.Functions)
                {
                    writer.WriteLine("\t\t/*");
                    writer.WriteLine("\t\t * " + function.ToString());
                    writer.WriteLine("\t\t */");
                    writer.WriteLine("\t\t[DllImport(\"libpurple.dll\")]");
                    writer.WriteLine("\t\tprivate static extern " + function.GetCSharpPrivateFunction() + ";");
                    writer.WriteLine();
                    writer.WriteLine("\t\tpublic static " + function.GetCSharpPublicFunction(file.FileNameAsClassName));
                    writer.WriteLine("\t\t{");
                    writer.WriteLine("\t\t\tthrow new NotImplementedException();");
                    writer.WriteLine("\t\t}");
                    writer.WriteLine();
                }

                writer.WriteLine("\t}");
                writer.WriteLine("}");
                writer.WriteLine();

                writer.Close();
            }
        }
    }
}
