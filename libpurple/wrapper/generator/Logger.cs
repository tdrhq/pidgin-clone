using System;
using System.IO;
using System.Collections.Generic;

namespace Scripts
{
    class Logger
    {
        private static StreamWriter writer = new StreamWriter("log.txt");
        private static Stack<string> prefixList = new Stack<string>();

        public static void Log(String message)
        {
            message = message.Replace("\n", "\\n").Replace("\t", "\\t");
            while (message.Contains("  "))
                message = message.Replace("  ", " ");

            foreach (String prefix in prefixList)
                message = prefix + message;

            writer.WriteLine(message);
        }

        public static void AddPrefix(String prefix)
        {
            prefixList.Push(prefix);
        }

        public static void RemovePrefix()
        {
            prefixList.Pop();
        }

        public static void CloseLog()
        {
            writer.Close();
        }
    }
}
