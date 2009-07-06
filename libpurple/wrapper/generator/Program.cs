using System;

namespace Scripts
{
    class Program
    {
        static void Main(string[] args)
        {
            WrapperGenerator wrapperGen = new WrapperGenerator();

            Console.WriteLine("Parsing .h files...");
            wrapperGen.ParseHFilesInDirectory(@"C:\Users\Wade\Desktop\pidgin-src\im.pidgin.soc.2009.penguin.visualstudio\libpurple");
            //wrapperGen.ParseHFile(@"C:\Users\Wade\Desktop\pidgin-src\im.pidgin.soc.2009.penguin.visualstudio\libpurple\plugin.h");

            Console.WriteLine("Writing struct data...");
            wrapperGen.GenerateStructFiles();

            Console.WriteLine("Writing function data...");
            wrapperGen.GenerateFunctionWrappers();

            Console.WriteLine("Writing base classes and visual studio project file...");
            wrapperGen.WriteVisualStudioProjectFile();
            wrapperGen.WriteBaseClasses();

            Logger.CloseLog();

            Console.WriteLine("Finished!");
            Console.ReadLine();


        }
    }
}
