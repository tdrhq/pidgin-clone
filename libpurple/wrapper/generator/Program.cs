using System;

namespace Scripts
{
    class Program
    {
        static void Main(string[] args)
        {
            WrapperGenerator wrapperGen = new WrapperGenerator();

            Console.WriteLine("Parsing .h files...");
            wrapperGen.ParseHFilesInDirectory(@"C:\Users\Wade\Desktop\fullOA\OABuild\libpurple");

            Console.WriteLine("Writing struct data...");
            wrapperGen.GenerateStructFiles();

            Console.WriteLine("Writing function data...");
            wrapperGen.GenerateFunctionWrappers();
        }
    }
}
