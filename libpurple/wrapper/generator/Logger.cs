using System;
using System.IO;

namespace Scripts
{
    class Logger
    {
        private static StreamWriter writer = new StreamWriter("log.txt");

        public static void Log(String message)
        {
            message = message.Replace("\n", "\\n").Replace("\t", "\\t");
            while (message.Contains("  "))
                message = message.Replace("  ", " ");
            writer.WriteLine(message);
        }

        public static void CloseLog()
        {
            writer.Close();
        }
    }
}
