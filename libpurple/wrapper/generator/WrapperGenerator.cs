using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;

namespace Scripts
{
    class WrapperGenerator
    {
        private String path = @"C:\Users\Wade\Desktop\gen-test\";

        public void ParseHFilesInDirectory(String directory)
        {
            DirectoryInfo directionryInfo = new DirectoryInfo(directory);
            FileInfo[] fileInfos = directionryInfo.GetFiles("*.h");

            foreach (FileInfo fileInfo in fileInfos)
            {
                if (fileInfo.Name == "internal.h" || fileInfo.Name == "dbus.h" || fileInfo.Name == "dbus-bindings.h" ||
                    fileInfo.Name == "dbus-maybe.h" || fileInfo.Name == "dbus-define-api.h" || fileInfo.Name == "dbus-purple.h" ||
                    fileInfo.Name == "dbus-server.h" || fileInfo.Name == "dbus-useful.h" || fileInfo.Name == "gaim-compat.h" ||
                    fileInfo.Name == "media-gst.h")
                {
                    continue;
                }

                ParseHFile(fileInfo.FullName);
            }
        }


        public void ParseHFile(String path)
        {
            Regex structRegex =         new Regex(@"struct \s+ (?<StructName> \w+ ) \s* " +
                                                  @"\{ " +
                                                  @"      (?<StructInternal>.*?) " +
                                                  @"\} \s* (\w+ \s*)? ;",
                                                  RegexOptions.Singleline | RegexOptions.IgnorePatternWhitespace);

            Regex structInternalRegex = new Regex(@"(?<VariableType>   " +
                                                  @"      ( const \s+ unsigned \s+ \w+ \s+ [\*]+ )    |" +
                                                  @"      ( const \s+ unsigned \s+ \w+ \s+       )    |" +
                                                  @"      ( const \s+              \w+ \s+ [\*]+ )    |" +
                                                  @"      ( const \s+              \w+ \s+       )    |" +
                                                  @"      (           unsigned \s+ \w+ \s+ [\*]+ )    |" +
                                                  @"      (           unsigned \s+ \w+ \s+       )    |" +
                                                  @"      (                        \w+ \s+ [\*]+ )    |" +
                                                  @"      (                        \w+ \s+       )     " +
                                                  @")" +
                                                  @"(  ( \( \* (?<Name> \w+ ) \) \s* \( (?<FunctionPointerArguments> .*?) \) )" +
                                                  @"    |  (?<Name> .*? ) ) ;", RegexOptions.Singleline | RegexOptions.IgnorePatternWhitespace);

            Regex functionRegEx =
                new Regex(@"(?<ReturnType> " +
                          @"     ( unsigned \s+ \w+ \s* [\*]+ \s* )     | " +
                          @"     ( unsigned \s+ \w+ \s+           )     | " +
                          @"     (              \w+ \s* [\*]+ \s* )     | " +
                          @"     (              \w+ \s+           )       " +
                          @")" +
                          @"(?<Name>\w+?) \s* \( (?<ParameterList> [a-zA-Z0-9_,\* ]*) \)", RegexOptions.Singleline | RegexOptions.IgnorePatternWhitespace);
            
            Regex functionArgumentsRegEx =
                new Regex(@"(?<VariableType> " +
                          @"     ( const unsigned \s+ \w+ \s* [\*]+ \s* )     | " +
                          @"     ( const unsigned \s+ \w+ \s+           )     | " +
                          @"     ( const              \w+ \s* [\*]+ \s* )     | " +
                          @"     ( const              \w+ \s+           )     | " +
                          @"     (       unsigned \s+ \w+ \s* [\*]+ \s* )     | " +
                          @"     (       unsigned \s+ \w+ \s+           )     | " +
                          @"     (                    \w+ \s* [\*]+ \s* )     | " +
                          @"     (                    \w+ \s+           )       " +
                          @") " +
                          @"(?<VariableName> (\w | \[ | \])* )",
                          RegexOptions.Singleline | RegexOptions.IgnorePatternWhitespace);

            Regex commentRegex = new Regex(@"(?<Comment> /\* .*? \*/)", RegexOptions.Singleline | RegexOptions.IgnorePatternWhitespace);

            Regex enumRegex = new Regex(@"typedef \s+ enum \s* \w* \s* \{ \s* (?<EnumInternalPlusClose> .*? \s* \} ) \s* (?<Name> \w+ ) \s* ;", RegexOptions.Singleline | RegexOptions.IgnorePatternWhitespace);
            Regex enumInternalRegex = new Regex(@"(?<Name> \w+ ) \s* ( = \s* (?<Value> .+?) )? \s* (, | \})", RegexOptions.Singleline | RegexOptions.IgnorePatternWhitespace);

            Regex functionPointerRegex = new Regex(@"typedef \s* (?<ReturnType> \w* ) \s* \( \* (?<Name> \w* ) \) \s* \( (?<FunctionPointerArguments> .*?) \);", RegexOptions.Singleline | RegexOptions.IgnorePatternWhitespace);


            FileInfo fileInfo = new FileInfo(path);
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
                Logger.Log("Reading struct [" + match.Groups[0].Value + "]...");
                CStruct structObject = new CStruct(currentFile, match.Groups["StructName"].Value);

                String internalStructData = match.Groups["StructInternal"].Value;

                if (internalStructData != null)
                {
                    foreach (Match internalStructMatch in structInternalRegex.Matches(internalStructData))
                    {
                        Logger.Log("Reading struct element: [" + internalStructMatch.Groups[0].Value + "]...");
                        string elementType = internalStructMatch.Groups["VariableType"].Value.Trim();
                        string name = internalStructMatch.Groups["Name"].Value;

                        CArgument argument = new CArgument(currentFile, elementType, name);

                        string functionPointerArguments = internalStructMatch.Groups["FunctionPointerArguments"].Value;
                        if (functionPointerArguments != "")
                        {
                            argument.IsFunctionPointer = true;

                            String[] functionPointerArgumentsList = functionPointerArguments.Split(',');
                            foreach (String functionPointerArgument in functionPointerArgumentsList)
                            {
                                String currentArgument = functionPointerArgument.Replace("const ", "").Trim();

                                CArgument functionPointerArgumentObject = new CArgument(currentFile);
                                if (currentArgument == "void")
                                    continue;
                                else
                                {
                                    Match parameterMatch = functionArgumentsRegEx.Match(currentArgument);
                                    functionPointerArgumentObject.Type = parameterMatch.Groups["VariableType"].Value.Trim();
                                    functionPointerArgumentObject.Name = parameterMatch.Groups["VariableName"].Value;

                                    if (functionPointerArgumentObject.Name.Contains("["))
                                    {
                                        // TODO: Array size (eg: "char hostname[256]")
                                        functionPointerArgumentObject.IsArray = true;
                                        functionPointerArgumentObject.Name = functionPointerArgumentObject.Name.Substring(functionPointerArgumentObject.Name.IndexOf("["));
                                    }
                                }

                                argument.AddFunctionPointerArgument(functionPointerArgumentObject);
                            }
                        }

                        structObject.AddField(argument);
                    }
                }

                //Console.WriteLine(structObject.ToString()); Console.WriteLine();
                currentFile.addStruct(structObject);
            }

            foreach (Match match in functionRegEx.Matches(fullFile))
            {
                Logger.Log("Reading function [" + match.Groups[0].Value + "]...");
                String functionReturnType = match.Groups["ReturnType"].Value.Trim();
                String functionName = match.Groups["Name"].Value;
                String parameterList = match.Groups["ParameterList"].Value;


                /* There's a few messy #define's for PURPLE_INIT_PLUGIN that yields some false
                 * matches... so we throw them out. */
                if (currentFile.FileName == "plugin.h")
                {
                    if (functionName == "_FUNC_NAME" || functionName == "purple_init_plugin")
                        continue;

                    if (functionReturnType == "return")
                        continue;
                }

                if (functionReturnType == "typedef" || functionReturnType == "define")
                    continue;


                CFunction function = new CFunction(currentFile, functionReturnType, functionName);

                String[] functionParameters = parameterList.Split(',');
                foreach (String functionParameterInList in functionParameters)
                {
                    String functionParameter = functionParameterInList.Replace("const ", "").Trim();

                    CArgument argument = new CArgument(currentFile);
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

                        if (argument.Name.EndsWith("[]"))
                        {
                            argument.Name = argument.Name.Substring(0, argument.Name.Length - 2);
                            argument.IsArray = true;
                        }
                    }

                    function.AddArgument(argument);
                }

                currentFile.addFunction(function);
                //Console.WriteLine(function.ToString());
                //Console.WriteLine(function.GetCSharpPrivateFunction());
                //Console.WriteLine(function.GetCSharpPublicFunction());
                //Console.WriteLine();
            }

            foreach (Match match in enumRegex.Matches(fullFile))
            {
                Logger.Log("Reading enum [" + match.Groups[0].Value + "]...");
                CEnum enumeration = new CEnum(currentFile, match.Groups["Name"].Value);

                foreach (Match enumElementMatch in enumInternalRegex.Matches(match.Groups["EnumInternalPlusClose"].Value))
                {
                    Logger.Log("Reading enum element [" + enumElementMatch.Groups[0].Value + "]...");
                    enumeration.AddElement(enumElementMatch.Groups["Name"].Value, enumElementMatch.Groups["Value"].Value);
                }

                currentFile.AddEnum(enumeration);
                //Console.WriteLine(enumeration.ToString());
            }


            foreach (Match match in functionPointerRegex.Matches(fullFile))
            {
                Logger.Log("Reading function pointer [" + match.Groups[0].Value + "]...");
                CFunction function = new CFunction(currentFile, match.Groups["ReturnType"].Value, match.Groups["Name"].Value);
                function.IsFunctionPointer = true;

                string functionPointerArguments = match.Groups["FunctionPointerArguments"].Value;
                if (functionPointerArguments != "")
                {
                    String[] functionPointerArgumentsList = functionPointerArguments.Split(',');
                    foreach (String functionPointerArgument in functionPointerArgumentsList)
                    {
                        String currentArgument = functionPointerArgument.Replace("const ", "").Trim();

                        CArgument functionPointerArgumentObject = new CArgument(currentFile);
                        if (currentArgument == "void")
                            continue;
                        else
                        {
                            Match parameterMatch = functionArgumentsRegEx.Match(currentArgument);
                            functionPointerArgumentObject.Type = parameterMatch.Groups["VariableType"].Value.Trim();
                            functionPointerArgumentObject.Name = parameterMatch.Groups["VariableName"].Value;
                        }

                        function.AddArgument(functionPointerArgumentObject);
                    }
                }

                currentFile.addFunctionPointer(function);
                //Console.WriteLine(function.ToString());
            }

            CFile.FileCollection.Add(currentFile);
        }

        public void WriteBaseClasses()
        {
            StreamWriter writer = new StreamWriter(path + "UnmanagedWrapper.cs");
            
            writer.WriteLine("using System;");
            writer.WriteLine();
            writer.WriteLine("namespace UnmanagedWrapper");
            writer.WriteLine("{");
            writer.WriteLine("    public abstract class UnmanagedWrapper<T>");
            writer.WriteLine("    {");
            writer.WriteLine("        private T data;");
            writer.WriteLine("        private IntPtr reference;");
            writer.WriteLine();
            writer.WriteLine("        public Type GetUnmanagedType() { return typeof(T); }");
            writer.WriteLine();
            writer.WriteLine("        public IntPtr Reference");
            writer.WriteLine("        {");
            writer.WriteLine("            get { return reference; }");
            writer.WriteLine("            set { reference = value; }");
            writer.WriteLine("        }");
            writer.WriteLine();
            writer.WriteLine("        public T Data");
            writer.WriteLine("        {");
            writer.WriteLine("            get { return data; }");
            writer.WriteLine("            set { data = value; }");
            writer.WriteLine("        }");
            writer.WriteLine("    }");
            writer.WriteLine("}");
            writer.WriteLine();

            writer.Close();
        }

        public void WriteVisualStudioProjectFile()
        {
            StreamWriter writer = new StreamWriter(path + "PurpleWrapper.csproj");

            writer.WriteLine("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
            writer.WriteLine("<Project ToolsVersion=\"3.5\" DefaultTargets=\"Build\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">");
            writer.WriteLine("  <PropertyGroup>");
            writer.WriteLine("    <Configuration Condition=\" '$(Configuration)' == '' \">Debug</Configuration>");
            writer.WriteLine("    <Platform Condition=\" '$(Platform)' == '' \">AnyCPU</Platform>");
            writer.WriteLine("    <ProductVersion>9.0.30729</ProductVersion>");
            writer.WriteLine("    <SchemaVersion>2.0</SchemaVersion>");
            writer.WriteLine("    <ProjectGuid>{8034DEA9-30CC-DEA0-3903-80210CE809FA}</ProjectGuid>");
            writer.WriteLine("    <OutputType>Exe</OutputType>");
            writer.WriteLine("    <AppDesignerFolder>Properties</AppDesignerFolder>");
            writer.WriteLine("    <RootNamespace>Scripts</RootNamespace>");
            writer.WriteLine("    <AssemblyName>Scripts</AssemblyName>");
            writer.WriteLine("    <TargetFrameworkVersion>v3.5</TargetFrameworkVersion>");
            writer.WriteLine("    <FileAlignment>512</FileAlignment>");
            writer.WriteLine("  </PropertyGroup>");
            writer.WriteLine("  <PropertyGroup Condition=\" '$(Configuration)|$(Platform)' == 'Debug|AnyCPU' \">");
            writer.WriteLine("    <DebugSymbols>true</DebugSymbols>");
            writer.WriteLine("    <DebugType>full</DebugType>");
            writer.WriteLine("    <Optimize>false</Optimize>");
            writer.WriteLine("    <OutputPath>bin\\Debug\\</OutputPath>");
            writer.WriteLine("    <DefineConstants>DEBUG;TRACE</DefineConstants>");
            writer.WriteLine("    <ErrorReport>prompt</ErrorReport>");
            writer.WriteLine("    <WarningLevel>4</WarningLevel>");
            writer.WriteLine("  </PropertyGroup>");
            writer.WriteLine("  <PropertyGroup Condition=\" '$(Configuration)|$(Platform)' == 'Release|AnyCPU' \">");
            writer.WriteLine("    <DebugType>pdbonly</DebugType>");
            writer.WriteLine("    <Optimize>true</Optimize>");
            writer.WriteLine("    <OutputPath>bin\\Release\\</OutputPath>");
            writer.WriteLine("    <DefineConstants>TRACE</DefineConstants>");
            writer.WriteLine("    <ErrorReport>prompt</ErrorReport>");
            writer.WriteLine("    <WarningLevel>4</WarningLevel>");
            writer.WriteLine("  </PropertyGroup>");
            writer.WriteLine("  <ItemGroup>");
            writer.WriteLine("    <Reference Include=\"System\" />");
            writer.WriteLine("    <Reference Include=\"System.Core\">");
            writer.WriteLine("      <RequiredTargetFramework>3.5</RequiredTargetFramework>");
            writer.WriteLine("    </Reference>");
            writer.WriteLine("  </ItemGroup>");
            writer.WriteLine("  <ItemGroup>");

            writer.WriteLine("    <Compile Include=\"UnmanagedWrapper.cs\" />");
            foreach (CFile file in CFile.FileCollection)
            {
                writer.WriteLine("    <Compile Include=\"" + file.FileNameAsClassName + ".cs\" />");

                foreach (CStruct structure in file.Structs)
                {
                    writer.WriteLine("    <Compile Include=\"" + structure.Name + ".cs\" />");
                }
            }


            writer.WriteLine("  </ItemGroup>");
            writer.WriteLine("  <Import Project=\"$(MSBuildToolsPath)\\Microsoft.CSharp.targets\" />");
            writer.WriteLine("</Project>");

            writer.Close();
        }


        private String GetCommentHeader()
        {
            StringBuilder sb = new StringBuilder();

            sb.AppendLine("/* PurpleWrapper - A .NET (CLR) wrapper for libpurple");
            sb.AppendLine(" *");
            sb.AppendLine(" * Purple is the legal property of its developers, whose names are too numerous");
            sb.AppendLine(" * to list here.  Please refer to the COPYRIGHT file distributed with this");
            sb.AppendLine(" * source distribution.");
            sb.AppendLine(" *");
            sb.AppendLine(" * This program is free software; you can redistribute it and/or modify");
            sb.AppendLine(" * it under the terms of the GNU General Public License as published by");
            sb.AppendLine(" * the Free Software Foundation; either version 2 of the License, or");
            sb.AppendLine(" * (at your option) any later version.");
            sb.AppendLine(" *");
            sb.AppendLine(" * This program is distributed in the hope that it will be useful,");
            sb.AppendLine(" * but WITHOUT ANY WARRANTY; without even the implied warranty of");
            sb.AppendLine(" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the");
            sb.AppendLine(" * GNU General Public License for more details.");
            sb.AppendLine(" *");
            sb.AppendLine(" * You should have received a copy of the GNU General Public License");
            sb.AppendLine(" * along with this program; if not, write to the Free Software");
            sb.AppendLine(" * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA");
            sb.AppendLine(" */");
            sb.AppendLine();
            sb.AppendLine("/*");
            sb.AppendLine(" * This file was auto-generated from the libpurple header files to provide a");
            sb.AppendLine(" * clean interface between .NET/CLR and the unmanaged C library libpurple.");
            sb.AppendLine(" *");
            sb.AppendLine(" * This is the second major commit of the code.");
            sb.AppendLine(" * Next things:");
            sb.AppendLine(" *  - A few of the .h files have anonymous parameter names (eg: void cat(int, int).");
            sb.AppendLine(" *    This program will need to assign these parameters names.");
            sb.AppendLine(" *  - Function pointers inside structs aren't translated correctly into C#.");
            sb.AppendLine(" *  - Two places there are specific-length arrays (eg: char hostname[256]). The parser");
            sb.AppendLine(" *    does not detect them as an array.");
            sb.AppendLine(" */");

            return sb.ToString();
        }

        public void GenerateStructFiles()
        {
            StreamWriter writer;
            foreach (CFile file in CFile.FileCollection)
            {
                foreach (CStruct structure in file.Structs)
                {
                    StringBuilder structureStringBuilder = new StringBuilder();

                    structureStringBuilder.AppendLine(GetCommentHeader());

                    structureStringBuilder.AppendLine("using System;");
                    structureStringBuilder.AppendLine("using System.Collections.Generic;");
                    structureStringBuilder.AppendLine("using System.Runtime.InteropServices;");
                    structureStringBuilder.AppendLine();
                    structureStringBuilder.AppendLine("namespace PurpleWrapper");
                    structureStringBuilder.AppendLine("{");
                    structureStringBuilder.AppendLine("\tpublic class " + structure.Name + " : UnmanagedWrapper<_" + structure.Name + ">");
                    structureStringBuilder.AppendLine("\t{");


                    try
                    {
                        StringBuilder sb = new StringBuilder();

                        sb.AppendLine("\t\tpublic " + structure.Name + "()");
                        sb.AppendLine("\t\t{");
                        sb.AppendLine("\t\t}");
                        sb.AppendLine();

                        sb.AppendLine("\t\tpublic " + structure.Name + "(IntPtr refernece)");
                        sb.AppendLine("\t\t{");
                        sb.AppendLine("\t\t\tthis.Reference = reference;");
                        sb.AppendLine("\t\t\tthis.Data = (_" + structure.Name + ")Marshal.PtrToStructure(this.Reference, typeof(_" + structure.Name + "));");
                        sb.AppendLine("\t\t}");
                        sb.AppendLine();



                        foreach (CArgument argument in structure.Fields)
                        {
                            sb.AppendLine("\t\tpublic " + argument.GetCSharpPublicFunction());
                            sb.AppendLine("\t\t{");


                            /* get */
                            sb.AppendLine("\t\t\tget");
                            sb.AppendLine("\t\t\t{");
                            if (argument.IsTypeNative)
                                sb.AppendLine("\t\t\t\treturn this.Data." + argument.Name + ";");
                            else
                                sb.AppendLine("\t\t\t\tthrow new NotImplementedException(); /* Non-native type. */");
                            sb.AppendLine("\t\t\t}");


                            /* set */
                            sb.AppendLine("\t\t\tset");
                            sb.AppendLine("\t\t\t{");
                            sb.AppendLine("\t\t\t\tif (this.Reference != IntPtr.Zero)");
                            sb.AppendLine("\t\t\t\t\tthis.Reference = IntPtr.Zero;");
                            sb.AppendLine();

                            if (argument.IsTypeNative)
                                sb.AppendLine("\t\t\t\tthis.Data." + argument.Name + " = value;");
                            else
                                sb.AppendLine("\t\t\t\tthrow new NotImplementedException(); /* Non-native type. */");

                            sb.AppendLine("\t\t\t}");
                            sb.AppendLine("\t\t}");
                            sb.AppendLine();
                        }

                        sb.AppendLine("\t}");
                        sb.AppendLine();
                        sb.AppendLine();


                        sb.AppendLine("\t[StructLayout(LayoutKind.Sequential)]");
                        sb.AppendLine("\tclass _" + structure.Name);
                        sb.AppendLine("\t{");

                        foreach (CArgument argument in structure.Fields)
                        {
                            sb.AppendLine("\t\t/*");
                            sb.AppendLine("\t\t * " + argument.ToString());
                            sb.AppendLine("\t\t */");

                            if (argument.IsFunctionPointer)
                                sb.AppendLine("\t\tIntPtr " + argument.SafeName + ";");
                            else
                            {
                                switch (argument.Category)
                                {
                                    case CTyped.TypeCategory.Native:
                                    case CTyped.TypeCategory.VoidPointer:
                                        sb.AppendLine("\t\tIntPtr " + argument.SafeName + ";");
                                        break;

                                    case CTyped.TypeCategory.DateTime:
                                        sb.AppendLine("\t\tulong " + argument.SafeName + ";");
                                        break;

                                    default:
                                        sb.AppendLine("\t\t/* Cannot generate struct for type " + argument.Category.ToString() + " */");
                                        break;
                                }
                            }

                            //sb.AppendLine("\t\t" + argument.GetCSharpPrivateFunction() + ";");
                            sb.AppendLine();
                        }

                        sb.AppendLine("\t}");

                        structureStringBuilder.AppendLine(sb.ToString());
                    }
                    catch (UnableToCreateWrapperException e)
                    {
                        Console.WriteLine("Could not generate a wrapper for " + structure.Name + " in file \"" + structure.File.FileName + "\".");
                        Console.WriteLine("Message: " + e.Message);

                        structureStringBuilder.AppendLine("\t\t/* ");
                        structureStringBuilder.AppendLine("\t\t * " + "Could not generate a wrapper for " + structure.Name + " in file \"" + structure.File.FileName + "\".");
                        structureStringBuilder.AppendLine("\t\t * " + "Message: " + e.Message);
                        structureStringBuilder.AppendLine("\t\t */");
                        structureStringBuilder.AppendLine();

                        structureStringBuilder.AppendLine("\t\tpublic " + structure.Name + "()");
                        structureStringBuilder.AppendLine("\t\t{");
                        structureStringBuilder.AppendLine("\t\t\tthrow new NotImplementedException();");
                        structureStringBuilder.AppendLine("\t\t}");
                        structureStringBuilder.AppendLine();

                        structureStringBuilder.AppendLine("\t\tpublic " + structure.Name + "(IntPtr refernece)");
                        structureStringBuilder.AppendLine("\t\t{");
                        structureStringBuilder.AppendLine("\t\t\tthrow new NotImplementedException();");
                        structureStringBuilder.AppendLine("\t\t}");
                        structureStringBuilder.AppendLine();

                        structureStringBuilder.AppendLine("\t}");
                        structureStringBuilder.AppendLine();
                        structureStringBuilder.AppendLine("\t[StructLayout(LayoutKind.Sequential)]");
                        structureStringBuilder.AppendLine("\tclass _" + structure.Name + " { }");
                    }

                    structureStringBuilder.AppendLine("}");


                    writer = new StreamWriter(path + structure.Name + ".cs");
                    writer.WriteLine(structureStringBuilder.ToString());
                    writer.Close();

                }
            }
        }

        public void GenerateFunctionWrappers()
        {
            foreach (CFile file in CFile.FileCollection)
            {
                StreamWriter writer = new StreamWriter(path + file.FileNameAsClassName + ".cs");
                writer.WriteLine(GetCommentHeader());

                writer.WriteLine("using System;");
                writer.WriteLine("using System.Collections.Generic;");
                writer.WriteLine("using System.Runtime.InteropServices;");
                writer.WriteLine();
                writer.WriteLine("namespace PurpleWrapper");
                writer.WriteLine("{");

                writer.WriteLine("\tpublic class " + file.FileNameAsClassName);
                writer.WriteLine("\t{");

                foreach (CEnum enumeration in file.Enums)
                {
                    enumeration.WriteForCSharp(writer, "\t\t");
                }


                foreach (CFunction function in file.Functions)
                {
                    try
                    {
                        StringBuilder sb = new StringBuilder();

                        sb.AppendLine("\t\t/*");
                        sb.AppendLine("\t\t * " + function.ToString());
                        sb.AppendLine("\t\t */");
                        sb.AppendLine("\t\t[DllImport(\"libpurple.dll\")]");
                        sb.AppendLine("\t\tprivate static extern " + function.GetCSharpPrivateFunction() + ";");
                        sb.AppendLine();
                        sb.AppendLine("\t\tpublic static " + function.GetCSharpPublicFunction());
                        sb.AppendLine("\t\t{");


                        String[] functionArgs = new String[function.Arguments.Count];
                        bool allArgsCanBeUsed = true;

                        for (int i = 0; i < function.Arguments.Count; i++)
                        {
                            CArgument arg = function.Arguments[i];

                            switch (arg.Category)
                            {
                                case CTyped.TypeCategory.PointerToKnownStruct:
                                    functionArgs[i] = arg.SafeName + ".Reference";
                                    break;

                                case CTyped.TypeCategory.Native:
                                case CTyped.TypeCategory.VoidPointer:
                                    functionArgs[i] = arg.SafeName;
                                    break;

                                case CTyped.TypeCategory.DateTime:
                                    sb.AppendLine("\t\t\tulong _PurpleWrapper_param" + i + " = (ulong)(" + arg.SafeName + " - new DateTime(1970, 1, 1)).TotalSeconds;");
                                    functionArgs[i] = "_PurpleWrapper_param" + i;
                                    break;

                                default:
                                    sb.AppendLine("\t\t\t/* Unable to process " + arg.SafeName + ", a " + arg.Category.ToString() + ". */");
                                    allArgsCanBeUsed = false;
                                    break;
                            }
                        }



                        if (allArgsCanBeUsed)
                        {
                            String functionCall = "";
                            functionCall = function.Name + "(";
                            for (int i = 0; i < functionArgs.Length; i++)
                            {
                                functionCall += functionArgs[i];

                                if (i != functionArgs.Length - 1)
                                    functionCall += ", ";
                            }
                            functionCall += ")";


                            String returnStatement = "";
                            switch (function.Category)
                            {
                                case CTyped.TypeCategory.Void:
                                    /* No return should be done. */
                                    returnStatement = functionCall + ";";
                                    break;

                                case CTyped.TypeCategory.Native:
                                case CTyped.TypeCategory.VoidPointer:
                                    /* Simply return the value returned -- it's all native. */
                                    returnStatement = "return " + functionCall + ";";
                                    break;

                                case CTyped.TypeCategory.PointerToKnownStruct:
                                    returnStatement = "return new " + function.RawType + "(" + functionCall + ");";
                                    break;

                                default:
                                    sb.AppendLine("\t\t\t/* Unable to process " + function.SafeName + ", a " + function.Category.ToString() + ". */");
                                    break;

                            }


                            if (allArgsCanBeUsed)
                                sb.AppendLine("\t\t\t" + returnStatement);
                            else
                                sb.AppendLine("\t\t\tthrow new NotImplementedException();");
                        }
                        else
                        {
                            sb.AppendLine("\t\t\tthrow new NotImplementedException();");
                        }

                        sb.AppendLine("\t\t}");

                        writer.WriteLine(sb.ToString());
                    }
                    catch (UnableToCreateWrapperException e)
                    {
                        Console.WriteLine("Could not generate a wrapper for " + function.Name + " in file \"" + function.File.FileName + "\".");
                        Console.WriteLine("Message: " + e.Message);

                        StringBuilder sb = new StringBuilder();
                        sb.AppendLine("\t\t/*");
                        sb.AppendLine("\t\t * " + function.ToString());
                        sb.AppendLine("\t\t * ");
                        sb.AppendLine("\t\t * " + "Could not generate a wrapper for " + function.Name + " in file \"" + function.File.FileName + "\".");
                        sb.AppendLine("\t\t * " + "Message: " + e.Message);
                        sb.AppendLine("\t\t */");

                        writer.WriteLine(sb.ToString());
                    }
                }

                writer.WriteLine("\t}");
                writer.WriteLine("}");
                writer.WriteLine();

                writer.Close();
            }
        }
    }
}
