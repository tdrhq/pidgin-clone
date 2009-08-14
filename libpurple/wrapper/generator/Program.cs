using System;

namespace Scripts
{
    class Program
    {
        static void Main(string[] args)
        {
            WrapperGenerator wrapperGen = new WrapperGenerator();

            Console.WriteLine("Parsing .h files...");
            wrapperGen.ParseHFilesInDirectory(@"../../../../");
            //wrapperGen.ParseHFile(@"../../../../plugin.h");

            Console.WriteLine("Writing struct data...");
            wrapperGen.GenerateStructFiles();

            Console.WriteLine("Writing function data...");
            wrapperGen.GenerateFunctionWrappers();

            Console.WriteLine("Writing base classes and visual studio project file...");
            wrapperGen.WriteVisualStudioProjectFile();
            wrapperGen.WriteBaseClasses();
            wrapperGen.WriteDefFile();

            /*
            Console.WriteLine();
            Console.WriteLine("Finding \"PurpleAccountRegistrationCb\"...");
            Console.WriteLine(CFile.LookUpName("PurpleAccountRegistrationCb").ToString());
            */

            Logger.CloseLog();

            Console.WriteLine("Finished!");
            Console.ReadLine();


        }
    }
}
