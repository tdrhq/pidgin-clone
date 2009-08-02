using System;

namespace PurpleWrapper
{
    public abstract class UnmanagedWrapper<T>
    {
        private T data;
        private IntPtr reference;

        public Type GetUnmanagedType() { return typeof(T); }

        public IntPtr Reference
        {
            get { return reference; }
            set { reference = value; }
        }

        public T Data
        {
            get { return data; }
            set { data = value; }
        }
    }
}

