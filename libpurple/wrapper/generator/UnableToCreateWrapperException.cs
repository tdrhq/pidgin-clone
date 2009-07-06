using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Scripts
{
    class UnableToCreateWrapperException : Exception
    {
        public UnableToCreateWrapperException(String message)
            : base(message)
        {
        }
    }
}
