using System.Runtime.InteropServices;

namespace QuickBms
{
    public static partial class G { }

    public static partial class NativeMethods
    {
        #region proxy.c
        [DllImport(LibraryName)] [return: MarshalAs(UnmanagedType.LPStr)] public static extern string proxy_ver();
        #endregion
    }
}