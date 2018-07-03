using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LogViewer
{
    public enum DebugLevel : int
    {
        Debug,
        Info,
        Warning,
        Error,
        Exception
    }

    public class LogItem
    {
        public DateTime   Time;
        public int ObjectInstance;
        public DebugLevel DebugLevel;
        public string Text;
    }
}
