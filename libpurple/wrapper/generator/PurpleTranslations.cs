using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Scripts
{
    class PurpleTranslations
    {
        public static string GetListElementType(string functionName, int argumentNumber)
        {
            if (functionName == "purple_account_set_status_types" && argumentNumber == 2)
                return "string";
            else if (functionName == "purple_account_get_status_types" && argumentNumber == 0)
                return "string";
            else
            {
                Console.WriteLine("Need: " + functionName);
                return null;
            }

        }
    }
}
