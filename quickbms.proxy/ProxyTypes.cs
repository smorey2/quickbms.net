using System;
using System.Runtime.InteropServices;

namespace QuickBms
{
    public static partial class G { }

    public static partial class NativeMethods
    {
        #region proxy.c
        [DllImport(LibraryName)] public static extern int proxy_test(int a);
        [DllImport(LibraryName)] public static extern IntPtr proxy_ver(); // [return: MarshalAs(UnmanagedType.LPStr)]
        [DllImport(LibraryName)] public static extern int proxy_script(); //[MarshalAs(UnmanagedType.LPStr)] string text);
        #endregion
    }
}